// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "Task/BXTask.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class UObject;
struct FBXTLPreviewObjectData;
struct FSoftObjectPath;
#ifdef BATTLEX_BXTask_generated_h
#error "BXTask.generated.h already included, missing '#pragma once' in BXTask.h"
#endif
#define BATTLEX_BXTask_generated_h

#if WITH_EDITOR
#define FID_Lyra_Plugins_BattleX_Source_BattleX_Task_BXTask_h_16_RPC_WRAPPERS_NO_PURE_DECLS_EOD \
	virtual void RefreshDataByPreviewObject_Implementation(UObject* InObject, FBXTLPreviewObjectData const& InData); \
	DECLARE_FUNCTION(execRefreshDataByPreviewObject);
#else // WITH_EDITOR
#define FID_Lyra_Plugins_BattleX_Source_BattleX_Task_BXTask_h_16_RPC_WRAPPERS_NO_PURE_DECLS_EOD
#endif // WITH_EDITOR


#define FID_Lyra_Plugins_BattleX_Source_BattleX_Task_BXTask_h_16_CALLBACK_WRAPPERS
#define FID_Lyra_Plugins_BattleX_Source_BattleX_Task_BXTask_h_16_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUBXTask(); \
	friend struct Z_Construct_UClass_UBXTask_Statics; \
public: \
	DECLARE_CLASS(UBXTask, UObject, COMPILED_IN_FLAGS(CLASS_Abstract), CASTCLASS_None, TEXT("/Script/BattleX"), NO_API) \
	DECLARE_SERIALIZER(UBXTask)


#define FID_Lyra_Plugins_BattleX_Source_BattleX_Task_BXTask_h_16_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	UBXTask(UBXTask&&); \
	UBXTask(const UBXTask&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UBXTask); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UBXTask); \
	DEFINE_ABSTRACT_DEFAULT_CONSTRUCTOR_CALL(UBXTask) \
	NO_API virtual ~UBXTask();


#define FID_Lyra_Plugins_BattleX_Source_BattleX_Task_BXTask_h_13_PROLOG
#define FID_Lyra_Plugins_BattleX_Source_BattleX_Task_BXTask_h_16_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Lyra_Plugins_BattleX_Source_BattleX_Task_BXTask_h_16_RPC_WRAPPERS_NO_PURE_DECLS_EOD \
	FID_Lyra_Plugins_BattleX_Source_BattleX_Task_BXTask_h_16_CALLBACK_WRAPPERS \
	FID_Lyra_Plugins_BattleX_Source_BattleX_Task_BXTask_h_16_INCLASS_NO_PURE_DECLS \
	FID_Lyra_Plugins_BattleX_Source_BattleX_Task_BXTask_h_16_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> BATTLEX_API UClass* StaticClass<class UBXTask>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Lyra_Plugins_BattleX_Source_BattleX_Task_BXTask_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
