// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "BattleXEditor/TimelineEditor/BXTLAssetThumbnailRenderer.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeBXTLAssetThumbnailRenderer() {}

// Begin Cross Module References
BATTLEXEDITOR_API UClass* Z_Construct_UClass_UBXTLAssetThumbnailRenderer();
BATTLEXEDITOR_API UClass* Z_Construct_UClass_UBXTLAssetThumbnailRenderer_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_UFont_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_UTexture2D_NoRegister();
UNREALED_API UClass* Z_Construct_UClass_UDefaultSizedThumbnailRenderer();
UPackage* Z_Construct_UPackage__Script_BattleXEditor();
// End Cross Module References

// Begin Class UBXTLAssetThumbnailRenderer
void UBXTLAssetThumbnailRenderer::StaticRegisterNativesUBXTLAssetThumbnailRenderer()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UBXTLAssetThumbnailRenderer);
UClass* Z_Construct_UClass_UBXTLAssetThumbnailRenderer_NoRegister()
{
	return UBXTLAssetThumbnailRenderer::StaticClass();
}
struct Z_Construct_UClass_UBXTLAssetThumbnailRenderer_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "TimelineEditor/BXTLAssetThumbnailRenderer.h" },
		{ "ModuleRelativePath", "TimelineEditor/BXTLAssetThumbnailRenderer.h" },
		{ "ObjectInitializerConstructorDeclared", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_m_NoImage_MetaData[] = {
		{ "ModuleRelativePath", "TimelineEditor/BXTLAssetThumbnailRenderer.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_NameFont_MetaData[] = {
		{ "ModuleRelativePath", "TimelineEditor/BXTLAssetThumbnailRenderer.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_TipFont_MetaData[] = {
		{ "ModuleRelativePath", "TimelineEditor/BXTLAssetThumbnailRenderer.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_m_NoImage;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_NameFont;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_TipFont;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UBXTLAssetThumbnailRenderer>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UBXTLAssetThumbnailRenderer_Statics::NewProp_m_NoImage = { "m_NoImage", nullptr, (EPropertyFlags)0x0040000000000000, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXTLAssetThumbnailRenderer, m_NoImage), Z_Construct_UClass_UTexture2D_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_m_NoImage_MetaData), NewProp_m_NoImage_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UBXTLAssetThumbnailRenderer_Statics::NewProp_NameFont = { "NameFont", nullptr, (EPropertyFlags)0x0040000000000000, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXTLAssetThumbnailRenderer, NameFont), Z_Construct_UClass_UFont_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_NameFont_MetaData), NewProp_NameFont_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UBXTLAssetThumbnailRenderer_Statics::NewProp_TipFont = { "TipFont", nullptr, (EPropertyFlags)0x0040000000000000, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXTLAssetThumbnailRenderer, TipFont), Z_Construct_UClass_UFont_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_TipFont_MetaData), NewProp_TipFont_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UBXTLAssetThumbnailRenderer_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXTLAssetThumbnailRenderer_Statics::NewProp_m_NoImage,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXTLAssetThumbnailRenderer_Statics::NewProp_NameFont,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXTLAssetThumbnailRenderer_Statics::NewProp_TipFont,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UBXTLAssetThumbnailRenderer_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_UBXTLAssetThumbnailRenderer_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UDefaultSizedThumbnailRenderer,
	(UObject* (*)())Z_Construct_UPackage__Script_BattleXEditor,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UBXTLAssetThumbnailRenderer_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UBXTLAssetThumbnailRenderer_Statics::ClassParams = {
	&UBXTLAssetThumbnailRenderer::StaticClass,
	"Editor",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_UBXTLAssetThumbnailRenderer_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_UBXTLAssetThumbnailRenderer_Statics::PropPointers),
	0,
	0x001000A4u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UBXTLAssetThumbnailRenderer_Statics::Class_MetaDataParams), Z_Construct_UClass_UBXTLAssetThumbnailRenderer_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UBXTLAssetThumbnailRenderer()
{
	if (!Z_Registration_Info_UClass_UBXTLAssetThumbnailRenderer.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UBXTLAssetThumbnailRenderer.OuterSingleton, Z_Construct_UClass_UBXTLAssetThumbnailRenderer_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UBXTLAssetThumbnailRenderer.OuterSingleton;
}
template<> BATTLEXEDITOR_API UClass* StaticClass<UBXTLAssetThumbnailRenderer>()
{
	return UBXTLAssetThumbnailRenderer::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(UBXTLAssetThumbnailRenderer);
UBXTLAssetThumbnailRenderer::~UBXTLAssetThumbnailRenderer() {}
// End Class UBXTLAssetThumbnailRenderer

// Begin Registration
struct Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_BXTLAssetThumbnailRenderer_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UBXTLAssetThumbnailRenderer, UBXTLAssetThumbnailRenderer::StaticClass, TEXT("UBXTLAssetThumbnailRenderer"), &Z_Registration_Info_UClass_UBXTLAssetThumbnailRenderer, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UBXTLAssetThumbnailRenderer), 2755162064U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_BXTLAssetThumbnailRenderer_h_2455054627(TEXT("/Script/BattleXEditor"),
	Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_BXTLAssetThumbnailRenderer_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_BXTLAssetThumbnailRenderer_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
