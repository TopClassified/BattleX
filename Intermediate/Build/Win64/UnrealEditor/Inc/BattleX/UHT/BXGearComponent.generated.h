// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "Gear/BXGearComponent.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class ABXGear;
class UBXGearData;
class UObject;
enum class EBXGearSlot : uint8;
struct FBXEquipGearInformation;
struct FBXSheathGearInformation;
struct FBXUsingGearInformation;
#ifdef BATTLEX_BXGearComponent_generated_h
#error "BXGearComponent.generated.h already included, missing '#pragma once' in BXGearComponent.h"
#endif
#define BATTLEX_BXGearComponent_generated_h

#define FID_Lyra_Plugins_BattleX_Source_BattleX_Gear_BXGearComponent_h_12_DELEGATE \
BATTLEX_API void FPreEquipGearDelegate_DelegateWrapper(const FMulticastScriptDelegate& PreEquipGearDelegate, ABXGear* Gear, FBXEquipGearInformation& Information);


#define FID_Lyra_Plugins_BattleX_Source_BattleX_Gear_BXGearComponent_h_13_DELEGATE \
BATTLEX_API void FPostEquipGearDelegate_DelegateWrapper(const FMulticastScriptDelegate& PostEquipGearDelegate, ABXGear* Gear, FBXEquipGearInformation& Information);


#define FID_Lyra_Plugins_BattleX_Source_BattleX_Gear_BXGearComponent_h_14_DELEGATE \
BATTLEX_API void FPreUnequipGearDelegate_DelegateWrapper(const FMulticastScriptDelegate& PreUnequipGearDelegate, ABXGear* Gear, FBXEquipGearInformation& Information);


#define FID_Lyra_Plugins_BattleX_Source_BattleX_Gear_BXGearComponent_h_15_DELEGATE \
BATTLEX_API void FPostUnequipGearDelegate_DelegateWrapper(const FMulticastScriptDelegate& PostUnequipGearDelegate, ABXGear* Gear, FBXEquipGearInformation& Information);


#define FID_Lyra_Plugins_BattleX_Source_BattleX_Gear_BXGearComponent_h_16_DELEGATE \
BATTLEX_API void FPreUsingGearDelegate_DelegateWrapper(const FMulticastScriptDelegate& PreUsingGearDelegate, ABXGear* Gear, FBXUsingGearInformation& Information);


#define FID_Lyra_Plugins_BattleX_Source_BattleX_Gear_BXGearComponent_h_17_DELEGATE \
BATTLEX_API void FPostUsingGearDelegate_DelegateWrapper(const FMulticastScriptDelegate& PostUsingGearDelegate, ABXGear* Gear, FBXUsingGearInformation& Information);


#define FID_Lyra_Plugins_BattleX_Source_BattleX_Gear_BXGearComponent_h_18_DELEGATE \
BATTLEX_API void FPreUnusingGearDelegate_DelegateWrapper(const FMulticastScriptDelegate& PreUnusingGearDelegate, ABXGear* Gear, FBXUsingGearInformation& Information);


#define FID_Lyra_Plugins_BattleX_Source_BattleX_Gear_BXGearComponent_h_19_DELEGATE \
BATTLEX_API void FPostUnusingGearDelegate_DelegateWrapper(const FMulticastScriptDelegate& PostUnusingGearDelegate, ABXGear* Gear, FBXUsingGearInformation& Information);


#define FID_Lyra_Plugins_BattleX_Source_BattleX_Gear_BXGearComponent_h_20_DELEGATE \
BATTLEX_API void FPreSheathGearDelegate_DelegateWrapper(const FMulticastScriptDelegate& PreSheathGearDelegate, ABXGear* Gear, FBXSheathGearInformation& Information);


#define FID_Lyra_Plugins_BattleX_Source_BattleX_Gear_BXGearComponent_h_21_DELEGATE \
BATTLEX_API void FPostSheathGearDelegate_DelegateWrapper(const FMulticastScriptDelegate& PostSheathGearDelegate, ABXGear* Gear, FBXSheathGearInformation& Information);


#define FID_Lyra_Plugins_BattleX_Source_BattleX_Gear_BXGearComponent_h_22_DELEGATE \
BATTLEX_API void FPreUnsheathGearDelegate_DelegateWrapper(const FMulticastScriptDelegate& PreUnsheathGearDelegate, ABXGear* Gear, FBXSheathGearInformation& Information);


#define FID_Lyra_Plugins_BattleX_Source_BattleX_Gear_BXGearComponent_h_23_DELEGATE \
BATTLEX_API void FPostUnsheathGearDelegate_DelegateWrapper(const FMulticastScriptDelegate& PostUnsheathGearDelegate, ABXGear* Gear, FBXSheathGearInformation& Information);


#define FID_Lyra_Plugins_BattleX_Source_BattleX_Gear_BXGearComponent_h_30_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execChangeEquipGearByData); \
	DECLARE_FUNCTION(execChangeEquipGearByClass); \
	DECLARE_FUNCTION(execChangeEquipGear); \
	DECLARE_FUNCTION(execGetEquipGears); \
	DECLARE_FUNCTION(execSheathGear); \
	DECLARE_FUNCTION(execSwitchUsingGear); \
	DECLARE_FUNCTION(execGetUsingGear);


#define FID_Lyra_Plugins_BattleX_Source_BattleX_Gear_BXGearComponent_h_30_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUBXGearComponent(); \
	friend struct Z_Construct_UClass_UBXGearComponent_Statics; \
public: \
	DECLARE_CLASS(UBXGearComponent, UActorComponent, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/BattleX"), NO_API) \
	DECLARE_SERIALIZER(UBXGearComponent)


#define FID_Lyra_Plugins_BattleX_Source_BattleX_Gear_BXGearComponent_h_30_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	UBXGearComponent(UBXGearComponent&&); \
	UBXGearComponent(const UBXGearComponent&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UBXGearComponent); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UBXGearComponent); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(UBXGearComponent) \
	NO_API virtual ~UBXGearComponent();


#define FID_Lyra_Plugins_BattleX_Source_BattleX_Gear_BXGearComponent_h_27_PROLOG
#define FID_Lyra_Plugins_BattleX_Source_BattleX_Gear_BXGearComponent_h_30_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Lyra_Plugins_BattleX_Source_BattleX_Gear_BXGearComponent_h_30_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_Lyra_Plugins_BattleX_Source_BattleX_Gear_BXGearComponent_h_30_INCLASS_NO_PURE_DECLS \
	FID_Lyra_Plugins_BattleX_Source_BattleX_Gear_BXGearComponent_h_30_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> BATTLEX_API UClass* StaticClass<class UBXGearComponent>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Lyra_Plugins_BattleX_Source_BattleX_Gear_BXGearComponent_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
