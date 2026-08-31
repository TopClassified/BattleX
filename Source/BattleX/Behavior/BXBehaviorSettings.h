#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "GameplayTagContainer.h"

#include "BXBehaviorEnums.h"

#include "BXBehaviorSettings.generated.h"



// 行索引负载:行键 → 接管列/禁止列(静态后处理产物,运行时只读)
USTRUCT()
struct FBXBehaviorRelationRow
{
	GENERATED_USTRUCT_BODY()

public:
	// 接管列(行进入时踢掉,不受豁免影响)
	UPROPERTY(Transient)
	FGameplayTagContainer ExpelColumns;

	// 禁止列(列存在时挡住行,豁免写入期折算)
	UPROPERTY(Transient)
	FGameplayTagContainer ForbidColumns;
};



// 全局行为关系配置(行为矩阵,各端一致的ini)
UCLASS(Config = BattleX, DefaultConfig, Meta = (DisplayName = "BattleXBehaviorRelations"))
class BATTLEX_API UBXBehaviorSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UBXBehaviorSettings();

	// Project Settings 页面定位(引擎自动发现UDeveloperSettings并按此注册,勿再手动ISettingsModule注册——会双页面)
	virtual FName GetContainerName() const override;
	virtual FName GetCategoryName() const override;
	virtual FName GetSectionName() const override;
#if WITH_EDITOR
	virtual FText GetSectionText() const override;
	virtual FText GetSectionDescription() const override;
#endif

	virtual void PostInitProperties() override;
	virtual void PostReloadConfig(class FProperty* PropertyThatWasLoaded) override;

	// 静态后处理:由二维表重建双索引(启动/ini重载/编辑器变更后调用;运行期只读零漂移)
	void RebuildRelationIndex();

	// 查询两个行为的关系(带族Tag层级匹配:轴上注册的族Tag整族参与关系判定;
	// 两张表同格配置时返回禁止+接管,拒绝与挤出各管一段互不遮蔽)
	EBXBehaviorRelation GetRelation(const FGameplayTag& InEntering, const FGameplayTag& InExisting) const;

	// 行为进入时需要挤出的全部活跃行为Tag(沿父链查行索引)
	void GetExpelTargets(const FGameplayTag& InEntering, TArray<FGameplayTag>& OutTags) const;

	// 查询某来源(潜在在位方)禁止的域集合(禁止贡献计算用;调用方沿父链逐级查询)
	const FGameplayTagContainer* FindForbidDomains(const FGameplayTag& InSourceTag) const { return ForbidDomainsBySource.Find(InSourceTag); }

protected:
	// 关系查值(行Tag→列Tag),无配置返回空
	EBXBehaviorRelation FindRelation(const FGameplayTag& InEntering, const FGameplayTag& InExisting) const;

public:
	// 矩阵轴Tag列表(支持族Tag如BXBehavior.Locomotion,整族参与关系)
	UPROPERTY(EditAnywhere, Config, Category = "Matrix")
	TArray<FGameplayTag> RelationTags;

	// 接管关系:行进入时踢掉列(中断/取代;不受豁免影响)
	UPROPERTY(EditAnywhere, Config, Category = "Matrix")
	TMap<FGameplayTag, FGameplayTagContainer> ExpelRelations;

	// 禁止关系:列存在时挡住行(在位方被豁免时不执行——折算发生在账本写入期)
	UPROPERTY(EditAnywhere, Config, Category = "Matrix")
	TMap<FGameplayTag, FGameplayTagContainer> RejectRelations;

protected:
	// 行索引:行键 → {接管列,禁止列}(清场求值/诊断)
	UPROPERTY(Transient)
	TMap<FGameplayTag, FBXBehaviorRelationRow> RelationRowIndex;

	// 列索引:来源(潜在在位方)Tag → 它禁止的域集合(禁止贡献计算,§4.8)
	UPROPERTY(Transient)
	TMap<FGameplayTag, FGameplayTagContainer> ForbidDomainsBySource;
};
