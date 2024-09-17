// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "BattleXEditor/TimelineEditor/BXTLAssetFactory.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeBXTLAssetFactory() {}

// Begin Cross Module References
BATTLEXEDITOR_API UClass* Z_Construct_UClass_UBXSkillAssetFactory();
BATTLEXEDITOR_API UClass* Z_Construct_UClass_UBXSkillAssetFactory_NoRegister();
BATTLEXEDITOR_API UClass* Z_Construct_UClass_UBXTLAssetFactory();
BATTLEXEDITOR_API UClass* Z_Construct_UClass_UBXTLAssetFactory_NoRegister();
UNREALED_API UClass* Z_Construct_UClass_UFactory();
UPackage* Z_Construct_UPackage__Script_BattleXEditor();
// End Cross Module References

// Begin Class UBXTLAssetFactory
void UBXTLAssetFactory::StaticRegisterNativesUBXTLAssetFactory()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UBXTLAssetFactory);
UClass* Z_Construct_UClass_UBXTLAssetFactory_NoRegister()
{
	return UBXTLAssetFactory::StaticClass();
}
struct Z_Construct_UClass_UBXTLAssetFactory_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "HideCategories", "Object" },
		{ "IncludePath", "TimelineEditor/BXTLAssetFactory.h" },
		{ "ModuleRelativePath", "TimelineEditor/BXTLAssetFactory.h" },
		{ "ObjectInitializerConstructorDeclared", "" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UBXTLAssetFactory>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_UBXTLAssetFactory_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UFactory,
	(UObject* (*)())Z_Construct_UPackage__Script_BattleXEditor,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UBXTLAssetFactory_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UBXTLAssetFactory_Statics::ClassParams = {
	&UBXTLAssetFactory::StaticClass,
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
	0x000800A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UBXTLAssetFactory_Statics::Class_MetaDataParams), Z_Construct_UClass_UBXTLAssetFactory_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UBXTLAssetFactory()
{
	if (!Z_Registration_Info_UClass_UBXTLAssetFactory.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UBXTLAssetFactory.OuterSingleton, Z_Construct_UClass_UBXTLAssetFactory_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UBXTLAssetFactory.OuterSingleton;
}
template<> BATTLEXEDITOR_API UClass* StaticClass<UBXTLAssetFactory>()
{
	return UBXTLAssetFactory::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(UBXTLAssetFactory);
// End Class UBXTLAssetFactory

// Begin Class UBXSkillAssetFactory
void UBXSkillAssetFactory::StaticRegisterNativesUBXSkillAssetFactory()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UBXSkillAssetFactory);
UClass* Z_Construct_UClass_UBXSkillAssetFactory_NoRegister()
{
	return UBXSkillAssetFactory::StaticClass();
}
struct Z_Construct_UClass_UBXSkillAssetFactory_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "HideCategories", "Object" },
		{ "IncludePath", "TimelineEditor/BXTLAssetFactory.h" },
		{ "ModuleRelativePath", "TimelineEditor/BXTLAssetFactory.h" },
		{ "ObjectInitializerConstructorDeclared", "" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UBXSkillAssetFactory>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_UBXSkillAssetFactory_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UFactory,
	(UObject* (*)())Z_Construct_UPackage__Script_BattleXEditor,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UBXSkillAssetFactory_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UBXSkillAssetFactory_Statics::ClassParams = {
	&UBXSkillAssetFactory::StaticClass,
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
	0x000800A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UBXSkillAssetFactory_Statics::Class_MetaDataParams), Z_Construct_UClass_UBXSkillAssetFactory_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UBXSkillAssetFactory()
{
	if (!Z_Registration_Info_UClass_UBXSkillAssetFactory.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UBXSkillAssetFactory.OuterSingleton, Z_Construct_UClass_UBXSkillAssetFactory_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UBXSkillAssetFactory.OuterSingleton;
}
template<> BATTLEXEDITOR_API UClass* StaticClass<UBXSkillAssetFactory>()
{
	return UBXSkillAssetFactory::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(UBXSkillAssetFactory);
// End Class UBXSkillAssetFactory

// Begin Registration
struct Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_BXTLAssetFactory_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UBXTLAssetFactory, UBXTLAssetFactory::StaticClass, TEXT("UBXTLAssetFactory"), &Z_Registration_Info_UClass_UBXTLAssetFactory, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UBXTLAssetFactory), 1930862443U) },
		{ Z_Construct_UClass_UBXSkillAssetFactory, UBXSkillAssetFactory::StaticClass, TEXT("UBXSkillAssetFactory"), &Z_Registration_Info_UClass_UBXSkillAssetFactory, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UBXSkillAssetFactory), 3583863001U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_BXTLAssetFactory_h_2611164425(TEXT("/Script/BattleXEditor"),
	Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_BXTLAssetFactory_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_BXTLAssetFactory_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
