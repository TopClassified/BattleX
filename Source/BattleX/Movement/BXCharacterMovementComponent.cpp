#include "BXCharacterMovementComponent.h"

#include "BXStructs.h"
#include "BXGameplayTags.h"
#include "BXEventStructs.h"
#include "BXEventManager.h"
#include "Behavior/BXBehaviorFunctionLibrary.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "TimerManager.h"



#pragma region Important
void UBXCharacterMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 滞空/下坠行为事实边沿维护(物理推进后判定)
	UpdateFallingBehavior();

	UWorld* World = GetWorld();
	if (IsValid(World) && IsValid(UpdatedComponent))
	{
		float CurrentTime = World->GetTimeSeconds();
		TrajectoryPoints.Add(FBXTrajectoryPoint(UpdatedComponent->GetComponentTransform(), CurrentTime));

		// 同时记录角色SkeletalMeshComponent的世界Transform
		ACharacter* OwnerChar = Cast<ACharacter>(GetOwner());
		if (IsValid(OwnerChar))
		{
			USkeletalMeshComponent* MeshComp = OwnerChar->GetMesh();
			if (IsValid(MeshComp))
			{
				MeshTrajectoryPoints.Add(FBXTrajectoryPoint(MeshComp->GetComponentTransform(), CurrentTime));
			}
		}
	}

	CleanTimer += DeltaTime;
	if (CleanTimer > CleanInterval)
	{
		CleanTimer = 0.0f;

		CleanTrajectoryPoints();
	}
}

#pragma endregion Important



#pragma region Override
void UBXCharacterMovementComponent::CalcVelocity(float DeltaTime, float Friction, bool bFluid, float BrakingDeceleration)
{
	// Root Motion/RootMotionSource期间不算主动移动:在位先收束走路事实再走引擎早退(结束后由下方边沿按输入恢复)
	if (bProactiveMoving && (HasAnimRootMotion() || HasRootMotionSources()))
	{
		bProactiveMoving = false;
		UBXBehaviorFunctionLibrary::StopBehavior(GetOwner(), BXGameplayTags::BXBehavior_Walk);
	}

	if (!HasValidData() || HasAnimRootMotion() || DeltaTime < MIN_TICK_TIME)
	{
		return;
	}
	
	if (GetCharacterOwner() && GetCharacterOwner()->GetLocalRole() == ROLE_SimulatedProxy && !bWasSimulatingRootMotion)
	{
		return;
	}

	Friction = FMath::Max(0.0f, Friction);
	const float MaxAccel = GetMaxAcceleration();
	float MaxSpeed = GetMaxSpeed();
	
	bool bZeroRequestedAcceleration = true;
	FVector RequestedAcceleration = FVector::ZeroVector;
	float RequestedSpeed = 0.0f;
	if (ApplyRequestedMove(DeltaTime, MaxAccel, MaxSpeed, Friction, BrakingDeceleration, RequestedAcceleration, RequestedSpeed))
	{
		bZeroRequestedAcceleration = false;
	}

	if (bForceMaxAccel)
	{
		if (Acceleration.SizeSquared() > UE_SMALL_NUMBER)
		{
			Acceleration = Acceleration.GetSafeNormal() * MaxAccel;
		}
		else 
		{
			Acceleration = MaxAccel * (Velocity.SizeSquared() < UE_SMALL_NUMBER ? UpdatedComponent->GetForwardVector() : Velocity.GetSafeNormal());
		}

		AnalogInputModifier = 1.0f;
	}

	const float MaxInputSpeed = FMath::Max(MaxSpeed * AnalogInputModifier, GetMinAnalogSpeed());
	MaxSpeed = FMath::Max(RequestedSpeed, MaxInputSpeed);

	bool bZeroAcceleration = Acceleration.IsZero();
	bool bVelocityOverMax = IsExceedingMaxSpeed(MaxSpeed);

	// 禁止主动移动(行为代理下推开关),将寻路和输入的加速度标记为零向量
	if (bBehaviorMoveBlocked)
	{
		Acceleration = FVector::ZeroVector;
		bZeroAcceleration = true;
		
		RequestedAcceleration = FVector::ZeroVector;
		bZeroRequestedAcceleration = true;
	}
	
	if ((bZeroAcceleration && bZeroRequestedAcceleration) || bVelocityOverMax)
	{
		const FVector OldVelocity = Velocity;
		const float ActualBrakingFriction = (bUseSeparateBrakingFriction ? BrakingFriction : Friction);
		ApplyVelocityBraking(DeltaTime, ActualBrakingFriction, BrakingDeceleration);
	
		if (bVelocityOverMax && Velocity.SizeSquared() < FMath::Square(MaxSpeed) && FVector::DotProduct(Acceleration, OldVelocity) > 0.0f)
		{
			Velocity = OldVelocity.GetSafeNormal() * MaxSpeed;
		}
	}
	else if (!bZeroAcceleration)
	{
		const FVector AccelDir = Acceleration.GetSafeNormal();
		const float VelSize = Velocity.Size();
		Velocity = Velocity - (Velocity - AccelDir * VelSize) * FMath::Min(DeltaTime * Friction, 1.0f);
	}

	if (bFluid)
	{
		Velocity = Velocity * (1.0f - FMath::Min(Friction * DeltaTime, 1.0f));
	}

	if (!bZeroAcceleration)
	{
		const float NewMaxInputSpeed = IsExceedingMaxSpeed(MaxInputSpeed) ? Velocity.Size() : MaxInputSpeed;
		Velocity += Acceleration * DeltaTime;
		Velocity = Velocity.GetClampedToMaxSize(NewMaxInputSpeed);
	}

	if (!bZeroRequestedAcceleration)
	{
		const float NewMaxRequestedSpeed = IsExceedingMaxSpeed(RequestedSpeed) ? Velocity.Size() : RequestedSpeed;
		Velocity += RequestedAcceleration * DeltaTime;
		Velocity = Velocity.GetClampedToMaxSize(NewMaxRequestedSpeed);
	}

	if (bUseRVOAvoidance)
	{
		CalcAvoidanceVelocity(DeltaTime);
	}
	
	// 更新主动移动的状态，并广播事件
	// 主动移动事实上报暂统一报走路(BXBehavior.Walk);走路/跑步/冲刺的速度分档待移动状态设计落地后在此区分
	// Root Motion/RootMotionSource期间不算主动移动(位移由动画或源驱动,与输入加速度无关),两侧边沿均排除
	if (UBXEventManager* BXEMgr = UBXEventManager::Get(this))
	{
		if (bProactiveMoving)
		{
			if ((bZeroAcceleration && bZeroRequestedAcceleration) || HasAnimRootMotion() || HasRootMotionSources())
			{
				bProactiveMoving = false;
				// 通知到行为组件，停止主动移动
				UBXBehaviorFunctionLibrary::StopBehavior(GetOwner(), BXGameplayTags::BXBehavior_Walk);
			}
		}
		else
		{
			if ((!bZeroAcceleration || !bZeroRequestedAcceleration) && !HasAnimRootMotion() && !HasRootMotionSources())
			{
				bProactiveMoving = true;
				// 通知到行为组件，开始主动移动
				UBXBehaviorFunctionLibrary::StartBehavior(GetOwner(), BXGameplayTags::BXBehavior_Walk);
			}
		}	
	}
}

FVector UBXCharacterMovementComponent::ComputeSlideVector(const FVector& Delta, const float Time, const FVector& Normal, const FHitResult& Hit) const
{
	// 移动被禁(行为代理下推开关)时不参与滑动修正
	if (bBehaviorMoveBlocked)
	{
		return Delta;
	}
	
	return Super::ComputeSlideVector(Delta, Time, Normal, Hit);
}

void UBXCharacterMovementComponent::PhysicsRotation(float DeltaTime)
{
	UBXEventManager* BXEMgr = UBXEventManager::Get(this);
	if (!IsValid(BXEMgr))
	{
		return;
	}
	
	if (!(bOrientRotationToMovement || bUseControllerDesiredRotation) || bBehaviorRotateBlocked)
	{
		return;
	}

	if (!HasValidData() || (!CharacterOwner->Controller && !bRunPhysicsWithNoController))
	{
		return;
	}

	FRotator CurrentRotation = UpdatedComponent->GetComponentRotation();
	CurrentRotation.DiagnosticCheckNaN(TEXT("CharacterMovementComponent::PhysicsRotation(): CurrentRotation"));

	FRotator DeltaRot = GetDeltaRotation(DeltaTime);
	DeltaRot.DiagnosticCheckNaN(TEXT("CharacterMovementComponent::PhysicsRotation(): GetDeltaRotation"));

	FRotator DesiredRotation = CurrentRotation;
	if (bOrientRotationToMovement)
	{
		DesiredRotation = ComputeOrientToMovementRotation(CurrentRotation, DeltaTime, DeltaRot);
	}
	else if (CharacterOwner->Controller && bUseControllerDesiredRotation)
	{
		DesiredRotation = CharacterOwner->Controller->GetDesiredRotation();
	}
	else if (!CharacterOwner->Controller && bRunPhysicsWithNoController && bUseControllerDesiredRotation)
	{
		if (AController* ControllerOwner = Cast<AController>(CharacterOwner->GetOwner()))
		{
			DesiredRotation = ControllerOwner->GetDesiredRotation();
		}
	}
	else
	{
		return;
	}

	const bool bWantsToBeVertical = ShouldRemainVertical();

	if (bWantsToBeVertical)
	{
		if (HasCustomGravity())
		{
			FRotator GravityRelativeDesiredRotation = (GetGravityToWorldTransform() * DesiredRotation.Quaternion()).Rotator();
			GravityRelativeDesiredRotation.Pitch = 0.0f;
			GravityRelativeDesiredRotation.Yaw = FRotator::NormalizeAxis(GravityRelativeDesiredRotation.Yaw);
			GravityRelativeDesiredRotation.Roll = 0.0f;
			DesiredRotation = (GetWorldToGravityTransform() * GravityRelativeDesiredRotation.Quaternion()).Rotator();
		}
		else
		{
			DesiredRotation.Pitch = 0.f;
			DesiredRotation.Yaw = FRotator::NormalizeAxis(DesiredRotation.Yaw);
			DesiredRotation.Roll = 0.f;
		}
	}
	else
	{
		DesiredRotation.Normalize();
	}
	
	const float AngleTolerance = 1e-3f;
	if (!CurrentRotation.Equals(DesiredRotation, AngleTolerance))
	{
		if (bWantsToBeVertical)
		{
			if (FMath::IsNearlyZero(DeltaRot.Pitch))
			{
				DeltaRot.Pitch = 360.0;
			}
			if (FMath::IsNearlyZero(DeltaRot.Roll))
			{
				DeltaRot.Roll = 360.0;
			}
		}

		if (HasCustomGravity())
		{
			FRotator GravityRelativeCurrentRotation = (GetGravityToWorldTransform() * CurrentRotation.Quaternion()).Rotator();
			FRotator GravityRelativeDesiredRotation = (GetGravityToWorldTransform() * DesiredRotation.Quaternion()).Rotator();

			if (!FMath::IsNearlyEqual(GravityRelativeCurrentRotation.Pitch, GravityRelativeDesiredRotation.Pitch, AngleTolerance))
			{
				GravityRelativeDesiredRotation.Pitch = FMath::FixedTurn(GravityRelativeCurrentRotation.Pitch, GravityRelativeDesiredRotation.Pitch, DeltaRot.Pitch);
			}

			if (!FMath::IsNearlyEqual(GravityRelativeCurrentRotation.Yaw, GravityRelativeDesiredRotation.Yaw, AngleTolerance))
			{
				GravityRelativeDesiredRotation.Yaw = FMath::FixedTurn(GravityRelativeCurrentRotation.Yaw, GravityRelativeDesiredRotation.Yaw, DeltaRot.Yaw);
			}

			if (!FMath::IsNearlyEqual(GravityRelativeCurrentRotation.Roll, GravityRelativeDesiredRotation.Roll, AngleTolerance))
			{
				GravityRelativeDesiredRotation.Roll = FMath::FixedTurn(GravityRelativeCurrentRotation.Roll, GravityRelativeDesiredRotation.Roll, DeltaRot.Roll);
			}

			DesiredRotation = (GetWorldToGravityTransform() * GravityRelativeDesiredRotation.Quaternion()).Rotator();
		}
		else
		{
			if (!FMath::IsNearlyEqual(CurrentRotation.Pitch, DesiredRotation.Pitch, AngleTolerance))
			{
				DesiredRotation.Pitch = FMath::FixedTurn(CurrentRotation.Pitch, DesiredRotation.Pitch, DeltaRot.Pitch);
			}

			if (!FMath::IsNearlyEqual(CurrentRotation.Yaw, DesiredRotation.Yaw, AngleTolerance))
			{
				DesiredRotation.Yaw = FMath::FixedTurn(CurrentRotation.Yaw, DesiredRotation.Yaw, DeltaRot.Yaw);
			}

			if (!FMath::IsNearlyEqual(CurrentRotation.Roll, DesiredRotation.Roll, AngleTolerance))
			{
				DesiredRotation.Roll = FMath::FixedTurn(CurrentRotation.Roll, DesiredRotation.Roll, DeltaRot.Roll);
			}
		}
		
		DesiredRotation.DiagnosticCheckNaN(TEXT("CharacterMovementComponent::PhysicsRotation(): DesiredRotation"));
		MoveUpdatedComponent( FVector::ZeroVector, DesiredRotation, /*bSweep*/ false );
	}
}

void UBXCharacterMovementComponent::ProcessLanded(const FHitResult& Hit, float remainingTime, int32 Iterations)
{
	if(IsFalling())
	{
		if (GetGroundMovementMode() == MOVE_NavWalking)
		{
			const FVector TestLocation = GetActorFeetLocation();
			FNavLocation NavLocation;

			const bool bHasNavigationData = FindNavFloor(TestLocation, NavLocation);
			if (!bHasNavigationData || NavLocation.NodeRef == INVALID_NAVNODEREF)
			{
				SetGroundMovementMode(MOVE_Walking);
			}
		}
		SetPostLandedPhysics(Hit);
	}
	
	if (IPathFollowingAgentInterface* PFAgent = GetPathFollowingAgent())
	{
		PFAgent->OnLanded();
	}

	// 兜底收束跳跃行为(上升段直接触地等未经下坠边沿的路径;幂等)
	UBXBehaviorFunctionLibrary::StopBehavior(GetOwner(), BXGameplayTags::BXBehavior_Jump);

	// 通知到行为组件，落地
	UBXBehaviorFunctionLibrary::StartBehaviorWithParameter<FHitResult>(GetOwner(), BXGameplayTags::BXBehavior_Landed, Hit);

	// 落地行为定时自动停止(每次落地重置计时,Stop 幂等);时长下限钳0.05,ini/代码直改<=0也按0.05走(常驻=落地后矩阵全锁死)
	constexpr float MinLandedBehaviorDuration = 0.05f;
	const float ClampedLandedDuration = FMath::Max(LandedBehaviorDuration, MinLandedBehaviorDuration);
	TWeakObjectPtr<UBXCharacterMovementComponent> WeakThis(this);
	GetWorld()->GetTimerManager().SetTimer(LandedBehaviorTimerHandle, FTimerDelegate::CreateLambda([WeakThis]()
	{
		if (WeakThis.IsValid())
		{
			UBXBehaviorFunctionLibrary::StopBehavior(WeakThis->GetOwner(), BXGameplayTags::BXBehavior_Landed);
		}
	}), ClampedLandedDuration, false);

	StartNewPhysics(remainingTime, Iterations);
}

void UBXCharacterMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);

	// 离开MOVE_Falling(落地/游泳/飞行等)立即收束下坠与跳跃行为,Sign=0系统常驻来源
	// 进入MOVE_Falling不在此上报:下坠须速度沿重力方向,由UpdateFallingBehavior逐帧边沿判定
	if (PreviousMovementMode == MOVE_Falling && MovementMode != MOVE_Falling)
	{
		UBXBehaviorFunctionLibrary::StopBehavior(GetOwner(), BXGameplayTags::BXBehavior_Falling);

		// 跳跃收束覆盖"上升段被切走到非地面模式"(入水/飞行等)的路径:既不经下坠边沿也不经落地
		UBXBehaviorFunctionLibrary::StopBehavior(GetOwner(), BXGameplayTags::BXBehavior_Jump);
	}
}

void UBXCharacterMovementComponent::UpdateFallingBehavior()
{
	// 下坠判定不能只看滞空模式:速度不逆重力方向(>=0,零速/水平离地即算下坠,仅上升段不算)
	const bool bNowDescending = IsFalling() && FVector::DotProduct(Velocity, GetGravityDirection()) >= 0.0f;
	if (bNowDescending == bDescending)
	{
		return;
	}
	bDescending = bNowDescending;

	if (bDescending)
	{
		// 因重力开始下坠:收束跳跃行为(正常跳跃在此结束;异常路径由ProcessLanded兜底),开始下坠行为
		UBXBehaviorFunctionLibrary::StopBehavior(GetOwner(), BXGameplayTags::BXBehavior_Jump);
		UBXBehaviorFunctionLibrary::StartBehavior(GetOwner(), BXGameplayTags::BXBehavior_Falling);
	}
	else
	{
		// 下坠结束:模式离开MOVE_Falling已在OnMovementModeChanged收束,此处兜底滞空中速度转为向上的场景(空中被上挑等)
		UBXBehaviorFunctionLibrary::StopBehavior(GetOwner(), BXGameplayTags::BXBehavior_Falling);
	}
}

bool UBXCharacterMovementComponent::CanAttemptJump() const
{
	// 跳跃被禁走行为代理下推开关
	return !bBehaviorJumpBlocked && !bWantsToCrouch && IsMovingOnGround();
}

bool UBXCharacterMovementComponent::DoJump(bool bReplayingMoves)
{
	if (!CanAttemptJump())
	{
		return false;
	}

	if (!bConstrainToPlane || FMath::Abs(PlaneConstraintNormal.Z) != 1.0f)
	{
		if (HasCustomGravity())
		{
			FVector GravityRelativeVelocity = RotateWorldToGravity(Velocity);
			GravityRelativeVelocity.Z = FMath::Max<FVector::FReal>(GravityRelativeVelocity.Z, JumpZVelocity);
			Velocity = RotateGravityToWorld(GravityRelativeVelocity);
		}
		else
		{
			Velocity.Z = FMath::Max<FVector::FReal>(Velocity.Z, JumpZVelocity);
		}
		
		SetMovementMode(MOVE_Falling);

		// 通知到行为组件，跳跃
		UBXBehaviorFunctionLibrary::StartBehavior(GetOwner(), BXGameplayTags::BXBehavior_Jump);
		
		return true;
	}
	
	return false;
}

#pragma endregion Override



// 在按时间升序的轨迹数组中二分查找首个 Time >= InTime 的索引
static int32 FindLowerBoundTime(const TArray<FBXTrajectoryPoint>& InList, float InTime)
{
	int32 Lo = 0;
	int32 Hi = InList.Num();
	while (Lo < Hi)
	{
		int32 Mid = Lo + (Hi - Lo) / 2;
		if (InList[Mid].Time < InTime)
		{
			Lo = Mid + 1;
		}
		else
		{
			Hi = Mid;
		}
	}
	return Lo;
}

// 在轨迹列表中按时间插值,返回是否命中
static bool SampleTrajectory(const TArray<FBXTrajectoryPoint>& InList, float InTime, FTransform& OutTransform)
{
	if (InList.Num() <= 0)
	{
		return false;
	}

	int32 CurrentIndex = FindLowerBoundTime(InList, InTime);

	// 全部早于 InTime,未命中
	if (CurrentIndex == InList.Num())
	{
		return false;
	}

	if (CurrentIndex == 0)
	{
		OutTransform = InList[0].Transform;
		return true;
	}

	const FBXTrajectoryPoint& Start = InList[CurrentIndex - 1];
	const FBXTrajectoryPoint& End = InList[CurrentIndex];

	float Alpha = (InTime - Start.Time) / (End.Time - Start.Time + 1e-8f);
	OutTransform = FTransform
	(
		FQuat::Slerp(Start.Transform.GetRotation(), End.Transform.GetRotation(), Alpha),
		FMath::Lerp(Start.Transform.GetLocation(), End.Transform.GetLocation(), Alpha),
		FMath::Lerp(Start.Transform.GetScale3D(), End.Transform.GetScale3D(), Alpha)
	);
	return true;
}



#pragma region Record
FTransform UBXCharacterMovementComponent::GetHistoryTransformByTime(float InTime)
{
	FTransform Result;
	if (SampleTrajectory(TrajectoryPoints, InTime, Result))
	{
		return Result;
	}

	if (IsValid(UpdatedComponent))
	{
		return UpdatedComponent->GetComponentTransform();
	}

	return FTransform();
}

FTransform UBXCharacterMovementComponent::GetHistoryMeshTransformByTime(float InTime)
{
	FTransform Result;
	if (SampleTrajectory(MeshTrajectoryPoints, InTime, Result))
	{
		return Result;
	}

	// 历史数据未命中时回退到当前Mesh组件Transform
	ACharacter* OwnerChar = Cast<ACharacter>(GetOwner());
	if (IsValid(OwnerChar))
	{
		USkeletalMeshComponent* MeshComp = OwnerChar->GetMesh();
		if (IsValid(MeshComp))
		{
			return MeshComp->GetComponentTransform();
		}
	}

	if (IsValid(UpdatedComponent))
	{
		return UpdatedComponent->GetComponentTransform();
	}

	return FTransform();
}

void UBXCharacterMovementComponent::CleanTrajectoryPoints()
{
	float CurrentTime = GetWorld()->GetTimeSeconds();
	TrajectoryPoints.RemoveAll
	(
		[&](const FBXTrajectoryPoint& A)
		{
			return CurrentTime - A.Time >= RecordTime;
		}
	);
	MeshTrajectoryPoints.RemoveAll
	(
		[&](const FBXTrajectoryPoint& A)
		{
			return CurrentTime - A.Time >= RecordTime;
		}
	);
}

#pragma endregion Record
