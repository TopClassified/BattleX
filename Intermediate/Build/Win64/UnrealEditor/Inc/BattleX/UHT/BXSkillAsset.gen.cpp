// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "BattleX/Timeline/BXSkillAsset.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeBXSkillAsset() {}

// Begin Cross Module References
BATTLEX_API UClass* Z_Construct_UClass_UBXSkillAsset();
BATTLEX_API UClass* Z_Construct_UClass_UBXSkillAsset_NoRegister();
BATTLEX_API UClass* Z_Construct_UClass_UBXTLAsset();
UPackage* Z_Construct_UPackage__Script_BattleX();
// End Cross Module References

// Begin Class UBXSkillAsset
void UBXSkillAsset::StaticRegisterNativesUBXSkillAsset()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UBXSkillAsset);
UClass* Z_Construct_UClass_UBXSkillAsset_NoRegister()
{
	return UBXSkillAsset::StaticClass();
}
struct Z_Construct_UClass_UBXSkillAsset_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "Timeline/BXSkillAsset.h" },
		{ "ModuleRelativePath", "Timeline/BXSkillAsset.h" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UBXSkillAsset>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_UBXSkillAsset_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UBXTLAsset,
	(UObject* (*)())Z_Construct_UPackage__Script_BattleX,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UBXSkillAsset_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UBXSkillAsset_Statics::ClassParams = {
	&UBXSkillAsset::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	nullptr,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	0,
	0,
	0x009000A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UBXSkillAsset_Statics::Class_MetaDataParams), Z_Construct_UClass_UBXSkillAsset_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UBXSkillAsset()
{
	if (!Z_Registration_Info_UClass_UBXSkillAsset.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UBXSkillAsset.OuterSingleton, Z_Construct_UClass_UBXSkillAsset_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UBXSkillAsset.OuterSingleton;
}
template<> BATTLEX_API UClass* StaticClass<UBXSkillAsset>()
{
	return UBXSkillAsset::StaticClass();
}
UBXSkillAsset::UBXSkillAsset() {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UBXSkillAsset);
UBXSkillAsset::~UBXSkillAsset() {}
// End Class UBXSkillAsset

// Begin Registration
struct Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Timeline_BXSkillAsset_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UBXSkillAsset, UBXSkillAsset::StaticClass, TEXT("UBXSkillAsset"), &Z_Registration_Info_UClass_UBXSkillAsset, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UBXSkillAsset), 1802729020U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Timeline_BXSkillAsset_h_2769281495(TEXT("/Script/BattleX"),
	Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Timeline_BXSkillAsset_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Timeline_BXSkillAsset_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
