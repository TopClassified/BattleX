#include "BXStateBehaviorSettings.h"

#include "Interfaces/IPluginManager.h"
#include "Misc/ConfigCacheIni.h"



void UBXStateBehaviorSettings::SaveToPluginConfig()
{
	// 状态关系配置写入插件 Config 目录(随插件分发):SaveConfig 默认落点是项目 Config/DefaultBattleX.ini,
	// 与插件层副本形成两份漂移;显式传 Filename 落插件文件(读取侧 PostInitProperties 亦直读该文件)
	const TCHAR* ConfigSectionName = TEXT("/Script/BattleX.BXStateBehaviorSettings");
	const FString PluginIniPath = GetPluginConfigIniPath();
	if (!PluginIniPath.IsEmpty())
	{
		SaveConfig(CPF_Config, *PluginIniPath);
		GConfig->Flush(false, *PluginIniPath);

		// 迁移清理:默认落点(项目 DefaultBattleX.ini)若残留本类旧节,清掉并落盘——项目层同节会以更高优先级遮蔽插件层
		const FString ProjectIniPath = GConfig->GetConfigFilename(TEXT("BattleX"));
		if (!ProjectIniPath.Equals(PluginIniPath) && GConfig->DoesSectionExist(ConfigSectionName, ProjectIniPath))
		{
			GConfig->EmptySection(ConfigSectionName, ProjectIniPath);
			GConfig->Flush(false, ProjectIniPath);
		}
	}
	else
	{
		SaveConfig();
	}
}



// Project Settings 页面定位:项目设置 → 插件 → BattleXStateBehaviorRelations
// (引擎SettingsEditor自动发现UDeveloperSettings CDO并按这组虚函数注册;手动再注册会同位置双页面)
FName UBXStateBehaviorSettings::GetContainerName() const
{
	static const FName ProjectName("Project");
	return ProjectName;
}

FName UBXStateBehaviorSettings::GetCategoryName() const
{
	static const FName PluginsName("Plugins");
	return PluginsName;
}

FName UBXStateBehaviorSettings::GetSectionName() const
{
	static const FName RelationsSectionName("BattleXStateBehaviorRelations");
	return RelationsSectionName;
}

#if WITH_EDITOR
FText UBXStateBehaviorSettings::GetSectionText() const
{
#define LOCTEXT_NAMESPACE "BXStateBehaviorSettings"
	return LOCTEXT("SectionText", "BattleXStateBehaviorRelations");
#undef LOCTEXT_NAMESPACE
}

FText UBXStateBehaviorSettings::GetSectionDescription() const
{
#define LOCTEXT_NAMESPACE "BXStateBehaviorSettings"
	return LOCTEXT("SectionDesc", "状态矩阵:状态对行为的中断/禁用关系配置(行=状态,列=行为;空格=天然共存)");
#undef LOCTEXT_NAMESPACE
}
#endif

FString UBXStateBehaviorSettings::GetPluginConfigIniPath()
{
	// 状态关系配置随插件分发:插件 Config/DefaultBattleX.ini(读写均直连该文件,不经层级合并)
	if (const TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin(TEXT("BattleX")))
	{
		return FPaths::Combine(Plugin->GetBaseDir(), TEXT("Config"), TEXT("DefaultBattleX.ini"));
	}
	return FString();
}

void UBXStateBehaviorSettings::PostInitProperties()
{
	Super::PostInitProperties();

	// 状态关系配置直读插件 ini(绕过 BattleX 自定义链的层级合并——裸键数组跨层合并会退化为末值,
	// 行为关系配置曾因此重启后只剩最后添加的轴;插件文件是本类唯一事实源)。路径解析失败时保持默认链读取
	const FString PluginIniPath = GetPluginConfigIniPath();
	if (!PluginIniPath.IsEmpty())
	{
		LoadConfig(nullptr, *PluginIniPath);
	}
}

void UBXStateBehaviorSettings::GetStateBehaviorGates(const FGameplayTag& InStateTag, FGameplayTagContainer& OutInterrupt, FGameplayTagContainer& OutForbid) const
{
	OutInterrupt.Reset();
	OutForbid.Reset();

	if (!InStateTag.IsValid())
	{
		return;
	}

	if (const FGameplayTagContainer* Container = InterruptRelations.Find(InStateTag))
	{
		OutInterrupt = *Container;
	}
	if (const FGameplayTagContainer* Container = ForbidRelations.Find(InStateTag))
	{
		OutForbid = *Container;
	}
}

bool UBXStateBehaviorSettings::DoesStateGateBehavior(const FGameplayTag& InStateTag, const FGameplayTag& InBehaviorTag) const
{
	if (!InStateTag.IsValid() || !InBehaviorTag.IsValid())
	{
		return false;
	}

	if (const FGameplayTagContainer* Container = InterruptRelations.Find(InStateTag))
	{
		if (Container->HasTag(InBehaviorTag))
		{
			return true;
		}
	}
	if (const FGameplayTagContainer* Container = ForbidRelations.Find(InStateTag))
	{
		if (Container->HasTag(InBehaviorTag))
		{
			return true;
		}
	}

	return false;
}
