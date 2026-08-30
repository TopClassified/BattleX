#pragma once

#include "CoreMinimal.h"
#include "StructUtils/InstancedStruct.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"

#include "BXStateStructs.h"
#include "StateMachine/BXStateMachineAsset.h"
#include "StateMachine/BXStateMachineInstance.h"
#include "Net/BXStateBehaviorReplicated.h"

#include "BXStateComponent.generated.h"


// 状态系统组件(唯一事实表+状态机实例管理+到期快照+禁用行为门控+表现触发)
// 服务器权威:状态机转移评估仅服务器执行,客户端经多播跟随(同步模型见StateBehaviorSystemDesign.md §5.7)
UCLASS(ClassGroup = "BattleX", meta = (BlueprintSpawnableComponent))
class BATTLEX_API UBXStateComponent : public UActorComponent
{
	GENERATED_BODY()

#pragma region Important
public:
	UBXStateComponent();

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// 服务器端、属性收集前每帧调用:远程连接数增加(新客户端连入)时从ActiveStates重建快照
	// (配合COND_InitialOnly仅初始同步发送,已有连接零属性流量)
	virtual void PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker) override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// 状态机资产列表
	UPROPERTY(EditDefaultsOnly, Category = "StateMachine")
	TArray<TObjectPtr<UBXStateMachineAsset>> StateMachineAssets;

	// 裸状态配置(非族状态:Buff/标记类)
	UPROPERTY(EditDefaultsOnly, Category = "State")
	TMap<FGameplayTag, FBXStateConfig> StateConfigs;

	// 状态机实例(BeginPlay按资产创建)
	UPROPERTY(Transient)
	TArray<TObjectPtr<UBXStateMachineInstance>> StateMachineInstances;

	// 状态→状态机实例映射(外部进入路由用)
	UPROPERTY(Transient)
	TMap<FGameplayTag, TObjectPtr<UBXStateMachineInstance>> StateToMachineMap;

	// 活跃状态(唯一事实表)
	UPROPERTY(Transient, BlueprintReadOnly, Category = "State")
	TMap<FGameplayTag, FBXStateRuntimeData> ActiveStates;

	// 进入链深度守卫
	int32 EnterChainDepth = 0;

	// 运行中状态的复制快照(COND_InitialOnly:新复制到客户端的对象初始同步时LateJoin重建用,
	// 已有连接的状态动态由显式RPC维护,详见BXStateBehaviorReplicated.h文件头注释)
	UPROPERTY(ReplicatedUsing = OnRep_RunningStateStates)
	TArray<FBXStateReplicatedState> RunningStateStates;

	// 上次投影时的远程连接数(-1保证组件首个复制周期必建一次基线快照;连接数增加才重建快照)
	int32 LastProjectedConnectionCount = -1;

	// 预测缓冲条目(AutonomousProxy:本地已执行待服务器确认/拒绝/超时)
	// 注意:类内嵌套类型须先声明后用,故定义在成员声明之前(C++成员声明不是完整类上下文)
	struct FBXPredictedState
	{
		FGameplayTag Tag;
		int64 Sign = 0;
		float ElapsedTime = 0.0f;
	};

	// 预测缓冲(AutonomousProxy本地已执行待服务器结算;权威端恒空)
	TArray<FBXPredictedState> PredictedStates;

#pragma endregion Important



#pragma region API
public:
	// 查询状态是否活跃(族Tag语义:精确命中或任一族成员活跃,与行为侧CheckActiveBehavior对称)
	UFUNCTION(BlueprintCallable, Category = "BattleX|State")
	bool CheckStateActive(const FGameplayTag& InStateTag) const;

	// 获取全部活跃状态
	UFUNCTION(BlueprintCallable, Category = "BattleX|State")
	void GetActiveStates(FGameplayTagContainer& OutStates) const;

	// 获取状态剩余时间(多来源取最小;含无限来源返回浮点最大值,无条目返回-1)
	UFUNCTION(BlueprintCallable, Category = "BattleX|State")
	float GetStateRemainingSeconds(const FGameplayTag& InStateTag) const;

	// 查询某状态是否禁用指定行为(中断∨禁止;禁用门控查询,行为组件恢复判定用)
	bool DoesStateDisableBehavior(const FGameplayTag& InStateTag, const FGameplayTag& InBehaviorTag) const;

	// 进入状态(Sign=技能SkillID/时间轴TLID/0系统;Duration≤0用节点/配置默认)
	UFUNCTION(BlueprintCallable, Category = "BattleX|State")
	bool EnterState(const FGameplayTag& InStateTag, int64 InSign = 0, float InDuration = -1.0f);

	// 退出状态(仅移除该Sign来源,先到先退;技能结束收束按FinishReason传中断/回滚语义)
	UFUNCTION(BlueprintCallable, Category = "BattleX|State")
	bool ExitState(const FGameplayTag& InStateTag, int64 InSign = 0, EBXStateEndReason InReason = EBXStateEndReason::SER_Manual);

	// 退出状态全部来源(显式退出语义;bSuppressPresentation=true抑制内置Exit表现,转移路径用)
	UFUNCTION(BlueprintCallable, Category = "BattleX|State")
	bool ExitStateAllSources(const FGameplayTag& InStateTag, EBXStateEndReason InReason = EBXStateEndReason::SER_Manual, bool bSuppressPresentation = false);

	// 刷新状态时长(同Sign存在则重置计时)
	UFUNCTION(BlueprintCallable, Category = "BattleX|State")
	bool RefreshStateDuration(const FGameplayTag& InStateTag, int64 InSign, float InDuration);

#pragma endregion API



#pragma region Internal
protected:
	// 内部进入状态(管线:族内定位/表更新/禁用门控/表现/事件;族内表现由转移边统一触发,此处仅裸状态触发配置的进入表现)
	bool InternalEnterState(const FGameplayTag& InStateTag, int64 InSign, float InDuration, EBXStateEndReason InExternalReason);

	// 内部退出状态(管线:表更新/表现/门控解除/事件,全部退出路径收束于此;
	// bSuppressPresentation=true抑制内置Exit表现,转移路径用;bDeferBehaviorRelease=延迟门控解除,转移路径用)
	bool InternalExitState(const FGameplayTag& InStateTag, int64 InSign, EBXStateEndReason InReason, bool bSuppressPresentation = false, bool bDeferBehaviorRelease = false);

	// 到期评估(Tick:快照收集→逐条处理)
	void UpdateExpiredStates(float InDeltaTime);

	// 状态机转移评估(Tick:TE_OnExpired边在到期时评估,TE_OnTick边每帧评估)
	void UpdateStateMachines(float InDeltaTime);

	// 执行状态机转移(退出当前+进入目标;旧状态禁用解除延迟到新状态登记后,共享禁用Tag经遮蔽多重登记保持无抖动)
	bool ExecuteTransition(UBXStateMachineInstance* InMachine, UBXSMStateNode* InTargetNode, EBXStateEndReason InReason);

	// 退出状态全部来源(内部版:转移路径延迟门控解除,由调用方在新状态登记后统一解除)
	bool ExitStateAllSourcesInternal(const FGameplayTag& InStateTag, EBXStateEndReason InReason, bool bSuppressPresentation, bool bDeferBehaviorRelease);

	// 触发状态表现(各端本地;入口=转移边表现/裸状态进入退出表现)
	void TriggerPresentation(const FBXStatePresentation& InPresentation, const FGameplayTag& InStateTag);

	// 禁用门控:进入时按两列表组合中断/禁止原子(唯一跨系统调用)
	void ApplyBehaviorGates(const FGameplayTagContainer& InInterruptBehaviors, const FGameplayTagContainer& InForbidBehaviors, const FGameplayTag& InByState);

	// 禁用门控:退出时按两列表解除(重入保护内置)
	void ReleaseBehaviorGates(const FGameplayTagContainer& InForbidBehaviors, const FGameplayTag& InByState);

	// 收集活跃状态Tag快照
	void CollectActiveStateTags(TArray<FGameplayTag>& OutTags) const;

	// 广播状态事件
	void BroadcastStateEvent(bool bEnter, const FGameplayTag& InStateTag, int64 InSign, float InDuration, EBXStateEndReason InReason);

	// 查询状态Tag所属状态机实例
	UBXStateMachineInstance* FindMachineByStateTag(const FGameplayTag& InStateTag) const;

	// 查询状态配置(族内节点或裸状态配置:时长+中断/禁止两列表)
	bool GetStateBehaviorConfig(const FGameplayTag& InStateTag, float& OutDuration, FGameplayTagContainer& OutInterrupt, FGameplayTagContainer& OutForbid) const;

#pragma endregion Internal



#pragma region API Net
public:
	// 网络入口:进入状态(显式网络分流——权威端直接执行;AutonomousProxy本地执行+预测缓冲+ServerEnterState上报,
	// 超时/拒绝回滚;SimulatedProxy拒绝。InSign非Client签名时自动生成ClientSyncID并返回生效Sign,失败返回-1;
	// 族内SM状态仅权威驱动,客户端请求在服务器侧拒绝);
	// 技能链路使用本地API(EnterState系列,Sign=SkillID随技能预测携带不走独立RPC),勿切此入口
	UFUNCTION(BlueprintCallable, Category = "BattleX|State")
	int64 EnterStateNet(const FGameplayTag& InStateTag, float InDuration = -1.0f);

	// 网络入口:退出状态(仅允许退出Client签名来源——EnterStateNet返回的生效Sign;Server/系统Sign归权威管线,双端均拒绝)
	UFUNCTION(BlueprintCallable, Category = "BattleX|State")
	bool ExitStateNet(const FGameplayTag& InStateTag, int64 InSign);

#pragma endregion API Net


#pragma region RPC StateSync
public:
	// 客户端请求进入状态(AutonomousProxy预测通道;技能链路Sign=SkillID随技能预测携带不走此RPC)
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerEnterState(FGameplayTag InStateTag, int64 InSign, float InDuration, int64 InClientTimestamp);

	// 客户端请求退出状态
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerExitState(FGameplayTag InStateTag, int64 InSign);

	// 请求被拒回滚通知(发起端收到即本地回滚)
	UFUNCTION(Client, Reliable)
	void ClientRejectState(FGameplayTag InStateTag, int64 InSign);

	// 广播状态进入(Reliable:事实动态主通道;多播到达即预测确认)
	UFUNCTION(NetMulticast, Reliable)
	void MulticastStateEnter(FGameplayTag InStateTag, int64 InSign, float InDuration);

	// 广播状态退出(InReason透传)
	UFUNCTION(NetMulticast, Reliable)
	void MulticastStateExit(FGameplayTag InStateTag, int64 InSign, uint8 InReason);

	// 广播状态表现触发(转移边表现/裸状态进出场表现的跟随端载体:权威端TriggerPresentation收束点发出,
	// 接收端本播——各端本地语义与单机一致,不随网络延迟缩水)
	UFUNCTION(NetMulticast, Reliable)
	void MulticastStatePresentation(FGameplayTag InStateTag, FBXStatePresentation InPresentation);

#pragma endregion RPC StateSync


#pragma region Internal Net
protected:
	// 登记预测条目(去重;超过上限仅告警不再登记——本地执行与上报不受影响,失去自动回滚保护)
	void RegisterPredictedState(const FGameplayTag& InStateTag, int64 InSign);

	// 注销匹配预测条目(服务器确认/主动退出时调用,返回是否命中)
	bool UnregisterPredictedState(const FGameplayTag& InStateTag, int64 InSign);

	// 预测超时扫描(Tick末尾客户端调用;快照收集后逐条回滚,回调中增删缓冲安全)
	void UpdatePredictedStateTimeouts(float InDeltaTime);

	// 从ActiveStates重建RunningStateStates快照(新客户端连入时PreReplication调用;来源投影为剩余时长)
	void RebuildRunningStateStates();

	// 多播接收:跟随进入(建事实条目+本地事件;门控与转移评估均不执行——权威事实镜像;
	// 转移边/裸状态表现不在此通道,由MulticastStatePresentation专用多播在各端本播)
	void HandleClientStateEnter(const FGameplayTag& InStateTag, int64 InSign, float InDuration);

	// 多播接收:跟随退出(裸状态退场表现各端本播;移除匹配预测条目=退出确认)
	void HandleClientStateExit(const FGameplayTag& InStateTag, int64 InSign, EBXStateEndReason InReason);

	// Late Join重建单个状态(OnRep新增条目;静默填表+SM CurrentNode由条目Tag反查恢复,不触发表现/事件/门控)
	void RebuildStateFromState(const FBXStateReplicatedState& InState);

protected:
	// 复制快照OnRep(COND_InitialOnly仅新连接初始同步;带旧值差分:新增条目LateJoin重建,消失条目兜底清理)
	UFUNCTION()
	void OnRep_RunningStateStates(TArray<FBXStateReplicatedState> InOldStates);

#pragma endregion Internal Net
};
