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

	// 代理帧更新转发(已启用且声明需要的代理;代理容器BeginPlay预建后只读,回调增删行为表不影响遍历)
	for (const TPair<FGameplayTag, TObjectPtr<UBXBehaviorProxy>>& Pair : BehaviorProxies)
	{
		UBXBehaviorProxy* Proxy = Pair.Value;
		if (IsValid(Proxy) && Proxy->WantsProxyUpdate() && Proxy->IsEnabled())
		{
			Proxy->UpdateProxy(DeltaTime);
		}
	}
}

void UBXBehaviorComponent::BeginPlay()
{
	// 初始化行为代理实例
	for (const TPair<FGameplayTag, FBXBehaviorProxyConfig>& Pair : BehaviorProxyConfigs)
	{
		if (!IsValid(Pair.Value.ProxyClass))
		{
			continue;
		}

		UBXBehaviorProxy* NewProxy = NewObject<UBXBehaviorProxy>(this, Pair.Value.ProxyClass);
		if (!IsValid(NewProxy))
		{
			continue;
		}

		NewProxy->Initialize();
		BehaviorProxies.Add(Pair.Key, NewProxy);  // 代理出生即启用(bEnabled=true),禁用只由禁止原子造成
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
	for (const TPair<FGameplayTag, TObjectPtr<UBXBehaviorProxy>>& Pair : BehaviorProxies)
	{
		if (!IsValid(Pair.Value))
		{
			continue;
		}

		Pair.Value->Deinitialize();
		Pair.Value->MarkAsGarbage();
	}
	BehaviorProxies.Empty();

	ForbidLedger.Empty();
	BehaviorWaivers.Empty();
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

	// 挤出接管不受豁免影响,无条件执行:恒受理(bResult保留bool供扩展)
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

bool UBXBehaviorComponent::IsBehaviorWaived(const FGameplayTag& InSubjectTag) const
{
	// 族匹配:豁免键为祖先或自身即覆盖(族域豁免覆盖族成员)
	for (const TPair<FGameplayTag, TArray<int64>>& Pair : BehaviorWaivers)
	{
		if (InSubjectTag.MatchesTag(Pair.Key) && !Pair.Value.IsEmpty())
		{
			return true;
		}
	}

	return false;
}

void UBXBehaviorComponent::SetBehaviorWaiver(const FGameplayTag& InSubjectTag, int64 InSign, bool bWaived)
{
	if (!InSubjectTag.IsValid())
	{
		return;
	}

	TArray<int64>* Signs = BehaviorWaivers.Find(InSubjectTag);
	if (bWaived)
	{
		if (!Signs)
		{
			BehaviorWaivers.Add(InSubjectTag, { InSign });
		}
		else
		{
			Signs->AddUnique(InSign);
		}
	}
	else
	{
		// 撤销:移除该来源登记,列表空则清键(多来源叠加,最后一个移除才失效)
		if (!Signs)
		{
			return;
		}

		Signs->RemoveAll([InSign](int64 Sign) { return Sign == InSign; });
		if (Signs->IsEmpty())
		{
			BehaviorWaivers.Remove(InSubjectTag);
		}
	}

	// 收束点⑥:豁免在写入期折算——翻转后刷新匹配域的全部活跃在位方(撤回/恢复其禁止贡献)
	for (const TPair<FGameplayTag, FBXBehaviorRuntimeData>& Pair : ActiveBehaviors)
	{
		if (Pair.Key.MatchesTag(InSubjectTag))
		{
			RefreshForbidSources(Pair.Key);
		}
	}
}

void UBXBehaviorComponent::RemoveWaiversBySign(int64 InSign)
{
	// 收集发生变化的豁免域(移除键会使迭代器失效,先收集后删)
	TArray<FGameplayTag> AffectedDomains;
	TArray<FGameplayTag> EmptyKeys;
	for (TPair<FGameplayTag, TArray<int64>>& Pair : BehaviorWaivers)
	{
		const int32 Removed = Pair.Value.RemoveAll([InSign](int64 Sign) { return Sign == InSign; });
		if (Removed > 0)
		{
			AffectedDomains.AddUnique(Pair.Key);
		}

		if (Pair.Value.IsEmpty())
		{
			EmptyKeys.Add(Pair.Key);
		}
	}
	for (const FGameplayTag& Key : EmptyKeys)
	{
		BehaviorWaivers.Remove(Key);
	}

	// 收束点⑥:豁免解除后恢复匹配在位方的禁止贡献
	for (const TPair<FGameplayTag, FBXBehaviorRuntimeData>& Pair : ActiveBehaviors)
	{
		bool bAffected = false;
		for (const FGameplayTag& Domain : AffectedDomains)
		{
			if (Pair.Key.MatchesTag(Domain))
			{
				bAffected = true;
				break;
			}
		}

		if (bAffected)
		{
			RefreshForbidSources(Pair.Key);
		}
	}
}

void UBXBehaviorComponent::InterruptBehavior(const FGameplayTag& InDomainTag)
{
	if (!InDomainTag.IsValid())
	{
		return;
	}

	// 一次性动作:对域覆盖代理执行 StopBehavior(子类未实现停止槽位=无操作;不记账不恢复不挡启动)
	for (const TPair<FGameplayTag, FBXBehaviorProxyConfig>& Pair : BehaviorProxyConfigs)
	{
		if (!Pair.Key.MatchesTag(InDomainTag))
		{
			continue;
		}

		if (const TObjectPtr<UBXBehaviorProxy>* FindResult = BehaviorProxies.Find(Pair.Key))
		{
			if (IsValid(*FindResult))
			{
				FInstancedStruct EmptyParam;
				(*FindResult)->StopBehavior(EmptyParam);
			}
		}
	}

	// 活跃条目逐来源广播Exit(技能互锁监听点)
	for (const TPair<FGameplayTag, FBXBehaviorRuntimeData>& Pair : ActiveBehaviors)
	{
		if (!Pair.Key.MatchesTag(InDomainTag))
		{
			continue;
		}

		// 快照来源(事件回调可能同步移除来源或条目,指针不可跨回调解引用)
		const TArray<FBXBehaviorSource> Sources = Pair.Value.Sources;
		for (const FBXBehaviorSource& Source : Sources)
		{
			BroadcastBehaviorEvent(false, Pair.Key, Source.Sign, EBXBehaviorEndReason::BER_Interrupted);
		}
	}

	// 权威端原子重放控制包(跟随端同构执行 Stop)
	AActor* Owner = GetOwner();
	if (Owner && Owner->GetLocalRole() == ENetRole::ROLE_Authority)
	{
		MulticastInterruptBehavior(InDomainTag);
	}
}

bool UBXBehaviorComponent::IsBehaviorDisabled(const FGameplayTag& InBehaviorTag) const
{
	// 被禁用=禁止命中(挡启动)
	return IsForbiddenByLedger(InBehaviorTag, nullptr);
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

	// 权威裁决走同一本地管线(矩阵/挂起/代理检查);失败回滚通知发起端
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

	// 挂起事件流由控制包按Tag粒度镜像(精确对齐服务器代理单次停转),此处防意外混入造成代理双停
	const EBXBehaviorEndReason Reason = static_cast<EBXBehaviorEndReason>(InReason);
	if (Reason == EBXBehaviorEndReason::BER_Interrupted)
	{
		return;
	}

	HandleClientBehaviorExit(InBehaviorTag, InSign, Reason);
}

void UBXBehaviorComponent::MulticastForbidBehavior_Implementation(FGameplayTag InDomainTag, FGameplayTag InSourceTag, int64 InSign)
{
	AActor* Owner = GetOwner();
	if (!Owner || Owner->GetLocalRole() == ENetRole::ROLE_Authority)
	{
		return;
	}

	HandleClientForbidBehavior(InDomainTag, InSourceTag, InSign);
}

void UBXBehaviorComponent::MulticastUnforbidBehavior_Implementation(FGameplayTag InDomainTag, FGameplayTag InSourceTag, int64 InSign)
{
	AActor* Owner = GetOwner();
	if (!Owner || Owner->GetLocalRole() == ENetRole::ROLE_Authority)
	{
		return;
	}

	HandleClientUnforbidBehavior(InDomainTag, InSourceTag, InSign);
}

void UBXBehaviorComponent::MulticastInterruptBehavior_Implementation(FGameplayTag InDomainTag)
{
	AActor* Owner = GetOwner();
	if (!Owner || Owner->GetLocalRole() == ENetRole::ROLE_Authority)
	{
		return;
	}

	HandleClientInterruptBehavior(InDomainTag);
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

	// 代理权限检查(端内镜像防御:被禁止原子的 DisableProxy 关掉的代理不可启动;账本为权威判据)
	if (const TObjectPtr<UBXBehaviorProxy>* ProxyResult = BehaviorProxies.Find(InBehaviorTag))
	{
		if (IsValid(*ProxyResult) && !(*ProxyResult)->IsEnabled())
		{
			OutCheck.bCanStart = false;
			OutCheck.FailReason = TEXT("ProxyDisabled");
			return false;
		}
	}

	// 禁止裁决(唯一挡启动判据)
	{
		FGameplayTag ForbiddenBy;
		if (IsForbiddenByLedger(InBehaviorTag, &ForbiddenBy))
		{
			OutCheck.bCanStart = false;
			OutCheck.FailReason = FString::Printf(TEXT("ForbiddenBy:%s"), *ForbiddenBy.ToString());
			return false;
		}
	}

	// 代理检查(纯查询无副作用;代理持非const指针:CheckStartBehavior为UFUNCTION含蓝图事件调用,无法声明const)
	if (const TObjectPtr<UBXBehaviorProxy>* FindResult = BehaviorProxies.Find(InBehaviorTag))
	{
		UBXBehaviorProxy* Proxy = *FindResult;
		if (!IsValid(Proxy))
		{
			OutCheck.bCanStart = false;
			OutCheck.FailReason = TEXT("ProxyInvalid");
			return false;
		}

		if (!Proxy->CheckStartBehavior(InParameter))
		{
			OutCheck.bCanStart = false;
			OutCheck.FailReason = TEXT("ProxyCheckFailed");
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

	// 代理执行(重复Start=重启语义;具名右值引用作左值使用,参数此时未被移动)
	if (const TObjectPtr<UBXBehaviorProxy>* FindResult = BehaviorProxies.Find(InBehaviorTag))
	{
		UBXBehaviorProxy* Proxy = *FindResult;
		if (!IsValid(Proxy))
		{
			UE_LOG(BXBEHAVIOR, Warning, TEXT("UBXBehaviorComponent::InternalStartBehavior: proxy invalid. Tag=%s"), *InBehaviorTag.ToString());
			return false;
		}

		if (!Proxy->StartBehavior(InParameter))
		{
			UE_LOG(BXBEHAVIOR, Warning, TEXT("UBXBehaviorComponent::InternalStartBehavior: proxy start failed. Tag=%s"), *InBehaviorTag.ToString());
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

	// 条目诞生边沿 → 禁止贡献登记(矩阵禁止列;豁免写入期折算)
	if (bNewEntry)
	{
		RefreshForbidSources(InBehaviorTag);
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

	// 代理停止
	if (const TObjectPtr<UBXBehaviorProxy>* FindResult = BehaviorProxies.Find(InBehaviorTag))
	{
		UBXBehaviorProxy* Proxy = *FindResult;
		if (IsValid(Proxy))
		{
			Proxy->StopBehavior(InParameter);
		}
	}

	// 移除条目
	ActiveBehaviors.Remove(InBehaviorTag);

	// 条目死亡边沿 → 禁止贡献注销(在位方不再禁止任何域)
	RefreshForbidSources(InBehaviorTag);

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

	// 权威端同步多播(已有连接行为动态的主通道;中断/恢复事件流由控制包按Tag粒度镜像防代理双停双启,
	// 清场随Actor销毁广播无意义;回滚仅发生于客户端,权威门自动排除)
	AActor* Owner = GetOwner();
	if (Owner && Owner->GetLocalRole() == ENetRole::ROLE_Authority
		&& InReason != EBXBehaviorEndReason::BER_Interrupted
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

bool UBXBehaviorComponent::IsForbiddenByLedger(const FGameplayTag& InBehaviorTag, FGameplayTag* OutBy) const
{
	// 遍历账本键,族匹配命中即禁止(唯一挡启动判据;豁免已在写入期折算,读路径零豁免依赖)
	for (const TPair<FGameplayTag, TArray<FBXBehaviorForbidSource>>& Pair : ForbidLedger)
	{
		if (InBehaviorTag.MatchesTag(Pair.Key))
		{
			if (OutBy)
			{
				*OutBy = Pair.Key;
			}
			return true;
		}
	}

	return false;
}

void UBXBehaviorComponent::ForbidBehavior(const FGameplayTag& InDomainTag, const FGameplayTag& InSourceTag, int64 InSign)
{
	if (!InDomainTag.IsValid() || !InSourceTag.IsValid())
	{
		return;
	}

	// 账本登记(持续禁令,来源解除才失效)
	TArray<FBXBehaviorForbidSource>* Sources = ForbidLedger.Find(InDomainTag);
	FBXBehaviorForbidSource NewSource(InSourceTag, InSign);
	if (!Sources)
	{
		ForbidLedger.Add(InDomainTag, { NewSource });
	}
	else if (!Sources->Contains(NewSource))
	{
		Sources->Add(NewSource);
	}
	else
	{
		return;
	}

	// 禁止原子直调代理:域覆盖代理执行 DisableProxy(子类未实现Disable槽位=纯记账)
	for (const TPair<FGameplayTag, FBXBehaviorProxyConfig>& Pair : BehaviorProxyConfigs)
	{
		if (!Pair.Key.MatchesTag(InDomainTag))
		{
			continue;
		}

		if (const TObjectPtr<UBXBehaviorProxy>* FindResult = BehaviorProxies.Find(Pair.Key))
		{
			if (IsValid(*FindResult))
			{
				(*FindResult)->DisableProxy();
			}
		}
	}

	// 权威端原子重放控制包(跟随端同构执行 Forbid)
	AActor* Owner = GetOwner();
	if (Owner && Owner->GetLocalRole() == ENetRole::ROLE_Authority)
	{
		MulticastForbidBehavior(InDomainTag, InSourceTag, InSign);
	}
}

void UBXBehaviorComponent::UnforbidBehavior(const FGameplayTag& InDomainTag, const FGameplayTag& InSourceTag, int64 InSign)
{
	TArray<FBXBehaviorForbidSource>* Sources = ForbidLedger.Find(InDomainTag);
	if (!Sources)
	{
		return;
	}

	Sources->RemoveAll([InSourceTag, InSign](const FBXBehaviorForbidSource& S)
	{
		return S.SourceTag == InSourceTag && S.Sign == InSign;
	});

	// 账本键空才真解除(多来源叠加,最后一个移除才失效);解除后对域覆盖代理直调 EnableProxy
	if (!Sources->IsEmpty())
	{
		return;
	}

	ForbidLedger.Remove(InDomainTag);

	for (const TPair<FGameplayTag, FBXBehaviorProxyConfig>& Pair : BehaviorProxyConfigs)
	{
		if (!Pair.Key.MatchesTag(InDomainTag))
		{
			continue;
		}

		if (const TObjectPtr<UBXBehaviorProxy>* FindResult = BehaviorProxies.Find(Pair.Key))
		{
			if (IsValid(*FindResult))
			{
				(*FindResult)->EnableProxy();
			}
		}
	}

	// 权威端原子重放控制包(跟随端同构执行 Unforbid)
	AActor* Owner = GetOwner();
	if (Owner && Owner->GetLocalRole() == ENetRole::ROLE_Authority)
	{
		MulticastUnforbidBehavior(InDomainTag, InSourceTag, InSign);
	}
}

void UBXBehaviorComponent::UnforbidBySign(int64 InSign)
{
	// 按来源签名收束全部禁止条目(动态禁止的系统结束清理;矩阵贡献Sign=0不受影响)
	// 键空=该域禁止完全解除 → 对域覆盖代理直调 EnableProxy
	TArray<FGameplayTag> LiftedDomains;
	TArray<FGameplayTag> EmptyKeys;
	for (TPair<FGameplayTag, TArray<FBXBehaviorForbidSource>>& Pair : ForbidLedger)
	{
		Pair.Value.RemoveAll([InSign](const FBXBehaviorForbidSource& S) { return S.Sign == InSign; });

		if (Pair.Value.IsEmpty())
		{
			EmptyKeys.Add(Pair.Key);
			LiftedDomains.AddUnique(Pair.Key);
		}
	}
	for (const FGameplayTag& Key : EmptyKeys)
	{
		ForbidLedger.Remove(Key);
	}

	for (const FGameplayTag& Domain : LiftedDomains)
	{
		for (const TPair<FGameplayTag, FBXBehaviorProxyConfig>& Pair : BehaviorProxyConfigs)
		{
			if (!Pair.Key.MatchesTag(Domain))
			{
				continue;
			}

			if (const TObjectPtr<UBXBehaviorProxy>* FindResult = BehaviorProxies.Find(Pair.Key))
			{
				if (IsValid(*FindResult))
				{
					(*FindResult)->EnableProxy();
				}
			}
		}
	}
}

void UBXBehaviorComponent::RefreshForbidSources(const FGameplayTag& InIncumbentTag)
{
	if (!InIncumbentTag.IsValid())
	{
		return;
	}

	// 1. 注销该在位方的全部旧禁止贡献(按来源Tag扫账本)
	TArray<FGameplayTag> LiftedDomains;
	TArray<FGameplayTag> AddedDomains;
	TArray<FGameplayTag> EmptyKeys;
	for (TPair<FGameplayTag, TArray<FBXBehaviorForbidSource>>& Pair : ForbidLedger)
	{
		Pair.Value.RemoveAll([&InIncumbentTag](const FBXBehaviorForbidSource& S) { return S.SourceTag == InIncumbentTag; });

		if (Pair.Value.IsEmpty())
		{
			EmptyKeys.Add(Pair.Key);
			LiftedDomains.AddUnique(Pair.Key);
		}
	}
	for (const FGameplayTag& Key : EmptyKeys)
	{
		ForbidLedger.Remove(Key);
	}

	// 2. 在位且未被豁免 → 沿父链查列索引重新贡献(豁免写入期折算:被豁免的在位方不贡献)
	if (ActiveBehaviors.Contains(InIncumbentTag) && !IsBehaviorWaived(InIncumbentTag))
	{
		const UBXBehaviorSettings* Settings = GetDefault<UBXBehaviorSettings>();
		FGameplayTagContainer ForbidDomains;
		FGameplayTag Cursor = InIncumbentTag;
		while (Cursor.IsValid())
		{
			if (Settings)
			{
				if (const FGameplayTagContainer* Domains = Settings->FindForbidDomains(Cursor))
				{
					ForbidDomains.AppendTags(*Domains);
				}
			}

			Cursor = Cursor.RequestDirectParent();
		}

		for (int32 i = 0; i < ForbidDomains.Num(); ++i)
		{
			const FGameplayTag& Domain = ForbidDomains.GetByIndex(i);
			TArray<FBXBehaviorForbidSource>* Sources = ForbidLedger.Find(Domain);
			FBXBehaviorForbidSource NewSource(InIncumbentTag, 0);
			if (!Sources)
			{
				ForbidLedger.Add(Domain, { NewSource });
				AddedDomains.AddUnique(Domain);
			}
			else if (!Sources->Contains(NewSource))
			{
				Sources->Add(NewSource);
				AddedDomains.AddUnique(Domain);
			}
		}
	}

	// 差分直调代理:新禁止域 Disable、解除域 Enable(幂等)
	for (const FGameplayTag& Domain : AddedDomains)
	{
		for (const TPair<FGameplayTag, FBXBehaviorProxyConfig>& Pair : BehaviorProxyConfigs)
		{
			if (!Pair.Key.MatchesTag(Domain))
			{
				continue;
			}

			if (const TObjectPtr<UBXBehaviorProxy>* FindResult = BehaviorProxies.Find(Pair.Key))
			{
				if (IsValid(*FindResult))
				{
					(*FindResult)->DisableProxy();
				}
			}
		}
	}
	for (const FGameplayTag& Domain : LiftedDomains)
	{
		for (const TPair<FGameplayTag, FBXBehaviorProxyConfig>& Pair : BehaviorProxyConfigs)
		{
			if (!Pair.Key.MatchesTag(Domain))
			{
				continue;
			}

			if (const TObjectPtr<UBXBehaviorProxy>* FindResult = BehaviorProxies.Find(Pair.Key))
			{
				if (IsValid(*FindResult))
				{
					(*FindResult)->EnableProxy();
				}
			}
		}
	}
}

bool UBXBehaviorComponent::EnableAndStartProxy(const FGameplayTag& InProxyTag, const FInstancedStruct& InParameter)
{
	// 跟随端/LateJoin 启动路径:代理出生即启用,直接 Start
	const TObjectPtr<UBXBehaviorProxy>* FindResult = BehaviorProxies.Find(InProxyTag);
	if (!FindResult || !IsValid(*FindResult))
	{
		return false;
	}

	return (*FindResult)->StartBehavior(InParameter);
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

		// 条目在表但代理未启动(被中断停运)→ 标记(LateJoin重建时不自动Start)
		if (const TObjectPtr<UBXBehaviorProxy>* ProxyResult = BehaviorProxies.Find(Pair.Key))
		{
			if (IsValid(*ProxyResult) && !(*ProxyResult)->IsStarted())
			{
				Flags |= BX_SYNC_FLAG_BEHAVIOR_STOPPED;
			}
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

			// 代理停转+静默摘表(乱序竞态防御路径,不走管线防误广播)
			if (const TObjectPtr<UBXBehaviorProxy>* FindResult = BehaviorProxies.Find(OldState.BehaviorTag))
			{
				UBXBehaviorProxy* Proxy = *FindResult;
				if (IsValid(Proxy))
				{
					FInstancedStruct EmptyParam;
					Proxy->StopBehavior(EmptyParam);
				}
			}
			ActiveBehaviors.Remove(OldState.BehaviorTag);

			// 条目死亡边沿 → 禁止贡献注销(客户端账本镜像)
			RefreshForbidSources(OldState.BehaviorTag);
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

	// 条目诞生边沿 → 禁止贡献登记(客户端账本镜像)
	RefreshForbidSources(InBehaviorTag);

	// 代理启用+开始(事件型隐式启用)+本地事件(表现层各端本地运行;权威门收束点非权威端不再转发多播)
	EnableAndStartProxy(InBehaviorTag, FInstancedStruct());
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

	// 跟随退出走同一管线(条目移除+代理停+本地事件;管线内中断原因不转发多播的门控对非权威端天然无效)
	InternalStopBehavior(InBehaviorTag, FInstancedStruct(), InSign, InReason);
}

void UBXBehaviorComponent::HandleClientForbidBehavior(const FGameplayTag& InDomainTag, const FGameplayTag& InSourceTag, int64 InSign)
{
	// 跟随端同构执行禁止原子(账本登记+DisableProxy;客户端账本镜像,预测裁决两端一致)
	ForbidBehavior(InDomainTag, InSourceTag, InSign);
}

void UBXBehaviorComponent::HandleClientUnforbidBehavior(const FGameplayTag& InDomainTag, const FGameplayTag& InSourceTag, int64 InSign)
{
	// 跟随端同构执行解禁原子
	UnforbidBehavior(InDomainTag, InSourceTag, InSign);
}

void UBXBehaviorComponent::HandleClientInterruptBehavior(const FGameplayTag& InDomainTag)
{
	// 跟随端同构执行中断原子(一次性 Stop)
	InterruptBehavior(InDomainTag);
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

	// 停运条目(服务器上被中断):重建时不自动Start;标志缺省视为活跃直接重建
	if (InState.Flags & BX_SYNC_FLAG_BEHAVIOR_STOPPED)
	{
		// 仅建事实表+禁用贡献登记;代理保持启用未启动,后续 Start 事件自然接管
		RefreshForbidSources(InState.BehaviorTag);
	}
	else
	{
		EnableAndStartProxy(InState.BehaviorTag, FInstancedStruct());
		RefreshForbidSources(InState.BehaviorTag);
	}
}

#pragma endregion Internal Net
