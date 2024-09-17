// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "BattleX/Collision/BXShapeComponent.h"
#include "BattleX/BXStructs.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeBXShapeComponent() {}

// Begin Cross Module References
BATTLEX_API UClass* Z_Construct_UClass_UBXShapeComponent();
BATTLEX_API UClass* Z_Construct_UClass_UBXShapeComponent_NoRegister();
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXShapeInformation();
ENGINE_API UClass* Z_Construct_UClass_UActorComponent();
UPackage* Z_Construct_UPackage__Script_BattleX();
// End Cross Module References

// Begin Class UBXShapeComponent
void UBXShapeComponent::StaticRegisterNativesUBXShapeComponent()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UBXShapeComponent);
UClass* Z_Construct_UClass_UBXShapeComponent_NoRegister()
{
	return UBXShapeComponent::StaticClass();
}
struct Z_Construct_UClass_UBXShapeComponent_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "BlueprintSpawnableComponent", "" },
		{ "ClassGroupNames", "Custom" },
		{ "IncludePath", "Collision/BXShapeComponent.h" },
		{ "ModuleRelativePath", "Collision/BXShapeComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ShapeInformations_MetaData[] = {
		{ "Category", "BXShapeComponent" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xef\xbf\xbd\xd7\xb4\xef\xbf\xbd\xef\xbf\xbd\xcf\xa2\n" },
#endif
		{ "ModuleRelativePath", "Collision/BXShapeComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xef\xbf\xbd\xd7\xb4\xef\xbf\xbd\xef\xbf\xbd\xcf\xa2" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_ShapeInformations_ValueProp;
	static const UECodeGen_Private::FNamePropertyParams NewProp_ShapeInformations_Key_KeyProp;
	static const UECodeGen_Private::FMapPropertyParams NewProp_ShapeInformations;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UBXShapeComponent>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UBXShapeComponent_Statics::NewProp_ShapeInformations_ValueProp = { "ShapeInformations", nullptr, (EPropertyFlags)0x0000000000000001, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 1, Z_Construct_UScriptStruct_FBXShapeInformation, METADATA_PARAMS(0, nullptr) }; // 3264309202
const UECodeGen_Private::FNamePropertyParams Z_Construct_UClass_UBXShapeComponent_Statics::NewProp_ShapeInformations_Key_KeyProp = { "ShapeInformations_Key", nullptr, (EPropertyFlags)0x0000000000000001, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FMapPropertyParams Z_Construct_UClass_UBXShapeComponent_Statics::NewProp_ShapeInformations = { "ShapeInformations", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Map, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXShapeComponent, ShapeInformations), EMapPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ShapeInformations_MetaData), NewProp_ShapeInformations_MetaData) }; // 3264309202
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UBXShapeComponent_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXShapeComponent_Statics::NewProp_ShapeInformations_ValueProp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXShapeComponent_Statics::NewProp_ShapeInformations_Key_KeyProp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXShapeComponent_Statics::NewProp_ShapeInformations,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UBXShapeComponent_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_UBXShapeComponent_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UActorComponent,
	(UObject* (*)())Z_Construct_UPackage__Script_BattleX,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UBXShapeComponent_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UBXShapeComponent_Statics::ClassParams = {
	&UBXShapeComponent::StaticClass,
	"Engine",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_UBXShapeComponent_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_UBXShapeComponent_Statics::PropPointers),
	0,
	0x00B000A4u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UBXShapeComponent_Statics::Class_MetaDataParams), Z_Construct_UClass_UBXShapeComponent_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UBXShapeComponent()
{
	if (!Z_Registration_Info_UClass_UBXShapeComponent.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UBXShapeComponent.OuterSingleton, Z_Construct_UClass_UBXShapeComponent_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UBXShapeComponent.OuterSingleton;
}
template<> BATTLEX_API UClass* StaticClass<UBXShapeComponent>()
{
	return UBXShapeComponent::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(UBXShapeComponent);
UBXShapeComponent::~UBXShapeComponent() {}
// End Class UBXShapeComponent

// Begin Registration
struct Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Collision_BXShapeComponent_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UBXShapeComponent, UBXShapeComponent::StaticClass, TEXT("UBXShapeComponent"), &Z_Registration_Info_UClass_UBXShapeComponent, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UBXShapeComponent), 3701892991U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Collision_BXShapeComponent_h_3410803561(TEXT("/Script/BattleX"),
	Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Collision_BXShapeComponent_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Collision_BXShapeComponent_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
