// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "TimelineEditor/Preview/BXTLPreviewGameState.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef BATTLEXEDITOR_BXTLPreviewGameState_generated_h
#error "BXTLPreviewGameState.generated.h already included, missing '#pragma once' in BXTLPreviewGameState.h"
#endif
#define BATTLEXEDITOR_BXTLPreviewGameState_generated_h

#define FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_Preview_BXTLPreviewGameState_h_13_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesABXTLPreviewGameState(); \
	friend struct Z_Construct_UClass_ABXTLPreviewGameState_Statics; \
public: \
	DECLARE_CLASS(ABXTLPreviewGameState, AGameState, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/BattleXEditor"), NO_API) \
	DECLARE_SERIALIZER(ABXTLPreviewGameState)


#define FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_Preview_BXTLPreviewGameState_h_13_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API ABXTLPreviewGameState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	ABXTLPreviewGameState(ABXTLPreviewGameState&&); \
	ABXTLPreviewGameState(const ABXTLPreviewGameState&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, ABXTLPreviewGameState); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(ABXTLPreviewGameState); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(ABXTLPreviewGameState) \
	NO_API virtual ~ABXTLPreviewGameState();


#define FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_Preview_BXTLPreviewGameState_h_10_PROLOG
#define FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_Preview_BXTLPreviewGameState_h_13_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_Preview_BXTLPreviewGameState_h_13_INCLASS_NO_PURE_DECLS \
	FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_Preview_BXTLPreviewGameState_h_13_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> BATTLEXEDITOR_API UClass* StaticClass<class ABXTLPreviewGameState>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_Preview_BXTLPreviewGameState_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
