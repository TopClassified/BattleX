// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "BattleX/BXEnums.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeBXEnums() {}

// Begin Cross Module References
BATTLEX_API UEnum* Z_Construct_UEnum_BattleX_EBXAttackerLocationType();
BATTLEX_API UEnum* Z_Construct_UEnum_BattleX_EBXAttackForce();
BATTLEX_API UEnum* Z_Construct_UEnum_BattleX_EBXAttackType();
BATTLEX_API UEnum* Z_Construct_UEnum_BattleX_EBXBodyPartType();
BATTLEX_API UEnum* Z_Construct_UEnum_BattleX_EBXCompareSign();
BATTLEX_API UEnum* Z_Construct_UEnum_BattleX_EBXDamageType();
BATTLEX_API UEnum* Z_Construct_UEnum_BattleX_EBXDataType();
BATTLEX_API UEnum* Z_Construct_UEnum_BattleX_EBXDynamicMaterialParamType();
BATTLEX_API UEnum* Z_Construct_UEnum_BattleX_EBXLockTargetType();
BATTLEX_API UEnum* Z_Construct_UEnum_BattleX_EBXShapeType();
BATTLEX_API UEnum* Z_Construct_UEnum_BattleX_EBXStaggerState();
UPackage* Z_Construct_UPackage__Script_BattleX();
// End Cross Module References

// Begin Enum EBXCompareSign
static FEnumRegistrationInfo Z_Registration_Info_UEnum_EBXCompareSign;
static UEnum* EBXCompareSign_StaticEnum()
{
	if (!Z_Registration_Info_UEnum_EBXCompareSign.OuterSingleton)
	{
		Z_Registration_Info_UEnum_EBXCompareSign.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_BattleX_EBXCompareSign, (UObject*)Z_Construct_UPackage__Script_BattleX(), TEXT("EBXCompareSign"));
	}
	return Z_Registration_Info_UEnum_EBXCompareSign.OuterSingleton;
}
template<> BATTLEX_API UEnum* StaticEnum<EBXCompareSign>()
{
	return EBXCompareSign_StaticEnum();
}
struct Z_Construct_UEnum_BattleX_EBXCompareSign_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe6\xaf\x94\xe8\xbe\x83\xe7\xac\xa6\xe5\x8f\xb7\n" },
#endif
		{ "CS_Equal.DisplayName", "\xe7\xad\x89\xe4\xba\x8e" },
		{ "CS_Equal.Name", "EBXCompareSign::CS_Equal" },
		{ "CS_Greater.DisplayName", "\xe5\xa4\xa7\xe4\xba\x8e" },
		{ "CS_Greater.Name", "EBXCompareSign::CS_Greater" },
		{ "CS_GreaterOrEqual.DisplayName", "\xe5\xa4\xa7\xe4\xba\x8e\xe7\xad\x89\xe4\xba\x8e" },
		{ "CS_GreaterOrEqual.Name", "EBXCompareSign::CS_GreaterOrEqual" },
		{ "CS_Less.DisplayName", "\xe5\xb0\x8f\xe4\xba\x8e" },
		{ "CS_Less.Name", "EBXCompareSign::CS_Less" },
		{ "CS_LessOrEqual.DisplayName", "\xe5\xb0\x8f\xe4\xba\x8e\xe7\xad\x89\xe4\xba\x8e" },
		{ "CS_LessOrEqual.Name", "EBXCompareSign::CS_LessOrEqual" },
		{ "CS_TMax.Hidden", "" },
		{ "CS_TMax.Name", "EBXCompareSign::CS_TMax" },
		{ "ModuleRelativePath", "BXEnums.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe6\xaf\x94\xe8\xbe\x83\xe7\xac\xa6\xe5\x8f\xb7" },
#endif
	};
#endif // WITH_METADATA
	static constexpr UECodeGen_Private::FEnumeratorParam Enumerators[] = {
		{ "EBXCompareSign::CS_Less", (int64)EBXCompareSign::CS_Less },
		{ "EBXCompareSign::CS_LessOrEqual", (int64)EBXCompareSign::CS_LessOrEqual },
		{ "EBXCompareSign::CS_Equal", (int64)EBXCompareSign::CS_Equal },
		{ "EBXCompareSign::CS_GreaterOrEqual", (int64)EBXCompareSign::CS_GreaterOrEqual },
		{ "EBXCompareSign::CS_Greater", (int64)EBXCompareSign::CS_Greater },
		{ "EBXCompareSign::CS_TMax", (int64)EBXCompareSign::CS_TMax },
	};
	static const UECodeGen_Private::FEnumParams EnumParams;
};
const UECodeGen_Private::FEnumParams Z_Construct_UEnum_BattleX_EBXCompareSign_Statics::EnumParams = {
	(UObject*(*)())Z_Construct_UPackage__Script_BattleX,
	nullptr,
	"EBXCompareSign",
	"EBXCompareSign",
	Z_Construct_UEnum_BattleX_EBXCompareSign_Statics::Enumerators,
	RF_Public|RF_Transient|RF_MarkAsNative,
	UE_ARRAY_COUNT(Z_Construct_UEnum_BattleX_EBXCompareSign_Statics::Enumerators),
	EEnumFlags::None,
	(uint8)UEnum::ECppForm::EnumClass,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UEnum_BattleX_EBXCompareSign_Statics::Enum_MetaDataParams), Z_Construct_UEnum_BattleX_EBXCompareSign_Statics::Enum_MetaDataParams)
};
UEnum* Z_Construct_UEnum_BattleX_EBXCompareSign()
{
	if (!Z_Registration_Info_UEnum_EBXCompareSign.InnerSingleton)
	{
		UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_EBXCompareSign.InnerSingleton, Z_Construct_UEnum_BattleX_EBXCompareSign_Statics::EnumParams);
	}
	return Z_Registration_Info_UEnum_EBXCompareSign.InnerSingleton;
}
// End Enum EBXCompareSign

// Begin Enum EBXDamageType
static FEnumRegistrationInfo Z_Registration_Info_UEnum_EBXDamageType;
static UEnum* EBXDamageType_StaticEnum()
{
	if (!Z_Registration_Info_UEnum_EBXDamageType.OuterSingleton)
	{
		Z_Registration_Info_UEnum_EBXDamageType.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_BattleX_EBXDamageType, (UObject*)Z_Construct_UPackage__Script_BattleX(), TEXT("EBXDamageType"));
	}
	return Z_Registration_Info_UEnum_EBXDamageType.OuterSingleton;
}
template<> BATTLEX_API UEnum* StaticEnum<EBXDamageType>()
{
	return EBXDamageType_StaticEnum();
}
struct Z_Construct_UEnum_BattleX_EBXDamageType_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe4\xbc\xa4\xe5\xae\xb3\xe7\xb1\xbb\xe5\x9e\x8b\n" },
#endif
		{ "DT_Magic.DisplayName", "\xe9\xad\x94\xe6\xb3\x95\xe4\xbc\xa4\xe5\xae\xb3" },
		{ "DT_Magic.Name", "EBXDamageType::DT_Magic" },
		{ "DT_Physics.DisplayName", "\xe7\x89\xa9\xe7\x90\x86\xe4\xbc\xa4\xe5\xae\xb3" },
		{ "DT_Physics.Name", "EBXDamageType::DT_Physics" },
		{ "DT_Purely.DisplayName", "\xe7\xba\xaf\xe7\xb2\xb9\xe4\xbc\xa4\xe5\xae\xb3" },
		{ "DT_Purely.Name", "EBXDamageType::DT_Purely" },
		{ "DT_TMax.Hidden", "" },
		{ "DT_TMax.Name", "EBXDamageType::DT_TMax" },
		{ "ModuleRelativePath", "BXEnums.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe4\xbc\xa4\xe5\xae\xb3\xe7\xb1\xbb\xe5\x9e\x8b" },
#endif
	};
#endif // WITH_METADATA
	static constexpr UECodeGen_Private::FEnumeratorParam Enumerators[] = {
		{ "EBXDamageType::DT_Physics", (int64)EBXDamageType::DT_Physics },
		{ "EBXDamageType::DT_Magic", (int64)EBXDamageType::DT_Magic },
		{ "EBXDamageType::DT_Purely", (int64)EBXDamageType::DT_Purely },
		{ "EBXDamageType::DT_TMax", (int64)EBXDamageType::DT_TMax },
	};
	static const UECodeGen_Private::FEnumParams EnumParams;
};
const UECodeGen_Private::FEnumParams Z_Construct_UEnum_BattleX_EBXDamageType_Statics::EnumParams = {
	(UObject*(*)())Z_Construct_UPackage__Script_BattleX,
	nullptr,
	"EBXDamageType",
	"EBXDamageType",
	Z_Construct_UEnum_BattleX_EBXDamageType_Statics::Enumerators,
	RF_Public|RF_Transient|RF_MarkAsNative,
	UE_ARRAY_COUNT(Z_Construct_UEnum_BattleX_EBXDamageType_Statics::Enumerators),
	EEnumFlags::None,
	(uint8)UEnum::ECppForm::EnumClass,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UEnum_BattleX_EBXDamageType_Statics::Enum_MetaDataParams), Z_Construct_UEnum_BattleX_EBXDamageType_Statics::Enum_MetaDataParams)
};
UEnum* Z_Construct_UEnum_BattleX_EBXDamageType()
{
	if (!Z_Registration_Info_UEnum_EBXDamageType.InnerSingleton)
	{
		UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_EBXDamageType.InnerSingleton, Z_Construct_UEnum_BattleX_EBXDamageType_Statics::EnumParams);
	}
	return Z_Registration_Info_UEnum_EBXDamageType.InnerSingleton;
}
// End Enum EBXDamageType

// Begin Enum EBXStaggerState
static FEnumRegistrationInfo Z_Registration_Info_UEnum_EBXStaggerState;
static UEnum* EBXStaggerState_StaticEnum()
{
	if (!Z_Registration_Info_UEnum_EBXStaggerState.OuterSingleton)
	{
		Z_Registration_Info_UEnum_EBXStaggerState.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_BattleX_EBXStaggerState, (UObject*)Z_Construct_UPackage__Script_BattleX(), TEXT("EBXStaggerState"));
	}
	return Z_Registration_Info_UEnum_EBXStaggerState.OuterSingleton;
}
template<> BATTLEX_API UEnum* StaticEnum<EBXStaggerState>()
{
	return EBXStaggerState_StaticEnum();
}
struct Z_Construct_UEnum_BattleX_EBXStaggerState_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
		{ "Bitflags", "" },
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe8\xa7\x92\xe8\x89\xb2\xe5\x8f\x97\xe5\x87\xbb\xe7\xa1\xac\xe7\x9b\xb4\xe7\x8a\xb6\xe6\x80\x81\xe6\x9e\x9a\xe4\xb8\xbe\n" },
#endif
		{ "ModuleRelativePath", "BXEnums.h" },
		{ "SS_008.Hidden", "" },
		{ "SS_008.Name", "EBXStaggerState::SS_008" },
		{ "SS_009.Hidden", "" },
		{ "SS_009.Name", "EBXStaggerState::SS_009" },
		{ "SS_010.Hidden", "" },
		{ "SS_010.Name", "EBXStaggerState::SS_010" },
		{ "SS_011.Hidden", "" },
		{ "SS_011.Name", "EBXStaggerState::SS_011" },
		{ "SS_012.Hidden", "" },
		{ "SS_012.Name", "EBXStaggerState::SS_012" },
		{ "SS_013.Hidden", "" },
		{ "SS_013.Name", "EBXStaggerState::SS_013" },
		{ "SS_014.Hidden", "" },
		{ "SS_014.Name", "EBXStaggerState::SS_014" },
		{ "SS_015.Hidden", "" },
		{ "SS_015.Name", "EBXStaggerState::SS_015" },
		{ "SS_Crawl.DisplayName", "\xe8\xb6\xb4\xe4\xb8\x8b" },
		{ "SS_Crawl.Name", "EBXStaggerState::SS_Crawl" },
		{ "SS_Execution.DisplayName", "\xe5\xa4\x84\xe5\x86\xb3" },
		{ "SS_Execution.Name", "EBXStaggerState::SS_Execution" },
		{ "SS_Fall.DisplayName", "\xe5\x9d\xa0\xe8\x90\xbd" },
		{ "SS_Fall.Name", "EBXStaggerState::SS_Fall" },
		{ "SS_Float.DisplayName", "\xe6\xbc\x82\xe6\xb5\xae" },
		{ "SS_Float.Name", "EBXStaggerState::SS_Float" },
		{ "SS_LieDown.DisplayName", "\xe8\xba\xba\xe5\x80\x92" },
		{ "SS_LieDown.Name", "EBXStaggerState::SS_LieDown" },
		{ "SS_Normal.DisplayName", "\xe6\x97\xa0\xe7\xa1\xac\xe7\x9b\xb4" },
		{ "SS_Normal.Name", "EBXStaggerState::SS_Normal" },
		{ "SS_Repel.DisplayName", "\xe5\x87\xbb\xe9\x80\x80" },
		{ "SS_Repel.Name", "EBXStaggerState::SS_Repel" },
		{ "SS_Rise.DisplayName", "\xe4\xb8\x8a\xe6\xb5\xae" },
		{ "SS_Rise.Name", "EBXStaggerState::SS_Rise" },
		{ "SS_TMax.Hidden", "" },
		{ "SS_TMax.Name", "EBXStaggerState::SS_TMax" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe8\xa7\x92\xe8\x89\xb2\xe5\x8f\x97\xe5\x87\xbb\xe7\xa1\xac\xe7\x9b\xb4\xe7\x8a\xb6\xe6\x80\x81\xe6\x9e\x9a\xe4\xb8\xbe" },
#endif
	};
#endif // WITH_METADATA
	static constexpr UECodeGen_Private::FEnumeratorParam Enumerators[] = {
		{ "EBXStaggerState::SS_Normal", (int64)EBXStaggerState::SS_Normal },
		{ "EBXStaggerState::SS_Repel", (int64)EBXStaggerState::SS_Repel },
		{ "EBXStaggerState::SS_LieDown", (int64)EBXStaggerState::SS_LieDown },
		{ "EBXStaggerState::SS_Crawl", (int64)EBXStaggerState::SS_Crawl },
		{ "EBXStaggerState::SS_Rise", (int64)EBXStaggerState::SS_Rise },
		{ "EBXStaggerState::SS_Fall", (int64)EBXStaggerState::SS_Fall },
		{ "EBXStaggerState::SS_Float", (int64)EBXStaggerState::SS_Float },
		{ "EBXStaggerState::SS_Execution", (int64)EBXStaggerState::SS_Execution },
		{ "EBXStaggerState::SS_008", (int64)EBXStaggerState::SS_008 },
		{ "EBXStaggerState::SS_009", (int64)EBXStaggerState::SS_009 },
		{ "EBXStaggerState::SS_010", (int64)EBXStaggerState::SS_010 },
		{ "EBXStaggerState::SS_011", (int64)EBXStaggerState::SS_011 },
		{ "EBXStaggerState::SS_012", (int64)EBXStaggerState::SS_012 },
		{ "EBXStaggerState::SS_013", (int64)EBXStaggerState::SS_013 },
		{ "EBXStaggerState::SS_014", (int64)EBXStaggerState::SS_014 },
		{ "EBXStaggerState::SS_015", (int64)EBXStaggerState::SS_015 },
		{ "EBXStaggerState::SS_TMax", (int64)EBXStaggerState::SS_TMax },
	};
	static const UECodeGen_Private::FEnumParams EnumParams;
};
const UECodeGen_Private::FEnumParams Z_Construct_UEnum_BattleX_EBXStaggerState_Statics::EnumParams = {
	(UObject*(*)())Z_Construct_UPackage__Script_BattleX,
	nullptr,
	"EBXStaggerState",
	"EBXStaggerState",
	Z_Construct_UEnum_BattleX_EBXStaggerState_Statics::Enumerators,
	RF_Public|RF_Transient|RF_MarkAsNative,
	UE_ARRAY_COUNT(Z_Construct_UEnum_BattleX_EBXStaggerState_Statics::Enumerators),
	EEnumFlags::None,
	(uint8)UEnum::ECppForm::EnumClass,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UEnum_BattleX_EBXStaggerState_Statics::Enum_MetaDataParams), Z_Construct_UEnum_BattleX_EBXStaggerState_Statics::Enum_MetaDataParams)
};
UEnum* Z_Construct_UEnum_BattleX_EBXStaggerState()
{
	if (!Z_Registration_Info_UEnum_EBXStaggerState.InnerSingleton)
	{
		UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_EBXStaggerState.InnerSingleton, Z_Construct_UEnum_BattleX_EBXStaggerState_Statics::EnumParams);
	}
	return Z_Registration_Info_UEnum_EBXStaggerState.InnerSingleton;
}
// End Enum EBXStaggerState

// Begin Enum EBXAttackType
static FEnumRegistrationInfo Z_Registration_Info_UEnum_EBXAttackType;
static UEnum* EBXAttackType_StaticEnum()
{
	if (!Z_Registration_Info_UEnum_EBXAttackType.OuterSingleton)
	{
		Z_Registration_Info_UEnum_EBXAttackType.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_BattleX_EBXAttackType, (UObject*)Z_Construct_UPackage__Script_BattleX(), TEXT("EBXAttackType"));
	}
	return Z_Registration_Info_UEnum_EBXAttackType.OuterSingleton;
}
template<> BATTLEX_API UEnum* StaticEnum<EBXAttackType>()
{
	return EBXAttackType_StaticEnum();
}
struct Z_Construct_UEnum_BattleX_EBXAttackType_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
		{ "AT_BlowDown.DisplayName", "\xe5\x87\xbb\xe8\x90\xbd" },
		{ "AT_BlowDown.Name", "EBXAttackType::AT_BlowDown" },
		{ "AT_BlowFly.DisplayName", "\xe5\x87\xbb\xe9\xa3\x9e" },
		{ "AT_BlowFly.Name", "EBXAttackType::AT_BlowFly" },
		{ "AT_Drag.DisplayName", "\xe6\x8b\x96\xe6\x8b\xbd" },
		{ "AT_Drag.Name", "EBXAttackType::AT_Drag" },
		{ "AT_HitDown.DisplayName", "\xe5\x87\xbb\xe5\x80\x92" },
		{ "AT_HitDown.Name", "EBXAttackType::AT_HitDown" },
		{ "AT_LiftUp.DisplayName", "\xe6\x8c\x91\xe7\xa9\xba" },
		{ "AT_LiftUp.Name", "EBXAttackType::AT_LiftUp" },
		{ "AT_Light.DisplayName", "\xe8\xbd\xbb\xe6\x94\xbb\xe5\x87\xbb" },
		{ "AT_Light.Name", "EBXAttackType::AT_Light" },
		{ "AT_Repel.DisplayName", "\xe5\x87\xbb\xe9\x80\x80" },
		{ "AT_Repel.Name", "EBXAttackType::AT_Repel" },
		{ "AT_TMax.Hidden", "" },
		{ "AT_TMax.Name", "EBXAttackType::AT_TMax" },
		{ "Bitflags", "" },
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe6\x94\xbb\xe5\x87\xbb\xe7\xb1\xbb\xe5\x9e\x8b\n" },
#endif
		{ "ModuleRelativePath", "BXEnums.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe6\x94\xbb\xe5\x87\xbb\xe7\xb1\xbb\xe5\x9e\x8b" },
#endif
	};
#endif // WITH_METADATA
	static constexpr UECodeGen_Private::FEnumeratorParam Enumerators[] = {
		{ "EBXAttackType::AT_Light", (int64)EBXAttackType::AT_Light },
		{ "EBXAttackType::AT_Repel", (int64)EBXAttackType::AT_Repel },
		{ "EBXAttackType::AT_HitDown", (int64)EBXAttackType::AT_HitDown },
		{ "EBXAttackType::AT_BlowFly", (int64)EBXAttackType::AT_BlowFly },
		{ "EBXAttackType::AT_LiftUp", (int64)EBXAttackType::AT_LiftUp },
		{ "EBXAttackType::AT_BlowDown", (int64)EBXAttackType::AT_BlowDown },
		{ "EBXAttackType::AT_Drag", (int64)EBXAttackType::AT_Drag },
		{ "EBXAttackType::AT_TMax", (int64)EBXAttackType::AT_TMax },
	};
	static const UECodeGen_Private::FEnumParams EnumParams;
};
const UECodeGen_Private::FEnumParams Z_Construct_UEnum_BattleX_EBXAttackType_Statics::EnumParams = {
	(UObject*(*)())Z_Construct_UPackage__Script_BattleX,
	nullptr,
	"EBXAttackType",
	"EBXAttackType",
	Z_Construct_UEnum_BattleX_EBXAttackType_Statics::Enumerators,
	RF_Public|RF_Transient|RF_MarkAsNative,
	UE_ARRAY_COUNT(Z_Construct_UEnum_BattleX_EBXAttackType_Statics::Enumerators),
	EEnumFlags::None,
	(uint8)UEnum::ECppForm::EnumClass,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UEnum_BattleX_EBXAttackType_Statics::Enum_MetaDataParams), Z_Construct_UEnum_BattleX_EBXAttackType_Statics::Enum_MetaDataParams)
};
UEnum* Z_Construct_UEnum_BattleX_EBXAttackType()
{
	if (!Z_Registration_Info_UEnum_EBXAttackType.InnerSingleton)
	{
		UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_EBXAttackType.InnerSingleton, Z_Construct_UEnum_BattleX_EBXAttackType_Statics::EnumParams);
	}
	return Z_Registration_Info_UEnum_EBXAttackType.InnerSingleton;
}
// End Enum EBXAttackType

// Begin Enum EBXAttackForce
static FEnumRegistrationInfo Z_Registration_Info_UEnum_EBXAttackForce;
static UEnum* EBXAttackForce_StaticEnum()
{
	if (!Z_Registration_Info_UEnum_EBXAttackForce.OuterSingleton)
	{
		Z_Registration_Info_UEnum_EBXAttackForce.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_BattleX_EBXAttackForce, (UObject*)Z_Construct_UPackage__Script_BattleX(), TEXT("EBXAttackForce"));
	}
	return Z_Registration_Info_UEnum_EBXAttackForce.OuterSingleton;
}
template<> BATTLEX_API UEnum* StaticEnum<EBXAttackForce>()
{
	return EBXAttackForce_StaticEnum();
}
struct Z_Construct_UEnum_BattleX_EBXAttackForce_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
		{ "AF_0.DisplayName", "\xe5\x8a\x9b\xe5\xba\xa6""0" },
		{ "AF_0.Name", "EBXAttackForce::AF_0" },
		{ "AF_1.DisplayName", "\xe5\x8a\x9b\xe5\xba\xa6""1" },
		{ "AF_1.Name", "EBXAttackForce::AF_1" },
		{ "AF_2.DisplayName", "\xe5\x8a\x9b\xe5\xba\xa6""2" },
		{ "AF_2.Name", "EBXAttackForce::AF_2" },
		{ "AF_3.DisplayName", "\xe5\x8a\x9b\xe5\xba\xa6""3" },
		{ "AF_3.Name", "EBXAttackForce::AF_3" },
		{ "AF_4.DisplayName", "\xe5\x8a\x9b\xe5\xba\xa6""4" },
		{ "AF_4.Name", "EBXAttackForce::AF_4" },
		{ "AF_5.DisplayName", "\xe5\x8a\x9b\xe5\xba\xa6""5" },
		{ "AF_5.Name", "EBXAttackForce::AF_5" },
		{ "AF_TMax.Hidden", "" },
		{ "AF_TMax.Name", "EBXAttackForce::AF_TMax" },
		{ "Bitflags", "" },
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe6\x94\xbb\xe5\x87\xbb\xe5\x8a\x9b\xe5\xba\xa6\n" },
#endif
		{ "ModuleRelativePath", "BXEnums.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe6\x94\xbb\xe5\x87\xbb\xe5\x8a\x9b\xe5\xba\xa6" },
#endif
	};
#endif // WITH_METADATA
	static constexpr UECodeGen_Private::FEnumeratorParam Enumerators[] = {
		{ "EBXAttackForce::AF_0", (int64)EBXAttackForce::AF_0 },
		{ "EBXAttackForce::AF_1", (int64)EBXAttackForce::AF_1 },
		{ "EBXAttackForce::AF_2", (int64)EBXAttackForce::AF_2 },
		{ "EBXAttackForce::AF_3", (int64)EBXAttackForce::AF_3 },
		{ "EBXAttackForce::AF_4", (int64)EBXAttackForce::AF_4 },
		{ "EBXAttackForce::AF_5", (int64)EBXAttackForce::AF_5 },
		{ "EBXAttackForce::AF_TMax", (int64)EBXAttackForce::AF_TMax },
	};
	static const UECodeGen_Private::FEnumParams EnumParams;
};
const UECodeGen_Private::FEnumParams Z_Construct_UEnum_BattleX_EBXAttackForce_Statics::EnumParams = {
	(UObject*(*)())Z_Construct_UPackage__Script_BattleX,
	nullptr,
	"EBXAttackForce",
	"EBXAttackForce",
	Z_Construct_UEnum_BattleX_EBXAttackForce_Statics::Enumerators,
	RF_Public|RF_Transient|RF_MarkAsNative,
	UE_ARRAY_COUNT(Z_Construct_UEnum_BattleX_EBXAttackForce_Statics::Enumerators),
	EEnumFlags::None,
	(uint8)UEnum::ECppForm::EnumClass,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UEnum_BattleX_EBXAttackForce_Statics::Enum_MetaDataParams), Z_Construct_UEnum_BattleX_EBXAttackForce_Statics::Enum_MetaDataParams)
};
UEnum* Z_Construct_UEnum_BattleX_EBXAttackForce()
{
	if (!Z_Registration_Info_UEnum_EBXAttackForce.InnerSingleton)
	{
		UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_EBXAttackForce.InnerSingleton, Z_Construct_UEnum_BattleX_EBXAttackForce_Statics::EnumParams);
	}
	return Z_Registration_Info_UEnum_EBXAttackForce.InnerSingleton;
}
// End Enum EBXAttackForce

// Begin Enum EBXAttackerLocationType
static FEnumRegistrationInfo Z_Registration_Info_UEnum_EBXAttackerLocationType;
static UEnum* EBXAttackerLocationType_StaticEnum()
{
	if (!Z_Registration_Info_UEnum_EBXAttackerLocationType.OuterSingleton)
	{
		Z_Registration_Info_UEnum_EBXAttackerLocationType.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_BattleX_EBXAttackerLocationType, (UObject*)Z_Construct_UPackage__Script_BattleX(), TEXT("EBXAttackerLocationType"));
	}
	return Z_Registration_Info_UEnum_EBXAttackerLocationType.OuterSingleton;
}
template<> BATTLEX_API UEnum* StaticEnum<EBXAttackerLocationType>()
{
	return EBXAttackerLocationType_StaticEnum();
}
struct Z_Construct_UEnum_BattleX_EBXAttackerLocationType_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
		{ "ALT_Back.DisplayName", "\xe5\x90\x8e\xe6\x96\xb9" },
		{ "ALT_Back.Name", "EBXAttackerLocationType::ALT_Back" },
		{ "ALT_Front.DisplayName", "\xe5\x89\x8d\xe6\x96\xb9" },
		{ "ALT_Front.Name", "EBXAttackerLocationType::ALT_Front" },
		{ "ALT_Left.DisplayName", "\xe5\xb7\xa6\xe6\x96\xb9" },
		{ "ALT_Left.Name", "EBXAttackerLocationType::ALT_Left" },
		{ "ALT_Right.DisplayName", "\xe5\x8f\xb3\xe6\x96\xb9" },
		{ "ALT_Right.Name", "EBXAttackerLocationType::ALT_Right" },
		{ "ALT_TMax.Name", "EBXAttackerLocationType::ALT_TMax" },
		{ "Bitflags", "" },
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe6\x94\xbb\xe5\x87\xbb\xe8\x80\x85\xe7\x9b\xb8\xe5\xaf\xb9\xe4\xba\x8e\xe5\x8f\x97\xe5\x87\xbb\xe8\x80\x85\xe7\x9a\x84\xe6\x96\xb9\xe5\x90\x91\n" },
#endif
		{ "ModuleRelativePath", "BXEnums.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe6\x94\xbb\xe5\x87\xbb\xe8\x80\x85\xe7\x9b\xb8\xe5\xaf\xb9\xe4\xba\x8e\xe5\x8f\x97\xe5\x87\xbb\xe8\x80\x85\xe7\x9a\x84\xe6\x96\xb9\xe5\x90\x91" },
#endif
	};
#endif // WITH_METADATA
	static constexpr UECodeGen_Private::FEnumeratorParam Enumerators[] = {
		{ "EBXAttackerLocationType::ALT_Front", (int64)EBXAttackerLocationType::ALT_Front },
		{ "EBXAttackerLocationType::ALT_Back", (int64)EBXAttackerLocationType::ALT_Back },
		{ "EBXAttackerLocationType::ALT_Left", (int64)EBXAttackerLocationType::ALT_Left },
		{ "EBXAttackerLocationType::ALT_Right", (int64)EBXAttackerLocationType::ALT_Right },
		{ "EBXAttackerLocationType::ALT_TMax", (int64)EBXAttackerLocationType::ALT_TMax },
	};
	static const UECodeGen_Private::FEnumParams EnumParams;
};
const UECodeGen_Private::FEnumParams Z_Construct_UEnum_BattleX_EBXAttackerLocationType_Statics::EnumParams = {
	(UObject*(*)())Z_Construct_UPackage__Script_BattleX,
	nullptr,
	"EBXAttackerLocationType",
	"EBXAttackerLocationType",
	Z_Construct_UEnum_BattleX_EBXAttackerLocationType_Statics::Enumerators,
	RF_Public|RF_Transient|RF_MarkAsNative,
	UE_ARRAY_COUNT(Z_Construct_UEnum_BattleX_EBXAttackerLocationType_Statics::Enumerators),
	EEnumFlags::None,
	(uint8)UEnum::ECppForm::EnumClass,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UEnum_BattleX_EBXAttackerLocationType_Statics::Enum_MetaDataParams), Z_Construct_UEnum_BattleX_EBXAttackerLocationType_Statics::Enum_MetaDataParams)
};
UEnum* Z_Construct_UEnum_BattleX_EBXAttackerLocationType()
{
	if (!Z_Registration_Info_UEnum_EBXAttackerLocationType.InnerSingleton)
	{
		UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_EBXAttackerLocationType.InnerSingleton, Z_Construct_UEnum_BattleX_EBXAttackerLocationType_Statics::EnumParams);
	}
	return Z_Registration_Info_UEnum_EBXAttackerLocationType.InnerSingleton;
}
// End Enum EBXAttackerLocationType

// Begin Enum EBXDataType
static FEnumRegistrationInfo Z_Registration_Info_UEnum_EBXDataType;
static UEnum* EBXDataType_StaticEnum()
{
	if (!Z_Registration_Info_UEnum_EBXDataType.OuterSingleton)
	{
		Z_Registration_Info_UEnum_EBXDataType.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_BattleX_EBXDataType, (UObject*)Z_Construct_UPackage__Script_BattleX(), TEXT("EBXDataType"));
	}
	return Z_Registration_Info_UEnum_EBXDataType.OuterSingleton;
}
template<> BATTLEX_API UEnum* StaticEnum<EBXDataType>()
{
	return EBXDataType_StaticEnum();
}
struct Z_Construct_UEnum_BattleX_EBXDataType_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe6\x95\xb0\xe6\x8d\xae\xe7\xb1\xbb\xe5\x9e\x8b\n" },
#endif
		{ "DT_Float.DisplayName", "\xe6\xb5\xae\xe7\x82\xb9\xe5\x9e\x8b" },
		{ "DT_Float.Name", "EBXDataType::DT_Float" },
		{ "DT_Integer.DisplayName", "\xe6\x95\xb4\xe5\x9e\x8b" },
		{ "DT_Integer.Name", "EBXDataType::DT_Integer" },
		{ "DT_Struct.DisplayName", "\xe7\xbb\x93\xe6\x9e\x84\xe4\xbd\x93" },
		{ "DT_Struct.Name", "EBXDataType::DT_Struct" },
		{ "DT_TMax.Hidden", "" },
		{ "DT_TMax.Name", "EBXDataType::DT_TMax" },
		{ "ModuleRelativePath", "BXEnums.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe6\x95\xb0\xe6\x8d\xae\xe7\xb1\xbb\xe5\x9e\x8b" },
#endif
	};
#endif // WITH_METADATA
	static constexpr UECodeGen_Private::FEnumeratorParam Enumerators[] = {
		{ "EBXDataType::DT_Integer", (int64)EBXDataType::DT_Integer },
		{ "EBXDataType::DT_Float", (int64)EBXDataType::DT_Float },
		{ "EBXDataType::DT_Struct", (int64)EBXDataType::DT_Struct },
		{ "EBXDataType::DT_TMax", (int64)EBXDataType::DT_TMax },
	};
	static const UECodeGen_Private::FEnumParams EnumParams;
};
const UECodeGen_Private::FEnumParams Z_Construct_UEnum_BattleX_EBXDataType_Statics::EnumParams = {
	(UObject*(*)())Z_Construct_UPackage__Script_BattleX,
	nullptr,
	"EBXDataType",
	"EBXDataType",
	Z_Construct_UEnum_BattleX_EBXDataType_Statics::Enumerators,
	RF_Public|RF_Transient|RF_MarkAsNative,
	UE_ARRAY_COUNT(Z_Construct_UEnum_BattleX_EBXDataType_Statics::Enumerators),
	EEnumFlags::None,
	(uint8)UEnum::ECppForm::EnumClass,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UEnum_BattleX_EBXDataType_Statics::Enum_MetaDataParams), Z_Construct_UEnum_BattleX_EBXDataType_Statics::Enum_MetaDataParams)
};
UEnum* Z_Construct_UEnum_BattleX_EBXDataType()
{
	if (!Z_Registration_Info_UEnum_EBXDataType.InnerSingleton)
	{
		UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_EBXDataType.InnerSingleton, Z_Construct_UEnum_BattleX_EBXDataType_Statics::EnumParams);
	}
	return Z_Registration_Info_UEnum_EBXDataType.InnerSingleton;
}
// End Enum EBXDataType

// Begin Enum EBXShapeType
static FEnumRegistrationInfo Z_Registration_Info_UEnum_EBXShapeType;
static UEnum* EBXShapeType_StaticEnum()
{
	if (!Z_Registration_Info_UEnum_EBXShapeType.OuterSingleton)
	{
		Z_Registration_Info_UEnum_EBXShapeType.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_BattleX_EBXShapeType, (UObject*)Z_Construct_UPackage__Script_BattleX(), TEXT("EBXShapeType"));
	}
	return Z_Registration_Info_UEnum_EBXShapeType.OuterSingleton;
}
template<> BATTLEX_API UEnum* StaticEnum<EBXShapeType>()
{
	return EBXShapeType_StaticEnum();
}
struct Z_Construct_UEnum_BattleX_EBXShapeType_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe5\xbd\xa2\xe7\x8a\xb6\xe7\xb1\xbb\xe5\x9e\x8b\n" },
#endif
		{ "ModuleRelativePath", "BXEnums.h" },
		{ "ST_Box.DisplayName", "\xe9\x95\xbf\xe6\x96\xb9\xe4\xbd\x93" },
		{ "ST_Box.Name", "EBXShapeType::ST_Box" },
		{ "ST_Capsule.DisplayName", "\xe8\x83\xb6\xe5\x9b\x8a\xe4\xbd\x93" },
		{ "ST_Capsule.Name", "EBXShapeType::ST_Capsule" },
		{ "ST_Sphere.DisplayName", "\xe7\x90\x83\xe4\xbd\x93" },
		{ "ST_Sphere.Name", "EBXShapeType::ST_Sphere" },
		{ "ST_TMax.Hidden", "" },
		{ "ST_TMax.Name", "EBXShapeType::ST_TMax" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe5\xbd\xa2\xe7\x8a\xb6\xe7\xb1\xbb\xe5\x9e\x8b" },
#endif
	};
#endif // WITH_METADATA
	static constexpr UECodeGen_Private::FEnumeratorParam Enumerators[] = {
		{ "EBXShapeType::ST_Sphere", (int64)EBXShapeType::ST_Sphere },
		{ "EBXShapeType::ST_Capsule", (int64)EBXShapeType::ST_Capsule },
		{ "EBXShapeType::ST_Box", (int64)EBXShapeType::ST_Box },
		{ "EBXShapeType::ST_TMax", (int64)EBXShapeType::ST_TMax },
	};
	static const UECodeGen_Private::FEnumParams EnumParams;
};
const UECodeGen_Private::FEnumParams Z_Construct_UEnum_BattleX_EBXShapeType_Statics::EnumParams = {
	(UObject*(*)())Z_Construct_UPackage__Script_BattleX,
	nullptr,
	"EBXShapeType",
	"EBXShapeType",
	Z_Construct_UEnum_BattleX_EBXShapeType_Statics::Enumerators,
	RF_Public|RF_Transient|RF_MarkAsNative,
	UE_ARRAY_COUNT(Z_Construct_UEnum_BattleX_EBXShapeType_Statics::Enumerators),
	EEnumFlags::None,
	(uint8)UEnum::ECppForm::EnumClass,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UEnum_BattleX_EBXShapeType_Statics::Enum_MetaDataParams), Z_Construct_UEnum_BattleX_EBXShapeType_Statics::Enum_MetaDataParams)
};
UEnum* Z_Construct_UEnum_BattleX_EBXShapeType()
{
	if (!Z_Registration_Info_UEnum_EBXShapeType.InnerSingleton)
	{
		UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_EBXShapeType.InnerSingleton, Z_Construct_UEnum_BattleX_EBXShapeType_Statics::EnumParams);
	}
	return Z_Registration_Info_UEnum_EBXShapeType.InnerSingleton;
}
// End Enum EBXShapeType

// Begin Enum EBXDynamicMaterialParamType
static FEnumRegistrationInfo Z_Registration_Info_UEnum_EBXDynamicMaterialParamType;
static UEnum* EBXDynamicMaterialParamType_StaticEnum()
{
	if (!Z_Registration_Info_UEnum_EBXDynamicMaterialParamType.OuterSingleton)
	{
		Z_Registration_Info_UEnum_EBXDynamicMaterialParamType.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_BattleX_EBXDynamicMaterialParamType, (UObject*)Z_Construct_UPackage__Script_BattleX(), TEXT("EBXDynamicMaterialParamType"));
	}
	return Z_Registration_Info_UEnum_EBXDynamicMaterialParamType.OuterSingleton;
}
template<> BATTLEX_API UEnum* StaticEnum<EBXDynamicMaterialParamType>()
{
	return EBXDynamicMaterialParamType_StaticEnum();
}
struct Z_Construct_UEnum_BattleX_EBXDynamicMaterialParamType_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe5\x8a\xa8\xe6\x80\x81\xe6\x9d\x90\xe8\xb4\xa8\xe5\x8f\x82\xe6\x95\xb0\xe7\xb1\xbb\xe5\x9e\x8b\n" },
#endif
		{ "DMPT_LinearColor.DisplayName", "\xe9\xa2\x9c\xe8\x89\xb2" },
		{ "DMPT_LinearColor.Name", "EBXDynamicMaterialParamType::DMPT_LinearColor" },
		{ "DMPT_Scalar.DisplayName", "\xe6\xa0\x87\xe9\x87\x8f" },
		{ "DMPT_Scalar.Name", "EBXDynamicMaterialParamType::DMPT_Scalar" },
		{ "DMPT_Texture.DisplayName", "\xe7\xba\xb9\xe7\x90\x86" },
		{ "DMPT_Texture.Name", "EBXDynamicMaterialParamType::DMPT_Texture" },
		{ "DMPT_TMax.Hidden", "" },
		{ "DMPT_TMax.Name", "EBXDynamicMaterialParamType::DMPT_TMax" },
		{ "DMPT_Vector.DisplayName", "\xe5\x90\x91\xe9\x87\x8f" },
		{ "DMPT_Vector.Name", "EBXDynamicMaterialParamType::DMPT_Vector" },
		{ "ModuleRelativePath", "BXEnums.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe5\x8a\xa8\xe6\x80\x81\xe6\x9d\x90\xe8\xb4\xa8\xe5\x8f\x82\xe6\x95\xb0\xe7\xb1\xbb\xe5\x9e\x8b" },
#endif
	};
#endif // WITH_METADATA
	static constexpr UECodeGen_Private::FEnumeratorParam Enumerators[] = {
		{ "EBXDynamicMaterialParamType::DMPT_Scalar", (int64)EBXDynamicMaterialParamType::DMPT_Scalar },
		{ "EBXDynamicMaterialParamType::DMPT_Vector", (int64)EBXDynamicMaterialParamType::DMPT_Vector },
		{ "EBXDynamicMaterialParamType::DMPT_Texture", (int64)EBXDynamicMaterialParamType::DMPT_Texture },
		{ "EBXDynamicMaterialParamType::DMPT_LinearColor", (int64)EBXDynamicMaterialParamType::DMPT_LinearColor },
		{ "EBXDynamicMaterialParamType::DMPT_TMax", (int64)EBXDynamicMaterialParamType::DMPT_TMax },
	};
	static const UECodeGen_Private::FEnumParams EnumParams;
};
const UECodeGen_Private::FEnumParams Z_Construct_UEnum_BattleX_EBXDynamicMaterialParamType_Statics::EnumParams = {
	(UObject*(*)())Z_Construct_UPackage__Script_BattleX,
	nullptr,
	"EBXDynamicMaterialParamType",
	"EBXDynamicMaterialParamType",
	Z_Construct_UEnum_BattleX_EBXDynamicMaterialParamType_Statics::Enumerators,
	RF_Public|RF_Transient|RF_MarkAsNative,
	UE_ARRAY_COUNT(Z_Construct_UEnum_BattleX_EBXDynamicMaterialParamType_Statics::Enumerators),
	EEnumFlags::None,
	(uint8)UEnum::ECppForm::EnumClass,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UEnum_BattleX_EBXDynamicMaterialParamType_Statics::Enum_MetaDataParams), Z_Construct_UEnum_BattleX_EBXDynamicMaterialParamType_Statics::Enum_MetaDataParams)
};
UEnum* Z_Construct_UEnum_BattleX_EBXDynamicMaterialParamType()
{
	if (!Z_Registration_Info_UEnum_EBXDynamicMaterialParamType.InnerSingleton)
	{
		UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_EBXDynamicMaterialParamType.InnerSingleton, Z_Construct_UEnum_BattleX_EBXDynamicMaterialParamType_Statics::EnumParams);
	}
	return Z_Registration_Info_UEnum_EBXDynamicMaterialParamType.InnerSingleton;
}
// End Enum EBXDynamicMaterialParamType

// Begin Enum EBXLockTargetType
static FEnumRegistrationInfo Z_Registration_Info_UEnum_EBXLockTargetType;
static UEnum* EBXLockTargetType_StaticEnum()
{
	if (!Z_Registration_Info_UEnum_EBXLockTargetType.OuterSingleton)
	{
		Z_Registration_Info_UEnum_EBXLockTargetType.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_BattleX_EBXLockTargetType, (UObject*)Z_Construct_UPackage__Script_BattleX(), TEXT("EBXLockTargetType"));
	}
	return Z_Registration_Info_UEnum_EBXLockTargetType.OuterSingleton;
}
template<> BATTLEX_API UEnum* StaticEnum<EBXLockTargetType>()
{
	return EBXLockTargetType_StaticEnum();
}
struct Z_Construct_UEnum_BattleX_EBXLockTargetType_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe7\x9b\xae\xe6\xa0\x87\xe9\x94\x81\xe5\xae\x9a\xe7\xb1\xbb\xe5\x9e\x8b\n" },
#endif
		{ "ModuleRelativePath", "BXEnums.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe7\x9b\xae\xe6\xa0\x87\xe9\x94\x81\xe5\xae\x9a\xe7\xb1\xbb\xe5\x9e\x8b" },
#endif
		{ "TT_ExtraFour.DisplayName", "\xe6\x89\xa9\xe5\xb1\x95\xe7\x9b\xae\xe6\xa0\x87\xe5\x9b\x9b" },
		{ "TT_ExtraFour.Name", "EBXLockTargetType::TT_ExtraFour" },
		{ "TT_ExtraOne.DisplayName", "\xe6\x89\xa9\xe5\xb1\x95\xe7\x9b\xae\xe6\xa0\x87\xe4\xb8\x80" },
		{ "TT_ExtraOne.Name", "EBXLockTargetType::TT_ExtraOne" },
		{ "TT_ExtraThree.DisplayName", "\xe6\x89\xa9\xe5\xb1\x95\xe7\x9b\xae\xe6\xa0\x87\xe4\xb8\x89" },
		{ "TT_ExtraThree.Name", "EBXLockTargetType::TT_ExtraThree" },
		{ "TT_ExtraTwo.DisplayName", "\xe6\x89\xa9\xe5\xb1\x95\xe7\x9b\xae\xe6\xa0\x87\xe4\xba\x8c" },
		{ "TT_ExtraTwo.Name", "EBXLockTargetType::TT_ExtraTwo" },
		{ "TT_Skill.DisplayName", "\xe6\x8a\x80\xe8\x83\xbd\xe7\x9b\xae\xe6\xa0\x87" },
		{ "TT_Skill.Name", "EBXLockTargetType::TT_Skill" },
		{ "TT_TMax.DisplayName", "\xe6\x97\xa0\xe6\x95\x88\xe5\x80\xbc" },
		{ "TT_TMax.Name", "EBXLockTargetType::TT_TMax" },
	};
#endif // WITH_METADATA
	static constexpr UECodeGen_Private::FEnumeratorParam Enumerators[] = {
		{ "EBXLockTargetType::TT_Skill", (int64)EBXLockTargetType::TT_Skill },
		{ "EBXLockTargetType::TT_ExtraOne", (int64)EBXLockTargetType::TT_ExtraOne },
		{ "EBXLockTargetType::TT_ExtraTwo", (int64)EBXLockTargetType::TT_ExtraTwo },
		{ "EBXLockTargetType::TT_ExtraThree", (int64)EBXLockTargetType::TT_ExtraThree },
		{ "EBXLockTargetType::TT_ExtraFour", (int64)EBXLockTargetType::TT_ExtraFour },
		{ "EBXLockTargetType::TT_TMax", (int64)EBXLockTargetType::TT_TMax },
	};
	static const UECodeGen_Private::FEnumParams EnumParams;
};
const UECodeGen_Private::FEnumParams Z_Construct_UEnum_BattleX_EBXLockTargetType_Statics::EnumParams = {
	(UObject*(*)())Z_Construct_UPackage__Script_BattleX,
	nullptr,
	"EBXLockTargetType",
	"EBXLockTargetType",
	Z_Construct_UEnum_BattleX_EBXLockTargetType_Statics::Enumerators,
	RF_Public|RF_Transient|RF_MarkAsNative,
	UE_ARRAY_COUNT(Z_Construct_UEnum_BattleX_EBXLockTargetType_Statics::Enumerators),
	EEnumFlags::None,
	(uint8)UEnum::ECppForm::EnumClass,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UEnum_BattleX_EBXLockTargetType_Statics::Enum_MetaDataParams), Z_Construct_UEnum_BattleX_EBXLockTargetType_Statics::Enum_MetaDataParams)
};
UEnum* Z_Construct_UEnum_BattleX_EBXLockTargetType()
{
	if (!Z_Registration_Info_UEnum_EBXLockTargetType.InnerSingleton)
	{
		UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_EBXLockTargetType.InnerSingleton, Z_Construct_UEnum_BattleX_EBXLockTargetType_Statics::EnumParams);
	}
	return Z_Registration_Info_UEnum_EBXLockTargetType.InnerSingleton;
}
// End Enum EBXLockTargetType

// Begin Enum EBXBodyPartType
static FEnumRegistrationInfo Z_Registration_Info_UEnum_EBXBodyPartType;
static UEnum* EBXBodyPartType_StaticEnum()
{
	if (!Z_Registration_Info_UEnum_EBXBodyPartType.OuterSingleton)
	{
		Z_Registration_Info_UEnum_EBXBodyPartType.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_BattleX_EBXBodyPartType, (UObject*)Z_Construct_UPackage__Script_BattleX(), TEXT("EBXBodyPartType"));
	}
	return Z_Registration_Info_UEnum_EBXBodyPartType.OuterSingleton;
}
template<> BATTLEX_API UEnum* StaticEnum<EBXBodyPartType>()
{
	return EBXBodyPartType_StaticEnum();
}
struct Z_Construct_UEnum_BattleX_EBXBodyPartType_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "BPT_Default.DisplayName", "\xe9\xbb\x98\xe8\xae\xa4" },
		{ "BPT_Default.Name", "EBXBodyPartType::BPT_Default" },
		{ "BPT_Head.DisplayName", "\xe5\xa4\xb4\xe9\x83\xa8" },
		{ "BPT_Head.Name", "EBXBodyPartType::BPT_Head" },
		{ "BPT_Limbs1.DisplayName", "\xe8\x82\xa2\xe4\xbd\x93""1" },
		{ "BPT_Limbs1.Name", "EBXBodyPartType::BPT_Limbs1" },
		{ "BPT_Limbs2.DisplayName", "\xe8\x82\xa2\xe4\xbd\x93""2" },
		{ "BPT_Limbs2.Name", "EBXBodyPartType::BPT_Limbs2" },
		{ "BPT_Limbs3.DisplayName", "\xe8\x82\xa2\xe4\xbd\x93""3" },
		{ "BPT_Limbs3.Name", "EBXBodyPartType::BPT_Limbs3" },
		{ "BPT_Limbs4.DisplayName", "\xe8\x82\xa2\xe4\xbd\x93""4" },
		{ "BPT_Limbs4.Name", "EBXBodyPartType::BPT_Limbs4" },
		{ "BPT_Limbs5.DisplayName", "\xe8\x82\xa2\xe4\xbd\x93""5" },
		{ "BPT_Limbs5.Name", "EBXBodyPartType::BPT_Limbs5" },
		{ "BPT_Limbs6.DisplayName", "\xe8\x82\xa2\xe4\xbd\x93""6" },
		{ "BPT_Limbs6.Name", "EBXBodyPartType::BPT_Limbs6" },
		{ "BPT_Limbs7.DisplayName", "\xe8\x82\xa2\xe4\xbd\x93""7" },
		{ "BPT_Limbs7.Name", "EBXBodyPartType::BPT_Limbs7" },
		{ "BPT_Limbs8.DisplayName", "\xe8\x82\xa2\xe4\xbd\x93""8" },
		{ "BPT_Limbs8.Name", "EBXBodyPartType::BPT_Limbs8" },
		{ "BPT_LowerBody.DisplayName", "\xe4\xb8\x8b\xe5\x8d\x8a\xe8\xba\xab" },
		{ "BPT_LowerBody.Name", "EBXBodyPartType::BPT_LowerBody" },
		{ "BPT_Tail.DisplayName", "\xe5\xb0\xbe\xe5\xb7\xb4" },
		{ "BPT_Tail.Name", "EBXBodyPartType::BPT_Tail" },
		{ "BPT_TMax.DisplayName", "\xe6\x97\xa0\xe6\x95\x88\xe5\x80\xbc" },
		{ "BPT_TMax.Name", "EBXBodyPartType::BPT_TMax" },
		{ "BPT_UpperBody.DisplayName", "\xe4\xb8\x8a\xe5\x8d\x8a\xe8\xba\xab" },
		{ "BPT_UpperBody.Name", "EBXBodyPartType::BPT_UpperBody" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe8\xba\xab\xe4\xbd\x93\xe9\x83\xa8\xe4\xbd\x8d\n" },
#endif
		{ "ModuleRelativePath", "BXEnums.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe8\xba\xab\xe4\xbd\x93\xe9\x83\xa8\xe4\xbd\x8d" },
#endif
	};
#endif // WITH_METADATA
	static constexpr UECodeGen_Private::FEnumeratorParam Enumerators[] = {
		{ "EBXBodyPartType::BPT_Default", (int64)EBXBodyPartType::BPT_Default },
		{ "EBXBodyPartType::BPT_Head", (int64)EBXBodyPartType::BPT_Head },
		{ "EBXBodyPartType::BPT_UpperBody", (int64)EBXBodyPartType::BPT_UpperBody },
		{ "EBXBodyPartType::BPT_LowerBody", (int64)EBXBodyPartType::BPT_LowerBody },
		{ "EBXBodyPartType::BPT_Tail", (int64)EBXBodyPartType::BPT_Tail },
		{ "EBXBodyPartType::BPT_Limbs1", (int64)EBXBodyPartType::BPT_Limbs1 },
		{ "EBXBodyPartType::BPT_Limbs2", (int64)EBXBodyPartType::BPT_Limbs2 },
		{ "EBXBodyPartType::BPT_Limbs3", (int64)EBXBodyPartType::BPT_Limbs3 },
		{ "EBXBodyPartType::BPT_Limbs4", (int64)EBXBodyPartType::BPT_Limbs4 },
		{ "EBXBodyPartType::BPT_Limbs5", (int64)EBXBodyPartType::BPT_Limbs5 },
		{ "EBXBodyPartType::BPT_Limbs6", (int64)EBXBodyPartType::BPT_Limbs6 },
		{ "EBXBodyPartType::BPT_Limbs7", (int64)EBXBodyPartType::BPT_Limbs7 },
		{ "EBXBodyPartType::BPT_Limbs8", (int64)EBXBodyPartType::BPT_Limbs8 },
		{ "EBXBodyPartType::BPT_TMax", (int64)EBXBodyPartType::BPT_TMax },
	};
	static const UECodeGen_Private::FEnumParams EnumParams;
};
const UECodeGen_Private::FEnumParams Z_Construct_UEnum_BattleX_EBXBodyPartType_Statics::EnumParams = {
	(UObject*(*)())Z_Construct_UPackage__Script_BattleX,
	nullptr,
	"EBXBodyPartType",
	"EBXBodyPartType",
	Z_Construct_UEnum_BattleX_EBXBodyPartType_Statics::Enumerators,
	RF_Public|RF_Transient|RF_MarkAsNative,
	UE_ARRAY_COUNT(Z_Construct_UEnum_BattleX_EBXBodyPartType_Statics::Enumerators),
	EEnumFlags::None,
	(uint8)UEnum::ECppForm::EnumClass,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UEnum_BattleX_EBXBodyPartType_Statics::Enum_MetaDataParams), Z_Construct_UEnum_BattleX_EBXBodyPartType_Statics::Enum_MetaDataParams)
};
UEnum* Z_Construct_UEnum_BattleX_EBXBodyPartType()
{
	if (!Z_Registration_Info_UEnum_EBXBodyPartType.InnerSingleton)
	{
		UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_EBXBodyPartType.InnerSingleton, Z_Construct_UEnum_BattleX_EBXBodyPartType_Statics::EnumParams);
	}
	return Z_Registration_Info_UEnum_EBXBodyPartType.InnerSingleton;
}
// End Enum EBXBodyPartType

// Begin Registration
struct Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_BXEnums_h_Statics
{
	static constexpr FEnumRegisterCompiledInInfo EnumInfo[] = {
		{ EBXCompareSign_StaticEnum, TEXT("EBXCompareSign"), &Z_Registration_Info_UEnum_EBXCompareSign, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 869846240U) },
		{ EBXDamageType_StaticEnum, TEXT("EBXDamageType"), &Z_Registration_Info_UEnum_EBXDamageType, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 47245976U) },
		{ EBXStaggerState_StaticEnum, TEXT("EBXStaggerState"), &Z_Registration_Info_UEnum_EBXStaggerState, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 4239013078U) },
		{ EBXAttackType_StaticEnum, TEXT("EBXAttackType"), &Z_Registration_Info_UEnum_EBXAttackType, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 2244227638U) },
		{ EBXAttackForce_StaticEnum, TEXT("EBXAttackForce"), &Z_Registration_Info_UEnum_EBXAttackForce, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 2833003715U) },
		{ EBXAttackerLocationType_StaticEnum, TEXT("EBXAttackerLocationType"), &Z_Registration_Info_UEnum_EBXAttackerLocationType, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 3281591186U) },
		{ EBXDataType_StaticEnum, TEXT("EBXDataType"), &Z_Registration_Info_UEnum_EBXDataType, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 3648073204U) },
		{ EBXShapeType_StaticEnum, TEXT("EBXShapeType"), &Z_Registration_Info_UEnum_EBXShapeType, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 3113872912U) },
		{ EBXDynamicMaterialParamType_StaticEnum, TEXT("EBXDynamicMaterialParamType"), &Z_Registration_Info_UEnum_EBXDynamicMaterialParamType, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 3388700289U) },
		{ EBXLockTargetType_StaticEnum, TEXT("EBXLockTargetType"), &Z_Registration_Info_UEnum_EBXLockTargetType, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 368348470U) },
		{ EBXBodyPartType_StaticEnum, TEXT("EBXBodyPartType"), &Z_Registration_Info_UEnum_EBXBodyPartType, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 84625349U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_BXEnums_h_139124874(TEXT("/Script/BattleX"),
	nullptr, 0,
	nullptr, 0,
	Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_BXEnums_h_Statics::EnumInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_BXEnums_h_Statics::EnumInfo));
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
