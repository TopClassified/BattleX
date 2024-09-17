// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "Task/BXTEnums.h"
#include "Templates/IsUEnumClass.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ReflectedTypeAccessors.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef BATTLEX_BXTEnums_generated_h
#error "BXTEnums.generated.h already included, missing '#pragma once' in BXTEnums.h"
#endif
#define BATTLEX_BXTEnums_generated_h

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Lyra_Plugins_BattleX_Source_BattleX_Task_BXTEnums_h


#define FOREACH_ENUM_EBXTTARGETTYPE(op) \
	op(EBXTTargetType::T_Owner) \
	op(EBXTTargetType::T_Instigator) \
	op(EBXTTargetType::T_Trigger) \
	op(EBXTTargetType::T_LockTargets) \
	op(EBXTTargetType::T_CollisionResults) \
	op(EBXTTargetType::T_TMax) 

enum class EBXTTargetType : uint8;
template<> struct TIsUEnumClass<EBXTTargetType> { enum { Value = true }; };
template<> BATTLEX_API UEnum* StaticEnum<EBXTTargetType>();

#define FOREACH_ENUM_EBXTLIFETYPE(op) \
	op(EBXTLifeType::L_Instant) \
	op(EBXTLifeType::L_Duration) \
	op(EBXTLifeType::L_Timeline) \
	op(EBXTLifeType::L_DurationTimeline) \
	op(EBXTLifeType::L_TMax) 

enum class EBXTLifeType : uint8;
template<> struct TIsUEnumClass<EBXTLifeType> { enum { Value = true }; };
template<> BATTLEX_API UEnum* StaticEnum<EBXTLifeType>();

#define FOREACH_ENUM_EBXTNETTYPE(op) \
	op(EBXTNetType::N_Authority) \
	op(EBXTNetType::N_Autonomous) \
	op(EBXTNetType::N_Simulated) \
	op(EBXTNetType::N_TMax) 

enum class EBXTNetType : uint8;
template<> struct TIsUEnumClass<EBXTNetType> { enum { Value = true }; };
template<> BATTLEX_API UEnum* StaticEnum<EBXTNetType>();

#define FOREACH_ENUM_EBXTTRIGGERTYPE(op) \
	op(EBXTTriggerType::T_Timeline) \
	op(EBXTTriggerType::T_SectionEnd) \
	op(EBXTTriggerType::T_SectionInterrupt) \
	op(EBXTTriggerType::T_TMax) 

enum class EBXTTriggerType : uint8;
template<> struct TIsUEnumClass<EBXTTriggerType> { enum { Value = true }; };
template<> BATTLEX_API UEnum* StaticEnum<EBXTTriggerType>();

#define FOREACH_ENUM_EBXTCOORDINATETYPE(op) \
	op(EBXTCoordinateType::C_Target) \
	op(EBXTCoordinateType::C_Owner) \
	op(EBXTCoordinateType::C_Instigator) \
	op(EBXTCoordinateType::C_Trigger) \
	op(EBXTCoordinateType::C_LockTarget) \
	op(EBXTCoordinateType::C_LockPart) \
	op(EBXTCoordinateType::C_Special) \
	op(EBXTCoordinateType::C_World) \
	op(EBXTCoordinateType::C_TMax) 

enum class EBXTCoordinateType : uint8;
template<> struct TIsUEnumClass<EBXTCoordinateType> { enum { Value = true }; };
template<> BATTLEX_API UEnum* StaticEnum<EBXTCoordinateType>();

#define FOREACH_ENUM_EBXTCONNECTIONTYPE(op) \
	op(EBXTConnectionType::C_FromOriginIn2D) \
	op(EBXTConnectionType::C_FromOriginIn3D) \
	op(EBXTConnectionType::C_FromXAxisIn2D) \
	op(EBXTConnectionType::C_FromXAxisIn3D) \
	op(EBXTConnectionType::C_TMax) 

enum class EBXTConnectionType : uint8;
template<> struct TIsUEnumClass<EBXTConnectionType> { enum { Value = true }; };
template<> BATTLEX_API UEnum* StaticEnum<EBXTConnectionType>();

#define FOREACH_ENUM_EBXTPROCESSORFUNCTION(op) \
	op(EBXTProcessorFunction::PF_NativeStart) \
	op(EBXTProcessorFunction::PF_BPStart) \
	op(EBXTProcessorFunction::PF_NativeUpdate) \
	op(EBXTProcessorFunction::PF_BPUpdate) \
	op(EBXTProcessorFunction::PF_NativeEnd) \
	op(EBXTProcessorFunction::PF_BPEnd) \
	op(EBXTProcessorFunction::PF_TMax) 

enum class EBXTProcessorFunction : uint8;
template<> struct TIsUEnumClass<EBXTProcessorFunction> { enum { Value = true }; };
template<> BATTLEX_API UEnum* StaticEnum<EBXTProcessorFunction>();

PRAGMA_ENABLE_DEPRECATION_WARNINGS
