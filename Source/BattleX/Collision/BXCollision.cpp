#include "BXCollision.h"

#include "Engine/OverlapResult.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/LineBatchComponent.h"

#include "BXSettings.h"
#include "BXFunctionLibrary.h"
#include "BXGearStructs.h"

#if WITH_EDITOR
#include "DrawDebugHelpers.h"
#endif



#define MiniSize 1e-6
DEFINE_LOG_CATEGORY(BXCOLLISION);



namespace BXCollisionHelper
{
	FCollisionObjectQueryParams ConfigureCollisionObjectParams(const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes)
	{
		TArray<TEnumAsByte<ECollisionChannel>> CollisionObjectTraces;
		CollisionObjectTraces.AddUninitialized(ObjectTypes.Num());

		for (auto Iter = ObjectTypes.CreateConstIterator(); Iter; ++Iter)
		{
			CollisionObjectTraces[Iter.GetIndex()] = UEngineTypes::ConvertToCollisionChannel(*Iter);
		}

		FCollisionObjectQueryParams ObjectParams;
		for (auto Iter = CollisionObjectTraces.CreateConstIterator(); Iter; ++Iter)
		{
			const ECollisionChannel& Channel = (*Iter);
			if (FCollisionObjectQueryParams::IsValidObjectQuery(Channel))
			{
				ObjectParams.AddObjectTypesToQuery(Channel);
			}
		}

		return ObjectParams;
	}
}



bool UBXCollisionLibrary::CheckCollisionResult(AActor* InRequester, const FHitResult& InResult, const FBXCFilter& InFilter)
{
	// 检查碰撞信息是否有效
	if (!InResult.GetActor() || !InResult.GetComponent())
	{
		return false;
	}

	// 忽略自身
	if (InFilter.bIgnoreSelf && InRequester == InResult.GetActor())
	{
		return false;
	}

	// 检查Class
	UClass* CurClassType = InResult.GetActor()->GetClass();
	if (InFilter.ClassTypes.Num() > 0)
	{
		bool ClassFlag = false;
		for (int32 j = 0; j < InFilter.ClassTypes.Num(); ++j)
		{
			if (InFilter.ClassTypes[j].IsValid() && CurClassType->IsChildOf(InFilter.ClassTypes[j].Get()))
			{
				ClassFlag = true;
				break;
			}
		}
		if (!ClassFlag)
		{
			return false;
		}
	}

	// 检查需要忽略的Class
	if (InFilter.IgnoreClassTypes.Num() > 0)
	{
		for (int32 j = 0; j < InFilter.IgnoreClassTypes.Num(); ++j)
		{
			// 如果是其子类，则失败
			if (InFilter.IgnoreClassTypes[j].IsValid() && CurClassType->IsChildOf(InFilter.IgnoreClassTypes[j].Get()))
			{
				return false;
			}
		}
	}

	// 检查Actor的Tag
	if (InFilter.ActorTags.Num() > 0)
	{
		bool TagFlag = false;
		// 检查Tag是否合法
		for (int32 i = 0; i < InFilter.ActorTags.Num(); ++i)
		{
			if (InResult.GetActor()->ActorHasTag(InFilter.ActorTags[i]))
			{
				TagFlag = true;
				break;
			}
		}
		if (!TagFlag)
		{
			return false;
		}
	}

	// 检查需要忽略的Actor Tag
	if (InFilter.IgnoreActorTags.Num() > 0)
	{
		// 如果检查Tag成功，则失败
		for (int32 i = 0; i < InFilter.IgnoreActorTags.Num(); ++i)
		{
			if (InResult.GetActor()->ActorHasTag(InFilter.IgnoreActorTags[i]))
			{
				return false;
			}
		}
	}

	// 检查Component的Tag
	if (InFilter.ComponentTags.Num() > 0)
	{
		bool CompFlag = false;
		// 检查Tag是否合法
		for (int32 i = 0; i < InFilter.ComponentTags.Num(); ++i)
		{
			if (InResult.GetComponent()->ComponentHasTag(InFilter.ComponentTags[i]))
			{
				CompFlag = true;
				break;
			}
		}
		if (!CompFlag)
		{
			return false;
		}
	}

	// 检查需要忽略的Component Tag
	if (InFilter.IgnoreComponentTags.Num() > 0)
	{
		// 如果检查Tag成功，则失败
		for (int32 i = 0; i < InFilter.IgnoreComponentTags.Num(); ++i)
		{
			if (InResult.GetComponent()->ComponentHasTag(InFilter.IgnoreComponentTags[i]))
			{
				return false;
			}
		}
	}

	return true;
}

void UBXCollisionLibrary::CombineCollisionResults(const TArray<FHitResult>& InSourceList, TArray<FHitResult>& InOutList)
{
	// 用 (Component, BoneName) 作为键去重,避免 O(N*M) 双重循环
	// 仅与 InOutList 比对,InSourceList 内部重复项保留原行为
	TMap<UPrimitiveComponent*, TSet<FName>> Visited;
	for (const FHitResult& HR : InOutList)
	{
		Visited.FindOrAdd(HR.GetComponent()).Add(HR.BoneName);
	}

	for (const FHitResult& HR : InSourceList)
	{
		TSet<FName>& BoneSet = Visited.FindOrAdd(HR.GetComponent());
		if (BoneSet.Contains(HR.BoneName))
		{
			continue;
		}

		InOutList.Add(HR);
	}
}

TArray<FHitResult> UBXCollisionLibrary::SphereCheck(const FBXCParameter& Parameter, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, float SphereSize, const FBXCFilter& Filter)
{
	TArray<FHitResult> OutResult;

	if (SphereSize < MiniSize)
	{
		UE_LOG(BXCOLLISION, Warning, TEXT("Sphere Size Is Too Small, Please Check The Input Value!"));
		return OutResult;
	}

	UWorld* QueryWorld = Parameter.Requester ? Parameter.Requester->GetWorld() : nullptr;
	if (!QueryWorld)
	{
		return OutResult;
	}

	FCollisionObjectQueryParams OQP = BXCollisionHelper::ConfigureCollisionObjectParams(ObjectTypes);
	if (!OQP.IsValid())
	{
		return OutResult;
	}

	float CurrentSphereSize = SphereSize * Parameter.Scale.X;

	FCollisionShape Sphere = FCollisionShape::MakeSphere(CurrentSphereSize);

	if (Parameter.EndLocation == Parameter.StartLocation)
	{
		TArray<FOverlapResult> OverlapResults;
		QueryWorld->OverlapMultiByObjectType(OverlapResults, Parameter.StartLocation, Parameter.StartRotation.Quaternion(), OQP, Sphere);

#if WITH_EDITOR
		if (const UBXSettings* Setting = GetDefault<UBXSettings>())
		{
			if (Setting->CollisionDebugDuration > 0.0f)
			{
				DrawDebugSphere(QueryWorld, Parameter.StartLocation, CurrentSphereSize, 32, FColor::Red, false, Setting->CollisionDebugDuration);
			}
		}
#endif

		for (int32 i = 0; i < OverlapResults.Num(); ++i)
		{
			if (!OverlapResults[i].GetComponent())
			{
				continue;
			}

			FHitResult HR(OverlapResults[i].GetActor(), OverlapResults[i].GetComponent(), Parameter.StartLocation, FVector::ZeroVector);
			if (UBXCollisionLibrary::CheckCollisionResult(Parameter.Requester, HR, Filter))
			{
				OutResult.Add(HR);
			}
		}
	}
	else
	{
		FCollisionQueryParams CQP;

		QueryWorld->SweepMultiByObjectType(OutResult, Parameter.StartLocation, Parameter.EndLocation, FQuat(), OQP, Sphere, CQP);

		for (TArray<FHitResult>::TIterator It(OutResult); It; ++It)
		{
			if (!UBXCollisionLibrary::CheckCollisionResult(Parameter.Requester, *It, Filter))
			{
				It.RemoveCurrent();
			}
		}

#if WITH_EDITOR
		if (const UBXSettings* Setting = GetDefault<UBXSettings>())
		{
			if (Setting->CollisionDebugDuration > 0.0f)
			{
				DrawDebugSphere(QueryWorld, Parameter.StartLocation, CurrentSphereSize, 32, FColor::Red, false, Setting->CollisionDebugDuration, 5.0f);
				UBXFunctionLibrary::DrawDebugSweepSphereInEditor(QueryWorld, Parameter.StartLocation, Parameter.EndLocation, Sphere.GetSphereRadius(), FColor::Blue, false, Setting->CollisionDebugDuration, 0.0f);
			}
		}
#endif
	}

	return OutResult;
}

TArray<FHitResult> UBXCollisionLibrary::CapsuleCheck(const FBXCParameter& Parameter, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, FVector2D CapsuleSize, const FBXCFilter& Filter, float AngleStep)
{
	TArray<FHitResult> OutResult;

	UWorld* QueryWorld = Parameter.Requester ? Parameter.Requester->GetWorld() : nullptr;
	if (!QueryWorld)
	{
		return OutResult;
	}

	FCollisionObjectQueryParams OQP = BXCollisionHelper::ConfigureCollisionObjectParams(ObjectTypes);
	if (!OQP.IsValid())
	{
		return OutResult;
	}

	FVector2D CurrentCapsuleSize = CapsuleSize * FVector2D(Parameter.Scale.X, Parameter.Scale.Y);
	CurrentCapsuleSize.Y = FMath::Max(CurrentCapsuleSize.Y, CurrentCapsuleSize.X);
	if (CurrentCapsuleSize.X < MiniSize)
	{
		UE_LOG(BXCOLLISION, Warning, TEXT("Capsule Size Is Invalid, Please Check The Input Value!"));
		return OutResult;
	}

	FCollisionShape Capsule = FCollisionShape::MakeCapsule(CurrentCapsuleSize.X, CurrentCapsuleSize.Y);

	if (Parameter.EndLocation == Parameter.StartLocation)
	{
		TArray<FOverlapResult> OverlapResults;
		QueryWorld->OverlapMultiByObjectType(OverlapResults, Parameter.StartLocation, Parameter.StartRotation.Quaternion(), OQP, Capsule);

#if WITH_EDITOR
		if (const UBXSettings* Setting = GetDefault<UBXSettings>())
		{
			if (Setting->CollisionDebugDuration > 0.0f)
			{
				DrawDebugCapsule(QueryWorld, Parameter.StartLocation, CurrentCapsuleSize.Y, CurrentCapsuleSize.X, Parameter.StartRotation.Quaternion(), FColor::Red, false, Setting->CollisionDebugDuration);
			}
		}
#endif

		for (int32 i = 0; i < OverlapResults.Num(); ++i)
		{
			if (!OverlapResults[i].GetComponent())
			{
				continue;
			}

			FHitResult HR(OverlapResults[i].GetActor(), OverlapResults[i].GetComponent(), Parameter.StartLocation, FVector::ZeroVector);
			if (UBXCollisionLibrary::CheckCollisionResult(Parameter.Requester, HR, Filter))
			{
				OutResult.Add(HR);
			}
		}
	}
	else
	{
		FCollisionQueryParams CQP;

		FQuat StartQuaternion = Parameter.StartRotation.Quaternion();
		FQuat EndQuaternion = Parameter.EndRotation.Quaternion();
		float DeltaDegree = FMath::RadiansToDegrees(StartQuaternion.AngularDistance(EndQuaternion));
		if (DeltaDegree < 1.0f)
		{
			QueryWorld->SweepMultiByObjectType(OutResult, Parameter.StartLocation, Parameter.EndLocation, StartQuaternion, OQP, Capsule, CQP);

			for (TArray<FHitResult>::TIterator It(OutResult); It; ++It)
			{
				if (!UBXCollisionLibrary::CheckCollisionResult(Parameter.Requester, *It, Filter))
				{
					It.RemoveCurrent();
				}
			}

#if WITH_EDITOR
			if (const UBXSettings* Setting = GetDefault<UBXSettings>())
			{
				if (Setting->CollisionDebugDuration > 0.0f)
				{
					DrawDebugCapsule(QueryWorld, Parameter.StartLocation, CurrentCapsuleSize.Y, CurrentCapsuleSize.X, StartQuaternion, FColor::Red, false, Setting->CollisionDebugDuration);
					UBXFunctionLibrary::DrawDebugSweepBoxInEditor(QueryWorld, Parameter.StartLocation, Parameter.EndLocation, StartQuaternion.Rotator(), FVector(CurrentCapsuleSize.X, CurrentCapsuleSize.X, CurrentCapsuleSize.Y), FColor::Blue, false, Setting->CollisionDebugDuration, 0.0f);
				}
			}
#endif
		}
		else
		{
			if (AngleStep < 1.0f)
			{
				AngleStep = 30.0f;
			}

			TSet<UPrimitiveComponent*> VisitedComponents;
			TArray<FHitResult> CurrentResult;
			int32 TotalStep = FMath::CeilToInt(DeltaDegree / AngleStep) + 1;
			for (int32 i = 0; i < TotalStep; ++i)
			{
				FQuat CurrentQuat = FQuat::Slerp(StartQuaternion, EndQuaternion, (i * 1.0f) / (TotalStep * 1.0f - 1.0f));

				FVector Location1 = FMath::Lerp(Parameter.StartLocation, Parameter.EndLocation, (i * 1.0f) / (TotalStep * 1.0f));
				FVector Location2 = FMath::Lerp(Parameter.StartLocation, Parameter.EndLocation, (i + 1.0f) / (TotalStep * 1.0f));

				QueryWorld->SweepMultiByObjectType(CurrentResult, Location1, Location2, CurrentQuat, OQP, Capsule, CQP);

				for (int32 j = 0; j < CurrentResult.Num(); ++j)
				{
					const FHitResult& HR = CurrentResult[j];
					if (!HR.GetComponent() || VisitedComponents.Contains(HR.GetComponent()))
					{
						continue;
					}

					VisitedComponents.Add(HR.GetComponent());

					if (UBXCollisionLibrary::CheckCollisionResult(Parameter.Requester, HR, Filter))
					{
						OutResult.Add(HR);
					}
				}

#if WITH_EDITOR
				if (const UBXSettings* Setting = GetDefault<UBXSettings>())
				{
					if (Setting->CollisionDebugDuration > 0.0f)
					{
						DrawDebugCapsule(QueryWorld, Location1, CurrentCapsuleSize.Y, CurrentCapsuleSize.X, CurrentQuat, FColor::Red, false, Setting->CollisionDebugDuration);
						UBXFunctionLibrary::DrawDebugSweepBoxInEditor(QueryWorld, Location1, Location2, CurrentQuat.Rotator(), FVector(CurrentCapsuleSize.X, CurrentCapsuleSize.X, CurrentCapsuleSize.Y), FColor::Blue, false, Setting->CollisionDebugDuration, 0.0f);
					}
				}
#endif
			}
		}
	}	

	return OutResult;
}

TArray<FHitResult> UBXCollisionLibrary::CylinderCheck(const FBXCParameter& Parameter, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, FVector2D CylinderSize, const FBXCFilter& Filter)
{
	TArray<FHitResult> OutResult;

	UWorld* QueryWorld = Parameter.Requester ? Parameter.Requester->GetWorld() : nullptr;
	if (!QueryWorld)
	{
		return OutResult;
	}

	FCollisionObjectQueryParams OQP = BXCollisionHelper::ConfigureCollisionObjectParams(ObjectTypes);
	if (!OQP.IsValid())
	{
		return OutResult;
	}

	FVector2D CurrentCapsuleSize = CylinderSize * FVector2D(Parameter.Scale.X, Parameter.Scale.Y);
	CurrentCapsuleSize.Y = FMath::Max(CurrentCapsuleSize.Y, CurrentCapsuleSize.X);
	if (CurrentCapsuleSize.X < MiniSize)
	{
		UE_LOG(BXCOLLISION, Warning, TEXT("Cylinder Size Is Too Small, Please Check The Input Value!"));
		return OutResult;
	}

	FVector CurrentBoxSize(CylinderSize.X, CylinderSize.X, CylinderSize.Y);
	CurrentBoxSize = CurrentBoxSize * FVector(Parameter.Scale.X, Parameter.Scale.X, Parameter.Scale.Y);
	if (CurrentBoxSize.X < MiniSize || CurrentBoxSize.Y < MiniSize || CurrentBoxSize.Z < MiniSize)
	{
		UE_LOG(BXCOLLISION, Warning, TEXT("Cylinder Size Is Too Small, Please Check The Input Value!"));
		return OutResult;
	}

	FCollisionShape Capsule = FCollisionShape::MakeCapsule(CurrentCapsuleSize.X, CurrentCapsuleSize.Y);
	TArray<FOverlapResult> OverlapResults1;
	QueryWorld->OverlapMultiByObjectType(OverlapResults1, Parameter.StartLocation, Parameter.StartRotation.Quaternion(), OQP, Capsule);

	FCollisionShape Box = FCollisionShape::MakeBox(CurrentBoxSize);
	TArray<FOverlapResult> OverlapResults2;
	QueryWorld->OverlapMultiByObjectType(OverlapResults2, Parameter.StartLocation, Parameter.StartRotation.Quaternion(), OQP, Box);

#if WITH_EDITOR
	if (const UBXSettings* Setting = GetDefault<UBXSettings>())
	{
		if (Setting->CollisionDebugDuration > 0.0f)
		{
			DrawDebugBox(QueryWorld, Parameter.StartLocation, CurrentBoxSize, Parameter.StartRotation.Quaternion(), FColor::Red, false, Setting->CollisionDebugDuration);
			DrawDebugCapsule(QueryWorld, Parameter.StartLocation, CurrentCapsuleSize.Y, CurrentCapsuleSize.X, Parameter.StartRotation.Quaternion(), FColor::Red, false, Setting->CollisionDebugDuration);
		}
	}
#endif

	// 两次碰撞都得命中
	for (int32 i = 0; i < OverlapResults1.Num(); ++i)
	{
		if (!OverlapResults1[i].GetComponent())
		{
			continue;
		}

		bool Flag = false;
		for (int32 j = 0; j < OverlapResults2.Num(); ++j)
		{
			if (OverlapResults2[j].GetComponent() == OverlapResults1[i].GetComponent())
			{
				Flag = true;
				break;
			}
		}
		if (!Flag)
		{
			continue;
		}

		FHitResult HR(OverlapResults1[i].GetActor(), OverlapResults1[i].GetComponent(), Parameter.StartLocation, FVector::ZeroVector);
		if (UBXCollisionLibrary::CheckCollisionResult(Parameter.Requester, HR, Filter))
		{
			OutResult.Add(HR);
		}
	}

	return OutResult;
}

TArray<FHitResult> UBXCollisionLibrary::HollowCylinderCheck(const FBXCParameter& Parameter, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, FVector HollowCylinderSize, const FBXCFilter& Filter, float AngleStep)
{
	TArray<FHitResult> OutResult;

	UWorld* QueryWorld = Parameter.Requester ? Parameter.Requester->GetWorld() : nullptr;
	if (!QueryWorld)
	{
		return OutResult;
	}

	FCollisionObjectQueryParams OQP = BXCollisionHelper::ConfigureCollisionObjectParams(ObjectTypes);
	if (!OQP.IsValid())
	{
		return OutResult;
	}

	FVector CurrentHollowCylinderSize = HollowCylinderSize * FVector(Parameter.Scale.X, Parameter.Scale.Y, Parameter.Scale.Z);
	if (CurrentHollowCylinderSize.X < MiniSize || CurrentHollowCylinderSize.Y <= CurrentHollowCylinderSize.X || CurrentHollowCylinderSize.Z < MiniSize)
	{
		UE_LOG(BXCOLLISION, Warning, TEXT("Hollow Cylinder Size Is Invalid, Please Check The Input Value!"));
		return OutResult;
	}

	if (AngleStep < 1.0f)
	{
		AngleStep = 30.0f;
	}

	FTransform Transform = FTransform(Parameter.StartRotation, Parameter.StartLocation);

	int32 StepNum = FMath::CeilToInt(360.0f / AngleStep - MiniSize);
	float DeltaAngle = 360.0f / StepNum;

	FVector CurrentBoxSize((CurrentHollowCylinderSize.Y - CurrentHollowCylinderSize.X) * 0.5f, CurrentHollowCylinderSize.Y * FMath::Sin(FMath::DegreesToRadians(DeltaAngle * 0.5f)), CurrentHollowCylinderSize.Z);
	FCollisionShape Box = FCollisionShape::MakeBox(CurrentBoxSize);
	float OffsetLength = CurrentHollowCylinderSize.X + CurrentBoxSize.X;

	TArray<FOverlapResult> OverlapResults;
	TSet<UPrimitiveComponent*> VisitedComponents;
	for (int32 i = 0; i < StepNum; ++i)
	{
		OverlapResults.Reset();

		float Angle = DeltaAngle * i;

		FVector Location(OffsetLength * FMath::Cos(FMath::DegreesToRadians(Angle)), OffsetLength * FMath::Sin(FMath::DegreesToRadians(Angle)), 0.0f);
		Location = Transform.TransformPosition(Location);

		FQuat Rotation = Transform.TransformRotation(FRotator(0.0f, Angle, 0.0f).Quaternion());

		QueryWorld->OverlapMultiByObjectType(OverlapResults, Location, Rotation, OQP, Box);

		for (int32 j = 0; j < OverlapResults.Num(); ++j)
		{
			const FOverlapResult& OR = OverlapResults[j];

			if (!OR.GetComponent() || VisitedComponents.Contains(OR.GetComponent()))
			{
				continue;
			}

			VisitedComponents.Add(OR.GetComponent());

			FHitResult HR(OR.GetActor(), OR.GetComponent(), Location, FVector::ZeroVector);
			if (UBXCollisionLibrary::CheckCollisionResult(Parameter.Requester, HR, Filter))
			{
				OutResult.Add(HR);
			}
		}

#if WITH_EDITOR
		if (const UBXSettings* Setting = GetDefault<UBXSettings>())
		{
			if (Setting->CollisionDebugDuration > 0.0f)
			{
				DrawDebugBox(QueryWorld, Location, CurrentBoxSize, Rotation, FColor::Blue, false, Setting->CollisionDebugDuration);
			}
		}
#endif
	}

#if WITH_EDITOR
	if (const UBXSettings* Setting = GetDefault<UBXSettings>())
	{
		if (Setting->CollisionDebugDuration > 0.0f)
		{
			FMatrix TransformMatrix = FTransform(Transform.Rotator(), Transform.GetLocation()).ToMatrixNoScale();
			ULineBatchComponent* const LineBatcher = QueryWorld->PersistentLineBatcher;
			if (LineBatcher != NULL)
			{
				const float LineLifeTime = Setting->CollisionDebugDuration;

				float Segments = 32;

				// Need at least 4 segments
				float Step = 360.0f / float(Segments);
				Step = FMath::DegreesToRadians(Step);

				const FVector Center = TransformMatrix.GetOrigin();
				const FVector AxisX = TransformMatrix.GetScaledAxis(EAxis::X);
				const FVector AxisY = TransformMatrix.GetScaledAxis(EAxis::Y);

				TArray<FBatchedLine> Lines;
				Lines.Empty(Segments);

				float Angle = 90.0f;
				Angle = FMath::DegreesToRadians(Angle);

				float FBigRadius = CurrentHollowCylinderSize.Y;
				float FSmallRadius = CurrentHollowCylinderSize.X;
				while (Segments--)
				{
					const FVector VertexBig1 = Center + FBigRadius * (AxisY * FMath::Cos(Angle) + AxisX * FMath::Sin(Angle));
					const FVector VertexSmall1 = Center + FSmallRadius * (AxisY * FMath::Cos(Angle) + AxisX * FMath::Sin(Angle));
					Angle += Step;
					const FVector VertexBig2 = Center + FBigRadius * (AxisY * FMath::Cos(Angle) + AxisX * FMath::Sin(Angle));
					const FVector VertexSmall2 = Center + FSmallRadius * (AxisY * FMath::Cos(Angle) + AxisX * FMath::Sin(Angle));
					Lines.Add(FBatchedLine(VertexBig1, VertexBig2, FColor::Red, LineLifeTime, 0.0f, false));
					Lines.Add(FBatchedLine(VertexSmall1, VertexSmall2, FColor::Red, LineLifeTime, 0.0f, false));
				}
				LineBatcher->DrawLines(Lines);
			}
		}
	}
#endif

	return OutResult;
}

TArray<FHitResult> UBXCollisionLibrary::BoxCheck(const FBXCParameter& Parameter, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, FVector BoxSize, const FBXCFilter& Filter, float AngleStep)
{
	TArray<FHitResult> OutResult;

	UWorld* QueryWorld = Parameter.Requester ? Parameter.Requester->GetWorld() : nullptr;
	if (!QueryWorld)
	{
		return OutResult;
	}

	FCollisionObjectQueryParams OQP = BXCollisionHelper::ConfigureCollisionObjectParams(ObjectTypes);
	if (!OQP.IsValid())
	{
		return OutResult;
	}

	FVector CurrentBoxSize = BoxSize * Parameter.Scale;
	if (CurrentBoxSize.X < MiniSize || CurrentBoxSize.Y < MiniSize || CurrentBoxSize.Z < MiniSize)
	{
		UE_LOG(BXCOLLISION, Warning, TEXT("Box Size Is Too Small, Please Check The Input Value!"));
		return OutResult;
	}

	FCollisionShape Box = FCollisionShape::MakeBox(CurrentBoxSize);

	if (Parameter.EndLocation == Parameter.StartLocation)
	{
		TArray<FOverlapResult> OverlapResults;
		QueryWorld->OverlapMultiByObjectType(OverlapResults, Parameter.StartLocation, Parameter.StartRotation.Quaternion(), OQP, Box);

#if WITH_EDITOR
		if (const UBXSettings* Setting = GetDefault<UBXSettings>())
		{
			if (Setting->CollisionDebugDuration > 0.0f)
			{
				DrawDebugBox(QueryWorld, Parameter.StartLocation, CurrentBoxSize, Parameter.StartRotation.Quaternion(), FColor::Red, false, Setting->CollisionDebugDuration);
			}
		}
#endif

		for (int32 i = 0; i < OverlapResults.Num(); ++i)
		{
			if (!OverlapResults[i].GetComponent())
			{
				continue;
			}

			FHitResult HR(OverlapResults[i].GetActor(), OverlapResults[i].GetComponent(), Parameter.StartLocation, FVector::ZeroVector);
			if (UBXCollisionLibrary::CheckCollisionResult(Parameter.Requester, HR, Filter))
			{
				OutResult.Add(HR);
			}
		}
	}
	else
	{
		FCollisionQueryParams CQP;

		FQuat StartQuaternion = Parameter.StartRotation.Quaternion();
		FQuat EndQuaternion = Parameter.EndRotation.Quaternion();
		float DeltaDegree = FMath::RadiansToDegrees(StartQuaternion.AngularDistance(EndQuaternion));
		if (DeltaDegree < 1.0f)
		{
			QueryWorld->SweepMultiByObjectType(OutResult, Parameter.StartLocation, Parameter.EndLocation, StartQuaternion, OQP, Box, CQP);

			for (TArray<FHitResult>::TIterator It(OutResult); It; ++It)
			{
				if (!UBXCollisionLibrary::CheckCollisionResult(Parameter.Requester, *It, Filter))
				{
					It.RemoveCurrent();
				}
			}

#if WITH_EDITOR
			if (const UBXSettings* Setting = GetDefault<UBXSettings>())
			{
				if (Setting->CollisionDebugDuration > 0.0f)
				{
					DrawDebugBox(QueryWorld, Parameter.StartLocation, CurrentBoxSize, StartQuaternion, FColor::Red, false, Setting->CollisionDebugDuration, 5.0f);
					UBXFunctionLibrary::DrawDebugSweepBoxInEditor(QueryWorld, Parameter.StartLocation, Parameter.EndLocation, StartQuaternion.Rotator(), CurrentBoxSize, FColor::Blue, false, Setting->CollisionDebugDuration, 0.0f);
				}
			}
#endif
		}
		else
		{
			if (AngleStep < 1.0f)
			{
				AngleStep = 30.0f;
			}

			TSet<UPrimitiveComponent*> VisitedComponents;
			TArray<FHitResult> CurrentResult;
			int32 TotalStep = FMath::CeilToInt(DeltaDegree / AngleStep) + 1;
			for (int32 i = 0; i < TotalStep; ++i)
			{
				FQuat CurrentQuat = FQuat::Slerp(StartQuaternion, EndQuaternion, (i * 1.0f) / (TotalStep * 1.0f - 1.0f));

				FVector Location1 = FMath::Lerp(Parameter.StartLocation, Parameter.EndLocation, (i * 1.0f) / (TotalStep * 1.0f));
				FVector Location2 = FMath::Lerp(Parameter.StartLocation, Parameter.EndLocation, (i + 1.0f) / (TotalStep * 1.0f));

				QueryWorld->SweepMultiByObjectType(CurrentResult, Location1, Location2, CurrentQuat, OQP, Box, CQP);

				for (int32 j = 0; j < CurrentResult.Num(); ++j)
				{
					const FHitResult& HR = CurrentResult[j];
					if (!HR.GetComponent() || VisitedComponents.Contains(HR.GetComponent()))
					{
						continue;
					}

					VisitedComponents.Add(HR.GetComponent());

					if (UBXCollisionLibrary::CheckCollisionResult(Parameter.Requester, HR, Filter))
					{
						OutResult.Add(HR);
					}
				}

#if WITH_EDITOR
				if (const UBXSettings* Setting = GetDefault<UBXSettings>())
				{
					if (Setting->CollisionDebugDuration > 0.0f)
					{
						DrawDebugBox(QueryWorld, Location1, CurrentBoxSize, CurrentQuat, FColor::Red, false, Setting->CollisionDebugDuration);
						UBXFunctionLibrary::DrawDebugSweepBoxInEditor(QueryWorld, Location1, Location2, CurrentQuat.Rotator(), CurrentBoxSize, FColor::Blue, false, Setting->CollisionDebugDuration, 0.0f);
					}
				}
#endif
			}
		}
	}

	return OutResult;
}

TArray<FHitResult> UBXCollisionLibrary::SectorCheck(const FBXCParameter& Parameter, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, FVector4 SectorSize, EBXCDirection Direction, const FBXCFilter& Filter, float AngleStep, int32 SweepStep)
{
	TArray<FHitResult> OutResult;

	UWorld* QueryWorld = Parameter.Requester ? Parameter.Requester->GetWorld() : nullptr;
	if (!QueryWorld)
	{
		return OutResult;
	}

	FCollisionObjectQueryParams OQP = BXCollisionHelper::ConfigureCollisionObjectParams(ObjectTypes);
	if (!OQP.IsValid())
	{
		return OutResult;
	}

	if (AngleStep < 1.0f)
	{
		AngleStep = 30.0f;
	}

	if (SweepStep < 1)
	{
		SweepStep = 10;
	}

	// 计算真正的扇形体大小
	FVector4 SectorParameter = SectorSize;
	SectorParameter.X = SectorParameter.X * Parameter.Scale.X;
	SectorParameter.Y = SectorParameter.Y * Parameter.Scale.Y;
	SectorParameter.W = FMath::Clamp(SectorParameter.W * Parameter.Scale.Z, 0.5f, 179.5f);
	if (SectorParameter.X < MiniSize || SectorParameter.Y <= SectorParameter.X)
	{
		UE_LOG(BXCOLLISION, Warning, TEXT("Sector Size Is Too Small, Please Check The Input Value!"));
		return OutResult;
	}

	FTransform WorldTransform = FTransform(Parameter.StartRotation, Parameter.StartLocation);

	// 扇形的预处理信息
	FTransform LocalTransform = WorldTransform.Inverse();
	FVector FanWorldPos = WorldTransform.GetLocation();
	FVector ForwardVec = WorldTransform.GetRotation().GetForwardVector();
	FVector RightVec = WorldTransform.GetRotation().GetRightVector();
	float FanCos = FMath::Cos(FMath::DegreesToRadians(SectorParameter.W));
	float FanSin = FMath::Sin(FMath::DegreesToRadians(SectorParameter.W));

	// 计算粗略检测的BOX大小
	float BoxX = SectorParameter.Y;
	float BoxY = SectorParameter.Y;
	// 锐角、直角
	if (FanCos >= 0.0f)
	{
		BoxX -= FanCos * SectorParameter.X;
		BoxY *= FanSin * 2.0f;
	}
	// 钝角
	else
	{
		BoxX += FMath::Abs(FanCos) * SectorParameter.Y;
		BoxY *= 2.0f;
	}
	FVector BoxExtend = FVector(BoxX / 2.0f, BoxY / 2.0f, SectorParameter.Z);

	// 计算BOX的真正位置
	FTransform BoxWorldTransform = WorldTransform;
	BoxWorldTransform.SetLocation(WorldTransform.GetLocation() + ForwardVec * (SectorParameter.Y - BoxX / 2.0f));

	// 射线检查信息
	int32 MaxLineTraceNum = FMath::FloorToInt(SectorParameter.W * 2.0f / AngleStep);
	MaxLineTraceNum = (MaxLineTraceNum % 2 == 1) ? MaxLineTraceNum + 1 : MaxLineTraceNum;
	float DeltaDegree = (MaxLineTraceNum == 0) ? SectorParameter.W : SectorParameter.W / (MaxLineTraceNum / 2.0f);
	MaxLineTraceNum += 1;
	// 检测Box宽度的一半
	float SweepBoxSizeY = SectorParameter.Y * FMath::Sin(FMath::DegreesToRadians(DeltaDegree / 2.0f));

	// 使用BOX进行粗略检查
	FCollisionShape Box = FCollisionShape::MakeBox(BoxExtend);
	TArray<FOverlapResult> OverlapResults;
	if (QueryWorld->OverlapMultiByObjectType(OverlapResults, BoxWorldTransform.GetLocation(), WorldTransform.GetRotation(), OQP, Box))
	{
		// 通用的射线检查参数
		FVector EndPoint = FVector::ZeroVector, StartPoint = FVector::ZeroVector;
		TArray<FHitResult> SweepResults;
		FCollisionQueryParams CQP;

		FVector BoxCenter = BoxWorldTransform.GetLocation();
		TArray<UActorComponent*> CollisionComps;
		for (TArray<FOverlapResult>::TIterator It(OverlapResults); It; ++It)
		{
			if (!It->GetComponent())
			{
				It.RemoveCurrent();
				continue;
			}

			FHitResult HR(It->GetActor(), It->GetComponent(), It->GetComponent()->GetComponentLocation(), FVector::ZeroVector);

			if (!UBXCollisionLibrary::CheckCollisionResult(Parameter.Requester, HR, Filter))
			{
				It.RemoveCurrent();
			}
		}

		// 如果存在射线检查没有查询到的组件，则使用大规模Box Sweep来检查
		FCollisionShape SweepBox = FCollisionShape::MakeBox(FVector::ZeroVector);
		for (int32 i = 0; i < MaxLineTraceNum; ++i)
		{
			if (CollisionComps.Num() >= OverlapResults.Num())
			{
				break;
			}

			// 向外扩展的Box射线的Box大小
			if (Direction == EBXCDirection::D_InToOut)
			{
				SweepBox.SetBox(FVector3f(SweepBoxSizeY / SweepStep, SweepBoxSizeY, SectorParameter.Z));
			}
			// 横向扫描的Box射线大小
			else
			{
				SweepBox.SetBox(FVector3f((SectorParameter.Y - SectorParameter.X) / 2.0f, SweepBoxSizeY / SweepStep, SectorParameter.Z));
			}

			// 计算该次步进的方向
			FQuat BoxWorldRot = FQuat::Identity;
			FRotator TmpRot = FRotator(0.0f, i * DeltaDegree, 0.0f);
			if (Direction == EBXCDirection::D_RightToLeft)
			{
				TmpRot.Yaw = SectorParameter.W - TmpRot.Yaw;
			}
			else
			{
				TmpRot.Yaw = TmpRot.Yaw - SectorParameter.W;
			}
			BoxWorldRot = WorldTransform.TransformRotation(TmpRot.Quaternion());

			// 获取射线检测本地坐标系的方向
			FVector DirectionVector = TmpRot.RotateVector(FVector(1, 0, 0));
			DirectionVector = WorldTransform.TransformVector(DirectionVector) * FMath::Max((SectorParameter.Y - SweepBox.GetExtent().X), 0.0f);

			// 向外扩展的信息
			if (Direction == EBXCDirection::D_InToOut)
			{
				FVector BoxRightVec = BoxWorldRot.GetRightVector();
				StartPoint = FanWorldPos + DirectionVector.GetSafeNormal() * (SectorParameter.X + SweepBox.GetExtent().X);
				EndPoint = FanWorldPos + DirectionVector;

				if (i == 0 || i == MaxLineTraceNum - 1)
				{
					FVector OffsetLoc = BoxRightVec * SweepBox.GetExtent().Y * (i == 0 ? 0.5f : -0.5f);
					StartPoint += OffsetLoc;
					EndPoint += OffsetLoc;

					FVector Extend = SweepBox.GetExtent();
					SweepBox.SetBox(FVector3f(Extend.X, Extend.Y * 0.5f, Extend.Z));
				}
			}
			else
			{
				FVector BoxRightVec = BoxWorldRot.GetRightVector() * (Direction == EBXCDirection::D_LeftToRight ? 1.0f : -1.0f);

				if (i == 0 || i == MaxLineTraceNum - 1)
				{
					StartPoint = i == 0 ? FanWorldPos + DirectionVector : FanWorldPos + DirectionVector - BoxRightVec * SweepBox.GetExtent().Y * SweepStep;
					EndPoint = i == 0 ? FanWorldPos + DirectionVector + BoxRightVec * SweepBox.GetExtent().Y * SweepStep : FanWorldPos + DirectionVector;

					FVector Extend = SweepBox.GetExtent();
					SweepBox.SetBox(FVector3f(Extend.X, Extend.Y * 0.5f, Extend.Z));
				}
				else
				{
					StartPoint = FanWorldPos + DirectionVector - BoxRightVec * SweepBox.GetExtent().Y * SweepStep;
					EndPoint = FanWorldPos + DirectionVector + BoxRightVec * SweepBox.GetExtent().Y * SweepStep;
				}
			}


			// 进行Sweep检测
			QueryWorld->SweepMultiByObjectType(SweepResults, StartPoint, EndPoint, BoxWorldRot, OQP, SweepBox, CQP);

#if WITH_EDITOR
			if (const UBXSettings* Setting = GetDefault<UBXSettings>())
			{
				if (Setting->CollisionDebugDuration > 0.0f)
				{
					UBXFunctionLibrary::DrawDebugSweepBoxInEditor(QueryWorld, StartPoint, EndPoint, BoxWorldRot.Rotator(), SweepBox.GetBox(), FColor::Blue, false, Setting->CollisionDebugDuration, 0.0f);
				}
			}
#endif

			// 检测结果筛选
			for (int32 j = 0; j < SweepResults.Num(); ++j)
			{
				if (CollisionComps.Find(SweepResults[j].GetComponent()) < 0)
				{
					// 判断是否是想要检测的目标
					bool bIsOverlap = false;
					for (int32 k = 0; k < OverlapResults.Num(); ++k)
					{
						if (OverlapResults[k].GetComponent() == SweepResults[j].GetComponent())
						{
							bIsOverlap = true;
							break;
						}
					}

					if (bIsOverlap)
					{
						OutResult.Add(SweepResults[j]);
						CollisionComps.Add(SweepResults[j].GetComponent());
					}
				}
			}
		}
	}

#if WITH_EDITOR
	if (const UBXSettings* Setting = GetDefault<UBXSettings>())
	{
		if (Setting->CollisionDebugDuration > 0.0f)
		{
			DrawDebugBox(QueryWorld, BoxWorldTransform.GetLocation(), BoxExtend, WorldTransform.GetRotation(), FColor::Red, false, Setting->CollisionDebugDuration);

			FMatrix TransformMatrix = FTransform(WorldTransform.Rotator(), WorldTransform.GetLocation()).ToMatrixNoScale();
			ULineBatchComponent* const LineBatcher = QueryWorld->PersistentLineBatcher;
			if (LineBatcher != NULL)
			{
				const float LineLifeTime = Setting->CollisionDebugDuration;

				float Segments = 32;

				// Need at least 4 segments
				float Step = 2.f * SectorParameter.W / float(Segments);
				Step = FMath::DegreesToRadians(Step);

				const FVector Center = TransformMatrix.GetOrigin();
				const FVector AxisX = TransformMatrix.GetScaledAxis(EAxis::X);
				const FVector AxisY = TransformMatrix.GetScaledAxis(EAxis::Y);

				TArray<FBatchedLine> Lines;
				Lines.Empty(Segments);

				float Angle = -SectorParameter.W + 90.0f;
				Angle = FMath::DegreesToRadians(Angle);

				float FBigRadius = SectorParameter.Y;
				float FSmallRadius = SectorParameter.X;
				while (Segments--)
				{
					const FVector VertexBig1 = Center + FBigRadius * (AxisY * FMath::Cos(Angle) + AxisX * FMath::Sin(Angle));
					const FVector VertexSmall1 = Center + FSmallRadius * (AxisY * FMath::Cos(Angle) + AxisX * FMath::Sin(Angle));
					Angle += Step;
					const FVector VertexBig2 = Center + FBigRadius * (AxisY * FMath::Cos(Angle) + AxisX * FMath::Sin(Angle));
					const FVector VertexSmall2 = Center + FSmallRadius * (AxisY * FMath::Cos(Angle) + AxisX * FMath::Sin(Angle));
					Lines.Add(FBatchedLine(VertexBig1, VertexBig2, FColor::Red, LineLifeTime, 0.0f, false));
					Lines.Add(FBatchedLine(VertexSmall1, VertexSmall2, FColor::Red, LineLifeTime, 0.0f, false));
					if (Segments == 31)
					{
						Lines.Add(FBatchedLine(VertexBig1, VertexSmall1, FColor::Red, LineLifeTime, 0.0f, false));
					}
					if (Segments == 0)
					{
						Lines.Add(FBatchedLine(VertexBig2, VertexSmall2, FColor::Red, LineLifeTime, 0.0f, false));
					}
				}
				LineBatcher->DrawLines(Lines);
			}
		}
	}
#endif

	return OutResult;
}


// 折线分段内部辅助结构
namespace BXCurveSweepInternal
{
	struct FPolylineSegment
	{
		FVector StartLocation;
		FVector EndLocation;
		FQuat Rotation;
		FVector Scale;
	};

	// 将曲线Transform数组折线化为段数据,先按旋转差分段,剩余额度按共线拐点补充,处理反向延伸
	static void BuildPolylineSegments(
		const TArray<FTransform>& CurveTransforms,
		FIntVector PolylineConfig,
		const FBXCPolylineFrameLink& InFrameLink,
		float ShapeMaxRadius,
		TArray<FPolylineSegment>& OutSegments,
		FBXCPolylineFrameLink& OutFrameLink)
	{
		int32 NumPoints = CurveTransforms.Num();
		check(NumPoints >= 2);
		int32 MaxSegmentCount = FMath::Clamp(PolylineConfig.X, 1, 10);
		float CollinearThreshold = FMath::Clamp((float)PolylineConfig.Y, 1.0f, 60.0f);
		float RotationThreshold = FMath::Clamp((float)PolylineConfig.Z, 1.0f, 180.0f);

		// 累积弧长
		TArray<float> CumulativeArc;
		CumulativeArc.SetNumUninitialized(NumPoints);
		CumulativeArc[0] = 0.0f;
		for (int32 i = 1; i < NumPoints; ++i)
		{
			CumulativeArc[i] = CumulativeArc[i - 1] + FVector::Dist(
				CurveTransforms[i - 1].GetLocation(), CurveTransforms[i].GetLocation());
		}
		float TotalArc = CumulativeArc.Last();

		// 收集所有分段点弧长位置
		TArray<float> SplitArcs;

		// 旋转分段:累积所有相邻点的旋转差,避免S形曲线首末旋转接近但中间旋转大的情况
		float TotalRotDeltaDeg = 0.0f;
		for (int32 i = 1; i < NumPoints; ++i)
		{
			TotalRotDeltaDeg += FMath::RadiansToDegrees(
				CurveTransforms[i - 1].GetRotation().AngularDistance(CurveTransforms[i].GetRotation()));
		}
		int32 RotSegments = 1;
		if (TotalRotDeltaDeg >= RotationThreshold && RotationThreshold > KINDA_SMALL_NUMBER)
		{
			RotSegments = FMath::CeilToInt(TotalRotDeltaDeg / RotationThreshold);
		}
		RotSegments = FMath::Clamp(RotSegments, 1, MaxSegmentCount);
		for (int32 s = 1; s < RotSegments; ++s)
		{
			SplitArcs.Add(TotalArc * (float)s / (float)RotSegments);
		}

		// 共线补充分段:剩余额度内,检测位置拐点(三点不共线),按Y阈值
		int32 RemainingSlots = MaxSegmentCount - RotSegments;
		if (RemainingSlots > 0 && NumPoints >= 3 && CollinearThreshold > KINDA_SMALL_NUMBER)
		{
			int32 LastKeptIdx = 0;
			for (int32 i = 1; i < NumPoints - 1 && RemainingSlots > 0; ++i)
			{
				if (!UBXFunctionLibrary::AreCollinear(
					CurveTransforms[LastKeptIdx].GetLocation(),
					CurveTransforms[i].GetLocation(),
					CurveTransforms[i + 1].GetLocation(),
					CollinearThreshold))
				{
					SplitArcs.Add(CumulativeArc[i]);
					LastKeptIdx = i;
					--RemainingSlots;
				}
			}
		}

		// 分段点去重并按弧长排序
		SplitArcs.Sort();
		TArray<float> UniqueSplitArcs;
		for (float Arc : SplitArcs)
		{
			if (UniqueSplitArcs.Num() == 0 || FMath::Abs(Arc - UniqueSplitArcs.Last()) > KINDA_SMALL_NUMBER)
			{
				UniqueSplitArcs.Add(Arc);
			}
		}

		int32 SegmentCount = UniqueSplitArcs.Num() + 1;

		// 按弧长位置插值生成关键点Transform
		TArray<FTransform> KeyTransforms;
		KeyTransforms.Reserve(SegmentCount + 1);
		KeyTransforms.Add(CurveTransforms[0]);

		for (float TargetArc : UniqueSplitArcs)
		{
			int32 Idx = 1;
			while (Idx < NumPoints && CumulativeArc[Idx] < TargetArc)
			{
				++Idx;
			}
			if (Idx <= 0 || Idx >= NumPoints)
			{
				KeyTransforms.Add(CurveTransforms[FMath::Clamp(Idx, 0, NumPoints - 1)]);
			}
			else
			{
				float SegLen = CumulativeArc[Idx] - CumulativeArc[Idx - 1];
				float Alpha = SegLen > KINDA_SMALL_NUMBER ? (TargetArc - CumulativeArc[Idx - 1]) / SegLen : 0.0f;
				FVector Loc = FMath::Lerp(CurveTransforms[Idx - 1].GetLocation(), CurveTransforms[Idx].GetLocation(), Alpha);
				FQuat Rot = FQuat::Slerp(CurveTransforms[Idx - 1].GetRotation(), CurveTransforms[Idx].GetRotation(), Alpha);
				FVector Scale = FMath::Lerp(CurveTransforms[Idx - 1].GetScale3D(), CurveTransforms[Idx].GetScale3D(), Alpha);
				KeyTransforms.Add(FTransform(Rot, Loc, Scale));
			}
		}
		KeyTransforms.Add(CurveTransforms.Last());

		// 每段平均旋转
		int32 NumSegments = KeyTransforms.Num() - 1;
		TArray<FQuat> SegRotations;
		SegRotations.SetNumUninitialized(NumSegments);
		for (int32 s = 0; s < NumSegments; ++s)
		{
			SegRotations[s] = FQuat::Slerp(KeyTransforms[s].GetRotation(), KeyTransforms[s + 1].GetRotation(), 0.5f);
		}

		// 构建段数据,处理反向延伸填补旋转缺口
		OutSegments.Reset();
		for (int32 s = 0; s < NumSegments; ++s)
		{
			FPolylineSegment Seg;
			Seg.StartLocation = KeyTransforms[s].GetLocation();
			Seg.EndLocation = KeyTransforms[s + 1].GetLocation();
			Seg.Rotation = SegRotations[s];
			Seg.Scale = (KeyTransforms[s].GetScale3D() + KeyTransforms[s + 1].GetScale3D()) * 0.5f;

			// 反向延伸:段间用前一段方向,跨帧首段用上一帧最后一段方向
			FVector PrevDir = FVector::ZeroVector;
			FQuat PrevRotation = SegRotations[s];
			if (s > 0)
			{
				PrevDir = KeyTransforms[s].GetLocation() - KeyTransforms[s - 1].GetLocation();
				PrevRotation = SegRotations[s - 1];
			}
			else if (InFrameLink.bValid)
			{
				PrevDir = InFrameLink.LastSegDir;
				PrevRotation = FQuat(InFrameLink.LastSegRotation);
			}

			if (PrevDir.SizeSquared() > KINDA_SMALL_NUMBER)
			{
				PrevDir = PrevDir.GetSafeNormal();
				float RotDeltaRad = PrevRotation.AngularDistance(SegRotations[s]);
				// tan(θ/2)单调递增,sin(θ)在θ>90°时反而减小,tan(θ/2)能正确反映旋转缺口大小
				// 跨帧首段可能因帧间姿态突变导致旋转差接近π,clamp防止BackOff爆炸
				float RotDeltaRadClamped = FMath::Min(RotDeltaRad, FMath::DegreesToRadians(170.0f));
				float BackOff = ShapeMaxRadius * FMath::Tan(RotDeltaRadClamped * 0.5f);
				Seg.StartLocation -= PrevDir * BackOff;
			}

			OutSegments.Add(MoveTemp(Seg));
		}

		// 更新跨帧衔接信息
		OutFrameLink.LastSegDir = (KeyTransforms.Last().GetLocation() - KeyTransforms[NumSegments - 1].GetLocation()).GetSafeNormal();
		OutFrameLink.LastSegRotation = SegRotations.Last().Rotator();
		OutFrameLink.bValid = true;
	}
}

TArray<FHitResult> UBXCollisionLibrary::SphereSweepAlongCurve(AActor* Requester, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, float SphereSize, const FBXCFilter& Filter, const TArray<FTransform>& CurveTransforms, FIntVector PolylineConfig, UPARAM(ref) FBXCPolylineFrameLink& InOutFrameLink)
{
	TArray<FHitResult> OutResult;

	if (CurveTransforms.Num() <= 0)
	{
		InOutFrameLink.bValid = false;
		return OutResult;
	}

	FBXCParameter SegParam;
	SegParam.Requester = Requester;

	// 单点走Overlap
	if (CurveTransforms.Num() == 1)
	{
		SegParam.StartLocation = CurveTransforms[0].GetLocation();
		SegParam.StartRotation = CurveTransforms[0].GetRotation().Rotator();
		SegParam.EndLocation = SegParam.StartLocation;
		SegParam.EndRotation = SegParam.StartRotation;
		SegParam.Scale = CurveTransforms[0].GetScale3D();
		InOutFrameLink.bValid = false;
		return SphereCheck(SegParam, ObjectTypes, SphereSize, Filter);
	}

	// 折线分段
	TArray<BXCurveSweepInternal::FPolylineSegment> Segments;
	FBXCPolylineFrameLink NewFrameLink;
	BXCurveSweepInternal::BuildPolylineSegments(CurveTransforms, PolylineConfig, InOutFrameLink, SphereSize, Segments, NewFrameLink);

	for (const BXCurveSweepInternal::FPolylineSegment& Seg : Segments)
	{
		SegParam.StartLocation = Seg.StartLocation;
		SegParam.StartRotation = Seg.Rotation.Rotator();
		SegParam.EndLocation = Seg.EndLocation;
		SegParam.EndRotation = Seg.Rotation.Rotator();
		SegParam.Scale = Seg.Scale;
		TArray<FHitResult> SegResults = SphereCheck(SegParam, ObjectTypes, SphereSize, Filter);
		CombineCollisionResults(SegResults, OutResult);
	}

	InOutFrameLink = NewFrameLink;
	return OutResult;
}

TArray<FHitResult> UBXCollisionLibrary::CapsuleSweepAlongCurve(AActor* Requester, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, FVector2D CapsuleSize, const FBXCFilter& Filter, const TArray<FTransform>& CurveTransforms, FIntVector PolylineConfig, UPARAM(ref) FBXCPolylineFrameLink& InOutFrameLink)
{
	TArray<FHitResult> OutResult;

	if (CurveTransforms.Num() <= 0)
	{
		InOutFrameLink.bValid = false;
		return OutResult;
	}

	FBXCParameter SegParam;
	SegParam.Requester = Requester;

	// 单点走Overlap
	if (CurveTransforms.Num() == 1)
	{
		SegParam.StartLocation = CurveTransforms[0].GetLocation();
		SegParam.StartRotation = CurveTransforms[0].GetRotation().Rotator();
		SegParam.EndLocation = SegParam.StartLocation;
		SegParam.EndRotation = SegParam.StartRotation;
		SegParam.Scale = CurveTransforms[0].GetScale3D();
		InOutFrameLink.bValid = false;
		return CapsuleCheck(SegParam, ObjectTypes, CapsuleSize, Filter, 360.0f);
	}

	// 折线分段
	float ShapeMaxRadius = FMath::Max(CapsuleSize.X, CapsuleSize.Y);
	TArray<BXCurveSweepInternal::FPolylineSegment> Segments;
	FBXCPolylineFrameLink NewFrameLink;
	BXCurveSweepInternal::BuildPolylineSegments(CurveTransforms, PolylineConfig, InOutFrameLink, ShapeMaxRadius, Segments, NewFrameLink);

	for (const BXCurveSweepInternal::FPolylineSegment& Seg : Segments)
	{
		SegParam.StartLocation = Seg.StartLocation;
		SegParam.StartRotation = Seg.Rotation.Rotator();
		SegParam.EndLocation = Seg.EndLocation;
		SegParam.EndRotation = Seg.Rotation.Rotator();
		SegParam.Scale = Seg.Scale;
		TArray<FHitResult> SegResults = CapsuleCheck(SegParam, ObjectTypes, CapsuleSize, Filter, 360.0f);
		CombineCollisionResults(SegResults, OutResult);
	}

	InOutFrameLink = NewFrameLink;
	return OutResult;
}

TArray<FHitResult> UBXCollisionLibrary::BoxSweepAlongCurve(AActor* Requester, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, FVector BoxSize, const FBXCFilter& Filter, const TArray<FTransform>& CurveTransforms, FIntVector PolylineConfig, UPARAM(ref) FBXCPolylineFrameLink& InOutFrameLink)
{
	TArray<FHitResult> OutResult;

	if (CurveTransforms.Num() <= 0)
	{
		InOutFrameLink.bValid = false;
		return OutResult;
	}

	FBXCParameter SegParam;
	SegParam.Requester = Requester;

	// 单点走Overlap
	if (CurveTransforms.Num() == 1)
	{
		SegParam.StartLocation = CurveTransforms[0].GetLocation();
		SegParam.StartRotation = CurveTransforms[0].GetRotation().Rotator();
		SegParam.EndLocation = SegParam.StartLocation;
		SegParam.EndRotation = SegParam.StartRotation;
		SegParam.Scale = CurveTransforms[0].GetScale3D();
		InOutFrameLink.bValid = false;
		return BoxCheck(SegParam, ObjectTypes, BoxSize, Filter, 360.0f);
	}

	// 折线分段
	float ShapeMaxRadius = BoxSize.Size();
	TArray<BXCurveSweepInternal::FPolylineSegment> Segments;
	FBXCPolylineFrameLink NewFrameLink;
	BXCurveSweepInternal::BuildPolylineSegments(CurveTransforms, PolylineConfig, InOutFrameLink, ShapeMaxRadius, Segments, NewFrameLink);

	for (const BXCurveSweepInternal::FPolylineSegment& Seg : Segments)
	{
		SegParam.StartLocation = Seg.StartLocation;
		SegParam.StartRotation = Seg.Rotation.Rotator();
		SegParam.EndLocation = Seg.EndLocation;
		SegParam.EndRotation = Seg.Rotation.Rotator();
		SegParam.Scale = Seg.Scale;
		TArray<FHitResult> SegResults = BoxCheck(SegParam, ObjectTypes, BoxSize, Filter, 360.0f);
		CombineCollisionResults(SegResults, OutResult);
	}

	InOutFrameLink = NewFrameLink;
	return OutResult;
}