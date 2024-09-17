// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "BattleX/Collision/BXCollision.h"
#include "Runtime/Engine/Classes/Engine/HitResult.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeBXCollision() {}

// Begin Cross Module References
BATTLEX_API UClass* Z_Construct_UClass_UBXCollisionLibrary();
BATTLEX_API UClass* Z_Construct_UClass_UBXCollisionLibrary_NoRegister();
BATTLEX_API UEnum* Z_Construct_UEnum_BattleX_EBXCDirection();
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXCFilter();
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXCParameter();
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXCSBox();
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXCSCapsule();
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXCSCylinder();
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXCSHollowCylinder();
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXCSSector();
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXCSSphere();
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXCStrategy();
COREUOBJECT_API UClass* Z_Construct_UClass_UObject_NoRegister();
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FRotator();
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FVector();
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FVector2D();
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FVector4();
ENGINE_API UClass* Z_Construct_UClass_AActor_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_UBlueprintFunctionLibrary();
ENGINE_API UEnum* Z_Construct_UEnum_Engine_EObjectTypeQuery();
ENGINE_API UScriptStruct* Z_Construct_UScriptStruct_FHitResult();
UPackage* Z_Construct_UPackage__Script_BattleX();
// End Cross Module References

// Begin Enum EBXCDirection
static FEnumRegistrationInfo Z_Registration_Info_UEnum_EBXCDirection;
static UEnum* EBXCDirection_StaticEnum()
{
	if (!Z_Registration_Info_UEnum_EBXCDirection.OuterSingleton)
	{
		Z_Registration_Info_UEnum_EBXCDirection.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_BattleX_EBXCDirection, (UObject*)Z_Construct_UPackage__Script_BattleX(), TEXT("EBXCDirection"));
	}
	return Z_Registration_Info_UEnum_EBXCDirection.OuterSingleton;
}
template<> BATTLEX_API UEnum* StaticEnum<EBXCDirection>()
{
	return EBXCDirection_StaticEnum();
}
struct Z_Construct_UEnum_BattleX_EBXCDirection_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "D_InToOut.DisplayName", "\xe4\xbb\x8e\xe9\x87\x8c\xe5\x90\x91\xe5\xa4\x96" },
		{ "D_InToOut.Name", "EBXCDirection::D_InToOut" },
		{ "D_LeftToRight.DisplayName", "\xe4\xbb\x8e\xe5\xb7\xa6\xe5\x90\x91\xe5\x8f\xb3" },
		{ "D_LeftToRight.Name", "EBXCDirection::D_LeftToRight" },
		{ "D_RightToLeft.DisplayName", "\xe4\xbb\x8e\xe5\x8f\xb3\xe5\x90\x91\xe5\xb7\xa6" },
		{ "D_RightToLeft.Name", "EBXCDirection::D_RightToLeft" },
		{ "ModuleRelativePath", "Collision/BXCollision.h" },
	};
#endif // WITH_METADATA
	static constexpr UECodeGen_Private::FEnumeratorParam Enumerators[] = {
		{ "EBXCDirection::D_RightToLeft", (int64)EBXCDirection::D_RightToLeft },
		{ "EBXCDirection::D_LeftToRight", (int64)EBXCDirection::D_LeftToRight },
		{ "EBXCDirection::D_InToOut", (int64)EBXCDirection::D_InToOut },
	};
	static const UECodeGen_Private::FEnumParams EnumParams;
};
const UECodeGen_Private::FEnumParams Z_Construct_UEnum_BattleX_EBXCDirection_Statics::EnumParams = {
	(UObject*(*)())Z_Construct_UPackage__Script_BattleX,
	nullptr,
	"EBXCDirection",
	"EBXCDirection",
	Z_Construct_UEnum_BattleX_EBXCDirection_Statics::Enumerators,
	RF_Public|RF_Transient|RF_MarkAsNative,
	UE_ARRAY_COUNT(Z_Construct_UEnum_BattleX_EBXCDirection_Statics::Enumerators),
	EEnumFlags::None,
	(uint8)UEnum::ECppForm::EnumClass,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UEnum_BattleX_EBXCDirection_Statics::Enum_MetaDataParams), Z_Construct_UEnum_BattleX_EBXCDirection_Statics::Enum_MetaDataParams)
};
UEnum* Z_Construct_UEnum_BattleX_EBXCDirection()
{
	if (!Z_Registration_Info_UEnum_EBXCDirection.InnerSingleton)
	{
		UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_EBXCDirection.InnerSingleton, Z_Construct_UEnum_BattleX_EBXCDirection_Statics::EnumParams);
	}
	return Z_Registration_Info_UEnum_EBXCDirection.InnerSingleton;
}
// End Enum EBXCDirection

// Begin ScriptStruct FBXCFilter
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_BXCFilter;
class UScriptStruct* FBXCFilter::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_BXCFilter.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_BXCFilter.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FBXCFilter, (UObject*)Z_Construct_UPackage__Script_BattleX(), TEXT("BXCFilter"));
	}
	return Z_Registration_Info_UScriptStruct_BXCFilter.OuterSingleton;
}
template<> BATTLEX_API UScriptStruct* StaticStruct<FBXCFilter>()
{
	return FBXCFilter::StaticStruct();
}
struct Z_Construct_UScriptStruct_FBXCFilter_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe7\xa2\xb0\xe6\x92\x9e\xe7\xad\x9b\xe9\x80\x89\xe5\x99\xa8\n" },
#endif
		{ "ModuleRelativePath", "Collision/BXCollision.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe7\xa2\xb0\xe6\x92\x9e\xe7\xad\x9b\xe9\x80\x89\xe5\x99\xa8" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bIgnoreSelf_MetaData[] = {
		{ "Category", "BXCFilter" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe6\x98\xaf\xe5\x90\xa6\xe5\xbf\xbd\xe7\x95\xa5\xe8\x87\xaa\xe5\xb7\xb1\n" },
#endif
		{ "ModuleRelativePath", "Collision/BXCollision.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe6\x98\xaf\xe5\x90\xa6\xe5\xbf\xbd\xe7\x95\xa5\xe8\x87\xaa\xe5\xb7\xb1" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ClassTypes_MetaData[] = {
		{ "Category", "BXCFilter" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe5\x8f\xaf\xe4\xbb\xa5\xe9\x80\x9a\xe8\xbf\x87\xe7\xad\x9b\xe9\x80\x89\xe7\x9a\x84\xe7\x88\xb6\xe7\xb1\xbb\xe5\x9e\x8b\n" },
#endif
		{ "ModuleRelativePath", "Collision/BXCollision.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe5\x8f\xaf\xe4\xbb\xa5\xe9\x80\x9a\xe8\xbf\x87\xe7\xad\x9b\xe9\x80\x89\xe7\x9a\x84\xe7\x88\xb6\xe7\xb1\xbb\xe5\x9e\x8b" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_IgnoreClassTypes_MetaData[] = {
		{ "Category", "BXCFilter" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe4\xb8\x8d\xe5\x8f\xaf\xe4\xbb\xa5\xe9\x80\x9a\xe8\xbf\x87\xe7\xad\x9b\xe9\x80\x89\xe7\x9a\x84\xe7\x88\xb6\xe7\xb1\xbb\xe5\x9e\x8b\n" },
#endif
		{ "ModuleRelativePath", "Collision/BXCollision.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe4\xb8\x8d\xe5\x8f\xaf\xe4\xbb\xa5\xe9\x80\x9a\xe8\xbf\x87\xe7\xad\x9b\xe9\x80\x89\xe7\x9a\x84\xe7\x88\xb6\xe7\xb1\xbb\xe5\x9e\x8b" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ActorTags_MetaData[] = {
		{ "Category", "BXCFilter" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe5\x8f\xaf\xe4\xbb\xa5\xe9\x80\x9a\xe8\xbf\x87\xe7\xad\x9b\xe9\x80\x89\xe7\x9a\x84""Actor\xe6\xa0\x87\xe7\xad\xbe\n" },
#endif
		{ "ModuleRelativePath", "Collision/BXCollision.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe5\x8f\xaf\xe4\xbb\xa5\xe9\x80\x9a\xe8\xbf\x87\xe7\xad\x9b\xe9\x80\x89\xe7\x9a\x84""Actor\xe6\xa0\x87\xe7\xad\xbe" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_IgnoreActorTags_MetaData[] = {
		{ "Category", "BXCFilter" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe4\xb8\x8d\xe5\x8f\xaf\xe4\xbb\xa5\xe9\x80\x9a\xe8\xbf\x87\xe7\xad\x9b\xe9\x80\x89\xe7\x9a\x84""Actor\xe6\xa0\x87\xe7\xad\xbe\n" },
#endif
		{ "ModuleRelativePath", "Collision/BXCollision.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe4\xb8\x8d\xe5\x8f\xaf\xe4\xbb\xa5\xe9\x80\x9a\xe8\xbf\x87\xe7\xad\x9b\xe9\x80\x89\xe7\x9a\x84""Actor\xe6\xa0\x87\xe7\xad\xbe" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ComponentTags_MetaData[] = {
		{ "Category", "BXCFilter" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe5\x8f\xaf\xe4\xbb\xa5\xe9\x80\x9a\xe8\xbf\x87\xe7\xad\x9b\xe9\x80\x89\xe7\x9a\x84""Component\xe6\xa0\x87\xe7\xad\xbe\n" },
#endif
		{ "ModuleRelativePath", "Collision/BXCollision.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe5\x8f\xaf\xe4\xbb\xa5\xe9\x80\x9a\xe8\xbf\x87\xe7\xad\x9b\xe9\x80\x89\xe7\x9a\x84""Component\xe6\xa0\x87\xe7\xad\xbe" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_IgnoreComponentTags_MetaData[] = {
		{ "Category", "BXCFilter" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe4\xb8\x8d\xe5\x8f\xaf\xe4\xbb\xa5\xe9\x80\x9a\xe8\xbf\x87\xe7\xad\x9b\xe9\x80\x89\xe7\x9a\x84""Component\xe6\xa0\x87\xe7\xad\xbe\n" },
#endif
		{ "ModuleRelativePath", "Collision/BXCollision.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe4\xb8\x8d\xe5\x8f\xaf\xe4\xbb\xa5\xe9\x80\x9a\xe8\xbf\x87\xe7\xad\x9b\xe9\x80\x89\xe7\x9a\x84""Component\xe6\xa0\x87\xe7\xad\xbe" },
#endif
	};
#endif // WITH_METADATA
	static void NewProp_bIgnoreSelf_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bIgnoreSelf;
	static const UECodeGen_Private::FSoftClassPropertyParams NewProp_ClassTypes_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_ClassTypes;
	static const UECodeGen_Private::FSoftClassPropertyParams NewProp_IgnoreClassTypes_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_IgnoreClassTypes;
	static const UECodeGen_Private::FNamePropertyParams NewProp_ActorTags_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_ActorTags;
	static const UECodeGen_Private::FNamePropertyParams NewProp_IgnoreActorTags_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_IgnoreActorTags;
	static const UECodeGen_Private::FNamePropertyParams NewProp_ComponentTags_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_ComponentTags;
	static const UECodeGen_Private::FNamePropertyParams NewProp_IgnoreComponentTags_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_IgnoreComponentTags;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FBXCFilter>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
};
void Z_Construct_UScriptStruct_FBXCFilter_Statics::NewProp_bIgnoreSelf_SetBit(void* Obj)
{
	((FBXCFilter*)Obj)->bIgnoreSelf = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FBXCFilter_Statics::NewProp_bIgnoreSelf = { "bIgnoreSelf", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FBXCFilter), &Z_Construct_UScriptStruct_FBXCFilter_Statics::NewProp_bIgnoreSelf_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bIgnoreSelf_MetaData), NewProp_bIgnoreSelf_MetaData) };
const UECodeGen_Private::FSoftClassPropertyParams Z_Construct_UScriptStruct_FBXCFilter_Statics::NewProp_ClassTypes_Inner = { "ClassTypes", nullptr, (EPropertyFlags)0x0004000000000000, UECodeGen_Private::EPropertyGenFlags::SoftClass, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UClass_UObject_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FBXCFilter_Statics::NewProp_ClassTypes = { "ClassTypes", nullptr, (EPropertyFlags)0x0014000000000005, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXCFilter, ClassTypes), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ClassTypes_MetaData), NewProp_ClassTypes_MetaData) };
const UECodeGen_Private::FSoftClassPropertyParams Z_Construct_UScriptStruct_FBXCFilter_Statics::NewProp_IgnoreClassTypes_Inner = { "IgnoreClassTypes", nullptr, (EPropertyFlags)0x0004000000000000, UECodeGen_Private::EPropertyGenFlags::SoftClass, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UClass_UObject_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FBXCFilter_Statics::NewProp_IgnoreClassTypes = { "IgnoreClassTypes", nullptr, (EPropertyFlags)0x0014000000000005, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXCFilter, IgnoreClassTypes), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_IgnoreClassTypes_MetaData), NewProp_IgnoreClassTypes_MetaData) };
const UECodeGen_Private::FNamePropertyParams Z_Construct_UScriptStruct_FBXCFilter_Statics::NewProp_ActorTags_Inner = { "ActorTags", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FBXCFilter_Statics::NewProp_ActorTags = { "ActorTags", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXCFilter, ActorTags), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ActorTags_MetaData), NewProp_ActorTags_MetaData) };
const UECodeGen_Private::FNamePropertyParams Z_Construct_UScriptStruct_FBXCFilter_Statics::NewProp_IgnoreActorTags_Inner = { "IgnoreActorTags", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FBXCFilter_Statics::NewProp_IgnoreActorTags = { "IgnoreActorTags", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXCFilter, IgnoreActorTags), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_IgnoreActorTags_MetaData), NewProp_IgnoreActorTags_MetaData) };
const UECodeGen_Private::FNamePropertyParams Z_Construct_UScriptStruct_FBXCFilter_Statics::NewProp_ComponentTags_Inner = { "ComponentTags", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FBXCFilter_Statics::NewProp_ComponentTags = { "ComponentTags", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXCFilter, ComponentTags), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ComponentTags_MetaData), NewProp_ComponentTags_MetaData) };
const UECodeGen_Private::FNamePropertyParams Z_Construct_UScriptStruct_FBXCFilter_Statics::NewProp_IgnoreComponentTags_Inner = { "IgnoreComponentTags", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FBXCFilter_Statics::NewProp_IgnoreComponentTags = { "IgnoreComponentTags", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXCFilter, IgnoreComponentTags), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_IgnoreComponentTags_MetaData), NewProp_IgnoreComponentTags_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FBXCFilter_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXCFilter_Statics::NewProp_bIgnoreSelf,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXCFilter_Statics::NewProp_ClassTypes_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXCFilter_Statics::NewProp_ClassTypes,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXCFilter_Statics::NewProp_IgnoreClassTypes_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXCFilter_Statics::NewProp_IgnoreClassTypes,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXCFilter_Statics::NewProp_ActorTags_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXCFilter_Statics::NewProp_ActorTags,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXCFilter_Statics::NewProp_IgnoreActorTags_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXCFilter_Statics::NewProp_IgnoreActorTags,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXCFilter_Statics::NewProp_ComponentTags_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXCFilter_Statics::NewProp_ComponentTags,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXCFilter_Statics::NewProp_IgnoreComponentTags_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXCFilter_Statics::NewProp_IgnoreComponentTags,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXCFilter_Statics::PropPointers) < 2048);
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FBXCFilter_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_BattleX,
	nullptr,
	&NewStructOps,
	"BXCFilter",
	Z_Construct_UScriptStruct_FBXCFilter_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXCFilter_Statics::PropPointers),
	sizeof(FBXCFilter),
	alignof(FBXCFilter),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000201),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXCFilter_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FBXCFilter_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FBXCFilter()
{
	if (!Z_Registration_Info_UScriptStruct_BXCFilter.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_BXCFilter.InnerSingleton, Z_Construct_UScriptStruct_FBXCFilter_Statics::StructParams);
	}
	return Z_Registration_Info_UScriptStruct_BXCFilter.InnerSingleton;
}
// End ScriptStruct FBXCFilter

// Begin ScriptStruct FBXCParameter
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_BXCParameter;
class UScriptStruct* FBXCParameter::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_BXCParameter.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_BXCParameter.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FBXCParameter, (UObject*)Z_Construct_UPackage__Script_BattleX(), TEXT("BXCParameter"));
	}
	return Z_Registration_Info_UScriptStruct_BXCParameter.OuterSingleton;
}
template<> BATTLEX_API UScriptStruct* StaticStruct<FBXCParameter>()
{
	return FBXCParameter::StaticStruct();
}
struct Z_Construct_UScriptStruct_FBXCParameter_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe7\xa2\xb0\xe6\x92\x9e\xe5\x8f\x82\xe6\x95\xb0\n" },
#endif
		{ "ModuleRelativePath", "Collision/BXCollision.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe7\xa2\xb0\xe6\x92\x9e\xe5\x8f\x82\xe6\x95\xb0" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Requester_MetaData[] = {
		{ "Category", "BXCParameter" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe8\xaf\xb7\xe6\xb1\x82\xe7\xa2\xb0\xe6\x92\x9e\xe6\xa3\x80\xe6\x9f\xa5\xe7\x9a\x84\xe5\xaf\xb9\xe8\xb1\xa1\n" },
#endif
		{ "ModuleRelativePath", "Collision/BXCollision.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe8\xaf\xb7\xe6\xb1\x82\xe7\xa2\xb0\xe6\x92\x9e\xe6\xa3\x80\xe6\x9f\xa5\xe7\x9a\x84\xe5\xaf\xb9\xe8\xb1\xa1" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_StartLocation_MetaData[] = {
		{ "Category", "BXCParameter" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe8\xb5\xb7\xe5\xa7\x8b\xe4\xbd\x8d\xe7\xbd\xae\n" },
#endif
		{ "ModuleRelativePath", "Collision/BXCollision.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe8\xb5\xb7\xe5\xa7\x8b\xe4\xbd\x8d\xe7\xbd\xae" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_StartRotation_MetaData[] = {
		{ "Category", "BXCParameter" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe8\xb5\xb7\xe5\xa7\x8b\xe6\x9c\x9d\xe5\x90\x91\n" },
#endif
		{ "ModuleRelativePath", "Collision/BXCollision.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe8\xb5\xb7\xe5\xa7\x8b\xe6\x9c\x9d\xe5\x90\x91" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_EndLocation_MetaData[] = {
		{ "Category", "BXCParameter" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe7\xbb\x88\xe6\xad\xa2\xe4\xbd\x8d\xe7\xbd\xae\n" },
#endif
		{ "ModuleRelativePath", "Collision/BXCollision.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe7\xbb\x88\xe6\xad\xa2\xe4\xbd\x8d\xe7\xbd\xae" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_EndRotation_MetaData[] = {
		{ "Category", "BXCParameter" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe7\xbb\x88\xe6\xad\xa2\xe6\x9c\x9d\xe5\x90\x91\n" },
#endif
		{ "ModuleRelativePath", "Collision/BXCollision.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe7\xbb\x88\xe6\xad\xa2\xe6\x9c\x9d\xe5\x90\x91" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Scale_MetaData[] = {
		{ "Category", "BXCParameter" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe7\xbc\xa9\xe6\x94\xbe\xe5\xa4\xa7\xe5\xb0\x8f\n" },
#endif
		{ "ModuleRelativePath", "Collision/BXCollision.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe7\xbc\xa9\xe6\x94\xbe\xe5\xa4\xa7\xe5\xb0\x8f" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Requester;
	static const UECodeGen_Private::FStructPropertyParams NewProp_StartLocation;
	static const UECodeGen_Private::FStructPropertyParams NewProp_StartRotation;
	static const UECodeGen_Private::FStructPropertyParams NewProp_EndLocation;
	static const UECodeGen_Private::FStructPropertyParams NewProp_EndRotation;
	static const UECodeGen_Private::FStructPropertyParams NewProp_Scale;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FBXCParameter>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UScriptStruct_FBXCParameter_Statics::NewProp_Requester = { "Requester", nullptr, (EPropertyFlags)0x0010000000010005, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXCParameter, Requester), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Requester_MetaData), NewProp_Requester_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FBXCParameter_Statics::NewProp_StartLocation = { "StartLocation", nullptr, (EPropertyFlags)0x0010000000010005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXCParameter, StartLocation), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_StartLocation_MetaData), NewProp_StartLocation_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FBXCParameter_Statics::NewProp_StartRotation = { "StartRotation", nullptr, (EPropertyFlags)0x0010000000010005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXCParameter, StartRotation), Z_Construct_UScriptStruct_FRotator, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_StartRotation_MetaData), NewProp_StartRotation_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FBXCParameter_Statics::NewProp_EndLocation = { "EndLocation", nullptr, (EPropertyFlags)0x0010000000010005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXCParameter, EndLocation), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_EndLocation_MetaData), NewProp_EndLocation_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FBXCParameter_Statics::NewProp_EndRotation = { "EndRotation", nullptr, (EPropertyFlags)0x0010000000010005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXCParameter, EndRotation), Z_Construct_UScriptStruct_FRotator, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_EndRotation_MetaData), NewProp_EndRotation_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FBXCParameter_Statics::NewProp_Scale = { "Scale", nullptr, (EPropertyFlags)0x0010000000010005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXCParameter, Scale), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Scale_MetaData), NewProp_Scale_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FBXCParameter_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXCParameter_Statics::NewProp_Requester,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXCParameter_Statics::NewProp_StartLocation,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXCParameter_Statics::NewProp_StartRotation,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXCParameter_Statics::NewProp_EndLocation,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXCParameter_Statics::NewProp_EndRotation,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXCParameter_Statics::NewProp_Scale,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXCParameter_Statics::PropPointers) < 2048);
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FBXCParameter_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_BattleX,
	nullptr,
	&NewStructOps,
	"BXCParameter",
	Z_Construct_UScriptStruct_FBXCParameter_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXCParameter_Statics::PropPointers),
	sizeof(FBXCParameter),
	alignof(FBXCParameter),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000201),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXCParameter_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FBXCParameter_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FBXCParameter()
{
	if (!Z_Registration_Info_UScriptStruct_BXCParameter.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_BXCParameter.InnerSingleton, Z_Construct_UScriptStruct_FBXCParameter_Statics::StructParams);
	}
	return Z_Registration_Info_UScriptStruct_BXCParameter.InnerSingleton;
}
// End ScriptStruct FBXCParameter

// Begin ScriptStruct FBXCStrategy
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_BXCStrategy;
class UScriptStruct* FBXCStrategy::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_BXCStrategy.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_BXCStrategy.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FBXCStrategy, (UObject*)Z_Construct_UPackage__Script_BattleX(), TEXT("BXCStrategy"));
	}
	return Z_Registration_Info_UScriptStruct_BXCStrategy.OuterSingleton;
}
template<> BATTLEX_API UScriptStruct* StaticStruct<FBXCStrategy>()
{
	return FBXCStrategy::StaticStruct();
}
struct Z_Construct_UScriptStruct_FBXCStrategy_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe7\xa2\xb0\xe6\x92\x9e\xe7\xad\x96\xe7\x95\xa5\n" },
#endif
		{ "ModuleRelativePath", "Collision/BXCollision.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe7\xa2\xb0\xe6\x92\x9e\xe7\xad\x96\xe7\x95\xa5" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ObjectTypes_MetaData[] = {
		{ "Category", "BXCStrategy" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Object\xe7\x9a\x84\xe7\xa2\xb0\xe6\x92\x9e\xe7\xb1\xbb\xe5\x9e\x8b\n" },
#endif
		{ "ModuleRelativePath", "Collision/BXCollision.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Object\xe7\x9a\x84\xe7\xa2\xb0\xe6\x92\x9e\xe7\xb1\xbb\xe5\x9e\x8b" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Filter_MetaData[] = {
		{ "Category", "BXCStrategy" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe7\xa2\xb0\xe6\x92\x9e\xe7\xad\x9b\xe9\x80\x89\xe5\x99\xa8\n" },
#endif
		{ "ModuleRelativePath", "Collision/BXCollision.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe7\xa2\xb0\xe6\x92\x9e\xe7\xad\x9b\xe9\x80\x89\xe5\x99\xa8" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FBytePropertyParams NewProp_ObjectTypes_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_ObjectTypes;
	static const UECodeGen_Private::FStructPropertyParams NewProp_Filter;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FBXCStrategy>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
};
const UECodeGen_Private::FBytePropertyParams Z_Construct_UScriptStruct_FBXCStrategy_Statics::NewProp_ObjectTypes_Inner = { "ObjectTypes", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UEnum_Engine_EObjectTypeQuery, METADATA_PARAMS(0, nullptr) }; // 1798967895
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FBXCStrategy_Statics::NewProp_ObjectTypes = { "ObjectTypes", nullptr, (EPropertyFlags)0x0010000000010001, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXCStrategy, ObjectTypes), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ObjectTypes_MetaData), NewProp_ObjectTypes_MetaData) }; // 1798967895
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FBXCStrategy_Statics::NewProp_Filter = { "Filter", nullptr, (EPropertyFlags)0x0010000000010001, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXCStrategy, Filter), Z_Construct_UScriptStruct_FBXCFilter, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Filter_MetaData), NewProp_Filter_MetaData) }; // 366435299
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FBXCStrategy_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXCStrategy_Statics::NewProp_ObjectTypes_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXCStrategy_Statics::NewProp_ObjectTypes,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXCStrategy_Statics::NewProp_Filter,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXCStrategy_Statics::PropPointers) < 2048);
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FBXCStrategy_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_BattleX,
	nullptr,
	&NewStructOps,
	"BXCStrategy",
	Z_Construct_UScriptStruct_FBXCStrategy_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXCStrategy_Statics::PropPointers),
	sizeof(FBXCStrategy),
	alignof(FBXCStrategy),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000201),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXCStrategy_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FBXCStrategy_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FBXCStrategy()
{
	if (!Z_Registration_Info_UScriptStruct_BXCStrategy.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_BXCStrategy.InnerSingleton, Z_Construct_UScriptStruct_FBXCStrategy_Statics::StructParams);
	}
	return Z_Registration_Info_UScriptStruct_BXCStrategy.InnerSingleton;
}
// End ScriptStruct FBXCStrategy

// Begin ScriptStruct FBXCSSphere
static_assert(std::is_polymorphic<FBXCSSphere>() == std::is_polymorphic<FBXCStrategy>(), "USTRUCT FBXCSSphere cannot be polymorphic unless super FBXCStrategy is polymorphic");
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_BXCSSphere;
class UScriptStruct* FBXCSSphere::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_BXCSSphere.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_BXCSSphere.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FBXCSSphere, (UObject*)Z_Construct_UPackage__Script_BattleX(), TEXT("BXCSSphere"));
	}
	return Z_Registration_Info_UScriptStruct_BXCSSphere.OuterSingleton;
}
template<> BATTLEX_API UScriptStruct* StaticStruct<FBXCSSphere>()
{
	return FBXCSSphere::StaticStruct();
}
struct Z_Construct_UScriptStruct_FBXCSSphere_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe7\x90\x83\xe4\xbd\x93\xe7\xa2\xb0\xe6\x92\x9e\n" },
#endif
		{ "ModuleRelativePath", "Collision/BXCollision.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe7\x90\x83\xe4\xbd\x93\xe7\xa2\xb0\xe6\x92\x9e" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_SpereSize_MetaData[] = {
		{ "Category", "BXCSSphere" },
		{ "ClampMin", "0.001000" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe7\x90\x83\xe4\xbd\x93\xe5\xa4\xa7\xe5\xb0\x8f\n" },
#endif
		{ "ModuleRelativePath", "Collision/BXCollision.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe7\x90\x83\xe4\xbd\x93\xe5\xa4\xa7\xe5\xb0\x8f" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bUseSweep_MetaData[] = {
		{ "Category", "BXCSSphere" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe5\x90\xaf\xe7\x94\xa8\xe6\x89\xab\xe6\x8f\x8f\n" },
#endif
		{ "ModuleRelativePath", "Collision/BXCollision.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe5\x90\xaf\xe7\x94\xa8\xe6\x89\xab\xe6\x8f\x8f" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFloatPropertyParams NewProp_SpereSize;
	static void NewProp_bUseSweep_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bUseSweep;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FBXCSSphere>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
};
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FBXCSSphere_Statics::NewProp_SpereSize = { "SpereSize", nullptr, (EPropertyFlags)0x0010000000010001, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXCSSphere, SpereSize), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_SpereSize_MetaData), NewProp_SpereSize_MetaData) };
void Z_Construct_UScriptStruct_FBXCSSphere_Statics::NewProp_bUseSweep_SetBit(void* Obj)
{
	((FBXCSSphere*)Obj)->bUseSweep = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FBXCSSphere_Statics::NewProp_bUseSweep = { "bUseSweep", nullptr, (EPropertyFlags)0x0010000000010001, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FBXCSSphere), &Z_Construct_UScriptStruct_FBXCSSphere_Statics::NewProp_bUseSweep_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bUseSweep_MetaData), NewProp_bUseSweep_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FBXCSSphere_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXCSSphere_Statics::NewProp_SpereSize,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXCSSphere_Statics::NewProp_bUseSweep,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXCSSphere_Statics::PropPointers) < 2048);
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FBXCSSphere_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_BattleX,
	Z_Construct_UScriptStruct_FBXCStrategy,
	&NewStructOps,
	"BXCSSphere",
	Z_Construct_UScriptStruct_FBXCSSphere_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXCSSphere_Statics::PropPointers),
	sizeof(FBXCSSphere),
	alignof(FBXCSSphere),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000201),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXCSSphere_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FBXCSSphere_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FBXCSSphere()
{
	if (!Z_Registration_Info_UScriptStruct_BXCSSphere.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_BXCSSphere.InnerSingleton, Z_Construct_UScriptStruct_FBXCSSphere_Statics::StructParams);
	}
	return Z_Registration_Info_UScriptStruct_BXCSSphere.InnerSingleton;
}
// End ScriptStruct FBXCSSphere

// Begin ScriptStruct FBXCSCapsule
static_assert(std::is_polymorphic<FBXCSCapsule>() == std::is_polymorphic<FBXCStrategy>(), "USTRUCT FBXCSCapsule cannot be polymorphic unless super FBXCStrategy is polymorphic");
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_BXCSCapsule;
class UScriptStruct* FBXCSCapsule::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_BXCSCapsule.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_BXCSCapsule.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FBXCSCapsule, (UObject*)Z_Construct_UPackage__Script_BattleX(), TEXT("BXCSCapsule"));
	}
	return Z_Registration_Info_UScriptStruct_BXCSCapsule.OuterSingleton;
}
template<> BATTLEX_API UScriptStruct* StaticStruct<FBXCSCapsule>()
{
	return FBXCSCapsule::StaticStruct();
}
struct Z_Construct_UScriptStruct_FBXCSCapsule_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe8\x83\xb6\xe5\x9b\x8a\xe4\xbd\x93\xe7\xa2\xb0\xe6\x92\x9e\n" },
#endif
		{ "ModuleRelativePath", "Collision/BXCollision.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe8\x83\xb6\xe5\x9b\x8a\xe4\xbd\x93\xe7\xa2\xb0\xe6\x92\x9e" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_CapsuleSize_MetaData[] = {
		{ "Category", "BXCSCapsule" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe8\x83\xb6\xe5\x9b\x8a\xe4\xbd\x93\xe5\xa4\xa7\xe5\xb0\x8f(\xe5\x8d\x8a\xe5\xbe\x84\xe3\x80\x81\xe5\x8d\x8a\xe9\xab\x98)\n" },
#endif
		{ "ModuleRelativePath", "Collision/BXCollision.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe8\x83\xb6\xe5\x9b\x8a\xe4\xbd\x93\xe5\xa4\xa7\xe5\xb0\x8f(\xe5\x8d\x8a\xe5\xbe\x84\xe3\x80\x81\xe5\x8d\x8a\xe9\xab\x98)" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bUseSweep_MetaData[] = {
		{ "Category", "BXCSCapsule" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe5\x90\xaf\xe7\x94\xa8\xe6\x89\xab\xe6\x8f\x8f\n" },
#endif
		{ "ModuleRelativePath", "Collision/BXCollision.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe5\x90\xaf\xe7\x94\xa8\xe6\x89\xab\xe6\x8f\x8f" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_AngleStep_MetaData[] = {
		{ "Category", "BXCSCapsule" },
		{ "ClampMax", "90.000000" },
		{ "ClampMin", "5.000000" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe5\x90\xaf\xe7\x94\xa8\xe6\x89\xab\xe6\x8f\x8f \xe4\xb8\x94 \xe8\xb5\xb7\xe5\xa7\x8b\xe6\x9c\x9d\xe5\x90\x91\xe5\x92\x8c\xe7\xbb\x88\xe6\xad\xa2\xe6\x9c\x9d\xe5\x90\x91\xe4\xb8\x8d\xe4\xb8\x80\xe8\x87\xb4\xe6\x97\xb6\xef\xbc\x8c\xe8\xaf\xa5\xe5\x8f\x98\xe9\x87\x8f\xe8\xb5\xb7\xe6\x95\x88\xef\xbc\x8c\xe5\x80\xbc\xe8\xb6\x8a\xe5\xb0\x8f\xef\xbc\x8c\xe7\xa2\xb0\xe6\x92\x9e\xe7\xb2\xbe\xe5\xba\xa6\xe8\xb6\x8a\xe9\xab\x98\xef\xbc\x8c\xe6\x80\xa7\xe8\x83\xbd\xe6\xb6\x88\xe8\x80\x97\xe8\xb6\x8a\xe5\xa4\xa7\xef\xbc\x81\n" },
#endif
		{ "EditCondition", "bUseSweep" },
		{ "EditConditionHides", "" },
		{ "ModuleRelativePath", "Collision/BXCollision.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe5\x90\xaf\xe7\x94\xa8\xe6\x89\xab\xe6\x8f\x8f \xe4\xb8\x94 \xe8\xb5\xb7\xe5\xa7\x8b\xe6\x9c\x9d\xe5\x90\x91\xe5\x92\x8c\xe7\xbb\x88\xe6\xad\xa2\xe6\x9c\x9d\xe5\x90\x91\xe4\xb8\x8d\xe4\xb8\x80\xe8\x87\xb4\xe6\x97\xb6\xef\xbc\x8c\xe8\xaf\xa5\xe5\x8f\x98\xe9\x87\x8f\xe8\xb5\xb7\xe6\x95\x88\xef\xbc\x8c\xe5\x80\xbc\xe8\xb6\x8a\xe5\xb0\x8f\xef\xbc\x8c\xe7\xa2\xb0\xe6\x92\x9e\xe7\xb2\xbe\xe5\xba\xa6\xe8\xb6\x8a\xe9\xab\x98\xef\xbc\x8c\xe6\x80\xa7\xe8\x83\xbd\xe6\xb6\x88\xe8\x80\x97\xe8\xb6\x8a\xe5\xa4\xa7\xef\xbc\x81" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_CapsuleSize;
	static void NewProp_bUseSweep_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bUseSweep;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_AngleStep;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FBXCSCapsule>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FBXCSCapsule_Statics::NewProp_CapsuleSize = { "CapsuleSize", nullptr, (EPropertyFlags)0x0010000000010001, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXCSCapsule, CapsuleSize), Z_Construct_UScriptStruct_FVector2D, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_CapsuleSize_MetaData), NewProp_CapsuleSize_MetaData) };
void Z_Construct_UScriptStruct_FBXCSCapsule_Statics::NewProp_bUseSweep_SetBit(void* Obj)
{
	((FBXCSCapsule*)Obj)->bUseSweep = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FBXCSCapsule_Statics::NewProp_bUseSweep = { "bUseSweep", nullptr, (EPropertyFlags)0x0010000000010001, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FBXCSCapsule), &Z_Construct_UScriptStruct_FBXCSCapsule_Statics::NewProp_bUseSweep_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bUseSweep_MetaData), NewProp_bUseSweep_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FBXCSCapsule_Statics::NewProp_AngleStep = { "AngleStep", nullptr, (EPropertyFlags)0x0010000000010001, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXCSCapsule, AngleStep), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_AngleStep_MetaData), NewProp_AngleStep_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FBXCSCapsule_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXCSCapsule_Statics::NewProp_CapsuleSize,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXCSCapsule_Statics::NewProp_bUseSweep,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXCSCapsule_Statics::NewProp_AngleStep,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXCSCapsule_Statics::PropPointers) < 2048);
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FBXCSCapsule_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_BattleX,
	Z_Construct_UScriptStruct_FBXCStrategy,
	&NewStructOps,
	"BXCSCapsule",
	Z_Construct_UScriptStruct_FBXCSCapsule_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXCSCapsule_Statics::PropPointers),
	sizeof(FBXCSCapsule),
	alignof(FBXCSCapsule),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000201),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXCSCapsule_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FBXCSCapsule_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FBXCSCapsule()
{
	if (!Z_Registration_Info_UScriptStruct_BXCSCapsule.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_BXCSCapsule.InnerSingleton, Z_Construct_UScriptStruct_FBXCSCapsule_Statics::StructParams);
	}
	return Z_Registration_Info_UScriptStruct_BXCSCapsule.InnerSingleton;
}
// End ScriptStruct FBXCSCapsule

// Begin ScriptStruct FBXCSCylinder
static_assert(std::is_polymorphic<FBXCSCylinder>() == std::is_polymorphic<FBXCStrategy>(), "USTRUCT FBXCSCylinder cannot be polymorphic unless super FBXCStrategy is polymorphic");
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_BXCSCylinder;
class UScriptStruct* FBXCSCylinder::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_BXCSCylinder.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_BXCSCylinder.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FBXCSCylinder, (UObject*)Z_Construct_UPackage__Script_BattleX(), TEXT("BXCSCylinder"));
	}
	return Z_Registration_Info_UScriptStruct_BXCSCylinder.OuterSingleton;
}
template<> BATTLEX_API UScriptStruct* StaticStruct<FBXCSCylinder>()
{
	return FBXCSCylinder::StaticStruct();
}
struct Z_Construct_UScriptStruct_FBXCSCylinder_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe5\x9c\x86\xe6\x9f\xb1\xe4\xbd\x93\xe7\xa2\xb0\xe6\x92\x9e\n" },
#endif
		{ "ModuleRelativePath", "Collision/BXCollision.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe5\x9c\x86\xe6\x9f\xb1\xe4\xbd\x93\xe7\xa2\xb0\xe6\x92\x9e" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_CylinderSize_MetaData[] = {
		{ "Category", "BXCSCylinder" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe5\x9c\x86\xe6\x9f\xb1\xe4\xbd\x93\xe5\xa4\xa7\xe5\xb0\x8f(\xe5\x8d\x8a\xe5\xbe\x84\xe3\x80\x81\xe5\x8d\x8a\xe9\xab\x98)\n" },
#endif
		{ "ModuleRelativePath", "Collision/BXCollision.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe5\x9c\x86\xe6\x9f\xb1\xe4\xbd\x93\xe5\xa4\xa7\xe5\xb0\x8f(\xe5\x8d\x8a\xe5\xbe\x84\xe3\x80\x81\xe5\x8d\x8a\xe9\xab\x98)" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_CylinderSize;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FBXCSCylinder>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FBXCSCylinder_Statics::NewProp_CylinderSize = { "CylinderSize", nullptr, (EPropertyFlags)0x0010000000010001, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXCSCylinder, CylinderSize), Z_Construct_UScriptStruct_FVector2D, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_CylinderSize_MetaData), NewProp_CylinderSize_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FBXCSCylinder_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXCSCylinder_Statics::NewProp_CylinderSize,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXCSCylinder_Statics::PropPointers) < 2048);
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FBXCSCylinder_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_BattleX,
	Z_Construct_UScriptStruct_FBXCStrategy,
	&NewStructOps,
	"BXCSCylinder",
	Z_Construct_UScriptStruct_FBXCSCylinder_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXCSCylinder_Statics::PropPointers),
	sizeof(FBXCSCylinder),
	alignof(FBXCSCylinder),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000201),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXCSCylinder_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FBXCSCylinder_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FBXCSCylinder()
{
	if (!Z_Registration_Info_UScriptStruct_BXCSCylinder.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_BXCSCylinder.InnerSingleton, Z_Construct_UScriptStruct_FBXCSCylinder_Statics::StructParams);
	}
	return Z_Registration_Info_UScriptStruct_BXCSCylinder.InnerSingleton;
}
// End ScriptStruct FBXCSCylinder

// Begin ScriptStruct FBXCSHollowCylinder
static_assert(std::is_polymorphic<FBXCSHollowCylinder>() == std::is_polymorphic<FBXCStrategy>(), "USTRUCT FBXCSHollowCylinder cannot be polymorphic unless super FBXCStrategy is polymorphic");
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_BXCSHollowCylinder;
class UScriptStruct* FBXCSHollowCylinder::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_BXCSHollowCylinder.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_BXCSHollowCylinder.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FBXCSHollowCylinder, (UObject*)Z_Construct_UPackage__Script_BattleX(), TEXT("BXCSHollowCylinder"));
	}
	return Z_Registration_Info_UScriptStruct_BXCSHollowCylinder.OuterSingleton;
}
template<> BATTLEX_API UScriptStruct* StaticStruct<FBXCSHollowCylinder>()
{
	return FBXCSHollowCylinder::StaticStruct();
}
struct Z_Construct_UScriptStruct_FBXCSHollowCylinder_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe7\xa9\xba\xe5\xbf\x83\xe5\x9c\x86\xe6\x9f\xb1\xe4\xbd\x93\xe7\xa2\xb0\xe6\x92\x9e\n" },
#endif
		{ "ModuleRelativePath", "Collision/BXCollision.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe7\xa9\xba\xe5\xbf\x83\xe5\x9c\x86\xe6\x9f\xb1\xe4\xbd\x93\xe7\xa2\xb0\xe6\x92\x9e" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_HollowCylinderSize_MetaData[] = {
		{ "Category", "BXCSHollowCylinder" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe7\xa9\xba\xe5\xbf\x83\xe5\x9c\x86\xe6\x9f\xb1\xe4\xbd\x93\xe5\xa4\xa7\xe5\xb0\x8f(\xe5\x86\x85\xe5\x8d\x8a\xe5\xbe\x84\xe3\x80\x81\xe5\xa4\x96\xe5\x8d\x8a\xe5\xbe\x84\xe3\x80\x81\xe5\x8d\x8a\xe9\xab\x98)\n" },
#endif
		{ "ModuleRelativePath", "Collision/BXCollision.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe7\xa9\xba\xe5\xbf\x83\xe5\x9c\x86\xe6\x9f\xb1\xe4\xbd\x93\xe5\xa4\xa7\xe5\xb0\x8f(\xe5\x86\x85\xe5\x8d\x8a\xe5\xbe\x84\xe3\x80\x81\xe5\xa4\x96\xe5\x8d\x8a\xe5\xbe\x84\xe3\x80\x81\xe5\x8d\x8a\xe9\xab\x98)" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_AngleStep_MetaData[] = {
		{ "Category", "BXCSHollowCylinder" },
		{ "ClampMax", "60.000000" },
		{ "ClampMin", "10.000000" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe8\xa7\x92\xe5\xba\xa6\xe6\xad\xa5\xe8\xbf\x9b\xe8\xb6\x8a\xe5\xb0\x8f\xef\xbc\x8c\xe7\xa2\xb0\xe6\x92\x9e\xe7\xb2\xbe\xe5\xba\xa6\xe8\xb6\x8a\xe9\xab\x98\xef\xbc\x8c\xe6\x80\xa7\xe8\x83\xbd\xe6\xb6\x88\xe8\x80\x97\xe8\xb6\x8a\xe5\xa4\xa7\xef\xbc\x81\n" },
#endif
		{ "ModuleRelativePath", "Collision/BXCollision.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe8\xa7\x92\xe5\xba\xa6\xe6\xad\xa5\xe8\xbf\x9b\xe8\xb6\x8a\xe5\xb0\x8f\xef\xbc\x8c\xe7\xa2\xb0\xe6\x92\x9e\xe7\xb2\xbe\xe5\xba\xa6\xe8\xb6\x8a\xe9\xab\x98\xef\xbc\x8c\xe6\x80\xa7\xe8\x83\xbd\xe6\xb6\x88\xe8\x80\x97\xe8\xb6\x8a\xe5\xa4\xa7\xef\xbc\x81" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_HollowCylinderSize;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_AngleStep;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FBXCSHollowCylinder>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FBXCSHollowCylinder_Statics::NewProp_HollowCylinderSize = { "HollowCylinderSize", nullptr, (EPropertyFlags)0x0010000000010001, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXCSHollowCylinder, HollowCylinderSize), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_HollowCylinderSize_MetaData), NewProp_HollowCylinderSize_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FBXCSHollowCylinder_Statics::NewProp_AngleStep = { "AngleStep", nullptr, (EPropertyFlags)0x0010000000010001, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXCSHollowCylinder, AngleStep), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_AngleStep_MetaData), NewProp_AngleStep_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FBXCSHollowCylinder_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXCSHollowCylinder_Statics::NewProp_HollowCylinderSize,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXCSHollowCylinder_Statics::NewProp_AngleStep,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXCSHollowCylinder_Statics::PropPointers) < 2048);
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FBXCSHollowCylinder_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_BattleX,
	Z_Construct_UScriptStruct_FBXCStrategy,
	&NewStructOps,
	"BXCSHollowCylinder",
	Z_Construct_UScriptStruct_FBXCSHollowCylinder_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXCSHollowCylinder_Statics::PropPointers),
	sizeof(FBXCSHollowCylinder),
	alignof(FBXCSHollowCylinder),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000201),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXCSHollowCylinder_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FBXCSHollowCylinder_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FBXCSHollowCylinder()
{
	if (!Z_Registration_Info_UScriptStruct_BXCSHollowCylinder.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_BXCSHollowCylinder.InnerSingleton, Z_Construct_UScriptStruct_FBXCSHollowCylinder_Statics::StructParams);
	}
	return Z_Registration_Info_UScriptStruct_BXCSHollowCylinder.InnerSingleton;
}
// End ScriptStruct FBXCSHollowCylinder

// Begin ScriptStruct FBXCSBox
static_assert(std::is_polymorphic<FBXCSBox>() == std::is_polymorphic<FBXCStrategy>(), "USTRUCT FBXCSBox cannot be polymorphic unless super FBXCStrategy is polymorphic");
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_BXCSBox;
class UScriptStruct* FBXCSBox::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_BXCSBox.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_BXCSBox.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FBXCSBox, (UObject*)Z_Construct_UPackage__Script_BattleX(), TEXT("BXCSBox"));
	}
	return Z_Registration_Info_UScriptStruct_BXCSBox.OuterSingleton;
}
template<> BATTLEX_API UScriptStruct* StaticStruct<FBXCSBox>()
{
	return FBXCSBox::StaticStruct();
}
struct Z_Construct_UScriptStruct_FBXCSBox_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe9\x95\xbf\xe6\x96\xb9\xe4\xbd\x93\xe7\xa2\xb0\xe6\x92\x9e\n" },
#endif
		{ "ModuleRelativePath", "Collision/BXCollision.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe9\x95\xbf\xe6\x96\xb9\xe4\xbd\x93\xe7\xa2\xb0\xe6\x92\x9e" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_BoxSize_MetaData[] = {
		{ "Category", "BXCSBox" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe9\x95\xbf\xe6\x96\xb9\xe4\xbd\x93\xe5\xa4\xa7\xe5\xb0\x8f(\xe5\x8d\x8a\xe9\x95\xbf\xe3\x80\x81\xe5\x8d\x8a\xe5\xae\xbd\xe3\x80\x81\xe5\x8d\x8a\xe9\xab\x98)\n" },
#endif
		{ "ModuleRelativePath", "Collision/BXCollision.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe9\x95\xbf\xe6\x96\xb9\xe4\xbd\x93\xe5\xa4\xa7\xe5\xb0\x8f(\xe5\x8d\x8a\xe9\x95\xbf\xe3\x80\x81\xe5\x8d\x8a\xe5\xae\xbd\xe3\x80\x81\xe5\x8d\x8a\xe9\xab\x98)" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bUseSweep_MetaData[] = {
		{ "Category", "BXCSBox" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe5\x90\xaf\xe7\x94\xa8\xe6\x89\xab\xe6\x8f\x8f\n" },
#endif
		{ "ModuleRelativePath", "Collision/BXCollision.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe5\x90\xaf\xe7\x94\xa8\xe6\x89\xab\xe6\x8f\x8f" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_AngleStep_MetaData[] = {
		{ "Category", "BXCSBox" },
		{ "ClampMax", "90.000000" },
		{ "ClampMin", "5.000000" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe5\x90\xaf\xe7\x94\xa8\xe6\x89\xab\xe6\x8f\x8f \xe4\xb8\x94 \xe8\xb5\xb7\xe5\xa7\x8b\xe6\x9c\x9d\xe5\x90\x91\xe5\x92\x8c\xe7\xbb\x88\xe6\xad\xa2\xe6\x9c\x9d\xe5\x90\x91\xe4\xb8\x8d\xe4\xb8\x80\xe8\x87\xb4\xe6\x97\xb6\xef\xbc\x8c\xe8\xaf\xa5\xe5\x8f\x98\xe9\x87\x8f\xe8\xb5\xb7\xe6\x95\x88\xef\xbc\x8c\xe5\x80\xbc\xe8\xb6\x8a\xe5\xb0\x8f\xef\xbc\x8c\xe7\xa2\xb0\xe6\x92\x9e\xe7\xb2\xbe\xe5\xba\xa6\xe8\xb6\x8a\xe9\xab\x98\xef\xbc\x8c\xe6\x80\xa7\xe8\x83\xbd\xe6\xb6\x88\xe8\x80\x97\xe8\xb6\x8a\xe5\xa4\xa7\xef\xbc\x81\n" },
#endif
		{ "EditCondition", "bUseSweep" },
		{ "EditConditionHides", "" },
		{ "ModuleRelativePath", "Collision/BXCollision.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe5\x90\xaf\xe7\x94\xa8\xe6\x89\xab\xe6\x8f\x8f \xe4\xb8\x94 \xe8\xb5\xb7\xe5\xa7\x8b\xe6\x9c\x9d\xe5\x90\x91\xe5\x92\x8c\xe7\xbb\x88\xe6\xad\xa2\xe6\x9c\x9d\xe5\x90\x91\xe4\xb8\x8d\xe4\xb8\x80\xe8\x87\xb4\xe6\x97\xb6\xef\xbc\x8c\xe8\xaf\xa5\xe5\x8f\x98\xe9\x87\x8f\xe8\xb5\xb7\xe6\x95\x88\xef\xbc\x8c\xe5\x80\xbc\xe8\xb6\x8a\xe5\xb0\x8f\xef\xbc\x8c\xe7\xa2\xb0\xe6\x92\x9e\xe7\xb2\xbe\xe5\xba\xa6\xe8\xb6\x8a\xe9\xab\x98\xef\xbc\x8c\xe6\x80\xa7\xe8\x83\xbd\xe6\xb6\x88\xe8\x80\x97\xe8\xb6\x8a\xe5\xa4\xa7\xef\xbc\x81" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_BoxSize;
	static void NewProp_bUseSweep_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bUseSweep;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_AngleStep;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FBXCSBox>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FBXCSBox_Statics::NewProp_BoxSize = { "BoxSize", nullptr, (EPropertyFlags)0x0010000000010001, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXCSBox, BoxSize), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_BoxSize_MetaData), NewProp_BoxSize_MetaData) };
void Z_Construct_UScriptStruct_FBXCSBox_Statics::NewProp_bUseSweep_SetBit(void* Obj)
{
	((FBXCSBox*)Obj)->bUseSweep = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FBXCSBox_Statics::NewProp_bUseSweep = { "bUseSweep", nullptr, (EPropertyFlags)0x0010000000010001, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FBXCSBox), &Z_Construct_UScriptStruct_FBXCSBox_Statics::NewProp_bUseSweep_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bUseSweep_MetaData), NewProp_bUseSweep_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FBXCSBox_Statics::NewProp_AngleStep = { "AngleStep", nullptr, (EPropertyFlags)0x0010000000010001, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXCSBox, AngleStep), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_AngleStep_MetaData), NewProp_AngleStep_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FBXCSBox_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXCSBox_Statics::NewProp_BoxSize,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXCSBox_Statics::NewProp_bUseSweep,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXCSBox_Statics::NewProp_AngleStep,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXCSBox_Statics::PropPointers) < 2048);
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FBXCSBox_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_BattleX,
	Z_Construct_UScriptStruct_FBXCStrategy,
	&NewStructOps,
	"BXCSBox",
	Z_Construct_UScriptStruct_FBXCSBox_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXCSBox_Statics::PropPointers),
	sizeof(FBXCSBox),
	alignof(FBXCSBox),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000201),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXCSBox_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FBXCSBox_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FBXCSBox()
{
	if (!Z_Registration_Info_UScriptStruct_BXCSBox.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_BXCSBox.InnerSingleton, Z_Construct_UScriptStruct_FBXCSBox_Statics::StructParams);
	}
	return Z_Registration_Info_UScriptStruct_BXCSBox.InnerSingleton;
}
// End ScriptStruct FBXCSBox

// Begin ScriptStruct FBXCSSector
static_assert(std::is_polymorphic<FBXCSSector>() == std::is_polymorphic<FBXCStrategy>(), "USTRUCT FBXCSSector cannot be polymorphic unless super FBXCStrategy is polymorphic");
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_BXCSSector;
class UScriptStruct* FBXCSSector::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_BXCSSector.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_BXCSSector.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FBXCSSector, (UObject*)Z_Construct_UPackage__Script_BattleX(), TEXT("BXCSSector"));
	}
	return Z_Registration_Info_UScriptStruct_BXCSSector.OuterSingleton;
}
template<> BATTLEX_API UScriptStruct* StaticStruct<FBXCSSector>()
{
	return FBXCSSector::StaticStruct();
}
struct Z_Construct_UScriptStruct_FBXCSSector_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe6\x89\x87\xe6\x9f\xb1\xe4\xbd\x93\xe7\xa2\xb0\xe6\x92\x9e\n" },
#endif
		{ "ModuleRelativePath", "Collision/BXCollision.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe6\x89\x87\xe6\x9f\xb1\xe4\xbd\x93\xe7\xa2\xb0\xe6\x92\x9e" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_SectorInfo_MetaData[] = {
		{ "Category", "BXCSSector" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe6\x89\x87\xe6\x9f\xb1\xe4\xbd\x93\xe7\xa2\xb0\xe6\x92\x9e\xe4\xbf\xa1\xe6\x81\xaf(\xe6\x9c\x80\xe5\xb0\x8f\xe5\x8d\x8a\xe5\xbe\x84\xe3\x80\x81\xe6\x9c\x80\xe5\xa4\xa7\xe5\x8d\x8a\xe5\xbe\x84\xe3\x80\x81\xe5\x8d\x8a\xe9\xab\x98\xe3\x80\x81\xe5\x8d\x8a\xe8\xa7\x92)\n" },
#endif
		{ "ModuleRelativePath", "Collision/BXCollision.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe6\x89\x87\xe6\x9f\xb1\xe4\xbd\x93\xe7\xa2\xb0\xe6\x92\x9e\xe4\xbf\xa1\xe6\x81\xaf(\xe6\x9c\x80\xe5\xb0\x8f\xe5\x8d\x8a\xe5\xbe\x84\xe3\x80\x81\xe6\x9c\x80\xe5\xa4\xa7\xe5\x8d\x8a\xe5\xbe\x84\xe3\x80\x81\xe5\x8d\x8a\xe9\xab\x98\xe3\x80\x81\xe5\x8d\x8a\xe8\xa7\x92)" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Direction_MetaData[] = {
		{ "Category", "BXCSSector" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe6\x89\x87\xe6\x9f\xb1\xe4\xbd\x93\xe7\xa2\xb0\xe6\x92\x9e\xe7\x9a\x84\xe7\xb2\xbe\xe7\xbb\x86\xe6\xa3\x80\xe6\xb5\x8b\xe6\x96\xb9\xe5\x90\x91\n" },
#endif
		{ "ModuleRelativePath", "Collision/BXCollision.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe6\x89\x87\xe6\x9f\xb1\xe4\xbd\x93\xe7\xa2\xb0\xe6\x92\x9e\xe7\x9a\x84\xe7\xb2\xbe\xe7\xbb\x86\xe6\xa3\x80\xe6\xb5\x8b\xe6\x96\xb9\xe5\x90\x91" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_AngleStep_MetaData[] = {
		{ "Category", "BXCSSector" },
		{ "ClampMax", "60.000000" },
		{ "ClampMin", "5.000000" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe8\xa7\x92\xe5\xba\xa6\xe6\xad\xa5\xe8\xbf\x9b\xe8\xb6\x8a\xe5\xb0\x8f\xef\xbc\x8c\xe7\xa2\xb0\xe6\x92\x9e\xe7\xb2\xbe\xe5\xba\xa6\xe8\xb6\x8a\xe9\xab\x98\xef\xbc\x8c\xe6\x80\xa7\xe8\x83\xbd\xe6\xb6\x88\xe8\x80\x97\xe8\xb6\x8a\xe5\xa4\xa7\xef\xbc\x81\n" },
#endif
		{ "ModuleRelativePath", "Collision/BXCollision.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe8\xa7\x92\xe5\xba\xa6\xe6\xad\xa5\xe8\xbf\x9b\xe8\xb6\x8a\xe5\xb0\x8f\xef\xbc\x8c\xe7\xa2\xb0\xe6\x92\x9e\xe7\xb2\xbe\xe5\xba\xa6\xe8\xb6\x8a\xe9\xab\x98\xef\xbc\x8c\xe6\x80\xa7\xe8\x83\xbd\xe6\xb6\x88\xe8\x80\x97\xe8\xb6\x8a\xe5\xa4\xa7\xef\xbc\x81" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_SweepStep_MetaData[] = {
		{ "Category", "BXCSSector" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe6\x89\xab\xe6\x8f\x8f\xe6\xad\xa5\xe8\xbf\x9b\xe6\xac\xa1\xe6\x95\xb0\n" },
#endif
		{ "ModuleRelativePath", "Collision/BXCollision.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe6\x89\xab\xe6\x8f\x8f\xe6\xad\xa5\xe8\xbf\x9b\xe6\xac\xa1\xe6\x95\xb0" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_SectorInfo;
	static const UECodeGen_Private::FBytePropertyParams NewProp_Direction_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_Direction;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_AngleStep;
	static const UECodeGen_Private::FIntPropertyParams NewProp_SweepStep;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FBXCSSector>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FBXCSSector_Statics::NewProp_SectorInfo = { "SectorInfo", nullptr, (EPropertyFlags)0x0010000000010001, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXCSSector, SectorInfo), Z_Construct_UScriptStruct_FVector4, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_SectorInfo_MetaData), NewProp_SectorInfo_MetaData) };
const UECodeGen_Private::FBytePropertyParams Z_Construct_UScriptStruct_FBXCSSector_Statics::NewProp_Direction_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UScriptStruct_FBXCSSector_Statics::NewProp_Direction = { "Direction", nullptr, (EPropertyFlags)0x0010000000010001, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXCSSector, Direction), Z_Construct_UEnum_BattleX_EBXCDirection, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Direction_MetaData), NewProp_Direction_MetaData) }; // 2768710293
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FBXCSSector_Statics::NewProp_AngleStep = { "AngleStep", nullptr, (EPropertyFlags)0x0010000000010001, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXCSSector, AngleStep), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_AngleStep_MetaData), NewProp_AngleStep_MetaData) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UScriptStruct_FBXCSSector_Statics::NewProp_SweepStep = { "SweepStep", nullptr, (EPropertyFlags)0x0010000000010001, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXCSSector, SweepStep), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_SweepStep_MetaData), NewProp_SweepStep_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FBXCSSector_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXCSSector_Statics::NewProp_SectorInfo,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXCSSector_Statics::NewProp_Direction_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXCSSector_Statics::NewProp_Direction,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXCSSector_Statics::NewProp_AngleStep,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXCSSector_Statics::NewProp_SweepStep,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXCSSector_Statics::PropPointers) < 2048);
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FBXCSSector_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_BattleX,
	Z_Construct_UScriptStruct_FBXCStrategy,
	&NewStructOps,
	"BXCSSector",
	Z_Construct_UScriptStruct_FBXCSSector_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXCSSector_Statics::PropPointers),
	sizeof(FBXCSSector),
	alignof(FBXCSSector),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000201),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXCSSector_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FBXCSSector_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FBXCSSector()
{
	if (!Z_Registration_Info_UScriptStruct_BXCSSector.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_BXCSSector.InnerSingleton, Z_Construct_UScriptStruct_FBXCSSector_Statics::StructParams);
	}
	return Z_Registration_Info_UScriptStruct_BXCSSector.InnerSingleton;
}
// End ScriptStruct FBXCSSector

// Begin Class UBXCollisionLibrary Function BoxCheck
struct Z_Construct_UFunction_UBXCollisionLibrary_BoxCheck_Statics
{
	struct BXCollisionLibrary_eventBoxCheck_Parms
	{
		FBXCParameter Parameter;
		TArray<TEnumAsByte<EObjectTypeQuery> > ObjectTypes;
		FVector BoxSize;
		FBXCFilter Filter;
		float AngleStep;
		TArray<FHitResult> ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe9\x95\xbf\xe6\x96\xb9\xe4\xbd\x93\xe7\xa2\xb0\xe6\x92\x9e\xe6\xa3\x80\xe6\x9f\xa5\n" },
#endif
		{ "CPP_Default_AngleStep", "30.000000" },
		{ "ModuleRelativePath", "Collision/BXCollision.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe9\x95\xbf\xe6\x96\xb9\xe4\xbd\x93\xe7\xa2\xb0\xe6\x92\x9e\xe6\xa3\x80\xe6\x9f\xa5" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Parameter_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ObjectTypes_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Filter_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_Parameter;
	static const UECodeGen_Private::FBytePropertyParams NewProp_ObjectTypes_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_ObjectTypes;
	static const UECodeGen_Private::FStructPropertyParams NewProp_BoxSize;
	static const UECodeGen_Private::FStructPropertyParams NewProp_Filter;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_AngleStep;
	static const UECodeGen_Private::FStructPropertyParams NewProp_ReturnValue_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXCollisionLibrary_BoxCheck_Statics::NewProp_Parameter = { "Parameter", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXCollisionLibrary_eventBoxCheck_Parms, Parameter), Z_Construct_UScriptStruct_FBXCParameter, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Parameter_MetaData), NewProp_Parameter_MetaData) }; // 4276197417
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UBXCollisionLibrary_BoxCheck_Statics::NewProp_ObjectTypes_Inner = { "ObjectTypes", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UEnum_Engine_EObjectTypeQuery, METADATA_PARAMS(0, nullptr) }; // 1798967895
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UFunction_UBXCollisionLibrary_BoxCheck_Statics::NewProp_ObjectTypes = { "ObjectTypes", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXCollisionLibrary_eventBoxCheck_Parms, ObjectTypes), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ObjectTypes_MetaData), NewProp_ObjectTypes_MetaData) }; // 1798967895
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXCollisionLibrary_BoxCheck_Statics::NewProp_BoxSize = { "BoxSize", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXCollisionLibrary_eventBoxCheck_Parms, BoxSize), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXCollisionLibrary_BoxCheck_Statics::NewProp_Filter = { "Filter", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXCollisionLibrary_eventBoxCheck_Parms, Filter), Z_Construct_UScriptStruct_FBXCFilter, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Filter_MetaData), NewProp_Filter_MetaData) }; // 366435299
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UBXCollisionLibrary_BoxCheck_Statics::NewProp_AngleStep = { "AngleStep", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXCollisionLibrary_eventBoxCheck_Parms, AngleStep), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXCollisionLibrary_BoxCheck_Statics::NewProp_ReturnValue_Inner = { "ReturnValue", nullptr, (EPropertyFlags)0x0000008000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UScriptStruct_FHitResult, METADATA_PARAMS(0, nullptr) }; // 4100991306
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UFunction_UBXCollisionLibrary_BoxCheck_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010008000000580, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXCollisionLibrary_eventBoxCheck_Parms, ReturnValue), EArrayPropertyFlags::None, METADATA_PARAMS(0, nullptr) }; // 4100991306
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UBXCollisionLibrary_BoxCheck_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXCollisionLibrary_BoxCheck_Statics::NewProp_Parameter,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXCollisionLibrary_BoxCheck_Statics::NewProp_ObjectTypes_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXCollisionLibrary_BoxCheck_Statics::NewProp_ObjectTypes,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXCollisionLibrary_BoxCheck_Statics::NewProp_BoxSize,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXCollisionLibrary_BoxCheck_Statics::NewProp_Filter,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXCollisionLibrary_BoxCheck_Statics::NewProp_AngleStep,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXCollisionLibrary_BoxCheck_Statics::NewProp_ReturnValue_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXCollisionLibrary_BoxCheck_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXCollisionLibrary_BoxCheck_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UBXCollisionLibrary_BoxCheck_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UBXCollisionLibrary, nullptr, "BoxCheck", nullptr, nullptr, Z_Construct_UFunction_UBXCollisionLibrary_BoxCheck_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UBXCollisionLibrary_BoxCheck_Statics::PropPointers), sizeof(Z_Construct_UFunction_UBXCollisionLibrary_BoxCheck_Statics::BXCollisionLibrary_eventBoxCheck_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04C22401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXCollisionLibrary_BoxCheck_Statics::Function_MetaDataParams), Z_Construct_UFunction_UBXCollisionLibrary_BoxCheck_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UBXCollisionLibrary_BoxCheck_Statics::BXCollisionLibrary_eventBoxCheck_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UBXCollisionLibrary_BoxCheck()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UBXCollisionLibrary_BoxCheck_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UBXCollisionLibrary::execBoxCheck)
{
	P_GET_STRUCT_REF(FBXCParameter,Z_Param_Out_Parameter);
	P_GET_TARRAY_REF(TEnumAsByte<EObjectTypeQuery>,Z_Param_Out_ObjectTypes);
	P_GET_STRUCT(FVector,Z_Param_BoxSize);
	P_GET_STRUCT_REF(FBXCFilter,Z_Param_Out_Filter);
	P_GET_PROPERTY(FFloatProperty,Z_Param_AngleStep);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(TArray<FHitResult>*)Z_Param__Result=UBXCollisionLibrary::BoxCheck(Z_Param_Out_Parameter,Z_Param_Out_ObjectTypes,Z_Param_BoxSize,Z_Param_Out_Filter,Z_Param_AngleStep);
	P_NATIVE_END;
}
// End Class UBXCollisionLibrary Function BoxCheck

// Begin Class UBXCollisionLibrary Function CapsuleCheck
struct Z_Construct_UFunction_UBXCollisionLibrary_CapsuleCheck_Statics
{
	struct BXCollisionLibrary_eventCapsuleCheck_Parms
	{
		FBXCParameter Parameter;
		TArray<TEnumAsByte<EObjectTypeQuery> > ObjectTypes;
		FVector2D CapsuleSize;
		FBXCFilter Filter;
		float AngleStep;
		TArray<FHitResult> ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe8\x83\xb6\xe5\x9b\x8a\xe4\xbd\x93\xe7\xa2\xb0\xe6\x92\x9e\xe6\xa3\x80\xe6\x9f\xa5\n" },
#endif
		{ "CPP_Default_AngleStep", "30.000000" },
		{ "ModuleRelativePath", "Collision/BXCollision.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe8\x83\xb6\xe5\x9b\x8a\xe4\xbd\x93\xe7\xa2\xb0\xe6\x92\x9e\xe6\xa3\x80\xe6\x9f\xa5" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Parameter_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ObjectTypes_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Filter_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_Parameter;
	static const UECodeGen_Private::FBytePropertyParams NewProp_ObjectTypes_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_ObjectTypes;
	static const UECodeGen_Private::FStructPropertyParams NewProp_CapsuleSize;
	static const UECodeGen_Private::FStructPropertyParams NewProp_Filter;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_AngleStep;
	static const UECodeGen_Private::FStructPropertyParams NewProp_ReturnValue_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXCollisionLibrary_CapsuleCheck_Statics::NewProp_Parameter = { "Parameter", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXCollisionLibrary_eventCapsuleCheck_Parms, Parameter), Z_Construct_UScriptStruct_FBXCParameter, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Parameter_MetaData), NewProp_Parameter_MetaData) }; // 4276197417
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UBXCollisionLibrary_CapsuleCheck_Statics::NewProp_ObjectTypes_Inner = { "ObjectTypes", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UEnum_Engine_EObjectTypeQuery, METADATA_PARAMS(0, nullptr) }; // 1798967895
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UFunction_UBXCollisionLibrary_CapsuleCheck_Statics::NewProp_ObjectTypes = { "ObjectTypes", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXCollisionLibrary_eventCapsuleCheck_Parms, ObjectTypes), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ObjectTypes_MetaData), NewProp_ObjectTypes_MetaData) }; // 1798967895
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXCollisionLibrary_CapsuleCheck_Statics::NewProp_CapsuleSize = { "CapsuleSize", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXCollisionLibrary_eventCapsuleCheck_Parms, CapsuleSize), Z_Construct_UScriptStruct_FVector2D, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXCollisionLibrary_CapsuleCheck_Statics::NewProp_Filter = { "Filter", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXCollisionLibrary_eventCapsuleCheck_Parms, Filter), Z_Construct_UScriptStruct_FBXCFilter, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Filter_MetaData), NewProp_Filter_MetaData) }; // 366435299
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UBXCollisionLibrary_CapsuleCheck_Statics::NewProp_AngleStep = { "AngleStep", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXCollisionLibrary_eventCapsuleCheck_Parms, AngleStep), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXCollisionLibrary_CapsuleCheck_Statics::NewProp_ReturnValue_Inner = { "ReturnValue", nullptr, (EPropertyFlags)0x0000008000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UScriptStruct_FHitResult, METADATA_PARAMS(0, nullptr) }; // 4100991306
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UFunction_UBXCollisionLibrary_CapsuleCheck_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010008000000580, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXCollisionLibrary_eventCapsuleCheck_Parms, ReturnValue), EArrayPropertyFlags::None, METADATA_PARAMS(0, nullptr) }; // 4100991306
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UBXCollisionLibrary_CapsuleCheck_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXCollisionLibrary_CapsuleCheck_Statics::NewProp_Parameter,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXCollisionLibrary_CapsuleCheck_Statics::NewProp_ObjectTypes_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXCollisionLibrary_CapsuleCheck_Statics::NewProp_ObjectTypes,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXCollisionLibrary_CapsuleCheck_Statics::NewProp_CapsuleSize,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXCollisionLibrary_CapsuleCheck_Statics::NewProp_Filter,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXCollisionLibrary_CapsuleCheck_Statics::NewProp_AngleStep,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXCollisionLibrary_CapsuleCheck_Statics::NewProp_ReturnValue_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXCollisionLibrary_CapsuleCheck_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXCollisionLibrary_CapsuleCheck_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UBXCollisionLibrary_CapsuleCheck_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UBXCollisionLibrary, nullptr, "CapsuleCheck", nullptr, nullptr, Z_Construct_UFunction_UBXCollisionLibrary_CapsuleCheck_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UBXCollisionLibrary_CapsuleCheck_Statics::PropPointers), sizeof(Z_Construct_UFunction_UBXCollisionLibrary_CapsuleCheck_Statics::BXCollisionLibrary_eventCapsuleCheck_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04C22401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXCollisionLibrary_CapsuleCheck_Statics::Function_MetaDataParams), Z_Construct_UFunction_UBXCollisionLibrary_CapsuleCheck_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UBXCollisionLibrary_CapsuleCheck_Statics::BXCollisionLibrary_eventCapsuleCheck_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UBXCollisionLibrary_CapsuleCheck()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UBXCollisionLibrary_CapsuleCheck_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UBXCollisionLibrary::execCapsuleCheck)
{
	P_GET_STRUCT_REF(FBXCParameter,Z_Param_Out_Parameter);
	P_GET_TARRAY_REF(TEnumAsByte<EObjectTypeQuery>,Z_Param_Out_ObjectTypes);
	P_GET_STRUCT(FVector2D,Z_Param_CapsuleSize);
	P_GET_STRUCT_REF(FBXCFilter,Z_Param_Out_Filter);
	P_GET_PROPERTY(FFloatProperty,Z_Param_AngleStep);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(TArray<FHitResult>*)Z_Param__Result=UBXCollisionLibrary::CapsuleCheck(Z_Param_Out_Parameter,Z_Param_Out_ObjectTypes,Z_Param_CapsuleSize,Z_Param_Out_Filter,Z_Param_AngleStep);
	P_NATIVE_END;
}
// End Class UBXCollisionLibrary Function CapsuleCheck

// Begin Class UBXCollisionLibrary Function CheckCollisionResult
struct Z_Construct_UFunction_UBXCollisionLibrary_CheckCollisionResult_Statics
{
	struct BXCollisionLibrary_eventCheckCollisionResult_Parms
	{
		AActor* InRequester;
		FHitResult InResult;
		FBXCFilter InFilter;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe6\xa3\x80\xe6\x9f\xa5\xe7\xa2\xb0\xe6\x92\x9e\xe7\xbb\x93\xe6\x9e\x9c\n" },
#endif
		{ "ModuleRelativePath", "Collision/BXCollision.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe6\xa3\x80\xe6\x9f\xa5\xe7\xa2\xb0\xe6\x92\x9e\xe7\xbb\x93\xe6\x9e\x9c" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InResult_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InFilter_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_InRequester;
	static const UECodeGen_Private::FStructPropertyParams NewProp_InResult;
	static const UECodeGen_Private::FStructPropertyParams NewProp_InFilter;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UBXCollisionLibrary_CheckCollisionResult_Statics::NewProp_InRequester = { "InRequester", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXCollisionLibrary_eventCheckCollisionResult_Parms, InRequester), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXCollisionLibrary_CheckCollisionResult_Statics::NewProp_InResult = { "InResult", nullptr, (EPropertyFlags)0x0010008008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXCollisionLibrary_eventCheckCollisionResult_Parms, InResult), Z_Construct_UScriptStruct_FHitResult, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InResult_MetaData), NewProp_InResult_MetaData) }; // 4100991306
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXCollisionLibrary_CheckCollisionResult_Statics::NewProp_InFilter = { "InFilter", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXCollisionLibrary_eventCheckCollisionResult_Parms, InFilter), Z_Construct_UScriptStruct_FBXCFilter, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InFilter_MetaData), NewProp_InFilter_MetaData) }; // 366435299
void Z_Construct_UFunction_UBXCollisionLibrary_CheckCollisionResult_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((BXCollisionLibrary_eventCheckCollisionResult_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UBXCollisionLibrary_CheckCollisionResult_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(BXCollisionLibrary_eventCheckCollisionResult_Parms), &Z_Construct_UFunction_UBXCollisionLibrary_CheckCollisionResult_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UBXCollisionLibrary_CheckCollisionResult_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXCollisionLibrary_CheckCollisionResult_Statics::NewProp_InRequester,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXCollisionLibrary_CheckCollisionResult_Statics::NewProp_InResult,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXCollisionLibrary_CheckCollisionResult_Statics::NewProp_InFilter,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXCollisionLibrary_CheckCollisionResult_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXCollisionLibrary_CheckCollisionResult_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UBXCollisionLibrary_CheckCollisionResult_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UBXCollisionLibrary, nullptr, "CheckCollisionResult", nullptr, nullptr, Z_Construct_UFunction_UBXCollisionLibrary_CheckCollisionResult_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UBXCollisionLibrary_CheckCollisionResult_Statics::PropPointers), sizeof(Z_Construct_UFunction_UBXCollisionLibrary_CheckCollisionResult_Statics::BXCollisionLibrary_eventCheckCollisionResult_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04422401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXCollisionLibrary_CheckCollisionResult_Statics::Function_MetaDataParams), Z_Construct_UFunction_UBXCollisionLibrary_CheckCollisionResult_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UBXCollisionLibrary_CheckCollisionResult_Statics::BXCollisionLibrary_eventCheckCollisionResult_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UBXCollisionLibrary_CheckCollisionResult()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UBXCollisionLibrary_CheckCollisionResult_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UBXCollisionLibrary::execCheckCollisionResult)
{
	P_GET_OBJECT(AActor,Z_Param_InRequester);
	P_GET_STRUCT_REF(FHitResult,Z_Param_Out_InResult);
	P_GET_STRUCT_REF(FBXCFilter,Z_Param_Out_InFilter);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=UBXCollisionLibrary::CheckCollisionResult(Z_Param_InRequester,Z_Param_Out_InResult,Z_Param_Out_InFilter);
	P_NATIVE_END;
}
// End Class UBXCollisionLibrary Function CheckCollisionResult

// Begin Class UBXCollisionLibrary Function CylinderCheck
struct Z_Construct_UFunction_UBXCollisionLibrary_CylinderCheck_Statics
{
	struct BXCollisionLibrary_eventCylinderCheck_Parms
	{
		FBXCParameter Parameter;
		TArray<TEnumAsByte<EObjectTypeQuery> > ObjectTypes;
		FVector2D CylinderSize;
		FBXCFilter Filter;
		TArray<FHitResult> ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe5\x9c\x86\xe6\x9f\xb1\xe4\xbd\x93\xe7\xa2\xb0\xe6\x92\x9e\xe6\xa3\x80\xe6\x9f\xa5\n" },
#endif
		{ "ModuleRelativePath", "Collision/BXCollision.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe5\x9c\x86\xe6\x9f\xb1\xe4\xbd\x93\xe7\xa2\xb0\xe6\x92\x9e\xe6\xa3\x80\xe6\x9f\xa5" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Parameter_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ObjectTypes_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Filter_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_Parameter;
	static const UECodeGen_Private::FBytePropertyParams NewProp_ObjectTypes_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_ObjectTypes;
	static const UECodeGen_Private::FStructPropertyParams NewProp_CylinderSize;
	static const UECodeGen_Private::FStructPropertyParams NewProp_Filter;
	static const UECodeGen_Private::FStructPropertyParams NewProp_ReturnValue_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXCollisionLibrary_CylinderCheck_Statics::NewProp_Parameter = { "Parameter", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXCollisionLibrary_eventCylinderCheck_Parms, Parameter), Z_Construct_UScriptStruct_FBXCParameter, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Parameter_MetaData), NewProp_Parameter_MetaData) }; // 4276197417
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UBXCollisionLibrary_CylinderCheck_Statics::NewProp_ObjectTypes_Inner = { "ObjectTypes", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UEnum_Engine_EObjectTypeQuery, METADATA_PARAMS(0, nullptr) }; // 1798967895
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UFunction_UBXCollisionLibrary_CylinderCheck_Statics::NewProp_ObjectTypes = { "ObjectTypes", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXCollisionLibrary_eventCylinderCheck_Parms, ObjectTypes), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ObjectTypes_MetaData), NewProp_ObjectTypes_MetaData) }; // 1798967895
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXCollisionLibrary_CylinderCheck_Statics::NewProp_CylinderSize = { "CylinderSize", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXCollisionLibrary_eventCylinderCheck_Parms, CylinderSize), Z_Construct_UScriptStruct_FVector2D, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXCollisionLibrary_CylinderCheck_Statics::NewProp_Filter = { "Filter", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXCollisionLibrary_eventCylinderCheck_Parms, Filter), Z_Construct_UScriptStruct_FBXCFilter, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Filter_MetaData), NewProp_Filter_MetaData) }; // 366435299
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXCollisionLibrary_CylinderCheck_Statics::NewProp_ReturnValue_Inner = { "ReturnValue", nullptr, (EPropertyFlags)0x0000008000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UScriptStruct_FHitResult, METADATA_PARAMS(0, nullptr) }; // 4100991306
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UFunction_UBXCollisionLibrary_CylinderCheck_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010008000000580, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXCollisionLibrary_eventCylinderCheck_Parms, ReturnValue), EArrayPropertyFlags::None, METADATA_PARAMS(0, nullptr) }; // 4100991306
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UBXCollisionLibrary_CylinderCheck_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXCollisionLibrary_CylinderCheck_Statics::NewProp_Parameter,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXCollisionLibrary_CylinderCheck_Statics::NewProp_ObjectTypes_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXCollisionLibrary_CylinderCheck_Statics::NewProp_ObjectTypes,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXCollisionLibrary_CylinderCheck_Statics::NewProp_CylinderSize,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXCollisionLibrary_CylinderCheck_Statics::NewProp_Filter,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXCollisionLibrary_CylinderCheck_Statics::NewProp_ReturnValue_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXCollisionLibrary_CylinderCheck_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXCollisionLibrary_CylinderCheck_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UBXCollisionLibrary_CylinderCheck_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UBXCollisionLibrary, nullptr, "CylinderCheck", nullptr, nullptr, Z_Construct_UFunction_UBXCollisionLibrary_CylinderCheck_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UBXCollisionLibrary_CylinderCheck_Statics::PropPointers), sizeof(Z_Construct_UFunction_UBXCollisionLibrary_CylinderCheck_Statics::BXCollisionLibrary_eventCylinderCheck_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04C22401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXCollisionLibrary_CylinderCheck_Statics::Function_MetaDataParams), Z_Construct_UFunction_UBXCollisionLibrary_CylinderCheck_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UBXCollisionLibrary_CylinderCheck_Statics::BXCollisionLibrary_eventCylinderCheck_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UBXCollisionLibrary_CylinderCheck()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UBXCollisionLibrary_CylinderCheck_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UBXCollisionLibrary::execCylinderCheck)
{
	P_GET_STRUCT_REF(FBXCParameter,Z_Param_Out_Parameter);
	P_GET_TARRAY_REF(TEnumAsByte<EObjectTypeQuery>,Z_Param_Out_ObjectTypes);
	P_GET_STRUCT(FVector2D,Z_Param_CylinderSize);
	P_GET_STRUCT_REF(FBXCFilter,Z_Param_Out_Filter);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(TArray<FHitResult>*)Z_Param__Result=UBXCollisionLibrary::CylinderCheck(Z_Param_Out_Parameter,Z_Param_Out_ObjectTypes,Z_Param_CylinderSize,Z_Param_Out_Filter);
	P_NATIVE_END;
}
// End Class UBXCollisionLibrary Function CylinderCheck

// Begin Class UBXCollisionLibrary Function HollowCylinderCheck
struct Z_Construct_UFunction_UBXCollisionLibrary_HollowCylinderCheck_Statics
{
	struct BXCollisionLibrary_eventHollowCylinderCheck_Parms
	{
		FBXCParameter Parameter;
		TArray<TEnumAsByte<EObjectTypeQuery> > ObjectTypes;
		FVector HollowCylinderSize;
		FBXCFilter Filter;
		float AngleStep;
		TArray<FHitResult> ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe7\xa9\xba\xe5\xbf\x83\xe5\x9c\x86\xe6\x9f\xb1\xe4\xbd\x93\xe7\xa2\xb0\xe6\x92\x9e\xe6\xa3\x80\xe6\x9f\xa5\n" },
#endif
		{ "CPP_Default_AngleStep", "30.000000" },
		{ "ModuleRelativePath", "Collision/BXCollision.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe7\xa9\xba\xe5\xbf\x83\xe5\x9c\x86\xe6\x9f\xb1\xe4\xbd\x93\xe7\xa2\xb0\xe6\x92\x9e\xe6\xa3\x80\xe6\x9f\xa5" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Parameter_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ObjectTypes_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Filter_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_Parameter;
	static const UECodeGen_Private::FBytePropertyParams NewProp_ObjectTypes_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_ObjectTypes;
	static const UECodeGen_Private::FStructPropertyParams NewProp_HollowCylinderSize;
	static const UECodeGen_Private::FStructPropertyParams NewProp_Filter;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_AngleStep;
	static const UECodeGen_Private::FStructPropertyParams NewProp_ReturnValue_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXCollisionLibrary_HollowCylinderCheck_Statics::NewProp_Parameter = { "Parameter", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXCollisionLibrary_eventHollowCylinderCheck_Parms, Parameter), Z_Construct_UScriptStruct_FBXCParameter, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Parameter_MetaData), NewProp_Parameter_MetaData) }; // 4276197417
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UBXCollisionLibrary_HollowCylinderCheck_Statics::NewProp_ObjectTypes_Inner = { "ObjectTypes", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UEnum_Engine_EObjectTypeQuery, METADATA_PARAMS(0, nullptr) }; // 1798967895
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UFunction_UBXCollisionLibrary_HollowCylinderCheck_Statics::NewProp_ObjectTypes = { "ObjectTypes", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXCollisionLibrary_eventHollowCylinderCheck_Parms, ObjectTypes), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ObjectTypes_MetaData), NewProp_ObjectTypes_MetaData) }; // 1798967895
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXCollisionLibrary_HollowCylinderCheck_Statics::NewProp_HollowCylinderSize = { "HollowCylinderSize", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXCollisionLibrary_eventHollowCylinderCheck_Parms, HollowCylinderSize), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXCollisionLibrary_HollowCylinderCheck_Statics::NewProp_Filter = { "Filter", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXCollisionLibrary_eventHollowCylinderCheck_Parms, Filter), Z_Construct_UScriptStruct_FBXCFilter, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Filter_MetaData), NewProp_Filter_MetaData) }; // 366435299
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UBXCollisionLibrary_HollowCylinderCheck_Statics::NewProp_AngleStep = { "AngleStep", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXCollisionLibrary_eventHollowCylinderCheck_Parms, AngleStep), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXCollisionLibrary_HollowCylinderCheck_Statics::NewProp_ReturnValue_Inner = { "ReturnValue", nullptr, (EPropertyFlags)0x0000008000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UScriptStruct_FHitResult, METADATA_PARAMS(0, nullptr) }; // 4100991306
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UFunction_UBXCollisionLibrary_HollowCylinderCheck_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010008000000580, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXCollisionLibrary_eventHollowCylinderCheck_Parms, ReturnValue), EArrayPropertyFlags::None, METADATA_PARAMS(0, nullptr) }; // 4100991306
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UBXCollisionLibrary_HollowCylinderCheck_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXCollisionLibrary_HollowCylinderCheck_Statics::NewProp_Parameter,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXCollisionLibrary_HollowCylinderCheck_Statics::NewProp_ObjectTypes_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXCollisionLibrary_HollowCylinderCheck_Statics::NewProp_ObjectTypes,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXCollisionLibrary_HollowCylinderCheck_Statics::NewProp_HollowCylinderSize,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXCollisionLibrary_HollowCylinderCheck_Statics::NewProp_Filter,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXCollisionLibrary_HollowCylinderCheck_Statics::NewProp_AngleStep,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXCollisionLibrary_HollowCylinderCheck_Statics::NewProp_ReturnValue_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXCollisionLibrary_HollowCylinderCheck_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXCollisionLibrary_HollowCylinderCheck_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UBXCollisionLibrary_HollowCylinderCheck_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UBXCollisionLibrary, nullptr, "HollowCylinderCheck", nullptr, nullptr, Z_Construct_UFunction_UBXCollisionLibrary_HollowCylinderCheck_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UBXCollisionLibrary_HollowCylinderCheck_Statics::PropPointers), sizeof(Z_Construct_UFunction_UBXCollisionLibrary_HollowCylinderCheck_Statics::BXCollisionLibrary_eventHollowCylinderCheck_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04C22401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXCollisionLibrary_HollowCylinderCheck_Statics::Function_MetaDataParams), Z_Construct_UFunction_UBXCollisionLibrary_HollowCylinderCheck_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UBXCollisionLibrary_HollowCylinderCheck_Statics::BXCollisionLibrary_eventHollowCylinderCheck_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UBXCollisionLibrary_HollowCylinderCheck()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UBXCollisionLibrary_HollowCylinderCheck_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UBXCollisionLibrary::execHollowCylinderCheck)
{
	P_GET_STRUCT_REF(FBXCParameter,Z_Param_Out_Parameter);
	P_GET_TARRAY_REF(TEnumAsByte<EObjectTypeQuery>,Z_Param_Out_ObjectTypes);
	P_GET_STRUCT(FVector,Z_Param_HollowCylinderSize);
	P_GET_STRUCT_REF(FBXCFilter,Z_Param_Out_Filter);
	P_GET_PROPERTY(FFloatProperty,Z_Param_AngleStep);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(TArray<FHitResult>*)Z_Param__Result=UBXCollisionLibrary::HollowCylinderCheck(Z_Param_Out_Parameter,Z_Param_Out_ObjectTypes,Z_Param_HollowCylinderSize,Z_Param_Out_Filter,Z_Param_AngleStep);
	P_NATIVE_END;
}
// End Class UBXCollisionLibrary Function HollowCylinderCheck

// Begin Class UBXCollisionLibrary Function SectorCheck
struct Z_Construct_UFunction_UBXCollisionLibrary_SectorCheck_Statics
{
	struct BXCollisionLibrary_eventSectorCheck_Parms
	{
		FBXCParameter Parameter;
		TArray<TEnumAsByte<EObjectTypeQuery> > ObjectTypes;
		FVector4 SectorSize;
		EBXCDirection Direction;
		FBXCFilter Filter;
		float AngleStep;
		int32 SweepStep;
		TArray<FHitResult> ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe6\x89\x87\xe6\x9f\xb1\xe4\xbd\x93\xe7\xa2\xb0\xe6\x92\x9e\xe6\xa3\x80\xe6\x9f\xa5\n" },
#endif
		{ "CPP_Default_AngleStep", "30.000000" },
		{ "CPP_Default_SweepStep", "10" },
		{ "ModuleRelativePath", "Collision/BXCollision.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe6\x89\x87\xe6\x9f\xb1\xe4\xbd\x93\xe7\xa2\xb0\xe6\x92\x9e\xe6\xa3\x80\xe6\x9f\xa5" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Parameter_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ObjectTypes_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Filter_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_Parameter;
	static const UECodeGen_Private::FBytePropertyParams NewProp_ObjectTypes_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_ObjectTypes;
	static const UECodeGen_Private::FStructPropertyParams NewProp_SectorSize;
	static const UECodeGen_Private::FBytePropertyParams NewProp_Direction_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_Direction;
	static const UECodeGen_Private::FStructPropertyParams NewProp_Filter;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_AngleStep;
	static const UECodeGen_Private::FIntPropertyParams NewProp_SweepStep;
	static const UECodeGen_Private::FStructPropertyParams NewProp_ReturnValue_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXCollisionLibrary_SectorCheck_Statics::NewProp_Parameter = { "Parameter", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXCollisionLibrary_eventSectorCheck_Parms, Parameter), Z_Construct_UScriptStruct_FBXCParameter, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Parameter_MetaData), NewProp_Parameter_MetaData) }; // 4276197417
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UBXCollisionLibrary_SectorCheck_Statics::NewProp_ObjectTypes_Inner = { "ObjectTypes", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UEnum_Engine_EObjectTypeQuery, METADATA_PARAMS(0, nullptr) }; // 1798967895
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UFunction_UBXCollisionLibrary_SectorCheck_Statics::NewProp_ObjectTypes = { "ObjectTypes", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXCollisionLibrary_eventSectorCheck_Parms, ObjectTypes), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ObjectTypes_MetaData), NewProp_ObjectTypes_MetaData) }; // 1798967895
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXCollisionLibrary_SectorCheck_Statics::NewProp_SectorSize = { "SectorSize", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXCollisionLibrary_eventSectorCheck_Parms, SectorSize), Z_Construct_UScriptStruct_FVector4, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UBXCollisionLibrary_SectorCheck_Statics::NewProp_Direction_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UBXCollisionLibrary_SectorCheck_Statics::NewProp_Direction = { "Direction", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXCollisionLibrary_eventSectorCheck_Parms, Direction), Z_Construct_UEnum_BattleX_EBXCDirection, METADATA_PARAMS(0, nullptr) }; // 2768710293
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXCollisionLibrary_SectorCheck_Statics::NewProp_Filter = { "Filter", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXCollisionLibrary_eventSectorCheck_Parms, Filter), Z_Construct_UScriptStruct_FBXCFilter, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Filter_MetaData), NewProp_Filter_MetaData) }; // 366435299
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UBXCollisionLibrary_SectorCheck_Statics::NewProp_AngleStep = { "AngleStep", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXCollisionLibrary_eventSectorCheck_Parms, AngleStep), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UBXCollisionLibrary_SectorCheck_Statics::NewProp_SweepStep = { "SweepStep", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXCollisionLibrary_eventSectorCheck_Parms, SweepStep), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXCollisionLibrary_SectorCheck_Statics::NewProp_ReturnValue_Inner = { "ReturnValue", nullptr, (EPropertyFlags)0x0000008000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UScriptStruct_FHitResult, METADATA_PARAMS(0, nullptr) }; // 4100991306
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UFunction_UBXCollisionLibrary_SectorCheck_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010008000000580, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXCollisionLibrary_eventSectorCheck_Parms, ReturnValue), EArrayPropertyFlags::None, METADATA_PARAMS(0, nullptr) }; // 4100991306
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UBXCollisionLibrary_SectorCheck_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXCollisionLibrary_SectorCheck_Statics::NewProp_Parameter,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXCollisionLibrary_SectorCheck_Statics::NewProp_ObjectTypes_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXCollisionLibrary_SectorCheck_Statics::NewProp_ObjectTypes,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXCollisionLibrary_SectorCheck_Statics::NewProp_SectorSize,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXCollisionLibrary_SectorCheck_Statics::NewProp_Direction_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXCollisionLibrary_SectorCheck_Statics::NewProp_Direction,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXCollisionLibrary_SectorCheck_Statics::NewProp_Filter,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXCollisionLibrary_SectorCheck_Statics::NewProp_AngleStep,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXCollisionLibrary_SectorCheck_Statics::NewProp_SweepStep,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXCollisionLibrary_SectorCheck_Statics::NewProp_ReturnValue_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXCollisionLibrary_SectorCheck_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXCollisionLibrary_SectorCheck_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UBXCollisionLibrary_SectorCheck_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UBXCollisionLibrary, nullptr, "SectorCheck", nullptr, nullptr, Z_Construct_UFunction_UBXCollisionLibrary_SectorCheck_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UBXCollisionLibrary_SectorCheck_Statics::PropPointers), sizeof(Z_Construct_UFunction_UBXCollisionLibrary_SectorCheck_Statics::BXCollisionLibrary_eventSectorCheck_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04C22401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXCollisionLibrary_SectorCheck_Statics::Function_MetaDataParams), Z_Construct_UFunction_UBXCollisionLibrary_SectorCheck_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UBXCollisionLibrary_SectorCheck_Statics::BXCollisionLibrary_eventSectorCheck_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UBXCollisionLibrary_SectorCheck()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UBXCollisionLibrary_SectorCheck_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UBXCollisionLibrary::execSectorCheck)
{
	P_GET_STRUCT_REF(FBXCParameter,Z_Param_Out_Parameter);
	P_GET_TARRAY_REF(TEnumAsByte<EObjectTypeQuery>,Z_Param_Out_ObjectTypes);
	P_GET_STRUCT(FVector4,Z_Param_SectorSize);
	P_GET_ENUM(EBXCDirection,Z_Param_Direction);
	P_GET_STRUCT_REF(FBXCFilter,Z_Param_Out_Filter);
	P_GET_PROPERTY(FFloatProperty,Z_Param_AngleStep);
	P_GET_PROPERTY(FIntProperty,Z_Param_SweepStep);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(TArray<FHitResult>*)Z_Param__Result=UBXCollisionLibrary::SectorCheck(Z_Param_Out_Parameter,Z_Param_Out_ObjectTypes,Z_Param_SectorSize,EBXCDirection(Z_Param_Direction),Z_Param_Out_Filter,Z_Param_AngleStep,Z_Param_SweepStep);
	P_NATIVE_END;
}
// End Class UBXCollisionLibrary Function SectorCheck

// Begin Class UBXCollisionLibrary Function SphereCheck
struct Z_Construct_UFunction_UBXCollisionLibrary_SphereCheck_Statics
{
	struct BXCollisionLibrary_eventSphereCheck_Parms
	{
		FBXCParameter Parameter;
		TArray<TEnumAsByte<EObjectTypeQuery> > ObjectTypes;
		float SpereSize;
		FBXCFilter Filter;
		TArray<FHitResult> ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe7\x90\x83\xe4\xbd\x93\xe7\xa2\xb0\xe6\x92\x9e\xe6\xa3\x80\xe6\x9f\xa5\n" },
#endif
		{ "ModuleRelativePath", "Collision/BXCollision.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe7\x90\x83\xe4\xbd\x93\xe7\xa2\xb0\xe6\x92\x9e\xe6\xa3\x80\xe6\x9f\xa5" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Parameter_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ObjectTypes_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Filter_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_Parameter;
	static const UECodeGen_Private::FBytePropertyParams NewProp_ObjectTypes_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_ObjectTypes;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_SpereSize;
	static const UECodeGen_Private::FStructPropertyParams NewProp_Filter;
	static const UECodeGen_Private::FStructPropertyParams NewProp_ReturnValue_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXCollisionLibrary_SphereCheck_Statics::NewProp_Parameter = { "Parameter", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXCollisionLibrary_eventSphereCheck_Parms, Parameter), Z_Construct_UScriptStruct_FBXCParameter, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Parameter_MetaData), NewProp_Parameter_MetaData) }; // 4276197417
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UBXCollisionLibrary_SphereCheck_Statics::NewProp_ObjectTypes_Inner = { "ObjectTypes", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UEnum_Engine_EObjectTypeQuery, METADATA_PARAMS(0, nullptr) }; // 1798967895
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UFunction_UBXCollisionLibrary_SphereCheck_Statics::NewProp_ObjectTypes = { "ObjectTypes", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXCollisionLibrary_eventSphereCheck_Parms, ObjectTypes), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ObjectTypes_MetaData), NewProp_ObjectTypes_MetaData) }; // 1798967895
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UBXCollisionLibrary_SphereCheck_Statics::NewProp_SpereSize = { "SpereSize", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXCollisionLibrary_eventSphereCheck_Parms, SpereSize), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXCollisionLibrary_SphereCheck_Statics::NewProp_Filter = { "Filter", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXCollisionLibrary_eventSphereCheck_Parms, Filter), Z_Construct_UScriptStruct_FBXCFilter, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Filter_MetaData), NewProp_Filter_MetaData) }; // 366435299
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UBXCollisionLibrary_SphereCheck_Statics::NewProp_ReturnValue_Inner = { "ReturnValue", nullptr, (EPropertyFlags)0x0000008000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UScriptStruct_FHitResult, METADATA_PARAMS(0, nullptr) }; // 4100991306
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UFunction_UBXCollisionLibrary_SphereCheck_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010008000000580, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BXCollisionLibrary_eventSphereCheck_Parms, ReturnValue), EArrayPropertyFlags::None, METADATA_PARAMS(0, nullptr) }; // 4100991306
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UBXCollisionLibrary_SphereCheck_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXCollisionLibrary_SphereCheck_Statics::NewProp_Parameter,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXCollisionLibrary_SphereCheck_Statics::NewProp_ObjectTypes_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXCollisionLibrary_SphereCheck_Statics::NewProp_ObjectTypes,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXCollisionLibrary_SphereCheck_Statics::NewProp_SpereSize,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXCollisionLibrary_SphereCheck_Statics::NewProp_Filter,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXCollisionLibrary_SphereCheck_Statics::NewProp_ReturnValue_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBXCollisionLibrary_SphereCheck_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXCollisionLibrary_SphereCheck_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UBXCollisionLibrary_SphereCheck_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UBXCollisionLibrary, nullptr, "SphereCheck", nullptr, nullptr, Z_Construct_UFunction_UBXCollisionLibrary_SphereCheck_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UBXCollisionLibrary_SphereCheck_Statics::PropPointers), sizeof(Z_Construct_UFunction_UBXCollisionLibrary_SphereCheck_Statics::BXCollisionLibrary_eventSphereCheck_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04422401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UBXCollisionLibrary_SphereCheck_Statics::Function_MetaDataParams), Z_Construct_UFunction_UBXCollisionLibrary_SphereCheck_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UBXCollisionLibrary_SphereCheck_Statics::BXCollisionLibrary_eventSphereCheck_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UBXCollisionLibrary_SphereCheck()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UBXCollisionLibrary_SphereCheck_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UBXCollisionLibrary::execSphereCheck)
{
	P_GET_STRUCT_REF(FBXCParameter,Z_Param_Out_Parameter);
	P_GET_TARRAY_REF(TEnumAsByte<EObjectTypeQuery>,Z_Param_Out_ObjectTypes);
	P_GET_PROPERTY(FFloatProperty,Z_Param_SpereSize);
	P_GET_STRUCT_REF(FBXCFilter,Z_Param_Out_Filter);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(TArray<FHitResult>*)Z_Param__Result=UBXCollisionLibrary::SphereCheck(Z_Param_Out_Parameter,Z_Param_Out_ObjectTypes,Z_Param_SpereSize,Z_Param_Out_Filter);
	P_NATIVE_END;
}
// End Class UBXCollisionLibrary Function SphereCheck

// Begin Class UBXCollisionLibrary
void UBXCollisionLibrary::StaticRegisterNativesUBXCollisionLibrary()
{
	UClass* Class = UBXCollisionLibrary::StaticClass();
	static const FNameNativePtrPair Funcs[] = {
		{ "BoxCheck", &UBXCollisionLibrary::execBoxCheck },
		{ "CapsuleCheck", &UBXCollisionLibrary::execCapsuleCheck },
		{ "CheckCollisionResult", &UBXCollisionLibrary::execCheckCollisionResult },
		{ "CylinderCheck", &UBXCollisionLibrary::execCylinderCheck },
		{ "HollowCylinderCheck", &UBXCollisionLibrary::execHollowCylinderCheck },
		{ "SectorCheck", &UBXCollisionLibrary::execSectorCheck },
		{ "SphereCheck", &UBXCollisionLibrary::execSphereCheck },
	};
	FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UBXCollisionLibrary);
UClass* Z_Construct_UClass_UBXCollisionLibrary_NoRegister()
{
	return UBXCollisionLibrary::StaticClass();
}
struct Z_Construct_UClass_UBXCollisionLibrary_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "IncludePath", "Collision/BXCollision.h" },
		{ "IsBlueprintBase", "true" },
		{ "ModuleRelativePath", "Collision/BXCollision.h" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UBXCollisionLibrary_BoxCheck, "BoxCheck" }, // 2973944967
		{ &Z_Construct_UFunction_UBXCollisionLibrary_CapsuleCheck, "CapsuleCheck" }, // 3847495045
		{ &Z_Construct_UFunction_UBXCollisionLibrary_CheckCollisionResult, "CheckCollisionResult" }, // 3442513108
		{ &Z_Construct_UFunction_UBXCollisionLibrary_CylinderCheck, "CylinderCheck" }, // 4111301311
		{ &Z_Construct_UFunction_UBXCollisionLibrary_HollowCylinderCheck, "HollowCylinderCheck" }, // 1148861012
		{ &Z_Construct_UFunction_UBXCollisionLibrary_SectorCheck, "SectorCheck" }, // 2785666405
		{ &Z_Construct_UFunction_UBXCollisionLibrary_SphereCheck, "SphereCheck" }, // 493882546
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UBXCollisionLibrary>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_UBXCollisionLibrary_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UBlueprintFunctionLibrary,
	(UObject* (*)())Z_Construct_UPackage__Script_BattleX,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UBXCollisionLibrary_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UBXCollisionLibrary_Statics::ClassParams = {
	&UBXCollisionLibrary::StaticClass,
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
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UBXCollisionLibrary_Statics::Class_MetaDataParams), Z_Construct_UClass_UBXCollisionLibrary_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UBXCollisionLibrary()
{
	if (!Z_Registration_Info_UClass_UBXCollisionLibrary.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UBXCollisionLibrary.OuterSingleton, Z_Construct_UClass_UBXCollisionLibrary_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UBXCollisionLibrary.OuterSingleton;
}
template<> BATTLEX_API UClass* StaticClass<UBXCollisionLibrary>()
{
	return UBXCollisionLibrary::StaticClass();
}
UBXCollisionLibrary::UBXCollisionLibrary(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UBXCollisionLibrary);
UBXCollisionLibrary::~UBXCollisionLibrary() {}
// End Class UBXCollisionLibrary

// Begin Registration
struct Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Collision_BXCollision_h_Statics
{
	static constexpr FEnumRegisterCompiledInInfo EnumInfo[] = {
		{ EBXCDirection_StaticEnum, TEXT("EBXCDirection"), &Z_Registration_Info_UEnum_EBXCDirection, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 2768710293U) },
	};
	static constexpr FStructRegisterCompiledInInfo ScriptStructInfo[] = {
		{ FBXCFilter::StaticStruct, Z_Construct_UScriptStruct_FBXCFilter_Statics::NewStructOps, TEXT("BXCFilter"), &Z_Registration_Info_UScriptStruct_BXCFilter, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FBXCFilter), 366435299U) },
		{ FBXCParameter::StaticStruct, Z_Construct_UScriptStruct_FBXCParameter_Statics::NewStructOps, TEXT("BXCParameter"), &Z_Registration_Info_UScriptStruct_BXCParameter, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FBXCParameter), 4276197417U) },
		{ FBXCStrategy::StaticStruct, Z_Construct_UScriptStruct_FBXCStrategy_Statics::NewStructOps, TEXT("BXCStrategy"), &Z_Registration_Info_UScriptStruct_BXCStrategy, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FBXCStrategy), 3653754596U) },
		{ FBXCSSphere::StaticStruct, Z_Construct_UScriptStruct_FBXCSSphere_Statics::NewStructOps, TEXT("BXCSSphere"), &Z_Registration_Info_UScriptStruct_BXCSSphere, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FBXCSSphere), 4198970703U) },
		{ FBXCSCapsule::StaticStruct, Z_Construct_UScriptStruct_FBXCSCapsule_Statics::NewStructOps, TEXT("BXCSCapsule"), &Z_Registration_Info_UScriptStruct_BXCSCapsule, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FBXCSCapsule), 2687993861U) },
		{ FBXCSCylinder::StaticStruct, Z_Construct_UScriptStruct_FBXCSCylinder_Statics::NewStructOps, TEXT("BXCSCylinder"), &Z_Registration_Info_UScriptStruct_BXCSCylinder, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FBXCSCylinder), 2042418414U) },
		{ FBXCSHollowCylinder::StaticStruct, Z_Construct_UScriptStruct_FBXCSHollowCylinder_Statics::NewStructOps, TEXT("BXCSHollowCylinder"), &Z_Registration_Info_UScriptStruct_BXCSHollowCylinder, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FBXCSHollowCylinder), 3957638556U) },
		{ FBXCSBox::StaticStruct, Z_Construct_UScriptStruct_FBXCSBox_Statics::NewStructOps, TEXT("BXCSBox"), &Z_Registration_Info_UScriptStruct_BXCSBox, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FBXCSBox), 318878674U) },
		{ FBXCSSector::StaticStruct, Z_Construct_UScriptStruct_FBXCSSector_Statics::NewStructOps, TEXT("BXCSSector"), &Z_Registration_Info_UScriptStruct_BXCSSector, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FBXCSSector), 1934981830U) },
	};
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UBXCollisionLibrary, UBXCollisionLibrary::StaticClass, TEXT("UBXCollisionLibrary"), &Z_Registration_Info_UClass_UBXCollisionLibrary, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UBXCollisionLibrary), 1745542747U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Collision_BXCollision_h_3902015480(TEXT("/Script/BattleX"),
	Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Collision_BXCollision_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Collision_BXCollision_h_Statics::ClassInfo),
	Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Collision_BXCollision_h_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Collision_BXCollision_h_Statics::ScriptStructInfo),
	Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Collision_BXCollision_h_Statics::EnumInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Collision_BXCollision_h_Statics::EnumInfo));
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
