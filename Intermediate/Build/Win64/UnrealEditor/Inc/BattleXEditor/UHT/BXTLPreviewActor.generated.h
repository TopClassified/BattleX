// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "TimelineEditor/Preview/BXTLPreviewActor.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class AActor;
class UWorld;
#ifdef BATTLEXEDITOR_BXTLPreviewActor_generated_h
#error "BXTLPreviewActor.generated.h already included, missing '#pragma once' in BXTLPreviewActor.h"
#endif
#define BATTLEXEDITOR_BXTLPreviewActor_generated_h

#define FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_Preview_BXTLPreviewActor_h_16_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execCreateActor);


#define FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_Preview_BXTLPreviewActor_h_16_CALLBACK_WRAPPERS
#define FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_Preview_BXTLPreviewActor_h_16_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUBXTLPreviewActor(); \
	friend struct Z_Construct_UClass_UBXTLPreviewActor_Statics; \
public: \
	DECLARE_CLASS(UBXTLPreviewActor, UObject, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/BattleXEditor"), NO_API) \
	DECLARE_SERIALIZER(UBXTLPreviewActor)


#define FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_Preview_BXTLPreviewActor_h_16_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UBXTLPreviewActor(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	UBXTLPreviewActor(UBXTLPreviewActor&&); \
	UBXTLPreviewActor(const UBXTLPreviewActor&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UBXTLPreviewActor); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UBXTLPreviewActor); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UBXTLPreviewActor) \
	NO_API virtual ~UBXTLPreviewActor();


#define FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_Preview_BXTLPreviewActor_h_13_PROLOG
#define FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_Preview_BXTLPreviewActor_h_16_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_Preview_BXTLPreviewActor_h_16_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_Preview_BXTLPreviewActor_h_16_CALLBACK_WRAPPERS \
	FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_Preview_BXTLPreviewActor_h_16_INCLASS_NO_PURE_DECLS \
	FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_Preview_BXTLPreviewActor_h_16_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> BATTLEXEDITOR_API UClass* StaticClass<class UBXTLPreviewActor>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_Preview_BXTLPreviewActor_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
