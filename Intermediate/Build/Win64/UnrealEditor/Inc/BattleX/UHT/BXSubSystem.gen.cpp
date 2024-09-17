// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "BattleX/BXSubSystem.h"
#include "Runtime/Engine/Classes/Engine/GameInstance.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeBXSubSystem() {}

// Begin Cross Module References
BATTLEX_API UClass* Z_Construct_UClass_UBXManager_NoRegister();
BATTLEX_API UClass* Z_Construct_UClass_UBXSubSystem();
BATTLEX_API UClass* Z_Construct_UClass_UBXSubSystem_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_UGameInstanceSubsystem();
UPackage* Z_Construct_UPackage__Script_BattleX();
// End Cross Module References

// Begin Class UBXSubSystem Function GetBXManager
struct Z_Construct_UFunction_UBXSubSystem_GetBXManager_Statics
{
	struct BXSubSystem_eventGetBXManager_Parms
	{
		UBXManager* ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "BXSubSystem.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UBXSubSystem_GetBXManager_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXSubSystem_eventGetBXManager_Parms, ReturnValue), Z_Construct_UClass_UBXManager_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UBXSubSystem_GetBXManager_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXSubSystem_GetBXManager_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXSubSystem_GetBXManager_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UBXSubSystem_GetBXManager_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UBXSubSystem, nullptr, "GetBXManager", nullptr, nullptr, Z_Construct_UFunction_UBXSubSystem_GetBXManager_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UBXSubSystem_GetBXManager_Statics::PropPointers), sizeof(Z_Construct_UFunction_UBXSubSystem_GetBXManager_Statics::BXSubSystem_eventGetBXManager_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXSubSystem_GetBXManager_Statics::Function_MetaDataParams), Z_Construct_UFunction_UBXSubSystem_GetBXManager_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UBXSubSystem_GetBXManager_Statics::BXSubSystem_eventGetBXManager_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UBXSubSystem_GetBXManager()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UBXSubSystem_GetBXManager_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UBXSubSystem::execGetBXManager)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(UBXManager**)Z_Param__Result=P_THIS->GetBXManager();
	P_NATIVE_END;
}
// End Class UBXSubSystem Function GetBXManager

// Begin Class UBXSubSystem
void UBXSubSystem::StaticRegisterNativesUBXSubSystem()
{
	UClass* Class = UBXSubSystem::StaticClass();
	static const FNameNativePtrPair Funcs[] = {
		{ "GetBXManager", &UBXSubSystem::execGetBXManager },
	};
	FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UBXSubSystem);
UClass* Z_Construct_UClass_UBXSubSystem_NoRegister()
{
	return UBXSubSystem::StaticClass();
}
struct Z_Construct_UClass_UBXSubSystem_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "DisplayName", "BattleX Subsystem" },
		{ "IncludePath", "BXSubSystem.h" },
		{ "ModuleRelativePath", "BXSubSystem.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_BXManager_MetaData[] = {
		{ "ModuleRelativePath", "BXSubSystem.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_BXManager;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UBXSubSystem_GetBXManager, "GetBXManager" }, // 3531665147
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UBXSubSystem>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UBXSubSystem_Statics::NewProp_BXManager = { "BXManager", nullptr, (EPropertyFlags)0x0040000000002000, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXSubSystem, BXManager), Z_Construct_UClass_UBXManager_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_BXManager_MetaData), NewProp_BXManager_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UBXSubSystem_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXSubSystem_Statics::NewProp_BXManager,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UBXSubSystem_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_UBXSubSystem_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UGameInstanceSubsystem,
	(UObject* (*)())Z_Construct_UPackage__Script_BattleX,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UBXSubSystem_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UBXSubSystem_Statics::ClassParams = {
	&UBXSubSystem::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	Z_Construct_UClass_UBXSubSystem_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	UE_ARRAY_COUNT(Z_Construct_UClass_UBXSubSystem_Statics::PropPointers),
	0,
	0x001000A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UBXSubSystem_Statics::Class_MetaDataParams), Z_Construct_UClass_UBXSubSystem_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UBXSubSystem()
{
	if (!Z_Registration_Info_UClass_UBXSubSystem.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UBXSubSystem.OuterSingleton, Z_Construct_UClass_UBXSubSystem_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UBXSubSystem.OuterSingleton;
}
template<> BATTLEX_API UClass* StaticClass<UBXSubSystem>()
{
	return UBXSubSystem::StaticClass();
}
UBXSubSystem::UBXSubSystem() {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UBXSubSystem);
UBXSubSystem::~UBXSubSystem() {}
// End Class UBXSubSystem

// Begin Registration
struct Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_BXSubSystem_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UBXSubSystem, UBXSubSystem::StaticClass, TEXT("UBXSubSystem"), &Z_Registration_Info_UClass_UBXSubSystem, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UBXSubSystem), 2321705683U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_BXSubSystem_h_1644435218(TEXT("/Script/BattleX"),
	Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_BXSubSystem_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_BXSubSystem_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
