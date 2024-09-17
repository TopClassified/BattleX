// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "BattleX/BXSettings.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeBXSettings() {}

// Begin Cross Module References
BATTLEX_API UClass* Z_Construct_UClass_UBXSettings();
BATTLEX_API UClass* Z_Construct_UClass_UBXSettings_NoRegister();
COREUOBJECT_API UClass* Z_Construct_UClass_UObject_NoRegister();
COREUOBJECT_API UClass* Z_Construct_UClass_UScriptStruct();
DEVELOPERSETTINGS_API UClass* Z_Construct_UClass_UDeveloperSettings();
ENGINE_API UEnum* Z_Construct_UEnum_Engine_ECollisionChannel();
UPackage* Z_Construct_UPackage__Script_BattleX();
// End Cross Module References

// Begin Class UBXSettings
void UBXSettings::StaticRegisterNativesUBXSettings()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UBXSettings);
UClass* Z_Construct_UClass_UBXSettings_NoRegister()
{
	return UBXSettings::StaticClass();
}
struct Z_Construct_UClass_UBXSettings_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "DisplayName", "BattleX Settings" },
		{ "IncludePath", "BXSettings.h" },
		{ "ModuleRelativePath", "BXSettings.h" },
		{ "ObjectInitializerConstructorDeclared", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_CollisionChannel_MetaData[] = {
		{ "Category", "Collision" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe5\x8f\x97\xe5\x87\xbb\xe7\xb1\xbb\xe5\x9e\x8b\xe7\xb1\xbb\xe5\x9e\x8b\n" },
#endif
		{ "ModuleRelativePath", "BXSettings.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe5\x8f\x97\xe5\x87\xbb\xe7\xb1\xbb\xe5\x9e\x8b\xe7\xb1\xbb\xe5\x9e\x8b" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_CollisionDebugDuration_MetaData[] = {
		{ "Category", "Collision" },
		{ "ClampMin", "0.1" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe7\xa2\xb0\xe6\x92\x9e""Debug\xe6\x97\xb6\xe9\x95\xbf\n" },
#endif
		{ "ModuleRelativePath", "BXSettings.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe7\xa2\xb0\xe6\x92\x9e""Debug\xe6\x97\xb6\xe9\x95\xbf" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_TaskProcessorMap_MetaData[] = {
		{ "Category", "Task" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe4\xbb\xbb\xe5\x8a\xa1\xe5\xa4\x84\xe7\x90\x86\xe5\x99\xa8\xe6\x98\xa0\xe5\xb0\x84\xe8\xa1\xa8\n" },
#endif
		{ "ModuleRelativePath", "BXSettings.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe4\xbb\xbb\xe5\x8a\xa1\xe5\xa4\x84\xe7\x90\x86\xe5\x99\xa8\xe6\x98\xa0\xe5\xb0\x84\xe8\xa1\xa8" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_TaskCustomDataMap_MetaData[] = {
		{ "Category", "Task" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe4\xbb\xbb\xe5\x8a\xa1\xe8\x87\xaa\xe5\xae\x9a\xe4\xb9\x89\xe6\x95\xb0\xe6\x8d\xae\xe9\x9b\x86\xe6\x98\xa0\xe5\xb0\x84\xe8\xa1\xa8\n" },
#endif
		{ "ModuleRelativePath", "BXSettings.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe4\xbb\xbb\xe5\x8a\xa1\xe8\x87\xaa\xe5\xae\x9a\xe4\xb9\x89\xe6\x95\xb0\xe6\x8d\xae\xe9\x9b\x86\xe6\x98\xa0\xe5\xb0\x84\xe8\xa1\xa8" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_FramePendingTaskLimit_MetaData[] = {
		{ "Category", "Task" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe5\xa4\x84\xe7\x90\x86\xe5\xbe\x85\xe6\x89\xa7\xe8\xa1\x8c\xe4\xbb\xbb\xe5\x8a\xa1\xe7\x9a\x84\xe6\xb7\xb1\xe5\xba\xa6\n" },
#endif
		{ "ModuleRelativePath", "BXSettings.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe5\xa4\x84\xe7\x90\x86\xe5\xbe\x85\xe6\x89\xa7\xe8\xa1\x8c\xe4\xbb\xbb\xe5\x8a\xa1\xe7\x9a\x84\xe6\xb7\xb1\xe5\xba\xa6" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FBytePropertyParams NewProp_CollisionChannel;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_CollisionDebugDuration;
	static const UECodeGen_Private::FSoftClassPropertyParams NewProp_TaskProcessorMap_ValueProp;
	static const UECodeGen_Private::FSoftClassPropertyParams NewProp_TaskProcessorMap_Key_KeyProp;
	static const UECodeGen_Private::FMapPropertyParams NewProp_TaskProcessorMap;
	static const UECodeGen_Private::FSoftObjectPropertyParams NewProp_TaskCustomDataMap_ValueProp;
	static const UECodeGen_Private::FSoftClassPropertyParams NewProp_TaskCustomDataMap_Key_KeyProp;
	static const UECodeGen_Private::FMapPropertyParams NewProp_TaskCustomDataMap;
	static const UECodeGen_Private::FIntPropertyParams NewProp_FramePendingTaskLimit;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UBXSettings>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FBytePropertyParams Z_Construct_UClass_UBXSettings_Statics::NewProp_CollisionChannel = { "CollisionChannel", nullptr, (EPropertyFlags)0x0010000000014001, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXSettings, CollisionChannel), Z_Construct_UEnum_Engine_ECollisionChannel, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_CollisionChannel_MetaData), NewProp_CollisionChannel_MetaData) }; // 756624936
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UBXSettings_Statics::NewProp_CollisionDebugDuration = { "CollisionDebugDuration", nullptr, (EPropertyFlags)0x0010000000014001, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXSettings, CollisionDebugDuration), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_CollisionDebugDuration_MetaData), NewProp_CollisionDebugDuration_MetaData) };
const UECodeGen_Private::FSoftClassPropertyParams Z_Construct_UClass_UBXSettings_Statics::NewProp_TaskProcessorMap_ValueProp = { "TaskProcessorMap", nullptr, (EPropertyFlags)0x0004000000004001, UECodeGen_Private::EPropertyGenFlags::SoftClass, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 1, Z_Construct_UClass_UObject_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FSoftClassPropertyParams Z_Construct_UClass_UBXSettings_Statics::NewProp_TaskProcessorMap_Key_KeyProp = { "TaskProcessorMap_Key", nullptr, (EPropertyFlags)0x0004000000004001, UECodeGen_Private::EPropertyGenFlags::SoftClass, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UClass_UObject_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FMapPropertyParams Z_Construct_UClass_UBXSettings_Statics::NewProp_TaskProcessorMap = { "TaskProcessorMap", nullptr, (EPropertyFlags)0x0014000000014001, UECodeGen_Private::EPropertyGenFlags::Map, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXSettings, TaskProcessorMap), EMapPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_TaskProcessorMap_MetaData), NewProp_TaskProcessorMap_MetaData) };
const UECodeGen_Private::FSoftObjectPropertyParams Z_Construct_UClass_UBXSettings_Statics::NewProp_TaskCustomDataMap_ValueProp = { "TaskCustomDataMap", nullptr, (EPropertyFlags)0x0004000000004001, UECodeGen_Private::EPropertyGenFlags::SoftObject, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 1, Z_Construct_UClass_UScriptStruct, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FSoftClassPropertyParams Z_Construct_UClass_UBXSettings_Statics::NewProp_TaskCustomDataMap_Key_KeyProp = { "TaskCustomDataMap_Key", nullptr, (EPropertyFlags)0x0004000000004001, UECodeGen_Private::EPropertyGenFlags::SoftClass, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UClass_UObject_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FMapPropertyParams Z_Construct_UClass_UBXSettings_Statics::NewProp_TaskCustomDataMap = { "TaskCustomDataMap", nullptr, (EPropertyFlags)0x0014000000014001, UECodeGen_Private::EPropertyGenFlags::Map, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXSettings, TaskCustomDataMap), EMapPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_TaskCustomDataMap_MetaData), NewProp_TaskCustomDataMap_MetaData) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UClass_UBXSettings_Statics::NewProp_FramePendingTaskLimit = { "FramePendingTaskLimit", nullptr, (EPropertyFlags)0x0010000000014001, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXSettings, FramePendingTaskLimit), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_FramePendingTaskLimit_MetaData), NewProp_FramePendingTaskLimit_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UBXSettings_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXSettings_Statics::NewProp_CollisionChannel,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXSettings_Statics::NewProp_CollisionDebugDuration,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXSettings_Statics::NewProp_TaskProcessorMap_ValueProp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXSettings_Statics::NewProp_TaskProcessorMap_Key_KeyProp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXSettings_Statics::NewProp_TaskProcessorMap,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXSettings_Statics::NewProp_TaskCustomDataMap_ValueProp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXSettings_Statics::NewProp_TaskCustomDataMap_Key_KeyProp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXSettings_Statics::NewProp_TaskCustomDataMap,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXSettings_Statics::NewProp_FramePendingTaskLimit,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UBXSettings_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_UBXSettings_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UDeveloperSettings,
	(UObject* (*)())Z_Construct_UPackage__Script_BattleX,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UBXSettings_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UBXSettings_Statics::ClassParams = {
	&UBXSettings::StaticClass,
	"Game",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_UBXSettings_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_UBXSettings_Statics::PropPointers),
	0,
	0x000000A6u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UBXSettings_Statics::Class_MetaDataParams), Z_Construct_UClass_UBXSettings_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UBXSettings()
{
	if (!Z_Registration_Info_UClass_UBXSettings.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UBXSettings.OuterSingleton, Z_Construct_UClass_UBXSettings_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UBXSettings.OuterSingleton;
}
template<> BATTLEX_API UClass* StaticClass<UBXSettings>()
{
	return UBXSettings::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(UBXSettings);
UBXSettings::~UBXSettings() {}
// End Class UBXSettings

// Begin Registration
struct Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_BXSettings_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UBXSettings, UBXSettings::StaticClass, TEXT("UBXSettings"), &Z_Registration_Info_UClass_UBXSettings, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UBXSettings), 2573613630U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_BXSettings_h_682263457(TEXT("/Script/BattleX"),
	Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_BXSettings_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_BXSettings_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
