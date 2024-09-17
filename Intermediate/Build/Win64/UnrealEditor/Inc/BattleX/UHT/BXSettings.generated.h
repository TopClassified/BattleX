// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "BXSettings.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef BATTLEX_BXSettings_generated_h
#error "BXSettings.generated.h already included, missing '#pragma once' in BXSettings.h"
#endif
#define BATTLEX_BXSettings_generated_h

#define FID_Lyra_Plugins_BattleX_Source_BattleX_BXSettings_h_16_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUBXSettings(); \
	friend struct Z_Construct_UClass_UBXSettings_Statics; \
public: \
	DECLARE_CLASS(UBXSettings, UDeveloperSettings, COMPILED_IN_FLAGS(0 | CLASS_DefaultConfig | CLASS_Config), CASTCLASS_None, TEXT("/Script/BattleX"), NO_API) \
	DECLARE_SERIALIZER(UBXSettings) \
	static const TCHAR* StaticConfigName() {return TEXT("Game");} \



#define FID_Lyra_Plugins_BattleX_Source_BattleX_BXSettings_h_16_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	UBXSettings(UBXSettings&&); \
	UBXSettings(const UBXSettings&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UBXSettings); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UBXSettings); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UBXSettings) \
	NO_API virtual ~UBXSettings();


#define FID_Lyra_Plugins_BattleX_Source_BattleX_BXSettings_h_13_PROLOG
#define FID_Lyra_Plugins_BattleX_Source_BattleX_BXSettings_h_16_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Lyra_Plugins_BattleX_Source_BattleX_BXSettings_h_16_INCLASS_NO_PURE_DECLS \
	FID_Lyra_Plugins_BattleX_Source_BattleX_BXSettings_h_16_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> BATTLEX_API UClass* StaticClass<class UBXSettings>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Lyra_Plugins_BattleX_Source_BattleX_BXSettings_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
