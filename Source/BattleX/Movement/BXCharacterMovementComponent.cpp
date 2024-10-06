#include "BXCharacterMovementComponent.h"

#include "GameFramework/Character.h"



#pragma region State
bool UBXCharacterMovementComponent::IsProactiveMovement() const
{
	return AllowProactiveMovement() && GetLastInputVector().SizeSquared() > 0.0f;
}

void UBXCharacterMovementComponent::ChangeDisableProactiveMovement(int64 InSign, bool bDisable)
{
	if (bDisable)
	{
		DisableProactiveMovement.AddUnique(InSign);
	}
	else
	{
		DisableProactiveMovement.RemoveSwap(InSign);
	}
}

bool UBXCharacterMovementComponent::AllowProactiveMovement() const
{
	return DisableProactiveMovement.Num() <= 0;
}

void UBXCharacterMovementComponent::ChangeDisableProactiveRotation(int64 InSign, bool bDisable)
{
	if (bDisable)
	{
		DisableProactiveRotation.AddUnique(InSign);
	}
	else
	{
		DisableProactiveRotation.RemoveSwap(InSign);
	}
}

bool UBXCharacterMovementComponent::AllowProactiveRotation() const
{
	return DisableProactiveRotation.Num() <= 0;
}

void UBXCharacterMovementComponent::ChangeDisableProactiveJump(int64 InSign, bool bDisable)
{
	if (bDisable)
	{
		DisableProactiveJump.AddUnique(InSign);
	}
	else
	{
		DisableProactiveJump.RemoveSwap(InSign);
	}

	SetJumpAllowed(DisableProactiveJump.Num() <= 0);
}

bool UBXCharacterMovementComponent::AllowProactiveJump() const
{
	return IsJumpAllowed();
}

void UBXCharacterMovementComponent::ChangeDisableMeshOffset(int64 InSign, bool bDisable)
{
	if (bDisable)
	{
		DisableMeshOffset.AddUnique(InSign);
	}
	else
	{
		DisableMeshOffset.RemoveSwap(InSign);
	}
}

bool UBXCharacterMovementComponent::AllowMeshOffset() const
{
	return DisableMeshOffset.Num() <= 0;
}

#pragma endregion State



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
	if (!AllowProactiveMovement())
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
}

FVector UBXCharacterMovementComponent::ComputeSlideVector(const FVector& Delta, const float Time, const FVector& Normal, const FHitResult& Hit) const
{
	if (!AllowProactiveMovement())
	{
		return Delta;
	}
	
	return Super::ComputeSlideVector(Delta, Time, Normal, Hit);
}

void UBXCharacterMovementComponent::PhysicsRotation(float DeltaTime)
{
	if (!AllowProactiveRotation())
	{
		return;
	}
	
	return Super::PhysicsRotation(DeltaTime);
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

	LandedEvent.Broadcast(Hit);

	StartNewPhysics(remainingTime, Iterations);
}

bool UBXCharacterMovementComponent::CanAttemptJump() const
{
	return AllowProactiveJump() && !bWantsToCrouch && IsMovingOnGround();
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

		DoJumpEvent.Broadcast();
		
		return true;
	}
	
	return false;
}

#pragma endregion Override
