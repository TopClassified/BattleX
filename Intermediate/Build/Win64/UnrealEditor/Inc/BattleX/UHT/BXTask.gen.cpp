// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "BattleX/Task/BXTask.h"
#include "BattleX/Task/BXTStructs.h"
#include "BattleX/Timeline/BXTLStructs.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeBXTask() {}

// Begin Cross Module References
BATTLEX_API UClass* Z_Construct_UClass_UBXTask();
BATTLEX_API UClass* Z_Construct_UClass_UBXTask_NoRegister();
BATTLEX_API UEnum* Z_Construct_UEnum_BattleX_EBXTLifeType();
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXTEvent();
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXTInputInfo();
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXTLPreviewObjectData();
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXTOutputInfo();
COREUOBJECT_API UClass* Z_Construct_UClass_UObject();
COREUOBJECT_API UClass* Z_Construct_UClass_UObject_NoRegister();
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FSoftObjectPath();
UPackage* Z_Construct_UPackage__Script_BattleX();
// End Cross Module References

// Begin Class UBXTask Function CollectBPResources
struct BXTask_eventCollectBPResources_Parms
{
	TArray<FSoftObjectPath> OutResources;
};
static FName NAME_UBXTask_CollectBPResources = FName(TEXT("CollectBPResources"));
void UBXTask::CollectBPResources(TArray<FSoftObjectPath>& OutResources)
{
	BXTask_eventCollectBPResources_Parms Parms;
	Parms.OutResources=OutResources;
	ProcessEvent(FindFunctionChecked(NAME_UBXTask_CollectBPResources),&Parms);
	OutResources=Parms.OutResources;
}
struct Z_Construct_UFunction_UBXTask_CollectBPResources_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe6\x94\xb6\xe9\x9b\x86\xe8\x93\x9d\xe5\x9b\xbe\xe5\xbc\x95\xe7\x94\xa8\xe8\xb5\x84\xe6\xba\x90\n" },
#endif
		{ "ModuleRelativePath", "Task/BXTask.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe6\x94\xb6\xe9\x9b\x86\xe8\x93\x9d\xe5\x9b\xbe\xe5\xbc\x95\xe7\x94\xa8\xe8\xb5\x84\xe6\xba\x90" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_OutResources_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_OutResources;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXTask_CollectBPResources_Statics::NewProp_OutResources_Inner = { "OutResources", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UScriptStruct_FSoftObjectPath, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UFunction_UBXTask_CollectBPResources_Statics::NewProp_OutResources = { "OutResources", nullptr, (EPropertyFlags)0x0010000000000180, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXTask_eventCollectBPResources_Parms, OutResources), EArrayPropertyFlags::None, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UBXTask_CollectBPResources_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXTask_CollectBPResources_Statics::NewProp_OutResources_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXTask_CollectBPResources_Statics::NewProp_OutResources,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXTask_CollectBPResources_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UBXTask_CollectBPResources_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UBXTask, nullptr, "CollectBPResources", nullptr, nullptr, Z_Construct_UFunction_UBXTask_CollectBPResources_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UBXTask_CollectBPResources_Statics::PropPointers), sizeof(BXTask_eventCollectBPResources_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08420800, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXTask_CollectBPResources_Statics::Function_MetaDataParams), Z_Construct_UFunction_UBXTask_CollectBPResources_Statics::Function_MetaDataParams) };
static_assert(sizeof(BXTask_eventCollectBPResources_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UBXTask_CollectBPResources()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UBXTask_CollectBPResources_Statics::FuncParams);
	}
	return ReturnFunction;
}
// End Class UBXTask Function CollectBPResources

// Begin Class UBXTask Function RefreshDataByPreviewObject
struct BXTask_eventRefreshDataByPreviewObject_Parms
{
	UObject* InObject;
	FBXTLPreviewObjectData InData;
};
#if WITH_EDITOR
static FName NAME_UBXTask_RefreshDataByPreviewObject = FName(TEXT("RefreshDataByPreviewObject"));
void UBXTask::RefreshDataByPreviewObject(UObject* InObject, FBXTLPreviewObjectData const& InData)
{
	BXTask_eventRefreshDataByPreviewObject_Parms Parms;
	Parms.InObject=InObject;
	Parms.InData=InData;
	ProcessEvent(FindFunctionChecked(NAME_UBXTask_RefreshDataByPreviewObject),&Parms);
}
#endif // WITH_EDITOR
#if WITH_EDITOR
struct Z_Construct_UFunction_UBXTask_RefreshDataByPreviewObject_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Task/BXTask.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InData_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_InObject;
	static const UECodeGen_Private::FStructPropertyParams NewProp_InData;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UBXTask_RefreshDataByPreviewObject_Statics::NewProp_InObject = { "InObject", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXTask_eventRefreshDataByPreviewObject_Parms, InObject), Z_Construct_UClass_UObject_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXTask_RefreshDataByPreviewObject_Statics::NewProp_InData = { "InData", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXTask_eventRefreshDataByPreviewObject_Parms, InData), Z_Construct_UScriptStruct_FBXTLPreviewObjectData, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InData_MetaData), NewProp_InData_MetaData) }; // 479753775
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UBXTask_RefreshDataByPreviewObject_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXTask_RefreshDataByPreviewObject_Statics::NewProp_InObject,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXTask_RefreshDataByPreviewObject_Statics::NewProp_InData,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXTask_RefreshDataByPreviewObject_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UBXTask_RefreshDataByPreviewObject_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UBXTask, nullptr, "RefreshDataByPreviewObject", nullptr, nullptr, Z_Construct_UFunction_UBXTask_RefreshDataByPreviewObject_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UBXTask_RefreshDataByPreviewObject_Statics::PropPointers), sizeof(BXTask_eventRefreshDataByPreviewObject_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x28420C00, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXTask_RefreshDataByPreviewObject_Statics::Function_MetaDataParams), Z_Construct_UFunction_UBXTask_RefreshDataByPreviewObject_Statics::Function_MetaDataParams) };
static_assert(sizeof(BXTask_eventRefreshDataByPreviewObject_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UBXTask_RefreshDataByPreviewObject()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UBXTask_RefreshDataByPreviewObject_Statics::FuncParams);
	}
	return ReturnFunction;
}
#endif // WITH_EDITOR
#if WITH_EDITOR
DEFINE_FUNCTION(UBXTask::execRefreshDataByPreviewObject)
{
	P_GET_OBJECT(UObject,Z_Param_InObject);
	P_GET_STRUCT_REF(FBXTLPreviewObjectData,Z_Param_Out_InData);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->RefreshDataByPreviewObject_Implementation(Z_Param_InObject,Z_Param_Out_InData);
	P_NATIVE_END;
}
#endif // WITH_EDITOR
// End Class UBXTask Function RefreshDataByPreviewObject

// Begin Class UBXTask Function ScriptRefreshProperty
struct BXTask_eventScriptRefreshProperty_Parms
{
	bool ReturnValue;

	/** Constructor, initializes return property only **/
	BXTask_eventScriptRefreshProperty_Parms()
		: ReturnValue(false)
	{
	}
};
#if WITH_EDITOR
static FName NAME_UBXTask_ScriptRefreshProperty = FName(TEXT("ScriptRefreshProperty"));
bool UBXTask::ScriptRefreshProperty()
{
	BXTask_eventScriptRefreshProperty_Parms Parms;
	ProcessEvent(FindFunctionChecked(NAME_UBXTask_ScriptRefreshProperty),&Parms);
	return !!Parms.ReturnValue;
}
#endif // WITH_EDITOR
#if WITH_EDITOR
struct Z_Construct_UFunction_UBXTask_ScriptRefreshProperty_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Task/BXTask.h" },
	};
#endif // WITH_METADATA
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
void Z_Construct_UFunction_UBXTask_ScriptRefreshProperty_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((BXTask_eventScriptRefreshProperty_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UBXTask_ScriptRefreshProperty_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(BXTask_eventScriptRefreshProperty_Parms), &Z_Construct_UFunction_UBXTask_ScriptRefreshProperty_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UBXTask_ScriptRefreshProperty_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXTask_ScriptRefreshProperty_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXTask_ScriptRefreshProperty_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UBXTask_ScriptRefreshProperty_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UBXTask, nullptr, "ScriptRefreshProperty", nullptr, nullptr, Z_Construct_UFunction_UBXTask_ScriptRefreshProperty_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UBXTask_ScriptRefreshProperty_Statics::PropPointers), sizeof(BXTask_eventScriptRefreshProperty_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x28020800, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXTask_ScriptRefreshProperty_Statics::Function_MetaDataParams), Z_Construct_UFunction_UBXTask_ScriptRefreshProperty_Statics::Function_MetaDataParams) };
static_assert(sizeof(BXTask_eventScriptRefreshProperty_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UBXTask_ScriptRefreshProperty()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UBXTask_ScriptRefreshProperty_Statics::FuncParams);
	}
	return ReturnFunction;
}
#endif // WITH_EDITOR
// End Class UBXTask Function ScriptRefreshProperty

// Begin Class UBXTask
void UBXTask::StaticRegisterNativesUBXTask()
{
#if WITH_EDITOR
	UClass* Class = UBXTask::StaticClass();
	static const FNameNativePtrPair Funcs[] = {
		{ "RefreshDataByPreviewObject", &UBXTask::execRefreshDataByPreviewObject },
	};
	FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
#endif // WITH_EDITOR
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UBXTask);
UClass* Z_Construct_UClass_UBXTask_NoRegister()
{
	return UBXTask::StaticClass();
}
struct Z_Construct_UClass_UBXTask_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "IncludePath", "Task/BXTask.h" },
		{ "IsBlueprintBase", "true" },
		{ "ModuleRelativePath", "Task/BXTask.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_NetTypes_MetaData[] = {
		{ "Bitmask", "" },
		{ "BitmaskEnum", "EBXTNetType" },
		{ "Category", "Important" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe7\xbd\x91\xe7\xbb\x9c\xe7\xb1\xbb\xe5\x9e\x8b\n" },
#endif
		{ "ModuleRelativePath", "Task/BXTask.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe7\xbd\x91\xe7\xbb\x9c\xe7\xb1\xbb\xe5\x9e\x8b" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_TriggerTypes_MetaData[] = {
		{ "Bitmask", "" },
		{ "BitmaskEnum", "EBXTTriggerType" },
		{ "Category", "Important" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe8\xa7\xa6\xe5\x8f\x91\xe6\x96\xb9\xe5\xbc\x8f\n" },
#endif
		{ "ModuleRelativePath", "Task/BXTask.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe8\xa7\xa6\xe5\x8f\x91\xe6\x96\xb9\xe5\xbc\x8f" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_StartTime_MetaData[] = {
		{ "Category", "Important" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe5\xbc\x80\xe5\xa7\x8b\xe6\x97\xb6\xe9\x97\xb4\n" },
#endif
		{ "ModuleRelativePath", "Task/BXTask.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe5\xbc\x80\xe5\xa7\x8b\xe6\x97\xb6\xe9\x97\xb4" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LifeType_MetaData[] = {
		{ "Category", "Important" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe7\x94\x9f\xe5\x91\xbd\xe5\x91\xa8\xe6\x9c\x9f\xe7\xb1\xbb\xe5\x9e\x8b\n" },
#endif
		{ "ModuleRelativePath", "Task/BXTask.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe7\x94\x9f\xe5\x91\xbd\xe5\x91\xa8\xe6\x9c\x9f\xe7\xb1\xbb\xe5\x9e\x8b" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Duration_MetaData[] = {
		{ "Category", "Important" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe6\x8c\x81\xe7\xbb\xad\xe6\x97\xb6\xe9\x95\xbf\n" },
#endif
		{ "ModuleRelativePath", "Task/BXTask.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe6\x8c\x81\xe7\xbb\xad\xe6\x97\xb6\xe9\x95\xbf" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_TargetTypes_MetaData[] = {
		{ "Bitmask", "" },
		{ "BitmaskEnum", "EBXTTargetType" },
		{ "Category", "Important" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe9\x80\x89\xe5\x8f\x96\xe7\x9a\x84\xe7\x9b\xae\xe6\xa0\x87\n" },
#endif
		{ "ModuleRelativePath", "Task/BXTask.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe9\x80\x89\xe5\x8f\x96\xe7\x9a\x84\xe7\x9b\xae\xe6\xa0\x87" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_CollisionInputDatas_MetaData[] = {
		{ "Category", "Important" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe5\xa6\x82\xe6\x9e\x9c\xe7\x9b\xae\xe6\xa0\x87\xe7\xb1\xbb\xe5\x9e\x8b\xe5\x8c\x85\xe5\x90\xab\"\xe7\xa2\xb0\xe6\x92\x9e\xe7\x9b\xae\xe6\xa0\x87\"\xef\xbc\x8c\xe5\x9c\xa8\xe8\xbf\x99\xe9\x87\x8c\xe6\xb7\xbb\xe5\x8a\xa0\xe7\xa2\xb0\xe6\x92\x9e\xe6\x95\xb0\xe6\x8d\xae\xe6\x9d\xa5\xe6\xba\x90\n" },
#endif
		{ "EditCondition", "(TargetTypes & 16) > 0" },
		{ "EditConditionHides", "" },
		{ "ModuleRelativePath", "Task/BXTask.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe5\xa6\x82\xe6\x9e\x9c\xe7\x9b\xae\xe6\xa0\x87\xe7\xb1\xbb\xe5\x9e\x8b\xe5\x8c\x85\xe5\x90\xab\"\xe7\xa2\xb0\xe6\x92\x9e\xe7\x9b\xae\xe6\xa0\x87\"\xef\xbc\x8c\xe5\x9c\xa8\xe8\xbf\x99\xe9\x87\x8c\xe6\xb7\xbb\xe5\x8a\xa0\xe7\xa2\xb0\xe6\x92\x9e\xe6\x95\xb0\xe6\x8d\xae\xe6\x9d\xa5\xe6\xba\x90" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InputDatas_MetaData[] = {
		{ "Category", "RunTimeData" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe8\xaf\xa5Task\xe7\x9a\x84\xe6\x95\xb0\xe6\x8d\xae\xe8\xaf\xbb\xe5\x8f\x96(\xe8\x8e\xb7\xe5\x8f\x96\"XXX\"Task\xe7\x94\x9f\xe4\xba\xa7\xe7\x9a\x84\xe5\x90\x8d\xe4\xb8\xba\"xxx\"\xe7\x9a\x84\xe6\x95\xb0\xe6\x8d\xae)\n" },
#endif
		{ "ModuleRelativePath", "Task/BXTask.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe8\xaf\xa5Task\xe7\x9a\x84\xe6\x95\xb0\xe6\x8d\xae\xe8\xaf\xbb\xe5\x8f\x96(\xe8\x8e\xb7\xe5\x8f\x96\"XXX\"Task\xe7\x94\x9f\xe4\xba\xa7\xe7\x9a\x84\xe5\x90\x8d\xe4\xb8\xba\"xxx\"\xe7\x9a\x84\xe6\x95\xb0\xe6\x8d\xae)" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_OutputDatas_MetaData[] = {
		{ "Category", "RunTimeData" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe8\xaf\xa5Task\xe7\x9a\x84\xe6\x95\xb0\xe6\x8d\xae\xe8\xbe\x93\xe5\x87\xba(\xe7\x94\x9f\xe4\xba\xa7\xe5\x90\x8d\xe4\xb8\xba\"xxx\"\xe7\x9a\x84\"xxx\"\xe7\xb1\xbb\xe5\x9e\x8b\xe7\x9a\x84\xe6\x95\xb0\xe6\x8d\xae)\n" },
#endif
		{ "ModuleRelativePath", "Task/BXTask.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe8\xaf\xa5Task\xe7\x9a\x84\xe6\x95\xb0\xe6\x8d\xae\xe8\xbe\x93\xe5\x87\xba(\xe7\x94\x9f\xe4\xba\xa7\xe5\x90\x8d\xe4\xb8\xba\"xxx\"\xe7\x9a\x84\"xxx\"\xe7\xb1\xbb\xe5\x9e\x8b\xe7\x9a\x84\xe6\x95\xb0\xe6\x8d\xae)" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Events_MetaData[] = {
		{ "Category", "Event" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe4\xba\x8b\xe4\xbb\xb6\xe8\xa7\xa6\xe5\x8f\x91Task\xe5\x88\x97\xe8\xa1\xa8\n" },
#endif
		{ "ModuleRelativePath", "Task/BXTask.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe4\xba\x8b\xe4\xbb\xb6\xe8\xa7\xa6\xe5\x8f\x91Task\xe5\x88\x97\xe8\xa1\xa8" },
#endif
	};
#if WITH_EDITORONLY_DATA
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_DisplayName_MetaData[] = {
		{ "Category", "Editor" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe7\xbc\x96\xe8\xbe\x91\xe5\x99\xa8\xe6\x98\xbe\xe7\xa4\xba\xe5\x90\x8d\xe7\xa7\xb0\n" },
#endif
		{ "ModuleRelativePath", "Task/BXTask.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe7\xbc\x96\xe8\xbe\x91\xe5\x99\xa8\xe6\x98\xbe\xe7\xa4\xba\xe5\x90\x8d\xe7\xa7\xb0" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Annotation_MetaData[] = {
		{ "Category", "Editor" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe7\xbc\x96\xe8\xbe\x91\xe5\x99\xa8\xe6\xb3\xa8\xe9\x87\x8a\n" },
#endif
		{ "ModuleRelativePath", "Task/BXTask.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe7\xbc\x96\xe8\xbe\x91\xe5\x99\xa8\xe6\xb3\xa8\xe9\x87\x8a" },
#endif
	};
#endif // WITH_EDITORONLY_DATA
#endif // WITH_METADATA
	static const UECodeGen_Private::FIntPropertyParams NewProp_NetTypes;
	static const UECodeGen_Private::FIntPropertyParams NewProp_TriggerTypes;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_StartTime;
	static const UECodeGen_Private::FBytePropertyParams NewProp_LifeType_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_LifeType;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_Duration;
	static const UECodeGen_Private::FIntPropertyParams NewProp_TargetTypes;
	static const UECodeGen_Private::FStructPropertyParams NewProp_CollisionInputDatas_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_CollisionInputDatas;
	static const UECodeGen_Private::FStructPropertyParams NewProp_InputDatas_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_InputDatas;
	static const UECodeGen_Private::FStructPropertyParams NewProp_OutputDatas_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_OutputDatas;
	static const UECodeGen_Private::FStructPropertyParams NewProp_Events_ValueProp;
	static const UECodeGen_Private::FNamePropertyParams NewProp_Events_Key_KeyProp;
	static const UECodeGen_Private::FMapPropertyParams NewProp_Events;
#if WITH_EDITORONLY_DATA
	static const UECodeGen_Private::FTextPropertyParams NewProp_DisplayName;
	static const UECodeGen_Private::FTextPropertyParams NewProp_Annotation;
#endif // WITH_EDITORONLY_DATA
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UBXTask_CollectBPResources, "CollectBPResources" }, // 2765849919
#if WITH_EDITOR
		{ &Z_Construct_UFunction_UBXTask_RefreshDataByPreviewObject, "RefreshDataByPreviewObject" }, // 2513038069
		{ &Z_Construct_UFunction_UBXTask_ScriptRefreshProperty, "ScriptRefreshProperty" }, // 2447545397
#endif // WITH_EDITOR
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UBXTask>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FIntPropertyParams Z_Construct_UClass_UBXTask_Statics::NewProp_NetTypes = { "NetTypes", nullptr, (EPropertyFlags)0x0010000000010005, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXTask, NetTypes), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_NetTypes_MetaData), NewProp_NetTypes_MetaData) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UClass_UBXTask_Statics::NewProp_TriggerTypes = { "TriggerTypes", nullptr, (EPropertyFlags)0x0010000000010005, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXTask, TriggerTypes), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_TriggerTypes_MetaData), NewProp_TriggerTypes_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UBXTask_Statics::NewProp_StartTime = { "StartTime", nullptr, (EPropertyFlags)0x0010000000010005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXTask, StartTime), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_StartTime_MetaData), NewProp_StartTime_MetaData) };
const UECodeGen_Private::FBytePropertyParams Z_Construct_UClass_UBXTask_Statics::NewProp_LifeType_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UClass_UBXTask_Statics::NewProp_LifeType = { "LifeType", nullptr, (EPropertyFlags)0x0010000000010005, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXTask, LifeType), Z_Construct_UEnum_BattleX_EBXTLifeType, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LifeType_MetaData), NewProp_LifeType_MetaData) }; // 1739170628
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UBXTask_Statics::NewProp_Duration = { "Duration", nullptr, (EPropertyFlags)0x0010000000010005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXTask, Duration), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Duration_MetaData), NewProp_Duration_MetaData) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UClass_UBXTask_Statics::NewProp_TargetTypes = { "TargetTypes", nullptr, (EPropertyFlags)0x0010000000010005, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXTask, TargetTypes), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_TargetTypes_MetaData), NewProp_TargetTypes_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UBXTask_Statics::NewProp_CollisionInputDatas_Inner = { "CollisionInputDatas", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UScriptStruct_FBXTInputInfo, METADATA_PARAMS(0, nullptr) }; // 579686498
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UClass_UBXTask_Statics::NewProp_CollisionInputDatas = { "CollisionInputDatas", nullptr, (EPropertyFlags)0x0010000000010005, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXTask, CollisionInputDatas), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_CollisionInputDatas_MetaData), NewProp_CollisionInputDatas_MetaData) }; // 579686498
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UBXTask_Statics::NewProp_InputDatas_Inner = { "InputDatas", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UScriptStruct_FBXTInputInfo, METADATA_PARAMS(0, nullptr) }; // 579686498
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UClass_UBXTask_Statics::NewProp_InputDatas = { "InputDatas", nullptr, (EPropertyFlags)0x0010000000010005, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXTask, InputDatas), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InputDatas_MetaData), NewProp_InputDatas_MetaData) }; // 579686498
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UBXTask_Statics::NewProp_OutputDatas_Inner = { "OutputDatas", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UScriptStruct_FBXTOutputInfo, METADATA_PARAMS(0, nullptr) }; // 513134278
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UClass_UBXTask_Statics::NewProp_OutputDatas = { "OutputDatas", nullptr, (EPropertyFlags)0x0010000000010005, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXTask, OutputDatas), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_OutputDatas_MetaData), NewProp_OutputDatas_MetaData) }; // 513134278
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UBXTask_Statics::NewProp_Events_ValueProp = { "Events", nullptr, (EPropertyFlags)0x0000000000000001, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 1, Z_Construct_UScriptStruct_FBXTEvent, METADATA_PARAMS(0, nullptr) }; // 2135766493
const UECodeGen_Private::FNamePropertyParams Z_Construct_UClass_UBXTask_Statics::NewProp_Events_Key_KeyProp = { "Events_Key", nullptr, (EPropertyFlags)0x0000000000000001, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FMapPropertyParams Z_Construct_UClass_UBXTask_Statics::NewProp_Events = { "Events", nullptr, (EPropertyFlags)0x0010000000010005, UECodeGen_Private::EPropertyGenFlags::Map, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXTask, Events), EMapPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Events_MetaData), NewProp_Events_MetaData) }; // 2135766493
#if WITH_EDITORONLY_DATA
const UECodeGen_Private::FTextPropertyParams Z_Construct_UClass_UBXTask_Statics::NewProp_DisplayName = { "DisplayName", nullptr, (EPropertyFlags)0x0020080800010001, UECodeGen_Private::EPropertyGenFlags::Text, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXTask, DisplayName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_DisplayName_MetaData), NewProp_DisplayName_MetaData) };
const UECodeGen_Private::FTextPropertyParams Z_Construct_UClass_UBXTask_Statics::NewProp_Annotation = { "Annotation", nullptr, (EPropertyFlags)0x0020080800010001, UECodeGen_Private::EPropertyGenFlags::Text, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXTask, Annotation), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Annotation_MetaData), NewProp_Annotation_MetaData) };
#endif // WITH_EDITORONLY_DATA
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UBXTask_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXTask_Statics::NewProp_NetTypes,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXTask_Statics::NewProp_TriggerTypes,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXTask_Statics::NewProp_StartTime,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXTask_Statics::NewProp_LifeType_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXTask_Statics::NewProp_LifeType,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXTask_Statics::NewProp_Duration,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXTask_Statics::NewProp_TargetTypes,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXTask_Statics::NewProp_CollisionInputDatas_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXTask_Statics::NewProp_CollisionInputDatas,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXTask_Statics::NewProp_InputDatas_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXTask_Statics::NewProp_InputDatas,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXTask_Statics::NewProp_OutputDatas_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXTask_Statics::NewProp_OutputDatas,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXTask_Statics::NewProp_Events_ValueProp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXTask_Statics::NewProp_Events_Key_KeyProp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXTask_Statics::NewProp_Events,
#if WITH_EDITORONLY_DATA
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXTask_Statics::NewProp_DisplayName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXTask_Statics::NewProp_Annotation,
#endif // WITH_EDITORONLY_DATA
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UBXTask_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_UBXTask_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UObject,
	(UObject* (*)())Z_Construct_UPackage__Script_BattleX,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UBXTask_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UBXTask_Statics::ClassParams = {
	&UBXTask::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	Z_Construct_UClass_UBXTask_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	UE_ARRAY_COUNT(Z_Construct_UClass_UBXTask_Statics::PropPointers),
	0,
	0x001010A1u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UBXTask_Statics::Class_MetaDataParams), Z_Construct_UClass_UBXTask_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UBXTask()
{
	if (!Z_Registration_Info_UClass_UBXTask.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UBXTask.OuterSingleton, Z_Construct_UClass_UBXTask_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UBXTask.OuterSingleton;
}
template<> BATTLEX_API UClass* StaticClass<UBXTask>()
{
	return UBXTask::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(UBXTask);
UBXTask::~UBXTask() {}
// End Class UBXTask

// Begin Registration
struct Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Task_BXTask_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UBXTask, UBXTask::StaticClass, TEXT("UBXTask"), &Z_Registration_Info_UClass_UBXTask, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UBXTask), 640160221U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Task_BXTask_h_612478646(TEXT("/Script/BattleX"),
	Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Task_BXTask_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Task_BXTask_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
