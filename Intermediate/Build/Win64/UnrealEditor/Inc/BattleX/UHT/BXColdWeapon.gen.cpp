// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "BattleX/Gear/BXColdWeapon.h"
#include "BattleX/Collision/BXCollision.h"
#include "BattleX/Gear/BXGearStructs.h"
#include "Runtime/Engine/Classes/Engine/HitResult.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeBXColdWeapon() {}

// Begin Cross Module References
BATTLEX_API UClass* Z_Construct_UClass_ABXColdWeapon();
BATTLEX_API UClass* Z_Construct_UClass_ABXColdWeapon_NoRegister();
BATTLEX_API UClass* Z_Construct_UClass_ABXGear();
BATTLEX_API UClass* Z_Construct_UClass_UBXShapeComponent_NoRegister();
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXCFilter();
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXGHitBoxRecords();
ENGINE_API UClass* Z_Construct_UClass_AActor_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_USceneComponent_NoRegister();
ENGINE_API UEnum* Z_Construct_UEnum_Engine_EObjectTypeQuery();
ENGINE_API UScriptStruct* Z_Construct_UScriptStruct_FHitResult();
UPackage* Z_Construct_UPackage__Script_BattleX();
// End Cross Module References

// Begin Class ABXColdWeapon Function GetAllHitBoxNames
struct Z_Construct_UFunction_ABXColdWeapon_GetAllHitBoxNames_Statics
{
	struct BXColdWeapon_eventGetAllHitBoxNames_Parms
	{
		TArray<FName> OutBoxNames;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Collision" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xef\xbf\xbd\xc8\xa1\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xd0\xb5\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xd7\xb2\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Gear/BXColdWeapon.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xef\xbf\xbd\xc8\xa1\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xd0\xb5\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xd7\xb2\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FNamePropertyParams NewProp_OutBoxNames_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_OutBoxNames;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FNamePropertyParams Z_Construct_UFunction_ABXColdWeapon_GetAllHitBoxNames_Statics::NewProp_OutBoxNames_Inner = { "OutBoxNames", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UFunction_ABXColdWeapon_GetAllHitBoxNames_Statics::NewProp_OutBoxNames = { "OutBoxNames", nullptr, (EPropertyFlags)0x0010000000000180, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXColdWeapon_eventGetAllHitBoxNames_Parms, OutBoxNames), EArrayPropertyFlags::None, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_ABXColdWeapon_GetAllHitBoxNames_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ABXColdWeapon_GetAllHitBoxNames_Statics::NewProp_OutBoxNames_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ABXColdWeapon_GetAllHitBoxNames_Statics::NewProp_OutBoxNames,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_ABXColdWeapon_GetAllHitBoxNames_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ABXColdWeapon_GetAllHitBoxNames_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_ABXColdWeapon, nullptr, "GetAllHitBoxNames", nullptr, nullptr, Z_Construct_UFunction_ABXColdWeapon_GetAllHitBoxNames_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_ABXColdWeapon_GetAllHitBoxNames_Statics::PropPointers), sizeof(Z_Construct_UFunction_ABXColdWeapon_GetAllHitBoxNames_Statics::BXColdWeapon_eventGetAllHitBoxNames_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04420400, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_ABXColdWeapon_GetAllHitBoxNames_Statics::Function_MetaDataParams), Z_Construct_UFunction_ABXColdWeapon_GetAllHitBoxNames_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_ABXColdWeapon_GetAllHitBoxNames_Statics::BXColdWeapon_eventGetAllHitBoxNames_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_ABXColdWeapon_GetAllHitBoxNames()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ABXColdWeapon_GetAllHitBoxNames_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(ABXColdWeapon::execGetAllHitBoxNames)
{
	P_GET_TARRAY_REF(FName,Z_Param_Out_OutBoxNames);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->GetAllHitBoxNames(Z_Param_Out_OutBoxNames);
	P_NATIVE_END;
}
// End Class ABXColdWeapon Function GetAllHitBoxNames

// Begin Class ABXColdWeapon Function GetHitResultsInSeconds
struct Z_Construct_UFunction_ABXColdWeapon_GetHitResultsInSeconds_Statics
{
	struct BXColdWeapon_eventGetHitResultsInSeconds_Parms
	{
		AActor* Requester;
		float Seconds;
		FName BoxName;
		TArray<TEnumAsByte<EObjectTypeQuery> > ObjectTypes;
		FBXCFilter Filter;
		TArray<FHitResult> OutResults;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Collision" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xef\xbf\xbd\xc8\xa1N\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xda\xa3\xef\xbf\xbd\xc4\xb3\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xd7\xb2\xef\xbf\xbd\xd0\xb5\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xd7\xb2\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xcf\xa2\n" },
#endif
		{ "ModuleRelativePath", "Gear/BXColdWeapon.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xef\xbf\xbd\xc8\xa1N\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xda\xa3\xef\xbf\xbd\xc4\xb3\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xd7\xb2\xef\xbf\xbd\xd0\xb5\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xd7\xb2\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xcf\xa2" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_BoxName_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ObjectTypes_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Filter_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Requester;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_Seconds;
	static const UECodeGen_Private::FNamePropertyParams NewProp_BoxName;
	static const UECodeGen_Private::FBytePropertyParams NewProp_ObjectTypes_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_ObjectTypes;
	static const UECodeGen_Private::FStructPropertyParams NewProp_Filter;
	static const UECodeGen_Private::FStructPropertyParams NewProp_OutResults_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_OutResults;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_ABXColdWeapon_GetHitResultsInSeconds_Statics::NewProp_Requester = { "Requester", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXColdWeapon_eventGetHitResultsInSeconds_Parms, Requester), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_ABXColdWeapon_GetHitResultsInSeconds_Statics::NewProp_Seconds = { "Seconds", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXColdWeapon_eventGetHitResultsInSeconds_Parms, Seconds), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FNamePropertyParams Z_Construct_UFunction_ABXColdWeapon_GetHitResultsInSeconds_Statics::NewProp_BoxName = { "BoxName", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXColdWeapon_eventGetHitResultsInSeconds_Parms, BoxName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_BoxName_MetaData), NewProp_BoxName_MetaData) };
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_ABXColdWeapon_GetHitResultsInSeconds_Statics::NewProp_ObjectTypes_Inner = { "ObjectTypes", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UEnum_Engine_EObjectTypeQuery, METADATA_PARAMS(0, nullptr) }; // 1798967895
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UFunction_ABXColdWeapon_GetHitResultsInSeconds_Statics::NewProp_ObjectTypes = { "ObjectTypes", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXColdWeapon_eventGetHitResultsInSeconds_Parms, ObjectTypes), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ObjectTypes_MetaData), NewProp_ObjectTypes_MetaData) }; // 1798967895
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_ABXColdWeapon_GetHitResultsInSeconds_Statics::NewProp_Filter = { "Filter", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXColdWeapon_eventGetHitResultsInSeconds_Parms, Filter), Z_Construct_UScriptStruct_FBXCFilter, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Filter_MetaData), NewProp_Filter_MetaData) }; // 366435299
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_ABXColdWeapon_GetHitResultsInSeconds_Statics::NewProp_OutResults_Inner = { "OutResults", nullptr, (EPropertyFlags)0x0000008000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UScriptStruct_FHitResult, METADATA_PARAMS(0, nullptr) }; // 4100991306
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UFunction_ABXColdWeapon_GetHitResultsInSeconds_Statics::NewProp_OutResults = { "OutResults", nullptr, (EPropertyFlags)0x0010008000000180, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXColdWeapon_eventGetHitResultsInSeconds_Parms, OutResults), EArrayPropertyFlags::None, METADATA_PARAMS(0, nullptr) }; // 4100991306
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_ABXColdWeapon_GetHitResultsInSeconds_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ABXColdWeapon_GetHitResultsInSeconds_Statics::NewProp_Requester,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ABXColdWeapon_GetHitResultsInSeconds_Statics::NewProp_Seconds,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ABXColdWeapon_GetHitResultsInSeconds_Statics::NewProp_BoxName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ABXColdWeapon_GetHitResultsInSeconds_Statics::NewProp_ObjectTypes_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ABXColdWeapon_GetHitResultsInSeconds_Statics::NewProp_ObjectTypes,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ABXColdWeapon_GetHitResultsInSeconds_Statics::NewProp_Filter,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ABXColdWeapon_GetHitResultsInSeconds_Statics::NewProp_OutResults_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ABXColdWeapon_GetHitResultsInSeconds_Statics::NewProp_OutResults,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_ABXColdWeapon_GetHitResultsInSeconds_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ABXColdWeapon_GetHitResultsInSeconds_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_ABXColdWeapon, nullptr, "GetHitResultsInSeconds", nullptr, nullptr, Z_Construct_UFunction_ABXColdWeapon_GetHitResultsInSeconds_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_ABXColdWeapon_GetHitResultsInSeconds_Statics::PropPointers), sizeof(Z_Construct_UFunction_ABXColdWeapon_GetHitResultsInSeconds_Statics::BXColdWeapon_eventGetHitResultsInSeconds_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04420400, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_ABXColdWeapon_GetHitResultsInSeconds_Statics::Function_MetaDataParams), Z_Construct_UFunction_ABXColdWeapon_GetHitResultsInSeconds_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_ABXColdWeapon_GetHitResultsInSeconds_Statics::BXColdWeapon_eventGetHitResultsInSeconds_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_ABXColdWeapon_GetHitResultsInSeconds()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ABXColdWeapon_GetHitResultsInSeconds_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(ABXColdWeapon::execGetHitResultsInSeconds)
{
	P_GET_OBJECT(AActor,Z_Param_Requester);
	P_GET_PROPERTY(FFloatProperty,Z_Param_Seconds);
	P_GET_PROPERTY_REF(FNameProperty,Z_Param_Out_BoxName);
	P_GET_TARRAY_REF(TEnumAsByte<EObjectTypeQuery>,Z_Param_Out_ObjectTypes);
	P_GET_STRUCT_REF(FBXCFilter,Z_Param_Out_Filter);
	P_GET_TARRAY_REF(FHitResult,Z_Param_Out_OutResults);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->GetHitResultsInSeconds(Z_Param_Requester,Z_Param_Seconds,Z_Param_Out_BoxName,Z_Param_Out_ObjectTypes,Z_Param_Out_Filter,Z_Param_Out_OutResults);
	P_NATIVE_END;
}
// End Class ABXColdWeapon Function GetHitResultsInSeconds

// Begin Class ABXColdWeapon Function StartRecordLocation
struct Z_Construct_UFunction_ABXColdWeapon_StartRecordLocation_Statics
{
	struct BXColdWeapon_eventStartRecordLocation_Parms
	{
		TArray<FName> BoxNames;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Collision" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xef\xbf\xbd\xca\xbc\xef\xbf\xbd\xef\xbf\xbd\xc2\xbc\xce\xbb\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Gear/BXColdWeapon.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xef\xbf\xbd\xca\xbc\xef\xbf\xbd\xef\xbf\xbd\xc2\xbc\xce\xbb\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_BoxNames_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FNamePropertyParams NewProp_BoxNames_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_BoxNames;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FNamePropertyParams Z_Construct_UFunction_ABXColdWeapon_StartRecordLocation_Statics::NewProp_BoxNames_Inner = { "BoxNames", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UFunction_ABXColdWeapon_StartRecordLocation_Statics::NewProp_BoxNames = { "BoxNames", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXColdWeapon_eventStartRecordLocation_Parms, BoxNames), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_BoxNames_MetaData), NewProp_BoxNames_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_ABXColdWeapon_StartRecordLocation_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ABXColdWeapon_StartRecordLocation_Statics::NewProp_BoxNames_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ABXColdWeapon_StartRecordLocation_Statics::NewProp_BoxNames,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_ABXColdWeapon_StartRecordLocation_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ABXColdWeapon_StartRecordLocation_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_ABXColdWeapon, nullptr, "StartRecordLocation", nullptr, nullptr, Z_Construct_UFunction_ABXColdWeapon_StartRecordLocation_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_ABXColdWeapon_StartRecordLocation_Statics::PropPointers), sizeof(Z_Construct_UFunction_ABXColdWeapon_StartRecordLocation_Statics::BXColdWeapon_eventStartRecordLocation_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04420400, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_ABXColdWeapon_StartRecordLocation_Statics::Function_MetaDataParams), Z_Construct_UFunction_ABXColdWeapon_StartRecordLocation_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_ABXColdWeapon_StartRecordLocation_Statics::BXColdWeapon_eventStartRecordLocation_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_ABXColdWeapon_StartRecordLocation()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ABXColdWeapon_StartRecordLocation_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(ABXColdWeapon::execStartRecordLocation)
{
	P_GET_TARRAY_REF(FName,Z_Param_Out_BoxNames);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->StartRecordLocation(Z_Param_Out_BoxNames);
	P_NATIVE_END;
}
// End Class ABXColdWeapon Function StartRecordLocation

// Begin Class ABXColdWeapon Function StopRecordLocation
struct Z_Construct_UFunction_ABXColdWeapon_StopRecordLocation_Statics
{
	struct BXColdWeapon_eventStopRecordLocation_Parms
	{
		TArray<FName> BoxNames;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Collision" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xcd\xa3\xd6\xb9\xef\xbf\xbd\xef\xbf\xbd\xc2\xbc\xce\xbb\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Gear/BXColdWeapon.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xcd\xa3\xd6\xb9\xef\xbf\xbd\xef\xbf\xbd\xc2\xbc\xce\xbb\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_BoxNames_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FNamePropertyParams NewProp_BoxNames_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_BoxNames;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FNamePropertyParams Z_Construct_UFunction_ABXColdWeapon_StopRecordLocation_Statics::NewProp_BoxNames_Inner = { "BoxNames", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UFunction_ABXColdWeapon_StopRecordLocation_Statics::NewProp_BoxNames = { "BoxNames", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXColdWeapon_eventStopRecordLocation_Parms, BoxNames), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_BoxNames_MetaData), NewProp_BoxNames_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_ABXColdWeapon_StopRecordLocation_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ABXColdWeapon_StopRecordLocation_Statics::NewProp_BoxNames_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ABXColdWeapon_StopRecordLocation_Statics::NewProp_BoxNames,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_ABXColdWeapon_StopRecordLocation_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ABXColdWeapon_StopRecordLocation_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_ABXColdWeapon, nullptr, "StopRecordLocation", nullptr, nullptr, Z_Construct_UFunction_ABXColdWeapon_StopRecordLocation_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_ABXColdWeapon_StopRecordLocation_Statics::PropPointers), sizeof(Z_Construct_UFunction_ABXColdWeapon_StopRecordLocation_Statics::BXColdWeapon_eventStopRecordLocation_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04420400, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_ABXColdWeapon_StopRecordLocation_Statics::Function_MetaDataParams), Z_Construct_UFunction_ABXColdWeapon_StopRecordLocation_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_ABXColdWeapon_StopRecordLocation_Statics::BXColdWeapon_eventStopRecordLocation_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_ABXColdWeapon_StopRecordLocation()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ABXColdWeapon_StopRecordLocation_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(ABXColdWeapon::execStopRecordLocation)
{
	P_GET_TARRAY_REF(FName,Z_Param_Out_BoxNames);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->StopRecordLocation(Z_Param_Out_BoxNames);
	P_NATIVE_END;
}
// End Class ABXColdWeapon Function StopRecordLocation

// Begin Class ABXColdWeapon
void ABXColdWeapon::StaticRegisterNativesABXColdWeapon()
{
	UClass* Class = ABXColdWeapon::StaticClass();
	static const FNameNativePtrPair Funcs[] = {
		{ "GetAllHitBoxNames", &ABXColdWeapon::execGetAllHitBoxNames },
		{ "GetHitResultsInSeconds", &ABXColdWeapon::execGetHitResultsInSeconds },
		{ "StartRecordLocation", &ABXColdWeapon::execStartRecordLocation },
		{ "StopRecordLocation", &ABXColdWeapon::execStopRecordLocation },
	};
	FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(ABXColdWeapon);
UClass* Z_Construct_UClass_ABXColdWeapon_NoRegister()
{
	return ABXColdWeapon::StaticClass();
}
struct Z_Construct_UClass_ABXColdWeapon_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "IncludePath", "Gear/BXColdWeapon.h" },
		{ "IsBlueprintBase", "true" },
		{ "ModuleRelativePath", "Gear/BXColdWeapon.h" },
		{ "ObjectInitializerConstructorDeclared", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_GCInterval_MetaData[] = {
		{ "Category", "BXColdWeapon" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Gear/BXColdWeapon.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_HitBoxComponent_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "BXColdWeapon" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xef\xbf\xbd\xd7\xb2\xef\xbf\xbd\xef\xbf\xbd\xcf\xa2\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Gear/BXColdWeapon.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xef\xbf\xbd\xd7\xb2\xef\xbf\xbd\xef\xbf\xbd\xcf\xa2\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_RecordBoxNames_MetaData[] = {
		{ "Category", "Collision" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xd2\xaa\xef\xbf\xbd\xef\xbf\xbd\xc2\xbc\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xd7\xb2\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Gear/BXColdWeapon.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xd2\xaa\xef\xbf\xbd\xef\xbf\xbd\xc2\xbc\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xd7\xb2\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_HitBoxRecords_MetaData[] = {
		{ "Category", "Collision" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// 1\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xd7\xb2\xef\xbf\xbd\xef\xbf\xbd\xce\xbb\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xcf\xa2\n" },
#endif
		{ "ModuleRelativePath", "Gear/BXColdWeapon.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "1\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xd7\xb2\xef\xbf\xbd\xef\xbf\xbd\xce\xbb\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xcf\xa2" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_HelpMap1_MetaData[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Gear/BXColdWeapon.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFloatPropertyParams NewProp_GCInterval;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_HitBoxComponent;
	static const UECodeGen_Private::FNamePropertyParams NewProp_RecordBoxNames_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_RecordBoxNames;
	static const UECodeGen_Private::FStructPropertyParams NewProp_HitBoxRecords_ValueProp;
	static const UECodeGen_Private::FNamePropertyParams NewProp_HitBoxRecords_Key_KeyProp;
	static const UECodeGen_Private::FMapPropertyParams NewProp_HitBoxRecords;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_HelpMap1_ValueProp;
	static const UECodeGen_Private::FNamePropertyParams NewProp_HelpMap1_Key_KeyProp;
	static const UECodeGen_Private::FMapPropertyParams NewProp_HelpMap1;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_ABXColdWeapon_GetAllHitBoxNames, "GetAllHitBoxNames" }, // 1475260983
		{ &Z_Construct_UFunction_ABXColdWeapon_GetHitResultsInSeconds, "GetHitResultsInSeconds" }, // 3353426201
		{ &Z_Construct_UFunction_ABXColdWeapon_StartRecordLocation, "StartRecordLocation" }, // 2915706238
		{ &Z_Construct_UFunction_ABXColdWeapon_StopRecordLocation, "StopRecordLocation" }, // 640367305
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ABXColdWeapon>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_ABXColdWeapon_Statics::NewProp_GCInterval = { "GCInterval", nullptr, (EPropertyFlags)0x0020080000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ABXColdWeapon, GCInterval), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_GCInterval_MetaData), NewProp_GCInterval_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ABXColdWeapon_Statics::NewProp_HitBoxComponent = { "HitBoxComponent", nullptr, (EPropertyFlags)0x00200800000a001d, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ABXColdWeapon, HitBoxComponent), Z_Construct_UClass_UBXShapeComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_HitBoxComponent_MetaData), NewProp_HitBoxComponent_MetaData) };
const UECodeGen_Private::FNamePropertyParams Z_Construct_UClass_ABXColdWeapon_Statics::NewProp_RecordBoxNames_Inner = { "RecordBoxNames", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UClass_ABXColdWeapon_Statics::NewProp_RecordBoxNames = { "RecordBoxNames", nullptr, (EPropertyFlags)0x0020080000002004, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ABXColdWeapon, RecordBoxNames), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_RecordBoxNames_MetaData), NewProp_RecordBoxNames_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_ABXColdWeapon_Statics::NewProp_HitBoxRecords_ValueProp = { "HitBoxRecords", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 1, Z_Construct_UScriptStruct_FBXGHitBoxRecords, METADATA_PARAMS(0, nullptr) }; // 1613214264
const UECodeGen_Private::FNamePropertyParams Z_Construct_UClass_ABXColdWeapon_Statics::NewProp_HitBoxRecords_Key_KeyProp = { "HitBoxRecords_Key", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FMapPropertyParams Z_Construct_UClass_ABXColdWeapon_Statics::NewProp_HitBoxRecords = { "HitBoxRecords", nullptr, (EPropertyFlags)0x0020080000002004, UECodeGen_Private::EPropertyGenFlags::Map, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ABXColdWeapon, HitBoxRecords), EMapPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_HitBoxRecords_MetaData), NewProp_HitBoxRecords_MetaData) }; // 1613214264
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ABXColdWeapon_Statics::NewProp_HelpMap1_ValueProp = { "HelpMap1", nullptr, (EPropertyFlags)0x0000000000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 1, Z_Construct_UClass_USceneComponent_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FNamePropertyParams Z_Construct_UClass_ABXColdWeapon_Statics::NewProp_HelpMap1_Key_KeyProp = { "HelpMap1_Key", nullptr, (EPropertyFlags)0x0000000000080008, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FMapPropertyParams Z_Construct_UClass_ABXColdWeapon_Statics::NewProp_HelpMap1 = { "HelpMap1", nullptr, (EPropertyFlags)0x0020088000002008, UECodeGen_Private::EPropertyGenFlags::Map, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ABXColdWeapon, HelpMap1), EMapPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_HelpMap1_MetaData), NewProp_HelpMap1_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_ABXColdWeapon_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ABXColdWeapon_Statics::NewProp_GCInterval,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ABXColdWeapon_Statics::NewProp_HitBoxComponent,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ABXColdWeapon_Statics::NewProp_RecordBoxNames_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ABXColdWeapon_Statics::NewProp_RecordBoxNames,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ABXColdWeapon_Statics::NewProp_HitBoxRecords_ValueProp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ABXColdWeapon_Statics::NewProp_HitBoxRecords_Key_KeyProp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ABXColdWeapon_Statics::NewProp_HitBoxRecords,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ABXColdWeapon_Statics::NewProp_HelpMap1_ValueProp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ABXColdWeapon_Statics::NewProp_HelpMap1_Key_KeyProp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ABXColdWeapon_Statics::NewProp_HelpMap1,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_ABXColdWeapon_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_ABXColdWeapon_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_ABXGear,
	(UObject* (*)())Z_Construct_UPackage__Script_BattleX,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_ABXColdWeapon_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_ABXColdWeapon_Statics::ClassParams = {
	&ABXColdWeapon::StaticClass,
	"Engine",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	Z_Construct_UClass_ABXColdWeapon_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	UE_ARRAY_COUNT(Z_Construct_UClass_ABXColdWeapon_Statics::PropPointers),
	0,
	0x009000A5u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ABXColdWeapon_Statics::Class_MetaDataParams), Z_Construct_UClass_ABXColdWeapon_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_ABXColdWeapon()
{
	if (!Z_Registration_Info_UClass_ABXColdWeapon.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_ABXColdWeapon.OuterSingleton, Z_Construct_UClass_ABXColdWeapon_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_ABXColdWeapon.OuterSingleton;
}
template<> BATTLEX_API UClass* StaticClass<ABXColdWeapon>()
{
	return ABXColdWeapon::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(ABXColdWeapon);
// End Class ABXColdWeapon

// Begin Registration
struct Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Gear_BXColdWeapon_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_ABXColdWeapon, ABXColdWeapon::StaticClass, TEXT("ABXColdWeapon"), &Z_Registration_Info_UClass_ABXColdWeapon, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(ABXColdWeapon), 3831904104U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Gear_BXColdWeapon_h_2785026016(TEXT("/Script/BattleX"),
	Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Gear_BXColdWeapon_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Gear_BXColdWeapon_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
