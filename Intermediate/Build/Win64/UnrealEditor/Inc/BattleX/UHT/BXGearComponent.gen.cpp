// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "BattleX/Gear/BXGearComponent.h"
#include "BattleX/Gear/BXGearStructs.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeBXGearComponent() {}

// Begin Cross Module References
BATTLEX_API UClass* Z_Construct_UClass_ABXGear_NoRegister();
BATTLEX_API UClass* Z_Construct_UClass_UBXGearComponent();
BATTLEX_API UClass* Z_Construct_UClass_UBXGearComponent_NoRegister();
BATTLEX_API UClass* Z_Construct_UClass_UBXGearData_NoRegister();
BATTLEX_API UEnum* Z_Construct_UEnum_BattleX_EBXGearSlot();
BATTLEX_API UFunction* Z_Construct_UDelegateFunction_BattleX_PostEquipGearDelegate__DelegateSignature();
BATTLEX_API UFunction* Z_Construct_UDelegateFunction_BattleX_PostSheathGearDelegate__DelegateSignature();
BATTLEX_API UFunction* Z_Construct_UDelegateFunction_BattleX_PostUnequipGearDelegate__DelegateSignature();
BATTLEX_API UFunction* Z_Construct_UDelegateFunction_BattleX_PostUnsheathGearDelegate__DelegateSignature();
BATTLEX_API UFunction* Z_Construct_UDelegateFunction_BattleX_PostUnusingGearDelegate__DelegateSignature();
BATTLEX_API UFunction* Z_Construct_UDelegateFunction_BattleX_PostUsingGearDelegate__DelegateSignature();
BATTLEX_API UFunction* Z_Construct_UDelegateFunction_BattleX_PreEquipGearDelegate__DelegateSignature();
BATTLEX_API UFunction* Z_Construct_UDelegateFunction_BattleX_PreSheathGearDelegate__DelegateSignature();
BATTLEX_API UFunction* Z_Construct_UDelegateFunction_BattleX_PreUnequipGearDelegate__DelegateSignature();
BATTLEX_API UFunction* Z_Construct_UDelegateFunction_BattleX_PreUnsheathGearDelegate__DelegateSignature();
BATTLEX_API UFunction* Z_Construct_UDelegateFunction_BattleX_PreUnusingGearDelegate__DelegateSignature();
BATTLEX_API UFunction* Z_Construct_UDelegateFunction_BattleX_PreUsingGearDelegate__DelegateSignature();
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXEquipGearInformation();
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXGears();
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXSheathGearInformation();
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXUsingGearInformation();
COREUOBJECT_API UClass* Z_Construct_UClass_UClass();
COREUOBJECT_API UClass* Z_Construct_UClass_UObject_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_UActorComponent();
UPackage* Z_Construct_UPackage__Script_BattleX();
// End Cross Module References

// Begin Delegate FPreEquipGearDelegate
struct Z_Construct_UDelegateFunction_BattleX_PreEquipGearDelegate__DelegateSignature_Statics
{
	struct _Script_BattleX_eventPreEquipGearDelegate_Parms
	{
		ABXGear* Gear;
		FBXEquipGearInformation Information;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Gear/BXGearComponent.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Gear;
	static const UECodeGen_Private::FStructPropertyParams NewProp_Information;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UDelegateFunction_BattleX_PreEquipGearDelegate__DelegateSignature_Statics::NewProp_Gear = { "Gear", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(_Script_BattleX_eventPreEquipGearDelegate_Parms, Gear), Z_Construct_UClass_ABXGear_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UDelegateFunction_BattleX_PreEquipGearDelegate__DelegateSignature_Statics::NewProp_Information = { "Information", nullptr, (EPropertyFlags)0x0010000008000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(_Script_BattleX_eventPreEquipGearDelegate_Parms, Information), Z_Construct_UScriptStruct_FBXEquipGearInformation, METADATA_PARAMS(0, nullptr) }; // 974305362
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UDelegateFunction_BattleX_PreEquipGearDelegate__DelegateSignature_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_BattleX_PreEquipGearDelegate__DelegateSignature_Statics::NewProp_Gear,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_BattleX_PreEquipGearDelegate__DelegateSignature_Statics::NewProp_Information,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_BattleX_PreEquipGearDelegate__DelegateSignature_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UDelegateFunction_BattleX_PreEquipGearDelegate__DelegateSignature_Statics::FuncParams = { (UObject*(*)())Z_Construct_UPackage__Script_BattleX, nullptr, "PreEquipGearDelegate__DelegateSignature", nullptr, nullptr, Z_Construct_UDelegateFunction_BattleX_PreEquipGearDelegate__DelegateSignature_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_BattleX_PreEquipGearDelegate__DelegateSignature_Statics::PropPointers), sizeof(Z_Construct_UDelegateFunction_BattleX_PreEquipGearDelegate__DelegateSignature_Statics::_Script_BattleX_eventPreEquipGearDelegate_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00130000, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_BattleX_PreEquipGearDelegate__DelegateSignature_Statics::Function_MetaDataParams), Z_Construct_UDelegateFunction_BattleX_PreEquipGearDelegate__DelegateSignature_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UDelegateFunction_BattleX_PreEquipGearDelegate__DelegateSignature_Statics::_Script_BattleX_eventPreEquipGearDelegate_Parms) < MAX_uint16);
UFunction* Z_Construct_UDelegateFunction_BattleX_PreEquipGearDelegate__DelegateSignature()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UDelegateFunction_BattleX_PreEquipGearDelegate__DelegateSignature_Statics::FuncParams);
	}
	return ReturnFunction;
}
void FPreEquipGearDelegate_DelegateWrapper(const FMulticastScriptDelegate& PreEquipGearDelegate, ABXGear* Gear, FBXEquipGearInformation& Information)
{
	struct _Script_BattleX_eventPreEquipGearDelegate_Parms
	{
		ABXGear* Gear;
		FBXEquipGearInformation Information;
	};
	_Script_BattleX_eventPreEquipGearDelegate_Parms Parms;
	Parms.Gear=Gear;
	Parms.Information=Information;
	PreEquipGearDelegate.ProcessMulticastDelegate<UObject>(&Parms);
	Information=Parms.Information;
}
// End Delegate FPreEquipGearDelegate

// Begin Delegate FPostEquipGearDelegate
struct Z_Construct_UDelegateFunction_BattleX_PostEquipGearDelegate__DelegateSignature_Statics
{
	struct _Script_BattleX_eventPostEquipGearDelegate_Parms
	{
		ABXGear* Gear;
		FBXEquipGearInformation Information;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Gear/BXGearComponent.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Gear;
	static const UECodeGen_Private::FStructPropertyParams NewProp_Information;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UDelegateFunction_BattleX_PostEquipGearDelegate__DelegateSignature_Statics::NewProp_Gear = { "Gear", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(_Script_BattleX_eventPostEquipGearDelegate_Parms, Gear), Z_Construct_UClass_ABXGear_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UDelegateFunction_BattleX_PostEquipGearDelegate__DelegateSignature_Statics::NewProp_Information = { "Information", nullptr, (EPropertyFlags)0x0010000008000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(_Script_BattleX_eventPostEquipGearDelegate_Parms, Information), Z_Construct_UScriptStruct_FBXEquipGearInformation, METADATA_PARAMS(0, nullptr) }; // 974305362
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UDelegateFunction_BattleX_PostEquipGearDelegate__DelegateSignature_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_BattleX_PostEquipGearDelegate__DelegateSignature_Statics::NewProp_Gear,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_BattleX_PostEquipGearDelegate__DelegateSignature_Statics::NewProp_Information,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_BattleX_PostEquipGearDelegate__DelegateSignature_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UDelegateFunction_BattleX_PostEquipGearDelegate__DelegateSignature_Statics::FuncParams = { (UObject*(*)())Z_Construct_UPackage__Script_BattleX, nullptr, "PostEquipGearDelegate__DelegateSignature", nullptr, nullptr, Z_Construct_UDelegateFunction_BattleX_PostEquipGearDelegate__DelegateSignature_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_BattleX_PostEquipGearDelegate__DelegateSignature_Statics::PropPointers), sizeof(Z_Construct_UDelegateFunction_BattleX_PostEquipGearDelegate__DelegateSignature_Statics::_Script_BattleX_eventPostEquipGearDelegate_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00130000, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_BattleX_PostEquipGearDelegate__DelegateSignature_Statics::Function_MetaDataParams), Z_Construct_UDelegateFunction_BattleX_PostEquipGearDelegate__DelegateSignature_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UDelegateFunction_BattleX_PostEquipGearDelegate__DelegateSignature_Statics::_Script_BattleX_eventPostEquipGearDelegate_Parms) < MAX_uint16);
UFunction* Z_Construct_UDelegateFunction_BattleX_PostEquipGearDelegate__DelegateSignature()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UDelegateFunction_BattleX_PostEquipGearDelegate__DelegateSignature_Statics::FuncParams);
	}
	return ReturnFunction;
}
void FPostEquipGearDelegate_DelegateWrapper(const FMulticastScriptDelegate& PostEquipGearDelegate, ABXGear* Gear, FBXEquipGearInformation& Information)
{
	struct _Script_BattleX_eventPostEquipGearDelegate_Parms
	{
		ABXGear* Gear;
		FBXEquipGearInformation Information;
	};
	_Script_BattleX_eventPostEquipGearDelegate_Parms Parms;
	Parms.Gear=Gear;
	Parms.Information=Information;
	PostEquipGearDelegate.ProcessMulticastDelegate<UObject>(&Parms);
	Information=Parms.Information;
}
// End Delegate FPostEquipGearDelegate

// Begin Delegate FPreUnequipGearDelegate
struct Z_Construct_UDelegateFunction_BattleX_PreUnequipGearDelegate__DelegateSignature_Statics
{
	struct _Script_BattleX_eventPreUnequipGearDelegate_Parms
	{
		ABXGear* Gear;
		FBXEquipGearInformation Information;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Gear/BXGearComponent.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Gear;
	static const UECodeGen_Private::FStructPropertyParams NewProp_Information;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UDelegateFunction_BattleX_PreUnequipGearDelegate__DelegateSignature_Statics::NewProp_Gear = { "Gear", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(_Script_BattleX_eventPreUnequipGearDelegate_Parms, Gear), Z_Construct_UClass_ABXGear_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UDelegateFunction_BattleX_PreUnequipGearDelegate__DelegateSignature_Statics::NewProp_Information = { "Information", nullptr, (EPropertyFlags)0x0010000008000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(_Script_BattleX_eventPreUnequipGearDelegate_Parms, Information), Z_Construct_UScriptStruct_FBXEquipGearInformation, METADATA_PARAMS(0, nullptr) }; // 974305362
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UDelegateFunction_BattleX_PreUnequipGearDelegate__DelegateSignature_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_BattleX_PreUnequipGearDelegate__DelegateSignature_Statics::NewProp_Gear,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_BattleX_PreUnequipGearDelegate__DelegateSignature_Statics::NewProp_Information,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_BattleX_PreUnequipGearDelegate__DelegateSignature_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UDelegateFunction_BattleX_PreUnequipGearDelegate__DelegateSignature_Statics::FuncParams = { (UObject*(*)())Z_Construct_UPackage__Script_BattleX, nullptr, "PreUnequipGearDelegate__DelegateSignature", nullptr, nullptr, Z_Construct_UDelegateFunction_BattleX_PreUnequipGearDelegate__DelegateSignature_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_BattleX_PreUnequipGearDelegate__DelegateSignature_Statics::PropPointers), sizeof(Z_Construct_UDelegateFunction_BattleX_PreUnequipGearDelegate__DelegateSignature_Statics::_Script_BattleX_eventPreUnequipGearDelegate_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00130000, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_BattleX_PreUnequipGearDelegate__DelegateSignature_Statics::Function_MetaDataParams), Z_Construct_UDelegateFunction_BattleX_PreUnequipGearDelegate__DelegateSignature_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UDelegateFunction_BattleX_PreUnequipGearDelegate__DelegateSignature_Statics::_Script_BattleX_eventPreUnequipGearDelegate_Parms) < MAX_uint16);
UFunction* Z_Construct_UDelegateFunction_BattleX_PreUnequipGearDelegate__DelegateSignature()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UDelegateFunction_BattleX_PreUnequipGearDelegate__DelegateSignature_Statics::FuncParams);
	}
	return ReturnFunction;
}
void FPreUnequipGearDelegate_DelegateWrapper(const FMulticastScriptDelegate& PreUnequipGearDelegate, ABXGear* Gear, FBXEquipGearInformation& Information)
{
	struct _Script_BattleX_eventPreUnequipGearDelegate_Parms
	{
		ABXGear* Gear;
		FBXEquipGearInformation Information;
	};
	_Script_BattleX_eventPreUnequipGearDelegate_Parms Parms;
	Parms.Gear=Gear;
	Parms.Information=Information;
	PreUnequipGearDelegate.ProcessMulticastDelegate<UObject>(&Parms);
	Information=Parms.Information;
}
// End Delegate FPreUnequipGearDelegate

// Begin Delegate FPostUnequipGearDelegate
struct Z_Construct_UDelegateFunction_BattleX_PostUnequipGearDelegate__DelegateSignature_Statics
{
	struct _Script_BattleX_eventPostUnequipGearDelegate_Parms
	{
		ABXGear* Gear;
		FBXEquipGearInformation Information;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Gear/BXGearComponent.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Gear;
	static const UECodeGen_Private::FStructPropertyParams NewProp_Information;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UDelegateFunction_BattleX_PostUnequipGearDelegate__DelegateSignature_Statics::NewProp_Gear = { "Gear", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(_Script_BattleX_eventPostUnequipGearDelegate_Parms, Gear), Z_Construct_UClass_ABXGear_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UDelegateFunction_BattleX_PostUnequipGearDelegate__DelegateSignature_Statics::NewProp_Information = { "Information", nullptr, (EPropertyFlags)0x0010000008000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(_Script_BattleX_eventPostUnequipGearDelegate_Parms, Information), Z_Construct_UScriptStruct_FBXEquipGearInformation, METADATA_PARAMS(0, nullptr) }; // 974305362
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UDelegateFunction_BattleX_PostUnequipGearDelegate__DelegateSignature_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_BattleX_PostUnequipGearDelegate__DelegateSignature_Statics::NewProp_Gear,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_BattleX_PostUnequipGearDelegate__DelegateSignature_Statics::NewProp_Information,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_BattleX_PostUnequipGearDelegate__DelegateSignature_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UDelegateFunction_BattleX_PostUnequipGearDelegate__DelegateSignature_Statics::FuncParams = { (UObject*(*)())Z_Construct_UPackage__Script_BattleX, nullptr, "PostUnequipGearDelegate__DelegateSignature", nullptr, nullptr, Z_Construct_UDelegateFunction_BattleX_PostUnequipGearDelegate__DelegateSignature_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_BattleX_PostUnequipGearDelegate__DelegateSignature_Statics::PropPointers), sizeof(Z_Construct_UDelegateFunction_BattleX_PostUnequipGearDelegate__DelegateSignature_Statics::_Script_BattleX_eventPostUnequipGearDelegate_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00130000, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_BattleX_PostUnequipGearDelegate__DelegateSignature_Statics::Function_MetaDataParams), Z_Construct_UDelegateFunction_BattleX_PostUnequipGearDelegate__DelegateSignature_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UDelegateFunction_BattleX_PostUnequipGearDelegate__DelegateSignature_Statics::_Script_BattleX_eventPostUnequipGearDelegate_Parms) < MAX_uint16);
UFunction* Z_Construct_UDelegateFunction_BattleX_PostUnequipGearDelegate__DelegateSignature()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UDelegateFunction_BattleX_PostUnequipGearDelegate__DelegateSignature_Statics::FuncParams);
	}
	return ReturnFunction;
}
void FPostUnequipGearDelegate_DelegateWrapper(const FMulticastScriptDelegate& PostUnequipGearDelegate, ABXGear* Gear, FBXEquipGearInformation& Information)
{
	struct _Script_BattleX_eventPostUnequipGearDelegate_Parms
	{
		ABXGear* Gear;
		FBXEquipGearInformation Information;
	};
	_Script_BattleX_eventPostUnequipGearDelegate_Parms Parms;
	Parms.Gear=Gear;
	Parms.Information=Information;
	PostUnequipGearDelegate.ProcessMulticastDelegate<UObject>(&Parms);
	Information=Parms.Information;
}
// End Delegate FPostUnequipGearDelegate

// Begin Delegate FPreUsingGearDelegate
struct Z_Construct_UDelegateFunction_BattleX_PreUsingGearDelegate__DelegateSignature_Statics
{
	struct _Script_BattleX_eventPreUsingGearDelegate_Parms
	{
		ABXGear* Gear;
		FBXUsingGearInformation Information;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Gear/BXGearComponent.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Gear;
	static const UECodeGen_Private::FStructPropertyParams NewProp_Information;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UDelegateFunction_BattleX_PreUsingGearDelegate__DelegateSignature_Statics::NewProp_Gear = { "Gear", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(_Script_BattleX_eventPreUsingGearDelegate_Parms, Gear), Z_Construct_UClass_ABXGear_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UDelegateFunction_BattleX_PreUsingGearDelegate__DelegateSignature_Statics::NewProp_Information = { "Information", nullptr, (EPropertyFlags)0x0010000008000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(_Script_BattleX_eventPreUsingGearDelegate_Parms, Information), Z_Construct_UScriptStruct_FBXUsingGearInformation, METADATA_PARAMS(0, nullptr) }; // 2397478069
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UDelegateFunction_BattleX_PreUsingGearDelegate__DelegateSignature_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_BattleX_PreUsingGearDelegate__DelegateSignature_Statics::NewProp_Gear,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_BattleX_PreUsingGearDelegate__DelegateSignature_Statics::NewProp_Information,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_BattleX_PreUsingGearDelegate__DelegateSignature_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UDelegateFunction_BattleX_PreUsingGearDelegate__DelegateSignature_Statics::FuncParams = { (UObject*(*)())Z_Construct_UPackage__Script_BattleX, nullptr, "PreUsingGearDelegate__DelegateSignature", nullptr, nullptr, Z_Construct_UDelegateFunction_BattleX_PreUsingGearDelegate__DelegateSignature_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_BattleX_PreUsingGearDelegate__DelegateSignature_Statics::PropPointers), sizeof(Z_Construct_UDelegateFunction_BattleX_PreUsingGearDelegate__DelegateSignature_Statics::_Script_BattleX_eventPreUsingGearDelegate_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00130000, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_BattleX_PreUsingGearDelegate__DelegateSignature_Statics::Function_MetaDataParams), Z_Construct_UDelegateFunction_BattleX_PreUsingGearDelegate__DelegateSignature_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UDelegateFunction_BattleX_PreUsingGearDelegate__DelegateSignature_Statics::_Script_BattleX_eventPreUsingGearDelegate_Parms) < MAX_uint16);
UFunction* Z_Construct_UDelegateFunction_BattleX_PreUsingGearDelegate__DelegateSignature()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UDelegateFunction_BattleX_PreUsingGearDelegate__DelegateSignature_Statics::FuncParams);
	}
	return ReturnFunction;
}
void FPreUsingGearDelegate_DelegateWrapper(const FMulticastScriptDelegate& PreUsingGearDelegate, ABXGear* Gear, FBXUsingGearInformation& Information)
{
	struct _Script_BattleX_eventPreUsingGearDelegate_Parms
	{
		ABXGear* Gear;
		FBXUsingGearInformation Information;
	};
	_Script_BattleX_eventPreUsingGearDelegate_Parms Parms;
	Parms.Gear=Gear;
	Parms.Information=Information;
	PreUsingGearDelegate.ProcessMulticastDelegate<UObject>(&Parms);
	Information=Parms.Information;
}
// End Delegate FPreUsingGearDelegate

// Begin Delegate FPostUsingGearDelegate
struct Z_Construct_UDelegateFunction_BattleX_PostUsingGearDelegate__DelegateSignature_Statics
{
	struct _Script_BattleX_eventPostUsingGearDelegate_Parms
	{
		ABXGear* Gear;
		FBXUsingGearInformation Information;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Gear/BXGearComponent.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Gear;
	static const UECodeGen_Private::FStructPropertyParams NewProp_Information;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UDelegateFunction_BattleX_PostUsingGearDelegate__DelegateSignature_Statics::NewProp_Gear = { "Gear", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(_Script_BattleX_eventPostUsingGearDelegate_Parms, Gear), Z_Construct_UClass_ABXGear_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UDelegateFunction_BattleX_PostUsingGearDelegate__DelegateSignature_Statics::NewProp_Information = { "Information", nullptr, (EPropertyFlags)0x0010000008000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(_Script_BattleX_eventPostUsingGearDelegate_Parms, Information), Z_Construct_UScriptStruct_FBXUsingGearInformation, METADATA_PARAMS(0, nullptr) }; // 2397478069
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UDelegateFunction_BattleX_PostUsingGearDelegate__DelegateSignature_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_BattleX_PostUsingGearDelegate__DelegateSignature_Statics::NewProp_Gear,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_BattleX_PostUsingGearDelegate__DelegateSignature_Statics::NewProp_Information,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_BattleX_PostUsingGearDelegate__DelegateSignature_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UDelegateFunction_BattleX_PostUsingGearDelegate__DelegateSignature_Statics::FuncParams = { (UObject*(*)())Z_Construct_UPackage__Script_BattleX, nullptr, "PostUsingGearDelegate__DelegateSignature", nullptr, nullptr, Z_Construct_UDelegateFunction_BattleX_PostUsingGearDelegate__DelegateSignature_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_BattleX_PostUsingGearDelegate__DelegateSignature_Statics::PropPointers), sizeof(Z_Construct_UDelegateFunction_BattleX_PostUsingGearDelegate__DelegateSignature_Statics::_Script_BattleX_eventPostUsingGearDelegate_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00130000, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_BattleX_PostUsingGearDelegate__DelegateSignature_Statics::Function_MetaDataParams), Z_Construct_UDelegateFunction_BattleX_PostUsingGearDelegate__DelegateSignature_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UDelegateFunction_BattleX_PostUsingGearDelegate__DelegateSignature_Statics::_Script_BattleX_eventPostUsingGearDelegate_Parms) < MAX_uint16);
UFunction* Z_Construct_UDelegateFunction_BattleX_PostUsingGearDelegate__DelegateSignature()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UDelegateFunction_BattleX_PostUsingGearDelegate__DelegateSignature_Statics::FuncParams);
	}
	return ReturnFunction;
}
void FPostUsingGearDelegate_DelegateWrapper(const FMulticastScriptDelegate& PostUsingGearDelegate, ABXGear* Gear, FBXUsingGearInformation& Information)
{
	struct _Script_BattleX_eventPostUsingGearDelegate_Parms
	{
		ABXGear* Gear;
		FBXUsingGearInformation Information;
	};
	_Script_BattleX_eventPostUsingGearDelegate_Parms Parms;
	Parms.Gear=Gear;
	Parms.Information=Information;
	PostUsingGearDelegate.ProcessMulticastDelegate<UObject>(&Parms);
	Information=Parms.Information;
}
// End Delegate FPostUsingGearDelegate

// Begin Delegate FPreUnusingGearDelegate
struct Z_Construct_UDelegateFunction_BattleX_PreUnusingGearDelegate__DelegateSignature_Statics
{
	struct _Script_BattleX_eventPreUnusingGearDelegate_Parms
	{
		ABXGear* Gear;
		FBXUsingGearInformation Information;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Gear/BXGearComponent.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Gear;
	static const UECodeGen_Private::FStructPropertyParams NewProp_Information;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UDelegateFunction_BattleX_PreUnusingGearDelegate__DelegateSignature_Statics::NewProp_Gear = { "Gear", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(_Script_BattleX_eventPreUnusingGearDelegate_Parms, Gear), Z_Construct_UClass_ABXGear_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UDelegateFunction_BattleX_PreUnusingGearDelegate__DelegateSignature_Statics::NewProp_Information = { "Information", nullptr, (EPropertyFlags)0x0010000008000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(_Script_BattleX_eventPreUnusingGearDelegate_Parms, Information), Z_Construct_UScriptStruct_FBXUsingGearInformation, METADATA_PARAMS(0, nullptr) }; // 2397478069
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UDelegateFunction_BattleX_PreUnusingGearDelegate__DelegateSignature_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_BattleX_PreUnusingGearDelegate__DelegateSignature_Statics::NewProp_Gear,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_BattleX_PreUnusingGearDelegate__DelegateSignature_Statics::NewProp_Information,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_BattleX_PreUnusingGearDelegate__DelegateSignature_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UDelegateFunction_BattleX_PreUnusingGearDelegate__DelegateSignature_Statics::FuncParams = { (UObject*(*)())Z_Construct_UPackage__Script_BattleX, nullptr, "PreUnusingGearDelegate__DelegateSignature", nullptr, nullptr, Z_Construct_UDelegateFunction_BattleX_PreUnusingGearDelegate__DelegateSignature_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_BattleX_PreUnusingGearDelegate__DelegateSignature_Statics::PropPointers), sizeof(Z_Construct_UDelegateFunction_BattleX_PreUnusingGearDelegate__DelegateSignature_Statics::_Script_BattleX_eventPreUnusingGearDelegate_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00130000, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_BattleX_PreUnusingGearDelegate__DelegateSignature_Statics::Function_MetaDataParams), Z_Construct_UDelegateFunction_BattleX_PreUnusingGearDelegate__DelegateSignature_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UDelegateFunction_BattleX_PreUnusingGearDelegate__DelegateSignature_Statics::_Script_BattleX_eventPreUnusingGearDelegate_Parms) < MAX_uint16);
UFunction* Z_Construct_UDelegateFunction_BattleX_PreUnusingGearDelegate__DelegateSignature()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UDelegateFunction_BattleX_PreUnusingGearDelegate__DelegateSignature_Statics::FuncParams);
	}
	return ReturnFunction;
}
void FPreUnusingGearDelegate_DelegateWrapper(const FMulticastScriptDelegate& PreUnusingGearDelegate, ABXGear* Gear, FBXUsingGearInformation& Information)
{
	struct _Script_BattleX_eventPreUnusingGearDelegate_Parms
	{
		ABXGear* Gear;
		FBXUsingGearInformation Information;
	};
	_Script_BattleX_eventPreUnusingGearDelegate_Parms Parms;
	Parms.Gear=Gear;
	Parms.Information=Information;
	PreUnusingGearDelegate.ProcessMulticastDelegate<UObject>(&Parms);
	Information=Parms.Information;
}
// End Delegate FPreUnusingGearDelegate

// Begin Delegate FPostUnusingGearDelegate
struct Z_Construct_UDelegateFunction_BattleX_PostUnusingGearDelegate__DelegateSignature_Statics
{
	struct _Script_BattleX_eventPostUnusingGearDelegate_Parms
	{
		ABXGear* Gear;
		FBXUsingGearInformation Information;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Gear/BXGearComponent.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Gear;
	static const UECodeGen_Private::FStructPropertyParams NewProp_Information;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UDelegateFunction_BattleX_PostUnusingGearDelegate__DelegateSignature_Statics::NewProp_Gear = { "Gear", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(_Script_BattleX_eventPostUnusingGearDelegate_Parms, Gear), Z_Construct_UClass_ABXGear_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UDelegateFunction_BattleX_PostUnusingGearDelegate__DelegateSignature_Statics::NewProp_Information = { "Information", nullptr, (EPropertyFlags)0x0010000008000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(_Script_BattleX_eventPostUnusingGearDelegate_Parms, Information), Z_Construct_UScriptStruct_FBXUsingGearInformation, METADATA_PARAMS(0, nullptr) }; // 2397478069
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UDelegateFunction_BattleX_PostUnusingGearDelegate__DelegateSignature_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_BattleX_PostUnusingGearDelegate__DelegateSignature_Statics::NewProp_Gear,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_BattleX_PostUnusingGearDelegate__DelegateSignature_Statics::NewProp_Information,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_BattleX_PostUnusingGearDelegate__DelegateSignature_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UDelegateFunction_BattleX_PostUnusingGearDelegate__DelegateSignature_Statics::FuncParams = { (UObject*(*)())Z_Construct_UPackage__Script_BattleX, nullptr, "PostUnusingGearDelegate__DelegateSignature", nullptr, nullptr, Z_Construct_UDelegateFunction_BattleX_PostUnusingGearDelegate__DelegateSignature_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_BattleX_PostUnusingGearDelegate__DelegateSignature_Statics::PropPointers), sizeof(Z_Construct_UDelegateFunction_BattleX_PostUnusingGearDelegate__DelegateSignature_Statics::_Script_BattleX_eventPostUnusingGearDelegate_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00130000, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_BattleX_PostUnusingGearDelegate__DelegateSignature_Statics::Function_MetaDataParams), Z_Construct_UDelegateFunction_BattleX_PostUnusingGearDelegate__DelegateSignature_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UDelegateFunction_BattleX_PostUnusingGearDelegate__DelegateSignature_Statics::_Script_BattleX_eventPostUnusingGearDelegate_Parms) < MAX_uint16);
UFunction* Z_Construct_UDelegateFunction_BattleX_PostUnusingGearDelegate__DelegateSignature()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UDelegateFunction_BattleX_PostUnusingGearDelegate__DelegateSignature_Statics::FuncParams);
	}
	return ReturnFunction;
}
void FPostUnusingGearDelegate_DelegateWrapper(const FMulticastScriptDelegate& PostUnusingGearDelegate, ABXGear* Gear, FBXUsingGearInformation& Information)
{
	struct _Script_BattleX_eventPostUnusingGearDelegate_Parms
	{
		ABXGear* Gear;
		FBXUsingGearInformation Information;
	};
	_Script_BattleX_eventPostUnusingGearDelegate_Parms Parms;
	Parms.Gear=Gear;
	Parms.Information=Information;
	PostUnusingGearDelegate.ProcessMulticastDelegate<UObject>(&Parms);
	Information=Parms.Information;
}
// End Delegate FPostUnusingGearDelegate

// Begin Delegate FPreSheathGearDelegate
struct Z_Construct_UDelegateFunction_BattleX_PreSheathGearDelegate__DelegateSignature_Statics
{
	struct _Script_BattleX_eventPreSheathGearDelegate_Parms
	{
		ABXGear* Gear;
		FBXSheathGearInformation Information;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Gear/BXGearComponent.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Gear;
	static const UECodeGen_Private::FStructPropertyParams NewProp_Information;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UDelegateFunction_BattleX_PreSheathGearDelegate__DelegateSignature_Statics::NewProp_Gear = { "Gear", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(_Script_BattleX_eventPreSheathGearDelegate_Parms, Gear), Z_Construct_UClass_ABXGear_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UDelegateFunction_BattleX_PreSheathGearDelegate__DelegateSignature_Statics::NewProp_Information = { "Information", nullptr, (EPropertyFlags)0x0010000008000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(_Script_BattleX_eventPreSheathGearDelegate_Parms, Information), Z_Construct_UScriptStruct_FBXSheathGearInformation, METADATA_PARAMS(0, nullptr) }; // 1661981629
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UDelegateFunction_BattleX_PreSheathGearDelegate__DelegateSignature_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_BattleX_PreSheathGearDelegate__DelegateSignature_Statics::NewProp_Gear,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_BattleX_PreSheathGearDelegate__DelegateSignature_Statics::NewProp_Information,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_BattleX_PreSheathGearDelegate__DelegateSignature_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UDelegateFunction_BattleX_PreSheathGearDelegate__DelegateSignature_Statics::FuncParams = { (UObject*(*)())Z_Construct_UPackage__Script_BattleX, nullptr, "PreSheathGearDelegate__DelegateSignature", nullptr, nullptr, Z_Construct_UDelegateFunction_BattleX_PreSheathGearDelegate__DelegateSignature_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_BattleX_PreSheathGearDelegate__DelegateSignature_Statics::PropPointers), sizeof(Z_Construct_UDelegateFunction_BattleX_PreSheathGearDelegate__DelegateSignature_Statics::_Script_BattleX_eventPreSheathGearDelegate_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00130000, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_BattleX_PreSheathGearDelegate__DelegateSignature_Statics::Function_MetaDataParams), Z_Construct_UDelegateFunction_BattleX_PreSheathGearDelegate__DelegateSignature_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UDelegateFunction_BattleX_PreSheathGearDelegate__DelegateSignature_Statics::_Script_BattleX_eventPreSheathGearDelegate_Parms) < MAX_uint16);
UFunction* Z_Construct_UDelegateFunction_BattleX_PreSheathGearDelegate__DelegateSignature()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UDelegateFunction_BattleX_PreSheathGearDelegate__DelegateSignature_Statics::FuncParams);
	}
	return ReturnFunction;
}
void FPreSheathGearDelegate_DelegateWrapper(const FMulticastScriptDelegate& PreSheathGearDelegate, ABXGear* Gear, FBXSheathGearInformation& Information)
{
	struct _Script_BattleX_eventPreSheathGearDelegate_Parms
	{
		ABXGear* Gear;
		FBXSheathGearInformation Information;
	};
	_Script_BattleX_eventPreSheathGearDelegate_Parms Parms;
	Parms.Gear=Gear;
	Parms.Information=Information;
	PreSheathGearDelegate.ProcessMulticastDelegate<UObject>(&Parms);
	Information=Parms.Information;
}
// End Delegate FPreSheathGearDelegate

// Begin Delegate FPostSheathGearDelegate
struct Z_Construct_UDelegateFunction_BattleX_PostSheathGearDelegate__DelegateSignature_Statics
{
	struct _Script_BattleX_eventPostSheathGearDelegate_Parms
	{
		ABXGear* Gear;
		FBXSheathGearInformation Information;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Gear/BXGearComponent.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Gear;
	static const UECodeGen_Private::FStructPropertyParams NewProp_Information;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UDelegateFunction_BattleX_PostSheathGearDelegate__DelegateSignature_Statics::NewProp_Gear = { "Gear", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(_Script_BattleX_eventPostSheathGearDelegate_Parms, Gear), Z_Construct_UClass_ABXGear_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UDelegateFunction_BattleX_PostSheathGearDelegate__DelegateSignature_Statics::NewProp_Information = { "Information", nullptr, (EPropertyFlags)0x0010000008000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(_Script_BattleX_eventPostSheathGearDelegate_Parms, Information), Z_Construct_UScriptStruct_FBXSheathGearInformation, METADATA_PARAMS(0, nullptr) }; // 1661981629
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UDelegateFunction_BattleX_PostSheathGearDelegate__DelegateSignature_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_BattleX_PostSheathGearDelegate__DelegateSignature_Statics::NewProp_Gear,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_BattleX_PostSheathGearDelegate__DelegateSignature_Statics::NewProp_Information,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_BattleX_PostSheathGearDelegate__DelegateSignature_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UDelegateFunction_BattleX_PostSheathGearDelegate__DelegateSignature_Statics::FuncParams = { (UObject*(*)())Z_Construct_UPackage__Script_BattleX, nullptr, "PostSheathGearDelegate__DelegateSignature", nullptr, nullptr, Z_Construct_UDelegateFunction_BattleX_PostSheathGearDelegate__DelegateSignature_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_BattleX_PostSheathGearDelegate__DelegateSignature_Statics::PropPointers), sizeof(Z_Construct_UDelegateFunction_BattleX_PostSheathGearDelegate__DelegateSignature_Statics::_Script_BattleX_eventPostSheathGearDelegate_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00130000, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_BattleX_PostSheathGearDelegate__DelegateSignature_Statics::Function_MetaDataParams), Z_Construct_UDelegateFunction_BattleX_PostSheathGearDelegate__DelegateSignature_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UDelegateFunction_BattleX_PostSheathGearDelegate__DelegateSignature_Statics::_Script_BattleX_eventPostSheathGearDelegate_Parms) < MAX_uint16);
UFunction* Z_Construct_UDelegateFunction_BattleX_PostSheathGearDelegate__DelegateSignature()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UDelegateFunction_BattleX_PostSheathGearDelegate__DelegateSignature_Statics::FuncParams);
	}
	return ReturnFunction;
}
void FPostSheathGearDelegate_DelegateWrapper(const FMulticastScriptDelegate& PostSheathGearDelegate, ABXGear* Gear, FBXSheathGearInformation& Information)
{
	struct _Script_BattleX_eventPostSheathGearDelegate_Parms
	{
		ABXGear* Gear;
		FBXSheathGearInformation Information;
	};
	_Script_BattleX_eventPostSheathGearDelegate_Parms Parms;
	Parms.Gear=Gear;
	Parms.Information=Information;
	PostSheathGearDelegate.ProcessMulticastDelegate<UObject>(&Parms);
	Information=Parms.Information;
}
// End Delegate FPostSheathGearDelegate

// Begin Delegate FPreUnsheathGearDelegate
struct Z_Construct_UDelegateFunction_BattleX_PreUnsheathGearDelegate__DelegateSignature_Statics
{
	struct _Script_BattleX_eventPreUnsheathGearDelegate_Parms
	{
		ABXGear* Gear;
		FBXSheathGearInformation Information;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Gear/BXGearComponent.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Gear;
	static const UECodeGen_Private::FStructPropertyParams NewProp_Information;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UDelegateFunction_BattleX_PreUnsheathGearDelegate__DelegateSignature_Statics::NewProp_Gear = { "Gear", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(_Script_BattleX_eventPreUnsheathGearDelegate_Parms, Gear), Z_Construct_UClass_ABXGear_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UDelegateFunction_BattleX_PreUnsheathGearDelegate__DelegateSignature_Statics::NewProp_Information = { "Information", nullptr, (EPropertyFlags)0x0010000008000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(_Script_BattleX_eventPreUnsheathGearDelegate_Parms, Information), Z_Construct_UScriptStruct_FBXSheathGearInformation, METADATA_PARAMS(0, nullptr) }; // 1661981629
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UDelegateFunction_BattleX_PreUnsheathGearDelegate__DelegateSignature_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_BattleX_PreUnsheathGearDelegate__DelegateSignature_Statics::NewProp_Gear,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_BattleX_PreUnsheathGearDelegate__DelegateSignature_Statics::NewProp_Information,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_BattleX_PreUnsheathGearDelegate__DelegateSignature_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UDelegateFunction_BattleX_PreUnsheathGearDelegate__DelegateSignature_Statics::FuncParams = { (UObject*(*)())Z_Construct_UPackage__Script_BattleX, nullptr, "PreUnsheathGearDelegate__DelegateSignature", nullptr, nullptr, Z_Construct_UDelegateFunction_BattleX_PreUnsheathGearDelegate__DelegateSignature_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_BattleX_PreUnsheathGearDelegate__DelegateSignature_Statics::PropPointers), sizeof(Z_Construct_UDelegateFunction_BattleX_PreUnsheathGearDelegate__DelegateSignature_Statics::_Script_BattleX_eventPreUnsheathGearDelegate_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00130000, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_BattleX_PreUnsheathGearDelegate__DelegateSignature_Statics::Function_MetaDataParams), Z_Construct_UDelegateFunction_BattleX_PreUnsheathGearDelegate__DelegateSignature_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UDelegateFunction_BattleX_PreUnsheathGearDelegate__DelegateSignature_Statics::_Script_BattleX_eventPreUnsheathGearDelegate_Parms) < MAX_uint16);
UFunction* Z_Construct_UDelegateFunction_BattleX_PreUnsheathGearDelegate__DelegateSignature()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UDelegateFunction_BattleX_PreUnsheathGearDelegate__DelegateSignature_Statics::FuncParams);
	}
	return ReturnFunction;
}
void FPreUnsheathGearDelegate_DelegateWrapper(const FMulticastScriptDelegate& PreUnsheathGearDelegate, ABXGear* Gear, FBXSheathGearInformation& Information)
{
	struct _Script_BattleX_eventPreUnsheathGearDelegate_Parms
	{
		ABXGear* Gear;
		FBXSheathGearInformation Information;
	};
	_Script_BattleX_eventPreUnsheathGearDelegate_Parms Parms;
	Parms.Gear=Gear;
	Parms.Information=Information;
	PreUnsheathGearDelegate.ProcessMulticastDelegate<UObject>(&Parms);
	Information=Parms.Information;
}
// End Delegate FPreUnsheathGearDelegate

// Begin Delegate FPostUnsheathGearDelegate
struct Z_Construct_UDelegateFunction_BattleX_PostUnsheathGearDelegate__DelegateSignature_Statics
{
	struct _Script_BattleX_eventPostUnsheathGearDelegate_Parms
	{
		ABXGear* Gear;
		FBXSheathGearInformation Information;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Gear/BXGearComponent.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Gear;
	static const UECodeGen_Private::FStructPropertyParams NewProp_Information;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UDelegateFunction_BattleX_PostUnsheathGearDelegate__DelegateSignature_Statics::NewProp_Gear = { "Gear", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(_Script_BattleX_eventPostUnsheathGearDelegate_Parms, Gear), Z_Construct_UClass_ABXGear_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UDelegateFunction_BattleX_PostUnsheathGearDelegate__DelegateSignature_Statics::NewProp_Information = { "Information", nullptr, (EPropertyFlags)0x0010000008000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(_Script_BattleX_eventPostUnsheathGearDelegate_Parms, Information), Z_Construct_UScriptStruct_FBXSheathGearInformation, METADATA_PARAMS(0, nullptr) }; // 1661981629
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UDelegateFunction_BattleX_PostUnsheathGearDelegate__DelegateSignature_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_BattleX_PostUnsheathGearDelegate__DelegateSignature_Statics::NewProp_Gear,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_BattleX_PostUnsheathGearDelegate__DelegateSignature_Statics::NewProp_Information,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_BattleX_PostUnsheathGearDelegate__DelegateSignature_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UDelegateFunction_BattleX_PostUnsheathGearDelegate__DelegateSignature_Statics::FuncParams = { (UObject*(*)())Z_Construct_UPackage__Script_BattleX, nullptr, "PostUnsheathGearDelegate__DelegateSignature", nullptr, nullptr, Z_Construct_UDelegateFunction_BattleX_PostUnsheathGearDelegate__DelegateSignature_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_BattleX_PostUnsheathGearDelegate__DelegateSignature_Statics::PropPointers), sizeof(Z_Construct_UDelegateFunction_BattleX_PostUnsheathGearDelegate__DelegateSignature_Statics::_Script_BattleX_eventPostUnsheathGearDelegate_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00130000, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_BattleX_PostUnsheathGearDelegate__DelegateSignature_Statics::Function_MetaDataParams), Z_Construct_UDelegateFunction_BattleX_PostUnsheathGearDelegate__DelegateSignature_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UDelegateFunction_BattleX_PostUnsheathGearDelegate__DelegateSignature_Statics::_Script_BattleX_eventPostUnsheathGearDelegate_Parms) < MAX_uint16);
UFunction* Z_Construct_UDelegateFunction_BattleX_PostUnsheathGearDelegate__DelegateSignature()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UDelegateFunction_BattleX_PostUnsheathGearDelegate__DelegateSignature_Statics::FuncParams);
	}
	return ReturnFunction;
}
void FPostUnsheathGearDelegate_DelegateWrapper(const FMulticastScriptDelegate& PostUnsheathGearDelegate, ABXGear* Gear, FBXSheathGearInformation& Information)
{
	struct _Script_BattleX_eventPostUnsheathGearDelegate_Parms
	{
		ABXGear* Gear;
		FBXSheathGearInformation Information;
	};
	_Script_BattleX_eventPostUnsheathGearDelegate_Parms Parms;
	Parms.Gear=Gear;
	Parms.Information=Information;
	PostUnsheathGearDelegate.ProcessMulticastDelegate<UObject>(&Parms);
	Information=Parms.Information;
}
// End Delegate FPostUnsheathGearDelegate

// Begin Class UBXGearComponent Function ChangeEquipGear
struct Z_Construct_UFunction_UBXGearComponent_ChangeEquipGear_Statics
{
	struct BXGearComponent_eventChangeEquipGear_Parms
	{
		EBXGearSlot InSlot;
		int32 InIndex;
		ABXGear* InGear;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xc4\xb1\xef\xbf\xbd\xd7\xb0\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Gear/BXGearComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xc4\xb1\xef\xbf\xbd\xd7\xb0\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FBytePropertyParams NewProp_InSlot_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_InSlot;
	static const UECodeGen_Private::FIntPropertyParams NewProp_InIndex;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_InGear;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UBXGearComponent_ChangeEquipGear_Statics::NewProp_InSlot_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UBXGearComponent_ChangeEquipGear_Statics::NewProp_InSlot = { "InSlot", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXGearComponent_eventChangeEquipGear_Parms, InSlot), Z_Construct_UEnum_BattleX_EBXGearSlot, METADATA_PARAMS(0, nullptr) }; // 1739704360
const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UBXGearComponent_ChangeEquipGear_Statics::NewProp_InIndex = { "InIndex", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXGearComponent_eventChangeEquipGear_Parms, InIndex), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UBXGearComponent_ChangeEquipGear_Statics::NewProp_InGear = { "InGear", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXGearComponent_eventChangeEquipGear_Parms, InGear), Z_Construct_UClass_ABXGear_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UBXGearComponent_ChangeEquipGear_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXGearComponent_ChangeEquipGear_Statics::NewProp_InSlot_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXGearComponent_ChangeEquipGear_Statics::NewProp_InSlot,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXGearComponent_ChangeEquipGear_Statics::NewProp_InIndex,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXGearComponent_ChangeEquipGear_Statics::NewProp_InGear,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXGearComponent_ChangeEquipGear_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UBXGearComponent_ChangeEquipGear_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UBXGearComponent, nullptr, "ChangeEquipGear", nullptr, nullptr, Z_Construct_UFunction_UBXGearComponent_ChangeEquipGear_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UBXGearComponent_ChangeEquipGear_Statics::PropPointers), sizeof(Z_Construct_UFunction_UBXGearComponent_ChangeEquipGear_Statics::BXGearComponent_eventChangeEquipGear_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020400, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXGearComponent_ChangeEquipGear_Statics::Function_MetaDataParams), Z_Construct_UFunction_UBXGearComponent_ChangeEquipGear_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UBXGearComponent_ChangeEquipGear_Statics::BXGearComponent_eventChangeEquipGear_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UBXGearComponent_ChangeEquipGear()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UBXGearComponent_ChangeEquipGear_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UBXGearComponent::execChangeEquipGear)
{
	P_GET_ENUM(EBXGearSlot,Z_Param_InSlot);
	P_GET_PROPERTY(FIntProperty,Z_Param_InIndex);
	P_GET_OBJECT(ABXGear,Z_Param_InGear);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->ChangeEquipGear(EBXGearSlot(Z_Param_InSlot),Z_Param_InIndex,Z_Param_InGear);
	P_NATIVE_END;
}
// End Class UBXGearComponent Function ChangeEquipGear

// Begin Class UBXGearComponent Function ChangeEquipGearByClass
struct Z_Construct_UFunction_UBXGearComponent_ChangeEquipGearByClass_Statics
{
	struct BXGearComponent_eventChangeEquipGearByClass_Parms
	{
		EBXGearSlot InSlot;
		int32 InIndex;
		UClass* InGearClass;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xc4\xb1\xef\xbf\xbd\xd7\xb0\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Gear/BXGearComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xc4\xb1\xef\xbf\xbd\xd7\xb0\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FBytePropertyParams NewProp_InSlot_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_InSlot;
	static const UECodeGen_Private::FIntPropertyParams NewProp_InIndex;
	static const UECodeGen_Private::FClassPropertyParams NewProp_InGearClass;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UBXGearComponent_ChangeEquipGearByClass_Statics::NewProp_InSlot_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UBXGearComponent_ChangeEquipGearByClass_Statics::NewProp_InSlot = { "InSlot", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXGearComponent_eventChangeEquipGearByClass_Parms, InSlot), Z_Construct_UEnum_BattleX_EBXGearSlot, METADATA_PARAMS(0, nullptr) }; // 1739704360
const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UBXGearComponent_ChangeEquipGearByClass_Statics::NewProp_InIndex = { "InIndex", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXGearComponent_eventChangeEquipGearByClass_Parms, InIndex), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FClassPropertyParams Z_Construct_UFunction_UBXGearComponent_ChangeEquipGearByClass_Statics::NewProp_InGearClass = { "InGearClass", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Class, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXGearComponent_eventChangeEquipGearByClass_Parms, InGearClass), Z_Construct_UClass_UClass, Z_Construct_UClass_UObject_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UBXGearComponent_ChangeEquipGearByClass_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXGearComponent_ChangeEquipGearByClass_Statics::NewProp_InSlot_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXGearComponent_ChangeEquipGearByClass_Statics::NewProp_InSlot,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXGearComponent_ChangeEquipGearByClass_Statics::NewProp_InIndex,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXGearComponent_ChangeEquipGearByClass_Statics::NewProp_InGearClass,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXGearComponent_ChangeEquipGearByClass_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UBXGearComponent_ChangeEquipGearByClass_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UBXGearComponent, nullptr, "ChangeEquipGearByClass", nullptr, nullptr, Z_Construct_UFunction_UBXGearComponent_ChangeEquipGearByClass_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UBXGearComponent_ChangeEquipGearByClass_Statics::PropPointers), sizeof(Z_Construct_UFunction_UBXGearComponent_ChangeEquipGearByClass_Statics::BXGearComponent_eventChangeEquipGearByClass_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020400, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXGearComponent_ChangeEquipGearByClass_Statics::Function_MetaDataParams), Z_Construct_UFunction_UBXGearComponent_ChangeEquipGearByClass_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UBXGearComponent_ChangeEquipGearByClass_Statics::BXGearComponent_eventChangeEquipGearByClass_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UBXGearComponent_ChangeEquipGearByClass()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UBXGearComponent_ChangeEquipGearByClass_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UBXGearComponent::execChangeEquipGearByClass)
{
	P_GET_ENUM(EBXGearSlot,Z_Param_InSlot);
	P_GET_PROPERTY(FIntProperty,Z_Param_InIndex);
	P_GET_OBJECT(UClass,Z_Param_InGearClass);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->ChangeEquipGearByClass(EBXGearSlot(Z_Param_InSlot),Z_Param_InIndex,Z_Param_InGearClass);
	P_NATIVE_END;
}
// End Class UBXGearComponent Function ChangeEquipGearByClass

// Begin Class UBXGearComponent Function ChangeEquipGearByData
struct Z_Construct_UFunction_UBXGearComponent_ChangeEquipGearByData_Statics
{
	struct BXGearComponent_eventChangeEquipGearByData_Parms
	{
		EBXGearSlot InSlot;
		int32 InIndex;
		UBXGearData* InGearData;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xc4\xb1\xef\xbf\xbd\xd7\xb0\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Gear/BXGearComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xc4\xb1\xef\xbf\xbd\xd7\xb0\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FBytePropertyParams NewProp_InSlot_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_InSlot;
	static const UECodeGen_Private::FIntPropertyParams NewProp_InIndex;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_InGearData;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UBXGearComponent_ChangeEquipGearByData_Statics::NewProp_InSlot_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UBXGearComponent_ChangeEquipGearByData_Statics::NewProp_InSlot = { "InSlot", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXGearComponent_eventChangeEquipGearByData_Parms, InSlot), Z_Construct_UEnum_BattleX_EBXGearSlot, METADATA_PARAMS(0, nullptr) }; // 1739704360
const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UBXGearComponent_ChangeEquipGearByData_Statics::NewProp_InIndex = { "InIndex", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXGearComponent_eventChangeEquipGearByData_Parms, InIndex), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UBXGearComponent_ChangeEquipGearByData_Statics::NewProp_InGearData = { "InGearData", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXGearComponent_eventChangeEquipGearByData_Parms, InGearData), Z_Construct_UClass_UBXGearData_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UBXGearComponent_ChangeEquipGearByData_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXGearComponent_ChangeEquipGearByData_Statics::NewProp_InSlot_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXGearComponent_ChangeEquipGearByData_Statics::NewProp_InSlot,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXGearComponent_ChangeEquipGearByData_Statics::NewProp_InIndex,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXGearComponent_ChangeEquipGearByData_Statics::NewProp_InGearData,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXGearComponent_ChangeEquipGearByData_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UBXGearComponent_ChangeEquipGearByData_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UBXGearComponent, nullptr, "ChangeEquipGearByData", nullptr, nullptr, Z_Construct_UFunction_UBXGearComponent_ChangeEquipGearByData_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UBXGearComponent_ChangeEquipGearByData_Statics::PropPointers), sizeof(Z_Construct_UFunction_UBXGearComponent_ChangeEquipGearByData_Statics::BXGearComponent_eventChangeEquipGearByData_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020400, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXGearComponent_ChangeEquipGearByData_Statics::Function_MetaDataParams), Z_Construct_UFunction_UBXGearComponent_ChangeEquipGearByData_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UBXGearComponent_ChangeEquipGearByData_Statics::BXGearComponent_eventChangeEquipGearByData_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UBXGearComponent_ChangeEquipGearByData()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UBXGearComponent_ChangeEquipGearByData_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UBXGearComponent::execChangeEquipGearByData)
{
	P_GET_ENUM(EBXGearSlot,Z_Param_InSlot);
	P_GET_PROPERTY(FIntProperty,Z_Param_InIndex);
	P_GET_OBJECT(UBXGearData,Z_Param_InGearData);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->ChangeEquipGearByData(EBXGearSlot(Z_Param_InSlot),Z_Param_InIndex,Z_Param_InGearData);
	P_NATIVE_END;
}
// End Class UBXGearComponent Function ChangeEquipGearByData

// Begin Class UBXGearComponent Function GetEquipGears
struct Z_Construct_UFunction_UBXGearComponent_GetEquipGears_Statics
{
	struct BXGearComponent_eventGetEquipGears_Parms
	{
		EBXGearSlot InSlot;
		TArray<ABXGear*> OutGears;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xef\xbf\xbd\xc8\xa1\xd7\xb0\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xdb\xb5\xef\xbf\xbd\xd7\xb0\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xd0\xb1\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Gear/BXGearComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xef\xbf\xbd\xc8\xa1\xd7\xb0\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xdb\xb5\xef\xbf\xbd\xd7\xb0\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xd0\xb1\xef\xbf\xbd" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FBytePropertyParams NewProp_InSlot_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_InSlot;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_OutGears_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_OutGears;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UBXGearComponent_GetEquipGears_Statics::NewProp_InSlot_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UBXGearComponent_GetEquipGears_Statics::NewProp_InSlot = { "InSlot", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXGearComponent_eventGetEquipGears_Parms, InSlot), Z_Construct_UEnum_BattleX_EBXGearSlot, METADATA_PARAMS(0, nullptr) }; // 1739704360
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UBXGearComponent_GetEquipGears_Statics::NewProp_OutGears_Inner = { "OutGears", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UClass_ABXGear_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UFunction_UBXGearComponent_GetEquipGears_Statics::NewProp_OutGears = { "OutGears", nullptr, (EPropertyFlags)0x0010000000000180, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXGearComponent_eventGetEquipGears_Parms, OutGears), EArrayPropertyFlags::None, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UBXGearComponent_GetEquipGears_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXGearComponent_GetEquipGears_Statics::NewProp_InSlot_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXGearComponent_GetEquipGears_Statics::NewProp_InSlot,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXGearComponent_GetEquipGears_Statics::NewProp_OutGears_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXGearComponent_GetEquipGears_Statics::NewProp_OutGears,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXGearComponent_GetEquipGears_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UBXGearComponent_GetEquipGears_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UBXGearComponent, nullptr, "GetEquipGears", nullptr, nullptr, Z_Construct_UFunction_UBXGearComponent_GetEquipGears_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UBXGearComponent_GetEquipGears_Statics::PropPointers), sizeof(Z_Construct_UFunction_UBXGearComponent_GetEquipGears_Statics::BXGearComponent_eventGetEquipGears_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04420400, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXGearComponent_GetEquipGears_Statics::Function_MetaDataParams), Z_Construct_UFunction_UBXGearComponent_GetEquipGears_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UBXGearComponent_GetEquipGears_Statics::BXGearComponent_eventGetEquipGears_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UBXGearComponent_GetEquipGears()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UBXGearComponent_GetEquipGears_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UBXGearComponent::execGetEquipGears)
{
	P_GET_ENUM(EBXGearSlot,Z_Param_InSlot);
	P_GET_TARRAY_REF(ABXGear*,Z_Param_Out_OutGears);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->GetEquipGears(EBXGearSlot(Z_Param_InSlot),Z_Param_Out_OutGears);
	P_NATIVE_END;
}
// End Class UBXGearComponent Function GetEquipGears

// Begin Class UBXGearComponent Function GetUsingGear
struct Z_Construct_UFunction_UBXGearComponent_GetUsingGear_Statics
{
	struct BXGearComponent_eventGetUsingGear_Parms
	{
		EBXGearSlot InSlot;
		ABXGear* ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xef\xbf\xbd\xc8\xa1\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xca\xb9\xef\xbf\xbd\xc3\xb5\xef\xbf\xbd\xd7\xb0\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Gear/BXGearComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xef\xbf\xbd\xc8\xa1\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xca\xb9\xef\xbf\xbd\xc3\xb5\xef\xbf\xbd\xd7\xb0\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FBytePropertyParams NewProp_InSlot_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_InSlot;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UBXGearComponent_GetUsingGear_Statics::NewProp_InSlot_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UBXGearComponent_GetUsingGear_Statics::NewProp_InSlot = { "InSlot", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXGearComponent_eventGetUsingGear_Parms, InSlot), Z_Construct_UEnum_BattleX_EBXGearSlot, METADATA_PARAMS(0, nullptr) }; // 1739704360
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UBXGearComponent_GetUsingGear_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXGearComponent_eventGetUsingGear_Parms, ReturnValue), Z_Construct_UClass_ABXGear_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UBXGearComponent_GetUsingGear_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXGearComponent_GetUsingGear_Statics::NewProp_InSlot_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXGearComponent_GetUsingGear_Statics::NewProp_InSlot,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXGearComponent_GetUsingGear_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXGearComponent_GetUsingGear_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UBXGearComponent_GetUsingGear_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UBXGearComponent, nullptr, "GetUsingGear", nullptr, nullptr, Z_Construct_UFunction_UBXGearComponent_GetUsingGear_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UBXGearComponent_GetUsingGear_Statics::PropPointers), sizeof(Z_Construct_UFunction_UBXGearComponent_GetUsingGear_Statics::BXGearComponent_eventGetUsingGear_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020400, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXGearComponent_GetUsingGear_Statics::Function_MetaDataParams), Z_Construct_UFunction_UBXGearComponent_GetUsingGear_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UBXGearComponent_GetUsingGear_Statics::BXGearComponent_eventGetUsingGear_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UBXGearComponent_GetUsingGear()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UBXGearComponent_GetUsingGear_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UBXGearComponent::execGetUsingGear)
{
	P_GET_ENUM(EBXGearSlot,Z_Param_InSlot);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(ABXGear**)Z_Param__Result=P_THIS->GetUsingGear(EBXGearSlot(Z_Param_InSlot));
	P_NATIVE_END;
}
// End Class UBXGearComponent Function GetUsingGear

// Begin Class UBXGearComponent Function SheathGear
struct Z_Construct_UFunction_UBXGearComponent_SheathGear_Statics
{
	struct BXGearComponent_eventSheathGear_Parms
	{
		EBXGearSlot InSlot;
		bool bSheath;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xd7\xb0\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Gear/BXGearComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xd7\xb0\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FBytePropertyParams NewProp_InSlot_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_InSlot;
	static void NewProp_bSheath_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bSheath;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UBXGearComponent_SheathGear_Statics::NewProp_InSlot_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UBXGearComponent_SheathGear_Statics::NewProp_InSlot = { "InSlot", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXGearComponent_eventSheathGear_Parms, InSlot), Z_Construct_UEnum_BattleX_EBXGearSlot, METADATA_PARAMS(0, nullptr) }; // 1739704360
void Z_Construct_UFunction_UBXGearComponent_SheathGear_Statics::NewProp_bSheath_SetBit(void* Obj)
{
	((BXGearComponent_eventSheathGear_Parms*)Obj)->bSheath = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UBXGearComponent_SheathGear_Statics::NewProp_bSheath = { "bSheath", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(BXGearComponent_eventSheathGear_Parms), &Z_Construct_UFunction_UBXGearComponent_SheathGear_Statics::NewProp_bSheath_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UBXGearComponent_SheathGear_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXGearComponent_SheathGear_Statics::NewProp_InSlot_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXGearComponent_SheathGear_Statics::NewProp_InSlot,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXGearComponent_SheathGear_Statics::NewProp_bSheath,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXGearComponent_SheathGear_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UBXGearComponent_SheathGear_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UBXGearComponent, nullptr, "SheathGear", nullptr, nullptr, Z_Construct_UFunction_UBXGearComponent_SheathGear_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UBXGearComponent_SheathGear_Statics::PropPointers), sizeof(Z_Construct_UFunction_UBXGearComponent_SheathGear_Statics::BXGearComponent_eventSheathGear_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020400, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXGearComponent_SheathGear_Statics::Function_MetaDataParams), Z_Construct_UFunction_UBXGearComponent_SheathGear_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UBXGearComponent_SheathGear_Statics::BXGearComponent_eventSheathGear_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UBXGearComponent_SheathGear()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UBXGearComponent_SheathGear_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UBXGearComponent::execSheathGear)
{
	P_GET_ENUM(EBXGearSlot,Z_Param_InSlot);
	P_GET_UBOOL(Z_Param_bSheath);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->SheathGear(EBXGearSlot(Z_Param_InSlot),Z_Param_bSheath);
	P_NATIVE_END;
}
// End Class UBXGearComponent Function SheathGear

// Begin Class UBXGearComponent Function SwitchUsingGear
struct Z_Construct_UFunction_UBXGearComponent_SwitchUsingGear_Statics
{
	struct BXGearComponent_eventSwitchUsingGear_Parms
	{
		EBXGearSlot InSlot;
		int32 InIndex;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xd0\xbb\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xca\xb9\xef\xbf\xbd\xc3\xb5\xef\xbf\xbd\xd7\xb0\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Gear/BXGearComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xd0\xbb\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xca\xb9\xef\xbf\xbd\xc3\xb5\xef\xbf\xbd\xd7\xb0\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FBytePropertyParams NewProp_InSlot_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_InSlot;
	static const UECodeGen_Private::FIntPropertyParams NewProp_InIndex;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UBXGearComponent_SwitchUsingGear_Statics::NewProp_InSlot_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UBXGearComponent_SwitchUsingGear_Statics::NewProp_InSlot = { "InSlot", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXGearComponent_eventSwitchUsingGear_Parms, InSlot), Z_Construct_UEnum_BattleX_EBXGearSlot, METADATA_PARAMS(0, nullptr) }; // 1739704360
const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UBXGearComponent_SwitchUsingGear_Statics::NewProp_InIndex = { "InIndex", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXGearComponent_eventSwitchUsingGear_Parms, InIndex), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UBXGearComponent_SwitchUsingGear_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXGearComponent_SwitchUsingGear_Statics::NewProp_InSlot_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXGearComponent_SwitchUsingGear_Statics::NewProp_InSlot,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXGearComponent_SwitchUsingGear_Statics::NewProp_InIndex,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXGearComponent_SwitchUsingGear_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UBXGearComponent_SwitchUsingGear_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UBXGearComponent, nullptr, "SwitchUsingGear", nullptr, nullptr, Z_Construct_UFunction_UBXGearComponent_SwitchUsingGear_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UBXGearComponent_SwitchUsingGear_Statics::PropPointers), sizeof(Z_Construct_UFunction_UBXGearComponent_SwitchUsingGear_Statics::BXGearComponent_eventSwitchUsingGear_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020400, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXGearComponent_SwitchUsingGear_Statics::Function_MetaDataParams), Z_Construct_UFunction_UBXGearComponent_SwitchUsingGear_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UBXGearComponent_SwitchUsingGear_Statics::BXGearComponent_eventSwitchUsingGear_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UBXGearComponent_SwitchUsingGear()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UBXGearComponent_SwitchUsingGear_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UBXGearComponent::execSwitchUsingGear)
{
	P_GET_ENUM(EBXGearSlot,Z_Param_InSlot);
	P_GET_PROPERTY(FIntProperty,Z_Param_InIndex);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->SwitchUsingGear(EBXGearSlot(Z_Param_InSlot),Z_Param_InIndex);
	P_NATIVE_END;
}
// End Class UBXGearComponent Function SwitchUsingGear

// Begin Class UBXGearComponent
void UBXGearComponent::StaticRegisterNativesUBXGearComponent()
{
	UClass* Class = UBXGearComponent::StaticClass();
	static const FNameNativePtrPair Funcs[] = {
		{ "ChangeEquipGear", &UBXGearComponent::execChangeEquipGear },
		{ "ChangeEquipGearByClass", &UBXGearComponent::execChangeEquipGearByClass },
		{ "ChangeEquipGearByData", &UBXGearComponent::execChangeEquipGearByData },
		{ "GetEquipGears", &UBXGearComponent::execGetEquipGears },
		{ "GetUsingGear", &UBXGearComponent::execGetUsingGear },
		{ "SheathGear", &UBXGearComponent::execSheathGear },
		{ "SwitchUsingGear", &UBXGearComponent::execSwitchUsingGear },
	};
	FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UBXGearComponent);
UClass* Z_Construct_UClass_UBXGearComponent_NoRegister()
{
	return UBXGearComponent::StaticClass();
}
struct Z_Construct_UClass_UBXGearComponent_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "BlueprintSpawnableComponent", "" },
		{ "ClassGroupNames", "Custom" },
		{ "IncludePath", "Gear/BXGearComponent.h" },
		{ "ModuleRelativePath", "Gear/BXGearComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_SlotMaxSize_MetaData[] = {
		{ "Category", "BXGearComponent" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xc3\xbf\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xdb\xb5\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Gear/BXGearComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xc3\xbf\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xdb\xb5\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_EquipGears_MetaData[] = {
		{ "Category", "BXGearComponent" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xd7\xb0\xef\xbf\xbd\xcf\xb5\xef\xbf\xbd\xd7\xb0\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Gear/BXGearComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xd7\xb0\xef\xbf\xbd\xcf\xb5\xef\xbf\xbd\xd7\xb0\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_UsingGearIndexs_MetaData[] = {
		{ "Category", "BXGearComponent" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xca\xb9\xef\xbf\xbd\xc3\xb5\xef\xbf\xbd\xd7\xb0\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Gear/BXGearComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xca\xb9\xef\xbf\xbd\xc3\xb5\xef\xbf\xbd\xd7\xb0\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_HelpEquipInformation_MetaData[] = {
		{ "ModuleRelativePath", "Gear/BXGearComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_HelpUsingInformation_MetaData[] = {
		{ "ModuleRelativePath", "Gear/BXGearComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_HelpSheathInformation_MetaData[] = {
		{ "ModuleRelativePath", "Gear/BXGearComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_OnPreEquipGear_MetaData[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xd7\xb0\xef\xbf\xbd\xef\xbf\xbd\xc7\xb0\n" },
#endif
		{ "ModuleRelativePath", "Gear/BXGearComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xd7\xb0\xef\xbf\xbd\xef\xbf\xbd\xc7\xb0" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_OnPostEquipGear_MetaData[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xd7\xb0\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Gear/BXGearComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xd7\xb0\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_OnPreUnequipGear_MetaData[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xd0\xb6\xef\xbf\xbd\xef\xbf\xbd\xd7\xb0\xef\xbf\xbd\xef\xbf\xbd\xc7\xb0\n" },
#endif
		{ "ModuleRelativePath", "Gear/BXGearComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xd0\xb6\xef\xbf\xbd\xef\xbf\xbd\xd7\xb0\xef\xbf\xbd\xef\xbf\xbd\xc7\xb0" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_OnPostUnequipGear_MetaData[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xd0\xb6\xef\xbf\xbd\xef\xbf\xbd\xd7\xb0\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Gear/BXGearComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xd0\xb6\xef\xbf\xbd\xef\xbf\xbd\xd7\xb0\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_OnPreUsingGear_MetaData[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xca\xb9\xef\xbf\xbd\xef\xbf\xbd\xd7\xb0\xef\xbf\xbd\xef\xbf\xbd\xc7\xb0\n" },
#endif
		{ "ModuleRelativePath", "Gear/BXGearComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xca\xb9\xef\xbf\xbd\xef\xbf\xbd\xd7\xb0\xef\xbf\xbd\xef\xbf\xbd\xc7\xb0" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_OnPostUsingGear_MetaData[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xca\xb9\xef\xbf\xbd\xef\xbf\xbd\xd7\xb0\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Gear/BXGearComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xca\xb9\xef\xbf\xbd\xef\xbf\xbd\xd7\xb0\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_OnPreUnusingGear_MetaData[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xd7\xb0\xef\xbf\xbd\xef\xbf\xbd\xc7\xb0\n" },
#endif
		{ "ModuleRelativePath", "Gear/BXGearComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xd7\xb0\xef\xbf\xbd\xef\xbf\xbd\xc7\xb0" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_OnPostUnusingGear_MetaData[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xd7\xb0\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Gear/BXGearComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xd7\xb0\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_OnPreSheathGear_MetaData[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xd7\xb0\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xc7\xb0\n" },
#endif
		{ "ModuleRelativePath", "Gear/BXGearComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xd7\xb0\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xc7\xb0" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_OnPostSheathGear_MetaData[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xd7\xb0\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Gear/BXGearComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xd7\xb0\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_OnPreUnsheathGear_MetaData[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xd7\xb0\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xc7\xb0\n" },
#endif
		{ "ModuleRelativePath", "Gear/BXGearComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xd7\xb0\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xc7\xb0" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_OnPostUnsheathGear_MetaData[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xd7\xb0\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xd1\xba\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Gear/BXGearComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xd7\xb0\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xd1\xba\xef\xbf\xbd" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FIntPropertyParams NewProp_SlotMaxSize_ValueProp;
	static const UECodeGen_Private::FBytePropertyParams NewProp_SlotMaxSize_Key_KeyProp_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_SlotMaxSize_Key_KeyProp;
	static const UECodeGen_Private::FMapPropertyParams NewProp_SlotMaxSize;
	static const UECodeGen_Private::FStructPropertyParams NewProp_EquipGears_ValueProp;
	static const UECodeGen_Private::FBytePropertyParams NewProp_EquipGears_Key_KeyProp_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_EquipGears_Key_KeyProp;
	static const UECodeGen_Private::FMapPropertyParams NewProp_EquipGears;
	static const UECodeGen_Private::FIntPropertyParams NewProp_UsingGearIndexs_ValueProp;
	static const UECodeGen_Private::FBytePropertyParams NewProp_UsingGearIndexs_Key_KeyProp_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_UsingGearIndexs_Key_KeyProp;
	static const UECodeGen_Private::FMapPropertyParams NewProp_UsingGearIndexs;
	static const UECodeGen_Private::FStructPropertyParams NewProp_HelpEquipInformation;
	static const UECodeGen_Private::FStructPropertyParams NewProp_HelpUsingInformation;
	static const UECodeGen_Private::FStructPropertyParams NewProp_HelpSheathInformation;
	static const UECodeGen_Private::FMulticastDelegatePropertyParams NewProp_OnPreEquipGear;
	static const UECodeGen_Private::FMulticastDelegatePropertyParams NewProp_OnPostEquipGear;
	static const UECodeGen_Private::FMulticastDelegatePropertyParams NewProp_OnPreUnequipGear;
	static const UECodeGen_Private::FMulticastDelegatePropertyParams NewProp_OnPostUnequipGear;
	static const UECodeGen_Private::FMulticastDelegatePropertyParams NewProp_OnPreUsingGear;
	static const UECodeGen_Private::FMulticastDelegatePropertyParams NewProp_OnPostUsingGear;
	static const UECodeGen_Private::FMulticastDelegatePropertyParams NewProp_OnPreUnusingGear;
	static const UECodeGen_Private::FMulticastDelegatePropertyParams NewProp_OnPostUnusingGear;
	static const UECodeGen_Private::FMulticastDelegatePropertyParams NewProp_OnPreSheathGear;
	static const UECodeGen_Private::FMulticastDelegatePropertyParams NewProp_OnPostSheathGear;
	static const UECodeGen_Private::FMulticastDelegatePropertyParams NewProp_OnPreUnsheathGear;
	static const UECodeGen_Private::FMulticastDelegatePropertyParams NewProp_OnPostUnsheathGear;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UBXGearComponent_ChangeEquipGear, "ChangeEquipGear" }, // 1162204956
		{ &Z_Construct_UFunction_UBXGearComponent_ChangeEquipGearByClass, "ChangeEquipGearByClass" }, // 1216620345
		{ &Z_Construct_UFunction_UBXGearComponent_ChangeEquipGearByData, "ChangeEquipGearByData" }, // 1664088284
		{ &Z_Construct_UFunction_UBXGearComponent_GetEquipGears, "GetEquipGears" }, // 3314113380
		{ &Z_Construct_UFunction_UBXGearComponent_GetUsingGear, "GetUsingGear" }, // 1800819903
		{ &Z_Construct_UFunction_UBXGearComponent_SheathGear, "SheathGear" }, // 3787050435
		{ &Z_Construct_UFunction_UBXGearComponent_SwitchUsingGear, "SwitchUsingGear" }, // 2349617041
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UBXGearComponent>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FIntPropertyParams Z_Construct_UClass_UBXGearComponent_Statics::NewProp_SlotMaxSize_ValueProp = { "SlotMaxSize", nullptr, (EPropertyFlags)0x0000000000000001, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 1, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FBytePropertyParams Z_Construct_UClass_UBXGearComponent_Statics::NewProp_SlotMaxSize_Key_KeyProp_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UClass_UBXGearComponent_Statics::NewProp_SlotMaxSize_Key_KeyProp = { "SlotMaxSize_Key", nullptr, (EPropertyFlags)0x0000000000000001, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UEnum_BattleX_EBXGearSlot, METADATA_PARAMS(0, nullptr) }; // 1739704360
const UECodeGen_Private::FMapPropertyParams Z_Construct_UClass_UBXGearComponent_Statics::NewProp_SlotMaxSize = { "SlotMaxSize", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Map, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXGearComponent, SlotMaxSize), EMapPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_SlotMaxSize_MetaData), NewProp_SlotMaxSize_MetaData) }; // 1739704360
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UBXGearComponent_Statics::NewProp_EquipGears_ValueProp = { "EquipGears", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 1, Z_Construct_UScriptStruct_FBXGears, METADATA_PARAMS(0, nullptr) }; // 2352692587
const UECodeGen_Private::FBytePropertyParams Z_Construct_UClass_UBXGearComponent_Statics::NewProp_EquipGears_Key_KeyProp_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UClass_UBXGearComponent_Statics::NewProp_EquipGears_Key_KeyProp = { "EquipGears_Key", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UEnum_BattleX_EBXGearSlot, METADATA_PARAMS(0, nullptr) }; // 1739704360
const UECodeGen_Private::FMapPropertyParams Z_Construct_UClass_UBXGearComponent_Statics::NewProp_EquipGears = { "EquipGears", nullptr, (EPropertyFlags)0x0010000000002004, UECodeGen_Private::EPropertyGenFlags::Map, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXGearComponent, EquipGears), EMapPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_EquipGears_MetaData), NewProp_EquipGears_MetaData) }; // 1739704360 2352692587
const UECodeGen_Private::FIntPropertyParams Z_Construct_UClass_UBXGearComponent_Statics::NewProp_UsingGearIndexs_ValueProp = { "UsingGearIndexs", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 1, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FBytePropertyParams Z_Construct_UClass_UBXGearComponent_Statics::NewProp_UsingGearIndexs_Key_KeyProp_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UClass_UBXGearComponent_Statics::NewProp_UsingGearIndexs_Key_KeyProp = { "UsingGearIndexs_Key", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UEnum_BattleX_EBXGearSlot, METADATA_PARAMS(0, nullptr) }; // 1739704360
const UECodeGen_Private::FMapPropertyParams Z_Construct_UClass_UBXGearComponent_Statics::NewProp_UsingGearIndexs = { "UsingGearIndexs", nullptr, (EPropertyFlags)0x0010000000002004, UECodeGen_Private::EPropertyGenFlags::Map, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXGearComponent, UsingGearIndexs), EMapPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_UsingGearIndexs_MetaData), NewProp_UsingGearIndexs_MetaData) }; // 1739704360
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UBXGearComponent_Statics::NewProp_HelpEquipInformation = { "HelpEquipInformation", nullptr, (EPropertyFlags)0x0010000000002000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXGearComponent, HelpEquipInformation), Z_Construct_UScriptStruct_FBXEquipGearInformation, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_HelpEquipInformation_MetaData), NewProp_HelpEquipInformation_MetaData) }; // 974305362
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UBXGearComponent_Statics::NewProp_HelpUsingInformation = { "HelpUsingInformation", nullptr, (EPropertyFlags)0x0010000000002000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXGearComponent, HelpUsingInformation), Z_Construct_UScriptStruct_FBXUsingGearInformation, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_HelpUsingInformation_MetaData), NewProp_HelpUsingInformation_MetaData) }; // 2397478069
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UBXGearComponent_Statics::NewProp_HelpSheathInformation = { "HelpSheathInformation", nullptr, (EPropertyFlags)0x0010000000002000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXGearComponent, HelpSheathInformation), Z_Construct_UScriptStruct_FBXSheathGearInformation, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_HelpSheathInformation_MetaData), NewProp_HelpSheathInformation_MetaData) }; // 1661981629
const UECodeGen_Private::FMulticastDelegatePropertyParams Z_Construct_UClass_UBXGearComponent_Statics::NewProp_OnPreEquipGear = { "OnPreEquipGear", nullptr, (EPropertyFlags)0x0010000010080000, UECodeGen_Private::EPropertyGenFlags::InlineMulticastDelegate, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXGearComponent, OnPreEquipGear), Z_Construct_UDelegateFunction_BattleX_PreEquipGearDelegate__DelegateSignature, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_OnPreEquipGear_MetaData), NewProp_OnPreEquipGear_MetaData) }; // 1915257292
const UECodeGen_Private::FMulticastDelegatePropertyParams Z_Construct_UClass_UBXGearComponent_Statics::NewProp_OnPostEquipGear = { "OnPostEquipGear", nullptr, (EPropertyFlags)0x0010000010080000, UECodeGen_Private::EPropertyGenFlags::InlineMulticastDelegate, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXGearComponent, OnPostEquipGear), Z_Construct_UDelegateFunction_BattleX_PostEquipGearDelegate__DelegateSignature, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_OnPostEquipGear_MetaData), NewProp_OnPostEquipGear_MetaData) }; // 1527033811
const UECodeGen_Private::FMulticastDelegatePropertyParams Z_Construct_UClass_UBXGearComponent_Statics::NewProp_OnPreUnequipGear = { "OnPreUnequipGear", nullptr, (EPropertyFlags)0x0010000010080000, UECodeGen_Private::EPropertyGenFlags::InlineMulticastDelegate, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXGearComponent, OnPreUnequipGear), Z_Construct_UDelegateFunction_BattleX_PreUnequipGearDelegate__DelegateSignature, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_OnPreUnequipGear_MetaData), NewProp_OnPreUnequipGear_MetaData) }; // 1546839477
const UECodeGen_Private::FMulticastDelegatePropertyParams Z_Construct_UClass_UBXGearComponent_Statics::NewProp_OnPostUnequipGear = { "OnPostUnequipGear", nullptr, (EPropertyFlags)0x0010000010080000, UECodeGen_Private::EPropertyGenFlags::InlineMulticastDelegate, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXGearComponent, OnPostUnequipGear), Z_Construct_UDelegateFunction_BattleX_PostUnequipGearDelegate__DelegateSignature, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_OnPostUnequipGear_MetaData), NewProp_OnPostUnequipGear_MetaData) }; // 1480261277
const UECodeGen_Private::FMulticastDelegatePropertyParams Z_Construct_UClass_UBXGearComponent_Statics::NewProp_OnPreUsingGear = { "OnPreUsingGear", nullptr, (EPropertyFlags)0x0010000010080000, UECodeGen_Private::EPropertyGenFlags::InlineMulticastDelegate, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXGearComponent, OnPreUsingGear), Z_Construct_UDelegateFunction_BattleX_PreUsingGearDelegate__DelegateSignature, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_OnPreUsingGear_MetaData), NewProp_OnPreUsingGear_MetaData) }; // 533851686
const UECodeGen_Private::FMulticastDelegatePropertyParams Z_Construct_UClass_UBXGearComponent_Statics::NewProp_OnPostUsingGear = { "OnPostUsingGear", nullptr, (EPropertyFlags)0x0010000010080000, UECodeGen_Private::EPropertyGenFlags::InlineMulticastDelegate, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXGearComponent, OnPostUsingGear), Z_Construct_UDelegateFunction_BattleX_PostUsingGearDelegate__DelegateSignature, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_OnPostUsingGear_MetaData), NewProp_OnPostUsingGear_MetaData) }; // 4131693240
const UECodeGen_Private::FMulticastDelegatePropertyParams Z_Construct_UClass_UBXGearComponent_Statics::NewProp_OnPreUnusingGear = { "OnPreUnusingGear", nullptr, (EPropertyFlags)0x0010000010080000, UECodeGen_Private::EPropertyGenFlags::InlineMulticastDelegate, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXGearComponent, OnPreUnusingGear), Z_Construct_UDelegateFunction_BattleX_PreUnusingGearDelegate__DelegateSignature, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_OnPreUnusingGear_MetaData), NewProp_OnPreUnusingGear_MetaData) }; // 1661827247
const UECodeGen_Private::FMulticastDelegatePropertyParams Z_Construct_UClass_UBXGearComponent_Statics::NewProp_OnPostUnusingGear = { "OnPostUnusingGear", nullptr, (EPropertyFlags)0x0010000010080000, UECodeGen_Private::EPropertyGenFlags::InlineMulticastDelegate, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXGearComponent, OnPostUnusingGear), Z_Construct_UDelegateFunction_BattleX_PostUnusingGearDelegate__DelegateSignature, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_OnPostUnusingGear_MetaData), NewProp_OnPostUnusingGear_MetaData) }; // 1291376033
const UECodeGen_Private::FMulticastDelegatePropertyParams Z_Construct_UClass_UBXGearComponent_Statics::NewProp_OnPreSheathGear = { "OnPreSheathGear", nullptr, (EPropertyFlags)0x0010000010080000, UECodeGen_Private::EPropertyGenFlags::InlineMulticastDelegate, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXGearComponent, OnPreSheathGear), Z_Construct_UDelegateFunction_BattleX_PreSheathGearDelegate__DelegateSignature, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_OnPreSheathGear_MetaData), NewProp_OnPreSheathGear_MetaData) }; // 1588087843
const UECodeGen_Private::FMulticastDelegatePropertyParams Z_Construct_UClass_UBXGearComponent_Statics::NewProp_OnPostSheathGear = { "OnPostSheathGear", nullptr, (EPropertyFlags)0x0010000010080000, UECodeGen_Private::EPropertyGenFlags::InlineMulticastDelegate, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXGearComponent, OnPostSheathGear), Z_Construct_UDelegateFunction_BattleX_PostSheathGearDelegate__DelegateSignature, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_OnPostSheathGear_MetaData), NewProp_OnPostSheathGear_MetaData) }; // 1702722271
const UECodeGen_Private::FMulticastDelegatePropertyParams Z_Construct_UClass_UBXGearComponent_Statics::NewProp_OnPreUnsheathGear = { "OnPreUnsheathGear", nullptr, (EPropertyFlags)0x0010000010080000, UECodeGen_Private::EPropertyGenFlags::InlineMulticastDelegate, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXGearComponent, OnPreUnsheathGear), Z_Construct_UDelegateFunction_BattleX_PreUnsheathGearDelegate__DelegateSignature, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_OnPreUnsheathGear_MetaData), NewProp_OnPreUnsheathGear_MetaData) }; // 3402641815
const UECodeGen_Private::FMulticastDelegatePropertyParams Z_Construct_UClass_UBXGearComponent_Statics::NewProp_OnPostUnsheathGear = { "OnPostUnsheathGear", nullptr, (EPropertyFlags)0x0010000010080000, UECodeGen_Private::EPropertyGenFlags::InlineMulticastDelegate, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXGearComponent, OnPostUnsheathGear), Z_Construct_UDelegateFunction_BattleX_PostUnsheathGearDelegate__DelegateSignature, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_OnPostUnsheathGear_MetaData), NewProp_OnPostUnsheathGear_MetaData) }; // 1469070953
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UBXGearComponent_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXGearComponent_Statics::NewProp_SlotMaxSize_ValueProp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXGearComponent_Statics::NewProp_SlotMaxSize_Key_KeyProp_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXGearComponent_Statics::NewProp_SlotMaxSize_Key_KeyProp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXGearComponent_Statics::NewProp_SlotMaxSize,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXGearComponent_Statics::NewProp_EquipGears_ValueProp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXGearComponent_Statics::NewProp_EquipGears_Key_KeyProp_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXGearComponent_Statics::NewProp_EquipGears_Key_KeyProp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXGearComponent_Statics::NewProp_EquipGears,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXGearComponent_Statics::NewProp_UsingGearIndexs_ValueProp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXGearComponent_Statics::NewProp_UsingGearIndexs_Key_KeyProp_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXGearComponent_Statics::NewProp_UsingGearIndexs_Key_KeyProp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXGearComponent_Statics::NewProp_UsingGearIndexs,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXGearComponent_Statics::NewProp_HelpEquipInformation,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXGearComponent_Statics::NewProp_HelpUsingInformation,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXGearComponent_Statics::NewProp_HelpSheathInformation,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXGearComponent_Statics::NewProp_OnPreEquipGear,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXGearComponent_Statics::NewProp_OnPostEquipGear,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXGearComponent_Statics::NewProp_OnPreUnequipGear,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXGearComponent_Statics::NewProp_OnPostUnequipGear,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXGearComponent_Statics::NewProp_OnPreUsingGear,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXGearComponent_Statics::NewProp_OnPostUsingGear,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXGearComponent_Statics::NewProp_OnPreUnusingGear,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXGearComponent_Statics::NewProp_OnPostUnusingGear,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXGearComponent_Statics::NewProp_OnPreSheathGear,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXGearComponent_Statics::NewProp_OnPostSheathGear,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXGearComponent_Statics::NewProp_OnPreUnsheathGear,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXGearComponent_Statics::NewProp_OnPostUnsheathGear,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UBXGearComponent_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_UBXGearComponent_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UActorComponent,
	(UObject* (*)())Z_Construct_UPackage__Script_BattleX,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UBXGearComponent_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UBXGearComponent_Statics::ClassParams = {
	&UBXGearComponent::StaticClass,
	"Engine",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	Z_Construct_UClass_UBXGearComponent_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	UE_ARRAY_COUNT(Z_Construct_UClass_UBXGearComponent_Statics::PropPointers),
	0,
	0x00B000A4u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UBXGearComponent_Statics::Class_MetaDataParams), Z_Construct_UClass_UBXGearComponent_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UBXGearComponent()
{
	if (!Z_Registration_Info_UClass_UBXGearComponent.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UBXGearComponent.OuterSingleton, Z_Construct_UClass_UBXGearComponent_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UBXGearComponent.OuterSingleton;
}
template<> BATTLEX_API UClass* StaticClass<UBXGearComponent>()
{
	return UBXGearComponent::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(UBXGearComponent);
UBXGearComponent::~UBXGearComponent() {}
// End Class UBXGearComponent

// Begin Registration
struct Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Gear_BXGearComponent_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UBXGearComponent, UBXGearComponent::StaticClass, TEXT("UBXGearComponent"), &Z_Registration_Info_UClass_UBXGearComponent, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UBXGearComponent), 4058520543U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Gear_BXGearComponent_h_2396083626(TEXT("/Script/BattleX"),
	Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Gear_BXGearComponent_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Gear_BXGearComponent_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
