// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "BXSubSystem.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class UBXManager;
#ifdef BATTLEX_BXSubSystem_generated_h
#error "BXSubSystem.generated.h already included, missing '#pragma once' in BXSubSystem.h"
#endif
#define BATTLEX_BXSubSystem_generated_h

#define FID_Lyra_Plugins_BattleX_Source_BattleX_BXSubSystem_h_14_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execGetBXManager);


#define FID_Lyra_Plugins_BattleX_Source_BattleX_BXSubSystem_h_14_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUBXSubSystem(); \
	friend struct Z_Construct_UClass_UBXSubSystem_Statics; \
public: \
	DECLARE_CLASS(UBXSubSystem, UGameInstanceSubsystem, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/BattleX"), NO_API) \
	DECLARE_SERIALIZER(UBXSubSystem)


#define FID_Lyra_Plugins_BattleX_Source_BattleX_BXSubSystem_h_14_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UBXSubSystem(); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	UBXSubSystem(UBXSubSystem&&); \
	UBXSubSystem(const UBXSubSystem&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UBXSubSystem); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UBXSubSystem); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(UBXSubSystem) \
	NO_API virtual ~UBXSubSystem();


#define FID_Lyra_Plugins_BattleX_Source_BattleX_BXSubSystem_h_11_PROLOG
#define FID_Lyra_Plugins_BattleX_Source_BattleX_BXSubSystem_h_14_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Lyra_Plugins_BattleX_Source_BattleX_BXSubSystem_h_14_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_Lyra_Plugins_BattleX_Source_BattleX_BXSubSystem_h_14_INCLASS_NO_PURE_DECLS \
	FID_Lyra_Plugins_BattleX_Source_BattleX_BXSubSystem_h_14_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> BATTLEX_API UClass* StaticClass<class UBXSubSystem>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Lyra_Plugins_BattleX_Source_BattleX_BXSubSystem_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
