// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "BattleXEditor/TimelineEditor/BXTLEditorTemplate.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeBXTLEditorTemplate() {}

// Begin Cross Module References
BATTLEX_API UClass* Z_Construct_UClass_UBXTask_NoRegister();
BATTLEXEDITOR_API UClass* Z_Construct_UClass_UBXTLTaskGroupTemplate();
BATTLEXEDITOR_API UClass* Z_Construct_UClass_UBXTLTaskGroupTemplate_NoRegister();
BATTLEXEDITOR_API UScriptStruct* Z_Construct_UScriptStruct_FBXTLTaskGroupInformation();
ENGINE_API UClass* Z_Construct_UClass_UPrimaryDataAsset();
UPackage* Z_Construct_UPackage__Script_BattleXEditor();
// End Cross Module References

// Begin ScriptStruct FBXTLTaskGroupInformation
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_BXTLTaskGroupInformation;
class UScriptStruct* FBXTLTaskGroupInformation::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_BXTLTaskGroupInformation.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_BXTLTaskGroupInformation.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FBXTLTaskGroupInformation, (UObject*)Z_Construct_UPackage__Script_BattleXEditor(), TEXT("BXTLTaskGroupInformation"));
	}
	return Z_Registration_Info_UScriptStruct_BXTLTaskGroupInformation.OuterSingleton;
}
template<> BATTLEXEDITOR_API UScriptStruct* StaticStruct<FBXTLTaskGroupInformation>()
{
	return FBXTLTaskGroupInformation::StaticStruct();
}
struct Z_Construct_UScriptStruct_FBXTLTaskGroupInformation_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "ModuleRelativePath", "TimelineEditor/BXTLEditorTemplate.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Name_MetaData[] = {
		{ "Category", "BXTLTaskGroupInformation" },
		{ "ModuleRelativePath", "TimelineEditor/BXTLEditorTemplate.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_TaskList_MetaData[] = {
		{ "Category", "BXTLTaskGroupInformation" },
		{ "ModuleRelativePath", "TimelineEditor/BXTLEditorTemplate.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FNamePropertyParams NewProp_Name;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_TaskList_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_TaskList;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FBXTLTaskGroupInformation>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
};
const UECodeGen_Private::FNamePropertyParams Z_Construct_UScriptStruct_FBXTLTaskGroupInformation_Statics::NewProp_Name = { "Name", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTLTaskGroupInformation, Name), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Name_MetaData), NewProp_Name_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UScriptStruct_FBXTLTaskGroupInformation_Statics::NewProp_TaskList_Inner = { "TaskList", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UClass_UBXTask_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FBXTLTaskGroupInformation_Statics::NewProp_TaskList = { "TaskList", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTLTaskGroupInformation, TaskList), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_TaskList_MetaData), NewProp_TaskList_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FBXTLTaskGroupInformation_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLTaskGroupInformation_Statics::NewProp_Name,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLTaskGroupInformation_Statics::NewProp_TaskList_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLTaskGroupInformation_Statics::NewProp_TaskList,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXTLTaskGroupInformation_Statics::PropPointers) < 2048);
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FBXTLTaskGroupInformation_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_BattleXEditor,
	nullptr,
	&NewStructOps,
	"BXTLTaskGroupInformation",
	Z_Construct_UScriptStruct_FBXTLTaskGroupInformation_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXTLTaskGroupInformation_Statics::PropPointers),
	sizeof(FBXTLTaskGroupInformation),
	alignof(FBXTLTaskGroupInformation),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000001),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXTLTaskGroupInformation_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FBXTLTaskGroupInformation_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FBXTLTaskGroupInformation()
{
	if (!Z_Registration_Info_UScriptStruct_BXTLTaskGroupInformation.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_BXTLTaskGroupInformation.InnerSingleton, Z_Construct_UScriptStruct_FBXTLTaskGroupInformation_Statics::StructParams);
	}
	return Z_Registration_Info_UScriptStruct_BXTLTaskGroupInformation.InnerSingleton;
}
// End ScriptStruct FBXTLTaskGroupInformation

// Begin Class UBXTLTaskGroupTemplate
void UBXTLTaskGroupTemplate::StaticRegisterNativesUBXTLTaskGroupTemplate()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UBXTLTaskGroupTemplate);
UClass* Z_Construct_UClass_UBXTLTaskGroupTemplate_NoRegister()
{
	return UBXTLTaskGroupTemplate::StaticClass();
}
struct Z_Construct_UClass_UBXTLTaskGroupTemplate_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "IncludePath", "TimelineEditor/BXTLEditorTemplate.h" },
		{ "IsBlueprintBase", "true" },
		{ "ModuleRelativePath", "TimelineEditor/BXTLEditorTemplate.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Groups_MetaData[] = {
		{ "Category", "BXTLTaskGroupTemplate" },
		{ "ModuleRelativePath", "TimelineEditor/BXTLEditorTemplate.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_Groups_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_Groups;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UBXTLTaskGroupTemplate>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UBXTLTaskGroupTemplate_Statics::NewProp_Groups_Inner = { "Groups", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UScriptStruct_FBXTLTaskGroupInformation, METADATA_PARAMS(0, nullptr) }; // 315647319
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UClass_UBXTLTaskGroupTemplate_Statics::NewProp_Groups = { "Groups", nullptr, (EPropertyFlags)0x0010000000010001, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXTLTaskGroupTemplate, Groups), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Groups_MetaData), NewProp_Groups_MetaData) }; // 315647319
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UBXTLTaskGroupTemplate_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXTLTaskGroupTemplate_Statics::NewProp_Groups_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXTLTaskGroupTemplate_Statics::NewProp_Groups,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UBXTLTaskGroupTemplate_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_UBXTLTaskGroupTemplate_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UPrimaryDataAsset,
	(UObject* (*)())Z_Construct_UPackage__Script_BattleXEditor,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UBXTLTaskGroupTemplate_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UBXTLTaskGroupTemplate_Statics::ClassParams = {
	&UBXTLTaskGroupTemplate::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_UBXTLTaskGroupTemplate_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_UBXTLTaskGroupTemplate_Statics::PropPointers),
	0,
	0x000000A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UBXTLTaskGroupTemplate_Statics::Class_MetaDataParams), Z_Construct_UClass_UBXTLTaskGroupTemplate_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UBXTLTaskGroupTemplate()
{
	if (!Z_Registration_Info_UClass_UBXTLTaskGroupTemplate.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UBXTLTaskGroupTemplate.OuterSingleton, Z_Construct_UClass_UBXTLTaskGroupTemplate_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UBXTLTaskGroupTemplate.OuterSingleton;
}
template<> BATTLEXEDITOR_API UClass* StaticClass<UBXTLTaskGroupTemplate>()
{
	return UBXTLTaskGroupTemplate::StaticClass();
}
UBXTLTaskGroupTemplate::UBXTLTaskGroupTemplate(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UBXTLTaskGroupTemplate);
UBXTLTaskGroupTemplate::~UBXTLTaskGroupTemplate() {}
// End Class UBXTLTaskGroupTemplate

// Begin Registration
struct Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_BXTLEditorTemplate_h_Statics
{
	static constexpr FStructRegisterCompiledInInfo ScriptStructInfo[] = {
		{ FBXTLTaskGroupInformation::StaticStruct, Z_Construct_UScriptStruct_FBXTLTaskGroupInformation_Statics::NewStructOps, TEXT("BXTLTaskGroupInformation"), &Z_Registration_Info_UScriptStruct_BXTLTaskGroupInformation, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FBXTLTaskGroupInformation), 315647319U) },
	};
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UBXTLTaskGroupTemplate, UBXTLTaskGroupTemplate::StaticClass, TEXT("UBXTLTaskGroupTemplate"), &Z_Registration_Info_UClass_UBXTLTaskGroupTemplate, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UBXTLTaskGroupTemplate), 6520953U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_BXTLEditorTemplate_h_921572300(TEXT("/Script/BattleXEditor"),
	Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_BXTLEditorTemplate_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_BXTLEditorTemplate_h_Statics::ClassInfo),
	Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_BXTLEditorTemplate_h_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_BXTLEditorTemplate_h_Statics::ScriptStructInfo),
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
