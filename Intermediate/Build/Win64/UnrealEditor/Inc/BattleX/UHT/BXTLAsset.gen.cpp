// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "BattleX/Timeline/BXTLAsset.h"
#include "BattleX/Timeline/BXTLStructs.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeBXTLAsset() {}

// Begin Cross Module References
BATTLEX_API UClass* Z_Construct_UClass_UBXTask_NoRegister();
BATTLEX_API UClass* Z_Construct_UClass_UBXTLAsset();
BATTLEX_API UClass* Z_Construct_UClass_UBXTLAsset_NoRegister();
BATTLEX_API UEnum* Z_Construct_UEnum_BattleX_EBXTLExtraType();
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXTLKeyFrame();
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXTLSection();
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXTLTaskGroup();
COREUOBJECT_API UClass* Z_Construct_UClass_UObject_NoRegister();
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FSoftObjectPath();
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FVector2f();
ENGINE_API UClass* Z_Construct_UClass_UEdGraph_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_UPrimaryDataAsset();
UPackage* Z_Construct_UPackage__Script_BattleX();
// End Cross Module References

// Begin ScriptStruct FBXTLTaskGroup
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_BXTLTaskGroup;
class UScriptStruct* FBXTLTaskGroup::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_BXTLTaskGroup.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_BXTLTaskGroup.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FBXTLTaskGroup, (UObject*)Z_Construct_UPackage__Script_BattleX(), TEXT("BXTLTaskGroup"));
	}
	return Z_Registration_Info_UScriptStruct_BXTLTaskGroup.OuterSingleton;
}
template<> BATTLEX_API UScriptStruct* StaticStruct<FBXTLTaskGroup>()
{
	return FBXTLTaskGroup::StaticStruct();
}
struct Z_Construct_UScriptStruct_FBXTLTaskGroup_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "ModuleRelativePath", "Timeline/BXTLAsset.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Name_MetaData[] = {
		{ "ModuleRelativePath", "Timeline/BXTLAsset.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_TaskList_MetaData[] = {
		{ "ModuleRelativePath", "Timeline/BXTLAsset.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FTextPropertyParams NewProp_Name;
	static const UECodeGen_Private::FWeakObjectPropertyParams NewProp_TaskList_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_TaskList;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FBXTLTaskGroup>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
};
const UECodeGen_Private::FTextPropertyParams Z_Construct_UScriptStruct_FBXTLTaskGroup_Statics::NewProp_Name = { "Name", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Text, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTLTaskGroup, Name), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Name_MetaData), NewProp_Name_MetaData) };
const UECodeGen_Private::FWeakObjectPropertyParams Z_Construct_UScriptStruct_FBXTLTaskGroup_Statics::NewProp_TaskList_Inner = { "TaskList", nullptr, (EPropertyFlags)0x0004000000000000, UECodeGen_Private::EPropertyGenFlags::WeakObject, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UClass_UBXTask_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FBXTLTaskGroup_Statics::NewProp_TaskList = { "TaskList", nullptr, (EPropertyFlags)0x0014000000000000, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTLTaskGroup, TaskList), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_TaskList_MetaData), NewProp_TaskList_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FBXTLTaskGroup_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLTaskGroup_Statics::NewProp_Name,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLTaskGroup_Statics::NewProp_TaskList_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLTaskGroup_Statics::NewProp_TaskList,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXTLTaskGroup_Statics::PropPointers) < 2048);
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FBXTLTaskGroup_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_BattleX,
	nullptr,
	&NewStructOps,
	"BXTLTaskGroup",
	Z_Construct_UScriptStruct_FBXTLTaskGroup_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXTLTaskGroup_Statics::PropPointers),
	sizeof(FBXTLTaskGroup),
	alignof(FBXTLTaskGroup),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000001),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXTLTaskGroup_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FBXTLTaskGroup_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FBXTLTaskGroup()
{
	if (!Z_Registration_Info_UScriptStruct_BXTLTaskGroup.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_BXTLTaskGroup.InnerSingleton, Z_Construct_UScriptStruct_FBXTLTaskGroup_Statics::StructParams);
	}
	return Z_Registration_Info_UScriptStruct_BXTLTaskGroup.InnerSingleton;
}
// End ScriptStruct FBXTLTaskGroup

// Begin ScriptStruct FBXTLSection
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_BXTLSection;
class UScriptStruct* FBXTLSection::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_BXTLSection.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_BXTLSection.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FBXTLSection, (UObject*)Z_Construct_UPackage__Script_BattleX(), TEXT("BXTLSection"));
	}
	return Z_Registration_Info_UScriptStruct_BXTLSection.OuterSingleton;
}
template<> BATTLEX_API UScriptStruct* StaticStruct<FBXTLSection>()
{
	return FBXTLSection::StaticStruct();
}
struct Z_Construct_UScriptStruct_FBXTLSection_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Timeline/BXTLAsset.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Duration_MetaData[] = {
		{ "Category", "BXTLSection" },
		{ "ClampMin", "0.5" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe6\x8c\x81\xe7\xbb\xad\xe6\x97\xb6\xe9\x97\xb4\n" },
#endif
		{ "ModuleRelativePath", "Timeline/BXTLAsset.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe6\x8c\x81\xe7\xbb\xad\xe6\x97\xb6\xe9\x97\xb4" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LoopTime_MetaData[] = {
		{ "Category", "BXTLSection" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe5\xbe\xaa\xe7\x8e\xaf\xe6\xac\xa1\xe6\x95\xb0(\xe5\xb0\x8f\xe4\xba\x8e\xe7\xad\x89\xe4\xba\x8e""0\xe4\xbb\xa3\xe8\xa1\xa8\xe6\x97\xa0\xe9\x99\x90\xe5\xbe\xaa\xe7\x8e\xaf)\n" },
#endif
		{ "ModuleRelativePath", "Timeline/BXTLAsset.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe5\xbe\xaa\xe7\x8e\xaf\xe6\xac\xa1\xe6\x95\xb0(\xe5\xb0\x8f\xe4\xba\x8e\xe7\xad\x89\xe4\xba\x8e""0\xe4\xbb\xa3\xe8\xa1\xa8\xe6\x97\xa0\xe9\x99\x90\xe5\xbe\xaa\xe7\x8e\xaf)" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ExtraTime_MetaData[] = {
		{ "Category", "BXTLSection" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe9\xa2\x9d\xe5\xa4\x96\xe7\x9a\x84\xe6\x97\xb6\xe9\x97\xb4\xe4\xbf\xa1\xe6\x81\xaf\n" },
#endif
		{ "ModuleRelativePath", "Timeline/BXTLAsset.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe9\xa2\x9d\xe5\xa4\x96\xe7\x9a\x84\xe6\x97\xb6\xe9\x97\xb4\xe4\xbf\xa1\xe6\x81\xaf" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_NextIndex_MetaData[] = {
		{ "Category", "BXTLSection" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe4\xb8\x8b\xe4\xb8\x80\xe4\xb8\xaa\xe6\x97\xb6\xe9\x97\xb4\xe8\xbd\xb4\xe7\xb4\xa2\xe5\xbc\x95(\xe5\xa4\xa7\xe4\xba\x8e\xe7\xad\x89\xe4\xba\x8e""0\xe8\xb5\xb7\xe6\x95\x88)\n" },
#endif
		{ "ModuleRelativePath", "Timeline/BXTLAsset.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe4\xb8\x8b\xe4\xb8\x80\xe4\xb8\xaa\xe6\x97\xb6\xe9\x97\xb4\xe8\xbd\xb4\xe7\xb4\xa2\xe5\xbc\x95(\xe5\xa4\xa7\xe4\xba\x8e\xe7\xad\x89\xe4\xba\x8e""0\xe8\xb5\xb7\xe6\x95\x88)" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_TaskList_Inner_MetaData[] = {
		{ "Category", "BXTLSection" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Task\xe5\x88\x97\xe8\xa1\xa8\n" },
#endif
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Timeline/BXTLAsset.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Task\xe5\x88\x97\xe8\xa1\xa8" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_TaskList_MetaData[] = {
		{ "Category", "BXTLSection" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Task\xe5\x88\x97\xe8\xa1\xa8\n" },
#endif
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Timeline/BXTLAsset.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Task\xe5\x88\x97\xe8\xa1\xa8" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_KeyFrames_MetaData[] = {
		{ "Category", "BXTLSection" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe5\x85\xb3\xe9\x94\xae\xe5\xb8\xa7\xe5\x88\x97\xe8\xa1\xa8\n" },
#endif
		{ "ModuleRelativePath", "Timeline/BXTLAsset.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe5\x85\xb3\xe9\x94\xae\xe5\xb8\xa7\xe5\x88\x97\xe8\xa1\xa8" },
#endif
	};
#if WITH_EDITORONLY_DATA
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Name_MetaData[] = {
		{ "Category", "BXTLSection" },
		{ "ModuleRelativePath", "Timeline/BXTLAsset.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Groups_MetaData[] = {
		{ "ModuleRelativePath", "Timeline/BXTLAsset.h" },
	};
#endif // WITH_EDITORONLY_DATA
#endif // WITH_METADATA
	static const UECodeGen_Private::FFloatPropertyParams NewProp_Duration;
	static const UECodeGen_Private::FIntPropertyParams NewProp_LoopTime;
	static const UECodeGen_Private::FStructPropertyParams NewProp_ExtraTime_ValueProp;
	static const UECodeGen_Private::FBytePropertyParams NewProp_ExtraTime_Key_KeyProp_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_ExtraTime_Key_KeyProp;
	static const UECodeGen_Private::FMapPropertyParams NewProp_ExtraTime;
	static const UECodeGen_Private::FIntPropertyParams NewProp_NextIndex;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_TaskList_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_TaskList;
	static const UECodeGen_Private::FStructPropertyParams NewProp_KeyFrames_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_KeyFrames;
#if WITH_EDITORONLY_DATA
	static const UECodeGen_Private::FTextPropertyParams NewProp_Name;
	static const UECodeGen_Private::FStructPropertyParams NewProp_Groups_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_Groups;
#endif // WITH_EDITORONLY_DATA
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FBXTLSection>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
};
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FBXTLSection_Statics::NewProp_Duration = { "Duration", nullptr, (EPropertyFlags)0x0010000000010015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTLSection, Duration), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Duration_MetaData), NewProp_Duration_MetaData) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UScriptStruct_FBXTLSection_Statics::NewProp_LoopTime = { "LoopTime", nullptr, (EPropertyFlags)0x0010000000010015, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTLSection, LoopTime), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LoopTime_MetaData), NewProp_LoopTime_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FBXTLSection_Statics::NewProp_ExtraTime_ValueProp = { "ExtraTime", nullptr, (EPropertyFlags)0x0000000000000001, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 1, Z_Construct_UScriptStruct_FVector2f, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FBytePropertyParams Z_Construct_UScriptStruct_FBXTLSection_Statics::NewProp_ExtraTime_Key_KeyProp_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UScriptStruct_FBXTLSection_Statics::NewProp_ExtraTime_Key_KeyProp = { "ExtraTime_Key", nullptr, (EPropertyFlags)0x0000000000000001, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UEnum_BattleX_EBXTLExtraType, METADATA_PARAMS(0, nullptr) }; // 737436919
const UECodeGen_Private::FMapPropertyParams Z_Construct_UScriptStruct_FBXTLSection_Statics::NewProp_ExtraTime = { "ExtraTime", nullptr, (EPropertyFlags)0x0010000000010015, UECodeGen_Private::EPropertyGenFlags::Map, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTLSection, ExtraTime), EMapPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ExtraTime_MetaData), NewProp_ExtraTime_MetaData) }; // 737436919
const UECodeGen_Private::FIntPropertyParams Z_Construct_UScriptStruct_FBXTLSection_Statics::NewProp_NextIndex = { "NextIndex", nullptr, (EPropertyFlags)0x0010000000010015, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTLSection, NextIndex), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_NextIndex_MetaData), NewProp_NextIndex_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UScriptStruct_FBXTLSection_Statics::NewProp_TaskList_Inner = { "TaskList", nullptr, (EPropertyFlags)0x0002000000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UClass_UBXTask_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_TaskList_Inner_MetaData), NewProp_TaskList_Inner_MetaData) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FBXTLSection_Statics::NewProp_TaskList = { "TaskList", nullptr, (EPropertyFlags)0x001000800001001d, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTLSection, TaskList), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_TaskList_MetaData), NewProp_TaskList_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FBXTLSection_Statics::NewProp_KeyFrames_Inner = { "KeyFrames", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UScriptStruct_FBXTLKeyFrame, METADATA_PARAMS(0, nullptr) }; // 2356335277
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FBXTLSection_Statics::NewProp_KeyFrames = { "KeyFrames", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTLSection, KeyFrames), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_KeyFrames_MetaData), NewProp_KeyFrames_MetaData) }; // 2356335277
#if WITH_EDITORONLY_DATA
const UECodeGen_Private::FTextPropertyParams Z_Construct_UScriptStruct_FBXTLSection_Statics::NewProp_Name = { "Name", nullptr, (EPropertyFlags)0x0010000800010001, UECodeGen_Private::EPropertyGenFlags::Text, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTLSection, Name), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Name_MetaData), NewProp_Name_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FBXTLSection_Statics::NewProp_Groups_Inner = { "Groups", nullptr, (EPropertyFlags)0x0000000800000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UScriptStruct_FBXTLTaskGroup, METADATA_PARAMS(0, nullptr) }; // 1190820354
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FBXTLSection_Statics::NewProp_Groups = { "Groups", nullptr, (EPropertyFlags)0x0010000800000000, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTLSection, Groups), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Groups_MetaData), NewProp_Groups_MetaData) }; // 1190820354
#endif // WITH_EDITORONLY_DATA
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FBXTLSection_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLSection_Statics::NewProp_Duration,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLSection_Statics::NewProp_LoopTime,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLSection_Statics::NewProp_ExtraTime_ValueProp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLSection_Statics::NewProp_ExtraTime_Key_KeyProp_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLSection_Statics::NewProp_ExtraTime_Key_KeyProp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLSection_Statics::NewProp_ExtraTime,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLSection_Statics::NewProp_NextIndex,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLSection_Statics::NewProp_TaskList_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLSection_Statics::NewProp_TaskList,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLSection_Statics::NewProp_KeyFrames_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLSection_Statics::NewProp_KeyFrames,
#if WITH_EDITORONLY_DATA
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLSection_Statics::NewProp_Name,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLSection_Statics::NewProp_Groups_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLSection_Statics::NewProp_Groups,
#endif // WITH_EDITORONLY_DATA
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXTLSection_Statics::PropPointers) < 2048);
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FBXTLSection_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_BattleX,
	nullptr,
	&NewStructOps,
	"BXTLSection",
	Z_Construct_UScriptStruct_FBXTLSection_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXTLSection_Statics::PropPointers),
	sizeof(FBXTLSection),
	alignof(FBXTLSection),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000005),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXTLSection_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FBXTLSection_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FBXTLSection()
{
	if (!Z_Registration_Info_UScriptStruct_BXTLSection.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_BXTLSection.InnerSingleton, Z_Construct_UScriptStruct_FBXTLSection_Statics::StructParams);
	}
	return Z_Registration_Info_UScriptStruct_BXTLSection.InnerSingleton;
}
// End ScriptStruct FBXTLSection

// Begin Class UBXTLAsset
void UBXTLAsset::StaticRegisterNativesUBXTLAsset()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UBXTLAsset);
UClass* Z_Construct_UClass_UBXTLAsset_NoRegister()
{
	return UBXTLAsset::StaticClass();
}
struct Z_Construct_UClass_UBXTLAsset_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "IncludePath", "Timeline/BXTLAsset.h" },
		{ "IsBlueprintBase", "true" },
		{ "ModuleRelativePath", "Timeline/BXTLAsset.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ID_MetaData[] = {
		{ "Category", "Important" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe8\xb5\x84\xe6\xba\x90ID\n" },
#endif
		{ "ModuleRelativePath", "Timeline/BXTLAsset.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe8\xb5\x84\xe6\xba\x90ID" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Sections_MetaData[] = {
		{ "Category", "Important" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe8\xbf\x90\xe8\xa1\x8c\xe7\x89\x87\xe6\xae\xb5\n" },
#endif
		{ "ModuleRelativePath", "Timeline/BXTLAsset.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe8\xbf\x90\xe8\xa1\x8c\xe7\x89\x87\xe6\xae\xb5" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_StartSectionIndexes_MetaData[] = {
		{ "Category", "Important" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe5\xbc\x80\xe5\xa7\x8b\xe6\x97\xb6\xe6\x89\xa7\xe8\xa1\x8c\xe5\x93\xaa\xe4\xba\x9b\xe6\x97\xb6\xe9\x97\xb4\xe7\x89\x87\xe6\xae\xb5\n" },
#endif
		{ "ModuleRelativePath", "Timeline/BXTLAsset.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe5\xbc\x80\xe5\xa7\x8b\xe6\x97\xb6\xe6\x89\xa7\xe8\xa1\x8c\xe5\x93\xaa\xe4\xba\x9b\xe6\x97\xb6\xe9\x97\xb4\xe7\x89\x87\xe6\xae\xb5" },
#endif
	};
#if WITH_EDITORONLY_DATA
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Graph_MetaData[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe9\x80\xbb\xe8\xbe\x91\xe5\x9b\xbe\n" },
#endif
		{ "ModuleRelativePath", "Timeline/BXTLAsset.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe9\x80\xbb\xe8\xbe\x91\xe5\x9b\xbe" },
#endif
	};
#endif // WITH_EDITORONLY_DATA
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_AllResources_MetaData[] = {
		{ "Category", "Resource" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe6\x89\x80\xe6\x9c\x89\xe5\xbc\x95\xe7\x94\xa8\xe7\x9a\x84\xe8\xb5\x84\xe6\xba\x90\xe8\xb7\xaf\xe5\xbe\x84\n" },
#endif
		{ "ModuleRelativePath", "Timeline/BXTLAsset.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe6\x89\x80\xe6\x9c\x89\xe5\xbc\x95\xe7\x94\xa8\xe7\x9a\x84\xe8\xb5\x84\xe6\xba\x90\xe8\xb7\xaf\xe5\xbe\x84" },
#endif
	};
#if WITH_EDITORONLY_DATA
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Name_MetaData[] = {
		{ "Category", "Edit" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe5\x90\x8d\xe7\xa7\xb0\n" },
#endif
		{ "ModuleRelativePath", "Timeline/BXTLAsset.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe5\x90\x8d\xe7\xa7\xb0" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Tips_MetaData[] = {
		{ "Category", "Edit" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe8\xaf\xb4\xe6\x98\x8e\n" },
#endif
		{ "ModuleRelativePath", "Timeline/BXTLAsset.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe8\xaf\xb4\xe6\x98\x8e" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_PlayerInformation_MetaData[] = {
		{ "Category", "Edit" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe9\xa2\x84\xe8\xa7\x88\xe8\xa7\x92\xe8\x89\xb2\n" },
#endif
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Timeline/BXTLAsset.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe9\xa2\x84\xe8\xa7\x88\xe8\xa7\x92\xe8\x89\xb2" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_TargetInformation_MetaData[] = {
		{ "Category", "Edit" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe9\xa2\x84\xe8\xa7\x88\xe7\x9b\xae\xe6\xa0\x87\n" },
#endif
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Timeline/BXTLAsset.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe9\xa2\x84\xe8\xa7\x88\xe7\x9b\xae\xe6\xa0\x87" },
#endif
	};
#endif // WITH_EDITORONLY_DATA
#endif // WITH_METADATA
	static const UECodeGen_Private::FIntPropertyParams NewProp_ID;
	static const UECodeGen_Private::FStructPropertyParams NewProp_Sections_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_Sections;
	static const UECodeGen_Private::FIntPropertyParams NewProp_StartSectionIndexes_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_StartSectionIndexes;
#if WITH_EDITORONLY_DATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Graph;
#endif // WITH_EDITORONLY_DATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_AllResources_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_AllResources;
#if WITH_EDITORONLY_DATA
	static const UECodeGen_Private::FTextPropertyParams NewProp_Name;
	static const UECodeGen_Private::FTextPropertyParams NewProp_Tips;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_PlayerInformation;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_TargetInformation;
#endif // WITH_EDITORONLY_DATA
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UBXTLAsset>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FIntPropertyParams Z_Construct_UClass_UBXTLAsset_Statics::NewProp_ID = { "ID", nullptr, (EPropertyFlags)0x0010000000010015, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXTLAsset, ID), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ID_MetaData), NewProp_ID_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UBXTLAsset_Statics::NewProp_Sections_Inner = { "Sections", nullptr, (EPropertyFlags)0x0000008000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UScriptStruct_FBXTLSection, METADATA_PARAMS(0, nullptr) }; // 1286726932
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UClass_UBXTLAsset_Statics::NewProp_Sections = { "Sections", nullptr, (EPropertyFlags)0x0010008000010015, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXTLAsset, Sections), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Sections_MetaData), NewProp_Sections_MetaData) }; // 1286726932
const UECodeGen_Private::FIntPropertyParams Z_Construct_UClass_UBXTLAsset_Statics::NewProp_StartSectionIndexes_Inner = { "StartSectionIndexes", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UClass_UBXTLAsset_Statics::NewProp_StartSectionIndexes = { "StartSectionIndexes", nullptr, (EPropertyFlags)0x0010000000010015, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXTLAsset, StartSectionIndexes), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_StartSectionIndexes_MetaData), NewProp_StartSectionIndexes_MetaData) };
#if WITH_EDITORONLY_DATA
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UBXTLAsset_Statics::NewProp_Graph = { "Graph", nullptr, (EPropertyFlags)0x0010000800000000, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXTLAsset, Graph), Z_Construct_UClass_UEdGraph_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Graph_MetaData), NewProp_Graph_MetaData) };
#endif // WITH_EDITORONLY_DATA
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UBXTLAsset_Statics::NewProp_AllResources_Inner = { "AllResources", nullptr, (EPropertyFlags)0x0000000000020000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UScriptStruct_FSoftObjectPath, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UClass_UBXTLAsset_Statics::NewProp_AllResources = { "AllResources", nullptr, (EPropertyFlags)0x0020080000030001, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXTLAsset, AllResources), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_AllResources_MetaData), NewProp_AllResources_MetaData) };
#if WITH_EDITORONLY_DATA
const UECodeGen_Private::FTextPropertyParams Z_Construct_UClass_UBXTLAsset_Statics::NewProp_Name = { "Name", nullptr, (EPropertyFlags)0x0010000800010001, UECodeGen_Private::EPropertyGenFlags::Text, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXTLAsset, Name), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Name_MetaData), NewProp_Name_MetaData) };
const UECodeGen_Private::FTextPropertyParams Z_Construct_UClass_UBXTLAsset_Statics::NewProp_Tips = { "Tips", nullptr, (EPropertyFlags)0x0010000800010001, UECodeGen_Private::EPropertyGenFlags::Text, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXTLAsset, Tips), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Tips_MetaData), NewProp_Tips_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UBXTLAsset_Statics::NewProp_PlayerInformation = { "PlayerInformation", nullptr, (EPropertyFlags)0x0012000800090009, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXTLAsset, PlayerInformation), Z_Construct_UClass_UObject_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_PlayerInformation_MetaData), NewProp_PlayerInformation_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UBXTLAsset_Statics::NewProp_TargetInformation = { "TargetInformation", nullptr, (EPropertyFlags)0x0012000800090009, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXTLAsset, TargetInformation), Z_Construct_UClass_UObject_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_TargetInformation_MetaData), NewProp_TargetInformation_MetaData) };
#endif // WITH_EDITORONLY_DATA
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UBXTLAsset_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXTLAsset_Statics::NewProp_ID,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXTLAsset_Statics::NewProp_Sections_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXTLAsset_Statics::NewProp_Sections,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXTLAsset_Statics::NewProp_StartSectionIndexes_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXTLAsset_Statics::NewProp_StartSectionIndexes,
#if WITH_EDITORONLY_DATA
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXTLAsset_Statics::NewProp_Graph,
#endif // WITH_EDITORONLY_DATA
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXTLAsset_Statics::NewProp_AllResources_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXTLAsset_Statics::NewProp_AllResources,
#if WITH_EDITORONLY_DATA
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXTLAsset_Statics::NewProp_Name,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXTLAsset_Statics::NewProp_Tips,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXTLAsset_Statics::NewProp_PlayerInformation,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXTLAsset_Statics::NewProp_TargetInformation,
#endif // WITH_EDITORONLY_DATA
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UBXTLAsset_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_UBXTLAsset_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UPrimaryDataAsset,
	(UObject* (*)())Z_Construct_UPackage__Script_BattleX,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UBXTLAsset_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UBXTLAsset_Statics::ClassParams = {
	&UBXTLAsset::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_UBXTLAsset_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_UBXTLAsset_Statics::PropPointers),
	0,
	0x009000A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UBXTLAsset_Statics::Class_MetaDataParams), Z_Construct_UClass_UBXTLAsset_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UBXTLAsset()
{
	if (!Z_Registration_Info_UClass_UBXTLAsset.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UBXTLAsset.OuterSingleton, Z_Construct_UClass_UBXTLAsset_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UBXTLAsset.OuterSingleton;
}
template<> BATTLEX_API UClass* StaticClass<UBXTLAsset>()
{
	return UBXTLAsset::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(UBXTLAsset);
UBXTLAsset::~UBXTLAsset() {}
// End Class UBXTLAsset

// Begin Registration
struct Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Timeline_BXTLAsset_h_Statics
{
	static constexpr FStructRegisterCompiledInInfo ScriptStructInfo[] = {
		{ FBXTLTaskGroup::StaticStruct, Z_Construct_UScriptStruct_FBXTLTaskGroup_Statics::NewStructOps, TEXT("BXTLTaskGroup"), &Z_Registration_Info_UScriptStruct_BXTLTaskGroup, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FBXTLTaskGroup), 1190820354U) },
		{ FBXTLSection::StaticStruct, Z_Construct_UScriptStruct_FBXTLSection_Statics::NewStructOps, TEXT("BXTLSection"), &Z_Registration_Info_UScriptStruct_BXTLSection, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FBXTLSection), 1286726932U) },
	};
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UBXTLAsset, UBXTLAsset::StaticClass, TEXT("UBXTLAsset"), &Z_Registration_Info_UClass_UBXTLAsset, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UBXTLAsset), 2355476630U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Timeline_BXTLAsset_h_1861498487(TEXT("/Script/BattleX"),
	Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Timeline_BXTLAsset_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Timeline_BXTLAsset_h_Statics::ClassInfo),
	Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Timeline_BXTLAsset_h_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Timeline_BXTLAsset_h_Statics::ScriptStructInfo),
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
