// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "BattleX/Task/BXTStructs.h"
#include "BattleX/BXStructs.h"
#include "Runtime/Engine/Classes/Engine/HitResult.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeBXTStructs() {}

// Begin Cross Module References
BATTLEX_API UClass* Z_Construct_UClass_UBXTask_NoRegister();
BATTLEX_API UEnum* Z_Construct_UEnum_BattleX_EBXDataType();
BATTLEX_API UEnum* Z_Construct_UEnum_BattleX_EBXTConnectionType();
BATTLEX_API UEnum* Z_Construct_UEnum_BattleX_EBXTCoordinateType();
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXBoneSelector();
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXTEvent();
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXTHitResults();
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXTInputInfo();
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXTObject();
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXTOutputInfo();
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXTTransformCreater();
COREUOBJECT_API UClass* Z_Construct_UClass_UObject_NoRegister();
COREUOBJECT_API UClass* Z_Construct_UClass_UScriptStruct();
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FTransform();
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FVector();
ENGINE_API UScriptStruct* Z_Construct_UScriptStruct_FHitResult();
UPackage* Z_Construct_UPackage__Script_BattleX();
// End Cross Module References

// Begin ScriptStruct FBXTEvent
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_BXTEvent;
class UScriptStruct* FBXTEvent::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_BXTEvent.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_BXTEvent.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FBXTEvent, (UObject*)Z_Construct_UPackage__Script_BattleX(), TEXT("BXTEvent"));
	}
	return Z_Registration_Info_UScriptStruct_BXTEvent.OuterSingleton;
}
template<> BATTLEX_API UScriptStruct* StaticStruct<FBXTEvent>()
{
	return FBXTEvent::StaticStruct();
}
struct Z_Construct_UScriptStruct_FBXTEvent_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe8\xa7\xa6\xe5\x8f\x91\xe4\xba\x8b\xe4\xbb\xb6\n" },
#endif
		{ "ModuleRelativePath", "Task/BXTStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe8\xa7\xa6\xe5\x8f\x91\xe4\xba\x8b\xe4\xbb\xb6" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bMulticast_MetaData[] = {
		{ "Category", "BXTEvent" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe9\x9c\x80\xe8\xa6\x81\xe8\xbf\x9b\xe8\xa1\x8c\xe7\xbd\x91\xe7\xbb\x9c\xe5\xb9\xbf\xe6\x92\xad\n" },
#endif
		{ "ModuleRelativePath", "Task/BXTStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe9\x9c\x80\xe8\xa6\x81\xe8\xbf\x9b\xe8\xa1\x8c\xe7\xbd\x91\xe7\xbb\x9c\xe5\xb9\xbf\xe6\x92\xad" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Event_MetaData[] = {
		{ "Category", "BXTEvent" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe8\xa7\xa6\xe5\x8f\x91\xe4\xba\x8b\xe4\xbb\xb6\xef\xbc\x8c\xe5\xbb\xb6\xe8\xbf\x9f\xe8\xa7\xa6\xe5\x8f\x91\xe6\x97\xb6\xe9\x97\xb4\n" },
#endif
		{ "ModuleRelativePath", "Task/BXTStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe8\xa7\xa6\xe5\x8f\x91\xe4\xba\x8b\xe4\xbb\xb6\xef\xbc\x8c\xe5\xbb\xb6\xe8\xbf\x9f\xe8\xa7\xa6\xe5\x8f\x91\xe6\x97\xb6\xe9\x97\xb4" },
#endif
	};
#endif // WITH_METADATA
	static void NewProp_bMulticast_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bMulticast;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_Event_ValueProp;
	static const UECodeGen_Private::FSoftObjectPropertyParams NewProp_Event_Key_KeyProp;
	static const UECodeGen_Private::FMapPropertyParams NewProp_Event;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FBXTEvent>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
};
void Z_Construct_UScriptStruct_FBXTEvent_Statics::NewProp_bMulticast_SetBit(void* Obj)
{
	((FBXTEvent*)Obj)->bMulticast = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FBXTEvent_Statics::NewProp_bMulticast = { "bMulticast", nullptr, (EPropertyFlags)0x0010000000010005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FBXTEvent), &Z_Construct_UScriptStruct_FBXTEvent_Statics::NewProp_bMulticast_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bMulticast_MetaData), NewProp_bMulticast_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FBXTEvent_Statics::NewProp_Event_ValueProp = { "Event", nullptr, (EPropertyFlags)0x0000000000020001, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 1, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FSoftObjectPropertyParams Z_Construct_UScriptStruct_FBXTEvent_Statics::NewProp_Event_Key_KeyProp = { "Event_Key", nullptr, (EPropertyFlags)0x0004000000020001, UECodeGen_Private::EPropertyGenFlags::SoftObject, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UClass_UBXTask_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FMapPropertyParams Z_Construct_UScriptStruct_FBXTEvent_Statics::NewProp_Event = { "Event", nullptr, (EPropertyFlags)0x0010000000030005, UECodeGen_Private::EPropertyGenFlags::Map, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTEvent, Event), EMapPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Event_MetaData), NewProp_Event_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FBXTEvent_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTEvent_Statics::NewProp_bMulticast,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTEvent_Statics::NewProp_Event_ValueProp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTEvent_Statics::NewProp_Event_Key_KeyProp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTEvent_Statics::NewProp_Event,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXTEvent_Statics::PropPointers) < 2048);
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FBXTEvent_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_BattleX,
	nullptr,
	&NewStructOps,
	"BXTEvent",
	Z_Construct_UScriptStruct_FBXTEvent_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXTEvent_Statics::PropPointers),
	sizeof(FBXTEvent),
	alignof(FBXTEvent),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000001),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXTEvent_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FBXTEvent_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FBXTEvent()
{
	if (!Z_Registration_Info_UScriptStruct_BXTEvent.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_BXTEvent.InnerSingleton, Z_Construct_UScriptStruct_FBXTEvent_Statics::StructParams);
	}
	return Z_Registration_Info_UScriptStruct_BXTEvent.InnerSingleton;
}
// End ScriptStruct FBXTEvent

// Begin ScriptStruct FBXTInputInfo
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_BXTInputInfo;
class UScriptStruct* FBXTInputInfo::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_BXTInputInfo.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_BXTInputInfo.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FBXTInputInfo, (UObject*)Z_Construct_UPackage__Script_BattleX(), TEXT("BXTInputInfo"));
	}
	return Z_Registration_Info_UScriptStruct_BXTInputInfo.OuterSingleton;
}
template<> BATTLEX_API UScriptStruct* StaticStruct<FBXTInputInfo>()
{
	return FBXTInputInfo::StaticStruct();
}
struct Z_Construct_UScriptStruct_FBXTInputInfo_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe8\xaf\xbb\xe5\x8f\x96\xe6\x95\xb0\xe6\x8d\xae\xe7\x9a\x84\xe4\xbf\xa1\xe6\x81\xaf\n" },
#endif
		{ "ModuleRelativePath", "Task/BXTStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe8\xaf\xbb\xe5\x8f\x96\xe6\x95\xb0\xe6\x8d\xae\xe7\x9a\x84\xe4\xbf\xa1\xe6\x81\xaf" },
#endif
	};
#if WITH_EDITORONLY_DATA
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_DisplayName_MetaData[] = {
		{ "Category", "BXTInputInfo" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe8\xbe\x93\xe5\x85\xa5\xe7\x9a\x84\xe9\x92\x88\xe8\x84\x9a\xe5\x90\x8d\xe7\xa7\xb0\n" },
#endif
		{ "ModuleRelativePath", "Task/BXTStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe8\xbe\x93\xe5\x85\xa5\xe7\x9a\x84\xe9\x92\x88\xe8\x84\x9a\xe5\x90\x8d\xe7\xa7\xb0" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_DataType_MetaData[] = {
		{ "Category", "BXTInputInfo" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe8\xbe\x93\xe5\x85\xa5\xe7\x9a\x84\xe6\x95\xb0\xe6\x8d\xae\xe7\xb1\xbb\xe5\x9e\x8b\n" },
#endif
		{ "ModuleRelativePath", "Task/BXTStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe8\xbe\x93\xe5\x85\xa5\xe7\x9a\x84\xe6\x95\xb0\xe6\x8d\xae\xe7\xb1\xbb\xe5\x9e\x8b" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_StructType_MetaData[] = {
		{ "Category", "BXTInputInfo" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe8\xbe\x93\xe5\x85\xa5\xe7\x9a\x84\xe7\xbb\x93\xe6\x9e\x84\xe4\xbd\x93\xe7\xb1\xbb\xe5\x9e\x8b\n" },
#endif
		{ "EditCondition", "DataType == EBXDataType::DT_Struct" },
		{ "ModuleRelativePath", "Task/BXTStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe8\xbe\x93\xe5\x85\xa5\xe7\x9a\x84\xe7\xbb\x93\xe6\x9e\x84\xe4\xbd\x93\xe7\xb1\xbb\xe5\x9e\x8b" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_UniqueID_MetaData[] = {
		{ "ModuleRelativePath", "Task/BXTStructs.h" },
	};
#endif // WITH_EDITORONLY_DATA
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_DataTask_MetaData[] = {
		{ "Category", "BXTInputInfo" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe8\x8e\xb7\xe5\x8f\x96\xe5\x93\xaa\xe4\xb8\xaa\xe4\xbb\xbb\xe5\x8a\xa1\xe7\x94\x9f\xe4\xba\xa7\xe7\x9a\x84\xe6\x95\xb0\xe6\x8d\xae\n" },
#endif
		{ "ModuleRelativePath", "Task/BXTStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe8\x8e\xb7\xe5\x8f\x96\xe5\x93\xaa\xe4\xb8\xaa\xe4\xbb\xbb\xe5\x8a\xa1\xe7\x94\x9f\xe4\xba\xa7\xe7\x9a\x84\xe6\x95\xb0\xe6\x8d\xae" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_DataDesc_MetaData[] = {
		{ "Category", "BXTInputInfo" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe8\x8e\xb7\xe5\x8f\x96\xe5\x90\x8d\xe7\xa7\xb0\xe4\xb8\xba\"xxx\"\xe7\x9a\x84\xe6\x95\xb0\xe6\x8d\xae\n" },
#endif
		{ "ModuleRelativePath", "Task/BXTStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe8\x8e\xb7\xe5\x8f\x96\xe5\x90\x8d\xe7\xa7\xb0\xe4\xb8\xba\"xxx\"\xe7\x9a\x84\xe6\x95\xb0\xe6\x8d\xae" },
#endif
	};
#endif // WITH_METADATA
#if WITH_EDITORONLY_DATA
	static const UECodeGen_Private::FNamePropertyParams NewProp_DisplayName;
	static const UECodeGen_Private::FBytePropertyParams NewProp_DataType_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_DataType;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_StructType;
	static const UECodeGen_Private::FInt64PropertyParams NewProp_UniqueID;
#endif // WITH_EDITORONLY_DATA
	static const UECodeGen_Private::FSoftObjectPropertyParams NewProp_DataTask;
	static const UECodeGen_Private::FNamePropertyParams NewProp_DataDesc;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FBXTInputInfo>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
};
#if WITH_EDITORONLY_DATA
const UECodeGen_Private::FNamePropertyParams Z_Construct_UScriptStruct_FBXTInputInfo_Statics::NewProp_DisplayName = { "DisplayName", nullptr, (EPropertyFlags)0x0010000800010001, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTInputInfo, DisplayName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_DisplayName_MetaData), NewProp_DisplayName_MetaData) };
const UECodeGen_Private::FBytePropertyParams Z_Construct_UScriptStruct_FBXTInputInfo_Statics::NewProp_DataType_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UScriptStruct_FBXTInputInfo_Statics::NewProp_DataType = { "DataType", nullptr, (EPropertyFlags)0x0010000800010001, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTInputInfo, DataType), Z_Construct_UEnum_BattleX_EBXDataType, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_DataType_MetaData), NewProp_DataType_MetaData) }; // 3648073204
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UScriptStruct_FBXTInputInfo_Statics::NewProp_StructType = { "StructType", nullptr, (EPropertyFlags)0x0010000800010001, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTInputInfo, StructType), Z_Construct_UClass_UScriptStruct, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_StructType_MetaData), NewProp_StructType_MetaData) };
const UECodeGen_Private::FInt64PropertyParams Z_Construct_UScriptStruct_FBXTInputInfo_Statics::NewProp_UniqueID = { "UniqueID", nullptr, (EPropertyFlags)0x0040000800000000, UECodeGen_Private::EPropertyGenFlags::Int64, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTInputInfo, UniqueID), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_UniqueID_MetaData), NewProp_UniqueID_MetaData) };
#endif // WITH_EDITORONLY_DATA
const UECodeGen_Private::FSoftObjectPropertyParams Z_Construct_UScriptStruct_FBXTInputInfo_Statics::NewProp_DataTask = { "DataTask", nullptr, (EPropertyFlags)0x0014000000030015, UECodeGen_Private::EPropertyGenFlags::SoftObject, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTInputInfo, DataTask), Z_Construct_UClass_UBXTask_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_DataTask_MetaData), NewProp_DataTask_MetaData) };
const UECodeGen_Private::FNamePropertyParams Z_Construct_UScriptStruct_FBXTInputInfo_Statics::NewProp_DataDesc = { "DataDesc", nullptr, (EPropertyFlags)0x0010000000030015, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTInputInfo, DataDesc), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_DataDesc_MetaData), NewProp_DataDesc_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FBXTInputInfo_Statics::PropPointers[] = {
#if WITH_EDITORONLY_DATA
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTInputInfo_Statics::NewProp_DisplayName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTInputInfo_Statics::NewProp_DataType_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTInputInfo_Statics::NewProp_DataType,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTInputInfo_Statics::NewProp_StructType,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTInputInfo_Statics::NewProp_UniqueID,
#endif // WITH_EDITORONLY_DATA
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTInputInfo_Statics::NewProp_DataTask,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTInputInfo_Statics::NewProp_DataDesc,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXTInputInfo_Statics::PropPointers) < 2048);
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FBXTInputInfo_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_BattleX,
	nullptr,
	&NewStructOps,
	"BXTInputInfo",
	Z_Construct_UScriptStruct_FBXTInputInfo_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXTInputInfo_Statics::PropPointers),
	sizeof(FBXTInputInfo),
	alignof(FBXTInputInfo),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000201),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXTInputInfo_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FBXTInputInfo_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FBXTInputInfo()
{
	if (!Z_Registration_Info_UScriptStruct_BXTInputInfo.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_BXTInputInfo.InnerSingleton, Z_Construct_UScriptStruct_FBXTInputInfo_Statics::StructParams);
	}
	return Z_Registration_Info_UScriptStruct_BXTInputInfo.InnerSingleton;
}
// End ScriptStruct FBXTInputInfo

// Begin ScriptStruct FBXTOutputInfo
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_BXTOutputInfo;
class UScriptStruct* FBXTOutputInfo::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_BXTOutputInfo.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_BXTOutputInfo.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FBXTOutputInfo, (UObject*)Z_Construct_UPackage__Script_BattleX(), TEXT("BXTOutputInfo"));
	}
	return Z_Registration_Info_UScriptStruct_BXTOutputInfo.OuterSingleton;
}
template<> BATTLEX_API UScriptStruct* StaticStruct<FBXTOutputInfo>()
{
	return FBXTOutputInfo::StaticStruct();
}
struct Z_Construct_UScriptStruct_FBXTOutputInfo_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe8\xbe\x93\xe5\x87\xba\xe6\x95\xb0\xe6\x8d\xae\xe7\x9a\x84\xe4\xbf\xa1\xe6\x81\xaf\n" },
#endif
		{ "ModuleRelativePath", "Task/BXTStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe8\xbe\x93\xe5\x87\xba\xe6\x95\xb0\xe6\x8d\xae\xe7\x9a\x84\xe4\xbf\xa1\xe6\x81\xaf" },
#endif
	};
#if WITH_EDITORONLY_DATA
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_UniqueID_MetaData[] = {
		{ "ModuleRelativePath", "Task/BXTStructs.h" },
	};
#endif // WITH_EDITORONLY_DATA
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_DataDesc_MetaData[] = {
		{ "Category", "BXTOutputInfo" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe8\xbe\x93\xe5\x87\xba\xe7\x9a\x84\xe6\x95\xb0\xe6\x8d\xae\xe5\x90\x8d\xe7\xa7\xb0\n" },
#endif
		{ "ModuleRelativePath", "Task/BXTStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe8\xbe\x93\xe5\x87\xba\xe7\x9a\x84\xe6\x95\xb0\xe6\x8d\xae\xe5\x90\x8d\xe7\xa7\xb0" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_DataType_MetaData[] = {
		{ "Category", "BXTOutputInfo" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe8\xbe\x93\xe5\x87\xba\xe7\x9a\x84\xe6\x95\xb0\xe6\x8d\xae\xe7\xb1\xbb\xe5\x9e\x8b\n" },
#endif
		{ "ModuleRelativePath", "Task/BXTStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe8\xbe\x93\xe5\x87\xba\xe7\x9a\x84\xe6\x95\xb0\xe6\x8d\xae\xe7\xb1\xbb\xe5\x9e\x8b" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_StructType_MetaData[] = {
		{ "Category", "BXTOutputInfo" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe8\xbe\x93\xe5\x87\xba\xe7\x9a\x84\xe7\xbb\x93\xe6\x9e\x84\xe4\xbd\x93\xe7\xb1\xbb\xe5\x9e\x8b\n" },
#endif
		{ "EditCondition", "DataType == EBXDataType::DT_Struct" },
		{ "ModuleRelativePath", "Task/BXTStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe8\xbe\x93\xe5\x87\xba\xe7\x9a\x84\xe7\xbb\x93\xe6\x9e\x84\xe4\xbd\x93\xe7\xb1\xbb\xe5\x9e\x8b" },
#endif
	};
#endif // WITH_METADATA
#if WITH_EDITORONLY_DATA
	static const UECodeGen_Private::FIntPropertyParams NewProp_UniqueID;
#endif // WITH_EDITORONLY_DATA
	static const UECodeGen_Private::FNamePropertyParams NewProp_DataDesc;
	static const UECodeGen_Private::FBytePropertyParams NewProp_DataType_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_DataType;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_StructType;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FBXTOutputInfo>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
};
#if WITH_EDITORONLY_DATA
const UECodeGen_Private::FIntPropertyParams Z_Construct_UScriptStruct_FBXTOutputInfo_Statics::NewProp_UniqueID = { "UniqueID", nullptr, (EPropertyFlags)0x0040000800000000, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTOutputInfo, UniqueID), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_UniqueID_MetaData), NewProp_UniqueID_MetaData) };
#endif // WITH_EDITORONLY_DATA
const UECodeGen_Private::FNamePropertyParams Z_Construct_UScriptStruct_FBXTOutputInfo_Statics::NewProp_DataDesc = { "DataDesc", nullptr, (EPropertyFlags)0x0010000000010015, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTOutputInfo, DataDesc), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_DataDesc_MetaData), NewProp_DataDesc_MetaData) };
const UECodeGen_Private::FBytePropertyParams Z_Construct_UScriptStruct_FBXTOutputInfo_Statics::NewProp_DataType_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UScriptStruct_FBXTOutputInfo_Statics::NewProp_DataType = { "DataType", nullptr, (EPropertyFlags)0x0010000000010015, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTOutputInfo, DataType), Z_Construct_UEnum_BattleX_EBXDataType, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_DataType_MetaData), NewProp_DataType_MetaData) }; // 3648073204
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UScriptStruct_FBXTOutputInfo_Statics::NewProp_StructType = { "StructType", nullptr, (EPropertyFlags)0x0010000000010015, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTOutputInfo, StructType), Z_Construct_UClass_UScriptStruct, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_StructType_MetaData), NewProp_StructType_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FBXTOutputInfo_Statics::PropPointers[] = {
#if WITH_EDITORONLY_DATA
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTOutputInfo_Statics::NewProp_UniqueID,
#endif // WITH_EDITORONLY_DATA
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTOutputInfo_Statics::NewProp_DataDesc,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTOutputInfo_Statics::NewProp_DataType_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTOutputInfo_Statics::NewProp_DataType,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTOutputInfo_Statics::NewProp_StructType,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXTOutputInfo_Statics::PropPointers) < 2048);
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FBXTOutputInfo_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_BattleX,
	nullptr,
	&NewStructOps,
	"BXTOutputInfo",
	Z_Construct_UScriptStruct_FBXTOutputInfo_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXTOutputInfo_Statics::PropPointers),
	sizeof(FBXTOutputInfo),
	alignof(FBXTOutputInfo),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000201),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXTOutputInfo_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FBXTOutputInfo_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FBXTOutputInfo()
{
	if (!Z_Registration_Info_UScriptStruct_BXTOutputInfo.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_BXTOutputInfo.InnerSingleton, Z_Construct_UScriptStruct_FBXTOutputInfo_Statics::StructParams);
	}
	return Z_Registration_Info_UScriptStruct_BXTOutputInfo.InnerSingleton;
}
// End ScriptStruct FBXTOutputInfo

// Begin ScriptStruct FBXTObject
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_BXTObject;
class UScriptStruct* FBXTObject::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_BXTObject.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_BXTObject.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FBXTObject, (UObject*)Z_Construct_UPackage__Script_BattleX(), TEXT("BXTObject"));
	}
	return Z_Registration_Info_UScriptStruct_BXTObject.OuterSingleton;
}
template<> BATTLEX_API UScriptStruct* StaticStruct<FBXTObject>()
{
	return FBXTObject::StaticStruct();
}
struct Z_Construct_UScriptStruct_FBXTObject_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe6\x8c\x87\xe9\x92\x88\xe5\xb0\x81\xe8\xa3\x85\n" },
#endif
		{ "ModuleRelativePath", "Task/BXTStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe6\x8c\x87\xe9\x92\x88\xe5\xb0\x81\xe8\xa3\x85" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Object_MetaData[] = {
		{ "Category", "BXTObject" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe5\xaf\xb9\xe8\xb1\xa1\xe6\x8c\x87\xe9\x92\x88\n" },
#endif
		{ "ModuleRelativePath", "Task/BXTStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe5\xaf\xb9\xe8\xb1\xa1\xe6\x8c\x87\xe9\x92\x88" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FSoftObjectPropertyParams NewProp_Object;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FBXTObject>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
};
const UECodeGen_Private::FSoftObjectPropertyParams Z_Construct_UScriptStruct_FBXTObject_Statics::NewProp_Object = { "Object", nullptr, (EPropertyFlags)0x0014000000010005, UECodeGen_Private::EPropertyGenFlags::SoftObject, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTObject, Object), Z_Construct_UClass_UObject_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Object_MetaData), NewProp_Object_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FBXTObject_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTObject_Statics::NewProp_Object,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXTObject_Statics::PropPointers) < 2048);
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FBXTObject_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_BattleX,
	nullptr,
	&NewStructOps,
	"BXTObject",
	Z_Construct_UScriptStruct_FBXTObject_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXTObject_Statics::PropPointers),
	sizeof(FBXTObject),
	alignof(FBXTObject),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000001),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXTObject_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FBXTObject_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FBXTObject()
{
	if (!Z_Registration_Info_UScriptStruct_BXTObject.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_BXTObject.InnerSingleton, Z_Construct_UScriptStruct_FBXTObject_Statics::StructParams);
	}
	return Z_Registration_Info_UScriptStruct_BXTObject.InnerSingleton;
}
// End ScriptStruct FBXTObject

// Begin ScriptStruct FBXTHitResults
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_BXTHitResults;
class UScriptStruct* FBXTHitResults::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_BXTHitResults.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_BXTHitResults.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FBXTHitResults, (UObject*)Z_Construct_UPackage__Script_BattleX(), TEXT("BXTHitResults"));
	}
	return Z_Registration_Info_UScriptStruct_BXTHitResults.OuterSingleton;
}
template<> BATTLEX_API UScriptStruct* StaticStruct<FBXTHitResults>()
{
	return FBXTHitResults::StaticStruct();
}
struct Z_Construct_UScriptStruct_FBXTHitResults_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe7\xa2\xb0\xe6\x92\x9e\xe4\xbf\xa1\xe6\x81\xaf\n" },
#endif
		{ "ModuleRelativePath", "Task/BXTStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe7\xa2\xb0\xe6\x92\x9e\xe4\xbf\xa1\xe6\x81\xaf" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Results_MetaData[] = {
		{ "Category", "BXTHitResults" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe7\xa2\xb0\xe6\x92\x9e\xe4\xbf\xa1\xe6\x81\xaf\xe5\x88\x97\xe8\xa1\xa8\n" },
#endif
		{ "ModuleRelativePath", "Task/BXTStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe7\xa2\xb0\xe6\x92\x9e\xe4\xbf\xa1\xe6\x81\xaf\xe5\x88\x97\xe8\xa1\xa8" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_Results_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_Results;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FBXTHitResults>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FBXTHitResults_Statics::NewProp_Results_Inner = { "Results", nullptr, (EPropertyFlags)0x0000008000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UScriptStruct_FHitResult, METADATA_PARAMS(0, nullptr) }; // 4100991306
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FBXTHitResults_Statics::NewProp_Results = { "Results", nullptr, (EPropertyFlags)0x0010008000010005, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTHitResults, Results), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Results_MetaData), NewProp_Results_MetaData) }; // 4100991306
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FBXTHitResults_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTHitResults_Statics::NewProp_Results_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTHitResults_Statics::NewProp_Results,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXTHitResults_Statics::PropPointers) < 2048);
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FBXTHitResults_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_BattleX,
	nullptr,
	&NewStructOps,
	"BXTHitResults",
	Z_Construct_UScriptStruct_FBXTHitResults_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXTHitResults_Statics::PropPointers),
	sizeof(FBXTHitResults),
	alignof(FBXTHitResults),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000005),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXTHitResults_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FBXTHitResults_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FBXTHitResults()
{
	if (!Z_Registration_Info_UScriptStruct_BXTHitResults.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_BXTHitResults.InnerSingleton, Z_Construct_UScriptStruct_FBXTHitResults_Statics::StructParams);
	}
	return Z_Registration_Info_UScriptStruct_BXTHitResults.InnerSingleton;
}
// End ScriptStruct FBXTHitResults

// Begin ScriptStruct FBXTTransformCreater
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_BXTTransformCreater;
class UScriptStruct* FBXTTransformCreater::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_BXTTransformCreater.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_BXTTransformCreater.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FBXTTransformCreater, (UObject*)Z_Construct_UPackage__Script_BattleX(), TEXT("BXTTransformCreater"));
	}
	return Z_Registration_Info_UScriptStruct_BXTTransformCreater.OuterSingleton;
}
template<> BATTLEX_API UScriptStruct* StaticStruct<FBXTTransformCreater>()
{
	return FBXTTransformCreater::StaticStruct();
}
struct Z_Construct_UScriptStruct_FBXTTransformCreater_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe5\x9d\x90\xe6\xa0\x87\xe7\xb3\xbb\xe5\x88\x9b\xe5\xbb\xba\xe5\x99\xa8\n" },
#endif
		{ "ModuleRelativePath", "Task/BXTStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe5\x9d\x90\xe6\xa0\x87\xe7\xb3\xbb\xe5\x88\x9b\xe5\xbb\xba\xe5\x99\xa8" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_OriginType_MetaData[] = {
		{ "Category", "BXTTransformCreater" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe5\x8e\x9f\xe7\x82\xb9\xe9\x80\x89\xe5\x8f\x96\xe8\xa7\x84\xe5\x88\x99\n" },
#endif
		{ "ModuleRelativePath", "Task/BXTStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe5\x8e\x9f\xe7\x82\xb9\xe9\x80\x89\xe5\x8f\x96\xe8\xa7\x84\xe5\x88\x99" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_OriginInput_MetaData[] = {
		{ "Category", "BXTTransformCreater" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe7\x89\xb9\xe5\xae\x9a\xe5\x8e\x9f\xe7\x82\xb9\xe8\xbe\x93\xe5\x85\xa5\xe6\x95\xb0\xe6\x8d\xae\n" },
#endif
		{ "EditCondition", "OriginType == EBXTCoordinateType::C_Special" },
		{ "EditConditionHides", "" },
		{ "ModuleRelativePath", "Task/BXTStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe7\x89\xb9\xe5\xae\x9a\xe5\x8e\x9f\xe7\x82\xb9\xe8\xbe\x93\xe5\x85\xa5\xe6\x95\xb0\xe6\x8d\xae" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_OriginBoneName_MetaData[] = {
		{ "Category", "BXTTransformCreater" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe5\x8e\x9f\xe7\x82\xb9\xe9\x80\x89\xe5\x8f\x96\xe7\x9a\x84\xe9\xaa\xa8\xe9\xaa\xbc\xe5\x90\x8d\xe7\xa7\xb0\n" },
#endif
		{ "ModuleRelativePath", "Task/BXTStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe5\x8e\x9f\xe7\x82\xb9\xe9\x80\x89\xe5\x8f\x96\xe7\x9a\x84\xe9\xaa\xa8\xe9\xaa\xbc\xe5\x90\x8d\xe7\xa7\xb0" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_OriginDelta_MetaData[] = {
		{ "Category", "BXTTransformCreater" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe5\x8e\x9f\xe7\x82\xb9\xe5\x81\x8f\xe7\xa7\xbb\n" },
#endif
		{ "ModuleRelativePath", "Task/BXTStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe5\x8e\x9f\xe7\x82\xb9\xe5\x81\x8f\xe7\xa7\xbb" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_XAxisType_MetaData[] = {
		{ "Category", "BXTTransformCreater" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// X\xe8\xbd\xb4\xe9\x80\x89\xe5\x8f\x96\xe8\xa7\x84\xe5\x88\x99(\xe4\xb8\x8d\xe5\xa1\xab\xef\xbc\x8c\xe5\x88\x99\xe9\xbb\x98\xe8\xae\xa4\xe4\xbd\xbf\xe7\x94\xa8\xe5\x8e\x9f\xe7\x82\xb9\xe7\x9a\x84\xe5\x9d\x90\xe6\xa0\x87\xe7\xb3\xbb\xe8\xbd\xb4\xe5\x90\x91)\n" },
#endif
		{ "ModuleRelativePath", "Task/BXTStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "X\xe8\xbd\xb4\xe9\x80\x89\xe5\x8f\x96\xe8\xa7\x84\xe5\x88\x99(\xe4\xb8\x8d\xe5\xa1\xab\xef\xbc\x8c\xe5\x88\x99\xe9\xbb\x98\xe8\xae\xa4\xe4\xbd\xbf\xe7\x94\xa8\xe5\x8e\x9f\xe7\x82\xb9\xe7\x9a\x84\xe5\x9d\x90\xe6\xa0\x87\xe7\xb3\xbb\xe8\xbd\xb4\xe5\x90\x91)" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_XAxisInput_MetaData[] = {
		{ "Category", "BXTTransformCreater" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe7\x89\xb9\xe5\xae\x9aX\xe8\xbd\xb4\xe8\xbe\x93\xe5\x85\xa5\xe6\x95\xb0\xe6\x8d\xaeID\n" },
#endif
		{ "EditCondition", "XAxisType == EBXTCoordinateType::C_Special" },
		{ "EditConditionHides", "" },
		{ "ModuleRelativePath", "Task/BXTStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe7\x89\xb9\xe5\xae\x9aX\xe8\xbd\xb4\xe8\xbe\x93\xe5\x85\xa5\xe6\x95\xb0\xe6\x8d\xaeID" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_XAxisBoneName_MetaData[] = {
		{ "Category", "BXTTransformCreater" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// X\xe8\xbd\xb4\xe9\x80\x89\xe5\x8f\x96\xe7\x9a\x84\xe9\xaa\xa8\xe9\xaa\xbc\xe5\x90\x8d\xe7\xa7\xb0\n" },
#endif
		{ "EditCondition", "XAxisType != EBXTCoordinateType::C_TMax" },
		{ "EditConditionHides", "" },
		{ "ModuleRelativePath", "Task/BXTStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "X\xe8\xbd\xb4\xe9\x80\x89\xe5\x8f\x96\xe7\x9a\x84\xe9\xaa\xa8\xe9\xaa\xbc\xe5\x90\x8d\xe7\xa7\xb0" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ConnectionType_MetaData[] = {
		{ "Category", "BXTTransformCreater" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe4\xbd\xbf\xe7\x94\xa8\xe8\xbf\x9e\xe7\xba\xbf\xe4\xbd\x9c\xe4\xb8\xbaX\xe8\xbd\xb4\xe7\x9a\x84\xe8\xbd\xb4\xe5\x90\x91\n" },
#endif
		{ "EditCondition", "XAxisType != EBXTCoordinateType::C_TMax" },
		{ "EditConditionHides", "" },
		{ "ModuleRelativePath", "Task/BXTStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe4\xbd\xbf\xe7\x94\xa8\xe8\xbf\x9e\xe7\xba\xbf\xe4\xbd\x9c\xe4\xb8\xbaX\xe8\xbd\xb4\xe7\x9a\x84\xe8\xbd\xb4\xe5\x90\x91" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_PostOffset_MetaData[] = {
		{ "Category", "BXTTransformCreater" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe5\x88\x9b\xe5\xbb\xba\xe5\xae\x8c\xe5\x9d\x90\xe6\xa0\x87\xe7\xb3\xbb\xe5\x90\x8e\xe7\x9a\x84\xe6\x80\xbb\xe4\xbd\x93\xe5\x81\x8f\xe7\xa7\xbb\n" },
#endif
		{ "ModuleRelativePath", "Task/BXTStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe5\x88\x9b\xe5\xbb\xba\xe5\xae\x8c\xe5\x9d\x90\xe6\xa0\x87\xe7\xb3\xbb\xe5\x90\x8e\xe7\x9a\x84\xe6\x80\xbb\xe4\xbd\x93\xe5\x81\x8f\xe7\xa7\xbb" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FBytePropertyParams NewProp_OriginType_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_OriginType;
	static const UECodeGen_Private::FStructPropertyParams NewProp_OriginInput;
	static const UECodeGen_Private::FStructPropertyParams NewProp_OriginBoneName;
	static const UECodeGen_Private::FStructPropertyParams NewProp_OriginDelta;
	static const UECodeGen_Private::FBytePropertyParams NewProp_XAxisType_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_XAxisType;
	static const UECodeGen_Private::FStructPropertyParams NewProp_XAxisInput;
	static const UECodeGen_Private::FStructPropertyParams NewProp_XAxisBoneName;
	static const UECodeGen_Private::FBytePropertyParams NewProp_ConnectionType_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_ConnectionType;
	static const UECodeGen_Private::FStructPropertyParams NewProp_PostOffset;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FBXTTransformCreater>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
};
const UECodeGen_Private::FBytePropertyParams Z_Construct_UScriptStruct_FBXTTransformCreater_Statics::NewProp_OriginType_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UScriptStruct_FBXTTransformCreater_Statics::NewProp_OriginType = { "OriginType", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTTransformCreater, OriginType), Z_Construct_UEnum_BattleX_EBXTCoordinateType, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_OriginType_MetaData), NewProp_OriginType_MetaData) }; // 2214625094
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FBXTTransformCreater_Statics::NewProp_OriginInput = { "OriginInput", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTTransformCreater, OriginInput), Z_Construct_UScriptStruct_FBXTInputInfo, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_OriginInput_MetaData), NewProp_OriginInput_MetaData) }; // 579686498
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FBXTTransformCreater_Statics::NewProp_OriginBoneName = { "OriginBoneName", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTTransformCreater, OriginBoneName), Z_Construct_UScriptStruct_FBXBoneSelector, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_OriginBoneName_MetaData), NewProp_OriginBoneName_MetaData) }; // 1217301433
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FBXTTransformCreater_Statics::NewProp_OriginDelta = { "OriginDelta", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTTransformCreater, OriginDelta), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_OriginDelta_MetaData), NewProp_OriginDelta_MetaData) };
const UECodeGen_Private::FBytePropertyParams Z_Construct_UScriptStruct_FBXTTransformCreater_Statics::NewProp_XAxisType_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UScriptStruct_FBXTTransformCreater_Statics::NewProp_XAxisType = { "XAxisType", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTTransformCreater, XAxisType), Z_Construct_UEnum_BattleX_EBXTCoordinateType, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_XAxisType_MetaData), NewProp_XAxisType_MetaData) }; // 2214625094
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FBXTTransformCreater_Statics::NewProp_XAxisInput = { "XAxisInput", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTTransformCreater, XAxisInput), Z_Construct_UScriptStruct_FBXTInputInfo, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_XAxisInput_MetaData), NewProp_XAxisInput_MetaData) }; // 579686498
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FBXTTransformCreater_Statics::NewProp_XAxisBoneName = { "XAxisBoneName", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTTransformCreater, XAxisBoneName), Z_Construct_UScriptStruct_FBXBoneSelector, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_XAxisBoneName_MetaData), NewProp_XAxisBoneName_MetaData) }; // 1217301433
const UECodeGen_Private::FBytePropertyParams Z_Construct_UScriptStruct_FBXTTransformCreater_Statics::NewProp_ConnectionType_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UScriptStruct_FBXTTransformCreater_Statics::NewProp_ConnectionType = { "ConnectionType", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTTransformCreater, ConnectionType), Z_Construct_UEnum_BattleX_EBXTConnectionType, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ConnectionType_MetaData), NewProp_ConnectionType_MetaData) }; // 2224598816
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FBXTTransformCreater_Statics::NewProp_PostOffset = { "PostOffset", nullptr, (EPropertyFlags)0x0010000000010005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTTransformCreater, PostOffset), Z_Construct_UScriptStruct_FTransform, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_PostOffset_MetaData), NewProp_PostOffset_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FBXTTransformCreater_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTTransformCreater_Statics::NewProp_OriginType_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTTransformCreater_Statics::NewProp_OriginType,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTTransformCreater_Statics::NewProp_OriginInput,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTTransformCreater_Statics::NewProp_OriginBoneName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTTransformCreater_Statics::NewProp_OriginDelta,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTTransformCreater_Statics::NewProp_XAxisType_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTTransformCreater_Statics::NewProp_XAxisType,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTTransformCreater_Statics::NewProp_XAxisInput,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTTransformCreater_Statics::NewProp_XAxisBoneName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTTransformCreater_Statics::NewProp_ConnectionType_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTTransformCreater_Statics::NewProp_ConnectionType,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTTransformCreater_Statics::NewProp_PostOffset,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXTTransformCreater_Statics::PropPointers) < 2048);
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FBXTTransformCreater_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_BattleX,
	nullptr,
	&NewStructOps,
	"BXTTransformCreater",
	Z_Construct_UScriptStruct_FBXTTransformCreater_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXTTransformCreater_Statics::PropPointers),
	sizeof(FBXTTransformCreater),
	alignof(FBXTTransformCreater),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000201),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXTTransformCreater_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FBXTTransformCreater_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FBXTTransformCreater()
{
	if (!Z_Registration_Info_UScriptStruct_BXTTransformCreater.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_BXTTransformCreater.InnerSingleton, Z_Construct_UScriptStruct_FBXTTransformCreater_Statics::StructParams);
	}
	return Z_Registration_Info_UScriptStruct_BXTTransformCreater.InnerSingleton;
}
// End ScriptStruct FBXTTransformCreater

// Begin Registration
struct Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Task_BXTStructs_h_Statics
{
	static constexpr FStructRegisterCompiledInInfo ScriptStructInfo[] = {
		{ FBXTEvent::StaticStruct, Z_Construct_UScriptStruct_FBXTEvent_Statics::NewStructOps, TEXT("BXTEvent"), &Z_Registration_Info_UScriptStruct_BXTEvent, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FBXTEvent), 2135766493U) },
		{ FBXTInputInfo::StaticStruct, Z_Construct_UScriptStruct_FBXTInputInfo_Statics::NewStructOps, TEXT("BXTInputInfo"), &Z_Registration_Info_UScriptStruct_BXTInputInfo, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FBXTInputInfo), 579686498U) },
		{ FBXTOutputInfo::StaticStruct, Z_Construct_UScriptStruct_FBXTOutputInfo_Statics::NewStructOps, TEXT("BXTOutputInfo"), &Z_Registration_Info_UScriptStruct_BXTOutputInfo, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FBXTOutputInfo), 513134278U) },
		{ FBXTObject::StaticStruct, Z_Construct_UScriptStruct_FBXTObject_Statics::NewStructOps, TEXT("BXTObject"), &Z_Registration_Info_UScriptStruct_BXTObject, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FBXTObject), 3922119643U) },
		{ FBXTHitResults::StaticStruct, Z_Construct_UScriptStruct_FBXTHitResults_Statics::NewStructOps, TEXT("BXTHitResults"), &Z_Registration_Info_UScriptStruct_BXTHitResults, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FBXTHitResults), 2666346688U) },
		{ FBXTTransformCreater::StaticStruct, Z_Construct_UScriptStruct_FBXTTransformCreater_Statics::NewStructOps, TEXT("BXTTransformCreater"), &Z_Registration_Info_UScriptStruct_BXTTransformCreater, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FBXTTransformCreater), 3395443164U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Task_BXTStructs_h_3928660016(TEXT("/Script/BattleX"),
	nullptr, 0,
	Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Task_BXTStructs_h_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Task_BXTStructs_h_Statics::ScriptStructInfo),
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
