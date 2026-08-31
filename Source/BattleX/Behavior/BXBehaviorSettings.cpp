#include "BXBehaviorSettings.h"

#include "BXGameplayTags.h"



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
	return LOCTEXT("SectionDesc", "行为矩阵:行为域之间的禁止/接管关系配置(轴经页面内按钮添加)");
#undef LOCTEXT_NAMESPACE
}
#endif

void UBXBehaviorSettings::PostInitProperties()
{
	Super::PostInitProperties();

	// 配置加载完成后重建后处理索引
	RebuildRelationIndex();
}

void UBXBehaviorSettings::PostReloadConfig(class FProperty* PropertyThatWasLoaded)
{
	Super::PostReloadConfig(PropertyThatWasLoaded);

	RebuildRelationIndex();
}

void UBXBehaviorSettings::RebuildRelationIndex()
{
	RelationRowIndex.Reset();
	ForbidDomainsBySource.Reset();

	// 行索引:两表按行键合并(同格配置=禁止+接管,各管一段互不遮蔽)
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

	// 列索引:反排禁止列(来源 → 它禁止的域集合;接管是动作不贡献账本,不入索引)
	for (const TPair<FGameplayTag, FGameplayTagContainer>& Pair : RejectRelations)
	{
		for (int32 i = 0; i < Pair.Value.Num(); ++i)
		{
			ForbidDomainsBySource.FindOrAdd(Pair.Value.GetByIndex(i)).AddTag(Pair.Key);
		}
	}
}

EBXBehaviorRelation UBXBehaviorSettings::GetRelation(const FGameplayTag& InEntering, const FGameplayTag& InExisting) const
{
	if (InEntering == InExisting || !InEntering.IsValid() || !InExisting.IsValid())
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

	// 沿父链查行索引(精确键命中穷尽 MatchesTag 的全部行),收集接管列
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
	// 两轴独立求值后合并(同格配置时返回禁止+接管,拒绝与挤出各管一段互不遮蔽)
	bool bExpel = false;
	bool bForbid = false;

	if (const FGameplayTagContainer* Container = ExpelRelations.Find(InEntering))
	{
		if (InExisting.MatchesAny(*Container))
		{
			bExpel = true;
		}
	}

	if (const FGameplayTagContainer* Container = RejectRelations.Find(InEntering))
	{
		if (InExisting.MatchesAny(*Container))
		{
			bForbid = true;
		}
	}

	// 族Tag方向匹配:行Tag为族,进入行为为其子Tag时继承关系
	for (const TPair<FGameplayTag, FGameplayTagContainer>& Pair : ExpelRelations)
	{
		if (!bExpel && InEntering.MatchesTag(Pair.Key) && InExisting.MatchesAny(Pair.Value))
		{
			bExpel = true;
		}
	}

	for (const TPair<FGameplayTag, FGameplayTagContainer>& Pair : RejectRelations)
	{
		if (!bForbid && InEntering.MatchesTag(Pair.Key) && InExisting.MatchesAny(Pair.Value))
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
