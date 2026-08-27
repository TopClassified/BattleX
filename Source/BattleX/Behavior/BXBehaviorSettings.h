#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "GameplayTagContainer.h"

#include "BXBehaviorEnums.h"

#include "BXBehaviorSettings.generated.h"



// 全局行为关系配置(行为矩阵,各端一致的ini)
UCLASS(Config = BattleX, DefaultConfig, Meta = (DisplayName = "BattleX Behavior Relations"))
class BATTLEX_API UBXBehaviorSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	// 查询两个行为的关系(带族Tag层级匹配:轴上注册的族Tag整族参与关系判定)
	EBXBehaviorRelation GetRelation(const FGameplayTag& InEntering, const FGameplayTag& InExisting) const;

	// 行为进入时需要挤出的全部活跃行为Tag
	void GetExpelTargets(const FGameplayTag& InEntering, TArray<FGameplayTag>& OutTags) const;

	// 是否存在与进入行为互斥的活跃行为(调用方需自行判定活跃集合)
	bool IsRejectedByAny(const FGameplayTag& InEntering, const FGameplayTagContainer& InActiveBehaviors) const;

protected:
	// 关系查值(行Tag→列Tag),无配置返回并存
	EBXBehaviorRelation FindRelation(const FGameplayTag& InEntering, const FGameplayTag& InExisting) const;

public:
	// 矩阵轴Tag列表(支持族Tag如BXBehavior.Locomotion,整族参与关系)
	UPROPERTY(EditAnywhere, Config, Category = "Matrix")
	TArray<FGameplayTag> RelationTags;

	// 挤出关系:行进入时踢掉列(中断/取代)
	UPROPERTY(EditAnywhere, Config, Category = "Matrix")
	TMap<FGameplayTag, FGameplayTagContainer> ExpelRelations;

	// 拒绝关系:列存在时挡住行(禁用/互斥不能同时)
	UPROPERTY(EditAnywhere, Config, Category = "Matrix")
	TMap<FGameplayTag, FGameplayTagContainer> RejectRelations;
};
