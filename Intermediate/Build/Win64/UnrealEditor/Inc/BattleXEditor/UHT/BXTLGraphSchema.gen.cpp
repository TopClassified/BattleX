// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "BattleXEditor/TimelineEditor/Graph/BXTLGraphSchema.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeBXTLGraphSchema() {}

// Begin Cross Module References
BATTLEXEDITOR_API UClass* Z_Construct_UClass_UBXTLGraphSchema();
BATTLEXEDITOR_API UClass* Z_Construct_UClass_UBXTLGraphSchema_NoRegister();
BATTLEXEDITOR_API UScriptStruct* Z_Construct_UScriptStruct_FBXTLGCreatTaskNode();
ENGINE_API UClass* Z_Construct_UClass_UEdGraphSchema();
ENGINE_API UScriptStruct* Z_Construct_UScriptStruct_FEdGraphSchemaAction();
UPackage* Z_Construct_UPackage__Script_BattleXEditor();
// End Cross Module References

// Begin ScriptStruct FBXTLGCreatTaskNode
static_assert(std::is_polymorphic<FBXTLGCreatTaskNode>() == std::is_polymorphic<FEdGraphSchemaAction>(), "USTRUCT FBXTLGCreatTaskNode cannot be polymorphic unless super FEdGraphSchemaAction is polymorphic");
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_BXTLGCreatTaskNode;
class UScriptStruct* FBXTLGCreatTaskNode::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_BXTLGCreatTaskNode.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_BXTLGCreatTaskNode.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FBXTLGCreatTaskNode, (UObject*)Z_Construct_UPackage__Script_BattleXEditor(), TEXT("BXTLGCreatTaskNode"));
	}
	return Z_Registration_Info_UScriptStruct_BXTLGCreatTaskNode.OuterSingleton;
}
template<> BATTLEXEDITOR_API UScriptStruct* StaticStruct<FBXTLGCreatTaskNode>()
{
	return FBXTLGCreatTaskNode::StaticStruct();
}
struct Z_Construct_UScriptStruct_FBXTLGCreatTaskNode_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "ModuleRelativePath", "TimelineEditor/Graph/BXTLGraphSchema.h" },
	};
#endif // WITH_METADATA
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FBXTLGCreatTaskNode>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
};
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FBXTLGCreatTaskNode_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_BattleXEditor,
	Z_Construct_UScriptStruct_FEdGraphSchemaAction,
	&NewStructOps,
	"BXTLGCreatTaskNode",
	nullptr,
	0,
	sizeof(FBXTLGCreatTaskNode),
	alignof(FBXTLGCreatTaskNode),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000001),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBXTLGCreatTaskNode_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FBXTLGCreatTaskNode_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FBXTLGCreatTaskNode()
{
	if (!Z_Registration_Info_UScriptStruct_BXTLGCreatTaskNode.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_BXTLGCreatTaskNode.InnerSingleton, Z_Construct_UScriptStruct_FBXTLGCreatTaskNode_Statics::StructParams);
	}
	return Z_Registration_Info_UScriptStruct_BXTLGCreatTaskNode.InnerSingleton;
}
// End ScriptStruct FBXTLGCreatTaskNode

// Begin Class UBXTLGraphSchema
void UBXTLGraphSchema::StaticRegisterNativesUBXTLGraphSchema()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UBXTLGraphSchema);
UClass* Z_Construct_UClass_UBXTLGraphSchema_NoRegister()
{
	return UBXTLGraphSchema::StaticClass();
}
struct Z_Construct_UClass_UBXTLGraphSchema_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "TimelineEditor/Graph/BXTLGraphSchema.h" },
		{ "ModuleRelativePath", "TimelineEditor/Graph/BXTLGraphSchema.h" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UBXTLGraphSchema>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_UBXTLGraphSchema_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UEdGraphSchema,
	(UObject* (*)())Z_Construct_UPackage__Script_BattleXEditor,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UBXTLGraphSchema_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UBXTLGraphSchema_Statics::ClassParams = {
	&UBXTLGraphSchema::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	nullptr,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	0,
	0,
	0x000800A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UBXTLGraphSchema_Statics::Class_MetaDataParams), Z_Construct_UClass_UBXTLGraphSchema_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UBXTLGraphSchema()
{
	if (!Z_Registration_Info_UClass_UBXTLGraphSchema.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UBXTLGraphSchema.OuterSingleton, Z_Construct_UClass_UBXTLGraphSchema_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UBXTLGraphSchema.OuterSingleton;
}
template<> BATTLEXEDITOR_API UClass* StaticClass<UBXTLGraphSchema>()
{
	return UBXTLGraphSchema::StaticClass();
}
UBXTLGraphSchema::UBXTLGraphSchema(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UBXTLGraphSchema);
UBXTLGraphSchema::~UBXTLGraphSchema() {}
// End Class UBXTLGraphSchema

// Begin Registration
struct Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_Graph_BXTLGraphSchema_h_Statics
{
	static constexpr FStructRegisterCompiledInInfo ScriptStructInfo[] = {
		{ FBXTLGCreatTaskNode::StaticStruct, Z_Construct_UScriptStruct_FBXTLGCreatTaskNode_Statics::NewStructOps, TEXT("BXTLGCreatTaskNode"), &Z_Registration_Info_UScriptStruct_BXTLGCreatTaskNode, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FBXTLGCreatTaskNode), 1547184755U) },
	};
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UBXTLGraphSchema, UBXTLGraphSchema::StaticClass, TEXT("UBXTLGraphSchema"), &Z_Registration_Info_UClass_UBXTLGraphSchema, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UBXTLGraphSchema), 4259108354U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_Graph_BXTLGraphSchema_h_1958398878(TEXT("/Script/BattleXEditor"),
	Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_Graph_BXTLGraphSchema_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_Graph_BXTLGraphSchema_h_Statics::ClassInfo),
	Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_Graph_BXTLGraphSchema_h_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_Graph_BXTLGraphSchema_h_Statics::ScriptStructInfo),
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
