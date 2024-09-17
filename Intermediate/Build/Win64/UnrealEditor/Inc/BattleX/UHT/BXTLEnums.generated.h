// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "Timeline/BXTLEnums.h"
#include "Templates/IsUEnumClass.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ReflectedTypeAccessors.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef BATTLEX_BXTLEnums_generated_h
#error "BXTLEnums.generated.h already included, missing '#pragma once' in BXTLEnums.h"
#endif
#define BATTLEX_BXTLEnums_generated_h

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Lyra_Plugins_BattleX_Source_BattleX_Timeline_BXTLEnums_h


#define FOREACH_ENUM_EBXTLFINISHREASON(op) \
	op(EBXTLFinishReason::FR_EndOfLife) \
	op(EBXTLFinishReason::FR_Interrupt) \
	op(EBXTLFinishReason::FR_PredictionFailure) \
	op(EBXTLFinishReason::FR_TMax) 

enum class EBXTLFinishReason : uint8;
template<> struct TIsUEnumClass<EBXTLFinishReason> { enum { Value = true }; };
template<> BATTLEX_API UEnum* StaticEnum<EBXTLFinishReason>();

#define FOREACH_ENUM_EBXTLTARGETSELECTION(op) \
	op(EBXTLTargetSelection::ST_Self) \
	op(EBXTLTargetSelection::ST_Owner) \
	op(EBXTLTargetSelection::ST_Instigator) \
	op(EBXTLTargetSelection::ST_Trigger) \
	op(EBXTLTargetSelection::ST_Target) \
	op(EBXTLTargetSelection::ST_TMax) 

enum class EBXTLTargetSelection : uint8;
template<> struct TIsUEnumClass<EBXTLTargetSelection> { enum { Value = true }; };
template<> BATTLEX_API UEnum* StaticEnum<EBXTLTargetSelection>();

#define FOREACH_ENUM_EBXTLEXTRATYPE(op) \
	op(EBXTLExtraType::ET_Anticipation) \
	op(EBXTLExtraType::ET_Recovery) \
	op(EBXTLExtraType::ET_ComboWindow1) \
	op(EBXTLExtraType::ET_ComboWindow2) \
	op(EBXTLExtraType::ET_ComboWindow3) \
	op(EBXTLExtraType::ET_ComboWindow4) \
	op(EBXTLExtraType::ET_ComboWindow5) \
	op(EBXTLExtraType::ET_ComboWindow6) \
	op(EBXTLExtraType::ET_ComboWindow7) \
	op(EBXTLExtraType::ET_ComboWindow8) \
	op(EBXTLExtraType::ET_TMax) 

enum class EBXTLExtraType : uint8;
template<> struct TIsUEnumClass<EBXTLExtraType> { enum { Value = true }; };
template<> BATTLEX_API UEnum* StaticEnum<EBXTLExtraType>();

PRAGMA_ENABLE_DEPRECATION_WARNINGS
