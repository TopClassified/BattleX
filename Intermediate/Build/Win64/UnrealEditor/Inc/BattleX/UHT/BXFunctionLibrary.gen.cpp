// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "BattleX/BXFunctionLibrary.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeBXFunctionLibrary() {}

// Begin Cross Module References
BATTLEX_API UClass* Z_Construct_UClass_UBXFunctionLibrary();
BATTLEX_API UClass* Z_Construct_UClass_UBXFunctionLibrary_NoRegister();
BATTLEX_API UClass* Z_Construct_UClass_UBXTask_NoRegister();
BATTLEX_API UClass* Z_Construct_UClass_UBXTLAsset_NoRegister();
COREUOBJECT_API UClass* Z_Construct_UClass_UClass();
COREUOBJECT_API UClass* Z_Construct_UClass_UObject_NoRegister();
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FRotator();
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FVector();
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FVector2D();
ENGINE_API UClass* Z_Construct_UClass_AActor_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_UBlueprintFunctionLibrary();
ENGINE_API UClass* Z_Construct_UClass_USceneComponent_NoRegister();
UPackage* Z_Construct_UPackage__Script_BattleX();
// End Cross Module References

// Begin Class UBXFunctionLibrary Function AlignTime
struct Z_Construct_UFunction_UBXFunctionLibrary_AlignTime_Statics
{
	struct BXFunctionLibrary_eventAlignTime_Parms
	{
		float InTime;
		float InAlign;
		float ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Misc" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe6\x97\xb6\xe9\x97\xb4\xe5\xaf\xb9\xe9\xbd\x90\n" },
#endif
		{ "ModuleRelativePath", "BXFunctionLibrary.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe6\x97\xb6\xe9\x97\xb4\xe5\xaf\xb9\xe9\xbd\x90" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFloatPropertyParams NewProp_InTime;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_InAlign;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UBXFunctionLibrary_AlignTime_Statics::NewProp_InTime = { "InTime", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventAlignTime_Parms, InTime), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UBXFunctionLibrary_AlignTime_Statics::NewProp_InAlign = { "InAlign", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventAlignTime_Parms, InAlign), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UBXFunctionLibrary_AlignTime_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventAlignTime_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UBXFunctionLibrary_AlignTime_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_AlignTime_Statics::NewProp_InTime,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_AlignTime_Statics::NewProp_InAlign,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_AlignTime_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXFunctionLibrary_AlignTime_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UBXFunctionLibrary_AlignTime_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UBXFunctionLibrary, nullptr, "AlignTime", nullptr, nullptr, Z_Construct_UFunction_UBXFunctionLibrary_AlignTime_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UBXFunctionLibrary_AlignTime_Statics::PropPointers), sizeof(Z_Construct_UFunction_UBXFunctionLibrary_AlignTime_Statics::BXFunctionLibrary_eventAlignTime_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14022401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXFunctionLibrary_AlignTime_Statics::Function_MetaDataParams), Z_Construct_UFunction_UBXFunctionLibrary_AlignTime_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UBXFunctionLibrary_AlignTime_Statics::BXFunctionLibrary_eventAlignTime_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UBXFunctionLibrary_AlignTime()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UBXFunctionLibrary_AlignTime_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UBXFunctionLibrary::execAlignTime)
{
	P_GET_PROPERTY(FFloatProperty,Z_Param_InTime);
	P_GET_PROPERTY(FFloatProperty,Z_Param_InAlign);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(float*)Z_Param__Result=UBXFunctionLibrary::AlignTime(Z_Param_InTime,Z_Param_InAlign);
	P_NATIVE_END;
}
// End Class UBXFunctionLibrary Function AlignTime

// Begin Class UBXFunctionLibrary Function CopyObject
struct Z_Construct_UFunction_UBXFunctionLibrary_CopyObject_Statics
{
	struct BXFunctionLibrary_eventCopyObject_Parms
	{
		UObject* DestObject;
		UObject* SrcObject;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Property" },
		{ "ModuleRelativePath", "BXFunctionLibrary.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_DestObject;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_SrcObject;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UBXFunctionLibrary_CopyObject_Statics::NewProp_DestObject = { "DestObject", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventCopyObject_Parms, DestObject), Z_Construct_UClass_UObject_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UBXFunctionLibrary_CopyObject_Statics::NewProp_SrcObject = { "SrcObject", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventCopyObject_Parms, SrcObject), Z_Construct_UClass_UObject_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UBXFunctionLibrary_CopyObject_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_CopyObject_Statics::NewProp_DestObject,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_CopyObject_Statics::NewProp_SrcObject,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXFunctionLibrary_CopyObject_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UBXFunctionLibrary_CopyObject_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UBXFunctionLibrary, nullptr, "CopyObject", nullptr, nullptr, Z_Construct_UFunction_UBXFunctionLibrary_CopyObject_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UBXFunctionLibrary_CopyObject_Statics::PropPointers), sizeof(Z_Construct_UFunction_UBXFunctionLibrary_CopyObject_Statics::BXFunctionLibrary_eventCopyObject_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04022401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXFunctionLibrary_CopyObject_Statics::Function_MetaDataParams), Z_Construct_UFunction_UBXFunctionLibrary_CopyObject_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UBXFunctionLibrary_CopyObject_Statics::BXFunctionLibrary_eventCopyObject_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UBXFunctionLibrary_CopyObject()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UBXFunctionLibrary_CopyObject_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UBXFunctionLibrary::execCopyObject)
{
	P_GET_OBJECT(UObject,Z_Param_DestObject);
	P_GET_OBJECT(UObject,Z_Param_SrcObject);
	P_FINISH;
	P_NATIVE_BEGIN;
	UBXFunctionLibrary::CopyObject(Z_Param_DestObject,Z_Param_SrcObject);
	P_NATIVE_END;
}
// End Class UBXFunctionLibrary Function CopyObject

// Begin Class UBXFunctionLibrary Function GetGameMicrosecond
struct Z_Construct_UFunction_UBXFunctionLibrary_GetGameMicrosecond_Statics
{
	struct BXFunctionLibrary_eventGetGameMicrosecond_Parms
	{
		int64 ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Time" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe8\x8e\xb7\xe5\x8f\x96\xe6\xb8\xb8\xe6\x88\x8f\xe8\xbf\x90\xe8\xa1\x8c\xe6\x97\xb6\xe9\x97\xb4\xef\xbc\x88\xe5\xbe\xae\xe7\xa7\x92\xef\xbc\x89\n" },
#endif
		{ "ModuleRelativePath", "BXFunctionLibrary.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe8\x8e\xb7\xe5\x8f\x96\xe6\xb8\xb8\xe6\x88\x8f\xe8\xbf\x90\xe8\xa1\x8c\xe6\x97\xb6\xe9\x97\xb4\xef\xbc\x88\xe5\xbe\xae\xe7\xa7\x92\xef\xbc\x89" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FInt64PropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FInt64PropertyParams Z_Construct_UFunction_UBXFunctionLibrary_GetGameMicrosecond_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Int64, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventGetGameMicrosecond_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UBXFunctionLibrary_GetGameMicrosecond_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_GetGameMicrosecond_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXFunctionLibrary_GetGameMicrosecond_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UBXFunctionLibrary_GetGameMicrosecond_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UBXFunctionLibrary, nullptr, "GetGameMicrosecond", nullptr, nullptr, Z_Construct_UFunction_UBXFunctionLibrary_GetGameMicrosecond_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UBXFunctionLibrary_GetGameMicrosecond_Statics::PropPointers), sizeof(Z_Construct_UFunction_UBXFunctionLibrary_GetGameMicrosecond_Statics::BXFunctionLibrary_eventGetGameMicrosecond_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04022401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXFunctionLibrary_GetGameMicrosecond_Statics::Function_MetaDataParams), Z_Construct_UFunction_UBXFunctionLibrary_GetGameMicrosecond_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UBXFunctionLibrary_GetGameMicrosecond_Statics::BXFunctionLibrary_eventGetGameMicrosecond_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UBXFunctionLibrary_GetGameMicrosecond()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UBXFunctionLibrary_GetGameMicrosecond_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UBXFunctionLibrary::execGetGameMicrosecond)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(int64*)Z_Param__Result=UBXFunctionLibrary::GetGameMicrosecond();
	P_NATIVE_END;
}
// End Class UBXFunctionLibrary Function GetGameMicrosecond

// Begin Class UBXFunctionLibrary Function GetSceneComponentByNameAndClass
struct Z_Construct_UFunction_UBXFunctionLibrary_GetSceneComponentByNameAndClass_Statics
{
	struct BXFunctionLibrary_eventGetSceneComponentByNameAndClass_Parms
	{
		AActor* InActor;
		FName InName;
		UClass* InClass;
		bool bCheckChild;
		USceneComponent* ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Component" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe6\xa0\xb9\xe6\x8d\xae\xe5\x90\x8d\xe7\xa7\xb0\xef\xbc\x8c\xe8\x8e\xb7\xe5\x8f\x96\xe7\xbb\x84\xe4\xbb\xb6\n" },
#endif
		{ "CPP_Default_bCheckChild", "true" },
		{ "ModuleRelativePath", "BXFunctionLibrary.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe6\xa0\xb9\xe6\x8d\xae\xe5\x90\x8d\xe7\xa7\xb0\xef\xbc\x8c\xe8\x8e\xb7\xe5\x8f\x96\xe7\xbb\x84\xe4\xbb\xb6" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InName_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ReturnValue_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_InActor;
	static const UECodeGen_Private::FNamePropertyParams NewProp_InName;
	static const UECodeGen_Private::FClassPropertyParams NewProp_InClass;
	static void NewProp_bCheckChild_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bCheckChild;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UBXFunctionLibrary_GetSceneComponentByNameAndClass_Statics::NewProp_InActor = { "InActor", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventGetSceneComponentByNameAndClass_Parms, InActor), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FNamePropertyParams Z_Construct_UFunction_UBXFunctionLibrary_GetSceneComponentByNameAndClass_Statics::NewProp_InName = { "InName", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventGetSceneComponentByNameAndClass_Parms, InName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InName_MetaData), NewProp_InName_MetaData) };
const UECodeGen_Private::FClassPropertyParams Z_Construct_UFunction_UBXFunctionLibrary_GetSceneComponentByNameAndClass_Statics::NewProp_InClass = { "InClass", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Class, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventGetSceneComponentByNameAndClass_Parms, InClass), Z_Construct_UClass_UClass, Z_Construct_UClass_UObject_NoRegister, METADATA_PARAMS(0, nullptr) };
void Z_Construct_UFunction_UBXFunctionLibrary_GetSceneComponentByNameAndClass_Statics::NewProp_bCheckChild_SetBit(void* Obj)
{
	((BXFunctionLibrary_eventGetSceneComponentByNameAndClass_Parms*)Obj)->bCheckChild = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UBXFunctionLibrary_GetSceneComponentByNameAndClass_Statics::NewProp_bCheckChild = { "bCheckChild", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(BXFunctionLibrary_eventGetSceneComponentByNameAndClass_Parms), &Z_Construct_UFunction_UBXFunctionLibrary_GetSceneComponentByNameAndClass_Statics::NewProp_bCheckChild_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UBXFunctionLibrary_GetSceneComponentByNameAndClass_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000080588, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventGetSceneComponentByNameAndClass_Parms, ReturnValue), Z_Construct_UClass_USceneComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ReturnValue_MetaData), NewProp_ReturnValue_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UBXFunctionLibrary_GetSceneComponentByNameAndClass_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_GetSceneComponentByNameAndClass_Statics::NewProp_InActor,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_GetSceneComponentByNameAndClass_Statics::NewProp_InName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_GetSceneComponentByNameAndClass_Statics::NewProp_InClass,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_GetSceneComponentByNameAndClass_Statics::NewProp_bCheckChild,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_GetSceneComponentByNameAndClass_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXFunctionLibrary_GetSceneComponentByNameAndClass_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UBXFunctionLibrary_GetSceneComponentByNameAndClass_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UBXFunctionLibrary, nullptr, "GetSceneComponentByNameAndClass", nullptr, nullptr, Z_Construct_UFunction_UBXFunctionLibrary_GetSceneComponentByNameAndClass_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UBXFunctionLibrary_GetSceneComponentByNameAndClass_Statics::PropPointers), sizeof(Z_Construct_UFunction_UBXFunctionLibrary_GetSceneComponentByNameAndClass_Statics::BXFunctionLibrary_eventGetSceneComponentByNameAndClass_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04422401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXFunctionLibrary_GetSceneComponentByNameAndClass_Statics::Function_MetaDataParams), Z_Construct_UFunction_UBXFunctionLibrary_GetSceneComponentByNameAndClass_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UBXFunctionLibrary_GetSceneComponentByNameAndClass_Statics::BXFunctionLibrary_eventGetSceneComponentByNameAndClass_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UBXFunctionLibrary_GetSceneComponentByNameAndClass()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UBXFunctionLibrary_GetSceneComponentByNameAndClass_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UBXFunctionLibrary::execGetSceneComponentByNameAndClass)
{
	P_GET_OBJECT(AActor,Z_Param_InActor);
	P_GET_PROPERTY_REF(FNameProperty,Z_Param_Out_InName);
	P_GET_OBJECT(UClass,Z_Param_InClass);
	P_GET_UBOOL(Z_Param_bCheckChild);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(USceneComponent**)Z_Param__Result=UBXFunctionLibrary::GetSceneComponentByNameAndClass(Z_Param_InActor,Z_Param_Out_InName,Z_Param_InClass,Z_Param_bCheckChild);
	P_NATIVE_END;
}
// End Class UBXFunctionLibrary Function GetSceneComponentByNameAndClass

// Begin Class UBXFunctionLibrary Function GetShortUniqueID
struct Z_Construct_UFunction_UBXFunctionLibrary_GetShortUniqueID_Statics
{
	struct BXFunctionLibrary_eventGetShortUniqueID_Parms
	{
		int32 ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Misc" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// 31\xe4\xbd\x8d\xe7\x9a\x84\xe5\x94\xaf\xe4\xb8\x80\n" },
#endif
		{ "ModuleRelativePath", "BXFunctionLibrary.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "31\xe4\xbd\x8d\xe7\x9a\x84\xe5\x94\xaf\xe4\xb8\x80" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FIntPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UBXFunctionLibrary_GetShortUniqueID_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventGetShortUniqueID_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UBXFunctionLibrary_GetShortUniqueID_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_GetShortUniqueID_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXFunctionLibrary_GetShortUniqueID_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UBXFunctionLibrary_GetShortUniqueID_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UBXFunctionLibrary, nullptr, "GetShortUniqueID", nullptr, nullptr, Z_Construct_UFunction_UBXFunctionLibrary_GetShortUniqueID_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UBXFunctionLibrary_GetShortUniqueID_Statics::PropPointers), sizeof(Z_Construct_UFunction_UBXFunctionLibrary_GetShortUniqueID_Statics::BXFunctionLibrary_eventGetShortUniqueID_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14022401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXFunctionLibrary_GetShortUniqueID_Statics::Function_MetaDataParams), Z_Construct_UFunction_UBXFunctionLibrary_GetShortUniqueID_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UBXFunctionLibrary_GetShortUniqueID_Statics::BXFunctionLibrary_eventGetShortUniqueID_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UBXFunctionLibrary_GetShortUniqueID()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UBXFunctionLibrary_GetShortUniqueID_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UBXFunctionLibrary::execGetShortUniqueID)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(int32*)Z_Param__Result=UBXFunctionLibrary::GetShortUniqueID();
	P_NATIVE_END;
}
// End Class UBXFunctionLibrary Function GetShortUniqueID

// Begin Class UBXFunctionLibrary Function GetSoftTaskFullIndex
struct Z_Construct_UFunction_UBXFunctionLibrary_GetSoftTaskFullIndex_Statics
{
	struct BXFunctionLibrary_eventGetSoftTaskFullIndex_Parms
	{
		UBXTLAsset* InAsset;
		TSoftObjectPtr<UBXTask> InTask;
		int32 ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Timeline" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe8\x8e\xb7\xe5\x8f\x96\xe6\x97\xb6\xe9\x97\xb4\xe8\xbd\xb4\xe4\xbb\xbb\xe5\x8a\xa1\xe7\x9a\x84\xe7\xb4\xa2\xe5\xbc\x95\xe5\x80\xbc\n" },
#endif
		{ "ModuleRelativePath", "BXFunctionLibrary.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe8\x8e\xb7\xe5\x8f\x96\xe6\x97\xb6\xe9\x97\xb4\xe8\xbd\xb4\xe4\xbb\xbb\xe5\x8a\xa1\xe7\x9a\x84\xe7\xb4\xa2\xe5\xbc\x95\xe5\x80\xbc" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_InAsset;
	static const UECodeGen_Private::FSoftObjectPropertyParams NewProp_InTask;
	static const UECodeGen_Private::FIntPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UBXFunctionLibrary_GetSoftTaskFullIndex_Statics::NewProp_InAsset = { "InAsset", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventGetSoftTaskFullIndex_Parms, InAsset), Z_Construct_UClass_UBXTLAsset_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FSoftObjectPropertyParams Z_Construct_UFunction_UBXFunctionLibrary_GetSoftTaskFullIndex_Statics::NewProp_InTask = { "InTask", nullptr, (EPropertyFlags)0x0014000000000080, UECodeGen_Private::EPropertyGenFlags::SoftObject, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventGetSoftTaskFullIndex_Parms, InTask), Z_Construct_UClass_UBXTask_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UBXFunctionLibrary_GetSoftTaskFullIndex_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventGetSoftTaskFullIndex_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UBXFunctionLibrary_GetSoftTaskFullIndex_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_GetSoftTaskFullIndex_Statics::NewProp_InAsset,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_GetSoftTaskFullIndex_Statics::NewProp_InTask,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_GetSoftTaskFullIndex_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXFunctionLibrary_GetSoftTaskFullIndex_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UBXFunctionLibrary_GetSoftTaskFullIndex_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UBXFunctionLibrary, nullptr, "GetSoftTaskFullIndex", nullptr, nullptr, Z_Construct_UFunction_UBXFunctionLibrary_GetSoftTaskFullIndex_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UBXFunctionLibrary_GetSoftTaskFullIndex_Statics::PropPointers), sizeof(Z_Construct_UFunction_UBXFunctionLibrary_GetSoftTaskFullIndex_Statics::BXFunctionLibrary_eventGetSoftTaskFullIndex_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14022401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXFunctionLibrary_GetSoftTaskFullIndex_Statics::Function_MetaDataParams), Z_Construct_UFunction_UBXFunctionLibrary_GetSoftTaskFullIndex_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UBXFunctionLibrary_GetSoftTaskFullIndex_Statics::BXFunctionLibrary_eventGetSoftTaskFullIndex_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UBXFunctionLibrary_GetSoftTaskFullIndex()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UBXFunctionLibrary_GetSoftTaskFullIndex_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UBXFunctionLibrary::execGetSoftTaskFullIndex)
{
	P_GET_OBJECT(UBXTLAsset,Z_Param_InAsset);
	P_GET_SOFTOBJECT(TSoftObjectPtr<UBXTask>,Z_Param_InTask);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(int32*)Z_Param__Result=UBXFunctionLibrary::GetSoftTaskFullIndex(Z_Param_InAsset,Z_Param_InTask);
	P_NATIVE_END;
}
// End Class UBXFunctionLibrary Function GetSoftTaskFullIndex

// Begin Class UBXFunctionLibrary Function GetTaskFullIndex
struct Z_Construct_UFunction_UBXFunctionLibrary_GetTaskFullIndex_Statics
{
	struct BXFunctionLibrary_eventGetTaskFullIndex_Parms
	{
		UBXTLAsset* InAsset;
		UBXTask* InTask;
		int32 ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Timeline" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe8\x8e\xb7\xe5\x8f\x96\xe6\x97\xb6\xe9\x97\xb4\xe8\xbd\xb4\xe4\xbb\xbb\xe5\x8a\xa1\xe7\x9a\x84\xe7\xb4\xa2\xe5\xbc\x95\xe5\x80\xbc\n" },
#endif
		{ "ModuleRelativePath", "BXFunctionLibrary.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe8\x8e\xb7\xe5\x8f\x96\xe6\x97\xb6\xe9\x97\xb4\xe8\xbd\xb4\xe4\xbb\xbb\xe5\x8a\xa1\xe7\x9a\x84\xe7\xb4\xa2\xe5\xbc\x95\xe5\x80\xbc" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_InAsset;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_InTask;
	static const UECodeGen_Private::FIntPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UBXFunctionLibrary_GetTaskFullIndex_Statics::NewProp_InAsset = { "InAsset", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventGetTaskFullIndex_Parms, InAsset), Z_Construct_UClass_UBXTLAsset_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UBXFunctionLibrary_GetTaskFullIndex_Statics::NewProp_InTask = { "InTask", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventGetTaskFullIndex_Parms, InTask), Z_Construct_UClass_UBXTask_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UBXFunctionLibrary_GetTaskFullIndex_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventGetTaskFullIndex_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UBXFunctionLibrary_GetTaskFullIndex_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_GetTaskFullIndex_Statics::NewProp_InAsset,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_GetTaskFullIndex_Statics::NewProp_InTask,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_GetTaskFullIndex_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXFunctionLibrary_GetTaskFullIndex_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UBXFunctionLibrary_GetTaskFullIndex_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UBXFunctionLibrary, nullptr, "GetTaskFullIndex", nullptr, nullptr, Z_Construct_UFunction_UBXFunctionLibrary_GetTaskFullIndex_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UBXFunctionLibrary_GetTaskFullIndex_Statics::PropPointers), sizeof(Z_Construct_UFunction_UBXFunctionLibrary_GetTaskFullIndex_Statics::BXFunctionLibrary_eventGetTaskFullIndex_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14022401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXFunctionLibrary_GetTaskFullIndex_Statics::Function_MetaDataParams), Z_Construct_UFunction_UBXFunctionLibrary_GetTaskFullIndex_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UBXFunctionLibrary_GetTaskFullIndex_Statics::BXFunctionLibrary_eventGetTaskFullIndex_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UBXFunctionLibrary_GetTaskFullIndex()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UBXFunctionLibrary_GetTaskFullIndex_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UBXFunctionLibrary::execGetTaskFullIndex)
{
	P_GET_OBJECT(UBXTLAsset,Z_Param_InAsset);
	P_GET_OBJECT(UBXTask,Z_Param_InTask);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(int32*)Z_Param__Result=UBXFunctionLibrary::GetTaskFullIndex(Z_Param_InAsset,Z_Param_InTask);
	P_NATIVE_END;
}
// End Class UBXFunctionLibrary Function GetTaskFullIndex

// Begin Class UBXFunctionLibrary Function GetUniqueID
struct Z_Construct_UFunction_UBXFunctionLibrary_GetUniqueID_Statics
{
	struct BXFunctionLibrary_eventGetUniqueID_Parms
	{
		int64 ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Misc" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// 63\xe4\xbd\x8d\xe7\x9a\x84\xe5\x94\xaf\xe4\xb8\x80\n" },
#endif
		{ "ModuleRelativePath", "BXFunctionLibrary.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "63\xe4\xbd\x8d\xe7\x9a\x84\xe5\x94\xaf\xe4\xb8\x80" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FInt64PropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FInt64PropertyParams Z_Construct_UFunction_UBXFunctionLibrary_GetUniqueID_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Int64, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventGetUniqueID_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UBXFunctionLibrary_GetUniqueID_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_GetUniqueID_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXFunctionLibrary_GetUniqueID_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UBXFunctionLibrary_GetUniqueID_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UBXFunctionLibrary, nullptr, "GetUniqueID", nullptr, nullptr, Z_Construct_UFunction_UBXFunctionLibrary_GetUniqueID_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UBXFunctionLibrary_GetUniqueID_Statics::PropPointers), sizeof(Z_Construct_UFunction_UBXFunctionLibrary_GetUniqueID_Statics::BXFunctionLibrary_eventGetUniqueID_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14022401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXFunctionLibrary_GetUniqueID_Statics::Function_MetaDataParams), Z_Construct_UFunction_UBXFunctionLibrary_GetUniqueID_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UBXFunctionLibrary_GetUniqueID_Statics::BXFunctionLibrary_eventGetUniqueID_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UBXFunctionLibrary_GetUniqueID()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UBXFunctionLibrary_GetUniqueID_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UBXFunctionLibrary::execGetUniqueID)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(int64*)Z_Param__Result=UBXFunctionLibrary::GetUniqueID();
	P_NATIVE_END;
}
// End Class UBXFunctionLibrary Function GetUniqueID

// Begin Class UBXFunctionLibrary Function GetUtcMillisecond
struct Z_Construct_UFunction_UBXFunctionLibrary_GetUtcMillisecond_Statics
{
	struct BXFunctionLibrary_eventGetUtcMillisecond_Parms
	{
		int64 ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Time" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe8\x8e\xb7\xe5\x8f\x96\xe5\xbd\x93\xe5\x89\x8d\xe6\x97\xb6\xe9\x97\xb4\xe6\x88\xb3\xef\xbc\x88\xe6\xaf\xab\xe7\xa7\x92\xef\xbc\x89\n" },
#endif
		{ "ModuleRelativePath", "BXFunctionLibrary.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe8\x8e\xb7\xe5\x8f\x96\xe5\xbd\x93\xe5\x89\x8d\xe6\x97\xb6\xe9\x97\xb4\xe6\x88\xb3\xef\xbc\x88\xe6\xaf\xab\xe7\xa7\x92\xef\xbc\x89" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FInt64PropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FInt64PropertyParams Z_Construct_UFunction_UBXFunctionLibrary_GetUtcMillisecond_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Int64, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventGetUtcMillisecond_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UBXFunctionLibrary_GetUtcMillisecond_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_GetUtcMillisecond_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXFunctionLibrary_GetUtcMillisecond_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UBXFunctionLibrary_GetUtcMillisecond_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UBXFunctionLibrary, nullptr, "GetUtcMillisecond", nullptr, nullptr, Z_Construct_UFunction_UBXFunctionLibrary_GetUtcMillisecond_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UBXFunctionLibrary_GetUtcMillisecond_Statics::PropPointers), sizeof(Z_Construct_UFunction_UBXFunctionLibrary_GetUtcMillisecond_Statics::BXFunctionLibrary_eventGetUtcMillisecond_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04022401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXFunctionLibrary_GetUtcMillisecond_Statics::Function_MetaDataParams), Z_Construct_UFunction_UBXFunctionLibrary_GetUtcMillisecond_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UBXFunctionLibrary_GetUtcMillisecond_Statics::BXFunctionLibrary_eventGetUtcMillisecond_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UBXFunctionLibrary_GetUtcMillisecond()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UBXFunctionLibrary_GetUtcMillisecond_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UBXFunctionLibrary::execGetUtcMillisecond)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(int64*)Z_Param__Result=UBXFunctionLibrary::GetUtcMillisecond();
	P_NATIVE_END;
}
// End Class UBXFunctionLibrary Function GetUtcMillisecond

// Begin Class UBXFunctionLibrary Function PointToBox
struct Z_Construct_UFunction_UBXFunctionLibrary_PointToBox_Statics
{
	struct BXFunctionLibrary_eventPointToBox_Parms
	{
		FVector InP;
		FVector InCenter;
		FRotator InRotation;
		FVector InExtent;
		float ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Math" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe8\xae\xa1\xe7\xae\x97\xe7\x82\xb9\xe5\x88\xb0\xe9\x95\xbf\xe6\x96\xb9\xe4\xbd\x93\xe7\x9a\x84\xe6\x9c\x80\xe7\x9f\xad\xe8\xb7\x9d\xe7\xa6\xbb\n" },
#endif
		{ "ModuleRelativePath", "BXFunctionLibrary.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe8\xae\xa1\xe7\xae\x97\xe7\x82\xb9\xe5\x88\xb0\xe9\x95\xbf\xe6\x96\xb9\xe4\xbd\x93\xe7\x9a\x84\xe6\x9c\x80\xe7\x9f\xad\xe8\xb7\x9d\xe7\xa6\xbb" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InP_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InCenter_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InRotation_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InExtent_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_InP;
	static const UECodeGen_Private::FStructPropertyParams NewProp_InCenter;
	static const UECodeGen_Private::FStructPropertyParams NewProp_InRotation;
	static const UECodeGen_Private::FStructPropertyParams NewProp_InExtent;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXFunctionLibrary_PointToBox_Statics::NewProp_InP = { "InP", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventPointToBox_Parms, InP), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InP_MetaData), NewProp_InP_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXFunctionLibrary_PointToBox_Statics::NewProp_InCenter = { "InCenter", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventPointToBox_Parms, InCenter), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InCenter_MetaData), NewProp_InCenter_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXFunctionLibrary_PointToBox_Statics::NewProp_InRotation = { "InRotation", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventPointToBox_Parms, InRotation), Z_Construct_UScriptStruct_FRotator, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InRotation_MetaData), NewProp_InRotation_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXFunctionLibrary_PointToBox_Statics::NewProp_InExtent = { "InExtent", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventPointToBox_Parms, InExtent), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InExtent_MetaData), NewProp_InExtent_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UBXFunctionLibrary_PointToBox_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventPointToBox_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UBXFunctionLibrary_PointToBox_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_PointToBox_Statics::NewProp_InP,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_PointToBox_Statics::NewProp_InCenter,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_PointToBox_Statics::NewProp_InRotation,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_PointToBox_Statics::NewProp_InExtent,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_PointToBox_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXFunctionLibrary_PointToBox_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UBXFunctionLibrary_PointToBox_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UBXFunctionLibrary, nullptr, "PointToBox", nullptr, nullptr, Z_Construct_UFunction_UBXFunctionLibrary_PointToBox_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UBXFunctionLibrary_PointToBox_Statics::PropPointers), sizeof(Z_Construct_UFunction_UBXFunctionLibrary_PointToBox_Statics::BXFunctionLibrary_eventPointToBox_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04C22401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXFunctionLibrary_PointToBox_Statics::Function_MetaDataParams), Z_Construct_UFunction_UBXFunctionLibrary_PointToBox_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UBXFunctionLibrary_PointToBox_Statics::BXFunctionLibrary_eventPointToBox_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UBXFunctionLibrary_PointToBox()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UBXFunctionLibrary_PointToBox_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UBXFunctionLibrary::execPointToBox)
{
	P_GET_STRUCT_REF(FVector,Z_Param_Out_InP);
	P_GET_STRUCT_REF(FVector,Z_Param_Out_InCenter);
	P_GET_STRUCT_REF(FRotator,Z_Param_Out_InRotation);
	P_GET_STRUCT_REF(FVector,Z_Param_Out_InExtent);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(float*)Z_Param__Result=UBXFunctionLibrary::PointToBox(Z_Param_Out_InP,Z_Param_Out_InCenter,Z_Param_Out_InRotation,Z_Param_Out_InExtent);
	P_NATIVE_END;
}
// End Class UBXFunctionLibrary Function PointToBox

// Begin Class UBXFunctionLibrary Function PointToCapsule
struct Z_Construct_UFunction_UBXFunctionLibrary_PointToCapsule_Statics
{
	struct BXFunctionLibrary_eventPointToCapsule_Parms
	{
		FVector InP;
		FVector InCenter;
		FRotator InRotation;
		float InRadius;
		float InHalfHeight;
		float ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Math" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe8\xae\xa1\xe7\xae\x97\xe7\x82\xb9\xe5\x88\xb0\xe8\x83\xb6\xe5\x9b\x8a\xe4\xbd\x93\xe7\x9a\x84\xe6\x9c\x80\xe7\x9f\xad\xe8\xb7\x9d\xe7\xa6\xbb\n" },
#endif
		{ "ModuleRelativePath", "BXFunctionLibrary.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe8\xae\xa1\xe7\xae\x97\xe7\x82\xb9\xe5\x88\xb0\xe8\x83\xb6\xe5\x9b\x8a\xe4\xbd\x93\xe7\x9a\x84\xe6\x9c\x80\xe7\x9f\xad\xe8\xb7\x9d\xe7\xa6\xbb" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InP_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InCenter_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InRotation_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InRadius_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InHalfHeight_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_InP;
	static const UECodeGen_Private::FStructPropertyParams NewProp_InCenter;
	static const UECodeGen_Private::FStructPropertyParams NewProp_InRotation;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_InRadius;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_InHalfHeight;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXFunctionLibrary_PointToCapsule_Statics::NewProp_InP = { "InP", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventPointToCapsule_Parms, InP), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InP_MetaData), NewProp_InP_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXFunctionLibrary_PointToCapsule_Statics::NewProp_InCenter = { "InCenter", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventPointToCapsule_Parms, InCenter), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InCenter_MetaData), NewProp_InCenter_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXFunctionLibrary_PointToCapsule_Statics::NewProp_InRotation = { "InRotation", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventPointToCapsule_Parms, InRotation), Z_Construct_UScriptStruct_FRotator, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InRotation_MetaData), NewProp_InRotation_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UBXFunctionLibrary_PointToCapsule_Statics::NewProp_InRadius = { "InRadius", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventPointToCapsule_Parms, InRadius), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InRadius_MetaData), NewProp_InRadius_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UBXFunctionLibrary_PointToCapsule_Statics::NewProp_InHalfHeight = { "InHalfHeight", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventPointToCapsule_Parms, InHalfHeight), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InHalfHeight_MetaData), NewProp_InHalfHeight_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UBXFunctionLibrary_PointToCapsule_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventPointToCapsule_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UBXFunctionLibrary_PointToCapsule_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_PointToCapsule_Statics::NewProp_InP,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_PointToCapsule_Statics::NewProp_InCenter,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_PointToCapsule_Statics::NewProp_InRotation,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_PointToCapsule_Statics::NewProp_InRadius,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_PointToCapsule_Statics::NewProp_InHalfHeight,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_PointToCapsule_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXFunctionLibrary_PointToCapsule_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UBXFunctionLibrary_PointToCapsule_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UBXFunctionLibrary, nullptr, "PointToCapsule", nullptr, nullptr, Z_Construct_UFunction_UBXFunctionLibrary_PointToCapsule_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UBXFunctionLibrary_PointToCapsule_Statics::PropPointers), sizeof(Z_Construct_UFunction_UBXFunctionLibrary_PointToCapsule_Statics::BXFunctionLibrary_eventPointToCapsule_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04C22401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXFunctionLibrary_PointToCapsule_Statics::Function_MetaDataParams), Z_Construct_UFunction_UBXFunctionLibrary_PointToCapsule_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UBXFunctionLibrary_PointToCapsule_Statics::BXFunctionLibrary_eventPointToCapsule_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UBXFunctionLibrary_PointToCapsule()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UBXFunctionLibrary_PointToCapsule_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UBXFunctionLibrary::execPointToCapsule)
{
	P_GET_STRUCT_REF(FVector,Z_Param_Out_InP);
	P_GET_STRUCT_REF(FVector,Z_Param_Out_InCenter);
	P_GET_STRUCT_REF(FRotator,Z_Param_Out_InRotation);
	P_GET_PROPERTY_REF(FFloatProperty,Z_Param_Out_InRadius);
	P_GET_PROPERTY_REF(FFloatProperty,Z_Param_Out_InHalfHeight);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(float*)Z_Param__Result=UBXFunctionLibrary::PointToCapsule(Z_Param_Out_InP,Z_Param_Out_InCenter,Z_Param_Out_InRotation,Z_Param_Out_InRadius,Z_Param_Out_InHalfHeight);
	P_NATIVE_END;
}
// End Class UBXFunctionLibrary Function PointToCapsule

// Begin Class UBXFunctionLibrary Function PointToCircle
struct Z_Construct_UFunction_UBXFunctionLibrary_PointToCircle_Statics
{
	struct BXFunctionLibrary_eventPointToCircle_Parms
	{
		FVector InP;
		FVector InCenter;
		FRotator InRotation;
		float InRadius;
		float ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Math" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe8\xae\xa1\xe7\xae\x97\xe7\x82\xb9\xe5\x88\xb0\xe5\x9c\x86\xe5\xbd\xa2\xe7\x9a\x84\xe6\x9c\x80\xe7\x9f\xad\xe8\xb7\x9d\xe7\xa6\xbb\n" },
#endif
		{ "ModuleRelativePath", "BXFunctionLibrary.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe8\xae\xa1\xe7\xae\x97\xe7\x82\xb9\xe5\x88\xb0\xe5\x9c\x86\xe5\xbd\xa2\xe7\x9a\x84\xe6\x9c\x80\xe7\x9f\xad\xe8\xb7\x9d\xe7\xa6\xbb" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InP_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InCenter_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InRotation_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InRadius_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_InP;
	static const UECodeGen_Private::FStructPropertyParams NewProp_InCenter;
	static const UECodeGen_Private::FStructPropertyParams NewProp_InRotation;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_InRadius;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXFunctionLibrary_PointToCircle_Statics::NewProp_InP = { "InP", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventPointToCircle_Parms, InP), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InP_MetaData), NewProp_InP_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXFunctionLibrary_PointToCircle_Statics::NewProp_InCenter = { "InCenter", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventPointToCircle_Parms, InCenter), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InCenter_MetaData), NewProp_InCenter_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXFunctionLibrary_PointToCircle_Statics::NewProp_InRotation = { "InRotation", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventPointToCircle_Parms, InRotation), Z_Construct_UScriptStruct_FRotator, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InRotation_MetaData), NewProp_InRotation_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UBXFunctionLibrary_PointToCircle_Statics::NewProp_InRadius = { "InRadius", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventPointToCircle_Parms, InRadius), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InRadius_MetaData), NewProp_InRadius_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UBXFunctionLibrary_PointToCircle_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventPointToCircle_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UBXFunctionLibrary_PointToCircle_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_PointToCircle_Statics::NewProp_InP,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_PointToCircle_Statics::NewProp_InCenter,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_PointToCircle_Statics::NewProp_InRotation,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_PointToCircle_Statics::NewProp_InRadius,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_PointToCircle_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXFunctionLibrary_PointToCircle_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UBXFunctionLibrary_PointToCircle_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UBXFunctionLibrary, nullptr, "PointToCircle", nullptr, nullptr, Z_Construct_UFunction_UBXFunctionLibrary_PointToCircle_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UBXFunctionLibrary_PointToCircle_Statics::PropPointers), sizeof(Z_Construct_UFunction_UBXFunctionLibrary_PointToCircle_Statics::BXFunctionLibrary_eventPointToCircle_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04C22401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXFunctionLibrary_PointToCircle_Statics::Function_MetaDataParams), Z_Construct_UFunction_UBXFunctionLibrary_PointToCircle_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UBXFunctionLibrary_PointToCircle_Statics::BXFunctionLibrary_eventPointToCircle_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UBXFunctionLibrary_PointToCircle()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UBXFunctionLibrary_PointToCircle_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UBXFunctionLibrary::execPointToCircle)
{
	P_GET_STRUCT_REF(FVector,Z_Param_Out_InP);
	P_GET_STRUCT_REF(FVector,Z_Param_Out_InCenter);
	P_GET_STRUCT_REF(FRotator,Z_Param_Out_InRotation);
	P_GET_PROPERTY_REF(FFloatProperty,Z_Param_Out_InRadius);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(float*)Z_Param__Result=UBXFunctionLibrary::PointToCircle(Z_Param_Out_InP,Z_Param_Out_InCenter,Z_Param_Out_InRotation,Z_Param_Out_InRadius);
	P_NATIVE_END;
}
// End Class UBXFunctionLibrary Function PointToCircle

// Begin Class UBXFunctionLibrary Function PointToCylinder
struct Z_Construct_UFunction_UBXFunctionLibrary_PointToCylinder_Statics
{
	struct BXFunctionLibrary_eventPointToCylinder_Parms
	{
		FVector InP;
		FVector InCenter;
		FRotator InRotation;
		float InRadius;
		float InHalfHeight;
		float ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Math" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe8\xae\xa1\xe7\xae\x97\xe7\x82\xb9\xe5\x88\xb0\xe5\x9c\x86\xe6\x9f\xb1\xe4\xbd\x93\xe7\x9a\x84\xe6\x9c\x80\xe7\x9f\xad\xe8\xb7\x9d\xe7\xa6\xbb\n" },
#endif
		{ "ModuleRelativePath", "BXFunctionLibrary.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe8\xae\xa1\xe7\xae\x97\xe7\x82\xb9\xe5\x88\xb0\xe5\x9c\x86\xe6\x9f\xb1\xe4\xbd\x93\xe7\x9a\x84\xe6\x9c\x80\xe7\x9f\xad\xe8\xb7\x9d\xe7\xa6\xbb" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InP_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InCenter_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InRotation_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InRadius_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InHalfHeight_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_InP;
	static const UECodeGen_Private::FStructPropertyParams NewProp_InCenter;
	static const UECodeGen_Private::FStructPropertyParams NewProp_InRotation;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_InRadius;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_InHalfHeight;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXFunctionLibrary_PointToCylinder_Statics::NewProp_InP = { "InP", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventPointToCylinder_Parms, InP), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InP_MetaData), NewProp_InP_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXFunctionLibrary_PointToCylinder_Statics::NewProp_InCenter = { "InCenter", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventPointToCylinder_Parms, InCenter), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InCenter_MetaData), NewProp_InCenter_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXFunctionLibrary_PointToCylinder_Statics::NewProp_InRotation = { "InRotation", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventPointToCylinder_Parms, InRotation), Z_Construct_UScriptStruct_FRotator, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InRotation_MetaData), NewProp_InRotation_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UBXFunctionLibrary_PointToCylinder_Statics::NewProp_InRadius = { "InRadius", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventPointToCylinder_Parms, InRadius), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InRadius_MetaData), NewProp_InRadius_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UBXFunctionLibrary_PointToCylinder_Statics::NewProp_InHalfHeight = { "InHalfHeight", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventPointToCylinder_Parms, InHalfHeight), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InHalfHeight_MetaData), NewProp_InHalfHeight_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UBXFunctionLibrary_PointToCylinder_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventPointToCylinder_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UBXFunctionLibrary_PointToCylinder_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_PointToCylinder_Statics::NewProp_InP,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_PointToCylinder_Statics::NewProp_InCenter,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_PointToCylinder_Statics::NewProp_InRotation,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_PointToCylinder_Statics::NewProp_InRadius,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_PointToCylinder_Statics::NewProp_InHalfHeight,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_PointToCylinder_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXFunctionLibrary_PointToCylinder_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UBXFunctionLibrary_PointToCylinder_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UBXFunctionLibrary, nullptr, "PointToCylinder", nullptr, nullptr, Z_Construct_UFunction_UBXFunctionLibrary_PointToCylinder_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UBXFunctionLibrary_PointToCylinder_Statics::PropPointers), sizeof(Z_Construct_UFunction_UBXFunctionLibrary_PointToCylinder_Statics::BXFunctionLibrary_eventPointToCylinder_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04C22401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXFunctionLibrary_PointToCylinder_Statics::Function_MetaDataParams), Z_Construct_UFunction_UBXFunctionLibrary_PointToCylinder_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UBXFunctionLibrary_PointToCylinder_Statics::BXFunctionLibrary_eventPointToCylinder_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UBXFunctionLibrary_PointToCylinder()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UBXFunctionLibrary_PointToCylinder_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UBXFunctionLibrary::execPointToCylinder)
{
	P_GET_STRUCT_REF(FVector,Z_Param_Out_InP);
	P_GET_STRUCT_REF(FVector,Z_Param_Out_InCenter);
	P_GET_STRUCT_REF(FRotator,Z_Param_Out_InRotation);
	P_GET_PROPERTY_REF(FFloatProperty,Z_Param_Out_InRadius);
	P_GET_PROPERTY_REF(FFloatProperty,Z_Param_Out_InHalfHeight);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(float*)Z_Param__Result=UBXFunctionLibrary::PointToCylinder(Z_Param_Out_InP,Z_Param_Out_InCenter,Z_Param_Out_InRotation,Z_Param_Out_InRadius,Z_Param_Out_InHalfHeight);
	P_NATIVE_END;
}
// End Class UBXFunctionLibrary Function PointToCylinder

// Begin Class UBXFunctionLibrary Function PointToRectangle
struct Z_Construct_UFunction_UBXFunctionLibrary_PointToRectangle_Statics
{
	struct BXFunctionLibrary_eventPointToRectangle_Parms
	{
		FVector InP;
		FVector InCenter;
		FRotator InRotation;
		FVector2D InExtent;
		float ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Math" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe8\xae\xa1\xe7\xae\x97\xe7\x82\xb9\xe5\x88\xb0\xe7\x9f\xa9\xe5\xbd\xa2\xe7\x9a\x84\xe6\x9c\x80\xe7\x9f\xad\xe8\xb7\x9d\xe7\xa6\xbb\n" },
#endif
		{ "ModuleRelativePath", "BXFunctionLibrary.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe8\xae\xa1\xe7\xae\x97\xe7\x82\xb9\xe5\x88\xb0\xe7\x9f\xa9\xe5\xbd\xa2\xe7\x9a\x84\xe6\x9c\x80\xe7\x9f\xad\xe8\xb7\x9d\xe7\xa6\xbb" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InP_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InCenter_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InRotation_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InExtent_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_InP;
	static const UECodeGen_Private::FStructPropertyParams NewProp_InCenter;
	static const UECodeGen_Private::FStructPropertyParams NewProp_InRotation;
	static const UECodeGen_Private::FStructPropertyParams NewProp_InExtent;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXFunctionLibrary_PointToRectangle_Statics::NewProp_InP = { "InP", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventPointToRectangle_Parms, InP), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InP_MetaData), NewProp_InP_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXFunctionLibrary_PointToRectangle_Statics::NewProp_InCenter = { "InCenter", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventPointToRectangle_Parms, InCenter), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InCenter_MetaData), NewProp_InCenter_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXFunctionLibrary_PointToRectangle_Statics::NewProp_InRotation = { "InRotation", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventPointToRectangle_Parms, InRotation), Z_Construct_UScriptStruct_FRotator, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InRotation_MetaData), NewProp_InRotation_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXFunctionLibrary_PointToRectangle_Statics::NewProp_InExtent = { "InExtent", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventPointToRectangle_Parms, InExtent), Z_Construct_UScriptStruct_FVector2D, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InExtent_MetaData), NewProp_InExtent_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UBXFunctionLibrary_PointToRectangle_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventPointToRectangle_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UBXFunctionLibrary_PointToRectangle_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_PointToRectangle_Statics::NewProp_InP,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_PointToRectangle_Statics::NewProp_InCenter,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_PointToRectangle_Statics::NewProp_InRotation,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_PointToRectangle_Statics::NewProp_InExtent,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_PointToRectangle_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXFunctionLibrary_PointToRectangle_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UBXFunctionLibrary_PointToRectangle_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UBXFunctionLibrary, nullptr, "PointToRectangle", nullptr, nullptr, Z_Construct_UFunction_UBXFunctionLibrary_PointToRectangle_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UBXFunctionLibrary_PointToRectangle_Statics::PropPointers), sizeof(Z_Construct_UFunction_UBXFunctionLibrary_PointToRectangle_Statics::BXFunctionLibrary_eventPointToRectangle_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04C22401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXFunctionLibrary_PointToRectangle_Statics::Function_MetaDataParams), Z_Construct_UFunction_UBXFunctionLibrary_PointToRectangle_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UBXFunctionLibrary_PointToRectangle_Statics::BXFunctionLibrary_eventPointToRectangle_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UBXFunctionLibrary_PointToRectangle()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UBXFunctionLibrary_PointToRectangle_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UBXFunctionLibrary::execPointToRectangle)
{
	P_GET_STRUCT_REF(FVector,Z_Param_Out_InP);
	P_GET_STRUCT_REF(FVector,Z_Param_Out_InCenter);
	P_GET_STRUCT_REF(FRotator,Z_Param_Out_InRotation);
	P_GET_STRUCT_REF(FVector2D,Z_Param_Out_InExtent);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(float*)Z_Param__Result=UBXFunctionLibrary::PointToRectangle(Z_Param_Out_InP,Z_Param_Out_InCenter,Z_Param_Out_InRotation,Z_Param_Out_InExtent);
	P_NATIVE_END;
}
// End Class UBXFunctionLibrary Function PointToRectangle

// Begin Class UBXFunctionLibrary Function PointToSegment
struct Z_Construct_UFunction_UBXFunctionLibrary_PointToSegment_Statics
{
	struct BXFunctionLibrary_eventPointToSegment_Parms
	{
		FVector InP;
		FVector InLS;
		FVector InLE;
		float ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Math" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe8\xae\xa1\xe7\xae\x97\xe7\x82\xb9\xe5\x88\xb0\xe7\xba\xbf\xe6\xae\xb5\xe7\x9a\x84\xe6\x9c\x80\xe7\x9f\xad\xe8\xb7\x9d\xe7\xa6\xbb\n" },
#endif
		{ "ModuleRelativePath", "BXFunctionLibrary.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe8\xae\xa1\xe7\xae\x97\xe7\x82\xb9\xe5\x88\xb0\xe7\xba\xbf\xe6\xae\xb5\xe7\x9a\x84\xe6\x9c\x80\xe7\x9f\xad\xe8\xb7\x9d\xe7\xa6\xbb" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InP_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InLS_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InLE_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_InP;
	static const UECodeGen_Private::FStructPropertyParams NewProp_InLS;
	static const UECodeGen_Private::FStructPropertyParams NewProp_InLE;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXFunctionLibrary_PointToSegment_Statics::NewProp_InP = { "InP", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventPointToSegment_Parms, InP), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InP_MetaData), NewProp_InP_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXFunctionLibrary_PointToSegment_Statics::NewProp_InLS = { "InLS", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventPointToSegment_Parms, InLS), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InLS_MetaData), NewProp_InLS_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXFunctionLibrary_PointToSegment_Statics::NewProp_InLE = { "InLE", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventPointToSegment_Parms, InLE), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InLE_MetaData), NewProp_InLE_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UBXFunctionLibrary_PointToSegment_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventPointToSegment_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UBXFunctionLibrary_PointToSegment_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_PointToSegment_Statics::NewProp_InP,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_PointToSegment_Statics::NewProp_InLS,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_PointToSegment_Statics::NewProp_InLE,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_PointToSegment_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXFunctionLibrary_PointToSegment_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UBXFunctionLibrary_PointToSegment_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UBXFunctionLibrary, nullptr, "PointToSegment", nullptr, nullptr, Z_Construct_UFunction_UBXFunctionLibrary_PointToSegment_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UBXFunctionLibrary_PointToSegment_Statics::PropPointers), sizeof(Z_Construct_UFunction_UBXFunctionLibrary_PointToSegment_Statics::BXFunctionLibrary_eventPointToSegment_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04C22401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXFunctionLibrary_PointToSegment_Statics::Function_MetaDataParams), Z_Construct_UFunction_UBXFunctionLibrary_PointToSegment_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UBXFunctionLibrary_PointToSegment_Statics::BXFunctionLibrary_eventPointToSegment_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UBXFunctionLibrary_PointToSegment()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UBXFunctionLibrary_PointToSegment_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UBXFunctionLibrary::execPointToSegment)
{
	P_GET_STRUCT_REF(FVector,Z_Param_Out_InP);
	P_GET_STRUCT_REF(FVector,Z_Param_Out_InLS);
	P_GET_STRUCT_REF(FVector,Z_Param_Out_InLE);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(float*)Z_Param__Result=UBXFunctionLibrary::PointToSegment(Z_Param_Out_InP,Z_Param_Out_InLS,Z_Param_Out_InLE);
	P_NATIVE_END;
}
// End Class UBXFunctionLibrary Function PointToSegment

// Begin Class UBXFunctionLibrary Function PointToSphere
struct Z_Construct_UFunction_UBXFunctionLibrary_PointToSphere_Statics
{
	struct BXFunctionLibrary_eventPointToSphere_Parms
	{
		FVector InP;
		FVector InCenter;
		float InRadius;
		float ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Math" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe8\xae\xa1\xe7\xae\x97\xe7\x82\xb9\xe5\x88\xb0\xe7\x90\x83\xe4\xbd\x93\xe7\x9a\x84\xe6\x9c\x80\xe7\x9f\xad\xe8\xb7\x9d\xe7\xa6\xbb\n" },
#endif
		{ "ModuleRelativePath", "BXFunctionLibrary.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe8\xae\xa1\xe7\xae\x97\xe7\x82\xb9\xe5\x88\xb0\xe7\x90\x83\xe4\xbd\x93\xe7\x9a\x84\xe6\x9c\x80\xe7\x9f\xad\xe8\xb7\x9d\xe7\xa6\xbb" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InP_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InCenter_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InRadius_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_InP;
	static const UECodeGen_Private::FStructPropertyParams NewProp_InCenter;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_InRadius;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXFunctionLibrary_PointToSphere_Statics::NewProp_InP = { "InP", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventPointToSphere_Parms, InP), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InP_MetaData), NewProp_InP_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXFunctionLibrary_PointToSphere_Statics::NewProp_InCenter = { "InCenter", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventPointToSphere_Parms, InCenter), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InCenter_MetaData), NewProp_InCenter_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UBXFunctionLibrary_PointToSphere_Statics::NewProp_InRadius = { "InRadius", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventPointToSphere_Parms, InRadius), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InRadius_MetaData), NewProp_InRadius_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UBXFunctionLibrary_PointToSphere_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventPointToSphere_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UBXFunctionLibrary_PointToSphere_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_PointToSphere_Statics::NewProp_InP,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_PointToSphere_Statics::NewProp_InCenter,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_PointToSphere_Statics::NewProp_InRadius,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_PointToSphere_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXFunctionLibrary_PointToSphere_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UBXFunctionLibrary_PointToSphere_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UBXFunctionLibrary, nullptr, "PointToSphere", nullptr, nullptr, Z_Construct_UFunction_UBXFunctionLibrary_PointToSphere_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UBXFunctionLibrary_PointToSphere_Statics::PropPointers), sizeof(Z_Construct_UFunction_UBXFunctionLibrary_PointToSphere_Statics::BXFunctionLibrary_eventPointToSphere_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04C22401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXFunctionLibrary_PointToSphere_Statics::Function_MetaDataParams), Z_Construct_UFunction_UBXFunctionLibrary_PointToSphere_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UBXFunctionLibrary_PointToSphere_Statics::BXFunctionLibrary_eventPointToSphere_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UBXFunctionLibrary_PointToSphere()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UBXFunctionLibrary_PointToSphere_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UBXFunctionLibrary::execPointToSphere)
{
	P_GET_STRUCT_REF(FVector,Z_Param_Out_InP);
	P_GET_STRUCT_REF(FVector,Z_Param_Out_InCenter);
	P_GET_PROPERTY_REF(FFloatProperty,Z_Param_Out_InRadius);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(float*)Z_Param__Result=UBXFunctionLibrary::PointToSphere(Z_Param_Out_InP,Z_Param_Out_InCenter,Z_Param_Out_InRadius);
	P_NATIVE_END;
}
// End Class UBXFunctionLibrary Function PointToSphere

// Begin Class UBXFunctionLibrary Function SegmentToBox
struct Z_Construct_UFunction_UBXFunctionLibrary_SegmentToBox_Statics
{
	struct BXFunctionLibrary_eventSegmentToBox_Parms
	{
		FVector InL1S;
		FVector InL1E;
		FVector InCenter;
		FRotator InRotation;
		FVector InExtent;
		float ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Math" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe8\xae\xa1\xe7\xae\x97\xe7\xba\xbf\xe6\xae\xb5\xe5\x88\xb0\xe9\x95\xbf\xe6\x96\xb9\xe4\xbd\x93\xe4\xb9\x8b\xe9\x97\xb4\xe7\x9a\x84\xe6\x9c\x80\xe7\x9f\xad\xe8\xb7\x9d\xe7\xa6\xbb\n" },
#endif
		{ "ModuleRelativePath", "BXFunctionLibrary.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe8\xae\xa1\xe7\xae\x97\xe7\xba\xbf\xe6\xae\xb5\xe5\x88\xb0\xe9\x95\xbf\xe6\x96\xb9\xe4\xbd\x93\xe4\xb9\x8b\xe9\x97\xb4\xe7\x9a\x84\xe6\x9c\x80\xe7\x9f\xad\xe8\xb7\x9d\xe7\xa6\xbb" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InL1S_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InL1E_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InCenter_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InRotation_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InExtent_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_InL1S;
	static const UECodeGen_Private::FStructPropertyParams NewProp_InL1E;
	static const UECodeGen_Private::FStructPropertyParams NewProp_InCenter;
	static const UECodeGen_Private::FStructPropertyParams NewProp_InRotation;
	static const UECodeGen_Private::FStructPropertyParams NewProp_InExtent;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXFunctionLibrary_SegmentToBox_Statics::NewProp_InL1S = { "InL1S", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventSegmentToBox_Parms, InL1S), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InL1S_MetaData), NewProp_InL1S_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXFunctionLibrary_SegmentToBox_Statics::NewProp_InL1E = { "InL1E", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventSegmentToBox_Parms, InL1E), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InL1E_MetaData), NewProp_InL1E_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXFunctionLibrary_SegmentToBox_Statics::NewProp_InCenter = { "InCenter", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventSegmentToBox_Parms, InCenter), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InCenter_MetaData), NewProp_InCenter_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXFunctionLibrary_SegmentToBox_Statics::NewProp_InRotation = { "InRotation", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventSegmentToBox_Parms, InRotation), Z_Construct_UScriptStruct_FRotator, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InRotation_MetaData), NewProp_InRotation_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXFunctionLibrary_SegmentToBox_Statics::NewProp_InExtent = { "InExtent", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventSegmentToBox_Parms, InExtent), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InExtent_MetaData), NewProp_InExtent_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UBXFunctionLibrary_SegmentToBox_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventSegmentToBox_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UBXFunctionLibrary_SegmentToBox_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_SegmentToBox_Statics::NewProp_InL1S,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_SegmentToBox_Statics::NewProp_InL1E,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_SegmentToBox_Statics::NewProp_InCenter,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_SegmentToBox_Statics::NewProp_InRotation,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_SegmentToBox_Statics::NewProp_InExtent,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_SegmentToBox_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXFunctionLibrary_SegmentToBox_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UBXFunctionLibrary_SegmentToBox_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UBXFunctionLibrary, nullptr, "SegmentToBox", nullptr, nullptr, Z_Construct_UFunction_UBXFunctionLibrary_SegmentToBox_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UBXFunctionLibrary_SegmentToBox_Statics::PropPointers), sizeof(Z_Construct_UFunction_UBXFunctionLibrary_SegmentToBox_Statics::BXFunctionLibrary_eventSegmentToBox_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04C22401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXFunctionLibrary_SegmentToBox_Statics::Function_MetaDataParams), Z_Construct_UFunction_UBXFunctionLibrary_SegmentToBox_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UBXFunctionLibrary_SegmentToBox_Statics::BXFunctionLibrary_eventSegmentToBox_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UBXFunctionLibrary_SegmentToBox()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UBXFunctionLibrary_SegmentToBox_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UBXFunctionLibrary::execSegmentToBox)
{
	P_GET_STRUCT_REF(FVector,Z_Param_Out_InL1S);
	P_GET_STRUCT_REF(FVector,Z_Param_Out_InL1E);
	P_GET_STRUCT_REF(FVector,Z_Param_Out_InCenter);
	P_GET_STRUCT_REF(FRotator,Z_Param_Out_InRotation);
	P_GET_STRUCT_REF(FVector,Z_Param_Out_InExtent);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(float*)Z_Param__Result=UBXFunctionLibrary::SegmentToBox(Z_Param_Out_InL1S,Z_Param_Out_InL1E,Z_Param_Out_InCenter,Z_Param_Out_InRotation,Z_Param_Out_InExtent);
	P_NATIVE_END;
}
// End Class UBXFunctionLibrary Function SegmentToBox

// Begin Class UBXFunctionLibrary Function SegmentToCapsule
struct Z_Construct_UFunction_UBXFunctionLibrary_SegmentToCapsule_Statics
{
	struct BXFunctionLibrary_eventSegmentToCapsule_Parms
	{
		FVector InL1S;
		FVector InL1E;
		FVector InCenter;
		FRotator InRotation;
		float InRadius;
		float InHalfHeight;
		float ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Math" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe8\xae\xa1\xe7\xae\x97\xe7\xba\xbf\xe6\xae\xb5\xe5\x88\xb0\xe8\x83\xb6\xe5\x9b\x8a\xe4\xbd\x93\xe4\xb9\x8b\xe9\x97\xb4\xe7\x9a\x84\xe6\x9c\x80\xe7\x9f\xad\xe8\xb7\x9d\xe7\xa6\xbb\n" },
#endif
		{ "ModuleRelativePath", "BXFunctionLibrary.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe8\xae\xa1\xe7\xae\x97\xe7\xba\xbf\xe6\xae\xb5\xe5\x88\xb0\xe8\x83\xb6\xe5\x9b\x8a\xe4\xbd\x93\xe4\xb9\x8b\xe9\x97\xb4\xe7\x9a\x84\xe6\x9c\x80\xe7\x9f\xad\xe8\xb7\x9d\xe7\xa6\xbb" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InL1S_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InL1E_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InCenter_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InRotation_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InRadius_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InHalfHeight_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_InL1S;
	static const UECodeGen_Private::FStructPropertyParams NewProp_InL1E;
	static const UECodeGen_Private::FStructPropertyParams NewProp_InCenter;
	static const UECodeGen_Private::FStructPropertyParams NewProp_InRotation;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_InRadius;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_InHalfHeight;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXFunctionLibrary_SegmentToCapsule_Statics::NewProp_InL1S = { "InL1S", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventSegmentToCapsule_Parms, InL1S), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InL1S_MetaData), NewProp_InL1S_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXFunctionLibrary_SegmentToCapsule_Statics::NewProp_InL1E = { "InL1E", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventSegmentToCapsule_Parms, InL1E), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InL1E_MetaData), NewProp_InL1E_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXFunctionLibrary_SegmentToCapsule_Statics::NewProp_InCenter = { "InCenter", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventSegmentToCapsule_Parms, InCenter), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InCenter_MetaData), NewProp_InCenter_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXFunctionLibrary_SegmentToCapsule_Statics::NewProp_InRotation = { "InRotation", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventSegmentToCapsule_Parms, InRotation), Z_Construct_UScriptStruct_FRotator, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InRotation_MetaData), NewProp_InRotation_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UBXFunctionLibrary_SegmentToCapsule_Statics::NewProp_InRadius = { "InRadius", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventSegmentToCapsule_Parms, InRadius), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InRadius_MetaData), NewProp_InRadius_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UBXFunctionLibrary_SegmentToCapsule_Statics::NewProp_InHalfHeight = { "InHalfHeight", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventSegmentToCapsule_Parms, InHalfHeight), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InHalfHeight_MetaData), NewProp_InHalfHeight_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UBXFunctionLibrary_SegmentToCapsule_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventSegmentToCapsule_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UBXFunctionLibrary_SegmentToCapsule_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_SegmentToCapsule_Statics::NewProp_InL1S,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_SegmentToCapsule_Statics::NewProp_InL1E,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_SegmentToCapsule_Statics::NewProp_InCenter,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_SegmentToCapsule_Statics::NewProp_InRotation,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_SegmentToCapsule_Statics::NewProp_InRadius,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_SegmentToCapsule_Statics::NewProp_InHalfHeight,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_SegmentToCapsule_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXFunctionLibrary_SegmentToCapsule_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UBXFunctionLibrary_SegmentToCapsule_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UBXFunctionLibrary, nullptr, "SegmentToCapsule", nullptr, nullptr, Z_Construct_UFunction_UBXFunctionLibrary_SegmentToCapsule_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UBXFunctionLibrary_SegmentToCapsule_Statics::PropPointers), sizeof(Z_Construct_UFunction_UBXFunctionLibrary_SegmentToCapsule_Statics::BXFunctionLibrary_eventSegmentToCapsule_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04C22401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXFunctionLibrary_SegmentToCapsule_Statics::Function_MetaDataParams), Z_Construct_UFunction_UBXFunctionLibrary_SegmentToCapsule_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UBXFunctionLibrary_SegmentToCapsule_Statics::BXFunctionLibrary_eventSegmentToCapsule_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UBXFunctionLibrary_SegmentToCapsule()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UBXFunctionLibrary_SegmentToCapsule_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UBXFunctionLibrary::execSegmentToCapsule)
{
	P_GET_STRUCT_REF(FVector,Z_Param_Out_InL1S);
	P_GET_STRUCT_REF(FVector,Z_Param_Out_InL1E);
	P_GET_STRUCT_REF(FVector,Z_Param_Out_InCenter);
	P_GET_STRUCT_REF(FRotator,Z_Param_Out_InRotation);
	P_GET_PROPERTY_REF(FFloatProperty,Z_Param_Out_InRadius);
	P_GET_PROPERTY_REF(FFloatProperty,Z_Param_Out_InHalfHeight);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(float*)Z_Param__Result=UBXFunctionLibrary::SegmentToCapsule(Z_Param_Out_InL1S,Z_Param_Out_InL1E,Z_Param_Out_InCenter,Z_Param_Out_InRotation,Z_Param_Out_InRadius,Z_Param_Out_InHalfHeight);
	P_NATIVE_END;
}
// End Class UBXFunctionLibrary Function SegmentToCapsule

// Begin Class UBXFunctionLibrary Function SegmentToSegment
struct Z_Construct_UFunction_UBXFunctionLibrary_SegmentToSegment_Statics
{
	struct BXFunctionLibrary_eventSegmentToSegment_Parms
	{
		FVector InL1S;
		FVector InL1E;
		FVector InL2S;
		FVector InL2E;
		float ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Math" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe8\xae\xa1\xe7\xae\x97\xe7\xba\xbf\xe6\xae\xb5\xe5\x88\xb0\xe7\xba\xbf\xe6\xae\xb5\xe4\xb9\x8b\xe9\x97\xb4\xe7\x9a\x84\xe6\x9c\x80\xe7\x9f\xad\xe8\xb7\x9d\xe7\xa6\xbb\n" },
#endif
		{ "ModuleRelativePath", "BXFunctionLibrary.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe8\xae\xa1\xe7\xae\x97\xe7\xba\xbf\xe6\xae\xb5\xe5\x88\xb0\xe7\xba\xbf\xe6\xae\xb5\xe4\xb9\x8b\xe9\x97\xb4\xe7\x9a\x84\xe6\x9c\x80\xe7\x9f\xad\xe8\xb7\x9d\xe7\xa6\xbb" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InL1S_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InL1E_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InL2S_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InL2E_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_InL1S;
	static const UECodeGen_Private::FStructPropertyParams NewProp_InL1E;
	static const UECodeGen_Private::FStructPropertyParams NewProp_InL2S;
	static const UECodeGen_Private::FStructPropertyParams NewProp_InL2E;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXFunctionLibrary_SegmentToSegment_Statics::NewProp_InL1S = { "InL1S", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventSegmentToSegment_Parms, InL1S), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InL1S_MetaData), NewProp_InL1S_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXFunctionLibrary_SegmentToSegment_Statics::NewProp_InL1E = { "InL1E", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventSegmentToSegment_Parms, InL1E), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InL1E_MetaData), NewProp_InL1E_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXFunctionLibrary_SegmentToSegment_Statics::NewProp_InL2S = { "InL2S", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventSegmentToSegment_Parms, InL2S), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InL2S_MetaData), NewProp_InL2S_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXFunctionLibrary_SegmentToSegment_Statics::NewProp_InL2E = { "InL2E", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventSegmentToSegment_Parms, InL2E), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InL2E_MetaData), NewProp_InL2E_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UBXFunctionLibrary_SegmentToSegment_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventSegmentToSegment_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UBXFunctionLibrary_SegmentToSegment_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_SegmentToSegment_Statics::NewProp_InL1S,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_SegmentToSegment_Statics::NewProp_InL1E,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_SegmentToSegment_Statics::NewProp_InL2S,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_SegmentToSegment_Statics::NewProp_InL2E,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_SegmentToSegment_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXFunctionLibrary_SegmentToSegment_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UBXFunctionLibrary_SegmentToSegment_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UBXFunctionLibrary, nullptr, "SegmentToSegment", nullptr, nullptr, Z_Construct_UFunction_UBXFunctionLibrary_SegmentToSegment_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UBXFunctionLibrary_SegmentToSegment_Statics::PropPointers), sizeof(Z_Construct_UFunction_UBXFunctionLibrary_SegmentToSegment_Statics::BXFunctionLibrary_eventSegmentToSegment_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04C22401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXFunctionLibrary_SegmentToSegment_Statics::Function_MetaDataParams), Z_Construct_UFunction_UBXFunctionLibrary_SegmentToSegment_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UBXFunctionLibrary_SegmentToSegment_Statics::BXFunctionLibrary_eventSegmentToSegment_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UBXFunctionLibrary_SegmentToSegment()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UBXFunctionLibrary_SegmentToSegment_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UBXFunctionLibrary::execSegmentToSegment)
{
	P_GET_STRUCT_REF(FVector,Z_Param_Out_InL1S);
	P_GET_STRUCT_REF(FVector,Z_Param_Out_InL1E);
	P_GET_STRUCT_REF(FVector,Z_Param_Out_InL2S);
	P_GET_STRUCT_REF(FVector,Z_Param_Out_InL2E);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(float*)Z_Param__Result=UBXFunctionLibrary::SegmentToSegment(Z_Param_Out_InL1S,Z_Param_Out_InL1E,Z_Param_Out_InL2S,Z_Param_Out_InL2E);
	P_NATIVE_END;
}
// End Class UBXFunctionLibrary Function SegmentToSegment

// Begin Class UBXFunctionLibrary Function SegmentToSphere
struct Z_Construct_UFunction_UBXFunctionLibrary_SegmentToSphere_Statics
{
	struct BXFunctionLibrary_eventSegmentToSphere_Parms
	{
		FVector InL1S;
		FVector InL1E;
		FVector InCenter;
		float InRadius;
		float ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Math" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe8\xae\xa1\xe7\xae\x97\xe7\xba\xbf\xe6\xae\xb5\xe5\x88\xb0\xe7\x90\x83\xe4\xbd\x93\xe4\xb9\x8b\xe9\x97\xb4\xe7\x9a\x84\xe6\x9c\x80\xe7\x9f\xad\xe8\xb7\x9d\xe7\xa6\xbb\n" },
#endif
		{ "ModuleRelativePath", "BXFunctionLibrary.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe8\xae\xa1\xe7\xae\x97\xe7\xba\xbf\xe6\xae\xb5\xe5\x88\xb0\xe7\x90\x83\xe4\xbd\x93\xe4\xb9\x8b\xe9\x97\xb4\xe7\x9a\x84\xe6\x9c\x80\xe7\x9f\xad\xe8\xb7\x9d\xe7\xa6\xbb" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InL1S_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InL1E_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InCenter_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InRadius_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_InL1S;
	static const UECodeGen_Private::FStructPropertyParams NewProp_InL1E;
	static const UECodeGen_Private::FStructPropertyParams NewProp_InCenter;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_InRadius;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXFunctionLibrary_SegmentToSphere_Statics::NewProp_InL1S = { "InL1S", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventSegmentToSphere_Parms, InL1S), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InL1S_MetaData), NewProp_InL1S_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXFunctionLibrary_SegmentToSphere_Statics::NewProp_InL1E = { "InL1E", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventSegmentToSphere_Parms, InL1E), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InL1E_MetaData), NewProp_InL1E_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXFunctionLibrary_SegmentToSphere_Statics::NewProp_InCenter = { "InCenter", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventSegmentToSphere_Parms, InCenter), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InCenter_MetaData), NewProp_InCenter_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UBXFunctionLibrary_SegmentToSphere_Statics::NewProp_InRadius = { "InRadius", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventSegmentToSphere_Parms, InRadius), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InRadius_MetaData), NewProp_InRadius_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UBXFunctionLibrary_SegmentToSphere_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXFunctionLibrary_eventSegmentToSphere_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UBXFunctionLibrary_SegmentToSphere_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_SegmentToSphere_Statics::NewProp_InL1S,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_SegmentToSphere_Statics::NewProp_InL1E,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_SegmentToSphere_Statics::NewProp_InCenter,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_SegmentToSphere_Statics::NewProp_InRadius,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXFunctionLibrary_SegmentToSphere_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXFunctionLibrary_SegmentToSphere_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UBXFunctionLibrary_SegmentToSphere_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UBXFunctionLibrary, nullptr, "SegmentToSphere", nullptr, nullptr, Z_Construct_UFunction_UBXFunctionLibrary_SegmentToSphere_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UBXFunctionLibrary_SegmentToSphere_Statics::PropPointers), sizeof(Z_Construct_UFunction_UBXFunctionLibrary_SegmentToSphere_Statics::BXFunctionLibrary_eventSegmentToSphere_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04C22401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXFunctionLibrary_SegmentToSphere_Statics::Function_MetaDataParams), Z_Construct_UFunction_UBXFunctionLibrary_SegmentToSphere_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UBXFunctionLibrary_SegmentToSphere_Statics::BXFunctionLibrary_eventSegmentToSphere_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UBXFunctionLibrary_SegmentToSphere()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UBXFunctionLibrary_SegmentToSphere_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UBXFunctionLibrary::execSegmentToSphere)
{
	P_GET_STRUCT_REF(FVector,Z_Param_Out_InL1S);
	P_GET_STRUCT_REF(FVector,Z_Param_Out_InL1E);
	P_GET_STRUCT_REF(FVector,Z_Param_Out_InCenter);
	P_GET_PROPERTY_REF(FFloatProperty,Z_Param_Out_InRadius);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(float*)Z_Param__Result=UBXFunctionLibrary::SegmentToSphere(Z_Param_Out_InL1S,Z_Param_Out_InL1E,Z_Param_Out_InCenter,Z_Param_Out_InRadius);
	P_NATIVE_END;
}
// End Class UBXFunctionLibrary Function SegmentToSphere

// Begin Class UBXFunctionLibrary
void UBXFunctionLibrary::StaticRegisterNativesUBXFunctionLibrary()
{
	UClass* Class = UBXFunctionLibrary::StaticClass();
	static const FNameNativePtrPair Funcs[] = {
		{ "AlignTime", &UBXFunctionLibrary::execAlignTime },
		{ "CopyObject", &UBXFunctionLibrary::execCopyObject },
		{ "GetGameMicrosecond", &UBXFunctionLibrary::execGetGameMicrosecond },
		{ "GetSceneComponentByNameAndClass", &UBXFunctionLibrary::execGetSceneComponentByNameAndClass },
		{ "GetShortUniqueID", &UBXFunctionLibrary::execGetShortUniqueID },
		{ "GetSoftTaskFullIndex", &UBXFunctionLibrary::execGetSoftTaskFullIndex },
		{ "GetTaskFullIndex", &UBXFunctionLibrary::execGetTaskFullIndex },
		{ "GetUniqueID", &UBXFunctionLibrary::execGetUniqueID },
		{ "GetUtcMillisecond", &UBXFunctionLibrary::execGetUtcMillisecond },
		{ "PointToBox", &UBXFunctionLibrary::execPointToBox },
		{ "PointToCapsule", &UBXFunctionLibrary::execPointToCapsule },
		{ "PointToCircle", &UBXFunctionLibrary::execPointToCircle },
		{ "PointToCylinder", &UBXFunctionLibrary::execPointToCylinder },
		{ "PointToRectangle", &UBXFunctionLibrary::execPointToRectangle },
		{ "PointToSegment", &UBXFunctionLibrary::execPointToSegment },
		{ "PointToSphere", &UBXFunctionLibrary::execPointToSphere },
		{ "SegmentToBox", &UBXFunctionLibrary::execSegmentToBox },
		{ "SegmentToCapsule", &UBXFunctionLibrary::execSegmentToCapsule },
		{ "SegmentToSegment", &UBXFunctionLibrary::execSegmentToSegment },
		{ "SegmentToSphere", &UBXFunctionLibrary::execSegmentToSphere },
	};
	FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UBXFunctionLibrary);
UClass* Z_Construct_UClass_UBXFunctionLibrary_NoRegister()
{
	return UBXFunctionLibrary::StaticClass();
}
struct Z_Construct_UClass_UBXFunctionLibrary_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "IncludePath", "BXFunctionLibrary.h" },
		{ "IsBlueprintBase", "true" },
		{ "ModuleRelativePath", "BXFunctionLibrary.h" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UBXFunctionLibrary_AlignTime, "AlignTime" }, // 1886905297
		{ &Z_Construct_UFunction_UBXFunctionLibrary_CopyObject, "CopyObject" }, // 2577207529
		{ &Z_Construct_UFunction_UBXFunctionLibrary_GetGameMicrosecond, "GetGameMicrosecond" }, // 102814641
		{ &Z_Construct_UFunction_UBXFunctionLibrary_GetSceneComponentByNameAndClass, "GetSceneComponentByNameAndClass" }, // 3925285226
		{ &Z_Construct_UFunction_UBXFunctionLibrary_GetShortUniqueID, "GetShortUniqueID" }, // 2517489430
		{ &Z_Construct_UFunction_UBXFunctionLibrary_GetSoftTaskFullIndex, "GetSoftTaskFullIndex" }, // 67061833
		{ &Z_Construct_UFunction_UBXFunctionLibrary_GetTaskFullIndex, "GetTaskFullIndex" }, // 216371196
		{ &Z_Construct_UFunction_UBXFunctionLibrary_GetUniqueID, "GetUniqueID" }, // 3242277954
		{ &Z_Construct_UFunction_UBXFunctionLibrary_GetUtcMillisecond, "GetUtcMillisecond" }, // 1388916242
		{ &Z_Construct_UFunction_UBXFunctionLibrary_PointToBox, "PointToBox" }, // 4235429983
		{ &Z_Construct_UFunction_UBXFunctionLibrary_PointToCapsule, "PointToCapsule" }, // 2921146617
		{ &Z_Construct_UFunction_UBXFunctionLibrary_PointToCircle, "PointToCircle" }, // 56077680
		{ &Z_Construct_UFunction_UBXFunctionLibrary_PointToCylinder, "PointToCylinder" }, // 2632305260
		{ &Z_Construct_UFunction_UBXFunctionLibrary_PointToRectangle, "PointToRectangle" }, // 1189236708
		{ &Z_Construct_UFunction_UBXFunctionLibrary_PointToSegment, "PointToSegment" }, // 3874560245
		{ &Z_Construct_UFunction_UBXFunctionLibrary_PointToSphere, "PointToSphere" }, // 3741992450
		{ &Z_Construct_UFunction_UBXFunctionLibrary_SegmentToBox, "SegmentToBox" }, // 3232544598
		{ &Z_Construct_UFunction_UBXFunctionLibrary_SegmentToCapsule, "SegmentToCapsule" }, // 975744357
		{ &Z_Construct_UFunction_UBXFunctionLibrary_SegmentToSegment, "SegmentToSegment" }, // 566148232
		{ &Z_Construct_UFunction_UBXFunctionLibrary_SegmentToSphere, "SegmentToSphere" }, // 2846608818
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UBXFunctionLibrary>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_UBXFunctionLibrary_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UBlueprintFunctionLibrary,
	(UObject* (*)())Z_Construct_UPackage__Script_BattleX,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UBXFunctionLibrary_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UBXFunctionLibrary_Statics::ClassParams = {
	&UBXFunctionLibrary::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	nullptr,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	0,
	0,
	0x001000A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UBXFunctionLibrary_Statics::Class_MetaDataParams), Z_Construct_UClass_UBXFunctionLibrary_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UBXFunctionLibrary()
{
	if (!Z_Registration_Info_UClass_UBXFunctionLibrary.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UBXFunctionLibrary.OuterSingleton, Z_Construct_UClass_UBXFunctionLibrary_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UBXFunctionLibrary.OuterSingleton;
}
template<> BATTLEX_API UClass* StaticClass<UBXFunctionLibrary>()
{
	return UBXFunctionLibrary::StaticClass();
}
UBXFunctionLibrary::UBXFunctionLibrary(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UBXFunctionLibrary);
UBXFunctionLibrary::~UBXFunctionLibrary() {}
// End Class UBXFunctionLibrary

// Begin Registration
struct Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_BXFunctionLibrary_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UBXFunctionLibrary, UBXFunctionLibrary::StaticClass, TEXT("UBXFunctionLibrary"), &Z_Registration_Info_UClass_UBXFunctionLibrary, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UBXFunctionLibrary), 4252769127U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_BXFunctionLibrary_h_614046660(TEXT("/Script/BattleX"),
	Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_BXFunctionLibrary_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_BXFunctionLibrary_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
