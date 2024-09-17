// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "BattleXEditor/TimelineEditor/Preview/BXTLPreviewActor.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeBXTLPreviewActor() {}

// Begin Cross Module References
BATTLEX_API UEnum* Z_Construct_UEnum_BattleX_EBXBodyPartType();
BATTLEXEDITOR_API UClass* Z_Construct_UClass_UBXTLPreviewActor();
BATTLEXEDITOR_API UClass* Z_Construct_UClass_UBXTLPreviewActor_NoRegister();
COREUOBJECT_API UClass* Z_Construct_UClass_UClass();
COREUOBJECT_API UClass* Z_Construct_UClass_UObject();
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FTransform();
ENGINE_API UClass* Z_Construct_UClass_AActor_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_UWorld_NoRegister();
UPackage* Z_Construct_UPackage__Script_BattleXEditor();
// End Cross Module References

// Begin Class UBXTLPreviewActor Function CreateActor
struct BXTLPreviewActor_eventCreateActor_Parms
{
	UWorld* InWorld;
	AActor* ReturnValue;

	/** Constructor, initializes return property only **/
	BXTLPreviewActor_eventCreateActor_Parms()
		: ReturnValue(NULL)
	{
	}
};
static FName NAME_UBXTLPreviewActor_CreateActor = FName(TEXT("CreateActor"));
AActor* UBXTLPreviewActor::CreateActor(UWorld* InWorld)
{
	BXTLPreviewActor_eventCreateActor_Parms Parms;
	Parms.InWorld=InWorld;
	ProcessEvent(FindFunctionChecked(NAME_UBXTLPreviewActor_CreateActor),&Parms);
	return Parms.ReturnValue;
}
struct Z_Construct_UFunction_UBXTLPreviewActor_CreateActor_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "TimelineEditor/Preview/BXTLPreviewActor.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_InWorld;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UBXTLPreviewActor_CreateActor_Statics::NewProp_InWorld = { "InWorld", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXTLPreviewActor_eventCreateActor_Parms, InWorld), Z_Construct_UClass_UWorld_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UBXTLPreviewActor_CreateActor_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXTLPreviewActor_eventCreateActor_Parms, ReturnValue), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UBXTLPreviewActor_CreateActor_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXTLPreviewActor_CreateActor_Statics::NewProp_InWorld,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXTLPreviewActor_CreateActor_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXTLPreviewActor_CreateActor_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UBXTLPreviewActor_CreateActor_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UBXTLPreviewActor, nullptr, "CreateActor", nullptr, nullptr, Z_Construct_UFunction_UBXTLPreviewActor_CreateActor_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UBXTLPreviewActor_CreateActor_Statics::PropPointers), sizeof(BXTLPreviewActor_eventCreateActor_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08020C00, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXTLPreviewActor_CreateActor_Statics::Function_MetaDataParams), Z_Construct_UFunction_UBXTLPreviewActor_CreateActor_Statics::Function_MetaDataParams) };
static_assert(sizeof(BXTLPreviewActor_eventCreateActor_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UBXTLPreviewActor_CreateActor()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UBXTLPreviewActor_CreateActor_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UBXTLPreviewActor::execCreateActor)
{
	P_GET_OBJECT(UWorld,Z_Param_InWorld);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(AActor**)Z_Param__Result=P_THIS->CreateActor_Implementation(Z_Param_InWorld);
	P_NATIVE_END;
}
// End Class UBXTLPreviewActor Function CreateActor

// Begin Class UBXTLPreviewActor
void UBXTLPreviewActor::StaticRegisterNativesUBXTLPreviewActor()
{
	UClass* Class = UBXTLPreviewActor::StaticClass();
	static const FNameNativePtrPair Funcs[] = {
		{ "CreateActor", &UBXTLPreviewActor::execCreateActor },
	};
	FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UBXTLPreviewActor);
UClass* Z_Construct_UClass_UBXTLPreviewActor_NoRegister()
{
	return UBXTLPreviewActor::StaticClass();
}
struct Z_Construct_UClass_UBXTLPreviewActor_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "IncludePath", "TimelineEditor/Preview/BXTLPreviewActor.h" },
		{ "IsBlueprintBase", "true" },
		{ "ModuleRelativePath", "TimelineEditor/Preview/BXTLPreviewActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ActorClass_MetaData[] = {
		{ "Category", "BXTLPreviewActor" },
		{ "ModuleRelativePath", "TimelineEditor/Preview/BXTLPreviewActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_SpawnTransform_MetaData[] = {
		{ "Category", "BXTLPreviewActor" },
		{ "ModuleRelativePath", "TimelineEditor/Preview/BXTLPreviewActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LockedBodyPart_MetaData[] = {
		{ "Category", "BXTLPreviewActor" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xd2\xaa\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xe5\xb2\xbf\xce\xbb\n" },
#endif
		{ "ModuleRelativePath", "TimelineEditor/Preview/BXTLPreviewActor.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xd2\xaa\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xe5\xb2\xbf\xce\xbb" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FClassPropertyParams NewProp_ActorClass;
	static const UECodeGen_Private::FStructPropertyParams NewProp_SpawnTransform;
	static const UECodeGen_Private::FBytePropertyParams NewProp_LockedBodyPart_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_LockedBodyPart;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UBXTLPreviewActor_CreateActor, "CreateActor" }, // 818285041
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UBXTLPreviewActor>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FClassPropertyParams Z_Construct_UClass_UBXTLPreviewActor_Statics::NewProp_ActorClass = { "ActorClass", nullptr, (EPropertyFlags)0x0014000000010001, UECodeGen_Private::EPropertyGenFlags::Class, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXTLPreviewActor, ActorClass), Z_Construct_UClass_UClass, Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ActorClass_MetaData), NewProp_ActorClass_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UBXTLPreviewActor_Statics::NewProp_SpawnTransform = { "SpawnTransform", nullptr, (EPropertyFlags)0x0010000000010001, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXTLPreviewActor, SpawnTransform), Z_Construct_UScriptStruct_FTransform, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_SpawnTransform_MetaData), NewProp_SpawnTransform_MetaData) };
const UECodeGen_Private::FBytePropertyParams Z_Construct_UClass_UBXTLPreviewActor_Statics::NewProp_LockedBodyPart_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UClass_UBXTLPreviewActor_Statics::NewProp_LockedBodyPart = { "LockedBodyPart", nullptr, (EPropertyFlags)0x0010000000010001, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXTLPreviewActor, LockedBodyPart), Z_Construct_UEnum_BattleX_EBXBodyPartType, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LockedBodyPart_MetaData), NewProp_LockedBodyPart_MetaData) }; // 84625349
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UBXTLPreviewActor_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXTLPreviewActor_Statics::NewProp_ActorClass,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXTLPreviewActor_Statics::NewProp_SpawnTransform,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXTLPreviewActor_Statics::NewProp_LockedBodyPart_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXTLPreviewActor_Statics::NewProp_LockedBodyPart,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UBXTLPreviewActor_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_UBXTLPreviewActor_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UObject,
	(UObject* (*)())Z_Construct_UPackage__Script_BattleXEditor,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UBXTLPreviewActor_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UBXTLPreviewActor_Statics::ClassParams = {
	&UBXTLPreviewActor::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	Z_Construct_UClass_UBXTLPreviewActor_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	UE_ARRAY_COUNT(Z_Construct_UClass_UBXTLPreviewActor_Statics::PropPointers),
	0,
	0x000010A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UBXTLPreviewActor_Statics::Class_MetaDataParams), Z_Construct_UClass_UBXTLPreviewActor_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UBXTLPreviewActor()
{
	if (!Z_Registration_Info_UClass_UBXTLPreviewActor.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UBXTLPreviewActor.OuterSingleton, Z_Construct_UClass_UBXTLPreviewActor_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UBXTLPreviewActor.OuterSingleton;
}
template<> BATTLEXEDITOR_API UClass* StaticClass<UBXTLPreviewActor>()
{
	return UBXTLPreviewActor::StaticClass();
}
UBXTLPreviewActor::UBXTLPreviewActor(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UBXTLPreviewActor);
UBXTLPreviewActor::~UBXTLPreviewActor() {}
// End Class UBXTLPreviewActor

// Begin Registration
struct Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_Preview_BXTLPreviewActor_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UBXTLPreviewActor, UBXTLPreviewActor::StaticClass, TEXT("UBXTLPreviewActor"), &Z_Registration_Info_UClass_UBXTLPreviewActor, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UBXTLPreviewActor), 1707074987U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_Preview_BXTLPreviewActor_h_3491822674(TEXT("/Script/BattleXEditor"),
	Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_Preview_BXTLPreviewActor_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_Preview_BXTLPreviewActor_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
