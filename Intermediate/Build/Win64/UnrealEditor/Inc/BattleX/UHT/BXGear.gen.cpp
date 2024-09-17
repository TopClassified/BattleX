// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "BattleX/Gear/BXGear.h"
#include "BattleX/Gear/BXGearStructs.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeBXGear() {}

// Begin Cross Module References
BATTLEX_API UClass* Z_Construct_UClass_ABXGear();
BATTLEX_API UClass* Z_Construct_UClass_ABXGear_NoRegister();
BATTLEX_API UClass* Z_Construct_UClass_UBXGearData_NoRegister();
BATTLEX_API UEnum* Z_Construct_UEnum_BattleX_EBXGearType();
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXEquipGearInformation();
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXSheathGearInformation();
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXUsingGearInformation();
ENGINE_API UClass* Z_Construct_UClass_AActor();
UPackage* Z_Construct_UPackage__Script_BattleX();
// End Cross Module References

// Begin Class ABXGear Function PostEquip
struct Z_Construct_UFunction_ABXGear_PostEquip_Statics
{
	struct BXGear_eventPostEquip_Parms
	{
		FBXEquipGearInformation EquipInfo;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Equip" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xd1\xbe\xef\xbf\xbd\xd7\xb0\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Gear/BXGear.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xd1\xbe\xef\xbf\xbd\xd7\xb0\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_EquipInfo;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_ABXGear_PostEquip_Statics::NewProp_EquipInfo = { "EquipInfo", nullptr, (EPropertyFlags)0x0010000008000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXGear_eventPostEquip_Parms, EquipInfo), Z_Construct_UScriptStruct_FBXEquipGearInformation, METADATA_PARAMS(0, nullptr) }; // 974305362
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_ABXGear_PostEquip_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ABXGear_PostEquip_Statics::NewProp_EquipInfo,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_ABXGear_PostEquip_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ABXGear_PostEquip_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_ABXGear, nullptr, "PostEquip", nullptr, nullptr, Z_Construct_UFunction_ABXGear_PostEquip_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_ABXGear_PostEquip_Statics::PropPointers), sizeof(Z_Construct_UFunction_ABXGear_PostEquip_Statics::BXGear_eventPostEquip_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04420401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_ABXGear_PostEquip_Statics::Function_MetaDataParams), Z_Construct_UFunction_ABXGear_PostEquip_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_ABXGear_PostEquip_Statics::BXGear_eventPostEquip_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_ABXGear_PostEquip()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ABXGear_PostEquip_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(ABXGear::execPostEquip)
{
	P_GET_STRUCT_REF(FBXEquipGearInformation,Z_Param_Out_EquipInfo);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->PostEquip(Z_Param_Out_EquipInfo);
	P_NATIVE_END;
}
// End Class ABXGear Function PostEquip

// Begin Class ABXGear Function PostSheath
struct Z_Construct_UFunction_ABXGear_PostSheath_Statics
{
	struct BXGear_eventPostSheath_Parms
	{
		FBXSheathGearInformation SheathInfo;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Sheath" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xd1\xbe\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Gear/BXGear.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xd1\xbe\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_SheathInfo;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_ABXGear_PostSheath_Statics::NewProp_SheathInfo = { "SheathInfo", nullptr, (EPropertyFlags)0x0010000008000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXGear_eventPostSheath_Parms, SheathInfo), Z_Construct_UScriptStruct_FBXSheathGearInformation, METADATA_PARAMS(0, nullptr) }; // 1661981629
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_ABXGear_PostSheath_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ABXGear_PostSheath_Statics::NewProp_SheathInfo,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_ABXGear_PostSheath_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ABXGear_PostSheath_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_ABXGear, nullptr, "PostSheath", nullptr, nullptr, Z_Construct_UFunction_ABXGear_PostSheath_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_ABXGear_PostSheath_Statics::PropPointers), sizeof(Z_Construct_UFunction_ABXGear_PostSheath_Statics::BXGear_eventPostSheath_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04420401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_ABXGear_PostSheath_Statics::Function_MetaDataParams), Z_Construct_UFunction_ABXGear_PostSheath_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_ABXGear_PostSheath_Statics::BXGear_eventPostSheath_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_ABXGear_PostSheath()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ABXGear_PostSheath_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(ABXGear::execPostSheath)
{
	P_GET_STRUCT_REF(FBXSheathGearInformation,Z_Param_Out_SheathInfo);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->PostSheath(Z_Param_Out_SheathInfo);
	P_NATIVE_END;
}
// End Class ABXGear Function PostSheath

// Begin Class ABXGear Function PostUnequip
struct Z_Construct_UFunction_ABXGear_PostUnequip_Statics
{
	struct BXGear_eventPostUnequip_Parms
	{
		FBXEquipGearInformation UnequipInfo;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Equip" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xd1\xbe\xef\xbf\xbd\xd0\xb6\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Gear/BXGear.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xd1\xbe\xef\xbf\xbd\xd0\xb6\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_UnequipInfo;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_ABXGear_PostUnequip_Statics::NewProp_UnequipInfo = { "UnequipInfo", nullptr, (EPropertyFlags)0x0010000008000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXGear_eventPostUnequip_Parms, UnequipInfo), Z_Construct_UScriptStruct_FBXEquipGearInformation, METADATA_PARAMS(0, nullptr) }; // 974305362
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_ABXGear_PostUnequip_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ABXGear_PostUnequip_Statics::NewProp_UnequipInfo,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_ABXGear_PostUnequip_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ABXGear_PostUnequip_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_ABXGear, nullptr, "PostUnequip", nullptr, nullptr, Z_Construct_UFunction_ABXGear_PostUnequip_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_ABXGear_PostUnequip_Statics::PropPointers), sizeof(Z_Construct_UFunction_ABXGear_PostUnequip_Statics::BXGear_eventPostUnequip_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04420401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_ABXGear_PostUnequip_Statics::Function_MetaDataParams), Z_Construct_UFunction_ABXGear_PostUnequip_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_ABXGear_PostUnequip_Statics::BXGear_eventPostUnequip_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_ABXGear_PostUnequip()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ABXGear_PostUnequip_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(ABXGear::execPostUnequip)
{
	P_GET_STRUCT_REF(FBXEquipGearInformation,Z_Param_Out_UnequipInfo);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->PostUnequip(Z_Param_Out_UnequipInfo);
	P_NATIVE_END;
}
// End Class ABXGear Function PostUnequip

// Begin Class ABXGear Function PostUnsheath
struct Z_Construct_UFunction_ABXGear_PostUnsheath_Statics
{
	struct BXGear_eventPostUnsheath_Parms
	{
		FBXSheathGearInformation UnsheathInfo;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Sheath" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xd1\xbe\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Gear/BXGear.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xd1\xbe\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_UnsheathInfo;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_ABXGear_PostUnsheath_Statics::NewProp_UnsheathInfo = { "UnsheathInfo", nullptr, (EPropertyFlags)0x0010000008000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXGear_eventPostUnsheath_Parms, UnsheathInfo), Z_Construct_UScriptStruct_FBXSheathGearInformation, METADATA_PARAMS(0, nullptr) }; // 1661981629
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_ABXGear_PostUnsheath_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ABXGear_PostUnsheath_Statics::NewProp_UnsheathInfo,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_ABXGear_PostUnsheath_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ABXGear_PostUnsheath_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_ABXGear, nullptr, "PostUnsheath", nullptr, nullptr, Z_Construct_UFunction_ABXGear_PostUnsheath_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_ABXGear_PostUnsheath_Statics::PropPointers), sizeof(Z_Construct_UFunction_ABXGear_PostUnsheath_Statics::BXGear_eventPostUnsheath_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04420401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_ABXGear_PostUnsheath_Statics::Function_MetaDataParams), Z_Construct_UFunction_ABXGear_PostUnsheath_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_ABXGear_PostUnsheath_Statics::BXGear_eventPostUnsheath_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_ABXGear_PostUnsheath()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ABXGear_PostUnsheath_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(ABXGear::execPostUnsheath)
{
	P_GET_STRUCT_REF(FBXSheathGearInformation,Z_Param_Out_UnsheathInfo);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->PostUnsheath(Z_Param_Out_UnsheathInfo);
	P_NATIVE_END;
}
// End Class ABXGear Function PostUnsheath

// Begin Class ABXGear Function PostUnusing
struct Z_Construct_UFunction_ABXGear_PostUnusing_Statics
{
	struct BXGear_eventPostUnusing_Parms
	{
		FBXUsingGearInformation UnusingInfo;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Use" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xd1\xbe\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Gear/BXGear.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xd1\xbe\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_UnusingInfo;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_ABXGear_PostUnusing_Statics::NewProp_UnusingInfo = { "UnusingInfo", nullptr, (EPropertyFlags)0x0010000008000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXGear_eventPostUnusing_Parms, UnusingInfo), Z_Construct_UScriptStruct_FBXUsingGearInformation, METADATA_PARAMS(0, nullptr) }; // 2397478069
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_ABXGear_PostUnusing_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ABXGear_PostUnusing_Statics::NewProp_UnusingInfo,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_ABXGear_PostUnusing_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ABXGear_PostUnusing_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_ABXGear, nullptr, "PostUnusing", nullptr, nullptr, Z_Construct_UFunction_ABXGear_PostUnusing_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_ABXGear_PostUnusing_Statics::PropPointers), sizeof(Z_Construct_UFunction_ABXGear_PostUnusing_Statics::BXGear_eventPostUnusing_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04420401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_ABXGear_PostUnusing_Statics::Function_MetaDataParams), Z_Construct_UFunction_ABXGear_PostUnusing_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_ABXGear_PostUnusing_Statics::BXGear_eventPostUnusing_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_ABXGear_PostUnusing()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ABXGear_PostUnusing_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(ABXGear::execPostUnusing)
{
	P_GET_STRUCT_REF(FBXUsingGearInformation,Z_Param_Out_UnusingInfo);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->PostUnusing(Z_Param_Out_UnusingInfo);
	P_NATIVE_END;
}
// End Class ABXGear Function PostUnusing

// Begin Class ABXGear Function PostUsing
struct Z_Construct_UFunction_ABXGear_PostUsing_Statics
{
	struct BXGear_eventPostUsing_Parms
	{
		FBXUsingGearInformation UsingInfo;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Use" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xd1\xbe\xef\xbf\xbd\xca\xb9\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Gear/BXGear.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xd1\xbe\xef\xbf\xbd\xca\xb9\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_UsingInfo;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_ABXGear_PostUsing_Statics::NewProp_UsingInfo = { "UsingInfo", nullptr, (EPropertyFlags)0x0010000008000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXGear_eventPostUsing_Parms, UsingInfo), Z_Construct_UScriptStruct_FBXUsingGearInformation, METADATA_PARAMS(0, nullptr) }; // 2397478069
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_ABXGear_PostUsing_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ABXGear_PostUsing_Statics::NewProp_UsingInfo,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_ABXGear_PostUsing_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ABXGear_PostUsing_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_ABXGear, nullptr, "PostUsing", nullptr, nullptr, Z_Construct_UFunction_ABXGear_PostUsing_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_ABXGear_PostUsing_Statics::PropPointers), sizeof(Z_Construct_UFunction_ABXGear_PostUsing_Statics::BXGear_eventPostUsing_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04420401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_ABXGear_PostUsing_Statics::Function_MetaDataParams), Z_Construct_UFunction_ABXGear_PostUsing_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_ABXGear_PostUsing_Statics::BXGear_eventPostUsing_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_ABXGear_PostUsing()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ABXGear_PostUsing_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(ABXGear::execPostUsing)
{
	P_GET_STRUCT_REF(FBXUsingGearInformation,Z_Param_Out_UsingInfo);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->PostUsing(Z_Param_Out_UsingInfo);
	P_NATIVE_END;
}
// End Class ABXGear Function PostUsing

// Begin Class ABXGear Function PreEquip
struct Z_Construct_UFunction_ABXGear_PreEquip_Statics
{
	struct BXGear_eventPreEquip_Parms
	{
		FBXEquipGearInformation EquipInfo;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Equip" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xef\xbf\xbd\xd2\xaa\xd7\xb0\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Gear/BXGear.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xef\xbf\xbd\xd2\xaa\xd7\xb0\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_EquipInfo;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_ABXGear_PreEquip_Statics::NewProp_EquipInfo = { "EquipInfo", nullptr, (EPropertyFlags)0x0010000008000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXGear_eventPreEquip_Parms, EquipInfo), Z_Construct_UScriptStruct_FBXEquipGearInformation, METADATA_PARAMS(0, nullptr) }; // 974305362
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_ABXGear_PreEquip_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ABXGear_PreEquip_Statics::NewProp_EquipInfo,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_ABXGear_PreEquip_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ABXGear_PreEquip_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_ABXGear, nullptr, "PreEquip", nullptr, nullptr, Z_Construct_UFunction_ABXGear_PreEquip_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_ABXGear_PreEquip_Statics::PropPointers), sizeof(Z_Construct_UFunction_ABXGear_PreEquip_Statics::BXGear_eventPreEquip_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04420401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_ABXGear_PreEquip_Statics::Function_MetaDataParams), Z_Construct_UFunction_ABXGear_PreEquip_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_ABXGear_PreEquip_Statics::BXGear_eventPreEquip_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_ABXGear_PreEquip()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ABXGear_PreEquip_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(ABXGear::execPreEquip)
{
	P_GET_STRUCT_REF(FBXEquipGearInformation,Z_Param_Out_EquipInfo);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->PreEquip(Z_Param_Out_EquipInfo);
	P_NATIVE_END;
}
// End Class ABXGear Function PreEquip

// Begin Class ABXGear Function PreSheath
struct Z_Construct_UFunction_ABXGear_PreSheath_Statics
{
	struct BXGear_eventPreSheath_Parms
	{
		FBXSheathGearInformation SheathInfo;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Sheath" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xef\xbf\xbd\xd2\xaa\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Gear/BXGear.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xef\xbf\xbd\xd2\xaa\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_SheathInfo;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_ABXGear_PreSheath_Statics::NewProp_SheathInfo = { "SheathInfo", nullptr, (EPropertyFlags)0x0010000008000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXGear_eventPreSheath_Parms, SheathInfo), Z_Construct_UScriptStruct_FBXSheathGearInformation, METADATA_PARAMS(0, nullptr) }; // 1661981629
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_ABXGear_PreSheath_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ABXGear_PreSheath_Statics::NewProp_SheathInfo,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_ABXGear_PreSheath_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ABXGear_PreSheath_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_ABXGear, nullptr, "PreSheath", nullptr, nullptr, Z_Construct_UFunction_ABXGear_PreSheath_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_ABXGear_PreSheath_Statics::PropPointers), sizeof(Z_Construct_UFunction_ABXGear_PreSheath_Statics::BXGear_eventPreSheath_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04420401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_ABXGear_PreSheath_Statics::Function_MetaDataParams), Z_Construct_UFunction_ABXGear_PreSheath_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_ABXGear_PreSheath_Statics::BXGear_eventPreSheath_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_ABXGear_PreSheath()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ABXGear_PreSheath_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(ABXGear::execPreSheath)
{
	P_GET_STRUCT_REF(FBXSheathGearInformation,Z_Param_Out_SheathInfo);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->PreSheath(Z_Param_Out_SheathInfo);
	P_NATIVE_END;
}
// End Class ABXGear Function PreSheath

// Begin Class ABXGear Function PreUnequip
struct Z_Construct_UFunction_ABXGear_PreUnequip_Statics
{
	struct BXGear_eventPreUnequip_Parms
	{
		FBXEquipGearInformation UnequipInfo;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Equip" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xef\xbf\xbd\xd2\xaa\xd0\xb6\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Gear/BXGear.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xef\xbf\xbd\xd2\xaa\xd0\xb6\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_UnequipInfo;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_ABXGear_PreUnequip_Statics::NewProp_UnequipInfo = { "UnequipInfo", nullptr, (EPropertyFlags)0x0010000008000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXGear_eventPreUnequip_Parms, UnequipInfo), Z_Construct_UScriptStruct_FBXEquipGearInformation, METADATA_PARAMS(0, nullptr) }; // 974305362
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_ABXGear_PreUnequip_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ABXGear_PreUnequip_Statics::NewProp_UnequipInfo,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_ABXGear_PreUnequip_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ABXGear_PreUnequip_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_ABXGear, nullptr, "PreUnequip", nullptr, nullptr, Z_Construct_UFunction_ABXGear_PreUnequip_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_ABXGear_PreUnequip_Statics::PropPointers), sizeof(Z_Construct_UFunction_ABXGear_PreUnequip_Statics::BXGear_eventPreUnequip_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04420401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_ABXGear_PreUnequip_Statics::Function_MetaDataParams), Z_Construct_UFunction_ABXGear_PreUnequip_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_ABXGear_PreUnequip_Statics::BXGear_eventPreUnequip_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_ABXGear_PreUnequip()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ABXGear_PreUnequip_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(ABXGear::execPreUnequip)
{
	P_GET_STRUCT_REF(FBXEquipGearInformation,Z_Param_Out_UnequipInfo);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->PreUnequip(Z_Param_Out_UnequipInfo);
	P_NATIVE_END;
}
// End Class ABXGear Function PreUnequip

// Begin Class ABXGear Function PreUnsheath
struct Z_Construct_UFunction_ABXGear_PreUnsheath_Statics
{
	struct BXGear_eventPreUnsheath_Parms
	{
		FBXSheathGearInformation UnsheathInfo;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Sheath" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xef\xbf\xbd\xd2\xaa\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Gear/BXGear.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xef\xbf\xbd\xd2\xaa\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_UnsheathInfo;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_ABXGear_PreUnsheath_Statics::NewProp_UnsheathInfo = { "UnsheathInfo", nullptr, (EPropertyFlags)0x0010000008000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXGear_eventPreUnsheath_Parms, UnsheathInfo), Z_Construct_UScriptStruct_FBXSheathGearInformation, METADATA_PARAMS(0, nullptr) }; // 1661981629
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_ABXGear_PreUnsheath_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ABXGear_PreUnsheath_Statics::NewProp_UnsheathInfo,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_ABXGear_PreUnsheath_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ABXGear_PreUnsheath_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_ABXGear, nullptr, "PreUnsheath", nullptr, nullptr, Z_Construct_UFunction_ABXGear_PreUnsheath_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_ABXGear_PreUnsheath_Statics::PropPointers), sizeof(Z_Construct_UFunction_ABXGear_PreUnsheath_Statics::BXGear_eventPreUnsheath_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04420401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_ABXGear_PreUnsheath_Statics::Function_MetaDataParams), Z_Construct_UFunction_ABXGear_PreUnsheath_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_ABXGear_PreUnsheath_Statics::BXGear_eventPreUnsheath_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_ABXGear_PreUnsheath()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ABXGear_PreUnsheath_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(ABXGear::execPreUnsheath)
{
	P_GET_STRUCT_REF(FBXSheathGearInformation,Z_Param_Out_UnsheathInfo);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->PreUnsheath(Z_Param_Out_UnsheathInfo);
	P_NATIVE_END;
}
// End Class ABXGear Function PreUnsheath

// Begin Class ABXGear Function PreUnusing
struct Z_Construct_UFunction_ABXGear_PreUnusing_Statics
{
	struct BXGear_eventPreUnusing_Parms
	{
		FBXUsingGearInformation UnusingInfo;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Use" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xef\xbf\xbd\xd2\xaa\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Gear/BXGear.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xef\xbf\xbd\xd2\xaa\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_UnusingInfo;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_ABXGear_PreUnusing_Statics::NewProp_UnusingInfo = { "UnusingInfo", nullptr, (EPropertyFlags)0x0010000008000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXGear_eventPreUnusing_Parms, UnusingInfo), Z_Construct_UScriptStruct_FBXUsingGearInformation, METADATA_PARAMS(0, nullptr) }; // 2397478069
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_ABXGear_PreUnusing_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ABXGear_PreUnusing_Statics::NewProp_UnusingInfo,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_ABXGear_PreUnusing_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ABXGear_PreUnusing_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_ABXGear, nullptr, "PreUnusing", nullptr, nullptr, Z_Construct_UFunction_ABXGear_PreUnusing_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_ABXGear_PreUnusing_Statics::PropPointers), sizeof(Z_Construct_UFunction_ABXGear_PreUnusing_Statics::BXGear_eventPreUnusing_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04420401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_ABXGear_PreUnusing_Statics::Function_MetaDataParams), Z_Construct_UFunction_ABXGear_PreUnusing_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_ABXGear_PreUnusing_Statics::BXGear_eventPreUnusing_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_ABXGear_PreUnusing()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ABXGear_PreUnusing_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(ABXGear::execPreUnusing)
{
	P_GET_STRUCT_REF(FBXUsingGearInformation,Z_Param_Out_UnusingInfo);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->PreUnusing(Z_Param_Out_UnusingInfo);
	P_NATIVE_END;
}
// End Class ABXGear Function PreUnusing

// Begin Class ABXGear Function PreUsing
struct Z_Construct_UFunction_ABXGear_PreUsing_Statics
{
	struct BXGear_eventPreUsing_Parms
	{
		FBXUsingGearInformation UsingInfo;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Use" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xef\xbf\xbd\xd2\xaa\xca\xb9\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Gear/BXGear.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xef\xbf\xbd\xd2\xaa\xca\xb9\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_UsingInfo;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_ABXGear_PreUsing_Statics::NewProp_UsingInfo = { "UsingInfo", nullptr, (EPropertyFlags)0x0010000008000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXGear_eventPreUsing_Parms, UsingInfo), Z_Construct_UScriptStruct_FBXUsingGearInformation, METADATA_PARAMS(0, nullptr) }; // 2397478069
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_ABXGear_PreUsing_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ABXGear_PreUsing_Statics::NewProp_UsingInfo,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_ABXGear_PreUsing_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ABXGear_PreUsing_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_ABXGear, nullptr, "PreUsing", nullptr, nullptr, Z_Construct_UFunction_ABXGear_PreUsing_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_ABXGear_PreUsing_Statics::PropPointers), sizeof(Z_Construct_UFunction_ABXGear_PreUsing_Statics::BXGear_eventPreUsing_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04420401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_ABXGear_PreUsing_Statics::Function_MetaDataParams), Z_Construct_UFunction_ABXGear_PreUsing_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_ABXGear_PreUsing_Statics::BXGear_eventPreUsing_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_ABXGear_PreUsing()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ABXGear_PreUsing_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(ABXGear::execPreUsing)
{
	P_GET_STRUCT_REF(FBXUsingGearInformation,Z_Param_Out_UsingInfo);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->PreUsing(Z_Param_Out_UsingInfo);
	P_NATIVE_END;
}
// End Class ABXGear Function PreUsing

// Begin Class ABXGear Function ScriptPostEquip
struct BXGear_eventScriptPostEquip_Parms
{
	FBXEquipGearInformation EquipInfo;
};
static FName NAME_ABXGear_ScriptPostEquip = FName(TEXT("ScriptPostEquip"));
void ABXGear::ScriptPostEquip(FBXEquipGearInformation& EquipInfo)
{
	BXGear_eventScriptPostEquip_Parms Parms;
	Parms.EquipInfo=EquipInfo;
	ProcessEvent(FindFunctionChecked(NAME_ABXGear_ScriptPostEquip),&Parms);
	EquipInfo=Parms.EquipInfo;
}
struct Z_Construct_UFunction_ABXGear_ScriptPostEquip_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Equip" },
		{ "ModuleRelativePath", "Gear/BXGear.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_EquipInfo;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_ABXGear_ScriptPostEquip_Statics::NewProp_EquipInfo = { "EquipInfo", nullptr, (EPropertyFlags)0x0010000008000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXGear_eventScriptPostEquip_Parms, EquipInfo), Z_Construct_UScriptStruct_FBXEquipGearInformation, METADATA_PARAMS(0, nullptr) }; // 974305362
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_ABXGear_ScriptPostEquip_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ABXGear_ScriptPostEquip_Statics::NewProp_EquipInfo,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_ABXGear_ScriptPostEquip_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ABXGear_ScriptPostEquip_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_ABXGear, nullptr, "ScriptPostEquip", nullptr, nullptr, Z_Construct_UFunction_ABXGear_ScriptPostEquip_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_ABXGear_ScriptPostEquip_Statics::PropPointers), sizeof(BXGear_eventScriptPostEquip_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08480800, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_ABXGear_ScriptPostEquip_Statics::Function_MetaDataParams), Z_Construct_UFunction_ABXGear_ScriptPostEquip_Statics::Function_MetaDataParams) };
static_assert(sizeof(BXGear_eventScriptPostEquip_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_ABXGear_ScriptPostEquip()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ABXGear_ScriptPostEquip_Statics::FuncParams);
	}
	return ReturnFunction;
}
// End Class ABXGear Function ScriptPostEquip

// Begin Class ABXGear Function ScriptPostSheath
struct BXGear_eventScriptPostSheath_Parms
{
	FBXSheathGearInformation SheathInfo;
};
static FName NAME_ABXGear_ScriptPostSheath = FName(TEXT("ScriptPostSheath"));
void ABXGear::ScriptPostSheath(FBXSheathGearInformation& SheathInfo)
{
	BXGear_eventScriptPostSheath_Parms Parms;
	Parms.SheathInfo=SheathInfo;
	ProcessEvent(FindFunctionChecked(NAME_ABXGear_ScriptPostSheath),&Parms);
	SheathInfo=Parms.SheathInfo;
}
struct Z_Construct_UFunction_ABXGear_ScriptPostSheath_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Sheath" },
		{ "ModuleRelativePath", "Gear/BXGear.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_SheathInfo;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_ABXGear_ScriptPostSheath_Statics::NewProp_SheathInfo = { "SheathInfo", nullptr, (EPropertyFlags)0x0010000008000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXGear_eventScriptPostSheath_Parms, SheathInfo), Z_Construct_UScriptStruct_FBXSheathGearInformation, METADATA_PARAMS(0, nullptr) }; // 1661981629
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_ABXGear_ScriptPostSheath_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ABXGear_ScriptPostSheath_Statics::NewProp_SheathInfo,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_ABXGear_ScriptPostSheath_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ABXGear_ScriptPostSheath_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_ABXGear, nullptr, "ScriptPostSheath", nullptr, nullptr, Z_Construct_UFunction_ABXGear_ScriptPostSheath_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_ABXGear_ScriptPostSheath_Statics::PropPointers), sizeof(BXGear_eventScriptPostSheath_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08480800, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_ABXGear_ScriptPostSheath_Statics::Function_MetaDataParams), Z_Construct_UFunction_ABXGear_ScriptPostSheath_Statics::Function_MetaDataParams) };
static_assert(sizeof(BXGear_eventScriptPostSheath_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_ABXGear_ScriptPostSheath()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ABXGear_ScriptPostSheath_Statics::FuncParams);
	}
	return ReturnFunction;
}
// End Class ABXGear Function ScriptPostSheath

// Begin Class ABXGear Function ScriptPostUnequip
struct BXGear_eventScriptPostUnequip_Parms
{
	FBXEquipGearInformation UnequipInfo;
};
static FName NAME_ABXGear_ScriptPostUnequip = FName(TEXT("ScriptPostUnequip"));
void ABXGear::ScriptPostUnequip(FBXEquipGearInformation& UnequipInfo)
{
	BXGear_eventScriptPostUnequip_Parms Parms;
	Parms.UnequipInfo=UnequipInfo;
	ProcessEvent(FindFunctionChecked(NAME_ABXGear_ScriptPostUnequip),&Parms);
	UnequipInfo=Parms.UnequipInfo;
}
struct Z_Construct_UFunction_ABXGear_ScriptPostUnequip_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Equip" },
		{ "ModuleRelativePath", "Gear/BXGear.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_UnequipInfo;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_ABXGear_ScriptPostUnequip_Statics::NewProp_UnequipInfo = { "UnequipInfo", nullptr, (EPropertyFlags)0x0010000008000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXGear_eventScriptPostUnequip_Parms, UnequipInfo), Z_Construct_UScriptStruct_FBXEquipGearInformation, METADATA_PARAMS(0, nullptr) }; // 974305362
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_ABXGear_ScriptPostUnequip_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ABXGear_ScriptPostUnequip_Statics::NewProp_UnequipInfo,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_ABXGear_ScriptPostUnequip_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ABXGear_ScriptPostUnequip_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_ABXGear, nullptr, "ScriptPostUnequip", nullptr, nullptr, Z_Construct_UFunction_ABXGear_ScriptPostUnequip_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_ABXGear_ScriptPostUnequip_Statics::PropPointers), sizeof(BXGear_eventScriptPostUnequip_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08480800, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_ABXGear_ScriptPostUnequip_Statics::Function_MetaDataParams), Z_Construct_UFunction_ABXGear_ScriptPostUnequip_Statics::Function_MetaDataParams) };
static_assert(sizeof(BXGear_eventScriptPostUnequip_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_ABXGear_ScriptPostUnequip()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ABXGear_ScriptPostUnequip_Statics::FuncParams);
	}
	return ReturnFunction;
}
// End Class ABXGear Function ScriptPostUnequip

// Begin Class ABXGear Function ScriptPostUnsheath
struct BXGear_eventScriptPostUnsheath_Parms
{
	FBXSheathGearInformation UnsheathInfo;
};
static FName NAME_ABXGear_ScriptPostUnsheath = FName(TEXT("ScriptPostUnsheath"));
void ABXGear::ScriptPostUnsheath(FBXSheathGearInformation& UnsheathInfo)
{
	BXGear_eventScriptPostUnsheath_Parms Parms;
	Parms.UnsheathInfo=UnsheathInfo;
	ProcessEvent(FindFunctionChecked(NAME_ABXGear_ScriptPostUnsheath),&Parms);
	UnsheathInfo=Parms.UnsheathInfo;
}
struct Z_Construct_UFunction_ABXGear_ScriptPostUnsheath_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Sheath" },
		{ "ModuleRelativePath", "Gear/BXGear.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_UnsheathInfo;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_ABXGear_ScriptPostUnsheath_Statics::NewProp_UnsheathInfo = { "UnsheathInfo", nullptr, (EPropertyFlags)0x0010000008000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXGear_eventScriptPostUnsheath_Parms, UnsheathInfo), Z_Construct_UScriptStruct_FBXSheathGearInformation, METADATA_PARAMS(0, nullptr) }; // 1661981629
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_ABXGear_ScriptPostUnsheath_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ABXGear_ScriptPostUnsheath_Statics::NewProp_UnsheathInfo,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_ABXGear_ScriptPostUnsheath_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ABXGear_ScriptPostUnsheath_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_ABXGear, nullptr, "ScriptPostUnsheath", nullptr, nullptr, Z_Construct_UFunction_ABXGear_ScriptPostUnsheath_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_ABXGear_ScriptPostUnsheath_Statics::PropPointers), sizeof(BXGear_eventScriptPostUnsheath_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08480800, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_ABXGear_ScriptPostUnsheath_Statics::Function_MetaDataParams), Z_Construct_UFunction_ABXGear_ScriptPostUnsheath_Statics::Function_MetaDataParams) };
static_assert(sizeof(BXGear_eventScriptPostUnsheath_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_ABXGear_ScriptPostUnsheath()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ABXGear_ScriptPostUnsheath_Statics::FuncParams);
	}
	return ReturnFunction;
}
// End Class ABXGear Function ScriptPostUnsheath

// Begin Class ABXGear Function ScriptPostUnusing
struct BXGear_eventScriptPostUnusing_Parms
{
	FBXUsingGearInformation UnusingInfo;
};
static FName NAME_ABXGear_ScriptPostUnusing = FName(TEXT("ScriptPostUnusing"));
void ABXGear::ScriptPostUnusing(FBXUsingGearInformation& UnusingInfo)
{
	BXGear_eventScriptPostUnusing_Parms Parms;
	Parms.UnusingInfo=UnusingInfo;
	ProcessEvent(FindFunctionChecked(NAME_ABXGear_ScriptPostUnusing),&Parms);
	UnusingInfo=Parms.UnusingInfo;
}
struct Z_Construct_UFunction_ABXGear_ScriptPostUnusing_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Use" },
		{ "ModuleRelativePath", "Gear/BXGear.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_UnusingInfo;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_ABXGear_ScriptPostUnusing_Statics::NewProp_UnusingInfo = { "UnusingInfo", nullptr, (EPropertyFlags)0x0010000008000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXGear_eventScriptPostUnusing_Parms, UnusingInfo), Z_Construct_UScriptStruct_FBXUsingGearInformation, METADATA_PARAMS(0, nullptr) }; // 2397478069
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_ABXGear_ScriptPostUnusing_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ABXGear_ScriptPostUnusing_Statics::NewProp_UnusingInfo,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_ABXGear_ScriptPostUnusing_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ABXGear_ScriptPostUnusing_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_ABXGear, nullptr, "ScriptPostUnusing", nullptr, nullptr, Z_Construct_UFunction_ABXGear_ScriptPostUnusing_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_ABXGear_ScriptPostUnusing_Statics::PropPointers), sizeof(BXGear_eventScriptPostUnusing_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08480800, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_ABXGear_ScriptPostUnusing_Statics::Function_MetaDataParams), Z_Construct_UFunction_ABXGear_ScriptPostUnusing_Statics::Function_MetaDataParams) };
static_assert(sizeof(BXGear_eventScriptPostUnusing_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_ABXGear_ScriptPostUnusing()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ABXGear_ScriptPostUnusing_Statics::FuncParams);
	}
	return ReturnFunction;
}
// End Class ABXGear Function ScriptPostUnusing

// Begin Class ABXGear Function ScriptPostUsing
struct BXGear_eventScriptPostUsing_Parms
{
	FBXUsingGearInformation UsingInfo;
};
static FName NAME_ABXGear_ScriptPostUsing = FName(TEXT("ScriptPostUsing"));
void ABXGear::ScriptPostUsing(FBXUsingGearInformation& UsingInfo)
{
	BXGear_eventScriptPostUsing_Parms Parms;
	Parms.UsingInfo=UsingInfo;
	ProcessEvent(FindFunctionChecked(NAME_ABXGear_ScriptPostUsing),&Parms);
	UsingInfo=Parms.UsingInfo;
}
struct Z_Construct_UFunction_ABXGear_ScriptPostUsing_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Use" },
		{ "ModuleRelativePath", "Gear/BXGear.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_UsingInfo;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_ABXGear_ScriptPostUsing_Statics::NewProp_UsingInfo = { "UsingInfo", nullptr, (EPropertyFlags)0x0010000008000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXGear_eventScriptPostUsing_Parms, UsingInfo), Z_Construct_UScriptStruct_FBXUsingGearInformation, METADATA_PARAMS(0, nullptr) }; // 2397478069
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_ABXGear_ScriptPostUsing_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ABXGear_ScriptPostUsing_Statics::NewProp_UsingInfo,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_ABXGear_ScriptPostUsing_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ABXGear_ScriptPostUsing_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_ABXGear, nullptr, "ScriptPostUsing", nullptr, nullptr, Z_Construct_UFunction_ABXGear_ScriptPostUsing_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_ABXGear_ScriptPostUsing_Statics::PropPointers), sizeof(BXGear_eventScriptPostUsing_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08480800, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_ABXGear_ScriptPostUsing_Statics::Function_MetaDataParams), Z_Construct_UFunction_ABXGear_ScriptPostUsing_Statics::Function_MetaDataParams) };
static_assert(sizeof(BXGear_eventScriptPostUsing_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_ABXGear_ScriptPostUsing()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ABXGear_ScriptPostUsing_Statics::FuncParams);
	}
	return ReturnFunction;
}
// End Class ABXGear Function ScriptPostUsing

// Begin Class ABXGear Function ScriptPreEquip
struct BXGear_eventScriptPreEquip_Parms
{
	FBXEquipGearInformation EquipInfo;
};
static FName NAME_ABXGear_ScriptPreEquip = FName(TEXT("ScriptPreEquip"));
void ABXGear::ScriptPreEquip(FBXEquipGearInformation& EquipInfo)
{
	BXGear_eventScriptPreEquip_Parms Parms;
	Parms.EquipInfo=EquipInfo;
	ProcessEvent(FindFunctionChecked(NAME_ABXGear_ScriptPreEquip),&Parms);
	EquipInfo=Parms.EquipInfo;
}
struct Z_Construct_UFunction_ABXGear_ScriptPreEquip_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Equip" },
		{ "ModuleRelativePath", "Gear/BXGear.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_EquipInfo;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_ABXGear_ScriptPreEquip_Statics::NewProp_EquipInfo = { "EquipInfo", nullptr, (EPropertyFlags)0x0010000008000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXGear_eventScriptPreEquip_Parms, EquipInfo), Z_Construct_UScriptStruct_FBXEquipGearInformation, METADATA_PARAMS(0, nullptr) }; // 974305362
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_ABXGear_ScriptPreEquip_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ABXGear_ScriptPreEquip_Statics::NewProp_EquipInfo,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_ABXGear_ScriptPreEquip_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ABXGear_ScriptPreEquip_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_ABXGear, nullptr, "ScriptPreEquip", nullptr, nullptr, Z_Construct_UFunction_ABXGear_ScriptPreEquip_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_ABXGear_ScriptPreEquip_Statics::PropPointers), sizeof(BXGear_eventScriptPreEquip_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08480800, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_ABXGear_ScriptPreEquip_Statics::Function_MetaDataParams), Z_Construct_UFunction_ABXGear_ScriptPreEquip_Statics::Function_MetaDataParams) };
static_assert(sizeof(BXGear_eventScriptPreEquip_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_ABXGear_ScriptPreEquip()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ABXGear_ScriptPreEquip_Statics::FuncParams);
	}
	return ReturnFunction;
}
// End Class ABXGear Function ScriptPreEquip

// Begin Class ABXGear Function ScriptPreSheath
struct BXGear_eventScriptPreSheath_Parms
{
	FBXSheathGearInformation SheathInfo;
};
static FName NAME_ABXGear_ScriptPreSheath = FName(TEXT("ScriptPreSheath"));
void ABXGear::ScriptPreSheath(FBXSheathGearInformation& SheathInfo)
{
	BXGear_eventScriptPreSheath_Parms Parms;
	Parms.SheathInfo=SheathInfo;
	ProcessEvent(FindFunctionChecked(NAME_ABXGear_ScriptPreSheath),&Parms);
	SheathInfo=Parms.SheathInfo;
}
struct Z_Construct_UFunction_ABXGear_ScriptPreSheath_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Sheath" },
		{ "ModuleRelativePath", "Gear/BXGear.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_SheathInfo;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_ABXGear_ScriptPreSheath_Statics::NewProp_SheathInfo = { "SheathInfo", nullptr, (EPropertyFlags)0x0010000008000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXGear_eventScriptPreSheath_Parms, SheathInfo), Z_Construct_UScriptStruct_FBXSheathGearInformation, METADATA_PARAMS(0, nullptr) }; // 1661981629
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_ABXGear_ScriptPreSheath_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ABXGear_ScriptPreSheath_Statics::NewProp_SheathInfo,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_ABXGear_ScriptPreSheath_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ABXGear_ScriptPreSheath_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_ABXGear, nullptr, "ScriptPreSheath", nullptr, nullptr, Z_Construct_UFunction_ABXGear_ScriptPreSheath_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_ABXGear_ScriptPreSheath_Statics::PropPointers), sizeof(BXGear_eventScriptPreSheath_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08480800, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_ABXGear_ScriptPreSheath_Statics::Function_MetaDataParams), Z_Construct_UFunction_ABXGear_ScriptPreSheath_Statics::Function_MetaDataParams) };
static_assert(sizeof(BXGear_eventScriptPreSheath_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_ABXGear_ScriptPreSheath()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ABXGear_ScriptPreSheath_Statics::FuncParams);
	}
	return ReturnFunction;
}
// End Class ABXGear Function ScriptPreSheath

// Begin Class ABXGear Function ScriptPreUnequip
struct BXGear_eventScriptPreUnequip_Parms
{
	FBXEquipGearInformation UnequipInfo;
};
static FName NAME_ABXGear_ScriptPreUnequip = FName(TEXT("ScriptPreUnequip"));
void ABXGear::ScriptPreUnequip(FBXEquipGearInformation& UnequipInfo)
{
	BXGear_eventScriptPreUnequip_Parms Parms;
	Parms.UnequipInfo=UnequipInfo;
	ProcessEvent(FindFunctionChecked(NAME_ABXGear_ScriptPreUnequip),&Parms);
	UnequipInfo=Parms.UnequipInfo;
}
struct Z_Construct_UFunction_ABXGear_ScriptPreUnequip_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Equip" },
		{ "ModuleRelativePath", "Gear/BXGear.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_UnequipInfo;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_ABXGear_ScriptPreUnequip_Statics::NewProp_UnequipInfo = { "UnequipInfo", nullptr, (EPropertyFlags)0x0010000008000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXGear_eventScriptPreUnequip_Parms, UnequipInfo), Z_Construct_UScriptStruct_FBXEquipGearInformation, METADATA_PARAMS(0, nullptr) }; // 974305362
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_ABXGear_ScriptPreUnequip_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ABXGear_ScriptPreUnequip_Statics::NewProp_UnequipInfo,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_ABXGear_ScriptPreUnequip_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ABXGear_ScriptPreUnequip_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_ABXGear, nullptr, "ScriptPreUnequip", nullptr, nullptr, Z_Construct_UFunction_ABXGear_ScriptPreUnequip_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_ABXGear_ScriptPreUnequip_Statics::PropPointers), sizeof(BXGear_eventScriptPreUnequip_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08480800, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_ABXGear_ScriptPreUnequip_Statics::Function_MetaDataParams), Z_Construct_UFunction_ABXGear_ScriptPreUnequip_Statics::Function_MetaDataParams) };
static_assert(sizeof(BXGear_eventScriptPreUnequip_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_ABXGear_ScriptPreUnequip()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ABXGear_ScriptPreUnequip_Statics::FuncParams);
	}
	return ReturnFunction;
}
// End Class ABXGear Function ScriptPreUnequip

// Begin Class ABXGear Function ScriptPreUnsheath
struct BXGear_eventScriptPreUnsheath_Parms
{
	FBXSheathGearInformation UnsheathInfo;
};
static FName NAME_ABXGear_ScriptPreUnsheath = FName(TEXT("ScriptPreUnsheath"));
void ABXGear::ScriptPreUnsheath(FBXSheathGearInformation& UnsheathInfo)
{
	BXGear_eventScriptPreUnsheath_Parms Parms;
	Parms.UnsheathInfo=UnsheathInfo;
	ProcessEvent(FindFunctionChecked(NAME_ABXGear_ScriptPreUnsheath),&Parms);
	UnsheathInfo=Parms.UnsheathInfo;
}
struct Z_Construct_UFunction_ABXGear_ScriptPreUnsheath_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Sheath" },
		{ "ModuleRelativePath", "Gear/BXGear.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_UnsheathInfo;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_ABXGear_ScriptPreUnsheath_Statics::NewProp_UnsheathInfo = { "UnsheathInfo", nullptr, (EPropertyFlags)0x0010000008000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXGear_eventScriptPreUnsheath_Parms, UnsheathInfo), Z_Construct_UScriptStruct_FBXSheathGearInformation, METADATA_PARAMS(0, nullptr) }; // 1661981629
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_ABXGear_ScriptPreUnsheath_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ABXGear_ScriptPreUnsheath_Statics::NewProp_UnsheathInfo,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_ABXGear_ScriptPreUnsheath_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ABXGear_ScriptPreUnsheath_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_ABXGear, nullptr, "ScriptPreUnsheath", nullptr, nullptr, Z_Construct_UFunction_ABXGear_ScriptPreUnsheath_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_ABXGear_ScriptPreUnsheath_Statics::PropPointers), sizeof(BXGear_eventScriptPreUnsheath_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08480800, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_ABXGear_ScriptPreUnsheath_Statics::Function_MetaDataParams), Z_Construct_UFunction_ABXGear_ScriptPreUnsheath_Statics::Function_MetaDataParams) };
static_assert(sizeof(BXGear_eventScriptPreUnsheath_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_ABXGear_ScriptPreUnsheath()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ABXGear_ScriptPreUnsheath_Statics::FuncParams);
	}
	return ReturnFunction;
}
// End Class ABXGear Function ScriptPreUnsheath

// Begin Class ABXGear Function ScriptPreUnusing
struct BXGear_eventScriptPreUnusing_Parms
{
	FBXUsingGearInformation UnusingInfo;
};
static FName NAME_ABXGear_ScriptPreUnusing = FName(TEXT("ScriptPreUnusing"));
void ABXGear::ScriptPreUnusing(FBXUsingGearInformation& UnusingInfo)
{
	BXGear_eventScriptPreUnusing_Parms Parms;
	Parms.UnusingInfo=UnusingInfo;
	ProcessEvent(FindFunctionChecked(NAME_ABXGear_ScriptPreUnusing),&Parms);
	UnusingInfo=Parms.UnusingInfo;
}
struct Z_Construct_UFunction_ABXGear_ScriptPreUnusing_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Use" },
		{ "ModuleRelativePath", "Gear/BXGear.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_UnusingInfo;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_ABXGear_ScriptPreUnusing_Statics::NewProp_UnusingInfo = { "UnusingInfo", nullptr, (EPropertyFlags)0x0010000008000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXGear_eventScriptPreUnusing_Parms, UnusingInfo), Z_Construct_UScriptStruct_FBXUsingGearInformation, METADATA_PARAMS(0, nullptr) }; // 2397478069
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_ABXGear_ScriptPreUnusing_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ABXGear_ScriptPreUnusing_Statics::NewProp_UnusingInfo,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_ABXGear_ScriptPreUnusing_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ABXGear_ScriptPreUnusing_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_ABXGear, nullptr, "ScriptPreUnusing", nullptr, nullptr, Z_Construct_UFunction_ABXGear_ScriptPreUnusing_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_ABXGear_ScriptPreUnusing_Statics::PropPointers), sizeof(BXGear_eventScriptPreUnusing_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08480800, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_ABXGear_ScriptPreUnusing_Statics::Function_MetaDataParams), Z_Construct_UFunction_ABXGear_ScriptPreUnusing_Statics::Function_MetaDataParams) };
static_assert(sizeof(BXGear_eventScriptPreUnusing_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_ABXGear_ScriptPreUnusing()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ABXGear_ScriptPreUnusing_Statics::FuncParams);
	}
	return ReturnFunction;
}
// End Class ABXGear Function ScriptPreUnusing

// Begin Class ABXGear Function ScriptPreUsing
struct BXGear_eventScriptPreUsing_Parms
{
	FBXUsingGearInformation UsingInfo;
};
static FName NAME_ABXGear_ScriptPreUsing = FName(TEXT("ScriptPreUsing"));
void ABXGear::ScriptPreUsing(FBXUsingGearInformation& UsingInfo)
{
	BXGear_eventScriptPreUsing_Parms Parms;
	Parms.UsingInfo=UsingInfo;
	ProcessEvent(FindFunctionChecked(NAME_ABXGear_ScriptPreUsing),&Parms);
	UsingInfo=Parms.UsingInfo;
}
struct Z_Construct_UFunction_ABXGear_ScriptPreUsing_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Use" },
		{ "ModuleRelativePath", "Gear/BXGear.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_UsingInfo;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_ABXGear_ScriptPreUsing_Statics::NewProp_UsingInfo = { "UsingInfo", nullptr, (EPropertyFlags)0x0010000008000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXGear_eventScriptPreUsing_Parms, UsingInfo), Z_Construct_UScriptStruct_FBXUsingGearInformation, METADATA_PARAMS(0, nullptr) }; // 2397478069
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_ABXGear_ScriptPreUsing_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ABXGear_ScriptPreUsing_Statics::NewProp_UsingInfo,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_ABXGear_ScriptPreUsing_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ABXGear_ScriptPreUsing_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_ABXGear, nullptr, "ScriptPreUsing", nullptr, nullptr, Z_Construct_UFunction_ABXGear_ScriptPreUsing_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_ABXGear_ScriptPreUsing_Statics::PropPointers), sizeof(BXGear_eventScriptPreUsing_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08480800, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_ABXGear_ScriptPreUsing_Statics::Function_MetaDataParams), Z_Construct_UFunction_ABXGear_ScriptPreUsing_Statics::Function_MetaDataParams) };
static_assert(sizeof(BXGear_eventScriptPreUsing_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_ABXGear_ScriptPreUsing()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ABXGear_ScriptPreUsing_Statics::FuncParams);
	}
	return ReturnFunction;
}
// End Class ABXGear Function ScriptPreUsing

// Begin Class ABXGear
void ABXGear::StaticRegisterNativesABXGear()
{
	UClass* Class = ABXGear::StaticClass();
	static const FNameNativePtrPair Funcs[] = {
		{ "PostEquip", &ABXGear::execPostEquip },
		{ "PostSheath", &ABXGear::execPostSheath },
		{ "PostUnequip", &ABXGear::execPostUnequip },
		{ "PostUnsheath", &ABXGear::execPostUnsheath },
		{ "PostUnusing", &ABXGear::execPostUnusing },
		{ "PostUsing", &ABXGear::execPostUsing },
		{ "PreEquip", &ABXGear::execPreEquip },
		{ "PreSheath", &ABXGear::execPreSheath },
		{ "PreUnequip", &ABXGear::execPreUnequip },
		{ "PreUnsheath", &ABXGear::execPreUnsheath },
		{ "PreUnusing", &ABXGear::execPreUnusing },
		{ "PreUsing", &ABXGear::execPreUsing },
	};
	FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(ABXGear);
UClass* Z_Construct_UClass_ABXGear_NoRegister()
{
	return ABXGear::StaticClass();
}
struct Z_Construct_UClass_ABXGear_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "IncludePath", "Gear/BXGear.h" },
		{ "IsBlueprintBase", "true" },
		{ "ModuleRelativePath", "Gear/BXGear.h" },
		{ "ObjectInitializerConstructorDeclared", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_GearType_MetaData[] = {
		{ "Category", "BXGear" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xd7\xb0\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Gear/BXGear.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xd7\xb0\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_GearData_MetaData[] = {
		{ "Category", "BXGear" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xd7\xb0\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Gear/BXGear.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xd7\xb0\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_EquipFunctions_MetaData[] = {
		{ "Bitmask", "" },
		{ "BitmaskEnum", "EBXEquipFunction" },
		{ "Category", "Equip" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xd2\xaa\xd6\xb4\xef\xbf\xbd\xd0\xb5\xc4\xba\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd(\xc4\xac\xef\xbf\xbd\xef\xbf\xbd\xd6\xbb\xd6\xb4\xef\xbf\xbd\xef\xbf\xbd""C++\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd)\n" },
#endif
		{ "ModuleRelativePath", "Gear/BXGear.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xd2\xaa\xd6\xb4\xef\xbf\xbd\xd0\xb5\xc4\xba\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd(\xc4\xac\xef\xbf\xbd\xef\xbf\xbd\xd6\xbb\xd6\xb4\xef\xbf\xbd\xef\xbf\xbd""C++\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd)" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_UseFunctions_MetaData[] = {
		{ "Bitmask", "" },
		{ "BitmaskEnum", "EBXUseFunction" },
		{ "Category", "Use" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xd2\xaa\xd6\xb4\xef\xbf\xbd\xd0\xb5\xc4\xba\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd(\xc4\xac\xef\xbf\xbd\xef\xbf\xbd\xd6\xbb\xd6\xb4\xef\xbf\xbd\xef\xbf\xbd""C++\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd)\n" },
#endif
		{ "ModuleRelativePath", "Gear/BXGear.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xd2\xaa\xd6\xb4\xef\xbf\xbd\xd0\xb5\xc4\xba\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd(\xc4\xac\xef\xbf\xbd\xef\xbf\xbd\xd6\xbb\xd6\xb4\xef\xbf\xbd\xef\xbf\xbd""C++\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd)" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_SheathFunctions_MetaData[] = {
		{ "Bitmask", "" },
		{ "BitmaskEnum", "EBXSheathFunction" },
		{ "Category", "Sheath" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xd2\xaa\xd6\xb4\xef\xbf\xbd\xd0\xb5\xc4\xba\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd(\xc4\xac\xef\xbf\xbd\xef\xbf\xbd\xd6\xbb\xd6\xb4\xef\xbf\xbd\xef\xbf\xbd""C++\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd)\n" },
#endif
		{ "ModuleRelativePath", "Gear/BXGear.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xd2\xaa\xd6\xb4\xef\xbf\xbd\xd0\xb5\xc4\xba\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd(\xc4\xac\xef\xbf\xbd\xef\xbf\xbd\xd6\xbb\xd6\xb4\xef\xbf\xbd\xef\xbf\xbd""C++\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd)" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FBytePropertyParams NewProp_GearType_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_GearType;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_GearData;
	static const UECodeGen_Private::FIntPropertyParams NewProp_EquipFunctions;
	static const UECodeGen_Private::FIntPropertyParams NewProp_UseFunctions;
	static const UECodeGen_Private::FIntPropertyParams NewProp_SheathFunctions;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_ABXGear_PostEquip, "PostEquip" }, // 1969518404
		{ &Z_Construct_UFunction_ABXGear_PostSheath, "PostSheath" }, // 334961187
		{ &Z_Construct_UFunction_ABXGear_PostUnequip, "PostUnequip" }, // 148307169
		{ &Z_Construct_UFunction_ABXGear_PostUnsheath, "PostUnsheath" }, // 1468168040
		{ &Z_Construct_UFunction_ABXGear_PostUnusing, "PostUnusing" }, // 290538469
		{ &Z_Construct_UFunction_ABXGear_PostUsing, "PostUsing" }, // 1995337030
		{ &Z_Construct_UFunction_ABXGear_PreEquip, "PreEquip" }, // 2013720233
		{ &Z_Construct_UFunction_ABXGear_PreSheath, "PreSheath" }, // 3694212611
		{ &Z_Construct_UFunction_ABXGear_PreUnequip, "PreUnequip" }, // 2093932844
		{ &Z_Construct_UFunction_ABXGear_PreUnsheath, "PreUnsheath" }, // 930773923
		{ &Z_Construct_UFunction_ABXGear_PreUnusing, "PreUnusing" }, // 3646994807
		{ &Z_Construct_UFunction_ABXGear_PreUsing, "PreUsing" }, // 1797788624
		{ &Z_Construct_UFunction_ABXGear_ScriptPostEquip, "ScriptPostEquip" }, // 3579738823
		{ &Z_Construct_UFunction_ABXGear_ScriptPostSheath, "ScriptPostSheath" }, // 115625012
		{ &Z_Construct_UFunction_ABXGear_ScriptPostUnequip, "ScriptPostUnequip" }, // 1541633312
		{ &Z_Construct_UFunction_ABXGear_ScriptPostUnsheath, "ScriptPostUnsheath" }, // 675337779
		{ &Z_Construct_UFunction_ABXGear_ScriptPostUnusing, "ScriptPostUnusing" }, // 3454373165
		{ &Z_Construct_UFunction_ABXGear_ScriptPostUsing, "ScriptPostUsing" }, // 3396851994
		{ &Z_Construct_UFunction_ABXGear_ScriptPreEquip, "ScriptPreEquip" }, // 645259141
		{ &Z_Construct_UFunction_ABXGear_ScriptPreSheath, "ScriptPreSheath" }, // 1136941815
		{ &Z_Construct_UFunction_ABXGear_ScriptPreUnequip, "ScriptPreUnequip" }, // 2660764804
		{ &Z_Construct_UFunction_ABXGear_ScriptPreUnsheath, "ScriptPreUnsheath" }, // 3277971726
		{ &Z_Construct_UFunction_ABXGear_ScriptPreUnusing, "ScriptPreUnusing" }, // 4285309717
		{ &Z_Construct_UFunction_ABXGear_ScriptPreUsing, "ScriptPreUsing" }, // 2393591839
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ABXGear>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FBytePropertyParams Z_Construct_UClass_ABXGear_Statics::NewProp_GearType_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UClass_ABXGear_Statics::NewProp_GearType = { "GearType", nullptr, (EPropertyFlags)0x0010000000000015, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ABXGear, GearType), Z_Construct_UEnum_BattleX_EBXGearType, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_GearType_MetaData), NewProp_GearType_MetaData) }; // 3771108800
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ABXGear_Statics::NewProp_GearData = { "GearData", nullptr, (EPropertyFlags)0x0010000000002014, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ABXGear, GearData), Z_Construct_UClass_UBXGearData_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_GearData_MetaData), NewProp_GearData_MetaData) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UClass_ABXGear_Statics::NewProp_EquipFunctions = { "EquipFunctions", nullptr, (EPropertyFlags)0x0020080000010001, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ABXGear, EquipFunctions), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_EquipFunctions_MetaData), NewProp_EquipFunctions_MetaData) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UClass_ABXGear_Statics::NewProp_UseFunctions = { "UseFunctions", nullptr, (EPropertyFlags)0x0020080000010001, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ABXGear, UseFunctions), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_UseFunctions_MetaData), NewProp_UseFunctions_MetaData) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UClass_ABXGear_Statics::NewProp_SheathFunctions = { "SheathFunctions", nullptr, (EPropertyFlags)0x0020080000010001, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ABXGear, SheathFunctions), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_SheathFunctions_MetaData), NewProp_SheathFunctions_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_ABXGear_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ABXGear_Statics::NewProp_GearType_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ABXGear_Statics::NewProp_GearType,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ABXGear_Statics::NewProp_GearData,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ABXGear_Statics::NewProp_EquipFunctions,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ABXGear_Statics::NewProp_UseFunctions,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ABXGear_Statics::NewProp_SheathFunctions,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_ABXGear_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_ABXGear_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_AActor,
	(UObject* (*)())Z_Construct_UPackage__Script_BattleX,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_ABXGear_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_ABXGear_Statics::ClassParams = {
	&ABXGear::StaticClass,
	"Engine",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	Z_Construct_UClass_ABXGear_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	UE_ARRAY_COUNT(Z_Construct_UClass_ABXGear_Statics::PropPointers),
	0,
	0x009000A5u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ABXGear_Statics::Class_MetaDataParams), Z_Construct_UClass_ABXGear_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_ABXGear()
{
	if (!Z_Registration_Info_UClass_ABXGear.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_ABXGear.OuterSingleton, Z_Construct_UClass_ABXGear_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_ABXGear.OuterSingleton;
}
template<> BATTLEX_API UClass* StaticClass<ABXGear>()
{
	return ABXGear::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(ABXGear);
// End Class ABXGear

// Begin Registration
struct Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Gear_BXGear_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_ABXGear, ABXGear::StaticClass, TEXT("ABXGear"), &Z_Registration_Info_UClass_ABXGear, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(ABXGear), 3840152758U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Gear_BXGear_h_1316886048(TEXT("/Script/BattleX"),
	Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Gear_BXGear_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Gear_BXGear_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
