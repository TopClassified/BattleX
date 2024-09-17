// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "BattleX/Gear/BXGearStructs.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeBXGearStructs() {}

// Begin Cross Module References
BATTLEX_API UClass* Z_Construct_UClass_ABXGear_NoRegister();
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXEquipGearInformation();
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXGears();
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXGHitBoxRecord();
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXGHitBoxRecords();
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXSheathGearInformation();
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXUsingGearInformation();
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FTransform();
UPackage* Z_Construct_UPackage__Script_BattleX();
// End Cross Module References

// Begin ScriptStruct FBXEquipGearInformation
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_BXEquipGearInformation;
class UScriptStruct* FBXEquipGearInformation::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_BXEquipGearInformation.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_BXEquipGearInformation.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FBXEquipGearInformation, (UObject*)Z_Construct_UPackage__Script_BattleX(), TEXT("BXEquipGearInformation"));
	}
	return Z_Registration_Info_UScriptStruct_BXEquipGearInformation.OuterSingleton;
}
template<> BATTLEX_API UScriptStruct* StaticStruct<FBXEquipGearInformation>()
{
	return FBXEquipGearInformation::StaticStruct();
}
struct Z_Construct_UScriptStruct_FBXEquipGearInformation_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe8\xa3\x85\xe4\xb8\x8a\xe8\xa3\x85\xe5\xa4\x87\xe7\x9a\x84\xe7\x9b\xb8\xe5\x85\xb3\xe4\xbf\xa1\xe6\x81\xaf\n" },
#endif
		{ "ModuleRelativePath", "Gear/BXGearStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe8\xa3\x85\xe4\xb8\x8a\xe8\xa3\x85\xe5\xa4\x87\xe7\x9a\x84\xe7\x9b\xb8\xe5\x85\xb3\xe4\xbf\xa1\xe6\x81\xaf" },
#endif
	};
#endif // WITH_METADATA
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FBXEquipGearInformation>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
};
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FBXEquipGearInformation_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_BattleX,
	nullptr,
	&NewStructOps,
	"BXEquipGearInformation",
	nullptr,
	0,
	sizeof(FBXEquipGearInformation),
	alignof(FBXEquipGearInformation),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000001),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXEquipGearInformation_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FBXEquipGearInformation_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FBXEquipGearInformation()
{
	if (!Z_Registration_Info_UScriptStruct_BXEquipGearInformation.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_BXEquipGearInformation.InnerSingleton, Z_Construct_UScriptStruct_FBXEquipGearInformation_Statics::StructParams);
	}
	return Z_Registration_Info_UScriptStruct_BXEquipGearInformation.InnerSingleton;
}
// End ScriptStruct FBXEquipGearInformation

// Begin ScriptStruct FBXUsingGearInformation
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_BXUsingGearInformation;
class UScriptStruct* FBXUsingGearInformation::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_BXUsingGearInformation.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_BXUsingGearInformation.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FBXUsingGearInformation, (UObject*)Z_Construct_UPackage__Script_BattleX(), TEXT("BXUsingGearInformation"));
	}
	return Z_Registration_Info_UScriptStruct_BXUsingGearInformation.OuterSingleton;
}
template<> BATTLEX_API UScriptStruct* StaticStruct<FBXUsingGearInformation>()
{
	return FBXUsingGearInformation::StaticStruct();
}
struct Z_Construct_UScriptStruct_FBXUsingGearInformation_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe4\xbd\xbf\xe7\x94\xa8\xe8\xa3\x85\xe5\xa4\x87\xe7\x9a\x84\xe7\x9b\xb8\xe5\x85\xb3\xe4\xbf\xa1\xe6\x81\xaf\n" },
#endif
		{ "ModuleRelativePath", "Gear/BXGearStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe4\xbd\xbf\xe7\x94\xa8\xe8\xa3\x85\xe5\xa4\x87\xe7\x9a\x84\xe7\x9b\xb8\xe5\x85\xb3\xe4\xbf\xa1\xe6\x81\xaf" },
#endif
	};
#endif // WITH_METADATA
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FBXUsingGearInformation>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
};
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FBXUsingGearInformation_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_BattleX,
	nullptr,
	&NewStructOps,
	"BXUsingGearInformation",
	nullptr,
	0,
	sizeof(FBXUsingGearInformation),
	alignof(FBXUsingGearInformation),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000001),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXUsingGearInformation_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FBXUsingGearInformation_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FBXUsingGearInformation()
{
	if (!Z_Registration_Info_UScriptStruct_BXUsingGearInformation.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_BXUsingGearInformation.InnerSingleton, Z_Construct_UScriptStruct_FBXUsingGearInformation_Statics::StructParams);
	}
	return Z_Registration_Info_UScriptStruct_BXUsingGearInformation.InnerSingleton;
}
// End ScriptStruct FBXUsingGearInformation

// Begin ScriptStruct FBXSheathGearInformation
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_BXSheathGearInformation;
class UScriptStruct* FBXSheathGearInformation::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_BXSheathGearInformation.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_BXSheathGearInformation.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FBXSheathGearInformation, (UObject*)Z_Construct_UPackage__Script_BattleX(), TEXT("BXSheathGearInformation"));
	}
	return Z_Registration_Info_UScriptStruct_BXSheathGearInformation.OuterSingleton;
}
template<> BATTLEX_API UScriptStruct* StaticStruct<FBXSheathGearInformation>()
{
	return FBXSheathGearInformation::StaticStruct();
}
struct Z_Construct_UScriptStruct_FBXSheathGearInformation_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe6\x8c\x82\xe8\xb5\xb7\xe8\xa3\x85\xe5\xa4\x87\xe7\x9b\xb8\xe5\x85\xb3\xe4\xbf\xa1\xe6\x81\xaf\n" },
#endif
		{ "ModuleRelativePath", "Gear/BXGearStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe6\x8c\x82\xe8\xb5\xb7\xe8\xa3\x85\xe5\xa4\x87\xe7\x9b\xb8\xe5\x85\xb3\xe4\xbf\xa1\xe6\x81\xaf" },
#endif
	};
#endif // WITH_METADATA
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FBXSheathGearInformation>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
};
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FBXSheathGearInformation_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_BattleX,
	nullptr,
	&NewStructOps,
	"BXSheathGearInformation",
	nullptr,
	0,
	sizeof(FBXSheathGearInformation),
	alignof(FBXSheathGearInformation),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000001),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXSheathGearInformation_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FBXSheathGearInformation_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FBXSheathGearInformation()
{
	if (!Z_Registration_Info_UScriptStruct_BXSheathGearInformation.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_BXSheathGearInformation.InnerSingleton, Z_Construct_UScriptStruct_FBXSheathGearInformation_Statics::StructParams);
	}
	return Z_Registration_Info_UScriptStruct_BXSheathGearInformation.InnerSingleton;
}
// End ScriptStruct FBXSheathGearInformation

// Begin ScriptStruct FBXGHitBoxRecord
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_BXGHitBoxRecord;
class UScriptStruct* FBXGHitBoxRecord::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_BXGHitBoxRecord.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_BXGHitBoxRecord.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FBXGHitBoxRecord, (UObject*)Z_Construct_UPackage__Script_BattleX(), TEXT("BXGHitBoxRecord"));
	}
	return Z_Registration_Info_UScriptStruct_BXGHitBoxRecord.OuterSingleton;
}
template<> BATTLEX_API UScriptStruct* StaticStruct<FBXGHitBoxRecord>()
{
	return FBXGHitBoxRecord::StaticStruct();
}
struct Z_Construct_UScriptStruct_FBXGHitBoxRecord_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe8\xa3\x85\xe5\xa4\x87\xe7\xa2\xb0\xe6\x92\x9e\xe7\x9b\x92\xe8\xae\xb0\xe5\xbd\x95\xe4\xbf\xa1\xe6\x81\xaf\n" },
#endif
		{ "ModuleRelativePath", "Gear/BXGearStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe8\xa3\x85\xe5\xa4\x87\xe7\xa2\xb0\xe6\x92\x9e\xe7\x9b\x92\xe8\xae\xb0\xe5\xbd\x95\xe4\xbf\xa1\xe6\x81\xaf" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_WTransform_MetaData[] = {
		{ "Category", "BXGHitBoxRecord" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe6\x96\xb9\xe4\xbd\x8d\n" },
#endif
		{ "ModuleRelativePath", "Gear/BXGearStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe6\x96\xb9\xe4\xbd\x8d" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_GameTime_MetaData[] = {
		{ "Category", "BXGHitBoxRecord" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe6\xb8\xb8\xe6\x88\x8f\xe5\x86\x85\xe8\x99\x9a\xe6\x8b\x9f\xe6\x97\xb6\xe9\x97\xb4\n" },
#endif
		{ "ModuleRelativePath", "Gear/BXGearStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe6\xb8\xb8\xe6\x88\x8f\xe5\x86\x85\xe8\x99\x9a\xe6\x8b\x9f\xe6\x97\xb6\xe9\x97\xb4" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_WTransform;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_GameTime;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FBXGHitBoxRecord>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FBXGHitBoxRecord_Statics::NewProp_WTransform = { "WTransform", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXGHitBoxRecord, WTransform), Z_Construct_UScriptStruct_FTransform, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_WTransform_MetaData), NewProp_WTransform_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FBXGHitBoxRecord_Statics::NewProp_GameTime = { "GameTime", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXGHitBoxRecord, GameTime), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_GameTime_MetaData), NewProp_GameTime_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FBXGHitBoxRecord_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXGHitBoxRecord_Statics::NewProp_WTransform,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXGHitBoxRecord_Statics::NewProp_GameTime,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXGHitBoxRecord_Statics::PropPointers) < 2048);
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FBXGHitBoxRecord_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_BattleX,
	nullptr,
	&NewStructOps,
	"BXGHitBoxRecord",
	Z_Construct_UScriptStruct_FBXGHitBoxRecord_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXGHitBoxRecord_Statics::PropPointers),
	sizeof(FBXGHitBoxRecord),
	alignof(FBXGHitBoxRecord),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000001),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXGHitBoxRecord_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FBXGHitBoxRecord_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FBXGHitBoxRecord()
{
	if (!Z_Registration_Info_UScriptStruct_BXGHitBoxRecord.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_BXGHitBoxRecord.InnerSingleton, Z_Construct_UScriptStruct_FBXGHitBoxRecord_Statics::StructParams);
	}
	return Z_Registration_Info_UScriptStruct_BXGHitBoxRecord.InnerSingleton;
}
// End ScriptStruct FBXGHitBoxRecord

// Begin ScriptStruct FBXGHitBoxRecords
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_BXGHitBoxRecords;
class UScriptStruct* FBXGHitBoxRecords::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_BXGHitBoxRecords.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_BXGHitBoxRecords.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FBXGHitBoxRecords, (UObject*)Z_Construct_UPackage__Script_BattleX(), TEXT("BXGHitBoxRecords"));
	}
	return Z_Registration_Info_UScriptStruct_BXGHitBoxRecords.OuterSingleton;
}
template<> BATTLEX_API UScriptStruct* StaticStruct<FBXGHitBoxRecords>()
{
	return FBXGHitBoxRecords::StaticStruct();
}
struct Z_Construct_UScriptStruct_FBXGHitBoxRecords_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Gear/BXGearStructs.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_List_MetaData[] = {
		{ "Category", "BXGHitBoxRecords" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe4\xbd\x8d\xe7\xbd\xae\n" },
#endif
		{ "ModuleRelativePath", "Gear/BXGearStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe4\xbd\x8d\xe7\xbd\xae" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_List_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_List;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FBXGHitBoxRecords>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FBXGHitBoxRecords_Statics::NewProp_List_Inner = { "List", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UScriptStruct_FBXGHitBoxRecord, METADATA_PARAMS(0, nullptr) }; // 4080141207
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FBXGHitBoxRecords_Statics::NewProp_List = { "List", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXGHitBoxRecords, List), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_List_MetaData), NewProp_List_MetaData) }; // 4080141207
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FBXGHitBoxRecords_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXGHitBoxRecords_Statics::NewProp_List_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXGHitBoxRecords_Statics::NewProp_List,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXGHitBoxRecords_Statics::PropPointers) < 2048);
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FBXGHitBoxRecords_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_BattleX,
	nullptr,
	&NewStructOps,
	"BXGHitBoxRecords",
	Z_Construct_UScriptStruct_FBXGHitBoxRecords_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXGHitBoxRecords_Statics::PropPointers),
	sizeof(FBXGHitBoxRecords),
	alignof(FBXGHitBoxRecords),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000001),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXGHitBoxRecords_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FBXGHitBoxRecords_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FBXGHitBoxRecords()
{
	if (!Z_Registration_Info_UScriptStruct_BXGHitBoxRecords.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_BXGHitBoxRecords.InnerSingleton, Z_Construct_UScriptStruct_FBXGHitBoxRecords_Statics::StructParams);
	}
	return Z_Registration_Info_UScriptStruct_BXGHitBoxRecords.InnerSingleton;
}
// End ScriptStruct FBXGHitBoxRecords

// Begin ScriptStruct FBXGears
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_BXGears;
class UScriptStruct* FBXGears::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_BXGears.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_BXGears.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FBXGears, (UObject*)Z_Construct_UPackage__Script_BattleX(), TEXT("BXGears"));
	}
	return Z_Registration_Info_UScriptStruct_BXGears.OuterSingleton;
}
template<> BATTLEX_API UScriptStruct* StaticStruct<FBXGears>()
{
	return FBXGears::StaticStruct();
}
struct Z_Construct_UScriptStruct_FBXGears_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe8\xa3\x85\xe5\xa4\x87\xe5\x88\x97\xe8\xa1\xa8\n" },
#endif
		{ "ModuleRelativePath", "Gear/BXGearStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe8\xa3\x85\xe5\xa4\x87\xe5\x88\x97\xe8\xa1\xa8" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_List_MetaData[] = {
		{ "Category", "BXGears" },
		{ "ModuleRelativePath", "Gear/BXGearStructs.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_List_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_List;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FBXGears>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UScriptStruct_FBXGears_Statics::NewProp_List_Inner = { "List", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UClass_ABXGear_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FBXGears_Statics::NewProp_List = { "List", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXGears, List), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_List_MetaData), NewProp_List_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FBXGears_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXGears_Statics::NewProp_List_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXGears_Statics::NewProp_List,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXGears_Statics::PropPointers) < 2048);
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FBXGears_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_BattleX,
	nullptr,
	&NewStructOps,
	"BXGears",
	Z_Construct_UScriptStruct_FBXGears_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXGears_Statics::PropPointers),
	sizeof(FBXGears),
	alignof(FBXGears),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000001),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXGears_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FBXGears_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FBXGears()
{
	if (!Z_Registration_Info_UScriptStruct_BXGears.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_BXGears.InnerSingleton, Z_Construct_UScriptStruct_FBXGears_Statics::StructParams);
	}
	return Z_Registration_Info_UScriptStruct_BXGears.InnerSingleton;
}
// End ScriptStruct FBXGears

// Begin Registration
struct Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Gear_BXGearStructs_h_Statics
{
	static constexpr FStructRegisterCompiledInInfo ScriptStructInfo[] = {
		{ FBXEquipGearInformation::StaticStruct, Z_Construct_UScriptStruct_FBXEquipGearInformation_Statics::NewStructOps, TEXT("BXEquipGearInformation"), &Z_Registration_Info_UScriptStruct_BXEquipGearInformation, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FBXEquipGearInformation), 974305362U) },
		{ FBXUsingGearInformation::StaticStruct, Z_Construct_UScriptStruct_FBXUsingGearInformation_Statics::NewStructOps, TEXT("BXUsingGearInformation"), &Z_Registration_Info_UScriptStruct_BXUsingGearInformation, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FBXUsingGearInformation), 2397478069U) },
		{ FBXSheathGearInformation::StaticStruct, Z_Construct_UScriptStruct_FBXSheathGearInformation_Statics::NewStructOps, TEXT("BXSheathGearInformation"), &Z_Registration_Info_UScriptStruct_BXSheathGearInformation, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FBXSheathGearInformation), 1661981629U) },
		{ FBXGHitBoxRecord::StaticStruct, Z_Construct_UScriptStruct_FBXGHitBoxRecord_Statics::NewStructOps, TEXT("BXGHitBoxRecord"), &Z_Registration_Info_UScriptStruct_BXGHitBoxRecord, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FBXGHitBoxRecord), 4080141207U) },
		{ FBXGHitBoxRecords::StaticStruct, Z_Construct_UScriptStruct_FBXGHitBoxRecords_Statics::NewStructOps, TEXT("BXGHitBoxRecords"), &Z_Registration_Info_UScriptStruct_BXGHitBoxRecords, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FBXGHitBoxRecords), 1613214264U) },
		{ FBXGears::StaticStruct, Z_Construct_UScriptStruct_FBXGears_Statics::NewStructOps, TEXT("BXGears"), &Z_Registration_Info_UScriptStruct_BXGears, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FBXGears), 2352692587U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Gear_BXGearStructs_h_3818217278(TEXT("/Script/BattleX"),
	nullptr, 0,
	Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Gear_BXGearStructs_h_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Gear_BXGearStructs_h_Statics::ScriptStructInfo),
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
