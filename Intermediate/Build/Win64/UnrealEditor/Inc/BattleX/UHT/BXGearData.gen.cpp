// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "BattleX/Gear/BXGearData.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeBXGearData() {}

// Begin Cross Module References
BATTLEX_API UClass* Z_Construct_UClass_ABXGear_NoRegister();
BATTLEX_API UClass* Z_Construct_UClass_UBXGearData();
BATTLEX_API UClass* Z_Construct_UClass_UBXGearData_NoRegister();
COREUOBJECT_API UClass* Z_Construct_UClass_UClass();
COREUOBJECT_API UClass* Z_Construct_UClass_UObject();
UPackage* Z_Construct_UPackage__Script_BattleX();
// End Cross Module References

// Begin Class UBXGearData
void UBXGearData::StaticRegisterNativesUBXGearData()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UBXGearData);
UClass* Z_Construct_UClass_UBXGearData_NoRegister()
{
	return UBXGearData::StaticClass();
}
struct Z_Construct_UClass_UBXGearData_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "Gear/BXGearData.h" },
		{ "ModuleRelativePath", "Gear/BXGearData.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_GearClass_MetaData[] = {
		{ "Category", "BXGearData" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xd7\xb0\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Gear/BXGearData.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xd7\xb0\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FClassPropertyParams NewProp_GearClass;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UBXGearData>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FClassPropertyParams Z_Construct_UClass_UBXGearData_Statics::NewProp_GearClass = { "GearClass", nullptr, (EPropertyFlags)0x0014000000000015, UECodeGen_Private::EPropertyGenFlags::Class, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXGearData, GearClass), Z_Construct_UClass_UClass, Z_Construct_UClass_ABXGear_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_GearClass_MetaData), NewProp_GearClass_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UBXGearData_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXGearData_Statics::NewProp_GearClass,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UBXGearData_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_UBXGearData_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UObject,
	(UObject* (*)())Z_Construct_UPackage__Script_BattleX,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UBXGearData_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UBXGearData_Statics::ClassParams = {
	&UBXGearData::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_UBXGearData_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_UBXGearData_Statics::PropPointers),
	0,
	0x001000A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UBXGearData_Statics::Class_MetaDataParams), Z_Construct_UClass_UBXGearData_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UBXGearData()
{
	if (!Z_Registration_Info_UClass_UBXGearData.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UBXGearData.OuterSingleton, Z_Construct_UClass_UBXGearData_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UBXGearData.OuterSingleton;
}
template<> BATTLEX_API UClass* StaticClass<UBXGearData>()
{
	return UBXGearData::StaticClass();
}
UBXGearData::UBXGearData(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UBXGearData);
UBXGearData::~UBXGearData() {}
// End Class UBXGearData

// Begin Registration
struct Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Gear_BXGearData_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UBXGearData, UBXGearData::StaticClass, TEXT("UBXGearData"), &Z_Registration_Info_UClass_UBXGearData, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UBXGearData), 3624766017U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Gear_BXGearData_h_1936420552(TEXT("/Script/BattleX"),
	Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Gear_BXGearData_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Gear_BXGearData_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
