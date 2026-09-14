#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/TimerHandle.h"

#include "BXStructs.h"
#include "OperateStack/BXOperateStack.h"

#include "BXCharacterMovementComponent.generated.h"



UCLASS(BlueprintType, Blueprintable)
class BATTLEX_API UBXCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
#pragma region Important
public:
	UBXCharacterMovementComponent();

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
	// 正在主动移动(边沿状态,驱动主动移动事实上报;暂统一报走路,速度分档待移动状态设计落地)
	bool bProactiveMoving = false;

	// 正在下坠(滞空∧速度沿重力方向;逐帧边沿状态,驱动下坠行为上报与跳跃行为收束)
	bool bDescending = false;

	// 滞空行为事实边沿维护(Tick每帧物理推进后调用)
	void UpdateFallingBehavior();

	// 落地行为自动停止时长(秒;使用时下限钳0.05禁止常驻——Landed在位禁用列过广,常驻=落地后全锁死)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ClampMin="0.05", ForceUnits="s"))
	float LandedBehaviorDuration = 0.1f;

	// 落地行为自动停止计时器(每次落地重置)
	FTimerHandle LandedBehaviorTimerHandle;

#pragma endregion Behavior



#pragma region BlockGate
protected:
	// 门控开关(基础服务,操作记录栈的生效值,任何系统经Add/Remove/ClearBlocked登记/移除/注销,默认false=允许)
	// 架构约定:本组件只读本地开关执行物理刹车,不反查行为组件;主动事实上报(Start/Stop)方向保持不变
	UPROPERTY(Transient, BlueprintReadOnly)
	bool bMoveBlocked = false;

	UPROPERTY(Transient, BlueprintReadOnly)
	bool bRotateBlocked = false;

	UPROPERTY(Transient, BlueprintReadOnly)
	bool bJumpBlocked = false;

	// 三条禁止的操作记录栈(记录谁声明了何种目标值;生效值=栈顶,自动刷新到上述开关)
	TBXOperateStack<bool> MoveBlockStack;
	TBXOperateStack<bool> RotateBlockStack;
	TBXOperateStack<bool> JumpBlockStack;

public:
	// 登记主动移动禁止的修改记录(InModifier=修改者标识;生效值=栈顶,自动刷新bMoveBlocked;返回句柄ID供RemoveByID)
	int64 AddMoveBlocked(bool bInBlocked, FName InModifier);

	// 登记主动转向禁止的修改记录(InModifier=修改者标识;生效值=栈顶,自动刷新bRotateBlocked;返回句柄ID供RemoveByID)
	int64 AddRotateBlocked(bool bInBlocked, FName InModifier);

	// 登记跳跃禁止的修改记录(InModifier=修改者标识;生效值=栈顶,自动刷新bJumpBlocked;返回句柄ID供RemoveByID)
	int64 AddJumpBlocked(bool bInBlocked, FName InModifier);

	// 按句柄移除主动移动禁止记录(生效值回落到余下栈顶;基线与未命中返回false)
	bool RemoveMoveBlocked(int64 InID);

	// 按句柄移除主动转向禁止记录(生效值回落到余下栈顶;基线与未命中返回false)
	bool RemoveRotateBlocked(int64 InID);

	// 按句柄移除跳跃禁止记录(生效值回落到余下栈顶;基线与未命中返回false)
	bool RemoveJumpBlocked(int64 InID);

	// 注销修改者的移动禁止记录(生效值回落到余下栈顶;无记录静默)
	void ClearMoveBlocked(FName InModifier);

	// 注销修改者的转向禁止记录(生效值回落到余下栈顶;无记录静默)
	void ClearRotateBlocked(FName InModifier);

	// 注销修改者的跳跃禁止记录(生效值回落到余下栈顶;无记录静默)
	void ClearJumpBlocked(FName InModifier);

#pragma endregion BlockGate



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

protected:
	// 移动模式变更(滞空事实上报:进入MOVE_Falling开始,离开自动停止)
	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;

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
