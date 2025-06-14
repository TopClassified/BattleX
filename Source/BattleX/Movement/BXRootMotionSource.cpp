#include "BXRootMotionSource.h"



static float EvaluateFloatCurveAtFraction(const UCurveFloat& Curve, const float Fraction)
{
	float MinCurveTime(0.0);
	float MaxCurveTime(1.0f);

	Curve.GetTimeRange(MinCurveTime, MaxCurveTime);
	return Curve.GetFloatValue(FMath::GetRangeValue(FVector2f(MinCurveTime, MaxCurveTime), Fraction));
}






FBXRootMotionSource_Uppercut::FBXRootMotionSource_Uppercut() : Direction(ForceInitToZero), Distance(-1.0f), Height(-1.0f), MoveTimeMappingCurve(nullptr), bChangeOrientation(false), StartOrientation(ForceInitToZero), EndOrientation(ForceInitToZero), RotateTimeMappingCurve(nullptr)
{
	Settings.SetFlag(ERootMotionSourceSettingsFlags::DisablePartialEndTick);
	Settings.SetFlag(ERootMotionSourceSettingsFlags::UseSensitiveLiftoffCheck);
}

FRootMotionSource* FBXRootMotionSource_Uppercut::Clone() const
{
	FBXRootMotionSource_Uppercut* CopyPtr = new FBXRootMotionSource_Uppercut(*this);
	return CopyPtr;
}

bool FBXRootMotionSource_Uppercut::Matches(const FRootMotionSource* Other) const
{
	if (!FRootMotionSource::Matches(Other))
	{
		return false;
	}

	const FBXRootMotionSource_Uppercut* OtherCast = static_cast<const FBXRootMotionSource_Uppercut*>(Other);

	return Direction.Equals(OtherCast->Direction, 1.0f) &&
		EndOrientation.Equals(OtherCast->EndOrientation, 1.0f) &&
		FMath::IsNearlyEqual(Distance, OtherCast->Distance, UE_SMALL_NUMBER) &&
		FMath::IsNearlyEqual(Height, OtherCast->Height, UE_SMALL_NUMBER) &&
		(MoveTimeMappingCurve == OtherCast->MoveTimeMappingCurve) &&
		(bChangeOrientation == OtherCast->bChangeOrientation) &&
		(RotateTimeMappingCurve == OtherCast->RotateTimeMappingCurve);
}

bool FBXRootMotionSource_Uppercut::MatchesAndHasSameState(const FRootMotionSource* Other) const
{
	if (!FRootMotionSource::MatchesAndHasSameState(Other))
	{
		return false;
	}

	return true;
}

bool FBXRootMotionSource_Uppercut::UpdateStateFrom(const FRootMotionSource* SourceToTakeStateFrom, bool bMarkForSimulatedCatchup)
{
	if (!FRootMotionSource::UpdateStateFrom(SourceToTakeStateFrom, bMarkForSimulatedCatchup))
	{
		return false;
	}

	return true;
}

FVector FBXRootMotionSource_Uppercut::GetPathOffset(const float MoveFraction) const
{
	FVector PathOffset(FVector::ZeroVector);
	
	const float Phi = MoveFraction - 1.0f;
	const float Z = 1.0f - (Phi * Phi);
	PathOffset.Z = Z;

	if (Height >= 0.0f)
	{
		PathOffset.Z = PathOffset.Z * Height;
	}

	return PathOffset;
}

FVector FBXRootMotionSource_Uppercut::GetRelativeLocation(float MoveFraction) const
{
	FVector RelativeLocationFacingSpace = FVector(MoveFraction * Distance, 0.0f, 0.0f) + GetPathOffset(MoveFraction);

	return Direction.RotateVector(RelativeLocationFacingSpace);
}

void FBXRootMotionSource_Uppercut::PrepareRootMotion(float SimulationTime, float MovementTickTime, const ACharacter& Character, const UCharacterMovementComponent& MoveComponent)
{
	RootMotionParams.Clear();

	if (Duration > UE_SMALL_NUMBER && MovementTickTime > UE_SMALL_NUMBER && SimulationTime > UE_SMALL_NUMBER)
	{
		float CurrentTimeFraction = GetTime() / Duration;
		float TargetTimeFraction = (GetTime() + SimulationTime) / Duration;

		if (TargetTimeFraction > 1.0f)
		{
			float TimeFractionPastAllowable = TargetTimeFraction - 1.0f;
			TargetTimeFraction = TargetTimeFraction - TimeFractionPastAllowable;
			CurrentTimeFraction = CurrentTimeFraction - TimeFractionPastAllowable;
		}

		float CurrentMoveFraction = CurrentTimeFraction;
		float TargetMoveFraction = TargetTimeFraction;
		if (MoveTimeMappingCurve)
		{
			CurrentMoveFraction = EvaluateFloatCurveAtFraction(*MoveTimeMappingCurve, CurrentMoveFraction);
			TargetMoveFraction = EvaluateFloatCurveAtFraction(*MoveTimeMappingCurve, TargetMoveFraction);
		}
		const FVector CurrentRelativeLocation = GetRelativeLocation(CurrentMoveFraction);
		const FVector TargetRelativeLocation = GetRelativeLocation(TargetMoveFraction);

		const FVector Force = (TargetRelativeLocation - CurrentRelativeLocation) / MovementTickTime;
		FTransform NewTransform(Force);

		if (bChangeOrientation)
		{
			float CurrentRotateFraction = CurrentTimeFraction;
			float TargetRotateFraction = TargetTimeFraction;
			if (RotateTimeMappingCurve)
			{
				CurrentRotateFraction = EvaluateFloatCurveAtFraction(*RotateTimeMappingCurve, CurrentRotateFraction);
				TargetRotateFraction = EvaluateFloatCurveAtFraction(*RotateTimeMappingCurve, TargetRotateFraction);
			}
			FQuat StartQuaternion = StartOrientation.Quaternion();
			FQuat EndQuaternion = EndOrientation.Quaternion();
			FQuat CurrentQuaternion = FQuat::Slerp(StartQuaternion, EndQuaternion, CurrentRotateFraction);
			FQuat TargetQuaternion = FQuat::Slerp(StartQuaternion, EndQuaternion, TargetRotateFraction);

			NewTransform.SetRotation(CurrentQuaternion * TargetQuaternion.Inverse());
		}
		
		RootMotionParams.Set(NewTransform);
	}
	else
	{
		checkf(Duration > UE_SMALL_NUMBER, TEXT("FBXRootMotionSource_Uppercut prepared with invalid duration."));
	}

	SetTime(GetTime() + SimulationTime);
}

bool FBXRootMotionSource_Uppercut::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	if (!FRootMotionSource::NetSerialize(Ar, Map, bOutSuccess))
	{
		return false;
	}

	Ar << Direction;
	Ar << Distance;
	Ar << Height;
	Ar << MoveTimeMappingCurve;
	Ar << bChangeOrientation;
	Ar << StartOrientation;
	Ar << EndOrientation;
	Ar << RotateTimeMappingCurve;

	bOutSuccess = true;
	return true;
}

UScriptStruct* FBXRootMotionSource_Uppercut::GetScriptStruct() const
{
	return FBXRootMotionSource_Uppercut::StaticStruct();
}

FString FBXRootMotionSource_Uppercut::ToSimpleString() const
{
	return FString::Printf(TEXT("[ID:%u]FBXRootMotionSource_Uppercut %s"), LocalID, *InstanceName.GetPlainNameString());
}

void FBXRootMotionSource_Uppercut::AddReferencedObjects(class FReferenceCollector& Collector)
{
	Collector.AddReferencedObject(MoveTimeMappingCurve);
	Collector.AddReferencedObject(RotateTimeMappingCurve);

	FRootMotionSource::AddReferencedObjects(Collector);
}