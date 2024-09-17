// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "TimelineEditor/BXTLEditorTemplate.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef BATTLEXEDITOR_BXTLEditorTemplate_generated_h
#error "BXTLEditorTemplate.generated.h already included, missing '#pragma once' in BXTLEditorTemplate.h"
#endif
#define BATTLEXEDITOR_BXTLEditorTemplate_generated_h

#define FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_BXTLEditorTemplate_h_17_GENERATED_BODY \
	friend struct Z_Construct_UScriptStruct_FBXTLTaskGroupInformation_Statics; \
	BATTLEXEDITOR_API static class UScriptStruct* StaticStruct();


template<> BATTLEXEDITOR_API UScriptStruct* StaticStruct<struct FBXTLTaskGroupInformation>();

#define FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_BXTLEditorTemplate_h_37_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUBXTLTaskGroupTemplate(); \
	friend struct Z_Construct_UClass_UBXTLTaskGroupTemplate_Statics; \
public: \
	DECLARE_CLASS(UBXTLTaskGroupTemplate, UPrimaryDataAsset, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/BattleXEditor"), NO_API) \
	DECLARE_SERIALIZER(UBXTLTaskGroupTemplate)


#define FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_BXTLEditorTemplate_h_37_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UBXTLTaskGroupTemplate(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	UBXTLTaskGroupTemplate(UBXTLTaskGroupTemplate&&); \
	UBXTLTaskGroupTemplate(const UBXTLTaskGroupTemplate&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UBXTLTaskGroupTemplate); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UBXTLTaskGroupTemplate); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UBXTLTaskGroupTemplate) \
	NO_API virtual ~UBXTLTaskGroupTemplate();


#define FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_BXTLEditorTemplate_h_34_PROLOG
#define FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_BXTLEditorTemplate_h_37_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_BXTLEditorTemplate_h_37_INCLASS_NO_PURE_DECLS \
	FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_BXTLEditorTemplate_h_37_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> BATTLEXEDITOR_API UClass* StaticClass<class UBXTLTaskGroupTemplate>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Lyra_Plugins_BattleX_Source_BattleXEditor_TimelineEditor_BXTLEditorTemplate_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
