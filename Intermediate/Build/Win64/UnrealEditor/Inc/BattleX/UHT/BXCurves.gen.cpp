// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "BattleX/BXCurves.h"
#include "Runtime/Engine/Classes/Curves/CurveFloat.h"
#include "Runtime/Engine/Classes/Curves/CurveLinearColor.h"
#include "Runtime/Engine/Classes/Curves/CurveVector.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeBXCurves() {}

// Begin Cross Module References
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXRemapColorCurve();
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXRemapFloatCurve();
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXRemapVectorCurve();
ENGINE_API UScriptStruct* Z_Construct_UScriptStruct_FRuntimeCurveLinearColor();
ENGINE_API UScriptStruct* Z_Construct_UScriptStruct_FRuntimeFloatCurve();
ENGINE_API UScriptStruct* Z_Construct_UScriptStruct_FRuntimeVectorCurve();
UPackage* Z_Construct_UPackage__Script_BattleX();
// End Cross Module References

// Begin ScriptStruct FBXRemapFloatCurve
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_BXRemapFloatCurve;
class UScriptStruct* FBXRemapFloatCurve::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_BXRemapFloatCurve.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_BXRemapFloatCurve.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FBXRemapFloatCurve, (UObject*)Z_Construct_UPackage__Script_BattleX(), TEXT("BXRemapFloatCurve"));
	}
	return Z_Registration_Info_UScriptStruct_BXRemapFloatCurve.OuterSingleton;
}
template<> BATTLEX_API UScriptStruct* StaticStruct<FBXRemapFloatCurve>()
{
	return FBXRemapFloatCurve::StaticStruct();
}
struct Z_Construct_UScriptStruct_FBXRemapFloatCurve_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "BXCurves.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bNeedRemap_MetaData[] = {
		{ "Category", "BXRemapFloatCurve" },
		{ "ModuleRelativePath", "BXCurves.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Curve_MetaData[] = {
		{ "Category", "BXRemapFloatCurve" },
		{ "ModuleRelativePath", "BXCurves.h" },
	};
#endif // WITH_METADATA
	static void NewProp_bNeedRemap_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bNeedRemap;
	static const UECodeGen_Private::FStructPropertyParams NewProp_Curve;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FBXRemapFloatCurve>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
};
void Z_Construct_UScriptStruct_FBXRemapFloatCurve_Statics::NewProp_bNeedRemap_SetBit(void* Obj)
{
	((FBXRemapFloatCurve*)Obj)->bNeedRemap = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FBXRemapFloatCurve_Statics::NewProp_bNeedRemap = { "bNeedRemap", nullptr, (EPropertyFlags)0x0010000000010001, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FBXRemapFloatCurve), &Z_Construct_UScriptStruct_FBXRemapFloatCurve_Statics::NewProp_bNeedRemap_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bNeedRemap_MetaData), NewProp_bNeedRemap_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FBXRemapFloatCurve_Statics::NewProp_Curve = { "Curve", nullptr, (EPropertyFlags)0x0010000000010001, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXRemapFloatCurve, Curve), Z_Construct_UScriptStruct_FRuntimeFloatCurve, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Curve_MetaData), NewProp_Curve_MetaData) }; // 762678829
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FBXRemapFloatCurve_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXRemapFloatCurve_Statics::NewProp_bNeedRemap,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXRemapFloatCurve_Statics::NewProp_Curve,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXRemapFloatCurve_Statics::PropPointers) < 2048);
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FBXRemapFloatCurve_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_BattleX,
	nullptr,
	&NewStructOps,
	"BXRemapFloatCurve",
	Z_Construct_UScriptStruct_FBXRemapFloatCurve_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXRemapFloatCurve_Statics::PropPointers),
	sizeof(FBXRemapFloatCurve),
	alignof(FBXRemapFloatCurve),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000201),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXRemapFloatCurve_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FBXRemapFloatCurve_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FBXRemapFloatCurve()
{
	if (!Z_Registration_Info_UScriptStruct_BXRemapFloatCurve.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_BXRemapFloatCurve.InnerSingleton, Z_Construct_UScriptStruct_FBXRemapFloatCurve_Statics::StructParams);
	}
	return Z_Registration_Info_UScriptStruct_BXRemapFloatCurve.InnerSingleton;
}
// End ScriptStruct FBXRemapFloatCurve

// Begin ScriptStruct FBXRemapVectorCurve
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_BXRemapVectorCurve;
class UScriptStruct* FBXRemapVectorCurve::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_BXRemapVectorCurve.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_BXRemapVectorCurve.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FBXRemapVectorCurve, (UObject*)Z_Construct_UPackage__Script_BattleX(), TEXT("BXRemapVectorCurve"));
	}
	return Z_Registration_Info_UScriptStruct_BXRemapVectorCurve.OuterSingleton;
}
template<> BATTLEX_API UScriptStruct* StaticStruct<FBXRemapVectorCurve>()
{
	return FBXRemapVectorCurve::StaticStruct();
}
struct Z_Construct_UScriptStruct_FBXRemapVectorCurve_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "BXCurves.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bNeedRemap_MetaData[] = {
		{ "Category", "BXRemapVectorCurve" },
		{ "ModuleRelativePath", "BXCurves.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Curve_MetaData[] = {
		{ "Category", "BXRemapVectorCurve" },
		{ "ModuleRelativePath", "BXCurves.h" },
	};
#endif // WITH_METADATA
	static void NewProp_bNeedRemap_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bNeedRemap;
	static const UECodeGen_Private::FStructPropertyParams NewProp_Curve;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FBXRemapVectorCurve>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
};
void Z_Construct_UScriptStruct_FBXRemapVectorCurve_Statics::NewProp_bNeedRemap_SetBit(void* Obj)
{
	((FBXRemapVectorCurve*)Obj)->bNeedRemap = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FBXRemapVectorCurve_Statics::NewProp_bNeedRemap = { "bNeedRemap", nullptr, (EPropertyFlags)0x0010000000010001, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FBXRemapVectorCurve), &Z_Construct_UScriptStruct_FBXRemapVectorCurve_Statics::NewProp_bNeedRemap_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bNeedRemap_MetaData), NewProp_bNeedRemap_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FBXRemapVectorCurve_Statics::NewProp_Curve = { "Curve", nullptr, (EPropertyFlags)0x0010000000010001, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXRemapVectorCurve, Curve), Z_Construct_UScriptStruct_FRuntimeVectorCurve, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Curve_MetaData), NewProp_Curve_MetaData) }; // 3233604077
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FBXRemapVectorCurve_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXRemapVectorCurve_Statics::NewProp_bNeedRemap,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXRemapVectorCurve_Statics::NewProp_Curve,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXRemapVectorCurve_Statics::PropPointers) < 2048);
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FBXRemapVectorCurve_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_BattleX,
	nullptr,
	&NewStructOps,
	"BXRemapVectorCurve",
	Z_Construct_UScriptStruct_FBXRemapVectorCurve_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXRemapVectorCurve_Statics::PropPointers),
	sizeof(FBXRemapVectorCurve),
	alignof(FBXRemapVectorCurve),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000201),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXRemapVectorCurve_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FBXRemapVectorCurve_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FBXRemapVectorCurve()
{
	if (!Z_Registration_Info_UScriptStruct_BXRemapVectorCurve.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_BXRemapVectorCurve.InnerSingleton, Z_Construct_UScriptStruct_FBXRemapVectorCurve_Statics::StructParams);
	}
	return Z_Registration_Info_UScriptStruct_BXRemapVectorCurve.InnerSingleton;
}
// End ScriptStruct FBXRemapVectorCurve

// Begin ScriptStruct FBXRemapColorCurve
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_BXRemapColorCurve;
class UScriptStruct* FBXRemapColorCurve::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_BXRemapColorCurve.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_BXRemapColorCurve.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FBXRemapColorCurve, (UObject*)Z_Construct_UPackage__Script_BattleX(), TEXT("BXRemapColorCurve"));
	}
	return Z_Registration_Info_UScriptStruct_BXRemapColorCurve.OuterSingleton;
}
template<> BATTLEX_API UScriptStruct* StaticStruct<FBXRemapColorCurve>()
{
	return FBXRemapColorCurve::StaticStruct();
}
struct Z_Construct_UScriptStruct_FBXRemapColorCurve_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "BXCurves.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bNeedRemap_MetaData[] = {
		{ "Category", "BXRemapColorCurve" },
		{ "ModuleRelativePath", "BXCurves.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Curve_MetaData[] = {
		{ "Category", "BXRemapColorCurve" },
		{ "ModuleRelativePath", "BXCurves.h" },
	};
#endif // WITH_METADATA
	static void NewProp_bNeedRemap_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bNeedRemap;
	static const UECodeGen_Private::FStructPropertyParams NewProp_Curve;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FBXRemapColorCurve>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
};
void Z_Construct_UScriptStruct_FBXRemapColorCurve_Statics::NewProp_bNeedRemap_SetBit(void* Obj)
{
	((FBXRemapColorCurve*)Obj)->bNeedRemap = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FBXRemapColorCurve_Statics::NewProp_bNeedRemap = { "bNeedRemap", nullptr, (EPropertyFlags)0x0010000000010001, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FBXRemapColorCurve), &Z_Construct_UScriptStruct_FBXRemapColorCurve_Statics::NewProp_bNeedRemap_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bNeedRemap_MetaData), NewProp_bNeedRemap_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FBXRemapColorCurve_Statics::NewProp_Curve = { "Curve", nullptr, (EPropertyFlags)0x0010000000010001, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXRemapColorCurve, Curve), Z_Construct_UScriptStruct_FRuntimeCurveLinearColor, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Curve_MetaData), NewProp_Curve_MetaData) }; // 2743372398
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FBXRemapColorCurve_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXRemapColorCurve_Statics::NewProp_bNeedRemap,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXRemapColorCurve_Statics::NewProp_Curve,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXRemapColorCurve_Statics::PropPointers) < 2048);
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FBXRemapColorCurve_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_BattleX,
	nullptr,
	&NewStructOps,
	"BXRemapColorCurve",
	Z_Construct_UScriptStruct_FBXRemapColorCurve_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXRemapColorCurve_Statics::PropPointers),
	sizeof(FBXRemapColorCurve),
	alignof(FBXRemapColorCurve),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000201),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXRemapColorCurve_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FBXRemapColorCurve_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FBXRemapColorCurve()
{
	if (!Z_Registration_Info_UScriptStruct_BXRemapColorCurve.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_BXRemapColorCurve.InnerSingleton, Z_Construct_UScriptStruct_FBXRemapColorCurve_Statics::StructParams);
	}
	return Z_Registration_Info_UScriptStruct_BXRemapColorCurve.InnerSingleton;
}
// End ScriptStruct FBXRemapColorCurve

// Begin Registration
struct Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_BXCurves_h_Statics
{
	static constexpr FStructRegisterCompiledInInfo ScriptStructInfo[] = {
		{ FBXRemapFloatCurve::StaticStruct, Z_Construct_UScriptStruct_FBXRemapFloatCurve_Statics::NewStructOps, TEXT("BXRemapFloatCurve"), &Z_Registration_Info_UScriptStruct_BXRemapFloatCurve, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FBXRemapFloatCurve), 1368283841U) },
		{ FBXRemapVectorCurve::StaticStruct, Z_Construct_UScriptStruct_FBXRemapVectorCurve_Statics::NewStructOps, TEXT("BXRemapVectorCurve"), &Z_Registration_Info_UScriptStruct_BXRemapVectorCurve, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FBXRemapVectorCurve), 1551862340U) },
		{ FBXRemapColorCurve::StaticStruct, Z_Construct_UScriptStruct_FBXRemapColorCurve_Statics::NewStructOps, TEXT("BXRemapColorCurve"), &Z_Registration_Info_UScriptStruct_BXRemapColorCurve, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FBXRemapColorCurve), 1499953459U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_BXCurves_h_3840542886(TEXT("/Script/BattleX"),
	nullptr, 0,
	Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_BXCurves_h_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_BXCurves_h_Statics::ScriptStructInfo),
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
