#include "BXBehaviorSettings.h"

#include "BXGameplayTags.h"
#include "Interfaces/IPluginManager.h"
#include "Misc/ConfigCacheIni.h"



UBXBehaviorSettings::UBXBehaviorSettings()
{
}

// Project Settings 页面定位:项目设置 → 插件 → BattleXBehaviorRelations
// (引擎SettingsEditor自动发现UDeveloperSettings CDO并按这组虚函数注册;手动再注册会同位置双页面)
FName UBXBehaviorSettings::GetContainerName() const
{
	static const FName ProjectName("Project");
	return ProjectName;
}

FName UBXBehaviorSettings::GetCategoryName() const
{
	static const FName PluginsName("Plugins");
	return PluginsName;
}

FName UBXBehaviorSettings::GetSectionName() const
{
	static const FName RelationsSectionName("BattleXBehaviorRelations");
	return RelationsSectionName;
}

#if WITH_EDITOR
FText UBXBehaviorSettings::GetSectionText() const
{
#define LOCTEXT_NAMESPACE "BXBehaviorSettings"
	return LOCTEXT("SectionText", "BattleXBehaviorRelations");
#undef LOCTEXT_NAMESPACE
}

FText UBXBehaviorSettings::GetSectionDescription() const
{
#define LOCTEXT_NAMESPACE "BXBehaviorSettings"
		return LOCTEXT("SectionDesc", "行为矩阵:行为域之间的禁用/中断关系配置(轴经页面内按钮添加;对角线=同行为自关系)");
#undef LOCTEXT_NAMESPACE
}
#endif

void UBXBehaviorSettings::PostInitProperties()
{
	Super::PostInitProperties();

	// 行为关系配置直读插件 ini(绕过 BattleX 自定义链的层级合并——裸键数组跨层合并会退化为末值,
	// 表现为重启后只剩最后添加的轴;插件文件是本类唯一事实源)。路径解析失败时保持默认链读取
	const FString PluginIniPath = GetPluginConfigIniPath();
	if (!PluginIniPath.IsEmpty())
	{
		LoadConfig(nullptr, *PluginIniPath);
	}

	// 配置加载完成后重建后处理索引
	RebuildRelationIndex();
}

void UBXBehaviorSettings::PostReloadConfig(class FProperty* PropertyThatWasLoaded)
{
	Super::PostReloadConfig(PropertyThatWasLoaded);

	RebuildRelationIndex();
}

FString UBXBehaviorSettings::GetPluginConfigIniPath()
{
	// 行为关系配置随插件分发:插件 Config/DefaultBattleX.ini(读写均直连该文件,不经层级合并)
	if (const TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin(TEXT("BattleX")))
	{
		return FPaths::Combine(Plugin->GetBaseDir(), TEXT("Config"), TEXT("DefaultBattleX.ini"));
	}
	return FString();
}

void UBXBehaviorSettings::RebuildRelationIndex()
{
	RelationRowIndex.Reset();
	ForbidDomainsBySource.Reset();

	// 清理:丢弃不在矩阵轴内的键与列(改名/删轴残留——编辑器矩阵只渲染轴,这些条目不可见无法手清)
	FGameplayTagContainer AxisContainer;
	AxisContainer.AppendTags(RelationTags);
	auto IsAxisRelated = [&AxisContainer](const FGameplayTag& Tag)
	{
		return AxisContainer.MatchesTag(Tag) || Tag.MatchesAny(AxisContainer);
	};
	{
		TArray<FGameplayTag> StaleExpelKeys;
		TArray<FGameplayTag> StaleRejectKeys;
		for (const TPair<FGameplayTag, FGameplayTagContainer>& Pair : ExpelRelations)
		{
			if (!IsAxisRelated(Pair.Key))
			{
				StaleExpelKeys.Add(Pair.Key);
			}
		}
		for (const FGameplayTag& StaleKey : StaleExpelKeys)
		{
			ExpelRelations.Remove(StaleKey);
		}
		for (const TPair<FGameplayTag, FGameplayTagContainer>& Pair : RejectRelations)
		{
			if (!IsAxisRelated(Pair.Key))
			{
				StaleRejectKeys.Add(Pair.Key);
			}
		}
		for (const FGameplayTag& StaleKey : StaleRejectKeys)
		{
			RejectRelations.Remove(StaleKey);
		}
		for (TPair<FGameplayTag, FGameplayTagContainer>& Pair : ExpelRelations)
		{
			for (int32 i = Pair.Value.Num() - 1; i >= 0; --i)
			{
				if (!IsAxisRelated(Pair.Value.GetByIndex(i)))
				{
					Pair.Value.RemoveTag(Pair.Value.GetByIndex(i));
				}
			}
		}
		for (TPair<FGameplayTag, FGameplayTagContainer>& Pair : RejectRelations)
		{
			for (int32 i = Pair.Value.Num() - 1; i >= 0; --i)
			{
				if (!IsAxisRelated(Pair.Value.GetByIndex(i)))
				{
					Pair.Value.RemoveTag(Pair.Value.GetByIndex(i));
				}
			}
		}
	}

	// 行索引:两表按行键合并(同格配置=禁用+中断,各管一段互不遮蔽)
	for (const TPair<FGameplayTag, FGameplayTagContainer>& Pair : ExpelRelations)
	{
		FBXBehaviorRelationRow& Row = RelationRowIndex.FindOrAdd(Pair.Key);
		Row.ExpelColumns.AppendTags(Pair.Value);
	}
	for (const TPair<FGameplayTag, FGameplayTagContainer>& Pair : RejectRelations)
	{
		FBXBehaviorRelationRow& Row = RelationRowIndex.FindOrAdd(Pair.Key);
		Row.ForbidColumns.AppendTags(Pair.Value);
	}

	// 列索引:行=禁用来源(在位行为),列=被禁域(用户语义:行在位期间禁用其列;
	// 行开始时中断列由行索引承载,中断是动作不贡献账本)
	for (const TPair<FGameplayTag, FGameplayTagContainer>& Pair : RejectRelations)
	{
		for (int32 i = 0; i < Pair.Value.Num(); ++i)
		{
			ForbidDomainsBySource.FindOrAdd(Pair.Key).AddTag(Pair.Value.GetByIndex(i));
		}
	}
}

EBXBehaviorRelation UBXBehaviorSettings::GetRelation(const FGameplayTag& InEntering, const FGameplayTag& InExisting) const
{
	// 同行为自关系可配(对角线):自禁用挡同Tag重入,自中断=新实例顶掉旧实例,不再特殊早退
	if (!InEntering.IsValid() || !InExisting.IsValid())
	{
		return EBXBehaviorRelation::BR_None;
	}

	return FindRelation(InEntering, InExisting);
}

void UBXBehaviorSettings::GetExpelTargets(const FGameplayTag& InEntering, TArray<FGameplayTag>& OutTags) const
{
	OutTags.Reset();

	if (!InEntering.IsValid())
	{
		return;
	}

	// 沿父链查行索引(精确键命中穷尽 MatchesTag 的全部行),收集中断列
	FGameplayTag Cursor = InEntering;
	while (Cursor.IsValid())
	{
		if (const FBXBehaviorRelationRow* Row = RelationRowIndex.Find(Cursor))
		{
			for (int32 i = 0; i < Row->ExpelColumns.Num(); ++i)
			{
				OutTags.AddUnique(Row->ExpelColumns.GetByIndex(i));
			}
		}

		Cursor = Cursor.RequestDirectParent();
	}
}

EBXBehaviorRelation UBXBehaviorSettings::FindRelation(const FGameplayTag& InEntering, const FGameplayTag& InExisting) const
{
	// 中断:进入者(InEntering)的中断列命中在位者(InExisting)——两遍:精确键+族行
	bool bExpel = false;
	bool bForbid = false;

	if (const FGameplayTagContainer* Container = ExpelRelations.Find(InEntering))
	{
		if (InExisting.MatchesAny(*Container))
		{
			bExpel = true;
		}
	}

	// 禁用(用户语义:行在位期间禁用其列):在位者(InExisting)命中的行,其禁用列覆盖进入者(InEntering)
	if (const FGameplayTagContainer* Container = RejectRelations.Find(InExisting))
	{
		if (InEntering.MatchesAny(*Container))
		{
			bForbid = true;
		}
	}

	// 族行两遍:进入者继承族行的中断列 / 在位者继承族行的禁用列
	for (const TPair<FGameplayTag, FGameplayTagContainer>& Pair : ExpelRelations)
	{
		if (!bExpel && InEntering.MatchesTag(Pair.Key) && InExisting.MatchesAny(Pair.Value))
		{
			bExpel = true;
		}
	}

	for (const TPair<FGameplayTag, FGameplayTagContainer>& Pair : RejectRelations)
	{
		if (!bForbid && InExisting.MatchesTag(Pair.Key) && InEntering.MatchesAny(Pair.Value))
		{
			bForbid = true;
		}
	}

	if (bExpel && bForbid)
	{
		return EBXBehaviorRelation::BR_ForbidExpel;
	}
	if (bExpel)
	{
		return EBXBehaviorRelation::BR_Expel;
	}
	if (bForbid)
	{
		return EBXBehaviorRelation::BR_Forbid;
	}

	return EBXBehaviorRelation::BR_None;
}
