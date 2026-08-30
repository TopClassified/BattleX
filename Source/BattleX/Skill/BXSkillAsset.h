#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#include "Timeline/BXTLAsset.h"
#include "BXSkillEnums.h"

#include "BXSkillAsset.generated.h"



class UBXTaskCondition;



// 技能取消窗口(窗口内解除姿态行为保护,允许互斥行为挤出实现连招取消)
USTRUCT(BlueprintType)
struct FBXSkillCancelWindow
{
	GENERATED_USTRUCT_BODY()

public:
	// 窗口起始时间(技能时间轴秒)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0.0"))
	float StartTime = 0.0f;

	// 窗口结束时间(技能时间轴秒)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0.0"))
	float EndTime = 0.0f;

	// 是否包含起点
	bool Contains(float InTime) const
	{
		return InTime >= StartTime && InTime <= EndTime;
	}
};



// 技能进入状态条目(数组序=进入序:族内互斥多状态按配置顺序顶掉,末位生效)
USTRUCT(BlueprintType)
struct FBXSkillEnterState
{
	GENERATED_USTRUCT_BODY()

public:
	// 状态Tag
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag StateTag;

	// 持续时长(≤0用状态配置默认)
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "-1.0"))
	float Duration = -1.0f;
};



// 技能资源(对应技能系统,继承Timeline资产复用Sections/TaskList)
UCLASS()
class BATTLEX_API UBXSkillAsset : public UBXTLAsset
{
	GENERATED_BODY()

#pragma region Release
public:
	// 释放条件列表(服务器校验用,复用UBXTaskCondition体系)
	UPROPERTY(EditDefaultsOnly, Instanced, Category = "Release")
	TArray<UBXTaskCondition*> ReleaseConditions;

	// 默认冷却时长(秒,-1代表无冷却)
	UPROPERTY(EditDefaultsOnly, Category = "Release", meta = (ClampMin = "-1.0"))
	float Cooldown = -1.0f;

	// 锁定类型(决定释放时附带的数据,见EBXSkillLockType)
	UPROPERTY(EditDefaultsOnly, Category = "Release")
	EBXSkillLockType LockType = EBXSkillLockType::None;

#pragma endregion Release



#pragma region BehaviorState
public:
	// 技能对应的行为Tag(1:1映射,技能姿态行为;空代表无行为纯技能)
	UPROPERTY(EditDefaultsOnly, Category = "Behavior")
	FGameplayTag BehaviorTag;

	// 取消窗口期内是否豁免自身互斥(窗口期因本技能行为在位而被拒的接招行为放行,接管方向矩阵挤出照旧执行)
	UPROPERTY(EditDefaultsOnly, Category = "Behavior")
	bool bWaiveOnCancelWindow = true;

	// 技能开始时进入的状态列表(按序进入;Sign=SkillID,技能结束收束退出)
	UPROPERTY(EditDefaultsOnly, Category = "State")
	TArray<FBXSkillEnterState> EnterStates;

	// 取消窗口列表(窗口内豁免自身互斥,接招行为放行)
	UPROPERTY(EditDefaultsOnly, Category = "Behavior")
	TArray<FBXSkillCancelWindow> CancelWindows;

	// 查询指定时间点是否处于取消窗口内
	bool IsInCancelWindow(float InTimelineTime) const;

#pragma endregion BehaviorState

};
