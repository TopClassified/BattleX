// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "BattleX/BXManager.h"
#include "BattleX/BXStructs.h"
#include "BattleX/Timeline/BXTLStructs.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeBXManager() {}

// Begin Cross Module References
BATTLEX_API UClass* Z_Construct_UClass_UBXManager();
BATTLEX_API UClass* Z_Construct_UClass_UBXManager_NoRegister();
BATTLEX_API UClass* Z_Construct_UClass_UBXTLAsset_NoRegister();
BATTLEX_API UClass* Z_Construct_UClass_UBXTProcessor_NoRegister();
BATTLEX_API UEnum* Z_Construct_UEnum_BattleX_EBXTLFinishReason();
BATTLEX_API UFunction* Z_Construct_UDelegateFunction_BattleX_BXTimelineStarted__DelegateSignature();
BATTLEX_API UFunction* Z_Construct_UDelegateFunction_BattleX_BXTimelineWillEnd__DelegateSignature();
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXTimelineAssetInformation();
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXTLPlayContext();
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXTLRunTimeData();
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXTLSectionRTData();
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXTLTaskHostingData();
COREUOBJECT_API UClass* Z_Construct_UClass_UClass();
COREUOBJECT_API UClass* Z_Construct_UClass_UObject();
COREUOBJECT_API UClass* Z_Construct_UClass_UObject_NoRegister();
COREUOBJECT_API UClass* Z_Construct_UClass_UScriptStruct();
ENGINE_API UClass* Z_Construct_UClass_AActor_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_UWorld_NoRegister();
UPackage* Z_Construct_UPackage__Script_BattleX();
// End Cross Module References

// Begin Delegate FBXTimelineStarted
struct Z_Construct_UDelegateFunction_BattleX_BXTimelineStarted__DelegateSignature_Statics
{
	struct _Script_BattleX_eventBXTimelineStarted_Parms
	{
		int64 TimelineID;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "BXManager.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FInt64PropertyParams NewProp_TimelineID;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FInt64PropertyParams Z_Construct_UDelegateFunction_BattleX_BXTimelineStarted__DelegateSignature_Statics::NewProp_TimelineID = { "TimelineID", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Int64, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(_Script_BattleX_eventBXTimelineStarted_Parms, TimelineID), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UDelegateFunction_BattleX_BXTimelineStarted__DelegateSignature_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_BattleX_BXTimelineStarted__DelegateSignature_Statics::NewProp_TimelineID,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_BattleX_BXTimelineStarted__DelegateSignature_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UDelegateFunction_BattleX_BXTimelineStarted__DelegateSignature_Statics::FuncParams = { (UObject*(*)())Z_Construct_UPackage__Script_BattleX, nullptr, "BXTimelineStarted__DelegateSignature", nullptr, nullptr, Z_Construct_UDelegateFunction_BattleX_BXTimelineStarted__DelegateSignature_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_BattleX_BXTimelineStarted__DelegateSignature_Statics::PropPointers), sizeof(Z_Construct_UDelegateFunction_BattleX_BXTimelineStarted__DelegateSignature_Statics::_Script_BattleX_eventBXTimelineStarted_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00130000, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_BattleX_BXTimelineStarted__DelegateSignature_Statics::Function_MetaDataParams), Z_Construct_UDelegateFunction_BattleX_BXTimelineStarted__DelegateSignature_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UDelegateFunction_BattleX_BXTimelineStarted__DelegateSignature_Statics::_Script_BattleX_eventBXTimelineStarted_Parms) < MAX_uint16);
UFunction* Z_Construct_UDelegateFunction_BattleX_BXTimelineStarted__DelegateSignature()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UDelegateFunction_BattleX_BXTimelineStarted__DelegateSignature_Statics::FuncParams);
	}
	return ReturnFunction;
}
void FBXTimelineStarted_DelegateWrapper(const FMulticastScriptDelegate& BXTimelineStarted, int64 TimelineID)
{
	struct _Script_BattleX_eventBXTimelineStarted_Parms
	{
		int64 TimelineID;
	};
	_Script_BattleX_eventBXTimelineStarted_Parms Parms;
	Parms.TimelineID=TimelineID;
	BXTimelineStarted.ProcessMulticastDelegate<UObject>(&Parms);
}
// End Delegate FBXTimelineStarted

// Begin Delegate FBXTimelineWillEnd
struct Z_Construct_UDelegateFunction_BattleX_BXTimelineWillEnd__DelegateSignature_Statics
{
	struct _Script_BattleX_eventBXTimelineWillEnd_Parms
	{
		int64 TimelineID;
		EBXTLFinishReason Reason;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "BXManager.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FInt64PropertyParams NewProp_TimelineID;
	static const UECodeGen_Private::FBytePropertyParams NewProp_Reason_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_Reason;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FInt64PropertyParams Z_Construct_UDelegateFunction_BattleX_BXTimelineWillEnd__DelegateSignature_Statics::NewProp_TimelineID = { "TimelineID", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Int64, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(_Script_BattleX_eventBXTimelineWillEnd_Parms, TimelineID), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FBytePropertyParams Z_Construct_UDelegateFunction_BattleX_BXTimelineWillEnd__DelegateSignature_Statics::NewProp_Reason_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UDelegateFunction_BattleX_BXTimelineWillEnd__DelegateSignature_Statics::NewProp_Reason = { "Reason", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(_Script_BattleX_eventBXTimelineWillEnd_Parms, Reason), Z_Construct_UEnum_BattleX_EBXTLFinishReason, METADATA_PARAMS(0, nullptr) }; // 4111085863
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UDelegateFunction_BattleX_BXTimelineWillEnd__DelegateSignature_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_BattleX_BXTimelineWillEnd__DelegateSignature_Statics::NewProp_TimelineID,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_BattleX_BXTimelineWillEnd__DelegateSignature_Statics::NewProp_Reason_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_BattleX_BXTimelineWillEnd__DelegateSignature_Statics::NewProp_Reason,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_BattleX_BXTimelineWillEnd__DelegateSignature_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UDelegateFunction_BattleX_BXTimelineWillEnd__DelegateSignature_Statics::FuncParams = { (UObject*(*)())Z_Construct_UPackage__Script_BattleX, nullptr, "BXTimelineWillEnd__DelegateSignature", nullptr, nullptr, Z_Construct_UDelegateFunction_BattleX_BXTimelineWillEnd__DelegateSignature_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_BattleX_BXTimelineWillEnd__DelegateSignature_Statics::PropPointers), sizeof(Z_Construct_UDelegateFunction_BattleX_BXTimelineWillEnd__DelegateSignature_Statics::_Script_BattleX_eventBXTimelineWillEnd_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00130000, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_BattleX_BXTimelineWillEnd__DelegateSignature_Statics::Function_MetaDataParams), Z_Construct_UDelegateFunction_BattleX_BXTimelineWillEnd__DelegateSignature_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UDelegateFunction_BattleX_BXTimelineWillEnd__DelegateSignature_Statics::_Script_BattleX_eventBXTimelineWillEnd_Parms) < MAX_uint16);
UFunction* Z_Construct_UDelegateFunction_BattleX_BXTimelineWillEnd__DelegateSignature()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UDelegateFunction_BattleX_BXTimelineWillEnd__DelegateSignature_Statics::FuncParams);
	}
	return ReturnFunction;
}
void FBXTimelineWillEnd_DelegateWrapper(const FMulticastScriptDelegate& BXTimelineWillEnd, int64 TimelineID, EBXTLFinishReason Reason)
{
	struct _Script_BattleX_eventBXTimelineWillEnd_Parms
	{
		int64 TimelineID;
		EBXTLFinishReason Reason;
	};
	_Script_BattleX_eventBXTimelineWillEnd_Parms Parms;
	Parms.TimelineID=TimelineID;
	Parms.Reason=Reason;
	BXTimelineWillEnd.ProcessMulticastDelegate<UObject>(&Parms);
}
// End Delegate FBXTimelineWillEnd

// Begin Class UBXManager Function ChangeShowCollision
struct Z_Construct_UFunction_UBXManager_ChangeShowCollision_Statics
{
	struct BXManager_eventChangeShowCollision_Parms
	{
		bool InShow;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "BXManager.h" },
	};
#endif // WITH_METADATA
	static void NewProp_InShow_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_InShow;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
void Z_Construct_UFunction_UBXManager_ChangeShowCollision_Statics::NewProp_InShow_SetBit(void* Obj)
{
	((BXManager_eventChangeShowCollision_Parms*)Obj)->InShow = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UBXManager_ChangeShowCollision_Statics::NewProp_InShow = { "InShow", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(BXManager_eventChangeShowCollision_Parms), &Z_Construct_UFunction_UBXManager_ChangeShowCollision_Statics::NewProp_InShow_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UBXManager_ChangeShowCollision_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXManager_ChangeShowCollision_Statics::NewProp_InShow,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXManager_ChangeShowCollision_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UBXManager_ChangeShowCollision_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UBXManager, nullptr, "ChangeShowCollision", nullptr, nullptr, Z_Construct_UFunction_UBXManager_ChangeShowCollision_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UBXManager_ChangeShowCollision_Statics::PropPointers), sizeof(Z_Construct_UFunction_UBXManager_ChangeShowCollision_Statics::BXManager_eventChangeShowCollision_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXManager_ChangeShowCollision_Statics::Function_MetaDataParams), Z_Construct_UFunction_UBXManager_ChangeShowCollision_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UBXManager_ChangeShowCollision_Statics::BXManager_eventChangeShowCollision_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UBXManager_ChangeShowCollision()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UBXManager_ChangeShowCollision_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UBXManager::execChangeShowCollision)
{
	P_GET_UBOOL(Z_Param_InShow);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->ChangeShowCollision(Z_Param_InShow);
	P_NATIVE_END;
}
// End Class UBXManager Function ChangeShowCollision

// Begin Class UBXManager Function Get
struct Z_Construct_UFunction_UBXManager_Get_Statics
{
	struct BXManager_eventGet_Parms
	{
		UObject* InWorldContext;
		UBXManager* ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "BXManager.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_InWorldContext;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UBXManager_Get_Statics::NewProp_InWorldContext = { "InWorldContext", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXManager_eventGet_Parms, InWorldContext), Z_Construct_UClass_UObject_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UBXManager_Get_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXManager_eventGet_Parms, ReturnValue), Z_Construct_UClass_UBXManager_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UBXManager_Get_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXManager_Get_Statics::NewProp_InWorldContext,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXManager_Get_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXManager_Get_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UBXManager_Get_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UBXManager, nullptr, "Get", nullptr, nullptr, Z_Construct_UFunction_UBXManager_Get_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UBXManager_Get_Statics::PropPointers), sizeof(Z_Construct_UFunction_UBXManager_Get_Statics::BXManager_eventGet_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04022401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXManager_Get_Statics::Function_MetaDataParams), Z_Construct_UFunction_UBXManager_Get_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UBXManager_Get_Statics::BXManager_eventGet_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UBXManager_Get()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UBXManager_Get_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UBXManager::execGet)
{
	P_GET_OBJECT(UObject,Z_Param_InWorldContext);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(UBXManager**)Z_Param__Result=UBXManager::Get(Z_Param_InWorldContext);
	P_NATIVE_END;
}
// End Class UBXManager Function Get

// Begin Class UBXManager Function GetShowCollision
struct Z_Construct_UFunction_UBXManager_GetShowCollision_Statics
{
	struct BXManager_eventGetShowCollision_Parms
	{
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "BXManager.h" },
	};
#endif // WITH_METADATA
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
void Z_Construct_UFunction_UBXManager_GetShowCollision_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((BXManager_eventGetShowCollision_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UBXManager_GetShowCollision_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(BXManager_eventGetShowCollision_Parms), &Z_Construct_UFunction_UBXManager_GetShowCollision_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UBXManager_GetShowCollision_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXManager_GetShowCollision_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXManager_GetShowCollision_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UBXManager_GetShowCollision_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UBXManager, nullptr, "GetShowCollision", nullptr, nullptr, Z_Construct_UFunction_UBXManager_GetShowCollision_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UBXManager_GetShowCollision_Statics::PropPointers), sizeof(Z_Construct_UFunction_UBXManager_GetShowCollision_Statics::BXManager_eventGetShowCollision_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXManager_GetShowCollision_Statics::Function_MetaDataParams), Z_Construct_UFunction_UBXManager_GetShowCollision_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UBXManager_GetShowCollision_Statics::BXManager_eventGetShowCollision_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UBXManager_GetShowCollision()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UBXManager_GetShowCollision_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UBXManager::execGetShowCollision)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->GetShowCollision();
	P_NATIVE_END;
}
// End Class UBXManager Function GetShowCollision

// Begin Class UBXManager Function GetSkillAssetByID
struct Z_Construct_UFunction_UBXManager_GetSkillAssetByID_Statics
{
	struct BXManager_eventGetSkillAssetByID_Parms
	{
		int32 InID;
		TSoftObjectPtr<UBXTLAsset> ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe6\xa0\xb9\xe6\x8d\xaeID\xe6\x89\xbe\xe5\x88\xb0\xe6\x8a\x80\xe8\x83\xbd\xe8\xb5\x84\xe6\xba\x90\n" },
#endif
		{ "ModuleRelativePath", "BXManager.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe6\xa0\xb9\xe6\x8d\xaeID\xe6\x89\xbe\xe5\x88\xb0\xe6\x8a\x80\xe8\x83\xbd\xe8\xb5\x84\xe6\xba\x90" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FIntPropertyParams NewProp_InID;
	static const UECodeGen_Private::FSoftObjectPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UBXManager_GetSkillAssetByID_Statics::NewProp_InID = { "InID", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXManager_eventGetSkillAssetByID_Parms, InID), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FSoftObjectPropertyParams Z_Construct_UFunction_UBXManager_GetSkillAssetByID_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0014000000000580, UECodeGen_Private::EPropertyGenFlags::SoftObject, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXManager_eventGetSkillAssetByID_Parms, ReturnValue), Z_Construct_UClass_UBXTLAsset_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UBXManager_GetSkillAssetByID_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXManager_GetSkillAssetByID_Statics::NewProp_InID,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXManager_GetSkillAssetByID_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXManager_GetSkillAssetByID_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UBXManager_GetSkillAssetByID_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UBXManager, nullptr, "GetSkillAssetByID", nullptr, nullptr, Z_Construct_UFunction_UBXManager_GetSkillAssetByID_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UBXManager_GetSkillAssetByID_Statics::PropPointers), sizeof(Z_Construct_UFunction_UBXManager_GetSkillAssetByID_Statics::BXManager_eventGetSkillAssetByID_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXManager_GetSkillAssetByID_Statics::Function_MetaDataParams), Z_Construct_UFunction_UBXManager_GetSkillAssetByID_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UBXManager_GetSkillAssetByID_Statics::BXManager_eventGetSkillAssetByID_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UBXManager_GetSkillAssetByID()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UBXManager_GetSkillAssetByID_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UBXManager::execGetSkillAssetByID)
{
	P_GET_PROPERTY(FIntProperty,Z_Param_InID);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(TSoftObjectPtr<UBXTLAsset>*)Z_Param__Result=P_THIS->GetSkillAssetByID(Z_Param_InID);
	P_NATIVE_END;
}
// End Class UBXManager Function GetSkillAssetByID

// Begin Class UBXManager Function GetSkillAssetIDs
struct Z_Construct_UFunction_UBXManager_GetSkillAssetIDs_Statics
{
	struct BXManager_eventGetSkillAssetIDs_Parms
	{
		TArray<int32> ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe8\x8e\xb7\xe5\x8f\x96\xe6\x8a\x80\xe8\x83\xbd\xe8\xb5\x84\xe6\xba\x90ID\xe5\x88\x97\xe8\xa1\xa8\n" },
#endif
		{ "ModuleRelativePath", "BXManager.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe8\x8e\xb7\xe5\x8f\x96\xe6\x8a\x80\xe8\x83\xbd\xe8\xb5\x84\xe6\xba\x90ID\xe5\x88\x97\xe8\xa1\xa8" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FIntPropertyParams NewProp_ReturnValue_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UBXManager_GetSkillAssetIDs_Statics::NewProp_ReturnValue_Inner = { "ReturnValue", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UFunction_UBXManager_GetSkillAssetIDs_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXManager_eventGetSkillAssetIDs_Parms, ReturnValue), EArrayPropertyFlags::None, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UBXManager_GetSkillAssetIDs_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXManager_GetSkillAssetIDs_Statics::NewProp_ReturnValue_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXManager_GetSkillAssetIDs_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXManager_GetSkillAssetIDs_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UBXManager_GetSkillAssetIDs_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UBXManager, nullptr, "GetSkillAssetIDs", nullptr, nullptr, Z_Construct_UFunction_UBXManager_GetSkillAssetIDs_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UBXManager_GetSkillAssetIDs_Statics::PropPointers), sizeof(Z_Construct_UFunction_UBXManager_GetSkillAssetIDs_Statics::BXManager_eventGetSkillAssetIDs_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXManager_GetSkillAssetIDs_Statics::Function_MetaDataParams), Z_Construct_UFunction_UBXManager_GetSkillAssetIDs_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UBXManager_GetSkillAssetIDs_Statics::BXManager_eventGetSkillAssetIDs_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UBXManager_GetSkillAssetIDs()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UBXManager_GetSkillAssetIDs_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UBXManager::execGetSkillAssetIDs)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(TArray<int32>*)Z_Param__Result=P_THIS->GetSkillAssetIDs();
	P_NATIVE_END;
}
// End Class UBXManager Function GetSkillAssetIDs

// Begin Class UBXManager Function GetTimelineAssetByID
struct Z_Construct_UFunction_UBXManager_GetTimelineAssetByID_Statics
{
	struct BXManager_eventGetTimelineAssetByID_Parms
	{
		int32 InID;
		TSoftObjectPtr<UBXTLAsset> ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe6\xa0\xb9\xe6\x8d\xaeID\xe6\x89\xbe\xe5\x88\xb0\xe6\x97\xb6\xe9\x97\xb4\xe8\xbd\xb4\xe8\xb5\x84\xe6\xba\x90\n" },
#endif
		{ "ModuleRelativePath", "BXManager.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe6\xa0\xb9\xe6\x8d\xaeID\xe6\x89\xbe\xe5\x88\xb0\xe6\x97\xb6\xe9\x97\xb4\xe8\xbd\xb4\xe8\xb5\x84\xe6\xba\x90" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FIntPropertyParams NewProp_InID;
	static const UECodeGen_Private::FSoftObjectPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UBXManager_GetTimelineAssetByID_Statics::NewProp_InID = { "InID", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXManager_eventGetTimelineAssetByID_Parms, InID), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FSoftObjectPropertyParams Z_Construct_UFunction_UBXManager_GetTimelineAssetByID_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0014000000000580, UECodeGen_Private::EPropertyGenFlags::SoftObject, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXManager_eventGetTimelineAssetByID_Parms, ReturnValue), Z_Construct_UClass_UBXTLAsset_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UBXManager_GetTimelineAssetByID_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXManager_GetTimelineAssetByID_Statics::NewProp_InID,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXManager_GetTimelineAssetByID_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXManager_GetTimelineAssetByID_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UBXManager_GetTimelineAssetByID_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UBXManager, nullptr, "GetTimelineAssetByID", nullptr, nullptr, Z_Construct_UFunction_UBXManager_GetTimelineAssetByID_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UBXManager_GetTimelineAssetByID_Statics::PropPointers), sizeof(Z_Construct_UFunction_UBXManager_GetTimelineAssetByID_Statics::BXManager_eventGetTimelineAssetByID_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXManager_GetTimelineAssetByID_Statics::Function_MetaDataParams), Z_Construct_UFunction_UBXManager_GetTimelineAssetByID_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UBXManager_GetTimelineAssetByID_Statics::BXManager_eventGetTimelineAssetByID_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UBXManager_GetTimelineAssetByID()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UBXManager_GetTimelineAssetByID_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UBXManager::execGetTimelineAssetByID)
{
	P_GET_PROPERTY(FIntProperty,Z_Param_InID);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(TSoftObjectPtr<UBXTLAsset>*)Z_Param__Result=P_THIS->GetTimelineAssetByID(Z_Param_InID);
	P_NATIVE_END;
}
// End Class UBXManager Function GetTimelineAssetByID

// Begin Class UBXManager Function GetTimelineAssetIDs
struct Z_Construct_UFunction_UBXManager_GetTimelineAssetIDs_Statics
{
	struct BXManager_eventGetTimelineAssetIDs_Parms
	{
		TArray<int32> ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe8\x8e\xb7\xe5\x8f\x96\xe6\x97\xb6\xe9\x97\xb4\xe8\xbd\xb4\xe8\xb5\x84\xe6\xba\x90ID\xe5\x88\x97\xe8\xa1\xa8\n" },
#endif
		{ "ModuleRelativePath", "BXManager.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe8\x8e\xb7\xe5\x8f\x96\xe6\x97\xb6\xe9\x97\xb4\xe8\xbd\xb4\xe8\xb5\x84\xe6\xba\x90ID\xe5\x88\x97\xe8\xa1\xa8" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FIntPropertyParams NewProp_ReturnValue_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UBXManager_GetTimelineAssetIDs_Statics::NewProp_ReturnValue_Inner = { "ReturnValue", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UFunction_UBXManager_GetTimelineAssetIDs_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXManager_eventGetTimelineAssetIDs_Parms, ReturnValue), EArrayPropertyFlags::None, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UBXManager_GetTimelineAssetIDs_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXManager_GetTimelineAssetIDs_Statics::NewProp_ReturnValue_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXManager_GetTimelineAssetIDs_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXManager_GetTimelineAssetIDs_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UBXManager_GetTimelineAssetIDs_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UBXManager, nullptr, "GetTimelineAssetIDs", nullptr, nullptr, Z_Construct_UFunction_UBXManager_GetTimelineAssetIDs_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UBXManager_GetTimelineAssetIDs_Statics::PropPointers), sizeof(Z_Construct_UFunction_UBXManager_GetTimelineAssetIDs_Statics::BXManager_eventGetTimelineAssetIDs_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXManager_GetTimelineAssetIDs_Statics::Function_MetaDataParams), Z_Construct_UFunction_UBXManager_GetTimelineAssetIDs_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UBXManager_GetTimelineAssetIDs_Statics::BXManager_eventGetTimelineAssetIDs_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UBXManager_GetTimelineAssetIDs()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UBXManager_GetTimelineAssetIDs_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UBXManager::execGetTimelineAssetIDs)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(TArray<int32>*)Z_Param__Result=P_THIS->GetTimelineAssetIDs();
	P_NATIVE_END;
}
// End Class UBXManager Function GetTimelineAssetIDs

// Begin Class UBXManager Function IsUpdatingTimeline
struct Z_Construct_UFunction_UBXManager_IsUpdatingTimeline_Statics
{
	struct BXManager_eventIsUpdatingTimeline_Parms
	{
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe6\xad\xa3\xe5\x9c\xa8\xe6\x9b\xb4\xe6\x96\xb0\xe6\x97\xb6\xe9\x97\xb4\xe8\xbd\xb4\xe8\xbf\x90\xe8\xa1\x8c\xe6\x95\xb0\xe6\x8d\xae\n" },
#endif
		{ "ModuleRelativePath", "BXManager.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe6\xad\xa3\xe5\x9c\xa8\xe6\x9b\xb4\xe6\x96\xb0\xe6\x97\xb6\xe9\x97\xb4\xe8\xbd\xb4\xe8\xbf\x90\xe8\xa1\x8c\xe6\x95\xb0\xe6\x8d\xae" },
#endif
	};
#endif // WITH_METADATA
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
void Z_Construct_UFunction_UBXManager_IsUpdatingTimeline_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((BXManager_eventIsUpdatingTimeline_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UBXManager_IsUpdatingTimeline_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(BXManager_eventIsUpdatingTimeline_Parms), &Z_Construct_UFunction_UBXManager_IsUpdatingTimeline_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UBXManager_IsUpdatingTimeline_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXManager_IsUpdatingTimeline_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXManager_IsUpdatingTimeline_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UBXManager_IsUpdatingTimeline_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UBXManager, nullptr, "IsUpdatingTimeline", nullptr, nullptr, Z_Construct_UFunction_UBXManager_IsUpdatingTimeline_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UBXManager_IsUpdatingTimeline_Statics::PropPointers), sizeof(Z_Construct_UFunction_UBXManager_IsUpdatingTimeline_Statics::BXManager_eventIsUpdatingTimeline_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXManager_IsUpdatingTimeline_Statics::Function_MetaDataParams), Z_Construct_UFunction_UBXManager_IsUpdatingTimeline_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UBXManager_IsUpdatingTimeline_Statics::BXManager_eventIsUpdatingTimeline_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UBXManager_IsUpdatingTimeline()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UBXManager_IsUpdatingTimeline_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UBXManager::execIsUpdatingTimeline)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->IsUpdatingTimeline();
	P_NATIVE_END;
}
// End Class UBXManager Function IsUpdatingTimeline

// Begin Class UBXManager Function OnWorldCleanupStart
struct Z_Construct_UFunction_UBXManager_OnWorldCleanupStart_Statics
{
	struct BXManager_eventOnWorldCleanupStart_Parms
	{
		UWorld* World;
		bool bSessionEnded;
		bool bCleanupResources;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "BXManager.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_World;
	static void NewProp_bSessionEnded_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bSessionEnded;
	static void NewProp_bCleanupResources_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bCleanupResources;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UBXManager_OnWorldCleanupStart_Statics::NewProp_World = { "World", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXManager_eventOnWorldCleanupStart_Parms, World), Z_Construct_UClass_UWorld_NoRegister, METADATA_PARAMS(0, nullptr) };
void Z_Construct_UFunction_UBXManager_OnWorldCleanupStart_Statics::NewProp_bSessionEnded_SetBit(void* Obj)
{
	((BXManager_eventOnWorldCleanupStart_Parms*)Obj)->bSessionEnded = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UBXManager_OnWorldCleanupStart_Statics::NewProp_bSessionEnded = { "bSessionEnded", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(BXManager_eventOnWorldCleanupStart_Parms), &Z_Construct_UFunction_UBXManager_OnWorldCleanupStart_Statics::NewProp_bSessionEnded_SetBit, METADATA_PARAMS(0, nullptr) };
void Z_Construct_UFunction_UBXManager_OnWorldCleanupStart_Statics::NewProp_bCleanupResources_SetBit(void* Obj)
{
	((BXManager_eventOnWorldCleanupStart_Parms*)Obj)->bCleanupResources = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UBXManager_OnWorldCleanupStart_Statics::NewProp_bCleanupResources = { "bCleanupResources", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(BXManager_eventOnWorldCleanupStart_Parms), &Z_Construct_UFunction_UBXManager_OnWorldCleanupStart_Statics::NewProp_bCleanupResources_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UBXManager_OnWorldCleanupStart_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXManager_OnWorldCleanupStart_Statics::NewProp_World,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXManager_OnWorldCleanupStart_Statics::NewProp_bSessionEnded,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXManager_OnWorldCleanupStart_Statics::NewProp_bCleanupResources,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXManager_OnWorldCleanupStart_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UBXManager_OnWorldCleanupStart_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UBXManager, nullptr, "OnWorldCleanupStart", nullptr, nullptr, Z_Construct_UFunction_UBXManager_OnWorldCleanupStart_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UBXManager_OnWorldCleanupStart_Statics::PropPointers), sizeof(Z_Construct_UFunction_UBXManager_OnWorldCleanupStart_Statics::BXManager_eventOnWorldCleanupStart_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04080401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXManager_OnWorldCleanupStart_Statics::Function_MetaDataParams), Z_Construct_UFunction_UBXManager_OnWorldCleanupStart_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UBXManager_OnWorldCleanupStart_Statics::BXManager_eventOnWorldCleanupStart_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UBXManager_OnWorldCleanupStart()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UBXManager_OnWorldCleanupStart_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UBXManager::execOnWorldCleanupStart)
{
	P_GET_OBJECT(UWorld,Z_Param_World);
	P_GET_UBOOL(Z_Param_bSessionEnded);
	P_GET_UBOOL(Z_Param_bCleanupResources);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->OnWorldCleanupStart(Z_Param_World,Z_Param_bSessionEnded,Z_Param_bCleanupResources);
	P_NATIVE_END;
}
// End Class UBXManager Function OnWorldCleanupStart

// Begin Class UBXManager Function PlayTimeline
struct Z_Construct_UFunction_UBXManager_PlayTimeline_Statics
{
	struct BXManager_eventPlayTimeline_Parms
	{
		UBXTLAsset* InAsset;
		AActor* InOwner;
		FBXTLPlayContext InContext;
		int64 ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe6\x92\xad\xe6\x94\xbe\xe6\x97\xb6\xe9\x97\xb4\xe8\xbd\xb4\n" },
#endif
		{ "ModuleRelativePath", "BXManager.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe6\x92\xad\xe6\x94\xbe\xe6\x97\xb6\xe9\x97\xb4\xe8\xbd\xb4" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_InAsset;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_InOwner;
	static const UECodeGen_Private::FStructPropertyParams NewProp_InContext;
	static const UECodeGen_Private::FInt64PropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UBXManager_PlayTimeline_Statics::NewProp_InAsset = { "InAsset", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXManager_eventPlayTimeline_Parms, InAsset), Z_Construct_UClass_UBXTLAsset_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UBXManager_PlayTimeline_Statics::NewProp_InOwner = { "InOwner", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXManager_eventPlayTimeline_Parms, InOwner), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXManager_PlayTimeline_Statics::NewProp_InContext = { "InContext", nullptr, (EPropertyFlags)0x0010008008000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXManager_eventPlayTimeline_Parms, InContext), Z_Construct_UScriptStruct_FBXTLPlayContext, METADATA_PARAMS(0, nullptr) }; // 1147000463
const UECodeGen_Private::FInt64PropertyParams Z_Construct_UFunction_UBXManager_PlayTimeline_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Int64, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXManager_eventPlayTimeline_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UBXManager_PlayTimeline_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXManager_PlayTimeline_Statics::NewProp_InAsset,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXManager_PlayTimeline_Statics::NewProp_InOwner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXManager_PlayTimeline_Statics::NewProp_InContext,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXManager_PlayTimeline_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXManager_PlayTimeline_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UBXManager_PlayTimeline_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UBXManager, nullptr, "PlayTimeline", nullptr, nullptr, Z_Construct_UFunction_UBXManager_PlayTimeline_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UBXManager_PlayTimeline_Statics::PropPointers), sizeof(Z_Construct_UFunction_UBXManager_PlayTimeline_Statics::BXManager_eventPlayTimeline_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04420401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXManager_PlayTimeline_Statics::Function_MetaDataParams), Z_Construct_UFunction_UBXManager_PlayTimeline_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UBXManager_PlayTimeline_Statics::BXManager_eventPlayTimeline_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UBXManager_PlayTimeline()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UBXManager_PlayTimeline_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UBXManager::execPlayTimeline)
{
	P_GET_OBJECT(UBXTLAsset,Z_Param_InAsset);
	P_GET_OBJECT(AActor,Z_Param_InOwner);
	P_GET_STRUCT_REF(FBXTLPlayContext,Z_Param_Out_InContext);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(int64*)Z_Param__Result=P_THIS->PlayTimeline(Z_Param_InAsset,Z_Param_InOwner,Z_Param_Out_InContext);
	P_NATIVE_END;
}
// End Class UBXManager Function PlayTimeline

// Begin Class UBXManager Function ScriptTick
struct BXManager_eventScriptTick_Parms
{
	float DeltaTime;
};
static FName NAME_UBXManager_ScriptTick = FName(TEXT("ScriptTick"));
void UBXManager::ScriptTick(float DeltaTime)
{
	BXManager_eventScriptTick_Parms Parms;
	Parms.DeltaTime=DeltaTime;
	ProcessEvent(FindFunctionChecked(NAME_UBXManager_ScriptTick),&Parms);
}
struct Z_Construct_UFunction_UBXManager_ScriptTick_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "BXManager.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFloatPropertyParams NewProp_DeltaTime;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UBXManager_ScriptTick_Statics::NewProp_DeltaTime = { "DeltaTime", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXManager_eventScriptTick_Parms, DeltaTime), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UBXManager_ScriptTick_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXManager_ScriptTick_Statics::NewProp_DeltaTime,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXManager_ScriptTick_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UBXManager_ScriptTick_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UBXManager, nullptr, "ScriptTick", nullptr, nullptr, Z_Construct_UFunction_UBXManager_ScriptTick_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UBXManager_ScriptTick_Statics::PropPointers), sizeof(BXManager_eventScriptTick_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08020800, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXManager_ScriptTick_Statics::Function_MetaDataParams), Z_Construct_UFunction_UBXManager_ScriptTick_Statics::Function_MetaDataParams) };
static_assert(sizeof(BXManager_eventScriptTick_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UBXManager_ScriptTick()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UBXManager_ScriptTick_Statics::FuncParams);
	}
	return ReturnFunction;
}
// End Class UBXManager Function ScriptTick

// Begin Class UBXManager Function StopTimeline
struct Z_Construct_UFunction_UBXManager_StopTimeline_Statics
{
	struct BXManager_eventStopTimeline_Parms
	{
		int64 InID;
		EBXTLFinishReason InReason;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe5\x81\x9c\xe6\xad\xa2\xe6\x97\xb6\xe9\x97\xb4\xe8\xbd\xb4\n" },
#endif
		{ "ModuleRelativePath", "BXManager.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe5\x81\x9c\xe6\xad\xa2\xe6\x97\xb6\xe9\x97\xb4\xe8\xbd\xb4" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FInt64PropertyParams NewProp_InID;
	static const UECodeGen_Private::FBytePropertyParams NewProp_InReason_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_InReason;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FInt64PropertyParams Z_Construct_UFunction_UBXManager_StopTimeline_Statics::NewProp_InID = { "InID", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Int64, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXManager_eventStopTimeline_Parms, InID), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UBXManager_StopTimeline_Statics::NewProp_InReason_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UBXManager_StopTimeline_Statics::NewProp_InReason = { "InReason", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXManager_eventStopTimeline_Parms, InReason), Z_Construct_UEnum_BattleX_EBXTLFinishReason, METADATA_PARAMS(0, nullptr) }; // 4111085863
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UBXManager_StopTimeline_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXManager_StopTimeline_Statics::NewProp_InID,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXManager_StopTimeline_Statics::NewProp_InReason_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXManager_StopTimeline_Statics::NewProp_InReason,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXManager_StopTimeline_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UBXManager_StopTimeline_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UBXManager, nullptr, "StopTimeline", nullptr, nullptr, Z_Construct_UFunction_UBXManager_StopTimeline_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UBXManager_StopTimeline_Statics::PropPointers), sizeof(Z_Construct_UFunction_UBXManager_StopTimeline_Statics::BXManager_eventStopTimeline_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXManager_StopTimeline_Statics::Function_MetaDataParams), Z_Construct_UFunction_UBXManager_StopTimeline_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UBXManager_StopTimeline_Statics::BXManager_eventStopTimeline_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UBXManager_StopTimeline()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UBXManager_StopTimeline_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UBXManager::execStopTimeline)
{
	P_GET_PROPERTY(FInt64Property,Z_Param_InID);
	P_GET_ENUM(EBXTLFinishReason,Z_Param_InReason);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->StopTimeline(Z_Param_InID,EBXTLFinishReason(Z_Param_InReason));
	P_NATIVE_END;
}
// End Class UBXManager Function StopTimeline

// Begin Class UBXManager
void UBXManager::StaticRegisterNativesUBXManager()
{
	UClass* Class = UBXManager::StaticClass();
	static const FNameNativePtrPair Funcs[] = {
		{ "ChangeShowCollision", &UBXManager::execChangeShowCollision },
		{ "Get", &UBXManager::execGet },
		{ "GetShowCollision", &UBXManager::execGetShowCollision },
		{ "GetSkillAssetByID", &UBXManager::execGetSkillAssetByID },
		{ "GetSkillAssetIDs", &UBXManager::execGetSkillAssetIDs },
		{ "GetTimelineAssetByID", &UBXManager::execGetTimelineAssetByID },
		{ "GetTimelineAssetIDs", &UBXManager::execGetTimelineAssetIDs },
		{ "IsUpdatingTimeline", &UBXManager::execIsUpdatingTimeline },
		{ "OnWorldCleanupStart", &UBXManager::execOnWorldCleanupStart },
		{ "PlayTimeline", &UBXManager::execPlayTimeline },
		{ "StopTimeline", &UBXManager::execStopTimeline },
	};
	FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UBXManager);
UClass* Z_Construct_UClass_UBXManager_NoRegister()
{
	return UBXManager::StaticClass();
}
struct Z_Construct_UClass_UBXManager_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "IncludePath", "BXManager.h" },
		{ "IsBlueprintBase", "true" },
		{ "ModuleRelativePath", "BXManager.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_GCInterval_MetaData[] = {
		{ "Category", "BXManager" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe5\x9e\x83\xe5\x9c\xbe\xe6\xb8\x85\xe7\x90\x86\xe6\x97\xb6\xe9\x97\xb4\xe9\x97\xb4\xe9\x9a\x94\n" },
#endif
		{ "ModuleRelativePath", "BXManager.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe5\x9e\x83\xe5\x9c\xbe\xe6\xb8\x85\xe7\x90\x86\xe6\x97\xb6\xe9\x97\xb4\xe9\x97\xb4\xe9\x9a\x94" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_TimelineAssetMap_MetaData[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe6\x97\xb6\xe9\x97\xb4\xe8\xbd\xb4\xe8\xb5\x84\xe6\xba\x90\xe8\xb7\xaf\xe5\xbe\x84\xe6\x9f\xa5\xe8\xaf\xa2\xe8\xa1\xa8\n" },
#endif
		{ "ModuleRelativePath", "BXManager.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe6\x97\xb6\xe9\x97\xb4\xe8\xbd\xb4\xe8\xb5\x84\xe6\xba\x90\xe8\xb7\xaf\xe5\xbe\x84\xe6\x9f\xa5\xe8\xaf\xa2\xe8\xa1\xa8" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_TimelineTaskProcessorTypeMap_MetaData[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe7\xbc\x93\xe5\xad\x98\xe4\xbb\xbb\xe5\x8a\xa1\xe5\xa4\x84\xe7\x90\x86\xe5\x99\xa8\xe7\xb1\xbb\xe5\x9e\x8b\xe6\x98\xa0\xe5\xb0\x84\xe8\xa1\xa8\n" },
#endif
		{ "ModuleRelativePath", "BXManager.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe7\xbc\x93\xe5\xad\x98\xe4\xbb\xbb\xe5\x8a\xa1\xe5\xa4\x84\xe7\x90\x86\xe5\x99\xa8\xe7\xb1\xbb\xe5\x9e\x8b\xe6\x98\xa0\xe5\xb0\x84\xe8\xa1\xa8" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_TimelineTaskTypeMap_MetaData[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe7\xbc\x93\xe5\xad\x98\xe4\xbb\xbb\xe5\x8a\xa1\xe8\x87\xaa\xe5\xae\x9a\xe4\xb9\x89\xe6\x95\xb0\xe6\x8d\xae\xe9\x9b\x86\xe7\xb1\xbb\xe5\x9e\x8b\xe6\x98\xa0\xe5\xb0\x84\xe8\xa1\xa8\n" },
#endif
		{ "ModuleRelativePath", "BXManager.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe7\xbc\x93\xe5\xad\x98\xe4\xbb\xbb\xe5\x8a\xa1\xe8\x87\xaa\xe5\xae\x9a\xe4\xb9\x89\xe6\x95\xb0\xe6\x8d\xae\xe9\x9b\x86\xe7\xb1\xbb\xe5\x9e\x8b\xe6\x98\xa0\xe5\xb0\x84\xe8\xa1\xa8" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_TimelineTaskProcessors_MetaData[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe4\xbb\xbb\xe5\x8a\xa1\xe5\xa4\x84\xe7\x90\x86\xe5\x99\xa8\xe5\x88\x97\xe8\xa1\xa8\n" },
#endif
		{ "ModuleRelativePath", "BXManager.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe4\xbb\xbb\xe5\x8a\xa1\xe5\xa4\x84\xe7\x90\x86\xe5\x99\xa8\xe5\x88\x97\xe8\xa1\xa8" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_TimelineRTDatas_MetaData[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe6\x97\xb6\xe9\x97\xb4\xe8\xbd\xb4\xe8\xbf\x90\xe8\xa1\x8c\xe6\x95\xb0\xe6\x8d\xae\xe5\x88\x97\xe8\xa1\xa8\n" },
#endif
		{ "ModuleRelativePath", "BXManager.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe6\x97\xb6\xe9\x97\xb4\xe8\xbd\xb4\xe8\xbf\x90\xe8\xa1\x8c\xe6\x95\xb0\xe6\x8d\xae\xe5\x88\x97\xe8\xa1\xa8" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_TimelineTaskHostingDatas_MetaData[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe6\x89\x98\xe7\xae\xa1\xe7\x9a\x84\xe4\xbb\xbb\xe5\x8a\xa1\xe6\x95\xb0\xe6\x8d\xae\xe5\x88\x97\xe8\xa1\xa8\n" },
#endif
		{ "ModuleRelativePath", "BXManager.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe6\x89\x98\xe7\xae\xa1\xe7\x9a\x84\xe4\xbb\xbb\xe5\x8a\xa1\xe6\x95\xb0\xe6\x8d\xae\xe5\x88\x97\xe8\xa1\xa8" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_HelpHostingData1_MetaData[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe8\xbf\x90\xe8\xa1\x8c\xe6\x89\x98\xe7\xae\xa1\xe4\xbb\xbb\xe5\x8a\xa1\xe6\x89\x80\xe9\x9c\x80\xe7\x9a\x84\xe8\xbe\x85\xe5\x8a\xa9\xe7\xbb\x93\xe6\x9e\x84\xe4\xbd\x93\n" },
#endif
		{ "ModuleRelativePath", "BXManager.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe8\xbf\x90\xe8\xa1\x8c\xe6\x89\x98\xe7\xae\xa1\xe4\xbb\xbb\xe5\x8a\xa1\xe6\x89\x80\xe9\x9c\x80\xe7\x9a\x84\xe8\xbe\x85\xe5\x8a\xa9\xe7\xbb\x93\xe6\x9e\x84\xe4\xbd\x93" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_HelpHostingData2_MetaData[] = {
		{ "ModuleRelativePath", "BXManager.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_TimelineStarted_MetaData[] = {
		{ "ModuleRelativePath", "BXManager.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_TimelineWillEnd_MetaData[] = {
		{ "ModuleRelativePath", "BXManager.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFloatPropertyParams NewProp_GCInterval;
	static const UECodeGen_Private::FStructPropertyParams NewProp_TimelineAssetMap_ValueProp;
	static const UECodeGen_Private::FIntPropertyParams NewProp_TimelineAssetMap_Key_KeyProp;
	static const UECodeGen_Private::FMapPropertyParams NewProp_TimelineAssetMap;
	static const UECodeGen_Private::FClassPropertyParams NewProp_TimelineTaskProcessorTypeMap_ValueProp;
	static const UECodeGen_Private::FClassPropertyParams NewProp_TimelineTaskProcessorTypeMap_Key_KeyProp;
	static const UECodeGen_Private::FMapPropertyParams NewProp_TimelineTaskProcessorTypeMap;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_TimelineTaskTypeMap_ValueProp;
	static const UECodeGen_Private::FClassPropertyParams NewProp_TimelineTaskTypeMap_Key_KeyProp;
	static const UECodeGen_Private::FMapPropertyParams NewProp_TimelineTaskTypeMap;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_TimelineTaskProcessors_ValueProp;
	static const UECodeGen_Private::FClassPropertyParams NewProp_TimelineTaskProcessors_Key_KeyProp;
	static const UECodeGen_Private::FMapPropertyParams NewProp_TimelineTaskProcessors;
	static const UECodeGen_Private::FStructPropertyParams NewProp_TimelineRTDatas_ValueProp;
	static const UECodeGen_Private::FInt64PropertyParams NewProp_TimelineRTDatas_Key_KeyProp;
	static const UECodeGen_Private::FMapPropertyParams NewProp_TimelineRTDatas;
	static const UECodeGen_Private::FStructPropertyParams NewProp_TimelineTaskHostingDatas_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_TimelineTaskHostingDatas;
	static const UECodeGen_Private::FStructPropertyParams NewProp_HelpHostingData1;
	static const UECodeGen_Private::FStructPropertyParams NewProp_HelpHostingData2;
	static const UECodeGen_Private::FMulticastDelegatePropertyParams NewProp_TimelineStarted;
	static const UECodeGen_Private::FMulticastDelegatePropertyParams NewProp_TimelineWillEnd;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UBXManager_ChangeShowCollision, "ChangeShowCollision" }, // 1272820220
		{ &Z_Construct_UFunction_UBXManager_Get, "Get" }, // 2918579546
		{ &Z_Construct_UFunction_UBXManager_GetShowCollision, "GetShowCollision" }, // 612823479
		{ &Z_Construct_UFunction_UBXManager_GetSkillAssetByID, "GetSkillAssetByID" }, // 1048135270
		{ &Z_Construct_UFunction_UBXManager_GetSkillAssetIDs, "GetSkillAssetIDs" }, // 2138959409
		{ &Z_Construct_UFunction_UBXManager_GetTimelineAssetByID, "GetTimelineAssetByID" }, // 2745238012
		{ &Z_Construct_UFunction_UBXManager_GetTimelineAssetIDs, "GetTimelineAssetIDs" }, // 1078391122
		{ &Z_Construct_UFunction_UBXManager_IsUpdatingTimeline, "IsUpdatingTimeline" }, // 2427523488
		{ &Z_Construct_UFunction_UBXManager_OnWorldCleanupStart, "OnWorldCleanupStart" }, // 1248133220
		{ &Z_Construct_UFunction_UBXManager_PlayTimeline, "PlayTimeline" }, // 3693647251
		{ &Z_Construct_UFunction_UBXManager_ScriptTick, "ScriptTick" }, // 964093356
		{ &Z_Construct_UFunction_UBXManager_StopTimeline, "StopTimeline" }, // 1416098902
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UBXManager>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UBXManager_Statics::NewProp_GCInterval = { "GCInterval", nullptr, (EPropertyFlags)0x0020080000010001, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXManager, GCInterval), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_GCInterval_MetaData), NewProp_GCInterval_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UBXManager_Statics::NewProp_TimelineAssetMap_ValueProp = { "TimelineAssetMap", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 1, Z_Construct_UScriptStruct_FBXTimelineAssetInformation, METADATA_PARAMS(0, nullptr) }; // 2326295132
const UECodeGen_Private::FIntPropertyParams Z_Construct_UClass_UBXManager_Statics::NewProp_TimelineAssetMap_Key_KeyProp = { "TimelineAssetMap_Key", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FMapPropertyParams Z_Construct_UClass_UBXManager_Statics::NewProp_TimelineAssetMap = { "TimelineAssetMap", nullptr, (EPropertyFlags)0x0020080000002000, UECodeGen_Private::EPropertyGenFlags::Map, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXManager, TimelineAssetMap), EMapPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_TimelineAssetMap_MetaData), NewProp_TimelineAssetMap_MetaData) }; // 2326295132
const UECodeGen_Private::FClassPropertyParams Z_Construct_UClass_UBXManager_Statics::NewProp_TimelineTaskProcessorTypeMap_ValueProp = { "TimelineTaskProcessorTypeMap", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Class, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 1, Z_Construct_UClass_UClass, Z_Construct_UClass_UObject_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FClassPropertyParams Z_Construct_UClass_UBXManager_Statics::NewProp_TimelineTaskProcessorTypeMap_Key_KeyProp = { "TimelineTaskProcessorTypeMap_Key", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Class, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UClass_UClass, Z_Construct_UClass_UObject_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FMapPropertyParams Z_Construct_UClass_UBXManager_Statics::NewProp_TimelineTaskProcessorTypeMap = { "TimelineTaskProcessorTypeMap", nullptr, (EPropertyFlags)0x0020080000002000, UECodeGen_Private::EPropertyGenFlags::Map, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXManager, TimelineTaskProcessorTypeMap), EMapPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_TimelineTaskProcessorTypeMap_MetaData), NewProp_TimelineTaskProcessorTypeMap_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UBXManager_Statics::NewProp_TimelineTaskTypeMap_ValueProp = { "TimelineTaskTypeMap", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 1, Z_Construct_UClass_UScriptStruct, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FClassPropertyParams Z_Construct_UClass_UBXManager_Statics::NewProp_TimelineTaskTypeMap_Key_KeyProp = { "TimelineTaskTypeMap_Key", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Class, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UClass_UClass, Z_Construct_UClass_UObject_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FMapPropertyParams Z_Construct_UClass_UBXManager_Statics::NewProp_TimelineTaskTypeMap = { "TimelineTaskTypeMap", nullptr, (EPropertyFlags)0x0020080000002000, UECodeGen_Private::EPropertyGenFlags::Map, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXManager, TimelineTaskTypeMap), EMapPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_TimelineTaskTypeMap_MetaData), NewProp_TimelineTaskTypeMap_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UBXManager_Statics::NewProp_TimelineTaskProcessors_ValueProp = { "TimelineTaskProcessors", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 1, Z_Construct_UClass_UBXTProcessor_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FClassPropertyParams Z_Construct_UClass_UBXManager_Statics::NewProp_TimelineTaskProcessors_Key_KeyProp = { "TimelineTaskProcessors_Key", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Class, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UClass_UClass, Z_Construct_UClass_UObject_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FMapPropertyParams Z_Construct_UClass_UBXManager_Statics::NewProp_TimelineTaskProcessors = { "TimelineTaskProcessors", nullptr, (EPropertyFlags)0x0020080000002000, UECodeGen_Private::EPropertyGenFlags::Map, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXManager, TimelineTaskProcessors), EMapPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_TimelineTaskProcessors_MetaData), NewProp_TimelineTaskProcessors_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UBXManager_Statics::NewProp_TimelineRTDatas_ValueProp = { "TimelineRTDatas", nullptr, (EPropertyFlags)0x0000008000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 1, Z_Construct_UScriptStruct_FBXTLRunTimeData, METADATA_PARAMS(0, nullptr) }; // 1082839337
const UECodeGen_Private::FInt64PropertyParams Z_Construct_UClass_UBXManager_Statics::NewProp_TimelineRTDatas_Key_KeyProp = { "TimelineRTDatas_Key", nullptr, (EPropertyFlags)0x0000008000000000, UECodeGen_Private::EPropertyGenFlags::Int64, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FMapPropertyParams Z_Construct_UClass_UBXManager_Statics::NewProp_TimelineRTDatas = { "TimelineRTDatas", nullptr, (EPropertyFlags)0x0020088000002000, UECodeGen_Private::EPropertyGenFlags::Map, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXManager, TimelineRTDatas), EMapPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_TimelineRTDatas_MetaData), NewProp_TimelineRTDatas_MetaData) }; // 1082839337
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UBXManager_Statics::NewProp_TimelineTaskHostingDatas_Inner = { "TimelineTaskHostingDatas", nullptr, (EPropertyFlags)0x0000008000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UScriptStruct_FBXTLTaskHostingData, METADATA_PARAMS(0, nullptr) }; // 627855925
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UClass_UBXManager_Statics::NewProp_TimelineTaskHostingDatas = { "TimelineTaskHostingDatas", nullptr, (EPropertyFlags)0x0020088000002000, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXManager, TimelineTaskHostingDatas), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_TimelineTaskHostingDatas_MetaData), NewProp_TimelineTaskHostingDatas_MetaData) }; // 627855925
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UBXManager_Statics::NewProp_HelpHostingData1 = { "HelpHostingData1", nullptr, (EPropertyFlags)0x0020088000002000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXManager, HelpHostingData1), Z_Construct_UScriptStruct_FBXTLRunTimeData, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_HelpHostingData1_MetaData), NewProp_HelpHostingData1_MetaData) }; // 1082839337
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UBXManager_Statics::NewProp_HelpHostingData2 = { "HelpHostingData2", nullptr, (EPropertyFlags)0x0020080000002000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXManager, HelpHostingData2), Z_Construct_UScriptStruct_FBXTLSectionRTData, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_HelpHostingData2_MetaData), NewProp_HelpHostingData2_MetaData) }; // 262691324
const UECodeGen_Private::FMulticastDelegatePropertyParams Z_Construct_UClass_UBXManager_Statics::NewProp_TimelineStarted = { "TimelineStarted", nullptr, (EPropertyFlags)0x0010000010080000, UECodeGen_Private::EPropertyGenFlags::InlineMulticastDelegate, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXManager, TimelineStarted), Z_Construct_UDelegateFunction_BattleX_BXTimelineStarted__DelegateSignature, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_TimelineStarted_MetaData), NewProp_TimelineStarted_MetaData) }; // 1311972880
const UECodeGen_Private::FMulticastDelegatePropertyParams Z_Construct_UClass_UBXManager_Statics::NewProp_TimelineWillEnd = { "TimelineWillEnd", nullptr, (EPropertyFlags)0x0010000010080000, UECodeGen_Private::EPropertyGenFlags::InlineMulticastDelegate, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXManager, TimelineWillEnd), Z_Construct_UDelegateFunction_BattleX_BXTimelineWillEnd__DelegateSignature, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_TimelineWillEnd_MetaData), NewProp_TimelineWillEnd_MetaData) }; // 2084244405
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UBXManager_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXManager_Statics::NewProp_GCInterval,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXManager_Statics::NewProp_TimelineAssetMap_ValueProp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXManager_Statics::NewProp_TimelineAssetMap_Key_KeyProp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXManager_Statics::NewProp_TimelineAssetMap,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXManager_Statics::NewProp_TimelineTaskProcessorTypeMap_ValueProp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXManager_Statics::NewProp_TimelineTaskProcessorTypeMap_Key_KeyProp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXManager_Statics::NewProp_TimelineTaskProcessorTypeMap,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXManager_Statics::NewProp_TimelineTaskTypeMap_ValueProp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXManager_Statics::NewProp_TimelineTaskTypeMap_Key_KeyProp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXManager_Statics::NewProp_TimelineTaskTypeMap,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXManager_Statics::NewProp_TimelineTaskProcessors_ValueProp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXManager_Statics::NewProp_TimelineTaskProcessors_Key_KeyProp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXManager_Statics::NewProp_TimelineTaskProcessors,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXManager_Statics::NewProp_TimelineRTDatas_ValueProp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXManager_Statics::NewProp_TimelineRTDatas_Key_KeyProp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXManager_Statics::NewProp_TimelineRTDatas,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXManager_Statics::NewProp_TimelineTaskHostingDatas_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXManager_Statics::NewProp_TimelineTaskHostingDatas,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXManager_Statics::NewProp_HelpHostingData1,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXManager_Statics::NewProp_HelpHostingData2,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXManager_Statics::NewProp_TimelineStarted,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXManager_Statics::NewProp_TimelineWillEnd,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UBXManager_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_UBXManager_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UObject,
	(UObject* (*)())Z_Construct_UPackage__Script_BattleX,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UBXManager_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UBXManager_Statics::ClassParams = {
	&UBXManager::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	Z_Construct_UClass_UBXManager_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	UE_ARRAY_COUNT(Z_Construct_UClass_UBXManager_Statics::PropPointers),
	0,
	0x009000A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UBXManager_Statics::Class_MetaDataParams), Z_Construct_UClass_UBXManager_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UBXManager()
{
	if (!Z_Registration_Info_UClass_UBXManager.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UBXManager.OuterSingleton, Z_Construct_UClass_UBXManager_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UBXManager.OuterSingleton;
}
template<> BATTLEX_API UClass* StaticClass<UBXManager>()
{
	return UBXManager::StaticClass();
}
UBXManager::UBXManager(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UBXManager);
UBXManager::~UBXManager() {}
// End Class UBXManager

// Begin Registration
struct Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_BXManager_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UBXManager, UBXManager::StaticClass, TEXT("UBXManager"), &Z_Registration_Info_UClass_UBXManager, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UBXManager), 273784023U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_BXManager_h_2892636326(TEXT("/Script/BattleX"),
	Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_BXManager_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_BXManager_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
