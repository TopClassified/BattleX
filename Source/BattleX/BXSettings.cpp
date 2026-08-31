#include "BXSettings.h"

#include "BXStructs.h"
#include "BXManager.h" 



// Project Settings 页面定位:项目设置 → 插件 → BattleX
// (引擎SettingsEditor自动发现UDeveloperSettings CDO并按这组虚函数注册;手动再注册会同位置双页面)
FName UBXSettings::GetContainerName() const
{
	static const FName ProjectName("Project");
	return ProjectName;
}

FName UBXSettings::GetCategoryName() const
{
	static const FName PluginsName("Plugins");
	return PluginsName;
}

FName UBXSettings::GetSectionName() const
{
	static const FName BattleXSectionName("BattleX");
	return BattleXSectionName;
}

#if WITH_EDITOR
FText UBXSettings::GetSectionText() const
{
#define LOCTEXT_NAMESPACE "BXSettings"
	return LOCTEXT("SectionText", "BattleX");
#undef LOCTEXT_NAMESPACE
}

FText UBXSettings::GetSectionDescription() const
{
#define LOCTEXT_NAMESPACE "BXSettings"
	return LOCTEXT("SectionDesc", "BattleX核心配置:管理器注册/碰撞Debug/任务处理器映射/技能同步参数等");
#undef LOCTEXT_NAMESPACE
}
#endif

UBXSettings::UBXSettings(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	FDirectoryPath DefaultPath;
	DefaultPath.Path = "/Game/BattleX/Projectile";
	ProjectileAssetPaths.Add(DefaultPath);
}
