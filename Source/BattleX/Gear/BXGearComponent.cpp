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
		OnPreUnusingGear.Broadcast(CurrentGear, HelpUsingInformation);
	}
	if (NewGear)
	{
		NewGear->PreUsing(HelpUsingInformation);
		OnPreUsingGear.Broadcast(NewGear, HelpUsingInformation);
	}

	// �ı�����ʹ�õ�װ��
	UsingGearIndexs.Add(InSlot, InIndex);

	if (CurrentGear)
	{
		CurrentGear->PostUnusing(HelpUsingInformation);
		OnPostUnusingGear.Broadcast(CurrentGear, HelpUsingInformation);
	}
	if (NewGear)
	{
		NewGear->PostUsing(HelpUsingInformation);
		OnPostUsingGear.Broadcast(NewGear, HelpUsingInformation);
	}
}

void UBXGearComponent::SheathGear(EBXGearSlot InSlot, bool bSheath)
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

	if (bSheath)
	{
		CurrentGear->PreSheath(HelpSheathInformation);
		OnPreSheathGear.Broadcast(CurrentGear, HelpSheathInformation);

		CurrentGear->PostSheath(HelpSheathInformation);
		OnPostSheathGear.Broadcast(CurrentGear, HelpSheathInformation);
	}
	else
	{
		CurrentGear->PreUnsheath(HelpSheathInformation);
		OnPreUnsheathGear.Broadcast(CurrentGear, HelpSheathInformation);

		CurrentGear->PostUnsheath(HelpSheathInformation);
		OnPostUnsheathGear.Broadcast(CurrentGear, HelpSheathInformation);
	}
}

void UBXGearComponent::GetEquipGears(EBXGearSlot InSlot, TArray<ABXGear*>& OutGears)
{
	OutGears.Reset();
	if (FBXGears* Gears = EquipGears.Find(InSlot))
	{
		OutGears = Gears->List;
	}
}

void UBXGearComponent::ChangeEquipGear(EBXGearSlot InSlot, int32 InIndex, ABXGear* InGear)
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

	if (CurrentGear)
	{
		// ���ø�װ��
		if (CurrentUsingIndex == InIndex)
		{
			SwitchUsingGear(InSlot, -1);
		}

		// ж��װ��֮ǰ
		CurrentGear->PreUnequip(HelpEquipInformation);
		OnPreUnequipGear.Broadcast(CurrentGear, HelpEquipInformation);
	}

	if (InGear)
	{
		// װ��װ��֮ǰ
		InGear->PreEquip(HelpEquipInformation);
		OnPreEquipGear.Broadcast(InGear, HelpEquipInformation);
	}

	// װ��װ��
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
		// ж��װ��֮��
		CurrentGear->PostUnequip(HelpEquipInformation);
		OnPostUnequipGear.Broadcast(CurrentGear, HelpEquipInformation);
	}

	if (InGear)
	{
		// װ��װ��֮��
		InGear->PostEquip(HelpEquipInformation);
		OnPostEquipGear.Broadcast(InGear, HelpEquipInformation);

		// ʹ�ø�װ��
		if (CurrentUsingIndex == InIndex)
		{
			SwitchUsingGear(InSlot, InIndex);
		}
	}
}

void UBXGearComponent::ChangeEquipGearByClass(EBXGearSlot InSlot, int32 InIndex, UClass* InGearClass)
{
	if (!InGearClass || !InGearClass->IsChildOf(ABXGear::StaticClass()))
	{
		return;
	}

	if (ABXGear* NewGear = GetWorld()->SpawnActor<ABXGear>(InGearClass))
	{
		ChangeEquipGear(InSlot, InIndex, NewGear);
	}
}

void UBXGearComponent::ChangeEquipGearByData(EBXGearSlot InSlot, int32 InIndex, UBXGearData* InGearData)
{
	if (!InGearData || !InGearData->GearClass)
	{
		return;
	}

	if (ABXGear* NewGear = GetWorld()->SpawnActor<ABXGear>(InGearData->GearClass))
	{
		NewGear->GearData = InGearData;
		ChangeEquipGear(InSlot, InIndex, NewGear);
	}
}

#pragma endregion API
