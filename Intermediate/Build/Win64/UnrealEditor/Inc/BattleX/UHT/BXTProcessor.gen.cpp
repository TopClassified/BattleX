// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "BattleX/Task/BXTProcessor.h"
#include "BattleX/Timeline/BXTLStructs.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeBXTProcessor() {}

// Begin Cross Module References
BATTLEX_API UClass* Z_Construct_UClass_UBXTask_NoRegister();
BATTLEX_API UClass* Z_Construct_UClass_UBXTProcessor();
BATTLEX_API UClass* Z_Construct_UClass_UBXTProcessor_NoRegister();
BATTLEX_API UEnum* Z_Construct_UEnum_BattleX_EBXTLFinishReason();
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXTLRunTimeData();
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXTLSectionRTData();
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXTLTaskRTData();
COREUOBJECT_API UClass* Z_Construct_UClass_UObject();
UPackage* Z_Construct_UPackage__Script_BattleX();
// End Cross Module References

// Begin Class UBXTProcessor Function AddPendingTask
struct Z_Construct_UFunction_UBXTProcessor_AddPendingTask_Statics
{
	struct BXTProcessor_eventAddPendingTask_Parms
	{
		FBXTLRunTimeData InOutRTData;
		FBXTLSectionRTData InOutRTSData;
		FBXTLTaskRTData InOutRTTData;
		FName InEventName;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "API" },
		{ "ModuleRelativePath", "Task/BXTProcessor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InEventName_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_InOutRTData;
	static const UECodeGen_Private::FStructPropertyParams NewProp_InOutRTSData;
	static const UECodeGen_Private::FStructPropertyParams NewProp_InOutRTTData;
	static const UECodeGen_Private::FNamePropertyParams NewProp_InEventName;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXTProcessor_AddPendingTask_Statics::NewProp_InOutRTData = { "InOutRTData", nullptr, (EPropertyFlags)0x0010008008000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXTProcessor_eventAddPendingTask_Parms, InOutRTData), Z_Construct_UScriptStruct_FBXTLRunTimeData, METADATA_PARAMS(0, nullptr) }; // 1082839337
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXTProcessor_AddPendingTask_Statics::NewProp_InOutRTSData = { "InOutRTSData", nullptr, (EPropertyFlags)0x0010000008000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXTProcessor_eventAddPendingTask_Parms, InOutRTSData), Z_Construct_UScriptStruct_FBXTLSectionRTData, METADATA_PARAMS(0, nullptr) }; // 262691324
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXTProcessor_AddPendingTask_Statics::NewProp_InOutRTTData = { "InOutRTTData", nullptr, (EPropertyFlags)0x0010000008000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXTProcessor_eventAddPendingTask_Parms, InOutRTTData), Z_Construct_UScriptStruct_FBXTLTaskRTData, METADATA_PARAMS(0, nullptr) }; // 2012750535
const UECodeGen_Private::FNamePropertyParams Z_Construct_UFunction_UBXTProcessor_AddPendingTask_Statics::NewProp_InEventName = { "InEventName", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXTProcessor_eventAddPendingTask_Parms, InEventName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InEventName_MetaData), NewProp_InEventName_MetaData) };
void Z_Construct_UFunction_UBXTProcessor_AddPendingTask_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((BXTProcessor_eventAddPendingTask_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UBXTProcessor_AddPendingTask_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(BXTProcessor_eventAddPendingTask_Parms), &Z_Construct_UFunction_UBXTProcessor_AddPendingTask_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UBXTProcessor_AddPendingTask_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXTProcessor_AddPendingTask_Statics::NewProp_InOutRTData,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXTProcessor_AddPendingTask_Statics::NewProp_InOutRTSData,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXTProcessor_AddPendingTask_Statics::NewProp_InOutRTTData,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXTProcessor_AddPendingTask_Statics::NewProp_InEventName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXTProcessor_AddPendingTask_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXTProcessor_AddPendingTask_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UBXTProcessor_AddPendingTask_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UBXTProcessor, nullptr, "AddPendingTask", nullptr, nullptr, Z_Construct_UFunction_UBXTProcessor_AddPendingTask_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UBXTProcessor_AddPendingTask_Statics::PropPointers), sizeof(Z_Construct_UFunction_UBXTProcessor_AddPendingTask_Statics::BXTProcessor_eventAddPendingTask_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04422401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXTProcessor_AddPendingTask_Statics::Function_MetaDataParams), Z_Construct_UFunction_UBXTProcessor_AddPendingTask_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UBXTProcessor_AddPendingTask_Statics::BXTProcessor_eventAddPendingTask_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UBXTProcessor_AddPendingTask()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UBXTProcessor_AddPendingTask_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UBXTProcessor::execAddPendingTask)
{
	P_GET_STRUCT_REF(FBXTLRunTimeData,Z_Param_Out_InOutRTData);
	P_GET_STRUCT_REF(FBXTLSectionRTData,Z_Param_Out_InOutRTSData);
	P_GET_STRUCT_REF(FBXTLTaskRTData,Z_Param_Out_InOutRTTData);
	P_GET_PROPERTY_REF(FNameProperty,Z_Param_Out_InEventName);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=UBXTProcessor::AddPendingTask(Z_Param_Out_InOutRTData,Z_Param_Out_InOutRTSData,Z_Param_Out_InOutRTTData,Z_Param_Out_InEventName);
	P_NATIVE_END;
}
// End Class UBXTProcessor Function AddPendingTask

// Begin Class UBXTProcessor Function EndTask
struct Z_Construct_UFunction_UBXTProcessor_EndTask_Statics
{
	struct BXTProcessor_eventEndTask_Parms
	{
		FBXTLRunTimeData InOutRTData;
		FBXTLSectionRTData InOutRTSData;
		FBXTLTaskRTData InOutRTTData;
		UBXTask* InTask;
		EBXTLFinishReason InReason;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "API" },
		{ "ModuleRelativePath", "Task/BXTProcessor.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_InOutRTData;
	static const UECodeGen_Private::FStructPropertyParams NewProp_InOutRTSData;
	static const UECodeGen_Private::FStructPropertyParams NewProp_InOutRTTData;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_InTask;
	static const UECodeGen_Private::FBytePropertyParams NewProp_InReason_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_InReason;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXTProcessor_EndTask_Statics::NewProp_InOutRTData = { "InOutRTData", nullptr, (EPropertyFlags)0x0010008008000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXTProcessor_eventEndTask_Parms, InOutRTData), Z_Construct_UScriptStruct_FBXTLRunTimeData, METADATA_PARAMS(0, nullptr) }; // 1082839337
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXTProcessor_EndTask_Statics::NewProp_InOutRTSData = { "InOutRTSData", nullptr, (EPropertyFlags)0x0010000008000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXTProcessor_eventEndTask_Parms, InOutRTSData), Z_Construct_UScriptStruct_FBXTLSectionRTData, METADATA_PARAMS(0, nullptr) }; // 262691324
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXTProcessor_EndTask_Statics::NewProp_InOutRTTData = { "InOutRTTData", nullptr, (EPropertyFlags)0x0010000008000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXTProcessor_eventEndTask_Parms, InOutRTTData), Z_Construct_UScriptStruct_FBXTLTaskRTData, METADATA_PARAMS(0, nullptr) }; // 2012750535
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UBXTProcessor_EndTask_Statics::NewProp_InTask = { "InTask", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXTProcessor_eventEndTask_Parms, InTask), Z_Construct_UClass_UBXTask_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UBXTProcessor_EndTask_Statics::NewProp_InReason_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UBXTProcessor_EndTask_Statics::NewProp_InReason = { "InReason", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXTProcessor_eventEndTask_Parms, InReason), Z_Construct_UEnum_BattleX_EBXTLFinishReason, METADATA_PARAMS(0, nullptr) }; // 4111085863
void Z_Construct_UFunction_UBXTProcessor_EndTask_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((BXTProcessor_eventEndTask_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UBXTProcessor_EndTask_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(BXTProcessor_eventEndTask_Parms), &Z_Construct_UFunction_UBXTProcessor_EndTask_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UBXTProcessor_EndTask_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXTProcessor_EndTask_Statics::NewProp_InOutRTData,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXTProcessor_EndTask_Statics::NewProp_InOutRTSData,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXTProcessor_EndTask_Statics::NewProp_InOutRTTData,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXTProcessor_EndTask_Statics::NewProp_InTask,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXTProcessor_EndTask_Statics::NewProp_InReason_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXTProcessor_EndTask_Statics::NewProp_InReason,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXTProcessor_EndTask_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXTProcessor_EndTask_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UBXTProcessor_EndTask_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UBXTProcessor, nullptr, "EndTask", nullptr, nullptr, Z_Construct_UFunction_UBXTProcessor_EndTask_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UBXTProcessor_EndTask_Statics::PropPointers), sizeof(Z_Construct_UFunction_UBXTProcessor_EndTask_Statics::BXTProcessor_eventEndTask_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04420401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXTProcessor_EndTask_Statics::Function_MetaDataParams), Z_Construct_UFunction_UBXTProcessor_EndTask_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UBXTProcessor_EndTask_Statics::BXTProcessor_eventEndTask_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UBXTProcessor_EndTask()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UBXTProcessor_EndTask_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UBXTProcessor::execEndTask)
{
	P_GET_STRUCT_REF(FBXTLRunTimeData,Z_Param_Out_InOutRTData);
	P_GET_STRUCT_REF(FBXTLSectionRTData,Z_Param_Out_InOutRTSData);
	P_GET_STRUCT_REF(FBXTLTaskRTData,Z_Param_Out_InOutRTTData);
	P_GET_OBJECT(UBXTask,Z_Param_InTask);
	P_GET_ENUM(EBXTLFinishReason,Z_Param_InReason);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->EndTask(Z_Param_Out_InOutRTData,Z_Param_Out_InOutRTSData,Z_Param_Out_InOutRTTData,Z_Param_InTask,EBXTLFinishReason(Z_Param_InReason));
	P_NATIVE_END;
}
// End Class UBXTProcessor Function EndTask

// Begin Class UBXTProcessor Function IsTaskCompleted
struct Z_Construct_UFunction_UBXTProcessor_IsTaskCompleted_Statics
{
	struct BXTProcessor_eventIsTaskCompleted_Parms
	{
		UBXTask* InTask;
		FBXTLTaskRTData InTaskData;
		EBXTLFinishReason OutReason;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "API" },
		{ "ModuleRelativePath", "Task/BXTProcessor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InTaskData_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_InTask;
	static const UECodeGen_Private::FStructPropertyParams NewProp_InTaskData;
	static const UECodeGen_Private::FBytePropertyParams NewProp_OutReason_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_OutReason;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UBXTProcessor_IsTaskCompleted_Statics::NewProp_InTask = { "InTask", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXTProcessor_eventIsTaskCompleted_Parms, InTask), Z_Construct_UClass_UBXTask_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXTProcessor_IsTaskCompleted_Statics::NewProp_InTaskData = { "InTaskData", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXTProcessor_eventIsTaskCompleted_Parms, InTaskData), Z_Construct_UScriptStruct_FBXTLTaskRTData, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InTaskData_MetaData), NewProp_InTaskData_MetaData) }; // 2012750535
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UBXTProcessor_IsTaskCompleted_Statics::NewProp_OutReason_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UBXTProcessor_IsTaskCompleted_Statics::NewProp_OutReason = { "OutReason", nullptr, (EPropertyFlags)0x0010000000000180, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXTProcessor_eventIsTaskCompleted_Parms, OutReason), Z_Construct_UEnum_BattleX_EBXTLFinishReason, METADATA_PARAMS(0, nullptr) }; // 4111085863
void Z_Construct_UFunction_UBXTProcessor_IsTaskCompleted_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((BXTProcessor_eventIsTaskCompleted_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UBXTProcessor_IsTaskCompleted_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(BXTProcessor_eventIsTaskCompleted_Parms), &Z_Construct_UFunction_UBXTProcessor_IsTaskCompleted_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UBXTProcessor_IsTaskCompleted_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXTProcessor_IsTaskCompleted_Statics::NewProp_InTask,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXTProcessor_IsTaskCompleted_Statics::NewProp_InTaskData,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXTProcessor_IsTaskCompleted_Statics::NewProp_OutReason_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXTProcessor_IsTaskCompleted_Statics::NewProp_OutReason,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXTProcessor_IsTaskCompleted_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXTProcessor_IsTaskCompleted_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UBXTProcessor_IsTaskCompleted_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UBXTProcessor, nullptr, "IsTaskCompleted", nullptr, nullptr, Z_Construct_UFunction_UBXTProcessor_IsTaskCompleted_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UBXTProcessor_IsTaskCompleted_Statics::PropPointers), sizeof(Z_Construct_UFunction_UBXTProcessor_IsTaskCompleted_Statics::BXTProcessor_eventIsTaskCompleted_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04422401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXTProcessor_IsTaskCompleted_Statics::Function_MetaDataParams), Z_Construct_UFunction_UBXTProcessor_IsTaskCompleted_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UBXTProcessor_IsTaskCompleted_Statics::BXTProcessor_eventIsTaskCompleted_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UBXTProcessor_IsTaskCompleted()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UBXTProcessor_IsTaskCompleted_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UBXTProcessor::execIsTaskCompleted)
{
	P_GET_OBJECT(UBXTask,Z_Param_InTask);
	P_GET_STRUCT_REF(FBXTLTaskRTData,Z_Param_Out_InTaskData);
	P_GET_ENUM_REF(EBXTLFinishReason,Z_Param_Out_OutReason);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=UBXTProcessor::IsTaskCompleted(Z_Param_InTask,Z_Param_Out_InTaskData,(EBXTLFinishReason&)(Z_Param_Out_OutReason));
	P_NATIVE_END;
}
// End Class UBXTProcessor Function IsTaskCompleted

// Begin Class UBXTProcessor Function ScriptEnd
struct BXTProcessor_eventScriptEnd_Parms
{
	FBXTLRunTimeData InOutRTData;
	FBXTLSectionRTData InOutRTSData;
	FBXTLTaskRTData InOutRTTData;
	UBXTask* InTask;
	EBXTLFinishReason InReason;
	bool ReturnValue;

	/** Constructor, initializes return property only **/
	BXTProcessor_eventScriptEnd_Parms()
		: ReturnValue(false)
	{
	}
};
static FName NAME_UBXTProcessor_ScriptEnd = FName(TEXT("ScriptEnd"));
bool UBXTProcessor::ScriptEnd(FBXTLRunTimeData& InOutRTData, FBXTLSectionRTData& InOutRTSData, FBXTLTaskRTData& InOutRTTData, UBXTask* InTask, EBXTLFinishReason InReason)
{
	BXTProcessor_eventScriptEnd_Parms Parms;
	Parms.InOutRTData=InOutRTData;
	Parms.InOutRTSData=InOutRTSData;
	Parms.InOutRTTData=InOutRTTData;
	Parms.InTask=InTask;
	Parms.InReason=InReason;
	ProcessEvent(FindFunctionChecked(NAME_UBXTProcessor_ScriptEnd),&Parms);
	InOutRTData=Parms.InOutRTData;
	InOutRTSData=Parms.InOutRTSData;
	InOutRTTData=Parms.InOutRTTData;
	return !!Parms.ReturnValue;
}
struct Z_Construct_UFunction_UBXTProcessor_ScriptEnd_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Important" },
		{ "ModuleRelativePath", "Task/BXTProcessor.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_InOutRTData;
	static const UECodeGen_Private::FStructPropertyParams NewProp_InOutRTSData;
	static const UECodeGen_Private::FStructPropertyParams NewProp_InOutRTTData;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_InTask;
	static const UECodeGen_Private::FBytePropertyParams NewProp_InReason_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_InReason;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXTProcessor_ScriptEnd_Statics::NewProp_InOutRTData = { "InOutRTData", nullptr, (EPropertyFlags)0x0010008008000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXTProcessor_eventScriptEnd_Parms, InOutRTData), Z_Construct_UScriptStruct_FBXTLRunTimeData, METADATA_PARAMS(0, nullptr) }; // 1082839337
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXTProcessor_ScriptEnd_Statics::NewProp_InOutRTSData = { "InOutRTSData", nullptr, (EPropertyFlags)0x0010000008000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXTProcessor_eventScriptEnd_Parms, InOutRTSData), Z_Construct_UScriptStruct_FBXTLSectionRTData, METADATA_PARAMS(0, nullptr) }; // 262691324
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXTProcessor_ScriptEnd_Statics::NewProp_InOutRTTData = { "InOutRTTData", nullptr, (EPropertyFlags)0x0010000008000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXTProcessor_eventScriptEnd_Parms, InOutRTTData), Z_Construct_UScriptStruct_FBXTLTaskRTData, METADATA_PARAMS(0, nullptr) }; // 2012750535
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UBXTProcessor_ScriptEnd_Statics::NewProp_InTask = { "InTask", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXTProcessor_eventScriptEnd_Parms, InTask), Z_Construct_UClass_UBXTask_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UBXTProcessor_ScriptEnd_Statics::NewProp_InReason_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UBXTProcessor_ScriptEnd_Statics::NewProp_InReason = { "InReason", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXTProcessor_eventScriptEnd_Parms, InReason), Z_Construct_UEnum_BattleX_EBXTLFinishReason, METADATA_PARAMS(0, nullptr) }; // 4111085863
void Z_Construct_UFunction_UBXTProcessor_ScriptEnd_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((BXTProcessor_eventScriptEnd_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UBXTProcessor_ScriptEnd_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(BXTProcessor_eventScriptEnd_Parms), &Z_Construct_UFunction_UBXTProcessor_ScriptEnd_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UBXTProcessor_ScriptEnd_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXTProcessor_ScriptEnd_Statics::NewProp_InOutRTData,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXTProcessor_ScriptEnd_Statics::NewProp_InOutRTSData,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXTProcessor_ScriptEnd_Statics::NewProp_InOutRTTData,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXTProcessor_ScriptEnd_Statics::NewProp_InTask,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXTProcessor_ScriptEnd_Statics::NewProp_InReason_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXTProcessor_ScriptEnd_Statics::NewProp_InReason,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXTProcessor_ScriptEnd_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXTProcessor_ScriptEnd_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UBXTProcessor_ScriptEnd_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UBXTProcessor, nullptr, "ScriptEnd", nullptr, nullptr, Z_Construct_UFunction_UBXTProcessor_ScriptEnd_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UBXTProcessor_ScriptEnd_Statics::PropPointers), sizeof(BXTProcessor_eventScriptEnd_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08480800, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXTProcessor_ScriptEnd_Statics::Function_MetaDataParams), Z_Construct_UFunction_UBXTProcessor_ScriptEnd_Statics::Function_MetaDataParams) };
static_assert(sizeof(BXTProcessor_eventScriptEnd_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UBXTProcessor_ScriptEnd()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UBXTProcessor_ScriptEnd_Statics::FuncParams);
	}
	return ReturnFunction;
}
// End Class UBXTProcessor Function ScriptEnd

// Begin Class UBXTProcessor Function ScriptStart
struct BXTProcessor_eventScriptStart_Parms
{
	FBXTLRunTimeData InOutRTData;
	FBXTLSectionRTData InOutRTSData;
	FBXTLTaskRTData InOutRTTData;
	UBXTask* InTask;
	bool ReturnValue;

	/** Constructor, initializes return property only **/
	BXTProcessor_eventScriptStart_Parms()
		: ReturnValue(false)
	{
	}
};
static FName NAME_UBXTProcessor_ScriptStart = FName(TEXT("ScriptStart"));
bool UBXTProcessor::ScriptStart(FBXTLRunTimeData& InOutRTData, FBXTLSectionRTData& InOutRTSData, FBXTLTaskRTData& InOutRTTData, UBXTask* InTask)
{
	BXTProcessor_eventScriptStart_Parms Parms;
	Parms.InOutRTData=InOutRTData;
	Parms.InOutRTSData=InOutRTSData;
	Parms.InOutRTTData=InOutRTTData;
	Parms.InTask=InTask;
	ProcessEvent(FindFunctionChecked(NAME_UBXTProcessor_ScriptStart),&Parms);
	InOutRTData=Parms.InOutRTData;
	InOutRTSData=Parms.InOutRTSData;
	InOutRTTData=Parms.InOutRTTData;
	return !!Parms.ReturnValue;
}
struct Z_Construct_UFunction_UBXTProcessor_ScriptStart_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Important" },
		{ "ModuleRelativePath", "Task/BXTProcessor.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_InOutRTData;
	static const UECodeGen_Private::FStructPropertyParams NewProp_InOutRTSData;
	static const UECodeGen_Private::FStructPropertyParams NewProp_InOutRTTData;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_InTask;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXTProcessor_ScriptStart_Statics::NewProp_InOutRTData = { "InOutRTData", nullptr, (EPropertyFlags)0x0010008008000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXTProcessor_eventScriptStart_Parms, InOutRTData), Z_Construct_UScriptStruct_FBXTLRunTimeData, METADATA_PARAMS(0, nullptr) }; // 1082839337
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXTProcessor_ScriptStart_Statics::NewProp_InOutRTSData = { "InOutRTSData", nullptr, (EPropertyFlags)0x0010000008000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXTProcessor_eventScriptStart_Parms, InOutRTSData), Z_Construct_UScriptStruct_FBXTLSectionRTData, METADATA_PARAMS(0, nullptr) }; // 262691324
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXTProcessor_ScriptStart_Statics::NewProp_InOutRTTData = { "InOutRTTData", nullptr, (EPropertyFlags)0x0010000008000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXTProcessor_eventScriptStart_Parms, InOutRTTData), Z_Construct_UScriptStruct_FBXTLTaskRTData, METADATA_PARAMS(0, nullptr) }; // 2012750535
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UBXTProcessor_ScriptStart_Statics::NewProp_InTask = { "InTask", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXTProcessor_eventScriptStart_Parms, InTask), Z_Construct_UClass_UBXTask_NoRegister, METADATA_PARAMS(0, nullptr) };
void Z_Construct_UFunction_UBXTProcessor_ScriptStart_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((BXTProcessor_eventScriptStart_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UBXTProcessor_ScriptStart_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(BXTProcessor_eventScriptStart_Parms), &Z_Construct_UFunction_UBXTProcessor_ScriptStart_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UBXTProcessor_ScriptStart_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXTProcessor_ScriptStart_Statics::NewProp_InOutRTData,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXTProcessor_ScriptStart_Statics::NewProp_InOutRTSData,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXTProcessor_ScriptStart_Statics::NewProp_InOutRTTData,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXTProcessor_ScriptStart_Statics::NewProp_InTask,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXTProcessor_ScriptStart_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXTProcessor_ScriptStart_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UBXTProcessor_ScriptStart_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UBXTProcessor, nullptr, "ScriptStart", nullptr, nullptr, Z_Construct_UFunction_UBXTProcessor_ScriptStart_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UBXTProcessor_ScriptStart_Statics::PropPointers), sizeof(BXTProcessor_eventScriptStart_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08480800, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXTProcessor_ScriptStart_Statics::Function_MetaDataParams), Z_Construct_UFunction_UBXTProcessor_ScriptStart_Statics::Function_MetaDataParams) };
static_assert(sizeof(BXTProcessor_eventScriptStart_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UBXTProcessor_ScriptStart()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UBXTProcessor_ScriptStart_Statics::FuncParams);
	}
	return ReturnFunction;
}
// End Class UBXTProcessor Function ScriptStart

// Begin Class UBXTProcessor Function ScriptUpdate
struct BXTProcessor_eventScriptUpdate_Parms
{
	FBXTLRunTimeData InOutRTData;
	FBXTLSectionRTData InOutRTSData;
	FBXTLTaskRTData InOutRTTData;
	UBXTask* InTask;
	float InDeltaTime;
	float InTimeRate;
	bool ReturnValue;

	/** Constructor, initializes return property only **/
	BXTProcessor_eventScriptUpdate_Parms()
		: ReturnValue(false)
	{
	}
};
static FName NAME_UBXTProcessor_ScriptUpdate = FName(TEXT("ScriptUpdate"));
bool UBXTProcessor::ScriptUpdate(FBXTLRunTimeData& InOutRTData, FBXTLSectionRTData& InOutRTSData, FBXTLTaskRTData& InOutRTTData, UBXTask* InTask, float InDeltaTime, float InTimeRate)
{
	BXTProcessor_eventScriptUpdate_Parms Parms;
	Parms.InOutRTData=InOutRTData;
	Parms.InOutRTSData=InOutRTSData;
	Parms.InOutRTTData=InOutRTTData;
	Parms.InTask=InTask;
	Parms.InDeltaTime=InDeltaTime;
	Parms.InTimeRate=InTimeRate;
	ProcessEvent(FindFunctionChecked(NAME_UBXTProcessor_ScriptUpdate),&Parms);
	InOutRTData=Parms.InOutRTData;
	InOutRTSData=Parms.InOutRTSData;
	InOutRTTData=Parms.InOutRTTData;
	return !!Parms.ReturnValue;
}
struct Z_Construct_UFunction_UBXTProcessor_ScriptUpdate_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Important" },
		{ "ModuleRelativePath", "Task/BXTProcessor.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_InOutRTData;
	static const UECodeGen_Private::FStructPropertyParams NewProp_InOutRTSData;
	static const UECodeGen_Private::FStructPropertyParams NewProp_InOutRTTData;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_InTask;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_InDeltaTime;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_InTimeRate;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXTProcessor_ScriptUpdate_Statics::NewProp_InOutRTData = { "InOutRTData", nullptr, (EPropertyFlags)0x0010008008000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXTProcessor_eventScriptUpdate_Parms, InOutRTData), Z_Construct_UScriptStruct_FBXTLRunTimeData, METADATA_PARAMS(0, nullptr) }; // 1082839337
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXTProcessor_ScriptUpdate_Statics::NewProp_InOutRTSData = { "InOutRTSData", nullptr, (EPropertyFlags)0x0010000008000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXTProcessor_eventScriptUpdate_Parms, InOutRTSData), Z_Construct_UScriptStruct_FBXTLSectionRTData, METADATA_PARAMS(0, nullptr) }; // 262691324
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXTProcessor_ScriptUpdate_Statics::NewProp_InOutRTTData = { "InOutRTTData", nullptr, (EPropertyFlags)0x0010000008000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXTProcessor_eventScriptUpdate_Parms, InOutRTTData), Z_Construct_UScriptStruct_FBXTLTaskRTData, METADATA_PARAMS(0, nullptr) }; // 2012750535
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UBXTProcessor_ScriptUpdate_Statics::NewProp_InTask = { "InTask", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXTProcessor_eventScriptUpdate_Parms, InTask), Z_Construct_UClass_UBXTask_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UBXTProcessor_ScriptUpdate_Statics::NewProp_InDeltaTime = { "InDeltaTime", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXTProcessor_eventScriptUpdate_Parms, InDeltaTime), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UBXTProcessor_ScriptUpdate_Statics::NewProp_InTimeRate = { "InTimeRate", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXTProcessor_eventScriptUpdate_Parms, InTimeRate), METADATA_PARAMS(0, nullptr) };
void Z_Construct_UFunction_UBXTProcessor_ScriptUpdate_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((BXTProcessor_eventScriptUpdate_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UBXTProcessor_ScriptUpdate_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(BXTProcessor_eventScriptUpdate_Parms), &Z_Construct_UFunction_UBXTProcessor_ScriptUpdate_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UBXTProcessor_ScriptUpdate_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXTProcessor_ScriptUpdate_Statics::NewProp_InOutRTData,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXTProcessor_ScriptUpdate_Statics::NewProp_InOutRTSData,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXTProcessor_ScriptUpdate_Statics::NewProp_InOutRTTData,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXTProcessor_ScriptUpdate_Statics::NewProp_InTask,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXTProcessor_ScriptUpdate_Statics::NewProp_InDeltaTime,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXTProcessor_ScriptUpdate_Statics::NewProp_InTimeRate,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXTProcessor_ScriptUpdate_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXTProcessor_ScriptUpdate_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UBXTProcessor_ScriptUpdate_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UBXTProcessor, nullptr, "ScriptUpdate", nullptr, nullptr, Z_Construct_UFunction_UBXTProcessor_ScriptUpdate_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UBXTProcessor_ScriptUpdate_Statics::PropPointers), sizeof(BXTProcessor_eventScriptUpdate_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08480800, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXTProcessor_ScriptUpdate_Statics::Function_MetaDataParams), Z_Construct_UFunction_UBXTProcessor_ScriptUpdate_Statics::Function_MetaDataParams) };
static_assert(sizeof(BXTProcessor_eventScriptUpdate_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UBXTProcessor_ScriptUpdate()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UBXTProcessor_ScriptUpdate_Statics::FuncParams);
	}
	return ReturnFunction;
}
// End Class UBXTProcessor Function ScriptUpdate

// Begin Class UBXTProcessor Function StartTask
struct Z_Construct_UFunction_UBXTProcessor_StartTask_Statics
{
	struct BXTProcessor_eventStartTask_Parms
	{
		FBXTLRunTimeData InOutRTData;
		FBXTLSectionRTData InOutRTSData;
		FBXTLTaskRTData InOutRTTData;
		UBXTask* InTask;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "API" },
		{ "ModuleRelativePath", "Task/BXTProcessor.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_InOutRTData;
	static const UECodeGen_Private::FStructPropertyParams NewProp_InOutRTSData;
	static const UECodeGen_Private::FStructPropertyParams NewProp_InOutRTTData;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_InTask;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXTProcessor_StartTask_Statics::NewProp_InOutRTData = { "InOutRTData", nullptr, (EPropertyFlags)0x0010008008000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXTProcessor_eventStartTask_Parms, InOutRTData), Z_Construct_UScriptStruct_FBXTLRunTimeData, METADATA_PARAMS(0, nullptr) }; // 1082839337
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXTProcessor_StartTask_Statics::NewProp_InOutRTSData = { "InOutRTSData", nullptr, (EPropertyFlags)0x0010000008000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXTProcessor_eventStartTask_Parms, InOutRTSData), Z_Construct_UScriptStruct_FBXTLSectionRTData, METADATA_PARAMS(0, nullptr) }; // 262691324
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXTProcessor_StartTask_Statics::NewProp_InOutRTTData = { "InOutRTTData", nullptr, (EPropertyFlags)0x0010000008000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXTProcessor_eventStartTask_Parms, InOutRTTData), Z_Construct_UScriptStruct_FBXTLTaskRTData, METADATA_PARAMS(0, nullptr) }; // 2012750535
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UBXTProcessor_StartTask_Statics::NewProp_InTask = { "InTask", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXTProcessor_eventStartTask_Parms, InTask), Z_Construct_UClass_UBXTask_NoRegister, METADATA_PARAMS(0, nullptr) };
void Z_Construct_UFunction_UBXTProcessor_StartTask_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((BXTProcessor_eventStartTask_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UBXTProcessor_StartTask_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(BXTProcessor_eventStartTask_Parms), &Z_Construct_UFunction_UBXTProcessor_StartTask_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UBXTProcessor_StartTask_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXTProcessor_StartTask_Statics::NewProp_InOutRTData,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXTProcessor_StartTask_Statics::NewProp_InOutRTSData,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXTProcessor_StartTask_Statics::NewProp_InOutRTTData,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXTProcessor_StartTask_Statics::NewProp_InTask,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXTProcessor_StartTask_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXTProcessor_StartTask_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UBXTProcessor_StartTask_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UBXTProcessor, nullptr, "StartTask", nullptr, nullptr, Z_Construct_UFunction_UBXTProcessor_StartTask_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UBXTProcessor_StartTask_Statics::PropPointers), sizeof(Z_Construct_UFunction_UBXTProcessor_StartTask_Statics::BXTProcessor_eventStartTask_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04420401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXTProcessor_StartTask_Statics::Function_MetaDataParams), Z_Construct_UFunction_UBXTProcessor_StartTask_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UBXTProcessor_StartTask_Statics::BXTProcessor_eventStartTask_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UBXTProcessor_StartTask()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UBXTProcessor_StartTask_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UBXTProcessor::execStartTask)
{
	P_GET_STRUCT_REF(FBXTLRunTimeData,Z_Param_Out_InOutRTData);
	P_GET_STRUCT_REF(FBXTLSectionRTData,Z_Param_Out_InOutRTSData);
	P_GET_STRUCT_REF(FBXTLTaskRTData,Z_Param_Out_InOutRTTData);
	P_GET_OBJECT(UBXTask,Z_Param_InTask);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->StartTask(Z_Param_Out_InOutRTData,Z_Param_Out_InOutRTSData,Z_Param_Out_InOutRTTData,Z_Param_InTask);
	P_NATIVE_END;
}
// End Class UBXTProcessor Function StartTask

// Begin Class UBXTProcessor Function UpdateTask
struct Z_Construct_UFunction_UBXTProcessor_UpdateTask_Statics
{
	struct BXTProcessor_eventUpdateTask_Parms
	{
		FBXTLRunTimeData InOutRTData;
		FBXTLSectionRTData InOutRTSData;
		FBXTLTaskRTData InOutRTTData;
		UBXTask* InTask;
		float InDeltaTime;
		float InTimeRate;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "API" },
		{ "CPP_Default_InTimeRate", "1.000000" },
		{ "ModuleRelativePath", "Task/BXTProcessor.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_InOutRTData;
	static const UECodeGen_Private::FStructPropertyParams NewProp_InOutRTSData;
	static const UECodeGen_Private::FStructPropertyParams NewProp_InOutRTTData;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_InTask;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_InDeltaTime;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_InTimeRate;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXTProcessor_UpdateTask_Statics::NewProp_InOutRTData = { "InOutRTData", nullptr, (EPropertyFlags)0x0010008008000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXTProcessor_eventUpdateTask_Parms, InOutRTData), Z_Construct_UScriptStruct_FBXTLRunTimeData, METADATA_PARAMS(0, nullptr) }; // 1082839337
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXTProcessor_UpdateTask_Statics::NewProp_InOutRTSData = { "InOutRTSData", nullptr, (EPropertyFlags)0x0010000008000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXTProcessor_eventUpdateTask_Parms, InOutRTSData), Z_Construct_UScriptStruct_FBXTLSectionRTData, METADATA_PARAMS(0, nullptr) }; // 262691324
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXTProcessor_UpdateTask_Statics::NewProp_InOutRTTData = { "InOutRTTData", nullptr, (EPropertyFlags)0x0010000008000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXTProcessor_eventUpdateTask_Parms, InOutRTTData), Z_Construct_UScriptStruct_FBXTLTaskRTData, METADATA_PARAMS(0, nullptr) }; // 2012750535
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UBXTProcessor_UpdateTask_Statics::NewProp_InTask = { "InTask", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXTProcessor_eventUpdateTask_Parms, InTask), Z_Construct_UClass_UBXTask_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UBXTProcessor_UpdateTask_Statics::NewProp_InDeltaTime = { "InDeltaTime", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXTProcessor_eventUpdateTask_Parms, InDeltaTime), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UBXTProcessor_UpdateTask_Statics::NewProp_InTimeRate = { "InTimeRate", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXTProcessor_eventUpdateTask_Parms, InTimeRate), METADATA_PARAMS(0, nullptr) };
void Z_Construct_UFunction_UBXTProcessor_UpdateTask_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((BXTProcessor_eventUpdateTask_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UBXTProcessor_UpdateTask_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(BXTProcessor_eventUpdateTask_Parms), &Z_Construct_UFunction_UBXTProcessor_UpdateTask_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UBXTProcessor_UpdateTask_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXTProcessor_UpdateTask_Statics::NewProp_InOutRTData,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXTProcessor_UpdateTask_Statics::NewProp_InOutRTSData,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXTProcessor_UpdateTask_Statics::NewProp_InOutRTTData,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXTProcessor_UpdateTask_Statics::NewProp_InTask,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXTProcessor_UpdateTask_Statics::NewProp_InDeltaTime,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXTProcessor_UpdateTask_Statics::NewProp_InTimeRate,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXTProcessor_UpdateTask_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXTProcessor_UpdateTask_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UBXTProcessor_UpdateTask_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UBXTProcessor, nullptr, "UpdateTask", nullptr, nullptr, Z_Construct_UFunction_UBXTProcessor_UpdateTask_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UBXTProcessor_UpdateTask_Statics::PropPointers), sizeof(Z_Construct_UFunction_UBXTProcessor_UpdateTask_Statics::BXTProcessor_eventUpdateTask_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04420401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXTProcessor_UpdateTask_Statics::Function_MetaDataParams), Z_Construct_UFunction_UBXTProcessor_UpdateTask_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UBXTProcessor_UpdateTask_Statics::BXTProcessor_eventUpdateTask_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UBXTProcessor_UpdateTask()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UBXTProcessor_UpdateTask_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UBXTProcessor::execUpdateTask)
{
	P_GET_STRUCT_REF(FBXTLRunTimeData,Z_Param_Out_InOutRTData);
	P_GET_STRUCT_REF(FBXTLSectionRTData,Z_Param_Out_InOutRTSData);
	P_GET_STRUCT_REF(FBXTLTaskRTData,Z_Param_Out_InOutRTTData);
	P_GET_OBJECT(UBXTask,Z_Param_InTask);
	P_GET_PROPERTY(FFloatProperty,Z_Param_InDeltaTime);
	P_GET_PROPERTY(FFloatProperty,Z_Param_InTimeRate);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->UpdateTask(Z_Param_Out_InOutRTData,Z_Param_Out_InOutRTSData,Z_Param_Out_InOutRTTData,Z_Param_InTask,Z_Param_InDeltaTime,Z_Param_InTimeRate);
	P_NATIVE_END;
}
// End Class UBXTProcessor Function UpdateTask

// Begin Class UBXTProcessor
void UBXTProcessor::StaticRegisterNativesUBXTProcessor()
{
	UClass* Class = UBXTProcessor::StaticClass();
	static const FNameNativePtrPair Funcs[] = {
		{ "AddPendingTask", &UBXTProcessor::execAddPendingTask },
		{ "EndTask", &UBXTProcessor::execEndTask },
		{ "IsTaskCompleted", &UBXTProcessor::execIsTaskCompleted },
		{ "StartTask", &UBXTProcessor::execStartTask },
		{ "UpdateTask", &UBXTProcessor::execUpdateTask },
	};
	FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UBXTProcessor);
UClass* Z_Construct_UClass_UBXTProcessor_NoRegister()
{
	return UBXTProcessor::StaticClass();
}
struct Z_Construct_UClass_UBXTProcessor_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "IncludePath", "Task/BXTProcessor.h" },
		{ "IsBlueprintBase", "true" },
		{ "ModuleRelativePath", "Task/BXTProcessor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ExecuteFunctions_MetaData[] = {
		{ "Bitmask", "" },
		{ "BitmaskEnum", "EBXTProcessorFunction" },
		{ "Category", "Important" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe8\xa6\x81\xe6\x89\xa7\xe8\xa1\x8c\xe7\x9a\x84\xe5\x87\xbd\xe6\x95\xb0(\xe9\xbb\x98\xe8\xae\xa4\xe5\x8f\xaa\xe6\x89\xa7\xe8\xa1\x8c""C++\xe5\x87\xbd\xe6\x95\xb0)\n" },
#endif
		{ "ModuleRelativePath", "Task/BXTProcessor.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe8\xa6\x81\xe6\x89\xa7\xe8\xa1\x8c\xe7\x9a\x84\xe5\x87\xbd\xe6\x95\xb0(\xe9\xbb\x98\xe8\xae\xa4\xe5\x8f\xaa\xe6\x89\xa7\xe8\xa1\x8c""C++\xe5\x87\xbd\xe6\x95\xb0)" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FIntPropertyParams NewProp_ExecuteFunctions;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UBXTProcessor_AddPendingTask, "AddPendingTask" }, // 3491015807
		{ &Z_Construct_UFunction_UBXTProcessor_EndTask, "EndTask" }, // 1463613588
		{ &Z_Construct_UFunction_UBXTProcessor_IsTaskCompleted, "IsTaskCompleted" }, // 2187364783
		{ &Z_Construct_UFunction_UBXTProcessor_ScriptEnd, "ScriptEnd" }, // 1725606828
		{ &Z_Construct_UFunction_UBXTProcessor_ScriptStart, "ScriptStart" }, // 2704606316
		{ &Z_Construct_UFunction_UBXTProcessor_ScriptUpdate, "ScriptUpdate" }, // 3684443804
		{ &Z_Construct_UFunction_UBXTProcessor_StartTask, "StartTask" }, // 2850856129
		{ &Z_Construct_UFunction_UBXTProcessor_UpdateTask, "UpdateTask" }, // 4283281210
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UBXTProcessor>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FIntPropertyParams Z_Construct_UClass_UBXTProcessor_Statics::NewProp_ExecuteFunctions = { "ExecuteFunctions", nullptr, (EPropertyFlags)0x0020080000010001, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXTProcessor, ExecuteFunctions), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ExecuteFunctions_MetaData), NewProp_ExecuteFunctions_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UBXTProcessor_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXTProcessor_Statics::NewProp_ExecuteFunctions,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UBXTProcessor_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_UBXTProcessor_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UObject,
	(UObject* (*)())Z_Construct_UPackage__Script_BattleX,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UBXTProcessor_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UBXTProcessor_Statics::ClassParams = {
	&UBXTProcessor::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	Z_Construct_UClass_UBXTProcessor_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	UE_ARRAY_COUNT(Z_Construct_UClass_UBXTProcessor_Statics::PropPointers),
	0,
	0x001000A9u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UBXTProcessor_Statics::Class_MetaDataParams), Z_Construct_UClass_UBXTProcessor_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UBXTProcessor()
{
	if (!Z_Registration_Info_UClass_UBXTProcessor.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UBXTProcessor.OuterSingleton, Z_Construct_UClass_UBXTProcessor_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UBXTProcessor.OuterSingleton;
}
template<> BATTLEX_API UClass* StaticClass<UBXTProcessor>()
{
	return UBXTProcessor::StaticClass();
}
UBXTProcessor::UBXTProcessor(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UBXTProcessor);
UBXTProcessor::~UBXTProcessor() {}
// End Class UBXTProcessor

// Begin Registration
struct Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Task_BXTProcessor_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UBXTProcessor, UBXTProcessor::StaticClass, TEXT("UBXTProcessor"), &Z_Registration_Info_UClass_UBXTProcessor, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UBXTProcessor), 2139196276U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Task_BXTProcessor_h_3802024493(TEXT("/Script/BattleX"),
	Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Task_BXTProcessor_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Task_BXTProcessor_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
