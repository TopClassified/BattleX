// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "BattleXEditor/TimelineEditor/Preview/BXTLPreviewGameMode.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeBXTLPreviewGameMode() {}

// Begin Cross Module References
BATTLEXEDITOR_API UClass* Z_Construct_UClass_ABXTLPreviewGameMode();
BATTLEXEDITOR_API UClass* Z_Construct_UClass_ABXTLPreviewGameMode_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_AGameModeBase();
UPackage* Z_Construct_UPackage__Script_BattleXEditor();
// End Cross Module References

// Begin Class ABXTLPreviewGameMode
void ABXTLPreviewGameMode::StaticRegisterNativesABXTLPreviewGameMode()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(ABXTLPreviewGameMode);
UClass* Z_Construct_UClass_ABXTLPreviewGameMode_NoRegister()
{
	return ABXTLPreviewGameMode::StaticClass();
}
struct Z_Construct_UClass_ABXTLPreviewGameMode_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "HideCategories", "Info Rendering MovementReplication Replication Actor Input Movement Collision Rendering HLOD WorldPartition DataLayers Transformation" },
		{ "IncludePath", "TimelineEditor/Preview/BXTLPreviewGameMode.h" },
		{ "ModuleRelativePath", "TimelineEditor/Preview/BXTLPreviewGameMode.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ABXTLPreviewGameMode>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_ABXTLPreviewGameMode_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_AGameModeBase,
	(UObject* (*)())Z_Construct_UPackage__Script_BattleXEditor,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_ABXTLPreviewGameMode_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_ABXTLPreviewGameMode_Statics::ClassParams = {
	&ABXTLPreviewGameMode::StaticClass,
	"Game",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	nullptr,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	0,
	0,
	0x008802ACu,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ABXTLPreviewGameMode_Statics::Class_MetaDataParams), Z_Construct_UClass_ABXTLPreviewGameMode_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_ABXTLPreviewGameMode()
{
	if (!Z_Registration_Info_UClass_ABXTLPreviewGameMode.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_ABXTLPreviewGameMode.OuterSingleton, Z_Construct_UClass_ABXTLPreviewGameMode_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_ABXTLPreviewGameMode.OuterSingleton;
}
template<> BATTLEXEDITOR_API UClass* StaticClass<ABXTLPreviewGameMode>()
{
	return ABXTLPreviewGameMode::StaticClass();
}
ABXTLPreviewGameMode::ABXTLPreviewGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(ABXTLPreviewGameMode);
ABXTLPreviewGameMode::~ABXTLPreviewGameMode() {}
// End Class ABXTLPreviewGameMode

// Begin Registration
struct Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_Preview_BXTLPreviewGameMode_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_ABXTLPreviewGameMode, ABXTLPreviewGameMode::StaticClass, TEXT("ABXTLPreviewGameMode"), &Z_Registration_Info_UClass_ABXTLPreviewGameMode, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(ABXTLPreviewGameMode), 2412773433U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_Preview_BXTLPreviewGameMode_h_2704943623(TEXT("/Script/BattleXEditor"),
	Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_Preview_BXTLPreviewGameMode_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_Preview_BXTLPreviewGameMode_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
