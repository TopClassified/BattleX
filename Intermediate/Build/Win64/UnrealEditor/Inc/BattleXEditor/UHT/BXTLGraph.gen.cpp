// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "BattleXEditor/TimelineEditor/Graph/BXTLGraph.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeBXTLGraph() {}

// Begin Cross Module References
BATTLEXEDITOR_API UClass* Z_Construct_UClass_UBXTLGraph();
BATTLEXEDITOR_API UClass* Z_Construct_UClass_UBXTLGraph_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_UEdGraph();
UPackage* Z_Construct_UPackage__Script_BattleXEditor();
// End Cross Module References

// Begin Class UBXTLGraph
void UBXTLGraph::StaticRegisterNativesUBXTLGraph()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UBXTLGraph);
UClass* Z_Construct_UClass_UBXTLGraph_NoRegister()
{
	return UBXTLGraph::StaticClass();
}
struct Z_Construct_UClass_UBXTLGraph_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "TimelineEditor/Graph/BXTLGraph.h" },
		{ "ModuleRelativePath", "TimelineEditor/Graph/BXTLGraph.h" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UBXTLGraph>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_UBXTLGraph_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UEdGraph,
	(UObject* (*)())Z_Construct_UPackage__Script_BattleXEditor,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UBXTLGraph_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UBXTLGraph_Statics::ClassParams = {
	&UBXTLGraph::StaticClass,
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
	0x000000A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UBXTLGraph_Statics::Class_MetaDataParams), Z_Construct_UClass_UBXTLGraph_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UBXTLGraph()
{
	if (!Z_Registration_Info_UClass_UBXTLGraph.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UBXTLGraph.OuterSingleton, Z_Construct_UClass_UBXTLGraph_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UBXTLGraph.OuterSingleton;
}
template<> BATTLEXEDITOR_API UClass* StaticClass<UBXTLGraph>()
{
	return UBXTLGraph::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(UBXTLGraph);
// End Class UBXTLGraph

// Begin Registration
struct Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_Graph_BXTLGraph_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UBXTLGraph, UBXTLGraph::StaticClass, TEXT("UBXTLGraph"), &Z_Registration_Info_UClass_UBXTLGraph, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UBXTLGraph), 4104185080U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_Graph_BXTLGraph_h_4033969864(TEXT("/Script/BattleXEditor"),
	Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_Graph_BXTLGraph_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_Graph_BXTLGraph_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
