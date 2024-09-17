// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "BattleX/Timeline/BXTLStructs.h"
#include "BattleX/BXStructs.h"
#include "StructUtils/Public/InstancedStruct.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeBXTLStructs() {}

// Begin Cross Module References
BATTLEX_API UClass* Z_Construct_UClass_UBXTask_NoRegister();
BATTLEX_API UClass* Z_Construct_UClass_UBXTLAsset_NoRegister();
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXBodyPartSelection();
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXTLBroadcastTaskInfo();
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXTLDynamicDataSearchKey();
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXTLKeyFrame();
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXTLPendingTaskInfo();
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXTLPlayContext();
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXTLPreviewObjectData();
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXTLRunTimeData();
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXTLSectionRTData();
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXTLTaskHostingData();
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXTLTaskRTData();
COREUOBJECT_API UClass* Z_Construct_UClass_UObject_NoRegister();
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FTransform();
ENGINE_API UClass* Z_Construct_UClass_AActor_NoRegister();
STRUCTUTILS_API UScriptStruct* Z_Construct_UScriptStruct_FInstancedStruct();
UPackage* Z_Construct_UPackage__Script_BattleX();
// End Cross Module References

// Begin ScriptStruct FBXTLPreviewObjectData
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_BXTLPreviewObjectData;
class UScriptStruct* FBXTLPreviewObjectData::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_BXTLPreviewObjectData.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_BXTLPreviewObjectData.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FBXTLPreviewObjectData, (UObject*)Z_Construct_UPackage__Script_BattleX(), TEXT("BXTLPreviewObjectData"));
	}
	return Z_Registration_Info_UScriptStruct_BXTLPreviewObjectData.OuterSingleton;
}
template<> BATTLEX_API UScriptStruct* StaticStruct<FBXTLPreviewObjectData>()
{
	return FBXTLPreviewObjectData::StaticStruct();
}
struct Z_Construct_UScriptStruct_FBXTLPreviewObjectData_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xca\xb1\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xd4\xa4\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xcc\xac\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Timeline/BXTLStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xca\xb1\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xd4\xa4\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xcc\xac\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Task_MetaData[] = {
		{ "Category", "BXTLPreviewObjectData" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xd3\xa6\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Timeline/BXTLStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xd3\xa6\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_NoOffsetTransform_MetaData[] = {
		{ "Category", "BXTLPreviewObjectData" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xc6\xab\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Timeline/BXTLStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xc6\xab\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Task;
	static const UECodeGen_Private::FStructPropertyParams NewProp_NoOffsetTransform;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FBXTLPreviewObjectData>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UScriptStruct_FBXTLPreviewObjectData_Statics::NewProp_Task = { "Task", nullptr, (EPropertyFlags)0x0010000000002004, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTLPreviewObjectData, Task), Z_Construct_UClass_UBXTask_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Task_MetaData), NewProp_Task_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FBXTLPreviewObjectData_Statics::NewProp_NoOffsetTransform = { "NoOffsetTransform", nullptr, (EPropertyFlags)0x0010000000002004, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTLPreviewObjectData, NoOffsetTransform), Z_Construct_UScriptStruct_FTransform, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_NoOffsetTransform_MetaData), NewProp_NoOffsetTransform_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FBXTLPreviewObjectData_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLPreviewObjectData_Statics::NewProp_Task,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLPreviewObjectData_Statics::NewProp_NoOffsetTransform,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXTLPreviewObjectData_Statics::PropPointers) < 2048);
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FBXTLPreviewObjectData_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_BattleX,
	nullptr,
	&NewStructOps,
	"BXTLPreviewObjectData",
	Z_Construct_UScriptStruct_FBXTLPreviewObjectData_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXTLPreviewObjectData_Statics::PropPointers),
	sizeof(FBXTLPreviewObjectData),
	alignof(FBXTLPreviewObjectData),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000001),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXTLPreviewObjectData_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FBXTLPreviewObjectData_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FBXTLPreviewObjectData()
{
	if (!Z_Registration_Info_UScriptStruct_BXTLPreviewObjectData.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_BXTLPreviewObjectData.InnerSingleton, Z_Construct_UScriptStruct_FBXTLPreviewObjectData_Statics::StructParams);
	}
	return Z_Registration_Info_UScriptStruct_BXTLPreviewObjectData.InnerSingleton;
}
// End ScriptStruct FBXTLPreviewObjectData

// Begin ScriptStruct FBXTLKeyFrame
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_BXTLKeyFrame;
class UScriptStruct* FBXTLKeyFrame::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_BXTLKeyFrame.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_BXTLKeyFrame.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FBXTLKeyFrame, (UObject*)Z_Construct_UPackage__Script_BattleX(), TEXT("BXTLKeyFrame"));
	}
	return Z_Registration_Info_UScriptStruct_BXTLKeyFrame.OuterSingleton;
}
template<> BATTLEX_API UScriptStruct* StaticStruct<FBXTLKeyFrame>()
{
	return FBXTLKeyFrame::StaticStruct();
}
struct Z_Construct_UScriptStruct_FBXTLKeyFrame_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xd8\xbc\xef\xbf\xbd\xd6\xa1\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Timeline/BXTLStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xd8\xbc\xef\xbf\xbd\xd6\xa1\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Time_MetaData[] = {
		{ "Category", "BXTLKeyFrame" },
		{ "ModuleRelativePath", "Timeline/BXTLStructs.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Tasks_MetaData[] = {
		{ "Category", "BXTLKeyFrame" },
		{ "ModuleRelativePath", "Timeline/BXTLStructs.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFloatPropertyParams NewProp_Time;
	static const UECodeGen_Private::FIntPropertyParams NewProp_Tasks_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_Tasks;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FBXTLKeyFrame>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
};
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FBXTLKeyFrame_Statics::NewProp_Time = { "Time", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTLKeyFrame, Time), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Time_MetaData), NewProp_Time_MetaData) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UScriptStruct_FBXTLKeyFrame_Statics::NewProp_Tasks_Inner = { "Tasks", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FBXTLKeyFrame_Statics::NewProp_Tasks = { "Tasks", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTLKeyFrame, Tasks), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Tasks_MetaData), NewProp_Tasks_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FBXTLKeyFrame_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLKeyFrame_Statics::NewProp_Time,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLKeyFrame_Statics::NewProp_Tasks_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLKeyFrame_Statics::NewProp_Tasks,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXTLKeyFrame_Statics::PropPointers) < 2048);
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FBXTLKeyFrame_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_BattleX,
	nullptr,
	&NewStructOps,
	"BXTLKeyFrame",
	Z_Construct_UScriptStruct_FBXTLKeyFrame_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXTLKeyFrame_Statics::PropPointers),
	sizeof(FBXTLKeyFrame),
	alignof(FBXTLKeyFrame),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000001),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXTLKeyFrame_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FBXTLKeyFrame_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FBXTLKeyFrame()
{
	if (!Z_Registration_Info_UScriptStruct_BXTLKeyFrame.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_BXTLKeyFrame.InnerSingleton, Z_Construct_UScriptStruct_FBXTLKeyFrame_Statics::StructParams);
	}
	return Z_Registration_Info_UScriptStruct_BXTLKeyFrame.InnerSingleton;
}
// End ScriptStruct FBXTLKeyFrame

// Begin ScriptStruct FBXTLPendingTaskInfo
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_BXTLPendingTaskInfo;
class UScriptStruct* FBXTLPendingTaskInfo::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_BXTLPendingTaskInfo.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_BXTLPendingTaskInfo.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FBXTLPendingTaskInfo, (UObject*)Z_Construct_UPackage__Script_BattleX(), TEXT("BXTLPendingTaskInfo"));
	}
	return Z_Registration_Info_UScriptStruct_BXTLPendingTaskInfo.OuterSingleton;
}
template<> BATTLEX_API UScriptStruct* StaticStruct<FBXTLPendingTaskInfo>()
{
	return FBXTLPendingTaskInfo::StaticStruct();
}
struct Z_Construct_UScriptStruct_FBXTLPendingTaskInfo_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xcf\xa2\n" },
#endif
		{ "ModuleRelativePath", "Timeline/BXTLStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xcf\xa2" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LocalIndex_MetaData[] = {
		{ "Category", "BXTLPendingTaskInfo" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xd6\xb2\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Timeline/BXTLStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xd6\xb2\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Time_MetaData[] = {
		{ "Category", "BXTLPendingTaskInfo" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xca\xb1\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Timeline/BXTLStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xca\xb1\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FIntPropertyParams NewProp_LocalIndex;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_Time;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FBXTLPendingTaskInfo>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
};
const UECodeGen_Private::FIntPropertyParams Z_Construct_UScriptStruct_FBXTLPendingTaskInfo_Statics::NewProp_LocalIndex = { "LocalIndex", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTLPendingTaskInfo, LocalIndex), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LocalIndex_MetaData), NewProp_LocalIndex_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FBXTLPendingTaskInfo_Statics::NewProp_Time = { "Time", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTLPendingTaskInfo, Time), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Time_MetaData), NewProp_Time_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FBXTLPendingTaskInfo_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLPendingTaskInfo_Statics::NewProp_LocalIndex,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLPendingTaskInfo_Statics::NewProp_Time,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXTLPendingTaskInfo_Statics::PropPointers) < 2048);
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FBXTLPendingTaskInfo_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_BattleX,
	nullptr,
	&NewStructOps,
	"BXTLPendingTaskInfo",
	Z_Construct_UScriptStruct_FBXTLPendingTaskInfo_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXTLPendingTaskInfo_Statics::PropPointers),
	sizeof(FBXTLPendingTaskInfo),
	alignof(FBXTLPendingTaskInfo),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000001),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXTLPendingTaskInfo_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FBXTLPendingTaskInfo_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FBXTLPendingTaskInfo()
{
	if (!Z_Registration_Info_UScriptStruct_BXTLPendingTaskInfo.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_BXTLPendingTaskInfo.InnerSingleton, Z_Construct_UScriptStruct_FBXTLPendingTaskInfo_Statics::StructParams);
	}
	return Z_Registration_Info_UScriptStruct_BXTLPendingTaskInfo.InnerSingleton;
}
// End ScriptStruct FBXTLPendingTaskInfo

// Begin ScriptStruct FBXTLBroadcastTaskInfo
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_BXTLBroadcastTaskInfo;
class UScriptStruct* FBXTLBroadcastTaskInfo::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_BXTLBroadcastTaskInfo.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_BXTLBroadcastTaskInfo.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FBXTLBroadcastTaskInfo, (UObject*)Z_Construct_UPackage__Script_BattleX(), TEXT("BXTLBroadcastTaskInfo"));
	}
	return Z_Registration_Info_UScriptStruct_BXTLBroadcastTaskInfo.OuterSingleton;
}
template<> BATTLEX_API UScriptStruct* StaticStruct<FBXTLBroadcastTaskInfo>()
{
	return FBXTLBroadcastTaskInfo::StaticStruct();
}
struct Z_Construct_UScriptStruct_FBXTLBroadcastTaskInfo_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xe3\xb2\xa5\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xcf\xa2\n" },
#endif
		{ "ModuleRelativePath", "Timeline/BXTLStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xe3\xb2\xa5\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xcf\xa2" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_FullIndex_MetaData[] = {
		{ "Category", "BXTLBroadcastTaskInfo" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xc8\xab\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Timeline/BXTLStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xc8\xab\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Name_MetaData[] = {
		{ "Category", "BXTLBroadcastTaskInfo" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xc2\xbc\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Timeline/BXTLStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xc2\xbc\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FIntPropertyParams NewProp_FullIndex;
	static const UECodeGen_Private::FNamePropertyParams NewProp_Name;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FBXTLBroadcastTaskInfo>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
};
const UECodeGen_Private::FIntPropertyParams Z_Construct_UScriptStruct_FBXTLBroadcastTaskInfo_Statics::NewProp_FullIndex = { "FullIndex", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTLBroadcastTaskInfo, FullIndex), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_FullIndex_MetaData), NewProp_FullIndex_MetaData) };
const UECodeGen_Private::FNamePropertyParams Z_Construct_UScriptStruct_FBXTLBroadcastTaskInfo_Statics::NewProp_Name = { "Name", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTLBroadcastTaskInfo, Name), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Name_MetaData), NewProp_Name_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FBXTLBroadcastTaskInfo_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLBroadcastTaskInfo_Statics::NewProp_FullIndex,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLBroadcastTaskInfo_Statics::NewProp_Name,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXTLBroadcastTaskInfo_Statics::PropPointers) < 2048);
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FBXTLBroadcastTaskInfo_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_BattleX,
	nullptr,
	&NewStructOps,
	"BXTLBroadcastTaskInfo",
	Z_Construct_UScriptStruct_FBXTLBroadcastTaskInfo_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXTLBroadcastTaskInfo_Statics::PropPointers),
	sizeof(FBXTLBroadcastTaskInfo),
	alignof(FBXTLBroadcastTaskInfo),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000001),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXTLBroadcastTaskInfo_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FBXTLBroadcastTaskInfo_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FBXTLBroadcastTaskInfo()
{
	if (!Z_Registration_Info_UScriptStruct_BXTLBroadcastTaskInfo.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_BXTLBroadcastTaskInfo.InnerSingleton, Z_Construct_UScriptStruct_FBXTLBroadcastTaskInfo_Statics::StructParams);
	}
	return Z_Registration_Info_UScriptStruct_BXTLBroadcastTaskInfo.InnerSingleton;
}
// End ScriptStruct FBXTLBroadcastTaskInfo

// Begin ScriptStruct FBXTLDynamicDataSearchKey
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_BXTLDynamicDataSearchKey;
class UScriptStruct* FBXTLDynamicDataSearchKey::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_BXTLDynamicDataSearchKey.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_BXTLDynamicDataSearchKey.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FBXTLDynamicDataSearchKey, (UObject*)Z_Construct_UPackage__Script_BattleX(), TEXT("BXTLDynamicDataSearchKey"));
	}
	return Z_Registration_Info_UScriptStruct_BXTLDynamicDataSearchKey.OuterSingleton;
}
template<> BATTLEX_API UScriptStruct* StaticStruct<FBXTLDynamicDataSearchKey>()
{
	return FBXTLDynamicDataSearchKey::StaticStruct();
}
struct Z_Construct_UScriptStruct_FBXTLDynamicDataSearchKey_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xef\xbf\xbd\xcc\xac\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xdd\xbc\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xd1\xaf\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Timeline/BXTLStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xef\xbf\xbd\xcc\xac\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xdd\xbc\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xd1\xaf\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Index_MetaData[] = {
		{ "Category", "BXTLDynamicDataSearchKey" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Timeline/BXTLStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Name_MetaData[] = {
		{ "Category", "BXTLDynamicDataSearchKey" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Timeline/BXTLStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FIntPropertyParams NewProp_Index;
	static const UECodeGen_Private::FNamePropertyParams NewProp_Name;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FBXTLDynamicDataSearchKey>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
};
const UECodeGen_Private::FIntPropertyParams Z_Construct_UScriptStruct_FBXTLDynamicDataSearchKey_Statics::NewProp_Index = { "Index", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTLDynamicDataSearchKey, Index), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Index_MetaData), NewProp_Index_MetaData) };
const UECodeGen_Private::FNamePropertyParams Z_Construct_UScriptStruct_FBXTLDynamicDataSearchKey_Statics::NewProp_Name = { "Name", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTLDynamicDataSearchKey, Name), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Name_MetaData), NewProp_Name_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FBXTLDynamicDataSearchKey_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLDynamicDataSearchKey_Statics::NewProp_Index,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLDynamicDataSearchKey_Statics::NewProp_Name,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXTLDynamicDataSearchKey_Statics::PropPointers) < 2048);
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FBXTLDynamicDataSearchKey_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_BattleX,
	nullptr,
	&NewStructOps,
	"BXTLDynamicDataSearchKey",
	Z_Construct_UScriptStruct_FBXTLDynamicDataSearchKey_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXTLDynamicDataSearchKey_Statics::PropPointers),
	sizeof(FBXTLDynamicDataSearchKey),
	alignof(FBXTLDynamicDataSearchKey),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000001),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXTLDynamicDataSearchKey_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FBXTLDynamicDataSearchKey_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FBXTLDynamicDataSearchKey()
{
	if (!Z_Registration_Info_UScriptStruct_BXTLDynamicDataSearchKey.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_BXTLDynamicDataSearchKey.InnerSingleton, Z_Construct_UScriptStruct_FBXTLDynamicDataSearchKey_Statics::StructParams);
	}
	return Z_Registration_Info_UScriptStruct_BXTLDynamicDataSearchKey.InnerSingleton;
}
// End ScriptStruct FBXTLDynamicDataSearchKey

// Begin ScriptStruct FBXTLTaskRTData
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_BXTLTaskRTData;
class UScriptStruct* FBXTLTaskRTData::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_BXTLTaskRTData.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_BXTLTaskRTData.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FBXTLTaskRTData, (UObject*)Z_Construct_UPackage__Script_BattleX(), TEXT("BXTLTaskRTData"));
	}
	return Z_Registration_Info_UScriptStruct_BXTLTaskRTData.OuterSingleton;
}
template<> BATTLEX_API UScriptStruct* StaticStruct<FBXTLTaskRTData>()
{
	return FBXTLTaskRTData::StaticStruct();
}
struct Z_Construct_UScriptStruct_FBXTLTaskRTData_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Timeline/BXTLStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Index_MetaData[] = {
		{ "Category", "BXTLTaskRTData" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xef\xbf\xbd\xcc\xac\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Timeline/BXTLStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xef\xbf\xbd\xcc\xac\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_StartTimestamp_MetaData[] = {
		{ "Category", "BXTLTaskRTData" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xef\xbf\xbd\xca\xbc\xca\xb1\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Timeline/BXTLStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xef\xbf\xbd\xca\xbc\xca\xb1\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_RunTime_MetaData[] = {
		{ "Category", "BXTLTaskRTData" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xca\xb1\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Timeline/BXTLStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xca\xb1\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bEarlyFinish_MetaData[] = {
		{ "Category", "BXTLTaskRTData" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xc7\xb7\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xc7\xb0\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Timeline/BXTLStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xc7\xb7\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xc7\xb0\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_DynamicData_MetaData[] = {
		{ "Category", "BXTLTaskRTData" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xd4\xb6\xef\xbf\xbd\xef\xbf\xbd\xe5\xb6\xaf\xcc\xac\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Timeline/BXTLStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xd4\xb6\xef\xbf\xbd\xef\xbf\xbd\xe5\xb6\xaf\xcc\xac\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FIntPropertyParams NewProp_Index;
	static const UECodeGen_Private::FInt64PropertyParams NewProp_StartTimestamp;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_RunTime;
	static void NewProp_bEarlyFinish_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bEarlyFinish;
	static const UECodeGen_Private::FStructPropertyParams NewProp_DynamicData;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FBXTLTaskRTData>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
};
const UECodeGen_Private::FIntPropertyParams Z_Construct_UScriptStruct_FBXTLTaskRTData_Statics::NewProp_Index = { "Index", nullptr, (EPropertyFlags)0x0010000000002014, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTLTaskRTData, Index), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Index_MetaData), NewProp_Index_MetaData) };
const UECodeGen_Private::FInt64PropertyParams Z_Construct_UScriptStruct_FBXTLTaskRTData_Statics::NewProp_StartTimestamp = { "StartTimestamp", nullptr, (EPropertyFlags)0x0010000000002004, UECodeGen_Private::EPropertyGenFlags::Int64, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTLTaskRTData, StartTimestamp), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_StartTimestamp_MetaData), NewProp_StartTimestamp_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FBXTLTaskRTData_Statics::NewProp_RunTime = { "RunTime", nullptr, (EPropertyFlags)0x0010000000002004, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTLTaskRTData, RunTime), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_RunTime_MetaData), NewProp_RunTime_MetaData) };
void Z_Construct_UScriptStruct_FBXTLTaskRTData_Statics::NewProp_bEarlyFinish_SetBit(void* Obj)
{
	((FBXTLTaskRTData*)Obj)->bEarlyFinish = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FBXTLTaskRTData_Statics::NewProp_bEarlyFinish = { "bEarlyFinish", nullptr, (EPropertyFlags)0x0010000000002004, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FBXTLTaskRTData), &Z_Construct_UScriptStruct_FBXTLTaskRTData_Statics::NewProp_bEarlyFinish_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bEarlyFinish_MetaData), NewProp_bEarlyFinish_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FBXTLTaskRTData_Statics::NewProp_DynamicData = { "DynamicData", nullptr, (EPropertyFlags)0x0010000000002004, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTLTaskRTData, DynamicData), Z_Construct_UScriptStruct_FInstancedStruct, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_DynamicData_MetaData), NewProp_DynamicData_MetaData) }; // 174410355
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FBXTLTaskRTData_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLTaskRTData_Statics::NewProp_Index,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLTaskRTData_Statics::NewProp_StartTimestamp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLTaskRTData_Statics::NewProp_RunTime,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLTaskRTData_Statics::NewProp_bEarlyFinish,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLTaskRTData_Statics::NewProp_DynamicData,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXTLTaskRTData_Statics::PropPointers) < 2048);
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FBXTLTaskRTData_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_BattleX,
	nullptr,
	&NewStructOps,
	"BXTLTaskRTData",
	Z_Construct_UScriptStruct_FBXTLTaskRTData_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXTLTaskRTData_Statics::PropPointers),
	sizeof(FBXTLTaskRTData),
	alignof(FBXTLTaskRTData),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000001),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXTLTaskRTData_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FBXTLTaskRTData_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FBXTLTaskRTData()
{
	if (!Z_Registration_Info_UScriptStruct_BXTLTaskRTData.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_BXTLTaskRTData.InnerSingleton, Z_Construct_UScriptStruct_FBXTLTaskRTData_Statics::StructParams);
	}
	return Z_Registration_Info_UScriptStruct_BXTLTaskRTData.InnerSingleton;
}
// End ScriptStruct FBXTLTaskRTData

// Begin ScriptStruct FBXTLTaskHostingData
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_BXTLTaskHostingData;
class UScriptStruct* FBXTLTaskHostingData::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_BXTLTaskHostingData.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_BXTLTaskHostingData.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FBXTLTaskHostingData, (UObject*)Z_Construct_UPackage__Script_BattleX(), TEXT("BXTLTaskHostingData"));
	}
	return Z_Registration_Info_UScriptStruct_BXTLTaskHostingData.OuterSingleton;
}
template<> BATTLEX_API UScriptStruct* StaticStruct<FBXTLTaskHostingData>()
{
	return FBXTLTaskHostingData::StaticStruct();
}
struct Z_Construct_UScriptStruct_FBXTLTaskHostingData_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xd0\xb9\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Timeline/BXTLStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xd0\xb9\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Task_MetaData[] = {
		{ "Category", "BXTLTaskHostingData" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xcc\xac\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Timeline/BXTLStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xcc\xac\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Owner_MetaData[] = {
		{ "Category", "BXTLTaskHostingData" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xd3\xb5\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Timeline/BXTLStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xd3\xb5\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Instigator_MetaData[] = {
		{ "Category", "BXTLTaskHostingData" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xca\xbc\xef\xbf\xbd\xef\xbf\xbd\xd9\xb8\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Timeline/BXTLStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xca\xbc\xef\xbf\xbd\xef\xbf\xbd\xd9\xb8\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Triggerer_MetaData[] = {
		{ "Category", "BXTLTaskHostingData" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Timeline/BXTLStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LockParts_MetaData[] = {
		{ "Category", "BXTLTaskHostingData" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xc4\xbf\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xd0\xb1\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Timeline/BXTLStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xc4\xbf\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xd0\xb1\xef\xbf\xbd" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_TaskRTData_MetaData[] = {
		{ "Category", "BXTLTaskHostingData" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Timeline/BXTLStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Task;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Owner;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Instigator;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Triggerer;
	static const UECodeGen_Private::FStructPropertyParams NewProp_LockParts_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_LockParts;
	static const UECodeGen_Private::FStructPropertyParams NewProp_TaskRTData;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FBXTLTaskHostingData>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UScriptStruct_FBXTLTaskHostingData_Statics::NewProp_Task = { "Task", nullptr, (EPropertyFlags)0x0010000000002004, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTLTaskHostingData, Task), Z_Construct_UClass_UBXTask_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Task_MetaData), NewProp_Task_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UScriptStruct_FBXTLTaskHostingData_Statics::NewProp_Owner = { "Owner", nullptr, (EPropertyFlags)0x0010000000002004, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTLTaskHostingData, Owner), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Owner_MetaData), NewProp_Owner_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UScriptStruct_FBXTLTaskHostingData_Statics::NewProp_Instigator = { "Instigator", nullptr, (EPropertyFlags)0x0010000000002004, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTLTaskHostingData, Instigator), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Instigator_MetaData), NewProp_Instigator_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UScriptStruct_FBXTLTaskHostingData_Statics::NewProp_Triggerer = { "Triggerer", nullptr, (EPropertyFlags)0x0010000000002004, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTLTaskHostingData, Triggerer), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Triggerer_MetaData), NewProp_Triggerer_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FBXTLTaskHostingData_Statics::NewProp_LockParts_Inner = { "LockParts", nullptr, (EPropertyFlags)0x0000008000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UScriptStruct_FBXBodyPartSelection, METADATA_PARAMS(0, nullptr) }; // 1620284852
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FBXTLTaskHostingData_Statics::NewProp_LockParts = { "LockParts", nullptr, (EPropertyFlags)0x0010008000002004, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTLTaskHostingData, LockParts), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LockParts_MetaData), NewProp_LockParts_MetaData) }; // 1620284852
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FBXTLTaskHostingData_Statics::NewProp_TaskRTData = { "TaskRTData", nullptr, (EPropertyFlags)0x0010000000002004, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTLTaskHostingData, TaskRTData), Z_Construct_UScriptStruct_FBXTLTaskRTData, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_TaskRTData_MetaData), NewProp_TaskRTData_MetaData) }; // 2012750535
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FBXTLTaskHostingData_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLTaskHostingData_Statics::NewProp_Task,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLTaskHostingData_Statics::NewProp_Owner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLTaskHostingData_Statics::NewProp_Instigator,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLTaskHostingData_Statics::NewProp_Triggerer,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLTaskHostingData_Statics::NewProp_LockParts_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLTaskHostingData_Statics::NewProp_LockParts,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLTaskHostingData_Statics::NewProp_TaskRTData,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXTLTaskHostingData_Statics::PropPointers) < 2048);
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FBXTLTaskHostingData_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_BattleX,
	nullptr,
	&NewStructOps,
	"BXTLTaskHostingData",
	Z_Construct_UScriptStruct_FBXTLTaskHostingData_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXTLTaskHostingData_Statics::PropPointers),
	sizeof(FBXTLTaskHostingData),
	alignof(FBXTLTaskHostingData),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000005),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXTLTaskHostingData_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FBXTLTaskHostingData_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FBXTLTaskHostingData()
{
	if (!Z_Registration_Info_UScriptStruct_BXTLTaskHostingData.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_BXTLTaskHostingData.InnerSingleton, Z_Construct_UScriptStruct_FBXTLTaskHostingData_Statics::StructParams);
	}
	return Z_Registration_Info_UScriptStruct_BXTLTaskHostingData.InnerSingleton;
}
// End ScriptStruct FBXTLTaskHostingData

// Begin ScriptStruct FBXTLSectionRTData
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_BXTLSectionRTData;
class UScriptStruct* FBXTLSectionRTData::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_BXTLSectionRTData.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_BXTLSectionRTData.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FBXTLSectionRTData, (UObject*)Z_Construct_UPackage__Script_BattleX(), TEXT("BXTLSectionRTData"));
	}
	return Z_Registration_Info_UScriptStruct_BXTLSectionRTData.OuterSingleton;
}
template<> BATTLEX_API UScriptStruct* StaticStruct<FBXTLSectionRTData>()
{
	return FBXTLSectionRTData::StaticStruct();
}
struct Z_Construct_UScriptStruct_FBXTLSectionRTData_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xca\xb1\xef\xbf\xbd\xef\xbf\xbd\xc6\xac\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Timeline/BXTLStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xca\xb1\xef\xbf\xbd\xef\xbf\xbd\xc6\xac\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Index_MetaData[] = {
		{ "Category", "BXTLSectionRTData" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xca\xb1\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Timeline/BXTLStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xca\xb1\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_KeyFrameIndex_MetaData[] = {
		{ "Category", "BXTLSectionRTData" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xef\xbf\xbd\xc7\xb0\xef\xbf\xbd\xc4\xb9\xd8\xbc\xef\xbf\xbd\xd6\xa1\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Timeline/BXTLStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xef\xbf\xbd\xc7\xb0\xef\xbf\xbd\xc4\xb9\xd8\xbc\xef\xbf\xbd\xd6\xa1\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Timestamp_MetaData[] = {
		{ "Category", "BXTLSectionRTData" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xef\xbf\xbd\xca\xbc\xca\xb1\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Timeline/BXTLStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xef\xbf\xbd\xca\xbc\xca\xb1\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_RunTime_MetaData[] = {
		{ "Category", "BXTLSectionRTData" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xca\xb1\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Timeline/BXTLStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xca\xb1\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LoopCount_MetaData[] = {
		{ "Category", "BXTLSectionRTData" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xd1\xad\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Timeline/BXTLStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xd1\xad\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_RunningTasks_MetaData[] = {
		{ "Category", "BXTLSectionRTData" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xd0\xb5\xef\xbf\xbdTask\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Timeline/BXTLStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xd0\xb5\xef\xbf\xbdTask\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_PendingTasks_MetaData[] = {
		{ "Category", "BXTLSectionRTData" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xef\xbf\xbd\xd6\xb4\xef\xbf\xbd\xef\xbf\xbdTask\xef\xbf\xbd\xd0\xb1\xef\xbf\xbd Task\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xca\xb1\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Timeline/BXTLStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xef\xbf\xbd\xd6\xb4\xef\xbf\xbd\xef\xbf\xbdTask\xef\xbf\xbd\xd0\xb1\xef\xbf\xbd Task\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xca\xb1\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_FramePendingTasks_MetaData[] = {
		{ "Category", "BXTLSectionRTData" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xd6\xa1\xef\xbf\xbd\xda\xb4\xef\xbf\xbd\xd6\xb4\xef\xbf\xbd\xef\xbf\xbdTask\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xd0\xb1\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Timeline/BXTLStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xd6\xa1\xef\xbf\xbd\xda\xb4\xef\xbf\xbd\xd6\xb4\xef\xbf\xbd\xef\xbf\xbdTask\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xd0\xb1\xef\xbf\xbd" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_BroadcastTasks_MetaData[] = {
		{ "Category", "BXTLSectionRTData" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xef\xbf\xbd\xd2\xaa\xef\xbf\xbd\xe3\xb2\xa5\xef\xbf\xbd\xef\xbf\xbdTask\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd \xca\xb1\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd*1000+Task\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xca\xb1\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Timeline/BXTLStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xef\xbf\xbd\xd2\xaa\xef\xbf\xbd\xe3\xb2\xa5\xef\xbf\xbd\xef\xbf\xbdTask\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd \xca\xb1\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd*1000+Task\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xca\xb1\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FIntPropertyParams NewProp_Index;
	static const UECodeGen_Private::FIntPropertyParams NewProp_KeyFrameIndex;
	static const UECodeGen_Private::FInt64PropertyParams NewProp_Timestamp;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_RunTime;
	static const UECodeGen_Private::FIntPropertyParams NewProp_LoopCount;
	static const UECodeGen_Private::FStructPropertyParams NewProp_RunningTasks_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_RunningTasks;
	static const UECodeGen_Private::FStructPropertyParams NewProp_PendingTasks_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_PendingTasks;
	static const UECodeGen_Private::FIntPropertyParams NewProp_FramePendingTasks_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_FramePendingTasks;
	static const UECodeGen_Private::FStructPropertyParams NewProp_BroadcastTasks_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_BroadcastTasks;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FBXTLSectionRTData>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
};
const UECodeGen_Private::FIntPropertyParams Z_Construct_UScriptStruct_FBXTLSectionRTData_Statics::NewProp_Index = { "Index", nullptr, (EPropertyFlags)0x0010000000002004, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTLSectionRTData, Index), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Index_MetaData), NewProp_Index_MetaData) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UScriptStruct_FBXTLSectionRTData_Statics::NewProp_KeyFrameIndex = { "KeyFrameIndex", nullptr, (EPropertyFlags)0x0010000000002004, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTLSectionRTData, KeyFrameIndex), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_KeyFrameIndex_MetaData), NewProp_KeyFrameIndex_MetaData) };
const UECodeGen_Private::FInt64PropertyParams Z_Construct_UScriptStruct_FBXTLSectionRTData_Statics::NewProp_Timestamp = { "Timestamp", nullptr, (EPropertyFlags)0x0010000000002004, UECodeGen_Private::EPropertyGenFlags::Int64, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTLSectionRTData, Timestamp), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Timestamp_MetaData), NewProp_Timestamp_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FBXTLSectionRTData_Statics::NewProp_RunTime = { "RunTime", nullptr, (EPropertyFlags)0x0010000000002004, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTLSectionRTData, RunTime), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_RunTime_MetaData), NewProp_RunTime_MetaData) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UScriptStruct_FBXTLSectionRTData_Statics::NewProp_LoopCount = { "LoopCount", nullptr, (EPropertyFlags)0x0010000000002004, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTLSectionRTData, LoopCount), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LoopCount_MetaData), NewProp_LoopCount_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FBXTLSectionRTData_Statics::NewProp_RunningTasks_Inner = { "RunningTasks", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UScriptStruct_FBXTLTaskRTData, METADATA_PARAMS(0, nullptr) }; // 2012750535
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FBXTLSectionRTData_Statics::NewProp_RunningTasks = { "RunningTasks", nullptr, (EPropertyFlags)0x0010000000002004, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTLSectionRTData, RunningTasks), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_RunningTasks_MetaData), NewProp_RunningTasks_MetaData) }; // 2012750535
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FBXTLSectionRTData_Statics::NewProp_PendingTasks_Inner = { "PendingTasks", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UScriptStruct_FBXTLPendingTaskInfo, METADATA_PARAMS(0, nullptr) }; // 3248114118
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FBXTLSectionRTData_Statics::NewProp_PendingTasks = { "PendingTasks", nullptr, (EPropertyFlags)0x0010000000002004, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTLSectionRTData, PendingTasks), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_PendingTasks_MetaData), NewProp_PendingTasks_MetaData) }; // 3248114118
const UECodeGen_Private::FIntPropertyParams Z_Construct_UScriptStruct_FBXTLSectionRTData_Statics::NewProp_FramePendingTasks_Inner = { "FramePendingTasks", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FBXTLSectionRTData_Statics::NewProp_FramePendingTasks = { "FramePendingTasks", nullptr, (EPropertyFlags)0x0010000000002004, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTLSectionRTData, FramePendingTasks), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_FramePendingTasks_MetaData), NewProp_FramePendingTasks_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FBXTLSectionRTData_Statics::NewProp_BroadcastTasks_Inner = { "BroadcastTasks", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UScriptStruct_FBXTLBroadcastTaskInfo, METADATA_PARAMS(0, nullptr) }; // 1765471375
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FBXTLSectionRTData_Statics::NewProp_BroadcastTasks = { "BroadcastTasks", nullptr, (EPropertyFlags)0x0010000000002004, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTLSectionRTData, BroadcastTasks), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_BroadcastTasks_MetaData), NewProp_BroadcastTasks_MetaData) }; // 1765471375
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FBXTLSectionRTData_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLSectionRTData_Statics::NewProp_Index,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLSectionRTData_Statics::NewProp_KeyFrameIndex,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLSectionRTData_Statics::NewProp_Timestamp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLSectionRTData_Statics::NewProp_RunTime,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLSectionRTData_Statics::NewProp_LoopCount,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLSectionRTData_Statics::NewProp_RunningTasks_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLSectionRTData_Statics::NewProp_RunningTasks,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLSectionRTData_Statics::NewProp_PendingTasks_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLSectionRTData_Statics::NewProp_PendingTasks,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLSectionRTData_Statics::NewProp_FramePendingTasks_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLSectionRTData_Statics::NewProp_FramePendingTasks,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLSectionRTData_Statics::NewProp_BroadcastTasks_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLSectionRTData_Statics::NewProp_BroadcastTasks,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXTLSectionRTData_Statics::PropPointers) < 2048);
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FBXTLSectionRTData_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_BattleX,
	nullptr,
	&NewStructOps,
	"BXTLSectionRTData",
	Z_Construct_UScriptStruct_FBXTLSectionRTData_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXTLSectionRTData_Statics::PropPointers),
	sizeof(FBXTLSectionRTData),
	alignof(FBXTLSectionRTData),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000001),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXTLSectionRTData_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FBXTLSectionRTData_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FBXTLSectionRTData()
{
	if (!Z_Registration_Info_UScriptStruct_BXTLSectionRTData.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_BXTLSectionRTData.InnerSingleton, Z_Construct_UScriptStruct_FBXTLSectionRTData_Statics::StructParams);
	}
	return Z_Registration_Info_UScriptStruct_BXTLSectionRTData.InnerSingleton;
}
// End ScriptStruct FBXTLSectionRTData

// Begin ScriptStruct FBXTLRunTimeData
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_BXTLRunTimeData;
class UScriptStruct* FBXTLRunTimeData::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_BXTLRunTimeData.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_BXTLRunTimeData.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FBXTLRunTimeData, (UObject*)Z_Construct_UPackage__Script_BattleX(), TEXT("BXTLRunTimeData"));
	}
	return Z_Registration_Info_UScriptStruct_BXTLRunTimeData.OuterSingleton;
}
template<> BATTLEX_API UScriptStruct* StaticStruct<FBXTLRunTimeData>()
{
	return FBXTLRunTimeData::StaticStruct();
}
struct Z_Construct_UScriptStruct_FBXTLRunTimeData_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xca\xb1\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Timeline/BXTLStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xca\xb1\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ID_MetaData[] = {
		{ "Category", "BXTLRunTimeData" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xce\xa8\xd2\xbbID\n" },
#endif
		{ "ModuleRelativePath", "Timeline/BXTLStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xce\xa8\xd2\xbbID" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_StaticData_MetaData[] = {
		{ "Category", "BXTLRunTimeData" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xef\xbf\xbd\xcc\xac\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Timeline/BXTLStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xef\xbf\xbd\xcc\xac\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Owner_MetaData[] = {
		{ "Category", "BXTLRunTimeData" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xd3\xb5\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Timeline/BXTLStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xd3\xb5\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Instigator_MetaData[] = {
		{ "Category", "BXTLRunTimeData" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xca\xbc\xef\xbf\xbd\xef\xbf\xbd\xd9\xb8\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Timeline/BXTLStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xca\xbc\xef\xbf\xbd\xef\xbf\xbd\xd9\xb8\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Triggerer_MetaData[] = {
		{ "Category", "BXTLRunTimeData" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Timeline/BXTLStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LockParts_MetaData[] = {
		{ "Category", "BXTLRunTimeData" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xc4\xbf\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xd0\xb1\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Timeline/BXTLStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xc4\xbf\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xd0\xb1\xef\xbf\xbd" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Timestamp_MetaData[] = {
		{ "Category", "BXTLRunTimeData" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xef\xbf\xbd\xca\xbc\xca\xb1\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Timeline/BXTLStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xef\xbf\xbd\xca\xbc\xca\xb1\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_RunTime_MetaData[] = {
		{ "Category", "BXTLRunTimeData" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xca\xb1\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Timeline/BXTLStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xca\xb1\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_RunRate_MetaData[] = {
		{ "Category", "BXTLRunTimeData" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Timeline/BXTLStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bEarlyFinish_MetaData[] = {
		{ "Category", "BXTLRunTimeData" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xc7\xb7\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xc7\xb0\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Timeline/BXTLStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xc7\xb7\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xc7\xb0\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_RunningSections_MetaData[] = {
		{ "Category", "BXTLRunTimeData" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xc6\xac\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xcf\xa2\n" },
#endif
		{ "ModuleRelativePath", "Timeline/BXTLStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xc6\xac\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xcf\xa2" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_DynamicDatas_MetaData[] = {
		{ "Category", "BXTLRunTimeData" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xef\xbf\xbd\xcc\xac\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xdd\xbc\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Timeline/BXTLStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xef\xbf\xbd\xcc\xac\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xdd\xbc\xef\xbf\xbd" },
#endif
	};
#if WITH_EDITORONLY_DATA
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_PreviewObjects_MetaData[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xd4\xa4\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xc8\xab\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xd3\xb3\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Timeline/BXTLStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xd4\xa4\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xc8\xab\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xd3\xb3\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
#endif // WITH_EDITORONLY_DATA
#endif // WITH_METADATA
	static const UECodeGen_Private::FInt64PropertyParams NewProp_ID;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_StaticData;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Owner;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Instigator;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Triggerer;
	static const UECodeGen_Private::FStructPropertyParams NewProp_LockParts_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_LockParts;
	static const UECodeGen_Private::FInt64PropertyParams NewProp_Timestamp;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_RunTime;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_RunRate;
	static void NewProp_bEarlyFinish_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bEarlyFinish;
	static const UECodeGen_Private::FStructPropertyParams NewProp_RunningSections_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_RunningSections;
	static const UECodeGen_Private::FStructPropertyParams NewProp_DynamicDatas_ValueProp;
	static const UECodeGen_Private::FStructPropertyParams NewProp_DynamicDatas_Key_KeyProp;
	static const UECodeGen_Private::FMapPropertyParams NewProp_DynamicDatas;
#if WITH_EDITORONLY_DATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_PreviewObjects_ValueProp;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_PreviewObjects_Key_KeyProp;
	static const UECodeGen_Private::FMapPropertyParams NewProp_PreviewObjects;
#endif // WITH_EDITORONLY_DATA
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FBXTLRunTimeData>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
};
const UECodeGen_Private::FInt64PropertyParams Z_Construct_UScriptStruct_FBXTLRunTimeData_Statics::NewProp_ID = { "ID", nullptr, (EPropertyFlags)0x0010000000002004, UECodeGen_Private::EPropertyGenFlags::Int64, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTLRunTimeData, ID), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ID_MetaData), NewProp_ID_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UScriptStruct_FBXTLRunTimeData_Statics::NewProp_StaticData = { "StaticData", nullptr, (EPropertyFlags)0x0010000000002004, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTLRunTimeData, StaticData), Z_Construct_UClass_UBXTLAsset_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_StaticData_MetaData), NewProp_StaticData_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UScriptStruct_FBXTLRunTimeData_Statics::NewProp_Owner = { "Owner", nullptr, (EPropertyFlags)0x0010000000002004, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTLRunTimeData, Owner), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Owner_MetaData), NewProp_Owner_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UScriptStruct_FBXTLRunTimeData_Statics::NewProp_Instigator = { "Instigator", nullptr, (EPropertyFlags)0x0010000000002004, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTLRunTimeData, Instigator), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Instigator_MetaData), NewProp_Instigator_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UScriptStruct_FBXTLRunTimeData_Statics::NewProp_Triggerer = { "Triggerer", nullptr, (EPropertyFlags)0x0010000000002004, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTLRunTimeData, Triggerer), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Triggerer_MetaData), NewProp_Triggerer_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FBXTLRunTimeData_Statics::NewProp_LockParts_Inner = { "LockParts", nullptr, (EPropertyFlags)0x0000008000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UScriptStruct_FBXBodyPartSelection, METADATA_PARAMS(0, nullptr) }; // 1620284852
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FBXTLRunTimeData_Statics::NewProp_LockParts = { "LockParts", nullptr, (EPropertyFlags)0x0010008000002004, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTLRunTimeData, LockParts), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LockParts_MetaData), NewProp_LockParts_MetaData) }; // 1620284852
const UECodeGen_Private::FInt64PropertyParams Z_Construct_UScriptStruct_FBXTLRunTimeData_Statics::NewProp_Timestamp = { "Timestamp", nullptr, (EPropertyFlags)0x0010000000002004, UECodeGen_Private::EPropertyGenFlags::Int64, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTLRunTimeData, Timestamp), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Timestamp_MetaData), NewProp_Timestamp_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FBXTLRunTimeData_Statics::NewProp_RunTime = { "RunTime", nullptr, (EPropertyFlags)0x0010000000002004, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTLRunTimeData, RunTime), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_RunTime_MetaData), NewProp_RunTime_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FBXTLRunTimeData_Statics::NewProp_RunRate = { "RunRate", nullptr, (EPropertyFlags)0x0010000000002004, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTLRunTimeData, RunRate), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_RunRate_MetaData), NewProp_RunRate_MetaData) };
void Z_Construct_UScriptStruct_FBXTLRunTimeData_Statics::NewProp_bEarlyFinish_SetBit(void* Obj)
{
	((FBXTLRunTimeData*)Obj)->bEarlyFinish = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FBXTLRunTimeData_Statics::NewProp_bEarlyFinish = { "bEarlyFinish", nullptr, (EPropertyFlags)0x0010000000002004, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FBXTLRunTimeData), &Z_Construct_UScriptStruct_FBXTLRunTimeData_Statics::NewProp_bEarlyFinish_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bEarlyFinish_MetaData), NewProp_bEarlyFinish_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FBXTLRunTimeData_Statics::NewProp_RunningSections_Inner = { "RunningSections", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UScriptStruct_FBXTLSectionRTData, METADATA_PARAMS(0, nullptr) }; // 262691324
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FBXTLRunTimeData_Statics::NewProp_RunningSections = { "RunningSections", nullptr, (EPropertyFlags)0x0010000000002004, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTLRunTimeData, RunningSections), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_RunningSections_MetaData), NewProp_RunningSections_MetaData) }; // 262691324
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FBXTLRunTimeData_Statics::NewProp_DynamicDatas_ValueProp = { "DynamicDatas", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 1, Z_Construct_UScriptStruct_FInstancedStruct, METADATA_PARAMS(0, nullptr) }; // 174410355
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FBXTLRunTimeData_Statics::NewProp_DynamicDatas_Key_KeyProp = { "DynamicDatas_Key", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UScriptStruct_FBXTLDynamicDataSearchKey, METADATA_PARAMS(0, nullptr) }; // 1243550611
const UECodeGen_Private::FMapPropertyParams Z_Construct_UScriptStruct_FBXTLRunTimeData_Statics::NewProp_DynamicDatas = { "DynamicDatas", nullptr, (EPropertyFlags)0x0010000000002004, UECodeGen_Private::EPropertyGenFlags::Map, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTLRunTimeData, DynamicDatas), EMapPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_DynamicDatas_MetaData), NewProp_DynamicDatas_MetaData) }; // 1243550611 174410355
#if WITH_EDITORONLY_DATA
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FBXTLRunTimeData_Statics::NewProp_PreviewObjects_ValueProp = { "PreviewObjects", nullptr, (EPropertyFlags)0x0000000800000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 1, Z_Construct_UScriptStruct_FBXTLPreviewObjectData, METADATA_PARAMS(0, nullptr) }; // 479753775
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UScriptStruct_FBXTLRunTimeData_Statics::NewProp_PreviewObjects_Key_KeyProp = { "PreviewObjects_Key", nullptr, (EPropertyFlags)0x0000000800000000, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UClass_UObject_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FMapPropertyParams Z_Construct_UScriptStruct_FBXTLRunTimeData_Statics::NewProp_PreviewObjects = { "PreviewObjects", nullptr, (EPropertyFlags)0x0010000800002000, UECodeGen_Private::EPropertyGenFlags::Map, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTLRunTimeData, PreviewObjects), EMapPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_PreviewObjects_MetaData), NewProp_PreviewObjects_MetaData) }; // 479753775
#endif // WITH_EDITORONLY_DATA
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FBXTLRunTimeData_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLRunTimeData_Statics::NewProp_ID,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLRunTimeData_Statics::NewProp_StaticData,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLRunTimeData_Statics::NewProp_Owner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLRunTimeData_Statics::NewProp_Instigator,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLRunTimeData_Statics::NewProp_Triggerer,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLRunTimeData_Statics::NewProp_LockParts_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLRunTimeData_Statics::NewProp_LockParts,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLRunTimeData_Statics::NewProp_Timestamp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLRunTimeData_Statics::NewProp_RunTime,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLRunTimeData_Statics::NewProp_RunRate,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLRunTimeData_Statics::NewProp_bEarlyFinish,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLRunTimeData_Statics::NewProp_RunningSections_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLRunTimeData_Statics::NewProp_RunningSections,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLRunTimeData_Statics::NewProp_DynamicDatas_ValueProp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLRunTimeData_Statics::NewProp_DynamicDatas_Key_KeyProp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLRunTimeData_Statics::NewProp_DynamicDatas,
#if WITH_EDITORONLY_DATA
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLRunTimeData_Statics::NewProp_PreviewObjects_ValueProp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLRunTimeData_Statics::NewProp_PreviewObjects_Key_KeyProp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLRunTimeData_Statics::NewProp_PreviewObjects,
#endif // WITH_EDITORONLY_DATA
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXTLRunTimeData_Statics::PropPointers) < 2048);
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FBXTLRunTimeData_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_BattleX,
	nullptr,
	&NewStructOps,
	"BXTLRunTimeData",
	Z_Construct_UScriptStruct_FBXTLRunTimeData_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXTLRunTimeData_Statics::PropPointers),
	sizeof(FBXTLRunTimeData),
	alignof(FBXTLRunTimeData),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000005),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXTLRunTimeData_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FBXTLRunTimeData_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FBXTLRunTimeData()
{
	if (!Z_Registration_Info_UScriptStruct_BXTLRunTimeData.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_BXTLRunTimeData.InnerSingleton, Z_Construct_UScriptStruct_FBXTLRunTimeData_Statics::StructParams);
	}
	return Z_Registration_Info_UScriptStruct_BXTLRunTimeData.InnerSingleton;
}
// End ScriptStruct FBXTLRunTimeData

// Begin ScriptStruct FBXTLPlayContext
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_BXTLPlayContext;
class UScriptStruct* FBXTLPlayContext::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_BXTLPlayContext.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_BXTLPlayContext.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FBXTLPlayContext, (UObject*)Z_Construct_UPackage__Script_BattleX(), TEXT("BXTLPlayContext"));
	}
	return Z_Registration_Info_UScriptStruct_BXTLPlayContext.OuterSingleton;
}
template<> BATTLEX_API UScriptStruct* StaticStruct<FBXTLPlayContext>()
{
	return FBXTLPlayContext::StaticStruct();
}
struct Z_Construct_UScriptStruct_FBXTLPlayContext_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xca\xb1\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Timeline/BXTLStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xca\xb1\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Instigator_MetaData[] = {
		{ "Category", "BXTLPlayContext" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xca\xbc\xef\xbf\xbd\xef\xbf\xbd\xd9\xb8\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Timeline/BXTLStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xca\xbc\xef\xbf\xbd\xef\xbf\xbd\xd9\xb8\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Triggerer_MetaData[] = {
		{ "Category", "BXTLPlayContext" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Timeline/BXTLStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LockParts_MetaData[] = {
		{ "Category", "BXTLPlayContext" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xc4\xbf\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xd0\xb1\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Timeline/BXTLStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xc4\xbf\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xd0\xb1\xef\xbf\xbd" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Timestamp_MetaData[] = {
		{ "Category", "BXTLPlayContext" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xef\xbf\xbd\xca\xbc\xca\xb1\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Timeline/BXTLStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xef\xbf\xbd\xca\xbc\xca\xb1\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InputDatas_MetaData[] = {
		{ "Category", "BXTLPlayContext" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xef\xbf\xbd\xcc\xac\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xdd\xbc\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Timeline/BXTLStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xef\xbf\xbd\xcc\xac\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xdd\xbc\xef\xbf\xbd" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Instigator;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Triggerer;
	static const UECodeGen_Private::FStructPropertyParams NewProp_LockParts_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_LockParts;
	static const UECodeGen_Private::FInt64PropertyParams NewProp_Timestamp;
	static const UECodeGen_Private::FStructPropertyParams NewProp_InputDatas_ValueProp;
	static const UECodeGen_Private::FNamePropertyParams NewProp_InputDatas_Key_KeyProp;
	static const UECodeGen_Private::FMapPropertyParams NewProp_InputDatas;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FBXTLPlayContext>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UScriptStruct_FBXTLPlayContext_Statics::NewProp_Instigator = { "Instigator", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTLPlayContext, Instigator), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Instigator_MetaData), NewProp_Instigator_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UScriptStruct_FBXTLPlayContext_Statics::NewProp_Triggerer = { "Triggerer", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTLPlayContext, Triggerer), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Triggerer_MetaData), NewProp_Triggerer_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FBXTLPlayContext_Statics::NewProp_LockParts_Inner = { "LockParts", nullptr, (EPropertyFlags)0x0000008000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UScriptStruct_FBXBodyPartSelection, METADATA_PARAMS(0, nullptr) }; // 1620284852
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FBXTLPlayContext_Statics::NewProp_LockParts = { "LockParts", nullptr, (EPropertyFlags)0x0010008000000004, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTLPlayContext, LockParts), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LockParts_MetaData), NewProp_LockParts_MetaData) }; // 1620284852
const UECodeGen_Private::FInt64PropertyParams Z_Construct_UScriptStruct_FBXTLPlayContext_Statics::NewProp_Timestamp = { "Timestamp", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Int64, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTLPlayContext, Timestamp), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Timestamp_MetaData), NewProp_Timestamp_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FBXTLPlayContext_Statics::NewProp_InputDatas_ValueProp = { "InputDatas", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 1, Z_Construct_UScriptStruct_FInstancedStruct, METADATA_PARAMS(0, nullptr) }; // 174410355
const UECodeGen_Private::FNamePropertyParams Z_Construct_UScriptStruct_FBXTLPlayContext_Statics::NewProp_InputDatas_Key_KeyProp = { "InputDatas_Key", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FMapPropertyParams Z_Construct_UScriptStruct_FBXTLPlayContext_Statics::NewProp_InputDatas = { "InputDatas", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Map, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTLPlayContext, InputDatas), EMapPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InputDatas_MetaData), NewProp_InputDatas_MetaData) }; // 174410355
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FBXTLPlayContext_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLPlayContext_Statics::NewProp_Instigator,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLPlayContext_Statics::NewProp_Triggerer,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLPlayContext_Statics::NewProp_LockParts_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLPlayContext_Statics::NewProp_LockParts,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLPlayContext_Statics::NewProp_Timestamp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLPlayContext_Statics::NewProp_InputDatas_ValueProp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLPlayContext_Statics::NewProp_InputDatas_Key_KeyProp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLPlayContext_Statics::NewProp_InputDatas,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXTLPlayContext_Statics::PropPointers) < 2048);
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FBXTLPlayContext_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_BattleX,
	nullptr,
	&NewStructOps,
	"BXTLPlayContext",
	Z_Construct_UScriptStruct_FBXTLPlayContext_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXTLPlayContext_Statics::PropPointers),
	sizeof(FBXTLPlayContext),
	alignof(FBXTLPlayContext),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000005),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXTLPlayContext_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FBXTLPlayContext_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FBXTLPlayContext()
{
	if (!Z_Registration_Info_UScriptStruct_BXTLPlayContext.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_BXTLPlayContext.InnerSingleton, Z_Construct_UScriptStruct_FBXTLPlayContext_Statics::StructParams);
	}
	return Z_Registration_Info_UScriptStruct_BXTLPlayContext.InnerSingleton;
}
// End ScriptStruct FBXTLPlayContext

// Begin Registration
struct Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Timeline_BXTLStructs_h_Statics
{
	static constexpr FStructRegisterCompiledInInfo ScriptStructInfo[] = {
		{ FBXTLPreviewObjectData::StaticStruct, Z_Construct_UScriptStruct_FBXTLPreviewObjectData_Statics::NewStructOps, TEXT("BXTLPreviewObjectData"), &Z_Registration_Info_UScriptStruct_BXTLPreviewObjectData, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FBXTLPreviewObjectData), 479753775U) },
		{ FBXTLKeyFrame::StaticStruct, Z_Construct_UScriptStruct_FBXTLKeyFrame_Statics::NewStructOps, TEXT("BXTLKeyFrame"), &Z_Registration_Info_UScriptStruct_BXTLKeyFrame, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FBXTLKeyFrame), 2356335277U) },
		{ FBXTLPendingTaskInfo::StaticStruct, Z_Construct_UScriptStruct_FBXTLPendingTaskInfo_Statics::NewStructOps, TEXT("BXTLPendingTaskInfo"), &Z_Registration_Info_UScriptStruct_BXTLPendingTaskInfo, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FBXTLPendingTaskInfo), 3248114118U) },
		{ FBXTLBroadcastTaskInfo::StaticStruct, Z_Construct_UScriptStruct_FBXTLBroadcastTaskInfo_Statics::NewStructOps, TEXT("BXTLBroadcastTaskInfo"), &Z_Registration_Info_UScriptStruct_BXTLBroadcastTaskInfo, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FBXTLBroadcastTaskInfo), 1765471375U) },
		{ FBXTLDynamicDataSearchKey::StaticStruct, Z_Construct_UScriptStruct_FBXTLDynamicDataSearchKey_Statics::NewStructOps, TEXT("BXTLDynamicDataSearchKey"), &Z_Registration_Info_UScriptStruct_BXTLDynamicDataSearchKey, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FBXTLDynamicDataSearchKey), 1243550611U) },
		{ FBXTLTaskRTData::StaticStruct, Z_Construct_UScriptStruct_FBXTLTaskRTData_Statics::NewStructOps, TEXT("BXTLTaskRTData"), &Z_Registration_Info_UScriptStruct_BXTLTaskRTData, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FBXTLTaskRTData), 2012750535U) },
		{ FBXTLTaskHostingData::StaticStruct, Z_Construct_UScriptStruct_FBXTLTaskHostingData_Statics::NewStructOps, TEXT("BXTLTaskHostingData"), &Z_Registration_Info_UScriptStruct_BXTLTaskHostingData, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FBXTLTaskHostingData), 627855925U) },
		{ FBXTLSectionRTData::StaticStruct, Z_Construct_UScriptStruct_FBXTLSectionRTData_Statics::NewStructOps, TEXT("BXTLSectionRTData"), &Z_Registration_Info_UScriptStruct_BXTLSectionRTData, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FBXTLSectionRTData), 262691324U) },
		{ FBXTLRunTimeData::StaticStruct, Z_Construct_UScriptStruct_FBXTLRunTimeData_Statics::NewStructOps, TEXT("BXTLRunTimeData"), &Z_Registration_Info_UScriptStruct_BXTLRunTimeData, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FBXTLRunTimeData), 1082839337U) },
		{ FBXTLPlayContext::StaticStruct, Z_Construct_UScriptStruct_FBXTLPlayContext_Statics::NewStructOps, TEXT("BXTLPlayContext"), &Z_Registration_Info_UScriptStruct_BXTLPlayContext, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FBXTLPlayContext), 1147000463U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Timeline_BXTLStructs_h_2889276845(TEXT("/Script/BattleX"),
	nullptr, 0,
	Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Timeline_BXTLStructs_h_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Timeline_BXTLStructs_h_Statics::ScriptStructInfo),
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
