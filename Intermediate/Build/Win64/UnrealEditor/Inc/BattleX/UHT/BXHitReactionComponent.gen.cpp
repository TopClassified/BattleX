// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "BattleX/Collision/BXHitReactionComponent.h"
#include "BattleX/BXStructs.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeBXHitReactionComponent() {}

// Begin Cross Module References
BATTLEX_API UClass* Z_Construct_UClass_UBXHitReactionComponent();
BATTLEX_API UClass* Z_Construct_UClass_UBXHitReactionComponent_NoRegister();
BATTLEX_API UEnum* Z_Construct_UEnum_BattleX_EBXBodyPartType();
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXBodyPartConfig();
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXBodyPartRTInformation();
ENGINE_API UClass* Z_Construct_UClass_UActorComponent();
ENGINE_API UClass* Z_Construct_UClass_USceneComponent_NoRegister();
UPackage* Z_Construct_UPackage__Script_BattleX();
// End Cross Module References

// Begin Class UBXHitReactionComponent Function GetBodyPartByComponentAndBone
struct Z_Construct_UFunction_UBXHitReactionComponent_GetBodyPartByComponentAndBone_Statics
{
	struct BXHitReactionComponent_eventGetBodyPartByComponentAndBone_Parms
	{
		USceneComponent* InComponent;
		FName InBoneName;
		FBXBodyPartRTInformation OutInformation;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xcd\xb9\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xc6\xa3\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xc8\xa1\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xe5\xb2\xbf\xce\xbb\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Collision/BXHitReactionComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xcd\xb9\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xc6\xa3\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xc8\xa1\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xe5\xb2\xbf\xce\xbb\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InComponent_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_InComponent;
	static const UECodeGen_Private::FNamePropertyParams NewProp_InBoneName;
	static const UECodeGen_Private::FStructPropertyParams NewProp_OutInformation;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UBXHitReactionComponent_GetBodyPartByComponentAndBone_Statics::NewProp_InComponent = { "InComponent", nullptr, (EPropertyFlags)0x0010000000080080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXHitReactionComponent_eventGetBodyPartByComponentAndBone_Parms, InComponent), Z_Construct_UClass_USceneComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InComponent_MetaData), NewProp_InComponent_MetaData) };
const UECodeGen_Private::FNamePropertyParams Z_Construct_UFunction_UBXHitReactionComponent_GetBodyPartByComponentAndBone_Statics::NewProp_InBoneName = { "InBoneName", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXHitReactionComponent_eventGetBodyPartByComponentAndBone_Parms, InBoneName), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXHitReactionComponent_GetBodyPartByComponentAndBone_Statics::NewProp_OutInformation = { "OutInformation", nullptr, (EPropertyFlags)0x0010008000000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXHitReactionComponent_eventGetBodyPartByComponentAndBone_Parms, OutInformation), Z_Construct_UScriptStruct_FBXBodyPartRTInformation, METADATA_PARAMS(0, nullptr) }; // 80757500
void Z_Construct_UFunction_UBXHitReactionComponent_GetBodyPartByComponentAndBone_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((BXHitReactionComponent_eventGetBodyPartByComponentAndBone_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UBXHitReactionComponent_GetBodyPartByComponentAndBone_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(BXHitReactionComponent_eventGetBodyPartByComponentAndBone_Parms), &Z_Construct_UFunction_UBXHitReactionComponent_GetBodyPartByComponentAndBone_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UBXHitReactionComponent_GetBodyPartByComponentAndBone_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXHitReactionComponent_GetBodyPartByComponentAndBone_Statics::NewProp_InComponent,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXHitReactionComponent_GetBodyPartByComponentAndBone_Statics::NewProp_InBoneName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXHitReactionComponent_GetBodyPartByComponentAndBone_Statics::NewProp_OutInformation,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXHitReactionComponent_GetBodyPartByComponentAndBone_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXHitReactionComponent_GetBodyPartByComponentAndBone_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UBXHitReactionComponent_GetBodyPartByComponentAndBone_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UBXHitReactionComponent, nullptr, "GetBodyPartByComponentAndBone", nullptr, nullptr, Z_Construct_UFunction_UBXHitReactionComponent_GetBodyPartByComponentAndBone_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UBXHitReactionComponent_GetBodyPartByComponentAndBone_Statics::PropPointers), sizeof(Z_Construct_UFunction_UBXHitReactionComponent_GetBodyPartByComponentAndBone_Statics::BXHitReactionComponent_eventGetBodyPartByComponentAndBone_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04420401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXHitReactionComponent_GetBodyPartByComponentAndBone_Statics::Function_MetaDataParams), Z_Construct_UFunction_UBXHitReactionComponent_GetBodyPartByComponentAndBone_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UBXHitReactionComponent_GetBodyPartByComponentAndBone_Statics::BXHitReactionComponent_eventGetBodyPartByComponentAndBone_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UBXHitReactionComponent_GetBodyPartByComponentAndBone()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UBXHitReactionComponent_GetBodyPartByComponentAndBone_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UBXHitReactionComponent::execGetBodyPartByComponentAndBone)
{
	P_GET_OBJECT(USceneComponent,Z_Param_InComponent);
	P_GET_PROPERTY(FNameProperty,Z_Param_InBoneName);
	P_GET_STRUCT_REF(FBXBodyPartRTInformation,Z_Param_Out_OutInformation);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->GetBodyPartByComponentAndBone(Z_Param_InComponent,Z_Param_InBoneName,Z_Param_Out_OutInformation);
	P_NATIVE_END;
}
// End Class UBXHitReactionComponent Function GetBodyPartByComponentAndBone

// Begin Class UBXHitReactionComponent Function GetBodyPartByType
struct Z_Construct_UFunction_UBXHitReactionComponent_GetBodyPartByType_Statics
{
	struct BXHitReactionComponent_eventGetBodyPartByType_Parms
	{
		EBXBodyPartType InType;
		FBXBodyPartRTInformation OutInformation;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xe5\xb2\xbf\xce\xbb\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xcd\xa3\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xc8\xa1\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xe5\xb2\xbf\xce\xbb\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Collision/BXHitReactionComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xe5\xb2\xbf\xce\xbb\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xcd\xa3\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xc8\xa1\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xe5\xb2\xbf\xce\xbb\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FBytePropertyParams NewProp_InType_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_InType;
	static const UECodeGen_Private::FStructPropertyParams NewProp_OutInformation;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UBXHitReactionComponent_GetBodyPartByType_Statics::NewProp_InType_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UBXHitReactionComponent_GetBodyPartByType_Statics::NewProp_InType = { "InType", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXHitReactionComponent_eventGetBodyPartByType_Parms, InType), Z_Construct_UEnum_BattleX_EBXBodyPartType, METADATA_PARAMS(0, nullptr) }; // 84625349
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXHitReactionComponent_GetBodyPartByType_Statics::NewProp_OutInformation = { "OutInformation", nullptr, (EPropertyFlags)0x0010008000000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXHitReactionComponent_eventGetBodyPartByType_Parms, OutInformation), Z_Construct_UScriptStruct_FBXBodyPartRTInformation, METADATA_PARAMS(0, nullptr) }; // 80757500
void Z_Construct_UFunction_UBXHitReactionComponent_GetBodyPartByType_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((BXHitReactionComponent_eventGetBodyPartByType_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UBXHitReactionComponent_GetBodyPartByType_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(BXHitReactionComponent_eventGetBodyPartByType_Parms), &Z_Construct_UFunction_UBXHitReactionComponent_GetBodyPartByType_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UBXHitReactionComponent_GetBodyPartByType_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXHitReactionComponent_GetBodyPartByType_Statics::NewProp_InType_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXHitReactionComponent_GetBodyPartByType_Statics::NewProp_InType,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXHitReactionComponent_GetBodyPartByType_Statics::NewProp_OutInformation,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXHitReactionComponent_GetBodyPartByType_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXHitReactionComponent_GetBodyPartByType_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UBXHitReactionComponent_GetBodyPartByType_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UBXHitReactionComponent, nullptr, "GetBodyPartByType", nullptr, nullptr, Z_Construct_UFunction_UBXHitReactionComponent_GetBodyPartByType_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UBXHitReactionComponent_GetBodyPartByType_Statics::PropPointers), sizeof(Z_Construct_UFunction_UBXHitReactionComponent_GetBodyPartByType_Statics::BXHitReactionComponent_eventGetBodyPartByType_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04420401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXHitReactionComponent_GetBodyPartByType_Statics::Function_MetaDataParams), Z_Construct_UFunction_UBXHitReactionComponent_GetBodyPartByType_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UBXHitReactionComponent_GetBodyPartByType_Statics::BXHitReactionComponent_eventGetBodyPartByType_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UBXHitReactionComponent_GetBodyPartByType()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UBXHitReactionComponent_GetBodyPartByType_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UBXHitReactionComponent::execGetBodyPartByType)
{
	P_GET_ENUM(EBXBodyPartType,Z_Param_InType);
	P_GET_STRUCT_REF(FBXBodyPartRTInformation,Z_Param_Out_OutInformation);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->GetBodyPartByType(EBXBodyPartType(Z_Param_InType),Z_Param_Out_OutInformation);
	P_NATIVE_END;
}
// End Class UBXHitReactionComponent Function GetBodyPartByType

// Begin Class UBXHitReactionComponent
void UBXHitReactionComponent::StaticRegisterNativesUBXHitReactionComponent()
{
	UClass* Class = UBXHitReactionComponent::StaticClass();
	static const FNameNativePtrPair Funcs[] = {
		{ "GetBodyPartByComponentAndBone", &UBXHitReactionComponent::execGetBodyPartByComponentAndBone },
		{ "GetBodyPartByType", &UBXHitReactionComponent::execGetBodyPartByType },
	};
	FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UBXHitReactionComponent);
UClass* Z_Construct_UClass_UBXHitReactionComponent_NoRegister()
{
	return UBXHitReactionComponent::StaticClass();
}
struct Z_Construct_UClass_UBXHitReactionComponent_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "BlueprintSpawnableComponent", "" },
		{ "ClassGroupNames", "Custom" },
		{ "IncludePath", "Collision/BXHitReactionComponent.h" },
		{ "ModuleRelativePath", "Collision/BXHitReactionComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_BodyPartConfigs_MetaData[] = {
		{ "Category", "BodyPart" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xe5\xb2\xbf\xce\xbb\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xcf\xa2\n" },
#endif
		{ "ModuleRelativePath", "Collision/BXHitReactionComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xe5\xb2\xbf\xce\xbb\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xcf\xa2" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_BodyPartRTDatas_MetaData[] = {
		{ "Category", "BodyPart" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xe5\xb2\xbf\xce\xbb\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xcf\xa2\n" },
#endif
		{ "ModuleRelativePath", "Collision/BXHitReactionComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xe5\xb2\xbf\xce\xbb\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xcf\xa2" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_BodyPartConfigs_ValueProp;
	static const UECodeGen_Private::FBytePropertyParams NewProp_BodyPartConfigs_Key_KeyProp_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_BodyPartConfigs_Key_KeyProp;
	static const UECodeGen_Private::FMapPropertyParams NewProp_BodyPartConfigs;
	static const UECodeGen_Private::FStructPropertyParams NewProp_BodyPartRTDatas_ValueProp;
	static const UECodeGen_Private::FBytePropertyParams NewProp_BodyPartRTDatas_Key_KeyProp_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_BodyPartRTDatas_Key_KeyProp;
	static const UECodeGen_Private::FMapPropertyParams NewProp_BodyPartRTDatas;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UBXHitReactionComponent_GetBodyPartByComponentAndBone, "GetBodyPartByComponentAndBone" }, // 1010865507
		{ &Z_Construct_UFunction_UBXHitReactionComponent_GetBodyPartByType, "GetBodyPartByType" }, // 3669313552
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UBXHitReactionComponent>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UBXHitReactionComponent_Statics::NewProp_BodyPartConfigs_ValueProp = { "BodyPartConfigs", nullptr, (EPropertyFlags)0x0000000000000001, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 1, Z_Construct_UScriptStruct_FBXBodyPartConfig, METADATA_PARAMS(0, nullptr) }; // 3005846379
const UECodeGen_Private::FBytePropertyParams Z_Construct_UClass_UBXHitReactionComponent_Statics::NewProp_BodyPartConfigs_Key_KeyProp_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UClass_UBXHitReactionComponent_Statics::NewProp_BodyPartConfigs_Key_KeyProp = { "BodyPartConfigs_Key", nullptr, (EPropertyFlags)0x0000000000000001, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UEnum_BattleX_EBXBodyPartType, METADATA_PARAMS(0, nullptr) }; // 84625349
const UECodeGen_Private::FMapPropertyParams Z_Construct_UClass_UBXHitReactionComponent_Statics::NewProp_BodyPartConfigs = { "BodyPartConfigs", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Map, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXHitReactionComponent, BodyPartConfigs), EMapPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_BodyPartConfigs_MetaData), NewProp_BodyPartConfigs_MetaData) }; // 84625349 3005846379
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UBXHitReactionComponent_Statics::NewProp_BodyPartRTDatas_ValueProp = { "BodyPartRTDatas", nullptr, (EPropertyFlags)0x0000008000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 1, Z_Construct_UScriptStruct_FBXBodyPartRTInformation, METADATA_PARAMS(0, nullptr) }; // 80757500
const UECodeGen_Private::FBytePropertyParams Z_Construct_UClass_UBXHitReactionComponent_Statics::NewProp_BodyPartRTDatas_Key_KeyProp_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UClass_UBXHitReactionComponent_Statics::NewProp_BodyPartRTDatas_Key_KeyProp = { "BodyPartRTDatas_Key", nullptr, (EPropertyFlags)0x0000008000000000, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UEnum_BattleX_EBXBodyPartType, METADATA_PARAMS(0, nullptr) }; // 84625349
const UECodeGen_Private::FMapPropertyParams Z_Construct_UClass_UBXHitReactionComponent_Statics::NewProp_BodyPartRTDatas = { "BodyPartRTDatas", nullptr, (EPropertyFlags)0x0020088000002004, UECodeGen_Private::EPropertyGenFlags::Map, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXHitReactionComponent, BodyPartRTDatas), EMapPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_BodyPartRTDatas_MetaData), NewProp_BodyPartRTDatas_MetaData) }; // 84625349 80757500
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UBXHitReactionComponent_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXHitReactionComponent_Statics::NewProp_BodyPartConfigs_ValueProp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXHitReactionComponent_Statics::NewProp_BodyPartConfigs_Key_KeyProp_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXHitReactionComponent_Statics::NewProp_BodyPartConfigs_Key_KeyProp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXHitReactionComponent_Statics::NewProp_BodyPartConfigs,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXHitReactionComponent_Statics::NewProp_BodyPartRTDatas_ValueProp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXHitReactionComponent_Statics::NewProp_BodyPartRTDatas_Key_KeyProp_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXHitReactionComponent_Statics::NewProp_BodyPartRTDatas_Key_KeyProp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXHitReactionComponent_Statics::NewProp_BodyPartRTDatas,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UBXHitReactionComponent_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_UBXHitReactionComponent_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UActorComponent,
	(UObject* (*)())Z_Construct_UPackage__Script_BattleX,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UBXHitReactionComponent_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UBXHitReactionComponent_Statics::ClassParams = {
	&UBXHitReactionComponent::StaticClass,
	"Engine",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	Z_Construct_UClass_UBXHitReactionComponent_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	UE_ARRAY_COUNT(Z_Construct_UClass_UBXHitReactionComponent_Statics::PropPointers),
	0,
	0x00B000A4u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UBXHitReactionComponent_Statics::Class_MetaDataParams), Z_Construct_UClass_UBXHitReactionComponent_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UBXHitReactionComponent()
{
	if (!Z_Registration_Info_UClass_UBXHitReactionComponent.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UBXHitReactionComponent.OuterSingleton, Z_Construct_UClass_UBXHitReactionComponent_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UBXHitReactionComponent.OuterSingleton;
}
template<> BATTLEX_API UClass* StaticClass<UBXHitReactionComponent>()
{
	return UBXHitReactionComponent::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(UBXHitReactionComponent);
UBXHitReactionComponent::~UBXHitReactionComponent() {}
// End Class UBXHitReactionComponent

// Begin Registration
struct Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Collision_BXHitReactionComponent_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UBXHitReactionComponent, UBXHitReactionComponent::StaticClass, TEXT("UBXHitReactionComponent"), &Z_Registration_Info_UClass_UBXHitReactionComponent, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UBXHitReactionComponent), 3881057354U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Collision_BXHitReactionComponent_h_1886677544(TEXT("/Script/BattleX"),
	Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Collision_BXHitReactionComponent_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Collision_BXHitReactionComponent_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
