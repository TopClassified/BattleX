// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "Timeline/BXTLAsset.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef BATTLEX_BXTLAsset_generated_h
#error "BXTLAsset.generated.h already included, missing '#pragma once' in BXTLAsset.h"
#endif
#define BATTLEX_BXTLAsset_generated_h

#define FID_Lyra_Plugins_BattleX_Source_BattleX_Timeline_BXTLAsset_h_20_GENERATED_BODY \
	friend struct Z_Construct_UScriptStruct_FBXTLTaskGroup_Statics; \
	BATTLEX_API static class UScriptStruct* StaticStruct();


template<> BATTLEX_API UScriptStruct* StaticStruct<struct FBXTLTaskGroup>();

#define FID_Lyra_Plugins_BattleX_Source_BattleX_Timeline_BXTLAsset_h_43_GENERATED_BODY \
	friend struct Z_Construct_UScriptStruct_FBXTLSection_Statics; \
	BATTLEX_API static class UScriptStruct* StaticStruct();


template<> BATTLEX_API UScriptStruct* StaticStruct<struct FBXTLSection>();

#define FID_Lyra_Plugins_BattleX_Source_BattleX_Timeline_BXTLAsset_h_109_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUBXTLAsset(); \
	friend struct Z_Construct_UClass_UBXTLAsset_Statics; \
public: \
	DECLARE_CLASS(UBXTLAsset, UPrimaryDataAsset, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/BattleX"), NO_API) \
	DECLARE_SERIALIZER(UBXTLAsset)


#define FID_Lyra_Plugins_BattleX_Source_BattleX_Timeline_BXTLAsset_h_109_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	UBXTLAsset(UBXTLAsset&&); \
	UBXTLAsset(const UBXTLAsset&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UBXTLAsset); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UBXTLAsset); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(UBXTLAsset) \
	NO_API virtual ~UBXTLAsset();


#define FID_Lyra_Plugins_BattleX_Source_BattleX_Timeline_BXTLAsset_h_106_PROLOG
#define FID_Lyra_Plugins_BattleX_Source_BattleX_Timeline_BXTLAsset_h_109_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Lyra_Plugins_BattleX_Source_BattleX_Timeline_BXTLAsset_h_109_INCLASS_NO_PURE_DECLS \
	FID_Lyra_Plugins_BattleX_Source_BattleX_Timeline_BXTLAsset_h_109_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> BATTLEX_API UClass* StaticClass<class UBXTLAsset>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Lyra_Plugins_BattleX_Source_BattleX_Timeline_BXTLAsset_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
