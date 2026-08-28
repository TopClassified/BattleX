#pragma once

#include "CoreMinimal.h"
#include "StructUtils/InstancedStruct.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"

#include "BXBehaviorEnums.h"
#include "BXBehaviorStructs.h"
#include "BehaviorProxy/BXBehaviorProxy.h"
#include "Net/BXStateBehaviorReplicated.h"

#include "BXBehaviorComponent.generated.h"


// 行为系统组件(唯一事实表+关系矩阵裁决+取消窗口保护+状态禁用挂起)
// 时序约定(技能五步链):判定(CanStartBehavior)→技能确立→本组件执行中断与登记
UCLASS(ClassGroup = "BattleX", meta = (BlueprintSpawnableComponent))
class BATTLEX_API UBXBehaviorComponent : public UActorComponent
{
	GENERATED_BODY()

#pragma region Important
public:
	UBXBehaviorComponent();

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// 服务器端、属性收集前每帧调用:远程连接数增加(新客户端连入)时从ActiveBehaviors重建快照
	// (配合COND_InitialOnly仅初始同步发送,已有连接零属性流量)
	virtual void PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker) override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// 行为代理配置(Tag→代理类+是否默认启用;常驻型门控代理默认启用,事件型随管线隐式启停)
	UPROPERTY(EditDefaultsOnly, Category = "Behavior")
	TMap<FGameplayTag, FBXBehaviorProxyConfig> BehaviorProxyConfigs;

	// 行为代理实例(BeginPlay预建,组件生命周期内复用)
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Behavior")
	TMap<FGameplayTag, TObjectPtr<UBXBehaviorProxy>> BehaviorProxies;

	// 代理门控状态(每代理:挂起位+最后命令值;拒绝位求值即算不落盘)
	struct FBXProxyGateState
	{
		// 挂起位(服务器遮蔽翻转置清/客户端控制包直接置清;常驻门控代理无活跃条目时同样生效)
		bool bSuspendBit = false;

		// 最后命令值(差分防重复命令)
		bool bCommandedEnabled = false;
	};
	TMap<FGameplayTag, FBXProxyGateState> ProxyGateStates;

	// 正在执行的行为(唯一事实表)
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Behavior")
	TMap<FGameplayTag, FBXBehaviorRuntimeData> ActiveBehaviors;

	// 取消窗口保护记录(技能驱动)
	TMap<FGameplayTag, TArray<FBXProtectionRecord>> ProtectionEntries;

	// 挂起遮蔽表(状态禁用通道:key=状态禁用Tag,行为条目不移表,仅代理门控命令+查询遮蔽)
	TMap<FGameplayTag, FBXSuspendMask> SuspendMasks;

	// 激活链深度守卫(防进入链环)
	int32 EnterChainDepth = 0;

	// 运行中行为的复制快照(COND_InitialOnly:新复制到客户端的对象初始同步时LateJoin重建用,
	// 已有连接的行为动态由显式RPC维护,详见BXStateBehaviorReplicated.h文件头注释)
	UPROPERTY(ReplicatedUsing = OnRep_RunningBehaviorStates)
	TArray<FBXBehaviorReplicatedState> RunningBehaviorStates;

	// 上次投影时的远程连接数(-1保证组件首个复制周期必建一次基线快照;连接数增加才重建快照)
	int32 LastProjectedConnectionCount = -1;

	// 预测缓冲条目(AutonomousProxy:本地已执行待服务器确认/拒绝/超时)
	// 注意:类内嵌套类型须先声明后用,故定义在成员声明之前(C++成员声明不是完整类上下文)
	struct FBXPredictedBehavior
	{
		FGameplayTag Tag;
		int64 Sign = 0;
		float ElapsedTime = 0.0f;
	};

	// 预测缓冲(AutonomousProxy本地已执行待服务器结算;权威端恒空)
	TArray<FBXPredictedBehavior> PredictedBehaviors;

#pragma endregion Important



#pragma region API
public:
	// 查询行为是否活跃(族Tag语义:精确命中或任一族成员活跃)
	UFUNCTION(BlueprintCallable, Category = "BattleX|Behavior")
	bool CheckActiveBehavior(const FGameplayTag& InBehaviorTag) const;

	// 获取全部活跃行为
	UFUNCTION(BlueprintCallable, Category = "BattleX|Behavior")
	void GetActiveBehaviors(FGameplayTagContainer& OutBehaviors) const;

	// 只读判定:行为能否开始(挂起/代理权限/拒绝关系/代理检查/挤出目标保护,无副作用;代理检查用空参数)
	UFUNCTION(BlueprintCallable, Category = "BattleX|Behavior")
	bool CanStartBehavior(const FGameplayTag& InBehaviorTag, FBXBehaviorStartCheck& OutCheck) const;

	// 开始行为(Sign=技能SkillID/时间轴TLID/0系统常驻)
	UFUNCTION(BlueprintCallable, Category = "BattleX|Behavior")
	bool StartBehavior(const FGameplayTag& InBehaviorTag, int64 InSign = 0);

	// 开始行为(有参数版本;按值收参+内部MoveTemp:蓝图调用方数据不受影响,C++可MoveTemp传入,LastStartParameter存储零额外拷贝)
	UFUNCTION(BlueprintCallable, Category = "BattleX|Behavior")
	bool StartBehaviorWithParameter(const FGameplayTag& InBehaviorTag, FInstancedStruct InParameter, int64 InSign = 0);

	// 停止行为(仅移除该Sign来源,先停先退)
	UFUNCTION(BlueprintCallable, Category = "BattleX|Behavior")
	bool StopBehavior(const FGameplayTag& InBehaviorTag, int64 InSign = 0);

	// 停止行为(有参数版本)
	UFUNCTION(BlueprintCallable, Category = "BattleX|Behavior")
	bool StopBehaviorWithParameter(const FGameplayTag& InBehaviorTag, const FInstancedStruct& InParameter, int64 InSign = 0);

	// 停止行为(全部来源)
	UFUNCTION(BlueprintCallable, Category = "BattleX|Behavior")
	bool StopBehaviorAllSources(const FGameplayTag& InBehaviorTag, EBXBehaviorEndReason InReason = EBXBehaviorEndReason::BER_Manual);

	// 技能预清场:挤出与进入行为互斥的全部活跃行为(技能确立后、首帧Task前调用;不含自身Tag)
	UFUNCTION(BlueprintCallable, Category = "BattleX|Behavior")
	bool InterruptBehaviorsConflicting(const FGameplayTag& InBehaviorTag);

	// 查询行为是否受取消窗口保护
	UFUNCTION(BlueprintCallable, Category = "BattleX|Behavior")
	bool IsBehaviorProtected(const FGameplayTag& InBehaviorTag) const;

	// 设置取消窗口保护(技能开始置true,窗口边界切换,技能结束移除)
	void SetBehaviorProtection(const FGameplayTag& InBehaviorTag, int64 InSign, bool bProtected);

	// 移除某来源的全部保护记录(技能结束收束)
	void RemoveProtectionBySign(int64 InSign);

	// 状态禁用门控:遮蔽生效(状态进入时调用;覆盖代理置挂起位+门控命令+活跃条目Exit事件,瞬间/未激活行为被CanStart拦截)
	void SuspendByForbiddenTag(const FGameplayTag& InForbiddenTag, const FGameplayTag& InByState);

	// 状态禁用门控:解除遮蔽(状态退出时调用;被其他活跃遮蔽键覆盖的代理保持挂起)
	void ResumeByForbiddenTag(const FGameplayTag& InForbiddenTag, const FGameplayTag& InByState);

	// 查询行为是否被状态挂起(任一活跃遮蔽键为该Tag的祖先或自身)
	UFUNCTION(BlueprintCallable, Category = "BattleX|Behavior")
	bool IsBehaviorSuspended(const FGameplayTag& InBehaviorTag) const;

	// 查询行为是否被禁止(挂起中或被拒关系挡住)
	UFUNCTION(BlueprintCallable, Category = "BattleX|Behavior")
	bool CheckForbiddenBehavior(const FGameplayTag& InBehaviorTag) const;

#pragma endregion API



#pragma region API Net
public:
	// 网络入口:开始行为(显式网络分流——权威端直接执行;AutonomousProxy本地执行+预测缓冲+ServerEnterBehavior上报,
	// 超时/拒绝回滚;SimulatedProxy拒绝。InSign非Client签名时自动生成ClientSyncID并返回生效Sign,失败返回-1。
	// 启动参数不上传RPC,Proxy需从基层组件现场取参);
		// 技能链路/CMC高频路径使用本地API(StartBehavior系列),勿切此入口
	UFUNCTION(BlueprintCallable, Category = "BattleX|Behavior")
	int64 StartBehaviorNet(const FGameplayTag& InBehaviorTag, FInstancedStruct InParameter, int64 InSign = 0);

	// 网络入口:停止行为(仅允许退出Client签名来源——本入口Start返回的生效Sign;Server/系统Sign归权威管线,双端均拒绝)
	UFUNCTION(BlueprintCallable, Category = "BattleX|Behavior")
	bool StopBehaviorNet(const FGameplayTag& InBehaviorTag, int64 InSign);

#pragma endregion API Net


#pragma region RPC BehaviorSync
public:
	// 客户端请求开始行为(AutonomousProxy预测通道;技能链路Sign=SkillID随技能预测携带不走此RPC)
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerEnterBehavior(FGameplayTag InBehaviorTag, int64 InSign, int64 InClientTimestamp);

	// 客户端请求退出行为
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerExitBehavior(FGameplayTag InBehaviorTag, int64 InSign);

	// 请求被拒回滚通知(发起端收到即本地回滚)
	UFUNCTION(Client, Reliable)
	void ClientRejectBehavior(FGameplayTag InBehaviorTag, int64 InSign);

	// 广播行为进入(Reliable:事实动态主通道,来源级新增同样到达;多播匹配即预测确认)。挂起恢复不在此通道(走控制包)
	UFUNCTION(NetMulticast, Reliable)
	void MulticastBehaviorEnter(FGameplayTag InBehaviorTag, int64 InSign);

	// 广播行为退出(InReason透传Suspended除外:挂起恢复走控制包按Tag粒度精确镜像服务器代理操作)
	UFUNCTION(NetMulticast, Reliable)
	void MulticastBehaviorExit(FGameplayTag InBehaviorTag, int64 InSign, uint8 InReason);

	// 行为控制包(代理粒度):Op=0挂起(置挂起位+禁用代理+每来源本地Exit事件)/1恢复(清挂起位+启用代理+每来源本地Resumed事件);
	// 常驻门控代理无活跃条目时同样生效(客户端挂起事实的唯一来源)
	UFUNCTION(NetMulticast, Reliable)
	void MulticastControlBehavior(FGameplayTag InBehaviorTag, uint8 InOp);

#pragma endregion RPC BehaviorSync


#pragma region Internal
protected:
	// 只读判定内部实现(带启动参数:参数依赖型代理用真实参数判定)
	bool CanStartBehaviorInternal(const FGameplayTag& InBehaviorTag, const FInstancedStruct& InParameter, FBXBehaviorStartCheck& OutCheck) const;

	// 内部开始行为(管线执行:代理隐式启用/表更新/事件;参数经移动语义存入LastStartParameter,代理在移动前使用)
	bool InternalStartBehavior(const FGameplayTag& InBehaviorTag, FInstancedStruct&& InParameter, int64 InSign);

	// 内部停止行为(管线执行:表更新/代理/事件,全部停止路径收束于此)
	bool InternalStopBehavior(const FGameplayTag& InBehaviorTag, const FInstancedStruct& InParameter, int64 InSign, EBXBehaviorEndReason InReason);

	// 收集活跃行为Tag快照(遍历前收集,回调中增删安全)
	void CollectActiveBehaviorTags(TArray<FGameplayTag>& OutTags) const;

	// 广播行为事件(本端表达层唯一收束点;权威端经此同步多播,挂起/恢复/清场除外——挂起恢复走控制包,清场不广播)
	void BroadcastBehaviorEvent(bool bEnter, const FGameplayTag& InBehaviorTag, int64 InSign, EBXBehaviorEndReason InReason);

	// 查询代理挂起位
	bool IsProxyTagSuspended(const FGameplayTag& InProxyTag) const;

	// 置/清代理挂起位并刷新命令(服务器遮蔽翻转与客户端控制包共用入口;内部含门控差分)
	void SetProxySuspendBit(const FGameplayTag& InProxyTag, bool bSuspended);

	// 代理门控求值与差分命令(目标=非挂起 且(常驻型:非拒绝/事件型:条目存在);表/遮蔽/位变更收尾统一调用)
	void RefreshProxyGates();

	// 代理启用+开始(管线开始/跟随进入/LateJoin重建共用;事件型隐式启用,已启用时不重复Enable)
	bool EnableAndStartProxy(const FGameplayTag& InProxyTag, const FInstancedStruct& InParameter);

#pragma endregion Internal



#pragma region Internal Net
protected:
	// 登记预测条目(去重;超过上限仅告警不再登记——本地执行与上报不受影响,失去自动回滚保护)
	void RegisterPredictedBehavior(const FGameplayTag& InBehaviorTag, int64 InSign);

	// 注销匹配预测条目(服务器确认/主动退出时调用,返回是否命中)
	bool UnregisterPredictedBehavior(const FGameplayTag& InBehaviorTag, int64 InSign);

	// 预测超时扫描(Tick末尾客户端调用;快照收集后逐条回滚,回调中增删缓冲安全)
	void UpdatePredictedBehaviorTimeouts(float InDeltaTime);

	// 从ActiveBehaviors重建RunningBehaviorStates快照(新客户端连入时PreReplication调用;含挂起条目,投影bSuspended标志)
	void RebuildRunningBehaviorStates();

	// 多播接收:跟随进入(新建事实条目+代理隐式启用启动+本地事件+视为预测确认;权威端早退,重复Sign幂等)
	void HandleClientBehaviorEnter(const FGameplayTag& InBehaviorTag, int64 InSign);

	// 多播接收:跟随退出(Suspended原因同控制包语义早退防双停;移除匹配预测条目=退出确认)
	void HandleClientBehaviorExit(const FGameplayTag& InBehaviorTag, int64 InSign, EBXBehaviorEndReason InReason);

	// 控制包接收:置/清挂起位(客户端挂起事实的唯一来源,直控代理门控命令)+活跃条目每来源本地事件流
	void HandleClientControlBehavior(const FGameplayTag& InBehaviorTag, bool bResume);

	// Late Join重建单个行为(OnRep新增条目;bSuspended=true仅建表+置挂起位禁用代理——代理从未启动,等待控制包恢复)
	void RebuildBehaviorFromState(const FBXBehaviorReplicatedState& InState);

protected:
	// 复制快照OnRep(COND_InitialOnly仅新连接初始同步;带旧值差分:新增条目LateJoin重建,消失条目兜底清理)
	UFUNCTION()
	void OnRep_RunningBehaviorStates(TArray<FBXBehaviorReplicatedState> InOldStates);

#pragma endregion Internal Net
};
