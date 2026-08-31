#include "BXStateComponent.h"

#include "BXGameplayTags.h"
#include "BXEventStructs.h"
#include "BXEventManager.h"
#include "BXFunctionLibrary.h"
#include "BXConditionManager.h"
#include "BXSettings.h"
#include "BXNetStructs.h"
#include "Behavior/BXBehaviorComponent.h"
#include "Skill/BXSkillComponent.h"
#include "Skill/BXSkillAsset.h"
#include "Timeline/BXTLComponent.h"
#include "Timeline/BXTLAsset.h"
#include "Animation/BXAnimInstance.h"

#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimMontage.h"
#include "Net/UnrealNetwork.h"
#include "Engine/NetDriver.h"

DEFINE_LOG_CATEGORY(BXSTATE);

constexpr int32 BX_STATE_ENTER_CHAIN_MAX = 8;

// 单端预测缓冲上限(防异常调用堆积;超限后仅失去自动回滚保护)
constexpr int32 BX_NET_PREDICT_MAX = 32;



UBXStateComponent::UBXStateComponent()
{
	SetIsReplicatedByDefault(true);

	PrimaryComponentTick.bCanEverTick = true;
}

void UBXStateComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 仅初始同步发送:新连接拿最新快照LateJoin重建,已有连接零属性流量(动态由显式RPC维护)
	DOREPLIFETIME_CONDITION(UBXStateComponent, RunningStateStates, COND_InitialOnly);
}

void UBXStateComponent::PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker)
{
	Super::PreReplication(ChangedPropertyTracker);

	// 服务器端、属性收集前每帧执行
	AActor* Owner = GetOwner();
	if (!Owner || Owner->GetLocalRole() != ENetRole::ROLE_Authority)
	{
		return;
	}

	// RunningStateStates为COND_InitialOnly:仅新连接初始同步时发送,已有连接零流量
	// 故仅当远程连接数增加(新客户端连入)时才重建快照
	// 时序:PreReplication在同一次flush的序列化前运行,当帧重建即被新连接消费,无窗口期
	UNetDriver* NetDriver = GetWorld() ? GetWorld()->GetNetDriver() : nullptr;
	if (!NetDriver)
	{
		return;
	}

	const int32 ConnectionCount = NetDriver->ClientConnections.Num();
	if (ConnectionCount > LastProjectedConnectionCount)
	{
		RebuildRunningStateStates();
	}
	// 断线回落仅同步计数不重建(InitialOnly已发收不回);不回落则"断N+连N"净计数不变会漏触发
	LastProjectedConnectionCount = ConnectionCount;
}

void UBXStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AActor* Owner = GetOwner();
	const bool bAuthority = Owner && Owner->GetLocalRole() == ENetRole::ROLE_Authority;

	// 到期评估链中的同步增删依赖快照(ExpiredTags/ExpiredSigns),回调中的Enter直接执行(表操作走快照安全)
	UpdateExpiredStates(DeltaTime);

	// SM转移评估仅服务器权威执行(SimulatedProxy纯多播跟随,禁本地评估分叉)
	if (bAuthority)
	{
		UpdateStateMachines(DeltaTime);
	}

	// 预测超时回滚(仅客户端且存在待结算条目;权威端缓冲恒空)
	if (!bAuthority && !PredictedStates.IsEmpty())
	{
		UpdatePredictedStateTimeouts(DeltaTime);
	}
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
	// 清空全部活跃状态(直摘表广播Exit,禁用解除交由行为组件EndPlay清空账本)
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
	PredictedStates.Empty();

	Super::EndPlay(EndPlayReason);
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

bool UBXStateComponent::DoesStateDisableBehavior(const FGameplayTag& InStateTag, const FGameplayTag& InBehaviorTag) const
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
				return Node->InterruptBehaviors.HasTag(InBehaviorTag) || Node->ForbidBehaviors.HasTag(InBehaviorTag);
			}
		}
	}

	// 裸状态查配置
	if (const FBXStateConfig* Config = StateConfigs.Find(InStateTag))
	{
		return Config->InterruptBehaviors.HasTag(InBehaviorTag) || Config->ForbidBehaviors.HasTag(InBehaviorTag);
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

bool UBXStateComponent::ExitStateAllSourcesInternal(const FGameplayTag& InStateTag, EBXStateEndReason InReason, bool bSuppressPresentation, bool bDeferBehaviorRelease)
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
		if (!InternalExitState(InStateTag, Source.Sign, InReason, bSuppressPresentation, bDeferBehaviorRelease))
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



#pragma region API Net
int64 UBXStateComponent::EnterStateNet(const FGameplayTag& InStateTag, float InDuration)
{
	AActor* Owner = GetOwner();
	const ENetRole OwnerRole = Owner ? Owner->GetLocalRole() : ROLE_None;

	// 权威端(含Standalone/ListenServer主机):直接本地执行
	if (OwnerRole == ENetRole::ROLE_Authority)
	{
		return EnterState(InStateTag, 0, InDuration) ? 0 : -1;
	}

	// AutonomousProxy:统一预测——本地完整执行+登记缓冲+服务器上报,确认靠Multicast匹配,拒绝/超时回滚
	// 预测Sign必须可定位:非Client签名时生成全新ClientSyncID
	if (OwnerRole == ENetRole::ROLE_AutonomousProxy)
	{
		// 族内SM状态预检早退(与服务器ServerEnterState对称):本地预测若先执行顶掉,
		// 拒绝回滚只移除新条目,被SER_External顶掉的旧状态在客户端丢失造成双端漂移
		if (FindMachineByStateTag(InStateTag))
		{
			UE_LOG(BXSTATE, Warning, TEXT("UBXStateComponent::EnterStateNet: SM state is authority-driven only. State=%s"), *InStateTag.ToString());
			return -1;
		}

		const int64 PredictSign = BXMakeSyncID(UBXFunctionLibrary::GetUniqueID(), EBXSyncInitiator::Client);

		++EnterChainDepth;
		const bool bResult = InternalEnterState(InStateTag, PredictSign, InDuration, EBXStateEndReason::SER_External);
		--EnterChainDepth;

		if (!bResult)
		{
			return -1;
		}

		RegisterPredictedState(InStateTag, PredictSign);
		ServerEnterState(InStateTag, PredictSign, InDuration, UBXFunctionLibrary::GetServerWorldTimeMilliseconds(this));
		return PredictSign;
	}

	UE_LOG(BXSTATE, Warning, TEXT("UBXStateComponent::EnterStateNet: simulated proxy follows multicast only. State=%s"), *InStateTag.ToString());
	return -1;
}

bool UBXStateComponent::ExitStateNet(const FGameplayTag& InStateTag, int64 InSign)
{
	AActor* Owner = GetOwner();
	const ENetRole OwnerRole = Owner ? Owner->GetLocalRole() : ROLE_None;

	// 权威端:直接本地执行
	if (OwnerRole == ENetRole::ROLE_Authority)
	{
		return ExitState(InStateTag, InSign);
	}

	// AutonomousProxy:本地执行+上报(退出可能早于确认到达,一并无条件注销预测条目);
	// 仅允许退出Client签名来源(EnterStateNet生成即Client签名,语义天然对齐;服务器侧同校验,提前拦截防双端漂移)
	if (OwnerRole == ENetRole::ROLE_AutonomousProxy)
	{
		if (BXGetSyncIDInitiator(InSign) != EBXSyncInitiator::Client || InSign == 0)
		{
			UE_LOG(BXSTATE, Warning, TEXT("UBXStateComponent::ExitStateNet: non-client sign rejected. State=%s Sign=%lld"), *InStateTag.ToString(), InSign);
			return false;
		}

		const bool bResult = InternalExitState(InStateTag, InSign, EBXStateEndReason::SER_Manual);
		UnregisterPredictedState(InStateTag, InSign);
		ServerExitState(InStateTag, InSign);
		return bResult;
	}

	return false;
}

#pragma endregion API Net


#pragma region RPC StateSync
bool UBXStateComponent::ServerEnterState_Validate(FGameplayTag InStateTag, int64 InSign, float InDuration, int64 InClientTimestamp)
{
	return FMath::IsFinite(InDuration) && FMath::Abs(InDuration) < 36000.0f;
}

void UBXStateComponent::ServerEnterState_Implementation(FGameplayTag InStateTag, int64 InSign, float InDuration, int64 InClientTimestamp)
{
	// 发起端校验:仅接受客户端签名(SM族内状态同样不接受客户端自主请求——族内互斥/转移由权威驱动)
	if (BXGetSyncIDInitiator(InSign) != EBXSyncInitiator::Client || InSign == 0)
	{
		UE_LOG(BXSTATE, Warning, TEXT("UBXStateComponent::ServerEnterState: invalid initiator. State=%s Sign=%lld"), *InStateTag.ToString(), InSign);
		ClientRejectState(InStateTag, InSign);
		return;
	}

	// 族内SM状态仅权威驱动(防客户端伪造硬直/浮空等处境;外部进入语义只服务技能等权威管线)
	if (FindMachineByStateTag(InStateTag))
	{
		UE_LOG(BXSTATE, Warning, TEXT("UBXStateComponent::ServerEnterState: SM state rejected from client. State=%s"), *InStateTag.ToString());
		ClientRejectState(InStateTag, InSign);
		return;
	}

	// 裸状态存在性校验(配置面=StateConfigs,不存在的Tag一律拒绝——
	// 若裸状态参与服务端判定即构成伪造作弊面;技能链路走本地API Sign=SkillID不受此限)
	if (!StateConfigs.Contains(InStateTag))
	{
		UE_LOG(BXSTATE, Warning, TEXT("UBXStateComponent::ServerEnterState: unconfigured bare state rejected. State=%s"), *InStateTag.ToString());
		ClientRejectState(InStateTag, InSign);
		return;
	}

	// 防重:同(Tag,Sign)已在事实表→静默忽略(Reliable不会重传到应用层,防御异常重放;不回Reject避免误删已确认条目)
	if (const FBXStateRuntimeData* Data = ActiveStates.Find(InStateTag))
	{
		if (Data->HasSource(InSign))
		{
			return;
		}
	}

	// 请求年龄校验(服务器世界时间域;客户端经引擎校时上传时间戳)
	const int64 AgeMs = UBXFunctionLibrary::GetServerWorldTimeMilliseconds(this) - InClientTimestamp;
	if (AgeMs > GetDefault<UBXSettings>()->StateRequestMaxAgeMs)
	{
		UE_LOG(BXSTATE, Log, TEXT("UBXStateComponent::ServerEnterState: stale request rejected. State=%s Sign=%lld Age=%lldms"), *InStateTag.ToString(), InSign, AgeMs);
		ClientRejectState(InStateTag, InSign);
		return;
	}

	// 权威执行(裸状态配置时长解析在管线内部完成);多播确认经BroadcastStateEvent权威门发出
	// 条目已存在时管线内不走 Enter广播(bNewEntry=false),事件门不会发出多播——
	// 此处前置记录,来源追加成功后显式补发确认多播,否则发起端预测缓冲超时误回滚造成双端漂移
	const bool bEntryExisted = ActiveStates.Contains(InStateTag);

	// 权威执行(裸状态配置时长解析在管线内部完成);条目新建场景的多播由BroadcastStateEvent权威门发出
	InternalEnterState(InStateTag, InSign, InDuration, EBXStateEndReason::SER_External);

	if (bEntryExisted)
	{
		MulticastStateEnter(InStateTag, InSign, InDuration);
	}
}

bool UBXStateComponent::ServerExitState_Validate(FGameplayTag InStateTag, int64 InSign)
{
	return true;
}

void UBXStateComponent::ServerExitState_Implementation(FGameplayTag InStateTag, int64 InSign)
{
	// 发起方校验:仅允许退出Client签名来源(Server/系统Sign=SkillID等来源的生命周期归权威管线,
	// 由技能收束/到期/转移驱动;客户端伪造上报一律忽略)
	if (BXGetSyncIDInitiator(InSign) != EBXSyncInitiator::Client || InSign == 0)
	{
		UE_LOG(BXSTATE, Warning, TEXT("UBXStateComponent::ServerExitState: invalid initiator rejected. State=%s Sign=%lld"), *InStateTag.ToString(), InSign);
		return;
	}

	// 幂等:查无即忽略
	InternalExitState(InStateTag, InSign, EBXStateEndReason::SER_Manual);
}

void UBXStateComponent::ClientRejectState_Implementation(FGameplayTag InStateTag, int64 InSign)
{
	// 回滚=本地移除该来源(SER_PredictRollback路径不触发表现);注销缓冲双保险
	UnregisterPredictedState(InStateTag, InSign);
	InternalExitState(InStateTag, InSign, EBXStateEndReason::SER_PredictRollback);
}

void UBXStateComponent::MulticastStateEnter_Implementation(FGameplayTag InStateTag, int64 InSign, float InDuration)
{
	AActor* Owner = GetOwner();
	if (!Owner || Owner->GetLocalRole() == ENetRole::ROLE_Authority)
	{
		return;
	}

	HandleClientStateEnter(InStateTag, InSign, InDuration);
}

void UBXStateComponent::MulticastStateExit_Implementation(FGameplayTag InStateTag, int64 InSign, uint8 InReason)
{
	AActor* Owner = GetOwner();
	if (!Owner || Owner->GetLocalRole() == ENetRole::ROLE_Authority)
	{
		return;
	}

	HandleClientStateExit(InStateTag, InSign, static_cast<EBXStateEndReason>(InReason));
}

void UBXStateComponent::MulticastStatePresentation_Implementation(FGameplayTag InStateTag, FBXStatePresentation InPresentation)
{
	AActor* Owner = GetOwner();
	if (!Owner || Owner->GetLocalRole() == ENetRole::ROLE_Authority)
	{
		return;
	}

	TriggerPresentation(InPresentation, InStateTag);
}

#pragma endregion RPC StateSync




#pragma region Internal
bool UBXStateComponent::InternalEnterState(const FGameplayTag& InStateTag, int64 InSign, float InDuration, EBXStateEndReason InExternalReason)
{
	if (!InStateTag.IsValid())
	{
		return false;
	}

	// 解析时长与门控列表(规则二:外部携带时长优先,≤0用节点/配置默认;门控列表始终查静态配置)
	float ResolvedDuration = InDuration;
	FGameplayTagContainer InterruptBehaviors;
	FGameplayTagContainer ForbidBehaviors;
	{
		float DefaultDuration = -1.0f;
		FGameplayTagContainer DefaultInterrupt;
		FGameplayTagContainer DefaultForbid;
		if (GetStateBehaviorConfig(InStateTag, DefaultDuration, DefaultInterrupt, DefaultForbid))
		{
			if (InDuration <= 0.0f)
			{
				ResolvedDuration = DefaultDuration;
			}
			InterruptBehaviors = MoveTemp(DefaultInterrupt);
			ForbidBehaviors = MoveTemp(DefaultForbid);
		}
	}

	// 族内单活:外部进入顶掉当前节点(规则一)
	UBXStateMachineInstance* Machine = FindMachineByStateTag(InStateTag);
	FGameplayTag DisplacedStateTag;
	FGameplayTagContainer DisplacedInterrupt;
	FGameplayTagContainer DisplacedForbid;
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
			// 缓存旧Tag与禁用列表:首个来源退出时InternalExitState会置空CurrentNode,循环内重复解引用会崩溃
			DisplacedStateTag = Machine->CurrentNode->StateTag;
			if (Machine->Asset)
			{
				if (const UBXSMStateNode* OldNode = Machine->Asset->FindStateNode(DisplacedStateTag))
				{
					DisplacedInterrupt = OldNode->InterruptBehaviors;
					DisplacedForbid = OldNode->ForbidBehaviors;
				}
			}

			TArray<FBXStateSource> Sources;
			if (const FBXStateRuntimeData* CurrentData = ActiveStates.Find(DisplacedStateTag))
			{
				Sources = CurrentData->Sources;
			}
			for (const FBXStateSource& Source : Sources)
			{
				// 门控解除延迟:待新状态登记账本后统一解除(同ExecuteTransition,共享禁用Tag无解禁→再禁抖动)
				InternalExitState(DisplacedStateTag, Source.Sign, InExternalReason, false, true);
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
		ApplyBehaviorGates(InterruptBehaviors, ForbidBehaviors, InStateTag);

		// 族内进入表现由ExecuteTransition按转移边统一触发,此处仅裸状态触发配置的进入表现
		if (!Machine)
		{
			if (const FBXStateConfig* Config = StateConfigs.Find(InStateTag))
			{
				TriggerPresentation(Config->EntryPresentation, InStateTag);
			}
		}

		BroadcastStateEvent(true, InStateTag, InSign, ResolvedDuration, EBXStateEndReason::SER_TMax);
	}

	// 被顶掉旧状态的禁止解除(新状态已登记:共享禁用Tag经账本多来源登记保持禁止,独占Tag解除恢复;
	// 事件监听者重入旧状态时ReleaseBehaviorGates内含重入保护,登记不被误清)
	if (DisplacedStateTag.IsValid())
	{
		ReleaseBehaviorGates(DisplacedForbid, DisplacedStateTag);
	}

	return true;
}

bool UBXStateComponent::InternalExitState(const FGameplayTag& InStateTag, int64 InSign, EBXStateEndReason InReason, bool bSuppressPresentation, bool bDeferBehaviorRelease)
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

	// 静态配置查询(表现与门控解除共用)
	FGameplayTagContainer InterruptBehaviors;
	FGameplayTagContainer ForbidBehaviors;
	UBXStateMachineInstance* Machine = FindMachineByStateTag(InStateTag);
	if (Machine)
	{
		if (Machine->Asset)
		{
			if (const UBXSMStateNode* Node = Machine->Asset->FindStateNode(InStateTag))
			{
				InterruptBehaviors = Node->InterruptBehaviors;
				ForbidBehaviors = Node->ForbidBehaviors;
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
		InterruptBehaviors = Config->InterruptBehaviors;
		ForbidBehaviors = Config->ForbidBehaviors;
	}

	// 表现(预测回滚强制不触发;仅裸状态触发配置的退出表现)
	if (!bSuppressPresentation && InReason != EBXStateEndReason::SER_PredictRollback && !Machine)
	{
		if (const FBXStateConfig* Config = StateConfigs.Find(InStateTag))
		{
			TriggerPresentation(Config->ExitPresentation, InStateTag);
		}
	}

	// 门控解除判定→行为恢复(表现抑制时门控仍需解除;转移路径延迟到新状态登记后由调用方统一解除)
	if (!bDeferBehaviorRelease)
	{
		ReleaseBehaviorGates(ForbidBehaviors, InStateTag);
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

	// 族内状态到期→状态机评估转移(不直接移除,由转移目标顶掉或评估失败后自然退出;评估仅服务器权威执行)
	// 裸状态到期→自然退出(仅服务器:跟随端事实由MulticastStateExit镜像,本地不裁决防分叉)
	AActor* Owner = GetOwner();
	const bool bAuthority = Owner && Owner->GetLocalRole() == ENetRole::ROLE_Authority;
	if (!bAuthority)
	{
		return;
	}

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

		// 边评估上下文(服务器世界时间域,与技能系统一致;单机=本地时间,网络游戏=GameState校时后的服务器时间)
		FBXSMTransitionContext Context;
		Context.Owner = GetOwner();
		Context.FromTag = CurrentTag;
		Context.ServerTimeMs = UBXFunctionLibrary::GetServerWorldTimeMilliseconds(this);

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

	// 旧状态门控集合快照(退出前收集:延迟解除用)
	const FGameplayTag CurrentTag = InMachine->CurrentNode->StateTag;
	const FGameplayTagContainer OldInterrupt = InMachine->CurrentNode->InterruptBehaviors;
	const FGameplayTagContainer OldForbid = InMachine->CurrentNode->ForbidBehaviors;

	// 退出当前(全部来源;抑制Exit表现;禁止解除延迟到新状态登记后:共享禁用Tag经账本多重登记保持,独占Tag解除,无解禁→再禁抖动)
	ExitStateAllSourcesInternal(CurrentTag, InReason, true, true);

	// 进入目标(节点默认时长,Sign=0状态机自身)
	bool bResult = InternalEnterState(InTargetNode->StateTag, 0, InTargetNode->Duration, InReason);

	// 解除旧状态禁止登记(新状态已登记:共享Tag的账本仍持新状态登记不会被解除,行为不抖动;进入失败同样解除,禁用不残留)
	ReleaseBehaviorGates(OldForbid, CurrentTag);

	// 转移表现(唯一入口=边上的TransitionPresentation,未配置则无表现)
	if (bResult && TransitionEdge)
	{
		TriggerPresentation(TransitionEdge->TransitionPresentation, InTargetNode->StateTag);
	}

	return bResult;
}

void UBXStateComponent::TriggerPresentation(const FBXStatePresentation& InPresentation, const FGameplayTag& InStateTag)
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

	// 表现触发点唯一收束多播(跟随端收到后调本函数本播;各端本地语义,非权威端不再转发)
	AActor* Owner = GetOwner();
	if (Owner->GetLocalRole() == ENetRole::ROLE_Authority)
	{
		MulticastStatePresentation(InStateTag, InPresentation);
	}

	switch (InPresentation.Type)
	{
	case EBXPresentationType::PT_Skill:
	{
		// 技能通道(自带同步体系):仅权威端本播,技能经自身多播同步到达各端——
		// 跟随端再本播会在自主端走客户端预测+ServerPlaySkill上报,服务器权威端二次播放造成全端双份
		// (代价:跟随端技能到达晚约1个RTT,与服务器发起的表现语义一致)
		if (Owner->GetLocalRole() != ENetRole::ROLE_Authority)
		{
			break;
		}

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

void UBXStateComponent::ApplyBehaviorGates(const FGameplayTagContainer& InInterruptBehaviors, const FGameplayTagContainer& InForbidBehaviors, const FGameplayTag& InByState)
{
	if (InInterruptBehaviors.IsEmpty() && InForbidBehaviors.IsEmpty())
	{
		return;
	}

	if (UBXBehaviorComponent* BehaviorComp = GetOwner()->FindComponentByClass<UBXBehaviorComponent>())
	{
		for (int32 i = 0; i < InInterruptBehaviors.Num(); ++i)
		{
			BehaviorComp->InterruptBehavior(InInterruptBehaviors.GetByIndex(i));
		}
		for (int32 i = 0; i < InForbidBehaviors.Num(); ++i)
		{
			BehaviorComp->ForbidBehavior(InForbidBehaviors.GetByIndex(i), InByState, 0);
		}
	}
}

void UBXStateComponent::ReleaseBehaviorGates(const FGameplayTagContainer& InForbidBehaviors, const FGameplayTag& InByState)
{
	if (InForbidBehaviors.IsEmpty())
	{
		return;
	}

	// 重入保护:解除窗口内状态被重新进入时登记仍有效,跳过解除
	// 场景1(延迟解除):ExecuteTransition的Exit/Enter事件监听者同步EnterState重入旧状态(ForbidBehavior幂等追加账本来源),随后统一解除会误清活跃状态登记
	// 场景2(即时解除):InternalExitState的Exit表现触发技能EnterStates重入自身,表现后解除会误清新登记
	if (ActiveStates.Contains(InByState))
	{
		return;
	}

	if (UBXBehaviorComponent* BehaviorComp = GetOwner()->FindComponentByClass<UBXBehaviorComponent>())
	{
		for (int32 i = 0; i < InForbidBehaviors.Num(); ++i)
		{
			// 账本按来源登记解除(最后一个禁用状态退出才解禁;中断是一次性动作无解除)
			BehaviorComp->UnforbidBehavior(InForbidBehaviors.GetByIndex(i), InByState, 0);
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

	AActor* Owner = GetOwner();
	FBXEventStateChanged Parameter(InStateTag, InSign, InDuration, InReason);
	if (bEnter)
	{
		EventMgr->BroadcastSingleEvent<FBXEventStateChanged>(BXGameplayTags::BXEvent_State_Enter, Owner, Parameter);
		EventMgr->BroadcastGlobalEvent<FBXEventStateChanged>(BXGameplayTags::BXEvent_State_Enter, Parameter);

		// 权威端事实动态主通道(多播到达即跟随端重建+预测确认;清场随Actor销毁广播无意义)
		if (Owner && Owner->GetLocalRole() == ENetRole::ROLE_Authority)
		{
			MulticastStateEnter(InStateTag, InSign, InDuration);
		}
	}
	else
	{
		EventMgr->BroadcastSingleEvent<FBXEventStateChanged>(BXGameplayTags::BXEvent_State_Exit, Owner, Parameter);
		EventMgr->BroadcastGlobalEvent<FBXEventStateChanged>(BXGameplayTags::BXEvent_State_Exit, Parameter);

		if (Owner && Owner->GetLocalRole() == ENetRole::ROLE_Authority && InReason != EBXStateEndReason::SER_Cleared)
		{
			MulticastStateExit(InStateTag, InSign, (uint8)InReason);
		}
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

bool UBXStateComponent::GetStateBehaviorConfig(const FGameplayTag& InStateTag, float& OutDuration, FGameplayTagContainer& OutInterrupt, FGameplayTagContainer& OutForbid) const
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
				OutInterrupt = Node->InterruptBehaviors;
				OutForbid = Node->ForbidBehaviors;
				return true;
			}
		}
	}

	// 裸状态配置
	if (const FBXStateConfig* Config = StateConfigs.Find(InStateTag))
	{
		OutDuration = Config->Duration;
		OutInterrupt = Config->InterruptBehaviors;
		OutForbid = Config->ForbidBehaviors;
		return true;
	}

	return false;
}

#pragma endregion Internal



#pragma region Internal Net
void UBXStateComponent::RegisterPredictedState(const FGameplayTag& InStateTag, int64 InSign)
{
	// 去重(同Tag同Sign重复登记无意义)
	for (const FBXPredictedState& Entry : PredictedStates)
	{
		if (Entry.Tag == InStateTag && Entry.Sign == InSign)
		{
			return;
		}
	}

	if (PredictedStates.Num() >= BX_NET_PREDICT_MAX)
	{
		UE_LOG(BXSTATE, Warning, TEXT("UBXStateComponent::RegisterPredictedState: buffer full (%d). State=%s Sign=%lld loses auto-rollback protection."), PredictedStates.Num(), *InStateTag.ToString(), InSign);
		return;
	}

	FBXPredictedState Entry;
	Entry.Tag = InStateTag;
	Entry.Sign = InSign;
	Entry.ElapsedTime = 0.0f;
	PredictedStates.Add(Entry);
}

bool UBXStateComponent::UnregisterPredictedState(const FGameplayTag& InStateTag, int64 InSign)
{
	const int32 RemovedNum = PredictedStates.RemoveAll([InStateTag, InSign](const FBXPredictedState& Entry) { return Entry.Tag == InStateTag && Entry.Sign == InSign; });
	return RemovedNum > 0;
}

void UBXStateComponent::UpdatePredictedStateTimeouts(float InDeltaTime)
{
	const float MaxDuration = GetDefault<UBXSettings>()->StatePredictMaxDuration;

	// 快照收集后逐条结算(TArray索引式遍历,移除自当前推进位安全;回调可能同步增删缓冲)
	for (int32 i = 0; i < PredictedStates.Num(); )
	{
		PredictedStates[i].ElapsedTime += InDeltaTime;
		if (PredictedStates[i].ElapsedTime < MaxDuration)
		{
			++i;
			continue;
		}

		const FBXPredictedState Entry = PredictedStates[i];
		PredictedStates.RemoveAt(i);

		UE_LOG(BXSTATE, Log, TEXT("UBXStateComponent::UpdatePredictedStateTimeouts: rollback. State=%s Sign=%lld"), *Entry.Tag.ToString(), Entry.Sign);
		InternalExitState(Entry.Tag, Entry.Sign, EBXStateEndReason::SER_PredictRollback);
	}
}

void UBXStateComponent::RebuildRunningStateStates()
{
	RunningStateStates.Reset();

	for (const TPair<FGameplayTag, FBXStateRuntimeData>& Pair : ActiveStates)
	{
		FBXStateReplicatedState State;
		State.StateTag = Pair.Key;

		// 来源投影为剩余时长(≤0无限保持;重建时以接收时刻为计时零点)
		for (const FBXStateSource& Source : Pair.Value.Sources)
		{
			FBXStateSyncSource SyncSource;
			SyncSource.Sign = Source.Sign;
			if (Source.Duration > 0.0f)
			{
				SyncSource.RemainingDuration = Source.Duration - Source.ElapsedTime;
				if (SyncSource.RemainingDuration <= 0.0f)
				{
					SyncSource.RemainingDuration = KINDA_SMALL_NUMBER;
				}
			}
			else
			{
				SyncSource.RemainingDuration = -1.0f;
			}
			State.Sources.Add(SyncSource);
		}

		RunningStateStates.Add(State);
	}
}

void UBXStateComponent::OnRep_RunningStateStates(TArray<FBXStateReplicatedState> InOldStates)
{
	AActor* Owner = GetOwner();
	if (!Owner || Owner->GetLocalRole() == ENetRole::ROLE_Authority)
	{
		return;
	}

	// 新增条目:LateJoin静默重建
	for (const FBXStateReplicatedState& State : RunningStateStates)
	{
		bool bExisted = false;
		for (const FBXStateReplicatedState& OldState : InOldStates)
		{
			if (OldState.StateTag == State.StateTag)
			{
				bExisted = true;
				break;
			}
		}

		if (!bExisted)
		{
			RebuildStateFromState(State);
		}
	}

	// 消失条目:兜底清理(主通道MulticastStateExit,仅处理RPC与属性乱序竞态;静默移除不广播不触表现)
	for (const FBXStateReplicatedState& OldState : InOldStates)
	{
		bool bStillExists = false;
		for (const FBXStateReplicatedState& State : RunningStateStates)
		{
			if (State.StateTag == OldState.StateTag)
			{
				bStillExists = true;
				break;
			}
		}

		if (!bStillExists && ActiveStates.Contains(OldState.StateTag))
		{
			// 静默摘表(RPC与快照乱序竞态的防御路径,不走管线防误触表现/事件)
			UE_LOG(BXSTATE, Log, TEXT("UBXStateComponent::OnRep_RunningStateStates: entry vanished, fallback cleanup. State=%s"), *OldState.StateTag.ToString());
			ActiveStates.Remove(OldState.StateTag);

			// SM CurrentNode镜像清理
			if (UBXStateMachineInstance* Machine = FindMachineByStateTag(OldState.StateTag))
			{
				if (Machine->CurrentNode && Machine->CurrentNode->StateTag == OldState.StateTag)
				{
					Machine->CurrentNode = nullptr;
				}
			}
		}
	}
}

void UBXStateComponent::HandleClientStateEnter(const FGameplayTag& InStateTag, int64 InSign, float InDuration)
{
	// 幂等:先注销预测条目再查重(重复到达与预测确认合一处理)
	UnregisterPredictedState(InStateTag, InSign);

	if (const FBXStateRuntimeData* Data = ActiveStates.Find(InStateTag))
	{
		if (Data->HasSource(InSign))
		{
			return;
		}
	}

	// 表更新(权威事实镜像:不做门控、不做SM顶掉——转移评估仅服务器执行)
	FBXStateRuntimeData& Data = ActiveStates.FindOrAdd(InStateTag);
	Data.Tag = InStateTag;
	Data.Sources.Add(FBXStateSource(InSign, InDuration));

	// SM CurrentNode镜像(与服务器InternalEnterState对齐:转移/外部进入均置为目标节点,维持本端查询一致性)
	if (UBXStateMachineInstance* Machine = FindMachineByStateTag(InStateTag))
	{
		if (Machine->Asset)
		{
			if (UBXSMStateNode* TargetNode = Machine->Asset->FindStateNode(InStateTag))
			{
				Machine->CurrentNode = TargetNode;
			}
		}
	}

	// 本地事件广播(表现层监听者各端驱动;非权威端不再转发多播)
	BroadcastStateEvent(true, InStateTag, InSign, InDuration, EBXStateEndReason::SER_TMax);
}

void UBXStateComponent::HandleClientStateExit(const FGameplayTag& InStateTag, int64 InSign, EBXStateEndReason InReason)
{
	// 查无该来源(RPC早于快照或重复到达):仅做预测确认注销后返回
	UnregisterPredictedState(InStateTag, InSign);
	const FBXStateRuntimeData* Data = ActiveStates.Find(InStateTag);
	if (!Data || !Data->HasSource(InSign))
	{
		return;
	}

	// 跟随退出走同一管线(条目移除+SM CurrentNode置空+裸状态退场表现+本地事件;SER_PredictRollback路径管线内自动不触表现)
	InternalExitState(InStateTag, InSign, InReason);
}

void UBXStateComponent::RebuildStateFromState(const FBXStateReplicatedState& InState)
{
	// Late Join静默重建(仅事实表+SM CurrentNode反查恢复;不发事件不触表现不动门控——
	// 行为快照侧STOPPED标志已编码停运终态,与技能侧RebuildSkillFromProjection一致)
	if (ActiveStates.Contains(InState.StateTag))
	{
		return;
	}

	FBXStateRuntimeData& Data = ActiveStates.Add(InState.StateTag);
	Data.Tag = InState.StateTag;
	for (const FBXStateSyncSource& SyncSource : InState.Sources)
	{
		Data.Sources.Add(FBXStateSource(SyncSource.Sign, SyncSource.RemainingDuration));
	}

	// SM CurrentNode由条目Tag反查资产恢复(后续转移虽仅服务器评估,CurrentNode维持查询一致性)
	if (UBXStateMachineInstance* Machine = FindMachineByStateTag(InState.StateTag))
	{
		if (Machine->Asset && !Machine->CurrentNode)
		{
			Machine->CurrentNode = Machine->Asset->FindStateNode(InState.StateTag);
		}
	}
}

#pragma endregion Internal Net
