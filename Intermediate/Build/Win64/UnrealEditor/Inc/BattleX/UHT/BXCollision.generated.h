// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "Collision/BXCollision.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class AActor;
enum class EBXCDirection : uint8;
struct FBXCFilter;
struct FBXCParameter;
struct FHitResult;
#ifdef BATTLEX_BXCollision_generated_h
#error "BXCollision.generated.h already included, missing '#pragma once' in BXCollision.h"
#endif
#define BATTLEX_BXCollision_generated_h

#define FID_Lyra_Plugins_BattleX_Source_BattleX_Collision_BXCollision_h_29_GENERATED_BODY \
	friend struct Z_Construct_UScriptStruct_FBXCFilter_Statics; \
	static class UScriptStruct* StaticStruct();


template<> BATTLEX_API UScriptStruct* StaticStruct<struct FBXCFilter>();

#define FID_Lyra_Plugins_BattleX_Source_BattleX_Collision_BXCollision_h_102_GENERATED_BODY \
	friend struct Z_Construct_UScriptStruct_FBXCParameter_Statics; \
	static class UScriptStruct* StaticStruct();


template<> BATTLEX_API UScriptStruct* StaticStruct<struct FBXCParameter>();

#define FID_Lyra_Plugins_BattleX_Source_BattleX_Collision_BXCollision_h_144_GENERATED_BODY \
	friend struct Z_Construct_UScriptStruct_FBXCStrategy_Statics; \
	static class UScriptStruct* StaticStruct();


template<> BATTLEX_API UScriptStruct* StaticStruct<struct FBXCStrategy>();

#define FID_Lyra_Plugins_BattleX_Source_BattleX_Collision_BXCollision_h_161_GENERATED_BODY \
	friend struct Z_Construct_UScriptStruct_FBXCSSphere_Statics; \
	static class UScriptStruct* StaticStruct(); \
	typedef FBXCStrategy Super;


template<> BATTLEX_API UScriptStruct* StaticStruct<struct FBXCSSphere>();

#define FID_Lyra_Plugins_BattleX_Source_BattleX_Collision_BXCollision_h_178_GENERATED_BODY \
	friend struct Z_Construct_UScriptStruct_FBXCSCapsule_Statics; \
	static class UScriptStruct* StaticStruct(); \
	typedef FBXCStrategy Super;


template<> BATTLEX_API UScriptStruct* StaticStruct<struct FBXCSCapsule>();

#define FID_Lyra_Plugins_BattleX_Source_BattleX_Collision_BXCollision_h_199_GENERATED_BODY \
	friend struct Z_Construct_UScriptStruct_FBXCSCylinder_Statics; \
	static class UScriptStruct* StaticStruct(); \
	typedef FBXCStrategy Super;


template<> BATTLEX_API UScriptStruct* StaticStruct<struct FBXCSCylinder>();

#define FID_Lyra_Plugins_BattleX_Source_BattleX_Collision_BXCollision_h_212_GENERATED_BODY \
	friend struct Z_Construct_UScriptStruct_FBXCSHollowCylinder_Statics; \
	static class UScriptStruct* StaticStruct(); \
	typedef FBXCStrategy Super;


template<> BATTLEX_API UScriptStruct* StaticStruct<struct FBXCSHollowCylinder>();

#define FID_Lyra_Plugins_BattleX_Source_BattleX_Collision_BXCollision_h_229_GENERATED_BODY \
	friend struct Z_Construct_UScriptStruct_FBXCSBox_Statics; \
	static class UScriptStruct* StaticStruct(); \
	typedef FBXCStrategy Super;


template<> BATTLEX_API UScriptStruct* StaticStruct<struct FBXCSBox>();

#define FID_Lyra_Plugins_BattleX_Source_BattleX_Collision_BXCollision_h_250_GENERATED_BODY \
	friend struct Z_Construct_UScriptStruct_FBXCSSector_Statics; \
	static class UScriptStruct* StaticStruct(); \
	typedef FBXCStrategy Super;


template<> BATTLEX_API UScriptStruct* StaticStruct<struct FBXCSSector>();

#define FID_Lyra_Plugins_BattleX_Source_BattleX_Collision_BXCollision_h_279_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execSectorCheck); \
	DECLARE_FUNCTION(execBoxCheck); \
	DECLARE_FUNCTION(execHollowCylinderCheck); \
	DECLARE_FUNCTION(execCylinderCheck); \
	DECLARE_FUNCTION(execCapsuleCheck); \
	DECLARE_FUNCTION(execSphereCheck); \
	DECLARE_FUNCTION(execCheckCollisionResult);


#define FID_Lyra_Plugins_BattleX_Source_BattleX_Collision_BXCollision_h_279_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUBXCollisionLibrary(); \
	friend struct Z_Construct_UClass_UBXCollisionLibrary_Statics; \
public: \
	DECLARE_CLASS(UBXCollisionLibrary, UBlueprintFunctionLibrary, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/BattleX"), NO_API) \
	DECLARE_SERIALIZER(UBXCollisionLibrary)


#define FID_Lyra_Plugins_BattleX_Source_BattleX_Collision_BXCollision_h_279_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UBXCollisionLibrary(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	UBXCollisionLibrary(UBXCollisionLibrary&&); \
	UBXCollisionLibrary(const UBXCollisionLibrary&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UBXCollisionLibrary); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UBXCollisionLibrary); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UBXCollisionLibrary) \
	NO_API virtual ~UBXCollisionLibrary();


#define FID_Lyra_Plugins_BattleX_Source_BattleX_Collision_BXCollision_h_276_PROLOG
#define FID_Lyra_Plugins_BattleX_Source_BattleX_Collision_BXCollision_h_279_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Lyra_Plugins_BattleX_Source_BattleX_Collision_BXCollision_h_279_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_Lyra_Plugins_BattleX_Source_BattleX_Collision_BXCollision_h_279_INCLASS_NO_PURE_DECLS \
	FID_Lyra_Plugins_BattleX_Source_BattleX_Collision_BXCollision_h_279_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> BATTLEX_API UClass* StaticClass<class UBXCollisionLibrary>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Lyra_Plugins_BattleX_Source_BattleX_Collision_BXCollision_h


#define FOREACH_ENUM_EBXCDIRECTION(op) \
	op(EBXCDirection::D_RightToLeft) \
	op(EBXCDirection::D_LeftToRight) \
	op(EBXCDirection::D_InToOut) 

enum class EBXCDirection : uint8;
template<> struct TIsUEnumClass<EBXCDirection> { enum { Value = true }; };
template<> BATTLEX_API UEnum* StaticEnum<EBXCDirection>();

PRAGMA_ENABLE_DEPRECATION_WARNINGS
