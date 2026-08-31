#include "BattleXTimelineEditor.h" 



UBattleXTimelineEditor::UBattleXTimelineEditor(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

// Project Settings 页面定位:项目设置 → 插件 → BattleXTimelineEditor
// (引擎SettingsEditor自动发现UDeveloperSettings CDO并按这组虚函数注册;手动再注册会同位置双页面)
FName UBattleXTimelineEditor::GetContainerName() const
{
	static const FName ProjectName("Project");
	return ProjectName;
}

FName UBattleXTimelineEditor::GetCategoryName() const
{
	static const FName PluginsName("Plugins");
	return PluginsName;
}

FName UBattleXTimelineEditor::GetSectionName() const
{
	static const FName TLSectionName("BattleXTimelineEditor");
	return TLSectionName;
}

#if WITH_EDITOR
FText UBattleXTimelineEditor::GetSectionText() const
{
#define LOCTEXT_NAMESPACE "BattleXTimelineEditor"
	return LOCTEXT("SectionText", "BattleXTimelineEditor");
#undef LOCTEXT_NAMESPACE
}

FText UBattleXTimelineEditor::GetSectionDescription() const
{
#define LOCTEXT_NAMESPACE "BattleXTimelineEditor"
	return LOCTEXT("SectionDesc", "BattleX时间轴编辑器:预览Tick频率/预览场景/模板等编辑器配置");
#undef LOCTEXT_NAMESPACE
}

void UBattleXTimelineEditor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{

}
#endif
