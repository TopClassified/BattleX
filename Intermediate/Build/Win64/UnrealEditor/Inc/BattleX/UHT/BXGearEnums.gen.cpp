// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "BattleX/Gear/BXGearEnums.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeBXGearEnums() {}

// Begin Cross Module References
BATTLEX_API UEnum* Z_Construct_UEnum_BattleX_EBXEquipFunction();
BATTLEX_API UEnum* Z_Construct_UEnum_BattleX_EBXGearSlot();
BATTLEX_API UEnum* Z_Construct_UEnum_BattleX_EBXGearType();
BATTLEX_API UEnum* Z_Construct_UEnum_BattleX_EBXSheathFunction();
BATTLEX_API UEnum* Z_Construct_UEnum_BattleX_EBXUseFunction();
UPackage* Z_Construct_UPackage__Script_BattleX();
// End Cross Module References

// Begin Enum EBXGearType
static FEnumRegistrationInfo Z_Registration_Info_UEnum_EBXGearType;
static UEnum* EBXGearType_StaticEnum()
{
	if (!Z_Registration_Info_UEnum_EBXGearType.OuterSingleton)
	{
		Z_Registration_Info_UEnum_EBXGearType.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_BattleX_EBXGearType, (UObject*)Z_Construct_UPackage__Script_BattleX(), TEXT("EBXGearType"));
	}
	return Z_Registration_Info_UEnum_EBXGearType.OuterSingleton;
}
template<> BATTLEX_API UEnum* StaticEnum<EBXGearType>()
{
	return EBXGearType_StaticEnum();
}
struct Z_Construct_UEnum_BattleX_EBXGearType_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe8\xa3\x85\xe5\xa4\x87\xe7\xb1\xbb\xe5\x9e\x8b\n" },
#endif
		{ "GT_Armor.DisplayName", "\xe9\x98\xb2\xe5\x85\xb7" },
		{ "GT_Armor.Name", "EBXGearType::GT_Armor" },
		{ "GT_None.DisplayName", "\xe6\x9c\xaa\xe7\x9f\xa5" },
		{ "GT_None.Name", "EBXGearType::GT_None" },
		{ "GT_Ornament.DisplayName", "\xe8\xa3\x85\xe9\xa5\xb0\xe5\x93\x81" },
		{ "GT_Ornament.Name", "EBXGearType::GT_Ornament" },
		{ "GT_TMax.Hidden", "" },
		{ "GT_TMax.Name", "EBXGearType::GT_TMax" },
		{ "GT_Weapon.DisplayName", "\xe6\xad\xa6\xe5\x99\xa8" },
		{ "GT_Weapon.Name", "EBXGearType::GT_Weapon" },
		{ "ModuleRelativePath", "Gear/BXGearEnums.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe8\xa3\x85\xe5\xa4\x87\xe7\xb1\xbb\xe5\x9e\x8b" },
#endif
	};
#endif // WITH_METADATA
	static constexpr UECodeGen_Private::FEnumeratorParam Enumerators[] = {
		{ "EBXGearType::GT_None", (int64)EBXGearType::GT_None },
		{ "EBXGearType::GT_Weapon", (int64)EBXGearType::GT_Weapon },
		{ "EBXGearType::GT_Armor", (int64)EBXGearType::GT_Armor },
		{ "EBXGearType::GT_Ornament", (int64)EBXGearType::GT_Ornament },
		{ "EBXGearType::GT_TMax", (int64)EBXGearType::GT_TMax },
	};
	static const UECodeGen_Private::FEnumParams EnumParams;
};
const UECodeGen_Private::FEnumParams Z_Construct_UEnum_BattleX_EBXGearType_Statics::EnumParams = {
	(UObject*(*)())Z_Construct_UPackage__Script_BattleX,
	nullptr,
	"EBXGearType",
	"EBXGearType",
	Z_Construct_UEnum_BattleX_EBXGearType_Statics::Enumerators,
	RF_Public|RF_Transient|RF_MarkAsNative,
	UE_ARRAY_COUNT(Z_Construct_UEnum_BattleX_EBXGearType_Statics::Enumerators),
	EEnumFlags::None,
	(uint8)UEnum::ECppForm::EnumClass,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UEnum_BattleX_EBXGearType_Statics::Enum_MetaDataParams), Z_Construct_UEnum_BattleX_EBXGearType_Statics::Enum_MetaDataParams)
};
UEnum* Z_Construct_UEnum_BattleX_EBXGearType()
{
	if (!Z_Registration_Info_UEnum_EBXGearType.InnerSingleton)
	{
		UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_EBXGearType.InnerSingleton, Z_Construct_UEnum_BattleX_EBXGearType_Statics::EnumParams);
	}
	return Z_Registration_Info_UEnum_EBXGearType.InnerSingleton;
}
// End Enum EBXGearType

// Begin Enum EBXGearSlot
static FEnumRegistrationInfo Z_Registration_Info_UEnum_EBXGearSlot;
static UEnum* EBXGearSlot_StaticEnum()
{
	if (!Z_Registration_Info_UEnum_EBXGearSlot.OuterSingleton)
	{
		Z_Registration_Info_UEnum_EBXGearSlot.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_BattleX_EBXGearSlot, (UObject*)Z_Construct_UPackage__Script_BattleX(), TEXT("EBXGearSlot"));
	}
	return Z_Registration_Info_UEnum_EBXGearSlot.OuterSingleton;
}
template<> BATTLEX_API UEnum* StaticEnum<EBXGearSlot>()
{
	return EBXGearSlot_StaticEnum();
}
struct Z_Construct_UEnum_BattleX_EBXGearSlot_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe8\xa3\x85\xe5\xa4\x87\xe6\x8f\x92\xe6\xa7\xbd\n" },
#endif
		{ "GS_Arms.DisplayName", "\xe8\x83\xb3\xe8\x86\x8a" },
		{ "GS_Arms.Name", "EBXGearSlot::GS_Arms" },
		{ "GS_Chest.DisplayName", "\xe8\x83\xb8\xe9\x83\xa8" },
		{ "GS_Chest.Name", "EBXGearSlot::GS_Chest" },
		{ "GS_Head.DisplayName", "\xe5\xa4\xb4\xe9\x83\xa8" },
		{ "GS_Head.Name", "EBXGearSlot::GS_Head" },
		{ "GS_Jewelry1.DisplayName", "\xe9\xa5\xb0\xe5\x93\x81""1" },
		{ "GS_Jewelry1.Name", "EBXGearSlot::GS_Jewelry1" },
		{ "GS_Jewelry2.DisplayName", "\xe9\xa5\xb0\xe5\x93\x81""2" },
		{ "GS_Jewelry2.Name", "EBXGearSlot::GS_Jewelry2" },
		{ "GS_Jewelry3.DisplayName", "\xe9\xa5\xb0\xe5\x93\x81""3" },
		{ "GS_Jewelry3.Name", "EBXGearSlot::GS_Jewelry3" },
		{ "GS_Jewelry4.DisplayName", "\xe9\xa5\xb0\xe5\x93\x81""4" },
		{ "GS_Jewelry4.Name", "EBXGearSlot::GS_Jewelry4" },
		{ "GS_Jewelry5.DisplayName", "\xe9\xa5\xb0\xe5\x93\x81""5" },
		{ "GS_Jewelry5.Name", "EBXGearSlot::GS_Jewelry5" },
		{ "GS_Jewelry6.DisplayName", "\xe9\xa5\xb0\xe5\x93\x81""6" },
		{ "GS_Jewelry6.Name", "EBXGearSlot::GS_Jewelry6" },
		{ "GS_Jewelry7.DisplayName", "\xe9\xa5\xb0\xe5\x93\x81""7" },
		{ "GS_Jewelry7.Name", "EBXGearSlot::GS_Jewelry7" },
		{ "GS_Jewelry8.DisplayName", "\xe9\xa5\xb0\xe5\x93\x81""8" },
		{ "GS_Jewelry8.Name", "EBXGearSlot::GS_Jewelry8" },
		{ "GS_LeftFoot.DisplayName", "\xe5\xb7\xa6\xe8\x84\x9a" },
		{ "GS_LeftFoot.Name", "EBXGearSlot::GS_LeftFoot" },
		{ "GS_LeftHand.DisplayName", "\xe5\xb7\xa6\xe6\x89\x8b" },
		{ "GS_LeftHand.Name", "EBXGearSlot::GS_LeftHand" },
		{ "GS_Legs.DisplayName", "\xe8\x85\xbf\xe9\x83\xa8" },
		{ "GS_Legs.Name", "EBXGearSlot::GS_Legs" },
		{ "GS_None.DisplayName", "\xe6\x9c\xaa\xe7\x9f\xa5" },
		{ "GS_None.Name", "EBXGearSlot::GS_None" },
		{ "GS_RightFoot.DisplayName", "\xe5\x8f\xb3\xe8\x84\x9a" },
		{ "GS_RightFoot.Name", "EBXGearSlot::GS_RightFoot" },
		{ "GS_RightHand.DisplayName", "\xe5\x8f\xb3\xe6\x89\x8b" },
		{ "GS_RightHand.Name", "EBXGearSlot::GS_RightHand" },
		{ "GS_TMax.Hidden", "" },
		{ "GS_TMax.Name", "EBXGearSlot::GS_TMax" },
		{ "GS_Waist.DisplayName", "\xe8\x85\xb0\xe9\x83\xa8" },
		{ "GS_Waist.Name", "EBXGearSlot::GS_Waist" },
		{ "ModuleRelativePath", "Gear/BXGearEnums.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe8\xa3\x85\xe5\xa4\x87\xe6\x8f\x92\xe6\xa7\xbd" },
#endif
	};
#endif // WITH_METADATA
	static constexpr UECodeGen_Private::FEnumeratorParam Enumerators[] = {
		{ "EBXGearSlot::GS_None", (int64)EBXGearSlot::GS_None },
		{ "EBXGearSlot::GS_LeftHand", (int64)EBXGearSlot::GS_LeftHand },
		{ "EBXGearSlot::GS_RightHand", (int64)EBXGearSlot::GS_RightHand },
		{ "EBXGearSlot::GS_Head", (int64)EBXGearSlot::GS_Head },
		{ "EBXGearSlot::GS_Chest", (int64)EBXGearSlot::GS_Chest },
		{ "EBXGearSlot::GS_Arms", (int64)EBXGearSlot::GS_Arms },
		{ "EBXGearSlot::GS_Waist", (int64)EBXGearSlot::GS_Waist },
		{ "EBXGearSlot::GS_Legs", (int64)EBXGearSlot::GS_Legs },
		{ "EBXGearSlot::GS_LeftFoot", (int64)EBXGearSlot::GS_LeftFoot },
		{ "EBXGearSlot::GS_RightFoot", (int64)EBXGearSlot::GS_RightFoot },
		{ "EBXGearSlot::GS_Jewelry1", (int64)EBXGearSlot::GS_Jewelry1 },
		{ "EBXGearSlot::GS_Jewelry2", (int64)EBXGearSlot::GS_Jewelry2 },
		{ "EBXGearSlot::GS_Jewelry3", (int64)EBXGearSlot::GS_Jewelry3 },
		{ "EBXGearSlot::GS_Jewelry4", (int64)EBXGearSlot::GS_Jewelry4 },
		{ "EBXGearSlot::GS_Jewelry5", (int64)EBXGearSlot::GS_Jewelry5 },
		{ "EBXGearSlot::GS_Jewelry6", (int64)EBXGearSlot::GS_Jewelry6 },
		{ "EBXGearSlot::GS_Jewelry7", (int64)EBXGearSlot::GS_Jewelry7 },
		{ "EBXGearSlot::GS_Jewelry8", (int64)EBXGearSlot::GS_Jewelry8 },
		{ "EBXGearSlot::GS_TMax", (int64)EBXGearSlot::GS_TMax },
	};
	static const UECodeGen_Private::FEnumParams EnumParams;
};
const UECodeGen_Private::FEnumParams Z_Construct_UEnum_BattleX_EBXGearSlot_Statics::EnumParams = {
	(UObject*(*)())Z_Construct_UPackage__Script_BattleX,
	nullptr,
	"EBXGearSlot",
	"EBXGearSlot",
	Z_Construct_UEnum_BattleX_EBXGearSlot_Statics::Enumerators,
	RF_Public|RF_Transient|RF_MarkAsNative,
	UE_ARRAY_COUNT(Z_Construct_UEnum_BattleX_EBXGearSlot_Statics::Enumerators),
	EEnumFlags::None,
	(uint8)UEnum::ECppForm::EnumClass,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UEnum_BattleX_EBXGearSlot_Statics::Enum_MetaDataParams), Z_Construct_UEnum_BattleX_EBXGearSlot_Statics::Enum_MetaDataParams)
};
UEnum* Z_Construct_UEnum_BattleX_EBXGearSlot()
{
	if (!Z_Registration_Info_UEnum_EBXGearSlot.InnerSingleton)
	{
		UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_EBXGearSlot.InnerSingleton, Z_Construct_UEnum_BattleX_EBXGearSlot_Statics::EnumParams);
	}
	return Z_Registration_Info_UEnum_EBXGearSlot.InnerSingleton;
}
// End Enum EBXGearSlot

// Begin Enum EBXEquipFunction
static FEnumRegistrationInfo Z_Registration_Info_UEnum_EBXEquipFunction;
static UEnum* EBXEquipFunction_StaticEnum()
{
	if (!Z_Registration_Info_UEnum_EBXEquipFunction.OuterSingleton)
	{
		Z_Registration_Info_UEnum_EBXEquipFunction.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_BattleX_EBXEquipFunction, (UObject*)Z_Construct_UPackage__Script_BattleX(), TEXT("EBXEquipFunction"));
	}
	return Z_Registration_Info_UEnum_EBXEquipFunction.OuterSingleton;
}
template<> BATTLEX_API UEnum* StaticEnum<EBXEquipFunction>()
{
	return EBXEquipFunction_StaticEnum();
}
struct Z_Construct_UEnum_BattleX_EBXEquipFunction_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
		{ "Bitflags", "" },
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe8\xa3\x85\xe4\xb8\x8a/\xe5\x8d\xb8\xe4\xb8\x8b\xe8\xa3\x85\xe5\xa4\x87\xe8\xa6\x81\xe6\x89\xa7\xe8\xa1\x8c\xe7\x9a\x84\xe5\x87\xbd\xe6\x95\xb0\n" },
#endif
		{ "EF_BPPostEquip.DisplayName", "\xe8\x93\x9d\xe5\x9b\xbe\xe8\xa3\x85\xe4\xb8\x8a\xe4\xb9\x8b\xe5\x90\x8e" },
		{ "EF_BPPostEquip.Name", "EBXEquipFunction::EF_BPPostEquip" },
		{ "EF_BPPostUnequip.DisplayName", "\xe8\x93\x9d\xe5\x9b\xbe\xe5\x8d\xb8\xe4\xb8\x8b\xe4\xb9\x8b\xe5\x90\x8e" },
		{ "EF_BPPostUnequip.Name", "EBXEquipFunction::EF_BPPostUnequip" },
		{ "EF_BPPreEquip.DisplayName", "\xe8\x93\x9d\xe5\x9b\xbe\xe8\xa3\x85\xe4\xb8\x8a\xe4\xb9\x8b\xe5\x89\x8d" },
		{ "EF_BPPreEquip.Name", "EBXEquipFunction::EF_BPPreEquip" },
		{ "EF_BPPreUnequip.DisplayName", "\xe8\x93\x9d\xe5\x9b\xbe\xe5\x8d\xb8\xe4\xb8\x8b\xe4\xb9\x8b\xe5\x89\x8d" },
		{ "EF_BPPreUnequip.Name", "EBXEquipFunction::EF_BPPreUnequip" },
		{ "EF_NativePostEquip.DisplayName", "C++\xe8\xa3\x85\xe4\xb8\x8a\xe4\xb9\x8b\xe5\x90\x8e" },
		{ "EF_NativePostEquip.Name", "EBXEquipFunction::EF_NativePostEquip" },
		{ "EF_NativePostUnequip.DisplayName", "C++\xe5\x8d\xb8\xe4\xb8\x8b\xe4\xb9\x8b\xe5\x90\x8e" },
		{ "EF_NativePostUnequip.Name", "EBXEquipFunction::EF_NativePostUnequip" },
		{ "EF_NativePreEquip.DisplayName", "C++\xe8\xa3\x85\xe4\xb8\x8a\xe4\xb9\x8b\xe5\x89\x8d" },
		{ "EF_NativePreEquip.Name", "EBXEquipFunction::EF_NativePreEquip" },
		{ "EF_NativePreUnequip.DisplayName", "C++\xe5\x8d\xb8\xe4\xb8\x8b\xe4\xb9\x8b\xe5\x89\x8d" },
		{ "EF_NativePreUnequip.Name", "EBXEquipFunction::EF_NativePreUnequip" },
		{ "EF_TMax.Hidden", "" },
		{ "EF_TMax.Name", "EBXEquipFunction::EF_TMax" },
		{ "ModuleRelativePath", "Gear/BXGearEnums.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe8\xa3\x85\xe4\xb8\x8a/\xe5\x8d\xb8\xe4\xb8\x8b\xe8\xa3\x85\xe5\xa4\x87\xe8\xa6\x81\xe6\x89\xa7\xe8\xa1\x8c\xe7\x9a\x84\xe5\x87\xbd\xe6\x95\xb0" },
#endif
	};
#endif // WITH_METADATA
	static constexpr UECodeGen_Private::FEnumeratorParam Enumerators[] = {
		{ "EBXEquipFunction::EF_NativePreEquip", (int64)EBXEquipFunction::EF_NativePreEquip },
		{ "EBXEquipFunction::EF_BPPreEquip", (int64)EBXEquipFunction::EF_BPPreEquip },
		{ "EBXEquipFunction::EF_NativePostEquip", (int64)EBXEquipFunction::EF_NativePostEquip },
		{ "EBXEquipFunction::EF_BPPostEquip", (int64)EBXEquipFunction::EF_BPPostEquip },
		{ "EBXEquipFunction::EF_NativePreUnequip", (int64)EBXEquipFunction::EF_NativePreUnequip },
		{ "EBXEquipFunction::EF_BPPreUnequip", (int64)EBXEquipFunction::EF_BPPreUnequip },
		{ "EBXEquipFunction::EF_NativePostUnequip", (int64)EBXEquipFunction::EF_NativePostUnequip },
		{ "EBXEquipFunction::EF_BPPostUnequip", (int64)EBXEquipFunction::EF_BPPostUnequip },
		{ "EBXEquipFunction::EF_TMax", (int64)EBXEquipFunction::EF_TMax },
	};
	static const UECodeGen_Private::FEnumParams EnumParams;
};
const UECodeGen_Private::FEnumParams Z_Construct_UEnum_BattleX_EBXEquipFunction_Statics::EnumParams = {
	(UObject*(*)())Z_Construct_UPackage__Script_BattleX,
	nullptr,
	"EBXEquipFunction",
	"EBXEquipFunction",
	Z_Construct_UEnum_BattleX_EBXEquipFunction_Statics::Enumerators,
	RF_Public|RF_Transient|RF_MarkAsNative,
	UE_ARRAY_COUNT(Z_Construct_UEnum_BattleX_EBXEquipFunction_Statics::Enumerators),
	EEnumFlags::None,
	(uint8)UEnum::ECppForm::EnumClass,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UEnum_BattleX_EBXEquipFunction_Statics::Enum_MetaDataParams), Z_Construct_UEnum_BattleX_EBXEquipFunction_Statics::Enum_MetaDataParams)
};
UEnum* Z_Construct_UEnum_BattleX_EBXEquipFunction()
{
	if (!Z_Registration_Info_UEnum_EBXEquipFunction.InnerSingleton)
	{
		UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_EBXEquipFunction.InnerSingleton, Z_Construct_UEnum_BattleX_EBXEquipFunction_Statics::EnumParams);
	}
	return Z_Registration_Info_UEnum_EBXEquipFunction.InnerSingleton;
}
// End Enum EBXEquipFunction

// Begin Enum EBXUseFunction
static FEnumRegistrationInfo Z_Registration_Info_UEnum_EBXUseFunction;
static UEnum* EBXUseFunction_StaticEnum()
{
	if (!Z_Registration_Info_UEnum_EBXUseFunction.OuterSingleton)
	{
		Z_Registration_Info_UEnum_EBXUseFunction.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_BattleX_EBXUseFunction, (UObject*)Z_Construct_UPackage__Script_BattleX(), TEXT("EBXUseFunction"));
	}
	return Z_Registration_Info_UEnum_EBXUseFunction.OuterSingleton;
}
template<> BATTLEX_API UEnum* StaticEnum<EBXUseFunction>()
{
	return EBXUseFunction_StaticEnum();
}
struct Z_Construct_UEnum_BattleX_EBXUseFunction_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
		{ "Bitflags", "" },
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe4\xbd\xbf\xe7\x94\xa8/\xe5\xbc\x83\xe7\x94\xa8\xe8\xa3\x85\xe5\xa4\x87\xe8\xa6\x81\xe6\x89\xa7\xe8\xa1\x8c\xe7\x9a\x84\xe5\x87\xbd\xe6\x95\xb0\n" },
#endif
		{ "EF_BPPostUnusing.DisplayName", "\xe8\x93\x9d\xe5\x9b\xbe\xe5\xbc\x83\xe7\x94\xa8\xe4\xb9\x8b\xe5\x90\x8e" },
		{ "EF_BPPostUnusing.Name", "EBXUseFunction::EF_BPPostUnusing" },
		{ "EF_BPPostUsing.DisplayName", "\xe8\x93\x9d\xe5\x9b\xbe\xe4\xbd\xbf\xe7\x94\xa8\xe4\xb9\x8b\xe5\x90\x8e" },
		{ "EF_BPPostUsing.Name", "EBXUseFunction::EF_BPPostUsing" },
		{ "EF_BPPreUnusing.DisplayName", "\xe8\x93\x9d\xe5\x9b\xbe\xe5\xbc\x83\xe7\x94\xa8\xe4\xb9\x8b\xe5\x89\x8d" },
		{ "EF_BPPreUnusing.Name", "EBXUseFunction::EF_BPPreUnusing" },
		{ "EF_BPPreUsing.DisplayName", "\xe8\x93\x9d\xe5\x9b\xbe\xe4\xbd\xbf\xe7\x94\xa8\xe4\xb9\x8b\xe5\x89\x8d" },
		{ "EF_BPPreUsing.Name", "EBXUseFunction::EF_BPPreUsing" },
		{ "EF_NativePostUnusing.DisplayName", "C++\xe5\xbc\x83\xe7\x94\xa8\xe4\xb9\x8b\xe5\x90\x8e" },
		{ "EF_NativePostUnusing.Name", "EBXUseFunction::EF_NativePostUnusing" },
		{ "EF_NativePostUsing.DisplayName", "C++\xe4\xbd\xbf\xe7\x94\xa8\xe4\xb9\x8b\xe5\x90\x8e" },
		{ "EF_NativePostUsing.Name", "EBXUseFunction::EF_NativePostUsing" },
		{ "EF_NativePreUnusing.DisplayName", "C++\xe5\xbc\x83\xe7\x94\xa8\xe4\xb9\x8b\xe5\x89\x8d" },
		{ "EF_NativePreUnusing.Name", "EBXUseFunction::EF_NativePreUnusing" },
		{ "EF_NativePreUsing.DisplayName", "C++\xe4\xbd\xbf\xe7\x94\xa8\xe4\xb9\x8b\xe5\x89\x8d" },
		{ "EF_NativePreUsing.Name", "EBXUseFunction::EF_NativePreUsing" },
		{ "EF_TMax.Hidden", "" },
		{ "EF_TMax.Name", "EBXUseFunction::EF_TMax" },
		{ "ModuleRelativePath", "Gear/BXGearEnums.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe4\xbd\xbf\xe7\x94\xa8/\xe5\xbc\x83\xe7\x94\xa8\xe8\xa3\x85\xe5\xa4\x87\xe8\xa6\x81\xe6\x89\xa7\xe8\xa1\x8c\xe7\x9a\x84\xe5\x87\xbd\xe6\x95\xb0" },
#endif
	};
#endif // WITH_METADATA
	static constexpr UECodeGen_Private::FEnumeratorParam Enumerators[] = {
		{ "EBXUseFunction::EF_NativePreUsing", (int64)EBXUseFunction::EF_NativePreUsing },
		{ "EBXUseFunction::EF_BPPreUsing", (int64)EBXUseFunction::EF_BPPreUsing },
		{ "EBXUseFunction::EF_NativePostUsing", (int64)EBXUseFunction::EF_NativePostUsing },
		{ "EBXUseFunction::EF_BPPostUsing", (int64)EBXUseFunction::EF_BPPostUsing },
		{ "EBXUseFunction::EF_NativePreUnusing", (int64)EBXUseFunction::EF_NativePreUnusing },
		{ "EBXUseFunction::EF_BPPreUnusing", (int64)EBXUseFunction::EF_BPPreUnusing },
		{ "EBXUseFunction::EF_NativePostUnusing", (int64)EBXUseFunction::EF_NativePostUnusing },
		{ "EBXUseFunction::EF_BPPostUnusing", (int64)EBXUseFunction::EF_BPPostUnusing },
		{ "EBXUseFunction::EF_TMax", (int64)EBXUseFunction::EF_TMax },
	};
	static const UECodeGen_Private::FEnumParams EnumParams;
};
const UECodeGen_Private::FEnumParams Z_Construct_UEnum_BattleX_EBXUseFunction_Statics::EnumParams = {
	(UObject*(*)())Z_Construct_UPackage__Script_BattleX,
	nullptr,
	"EBXUseFunction",
	"EBXUseFunction",
	Z_Construct_UEnum_BattleX_EBXUseFunction_Statics::Enumerators,
	RF_Public|RF_Transient|RF_MarkAsNative,
	UE_ARRAY_COUNT(Z_Construct_UEnum_BattleX_EBXUseFunction_Statics::Enumerators),
	EEnumFlags::None,
	(uint8)UEnum::ECppForm::EnumClass,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UEnum_BattleX_EBXUseFunction_Statics::Enum_MetaDataParams), Z_Construct_UEnum_BattleX_EBXUseFunction_Statics::Enum_MetaDataParams)
};
UEnum* Z_Construct_UEnum_BattleX_EBXUseFunction()
{
	if (!Z_Registration_Info_UEnum_EBXUseFunction.InnerSingleton)
	{
		UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_EBXUseFunction.InnerSingleton, Z_Construct_UEnum_BattleX_EBXUseFunction_Statics::EnumParams);
	}
	return Z_Registration_Info_UEnum_EBXUseFunction.InnerSingleton;
}
// End Enum EBXUseFunction

// Begin Enum EBXSheathFunction
static FEnumRegistrationInfo Z_Registration_Info_UEnum_EBXSheathFunction;
static UEnum* EBXSheathFunction_StaticEnum()
{
	if (!Z_Registration_Info_UEnum_EBXSheathFunction.OuterSingleton)
	{
		Z_Registration_Info_UEnum_EBXSheathFunction.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_BattleX_EBXSheathFunction, (UObject*)Z_Construct_UPackage__Script_BattleX(), TEXT("EBXSheathFunction"));
	}
	return Z_Registration_Info_UEnum_EBXSheathFunction.OuterSingleton;
}
template<> BATTLEX_API UEnum* StaticEnum<EBXSheathFunction>()
{
	return EBXSheathFunction_StaticEnum();
}
struct Z_Construct_UEnum_BattleX_EBXSheathFunction_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
		{ "Bitflags", "" },
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe5\x87\xba\xe9\x9e\x98/\xe5\x85\xa5\xe9\x9e\x98\xe8\xa3\x85\xe5\xa4\x87\xe8\xa6\x81\xe6\x89\xa7\xe8\xa1\x8c\xe7\x9a\x84\xe5\x87\xbd\xe6\x95\xb0\n" },
#endif
		{ "EF_BPPostSheath.DisplayName", "\xe8\x93\x9d\xe5\x9b\xbe\xe5\x85\xa5\xe9\x9e\x98\xe4\xb9\x8b\xe5\x90\x8e" },
		{ "EF_BPPostSheath.Name", "EBXSheathFunction::EF_BPPostSheath" },
		{ "EF_BPPostUnsheath.DisplayName", "\xe8\x93\x9d\xe5\x9b\xbe\xe5\x87\xba\xe9\x9e\x98\xe4\xb9\x8b\xe5\x90\x8e" },
		{ "EF_BPPostUnsheath.Name", "EBXSheathFunction::EF_BPPostUnsheath" },
		{ "EF_BPPreSheath.DisplayName", "\xe8\x93\x9d\xe5\x9b\xbe\xe5\x85\xa5\xe9\x9e\x98\xe4\xb9\x8b\xe5\x89\x8d" },
		{ "EF_BPPreSheath.Name", "EBXSheathFunction::EF_BPPreSheath" },
		{ "EF_BPPreUnsheath.DisplayName", "\xe8\x93\x9d\xe5\x9b\xbe\xe5\x87\xba\xe9\x9e\x98\xe4\xb9\x8b\xe5\x89\x8d" },
		{ "EF_BPPreUnsheath.Name", "EBXSheathFunction::EF_BPPreUnsheath" },
		{ "EF_NativePostSheath.DisplayName", "C++\xe5\x85\xa5\xe9\x9e\x98\xe4\xb9\x8b\xe5\x90\x8e" },
		{ "EF_NativePostSheath.Name", "EBXSheathFunction::EF_NativePostSheath" },
		{ "EF_NativePostUnsheath.DisplayName", "C++\xe5\x87\xba\xe9\x9e\x98\xe4\xb9\x8b\xe5\x90\x8e" },
		{ "EF_NativePostUnsheath.Name", "EBXSheathFunction::EF_NativePostUnsheath" },
		{ "EF_NativePreSheath.DisplayName", "C++\xe5\x85\xa5\xe9\x9e\x98\xe4\xb9\x8b\xe5\x89\x8d" },
		{ "EF_NativePreSheath.Name", "EBXSheathFunction::EF_NativePreSheath" },
		{ "EF_NativePreUnsheath.DisplayName", "C++\xe5\x87\xba\xe9\x9e\x98\xe4\xb9\x8b\xe5\x89\x8d" },
		{ "EF_NativePreUnsheath.Name", "EBXSheathFunction::EF_NativePreUnsheath" },
		{ "EF_TMax.Hidden", "" },
		{ "EF_TMax.Name", "EBXSheathFunction::EF_TMax" },
		{ "ModuleRelativePath", "Gear/BXGearEnums.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe5\x87\xba\xe9\x9e\x98/\xe5\x85\xa5\xe9\x9e\x98\xe8\xa3\x85\xe5\xa4\x87\xe8\xa6\x81\xe6\x89\xa7\xe8\xa1\x8c\xe7\x9a\x84\xe5\x87\xbd\xe6\x95\xb0" },
#endif
	};
#endif // WITH_METADATA
	static constexpr UECodeGen_Private::FEnumeratorParam Enumerators[] = {
		{ "EBXSheathFunction::EF_NativePreSheath", (int64)EBXSheathFunction::EF_NativePreSheath },
		{ "EBXSheathFunction::EF_BPPreSheath", (int64)EBXSheathFunction::EF_BPPreSheath },
		{ "EBXSheathFunction::EF_NativePostSheath", (int64)EBXSheathFunction::EF_NativePostSheath },
		{ "EBXSheathFunction::EF_BPPostSheath", (int64)EBXSheathFunction::EF_BPPostSheath },
		{ "EBXSheathFunction::EF_NativePreUnsheath", (int64)EBXSheathFunction::EF_NativePreUnsheath },
		{ "EBXSheathFunction::EF_BPPreUnsheath", (int64)EBXSheathFunction::EF_BPPreUnsheath },
		{ "EBXSheathFunction::EF_NativePostUnsheath", (int64)EBXSheathFunction::EF_NativePostUnsheath },
		{ "EBXSheathFunction::EF_BPPostUnsheath", (int64)EBXSheathFunction::EF_BPPostUnsheath },
		{ "EBXSheathFunction::EF_TMax", (int64)EBXSheathFunction::EF_TMax },
	};
	static const UECodeGen_Private::FEnumParams EnumParams;
};
const UECodeGen_Private::FEnumParams Z_Construct_UEnum_BattleX_EBXSheathFunction_Statics::EnumParams = {
	(UObject*(*)())Z_Construct_UPackage__Script_BattleX,
	nullptr,
	"EBXSheathFunction",
	"EBXSheathFunction",
	Z_Construct_UEnum_BattleX_EBXSheathFunction_Statics::Enumerators,
	RF_Public|RF_Transient|RF_MarkAsNative,
	UE_ARRAY_COUNT(Z_Construct_UEnum_BattleX_EBXSheathFunction_Statics::Enumerators),
	EEnumFlags::None,
	(uint8)UEnum::ECppForm::EnumClass,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UEnum_BattleX_EBXSheathFunction_Statics::Enum_MetaDataParams), Z_Construct_UEnum_BattleX_EBXSheathFunction_Statics::Enum_MetaDataParams)
};
UEnum* Z_Construct_UEnum_BattleX_EBXSheathFunction()
{
	if (!Z_Registration_Info_UEnum_EBXSheathFunction.InnerSingleton)
	{
		UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_EBXSheathFunction.InnerSingleton, Z_Construct_UEnum_BattleX_EBXSheathFunction_Statics::EnumParams);
	}
	return Z_Registration_Info_UEnum_EBXSheathFunction.InnerSingleton;
}
// End Enum EBXSheathFunction

// Begin Registration
struct Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Gear_BXGearEnums_h_Statics
{
	static constexpr FEnumRegisterCompiledInInfo EnumInfo[] = {
		{ EBXGearType_StaticEnum, TEXT("EBXGearType"), &Z_Registration_Info_UEnum_EBXGearType, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 3771108800U) },
		{ EBXGearSlot_StaticEnum, TEXT("EBXGearSlot"), &Z_Registration_Info_UEnum_EBXGearSlot, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 1739704360U) },
		{ EBXEquipFunction_StaticEnum, TEXT("EBXEquipFunction"), &Z_Registration_Info_UEnum_EBXEquipFunction, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 1920873858U) },
		{ EBXUseFunction_StaticEnum, TEXT("EBXUseFunction"), &Z_Registration_Info_UEnum_EBXUseFunction, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 3393557222U) },
		{ EBXSheathFunction_StaticEnum, TEXT("EBXSheathFunction"), &Z_Registration_Info_UEnum_EBXSheathFunction, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 367830168U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Gear_BXGearEnums_h_2750548456(TEXT("/Script/BattleX"),
	nullptr, 0,
	nullptr, 0,
	Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Gear_BXGearEnums_h_Statics::EnumInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Gear_BXGearEnums_h_Statics::EnumInfo));
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
