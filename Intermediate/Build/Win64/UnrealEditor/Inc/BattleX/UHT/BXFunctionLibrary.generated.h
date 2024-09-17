// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "BXFunctionLibrary.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class AActor;
class UBXTask;
class UBXTLAsset;
class UObject;
class USceneComponent;
#ifdef BATTLEX_BXFunctionLibrary_generated_h
#error "BXFunctionLibrary.generated.h already included, missing '#pragma once' in BXFunctionLibrary.h"
#endif
#define BATTLEX_BXFunctionLibrary_generated_h

#define FID_Lyra_Plugins_BattleX_Source_BattleX_BXFunctionLibrary_h_14_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execGetSceneComponentByNameAndClass); \
	DECLARE_FUNCTION(execSegmentToBox); \
	DECLARE_FUNCTION(execSegmentToCapsule); \
	DECLARE_FUNCTION(execSegmentToSphere); \
	DECLARE_FUNCTION(execSegmentToSegment); \
	DECLARE_FUNCTION(execPointToBox); \
	DECLARE_FUNCTION(execPointToCylinder); \
	DECLARE_FUNCTION(execPointToCapsule); \
	DECLARE_FUNCTION(execPointToSphere); \
	DECLARE_FUNCTION(execPointToRectangle); \
	DECLARE_FUNCTION(execPointToCircle); \
	DECLARE_FUNCTION(execPointToSegment); \
	DECLARE_FUNCTION(execGetSoftTaskFullIndex); \
	DECLARE_FUNCTION(execGetTaskFullIndex); \
	DECLARE_FUNCTION(execCopyObject); \
	DECLARE_FUNCTION(execAlignTime); \
	DECLARE_FUNCTION(execGetGameMicrosecond); \
	DECLARE_FUNCTION(execGetUtcMillisecond); \
	DECLARE_FUNCTION(execGetShortUniqueID); \
	DECLARE_FUNCTION(execGetUniqueID);


#define FID_Lyra_Plugins_BattleX_Source_BattleX_BXFunctionLibrary_h_14_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUBXFunctionLibrary(); \
	friend struct Z_Construct_UClass_UBXFunctionLibrary_Statics; \
public: \
	DECLARE_CLASS(UBXFunctionLibrary, UBlueprintFunctionLibrary, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/BattleX"), NO_API) \
	DECLARE_SERIALIZER(UBXFunctionLibrary)


#define FID_Lyra_Plugins_BattleX_Source_BattleX_BXFunctionLibrary_h_14_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UBXFunctionLibrary(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	UBXFunctionLibrary(UBXFunctionLibrary&&); \
	UBXFunctionLibrary(const UBXFunctionLibrary&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UBXFunctionLibrary); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UBXFunctionLibrary); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UBXFunctionLibrary) \
	NO_API virtual ~UBXFunctionLibrary();


#define FID_Lyra_Plugins_BattleX_Source_BattleX_BXFunctionLibrary_h_11_PROLOG
#define FID_Lyra_Plugins_BattleX_Source_BattleX_BXFunctionLibrary_h_14_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Lyra_Plugins_BattleX_Source_BattleX_BXFunctionLibrary_h_14_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_Lyra_Plugins_BattleX_Source_BattleX_BXFunctionLibrary_h_14_INCLASS_NO_PURE_DECLS \
	FID_Lyra_Plugins_BattleX_Source_BattleX_BXFunctionLibrary_h_14_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> BATTLEX_API UClass* StaticClass<class UBXFunctionLibrary>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Lyra_Plugins_BattleX_Source_BattleX_BXFunctionLibrary_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
