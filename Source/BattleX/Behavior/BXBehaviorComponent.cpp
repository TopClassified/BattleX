#include "BXBehaviorComponent.h"

#include "BXGameplayTags.h"
#include "BXEventStructs.h"
#include "BXEventManager.h"
#include "BXBehaviorSettings.h"
#include "BXSettings.h"
#include "BXFunctionLibrary.h"
#include "BXNetStructs.h"

#include "State/BXStateComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/NetDriver.h"

DEFINE_LOG_CATEGORY(BXBEHAVIOR);

constexpr int32 BX_BEHAVIOR_ENTER_CHAIN_MAX = 8;

// 行为控制包操作码(MulticastControlBehavior.InOp)
#define BX_NET_BEHAVIOR_OP_SUSPEND 0
#define BX_NET_BEHAVIOR_OP_RESUME  1

// 单端预测缓冲上限(防异常调用堆积;超限后仅失去自动回滚保护)
constexpr int32 BX_NET_PREDICT_MAX = 32;



UBXBehaviorComponent::UBXBehaviorComponent()
{
	SetIsReplicatedByDefault(true);

	PrimaryComponentTick.bCanEverTick = true;
}

void UBXBehaviorComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 仅初始同步发送:新连接拿最新快照LateJoin重建,已有连接零属性流量(动态由显式RPC维护)
	DOREPLIFETIME_CONDITION(UBXBehaviorComponent, RunningBehaviorStates, COND_InitialOnly);
}

void UBXBehaviorComponent::PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker)
{
	Super::PreReplication(ChangedPropertyTracker);

	// 服务器端、属性收集前每帧执行
	AActor* Owner = GetOwner();
	if (!Owner || Owner->GetLocalRole() != ENetRole::ROLE_Authority)
	{
		return;
	}

	// RunningBehaviorStates为COND_InitialOnly:仅新连接初始同步时发送,已有连接零流量
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
		RebuildRunningBehaviorStates();
	}
	// 断线回落仅同步计数不重建(InitialOnly已发收不回);不回落则"断N+连N"净计数不变会漏触发
	LastProjectedConnectionCount = ConnectionCount;
}

void UBXBehaviorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 预测超时回滚(仅客户端且存在待结算条目;权威端缓冲恒空)
	if (!PredictedBehaviors.IsEmpty())
	{
		AActor* Owner = GetOwner();
		if (Owner && Owner->GetLocalRole() != ENetRole::ROLE_Authority)
		{
			UpdatePredictedBehaviorTimeouts(DeltaTime);
		}
	}
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
	PredictedBehaviors.Empty();

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

		// 控制包广播(Tag粒度精确镜像服务器Agent停转;接收端配合本地每来源事件流重建表现)
		MulticastControlBehavior(Tag, BX_NET_BEHAVIOR_OP_SUSPEND);

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

		// 控制包广播(Tag粒度精确镜像服务器Agent重启;接收端以最近启动参数回放)
		MulticastControlBehavior(Tag, BX_NET_BEHAVIOR_OP_RESUME);

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



#pragma region API Net
int64 UBXBehaviorComponent::StartBehaviorNet(const FGameplayTag& InBehaviorTag, FInstancedStruct InParameter, int64 InSign)
{
	AActor* Owner = GetOwner();
	const ENetRole OwnerRole = Owner ? Owner->GetLocalRole() : ROLE_None;

	// 权威端(含Standalone/ListenServer主机):直接本地执行
	if (OwnerRole == ENetRole::ROLE_Authority)
	{
		return StartBehaviorWithParameter(InBehaviorTag, MoveTemp(InParameter), InSign) ? InSign : -1;
	}

	// AutonomousProxy:统一预测——本地完整执行+登记缓冲+服务器上报,确认靠Multicast匹配,拒绝/超时回滚
	// 预测Sign必须可定位:非Client签名时生成全新ClientSyncID(否则服务器无法防重/客户端无法定位回滚)
	if (OwnerRole == ENetRole::ROLE_AutonomousProxy)
	{
		const int64 PredictSign = (BXGetSyncIDInitiator(InSign) == EBXSyncInitiator::Client && InSign != 0)
			? InSign
			: BXMakeSyncID(UBXFunctionLibrary::GetUniqueID(), EBXSyncInitiator::Client);

		FBXBehaviorStartCheck Check;
		if (!CanStartBehaviorInternal(InBehaviorTag, InParameter, Check))
		{
			UE_LOG(BXBEHAVIOR, Warning, TEXT("UBXBehaviorComponent::StartBehaviorNet: rejected(%s). Tag=%s Sign=%lld"), *Check.FailReason, *InBehaviorTag.ToString(), PredictSign);
			return -1;
		}

		++EnterChainDepth;
		const bool bResult = InternalStartBehavior(InBehaviorTag, MoveTemp(InParameter), PredictSign);
		--EnterChainDepth;

		if (!bResult)
		{
			return -1;
		}

		RegisterPredictedBehavior(InBehaviorTag, PredictSign);
		ServerEnterBehavior(InBehaviorTag, PredictSign, UBXFunctionLibrary::GetServerWorldTimeMilliseconds(this));
		return PredictSign;
	}

	UE_LOG(BXBEHAVIOR, Warning, TEXT("UBXBehaviorComponent::StartBehaviorNet: simulated proxy follows multicast only. Tag=%s"), *InBehaviorTag.ToString());
	return -1;
}

bool UBXBehaviorComponent::StopBehaviorNet(const FGameplayTag& InBehaviorTag, int64 InSign)
{
	AActor* Owner = GetOwner();
	const ENetRole OwnerRole = Owner ? Owner->GetLocalRole() : ROLE_None;

	// 权威端:直接本地执行
	if (OwnerRole == ENetRole::ROLE_Authority)
	{
		return StopBehavior(InBehaviorTag, InSign);
	}

	// AutonomousProxy:本地执行+上报(退出可能早于确认到达,一并无条件注销预测条目);
	// 仅允许退出Client签名来源(Server/系统Sign的生命周期归权威管线,上报也会被服务器拒绝,提前拦截防双端漂移)
	if (OwnerRole == ENetRole::ROLE_AutonomousProxy)
	{
		if (BXGetSyncIDInitiator(InSign) != EBXSyncInitiator::Client || InSign == 0)
		{
			UE_LOG(BXBEHAVIOR, Warning, TEXT("UBXBehaviorComponent::StopBehaviorNet: non-client sign rejected. Tag=%s Sign=%lld"), *InBehaviorTag.ToString(), InSign);
			return false;
		}

		const bool bResult = InternalStopBehavior(InBehaviorTag, FInstancedStruct(), InSign, EBXBehaviorEndReason::BER_Manual);
		UnregisterPredictedBehavior(InBehaviorTag, InSign);
		ServerExitBehavior(InBehaviorTag, InSign);
		return bResult;
	}

	return false;
}

#pragma endregion API Net


#pragma region RPC BehaviorSync
bool UBXBehaviorComponent::ServerEnterBehavior_Validate(FGameplayTag InBehaviorTag, int64 InSign, int64 InClientTimestamp)
{
	return BXGetSyncIDInitiator(InSign) == EBXSyncInitiator::Client;
}

void UBXBehaviorComponent::ServerEnterBehavior_Implementation(FGameplayTag InBehaviorTag, int64 InSign, int64 InClientTimestamp)
{
	// 防重:同(Tag,Sign)已在事实表→静默忽略(Reliable不会重传到应用层,防御 packet 层异常重放;不回Reject避免误删已确认条目)
	if (const FBXBehaviorRuntimeData* Data = ActiveBehaviors.Find(InBehaviorTag))
	{
		if (Data->HasSource(InSign))
		{
			return;
		}
	}

	// 请求年龄校验(服务器世界时间域;客户端经引擎校时上传时间戳,过老请求视为重放/迟滞攻击面拒绝)
	const int64 AgeMs = UBXFunctionLibrary::GetServerWorldTimeMilliseconds(this) - InClientTimestamp;
	if (AgeMs > GetDefault<UBXSettings>()->BehaviorRequestMaxAgeMs)
	{
		UE_LOG(BXBEHAVIOR, Log, TEXT("UBXBehaviorComponent::ServerEnterBehavior: stale request rejected. Tag=%s Sign=%lld Age=%lldms"), *InBehaviorTag.ToString(), InSign, AgeMs);
		ClientRejectBehavior(InBehaviorTag, InSign);
		return;
	}

	// 权威裁决走同一本地管线(矩阵/挂起/Agent检查);失败回滚通知发起端
	FBXBehaviorStartCheck Check;
	if (!CanStartBehaviorInternal(InBehaviorTag, FInstancedStruct(), Check))
	{
		UE_LOG(BXBEHAVIOR, Log, TEXT("UBXBehaviorComponent::ServerEnterBehavior: server rejected(%s). Tag=%s Sign=%lld"), *Check.FailReason, *InBehaviorTag.ToString(), InSign);
		ClientRejectBehavior(InBehaviorTag, InSign);
		return;
	}

	// 条目已存在时管线内不走 Enter广播(bNewEntry=false),事件门不会发出多播——
	// 此处前置记录,来源追加成功后显式补发确认多播,否则发起端预测缓冲超时误回滚造成双端漂移
	const bool bEntryExisted = ActiveBehaviors.Contains(InBehaviorTag);

	++EnterChainDepth;
	InternalStartBehavior(InBehaviorTag, FInstancedStruct(), InSign);
	--EnterChainDepth;

	if (bEntryExisted)
	{
		MulticastBehaviorEnter(InBehaviorTag, InSign);
	}
	// 条目新建场景的多播已由BroadcastBehaviorEvent权威门收束发出(Multicast到达=预测确认)
}

bool UBXBehaviorComponent::ServerExitBehavior_Validate(FGameplayTag InBehaviorTag, int64 InSign)
{
	return true;
}

void UBXBehaviorComponent::ServerExitBehavior_Implementation(FGameplayTag InBehaviorTag, int64 InSign)
{
	// 发起方校验:仅允许退出Client签名来源(Server/系统Sign=SkillID/0常驻等来源的生命周期归权威管线,
	// 由技能收束/矩阵挤出/状态挂起驱动;客户端伪造上报一律忽略)
	if (BXGetSyncIDInitiator(InSign) != EBXSyncInitiator::Client || InSign == 0)
	{
		UE_LOG(BXBEHAVIOR, Warning, TEXT("UBXBehaviorComponent::ServerExitBehavior: invalid initiator rejected. Tag=%s Sign=%lld"), *InBehaviorTag.ToString(), InSign);
		return;
	}

	// 幂等:查无即忽略;上报不可自定Reason,固定Manual语义由服务器裁决
	InternalStopBehavior(InBehaviorTag, FInstancedStruct(), InSign, EBXBehaviorEndReason::BER_Manual);
}

void UBXBehaviorComponent::ClientRejectBehavior_Implementation(FGameplayTag InBehaviorTag, int64 InSign)
{
	// 回滚=本地移除该来源(不含表现);移除缓冲在HandleClientBehaviorExit路径同型处理,此处显式双保险
	UnregisterPredictedBehavior(InBehaviorTag, InSign);
	InternalStopBehavior(InBehaviorTag, FInstancedStruct(), InSign, EBXBehaviorEndReason::BER_PredictRollback);
}

void UBXBehaviorComponent::MulticastBehaviorEnter_Implementation(FGameplayTag InBehaviorTag, int64 InSign)
{
	AActor* Owner = GetOwner();
	if (!Owner || Owner->GetLocalRole() == ENetRole::ROLE_Authority)
	{
		return;
	}

	HandleClientBehaviorEnter(InBehaviorTag, InSign);
}

void UBXBehaviorComponent::MulticastBehaviorExit_Implementation(FGameplayTag InBehaviorTag, int64 InSign, uint8 InReason)
{
	AActor* Owner = GetOwner();
	if (!Owner || Owner->GetLocalRole() == ENetRole::ROLE_Authority)
	{
		return;
	}

	// 挂起事件流由控制包按Tag粒度镜像(精确对齐服务器Agent单次停转),此处防意外混入造成Agent双停
	const EBXBehaviorEndReason Reason = static_cast<EBXBehaviorEndReason>(InReason);
	if (Reason == EBXBehaviorEndReason::BER_Suspended || Reason == EBXBehaviorEndReason::BER_Resumed)
	{
		return;
	}

	HandleClientBehaviorExit(InBehaviorTag, InSign, Reason);
}

void UBXBehaviorComponent::MulticastControlBehavior_Implementation(FGameplayTag InBehaviorTag, uint8 InOp)
{
	AActor* Owner = GetOwner();
	if (!Owner || Owner->GetLocalRole() == ENetRole::ROLE_Authority)
	{
		return;
	}

	HandleClientControlBehavior(InBehaviorTag, InOp == BX_NET_BEHAVIOR_OP_RESUME);
}

#pragma endregion RPC BehaviorSync




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

	// 权威端同步多播(已有连接行为动态的主通道;挂起/恢复事件流由控制包按Tag粒度镜像防Agent双停双启,
	// 清场随Actor销毁广播无意义;回滚仅发生于客户端,权威门自动排除)
	AActor* Owner = GetOwner();
	if (Owner && Owner->GetLocalRole() == ENetRole::ROLE_Authority
		&& InReason != EBXBehaviorEndReason::BER_Suspended
		&& InReason != EBXBehaviorEndReason::BER_Resumed
		&& InReason != EBXBehaviorEndReason::BER_Cleared)
	{
		if (bEnter)
		{
			MulticastBehaviorEnter(InBehaviorTag, InSign);
		}
		else
		{
			MulticastBehaviorExit(InBehaviorTag, InSign, (uint8)InReason);
		}
	}
}

#pragma endregion Internal



#pragma region Internal Net
void UBXBehaviorComponent::RegisterPredictedBehavior(const FGameplayTag& InBehaviorTag, int64 InSign)
{
	// 去重(同Tag同Sign重复登记无意义)
	for (const FBXPredictedBehavior& Entry : PredictedBehaviors)
	{
		if (Entry.Tag == InBehaviorTag && Entry.Sign == InSign)
		{
			return;
		}
	}

	if (PredictedBehaviors.Num() >= BX_NET_PREDICT_MAX)
	{
		UE_LOG(BXBEHAVIOR, Warning, TEXT("UBXBehaviorComponent::RegisterPredictedBehavior: buffer full (%d). Tag=%s Sign=%lld loses auto-rollback protection."), PredictedBehaviors.Num(), *InBehaviorTag.ToString(), InSign);
		return;
	}

	FBXPredictedBehavior Entry;
	Entry.Tag = InBehaviorTag;
	Entry.Sign = InSign;
	Entry.ElapsedTime = 0.0f;
	PredictedBehaviors.Add(Entry);
}

bool UBXBehaviorComponent::UnregisterPredictedBehavior(const FGameplayTag& InBehaviorTag, int64 InSign)
{
	const int32 RemovedNum = PredictedBehaviors.RemoveAll([InBehaviorTag, InSign](const FBXPredictedBehavior& Entry) { return Entry.Tag == InBehaviorTag && Entry.Sign == InSign; });
	return RemovedNum > 0;
}

void UBXBehaviorComponent::UpdatePredictedBehaviorTimeouts(float InDeltaTime)
{
	const float MaxDuration = GetDefault<UBXSettings>()->BehaviorPredictMaxDuration;

	// 快照收集后逐条结算(TArray索引式遍历,移除自当前推进位安全;回调可能同步增删缓冲)
	for (int32 i = 0; i < PredictedBehaviors.Num(); )
	{
		PredictedBehaviors[i].ElapsedTime += InDeltaTime;
		if (PredictedBehaviors[i].ElapsedTime < MaxDuration)
		{
			++i;
			continue;
		}

		const FBXPredictedBehavior Entry = PredictedBehaviors[i];
		PredictedBehaviors.RemoveAt(i);

		UE_LOG(BXBEHAVIOR, Log, TEXT("UBXBehaviorComponent::UpdatePredictedBehaviorTimeouts: rollback. Tag=%s Sign=%lld"), *Entry.Tag.ToString(), Entry.Sign);
		InternalStopBehavior(Entry.Tag, FInstancedStruct(), Entry.Sign, EBXBehaviorEndReason::BER_PredictRollback);
	}
}

void UBXBehaviorComponent::RebuildRunningBehaviorStates()
{
	RunningBehaviorStates.Reset();

	for (const TPair<FGameplayTag, FBXBehaviorRuntimeData>& Pair : ActiveBehaviors)
	{
		FBXBehaviorReplicatedState State;
		State.BehaviorTag = Pair.Key;

		uint8 Flags = 0;
		for (const FBXBehaviorSource& Source : Pair.Value.Sources)
		{
			State.Signs.Add(Source.Sign);
		}

		// 挂起条目一并投影(LateJoin重建时仅建事实表不停转——Agent从未启动,等待控制包恢复)
		if (IsBehaviorSuspended(Pair.Key))
		{
			Flags |= BX_SYNC_FLAG_BEHAVIOR_SUSPENDED;
		}

		State.Flags = Flags;
		RunningBehaviorStates.Add(State);
	}
}

void UBXBehaviorComponent::OnRep_RunningBehaviorStates(TArray<FBXBehaviorReplicatedState> InOldStates)
{
	AActor* Owner = GetOwner();
	if (!Owner || Owner->GetLocalRole() == ENetRole::ROLE_Authority)
	{
		return;
	}

	// 新增条目:LateJoin重建(已存在Tag在重建内部幂等跳过)
	for (const FBXBehaviorReplicatedState& State : RunningBehaviorStates)
	{
		bool bExisted = false;
		for (const FBXBehaviorReplicatedState& OldState : InOldStates)
		{
			if (OldState.BehaviorTag == State.BehaviorTag)
			{
				bExisted = true;
				break;
			}
		}

		if (!bExisted)
		{
			RebuildBehaviorFromState(State);
		}
	}

	// 消失条目:兜底清理(主通道MulticastBehaviorExit,仅处理RPC与属性乱序竞态;静默移除不广播)
	for (const FBXBehaviorReplicatedState& OldState : InOldStates)
	{
		bool bStillExists = false;
		for (const FBXBehaviorReplicatedState& State : RunningBehaviorStates)
		{
			if (State.BehaviorTag == OldState.BehaviorTag)
			{
				bStillExists = true;
				break;
			}
		}

		if (!bStillExists && ActiveBehaviors.Contains(OldState.BehaviorTag))
		{
			UE_LOG(BXBEHAVIOR, Log, TEXT("UBXBehaviorComponent::OnRep_RunningBehaviorStates: entry vanished, fallback stop. Tag=%s"), *OldState.BehaviorTag.ToString());

			// Agent停转+静默摘表(乱序竞态防御路径,不走管线防误广播)
			if (const TObjectPtr<UBXBehaviorAgent>* FindResult = BehaviorAgents.Find(OldState.BehaviorTag))
			{
				UBXBehaviorAgent* Agent = *FindResult;
				if (IsValid(Agent))
				{
					FInstancedStruct EmptyParam;
					Agent->StopBehavior(EmptyParam);
				}
			}
			ActiveBehaviors.Remove(OldState.BehaviorTag);
		}
	}
}

void UBXBehaviorComponent::HandleClientBehaviorEnter(const FGameplayTag& InBehaviorTag, int64 InSign)
{
	// 幂等:已存在来源(重复到达)与预测确认合一处理;先注销缓冲再短路
	UnregisterPredictedBehavior(InBehaviorTag, InSign);

	if (const FBXBehaviorRuntimeData* Data = ActiveBehaviors.Find(InBehaviorTag))
	{
		if (Data->HasSource(InSign))
		{
			return;
		}
	}

	// 新建事实条目(SimulatedProxy跟随;启动参数为空,Agent从基层组件现场取参)
	FBXBehaviorRuntimeData& Data = ActiveBehaviors.FindOrAdd(InBehaviorTag);
	Data.Tag = InBehaviorTag;
	if (!Data.HasSource(InSign))
	{
		Data.Sources.Add(FBXBehaviorSource(InSign));
	}

	// Agent启动+本地事件(表现层各端本地运行;权威门收束点非权威端不再转发多播)
	if (const TObjectPtr<UBXBehaviorAgent>* FindResult = BehaviorAgents.Find(InBehaviorTag))
	{
		UBXBehaviorAgent* Agent = *FindResult;
		if (IsValid(Agent))
		{
			FInstancedStruct EmptyParam;
			Agent->StartBehavior(EmptyParam);
		}
	}
	BroadcastBehaviorEvent(true, InBehaviorTag, InSign, EBXBehaviorEndReason::BER_TMax);
}

void UBXBehaviorComponent::HandleClientBehaviorExit(const FGameplayTag& InBehaviorTag, int64 InSign, EBXBehaviorEndReason InReason)
{
	// 查无该来源(RPC早于快照或重复到达):仅做预测确认注销后返回
	UnregisterPredictedBehavior(InBehaviorTag, InSign);
	const FBXBehaviorRuntimeData* Data = ActiveBehaviors.Find(InBehaviorTag);
	if (!Data || !Data->HasSource(InSign))
	{
		return;
	}

	// 跟随退出走同一管线(条目移除+Agent停+本地事件;管线内挂起原因不转发多播的门控对非权威端天然无效)
	InternalStopBehavior(InBehaviorTag, FInstancedStruct(), InSign, InReason);
}

void UBXBehaviorComponent::HandleClientControlBehavior(const FGameplayTag& InBehaviorTag, bool bResume)
{
	FBXBehaviorRuntimeData* Data = ActiveBehaviors.Find(InBehaviorTag);
	if (!Data)
	{
		UE_LOG(BXBEHAVIOR, Warning, TEXT("UBXBehaviorComponent::HandleClientControlBehavior: entry missing. Tag=%s Op=%d"), *InBehaviorTag.ToString(), bResume ? 1 : 0);
		return;
	}

	// 快照参数与来源(Agent回调可能同步修改条目,指针不可跨回调解引用)
	const FInstancedStruct ResumeParameter = Data->LastStartParameter;
	const TArray<FBXBehaviorSource> Sources = Data->Sources;

	// Tag粒度精确镜像服务器Agent操作:挂起=单次停转,恢复=单次以最近启动参数重启(来源所有权不变)
	if (const TObjectPtr<UBXBehaviorAgent>* FindResult = BehaviorAgents.Find(InBehaviorTag))
	{
		UBXBehaviorAgent* Agent = *FindResult;
		if (IsValid(Agent))
		{
			FInstancedStruct EmptyParam;
			if (bResume)
			{
				Agent->StartBehavior(ResumeParameter);
			}
			else
			{
				Agent->StopBehavior(EmptyParam);
			}
		}
	}

	// 每来源本地事件流(表现层语义与服务器端SuspendByForbiddenTag/ResumeByForbiddenTag一致)
	for (const FBXBehaviorSource& Source : Sources)
	{
		BroadcastBehaviorEvent(bResume, InBehaviorTag, Source.Sign, bResume ? EBXBehaviorEndReason::BER_Resumed : EBXBehaviorEndReason::BER_Suspended);
	}
}

void UBXBehaviorComponent::RebuildBehaviorFromState(const FBXBehaviorReplicatedState& InState)
{
	// Late Join静默重建(仅事实表+Agent启动;不发事件不触表现——时机与监听者就绪次序不定,
	// 与技能侧RebuildSkillFromProjection一致;下一个启停/转移事件自然接管表现)
	if (ActiveBehaviors.Contains(InState.BehaviorTag))
	{
		return;
	}

	FBXBehaviorRuntimeData& Data = ActiveBehaviors.Add(InState.BehaviorTag);
	Data.Tag = InState.BehaviorTag;
	for (int64 Sign : InState.Signs)
	{
		Data.Sources.Add(FBXBehaviorSource(Sign));
	}

	// 挂起条目不启Agent(等待控制包恢复);标志缺省视为活跃直接重建
	if (!(InState.Flags & BX_SYNC_FLAG_BEHAVIOR_SUSPENDED))
	{
		if (const TObjectPtr<UBXBehaviorAgent>* FindResult = BehaviorAgents.Find(InState.BehaviorTag))
		{
			UBXBehaviorAgent* Agent = *FindResult;
			if (IsValid(Agent))
			{
				FInstancedStruct EmptyParam;
				Agent->StartBehavior(EmptyParam);
			}
		}
	}
}

#pragma endregion Internal Net
