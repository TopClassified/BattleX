#include "BXTCollision.h"

#include <Windows.Graphics.Display.h>

#include "UObject/ObjectSaveContext.h"

#include "BXColdWeapon.h"
#include "BXGearComponent.h"
#include "BXTPCollision.h"



UBXTTrackHitBox::UBXTTrackHitBox()
{
	DisplayName = FText::FromString(TEXT("碰撞盒轨迹命中检测"));
}

#pragma region Editor
void UBXTTrackHitBox::CleanBakedData_Implementation()
{
	BakedHBTrajectoryPoints.Reset();
}

void UBXTTrackHitBox::BakingData_Implementation(const FBXTLRunTimeData& InOutRTData, const FBXTLSectionRTData& InOutRTSData, const FBXTLTaskRTData& InOutRTTData)
{
	const FBXTPTrackHitBoxContext& TPC = InOutRTTData.DynamicData.Get<FBXTPTrackHitBoxContext>();
	
	for (TArray<UBXShapeComponent*>::TConstIterator It(TPC.ShapeComponents); It; ++It)
	{
		UBXShapeComponent* ShapeComponent = (*It);
		if (!IsValid(ShapeComponent))
		{
			continue;
		}
		
		AActor* HitBoxOwner = ShapeComponent->GetOwner();
		if (!IsValid(HitBoxOwner))
		{
			continue;
		}

		FTransform Transform;
		for (int32 i = 0; i < HitBoxTags.Num(); ++i)
		{
			const FGameplayTag& Tag = HitBoxTags.GetByIndex(i);
			if (!ShapeComponent->GetShapeTransformByTag(Tag, Transform))
			{
				continue;
			}
			
			FBXTrajectoryPoints* Points = BakedHBTrajectoryPoints.Find(Tag);
			if (!Points)
			{
				BakedHBTrajectoryPoints.Add(Tag);
				Points = BakedHBTrajectoryPoints.Find(Tag);
			}
			if (!Points)
			{
				continue;
			}

			FBXTrajectoryPoint NewPoint;
			NewPoint.Time = InOutRTTData.RunTime;
			NewPoint.Transform = Transform * HitBoxOwner->GetTransform().Inverse();
			
			Points->List.Add(NewPoint);
		}
	}
}

void UBXTTrackHitBox::PostBakeData_Implementation()
{
	Super::PostBakeData_Implementation();

	for (TMap<FGameplayTag, FBXTrajectoryPoints>::TIterator It(BakedHBTrajectoryPoints); It; ++It)
	{
		TArray<FBXTrajectoryPoint>& List = It.Value().List;
		
		// 共线合并
		TArray<int32> SlopeIndexList;
		SlopeIndexList.Add(0);
		for (int32 i = 1; i < List.Num() - 1; ++i)
		{
			// 如果不共线，则塞入列表
			if (!UBXFunctionLibrary::AreCollinear(List[SlopeIndexList.Last()].Transform.GetLocation(), List[i].Transform.GetLocation(), List[i + 1].Transform.GetLocation(), BakedTrajectoryOptimization.X))
			{
				SlopeIndexList.Add(i);
			}
		}
		SlopeIndexList.Add(List.Num() - 1);

		// 相同角度合并
		TArray<int32> AngleIndexList;
		AngleIndexList.Add(0);
		float RadiansError = FMath::DegreesToRadians(BakedTrajectoryOptimization.Y);
		for (int32 i = 1; i < List.Num() - 1; ++i)
		{
			// 如果角度相差大，则塞入列表
			if (RadiansError < List[AngleIndexList.Last()].Transform.GetRotation().AngularDistance(List[i].Transform.GetRotation()))
			{
				AngleIndexList.Add(i);
			}
		}
		AngleIndexList.Add(List.Num() - 1);

		// 相同大小合并
		TArray<int32> ScaleIndexList;
		ScaleIndexList.Add(0);
		for (int32 i = 1; i < List.Num() - 1; ++i)
		{
			if (!List[AngleIndexList.Last()].Transform.GetScale3D().Equals(List[i].Transform.GetScale3D(), BakedTrajectoryOptimization.Z))
			{
				ScaleIndexList.Add(i);
			}
		}
		ScaleIndexList.Add(List.Num() - 1);

		// 刷新路径列表
		TArray<FBXTrajectoryPoint> Results;
		for (int32 i = 0; i < List.Num(); ++i)
		{
			if (SlopeIndexList.Contains(i) || AngleIndexList.Contains(i) || ScaleIndexList.Contains(i))
			{
				Results.Add(List[i]);
			}
		}
		List.Reset();
		List.Append(Results);
	}
}

#if WITH_EDITOR
void UBXTTrackHitBox::PreSave(FObjectPreSaveContext SaveContext)
{
	Super::PreSave(SaveContext);
	
}

void UBXTTrackHitBox::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	if (LifeType == EBXTLifeType::L_Instant)
	{
		LifeType = EBXTLifeType::L_DurationTimeline;
	}

	// 这个Task只能让拥有者使用
	TargetTypes = (1 << (int32)EBXTTargetType::T_Owner);
	
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

bool UBXTTrackHitBox::EnablePassiveTrigger()
{
	return false;
}
#endif
#pragma endregion Editor






UBXTTrackWeaponHitBox::UBXTTrackWeaponHitBox()
{
	DisplayName = FText::FromString(TEXT("武器碰撞盒轨迹命中检测"));
}

#pragma region Editor
void UBXTTrackWeaponHitBox::BakingData_Implementation(const FBXTLRunTimeData& InOutRTData, const FBXTLSectionRTData& InOutRTSData, const FBXTLTaskRTData& InOutRTTData)
{
	const FBXTPTrackHitBoxContext& TPC = InOutRTTData.DynamicData.Get<FBXTPTrackHitBoxContext>();
	
	for (TArray<UBXShapeComponent*>::TConstIterator It(TPC.ShapeComponents); It; ++It)
	{
		ABXColdWeapon* CWeapon = Cast<ABXColdWeapon>((*It)->GetOwner());
		if (!IsValid(CWeapon))
		{
			continue;
		}

		AActor* GearOwner = CWeapon->OwnerComponent->GetOwner();
		if (!IsValid(GearOwner))
		{
			continue;
		}

		FTransform Transform;
		for (int32 i = 0; i < HitBoxTags.Num(); ++i)
		{
			const FGameplayTag& Tag = HitBoxTags.GetByIndex(i);
			if (!CWeapon->GetHitBoxTransform(Tag, Transform))
			{
				continue;
			}
			
			FBXTrajectoryPoints* Points = BakedHBTrajectoryPoints.Find(Tag);
			if (!Points)
			{
				BakedHBTrajectoryPoints.Add(Tag);
				Points = BakedHBTrajectoryPoints.Find(Tag);
			}
			if (!Points)
			{
				continue;
			}

			FBXTrajectoryPoint NewPoint;
			NewPoint.Time = InOutRTTData.RunTime;
			NewPoint.Transform = Transform * GearOwner->GetTransform().Inverse();
			
			Points->List.Add(NewPoint);
		}
	}
}
#pragma endregion Editor
