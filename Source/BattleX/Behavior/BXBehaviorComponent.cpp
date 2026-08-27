#include "BXBehaviorComponent.h"

#include "BXGameplayTags.h"
#include "BXEventStructs.h"
#include "BXEventManager.h"
#include "BXBehaviorSettings.h"

#include "State/BXStateComponent.h"

DEFINE_LOG_CATEGORY(BXBEHAVIOR);

constexpr int32 BX_BEHAVIOR_ENTER_CHAIN_MAX = 8;



UBXBehaviorComponent::UBXBehaviorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UBXBehaviorComponent::BeginPlay()
{
	// 初始化行为代理实例
	for (const TPair<FGameplayTag, TSubclassOf<UBXBehaviorAgent>>& Pair : BehaviorAgentConfigs)
	{
		if (!IsValid(Pair.Value))
		{
			continue;
		}

		UBXBehaviorAgent* NewAgent = NewObject<UBXBehaviorAgent>(this, Pair.Value);
		if (!IsValid(NewAgent))
		{
			continue;
		}

		NewAgent->Initialize();
		BehaviorAgents.Add(Pair.Key, NewAgent);
	}

	// 定义行为事件参数类型
	if (UBXEventManager* EventMgr = UBXEventManager::Get(this))
	{
		EventMgr->DefineEvent(BXGameplayTags::BXEvent_Behavior_Enter, FBXEventBehaviorChanged::StaticStruct());
		EventMgr->DefineEvent(BXGameplayTags::BXEvent_Behavior_Exit, FBXEventBehaviorChanged::StaticStruct());
	}

	Super::BeginPlay();
}

void UBXBehaviorComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// 清空全部活跃行为(条目移除走统一管线,全来源逐Sign退出)
	{
		TArray<FGameplayTag> ActiveTags;
		CollectActiveBehaviorTags(ActiveTags);
		for (const FGameplayTag& Tag : ActiveTags)
		{
			StopBehaviorAllSources(Tag, EBXBehaviorEndReason::BER_Cleared);
		}
		ActiveBehaviors.Empty();
	}

	// 释放代理
	for (const TPair<FGameplayTag, TObjectPtr<UBXBehaviorAgent>>& Pair : BehaviorAgents)
	{
		if (!IsValid(Pair.Value))
		{
			continue;
		}

		Pair.Value->Deinitialize();
		Pair.Value->MarkAsGarbage();
	}
	BehaviorAgents.Empty();

	SuspendMasks.Empty();
	ProtectionEntries.Empty();

	Super::EndPlay(EndPlayReason);
}




#pragma region API
bool UBXBehaviorComponent::CheckActiveBehavior(const FGameplayTag& InBehaviorTag) const
{
	// 事实表查询:族Tag精确匹配条目键(行为Tag即事实键,族查询走GetActiveBehaviors)
	if (ActiveBehaviors.Contains(InBehaviorTag))
	{
		return true;
	}

	// 族Tag语义(BXBehavior.Locomotion → 查询任一移动族行为活跃)
	for (const TPair<FGameplayTag, FBXBehaviorRuntimeData>& Pair : ActiveBehaviors)
	{
		if (Pair.Key.MatchesTag(InBehaviorTag))
		{
			return true;
		}
	}

	return false;
}

void UBXBehaviorComponent::GetActiveBehaviors(FGameplayTagContainer& OutBehaviors) const
{
	OutBehaviors.Reset();

	for (const TPair<FGameplayTag, FBXBehaviorRuntimeData>& Pair : ActiveBehaviors)
	{
		OutBehaviors.AddTag(Pair.Key);
	}
}

bool UBXBehaviorComponent::CanStartBehavior(const FGameplayTag& InBehaviorTag, FBXBehaviorStartCheck& OutCheck) const
{
	// 无参入口(出招表预检/技能五步链):Agent检查用空参数
	FInstancedStruct EmptyParam;
	return CanStartBehaviorInternal(InBehaviorTag, EmptyParam, OutCheck);
}

bool UBXBehaviorComponent::StartBehavior(const FGameplayTag& InBehaviorTag, int64 InSign)
{
	FInstancedStruct IS;
	return StartBehaviorWithParameter(InBehaviorTag, MoveTemp(IS), InSign);
}

bool UBXBehaviorComponent::StartBehaviorWithParameter(const FGameplayTag& InBehaviorTag, FInstancedStruct InParameter, int64 InSign)
{
	// 激活链深度守卫(防进入链环)
	if (EnterChainDepth >= BX_BEHAVIOR_ENTER_CHAIN_MAX)
	{
		UE_LOG(BXBEHAVIOR, Warning, TEXT("UBXBehaviorComponent::StartBehavior: Enter chain depth exceeded. Tag=%s"), *InBehaviorTag.ToString());
		return false;
	}

	// 带真实参数判定(参数依赖型Agent:如Move带方向查地形,空参判定会失真;判定在移动前使用参数)
	FBXBehaviorStartCheck Check;
	if (!CanStartBehaviorInternal(InBehaviorTag, InParameter, Check))
	{
		UE_LOG(BXBEHAVIOR, Warning, TEXT("UBXBehaviorComponent::StartBehavior: rejected(%s). Tag=%s Sign=%lld"), *Check.FailReason, *InBehaviorTag.ToString(), InSign);
		return false;
	}

	// 链深度守卫配对(Enter事件监听者同步Start的行为链,深度超限拒绝)
	++EnterChainDepth;
	const bool bResult = InternalStartBehavior(InBehaviorTag, MoveTemp(InParameter), InSign);
	--EnterChainDepth;

	return bResult;
}

bool UBXBehaviorComponent::StopBehavior(const FGameplayTag& InBehaviorTag, int64 InSign)
{
	FInstancedStruct IS;
	return StopBehaviorWithParameter(InBehaviorTag, IS, InSign);
}

bool UBXBehaviorComponent::StopBehaviorWithParameter(const FGameplayTag& InBehaviorTag, const FInstancedStruct& InParameter, int64 InSign)
{
	return InternalStopBehavior(InBehaviorTag, InParameter, InSign, EBXBehaviorEndReason::BER_Manual);
}

bool UBXBehaviorComponent::StopBehaviorAllSources(const FGameplayTag& InBehaviorTag, EBXBehaviorEndReason InReason)
{
	const FBXBehaviorRuntimeData* FindResult = ActiveBehaviors.Find(InBehaviorTag);
	if (!FindResult)
	{
		return false;
	}

	// 返回值语义:条目存在即受理(嵌套回调可能提前移除后续来源,逐条失败不改写受理结果,仅记日志)
	TArray<FBXBehaviorSource> Sources = FindResult->Sources;
	for (const FBXBehaviorSource& Source : Sources)
	{
		FInstancedStruct IS;
		if (!InternalStopBehavior(InBehaviorTag, IS, Source.Sign, InReason))
		{
			UE_LOG(BXBEHAVIOR, Verbose, TEXT("UBXBehaviorComponent::StopBehaviorAllSources: source already removed. Tag=%s Sign=%lld"), *InBehaviorTag.ToString(), Source.Sign);
		}
	}

	return true;
}

bool UBXBehaviorComponent::InterruptBehaviorsConflicting(const FGameplayTag& InBehaviorTag)
{
	const UBXBehaviorSettings* Settings = GetDefault<UBXBehaviorSettings>();
	if (!Settings)
	{
		return true;
	}

	// 收集挤出目标快照(逐个停止可能触发回调修改表)
	TArray<FGameplayTag> ExpelTargets;
	Settings->GetExpelTargets(InBehaviorTag, ExpelTargets);

	// 返回值语义:仅由保护目标决定(技能五步链据此中止释放;来源停止失败为嵌套回调竞态,不构成清场失败)
	bool bResult = true;
	for (const FGameplayTag& TargetTag : ExpelTargets)
	{
		// 族匹配收集(列Tag可为族Tag,活跃条目为其子Tag;先收集键快照,停止链回调增删表安全)
		TArray<FGameplayTag> MatchedTags;
		for (const TPair<FGameplayTag, FBXBehaviorRuntimeData>& Pair : ActiveBehaviors)
		{
			if (Pair.Key.MatchesTag(TargetTag))
			{
				MatchedTags.Add(Pair.Key);
			}
		}

		for (const FGameplayTag& MatchedTag : MatchedTags)
		{
			// 受保护目标跳过(调用方CanStart已查,幂等防御)
			if (IsBehaviorProtected(MatchedTag))
			{
				UE_LOG(BXBEHAVIOR, Warning, TEXT("UBXBehaviorComponent::InterruptBehaviorsConflicting: protected target %s"), *MatchedTag.ToString());
				bResult = false;
				continue;
			}

			const FBXBehaviorRuntimeData* FindResult = ActiveBehaviors.Find(MatchedTag);
			if (!FindResult)
			{
				continue;
			}

			TArray<FBXBehaviorSource> Sources = FindResult->Sources;
			for (const FBXBehaviorSource& Source : Sources)
			{
				FInstancedStruct IS;
				if (!InternalStopBehavior(MatchedTag, IS, Source.Sign, EBXBehaviorEndReason::BER_Expelled))
				{
					UE_LOG(BXBEHAVIOR, Verbose, TEXT("UBXBehaviorComponent::InterruptBehaviorsConflicting: source already removed. Tag=%s Sign=%lld"), *MatchedTag.ToString(), Source.Sign);
				}
			}
		}
	}

	return bResult;
}

bool UBXBehaviorComponent::IsBehaviorProtected(const FGameplayTag& InBehaviorTag) const
{
	if (const TArray<FBXProtectionRecord>* Records = ProtectionEntries.Find(InBehaviorTag))
	{
		for (const FBXProtectionRecord& Record : *Records)
		{
			if (Record.bProtected)
			{
				return true;
			}
		}
	}

	return false;
}

void UBXBehaviorComponent::SetBehaviorProtection(const FGameplayTag& InBehaviorTag, int64 InSign, bool bProtected)
{
	TArray<FBXProtectionRecord>* Records = ProtectionEntries.Find(InBehaviorTag);
	if (!Records)
	{
		if (!bProtected)
		{
			return;
		}

		ProtectionEntries.Add(InBehaviorTag, { FBXProtectionRecord(InSign, bProtected) });
		return;
	}

	// 同来源刷新
	for (FBXProtectionRecord& Record : *Records)
	{
		if (Record.Sign == InSign)
		{
			Record.bProtected = bProtected;
			return;
		}
	}

	if (bProtected)
	{
		Records->Add(FBXProtectionRecord(InSign, true));
	}
}

void UBXBehaviorComponent::RemoveProtectionBySign(int64 InSign)
{
	for (TPair<FGameplayTag, TArray<FBXProtectionRecord>>& Pair : ProtectionEntries)
	{
		Pair.Value.RemoveAll([InSign](const FBXProtectionRecord& Record) { return Record.Sign == InSign; });
	}

	// 清空键
	TArray<FGameplayTag> EmptyKeys;
	for (const TPair<FGameplayTag, TArray<FBXProtectionRecord>>& Pair : ProtectionEntries)
	{
		if (Pair.Value.IsEmpty())
		{
			EmptyKeys.Add(Pair.Key);
		}
	}
	for (const FGameplayTag& Key : EmptyKeys)
	{
		ProtectionEntries.Remove(Key);
	}
}

void UBXBehaviorComponent::SuspendByForbiddenTag(const FGameplayTag& InForbiddenTag, const FGameplayTag& InByState)
{
	if (!InForbiddenTag.IsValid())
	{
		return;
	}

	// 遮蔽键已存在(其他状态同样禁用此Tag):仅追加状态,遮蔽已生效无需重复处理行为
	if (FBXSuspendMask* ExistingMask = SuspendMasks.Find(InForbiddenTag))
	{
		ExistingMask->ByStates.Add(InByState);
		return;
	}

	// 新遮蔽生效:收集从"未被遮蔽"变"被遮蔽"的活跃行为(已被其他遮蔽键覆盖的不重复处理)
	// 行为条目不移表:事实保留,Agent停转+查询遮蔽
	TArray<FGameplayTag> NewlySuspended;
	for (const TPair<FGameplayTag, FBXBehaviorRuntimeData>& Pair : ActiveBehaviors)
	{
		if (Pair.Key.MatchesTag(InForbiddenTag) && !IsBehaviorSuspended(Pair.Key))
		{
			NewlySuspended.Add(Pair.Key);
		}
	}

	FBXSuspendMask NewMask;
	NewMask.ByStates.Add(InByState);
	SuspendMasks.Add(InForbiddenTag, MoveTemp(NewMask));

	for (const FGameplayTag& Tag : NewlySuspended)
	{
		const FBXBehaviorRuntimeData* BehaviorData = ActiveBehaviors.Find(Tag);
		if (!BehaviorData)
		{
			continue;
		}

		// 快照来源(Agent停止/事件回调可能同步移除来源或条目,指针不可跨回调解引用)
		const TArray<FBXBehaviorSource> Sources = BehaviorData->Sources;

		// Agent停止
		if (const TObjectPtr<UBXBehaviorAgent>* FindResult = BehaviorAgents.Find(Tag))
		{
			UBXBehaviorAgent* Agent = *FindResult;
			if (IsValid(Agent))
			{
				FInstancedStruct IS;
				Agent->StopBehavior(IS);
			}
		}

		// Agent停止后复查遮蔽仍生效:蓝图Agent可在Stop回调中同步退出禁用状态(Resume解除遮蔽),
		// 此时广播Suspended会误触发技能互锁,跳过
		if (!IsBehaviorSuspended(Tag))
		{
			continue;
		}

		// 每个来源广播Exit(技能互锁监听点)
		for (const FBXBehaviorSource& Source : Sources)
		{
			BroadcastBehaviorEvent(false, Tag, Source.Sign, EBXBehaviorEndReason::BER_Suspended);
		}
	}
}

void UBXBehaviorComponent::ResumeByForbiddenTag(const FGameplayTag& InForbiddenTag, const FGameplayTag& InByState)
{
	FBXSuspendMask Mask;
	if (!SuspendMasks.RemoveAndCopyValue(InForbiddenTag, Mask))
	{
		return;
	}

	// 移除退出状态的登记
	Mask.ByStates.Remove(InByState);

	// 仍有其他状态禁用此Tag:遮蔽保持(回写表)
	if (!Mask.ByStates.IsEmpty())
	{
		SuspendMasks.Add(InForbiddenTag, MoveTemp(Mask));
		return;
	}

	// 遮蔽解除:恢复匹配此Tag的活跃行为(条目仍在事实表中)
	// 仍被其他活跃遮蔽键覆盖(如状态A禁Locomotion退出,但状态B仍禁Move)的保持挂起
	TArray<FGameplayTag> MatchedTags;
	for (const TPair<FGameplayTag, FBXBehaviorRuntimeData>& Pair : ActiveBehaviors)
	{
		if (Pair.Key.MatchesTag(InForbiddenTag))
		{
			MatchedTags.Add(Pair.Key);
		}
	}

	for (const FGameplayTag& Tag : MatchedTags)
	{
		// 其他遮蔽键仍覆盖(恢复会违反其禁用)
		if (IsBehaviorSuspended(Tag))
		{
			continue;
		}

		const FBXBehaviorRuntimeData* BehaviorData = ActiveBehaviors.Find(Tag);
		if (!BehaviorData)
		{
			continue;
		}

		// 快照参数与来源(Agent重启/事件回调可能同步修改条目,指针不可跨回调解引用)
		const FInstancedStruct ResumeParameter = BehaviorData->LastStartParameter;
		const TArray<FBXBehaviorSource> Sources = BehaviorData->Sources;

		// Agent重启(回放最近启动参数,来源所有权不变,Sign原样)
		if (const TObjectPtr<UBXBehaviorAgent>* FindResult = BehaviorAgents.Find(Tag))
		{
			UBXBehaviorAgent* Agent = *FindResult;
			if (IsValid(Agent))
			{
				Agent->StartBehavior(ResumeParameter);
			}
		}

		// Agent重启后复查未被重新遮蔽:蓝图Agent可在Start回调中进入禁用状态,此时广播Resumed与实际状态不符,跳过
		if (IsBehaviorSuspended(Tag))
		{
			continue;
		}

		for (const FBXBehaviorSource& Source : Sources)
		{
			BroadcastBehaviorEvent(true, Tag, Source.Sign, EBXBehaviorEndReason::BER_Resumed);
		}
	}
}

bool UBXBehaviorComponent::IsBehaviorSuspended(const FGameplayTag& InBehaviorTag) const
{
	// 任一活跃遮蔽键为该Tag的祖先或自身 -> 被挂起(家族Tag遮蔽整族)
	for (const TPair<FGameplayTag, FBXSuspendMask>& Pair : SuspendMasks)
	{
		if (InBehaviorTag.MatchesTag(Pair.Key))
		{
			return true;
		}
	}

	return false;
}

bool UBXBehaviorComponent::CheckForbiddenBehavior(const FGameplayTag& InBehaviorTag) const
{
	// 挂起=被状态禁用
	if (IsBehaviorSuspended(InBehaviorTag))
	{
		return true;
	}

	// 拒绝关系=被活跃行为挡住
	const UBXBehaviorSettings* Settings = GetDefault<UBXBehaviorSettings>();
	if (Settings)
	{
		FGameplayTagContainer ActiveContainer;
		GetActiveBehaviors(ActiveContainer);
		return Settings->IsRejectedByAny(InBehaviorTag, ActiveContainer);
	}

	return false;
}

#pragma endregion API




#pragma region Internal
bool UBXBehaviorComponent::CanStartBehaviorInternal(const FGameplayTag& InBehaviorTag, const FInstancedStruct& InParameter, FBXBehaviorStartCheck& OutCheck) const
{
	OutCheck.bCanStart = true;
	OutCheck.FailReason.Reset();

	if (!InBehaviorTag.IsValid())
	{
		OutCheck.bCanStart = false;
		OutCheck.FailReason = TEXT("InvalidTag");
		return false;
	}

	// 挂起检查(仍被状态禁用:任一活跃遮蔽键覆盖)
	if (IsBehaviorSuspended(InBehaviorTag))
	{
		OutCheck.bCanStart = false;
		OutCheck.FailReason = TEXT("Suspended");
		return false;
	}

	// 拒绝关系检查(矩阵:列存在挡行)
	const UBXBehaviorSettings* Settings = GetDefault<UBXBehaviorSettings>();
	if (Settings)
	{
		FGameplayTagContainer ActiveContainer;
		GetActiveBehaviors(ActiveContainer);
		if (Settings->IsRejectedByAny(InBehaviorTag, ActiveContainer))
		{
			OutCheck.bCanStart = false;
			OutCheck.FailReason = TEXT("RejectedByRelation");
			return false;
		}

		// 挤出目标保护检查(目标受保护则本行为无法进入:霸体/取消窗口)
		// 列Tag可为族Tag:活跃条目为其子Tag时同样命中(与IsRejectedByAny列方向族语义一致)
		TArray<FGameplayTag> ExpelTargets;
		Settings->GetExpelTargets(InBehaviorTag, ExpelTargets);
		for (const FGameplayTag& TargetTag : ExpelTargets)
		{
			for (const TPair<FGameplayTag, FBXBehaviorRuntimeData>& Pair : ActiveBehaviors)
			{
				if (!Pair.Key.MatchesTag(TargetTag))
				{
					continue;
				}

				if (IsBehaviorProtected(Pair.Key))
				{
					OutCheck.bCanStart = false;
					OutCheck.FailReason = FString::Printf(TEXT("ExpelTargetProtected:%s"), *Pair.Key.ToString());
					return false;
				}
			}
		}
	}

	// Agent检查(纯查询无副作用;Agent持非const指针:CheckStartBehavior为UFUNCTION含蓝图事件调用,无法声明const)
	if (const TObjectPtr<UBXBehaviorAgent>* FindResult = BehaviorAgents.Find(InBehaviorTag))
	{
		UBXBehaviorAgent* Agent = *FindResult;
		if (!IsValid(Agent))
		{
			OutCheck.bCanStart = false;
			OutCheck.FailReason = TEXT("AgentInvalid");
			return false;
		}

		if (!Agent->CheckStartBehavior(InParameter))
		{
			OutCheck.bCanStart = false;
			OutCheck.FailReason = TEXT("AgentCheckFailed");
			return false;
		}
	}

	return true;
}

bool UBXBehaviorComponent::InternalStartBehavior(const FGameplayTag& InBehaviorTag, FInstancedStruct&& InParameter, int64 InSign)
{
	if (!InBehaviorTag.IsValid())
	{
		return false;
	}

	// Agent执行(重复Start=重启语义;具名右值引用作左值使用,参数此时未被移动)
	if (const TObjectPtr<UBXBehaviorAgent>* FindResult = BehaviorAgents.Find(InBehaviorTag))
	{
		UBXBehaviorAgent* Agent = *FindResult;
		if (!IsValid(Agent))
		{
			UE_LOG(BXBEHAVIOR, Warning, TEXT("UBXBehaviorComponent::InternalStartBehavior: agent invalid. Tag=%s"), *InBehaviorTag.ToString());
			return false;
		}

		if (!Agent->StartBehavior(InParameter))
		{
			UE_LOG(BXBEHAVIOR, Warning, TEXT("UBXBehaviorComponent::InternalStartBehavior: agent start failed. Tag=%s"), *InBehaviorTag.ToString());
			return false;
		}
	}

	// 表更新(新Sign追加/同Sign幂等)
	// 条目是否新建先判后建(FindOrAdd后Contains恒真):同Sign重复Start仅幂等,不重复广播Enter
	const bool bNewEntry = !ActiveBehaviors.Contains(InBehaviorTag);
	FBXBehaviorRuntimeData& Data = ActiveBehaviors.FindOrAdd(InBehaviorTag);
	Data.Tag = InBehaviorTag;
	// 移动存储(FuncLib模板路径LastStartParameter零额外拷贝;Agent已在上方使用完毕)
	Data.LastStartParameter = MoveTemp(InParameter);
	if (!Data.HasSource(InSign))
	{
		Data.Sources.Add(FBXBehaviorSource(InSign));
	}

	// 条目从无到有才广播Enter(重复Start不重复广播)
	if (bNewEntry)
	{
		BroadcastBehaviorEvent(true, InBehaviorTag, InSign, EBXBehaviorEndReason::BER_TMax);
	}

	return true;
}

bool UBXBehaviorComponent::InternalStopBehavior(const FGameplayTag& InBehaviorTag, const FInstancedStruct& InParameter, int64 InSign, EBXBehaviorEndReason InReason)
{
	FBXBehaviorRuntimeData* Data = ActiveBehaviors.Find(InBehaviorTag);
	if (!Data)
	{
		return false;
	}

	// 查无该Sign来源
	if (!Data->HasSource(InSign))
	{
		return false;
	}

	// 移除来源(先停先退:来源独立移除)
	Data->Sources.RemoveAll([InSign](const FBXBehaviorSource& Source) { return Source.Sign == InSign; });

	// 仍有其他来源:行为继续(仅来源退出)
	if (Data->Sources.Num() > 0)
	{
		return true;
	}

	// Agent停止
	if (const TObjectPtr<UBXBehaviorAgent>* FindResult = BehaviorAgents.Find(InBehaviorTag))
	{
		UBXBehaviorAgent* Agent = *FindResult;
		if (IsValid(Agent))
		{
			Agent->StopBehavior(InParameter);
		}
	}

	// 移除条目
	ActiveBehaviors.Remove(InBehaviorTag);

	// 广播Exit(最后来源退出,技能互锁监听点)
	BroadcastBehaviorEvent(false, InBehaviorTag, InSign, InReason);

	return true;
}

void UBXBehaviorComponent::CollectActiveBehaviorTags(TArray<FGameplayTag>& OutTags) const
{
	OutTags.Reset(ActiveBehaviors.Num());
	ActiveBehaviors.GetKeys(OutTags);
}

void UBXBehaviorComponent::BroadcastBehaviorEvent(bool bEnter, const FGameplayTag& InBehaviorTag, int64 InSign, EBXBehaviorEndReason InReason)
{
	UBXEventManager* EventMgr = UBXEventManager::Get(this);
	if (!IsValid(EventMgr))
	{
		return;
	}

	FBXEventBehaviorChanged Parameter(InBehaviorTag, InSign, InReason);
	if (bEnter)
	{
		EventMgr->BroadcastSingleEvent<FBXEventBehaviorChanged>(BXGameplayTags::BXEvent_Behavior_Enter, GetOwner(), Parameter);
		EventMgr->BroadcastGlobalEvent<FBXEventBehaviorChanged>(BXGameplayTags::BXEvent_Behavior_Enter, Parameter);
	}
	else
	{
		EventMgr->BroadcastSingleEvent<FBXEventBehaviorChanged>(BXGameplayTags::BXEvent_Behavior_Exit, GetOwner(), Parameter);
		EventMgr->BroadcastGlobalEvent<FBXEventBehaviorChanged>(BXGameplayTags::BXEvent_Behavior_Exit, Parameter);
	}
}

#pragma endregion Internal
