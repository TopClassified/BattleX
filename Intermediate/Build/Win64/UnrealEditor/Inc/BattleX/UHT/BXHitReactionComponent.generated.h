// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "Collision/BXHitReactionComponent.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class USceneComponent;
enum class EBXBodyPartType : uint8;
struct FBXBodyPartRTInformation;
#ifdef BATTLEX_BXHitReactionComponent_generated_h
#error "BXHitReactionComponent.generated.h already included, missing '#pragma once' in BXHitReactionComponent.h"
#endif
#define BATTLEX_BXHitReactionComponent_generated_h

#define FID_Lyra_Plugins_BattleX_Source_BattleX_Collision_BXHitReactionComponent_h_15_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execGetBodyPartByComponentAndBone); \
	DECLARE_FUNCTION(execGetBodyPartByType);


#define FID_Lyra_Plugins_BattleX_Source_BattleX_Collision_BXHitReactionComponent_h_15_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUBXHitReactionComponent(); \
	friend struct Z_Construct_UClass_UBXHitReactionComponent_Statics; \
public: \
	DECLARE_CLASS(UBXHitReactionComponent, UActorComponent, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/BattleX"), NO_API) \
	DECLARE_SERIALIZER(UBXHitReactionComponent)


#define FID_Lyra_Plugins_BattleX_Source_BattleX_Collision_BXHitReactionComponent_h_15_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	UBXHitReactionComponent(UBXHitReactionComponent&&); \
	UBXHitReactionComponent(const UBXHitReactionComponent&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UBXHitReactionComponent); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UBXHitReactionComponent); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(UBXHitReactionComponent) \
	NO_API virtual ~UBXHitReactionComponent();


#define FID_Lyra_Plugins_BattleX_Source_BattleX_Collision_BXHitReactionComponent_h_12_PROLOG
#define FID_Lyra_Plugins_BattleX_Source_BattleX_Collision_BXHitReactionComponent_h_15_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Lyra_Plugins_BattleX_Source_BattleX_Collision_BXHitReactionComponent_h_15_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_Lyra_Plugins_BattleX_Source_BattleX_Collision_BXHitReactionComponent_h_15_INCLASS_NO_PURE_DECLS \
	FID_Lyra_Plugins_BattleX_Source_BattleX_Collision_BXHitReactionComponent_h_15_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> BATTLEX_API UClass* StaticClass<class UBXHitReactionComponent>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Lyra_Plugins_BattleX_Source_BattleX_Collision_BXHitReactionComponent_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
