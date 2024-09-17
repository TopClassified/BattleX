// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "BattleX/BXStructs.h"
#include "Runtime/GameplayTags/Classes/GameplayTagContainer.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeBXStructs() {}

// Begin Cross Module References
BATTLEX_API UClass* Z_Construct_UClass_UBXHitReactionComponent_NoRegister();
BATTLEX_API UClass* Z_Construct_UClass_UBXTLAsset_NoRegister();
BATTLEX_API UEnum* Z_Construct_UEnum_BattleX_EBXBodyPartType();
BATTLEX_API UEnum* Z_Construct_UEnum_BattleX_EBXCompareSign();
BATTLEX_API UEnum* Z_Construct_UEnum_BattleX_EBXShapeType();
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXActorList();
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXAttributeComparator();
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXBodyPartConfig();
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXBodyPartRTInformation();
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXBodyPartSelection();
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXBoneSelector();
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXChangeBuff();
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXCollisionChannelInfo();
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXMeshCreater();
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXObjectList();
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXShapeInformation();
BATTLEX_API UScriptStruct* Z_Construct_UScriptStruct_FBXTimelineAssetInformation();
COREUOBJECT_API UClass* Z_Construct_UClass_UClass();
COREUOBJECT_API UClass* Z_Construct_UClass_UObject_NoRegister();
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FTransform();
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FVector();
ENGINE_API UClass* Z_Construct_UClass_AActor_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_UAnimationAsset_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_USceneComponent_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_USkeletalMesh_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_UStaticMesh_NoRegister();
ENGINE_API UEnum* Z_Construct_UEnum_Engine_ECollisionChannel();
ENGINE_API UEnum* Z_Construct_UEnum_Engine_ECollisionResponse();
ENGINE_API UEnum* Z_Construct_UEnum_Engine_ERendererStencilMask();
GAMEPLAYTAGS_API UScriptStruct* Z_Construct_UScriptStruct_FGameplayTag();
UPackage* Z_Construct_UPackage__Script_BattleX();
// End Cross Module References

// Begin ScriptStruct FBXObjectList
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_BXObjectList;
class UScriptStruct* FBXObjectList::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_BXObjectList.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_BXObjectList.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FBXObjectList, (UObject*)Z_Construct_UPackage__Script_BattleX(), TEXT("BXObjectList"));
	}
	return Z_Registration_Info_UScriptStruct_BXObjectList.OuterSingleton;
}
template<> BATTLEX_API UScriptStruct* StaticStruct<FBXObjectList>()
{
	return FBXObjectList::StaticStruct();
}
struct Z_Construct_UScriptStruct_FBXObjectList_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Object\xe5\x88\x97\xe8\xa1\xa8\n" },
#endif
		{ "ModuleRelativePath", "BXStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Object\xe5\x88\x97\xe8\xa1\xa8" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_List_MetaData[] = {
		{ "Category", "BXObjectList" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe5\xaf\xb9\xe8\xb1\xa1\xe5\x88\x97\xe8\xa1\xa8\n" },
#endif
		{ "ModuleRelativePath", "BXStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe5\xaf\xb9\xe8\xb1\xa1\xe5\x88\x97\xe8\xa1\xa8" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_List_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_List;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FBXObjectList>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UScriptStruct_FBXObjectList_Statics::NewProp_List_Inner = { "List", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UClass_UObject_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FBXObjectList_Statics::NewProp_List = { "List", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXObjectList, List), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_List_MetaData), NewProp_List_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FBXObjectList_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXObjectList_Statics::NewProp_List_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXObjectList_Statics::NewProp_List,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXObjectList_Statics::PropPointers) < 2048);
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FBXObjectList_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_BattleX,
	nullptr,
	&NewStructOps,
	"BXObjectList",
	Z_Construct_UScriptStruct_FBXObjectList_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXObjectList_Statics::PropPointers),
	sizeof(FBXObjectList),
	alignof(FBXObjectList),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000001),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXObjectList_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FBXObjectList_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FBXObjectList()
{
	if (!Z_Registration_Info_UScriptStruct_BXObjectList.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_BXObjectList.InnerSingleton, Z_Construct_UScriptStruct_FBXObjectList_Statics::StructParams);
	}
	return Z_Registration_Info_UScriptStruct_BXObjectList.InnerSingleton;
}
// End ScriptStruct FBXObjectList

// Begin ScriptStruct FBXActorList
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_BXActorList;
class UScriptStruct* FBXActorList::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_BXActorList.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_BXActorList.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FBXActorList, (UObject*)Z_Construct_UPackage__Script_BattleX(), TEXT("BXActorList"));
	}
	return Z_Registration_Info_UScriptStruct_BXActorList.OuterSingleton;
}
template<> BATTLEX_API UScriptStruct* StaticStruct<FBXActorList>()
{
	return FBXActorList::StaticStruct();
}
struct Z_Construct_UScriptStruct_FBXActorList_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Actor\xe5\x88\x97\xe8\xa1\xa8\n" },
#endif
		{ "ModuleRelativePath", "BXStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Actor\xe5\x88\x97\xe8\xa1\xa8" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_List_MetaData[] = {
		{ "Category", "BXActorList" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe8\xa7\x92\xe8\x89\xb2\xe5\x88\x97\xe8\xa1\xa8\n" },
#endif
		{ "ModuleRelativePath", "BXStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe8\xa7\x92\xe8\x89\xb2\xe5\x88\x97\xe8\xa1\xa8" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_List_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_List;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FBXActorList>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UScriptStruct_FBXActorList_Statics::NewProp_List_Inner = { "List", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FBXActorList_Statics::NewProp_List = { "List", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXActorList, List), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_List_MetaData), NewProp_List_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FBXActorList_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXActorList_Statics::NewProp_List_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXActorList_Statics::NewProp_List,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXActorList_Statics::PropPointers) < 2048);
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FBXActorList_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_BattleX,
	nullptr,
	&NewStructOps,
	"BXActorList",
	Z_Construct_UScriptStruct_FBXActorList_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXActorList_Statics::PropPointers),
	sizeof(FBXActorList),
	alignof(FBXActorList),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000001),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXActorList_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FBXActorList_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FBXActorList()
{
	if (!Z_Registration_Info_UScriptStruct_BXActorList.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_BXActorList.InnerSingleton, Z_Construct_UScriptStruct_FBXActorList_Statics::StructParams);
	}
	return Z_Registration_Info_UScriptStruct_BXActorList.InnerSingleton;
}
// End ScriptStruct FBXActorList

// Begin ScriptStruct FBXBoneSelector
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_BXBoneSelector;
class UScriptStruct* FBXBoneSelector::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_BXBoneSelector.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_BXBoneSelector.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FBXBoneSelector, (UObject*)Z_Construct_UPackage__Script_BattleX(), TEXT("BXBoneSelector"));
	}
	return Z_Registration_Info_UScriptStruct_BXBoneSelector.OuterSingleton;
}
template<> BATTLEX_API UScriptStruct* StaticStruct<FBXBoneSelector>()
{
	return FBXBoneSelector::StaticStruct();
}
struct Z_Construct_UScriptStruct_FBXBoneSelector_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe9\xaa\xa8\xe9\xaa\xbc\xe9\x80\x89\xe6\x8b\xa9\xe5\x99\xa8\n" },
#endif
		{ "ModuleRelativePath", "BXStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe9\xaa\xa8\xe9\xaa\xbc\xe9\x80\x89\xe6\x8b\xa9\xe5\x99\xa8" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_BoneName_MetaData[] = {
		{ "Category", "BXBoneSelector" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe9\xaa\xa8\xe9\xaa\xbc\xe5\x90\x8d\xe7\xa7\xb0\n" },
#endif
		{ "ModuleRelativePath", "BXStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe9\xaa\xa8\xe9\xaa\xbc\xe5\x90\x8d\xe7\xa7\xb0" },
#endif
	};
#if WITH_EDITORONLY_DATA
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_SkeletalMesh_MetaData[] = {
		{ "Category", "BXBoneSelector" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe9\xaa\xa8\xe9\xaa\xbc\xe8\xb5\x84\xe6\xba\x90\n" },
#endif
		{ "ModuleRelativePath", "BXStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe9\xaa\xa8\xe9\xaa\xbc\xe8\xb5\x84\xe6\xba\x90" },
#endif
	};
#endif // WITH_EDITORONLY_DATA
#endif // WITH_METADATA
	static const UECodeGen_Private::FNamePropertyParams NewProp_BoneName;
#if WITH_EDITORONLY_DATA
	static const UECodeGen_Private::FSoftObjectPropertyParams NewProp_SkeletalMesh;
#endif // WITH_EDITORONLY_DATA
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FBXBoneSelector>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
};
const UECodeGen_Private::FNamePropertyParams Z_Construct_UScriptStruct_FBXBoneSelector_Statics::NewProp_BoneName = { "BoneName", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXBoneSelector, BoneName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_BoneName_MetaData), NewProp_BoneName_MetaData) };
#if WITH_EDITORONLY_DATA
const UECodeGen_Private::FSoftObjectPropertyParams Z_Construct_UScriptStruct_FBXBoneSelector_Statics::NewProp_SkeletalMesh = { "SkeletalMesh", nullptr, (EPropertyFlags)0x0014000800000001, UECodeGen_Private::EPropertyGenFlags::SoftObject, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXBoneSelector, SkeletalMesh), Z_Construct_UClass_USkeletalMesh_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_SkeletalMesh_MetaData), NewProp_SkeletalMesh_MetaData) };
#endif // WITH_EDITORONLY_DATA
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FBXBoneSelector_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXBoneSelector_Statics::NewProp_BoneName,
#if WITH_EDITORONLY_DATA
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXBoneSelector_Statics::NewProp_SkeletalMesh,
#endif // WITH_EDITORONLY_DATA
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXBoneSelector_Statics::PropPointers) < 2048);
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FBXBoneSelector_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_BattleX,
	nullptr,
	&NewStructOps,
	"BXBoneSelector",
	Z_Construct_UScriptStruct_FBXBoneSelector_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXBoneSelector_Statics::PropPointers),
	sizeof(FBXBoneSelector),
	alignof(FBXBoneSelector),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000001),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXBoneSelector_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FBXBoneSelector_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FBXBoneSelector()
{
	if (!Z_Registration_Info_UScriptStruct_BXBoneSelector.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_BXBoneSelector.InnerSingleton, Z_Construct_UScriptStruct_FBXBoneSelector_Statics::StructParams);
	}
	return Z_Registration_Info_UScriptStruct_BXBoneSelector.InnerSingleton;
}
// End ScriptStruct FBXBoneSelector

// Begin ScriptStruct FBXMeshCreater
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_BXMeshCreater;
class UScriptStruct* FBXMeshCreater::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_BXMeshCreater.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_BXMeshCreater.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FBXMeshCreater, (UObject*)Z_Construct_UPackage__Script_BattleX(), TEXT("BXMeshCreater"));
	}
	return Z_Registration_Info_UScriptStruct_BXMeshCreater.OuterSingleton;
}
template<> BATTLEX_API UScriptStruct* StaticStruct<FBXMeshCreater>()
{
	return FBXMeshCreater::StaticStruct();
}
struct Z_Construct_UScriptStruct_FBXMeshCreater_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe6\xa8\xa1\xe5\x9e\x8b\xe5\x88\x9b\xe5\xbb\xba\xe5\x99\xa8\n" },
#endif
		{ "ModuleRelativePath", "BXStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe6\xa8\xa1\xe5\x9e\x8b\xe5\x88\x9b\xe5\xbb\xba\xe5\x99\xa8" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_MeshName_MetaData[] = {
		{ "Category", "BXMeshCreater" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe6\xa8\xa1\xe5\x9e\x8b\xe5\x90\x8d\xe7\xa7\xb0\n" },
#endif
		{ "ModuleRelativePath", "BXStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe6\xa8\xa1\xe5\x9e\x8b\xe5\x90\x8d\xe7\xa7\xb0" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bUseStaticMesh_MetaData[] = {
		{ "Category", "BXMeshCreater" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe4\xbd\xbf\xe7\x94\xa8\xe9\x9d\x99\xe6\x80\x81\xe6\xa8\xa1\xe5\x9e\x8b \xe8\xbf\x98\xe6\x98\xaf \xe9\xaa\xa8\xe9\xaa\xbc\xe6\xa8\xa1\xe5\x9e\x8b\n" },
#endif
		{ "ModuleRelativePath", "BXStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe4\xbd\xbf\xe7\x94\xa8\xe9\x9d\x99\xe6\x80\x81\xe6\xa8\xa1\xe5\x9e\x8b \xe8\xbf\x98\xe6\x98\xaf \xe9\xaa\xa8\xe9\xaa\xbc\xe6\xa8\xa1\xe5\x9e\x8b" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_StaticMesh_MetaData[] = {
		{ "Category", "BXMeshCreater" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe9\x9d\x99\xe6\x80\x81\xe6\xa8\xa1\xe5\x9e\x8b\n" },
#endif
		{ "EditCondition", "bUseStaticMesh" },
		{ "ModuleRelativePath", "BXStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe9\x9d\x99\xe6\x80\x81\xe6\xa8\xa1\xe5\x9e\x8b" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_SkeletalMesh_MetaData[] = {
		{ "Category", "BXMeshCreater" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe5\x8a\xa8\xe6\x80\x81\xe6\xa8\xa1\xe5\x9e\x8b\n" },
#endif
		{ "EditCondition", "!bUseStaticMesh" },
		{ "ModuleRelativePath", "BXStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe5\x8a\xa8\xe6\x80\x81\xe6\xa8\xa1\xe5\x9e\x8b" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_AnimSequence_MetaData[] = {
		{ "Category", "BXMeshCreater" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe5\x8a\xa8\xe6\x80\x81\xe6\xa8\xa1\xe5\x9e\x8b\xe5\x8a\xa8\xe7\x94\xbb\n" },
#endif
		{ "EditCondition", "!bUseStaticMesh" },
		{ "ModuleRelativePath", "BXStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe5\x8a\xa8\xe6\x80\x81\xe6\xa8\xa1\xe5\x9e\x8b\xe5\x8a\xa8\xe7\x94\xbb" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bNeedLoop_MetaData[] = {
		{ "Category", "BXMeshCreater" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe5\xbe\xaa\xe7\x8e\xaf\xe6\x92\xad\xe6\x94\xbe\xe5\x8a\xa8\xe7\x94\xbb\n" },
#endif
		{ "EditCondition", "!bUseStaticMesh" },
		{ "ModuleRelativePath", "BXStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe5\xbe\xaa\xe7\x8e\xaf\xe6\x92\xad\xe6\x94\xbe\xe5\x8a\xa8\xe7\x94\xbb" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bNeedAttach_MetaData[] = {
		{ "Category", "BXMeshCreater" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe6\x98\xaf\xe5\x90\xa6\xe8\xbf\x9b\xe8\xa1\x8c\xe6\x8c\x82\xe6\x8e\xa5\n" },
#endif
		{ "ModuleRelativePath", "BXStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe6\x98\xaf\xe5\x90\xa6\xe8\xbf\x9b\xe8\xa1\x8c\xe6\x8c\x82\xe6\x8e\xa5" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_AttachSocket_MetaData[] = {
		{ "Category", "BXMeshCreater" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe6\x8c\x82\xe6\x8e\xa5\xe9\xaa\xa8\xe9\xaa\xbc\xe5\x90\x8d\xe7\xa7\xb0\n" },
#endif
		{ "EditCondition", "bNeedAttach" },
		{ "ModuleRelativePath", "BXStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe6\x8c\x82\xe6\x8e\xa5\xe9\xaa\xa8\xe9\xaa\xbc\xe5\x90\x8d\xe7\xa7\xb0" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_RelativeTransform_MetaData[] = {
		{ "Category", "BXMeshCreater" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe7\x9b\xb8\xe5\xaf\xb9\xe4\xbd\x8d\xe7\xbd\xae\n" },
#endif
		{ "ModuleRelativePath", "BXStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe7\x9b\xb8\xe5\xaf\xb9\xe4\xbd\x8d\xe7\xbd\xae" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bReceivesDecals_MetaData[] = {
		{ "Category", "Render" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe6\x98\xaf\xe5\x90\xa6\xe6\x8e\xa5\xe5\x8f\x97\xe8\xb4\xb4\xe8\x8a\xb1\n" },
#endif
		{ "ModuleRelativePath", "BXStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe6\x98\xaf\xe5\x90\xa6\xe6\x8e\xa5\xe5\x8f\x97\xe8\xb4\xb4\xe8\x8a\xb1" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bUseCustomDepth_MetaData[] = {
		{ "Category", "Render" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// bRenderCustomDepth & bRenderInDepthPass\xef\xbc\x8c\xe4\xb8\xbatrue\xe6\x97\xb6\xe4\xbc\x9a\xe8\xbf\x9b\xe8\xa1\x8c\xe8\xbf\x99\xe7\xb3\xbb\xe5\x88\x97\xe7\x9a\x84Render\xe5\x8f\x82\xe6\x95\xb0\xe8\xae\xbe\xe7\xbd\xae\n" },
#endif
		{ "ModuleRelativePath", "BXStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "bRenderCustomDepth & bRenderInDepthPass\xef\xbc\x8c\xe4\xb8\xbatrue\xe6\x97\xb6\xe4\xbc\x9a\xe8\xbf\x9b\xe8\xa1\x8c\xe8\xbf\x99\xe7\xb3\xbb\xe5\x88\x97\xe7\x9a\x84Render\xe5\x8f\x82\xe6\x95\xb0\xe8\xae\xbe\xe7\xbd\xae" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bRenderInMainPass_MetaData[] = {
		{ "Category", "Render" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// bRenderInMainPass\n" },
#endif
		{ "EditCondition", "bUseCustomDepth" },
		{ "ModuleRelativePath", "BXStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "bRenderInMainPass" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_CustomDepthStencilWriteMask_MetaData[] = {
		{ "Category", "Render" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe8\x87\xaa\xe5\xae\x9a\xe4\xb9\x89\xe6\xb7\xb1\xe5\xba\xa6\xe7\x9a\x84Mask\n" },
#endif
		{ "EditCondition", "bUseCustomDepth" },
		{ "ModuleRelativePath", "BXStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe8\x87\xaa\xe5\xae\x9a\xe4\xb9\x89\xe6\xb7\xb1\xe5\xba\xa6\xe7\x9a\x84Mask" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_CustomDepthStencilValue_MetaData[] = {
		{ "Category", "Render" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe8\x87\xaa\xe5\xae\x9a\xe4\xb9\x89\xe6\xb7\xb1\xe5\xba\xa6\xe7\x9a\x84\xe5\x80\xbc\n" },
#endif
		{ "EditCondition", "bUseCustomDepth" },
		{ "ModuleRelativePath", "BXStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe8\x87\xaa\xe5\xae\x9a\xe4\xb9\x89\xe6\xb7\xb1\xe5\xba\xa6\xe7\x9a\x84\xe5\x80\xbc" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FNamePropertyParams NewProp_MeshName;
	static void NewProp_bUseStaticMesh_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bUseStaticMesh;
	static const UECodeGen_Private::FSoftObjectPropertyParams NewProp_StaticMesh;
	static const UECodeGen_Private::FSoftObjectPropertyParams NewProp_SkeletalMesh;
	static const UECodeGen_Private::FSoftObjectPropertyParams NewProp_AnimSequence;
	static void NewProp_bNeedLoop_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bNeedLoop;
	static void NewProp_bNeedAttach_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bNeedAttach;
	static const UECodeGen_Private::FStructPropertyParams NewProp_AttachSocket;
	static const UECodeGen_Private::FStructPropertyParams NewProp_RelativeTransform;
	static void NewProp_bReceivesDecals_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bReceivesDecals;
	static void NewProp_bUseCustomDepth_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bUseCustomDepth;
	static void NewProp_bRenderInMainPass_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bRenderInMainPass;
	static const UECodeGen_Private::FBytePropertyParams NewProp_CustomDepthStencilWriteMask_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_CustomDepthStencilWriteMask;
	static const UECodeGen_Private::FIntPropertyParams NewProp_CustomDepthStencilValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FBXMeshCreater>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
};
const UECodeGen_Private::FNamePropertyParams Z_Construct_UScriptStruct_FBXMeshCreater_Statics::NewProp_MeshName = { "MeshName", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXMeshCreater, MeshName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_MeshName_MetaData), NewProp_MeshName_MetaData) };
void Z_Construct_UScriptStruct_FBXMeshCreater_Statics::NewProp_bUseStaticMesh_SetBit(void* Obj)
{
	((FBXMeshCreater*)Obj)->bUseStaticMesh = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FBXMeshCreater_Statics::NewProp_bUseStaticMesh = { "bUseStaticMesh", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FBXMeshCreater), &Z_Construct_UScriptStruct_FBXMeshCreater_Statics::NewProp_bUseStaticMesh_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bUseStaticMesh_MetaData), NewProp_bUseStaticMesh_MetaData) };
const UECodeGen_Private::FSoftObjectPropertyParams Z_Construct_UScriptStruct_FBXMeshCreater_Statics::NewProp_StaticMesh = { "StaticMesh", nullptr, (EPropertyFlags)0x0014000000000005, UECodeGen_Private::EPropertyGenFlags::SoftObject, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXMeshCreater, StaticMesh), Z_Construct_UClass_UStaticMesh_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_StaticMesh_MetaData), NewProp_StaticMesh_MetaData) };
const UECodeGen_Private::FSoftObjectPropertyParams Z_Construct_UScriptStruct_FBXMeshCreater_Statics::NewProp_SkeletalMesh = { "SkeletalMesh", nullptr, (EPropertyFlags)0x0014000000000005, UECodeGen_Private::EPropertyGenFlags::SoftObject, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXMeshCreater, SkeletalMesh), Z_Construct_UClass_USkeletalMesh_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_SkeletalMesh_MetaData), NewProp_SkeletalMesh_MetaData) };
const UECodeGen_Private::FSoftObjectPropertyParams Z_Construct_UScriptStruct_FBXMeshCreater_Statics::NewProp_AnimSequence = { "AnimSequence", nullptr, (EPropertyFlags)0x0014000000000005, UECodeGen_Private::EPropertyGenFlags::SoftObject, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXMeshCreater, AnimSequence), Z_Construct_UClass_UAnimationAsset_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_AnimSequence_MetaData), NewProp_AnimSequence_MetaData) };
void Z_Construct_UScriptStruct_FBXMeshCreater_Statics::NewProp_bNeedLoop_SetBit(void* Obj)
{
	((FBXMeshCreater*)Obj)->bNeedLoop = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FBXMeshCreater_Statics::NewProp_bNeedLoop = { "bNeedLoop", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FBXMeshCreater), &Z_Construct_UScriptStruct_FBXMeshCreater_Statics::NewProp_bNeedLoop_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bNeedLoop_MetaData), NewProp_bNeedLoop_MetaData) };
void Z_Construct_UScriptStruct_FBXMeshCreater_Statics::NewProp_bNeedAttach_SetBit(void* Obj)
{
	((FBXMeshCreater*)Obj)->bNeedAttach = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FBXMeshCreater_Statics::NewProp_bNeedAttach = { "bNeedAttach", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FBXMeshCreater), &Z_Construct_UScriptStruct_FBXMeshCreater_Statics::NewProp_bNeedAttach_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bNeedAttach_MetaData), NewProp_bNeedAttach_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FBXMeshCreater_Statics::NewProp_AttachSocket = { "AttachSocket", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXMeshCreater, AttachSocket), Z_Construct_UScriptStruct_FBXBoneSelector, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_AttachSocket_MetaData), NewProp_AttachSocket_MetaData) }; // 1217301433
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FBXMeshCreater_Statics::NewProp_RelativeTransform = { "RelativeTransform", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXMeshCreater, RelativeTransform), Z_Construct_UScriptStruct_FTransform, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_RelativeTransform_MetaData), NewProp_RelativeTransform_MetaData) };
void Z_Construct_UScriptStruct_FBXMeshCreater_Statics::NewProp_bReceivesDecals_SetBit(void* Obj)
{
	((FBXMeshCreater*)Obj)->bReceivesDecals = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FBXMeshCreater_Statics::NewProp_bReceivesDecals = { "bReceivesDecals", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FBXMeshCreater), &Z_Construct_UScriptStruct_FBXMeshCreater_Statics::NewProp_bReceivesDecals_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bReceivesDecals_MetaData), NewProp_bReceivesDecals_MetaData) };
void Z_Construct_UScriptStruct_FBXMeshCreater_Statics::NewProp_bUseCustomDepth_SetBit(void* Obj)
{
	((FBXMeshCreater*)Obj)->bUseCustomDepth = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FBXMeshCreater_Statics::NewProp_bUseCustomDepth = { "bUseCustomDepth", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FBXMeshCreater), &Z_Construct_UScriptStruct_FBXMeshCreater_Statics::NewProp_bUseCustomDepth_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bUseCustomDepth_MetaData), NewProp_bUseCustomDepth_MetaData) };
void Z_Construct_UScriptStruct_FBXMeshCreater_Statics::NewProp_bRenderInMainPass_SetBit(void* Obj)
{
	((FBXMeshCreater*)Obj)->bRenderInMainPass = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FBXMeshCreater_Statics::NewProp_bRenderInMainPass = { "bRenderInMainPass", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FBXMeshCreater), &Z_Construct_UScriptStruct_FBXMeshCreater_Statics::NewProp_bRenderInMainPass_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bRenderInMainPass_MetaData), NewProp_bRenderInMainPass_MetaData) };
const UECodeGen_Private::FBytePropertyParams Z_Construct_UScriptStruct_FBXMeshCreater_Statics::NewProp_CustomDepthStencilWriteMask_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UScriptStruct_FBXMeshCreater_Statics::NewProp_CustomDepthStencilWriteMask = { "CustomDepthStencilWriteMask", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXMeshCreater, CustomDepthStencilWriteMask), Z_Construct_UEnum_Engine_ERendererStencilMask, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_CustomDepthStencilWriteMask_MetaData), NewProp_CustomDepthStencilWriteMask_MetaData) }; // 2385485039
const UECodeGen_Private::FIntPropertyParams Z_Construct_UScriptStruct_FBXMeshCreater_Statics::NewProp_CustomDepthStencilValue = { "CustomDepthStencilValue", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXMeshCreater, CustomDepthStencilValue), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_CustomDepthStencilValue_MetaData), NewProp_CustomDepthStencilValue_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FBXMeshCreater_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXMeshCreater_Statics::NewProp_MeshName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXMeshCreater_Statics::NewProp_bUseStaticMesh,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXMeshCreater_Statics::NewProp_StaticMesh,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXMeshCreater_Statics::NewProp_SkeletalMesh,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXMeshCreater_Statics::NewProp_AnimSequence,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXMeshCreater_Statics::NewProp_bNeedLoop,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXMeshCreater_Statics::NewProp_bNeedAttach,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXMeshCreater_Statics::NewProp_AttachSocket,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXMeshCreater_Statics::NewProp_RelativeTransform,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXMeshCreater_Statics::NewProp_bReceivesDecals,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXMeshCreater_Statics::NewProp_bUseCustomDepth,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXMeshCreater_Statics::NewProp_bRenderInMainPass,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXMeshCreater_Statics::NewProp_CustomDepthStencilWriteMask_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXMeshCreater_Statics::NewProp_CustomDepthStencilWriteMask,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXMeshCreater_Statics::NewProp_CustomDepthStencilValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXMeshCreater_Statics::PropPointers) < 2048);
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FBXMeshCreater_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_BattleX,
	nullptr,
	&NewStructOps,
	"BXMeshCreater",
	Z_Construct_UScriptStruct_FBXMeshCreater_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXMeshCreater_Statics::PropPointers),
	sizeof(FBXMeshCreater),
	alignof(FBXMeshCreater),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000001),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXMeshCreater_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FBXMeshCreater_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FBXMeshCreater()
{
	if (!Z_Registration_Info_UScriptStruct_BXMeshCreater.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_BXMeshCreater.InnerSingleton, Z_Construct_UScriptStruct_FBXMeshCreater_Statics::StructParams);
	}
	return Z_Registration_Info_UScriptStruct_BXMeshCreater.InnerSingleton;
}
// End ScriptStruct FBXMeshCreater

// Begin ScriptStruct FBXCollisionChannelInfo
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_BXCollisionChannelInfo;
class UScriptStruct* FBXCollisionChannelInfo::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_BXCollisionChannelInfo.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_BXCollisionChannelInfo.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FBXCollisionChannelInfo, (UObject*)Z_Construct_UPackage__Script_BattleX(), TEXT("BXCollisionChannelInfo"));
	}
	return Z_Registration_Info_UScriptStruct_BXCollisionChannelInfo.OuterSingleton;
}
template<> BATTLEX_API UScriptStruct* StaticStruct<FBXCollisionChannelInfo>()
{
	return FBXCollisionChannelInfo::StaticStruct();
}
struct Z_Construct_UScriptStruct_FBXCollisionChannelInfo_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe7\xa2\xb0\xe6\x92\x9e\xe9\x80\x9a\xe9\x81\x93\xe7\xad\x96\xe7\x95\xa5\n" },
#endif
		{ "IsBlueprintBase", "true" },
		{ "ModuleRelativePath", "BXStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe7\xa2\xb0\xe6\x92\x9e\xe9\x80\x9a\xe9\x81\x93\xe7\xad\x96\xe7\x95\xa5" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_NewCollisionChannel_MetaData[] = {
		{ "Category", "BXCollisionChannelInfo" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe7\xa2\xb0\xe6\x92\x9e\xe9\x80\x9a\xe9\x81\x93\n" },
#endif
		{ "ModuleRelativePath", "BXStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe7\xa2\xb0\xe6\x92\x9e\xe9\x80\x9a\xe9\x81\x93" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_NewCollisionResponse_MetaData[] = {
		{ "Category", "BXCollisionChannelInfo" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe7\xa2\xb0\xe6\x92\x9e\xe9\x80\x9a\xe9\x81\x93\xe5\xaf\xb9\xe5\xba\x94\xe7\x9a\x84\xe7\xad\x96\xe7\x95\xa5\n" },
#endif
		{ "ModuleRelativePath", "BXStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe7\xa2\xb0\xe6\x92\x9e\xe9\x80\x9a\xe9\x81\x93\xe5\xaf\xb9\xe5\xba\x94\xe7\x9a\x84\xe7\xad\x96\xe7\x95\xa5" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FBytePropertyParams NewProp_NewCollisionChannel;
	static const UECodeGen_Private::FBytePropertyParams NewProp_NewCollisionResponse;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FBXCollisionChannelInfo>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
};
const UECodeGen_Private::FBytePropertyParams Z_Construct_UScriptStruct_FBXCollisionChannelInfo_Statics::NewProp_NewCollisionChannel = { "NewCollisionChannel", nullptr, (EPropertyFlags)0x0010000000000015, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXCollisionChannelInfo, NewCollisionChannel), Z_Construct_UEnum_Engine_ECollisionChannel, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_NewCollisionChannel_MetaData), NewProp_NewCollisionChannel_MetaData) }; // 756624936
const UECodeGen_Private::FBytePropertyParams Z_Construct_UScriptStruct_FBXCollisionChannelInfo_Statics::NewProp_NewCollisionResponse = { "NewCollisionResponse", nullptr, (EPropertyFlags)0x0010000000000015, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXCollisionChannelInfo, NewCollisionResponse), Z_Construct_UEnum_Engine_ECollisionResponse, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_NewCollisionResponse_MetaData), NewProp_NewCollisionResponse_MetaData) }; // 1009580041
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FBXCollisionChannelInfo_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXCollisionChannelInfo_Statics::NewProp_NewCollisionChannel,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXCollisionChannelInfo_Statics::NewProp_NewCollisionResponse,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXCollisionChannelInfo_Statics::PropPointers) < 2048);
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FBXCollisionChannelInfo_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_BattleX,
	nullptr,
	&NewStructOps,
	"BXCollisionChannelInfo",
	Z_Construct_UScriptStruct_FBXCollisionChannelInfo_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXCollisionChannelInfo_Statics::PropPointers),
	sizeof(FBXCollisionChannelInfo),
	alignof(FBXCollisionChannelInfo),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000001),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXCollisionChannelInfo_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FBXCollisionChannelInfo_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FBXCollisionChannelInfo()
{
	if (!Z_Registration_Info_UScriptStruct_BXCollisionChannelInfo.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_BXCollisionChannelInfo.InnerSingleton, Z_Construct_UScriptStruct_FBXCollisionChannelInfo_Statics::StructParams);
	}
	return Z_Registration_Info_UScriptStruct_BXCollisionChannelInfo.InnerSingleton;
}
// End ScriptStruct FBXCollisionChannelInfo

// Begin ScriptStruct FBXAttributeComparator
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_BXAttributeComparator;
class UScriptStruct* FBXAttributeComparator::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_BXAttributeComparator.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_BXAttributeComparator.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FBXAttributeComparator, (UObject*)Z_Construct_UPackage__Script_BattleX(), TEXT("BXAttributeComparator"));
	}
	return Z_Registration_Info_UScriptStruct_BXAttributeComparator.OuterSingleton;
}
template<> BATTLEX_API UScriptStruct* StaticStruct<FBXAttributeComparator>()
{
	return FBXAttributeComparator::StaticStruct();
}
struct Z_Construct_UScriptStruct_FBXAttributeComparator_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe5\xb1\x9e\xe6\x80\xa7\xe6\xaf\x94\xe8\xbe\x83\xe5\x99\xa8\n" },
#endif
		{ "ModuleRelativePath", "BXStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe5\xb1\x9e\xe6\x80\xa7\xe6\xaf\x94\xe8\xbe\x83\xe5\x99\xa8" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Molecular_MetaData[] = {
		{ "Category", "BXAttributeComparator" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe5\x88\x86\xe5\xad\x90\xe5\xb1\x9e\xe6\x80\xa7\n" },
#endif
		{ "ModuleRelativePath", "BXStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe5\x88\x86\xe5\xad\x90\xe5\xb1\x9e\xe6\x80\xa7" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Denominator_MetaData[] = {
		{ "Category", "BXAttributeComparator" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe5\x88\x86\xe6\xaf\x8d\xe5\xb1\x9e\xe6\x80\xa7\n" },
#endif
		{ "ModuleRelativePath", "BXStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe5\x88\x86\xe6\xaf\x8d\xe5\xb1\x9e\xe6\x80\xa7" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Sign_MetaData[] = {
		{ "Category", "BXAttributeComparator" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe8\xbf\x90\xe7\xae\x97\xe7\xac\xa6\xe5\x8f\xb7\n" },
#endif
		{ "ModuleRelativePath", "BXStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe8\xbf\x90\xe7\xae\x97\xe7\xac\xa6\xe5\x8f\xb7" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Value_MetaData[] = {
		{ "Category", "BXAttributeComparator" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe8\xa6\x81\xe6\xaf\x94\xe8\xbe\x83\xe7\x9a\x84\xe5\x80\xbc\n" },
#endif
		{ "ModuleRelativePath", "BXStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe8\xa6\x81\xe6\xaf\x94\xe8\xbe\x83\xe7\x9a\x84\xe5\x80\xbc" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_Molecular;
	static const UECodeGen_Private::FStructPropertyParams NewProp_Denominator;
	static const UECodeGen_Private::FBytePropertyParams NewProp_Sign_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_Sign;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_Value;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FBXAttributeComparator>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FBXAttributeComparator_Statics::NewProp_Molecular = { "Molecular", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXAttributeComparator, Molecular), Z_Construct_UScriptStruct_FGameplayTag, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Molecular_MetaData), NewProp_Molecular_MetaData) }; // 1298103297
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FBXAttributeComparator_Statics::NewProp_Denominator = { "Denominator", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXAttributeComparator, Denominator), Z_Construct_UScriptStruct_FGameplayTag, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Denominator_MetaData), NewProp_Denominator_MetaData) }; // 1298103297
const UECodeGen_Private::FBytePropertyParams Z_Construct_UScriptStruct_FBXAttributeComparator_Statics::NewProp_Sign_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UScriptStruct_FBXAttributeComparator_Statics::NewProp_Sign = { "Sign", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXAttributeComparator, Sign), Z_Construct_UEnum_BattleX_EBXCompareSign, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Sign_MetaData), NewProp_Sign_MetaData) }; // 869846240
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FBXAttributeComparator_Statics::NewProp_Value = { "Value", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXAttributeComparator, Value), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Value_MetaData), NewProp_Value_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FBXAttributeComparator_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXAttributeComparator_Statics::NewProp_Molecular,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXAttributeComparator_Statics::NewProp_Denominator,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXAttributeComparator_Statics::NewProp_Sign_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXAttributeComparator_Statics::NewProp_Sign,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXAttributeComparator_Statics::NewProp_Value,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXAttributeComparator_Statics::PropPointers) < 2048);
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FBXAttributeComparator_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_BattleX,
	nullptr,
	&NewStructOps,
	"BXAttributeComparator",
	Z_Construct_UScriptStruct_FBXAttributeComparator_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXAttributeComparator_Statics::PropPointers),
	sizeof(FBXAttributeComparator),
	alignof(FBXAttributeComparator),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000001),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXAttributeComparator_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FBXAttributeComparator_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FBXAttributeComparator()
{
	if (!Z_Registration_Info_UScriptStruct_BXAttributeComparator.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_BXAttributeComparator.InnerSingleton, Z_Construct_UScriptStruct_FBXAttributeComparator_Statics::StructParams);
	}
	return Z_Registration_Info_UScriptStruct_BXAttributeComparator.InnerSingleton;
}
// End ScriptStruct FBXAttributeComparator

// Begin ScriptStruct FBXChangeBuff
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_BXChangeBuff;
class UScriptStruct* FBXChangeBuff::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_BXChangeBuff.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_BXChangeBuff.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FBXChangeBuff, (UObject*)Z_Construct_UPackage__Script_BattleX(), TEXT("BXChangeBuff"));
	}
	return Z_Registration_Info_UScriptStruct_BXChangeBuff.OuterSingleton;
}
template<> BATTLEX_API UScriptStruct* StaticStruct<FBXChangeBuff>()
{
	return FBXChangeBuff::StaticStruct();
}
struct Z_Construct_UScriptStruct_FBXChangeBuff_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "BXStructs.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_BuffID_MetaData[] = {
		{ "Category", "BXChangeBuff" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// BUFF\xe8\xb5\x84\xe6\xba\x90ID\n" },
#endif
		{ "ModuleRelativePath", "BXStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "BUFF\xe8\xb5\x84\xe6\xba\x90ID" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ChangeLayer_MetaData[] = {
		{ "Category", "BXChangeBuff" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe5\xb1\x82\xe6\x95\xb0\xe4\xbf\xa1\xe6\x81\xaf(\xe7\x94\xa8\xe4\xba\x8e\xe5\x88\xa0\xe9\x99\xa4""BUFF\xe6\x97\xb6\xef\xbc\x8c\xe8\xaf\xa5\xe5\x80\xbc\xe4\xb8\xba\xe9\x9b\xb6\xe4\xbb\xa3\xe8\xa1\xa8\xe5\x85\xa8\xe9\x83\xa8\xe5\x88\xa0\xe9\x99\xa4)\n" },
#endif
		{ "ModuleRelativePath", "BXStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe5\xb1\x82\xe6\x95\xb0\xe4\xbf\xa1\xe6\x81\xaf(\xe7\x94\xa8\xe4\xba\x8e\xe5\x88\xa0\xe9\x99\xa4""BUFF\xe6\x97\xb6\xef\xbc\x8c\xe8\xaf\xa5\xe5\x80\xbc\xe4\xb8\xba\xe9\x9b\xb6\xe4\xbb\xa3\xe8\xa1\xa8\xe5\x85\xa8\xe9\x83\xa8\xe5\x88\xa0\xe9\x99\xa4)" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FIntPropertyParams NewProp_BuffID;
	static const UECodeGen_Private::FIntPropertyParams NewProp_ChangeLayer;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FBXChangeBuff>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
};
const UECodeGen_Private::FIntPropertyParams Z_Construct_UScriptStruct_FBXChangeBuff_Statics::NewProp_BuffID = { "BuffID", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXChangeBuff, BuffID), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_BuffID_MetaData), NewProp_BuffID_MetaData) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UScriptStruct_FBXChangeBuff_Statics::NewProp_ChangeLayer = { "ChangeLayer", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXChangeBuff, ChangeLayer), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ChangeLayer_MetaData), NewProp_ChangeLayer_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FBXChangeBuff_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXChangeBuff_Statics::NewProp_BuffID,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXChangeBuff_Statics::NewProp_ChangeLayer,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXChangeBuff_Statics::PropPointers) < 2048);
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FBXChangeBuff_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_BattleX,
	nullptr,
	&NewStructOps,
	"BXChangeBuff",
	Z_Construct_UScriptStruct_FBXChangeBuff_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXChangeBuff_Statics::PropPointers),
	sizeof(FBXChangeBuff),
	alignof(FBXChangeBuff),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000001),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXChangeBuff_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FBXChangeBuff_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FBXChangeBuff()
{
	if (!Z_Registration_Info_UScriptStruct_BXChangeBuff.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_BXChangeBuff.InnerSingleton, Z_Construct_UScriptStruct_FBXChangeBuff_Statics::StructParams);
	}
	return Z_Registration_Info_UScriptStruct_BXChangeBuff.InnerSingleton;
}
// End ScriptStruct FBXChangeBuff

// Begin ScriptStruct FBXShapeInformation
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_BXShapeInformation;
class UScriptStruct* FBXShapeInformation::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_BXShapeInformation.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_BXShapeInformation.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FBXShapeInformation, (UObject*)Z_Construct_UPackage__Script_BattleX(), TEXT("BXShapeInformation"));
	}
	return Z_Registration_Info_UScriptStruct_BXShapeInformation.OuterSingleton;
}
template<> BATTLEX_API UScriptStruct* StaticStruct<FBXShapeInformation>()
{
	return FBXShapeInformation::StaticStruct();
}
struct Z_Construct_UScriptStruct_FBXShapeInformation_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe7\xa2\xb0\xe6\x92\x9e\xe5\xbd\xa2\xe7\x8a\xb6\xe4\xbf\xa1\xe6\x81\xaf\n" },
#endif
		{ "ModuleRelativePath", "BXStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe7\xa2\xb0\xe6\x92\x9e\xe5\xbd\xa2\xe7\x8a\xb6\xe4\xbf\xa1\xe6\x81\xaf" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ShapeType_MetaData[] = {
		{ "Category", "BXShapeInformation" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe7\xa2\xb0\xe6\x92\x9e\xe7\x9b\x92\xe7\xb1\xbb\xe5\x9e\x8b\n" },
#endif
		{ "ModuleRelativePath", "BXStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe7\xa2\xb0\xe6\x92\x9e\xe7\x9b\x92\xe7\xb1\xbb\xe5\x9e\x8b" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ShapeSize_MetaData[] = {
		{ "Category", "BXShapeInformation" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe7\xa2\xb0\xe6\x92\x9e\xe7\x9b\x92\xe5\xa4\xa7\xe5\xb0\x8f(\xe7\x90\x83\xe4\xbd\x93=X, \xe8\x83\xb6\xe5\x9b\x8a\xe4\xbd\x93=X,Y, \xe9\x95\xbf\xe6\x96\xb9\xe4\xbd\x93=X,Y,Z)\n" },
#endif
		{ "ModuleRelativePath", "BXStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe7\xa2\xb0\xe6\x92\x9e\xe7\x9b\x92\xe5\xa4\xa7\xe5\xb0\x8f(\xe7\x90\x83\xe4\xbd\x93=X, \xe8\x83\xb6\xe5\x9b\x8a\xe4\xbd\x93=X,Y, \xe9\x95\xbf\xe6\x96\xb9\xe4\xbd\x93=X,Y,Z)" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_AttachToComponent_MetaData[] = {
		{ "Category", "BXShapeInformation" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe6\x8c\x82\xe6\x8e\xa5\xe5\x88\xb0\xe5\x93\xaa\xe4\xb8\xaa\xe7\xbb\x84\xe4\xbb\xb6\n" },
#endif
		{ "ModuleRelativePath", "BXStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe6\x8c\x82\xe6\x8e\xa5\xe5\x88\xb0\xe5\x93\xaa\xe4\xb8\xaa\xe7\xbb\x84\xe4\xbb\xb6" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_AttachToSocket_MetaData[] = {
		{ "Category", "BXShapeInformation" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe6\x8c\x82\xe6\x8e\xa5\xe5\x88\xb0\xe5\x93\xaa\xe4\xb8\xaaSocket\n" },
#endif
		{ "ModuleRelativePath", "BXStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe6\x8c\x82\xe6\x8e\xa5\xe5\x88\xb0\xe5\x93\xaa\xe4\xb8\xaaSocket" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Relation_MetaData[] = {
		{ "Category", "BXShapeInformation" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe7\x9b\xb8\xe5\xaf\xb9\xe6\x96\xb9\xe4\xbd\x8d\n" },
#endif
		{ "ModuleRelativePath", "BXStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe7\x9b\xb8\xe5\xaf\xb9\xe6\x96\xb9\xe4\xbd\x8d" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FBytePropertyParams NewProp_ShapeType_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_ShapeType;
	static const UECodeGen_Private::FStructPropertyParams NewProp_ShapeSize;
	static const UECodeGen_Private::FNamePropertyParams NewProp_AttachToComponent;
	static const UECodeGen_Private::FStructPropertyParams NewProp_AttachToSocket;
	static const UECodeGen_Private::FStructPropertyParams NewProp_Relation;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FBXShapeInformation>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
};
const UECodeGen_Private::FBytePropertyParams Z_Construct_UScriptStruct_FBXShapeInformation_Statics::NewProp_ShapeType_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UScriptStruct_FBXShapeInformation_Statics::NewProp_ShapeType = { "ShapeType", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXShapeInformation, ShapeType), Z_Construct_UEnum_BattleX_EBXShapeType, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ShapeType_MetaData), NewProp_ShapeType_MetaData) }; // 3113872912
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FBXShapeInformation_Statics::NewProp_ShapeSize = { "ShapeSize", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXShapeInformation, ShapeSize), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ShapeSize_MetaData), NewProp_ShapeSize_MetaData) };
const UECodeGen_Private::FNamePropertyParams Z_Construct_UScriptStruct_FBXShapeInformation_Statics::NewProp_AttachToComponent = { "AttachToComponent", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXShapeInformation, AttachToComponent), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_AttachToComponent_MetaData), NewProp_AttachToComponent_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FBXShapeInformation_Statics::NewProp_AttachToSocket = { "AttachToSocket", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXShapeInformation, AttachToSocket), Z_Construct_UScriptStruct_FBXBoneSelector, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_AttachToSocket_MetaData), NewProp_AttachToSocket_MetaData) }; // 1217301433
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FBXShapeInformation_Statics::NewProp_Relation = { "Relation", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXShapeInformation, Relation), Z_Construct_UScriptStruct_FTransform, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Relation_MetaData), NewProp_Relation_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FBXShapeInformation_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXShapeInformation_Statics::NewProp_ShapeType_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXShapeInformation_Statics::NewProp_ShapeType,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXShapeInformation_Statics::NewProp_ShapeSize,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXShapeInformation_Statics::NewProp_AttachToComponent,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXShapeInformation_Statics::NewProp_AttachToSocket,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXShapeInformation_Statics::NewProp_Relation,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXShapeInformation_Statics::PropPointers) < 2048);
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FBXShapeInformation_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_BattleX,
	nullptr,
	&NewStructOps,
	"BXShapeInformation",
	Z_Construct_UScriptStruct_FBXShapeInformation_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXShapeInformation_Statics::PropPointers),
	sizeof(FBXShapeInformation),
	alignof(FBXShapeInformation),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000001),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXShapeInformation_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FBXShapeInformation_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FBXShapeInformation()
{
	if (!Z_Registration_Info_UScriptStruct_BXShapeInformation.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_BXShapeInformation.InnerSingleton, Z_Construct_UScriptStruct_FBXShapeInformation_Statics::StructParams);
	}
	return Z_Registration_Info_UScriptStruct_BXShapeInformation.InnerSingleton;
}
// End ScriptStruct FBXShapeInformation

// Begin ScriptStruct FBXTimelineAssetInformation
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_BXTimelineAssetInformation;
class UScriptStruct* FBXTimelineAssetInformation::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_BXTimelineAssetInformation.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_BXTimelineAssetInformation.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FBXTimelineAssetInformation, (UObject*)Z_Construct_UPackage__Script_BattleX(), TEXT("BXTimelineAssetInformation"));
	}
	return Z_Registration_Info_UScriptStruct_BXTimelineAssetInformation.OuterSingleton;
}
template<> BATTLEX_API UScriptStruct* StaticStruct<FBXTimelineAssetInformation>()
{
	return FBXTimelineAssetInformation::StaticStruct();
}
struct Z_Construct_UScriptStruct_FBXTimelineAssetInformation_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe6\x97\xb6\xe9\x97\xb4\xe8\xbd\xb4\xe8\xb5\x84\xe6\xba\x90\xe4\xbf\xa1\xe6\x81\xaf\n" },
#endif
		{ "ModuleRelativePath", "BXStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe6\x97\xb6\xe9\x97\xb4\xe8\xbd\xb4\xe8\xb5\x84\xe6\xba\x90\xe4\xbf\xa1\xe6\x81\xaf" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_AssetPath_MetaData[] = {
		{ "Category", "BXTimelineAssetInformation" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe6\x97\xb6\xe9\x97\xb4\xe8\xbd\xb4\xe8\xb5\x84\xe6\xba\x90\n" },
#endif
		{ "ModuleRelativePath", "BXStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe6\x97\xb6\xe9\x97\xb4\xe8\xbd\xb4\xe8\xb5\x84\xe6\xba\x90" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_AssetType_MetaData[] = {
		{ "Category", "BXTimelineAssetInformation" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe8\xb5\x84\xe6\xba\x90\xe7\xb1\xbb\xe5\x9e\x8b\n" },
#endif
		{ "ModuleRelativePath", "BXStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe8\xb5\x84\xe6\xba\x90\xe7\xb1\xbb\xe5\x9e\x8b" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FSoftObjectPropertyParams NewProp_AssetPath;
	static const UECodeGen_Private::FClassPropertyParams NewProp_AssetType;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FBXTimelineAssetInformation>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
};
const UECodeGen_Private::FSoftObjectPropertyParams Z_Construct_UScriptStruct_FBXTimelineAssetInformation_Statics::NewProp_AssetPath = { "AssetPath", nullptr, (EPropertyFlags)0x0014000000000005, UECodeGen_Private::EPropertyGenFlags::SoftObject, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTimelineAssetInformation, AssetPath), Z_Construct_UClass_UBXTLAsset_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_AssetPath_MetaData), NewProp_AssetPath_MetaData) };
const UECodeGen_Private::FClassPropertyParams Z_Construct_UScriptStruct_FBXTimelineAssetInformation_Statics::NewProp_AssetType = { "AssetType", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Class, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTimelineAssetInformation, AssetType), Z_Construct_UClass_UClass, Z_Construct_UClass_UObject_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_AssetType_MetaData), NewProp_AssetType_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FBXTimelineAssetInformation_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTimelineAssetInformation_Statics::NewProp_AssetPath,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTimelineAssetInformation_Statics::NewProp_AssetType,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXTimelineAssetInformation_Statics::PropPointers) < 2048);
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FBXTimelineAssetInformation_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_BattleX,
	nullptr,
	&NewStructOps,
	"BXTimelineAssetInformation",
	Z_Construct_UScriptStruct_FBXTimelineAssetInformation_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXTimelineAssetInformation_Statics::PropPointers),
	sizeof(FBXTimelineAssetInformation),
	alignof(FBXTimelineAssetInformation),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000001),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXTimelineAssetInformation_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FBXTimelineAssetInformation_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FBXTimelineAssetInformation()
{
	if (!Z_Registration_Info_UScriptStruct_BXTimelineAssetInformation.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_BXTimelineAssetInformation.InnerSingleton, Z_Construct_UScriptStruct_FBXTimelineAssetInformation_Statics::StructParams);
	}
	return Z_Registration_Info_UScriptStruct_BXTimelineAssetInformation.InnerSingleton;
}
// End ScriptStruct FBXTimelineAssetInformation

// Begin ScriptStruct FBXBodyPartSelection
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_BXBodyPartSelection;
class UScriptStruct* FBXBodyPartSelection::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_BXBodyPartSelection.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_BXBodyPartSelection.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FBXBodyPartSelection, (UObject*)Z_Construct_UPackage__Script_BattleX(), TEXT("BXBodyPartSelection"));
	}
	return Z_Registration_Info_UScriptStruct_BXBodyPartSelection.OuterSingleton;
}
template<> BATTLEX_API UScriptStruct* StaticStruct<FBXBodyPartSelection>()
{
	return FBXBodyPartSelection::StaticStruct();
}
struct Z_Construct_UScriptStruct_FBXBodyPartSelection_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe8\xba\xab\xe4\xbd\x93\xe9\x83\xa8\xe4\xbd\x8d\xe9\x80\x89\xe5\x8f\x96\xe4\xbf\xa1\xe6\x81\xaf\n" },
#endif
		{ "ModuleRelativePath", "BXStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe8\xba\xab\xe4\xbd\x93\xe9\x83\xa8\xe4\xbd\x8d\xe9\x80\x89\xe5\x8f\x96\xe4\xbf\xa1\xe6\x81\xaf" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Owner_MetaData[] = {
		{ "Category", "BXBodyPartSelection" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe9\x83\xa8\xe4\xbd\x8d\xe6\x8b\xa5\xe6\x9c\x89\xe8\x80\x85\n" },
#endif
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "BXStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe9\x83\xa8\xe4\xbd\x8d\xe6\x8b\xa5\xe6\x9c\x89\xe8\x80\x85" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_BodyPart_MetaData[] = {
		{ "Category", "BXBodyPartSelection" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe9\x83\xa8\xe4\xbd\x8d\xe7\xb1\xbb\xe5\x9e\x8b\n" },
#endif
		{ "ModuleRelativePath", "BXStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe9\x83\xa8\xe4\xbd\x8d\xe7\xb1\xbb\xe5\x9e\x8b" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Owner;
	static const UECodeGen_Private::FBytePropertyParams NewProp_BodyPart_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_BodyPart;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FBXBodyPartSelection>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UScriptStruct_FBXBodyPartSelection_Statics::NewProp_Owner = { "Owner", nullptr, (EPropertyFlags)0x001000000008000d, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXBodyPartSelection, Owner), Z_Construct_UClass_UBXHitReactionComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Owner_MetaData), NewProp_Owner_MetaData) };
const UECodeGen_Private::FBytePropertyParams Z_Construct_UScriptStruct_FBXBodyPartSelection_Statics::NewProp_BodyPart_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UScriptStruct_FBXBodyPartSelection_Statics::NewProp_BodyPart = { "BodyPart", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXBodyPartSelection, BodyPart), Z_Construct_UEnum_BattleX_EBXBodyPartType, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_BodyPart_MetaData), NewProp_BodyPart_MetaData) }; // 84625349
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FBXBodyPartSelection_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXBodyPartSelection_Statics::NewProp_Owner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXBodyPartSelection_Statics::NewProp_BodyPart_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXBodyPartSelection_Statics::NewProp_BodyPart,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXBodyPartSelection_Statics::PropPointers) < 2048);
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FBXBodyPartSelection_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_BattleX,
	nullptr,
	&NewStructOps,
	"BXBodyPartSelection",
	Z_Construct_UScriptStruct_FBXBodyPartSelection_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXBodyPartSelection_Statics::PropPointers),
	sizeof(FBXBodyPartSelection),
	alignof(FBXBodyPartSelection),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000005),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXBodyPartSelection_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FBXBodyPartSelection_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FBXBodyPartSelection()
{
	if (!Z_Registration_Info_UScriptStruct_BXBodyPartSelection.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_BXBodyPartSelection.InnerSingleton, Z_Construct_UScriptStruct_FBXBodyPartSelection_Statics::StructParams);
	}
	return Z_Registration_Info_UScriptStruct_BXBodyPartSelection.InnerSingleton;
}
// End ScriptStruct FBXBodyPartSelection

// Begin ScriptStruct FBXBodyPartConfig
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_BXBodyPartConfig;
class UScriptStruct* FBXBodyPartConfig::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_BXBodyPartConfig.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_BXBodyPartConfig.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FBXBodyPartConfig, (UObject*)Z_Construct_UPackage__Script_BattleX(), TEXT("BXBodyPartConfig"));
	}
	return Z_Registration_Info_UScriptStruct_BXBodyPartConfig.OuterSingleton;
}
template<> BATTLEX_API UScriptStruct* StaticStruct<FBXBodyPartConfig>()
{
	return FBXBodyPartConfig::StaticStruct();
}
struct Z_Construct_UScriptStruct_FBXBodyPartConfig_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe8\xba\xab\xe4\xbd\x93\xe9\x83\xa8\xe4\xbd\x8d\xe9\x85\x8d\xe7\xbd\xae\xe4\xbf\xa1\xe6\x81\xaf\n" },
#endif
		{ "ModuleRelativePath", "BXStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe8\xba\xab\xe4\xbd\x93\xe9\x83\xa8\xe4\xbd\x8d\xe9\x85\x8d\xe7\xbd\xae\xe4\xbf\xa1\xe6\x81\xaf" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ComponentName_MetaData[] = {
		{ "Category", "BXBodyPartConfig" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe7\xbb\x84\xe4\xbb\xb6\xe5\x90\x8d\xe7\xa7\xb0\n" },
#endif
		{ "ModuleRelativePath", "BXStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe7\xbb\x84\xe4\xbb\xb6\xe5\x90\x8d\xe7\xa7\xb0" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_BoneNames_MetaData[] = {
		{ "Category", "BXBodyPartConfig" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe9\xaa\xa8\xe9\xaa\xbc\xe5\x88\x97\xe8\xa1\xa8\n" },
#endif
		{ "ModuleRelativePath", "BXStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe9\xaa\xa8\xe9\xaa\xbc\xe5\x88\x97\xe8\xa1\xa8" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bCanCollision_MetaData[] = {
		{ "Category", "BXBodyPartConfig" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe6\x98\xaf\xe5\x90\xa6\xe5\x85\x81\xe8\xae\xb8\xe7\xa2\xb0\xe6\x92\x9e\n" },
#endif
		{ "ModuleRelativePath", "BXStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe6\x98\xaf\xe5\x90\xa6\xe5\x85\x81\xe8\xae\xb8\xe7\xa2\xb0\xe6\x92\x9e" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bCanLock_MetaData[] = {
		{ "Category", "BXBodyPartConfig" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe6\x98\xaf\xe5\x90\xa6\xe5\x8f\xaf\xe4\xbb\xa5\xe9\x94\x81\xe5\xae\x9a\n" },
#endif
		{ "ModuleRelativePath", "BXStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe6\x98\xaf\xe5\x90\xa6\xe5\x8f\xaf\xe4\xbb\xa5\xe9\x94\x81\xe5\xae\x9a" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FNamePropertyParams NewProp_ComponentName;
	static const UECodeGen_Private::FNamePropertyParams NewProp_BoneNames_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_BoneNames;
	static void NewProp_bCanCollision_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bCanCollision;
	static void NewProp_bCanLock_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bCanLock;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FBXBodyPartConfig>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
};
const UECodeGen_Private::FNamePropertyParams Z_Construct_UScriptStruct_FBXBodyPartConfig_Statics::NewProp_ComponentName = { "ComponentName", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXBodyPartConfig, ComponentName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ComponentName_MetaData), NewProp_ComponentName_MetaData) };
const UECodeGen_Private::FNamePropertyParams Z_Construct_UScriptStruct_FBXBodyPartConfig_Statics::NewProp_BoneNames_Inner = { "BoneNames", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FBXBodyPartConfig_Statics::NewProp_BoneNames = { "BoneNames", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXBodyPartConfig, BoneNames), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_BoneNames_MetaData), NewProp_BoneNames_MetaData) };
void Z_Construct_UScriptStruct_FBXBodyPartConfig_Statics::NewProp_bCanCollision_SetBit(void* Obj)
{
	((FBXBodyPartConfig*)Obj)->bCanCollision = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FBXBodyPartConfig_Statics::NewProp_bCanCollision = { "bCanCollision", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FBXBodyPartConfig), &Z_Construct_UScriptStruct_FBXBodyPartConfig_Statics::NewProp_bCanCollision_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bCanCollision_MetaData), NewProp_bCanCollision_MetaData) };
void Z_Construct_UScriptStruct_FBXBodyPartConfig_Statics::NewProp_bCanLock_SetBit(void* Obj)
{
	((FBXBodyPartConfig*)Obj)->bCanLock = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FBXBodyPartConfig_Statics::NewProp_bCanLock = { "bCanLock", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FBXBodyPartConfig), &Z_Construct_UScriptStruct_FBXBodyPartConfig_Statics::NewProp_bCanLock_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bCanLock_MetaData), NewProp_bCanLock_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FBXBodyPartConfig_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXBodyPartConfig_Statics::NewProp_ComponentName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXBodyPartConfig_Statics::NewProp_BoneNames_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXBodyPartConfig_Statics::NewProp_BoneNames,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXBodyPartConfig_Statics::NewProp_bCanCollision,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXBodyPartConfig_Statics::NewProp_bCanLock,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXBodyPartConfig_Statics::PropPointers) < 2048);
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FBXBodyPartConfig_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_BattleX,
	nullptr,
	&NewStructOps,
	"BXBodyPartConfig",
	Z_Construct_UScriptStruct_FBXBodyPartConfig_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXBodyPartConfig_Statics::PropPointers),
	sizeof(FBXBodyPartConfig),
	alignof(FBXBodyPartConfig),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000001),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXBodyPartConfig_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FBXBodyPartConfig_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FBXBodyPartConfig()
{
	if (!Z_Registration_Info_UScriptStruct_BXBodyPartConfig.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_BXBodyPartConfig.InnerSingleton, Z_Construct_UScriptStruct_FBXBodyPartConfig_Statics::StructParams);
	}
	return Z_Registration_Info_UScriptStruct_BXBodyPartConfig.InnerSingleton;
}
// End ScriptStruct FBXBodyPartConfig

// Begin ScriptStruct FBXBodyPartRTInformation
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_BXBodyPartRTInformation;
class UScriptStruct* FBXBodyPartRTInformation::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_BXBodyPartRTInformation.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_BXBodyPartRTInformation.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FBXBodyPartRTInformation, (UObject*)Z_Construct_UPackage__Script_BattleX(), TEXT("BXBodyPartRTInformation"));
	}
	return Z_Registration_Info_UScriptStruct_BXBodyPartRTInformation.OuterSingleton;
}
template<> BATTLEX_API UScriptStruct* StaticStruct<FBXBodyPartRTInformation>()
{
	return FBXBodyPartRTInformation::StaticStruct();
}
struct Z_Construct_UScriptStruct_FBXBodyPartRTInformation_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe8\xba\xab\xe4\xbd\x93\xe9\x83\xa8\xe4\xbd\x8d\xe8\xbf\x90\xe8\xa1\x8c\xe4\xbf\xa1\xe6\x81\xaf\n" },
#endif
		{ "ModuleRelativePath", "BXStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe8\xba\xab\xe4\xbd\x93\xe9\x83\xa8\xe4\xbd\x8d\xe8\xbf\x90\xe8\xa1\x8c\xe4\xbf\xa1\xe6\x81\xaf" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_PartType_MetaData[] = {
		{ "Category", "BXBodyPartRTInformation" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe9\x83\xa8\xe4\xbd\x8d\xe7\xb1\xbb\xe5\x9e\x8b\n" },
#endif
		{ "ModuleRelativePath", "BXStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe9\x83\xa8\xe4\xbd\x8d\xe7\xb1\xbb\xe5\x9e\x8b" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Component_MetaData[] = {
		{ "Category", "BXBodyPartRTInformation" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe7\xbb\x84\xe4\xbb\xb6\xe5\xaf\xb9\xe8\xb1\xa1\n" },
#endif
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "BXStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe7\xbb\x84\xe4\xbb\xb6\xe5\xaf\xb9\xe8\xb1\xa1" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_BoneNames_MetaData[] = {
		{ "Category", "BXBodyPartRTInformation" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe9\xaa\xa8\xe9\xaa\xbc\xe5\x88\x97\xe8\xa1\xa8\n" },
#endif
		{ "ModuleRelativePath", "BXStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe9\xaa\xa8\xe9\xaa\xbc\xe5\x88\x97\xe8\xa1\xa8" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bCanCollision_MetaData[] = {
		{ "Category", "BXBodyPartRTInformation" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe6\x98\xaf\xe5\x90\xa6\xe5\x85\x81\xe8\xae\xb8\xe7\xa2\xb0\xe6\x92\x9e\n" },
#endif
		{ "ModuleRelativePath", "BXStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe6\x98\xaf\xe5\x90\xa6\xe5\x85\x81\xe8\xae\xb8\xe7\xa2\xb0\xe6\x92\x9e" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bCanLock_MetaData[] = {
		{ "Category", "BXBodyPartRTInformation" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe6\x98\xaf\xe5\x90\xa6\xe5\x8f\xaf\xe4\xbb\xa5\xe9\x94\x81\xe5\xae\x9a\n" },
#endif
		{ "ModuleRelativePath", "BXStructs.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe6\x98\xaf\xe5\x90\xa6\xe5\x8f\xaf\xe4\xbb\xa5\xe9\x94\x81\xe5\xae\x9a" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FBytePropertyParams NewProp_PartType_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_PartType;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Component;
	static const UECodeGen_Private::FNamePropertyParams NewProp_BoneNames_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_BoneNames;
	static void NewProp_bCanCollision_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bCanCollision;
	static void NewProp_bCanLock_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bCanLock;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FBXBodyPartRTInformation>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
};
const UECodeGen_Private::FBytePropertyParams Z_Construct_UScriptStruct_FBXBodyPartRTInformation_Statics::NewProp_PartType_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UScriptStruct_FBXBodyPartRTInformation_Statics::NewProp_PartType = { "PartType", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXBodyPartRTInformation, PartType), Z_Construct_UEnum_BattleX_EBXBodyPartType, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_PartType_MetaData), NewProp_PartType_MetaData) }; // 84625349
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UScriptStruct_FBXBodyPartRTInformation_Statics::NewProp_Component = { "Component", nullptr, (EPropertyFlags)0x001000000008000d, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXBodyPartRTInformation, Component), Z_Construct_UClass_USceneComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Component_MetaData), NewProp_Component_MetaData) };
const UECodeGen_Private::FNamePropertyParams Z_Construct_UScriptStruct_FBXBodyPartRTInformation_Statics::NewProp_BoneNames_Inner = { "BoneNames", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FBXBodyPartRTInformation_Statics::NewProp_BoneNames = { "BoneNames", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXBodyPartRTInformation, BoneNames), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_BoneNames_MetaData), NewProp_BoneNames_MetaData) };
void Z_Construct_UScriptStruct_FBXBodyPartRTInformation_Statics::NewProp_bCanCollision_SetBit(void* Obj)
{
	((FBXBodyPartRTInformation*)Obj)->bCanCollision = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FBXBodyPartRTInformation_Statics::NewProp_bCanCollision = { "bCanCollision", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FBXBodyPartRTInformation), &Z_Construct_UScriptStruct_FBXBodyPartRTInformation_Statics::NewProp_bCanCollision_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bCanCollision_MetaData), NewProp_bCanCollision_MetaData) };
void Z_Construct_UScriptStruct_FBXBodyPartRTInformation_Statics::NewProp_bCanLock_SetBit(void* Obj)
{
	((FBXBodyPartRTInformation*)Obj)->bCanLock = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FBXBodyPartRTInformation_Statics::NewProp_bCanLock = { "bCanLock", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FBXBodyPartRTInformation), &Z_Construct_UScriptStruct_FBXBodyPartRTInformation_Statics::NewProp_bCanLock_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bCanLock_MetaData), NewProp_bCanLock_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FBXBodyPartRTInformation_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXBodyPartRTInformation_Statics::NewProp_PartType_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXBodyPartRTInformation_Statics::NewProp_PartType,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXBodyPartRTInformation_Statics::NewProp_Component,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXBodyPartRTInformation_Statics::NewProp_BoneNames_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXBodyPartRTInformation_Statics::NewProp_BoneNames,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXBodyPartRTInformation_Statics::NewProp_bCanCollision,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXBodyPartRTInformation_Statics::NewProp_bCanLock,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXBodyPartRTInformation_Statics::PropPointers) < 2048);
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FBXBodyPartRTInformation_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_BattleX,
	nullptr,
	&NewStructOps,
	"BXBodyPartRTInformation",
	Z_Construct_UScriptStruct_FBXBodyPartRTInformation_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXBodyPartRTInformation_Statics::PropPointers),
	sizeof(FBXBodyPartRTInformation),
	alignof(FBXBodyPartRTInformation),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000005),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXBodyPartRTInformation_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FBXBodyPartRTInformation_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FBXBodyPartRTInformation()
{
	if (!Z_Registration_Info_UScriptStruct_BXBodyPartRTInformation.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_BXBodyPartRTInformation.InnerSingleton, Z_Construct_UScriptStruct_FBXBodyPartRTInformation_Statics::StructParams);
	}
	return Z_Registration_Info_UScriptStruct_BXBodyPartRTInformation.InnerSingleton;
}
// End ScriptStruct FBXBodyPartRTInformation

// Begin Registration
struct Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_BXStructs_h_Statics
{
	static constexpr FStructRegisterCompiledInInfo ScriptStructInfo[] = {
		{ FBXObjectList::StaticStruct, Z_Construct_UScriptStruct_FBXObjectList_Statics::NewStructOps, TEXT("BXObjectList"), &Z_Registration_Info_UScriptStruct_BXObjectList, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FBXObjectList), 1490345823U) },
		{ FBXActorList::StaticStruct, Z_Construct_UScriptStruct_FBXActorList_Statics::NewStructOps, TEXT("BXActorList"), &Z_Registration_Info_UScriptStruct_BXActorList, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FBXActorList), 2162323314U) },
		{ FBXBoneSelector::StaticStruct, Z_Construct_UScriptStruct_FBXBoneSelector_Statics::NewStructOps, TEXT("BXBoneSelector"), &Z_Registration_Info_UScriptStruct_BXBoneSelector, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FBXBoneSelector), 1217301433U) },
		{ FBXMeshCreater::StaticStruct, Z_Construct_UScriptStruct_FBXMeshCreater_Statics::NewStructOps, TEXT("BXMeshCreater"), &Z_Registration_Info_UScriptStruct_BXMeshCreater, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FBXMeshCreater), 3636661294U) },
		{ FBXCollisionChannelInfo::StaticStruct, Z_Construct_UScriptStruct_FBXCollisionChannelInfo_Statics::NewStructOps, TEXT("BXCollisionChannelInfo"), &Z_Registration_Info_UScriptStruct_BXCollisionChannelInfo, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FBXCollisionChannelInfo), 221254959U) },
		{ FBXAttributeComparator::StaticStruct, Z_Construct_UScriptStruct_FBXAttributeComparator_Statics::NewStructOps, TEXT("BXAttributeComparator"), &Z_Registration_Info_UScriptStruct_BXAttributeComparator, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FBXAttributeComparator), 3281343516U) },
		{ FBXChangeBuff::StaticStruct, Z_Construct_UScriptStruct_FBXChangeBuff_Statics::NewStructOps, TEXT("BXChangeBuff"), &Z_Registration_Info_UScriptStruct_BXChangeBuff, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FBXChangeBuff), 888687332U) },
		{ FBXShapeInformation::StaticStruct, Z_Construct_UScriptStruct_FBXShapeInformation_Statics::NewStructOps, TEXT("BXShapeInformation"), &Z_Registration_Info_UScriptStruct_BXShapeInformation, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FBXShapeInformation), 3264309202U) },
		{ FBXTimelineAssetInformation::StaticStruct, Z_Construct_UScriptStruct_FBXTimelineAssetInformation_Statics::NewStructOps, TEXT("BXTimelineAssetInformation"), &Z_Registration_Info_UScriptStruct_BXTimelineAssetInformation, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FBXTimelineAssetInformation), 2326295132U) },
		{ FBXBodyPartSelection::StaticStruct, Z_Construct_UScriptStruct_FBXBodyPartSelection_Statics::NewStructOps, TEXT("BXBodyPartSelection"), &Z_Registration_Info_UScriptStruct_BXBodyPartSelection, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FBXBodyPartSelection), 1620284852U) },
		{ FBXBodyPartConfig::StaticStruct, Z_Construct_UScriptStruct_FBXBodyPartConfig_Statics::NewStructOps, TEXT("BXBodyPartConfig"), &Z_Registration_Info_UScriptStruct_BXBodyPartConfig, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FBXBodyPartConfig), 3005846379U) },
		{ FBXBodyPartRTInformation::StaticStruct, Z_Construct_UScriptStruct_FBXBodyPartRTInformation_Statics::NewStructOps, TEXT("BXBodyPartRTInformation"), &Z_Registration_Info_UScriptStruct_BXBodyPartRTInformation, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FBXBodyPartRTInformation), 80757500U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_BXStructs_h_769434989(TEXT("/Script/BattleX"),
	nullptr, 0,
	Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_BXStructs_h_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_BXStructs_h_Statics::ScriptStructInfo),
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
