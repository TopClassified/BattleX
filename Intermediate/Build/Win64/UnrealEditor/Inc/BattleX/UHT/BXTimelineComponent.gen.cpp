// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "BattleX/Timeline/BXTimelineComponent.h"
#include "BattleX/Timeline/BXTLStructs.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeBXTimelineComponent() {}

// Begin Cross Module References
BATTLEX_API UClass* Z_Construct_UClass_UBXTimelineComponent();
BATTLEX_API UClass* Z_Construct_UClass_UBXTimelineComponent_NoRegister();
BATTLEX_API UClass* Z_Construct_UClass_UBXTLAsset_NoRegister();
BATTLEX_API UEnum* Z_Construct_UEnum_BattleX_EBXTLFinishReason();
BATTLEX_API UFunction* Z_Construct_UDelegateFunction_BattleX_BXTimelineStarted__DelegateSignature();
BATTLEX_API UFunction* Z_Construct_UDelegateFunction_BattleX_BXTimelineWillEnd__DelegateSignature();
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXTLPlayContext();
ENGINE_API UClass* Z_Construct_UClass_UActorComponent();
UPackage* Z_Construct_UPackage__Script_BattleX();
// End Cross Module References

// Begin Class UBXTimelineComponent Function IsTimelineRunning
struct Z_Construct_UFunction_UBXTimelineComponent_IsTimelineRunning_Statics
{
	struct BXTimelineComponent_eventIsTimelineRunning_Parms
	{
		UBXTLAsset* InAsset;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xca\xb1\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xd4\xb4\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xd1\xaf\xef\xbf\xbd\xc7\xb7\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xd0\xb5\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Timeline/BXTimelineComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xca\xb1\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xd4\xb4\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xd1\xaf\xef\xbf\xbd\xc7\xb7\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xd0\xb5\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_InAsset;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UBXTimelineComponent_IsTimelineRunning_Statics::NewProp_InAsset = { "InAsset", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXTimelineComponent_eventIsTimelineRunning_Parms, InAsset), Z_Construct_UClass_UBXTLAsset_NoRegister, METADATA_PARAMS(0, nullptr) };
void Z_Construct_UFunction_UBXTimelineComponent_IsTimelineRunning_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((BXTimelineComponent_eventIsTimelineRunning_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UBXTimelineComponent_IsTimelineRunning_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(BXTimelineComponent_eventIsTimelineRunning_Parms), &Z_Construct_UFunction_UBXTimelineComponent_IsTimelineRunning_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UBXTimelineComponent_IsTimelineRunning_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXTimelineComponent_IsTimelineRunning_Statics::NewProp_InAsset,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXTimelineComponent_IsTimelineRunning_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXTimelineComponent_IsTimelineRunning_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UBXTimelineComponent_IsTimelineRunning_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UBXTimelineComponent, nullptr, "IsTimelineRunning", nullptr, nullptr, Z_Construct_UFunction_UBXTimelineComponent_IsTimelineRunning_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UBXTimelineComponent_IsTimelineRunning_Statics::PropPointers), sizeof(Z_Construct_UFunction_UBXTimelineComponent_IsTimelineRunning_Statics::BXTimelineComponent_eventIsTimelineRunning_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020400, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXTimelineComponent_IsTimelineRunning_Statics::Function_MetaDataParams), Z_Construct_UFunction_UBXTimelineComponent_IsTimelineRunning_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UBXTimelineComponent_IsTimelineRunning_Statics::BXTimelineComponent_eventIsTimelineRunning_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UBXTimelineComponent_IsTimelineRunning()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UBXTimelineComponent_IsTimelineRunning_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UBXTimelineComponent::execIsTimelineRunning)
{
	P_GET_OBJECT(UBXTLAsset,Z_Param_InAsset);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->IsTimelineRunning(Z_Param_InAsset);
	P_NATIVE_END;
}
// End Class UBXTimelineComponent Function IsTimelineRunning

// Begin Class UBXTimelineComponent Function IsTimelineRunningByID
struct Z_Construct_UFunction_UBXTimelineComponent_IsTimelineRunningByID_Statics
{
	struct BXTimelineComponent_eventIsTimelineRunningByID_Parms
	{
		int64 InID;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbdID\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xd1\xaf\xef\xbf\xbd\xc7\xb7\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xd0\xb5\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Timeline/BXTimelineComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbdID\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xd1\xaf\xef\xbf\xbd\xc7\xb7\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xd0\xb5\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FInt64PropertyParams NewProp_InID;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FInt64PropertyParams Z_Construct_UFunction_UBXTimelineComponent_IsTimelineRunningByID_Statics::NewProp_InID = { "InID", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Int64, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXTimelineComponent_eventIsTimelineRunningByID_Parms, InID), METADATA_PARAMS(0, nullptr) };
void Z_Construct_UFunction_UBXTimelineComponent_IsTimelineRunningByID_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((BXTimelineComponent_eventIsTimelineRunningByID_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UBXTimelineComponent_IsTimelineRunningByID_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(BXTimelineComponent_eventIsTimelineRunningByID_Parms), &Z_Construct_UFunction_UBXTimelineComponent_IsTimelineRunningByID_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UBXTimelineComponent_IsTimelineRunningByID_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXTimelineComponent_IsTimelineRunningByID_Statics::NewProp_InID,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXTimelineComponent_IsTimelineRunningByID_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXTimelineComponent_IsTimelineRunningByID_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UBXTimelineComponent_IsTimelineRunningByID_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UBXTimelineComponent, nullptr, "IsTimelineRunningByID", nullptr, nullptr, Z_Construct_UFunction_UBXTimelineComponent_IsTimelineRunningByID_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UBXTimelineComponent_IsTimelineRunningByID_Statics::PropPointers), sizeof(Z_Construct_UFunction_UBXTimelineComponent_IsTimelineRunningByID_Statics::BXTimelineComponent_eventIsTimelineRunningByID_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020400, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXTimelineComponent_IsTimelineRunningByID_Statics::Function_MetaDataParams), Z_Construct_UFunction_UBXTimelineComponent_IsTimelineRunningByID_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UBXTimelineComponent_IsTimelineRunningByID_Statics::BXTimelineComponent_eventIsTimelineRunningByID_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UBXTimelineComponent_IsTimelineRunningByID()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UBXTimelineComponent_IsTimelineRunningByID_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UBXTimelineComponent::execIsTimelineRunningByID)
{
	P_GET_PROPERTY(FInt64Property,Z_Param_InID);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->IsTimelineRunningByID(Z_Param_InID);
	P_NATIVE_END;
}
// End Class UBXTimelineComponent Function IsTimelineRunningByID

// Begin Class UBXTimelineComponent Function PlayTimeline
struct Z_Construct_UFunction_UBXTimelineComponent_PlayTimeline_Statics
{
	struct BXTimelineComponent_eventPlayTimeline_Parms
	{
		UBXTLAsset* InAsset;
		FBXTLPlayContext InContext;
		int64 ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xca\xb1\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Timeline/BXTimelineComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xca\xb1\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_InAsset;
	static const UECodeGen_Private::FStructPropertyParams NewProp_InContext;
	static const UECodeGen_Private::FInt64PropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UBXTimelineComponent_PlayTimeline_Statics::NewProp_InAsset = { "InAsset", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXTimelineComponent_eventPlayTimeline_Parms, InAsset), Z_Construct_UClass_UBXTLAsset_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXTimelineComponent_PlayTimeline_Statics::NewProp_InContext = { "InContext", nullptr, (EPropertyFlags)0x0010008008000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXTimelineComponent_eventPlayTimeline_Parms, InContext), Z_Construct_UScriptStruct_FBXTLPlayContext, METADATA_PARAMS(0, nullptr) }; // 1147000463
const UECodeGen_Private::FInt64PropertyParams Z_Construct_UFunction_UBXTimelineComponent_PlayTimeline_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Int64, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXTimelineComponent_eventPlayTimeline_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UBXTimelineComponent_PlayTimeline_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXTimelineComponent_PlayTimeline_Statics::NewProp_InAsset,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXTimelineComponent_PlayTimeline_Statics::NewProp_InContext,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXTimelineComponent_PlayTimeline_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXTimelineComponent_PlayTimeline_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UBXTimelineComponent_PlayTimeline_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UBXTimelineComponent, nullptr, "PlayTimeline", nullptr, nullptr, Z_Construct_UFunction_UBXTimelineComponent_PlayTimeline_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UBXTimelineComponent_PlayTimeline_Statics::PropPointers), sizeof(Z_Construct_UFunction_UBXTimelineComponent_PlayTimeline_Statics::BXTimelineComponent_eventPlayTimeline_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04420400, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXTimelineComponent_PlayTimeline_Statics::Function_MetaDataParams), Z_Construct_UFunction_UBXTimelineComponent_PlayTimeline_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UBXTimelineComponent_PlayTimeline_Statics::BXTimelineComponent_eventPlayTimeline_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UBXTimelineComponent_PlayTimeline()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UBXTimelineComponent_PlayTimeline_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UBXTimelineComponent::execPlayTimeline)
{
	P_GET_OBJECT(UBXTLAsset,Z_Param_InAsset);
	P_GET_STRUCT_REF(FBXTLPlayContext,Z_Param_Out_InContext);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(int64*)Z_Param__Result=P_THIS->PlayTimeline(Z_Param_InAsset,Z_Param_Out_InContext);
	P_NATIVE_END;
}
// End Class UBXTimelineComponent Function PlayTimeline

// Begin Class UBXTimelineComponent Function ScriptReceiveTimelineWillFinish
struct BXTimelineComponent_eventScriptReceiveTimelineWillFinish_Parms
{
	int64 InID;
	EBXTLFinishReason InReason;
};
static FName NAME_UBXTimelineComponent_ScriptReceiveTimelineWillFinish = FName(TEXT("ScriptReceiveTimelineWillFinish"));
void UBXTimelineComponent::ScriptReceiveTimelineWillFinish(int64 InID, EBXTLFinishReason InReason)
{
	BXTimelineComponent_eventScriptReceiveTimelineWillFinish_Parms Parms;
	Parms.InID=InID;
	Parms.InReason=InReason;
	ProcessEvent(FindFunctionChecked(NAME_UBXTimelineComponent_ScriptReceiveTimelineWillFinish),&Parms);
}
struct Z_Construct_UFunction_UBXTimelineComponent_ScriptReceiveTimelineWillFinish_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Timeline/BXTimelineComponent.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FInt64PropertyParams NewProp_InID;
	static const UECodeGen_Private::FBytePropertyParams NewProp_InReason_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_InReason;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FInt64PropertyParams Z_Construct_UFunction_UBXTimelineComponent_ScriptReceiveTimelineWillFinish_Statics::NewProp_InID = { "InID", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Int64, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXTimelineComponent_eventScriptReceiveTimelineWillFinish_Parms, InID), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UBXTimelineComponent_ScriptReceiveTimelineWillFinish_Statics::NewProp_InReason_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UBXTimelineComponent_ScriptReceiveTimelineWillFinish_Statics::NewProp_InReason = { "InReason", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXTimelineComponent_eventScriptReceiveTimelineWillFinish_Parms, InReason), Z_Construct_UEnum_BattleX_EBXTLFinishReason, METADATA_PARAMS(0, nullptr) }; // 4111085863
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UBXTimelineComponent_ScriptReceiveTimelineWillFinish_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXTimelineComponent_ScriptReceiveTimelineWillFinish_Statics::NewProp_InID,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXTimelineComponent_ScriptReceiveTimelineWillFinish_Statics::NewProp_InReason_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXTimelineComponent_ScriptReceiveTimelineWillFinish_Statics::NewProp_InReason,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXTimelineComponent_ScriptReceiveTimelineWillFinish_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UBXTimelineComponent_ScriptReceiveTimelineWillFinish_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UBXTimelineComponent, nullptr, "ScriptReceiveTimelineWillFinish", nullptr, nullptr, Z_Construct_UFunction_UBXTimelineComponent_ScriptReceiveTimelineWillFinish_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UBXTimelineComponent_ScriptReceiveTimelineWillFinish_Statics::PropPointers), sizeof(BXTimelineComponent_eventScriptReceiveTimelineWillFinish_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08020800, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXTimelineComponent_ScriptReceiveTimelineWillFinish_Statics::Function_MetaDataParams), Z_Construct_UFunction_UBXTimelineComponent_ScriptReceiveTimelineWillFinish_Statics::Function_MetaDataParams) };
static_assert(sizeof(BXTimelineComponent_eventScriptReceiveTimelineWillFinish_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UBXTimelineComponent_ScriptReceiveTimelineWillFinish()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UBXTimelineComponent_ScriptReceiveTimelineWillFinish_Statics::FuncParams);
	}
	return ReturnFunction;
}
// End Class UBXTimelineComponent Function ScriptReceiveTimelineWillFinish

// Begin Class UBXTimelineComponent Function StopTimeline
struct Z_Construct_UFunction_UBXTimelineComponent_StopTimeline_Statics
{
	struct BXTimelineComponent_eventStopTimeline_Parms
	{
		int64 InID;
		EBXTLFinishReason InReason;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xcd\xa3\xd6\xb9\xca\xb1\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Timeline/BXTimelineComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xcd\xa3\xd6\xb9\xca\xb1\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FInt64PropertyParams NewProp_InID;
	static const UECodeGen_Private::FBytePropertyParams NewProp_InReason_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_InReason;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FInt64PropertyParams Z_Construct_UFunction_UBXTimelineComponent_StopTimeline_Statics::NewProp_InID = { "InID", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Int64, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXTimelineComponent_eventStopTimeline_Parms, InID), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UBXTimelineComponent_StopTimeline_Statics::NewProp_InReason_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UBXTimelineComponent_StopTimeline_Statics::NewProp_InReason = { "InReason", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXTimelineComponent_eventStopTimeline_Parms, InReason), Z_Construct_UEnum_BattleX_EBXTLFinishReason, METADATA_PARAMS(0, nullptr) }; // 4111085863
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UBXTimelineComponent_StopTimeline_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXTimelineComponent_StopTimeline_Statics::NewProp_InID,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXTimelineComponent_StopTimeline_Statics::NewProp_InReason_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXTimelineComponent_StopTimeline_Statics::NewProp_InReason,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXTimelineComponent_StopTimeline_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UBXTimelineComponent_StopTimeline_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UBXTimelineComponent, nullptr, "StopTimeline", nullptr, nullptr, Z_Construct_UFunction_UBXTimelineComponent_StopTimeline_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UBXTimelineComponent_StopTimeline_Statics::PropPointers), sizeof(Z_Construct_UFunction_UBXTimelineComponent_StopTimeline_Statics::BXTimelineComponent_eventStopTimeline_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020400, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXTimelineComponent_StopTimeline_Statics::Function_MetaDataParams), Z_Construct_UFunction_UBXTimelineComponent_StopTimeline_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UBXTimelineComponent_StopTimeline_Statics::BXTimelineComponent_eventStopTimeline_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UBXTimelineComponent_StopTimeline()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UBXTimelineComponent_StopTimeline_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UBXTimelineComponent::execStopTimeline)
{
	P_GET_PROPERTY(FInt64Property,Z_Param_InID);
	P_GET_ENUM(EBXTLFinishReason,Z_Param_InReason);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->StopTimeline(Z_Param_InID,EBXTLFinishReason(Z_Param_InReason));
	P_NATIVE_END;
}
// End Class UBXTimelineComponent Function StopTimeline

// Begin Class UBXTimelineComponent
void UBXTimelineComponent::StaticRegisterNativesUBXTimelineComponent()
{
	UClass* Class = UBXTimelineComponent::StaticClass();
	static const FNameNativePtrPair Funcs[] = {
		{ "IsTimelineRunning", &UBXTimelineComponent::execIsTimelineRunning },
		{ "IsTimelineRunningByID", &UBXTimelineComponent::execIsTimelineRunningByID },
		{ "PlayTimeline", &UBXTimelineComponent::execPlayTimeline },
		{ "StopTimeline", &UBXTimelineComponent::execStopTimeline },
	};
	FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UBXTimelineComponent);
UClass* Z_Construct_UClass_UBXTimelineComponent_NoRegister()
{
	return UBXTimelineComponent::StaticClass();
}
struct Z_Construct_UClass_UBXTimelineComponent_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "BlueprintSpawnableComponent", "" },
		{ "ClassGroupNames", "Custom" },
		{ "IncludePath", "Timeline/BXTimelineComponent.h" },
		{ "ModuleRelativePath", "Timeline/BXTimelineComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_TimelineRunTimeDataIDs_MetaData[] = {
		{ "Category", "BXTimelineComponent" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xca\xb1\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbdID\xef\xbf\xbd\xd0\xb1\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Timeline/BXTimelineComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xca\xb1\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbdID\xef\xbf\xbd\xd0\xb1\xef\xbf\xbd" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_TimelineStarted_MetaData[] = {
		{ "ModuleRelativePath", "Timeline/BXTimelineComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_TimelineWillEnd_MetaData[] = {
		{ "ModuleRelativePath", "Timeline/BXTimelineComponent.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FInt64PropertyParams NewProp_TimelineRunTimeDataIDs_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_TimelineRunTimeDataIDs;
	static const UECodeGen_Private::FMulticastDelegatePropertyParams NewProp_TimelineStarted;
	static const UECodeGen_Private::FMulticastDelegatePropertyParams NewProp_TimelineWillEnd;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UBXTimelineComponent_IsTimelineRunning, "IsTimelineRunning" }, // 6636216
		{ &Z_Construct_UFunction_UBXTimelineComponent_IsTimelineRunningByID, "IsTimelineRunningByID" }, // 460099288
		{ &Z_Construct_UFunction_UBXTimelineComponent_PlayTimeline, "PlayTimeline" }, // 1348514805
		{ &Z_Construct_UFunction_UBXTimelineComponent_ScriptReceiveTimelineWillFinish, "ScriptReceiveTimelineWillFinish" }, // 3155939126
		{ &Z_Construct_UFunction_UBXTimelineComponent_StopTimeline, "StopTimeline" }, // 47875673
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UBXTimelineComponent>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FInt64PropertyParams Z_Construct_UClass_UBXTimelineComponent_Statics::NewProp_TimelineRunTimeDataIDs_Inner = { "TimelineRunTimeDataIDs", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Int64, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UClass_UBXTimelineComponent_Statics::NewProp_TimelineRunTimeDataIDs = { "TimelineRunTimeDataIDs", nullptr, (EPropertyFlags)0x0020080000002004, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXTimelineComponent, TimelineRunTimeDataIDs), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_TimelineRunTimeDataIDs_MetaData), NewProp_TimelineRunTimeDataIDs_MetaData) };
const UECodeGen_Private::FMulticastDelegatePropertyParams Z_Construct_UClass_UBXTimelineComponent_Statics::NewProp_TimelineStarted = { "TimelineStarted", nullptr, (EPropertyFlags)0x0010000010080000, UECodeGen_Private::EPropertyGenFlags::InlineMulticastDelegate, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXTimelineComponent, TimelineStarted), Z_Construct_UDelegateFunction_BattleX_BXTimelineStarted__DelegateSignature, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_TimelineStarted_MetaData), NewProp_TimelineStarted_MetaData) }; // 1311972880
const UECodeGen_Private::FMulticastDelegatePropertyParams Z_Construct_UClass_UBXTimelineComponent_Statics::NewProp_TimelineWillEnd = { "TimelineWillEnd", nullptr, (EPropertyFlags)0x0010000010080000, UECodeGen_Private::EPropertyGenFlags::InlineMulticastDelegate, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXTimelineComponent, TimelineWillEnd), Z_Construct_UDelegateFunction_BattleX_BXTimelineWillEnd__DelegateSignature, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_TimelineWillEnd_MetaData), NewProp_TimelineWillEnd_MetaData) }; // 2084244405
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UBXTimelineComponent_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXTimelineComponent_Statics::NewProp_TimelineRunTimeDataIDs_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXTimelineComponent_Statics::NewProp_TimelineRunTimeDataIDs,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXTimelineComponent_Statics::NewProp_TimelineStarted,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXTimelineComponent_Statics::NewProp_TimelineWillEnd,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UBXTimelineComponent_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_UBXTimelineComponent_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UActorComponent,
	(UObject* (*)())Z_Construct_UPackage__Script_BattleX,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UBXTimelineComponent_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UBXTimelineComponent_Statics::ClassParams = {
	&UBXTimelineComponent::StaticClass,
	"Engine",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	Z_Construct_UClass_UBXTimelineComponent_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	UE_ARRAY_COUNT(Z_Construct_UClass_UBXTimelineComponent_Statics::PropPointers),
	0,
	0x00B000A4u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UBXTimelineComponent_Statics::Class_MetaDataParams), Z_Construct_UClass_UBXTimelineComponent_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UBXTimelineComponent()
{
	if (!Z_Registration_Info_UClass_UBXTimelineComponent.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UBXTimelineComponent.OuterSingleton, Z_Construct_UClass_UBXTimelineComponent_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UBXTimelineComponent.OuterSingleton;
}
template<> BATTLEX_API UClass* StaticClass<UBXTimelineComponent>()
{
	return UBXTimelineComponent::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(UBXTimelineComponent);
UBXTimelineComponent::~UBXTimelineComponent() {}
// End Class UBXTimelineComponent

// Begin Registration
struct Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Timeline_BXTimelineComponent_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UBXTimelineComponent, UBXTimelineComponent::StaticClass, TEXT("UBXTimelineComponent"), &Z_Registration_Info_UClass_UBXTimelineComponent, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UBXTimelineComponent), 2057964013U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Timeline_BXTimelineComponent_h_1477046343(TEXT("/Script/BattleX"),
	Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Timeline_BXTimelineComponent_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Timeline_BXTimelineComponent_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
