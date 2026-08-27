#include "BXStateComponent.h"

#include "BXGameplayTags.h"
#include "BXEventStructs.h"
#include "BXEventManager.h"
#include "BXFunctionLibrary.h"
#include "BXConditionManager.h"
#include "Behavior/BXBehaviorComponent.h"
#include "Skill/BXSkillComponent.h"
#include "Skill/BXSkillAsset.h"
#include "Timeline/BXTLComponent.h"
#include "Timeline/BXTLAsset.h"
#include "Animation/BXAnimInstance.h"

#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimMontage.h"

DEFINE_LOG_CATEGORY(BXSTATE);

constexpr int32 BX_STATE_ENTER_CHAIN_MAX = 8;



UBXStateComponent::UBXStateComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UBXStateComponent::BeginPlay()
{
	// 创建状态机实例并建立状态→实例映射
	for (const TObjectPtr<UBXStateMachineAsset>& Asset : StateMachineAssets)
	{
		if (!IsValid(Asset))
		{
			continue;
		}

		UBXStateMachineInstance* Instance = NewObject<UBXStateMachineInstance>(this);
		Instance->Initialize(Asset);
		StateMachineInstances.Add(Instance);

		FGameplayTagContainer StateTags;
		Asset->CollectStateTags(StateTags);
		for (int32 i = 0; i < StateTags.Num(); ++i)
		{
			// 重复状态Tag:后注册资产静默覆盖映射(路由错乱根因),显式告警
			if (StateToMachineMap.Contains(StateTags.GetByIndex(i)))
			{
				UE_LOG(BXSTATE, Warning, TEXT("UBXStateComponent::BeginPlay: duplicate state tag %s across state machine assets, later one wins."), *StateTags.GetByIndex(i).ToString());
			}

			StateToMachineMap.Add(StateTags.GetByIndex(i), Instance);
		}
	}

	// 定义状态事件参数类型
	if (UBXEventManager* EventMgr = UBXEventManager::Get(this))
	{
		EventMgr->DefineEvent(BXGameplayTags::BXEvent_State_Enter, FBXEventStateChanged::StaticStruct());
		EventMgr->DefineEvent(BXGameplayTags::BXEvent_State_Exit, FBXEventStateChanged::StaticStruct());
	}

	Super::BeginPlay();
}

void UBXStateComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// 清空全部活跃状态(直摘表广播Exit,禁用解除交由行为组件EndPlay清空遮蔽表)
	{
		TArray<FGameplayTag> ActiveTags;
		CollectActiveStateTags(ActiveTags);
		for (const FGameplayTag& Tag : ActiveTags)
		{
			FBXStateRuntimeData Data;
			if (ActiveStates.RemoveAndCopyValue(Tag, Data))
			{
				for (const FBXStateSource& Source : Data.Sources)
				{
					BroadcastStateEvent(false, Tag, Source.Sign, -1.0f, EBXStateEndReason::SER_Cleared);
				}
			}
		}
		ActiveStates.Empty();
	}

	for (const TObjectPtr<UBXStateMachineInstance>& Instance : StateMachineInstances)
	{
		if (IsValid(Instance))
		{
			Instance->Deinitialize();
			Instance->MarkAsGarbage();
		}
	}
	StateMachineInstances.Empty();
	StateToMachineMap.Empty();

	Super::EndPlay(EndPlayReason);
}

void UBXStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 到期评估链中的同步增删依赖快照(ExpiredTags/ExpiredSigns),回调中的Enter直接执行(表操作走快照安全)
	UpdateExpiredStates(DeltaTime);
	UpdateStateMachines(DeltaTime);
}




#pragma region API
bool UBXStateComponent::CheckStateActive(const FGameplayTag& InStateTag) const
{
	// 精确命中
	if (ActiveStates.Contains(InStateTag))
	{
		return true;
	}

	// 族Tag语义(与行为侧CheckActiveBehavior对称:查族Tag→任一族成员活跃)
	for (const TPair<FGameplayTag, FBXStateRuntimeData>& Pair : ActiveStates)
	{
		if (Pair.Key.MatchesTag(InStateTag))
		{
			return true;
		}
	}

	return false;
}

void UBXStateComponent::GetActiveStates(FGameplayTagContainer& OutStates) const
{
	OutStates.Reset();

	for (const TPair<FGameplayTag, FBXStateRuntimeData>& Pair : ActiveStates)
	{
		OutStates.AddTag(Pair.Key);
	}
}

float UBXStateComponent::GetStateRemainingSeconds(const FGameplayTag& InStateTag) const
{
	if (const FBXStateRuntimeData* Data = ActiveStates.Find(InStateTag))
	{
		return Data->GetRemainingTime();
	}

	return -1.0f;
}

bool UBXStateComponent::IsBehaviorForbiddenByState(const FGameplayTag& InStateTag, const FGameplayTag& InBehaviorTag) const
{
	// 状态不活跃→不禁用
	if (!CheckStateActive(InStateTag))
	{
		return false;
	}

	// 族内节点查状态机资产(TObjectPtr值表:Find返回TObjectPtr指针,不可按裸指针二级指针接收)
	if (const TObjectPtr<UBXStateMachineInstance>* FindResult = StateToMachineMap.Find(InStateTag))
	{
		const UBXStateMachineInstance* Instance = *FindResult;
		if (Instance && Instance->Asset)
		{
			if (const UBXSMStateNode* Node = Instance->Asset->FindStateNode(InStateTag))
			{
				return Node->ForbiddenBehaviors.HasTag(InBehaviorTag);
			}
		}
	}

	// 裸状态查配置
	if (const FBXStateConfig* Config = StateConfigs.Find(InStateTag))
	{
		return Config->ForbiddenBehaviors.HasTag(InBehaviorTag);
	}

	return false;
}

bool UBXStateComponent::EnterState(const FGameplayTag& InStateTag, int64 InSign, float InDuration)
{
	// 进入链深度守卫(防EnterStates环)
	if (EnterChainDepth >= BX_STATE_ENTER_CHAIN_MAX)
	{
		UE_LOG(BXSTATE, Warning, TEXT("UBXStateComponent::EnterState: chain depth exceeded. Tag=%s"), *InStateTag.ToString());
		return false;
	}

	++EnterChainDepth;
	bool bResult = InternalEnterState(InStateTag, InSign, InDuration, EBXStateEndReason::SER_External);
	--EnterChainDepth;

	return bResult;
}

bool UBXStateComponent::ExitState(const FGameplayTag& InStateTag, int64 InSign, EBXStateEndReason InReason)
{
	return InternalExitState(InStateTag, InSign, InReason, false);
}

bool UBXStateComponent::ExitStateAllSources(const FGameplayTag& InStateTag, EBXStateEndReason InReason, bool bSuppressPresentation)
{
	return ExitStateAllSourcesInternal(InStateTag, InReason, bSuppressPresentation, false);
}

bool UBXStateComponent::ExitStateAllSourcesInternal(const FGameplayTag& InStateTag, EBXStateEndReason InReason, bool bSuppressPresentation, bool bDeferForbiddenRelease)
{
	const FBXStateRuntimeData* FindResult = ActiveStates.Find(InStateTag);
	if (!FindResult)
	{
		return false;
	}

	// 返回值语义:条目存在即受理(嵌套回调可能提前移除后续来源,逐条失败不改写受理结果,仅记日志)
	TArray<FBXStateSource> Sources = FindResult->Sources;
	for (const FBXStateSource& Source : Sources)
	{
		if (!InternalExitState(InStateTag, Source.Sign, InReason, bSuppressPresentation, bDeferForbiddenRelease))
		{
			UE_LOG(BXSTATE, Verbose, TEXT("UBXStateComponent::ExitStateAllSources: source already removed. Tag=%s Sign=%lld"), *InStateTag.ToString(), Source.Sign);
		}
	}

	return true;
}

bool UBXStateComponent::RefreshStateDuration(const FGameplayTag& InStateTag, int64 InSign, float InDuration)
{
	FBXStateRuntimeData* Data = ActiveStates.Find(InStateTag);
	if (!Data)
	{
		return false;
	}

	for (FBXStateSource& Source : Data->Sources)
	{
		if (Source.Sign == InSign)
		{
			Source.Duration = InDuration;
			Source.ElapsedTime = 0.0f;
			return true;
		}
	}

	return false;
}

#pragma endregion API




#pragma region Internal
bool UBXStateComponent::InternalEnterState(const FGameplayTag& InStateTag, int64 InSign, float InDuration, EBXStateEndReason InExternalReason, bool bSuppressEntryPresentation)
{
	if (!InStateTag.IsValid())
	{
		return false;
	}

	// 解析时长与禁用列表(规则二:外部携带时长优先,≤0用节点/配置默认;禁用列表始终查静态配置)
	float ResolvedDuration = InDuration;
	FGameplayTagContainer ForbiddenBehaviors;
	{
		float DefaultDuration = -1.0f;
		FGameplayTagContainer DefaultForbidden;
		if (GetStateDurationAndForbidden(InStateTag, DefaultDuration, DefaultForbidden))
		{
			if (InDuration <= 0.0f)
			{
				ResolvedDuration = DefaultDuration;
			}
			ForbiddenBehaviors = MoveTemp(DefaultForbidden);
		}
	}

	// 族内单活:外部进入顶掉当前节点(规则一)
	UBXStateMachineInstance* Machine = FindMachineByStateTag(InStateTag);
	if (Machine)
	{
		UBXSMStateNode* TargetNode = Machine->Asset ? Machine->Asset->FindStateNode(InStateTag) : nullptr;
		if (!TargetNode)
		{
			UE_LOG(BXSTATE, Warning, TEXT("UBXStateComponent::InternalEnterState: node not found. Tag=%s"), *InStateTag.ToString());
			return false;
		}

		if (Machine->CurrentNode && Machine->CurrentNode->StateTag != InStateTag)
		{
			// 顶掉当前节点状态(SER_External)
			// 缓存旧Tag:首个来源退出时InternalExitState会置空CurrentNode,循环内重复解引用会崩溃
			const FGameplayTag OldStateTag = Machine->CurrentNode->StateTag;
			TArray<FBXStateSource> Sources;
			if (const FBXStateRuntimeData* CurrentData = ActiveStates.Find(OldStateTag))
			{
				Sources = CurrentData->Sources;
			}
			for (const FBXStateSource& Source : Sources)
			{
				InternalExitState(OldStateTag, Source.Sign, InExternalReason, false);
			}
		}

		Machine->CurrentNode = TargetNode;
	}

	// 表更新(新Sign追加/同Sign刷新)
	// 条目是否新建先判后建(FindOrAdd后Contains恒真):同Sign刷新时长不重复门控/表现/广播
	const bool bNewEntry = !ActiveStates.Contains(InStateTag);
	FBXStateRuntimeData& Data = ActiveStates.FindOrAdd(InStateTag);
	Data.Tag = InStateTag;
	if (FBXStateSource* ExistingSource = Data.Sources.FindByPredicate([InSign](const FBXStateSource& Source) { return Source.Sign == InSign; }))
	{
		ExistingSource->Duration = ResolvedDuration;
		ExistingSource->ElapsedTime = 0.0f;
	}
	else
	{
		Data.Sources.Add(FBXStateSource(InSign, ResolvedDuration));
	}

	// 条目从无到有才执行门控/表现/事件
	if (bNewEntry)
	{
		ApplyForbiddenBehaviors(ForbiddenBehaviors, InStateTag);

		// 转移路径抑制内置Entry表现:由ExecuteTransition按边表现/目标Entry统一触发,避免双触发
		if (!bSuppressEntryPresentation)
		{
			if (Machine && Machine->CurrentNode)
			{
				TriggerPresentation(Machine->CurrentNode->EntryPresentation, true, InStateTag);
			}
			else if (const FBXStateConfig* Config = StateConfigs.Find(InStateTag))
			{
				TriggerPresentation(Config->EntryPresentation, true, InStateTag);
			}
		}

		BroadcastStateEvent(true, InStateTag, InSign, ResolvedDuration, EBXStateEndReason::SER_TMax);
	}

	return true;
}

bool UBXStateComponent::InternalExitState(const FGameplayTag& InStateTag, int64 InSign, EBXStateEndReason InReason, bool bSuppressPresentation, bool bDeferForbiddenRelease)
{
	FBXStateRuntimeData* Data = ActiveStates.Find(InStateTag);
	if (!Data)
	{
		return false;
	}

	// 查无该Sign来源
	if (!Data->HasSource(InSign))
	{
		return false;
	}

	// 移除来源(先到先退)
	Data->Sources.RemoveAll([InSign](const FBXStateSource& Source) { return Source.Sign == InSign; });

	// 仍有其他来源:状态继续
	if (Data->Sources.Num() > 0)
	{
		return true;
	}

	// 移除条目
	ActiveStates.Remove(InStateTag);

	// 静态配置查询(表现与禁用解除共用)
	FGameplayTagContainer ForbiddenBehaviors;
	const FBXStatePresentation* ExitPresentation = nullptr;
	UBXStateMachineInstance* Machine = FindMachineByStateTag(InStateTag);
	if (Machine)
	{
		if (Machine->Asset)
		{
			if (const UBXSMStateNode* Node = Machine->Asset->FindStateNode(InStateTag))
			{
				ForbiddenBehaviors = Node->ForbiddenBehaviors;
				ExitPresentation = &Node->ExitPresentation;
			}
		}

		// 状态机当前节点置空(SM空转)
		if (Machine->CurrentNode && Machine->CurrentNode->StateTag == InStateTag)
		{
			Machine->CurrentNode = nullptr;
		}
	}
	else if (const FBXStateConfig* Config = StateConfigs.Find(InStateTag))
	{
		ForbiddenBehaviors = Config->ForbiddenBehaviors;
		ExitPresentation = &Config->ExitPresentation;
	}

	// 表现(预测回滚强制不触发)
	if (!bSuppressPresentation && InReason != EBXStateEndReason::SER_PredictRollback && ExitPresentation)
	{
		TriggerPresentation(*ExitPresentation, false, InStateTag);
	}

	// 禁用解除判定→行为恢复(表现抑制时门控仍需解除;转移路径延迟到新状态登记后由调用方统一解除)
	if (!bDeferForbiddenRelease)
	{
		ReleaseForbiddenBehaviors(ForbiddenBehaviors, InStateTag);
	}

	// 广播Exit(最后来源退出)
	BroadcastStateEvent(false, InStateTag, InSign, -1.0f, InReason);

	return true;
}

void UBXStateComponent::UpdateExpiredStates(float InDeltaTime)
{
	// 快照收集(到期处理链中的Enter/Exit会同步修改表;TSet自动去重,替代数组线性Contains的O(n²))
	TSet<FGameplayTag> ExpiredTags;
	for (TPair<FGameplayTag, FBXStateRuntimeData>& Pair : ActiveStates)
	{
		for (FBXStateSource& Source : Pair.Value.Sources)
		{
			Source.ElapsedTime += InDeltaTime;

			// 计时来源到期(≤0无限)
			if (Source.Duration > 0.0f && Source.ElapsedTime >= Source.Duration)
			{
				ExpiredTags.Add(Pair.Key);
			}
		}
	}

	// 族内状态到期→状态机评估转移(不直接移除,由转移目标顶掉或评估失败后自然退出)
	// 裸状态到期→自然退出
	for (const FGameplayTag& Tag : ExpiredTags)
	{
		if (FindMachineByStateTag(Tag))
		{
			// 状态机状态:OnExpired边评估在UpdateStateMachines中处理,此处仅标记
			continue;
		}

		// 裸状态:全部到期来源移除
		if (const FBXStateRuntimeData* Data = ActiveStates.Find(Tag))
		{
			TArray<int64> ExpiredSigns;
			for (const FBXStateSource& Source : Data->Sources)
			{
				if (Source.Duration > 0.0f && Source.ElapsedTime >= Source.Duration)
				{
					ExpiredSigns.Add(Source.Sign);
				}
			}
			for (int64 Sign : ExpiredSigns)
			{
				InternalExitState(Tag, Sign, EBXStateEndReason::SER_Expired, false);
			}
		}
	}
}

void UBXStateComponent::UpdateStateMachines(float InDeltaTime)
{
	for (const TObjectPtr<UBXStateMachineInstance>& Instance : StateMachineInstances)
	{
		if (!IsValid(Instance) || !Instance->Asset)
		{
			continue;
		}

		// 空转回退:曾激活过后节点为空(链尾/被中断收束)→自动进入默认节点(未配置/Tag无效则保持休眠)
		if (!Instance->CurrentNode)
		{
			if (Instance->bActivatedOnce && Instance->DefaultNode && Instance->DefaultNode->StateTag.IsValid())
			{
				const FGameplayTag DefaultTag = Instance->DefaultNode->StateTag;
				if (ActiveStates.Contains(DefaultTag))
				{
					// 默认态仍活跃(外部重新进入过):直接归位
					Instance->CurrentNode = Instance->DefaultNode;
				}
				else
				{
					// 进入默认态(Sign=0状态机自身;经EnterState走链深度守卫:默认态进入链可触发监听者级联进入,失败下帧重试)
					EnterState(DefaultTag, 0, Instance->DefaultNode->Duration);
				}
			}
			continue;
		}

		// 曾激活标记(任何观察到有当前节点的帧置位)
		Instance->bActivatedOnce = true;

		const FGameplayTag CurrentTag = Instance->CurrentNode->StateTag;
		const FBXStateRuntimeData* Data = ActiveStates.Find(CurrentTag);
		if (!Data)
		{
			// 条目已不存在(被外部退出):SM空转
			Instance->CurrentNode = nullptr;
			continue;
		}

		// 到期检查(含无限来源永不到期;DeltaTime步进越界后剩余为负同样视为到期,旧>=0&&<=0判定会漏掉越界帧)
		const bool bExpired = !Data->HasInfiniteSource() && Data->GetRemainingTime() <= 0.0f;

		// 边评估上下文
		FBXSMTransitionContext Context;
		Context.Owner = GetOwner();
		Context.FromTag = CurrentTag;
		if (const UWorld* World = GetWorld())
		{
			// 单机=本地世界时间;P5网络期接入服务器校时
			Context.ServerTimeMs = static_cast<int64>(World->GetTimeSeconds() * 1000.0);
		}

		// 评估当前节点出边(顺序:OutEdges索引序)
		UBXSMTransitionEdge* TriggeredEdge = nullptr;
		for (int32 EdgeIndex : Instance->CurrentNode->OutEdges)
		{
			UBXDecisionTreeEdge* Edge = Instance->Asset->TreeEdges.IsValidIndex(EdgeIndex) ? Instance->Asset->TreeEdges[EdgeIndex] : nullptr;
			UBXSMTransitionEdge* SMEdge = Cast<UBXSMTransitionEdge>(Edge);
			if (!SMEdge)
			{
				continue;
			}

			// 评估时机过滤:OnExpired仅在到期时评估,OnTick每帧评估
			if (SMEdge->EvaluateType == EBXTransitionEvaluate::TE_OnExpired && !bExpired)
			{
				continue;
			}

			// 条件评估(空条件=无条件通过)
			bool bConditionPassed = true;
			if (IsValid(SMEdge->Condition))
			{
				UBXConditionManager* ConditionManager = UBXConditionManager::Get(this);
				bConditionPassed = ConditionManager
					? ConditionManager->CheckCondition(SMEdge->Condition, FBXSMTransitionContext::StaticStruct(), &Context)
					: false;
			}

			if (bConditionPassed)
			{
				TriggeredEdge = SMEdge;
				break;
			}
		}

		if (TriggeredEdge)
		{
			UBXSMStateNode* TargetNode = Cast<UBXSMStateNode>(TriggeredEdge->EndNode);
			if (TargetNode && TargetNode->StateTag == CurrentTag)
			{
				// 自环=驻留刷新(重置有限来源计时,无限来源不动;守卫语义:条件成立期间保持驻留)
				// 旧实现直接跳过:到期自环边永不离场,到期来源永不退出,状态卡死
				if (FBXStateRuntimeData* CurrentData = ActiveStates.Find(CurrentTag))
				{
					for (FBXStateSource& Source : CurrentData->Sources)
					{
						if (Source.Duration > 0.0f)
						{
							Source.ElapsedTime = 0.0f;
						}
					}
				}
			}
			else if (TargetNode)
			{
				// 转移执行(顶掉式退出+进入目标)
				ExecuteTransition(Instance, TargetNode, EBXStateEndReason::SER_Transition);
			}
		}
		else if (bExpired)
		{
			// 到期且无OnExpired边命中→逐来源自然退出(先到先退:仅移除已到期来源,未到期来源维持状态)
			// 旧实现ExitStateAllSources会把未到期来源一并退出,与"来源独立计时"原则矛盾
			if (const FBXStateRuntimeData* ExpiredData = ActiveStates.Find(CurrentTag))
			{
				TArray<int64> ExpiredSigns;
				for (const FBXStateSource& Source : ExpiredData->Sources)
				{
					if (Source.Duration > 0.0f && Source.ElapsedTime >= Source.Duration)
					{
						ExpiredSigns.Add(Source.Sign);
					}
				}

				// 快照Sign逐条退出(退出链回调可能修改表,不可持有条目指针跨调用)
				for (int64 Sign : ExpiredSigns)
				{
					InternalExitState(CurrentTag, Sign, EBXStateEndReason::SER_Expired, false);
				}
			}

			// 最后来源已退出(条目移除)→SM空转(默认节点回退由上方空转分支处理)
			if (!ActiveStates.Contains(CurrentTag))
			{
				Instance->CurrentNode = nullptr;
			}
		}
	}
}

bool UBXStateComponent::ExecuteTransition(UBXStateMachineInstance* InMachine, UBXSMStateNode* InTargetNode, EBXStateEndReason InReason)
{
	if (!InMachine || !InTargetNode || !InMachine->CurrentNode)
	{
		return false;
	}

	// 边表现查找(转移执行前查:退出/进入可能触发回调修改节点出边引用)
	UBXSMTransitionEdge* TransitionEdge = nullptr;
	for (int32 EdgeIndex : InMachine->CurrentNode->OutEdges)
	{
		if (InMachine->Asset->TreeEdges.IsValidIndex(EdgeIndex))
		{
			if (UBXSMTransitionEdge* SMEdge = Cast<UBXSMTransitionEdge>(InMachine->Asset->TreeEdges[EdgeIndex]))
			{
				if (SMEdge->EndNode == InTargetNode)
				{
					TransitionEdge = SMEdge;
					break;
				}
			}
		}
	}

	// 旧状态禁用集合快照(退出前收集:延迟解除用)
	const FGameplayTag CurrentTag = InMachine->CurrentNode->StateTag;
	const FGameplayTagContainer OldForbidden = InMachine->CurrentNode->ForbiddenBehaviors;

	// 退出当前(全部来源;抑制Exit表现;禁用解除延迟到新状态登记后:共享禁用Tag经遮蔽多重登记保持挂起,独占Tag解除恢复,消除Resume→Suspend抖动)
	ExitStateAllSourcesInternal(CurrentTag, InReason, true, true);

	// 进入目标(节点默认时长,Sign=0状态机自身;抑制内置Entry表现,下方边表现统一触发)
	bool bResult = InternalEnterState(InTargetNode->StateTag, 0, InTargetNode->Duration, InReason, true);

	// 解除旧状态禁用登记(新状态已登记:共享Tag的遮蔽仍持新状态登记不会被解除,行为不抖动;进入失败同样解除,禁用不残留)
	ReleaseForbiddenBehaviors(OldForbidden, CurrentTag);

	// 转移表现(边覆盖优先,空用目标节点EntryPresentation)
	if (bResult)
	{
		const FBXStatePresentation& Presentation = (TransitionEdge && TransitionEdge->TransitionPresentation.IsValid())
			? TransitionEdge->TransitionPresentation
			: InTargetNode->EntryPresentation;
		TriggerPresentation(Presentation, true, InTargetNode->StateTag);
	}

	return bResult;
}

void UBXStateComponent::TriggerPresentation(const FBXStatePresentation& InPresentation, bool bIsEntry, const FGameplayTag& InStateTag)
{
	if (!GetOwner())
	{
		return;
	}

	// 配置诊断(通道已选但对应资产未配置:IsValid按通道校验资产路径,静默跳过改为显式告警)
	if (InPresentation.Type != EBXPresentationType::PT_None && !InPresentation.IsValid())
	{
		UE_LOG(BXSTATE, Warning, TEXT("UBXStateComponent::TriggerPresentation: presentation type %d set but asset unset. State=%s"), (int32)InPresentation.Type, *InStateTag.ToString());
		return;
	}

	if (!InPresentation.IsValid())
	{
		return;
	}

	switch (InPresentation.Type)
	{
	case EBXPresentationType::PT_Skill:
	{
		// 技能通道(自带同步体系)
		if (UBXSkillComponent* SkillComponent = GetOwner()->FindComponentByClass<UBXSkillComponent>())
		{
			if (UBXSkillAsset* SkillAsset = InPresentation.SkillAsset.LoadSynchronous())
			{
				SkillComponent->PlaySkill(SkillAsset);
			}
			else
			{
				UE_LOG(BXSTATE, Warning, TEXT("UBXStateComponent::TriggerPresentation: skill asset load failed. State=%s"), *InStateTag.ToString());
			}
		}
		break;
	}

	case EBXPresentationType::PT_Timeline:
	{
		// 时间轴通道(不同步的技能,纯表现)
		if (UBXTLComponent* TLComponent = GetOwner()->FindComponentByClass<UBXTLComponent>())
		{
			if (UBXTLAsset* TLAsset = InPresentation.TimelineAsset.LoadSynchronous())
			{
				FBXTLPlayContext Context;
				TLComponent->PlayTimeline(TLAsset, Context);
			}
			else
			{
				UE_LOG(BXSTATE, Warning, TEXT("UBXStateComponent::TriggerPresentation: timeline asset load failed. State=%s"), *InStateTag.ToString());
			}
		}
		break;
	}

	case EBXPresentationType::PT_Animation:
	{
		// 纯动画通道(倒地循环/起身等)
		if (ACharacter* Character = Cast<ACharacter>(GetOwner()))
		{
			if (USkeletalMeshComponent* MeshComponent = Character->GetMesh())
			{
				if (UBXAnimInstance* AnimInstance = Cast<UBXAnimInstance>(MeshComponent->GetAnimInstance()))
				{
					if (UAnimMontage* Montage = InPresentation.MontageAsset.LoadSynchronous())
					{
						AnimInstance->Montage_Play(Montage);
					}
					else
					{
						UE_LOG(BXSTATE, Warning, TEXT("UBXStateComponent::TriggerPresentation: montage asset load failed. State=%s"), *InStateTag.ToString());
					}
				}
			}
		}
		break;
	}

	default:
		break;
	}
}

void UBXStateComponent::ApplyForbiddenBehaviors(const FGameplayTagContainer& InForbiddenBehaviors, const FGameplayTag& InByState)
{
	if (InForbiddenBehaviors.IsEmpty())
	{
		return;
	}

	if (UBXBehaviorComponent* BehaviorComp = GetOwner()->FindComponentByClass<UBXBehaviorComponent>())
	{
		for (int32 i = 0; i < InForbiddenBehaviors.Num(); ++i)
		{
			BehaviorComp->SuspendByForbiddenTag(InForbiddenBehaviors.GetByIndex(i), InByState);
		}
	}
}

void UBXStateComponent::ReleaseForbiddenBehaviors(const FGameplayTagContainer& InForbiddenBehaviors, const FGameplayTag& InByState)
{
	if (InForbiddenBehaviors.IsEmpty())
	{
		return;
	}

	// 重入保护:解除窗口内状态被重新进入时登记仍有效,跳过解除
	// 场景1(延迟解除):ExecuteTransition的Exit/Enter事件监听者同步EnterState重入旧状态(SuspendByForbiddenTag幂等追加ByStates),随后统一解除会误清活跃状态登记
	// 场景2(即时解除):InternalExitState的Exit表现触发技能EnterStates重入自身,表现后解除会误清新登记
	if (ActiveStates.Contains(InByState))
	{
		return;
	}

	if (UBXBehaviorComponent* BehaviorComp = GetOwner()->FindComponentByClass<UBXBehaviorComponent>())
	{
		for (int32 i = 0; i < InForbiddenBehaviors.Num(); ++i)
		{
			// 遮蔽表按状态登记解除(最后一个禁用状态退出才恢复行为)
			BehaviorComp->ResumeByForbiddenTag(InForbiddenBehaviors.GetByIndex(i), InByState);
		}
	}
}

void UBXStateComponent::CollectActiveStateTags(TArray<FGameplayTag>& OutTags) const
{
	OutTags.Reset(ActiveStates.Num());
	ActiveStates.GetKeys(OutTags);
}

void UBXStateComponent::BroadcastStateEvent(bool bEnter, const FGameplayTag& InStateTag, int64 InSign, float InDuration, EBXStateEndReason InReason)
{
	UBXEventManager* EventMgr = UBXEventManager::Get(this);
	if (!IsValid(EventMgr))
	{
		return;
	}

	FBXEventStateChanged Parameter(InStateTag, InSign, InDuration, InReason);
	if (bEnter)
	{
		EventMgr->BroadcastSingleEvent<FBXEventStateChanged>(BXGameplayTags::BXEvent_State_Enter, GetOwner(), Parameter);
		EventMgr->BroadcastGlobalEvent<FBXEventStateChanged>(BXGameplayTags::BXEvent_State_Enter, Parameter);
	}
	else
	{
		EventMgr->BroadcastSingleEvent<FBXEventStateChanged>(BXGameplayTags::BXEvent_State_Exit, GetOwner(), Parameter);
		EventMgr->BroadcastGlobalEvent<FBXEventStateChanged>(BXGameplayTags::BXEvent_State_Exit, Parameter);
	}
}

UBXStateMachineInstance* UBXStateComponent::FindMachineByStateTag(const FGameplayTag& InStateTag) const
{
	if (const TObjectPtr<UBXStateMachineInstance>* FindResult = StateToMachineMap.Find(InStateTag))
	{
		return *FindResult;
	}

	return nullptr;
}

bool UBXStateComponent::GetStateDurationAndForbidden(const FGameplayTag& InStateTag, float& OutDuration, FGameplayTagContainer& OutForbidden) const
{
	// 族内节点(TObjectPtr值表:Find返回TObjectPtr指针,不可按裸指针二级指针接收)
	if (const TObjectPtr<UBXStateMachineInstance>* FindResult = StateToMachineMap.Find(InStateTag))
	{
		const UBXStateMachineInstance* Instance = *FindResult;
		if (Instance && Instance->Asset)
		{
			if (const UBXSMStateNode* Node = Instance->Asset->FindStateNode(InStateTag))
			{
				OutDuration = Node->Duration;
				OutForbidden = Node->ForbiddenBehaviors;
				return true;
			}
		}
	}

	// 裸状态配置
	if (const FBXStateConfig* Config = StateConfigs.Find(InStateTag))
	{
		OutDuration = Config->Duration;
		OutForbidden = Config->ForbiddenBehaviors;
		return true;
	}

	return false;
}

#pragma endregion Internal
