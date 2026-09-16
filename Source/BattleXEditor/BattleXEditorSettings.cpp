#include "BattleXEditorSettings.h"



UBattleXEditorSettings::UBattleXEditorSettings(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

// Project Settings 页面定位:项目设置 → 插件 → BattleXEditor
// (引擎SettingsEditor自动发现UDeveloperSettings CDO并按这组虚函数注册;手动再注册会同位置双页面)
FName UBattleXEditorSettings::GetContainerName() const
{
	static const FName ProjectName("Project");
	return ProjectName;
}

FName UBattleXEditorSettings::GetCategoryName() const
{
	static const FName PluginsName("Plugins");
	return PluginsName;
}

FName UBattleXEditorSettings::GetSectionName() const
{
	static const FName EditorSectionName("BattleXEditor");
	return EditorSectionName;
}

#if WITH_EDITOR
FText UBattleXEditorSettings::GetSectionText() const
{
#define LOCTEXT_NAMESPACE "BattleXEditorSettings"
	return LOCTEXT("SectionText", "BattleXEditor");
#undef LOCTEXT_NAMESPACE
}

FText UBattleXEditorSettings::GetSectionDescription() const
{
#define LOCTEXT_NAMESPACE "BattleXEditorSettings"
	return LOCTEXT("SectionDesc", "BattleX编辑器:预览Tick频率/预览场景/模板等编辑器配置(编辑器设置收拢于此)");
#undef LOCTEXT_NAMESPACE
}

void UBattleXEditorSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{

}
#endif
