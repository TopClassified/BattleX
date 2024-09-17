// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "BattleX/Task/BXTEnums.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeBXTEnums() {}

// Begin Cross Module References
BATTLEX_API UEnum* Z_Construct_UEnum_BattleX_EBXTConnectionType();
BATTLEX_API UEnum* Z_Construct_UEnum_BattleX_EBXTCoordinateType();
BATTLEX_API UEnum* Z_Construct_UEnum_BattleX_EBXTLifeType();
BATTLEX_API UEnum* Z_Construct_UEnum_BattleX_EBXTNetType();
BATTLEX_API UEnum* Z_Construct_UEnum_BattleX_EBXTProcessorFunction();
BATTLEX_API UEnum* Z_Construct_UEnum_BattleX_EBXTTargetType();
BATTLEX_API UEnum* Z_Construct_UEnum_BattleX_EBXTTriggerType();
UPackage* Z_Construct_UPackage__Script_BattleX();
// End Cross Module References

// Begin Enum EBXTTargetType
static FEnumRegistrationInfo Z_Registration_Info_UEnum_EBXTTargetType;
static UEnum* EBXTTargetType_StaticEnum()
{
	if (!Z_Registration_Info_UEnum_EBXTTargetType.OuterSingleton)
	{
		Z_Registration_Info_UEnum_EBXTTargetType.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_BattleX_EBXTTargetType, (UObject*)Z_Construct_UPackage__Script_BattleX(), TEXT("EBXTTargetType"));
	}
	return Z_Registration_Info_UEnum_EBXTTargetType.OuterSingleton;
}
template<> BATTLEX_API UEnum* StaticEnum<EBXTTargetType>()
{
	return EBXTTargetType_StaticEnum();
}
struct Z_Construct_UEnum_BattleX_EBXTTargetType_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
		{ "Bitflags", "" },
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe7\x9b\xae\xe6\xa0\x87\xe7\xb1\xbb\xe5\x9e\x8b\n" },
#endif
		{ "ModuleRelativePath", "Task/BXTEnums.h" },
		{ "T_CollisionResults.DisplayName", "\xe7\xa2\xb0\xe6\x92\x9e\xe6\xa3\x80\xe6\xb5\x8b\xe7\xbb\x93\xe6\x9e\x9c" },
		{ "T_CollisionResults.Name", "EBXTTargetType::T_CollisionResults" },
		{ "T_Instigator.DisplayName", "\xe5\xa7\x8b\xe4\xbd\x9c\xe4\xbf\x91\xe8\x80\x85" },
		{ "T_Instigator.Name", "EBXTTargetType::T_Instigator" },
		{ "T_LockTargets.DisplayName", "\xe9\x94\x81\xe5\xae\x9a\xe7\x9a\x84\xe7\x9b\xae\xe6\xa0\x87" },
		{ "T_LockTargets.Name", "EBXTTargetType::T_LockTargets" },
		{ "T_Owner.DisplayName", "\xe6\x8b\xa5\xe6\x9c\x89\xe8\x80\x85" },
		{ "T_Owner.Name", "EBXTTargetType::T_Owner" },
		{ "T_TMax.DisplayName", "\xe6\x97\xa0\xe6\x95\x88\xe9\x80\x89\xe9\xa1\xb9" },
		{ "T_TMax.Name", "EBXTTargetType::T_TMax" },
		{ "T_Trigger.DisplayName", "\xe8\xa7\xa6\xe5\x8f\x91\xe8\x80\x85" },
		{ "T_Trigger.Name", "EBXTTargetType::T_Trigger" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe7\x9b\xae\xe6\xa0\x87\xe7\xb1\xbb\xe5\x9e\x8b" },
#endif
	};
#endif // WITH_METADATA
	static constexpr UECodeGen_Private::FEnumeratorParam Enumerators[] = {
		{ "EBXTTargetType::T_Owner", (int64)EBXTTargetType::T_Owner },
		{ "EBXTTargetType::T_Instigator", (int64)EBXTTargetType::T_Instigator },
		{ "EBXTTargetType::T_Trigger", (int64)EBXTTargetType::T_Trigger },
		{ "EBXTTargetType::T_LockTargets", (int64)EBXTTargetType::T_LockTargets },
		{ "EBXTTargetType::T_CollisionResults", (int64)EBXTTargetType::T_CollisionResults },
		{ "EBXTTargetType::T_TMax", (int64)EBXTTargetType::T_TMax },
	};
	static const UECodeGen_Private::FEnumParams EnumParams;
};
const UECodeGen_Private::FEnumParams Z_Construct_UEnum_BattleX_EBXTTargetType_Statics::EnumParams = {
	(UObject*(*)())Z_Construct_UPackage__Script_BattleX,
	nullptr,
	"EBXTTargetType",
	"EBXTTargetType",
	Z_Construct_UEnum_BattleX_EBXTTargetType_Statics::Enumerators,
	RF_Public|RF_Transient|RF_MarkAsNative,
	UE_ARRAY_COUNT(Z_Construct_UEnum_BattleX_EBXTTargetType_Statics::Enumerators),
	EEnumFlags::None,
	(uint8)UEnum::ECppForm::EnumClass,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UEnum_BattleX_EBXTTargetType_Statics::Enum_MetaDataParams), Z_Construct_UEnum_BattleX_EBXTTargetType_Statics::Enum_MetaDataParams)
};
UEnum* Z_Construct_UEnum_BattleX_EBXTTargetType()
{
	if (!Z_Registration_Info_UEnum_EBXTTargetType.InnerSingleton)
	{
		UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_EBXTTargetType.InnerSingleton, Z_Construct_UEnum_BattleX_EBXTTargetType_Statics::EnumParams);
	}
	return Z_Registration_Info_UEnum_EBXTTargetType.InnerSingleton;
}
// End Enum EBXTTargetType

// Begin Enum EBXTLifeType
static FEnumRegistrationInfo Z_Registration_Info_UEnum_EBXTLifeType;
static UEnum* EBXTLifeType_StaticEnum()
{
	if (!Z_Registration_Info_UEnum_EBXTLifeType.OuterSingleton)
	{
		Z_Registration_Info_UEnum_EBXTLifeType.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_BattleX_EBXTLifeType, (UObject*)Z_Construct_UPackage__Script_BattleX(), TEXT("EBXTLifeType"));
	}
	return Z_Registration_Info_UEnum_EBXTLifeType.OuterSingleton;
}
template<> BATTLEX_API UEnum* StaticEnum<EBXTLifeType>()
{
	return EBXTLifeType_StaticEnum();
}
struct Z_Construct_UEnum_BattleX_EBXTLifeType_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe7\x94\x9f\xe5\x91\xbd\xe5\x91\xa8\xe6\x9c\x9f\n" },
#endif
		{ "L_Duration.DisplayName", "\xe5\x9b\xba\xe5\xae\x9a\xe6\x97\xb6\xe9\x95\xbf" },
		{ "L_Duration.Name", "EBXTLifeType::L_Duration" },
		{ "L_DurationTimeline.DisplayName", "\xe5\x9b\xba\xe5\xae\x9a\xe6\x97\xb6\xe9\x95\xbf\xe4\xb8\x94\xe6\x97\xb6\xe9\x97\xb4\xe8\xbd\xb4\xe6\x8e\xa7\xe5\x88\xb6" },
		{ "L_DurationTimeline.Name", "EBXTLifeType::L_DurationTimeline" },
		{ "L_Instant.DisplayName", "\xe7\xab\x8b\xe5\x8d\xb3\xe7\xbb\x93\xe6\x9d\x9f" },
		{ "L_Instant.Name", "EBXTLifeType::L_Instant" },
		{ "L_Timeline.DisplayName", "\xe6\x97\xb6\xe9\x97\xb4\xe8\xbd\xb4\xe6\x8e\xa7\xe5\x88\xb6" },
		{ "L_Timeline.Name", "EBXTLifeType::L_Timeline" },
		{ "L_TMax.Hidden", "" },
		{ "L_TMax.Name", "EBXTLifeType::L_TMax" },
		{ "ModuleRelativePath", "Task/BXTEnums.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe7\x94\x9f\xe5\x91\xbd\xe5\x91\xa8\xe6\x9c\x9f" },
#endif
	};
#endif // WITH_METADATA
	static constexpr UECodeGen_Private::FEnumeratorParam Enumerators[] = {
		{ "EBXTLifeType::L_Instant", (int64)EBXTLifeType::L_Instant },
		{ "EBXTLifeType::L_Duration", (int64)EBXTLifeType::L_Duration },
		{ "EBXTLifeType::L_Timeline", (int64)EBXTLifeType::L_Timeline },
		{ "EBXTLifeType::L_DurationTimeline", (int64)EBXTLifeType::L_DurationTimeline },
		{ "EBXTLifeType::L_TMax", (int64)EBXTLifeType::L_TMax },
	};
	static const UECodeGen_Private::FEnumParams EnumParams;
};
const UECodeGen_Private::FEnumParams Z_Construct_UEnum_BattleX_EBXTLifeType_Statics::EnumParams = {
	(UObject*(*)())Z_Construct_UPackage__Script_BattleX,
	nullptr,
	"EBXTLifeType",
	"EBXTLifeType",
	Z_Construct_UEnum_BattleX_EBXTLifeType_Statics::Enumerators,
	RF_Public|RF_Transient|RF_MarkAsNative,
	UE_ARRAY_COUNT(Z_Construct_UEnum_BattleX_EBXTLifeType_Statics::Enumerators),
	EEnumFlags::None,
	(uint8)UEnum::ECppForm::EnumClass,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UEnum_BattleX_EBXTLifeType_Statics::Enum_MetaDataParams), Z_Construct_UEnum_BattleX_EBXTLifeType_Statics::Enum_MetaDataParams)
};
UEnum* Z_Construct_UEnum_BattleX_EBXTLifeType()
{
	if (!Z_Registration_Info_UEnum_EBXTLifeType.InnerSingleton)
	{
		UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_EBXTLifeType.InnerSingleton, Z_Construct_UEnum_BattleX_EBXTLifeType_Statics::EnumParams);
	}
	return Z_Registration_Info_UEnum_EBXTLifeType.InnerSingleton;
}
// End Enum EBXTLifeType

// Begin Enum EBXTNetType
static FEnumRegistrationInfo Z_Registration_Info_UEnum_EBXTNetType;
static UEnum* EBXTNetType_StaticEnum()
{
	if (!Z_Registration_Info_UEnum_EBXTNetType.OuterSingleton)
	{
		Z_Registration_Info_UEnum_EBXTNetType.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_BattleX_EBXTNetType, (UObject*)Z_Construct_UPackage__Script_BattleX(), TEXT("EBXTNetType"));
	}
	return Z_Registration_Info_UEnum_EBXTNetType.OuterSingleton;
}
template<> BATTLEX_API UEnum* StaticEnum<EBXTNetType>()
{
	return EBXTNetType_StaticEnum();
}
struct Z_Construct_UEnum_BattleX_EBXTNetType_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
		{ "Bitflags", "" },
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe7\xbd\x91\xe7\xbb\x9c\xe7\xb1\xbb\xe5\x9e\x8b\n" },
#endif
		{ "ModuleRelativePath", "Task/BXTEnums.h" },
		{ "N_Authority.DisplayName", "\xe6\x9d\x83\xe5\xa8\x81\xe7\xab\xaf" },
		{ "N_Authority.Name", "EBXTNetType::N_Authority" },
		{ "N_Autonomous.DisplayName", "\xe8\x87\xaa\xe4\xb8\xbb\xe7\xab\xaf" },
		{ "N_Autonomous.Name", "EBXTNetType::N_Autonomous" },
		{ "N_Simulated.DisplayName", "\xe6\xa8\xa1\xe6\x8b\x9f\xe7\xab\xaf" },
		{ "N_Simulated.Name", "EBXTNetType::N_Simulated" },
		{ "N_TMax.Hidden", "" },
		{ "N_TMax.Name", "EBXTNetType::N_TMax" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe7\xbd\x91\xe7\xbb\x9c\xe7\xb1\xbb\xe5\x9e\x8b" },
#endif
	};
#endif // WITH_METADATA
	static constexpr UECodeGen_Private::FEnumeratorParam Enumerators[] = {
		{ "EBXTNetType::N_Authority", (int64)EBXTNetType::N_Authority },
		{ "EBXTNetType::N_Autonomous", (int64)EBXTNetType::N_Autonomous },
		{ "EBXTNetType::N_Simulated", (int64)EBXTNetType::N_Simulated },
		{ "EBXTNetType::N_TMax", (int64)EBXTNetType::N_TMax },
	};
	static const UECodeGen_Private::FEnumParams EnumParams;
};
const UECodeGen_Private::FEnumParams Z_Construct_UEnum_BattleX_EBXTNetType_Statics::EnumParams = {
	(UObject*(*)())Z_Construct_UPackage__Script_BattleX,
	nullptr,
	"EBXTNetType",
	"EBXTNetType",
	Z_Construct_UEnum_BattleX_EBXTNetType_Statics::Enumerators,
	RF_Public|RF_Transient|RF_MarkAsNative,
	UE_ARRAY_COUNT(Z_Construct_UEnum_BattleX_EBXTNetType_Statics::Enumerators),
	EEnumFlags::None,
	(uint8)UEnum::ECppForm::EnumClass,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UEnum_BattleX_EBXTNetType_Statics::Enum_MetaDataParams), Z_Construct_UEnum_BattleX_EBXTNetType_Statics::Enum_MetaDataParams)
};
UEnum* Z_Construct_UEnum_BattleX_EBXTNetType()
{
	if (!Z_Registration_Info_UEnum_EBXTNetType.InnerSingleton)
	{
		UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_EBXTNetType.InnerSingleton, Z_Construct_UEnum_BattleX_EBXTNetType_Statics::EnumParams);
	}
	return Z_Registration_Info_UEnum_EBXTNetType.InnerSingleton;
}
// End Enum EBXTNetType

// Begin Enum EBXTTriggerType
static FEnumRegistrationInfo Z_Registration_Info_UEnum_EBXTTriggerType;
static UEnum* EBXTTriggerType_StaticEnum()
{
	if (!Z_Registration_Info_UEnum_EBXTTriggerType.OuterSingleton)
	{
		Z_Registration_Info_UEnum_EBXTTriggerType.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_BattleX_EBXTTriggerType, (UObject*)Z_Construct_UPackage__Script_BattleX(), TEXT("EBXTTriggerType"));
	}
	return Z_Registration_Info_UEnum_EBXTTriggerType.OuterSingleton;
}
template<> BATTLEX_API UEnum* StaticEnum<EBXTTriggerType>()
{
	return EBXTTriggerType_StaticEnum();
}
struct Z_Construct_UEnum_BattleX_EBXTTriggerType_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
		{ "Bitflags", "" },
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe8\xa7\xa6\xe5\x8f\x91\xe7\xb1\xbb\xe5\x9e\x8b\n" },
#endif
		{ "ModuleRelativePath", "Task/BXTEnums.h" },
		{ "T_SectionEnd.DisplayName", "\xe6\x97\xb6\xe9\x97\xb4\xe8\xbd\xb4\xe7\xbb\x93\xe6\x9d\x9f\xe6\x97\xb6\xe8\xa7\xa6\xe5\x8f\x91" },
		{ "T_SectionEnd.Name", "EBXTTriggerType::T_SectionEnd" },
		{ "T_SectionInterrupt.DisplayName", "\xe6\x97\xb6\xe9\x97\xb4\xe8\xbd\xb4\xe4\xb8\xad\xe6\x96\xad\xe6\x97\xb6\xe8\xa7\xa6\xe5\x8f\x91" },
		{ "T_SectionInterrupt.Name", "EBXTTriggerType::T_SectionInterrupt" },
		{ "T_Timeline.DisplayName", "\xe6\x97\xb6\xe9\x97\xb4\xe8\xbd\xb4\xe8\xa7\xa6\xe5\x8f\x91" },
		{ "T_Timeline.Name", "EBXTTriggerType::T_Timeline" },
		{ "T_TMax.Hidden", "" },
		{ "T_TMax.Name", "EBXTTriggerType::T_TMax" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe8\xa7\xa6\xe5\x8f\x91\xe7\xb1\xbb\xe5\x9e\x8b" },
#endif
	};
#endif // WITH_METADATA
	static constexpr UECodeGen_Private::FEnumeratorParam Enumerators[] = {
		{ "EBXTTriggerType::T_Timeline", (int64)EBXTTriggerType::T_Timeline },
		{ "EBXTTriggerType::T_SectionEnd", (int64)EBXTTriggerType::T_SectionEnd },
		{ "EBXTTriggerType::T_SectionInterrupt", (int64)EBXTTriggerType::T_SectionInterrupt },
		{ "EBXTTriggerType::T_TMax", (int64)EBXTTriggerType::T_TMax },
	};
	static const UECodeGen_Private::FEnumParams EnumParams;
};
const UECodeGen_Private::FEnumParams Z_Construct_UEnum_BattleX_EBXTTriggerType_Statics::EnumParams = {
	(UObject*(*)())Z_Construct_UPackage__Script_BattleX,
	nullptr,
	"EBXTTriggerType",
	"EBXTTriggerType",
	Z_Construct_UEnum_BattleX_EBXTTriggerType_Statics::Enumerators,
	RF_Public|RF_Transient|RF_MarkAsNative,
	UE_ARRAY_COUNT(Z_Construct_UEnum_BattleX_EBXTTriggerType_Statics::Enumerators),
	EEnumFlags::None,
	(uint8)UEnum::ECppForm::EnumClass,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UEnum_BattleX_EBXTTriggerType_Statics::Enum_MetaDataParams), Z_Construct_UEnum_BattleX_EBXTTriggerType_Statics::Enum_MetaDataParams)
};
UEnum* Z_Construct_UEnum_BattleX_EBXTTriggerType()
{
	if (!Z_Registration_Info_UEnum_EBXTTriggerType.InnerSingleton)
	{
		UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_EBXTTriggerType.InnerSingleton, Z_Construct_UEnum_BattleX_EBXTTriggerType_Statics::EnumParams);
	}
	return Z_Registration_Info_UEnum_EBXTTriggerType.InnerSingleton;
}
// End Enum EBXTTriggerType

// Begin Enum EBXTCoordinateType
static FEnumRegistrationInfo Z_Registration_Info_UEnum_EBXTCoordinateType;
static UEnum* EBXTCoordinateType_StaticEnum()
{
	if (!Z_Registration_Info_UEnum_EBXTCoordinateType.OuterSingleton)
	{
		Z_Registration_Info_UEnum_EBXTCoordinateType.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_BattleX_EBXTCoordinateType, (UObject*)Z_Construct_UPackage__Script_BattleX(), TEXT("EBXTCoordinateType"));
	}
	return Z_Registration_Info_UEnum_EBXTCoordinateType.OuterSingleton;
}
template<> BATTLEX_API UEnum* StaticEnum<EBXTCoordinateType>()
{
	return EBXTCoordinateType_StaticEnum();
}
struct Z_Construct_UEnum_BattleX_EBXTCoordinateType_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "C_Instigator.DisplayName", "\xe5\xa7\x8b\xe4\xbd\x9c\xe4\xbf\x91\xe8\x80\x85\xe7\x9a\x84\xe5\x9d\x90\xe6\xa0\x87\xe7\xb3\xbb" },
		{ "C_Instigator.Name", "EBXTCoordinateType::C_Instigator" },
		{ "C_LockPart.DisplayName", "\xe9\x94\x81\xe5\xae\x9a\xe9\x83\xa8\xe4\xbd\x8d\xe7\x9a\x84\xe5\x9d\x90\xe6\xa0\x87\xe7\xb3\xbb" },
		{ "C_LockPart.Name", "EBXTCoordinateType::C_LockPart" },
		{ "C_LockTarget.DisplayName", "\xe9\x94\x81\xe5\xae\x9a\xe7\x9b\xae\xe6\xa0\x87\xe7\x9a\x84\xe5\x9d\x90\xe6\xa0\x87\xe7\xb3\xbb" },
		{ "C_LockTarget.Name", "EBXTCoordinateType::C_LockTarget" },
		{ "C_Owner.DisplayName", "\xe6\x8b\xa5\xe6\x9c\x89\xe8\x80\x85\xe7\x9a\x84\xe5\x9d\x90\xe6\xa0\x87\xe7\xb3\xbb" },
		{ "C_Owner.Name", "EBXTCoordinateType::C_Owner" },
		{ "C_Special.DisplayName", "\xe7\x89\xb9\xe5\xae\x9a\xe5\xaf\xb9\xe8\xb1\xa1\xe7\x9a\x84\xe5\x9d\x90\xe6\xa0\x87\xe7\xb3\xbb" },
		{ "C_Special.Name", "EBXTCoordinateType::C_Special" },
		{ "C_Target.DisplayName", "\xe6\x89\xa7\xe8\xa1\x8c\xe8\x80\x85\xe7\x9a\x84\xe5\x9d\x90\xe6\xa0\x87\xe7\xb3\xbb" },
		{ "C_Target.Name", "EBXTCoordinateType::C_Target" },
		{ "C_TMax.DisplayName", "\xe6\x97\xa0\xe6\x95\x88\xe5\x80\xbc" },
		{ "C_TMax.Name", "EBXTCoordinateType::C_TMax" },
		{ "C_Trigger.DisplayName", "\xe8\xa7\xa6\xe5\x8f\x91\xe8\x80\x85\xe7\x9a\x84\xe5\x9d\x90\xe6\xa0\x87\xe7\xb3\xbb" },
		{ "C_Trigger.Name", "EBXTCoordinateType::C_Trigger" },
		{ "C_World.DisplayName", "\xe4\xb8\x96\xe7\x95\x8c\xe5\x9d\x90\xe6\xa0\x87\xe7\xb3\xbb" },
		{ "C_World.Name", "EBXTCoordinateType::C_World" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe5\x9d\x90\xe6\xa0\x87\xe7\xb3\xbb\xe7\xb1\xbb\xe5\x9e\x8b\n" },
#endif
		{ "ModuleRelativePath", "Task/BXTEnums.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe5\x9d\x90\xe6\xa0\x87\xe7\xb3\xbb\xe7\xb1\xbb\xe5\x9e\x8b" },
#endif
	};
#endif // WITH_METADATA
	static constexpr UECodeGen_Private::FEnumeratorParam Enumerators[] = {
		{ "EBXTCoordinateType::C_Target", (int64)EBXTCoordinateType::C_Target },
		{ "EBXTCoordinateType::C_Owner", (int64)EBXTCoordinateType::C_Owner },
		{ "EBXTCoordinateType::C_Instigator", (int64)EBXTCoordinateType::C_Instigator },
		{ "EBXTCoordinateType::C_Trigger", (int64)EBXTCoordinateType::C_Trigger },
		{ "EBXTCoordinateType::C_LockTarget", (int64)EBXTCoordinateType::C_LockTarget },
		{ "EBXTCoordinateType::C_LockPart", (int64)EBXTCoordinateType::C_LockPart },
		{ "EBXTCoordinateType::C_Special", (int64)EBXTCoordinateType::C_Special },
		{ "EBXTCoordinateType::C_World", (int64)EBXTCoordinateType::C_World },
		{ "EBXTCoordinateType::C_TMax", (int64)EBXTCoordinateType::C_TMax },
	};
	static const UECodeGen_Private::FEnumParams EnumParams;
};
const UECodeGen_Private::FEnumParams Z_Construct_UEnum_BattleX_EBXTCoordinateType_Statics::EnumParams = {
	(UObject*(*)())Z_Construct_UPackage__Script_BattleX,
	nullptr,
	"EBXTCoordinateType",
	"EBXTCoordinateType",
	Z_Construct_UEnum_BattleX_EBXTCoordinateType_Statics::Enumerators,
	RF_Public|RF_Transient|RF_MarkAsNative,
	UE_ARRAY_COUNT(Z_Construct_UEnum_BattleX_EBXTCoordinateType_Statics::Enumerators),
	EEnumFlags::None,
	(uint8)UEnum::ECppForm::EnumClass,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UEnum_BattleX_EBXTCoordinateType_Statics::Enum_MetaDataParams), Z_Construct_UEnum_BattleX_EBXTCoordinateType_Statics::Enum_MetaDataParams)
};
UEnum* Z_Construct_UEnum_BattleX_EBXTCoordinateType()
{
	if (!Z_Registration_Info_UEnum_EBXTCoordinateType.InnerSingleton)
	{
		UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_EBXTCoordinateType.InnerSingleton, Z_Construct_UEnum_BattleX_EBXTCoordinateType_Statics::EnumParams);
	}
	return Z_Registration_Info_UEnum_EBXTCoordinateType.InnerSingleton;
}
// End Enum EBXTCoordinateType

// Begin Enum EBXTConnectionType
static FEnumRegistrationInfo Z_Registration_Info_UEnum_EBXTConnectionType;
static UEnum* EBXTConnectionType_StaticEnum()
{
	if (!Z_Registration_Info_UEnum_EBXTConnectionType.OuterSingleton)
	{
		Z_Registration_Info_UEnum_EBXTConnectionType.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_BattleX_EBXTConnectionType, (UObject*)Z_Construct_UPackage__Script_BattleX(), TEXT("EBXTConnectionType"));
	}
	return Z_Registration_Info_UEnum_EBXTConnectionType.OuterSingleton;
}
template<> BATTLEX_API UEnum* StaticEnum<EBXTConnectionType>()
{
	return EBXTConnectionType_StaticEnum();
}
struct Z_Construct_UEnum_BattleX_EBXTConnectionType_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "C_FromOriginIn2D.DisplayName", "\xe4\xbb\x8e\xe5\x8e\x9f\xe7\x82\xb9\xe5\x87\xba\xe5\x8f\x91\xe6\x8c\x87\xe5\x90\x91X\xe8\xbd\xb4\xe5\x9d\x90\xe6\xa0\x87\xe7\xb3\xbb\xe5\x8e\x9f\xe7\x82\xb9(2D)" },
		{ "C_FromOriginIn2D.Name", "EBXTConnectionType::C_FromOriginIn2D" },
		{ "C_FromOriginIn3D.DisplayName", "\xe4\xbb\x8e\xe5\x8e\x9f\xe7\x82\xb9\xe5\x87\xba\xe5\x8f\x91\xe6\x8c\x87\xe5\x90\x91X\xe8\xbd\xb4\xe5\x9d\x90\xe6\xa0\x87\xe7\xb3\xbb\xe5\x8e\x9f\xe7\x82\xb9(3D)" },
		{ "C_FromOriginIn3D.Name", "EBXTConnectionType::C_FromOriginIn3D" },
		{ "C_FromXAxisIn2D.DisplayName", "\xe4\xbb\x8eX\xe8\xbd\xb4\xe5\x9d\x90\xe6\xa0\x87\xe7\xb3\xbb\xe5\x87\xba\xe5\x8f\x91\xe6\x8c\x87\xe5\x90\x91\xe5\x8e\x9f\xe7\x82\xb9(2D)" },
		{ "C_FromXAxisIn2D.Name", "EBXTConnectionType::C_FromXAxisIn2D" },
		{ "C_FromXAxisIn3D.DisplayName", "\xe4\xbb\x8eX\xe8\xbd\xb4\xe5\x9d\x90\xe6\xa0\x87\xe7\xb3\xbb\xe5\x87\xba\xe5\x8f\x91\xe6\x8c\x87\xe5\x90\x91\xe5\x8e\x9f\xe7\x82\xb9(3D)" },
		{ "C_FromXAxisIn3D.Name", "EBXTConnectionType::C_FromXAxisIn3D" },
		{ "C_TMax.DisplayName", "\xe6\x97\xa0\xe6\x95\x88\xe5\x80\xbc" },
		{ "C_TMax.Name", "EBXTConnectionType::C_TMax" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe8\xbf\x9e\xe7\xba\xbf\xe7\xb1\xbb\xe5\x9e\x8b\n" },
#endif
		{ "ModuleRelativePath", "Task/BXTEnums.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe8\xbf\x9e\xe7\xba\xbf\xe7\xb1\xbb\xe5\x9e\x8b" },
#endif
	};
#endif // WITH_METADATA
	static constexpr UECodeGen_Private::FEnumeratorParam Enumerators[] = {
		{ "EBXTConnectionType::C_FromOriginIn2D", (int64)EBXTConnectionType::C_FromOriginIn2D },
		{ "EBXTConnectionType::C_FromOriginIn3D", (int64)EBXTConnectionType::C_FromOriginIn3D },
		{ "EBXTConnectionType::C_FromXAxisIn2D", (int64)EBXTConnectionType::C_FromXAxisIn2D },
		{ "EBXTConnectionType::C_FromXAxisIn3D", (int64)EBXTConnectionType::C_FromXAxisIn3D },
		{ "EBXTConnectionType::C_TMax", (int64)EBXTConnectionType::C_TMax },
	};
	static const UECodeGen_Private::FEnumParams EnumParams;
};
const UECodeGen_Private::FEnumParams Z_Construct_UEnum_BattleX_EBXTConnectionType_Statics::EnumParams = {
	(UObject*(*)())Z_Construct_UPackage__Script_BattleX,
	nullptr,
	"EBXTConnectionType",
	"EBXTConnectionType",
	Z_Construct_UEnum_BattleX_EBXTConnectionType_Statics::Enumerators,
	RF_Public|RF_Transient|RF_MarkAsNative,
	UE_ARRAY_COUNT(Z_Construct_UEnum_BattleX_EBXTConnectionType_Statics::Enumerators),
	EEnumFlags::None,
	(uint8)UEnum::ECppForm::EnumClass,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UEnum_BattleX_EBXTConnectionType_Statics::Enum_MetaDataParams), Z_Construct_UEnum_BattleX_EBXTConnectionType_Statics::Enum_MetaDataParams)
};
UEnum* Z_Construct_UEnum_BattleX_EBXTConnectionType()
{
	if (!Z_Registration_Info_UEnum_EBXTConnectionType.InnerSingleton)
	{
		UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_EBXTConnectionType.InnerSingleton, Z_Construct_UEnum_BattleX_EBXTConnectionType_Statics::EnumParams);
	}
	return Z_Registration_Info_UEnum_EBXTConnectionType.InnerSingleton;
}
// End Enum EBXTConnectionType

// Begin Enum EBXTProcessorFunction
static FEnumRegistrationInfo Z_Registration_Info_UEnum_EBXTProcessorFunction;
static UEnum* EBXTProcessorFunction_StaticEnum()
{
	if (!Z_Registration_Info_UEnum_EBXTProcessorFunction.OuterSingleton)
	{
		Z_Registration_Info_UEnum_EBXTProcessorFunction.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_BattleX_EBXTProcessorFunction, (UObject*)Z_Construct_UPackage__Script_BattleX(), TEXT("EBXTProcessorFunction"));
	}
	return Z_Registration_Info_UEnum_EBXTProcessorFunction.OuterSingleton;
}
template<> BATTLEX_API UEnum* StaticEnum<EBXTProcessorFunction>()
{
	return EBXTProcessorFunction_StaticEnum();
}
struct Z_Construct_UEnum_BattleX_EBXTProcessorFunction_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
		{ "Bitflags", "" },
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe4\xbb\xbb\xe5\x8a\xa1\xe5\xa4\x84\xe7\x90\x86\xe5\x99\xa8\xe6\x89\xa7\xe8\xa1\x8c\xe5\x87\xbd\xe6\x95\xb0\xe7\xb1\xbb\xe5\x9e\x8b\n" },
#endif
		{ "ModuleRelativePath", "Task/BXTEnums.h" },
		{ "PF_BPEnd.DisplayName", "\xe8\x93\x9d\xe5\x9b\xbe\xe7\xbb\x93\xe6\x9d\x9f" },
		{ "PF_BPEnd.Name", "EBXTProcessorFunction::PF_BPEnd" },
		{ "PF_BPStart.DisplayName", "\xe8\x93\x9d\xe5\x9b\xbe\xe5\xbc\x80\xe5\xa7\x8b" },
		{ "PF_BPStart.Name", "EBXTProcessorFunction::PF_BPStart" },
		{ "PF_BPUpdate.DisplayName", "\xe8\x93\x9d\xe5\x9b\xbe\xe6\x9b\xb4\xe6\x96\xb0" },
		{ "PF_BPUpdate.Name", "EBXTProcessorFunction::PF_BPUpdate" },
		{ "PF_NativeEnd.DisplayName", "C++\xe7\xbb\x93\xe6\x9d\x9f" },
		{ "PF_NativeEnd.Name", "EBXTProcessorFunction::PF_NativeEnd" },
		{ "PF_NativeStart.DisplayName", "C++\xe5\xbc\x80\xe5\xa7\x8b" },
		{ "PF_NativeStart.Name", "EBXTProcessorFunction::PF_NativeStart" },
		{ "PF_NativeUpdate.DisplayName", "C++\xe6\x9b\xb4\xe6\x96\xb0" },
		{ "PF_NativeUpdate.Name", "EBXTProcessorFunction::PF_NativeUpdate" },
		{ "PF_TMax.Hidden", "" },
		{ "PF_TMax.Name", "EBXTProcessorFunction::PF_TMax" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe4\xbb\xbb\xe5\x8a\xa1\xe5\xa4\x84\xe7\x90\x86\xe5\x99\xa8\xe6\x89\xa7\xe8\xa1\x8c\xe5\x87\xbd\xe6\x95\xb0\xe7\xb1\xbb\xe5\x9e\x8b" },
#endif
	};
#endif // WITH_METADATA
	static constexpr UECodeGen_Private::FEnumeratorParam Enumerators[] = {
		{ "EBXTProcessorFunction::PF_NativeStart", (int64)EBXTProcessorFunction::PF_NativeStart },
		{ "EBXTProcessorFunction::PF_BPStart", (int64)EBXTProcessorFunction::PF_BPStart },
		{ "EBXTProcessorFunction::PF_NativeUpdate", (int64)EBXTProcessorFunction::PF_NativeUpdate },
		{ "EBXTProcessorFunction::PF_BPUpdate", (int64)EBXTProcessorFunction::PF_BPUpdate },
		{ "EBXTProcessorFunction::PF_NativeEnd", (int64)EBXTProcessorFunction::PF_NativeEnd },
		{ "EBXTProcessorFunction::PF_BPEnd", (int64)EBXTProcessorFunction::PF_BPEnd },
		{ "EBXTProcessorFunction::PF_TMax", (int64)EBXTProcessorFunction::PF_TMax },
	};
	static const UECodeGen_Private::FEnumParams EnumParams;
};
const UECodeGen_Private::FEnumParams Z_Construct_UEnum_BattleX_EBXTProcessorFunction_Statics::EnumParams = {
	(UObject*(*)())Z_Construct_UPackage__Script_BattleX,
	nullptr,
	"EBXTProcessorFunction",
	"EBXTProcessorFunction",
	Z_Construct_UEnum_BattleX_EBXTProcessorFunction_Statics::Enumerators,
	RF_Public|RF_Transient|RF_MarkAsNative,
	UE_ARRAY_COUNT(Z_Construct_UEnum_BattleX_EBXTProcessorFunction_Statics::Enumerators),
	EEnumFlags::None,
	(uint8)UEnum::ECppForm::EnumClass,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UEnum_BattleX_EBXTProcessorFunction_Statics::Enum_MetaDataParams), Z_Construct_UEnum_BattleX_EBXTProcessorFunction_Statics::Enum_MetaDataParams)
};
UEnum* Z_Construct_UEnum_BattleX_EBXTProcessorFunction()
{
	if (!Z_Registration_Info_UEnum_EBXTProcessorFunction.InnerSingleton)
	{
		UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_EBXTProcessorFunction.InnerSingleton, Z_Construct_UEnum_BattleX_EBXTProcessorFunction_Statics::EnumParams);
	}
	return Z_Registration_Info_UEnum_EBXTProcessorFunction.InnerSingleton;
}
// End Enum EBXTProcessorFunction

// Begin Registration
struct Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Task_BXTEnums_h_Statics
{
	static constexpr FEnumRegisterCompiledInInfo EnumInfo[] = {
		{ EBXTTargetType_StaticEnum, TEXT("EBXTTargetType"), &Z_Registration_Info_UEnum_EBXTTargetType, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 266591756U) },
		{ EBXTLifeType_StaticEnum, TEXT("EBXTLifeType"), &Z_Registration_Info_UEnum_EBXTLifeType, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 1739170628U) },
		{ EBXTNetType_StaticEnum, TEXT("EBXTNetType"), &Z_Registration_Info_UEnum_EBXTNetType, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 171255470U) },
		{ EBXTTriggerType_StaticEnum, TEXT("EBXTTriggerType"), &Z_Registration_Info_UEnum_EBXTTriggerType, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 339372580U) },
		{ EBXTCoordinateType_StaticEnum, TEXT("EBXTCoordinateType"), &Z_Registration_Info_UEnum_EBXTCoordinateType, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 2214625094U) },
		{ EBXTConnectionType_StaticEnum, TEXT("EBXTConnectionType"), &Z_Registration_Info_UEnum_EBXTConnectionType, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 2224598816U) },
		{ EBXTProcessorFunction_StaticEnum, TEXT("EBXTProcessorFunction"), &Z_Registration_Info_UEnum_EBXTProcessorFunction, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 1980869257U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Task_BXTEnums_h_1248021821(TEXT("/Script/BattleX"),
	nullptr, 0,
	nullptr, 0,
	Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Task_BXTEnums_h_Statics::EnumInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Task_BXTEnums_h_Statics::EnumInfo));
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
