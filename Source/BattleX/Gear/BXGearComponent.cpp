#include "BXGearComponent.h" 



#pragma region Important
UBXGearComponent::UBXGearComponent()
{
	for (int32 i = 0; i < (int32)EBXGearSlot::GS_Jewelry8; ++i)
	{
		SlotMaxSize.Add((EBXGearSlot)i, 1);
	}
}

void UBXGearComponent::BeginPlay()
{
	for (TMap<EBXGearSlot, int32>::TIterator It(SlotMaxSize); It; ++It)
	{
		EquipGears.Add(It->Key, FBXGears(It->Value));
		UsingGearIndexs.Add(It->Key, -1);
	}

	Super::BeginPlay();
}

void UBXGearComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	UnequipAllGears();

	UsingGearIndexs.Empty();
	EquipGears.Empty();
}

#pragma endregion Important



#pragma region API
void UBXGearComponent::GetEquipGearList(TArray<ABXGear*>& OutList, bool bNeedReset)
{
	if (bNeedReset)
	{
		OutList.Reset();
	}

	for (TMap<EBXGearSlot, FBXGears>::TIterator It(EquipGears); It; ++It)
	{
		for (TArray<ABXGear*>::TIterator It2(It->Value.List); It2; ++It2)
		{
			if (IsValid(*It2))
			{
				OutList.AddUnique(*It2);
			}
		}
	}
}

void UBXGearComponent::GetEquipGearsBySlot(EBXGearSlot InSlot, TArray<ABXGear*>& OutGears)
{
	OutGears.Reset();
	if (FBXGears* Gears = EquipGears.Find(InSlot))
	{
		OutGears = Gears->List;
	}
}

ABXGear* UBXGearComponent::GetUsingGear(EBXGearSlot InSlot)
{
	if (int32* Index = UsingGearIndexs.Find(InSlot))
	{
		if (FBXGears* GearList = EquipGears.Find(InSlot))
		{
			return GearList->List.IsValidIndex(*Index) ? GearList->List[*Index] : nullptr;
		}
	}

	return nullptr;
}

EBXGearSlot UBXGearComponent::GetUsingGearSlot(ABXGear* InGear)
{
	for (TMap<EBXGearSlot, int32>::TIterator It(UsingGearIndexs); It; ++It)
	{
		FBXGears* GearList = EquipGears.Find(It->Key);
		if (!GearList)
		{
			continue;
		}

		if (!GearList->List.IsValidIndex(It->Value))
		{
			continue;
		}

		if (GearList->List[It->Value] == InGear)
		{
			return It->Key;
		}
	}

	return EBXGearSlot::GS_None;
}

void UBXGearComponent::SwitchUsingGear(EBXGearSlot InSlot, int32 InIndex)
{
	FBXGears* GearList = EquipGears.Find(InSlot);
	if (!GearList)
	{
		return;
	}

	int32 CurrentIndex = -1;
	if (int32* Index = UsingGearIndexs.Find(InSlot))
	{
		CurrentIndex = *Index;
	}

	if (CurrentIndex == InIndex)
	{
		return;
	}

	ABXGear* CurrentGear = GearList->List.IsValidIndex(CurrentIndex) ? GearList->List[CurrentIndex] : nullptr;
	ABXGear* NewGear = GearList->List.IsValidIndex(InIndex) ? GearList->List[InIndex] : nullptr;

	if (CurrentGear)
	{
		HelpUsingInformation.Gear = CurrentGear;
		CurrentGear->PreUnusing(HelpUsingInformation);
		PreUnusingGearEvent.Broadcast(HelpUsingInformation);
	}
	if (NewGear)
	{
		HelpUsingInformation.Gear = NewGear;
		NewGear->PreUsing(HelpUsingInformation);
		PreUsingGearEvent.Broadcast(HelpUsingInformation);
	}

	// 改变正在使用的装备
	UsingGearIndexs.Add(InSlot, InIndex);

	if (CurrentGear)
	{
		HelpUsingInformation.Gear = CurrentGear;
		CurrentGear->PostUnusing(HelpUsingInformation);
		PostUnusingGearEvent.Broadcast(HelpUsingInformation);
	}
	if (NewGear)
	{
		HelpUsingInformation.Gear = NewGear;
		NewGear->PostUsing(HelpUsingInformation);
		PostUsingGearEvent.Broadcast(HelpUsingInformation);
	}
}

void UBXGearComponent::ChangeUsingGearState(EBXGearSlot InSlot, EBXGearState InNewState)
{
	FBXGears* GearList = EquipGears.Find(InSlot);
	if (!GearList)
	{
		return;
	}

	int32 CurrentIndex = -1;
	if (int32* Index = UsingGearIndexs.Find(InSlot))
	{
		CurrentIndex = *Index;
	}

	if (CurrentIndex < 0)
	{
		return;
	}

	ABXGear* CurrentGear = GearList->List.IsValidIndex(CurrentIndex) ? GearList->List[CurrentIndex] : nullptr;
	if (!CurrentGear)
	{
		return;
	}

	EBXGearState OldState = CurrentGear->GetCurrentState();

	CurrentGear->ChangeState(InNewState);
	ChangeGearStateEvent.Broadcast(CurrentGear, OldState, InNewState);
}

void UBXGearComponent::ChangeEquipGear(EBXGearSlot InSlot, int32 InIndex, ABXGear* InGear, USkeletalMeshComponent* AttachParent)
{
	ABXGear* CurrentGear = nullptr;
	if (FBXGears* GearList = EquipGears.Find(InSlot))
	{
		if (GearList->List.IsValidIndex(InIndex))
		{
			CurrentGear = GearList->List[InIndex];
		}
	}

	int32 CurrentUsingIndex = -1;
	if (int32* Index = UsingGearIndexs.Find(InSlot))
	{
		CurrentUsingIndex = *Index;
	}

	// 重置辅助结构体
	HelpEquipInformation.Reset();

	if (CurrentGear)
	{
		// 弃用该装备
		if (CurrentUsingIndex == InIndex)
		{
			SwitchUsingGear(InSlot, -1);
		}

		// 卸下装备之前
		HelpEquipInformation.Gear = CurrentGear;
		HelpEquipInformation.AttachParent = nullptr;
		HelpEquipInformation.OwnerComponent = nullptr;
		CurrentGear->PreUnequip(HelpEquipInformation);
		PreUnequipGearEvent.Broadcast(HelpEquipInformation);
	}

	if (InGear)
	{
		// 装上装备之前
		HelpEquipInformation.Gear = InGear;
		HelpEquipInformation.AttachParent = AttachParent;
		HelpEquipInformation.OwnerComponent = this;
		InGear->PreEquip(HelpEquipInformation);
		PreEquipGearEvent.Broadcast(HelpEquipInformation);
	}

	// 装上装备
	if (FBXGears* GearList = EquipGears.Find(InSlot))
	{
		if (GearList->List.IsValidIndex(InIndex))
		{
			GearList->List[InIndex] = InGear;
		}
		else
		{
			GearList->List.EmplaceAt(InIndex, InGear);
		}
	}

	if (CurrentGear)
	{
		// 卸下装备之后
		HelpEquipInformation.Gear = CurrentGear;
		HelpEquipInformation.AttachParent = nullptr;
		HelpEquipInformation.OwnerComponent = nullptr;
		CurrentGear->PostUnequip(HelpEquipInformation);
		PostUnequipGearEvent.Broadcast(HelpEquipInformation);
	}

	if (InGear)
	{
		// 装上装备之后
		HelpEquipInformation.Gear = InGear;
		HelpEquipInformation.AttachParent = AttachParent;
		HelpEquipInformation.OwnerComponent = this;
		InGear->PostEquip(HelpEquipInformation);
		PostEquipGearEvent.Broadcast(HelpEquipInformation);

		// 使用该装备
		if (CurrentUsingIndex == InIndex)
		{
			SwitchUsingGear(InSlot, InIndex);
		}
	}
}

void UBXGearComponent::ChangeEquipGearByClass(EBXGearSlot InSlot, int32 InIndex, UClass* InGearClass, USkeletalMeshComponent* AttachParent)
{
	if (!InGearClass || !InGearClass->IsChildOf(ABXGear::StaticClass()))
	{
		return;
	}

	if (ABXGear* NewGear = GetWorld()->SpawnActor<ABXGear>(InGearClass))
	{
		ChangeEquipGear(InSlot, InIndex, NewGear, AttachParent);
	}
}

void UBXGearComponent::ChangeEquipGearByData(EBXGearSlot InSlot, int32 InIndex, UBXGearData* InGearData, USkeletalMeshComponent* AttachParent)
{
	if (!InGearData || !InGearData->GearClass)
	{
		return;
	}

	if (ABXGear* NewGear = GetWorld()->SpawnActor<ABXGear>(InGearData->GearClass))
	{
		NewGear->GearData = InGearData;
		ChangeEquipGear(InSlot, InIndex, NewGear, AttachParent);
	}
}

void UBXGearComponent::UnequipAllGears(bool bForceDestroy)
{
	for (TMap<EBXGearSlot, FBXGears>::TIterator It(EquipGears); It; ++It)
	{
		for (TArray<ABXGear*>::TIterator GearIt(It->Value.List); GearIt; ++GearIt)
		{
			if (*GearIt)
			{
				ChangeEquipGear(It->Key, GearIt.GetIndex(), nullptr, nullptr);

				if (bForceDestroy && IsValid(*GearIt))
				{
					(*GearIt)->Destroy();
				}
			}
		}
	}
}

#pragma endregion API
