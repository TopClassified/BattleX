// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "BXManager.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class AActor;
class UBXManager;
class UBXTLAsset;
class UObject;
class UWorld;
enum class EBXTLFinishReason : uint8;
struct FBXTLPlayContext;
#ifdef BATTLEX_BXManager_generated_h
#error "BXManager.generated.h already included, missing '#pragma once' in BXManager.h"
#endif
#define BATTLEX_BXManager_generated_h

#define FID_Lyra_Plugins_BattleX_Source_BattleX_BXManager_h_15_DELEGATE \
BATTLEX_API void FBXTimelineStarted_DelegateWrapper(const FMulticastScriptDelegate& BXTimelineStarted, int64 TimelineID);


#define FID_Lyra_Plugins_BattleX_Source_BattleX_BXManager_h_16_DELEGATE \
BATTLEX_API void FBXTimelineWillEnd_DelegateWrapper(const FMulticastScriptDelegate& BXTimelineWillEnd, int64 TimelineID, EBXTLFinishReason Reason);


#define FID_Lyra_Plugins_BattleX_Source_BattleX_BXManager_h_23_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execChangeShowCollision); \
	DECLARE_FUNCTION(execGetShowCollision); \
	DECLARE_FUNCTION(execOnWorldCleanupStart); \
	DECLARE_FUNCTION(execStopTimeline); \
	DECLARE_FUNCTION(execPlayTimeline); \
	DECLARE_FUNCTION(execIsUpdatingTimeline); \
	DECLARE_FUNCTION(execGetSkillAssetIDs); \
	DECLARE_FUNCTION(execGetTimelineAssetIDs); \
	DECLARE_FUNCTION(execGetSkillAssetByID); \
	DECLARE_FUNCTION(execGetTimelineAssetByID); \
	DECLARE_FUNCTION(execGet);


#define FID_Lyra_Plugins_BattleX_Source_BattleX_BXManager_h_23_CALLBACK_WRAPPERS
#define FID_Lyra_Plugins_BattleX_Source_BattleX_BXManager_h_23_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUBXManager(); \
	friend struct Z_Construct_UClass_UBXManager_Statics; \
public: \
	DECLARE_CLASS(UBXManager, UObject, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/BattleX"), NO_API) \
	DECLARE_SERIALIZER(UBXManager)


#define FID_Lyra_Plugins_BattleX_Source_BattleX_BXManager_h_23_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UBXManager(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	UBXManager(UBXManager&&); \
	UBXManager(const UBXManager&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UBXManager); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UBXManager); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UBXManager) \
	NO_API virtual ~UBXManager();


#define FID_Lyra_Plugins_BattleX_Source_BattleX_BXManager_h_20_PROLOG
#define FID_Lyra_Plugins_BattleX_Source_BattleX_BXManager_h_23_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Lyra_Plugins_BattleX_Source_BattleX_BXManager_h_23_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_Lyra_Plugins_BattleX_Source_BattleX_BXManager_h_23_CALLBACK_WRAPPERS \
	FID_Lyra_Plugins_BattleX_Source_BattleX_BXManager_h_23_INCLASS_NO_PURE_DECLS \
	FID_Lyra_Plugins_BattleX_Source_BattleX_BXManager_h_23_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> BATTLEX_API UClass* StaticClass<class UBXManager>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Lyra_Plugins_BattleX_Source_BattleX_BXManager_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
