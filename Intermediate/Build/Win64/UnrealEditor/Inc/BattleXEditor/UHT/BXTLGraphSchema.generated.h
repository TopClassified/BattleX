// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "TimelineEditor/Graph/BXTLGraphSchema.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef BATTLEXEDITOR_BXTLGraphSchema_generated_h
#error "BXTLGraphSchema.generated.h already included, missing '#pragma once' in BXTLGraphSchema.h"
#endif
#define BATTLEXEDITOR_BXTLGraphSchema_generated_h

#define FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_Graph_BXTLGraphSchema_h_14_GENERATED_BODY \
	friend struct Z_Construct_UScriptStruct_FBXTLGCreatTaskNode_Statics; \
	BATTLEXEDITOR_API static class UScriptStruct* StaticStruct(); \
	typedef FEdGraphSchemaAction Super;


template<> BATTLEXEDITOR_API UScriptStruct* StaticStruct<struct FBXTLGCreatTaskNode>();

#define FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_Graph_BXTLGraphSchema_h_52_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUBXTLGraphSchema(); \
	friend struct Z_Construct_UClass_UBXTLGraphSchema_Statics; \
public: \
	DECLARE_CLASS(UBXTLGraphSchema, UEdGraphSchema, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/BattleXEditor"), BATTLEXEDITOR_API) \
	DECLARE_SERIALIZER(UBXTLGraphSchema)


#define FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_Graph_BXTLGraphSchema_h_52_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	BATTLEXEDITOR_API UBXTLGraphSchema(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	UBXTLGraphSchema(UBXTLGraphSchema&&); \
	UBXTLGraphSchema(const UBXTLGraphSchema&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(BATTLEXEDITOR_API, UBXTLGraphSchema); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UBXTLGraphSchema); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UBXTLGraphSchema) \
	BATTLEXEDITOR_API virtual ~UBXTLGraphSchema();


#define FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_Graph_BXTLGraphSchema_h_49_PROLOG
#define FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_Graph_BXTLGraphSchema_h_52_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_Graph_BXTLGraphSchema_h_52_INCLASS_NO_PURE_DECLS \
	FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_Graph_BXTLGraphSchema_h_52_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> BATTLEXEDITOR_API UClass* StaticClass<class UBXTLGraphSchema>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_Graph_BXTLGraphSchema_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
