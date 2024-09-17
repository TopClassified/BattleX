// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "Gear/BXGearData.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef BATTLEX_BXGearData_generated_h
#error "BXGearData.generated.h already included, missing '#pragma once' in BXGearData.h"
#endif
#define BATTLEX_BXGearData_generated_h

#define FID_Lyra_Plugins_BattleX_Source_BattleX_Gear_BXGearData_h_12_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUBXGearData(); \
	friend struct Z_Construct_UClass_UBXGearData_Statics; \
public: \
	DECLARE_CLASS(UBXGearData, UObject, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/BattleX"), NO_API) \
	DECLARE_SERIALIZER(UBXGearData)


#define FID_Lyra_Plugins_BattleX_Source_BattleX_Gear_BXGearData_h_12_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UBXGearData(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	UBXGearData(UBXGearData&&); \
	UBXGearData(const UBXGearData&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UBXGearData); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UBXGearData); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UBXGearData) \
	NO_API virtual ~UBXGearData();


#define FID_Lyra_Plugins_BattleX_Source_BattleX_Gear_BXGearData_h_9_PROLOG
#define FID_Lyra_Plugins_BattleX_Source_BattleX_Gear_BXGearData_h_12_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Lyra_Plugins_BattleX_Source_BattleX_Gear_BXGearData_h_12_INCLASS_NO_PURE_DECLS \
	FID_Lyra_Plugins_BattleX_Source_BattleX_Gear_BXGearData_h_12_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> BATTLEX_API UClass* StaticClass<class UBXGearData>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Lyra_Plugins_BattleX_Source_BattleX_Gear_BXGearData_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
