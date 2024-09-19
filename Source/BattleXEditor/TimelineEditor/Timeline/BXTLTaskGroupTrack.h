#pragma once

#include "TimelineTrack.h"

#include "BXTLAsset.h"



class FBXTLTaskGroupTrack : public FTimelineTrack
{
	TIMELINE_DECLARE_TRACK(FBXTLTaskGroupTrack, FTimelineTrack);

#pragma region Important
public:
	FBXTLTaskGroupTrack(const TSharedRef<class FBXTLController>& InController, FBXTLTaskGroup& InGroupData, const FText& InDisplayName, const FText& InToolTipText);

	// 绘制Group轨道左边的大纲
	TSharedRef<class SWidget> GenerateContainerWidgetForOutliner(const TSharedRef<SOutlinerItem>& InRow) override;

	bool CanRename() const override;

	FText GetLabel() const override;

protected:
	int32 GetGroupIndex() const;

private:
	FBXTLTaskGroup* GroupData = nullptr;

#pragma endregion Important



#pragma region Callback
protected:
	void OnCommitName(const FText& InText, ETextCommit::Type CommitInfo);

	TSharedRef<class SWidget> OnBuildSubMenu();

	void OnFillTaskMenu(class FMenuBuilder& MenuBuilder);

	void OnFillTaskTemplateMenu(class FMenuBuilder& MenuBuilder);

	void OnAddTask(UClass* InTaskClass);

	void OnPasteTasks();

	void OnAddGroup();

	void OnDeleteGroup();

	void OnAddTaskTemplate(FName TemplateName);

#pragma endregion Callback

};