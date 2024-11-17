#include "BXTCollision.h"

#include <Windows.Graphics.Display.h>

#include "UObject/ObjectSaveContext.h"

#include "BXColdWeapon.h"
#include "BXGearComponent.h"
#include "BXTPCollision.h"



UBXTTrackWeaponCollision::UBXTTrackWeaponCollision()
{
	DisplayName = FText::FromString(TEXT("武器路径碰撞检测"));
}



#pragma region Editor
void UBXTTrackWeaponCollision::CleanBakedData_Implementation()
{
	BakedHBTrajectoryPoints.Reset();
}

void UBXTTrackWeaponCollision::BakingData_Implementation(const FBXTLRunTimeData& InOutRTData, const FBXTLSectionRTData& InOutRTSData, const FBXTLTaskRTData& InOutRTTData)
{
	const FBXTPTrackWeaponCollisionContext& TPC = InOutRTTData.DynamicData.Get<FBXTPTrackWeaponCollisionContext>();
	
	for (TArray<ABXGear*>::TConstIterator It(TPC.Gears); It; ++It)
	{
		ABXColdWeapon* CWeapon = Cast<ABXColdWeapon>(*It);
		if (!CWeapon->IsValidLowLevelFast())
		{
			continue;
		}

		AActor* GearOwner = CWeapon->OwnerComponent->GetOwner();
		if (!GearOwner->IsValidLowLevelFast())
		{
			continue;
		}

		USceneComponent* AnchorComp = UBXFunctionLibrary::GetSceneComponentByNameAndClass(GearOwner, AnchorComponent, nullptr);
		if (!AnchorComp->IsValidLowLevelFast())
		{
			continue;
		}

		FTransform Transform;
		for (int32 i = 0; i < WeaponHitBoxTags.Num(); ++i)
		{
			const FGameplayTag& Tag = WeaponHitBoxTags.GetByIndex(i);
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
			NewPoint.Transform = Transform * AnchorComp->GetSocketTransform(AnchorSocket.BoneName).Inverse();
			
			Points->List.Add(NewPoint);
		}
	}
}

void UBXTTrackWeaponCollision::PostBakeData_Implementation()
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
void UBXTTrackWeaponCollision::PreSave(FObjectPreSaveContext SaveContext)
{
	Super::PreSave(SaveContext);
	
}

void UBXTTrackWeaponCollision::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	if (LifeType == EBXTLifeType::L_Instant)
	{
		LifeType = EBXTLifeType::L_DurationTimeline;
	}

	// 这个Task只能让拥有者使用
	TargetTypes = (1 << (int32)EBXTTargetType::T_Owner);
	
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

bool UBXTTrackWeaponCollision::EnablePassiveTrigger()
{
	return false;
}
#endif
#pragma endregion Editor
