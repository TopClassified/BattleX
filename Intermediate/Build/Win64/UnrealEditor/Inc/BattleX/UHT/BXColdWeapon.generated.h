// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "Gear/BXColdWeapon.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class AActor;
struct FBXCFilter;
struct FHitResult;
#ifdef BATTLEX_BXColdWeapon_generated_h
#error "BXColdWeapon.generated.h already included, missing '#pragma once' in BXColdWeapon.h"
#endif
#define BATTLEX_BXColdWeapon_generated_h

#define FID_Lyra_Plugins_BattleX_Source_BattleX_Gear_BXColdWeapon_h_17_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execGetHitResultsInSeconds); \
	DECLARE_FUNCTION(execStopRecordLocation); \
	DECLARE_FUNCTION(execStartRecordLocation); \
	DECLARE_FUNCTION(execGetAllHitBoxNames);


#define FID_Lyra_Plugins_BattleX_Source_BattleX_Gear_BXColdWeapon_h_17_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesABXColdWeapon(); \
	friend struct Z_Construct_UClass_ABXColdWeapon_Statics; \
public: \
	DECLARE_CLASS(ABXColdWeapon, ABXGear, COMPILED_IN_FLAGS(CLASS_Abstract | CLASS_Config), CASTCLASS_None, TEXT("/Script/BattleX"), NO_API) \
	DECLARE_SERIALIZER(ABXColdWeapon)


#define FID_Lyra_Plugins_BattleX_Source_BattleX_Gear_BXColdWeapon_h_17_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	ABXColdWeapon(ABXColdWeapon&&); \
	ABXColdWeapon(const ABXColdWeapon&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, ABXColdWeapon); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(ABXColdWeapon); \
	DEFINE_ABSTRACT_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(ABXColdWeapon)


#define FID_Lyra_Plugins_BattleX_Source_BattleX_Gear_BXColdWeapon_h_14_PROLOG
#define FID_Lyra_Plugins_BattleX_Source_BattleX_Gear_BXColdWeapon_h_17_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Lyra_Plugins_BattleX_Source_BattleX_Gear_BXColdWeapon_h_17_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_Lyra_Plugins_BattleX_Source_BattleX_Gear_BXColdWeapon_h_17_INCLASS_NO_PURE_DECLS \
	FID_Lyra_Plugins_BattleX_Source_BattleX_Gear_BXColdWeapon_h_17_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> BATTLEX_API UClass* StaticClass<class ABXColdWeapon>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Lyra_Plugins_BattleX_Source_BattleX_Gear_BXColdWeapon_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
