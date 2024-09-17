// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "BattleX/Timeline/BXTLEnums.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeBXTLEnums() {}

// Begin Cross Module References
BATTLEX_API UEnum* Z_Construct_UEnum_BattleX_EBXTLExtraType();
BATTLEX_API UEnum* Z_Construct_UEnum_BattleX_EBXTLFinishReason();
BATTLEX_API UEnum* Z_Construct_UEnum_BattleX_EBXTLTargetSelection();
UPackage* Z_Construct_UPackage__Script_BattleX();
// End Cross Module References

// Begin Enum EBXTLFinishReason
static FEnumRegistrationInfo Z_Registration_Info_UEnum_EBXTLFinishReason;
static UEnum* EBXTLFinishReason_StaticEnum()
{
	if (!Z_Registration_Info_UEnum_EBXTLFinishReason.OuterSingleton)
	{
		Z_Registration_Info_UEnum_EBXTLFinishReason.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_BattleX_EBXTLFinishReason, (UObject*)Z_Construct_UPackage__Script_BattleX(), TEXT("EBXTLFinishReason"));
	}
	return Z_Registration_Info_UEnum_EBXTLFinishReason.OuterSingleton;
}
template<> BATTLEX_API UEnum* StaticEnum<EBXTLFinishReason>()
{
	return EBXTLFinishReason_StaticEnum();
}
struct Z_Construct_UEnum_BattleX_EBXTLFinishReason_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe7\xbb\x93\xe6\x9d\x9f\xe7\xb1\xbb\xe5\x9e\x8b\n" },
#endif
		{ "FR_EndOfLife.DisplayName", "\xe5\xaf\xbf\xe7\xbb\x88\xe6\xad\xa3\xe5\xaf\x9d" },
		{ "FR_EndOfLife.Name", "EBXTLFinishReason::FR_EndOfLife" },
		{ "FR_Interrupt.DisplayName", "\xe8\xa2\xab\xe4\xb8\xad\xe6\x96\xad" },
		{ "FR_Interrupt.Name", "EBXTLFinishReason::FR_Interrupt" },
		{ "FR_PredictionFailure.DisplayName", "\xe9\xa2\x84\xe6\xb5\x8b\xe5\xa4\xb1\xe8\xb4\xa5" },
		{ "FR_PredictionFailure.Name", "EBXTLFinishReason::FR_PredictionFailure" },
		{ "FR_TMax.Hidden", "" },
		{ "FR_TMax.Name", "EBXTLFinishReason::FR_TMax" },
		{ "ModuleRelativePath", "Timeline/BXTLEnums.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe7\xbb\x93\xe6\x9d\x9f\xe7\xb1\xbb\xe5\x9e\x8b" },
#endif
	};
#endif // WITH_METADATA
	static constexpr UECodeGen_Private::FEnumeratorParam Enumerators[] = {
		{ "EBXTLFinishReason::FR_EndOfLife", (int64)EBXTLFinishReason::FR_EndOfLife },
		{ "EBXTLFinishReason::FR_Interrupt", (int64)EBXTLFinishReason::FR_Interrupt },
		{ "EBXTLFinishReason::FR_PredictionFailure", (int64)EBXTLFinishReason::FR_PredictionFailure },
		{ "EBXTLFinishReason::FR_TMax", (int64)EBXTLFinishReason::FR_TMax },
	};
	static const UECodeGen_Private::FEnumParams EnumParams;
};
const UECodeGen_Private::FEnumParams Z_Construct_UEnum_BattleX_EBXTLFinishReason_Statics::EnumParams = {
	(UObject*(*)())Z_Construct_UPackage__Script_BattleX,
	nullptr,
	"EBXTLFinishReason",
	"EBXTLFinishReason",
	Z_Construct_UEnum_BattleX_EBXTLFinishReason_Statics::Enumerators,
	RF_Public|RF_Transient|RF_MarkAsNative,
	UE_ARRAY_COUNT(Z_Construct_UEnum_BattleX_EBXTLFinishReason_Statics::Enumerators),
	EEnumFlags::None,
	(uint8)UEnum::ECppForm::EnumClass,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UEnum_BattleX_EBXTLFinishReason_Statics::Enum_MetaDataParams), Z_Construct_UEnum_BattleX_EBXTLFinishReason_Statics::Enum_MetaDataParams)
};
UEnum* Z_Construct_UEnum_BattleX_EBXTLFinishReason()
{
	if (!Z_Registration_Info_UEnum_EBXTLFinishReason.InnerSingleton)
	{
		UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_EBXTLFinishReason.InnerSingleton, Z_Construct_UEnum_BattleX_EBXTLFinishReason_Statics::EnumParams);
	}
	return Z_Registration_Info_UEnum_EBXTLFinishReason.InnerSingleton;
}
// End Enum EBXTLFinishReason

// Begin Enum EBXTLTargetSelection
static FEnumRegistrationInfo Z_Registration_Info_UEnum_EBXTLTargetSelection;
static UEnum* EBXTLTargetSelection_StaticEnum()
{
	if (!Z_Registration_Info_UEnum_EBXTLTargetSelection.OuterSingleton)
	{
		Z_Registration_Info_UEnum_EBXTLTargetSelection.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_BattleX_EBXTLTargetSelection, (UObject*)Z_Construct_UPackage__Script_BattleX(), TEXT("EBXTLTargetSelection"));
	}
	return Z_Registration_Info_UEnum_EBXTLTargetSelection.OuterSingleton;
}
template<> BATTLEX_API UEnum* StaticEnum<EBXTLTargetSelection>()
{
	return EBXTLTargetSelection_StaticEnum();
}
struct Z_Construct_UEnum_BattleX_EBXTLTargetSelection_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
		{ "Bitflags", "" },
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe7\x9b\xae\xe6\xa0\x87\xe9\x80\x89\xe6\x8b\xa9\xe7\xb1\xbb\xe5\x9e\x8b\n" },
#endif
		{ "ModuleRelativePath", "Timeline/BXTLEnums.h" },
		{ "ST_Instigator.DisplayName", "\xe5\xa7\x8b\xe4\xbd\x9c\xe4\xbf\x91\xe8\x80\x85" },
		{ "ST_Instigator.Name", "EBXTLTargetSelection::ST_Instigator" },
		{ "ST_Owner.DisplayName", "\xe6\x8b\xa5\xe6\x9c\x89\xe8\x80\x85" },
		{ "ST_Owner.Name", "EBXTLTargetSelection::ST_Owner" },
		{ "ST_Self.DisplayName", "\xe5\xbd\x93\xe5\x89\x8d\xe6\x89\xa7\xe8\xa1\x8c\xe7\x9b\xae\xe6\xa0\x87" },
		{ "ST_Self.Name", "EBXTLTargetSelection::ST_Self" },
		{ "ST_Target.DisplayName", "\xe9\x94\x81\xe5\xae\x9a\xe7\x9b\xae\xe6\xa0\x87" },
		{ "ST_Target.Name", "EBXTLTargetSelection::ST_Target" },
		{ "ST_TMax.DisplayName", "\xe6\x97\xa0\xe6\x95\x88\xe9\x80\x89\xe9\xa1\xb9" },
		{ "ST_TMax.Name", "EBXTLTargetSelection::ST_TMax" },
		{ "ST_Trigger.DisplayName", "\xe8\xa7\xa6\xe5\x8f\x91\xe8\x80\x85" },
		{ "ST_Trigger.Name", "EBXTLTargetSelection::ST_Trigger" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe7\x9b\xae\xe6\xa0\x87\xe9\x80\x89\xe6\x8b\xa9\xe7\xb1\xbb\xe5\x9e\x8b" },
#endif
	};
#endif // WITH_METADATA
	static constexpr UECodeGen_Private::FEnumeratorParam Enumerators[] = {
		{ "EBXTLTargetSelection::ST_Self", (int64)EBXTLTargetSelection::ST_Self },
		{ "EBXTLTargetSelection::ST_Owner", (int64)EBXTLTargetSelection::ST_Owner },
		{ "EBXTLTargetSelection::ST_Instigator", (int64)EBXTLTargetSelection::ST_Instigator },
		{ "EBXTLTargetSelection::ST_Trigger", (int64)EBXTLTargetSelection::ST_Trigger },
		{ "EBXTLTargetSelection::ST_Target", (int64)EBXTLTargetSelection::ST_Target },
		{ "EBXTLTargetSelection::ST_TMax", (int64)EBXTLTargetSelection::ST_TMax },
	};
	static const UECodeGen_Private::FEnumParams EnumParams;
};
const UECodeGen_Private::FEnumParams Z_Construct_UEnum_BattleX_EBXTLTargetSelection_Statics::EnumParams = {
	(UObject*(*)())Z_Construct_UPackage__Script_BattleX,
	nullptr,
	"EBXTLTargetSelection",
	"EBXTLTargetSelection",
	Z_Construct_UEnum_BattleX_EBXTLTargetSelection_Statics::Enumerators,
	RF_Public|RF_Transient|RF_MarkAsNative,
	UE_ARRAY_COUNT(Z_Construct_UEnum_BattleX_EBXTLTargetSelection_Statics::Enumerators),
	EEnumFlags::None,
	(uint8)UEnum::ECppForm::EnumClass,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UEnum_BattleX_EBXTLTargetSelection_Statics::Enum_MetaDataParams), Z_Construct_UEnum_BattleX_EBXTLTargetSelection_Statics::Enum_MetaDataParams)
};
UEnum* Z_Construct_UEnum_BattleX_EBXTLTargetSelection()
{
	if (!Z_Registration_Info_UEnum_EBXTLTargetSelection.InnerSingleton)
	{
		UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_EBXTLTargetSelection.InnerSingleton, Z_Construct_UEnum_BattleX_EBXTLTargetSelection_Statics::EnumParams);
	}
	return Z_Registration_Info_UEnum_EBXTLTargetSelection.InnerSingleton;
}
// End Enum EBXTLTargetSelection

// Begin Enum EBXTLExtraType
static FEnumRegistrationInfo Z_Registration_Info_UEnum_EBXTLExtraType;
static UEnum* EBXTLExtraType_StaticEnum()
{
	if (!Z_Registration_Info_UEnum_EBXTLExtraType.OuterSingleton)
	{
		Z_Registration_Info_UEnum_EBXTLExtraType.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_BattleX_EBXTLExtraType, (UObject*)Z_Construct_UPackage__Script_BattleX(), TEXT("EBXTLExtraType"));
	}
	return Z_Registration_Info_UEnum_EBXTLExtraType.OuterSingleton;
}
template<> BATTLEX_API UEnum* StaticEnum<EBXTLExtraType>()
{
	return EBXTLExtraType_StaticEnum();
}
struct Z_Construct_UEnum_BattleX_EBXTLExtraType_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe9\xa2\x9d\xe5\xa4\x96\xe6\x97\xb6\xe9\x97\xb4\xe8\xbd\xb4\xe7\xb1\xbb\xe5\x9e\x8b\n" },
#endif
		{ "ET_Anticipation.DisplayName", "\xe5\x89\x8d\xe6\x91\x87" },
		{ "ET_Anticipation.Name", "EBXTLExtraType::ET_Anticipation" },
		{ "ET_ComboWindow1.DisplayName", "\xe8\xbf\x9e\xe6\xae\xb5\xe7\xaa\x97\xe5\x8f\xa3""1" },
		{ "ET_ComboWindow1.Name", "EBXTLExtraType::ET_ComboWindow1" },
		{ "ET_ComboWindow2.DisplayName", "\xe8\xbf\x9e\xe6\xae\xb5\xe7\xaa\x97\xe5\x8f\xa3""2" },
		{ "ET_ComboWindow2.Name", "EBXTLExtraType::ET_ComboWindow2" },
		{ "ET_ComboWindow3.DisplayName", "\xe8\xbf\x9e\xe6\xae\xb5\xe7\xaa\x97\xe5\x8f\xa3""3" },
		{ "ET_ComboWindow3.Name", "EBXTLExtraType::ET_ComboWindow3" },
		{ "ET_ComboWindow4.DisplayName", "\xe8\xbf\x9e\xe6\xae\xb5\xe7\xaa\x97\xe5\x8f\xa3""4" },
		{ "ET_ComboWindow4.Name", "EBXTLExtraType::ET_ComboWindow4" },
		{ "ET_ComboWindow5.DisplayName", "\xe8\xbf\x9e\xe6\xae\xb5\xe7\xaa\x97\xe5\x8f\xa3""5" },
		{ "ET_ComboWindow5.Name", "EBXTLExtraType::ET_ComboWindow5" },
		{ "ET_ComboWindow6.DisplayName", "\xe8\xbf\x9e\xe6\xae\xb5\xe7\xaa\x97\xe5\x8f\xa3""6" },
		{ "ET_ComboWindow6.Name", "EBXTLExtraType::ET_ComboWindow6" },
		{ "ET_ComboWindow7.DisplayName", "\xe8\xbf\x9e\xe6\xae\xb5\xe7\xaa\x97\xe5\x8f\xa3""7" },
		{ "ET_ComboWindow7.Name", "EBXTLExtraType::ET_ComboWindow7" },
		{ "ET_ComboWindow8.DisplayName", "\xe8\xbf\x9e\xe6\xae\xb5\xe7\xaa\x97\xe5\x8f\xa3""8" },
		{ "ET_ComboWindow8.Name", "EBXTLExtraType::ET_ComboWindow8" },
		{ "ET_Recovery.DisplayName", "\xe5\x90\x8e\xe6\x91\x87" },
		{ "ET_Recovery.Name", "EBXTLExtraType::ET_Recovery" },
		{ "ET_TMax.Hidden", "" },
		{ "ET_TMax.Name", "EBXTLExtraType::ET_TMax" },
		{ "ModuleRelativePath", "Timeline/BXTLEnums.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe9\xa2\x9d\xe5\xa4\x96\xe6\x97\xb6\xe9\x97\xb4\xe8\xbd\xb4\xe7\xb1\xbb\xe5\x9e\x8b" },
#endif
	};
#endif // WITH_METADATA
	static constexpr UECodeGen_Private::FEnumeratorParam Enumerators[] = {
		{ "EBXTLExtraType::ET_Anticipation", (int64)EBXTLExtraType::ET_Anticipation },
		{ "EBXTLExtraType::ET_Recovery", (int64)EBXTLExtraType::ET_Recovery },
		{ "EBXTLExtraType::ET_ComboWindow1", (int64)EBXTLExtraType::ET_ComboWindow1 },
		{ "EBXTLExtraType::ET_ComboWindow2", (int64)EBXTLExtraType::ET_ComboWindow2 },
		{ "EBXTLExtraType::ET_ComboWindow3", (int64)EBXTLExtraType::ET_ComboWindow3 },
		{ "EBXTLExtraType::ET_ComboWindow4", (int64)EBXTLExtraType::ET_ComboWindow4 },
		{ "EBXTLExtraType::ET_ComboWindow5", (int64)EBXTLExtraType::ET_ComboWindow5 },
		{ "EBXTLExtraType::ET_ComboWindow6", (int64)EBXTLExtraType::ET_ComboWindow6 },
		{ "EBXTLExtraType::ET_ComboWindow7", (int64)EBXTLExtraType::ET_ComboWindow7 },
		{ "EBXTLExtraType::ET_ComboWindow8", (int64)EBXTLExtraType::ET_ComboWindow8 },
		{ "EBXTLExtraType::ET_TMax", (int64)EBXTLExtraType::ET_TMax },
	};
	static const UECodeGen_Private::FEnumParams EnumParams;
};
const UECodeGen_Private::FEnumParams Z_Construct_UEnum_BattleX_EBXTLExtraType_Statics::EnumParams = {
	(UObject*(*)())Z_Construct_UPackage__Script_BattleX,
	nullptr,
	"EBXTLExtraType",
	"EBXTLExtraType",
	Z_Construct_UEnum_BattleX_EBXTLExtraType_Statics::Enumerators,
	RF_Public|RF_Transient|RF_MarkAsNative,
	UE_ARRAY_COUNT(Z_Construct_UEnum_BattleX_EBXTLExtraType_Statics::Enumerators),
	EEnumFlags::None,
	(uint8)UEnum::ECppForm::EnumClass,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UEnum_BattleX_EBXTLExtraType_Statics::Enum_MetaDataParams), Z_Construct_UEnum_BattleX_EBXTLExtraType_Statics::Enum_MetaDataParams)
};
UEnum* Z_Construct_UEnum_BattleX_EBXTLExtraType()
{
	if (!Z_Registration_Info_UEnum_EBXTLExtraType.InnerSingleton)
	{
		UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_EBXTLExtraType.InnerSingleton, Z_Construct_UEnum_BattleX_EBXTLExtraType_Statics::EnumParams);
	}
	return Z_Registration_Info_UEnum_EBXTLExtraType.InnerSingleton;
}
// End Enum EBXTLExtraType

// Begin Registration
struct Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Timeline_BXTLEnums_h_Statics
{
	static constexpr FEnumRegisterCompiledInInfo EnumInfo[] = {
		{ EBXTLFinishReason_StaticEnum, TEXT("EBXTLFinishReason"), &Z_Registration_Info_UEnum_EBXTLFinishReason, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 4111085863U) },
		{ EBXTLTargetSelection_StaticEnum, TEXT("EBXTLTargetSelection"), &Z_Registration_Info_UEnum_EBXTLTargetSelection, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 3288795091U) },
		{ EBXTLExtraType_StaticEnum, TEXT("EBXTLExtraType"), &Z_Registration_Info_UEnum_EBXTLExtraType, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 737436919U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Timeline_BXTLEnums_h_28799689(TEXT("/Script/BattleX"),
	nullptr, 0,
	nullptr, 0,
	Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Timeline_BXTLEnums_h_Statics::EnumInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Lyra_Plugins_BattleX_Source_BattleX_Timeline_BXTLEnums_h_Statics::EnumInfo));
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
