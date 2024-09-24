#include "BXGearComponent.h" 



#pragma region Important
UBXGearComponent::UBXGearComponent()
{

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

#pragma endregion Important



#pragma region API
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
		CurrentGear->PreUnusing(HelpUsingInformation);
		HelpUsingInformation.Gear = CurrentGear;
		PreUnusingGearEvent.Broadcast(HelpUsingInformation);
	}
	if (NewGear)
	{
		NewGear->PreUsing(HelpUsingInformation);
		HelpUsingInformation.Gear = NewGear;
		PreUsingGearEvent.Broadcast(HelpUsingInformation);
	}

	// 改变正在使用的装备
	UsingGearIndexs.Add(InSlot, InIndex);

	if (CurrentGear)
	{
		CurrentGear->PostUnusing(HelpUsingInformation);
		HelpUsingInformation.Gear = CurrentGear;
		PostUnusingGearEvent.Broadcast(HelpUsingInformation);
	}
	if (NewGear)
	{
		NewGear->PostUsing(HelpUsingInformation);
		HelpUsingInformation.Gear = NewGear;
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

void UBXGearComponent::GetEquipGears(EBXGearSlot InSlot, TArray<ABXGear*>& OutGears)
{
	OutGears.Reset();
	if (FBXGears* Gears = EquipGears.Find(InSlot))
	{
		OutGears = Gears->List;
	}
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
		CurrentGear->PreUnequip(HelpEquipInformation);
		HelpEquipInformation.Gear = CurrentGear;
		HelpEquipInformation.AttachParent = nullptr;
		PreUnequipGearEvent.Broadcast(HelpEquipInformation);
	}

	if (InGear)
	{
		// 装上装备之前
		InGear->PreEquip(HelpEquipInformation);
		HelpEquipInformation.Gear = InGear;
		HelpEquipInformation.AttachParent = AttachParent;
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
		CurrentGear->PostUnequip(HelpEquipInformation);
		HelpEquipInformation.Gear = CurrentGear;
		HelpEquipInformation.AttachParent = nullptr;
		PostUnequipGearEvent.Broadcast(HelpEquipInformation);
	}

	if (InGear)
	{
		// 装上装备之后
		InGear->PostEquip(HelpEquipInformation);
		HelpEquipInformation.Gear = InGear;
		HelpEquipInformation.AttachParent = AttachParent;
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

#pragma endregion API
