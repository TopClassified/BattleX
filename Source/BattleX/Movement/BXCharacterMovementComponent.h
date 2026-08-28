#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "BXStructs.h"

#include "BXCharacterMovementComponent.generated.h"



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



#pragma region Behavior
protected:
	// 正在主动移动
	bool bProactiveMoving = false;

	// 正在主动转向
	bool bProactiveRotating = false;

	// 行为代理下推的门控开关(由UBXProxyMove/Rotate/Jump在Enable/Disable时推送,默认false=允许)
	// 架构约定:本组件只读本地开关执行物理刹车,不反查行为组件;主动事实上报(Start/Stop)方向保持不变
	UPROPERTY(Transient, BlueprintReadOnly)
	bool bBehaviorMoveBlocked = false;

	UPROPERTY(Transient, BlueprintReadOnly)
	bool bBehaviorRotateBlocked = false;

	UPROPERTY(Transient, BlueprintReadOnly)
	bool bBehaviorJumpBlocked = false;

public:
	// 设置主动移动禁止(移动代理门控下推专用,勿在别处刷写)
	void SetBehaviorMoveBlocked(bool bInBlocked) { bBehaviorMoveBlocked = bInBlocked; }

	// 设置主动转向禁止(转向代理门控下推专用,勿在别处刷写)
	void SetBehaviorRotateBlocked(bool bInBlocked) { bBehaviorRotateBlocked = bInBlocked; }

	// 设置跳跃禁止(跳跃代理门控下推专用,勿在别处刷写)
	void SetBehaviorJumpBlocked(bool bInBlocked) { bBehaviorJumpBlocked = bInBlocked; }

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

	// 根据时间获取历史SkeletalMeshComponent位置
	UFUNCTION(BlueprintCallable, Category = "Record")
	FTransform GetHistoryMeshTransformByTime(float InTime);

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

	// 角色SkeletalMeshComponent运行轨迹
	UPROPERTY(Transient)
	TArray<FBXTrajectoryPoint> MeshTrajectoryPoints;

#pragma endregion Record
	
};
