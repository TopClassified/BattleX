#pragma once

#include "CoreMinimal.h"
#include "StructUtils/InstancedStruct.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"

#include "BXStateStructs.h"
#include "StateMachine/BXStateMachineAsset.h"
#include "StateMachine/BXStateMachineInstance.h"

#include "BXStateComponent.generated.h"


// 状态系统组件(唯一事实表+状态机实例管理+到期快照+禁用行为门控+表现触发)
// 服务器权威:状态机转移评估仅服务器执行,客户端经多播跟随(首期本地单机语义,网络通道P5接入)
UCLASS(ClassGroup = "BattleX", meta = (BlueprintSpawnableComponent))
class BATTLEX_API UBXStateComponent : public UActorComponent
{
	GENERATED_BODY()

#pragma region Important
public:
	UBXStateComponent();

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

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

	// 查询某状态是否禁用指定行为(禁用门控查询,行为组件恢复判定用)
	bool IsBehaviorForbiddenByState(const FGameplayTag& InStateTag, const FGameplayTag& InBehaviorTag) const;

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
	// 内部进入状态(管线:族内定位/表更新/禁用门控/表现/事件;转移路径抑制内置Entry表现,由调用方边表现统一触发)
	bool InternalEnterState(const FGameplayTag& InStateTag, int64 InSign, float InDuration, EBXStateEndReason InExternalReason, bool bSuppressEntryPresentation = false);

	// 内部退出状态(管线:表更新/表现/禁用解除/事件,全部退出路径收束于此;bDeferForbiddenRelease=延迟禁用解除,转移路径用)
	bool InternalExitState(const FGameplayTag& InStateTag, int64 InSign, EBXStateEndReason InReason, bool bSuppressPresentation, bool bDeferForbiddenRelease = false);

	// 到期评估(Tick:快照收集→逐条处理)
	void UpdateExpiredStates(float InDeltaTime);

	// 状态机转移评估(Tick:TE_OnExpired边在到期时评估,TE_OnTick边每帧评估)
	void UpdateStateMachines(float InDeltaTime);

	// 执行状态机转移(退出当前+进入目标;旧状态禁用解除延迟到新状态登记后,共享禁用Tag经遮蔽多重登记保持无抖动)
	bool ExecuteTransition(UBXStateMachineInstance* InMachine, UBXSMStateNode* InTargetNode, EBXStateEndReason InReason);

	// 退出状态全部来源(内部版:转移路径延迟禁用解除,由调用方在新状态登记后统一解除)
	bool ExitStateAllSourcesInternal(const FGameplayTag& InStateTag, EBXStateEndReason InReason, bool bSuppressPresentation, bool bDeferForbiddenRelease);

	// 触发状态表现(各端本地)
	void TriggerPresentation(const FBXStatePresentation& InPresentation, bool bIsEntry, const FGameplayTag& InStateTag);

	// 状态禁用门控:进入时挂起行为
	void ApplyForbiddenBehaviors(const FGameplayTagContainer& InForbiddenBehaviors, const FGameplayTag& InByState);

	// 状态禁用门控:退出时尝试恢复行为
	void ReleaseForbiddenBehaviors(const FGameplayTagContainer& InForbiddenBehaviors, const FGameplayTag& InByState);

	// 收集活跃状态Tag快照
	void CollectActiveStateTags(TArray<FGameplayTag>& OutTags) const;

	// 广播状态事件
	void BroadcastStateEvent(bool bEnter, const FGameplayTag& InStateTag, int64 InSign, float InDuration, EBXStateEndReason InReason);

	// 查询状态Tag所属状态机实例
	UBXStateMachineInstance* FindMachineByStateTag(const FGameplayTag& InStateTag) const;

	// 查询状态配置(族内节点或裸状态配置)
	bool GetStateDurationAndForbidden(const FGameplayTag& InStateTag, float& OutDuration, FGameplayTagContainer& OutForbidden) const;

#pragma endregion Internal
};
