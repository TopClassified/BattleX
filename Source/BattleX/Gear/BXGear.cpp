#include "BXGear.h"



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
	if ((EquipFunctions & 256) > 0)
	{
		InternalPreUsing(UsingInfo);
	}

	if ((EquipFunctions & 512) > 0)
	{
		ScriptPreUsing(UsingInfo);
	}
}

void ABXGear::PostUsing(UPARAM(ref) FBXUsingGearInformation& UsingInfo)
{
	if ((EquipFunctions & 1024) > 0)
	{
		InternalPostUsing(UsingInfo);
	}

	if ((EquipFunctions & 2048) > 0)
	{
		ScriptPostUsing(UsingInfo);
	}
}

void ABXGear::PreUnusing(UPARAM(ref) FBXUsingGearInformation& UnusingInfo)
{
	if ((EquipFunctions & 4096) > 0)
	{
		InternalPreUnusing(UnusingInfo);
	}

	if ((EquipFunctions & 8192) > 0)
	{
		ScriptPreUnusing(UnusingInfo);
	}
}

void ABXGear::PostUnusing(UPARAM(ref) FBXUsingGearInformation& UnusingInfo)
{
	if ((EquipFunctions & 16384) > 0)
	{
		InternalPostUnusing(UnusingInfo);
	}

	if ((EquipFunctions & 32768) > 0)
	{
		ScriptPostUnusing(UnusingInfo);
	}
}

void ABXGear::InternalPreUsing(FBXUsingGearInformation& UsingInfo)
{

}

void ABXGear::InternalPostUsing(FBXUsingGearInformation& UsingInfo)
{

}

void ABXGear::InternalPreUnusing(FBXUsingGearInformation& UnusingInfo)
{

}

void ABXGear::InternalPostUnusing(FBXUsingGearInformation& UnusingInfo)
{

}

#pragma endregion Use



#pragma region Sheath
void ABXGear::PreSheath(UPARAM(ref) FBXSheathGearInformation& SheathInfo)
{
	if ((EquipFunctions & 256) > 0)
	{
		InternalPreSheath(SheathInfo);
	}

	if ((EquipFunctions & 512) > 0)
	{
		ScriptPreSheath(SheathInfo);
	}
}

void ABXGear::PostSheath(UPARAM(ref) FBXSheathGearInformation& SheathInfo)
{
	if ((EquipFunctions & 1024) > 0)
	{
		InternalPostSheath(SheathInfo);
	}

	if ((EquipFunctions & 2048) > 0)
	{
		ScriptPostSheath(SheathInfo);
	}
}

void ABXGear::PreUnsheath(UPARAM(ref) FBXSheathGearInformation& UnsheathInfo)
{
	if ((EquipFunctions & 4096) > 0)
	{
		InternalPreUnsheath(UnsheathInfo);
	}

	if ((EquipFunctions & 8192) > 0)
	{
		ScriptPreUnsheath(UnsheathInfo);
	}
}

void ABXGear::PostUnsheath(UPARAM(ref) FBXSheathGearInformation& UnsheathInfo)
{
	if ((EquipFunctions & 16384) > 0)
	{
		InternalPostUnsheath(UnsheathInfo);
	}

	if ((EquipFunctions & 32768) > 0)
	{
		ScriptPostUnsheath(UnsheathInfo);
	}
}

void ABXGear::InternalPreSheath(FBXSheathGearInformation& SheathInfo)
{

}

void ABXGear::InternalPostSheath(FBXSheathGearInformation& SheathInfo)
{

}

void ABXGear::InternalPreUnsheath(FBXSheathGearInformation& UnsheathInfo)
{

}

void ABXGear::InternalPostUnsheath(FBXSheathGearInformation& UnsheathInfo)
{

}

#pragma endregion Sheath
