// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "Gear/BXGear.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
struct FBXEquipGearInformation;
struct FBXSheathGearInformation;
struct FBXUsingGearInformation;
#ifdef BATTLEX_BXGear_generated_h
#error "BXGear.generated.h already included, missing '#pragma once' in BXGear.h"
#endif
#define BATTLEX_BXGear_generated_h

#define FID_Lyra_Plugins_BattleX_Source_BattleX_Gear_BXGear_h_17_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execPostUnsheath); \
	DECLARE_FUNCTION(execPreUnsheath); \
	DECLARE_FUNCTION(execPostSheath); \
	DECLARE_FUNCTION(execPreSheath); \
	DECLARE_FUNCTION(execPostUnusing); \
	DECLARE_FUNCTION(execPreUnusing); \
	DECLARE_FUNCTION(execPostUsing); \
	DECLARE_FUNCTION(execPreUsing); \
	DECLARE_FUNCTION(execPostUnequip); \
	DECLARE_FUNCTION(execPreUnequip); \
	DECLARE_FUNCTION(execPostEquip); \
	DECLARE_FUNCTION(execPreEquip);


#define FID_Lyra_Plugins_BattleX_Source_BattleX_Gear_BXGear_h_17_CALLBACK_WRAPPERS
#define FID_Lyra_Plugins_BattleX_Source_BattleX_Gear_BXGear_h_17_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesABXGear(); \
	friend struct Z_Construct_UClass_ABXGear_Statics; \
public: \
	DECLARE_CLASS(ABXGear, AActor, COMPILED_IN_FLAGS(CLASS_Abstract | CLASS_Config), CASTCLASS_None, TEXT("/Script/BattleX"), NO_API) \
	DECLARE_SERIALIZER(ABXGear)


#define FID_Lyra_Plugins_BattleX_Source_BattleX_Gear_BXGear_h_17_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	ABXGear(ABXGear&&); \
	ABXGear(const ABXGear&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, ABXGear); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(ABXGear); \
	DEFINE_ABSTRACT_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(ABXGear)


#define FID_Lyra_Plugins_BattleX_Source_BattleX_Gear_BXGear_h_14_PROLOG
#define FID_Lyra_Plugins_BattleX_Source_BattleX_Gear_BXGear_h_17_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Lyra_Plugins_BattleX_Source_BattleX_Gear_BXGear_h_17_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_Lyra_Plugins_BattleX_Source_BattleX_Gear_BXGear_h_17_CALLBACK_WRAPPERS \
	FID_Lyra_Plugins_BattleX_Source_BattleX_Gear_BXGear_h_17_INCLASS_NO_PURE_DECLS \
	FID_Lyra_Plugins_BattleX_Source_BattleX_Gear_BXGear_h_17_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> BATTLEX_API UClass* StaticClass<class ABXGear>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Lyra_Plugins_BattleX_Source_BattleX_Gear_BXGear_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
