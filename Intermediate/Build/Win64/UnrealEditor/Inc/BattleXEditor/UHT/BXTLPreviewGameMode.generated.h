// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "TimelineEditor/Preview/BXTLPreviewGameMode.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef BATTLEXEDITOR_BXTLPreviewGameMode_generated_h
#error "BXTLPreviewGameMode.generated.h already included, missing '#pragma once' in BXTLPreviewGameMode.h"
#endif
#define BATTLEXEDITOR_BXTLPreviewGameMode_generated_h

#define FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_Preview_BXTLPreviewGameMode_h_13_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesABXTLPreviewGameMode(); \
	friend struct Z_Construct_UClass_ABXTLPreviewGameMode_Statics; \
public: \
	DECLARE_CLASS(ABXTLPreviewGameMode, AGameModeBase, COMPILED_IN_FLAGS(0 | CLASS_Transient | CLASS_Config), CASTCLASS_None, TEXT("/Script/BattleXEditor"), BATTLEXEDITOR_API) \
	DECLARE_SERIALIZER(ABXTLPreviewGameMode)


#define FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_Preview_BXTLPreviewGameMode_h_13_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	BATTLEXEDITOR_API ABXTLPreviewGameMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	ABXTLPreviewGameMode(ABXTLPreviewGameMode&&); \
	ABXTLPreviewGameMode(const ABXTLPreviewGameMode&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(BATTLEXEDITOR_API, ABXTLPreviewGameMode); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(ABXTLPreviewGameMode); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(ABXTLPreviewGameMode) \
	BATTLEXEDITOR_API virtual ~ABXTLPreviewGameMode();


#define FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_Preview_BXTLPreviewGameMode_h_10_PROLOG
#define FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_Preview_BXTLPreviewGameMode_h_13_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_Preview_BXTLPreviewGameMode_h_13_INCLASS_NO_PURE_DECLS \
	FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_Preview_BXTLPreviewGameMode_h_13_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> BATTLEXEDITOR_API UClass* StaticClass<class ABXTLPreviewGameMode>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_Preview_BXTLPreviewGameMode_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
