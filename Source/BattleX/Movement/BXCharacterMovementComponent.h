#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "BXCharacterMovementComponent.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBXDoJump);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBXLanded, const FHitResult&, Hit);



UCLASS(BlueprintType, Blueprintable)
class BATTLEX_API UBXCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
#pragma region Important
public:

protected:
	
#pragma endregion Important



#pragma region Event
public:
	UPROPERTY(BlueprintAssignable)
	FBXDoJump DoJumpEvent;
	
	UPROPERTY(BlueprintAssignable)
	FBXLanded LandedEvent;
	
#pragma endregion Event



#pragma region State
public:
	UFUNCTION(BlueprintPure, Category = "State")
	bool IsProactiveMovement() const;
	
	UFUNCTION(BlueprintCallable, Category = "State")
	void ChangeDisableProactiveMovement(int64 InSign, bool bDisable);

	UFUNCTION(BlueprintPure, Category = "State")
	bool AllowProactiveMovement() const;

	UFUNCTION(BlueprintCallable, Category = "State")
	void ChangeDisableProactiveRotation(int64 InSign, bool bDisable);

	UFUNCTION(BlueprintPure, Category = "State")
	bool AllowProactiveRotation() const;

	UFUNCTION(BlueprintCallable, Category = "State")
	void ChangeDisableProactiveJump(int64 InSign, bool bDisable);

	UFUNCTION(BlueprintPure, Category = "State")
	bool AllowProactiveJump() const;

	UFUNCTION(BlueprintCallable, Category = "State")
	void ChangeDisableMeshOffset(int64 InSign, bool bDisable);

	UFUNCTION(BlueprintPure, Category = "State")
	bool AllowMeshOffset() const;
	
protected:
	// 禁止主动移动
	UPROPERTY(Transient)
	TArray<int64> DisableProactiveMovement;

	// 禁止主动转向
	UPROPERTY(Transient)
	TArray<int64> DisableProactiveRotation;

	// 禁止主动跳跃
	UPROPERTY(Transient)
	TArray<int64> DisableProactiveJump;

	// 禁止模型偏移
	UPROPERTY(Transient)
	TArray<int64> DisableMeshOffset;
	
#pragma endregion State



#pragma region Override
public:
	// 计算速度
	virtual void CalcVelocity(float DeltaTime, float Friction, bool bFluid, float BrakingDeceleration) override;

	// 计算滑动向量
	virtual FVector ComputeSlideVector(const FVector& Delta, const float Time, const FVector& Normal, const FHitResult& Hit) const override;

	// 物理转向
	virtual void PhysicsRotation(float DeltaTime) override;

	// 处理着地
	virtual void ProcessLanded(const FHitResult& Hit, float remainingTime, int32 Iterations) override;

	// 是否可以跳跃
	virtual bool CanAttemptJump() const override;
	
	// 跳跃
	virtual bool DoJump(bool bReplayingMoves) override;
	
#pragma endregion Override
	
};
