#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "GameplayTagContainer.h"

#include "BXStateBehaviorSettings.generated.h"



// 全局状态-行为关系配置(状态矩阵:行=状态Tag,列=行为Tag;各端一致的ini)
// 行=该状态进入时中断哪些行为+存续期禁用哪些行为;列=被作用的行为,无独立语义
// 状态对行为的门控唯一配置面(2026-09-15起,SM节点/裸状态配置上的两列表已移除)
UCLASS(Config = BattleX, DefaultConfig, Meta = (DisplayName = "BattleXStateBehaviorRelations"))
class BATTLEX_API UBXStateBehaviorSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	// Project Settings 页面定位(引擎自动发现UDeveloperSettings并按此注册,勿再手动ISettingsModule注册——会双页面)
	virtual FName GetContainerName() const override;
	virtual FName GetCategoryName() const override;
	virtual FName GetSectionName() const override;
#if WITH_EDITOR
	virtual FText GetSectionText() const override;
	virtual FText GetSectionDescription() const override;

	// 关闭设置页自动注册(2026-09-16起矩阵UI由UBXSettings的BattleX页面定制注入,本类不再有独立页面;
	// 仅影响页面可见性——配置加载/GetDefault访问/直连ini读写均不受影响;基类此虚函数在WITH_EDITOR内,重写须同样包裹)
	virtual bool SupportsAutoRegistration() const override { return false; }
#endif

	// 状态关系配置的目标 ini:插件 Config/DefaultBattleX.ini(与行为关系配置同文件不同节)
	static FString GetPluginConfigIniPath();

	virtual void PostInitProperties() override;

	// 变更落盘:直写插件Config/DefaultBattleX.ini+Flush+清项目层残留旧节(矩阵编辑器Commit调用)
	void SaveToPluginConfig();

	// 查询状态对行为的门控两列表(行键=状态Tag精确直查;行为列可含族Tag,族语义由行为组件侧匹配承担)
	void GetStateBehaviorGates(const FGameplayTag& InStateTag, FGameplayTagContainer& OutInterrupt, FGameplayTagContainer& OutForbid) const;

	// 查询某状态是否作用指定行为(中断∨禁止;DoesStateDisableBehavior查询用)
	bool DoesStateGateBehavior(const FGameplayTag& InStateTag, const FGameplayTag& InBehaviorTag) const;

public:
	// 矩阵行轴(状态Tag列表)
	UPROPERTY(EditAnywhere, Config, Category = "Matrix")
	TArray<FGameplayTag> StateRelationTags;

	// 矩阵列轴(行为Tag列表)
	UPROPERTY(EditAnywhere, Config, Category = "Matrix")
	TArray<FGameplayTag> BehaviorRelationTags;

	// 中断关系:状态进入时停运列中的在位行为(一次性动作不记账,状态退出无解除)
	UPROPERTY(EditAnywhere, Config, Category = "Matrix")
	TMap<FGameplayTag, FGameplayTagContainer> InterruptRelations;

	// 禁用关系:状态存续期禁用列中的行为(挡启动;状态退出自动解除)
	UPROPERTY(EditAnywhere, Config, Category = "Matrix")
	TMap<FGameplayTag, FGameplayTagContainer> ForbidRelations;
};
