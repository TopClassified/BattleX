// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "Task/BXTProcessor.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class UBXTask;
enum class EBXTLFinishReason : uint8;
struct FBXTLRunTimeData;
struct FBXTLSectionRTData;
struct FBXTLTaskRTData;
#ifdef BATTLEX_BXTProcessor_generated_h
#error "BXTProcessor.generated.h already included, missing '#pragma once' in BXTProcessor.h"
#endif
#define BATTLEX_BXTProcessor_generated_h

#define FID_Lyra_Plugins_BattleX_Source_BattleX_Task_BXTProcessor_h_20_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execAddPendingTask); \
	DECLARE_FUNCTION(execIsTaskCompleted); \
	DECLARE_FUNCTION(execEndTask); \
	DECLARE_FUNCTION(execUpdateTask); \
	DECLARE_FUNCTION(execStartTask);


#define FID_Lyra_Plugins_BattleX_Source_BattleX_Task_BXTProcessor_h_20_CALLBACK_WRAPPERS
#define FID_Lyra_Plugins_BattleX_Source_BattleX_Task_BXTProcessor_h_20_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUBXTProcessor(); \
	friend struct Z_Construct_UClass_UBXTProcessor_Statics; \
public: \
	DECLARE_CLASS(UBXTProcessor, UObject, COMPILED_IN_FLAGS(CLASS_Abstract | CLASS_Transient), CASTCLASS_None, TEXT("/Script/BattleX"), NO_API) \
	DECLARE_SERIALIZER(UBXTProcessor)


#define FID_Lyra_Plugins_BattleX_Source_BattleX_Task_BXTProcessor_h_20_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UBXTProcessor(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	UBXTProcessor(UBXTProcessor&&); \
	UBXTProcessor(const UBXTProcessor&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UBXTProcessor); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UBXTProcessor); \
	DEFINE_ABSTRACT_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UBXTProcessor) \
	NO_API virtual ~UBXTProcessor();


#define FID_Lyra_Plugins_BattleX_Source_BattleX_Task_BXTProcessor_h_17_PROLOG
#define FID_Lyra_Plugins_BattleX_Source_BattleX_Task_BXTProcessor_h_20_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Lyra_Plugins_BattleX_Source_BattleX_Task_BXTProcessor_h_20_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_Lyra_Plugins_BattleX_Source_BattleX_Task_BXTProcessor_h_20_CALLBACK_WRAPPERS \
	FID_Lyra_Plugins_BattleX_Source_BattleX_Task_BXTProcessor_h_20_INCLASS_NO_PURE_DECLS \
	FID_Lyra_Plugins_BattleX_Source_BattleX_Task_BXTProcessor_h_20_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> BATTLEX_API UClass* StaticClass<class UBXTProcessor>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Lyra_Plugins_BattleX_Source_BattleX_Task_BXTProcessor_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
