#include "BXCharacterMovementComponent.h"

#include "BXStructs.h"
#include "BXGameplayTags.h"
#include "BXEventStructs.h"
#include "BXEventManager.h"
#include "BXStateFunctionLibrary.h"
#include "GameFramework/Character.h"



#pragma region Important
void UBXCharacterMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UWorld* World = GetWorld();
	if (IsValid(World) && IsValid(UpdatedComponent))
	{
		float CurrentTime = World->GetTimeSeconds();
		TrajectoryPoints.Add(FBXTrajectoryPoint(UpdatedComponent->GetComponentTransform(), CurrentTime));
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

	// 禁止主动移动，将寻路和输入的加速度标记为零向量
	if (UBXStateFunctionLibrary::CheckForbiddenBehavior(GetOwner(), BXGameplayTags::BXBehavior_Locomotion_Move))
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
	if (UBXEventManager* BXEMgr = UBXEventManager::Get(this))
	{
		if (bProactiveMoving)
		{
			if (bZeroAcceleration && bZeroRequestedAcceleration)
			{
				bProactiveMoving = false;
				// 通知到行为组件，停止主动移动
				UBXStateFunctionLibrary::StopBehavior(GetOwner(), BXGameplayTags::BXBehavior_Locomotion_Move);
			}
		}
		else
		{
			if (!bZeroAcceleration || !bZeroRequestedAcceleration)
			{
				bProactiveMoving = true;
				// 通知到行为组件，开始主动移动
				UBXStateFunctionLibrary::StartBehavior(GetOwner(), BXGameplayTags::BXBehavior_Locomotion_Move);
			}
		}	
	}
}

FVector UBXCharacterMovementComponent::ComputeSlideVector(const FVector& Delta, const float Time, const FVector& Normal, const FHitResult& Hit) const
{
	if (UBXStateFunctionLibrary::CheckForbiddenBehavior(GetOwner(), BXGameplayTags::BXBehavior_Locomotion_Move))
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
	
	if (!(bOrientRotationToMovement || bUseControllerDesiredRotation) || !UBXStateFunctionLibrary::CheckForbiddenBehavior(GetOwner(), BXGameplayTags::BXBehavior_Locomotion_Rotate))
	{
		// 停止主动转向状态
		if (bProactiveRotating)
		{
			bProactiveRotating = false;
			// 通知到行为组件，停止主动转向
			UBXStateFunctionLibrary::StopBehavior(GetOwner(), BXGameplayTags::BXBehavior_Locomotion_Rotate);
		}
		
		return;
	}

	if (!HasValidData() || (!CharacterOwner->Controller && !bRunPhysicsWithNoController))
	{
		// 停止主动转向状态
		if (bProactiveRotating)
		{
			bProactiveRotating = false;
			// 通知到行为组件，停止主动转向
			UBXStateFunctionLibrary::StopBehavior(GetOwner(), BXGameplayTags::BXBehavior_Locomotion_Rotate);
		}
		
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

		// 开始主动转向状态
		if (!bProactiveRotating)
		{
			bProactiveRotating = true;
			// 通知到行为组件，开始主动转向
			UBXStateFunctionLibrary::StartBehavior(GetOwner(), BXGameplayTags::BXBehavior_Locomotion_Rotate);
		}
	}
	else
	{
		// 停止主动转向状态
		if (bProactiveRotating)
		{
			bProactiveRotating = false;
			// 通知到行为组件，停止主动转向
			UBXStateFunctionLibrary::StopBehavior(GetOwner(), BXGameplayTags::BXBehavior_Locomotion_Rotate);
		}
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

	// 通知到行为组件，落地
	LandedEvent.Broadcast(Hit);
	UBXStateFunctionLibrary::StartBehaviorWithParameter<FHitResult>(GetOwner(), BXGameplayTags::BXImmBehavior_Locomotion_Landed, Hit);
	
	StartNewPhysics(remainingTime, Iterations);
}

bool UBXCharacterMovementComponent::CanAttemptJump() const
{
	return UBXStateFunctionLibrary::CheckForbiddenBehavior(GetOwner(), BXGameplayTags::BXImmBehavior_Locomotion_Jump) && !bWantsToCrouch && IsMovingOnGround();
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
		UBXStateFunctionLibrary::StartBehavior(GetOwner(), BXGameplayTags::BXImmBehavior_Locomotion_Jump);
		
		return true;
	}
	
	return false;
}

#pragma endregion Override



#pragma region Record
FTransform UBXCharacterMovementComponent::GetHistoryTransformByTime(float InTime)
{
	for (TArray<FBXTrajectoryPoint>::TIterator It(TrajectoryPoints); It; ++It)
	{
		if (It->Time >= InTime)
		{
			int32 CurrentIndex = It.GetIndex();
			if (TrajectoryPoints.IsValidIndex(CurrentIndex - 1))
			{
				FTransform Start = TrajectoryPoints[CurrentIndex - 1].Transform;
				float StartTime = TrajectoryPoints[CurrentIndex - 1].Time;
				
				FTransform End = TrajectoryPoints[CurrentIndex].Transform;
				float EndTime = TrajectoryPoints[CurrentIndex].Time;
				
				return FTransform
				(
					FQuat::Slerp(Start.GetRotation(), End.GetRotation(), (InTime - StartTime) / (EndTime - StartTime + 1e-8)),
					FMath::Lerp(Start.GetLocation(), End.GetLocation(), (InTime - StartTime) / (EndTime - StartTime + 1e-8)),
					FMath::Lerp(Start.GetScale3D(), End.GetScale3D(), (InTime - StartTime) / (EndTime - StartTime + 1e-8))
				);
			}
			else
			{
				return It->Transform;
			}
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
}

#pragma endregion Record
