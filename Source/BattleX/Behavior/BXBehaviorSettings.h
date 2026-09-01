#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "GameplayTagContainer.h"

#include "BXBehaviorEnums.h"

#include "BXBehaviorSettings.generated.h"



// 行索引负载:行键 → 中断列/禁用列(静态后处理产物,运行时只读)
USTRUCT()
struct FBXBehaviorRelationRow
{
	GENERATED_USTRUCT_BODY()

public:
	// 中断列(行进入时停运列中的在位行为,不受豁免影响)
	UPROPERTY(Transient)
	FGameplayTagContainer ExpelColumns;

	// 禁用列(列存在时挡住行,豁免写入期折算)
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
	// 两张表同格配置时返回禁用+中断,拒绝与挤出各管一段互不遮蔽;
	// 对角线自关系可配:自禁用挡同Tag重入,自中断=新实例顶掉旧实例)
	EBXBehaviorRelation GetRelation(const FGameplayTag& InEntering, const FGameplayTag& InExisting) const;

	// 行为进入时需要中断(停运)的全部活跃行为Tag(沿父链查行索引)
	void GetExpelTargets(const FGameplayTag& InEntering, TArray<FGameplayTag>& OutTags) const;

	// 查询某来源(潜在在位方)禁止的域集合(禁止贡献计算用;调用方沿父链逐级查询)
	const FGameplayTagContainer* FindForbidDomains(const FGameplayTag& InSourceTag) const { return ForbidDomainsBySource.Find(InSourceTag); }

protected:
	// 关系查值(行Tag→列Tag),无配置返回空
	EBXBehaviorRelation FindRelation(const FGameplayTag& InEntering, const FGameplayTag& InExisting) const;

public:
	// 矩阵轴Tag列表(支持族Tag分层匹配;当前行为轴为 BXBehavior.* 平铺Tag,族语义保留给未来分层)
	UPROPERTY(EditAnywhere, Config, Category = "Matrix")
	TArray<FGameplayTag> RelationTags;

	// 中断关系:行进入时停运列中的在位行为(不受豁免影响)
	UPROPERTY(EditAnywhere, Config, Category = "Matrix")
	TMap<FGameplayTag, FGameplayTagContainer> ExpelRelations;

	// 禁用关系:列存在时挡住行(在位方被豁免时不执行——折算发生在账本写入期)
	UPROPERTY(EditAnywhere, Config, Category = "Matrix")
	TMap<FGameplayTag, FGameplayTagContainer> RejectRelations;

protected:
	// 行索引:行键 → {中断列,禁用列}(清场求值/诊断)
	UPROPERTY(Transient)
	TMap<FGameplayTag, FBXBehaviorRelationRow> RelationRowIndex;

	// 列索引:来源(潜在在位方)Tag → 它禁止的域集合(禁止贡献计算,§4.8)
	UPROPERTY(Transient)
	TMap<FGameplayTag, FGameplayTagContainer> ForbidDomainsBySource;
};
