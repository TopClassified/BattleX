#include "BXGear.h"

#include "BXGearComponent.h" 



#pragma region Important
ABXGear::ABXGear(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.SetTickFunctionEnable(false);
}

ABXGear::~ABXGear()
{

}

#pragma endregion Important



#pragma region Equip
void ABXGear::PreEquip(UPARAM(ref) FBXEquipGearInformation& EquipInfo)
{
	if (!EquipInfo.OwnerComponent)
	{
		return;
	}

	AttachTarget = EquipInfo.AttachParent;
	OwnerComponent = EquipInfo.OwnerComponent;

	if ((EquipFunctions & 1) > 0)
	{
		InternalPreEquip(EquipInfo);
	}

	if ((EquipFunctions & 2) > 0)
	{
		ScriptPreEquip(EquipInfo);
	}
}

void ABXGear::PostEquip(UPARAM(ref) FBXEquipGearInformation& EquipInfo)
{
	if ((EquipFunctions & 4) > 0)
	{
		InternalPostEquip(EquipInfo);
	}

	if ((EquipFunctions & 8) > 0)
	{
		ScriptPostEquip(EquipInfo);
	}
}

void ABXGear::PreUnequip(UPARAM(ref) FBXEquipGearInformation& UnequipInfo)
{
	if ((EquipFunctions & 16) > 0)
	{
		InternalPreUnequip(UnequipInfo);
	}

	if ((EquipFunctions & 32) > 0)
	{
		ScriptPreUnequip(UnequipInfo);
	}
}

void ABXGear::PostUnequip(UPARAM(ref) FBXEquipGearInformation& UnequipInfo)
{
	if ((EquipFunctions & 64) > 0)
	{
		InternalPostUnequip(UnequipInfo);
	}

	if ((EquipFunctions & 128) > 0)
	{
		ScriptPostUnequip(UnequipInfo);
	}

	AttachTarget = nullptr;
	OwnerComponent = nullptr;
}

void ABXGear::InternalPreEquip(FBXEquipGearInformation& EquipInfo)
{

}

void ABXGear::InternalPostEquip(FBXEquipGearInformation& EquipInfo)
{

}

void ABXGear::InternalPreUnequip(FBXEquipGearInformation& UnequipInfo)
{

}

void ABXGear::InternalPostUnequip(FBXEquipGearInformation& UnequipInfo)
{

}

#pragma endregion Equip



#pragma region Use
void ABXGear::PreUsing(UPARAM(ref) FBXUsingGearInformation& UsingInfo)
{
	if ((UseFunctions & 1) > 0)
	{
		InternalPreUsing(UsingInfo);
	}

	if ((UseFunctions & 2) > 0)
	{
		ScriptPreUsing(UsingInfo);
	}
}

void ABXGear::PostUsing(UPARAM(ref) FBXUsingGearInformation& UsingInfo)
{
	if ((UseFunctions & 4) > 0)
	{
		InternalPostUsing(UsingInfo);
	}

	if ((UseFunctions & 8) > 0)
	{
		ScriptPostUsing(UsingInfo);
	}
}

void ABXGear::PreUnusing(UPARAM(ref) FBXUsingGearInformation& UnusingInfo)
{
	if ((UseFunctions & 16) > 0)
	{
		InternalPreUnusing(UnusingInfo);
	}

	if ((UseFunctions & 32) > 0)
	{
		ScriptPreUnusing(UnusingInfo);
	}
}

void ABXGear::PostUnusing(UPARAM(ref) FBXUsingGearInformation& UnusingInfo)
{
	if ((UseFunctions & 64) > 0)
	{
		InternalPostUnusing(UnusingInfo);
	}

	if ((UseFunctions & 128) > 0)
	{
		ScriptPostUnusing(UnusingInfo);
	}
}

void ABXGear::InternalPreUsing(FBXUsingGearInformation& UsingInfo)
{

}

void ABXGear::InternalPostUsing(FBXUsingGearInformation& UsingInfo)
{
	SetActorHiddenInGame(false);
}

void ABXGear::InternalPreUnusing(FBXUsingGearInformation& UnusingInfo)
{

}

void ABXGear::InternalPostUnusing(FBXUsingGearInformation& UnusingInfo)
{
	SetActorHiddenInGame(true);
}

#pragma endregion Use



#pragma region State
EBXGearState ABXGear::GetCurrentState() const
{
	return CurrentState;
}

void ABXGear::ChangeState(EBXGearState NewState)
{
	if ((ChangeStateFunctions & 1) > 0)
	{
		InternalChangeState(NewState);
	}

	if ((ChangeStateFunctions & 2) > 0)
	{
		ScriptChangeState(NewState);
	}
}

void ABXGear::InternalChangeState(EBXGearState NewState)
{

}

#pragma endregion State



#pragma region Attach
void ABXGear::AttachToSocket()
{
	if (!AttachTarget || !OwnerComponent)
	{
		return;
	}

	USkeletalMesh* AttachMesh = AttachTarget->GetSkeletalMeshAsset();
	if (!AttachMesh)
	{
		return;
	}

	EBXGearSlot SlotType = OwnerComponent->GetUsingGearSlot(this);

	for (TArray<FBXGearAttachmentConfig>::TIterator It(AttachmentConfigs); It; ++It)
	{
		if (It->Mesh == AttachMesh && It->Slot == SlotType && It->State == CurrentState)
		{
			AttachToComponent(AttachTarget, FAttachmentTransformRules::KeepWorldTransform, It->Socket);

			SetActorRelativeTransform(It->Relation);

			return;
		}
	}
}

#pragma endregion Attach
