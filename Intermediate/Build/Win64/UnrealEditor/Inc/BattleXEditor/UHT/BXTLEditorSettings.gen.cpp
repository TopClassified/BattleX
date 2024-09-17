// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "BattleXEditor/TimelineEditor/BXTLEditorSettings.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeBXTLEditorSettings() {}

// Begin Cross Module References
BATTLEXEDITOR_API UClass* Z_Construct_UClass_ABXTLPreviewGameMode_NoRegister();
BATTLEXEDITOR_API UClass* Z_Construct_UClass_UBXTLEditorSettings();
BATTLEXEDITOR_API UClass* Z_Construct_UClass_UBXTLEditorSettings_NoRegister();
BATTLEXEDITOR_API UClass* Z_Construct_UClass_UBXTLPreviewActor_NoRegister();
BATTLEXEDITOR_API UClass* Z_Construct_UClass_UBXTLTaskGroupTemplate_NoRegister();
COREUOBJECT_API UClass* Z_Construct_UClass_UClass();
DEVELOPERSETTINGS_API UClass* Z_Construct_UClass_UDeveloperSettings();
ENGINE_API UClass* Z_Construct_UClass_UWorld_NoRegister();
UPackage* Z_Construct_UPackage__Script_BattleXEditor();
// End Cross Module References

// Begin Class UBXTLEditorSettings
void UBXTLEditorSettings::StaticRegisterNativesUBXTLEditorSettings()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UBXTLEditorSettings);
UClass* Z_Construct_UClass_UBXTLEditorSettings_NoRegister()
{
	return UBXTLEditorSettings::StaticClass();
}
struct Z_Construct_UClass_UBXTLEditorSettings_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "DisplayName", "BSA Editor Settings" },
		{ "IncludePath", "TimelineEditor/BXTLEditorSettings.h" },
		{ "ModuleRelativePath", "TimelineEditor/BXTLEditorSettings.h" },
		{ "ObjectInitializerConstructorDeclared", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_PreviewTickRate_MetaData[] = {
		{ "Category", "Preview" },
		{ "ModuleRelativePath", "TimelineEditor/BXTLEditorSettings.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_GameModeClass_MetaData[] = {
		{ "Category", "PreviewScene" },
		{ "ModuleRelativePath", "TimelineEditor/BXTLEditorSettings.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_DefaultViewMap_MetaData[] = {
		{ "Category", "PreviewScene" },
		{ "ModuleRelativePath", "TimelineEditor/BXTLEditorSettings.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_PlayerType_MetaData[] = {
		{ "Category", "PreviewScene" },
		{ "ModuleRelativePath", "TimelineEditor/BXTLEditorSettings.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_TargetType_MetaData[] = {
		{ "Category", "PreviewScene" },
		{ "ModuleRelativePath", "TimelineEditor/BXTLEditorSettings.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Template_MetaData[] = {
		{ "Category", "Edit" },
		{ "ModuleRelativePath", "TimelineEditor/BXTLEditorSettings.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFloatPropertyParams NewProp_PreviewTickRate;
	static const UECodeGen_Private::FClassPropertyParams NewProp_GameModeClass;
	static const UECodeGen_Private::FSoftObjectPropertyParams NewProp_DefaultViewMap;
	static const UECodeGen_Private::FClassPropertyParams NewProp_PlayerType;
	static const UECodeGen_Private::FClassPropertyParams NewProp_TargetType;
	static const UECodeGen_Private::FSoftObjectPropertyParams NewProp_Template;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UBXTLEditorSettings>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UBXTLEditorSettings_Statics::NewProp_PreviewTickRate = { "PreviewTickRate", nullptr, (EPropertyFlags)0x0010000000004001, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXTLEditorSettings, PreviewTickRate), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_PreviewTickRate_MetaData), NewProp_PreviewTickRate_MetaData) };
const UECodeGen_Private::FClassPropertyParams Z_Construct_UClass_UBXTLEditorSettings_Statics::NewProp_GameModeClass = { "GameModeClass", nullptr, (EPropertyFlags)0x0014000000004001, UECodeGen_Private::EPropertyGenFlags::Class, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXTLEditorSettings, GameModeClass), Z_Construct_UClass_UClass, Z_Construct_UClass_ABXTLPreviewGameMode_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_GameModeClass_MetaData), NewProp_GameModeClass_MetaData) };
const UECodeGen_Private::FSoftObjectPropertyParams Z_Construct_UClass_UBXTLEditorSettings_Statics::NewProp_DefaultViewMap = { "DefaultViewMap", nullptr, (EPropertyFlags)0x0014000000004001, UECodeGen_Private::EPropertyGenFlags::SoftObject, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXTLEditorSettings, DefaultViewMap), Z_Construct_UClass_UWorld_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_DefaultViewMap_MetaData), NewProp_DefaultViewMap_MetaData) };
const UECodeGen_Private::FClassPropertyParams Z_Construct_UClass_UBXTLEditorSettings_Statics::NewProp_PlayerType = { "PlayerType", nullptr, (EPropertyFlags)0x0014000000004001, UECodeGen_Private::EPropertyGenFlags::Class, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXTLEditorSettings, PlayerType), Z_Construct_UClass_UClass, Z_Construct_UClass_UBXTLPreviewActor_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_PlayerType_MetaData), NewProp_PlayerType_MetaData) };
const UECodeGen_Private::FClassPropertyParams Z_Construct_UClass_UBXTLEditorSettings_Statics::NewProp_TargetType = { "TargetType", nullptr, (EPropertyFlags)0x0014000000004001, UECodeGen_Private::EPropertyGenFlags::Class, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXTLEditorSettings, TargetType), Z_Construct_UClass_UClass, Z_Construct_UClass_UBXTLPreviewActor_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_TargetType_MetaData), NewProp_TargetType_MetaData) };
const UECodeGen_Private::FSoftObjectPropertyParams Z_Construct_UClass_UBXTLEditorSettings_Statics::NewProp_Template = { "Template", nullptr, (EPropertyFlags)0x0014000000004001, UECodeGen_Private::EPropertyGenFlags::SoftObject, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXTLEditorSettings, Template), Z_Construct_UClass_UBXTLTaskGroupTemplate_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Template_MetaData), NewProp_Template_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UBXTLEditorSettings_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXTLEditorSettings_Statics::NewProp_PreviewTickRate,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXTLEditorSettings_Statics::NewProp_GameModeClass,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXTLEditorSettings_Statics::NewProp_DefaultViewMap,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXTLEditorSettings_Statics::NewProp_PlayerType,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXTLEditorSettings_Statics::NewProp_TargetType,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXTLEditorSettings_Statics::NewProp_Template,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UBXTLEditorSettings_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_UBXTLEditorSettings_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UDeveloperSettings,
	(UObject* (*)())Z_Construct_UPackage__Script_BattleXEditor,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UBXTLEditorSettings_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UBXTLEditorSettings_Statics::ClassParams = {
	&UBXTLEditorSettings::StaticClass,
	"Editor",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_UBXTLEditorSettings_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_UBXTLEditorSettings_Statics::PropPointers),
	0,
	0x001000A6u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UBXTLEditorSettings_Statics::Class_MetaDataParams), Z_Construct_UClass_UBXTLEditorSettings_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UBXTLEditorSettings()
{
	if (!Z_Registration_Info_UClass_UBXTLEditorSettings.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UBXTLEditorSettings.OuterSingleton, Z_Construct_UClass_UBXTLEditorSettings_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UBXTLEditorSettings.OuterSingleton;
}
template<> BATTLEXEDITOR_API UClass* StaticClass<UBXTLEditorSettings>()
{
	return UBXTLEditorSettings::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(UBXTLEditorSettings);
UBXTLEditorSettings::~UBXTLEditorSettings() {}
// End Class UBXTLEditorSettings

// Begin Registration
struct Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_BXTLEditorSettings_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UBXTLEditorSettings, UBXTLEditorSettings::StaticClass, TEXT("UBXTLEditorSettings"), &Z_Registration_Info_UClass_UBXTLEditorSettings, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UBXTLEditorSettings), 4156659722U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_BXTLEditorSettings_h_4012846751(TEXT("/Script/BattleXEditor"),
	Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_BXTLEditorSettings_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_BXTLEditorSettings_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
