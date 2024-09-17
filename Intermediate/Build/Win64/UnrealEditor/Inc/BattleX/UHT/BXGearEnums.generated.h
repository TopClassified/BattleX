// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "Gear/BXGearEnums.h"
#include "Templates/IsUEnumClass.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ReflectedTypeAccessors.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef BATTLEX_BXGearEnums_generated_h
#error "BXGearEnums.generated.h already included, missing '#pragma once' in BXGearEnums.h"
#endif
#define BATTLEX_BXGearEnums_generated_h

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Lyra_Plugins_BattleX_Source_BattleX_Gear_BXGearEnums_h


#define FOREACH_ENUM_EBXGEARTYPE(op) \
	op(EBXGearType::GT_None) \
	op(EBXGearType::GT_Weapon) \
	op(EBXGearType::GT_Armor) \
	op(EBXGearType::GT_Ornament) \
	op(EBXGearType::GT_TMax) 

enum class EBXGearType : uint8;
template<> struct TIsUEnumClass<EBXGearType> { enum { Value = true }; };
template<> BATTLEX_API UEnum* StaticEnum<EBXGearType>();

#define FOREACH_ENUM_EBXGEARSLOT(op) \
	op(EBXGearSlot::GS_None) \
	op(EBXGearSlot::GS_LeftHand) \
	op(EBXGearSlot::GS_RightHand) \
	op(EBXGearSlot::GS_Head) \
	op(EBXGearSlot::GS_Chest) \
	op(EBXGearSlot::GS_Arms) \
	op(EBXGearSlot::GS_Waist) \
	op(EBXGearSlot::GS_Legs) \
	op(EBXGearSlot::GS_LeftFoot) \
	op(EBXGearSlot::GS_RightFoot) \
	op(EBXGearSlot::GS_Jewelry1) \
	op(EBXGearSlot::GS_Jewelry2) \
	op(EBXGearSlot::GS_Jewelry3) \
	op(EBXGearSlot::GS_Jewelry4) \
	op(EBXGearSlot::GS_Jewelry5) \
	op(EBXGearSlot::GS_Jewelry6) \
	op(EBXGearSlot::GS_Jewelry7) \
	op(EBXGearSlot::GS_Jewelry8) \
	op(EBXGearSlot::GS_TMax) 

enum class EBXGearSlot : uint8;
template<> struct TIsUEnumClass<EBXGearSlot> { enum { Value = true }; };
template<> BATTLEX_API UEnum* StaticEnum<EBXGearSlot>();

#define FOREACH_ENUM_EBXEQUIPFUNCTION(op) \
	op(EBXEquipFunction::EF_NativePreEquip) \
	op(EBXEquipFunction::EF_BPPreEquip) \
	op(EBXEquipFunction::EF_NativePostEquip) \
	op(EBXEquipFunction::EF_BPPostEquip) \
	op(EBXEquipFunction::EF_NativePreUnequip) \
	op(EBXEquipFunction::EF_BPPreUnequip) \
	op(EBXEquipFunction::EF_NativePostUnequip) \
	op(EBXEquipFunction::EF_BPPostUnequip) \
	op(EBXEquipFunction::EF_TMax) 

enum class EBXEquipFunction : uint8;
template<> struct TIsUEnumClass<EBXEquipFunction> { enum { Value = true }; };
template<> BATTLEX_API UEnum* StaticEnum<EBXEquipFunction>();

#define FOREACH_ENUM_EBXUSEFUNCTION(op) \
	op(EBXUseFunction::EF_NativePreUsing) \
	op(EBXUseFunction::EF_BPPreUsing) \
	op(EBXUseFunction::EF_NativePostUsing) \
	op(EBXUseFunction::EF_BPPostUsing) \
	op(EBXUseFunction::EF_NativePreUnusing) \
	op(EBXUseFunction::EF_BPPreUnusing) \
	op(EBXUseFunction::EF_NativePostUnusing) \
	op(EBXUseFunction::EF_BPPostUnusing) \
	op(EBXUseFunction::EF_TMax) 

enum class EBXUseFunction : uint8;
template<> struct TIsUEnumClass<EBXUseFunction> { enum { Value = true }; };
template<> BATTLEX_API UEnum* StaticEnum<EBXUseFunction>();

#define FOREACH_ENUM_EBXSHEATHFUNCTION(op) \
	op(EBXSheathFunction::EF_NativePreSheath) \
	op(EBXSheathFunction::EF_BPPreSheath) \
	op(EBXSheathFunction::EF_NativePostSheath) \
	op(EBXSheathFunction::EF_BPPostSheath) \
	op(EBXSheathFunction::EF_NativePreUnsheath) \
	op(EBXSheathFunction::EF_BPPreUnsheath) \
	op(EBXSheathFunction::EF_NativePostUnsheath) \
	op(EBXSheathFunction::EF_BPPostUnsheath) \
	op(EBXSheathFunction::EF_TMax) 

enum class EBXSheathFunction : uint8;
template<> struct TIsUEnumClass<EBXSheathFunction> { enum { Value = true }; };
template<> BATTLEX_API UEnum* StaticEnum<EBXSheathFunction>();

PRAGMA_ENABLE_DEPRECATION_WARNINGS
