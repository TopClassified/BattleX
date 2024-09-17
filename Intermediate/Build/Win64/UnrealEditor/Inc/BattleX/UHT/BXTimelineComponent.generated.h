// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "Timeline/BXTimelineComponent.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class UBXTLAsset;
enum class EBXTLFinishReason : uint8;
struct FBXTLPlayContext;
#ifdef BATTLEX_BXTimelineComponent_generated_h
#error "BXTimelineComponent.generated.h already included, missing '#pragma once' in BXTimelineComponent.h"
#endif
#define BATTLEX_BXTimelineComponent_generated_h

#define FID_Lyra_Plugins_BattleX_Source_BattleX_Timeline_BXTimelineComponent_h_16_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execIsTimelineRunningByID); \
	DECLARE_FUNCTION(execIsTimelineRunning); \
	DECLARE_FUNCTION(execStopTimeline); \
	DECLARE_FUNCTION(execPlayTimeline);


#define FID_Lyra_Plugins_BattleX_Source_BattleX_Timeline_BXTimelineComponent_h_16_CALLBACK_WRAPPERS
#define FID_Lyra_Plugins_BattleX_Source_BattleX_Timeline_BXTimelineComponent_h_16_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUBXTimelineComponent(); \
	friend struct Z_Construct_UClass_UBXTimelineComponent_Statics; \
public: \
	DECLARE_CLASS(UBXTimelineComponent, UActorComponent, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/BattleX"), NO_API) \
	DECLARE_SERIALIZER(UBXTimelineComponent)


#define FID_Lyra_Plugins_BattleX_Source_BattleX_Timeline_BXTimelineComponent_h_16_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	UBXTimelineComponent(UBXTimelineComponent&&); \
	UBXTimelineComponent(const UBXTimelineComponent&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UBXTimelineComponent); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UBXTimelineComponent); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(UBXTimelineComponent) \
	NO_API virtual ~UBXTimelineComponent();


#define FID_Lyra_Plugins_BattleX_Source_BattleX_Timeline_BXTimelineComponent_h_13_PROLOG
#define FID_Lyra_Plugins_BattleX_Source_BattleX_Timeline_BXTimelineComponent_h_16_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Lyra_Plugins_BattleX_Source_BattleX_Timeline_BXTimelineComponent_h_16_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_Lyra_Plugins_BattleX_Source_BattleX_Timeline_BXTimelineComponent_h_16_CALLBACK_WRAPPERS \
	FID_Lyra_Plugins_BattleX_Source_BattleX_Timeline_BXTimelineComponent_h_16_INCLASS_NO_PURE_DECLS \
	FID_Lyra_Plugins_BattleX_Source_BattleX_Timeline_BXTimelineComponent_h_16_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> BATTLEX_API UClass* StaticClass<class UBXTimelineComponent>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Lyra_Plugins_BattleX_Source_BattleX_Timeline_BXTimelineComponent_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
