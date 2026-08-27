#pragma once

#include "CoreMinimal.h"
#include "StructUtils/InstancedStruct.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"

#include "BXBehaviorEnums.h"
#include "BXBehaviorStructs.h"
#include "BehaviorAgent/BXBehaviorAgent.h"

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

protected:
	// 行为代理类配置
	UPROPERTY(EditDefaultsOnly, Category = "Behavior")
	TMap<FGameplayTag, TSubclassOf<UBXBehaviorAgent>> BehaviorAgentConfigs;

	// 行为代理实例(BeginPlay预建,组件生命周期内复用)
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Behavior")
	TMap<FGameplayTag, TObjectPtr<UBXBehaviorAgent>> BehaviorAgents;

	// 正在执行的行为(唯一事实表)
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Behavior")
	TMap<FGameplayTag, FBXBehaviorRuntimeData> ActiveBehaviors;

	// 取消窗口保护记录(技能驱动)
	TMap<FGameplayTag, TArray<FBXProtectionRecord>> ProtectionEntries;

	// 挂起遮蔽表(状态禁用通道:key=状态禁用Tag,行为条目不移表,仅Agent停转+查询遮蔽)
	TMap<FGameplayTag, FBXSuspendMask> SuspendMasks;

	// 激活链深度守卫(防进入链环)
	int32 EnterChainDepth = 0;

#pragma endregion Important



#pragma region API
public:
	// 查询行为是否活跃(族Tag语义:精确命中或任一族成员活跃)
	UFUNCTION(BlueprintCallable, Category = "BattleX|Behavior")
	bool CheckActiveBehavior(const FGameplayTag& InBehaviorTag) const;

	// 获取全部活跃行为
	UFUNCTION(BlueprintCallable, Category = "BattleX|Behavior")
	void GetActiveBehaviors(FGameplayTagContainer& OutBehaviors) const;

	// 只读判定:行为能否开始(挂起/拒绝关系/Agent检查/挤出目标保护,无副作用;Agent检查用空参数)
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

	// 状态禁用门控:遮蔽生效(状态进入时调用;活跃行为Agent停转+Exit事件,瞬间/未激活行为被CanStart拦截)
	void SuspendByForbiddenTag(const FGameplayTag& InForbiddenTag, const FGameplayTag& InByState);

	// 状态禁用门控:解除遮蔽(状态退出时调用;被其他活跃遮蔽键覆盖的行为保持挂起)
	void ResumeByForbiddenTag(const FGameplayTag& InForbiddenTag, const FGameplayTag& InByState);

	// 查询行为是否被状态挂起(任一活跃遮蔽键为该Tag的祖先或自身)
	UFUNCTION(BlueprintCallable, Category = "BattleX|Behavior")
	bool IsBehaviorSuspended(const FGameplayTag& InBehaviorTag) const;

	// 查询行为是否被禁止(挂起中或被拒关系挡住)
	UFUNCTION(BlueprintCallable, Category = "BattleX|Behavior")
	bool CheckForbiddenBehavior(const FGameplayTag& InBehaviorTag) const;

#pragma endregion API



#pragma region Internal
protected:
	// 只读判定内部实现(带启动参数:参数依赖型Agent用真实参数判定)
	bool CanStartBehaviorInternal(const FGameplayTag& InBehaviorTag, const FInstancedStruct& InParameter, FBXBehaviorStartCheck& OutCheck) const;

	// 内部开始行为(管线执行:表更新/Agent/事件;参数经移动语义存入LastStartParameter,Agent在移动前使用)
	bool InternalStartBehavior(const FGameplayTag& InBehaviorTag, FInstancedStruct&& InParameter, int64 InSign);

	// 内部停止行为(管线执行:表更新/Agent/事件,全部停止路径收束于此)
	bool InternalStopBehavior(const FGameplayTag& InBehaviorTag, const FInstancedStruct& InParameter, int64 InSign, EBXBehaviorEndReason InReason);

	// 收集活跃行为Tag快照(遍历前收集,回调中增删安全)
	void CollectActiveBehaviorTags(TArray<FGameplayTag>& OutTags) const;

	// 广播行为事件
	void BroadcastBehaviorEvent(bool bEnter, const FGameplayTag& InBehaviorTag, int64 InSign, EBXBehaviorEndReason InReason);

#pragma endregion Internal
};
