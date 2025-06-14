#pragma once

#include "CoreMinimal.h"
#include "InstancedStruct.h"

#include "BXGameplayTags.h"
#include "BXStateMachine.h"

#include "BXSMStun.generated.h"



// 无硬直状态参数
USTRUCT(BlueprintType)
struct FBXSSNormalParameter
{
	GENERATED_USTRUCT_BODY()

public:
	// 动画标签
	UPROPERTY(Transient, BlueprintReadWrite)
	FGameplayTag AnimTag;
	
	// 受击骨骼
	UPROPERTY(Transient, BlueprintReadWrite)
	FName BoneName;

	// 受击方向
	UPROPERTY(Transient, BlueprintReadWrite)
	FVector HitDirection;
};



// 击退状态参数
USTRUCT(BlueprintType)
struct FBXSSKnockbackParameter
{
	GENERATED_USTRUCT_BODY()

public:
	// 动画标签
	UPROPERTY(Transient, BlueprintReadWrite)
	FGameplayTag AnimTag;

	// 状态持续时间
	UPROPERTY(Transient, BlueprintReadWrite)
	float StateDuration = 1.0f;

	// 位移终点
	UPROPERTY(Transient, BlueprintReadWrite)
	FVector MovementPosition = FVector::ZeroVector;

	// 位移映射曲线
	UPROPERTY(Transient, BlueprintReadWrite)
	UCurveFloat* MovementCurve = nullptr;

	// 旋转基准轴（让相对于角色的某个方向的箭头旋转到和受击方向相同的方向，比如默认是角色局部X轴旋转到和受击方向相同）
	UPROPERTY(Transient, BlueprintReadWrite)
	FVector RotateAxis = FVector::ForwardVector;
};



// 击倒状态参数
USTRUCT(BlueprintType)
struct FBXSSKnockdownParameter
{
	GENERATED_USTRUCT_BODY()

public:
	// 动画标签
	UPROPERTY(Transient, BlueprintReadWrite)
	FGameplayTag AnimTag;

	// 状态持续时间
	UPROPERTY(Transient, BlueprintReadWrite)
	float StateDuration = 2.0f;

	// 位移持续时间
	UPROPERTY(Transient, BlueprintReadWrite)
	float MovementDuration = 1.0f;

	// 位移终点
	UPROPERTY(Transient, BlueprintReadWrite)
	FVector MovementPosition = FVector::ZeroVector;

	// 位移映射曲线
	UPROPERTY(Transient, BlueprintReadWrite)
	UCurveFloat* MovementCurve = nullptr;
};



// 击趴状态参数
USTRUCT(BlueprintType)
struct FBXSSProneParameter
{
	GENERATED_USTRUCT_BODY()

public:
	// 动画标签
	UPROPERTY(Transient, BlueprintReadWrite)
	FGameplayTag AnimTag;

	// 状态持续时间
	UPROPERTY(Transient, BlueprintReadWrite)
	float StateDuration = 2.0f;

	// 位移持续时间
	UPROPERTY(Transient, BlueprintReadWrite)
	float MovementDuration = 1.0f;

	// 位移终点
	UPROPERTY(Transient, BlueprintReadWrite)
	FVector MovementPosition = FVector::ZeroVector;

	// 位移映射曲线
	UPROPERTY(Transient, BlueprintReadWrite)
	UCurveFloat* MovementCurve = nullptr;
};



// 击飞上升状态参数
USTRUCT(BlueprintType)
struct FBXSSAscendingParameter
{
	GENERATED_USTRUCT_BODY()

public:
	// 动画标签
	UPROPERTY(Transient, BlueprintReadWrite)
	FGameplayTag AnimTag;

	// 状态持续时间
	UPROPERTY(Transient, BlueprintReadWrite)
	float StateDuration = 0.3f;

	// 滞空持续时间
	UPROPERTY(Transient, BlueprintReadWrite)
	float FloatingDuration = 0.2f;

	// 位移终点
	UPROPERTY(Transient, BlueprintReadWrite)
	FVector MovementPosition = FVector::ZeroVector;

	// 位移映射曲线
	UPROPERTY(Transient, BlueprintReadWrite)
	UCurveFloat* MovementCurve = nullptr;
};



UCLASS(BlueprintType, Blueprintable)
class BATTLEX_API UBXSMStun : public UBXStateMachine
{
	GENERATED_BODY()

#pragma region Important
public:
	// 更新状态机
	void NativeUpdate(float InDeltaTime);
	
#pragma endregion Important

	

#pragma region State
protected:
	// 进入某个状态
	bool NativeEnterState(const FGameplayTag& InState, const FInstancedStruct& InParameter);

	// 退出某个状态
	bool NativeExitState(const FGameplayTag& InState, const FInstancedStruct& InParameter);

protected:
	// 当前的硬直状态
	FGameplayTag CurrentState = BXGameplayTags::BXStunState_Normal;
	
#pragma endregion State



#pragma region Stun
protected:
	// 无硬直状态
	bool EnterNormal(const FInstancedStruct& InParameter);
	bool ExitNormal(const FInstancedStruct& InParameter);

	// 击退态
	bool EnterKnockback(const FInstancedStruct& InParameter);
	bool ExitKnockback(const FInstancedStruct& InParameter);

	// 击倒态
	bool EnterKnockdown(const FInstancedStruct& InParameter);
	bool ExitKnockdown(const FInstancedStruct& InParameter);

	// 击趴态
	bool EnterProne(const FInstancedStruct& InParameter);
	bool ExitProne(const FInstancedStruct& InParameter);

	// 上升态
	bool EnterAscending(const FInstancedStruct& InParameter);
	bool ExitAscending(const FInstancedStruct& InParameter);

	// 下落态
	bool EnterFalling(const FInstancedStruct& InParameter);
	bool ExitFalling(const FInstancedStruct& InParameter);

	// 滞空态
	bool EnterFloating(const FInstancedStruct& InParameter);
	bool ExitFloating(const FInstancedStruct& InParameter);

	// 处刑态
	bool EnterExecution(const FInstancedStruct& InParameter);
	bool ExitExecution(const FInstancedStruct& InParameter);
	
#pragma endregion Stun
	
};
