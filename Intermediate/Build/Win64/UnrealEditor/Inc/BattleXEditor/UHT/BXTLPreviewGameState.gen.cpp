// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "BattleXEditor/TimelineEditor/Preview/BXTLPreviewGameState.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeBXTLPreviewGameState() {}

// Begin Cross Module References
BATTLEXEDITOR_API UClass* Z_Construct_UClass_ABXTLPreviewGameState();
BATTLEXEDITOR_API UClass* Z_Construct_UClass_ABXTLPreviewGameState_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_AGameState();
UPackage* Z_Construct_UPackage__Script_BattleXEditor();
// End Cross Module References

// Begin Class ABXTLPreviewGameState
void ABXTLPreviewGameState::StaticRegisterNativesABXTLPreviewGameState()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(ABXTLPreviewGameState);
UClass* Z_Construct_UClass_ABXTLPreviewGameState_NoRegister()
{
	return ABXTLPreviewGameState::StaticClass();
}
struct Z_Construct_UClass_ABXTLPreviewGameState_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "HideCategories", "Input Movement Collision Rendering HLOD WorldPartition DataLayers Transformation" },
		{ "IncludePath", "TimelineEditor/Preview/BXTLPreviewGameState.h" },
		{ "ModuleRelativePath", "TimelineEditor/Preview/BXTLPreviewGameState.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ABXTLPreviewGameState>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_ABXTLPreviewGameState_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_AGameState,
	(UObject* (*)())Z_Construct_UPackage__Script_BattleXEditor,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_ABXTLPreviewGameState_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_ABXTLPreviewGameState_Statics::ClassParams = {
	&ABXTLPreviewGameState::StaticClass,
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
	0x008002A4u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ABXTLPreviewGameState_Statics::Class_MetaDataParams), Z_Construct_UClass_ABXTLPreviewGameState_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_ABXTLPreviewGameState()
{
	if (!Z_Registration_Info_UClass_ABXTLPreviewGameState.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_ABXTLPreviewGameState.OuterSingleton, Z_Construct_UClass_ABXTLPreviewGameState_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_ABXTLPreviewGameState.OuterSingleton;
}
template<> BATTLEXEDITOR_API UClass* StaticClass<ABXTLPreviewGameState>()
{
	return ABXTLPreviewGameState::StaticClass();
}
ABXTLPreviewGameState::ABXTLPreviewGameState(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(ABXTLPreviewGameState);
ABXTLPreviewGameState::~ABXTLPreviewGameState() {}
// End Class ABXTLPreviewGameState

// Begin Registration
struct Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_Preview_BXTLPreviewGameState_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_ABXTLPreviewGameState, ABXTLPreviewGameState::StaticClass, TEXT("ABXTLPreviewGameState"), &Z_Registration_Info_UClass_ABXTLPreviewGameState, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(ABXTLPreviewGameState), 3644456275U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_Preview_BXTLPreviewGameState_h_19626780(TEXT("/Script/BattleXEditor"),
	Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_Preview_BXTLPreviewGameState_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_Preview_BXTLPreviewGameState_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
