// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "BattleXEditor/TimelineEditor/Graph/BXTLGraphNode.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeBXTLGraphNode() {}

// Begin Cross Module References
BATTLEX_API UClass* Z_Construct_UClass_UBXTask_NoRegister();
BATTLEXEDITOR_API UClass* Z_Construct_UClass_UBXTLGraphNode();
BATTLEXEDITOR_API UClass* Z_Construct_UClass_UBXTLGraphNode_NoRegister();
BATTLEXEDITOR_API UScriptStruct* Z_Construct_UScriptStruct_FBXTLGNodePin();
ENGINE_API UClass* Z_Construct_UClass_UEdGraphNode();
UPackage* Z_Construct_UPackage__Script_BattleXEditor();
// End Cross Module References

// Begin ScriptStruct FBXTLGNodePin
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_BXTLGNodePin;
class UScriptStruct* FBXTLGNodePin::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_BXTLGNodePin.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_BXTLGNodePin.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FBXTLGNodePin, (UObject*)Z_Construct_UPackage__Script_BattleXEditor(), TEXT("BXTLGNodePin"));
	}
	return Z_Registration_Info_UScriptStruct_BXTLGNodePin.OuterSingleton;
}
template<> BATTLEXEDITOR_API UScriptStruct* StaticStruct<FBXTLGNodePin>()
{
	return FBXTLGNodePin::StaticStruct();
}
struct Z_Construct_UScriptStruct_FBXTLGNodePin_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "TimelineEditor/Graph/BXTLGraphNode.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_PinType_MetaData[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/*\n\x09* 0:Task\xe6\x89\xa7\xe8\xa1\x8c\n\x09* 1:Task\xe4\xba\x8b\xe4\xbb\xb6\n\x09* 2:Task\xe7\xa2\xb0\xe6\x92\x9e\xe8\xbe\x93\xe5\x85\xa5\n \x09* 3:Task\xe8\xbe\x93\xe5\x85\xa5\xe6\x95\xb0\xe6\x8d\xae\n\x09* 4:Task\xe8\xbe\x93\xe5\x87\xba\xe6\x95\xb0\xe6\x8d\xae\n\x09*/" },
#endif
		{ "ModuleRelativePath", "TimelineEditor/Graph/BXTLGraphNode.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "* 0:Task\xe6\x89\xa7\xe8\xa1\x8c\n* 1:Task\xe4\xba\x8b\xe4\xbb\xb6\n* 2:Task\xe7\xa2\xb0\xe6\x92\x9e\xe8\xbe\x93\xe5\x85\xa5\n* 3:Task\xe8\xbe\x93\xe5\x85\xa5\xe6\x95\xb0\xe6\x8d\xae\n* 4:Task\xe8\xbe\x93\xe5\x87\xba\xe6\x95\xb0\xe6\x8d\xae" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_UniqueID_MetaData[] = {
		{ "ModuleRelativePath", "TimelineEditor/Graph/BXTLGraphNode.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ExtraName_MetaData[] = {
		{ "ModuleRelativePath", "TimelineEditor/Graph/BXTLGraphNode.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_PinGUIDName_MetaData[] = {
		{ "ModuleRelativePath", "TimelineEditor/Graph/BXTLGraphNode.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FBytePropertyParams NewProp_PinType;
	static const UECodeGen_Private::FInt64PropertyParams NewProp_UniqueID;
	static const UECodeGen_Private::FNamePropertyParams NewProp_ExtraName;
	static const UECodeGen_Private::FNamePropertyParams NewProp_PinGUIDName;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FBXTLGNodePin>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
};
const UECodeGen_Private::FBytePropertyParams Z_Construct_UScriptStruct_FBXTLGNodePin_Statics::NewProp_PinType = { "PinType", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTLGNodePin, PinType), nullptr, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_PinType_MetaData), NewProp_PinType_MetaData) };
const UECodeGen_Private::FInt64PropertyParams Z_Construct_UScriptStruct_FBXTLGNodePin_Statics::NewProp_UniqueID = { "UniqueID", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Int64, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTLGNodePin, UniqueID), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_UniqueID_MetaData), NewProp_UniqueID_MetaData) };
const UECodeGen_Private::FNamePropertyParams Z_Construct_UScriptStruct_FBXTLGNodePin_Statics::NewProp_ExtraName = { "ExtraName", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTLGNodePin, ExtraName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ExtraName_MetaData), NewProp_ExtraName_MetaData) };
const UECodeGen_Private::FNamePropertyParams Z_Construct_UScriptStruct_FBXTLGNodePin_Statics::NewProp_PinGUIDName = { "PinGUIDName", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FBXTLGNodePin, PinGUIDName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_PinGUIDName_MetaData), NewProp_PinGUIDName_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FBXTLGNodePin_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLGNodePin_Statics::NewProp_PinType,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLGNodePin_Statics::NewProp_UniqueID,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLGNodePin_Statics::NewProp_ExtraName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBXTLGNodePin_Statics::NewProp_PinGUIDName,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXTLGNodePin_Statics::PropPointers) < 2048);
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FBXTLGNodePin_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_BattleXEditor,
	nullptr,
	&NewStructOps,
	"BXTLGNodePin",
	Z_Construct_UScriptStruct_FBXTLGNodePin_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXTLGNodePin_Statics::PropPointers),
	sizeof(FBXTLGNodePin),
	alignof(FBXTLGNodePin),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000001),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXTLGNodePin_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FBXTLGNodePin_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FBXTLGNodePin()
{
	if (!Z_Registration_Info_UScriptStruct_BXTLGNodePin.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_BXTLGNodePin.InnerSingleton, Z_Construct_UScriptStruct_FBXTLGNodePin_Statics::StructParams);
	}
	return Z_Registration_Info_UScriptStruct_BXTLGNodePin.InnerSingleton;
}
// End ScriptStruct FBXTLGNodePin

// Begin Class UBXTLGraphNode
void UBXTLGraphNode::StaticRegisterNativesUBXTLGraphNode()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UBXTLGraphNode);
UClass* Z_Construct_UClass_UBXTLGraphNode_NoRegister()
{
	return UBXTLGraphNode::StaticClass();
}
struct Z_Construct_UClass_UBXTLGraphNode_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "TimelineEditor/Graph/BXTLGraphNode.h" },
		{ "ModuleRelativePath", "TimelineEditor/Graph/BXTLGraphNode.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_CachedTask_MetaData[] = {
		{ "ModuleRelativePath", "TimelineEditor/Graph/BXTLGraphNode.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_PinInformations_MetaData[] = {
		{ "ModuleRelativePath", "TimelineEditor/Graph/BXTLGraphNode.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_CachedTask;
	static const UECodeGen_Private::FStructPropertyParams NewProp_PinInformations_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_PinInformations;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UBXTLGraphNode>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UBXTLGraphNode_Statics::NewProp_CachedTask = { "CachedTask", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXTLGraphNode, CachedTask), Z_Construct_UClass_UBXTask_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_CachedTask_MetaData), NewProp_CachedTask_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UBXTLGraphNode_Statics::NewProp_PinInformations_Inner = { "PinInformations", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UScriptStruct_FBXTLGNodePin, METADATA_PARAMS(0, nullptr) }; // 909551963
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UClass_UBXTLGraphNode_Statics::NewProp_PinInformations = { "PinInformations", nullptr, (EPropertyFlags)0x0040000000000000, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UBXTLGraphNode, PinInformations), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_PinInformations_MetaData), NewProp_PinInformations_MetaData) }; // 909551963
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UBXTLGraphNode_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXTLGraphNode_Statics::NewProp_CachedTask,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXTLGraphNode_Statics::NewProp_PinInformations_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBXTLGraphNode_Statics::NewProp_PinInformations,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UBXTLGraphNode_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_UBXTLGraphNode_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UEdGraphNode,
	(UObject* (*)())Z_Construct_UPackage__Script_BattleXEditor,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UBXTLGraphNode_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UBXTLGraphNode_Statics::ClassParams = {
	&UBXTLGraphNode::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_UBXTLGraphNode_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_UBXTLGraphNode_Statics::PropPointers),
	0,
	0x000800A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UBXTLGraphNode_Statics::Class_MetaDataParams), Z_Construct_UClass_UBXTLGraphNode_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UBXTLGraphNode()
{
	if (!Z_Registration_Info_UClass_UBXTLGraphNode.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UBXTLGraphNode.OuterSingleton, Z_Construct_UClass_UBXTLGraphNode_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UBXTLGraphNode.OuterSingleton;
}
template<> BATTLEXEDITOR_API UClass* StaticClass<UBXTLGraphNode>()
{
	return UBXTLGraphNode::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(UBXTLGraphNode);
// End Class UBXTLGraphNode

// Begin Registration
struct Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_Graph_BXTLGraphNode_h_Statics
{
	static constexpr FStructRegisterCompiledInInfo ScriptStructInfo[] = {
		{ FBXTLGNodePin::StaticStruct, Z_Construct_UScriptStruct_FBXTLGNodePin_Statics::NewStructOps, TEXT("BXTLGNodePin"), &Z_Registration_Info_UScriptStruct_BXTLGNodePin, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FBXTLGNodePin), 909551963U) },
	};
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UBXTLGraphNode, UBXTLGraphNode::StaticClass, TEXT("UBXTLGraphNode"), &Z_Registration_Info_UClass_UBXTLGraphNode, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UBXTLGraphNode), 1969107144U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_Graph_BXTLGraphNode_h_2852001893(TEXT("/Script/BattleXEditor"),
	Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_Graph_BXTLGraphNode_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_Graph_BXTLGraphNode_h_Statics::ClassInfo),
	Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_Graph_BXTLGraphNode_h_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_Graph_BXTLGraphNode_h_Statics::ScriptStructInfo),
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
