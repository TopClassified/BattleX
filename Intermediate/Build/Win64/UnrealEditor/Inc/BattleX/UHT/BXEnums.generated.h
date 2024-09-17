// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "BXEnums.h"
#include "Templates/IsUEnumClass.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ReflectedTypeAccessors.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef BATTLEX_BXEnums_generated_h
#error "BXEnums.generated.h already included, missing '#pragma once' in BXEnums.h"
#endif
#define BATTLEX_BXEnums_generated_h

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Lyra_Plugins_BattleX_Source_BattleX_BXEnums_h


#define FOREACH_ENUM_EBXCOMPARESIGN(op) \
	op(EBXCompareSign::CS_Less) \
	op(EBXCompareSign::CS_LessOrEqual) \
	op(EBXCompareSign::CS_Equal) \
	op(EBXCompareSign::CS_GreaterOrEqual) \
	op(EBXCompareSign::CS_Greater) \
	op(EBXCompareSign::CS_TMax) 

enum class EBXCompareSign : uint8;
template<> struct TIsUEnumClass<EBXCompareSign> { enum { Value = true }; };
template<> BATTLEX_API UEnum* StaticEnum<EBXCompareSign>();

#define FOREACH_ENUM_EBXDAMAGETYPE(op) \
	op(EBXDamageType::DT_Physics) \
	op(EBXDamageType::DT_Magic) \
	op(EBXDamageType::DT_Purely) \
	op(EBXDamageType::DT_TMax) 

enum class EBXDamageType : uint8;
template<> struct TIsUEnumClass<EBXDamageType> { enum { Value = true }; };
template<> BATTLEX_API UEnum* StaticEnum<EBXDamageType>();

#define FOREACH_ENUM_EBXSTAGGERSTATE(op) \
	op(EBXStaggerState::SS_Normal) \
	op(EBXStaggerState::SS_Repel) \
	op(EBXStaggerState::SS_LieDown) \
	op(EBXStaggerState::SS_Crawl) \
	op(EBXStaggerState::SS_Rise) \
	op(EBXStaggerState::SS_Fall) \
	op(EBXStaggerState::SS_Float) \
	op(EBXStaggerState::SS_Execution) \
	op(EBXStaggerState::SS_008) \
	op(EBXStaggerState::SS_009) \
	op(EBXStaggerState::SS_010) \
	op(EBXStaggerState::SS_011) \
	op(EBXStaggerState::SS_012) \
	op(EBXStaggerState::SS_013) \
	op(EBXStaggerState::SS_014) \
	op(EBXStaggerState::SS_015) \
	op(EBXStaggerState::SS_TMax) 

enum class EBXStaggerState : uint8;
template<> struct TIsUEnumClass<EBXStaggerState> { enum { Value = true }; };
template<> BATTLEX_API UEnum* StaticEnum<EBXStaggerState>();

#define FOREACH_ENUM_EBXATTACKTYPE(op) \
	op(EBXAttackType::AT_Light) \
	op(EBXAttackType::AT_Repel) \
	op(EBXAttackType::AT_HitDown) \
	op(EBXAttackType::AT_BlowFly) \
	op(EBXAttackType::AT_LiftUp) \
	op(EBXAttackType::AT_BlowDown) \
	op(EBXAttackType::AT_Drag) \
	op(EBXAttackType::AT_TMax) 

enum class EBXAttackType : uint8;
template<> struct TIsUEnumClass<EBXAttackType> { enum { Value = true }; };
template<> BATTLEX_API UEnum* StaticEnum<EBXAttackType>();

#define FOREACH_ENUM_EBXATTACKFORCE(op) \
	op(EBXAttackForce::AF_0) \
	op(EBXAttackForce::AF_1) \
	op(EBXAttackForce::AF_2) \
	op(EBXAttackForce::AF_3) \
	op(EBXAttackForce::AF_4) \
	op(EBXAttackForce::AF_5) \
	op(EBXAttackForce::AF_TMax) 

enum class EBXAttackForce : uint8;
template<> struct TIsUEnumClass<EBXAttackForce> { enum { Value = true }; };
template<> BATTLEX_API UEnum* StaticEnum<EBXAttackForce>();

#define FOREACH_ENUM_EBXATTACKERLOCATIONTYPE(op) \
	op(EBXAttackerLocationType::ALT_Front) \
	op(EBXAttackerLocationType::ALT_Back) \
	op(EBXAttackerLocationType::ALT_Left) \
	op(EBXAttackerLocationType::ALT_Right) \
	op(EBXAttackerLocationType::ALT_TMax) 

enum class EBXAttackerLocationType : uint8;
template<> struct TIsUEnumClass<EBXAttackerLocationType> { enum { Value = true }; };
template<> BATTLEX_API UEnum* StaticEnum<EBXAttackerLocationType>();

#define FOREACH_ENUM_EBXDATATYPE(op) \
	op(EBXDataType::DT_Integer) \
	op(EBXDataType::DT_Float) \
	op(EBXDataType::DT_Struct) \
	op(EBXDataType::DT_TMax) 

enum class EBXDataType : uint8;
template<> struct TIsUEnumClass<EBXDataType> { enum { Value = true }; };
template<> BATTLEX_API UEnum* StaticEnum<EBXDataType>();

#define FOREACH_ENUM_EBXSHAPETYPE(op) \
	op(EBXShapeType::ST_Sphere) \
	op(EBXShapeType::ST_Capsule) \
	op(EBXShapeType::ST_Box) \
	op(EBXShapeType::ST_TMax) 

enum class EBXShapeType : uint8;
template<> struct TIsUEnumClass<EBXShapeType> { enum { Value = true }; };
template<> BATTLEX_API UEnum* StaticEnum<EBXShapeType>();

#define FOREACH_ENUM_EBXDYNAMICMATERIALPARAMTYPE(op) \
	op(EBXDynamicMaterialParamType::DMPT_Scalar) \
	op(EBXDynamicMaterialParamType::DMPT_Vector) \
	op(EBXDynamicMaterialParamType::DMPT_Texture) \
	op(EBXDynamicMaterialParamType::DMPT_LinearColor) \
	op(EBXDynamicMaterialParamType::DMPT_TMax) 

enum class EBXDynamicMaterialParamType : uint8;
template<> struct TIsUEnumClass<EBXDynamicMaterialParamType> { enum { Value = true }; };
template<> BATTLEX_API UEnum* StaticEnum<EBXDynamicMaterialParamType>();

#define FOREACH_ENUM_EBXLOCKTARGETTYPE(op) \
	op(EBXLockTargetType::TT_Skill) \
	op(EBXLockTargetType::TT_ExtraOne) \
	op(EBXLockTargetType::TT_ExtraTwo) \
	op(EBXLockTargetType::TT_ExtraThree) \
	op(EBXLockTargetType::TT_ExtraFour) \
	op(EBXLockTargetType::TT_TMax) 

enum class EBXLockTargetType : uint8;
template<> struct TIsUEnumClass<EBXLockTargetType> { enum { Value = true }; };
template<> BATTLEX_API UEnum* StaticEnum<EBXLockTargetType>();

#define FOREACH_ENUM_EBXBODYPARTTYPE(op) \
	op(EBXBodyPartType::BPT_Default) \
	op(EBXBodyPartType::BPT_Head) \
	op(EBXBodyPartType::BPT_UpperBody) \
	op(EBXBodyPartType::BPT_LowerBody) \
	op(EBXBodyPartType::BPT_Tail) \
	op(EBXBodyPartType::BPT_Limbs1) \
	op(EBXBodyPartType::BPT_Limbs2) \
	op(EBXBodyPartType::BPT_Limbs3) \
	op(EBXBodyPartType::BPT_Limbs4) \
	op(EBXBodyPartType::BPT_Limbs5) \
	op(EBXBodyPartType::BPT_Limbs6) \
	op(EBXBodyPartType::BPT_Limbs7) \
	op(EBXBodyPartType::BPT_Limbs8) \
	op(EBXBodyPartType::BPT_TMax) 

enum class EBXBodyPartType : uint8;
template<> struct TIsUEnumClass<EBXBodyPartType> { enum { Value = true }; };
template<> BATTLEX_API UEnum* StaticEnum<EBXBodyPartType>();

PRAGMA_ENABLE_DEPRECATION_WARNINGS
