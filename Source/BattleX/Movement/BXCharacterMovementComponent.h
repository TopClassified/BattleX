#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "BXStructs.h"

#include "BXCharacterMovementComponent.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBXLanded, const FHitResult&, Hit);



UCLASS(BlueprintType, Blueprintable)
class BATTLEX_API UBXCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
#pragma region Important
public:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	
protected:
	// 清理时间间隔
	UPROPERTY(EditDefaultsOnly)
	float CleanInterval = 30.0f;

	// 清理计时器
	UPROPERTY(Transient)
	float CleanTimer = 0.0f;
	
#pragma endregion Important


	
#pragma region Event
public:
	UPROPERTY(BlueprintAssignable)
	FBXLanded LandedEvent;
	
#pragma endregion Event



#pragma region Behavior
protected:
	// 正在主动移动
	bool bProactiveMoving = false;
	
	// 正在主动转向
	bool bProactiveRotating = false;
	
#pragma endregion Behavior



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



#pragma region Record
public:
	// 根据时间获取历史位置
	UFUNCTION(BlueprintCallable, Category = "Record")
	FTransform GetHistoryTransformByTime(float InTime);

	// 清理超时运行轨迹
	UFUNCTION(BlueprintCallable, Category = "Record")
	void CleanTrajectoryPoints();
	
protected:
	// 记录时长
	UPROPERTY(EditDefaultsOnly, Category = "Record")
	float RecordTime = 20.0f;
	
	// 角色运行轨迹
	UPROPERTY(Transient)
	TArray<FBXTrajectoryPoint> TrajectoryPoints;
	
#pragma endregion Record
	
};
