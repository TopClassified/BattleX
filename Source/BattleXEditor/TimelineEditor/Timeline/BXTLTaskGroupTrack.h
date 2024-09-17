#pragma once

#include "TimelineTrack.h"

#include "BXTLAsset.h"



class FBXTLTaskGroupTrack : public FTimelineTrack
{
	TIMELINE_DECLARE_TRACK(FBXTLTaskGroupTrack, FTimelineTrack);

public:
	FBXTLTaskGroupTrack(const TSharedRef<class FBXTLController>& InController, FBXTLTaskGroup& InGroupData, const FText& InDisplayName, const FText& InToolTipText);

	// 绘制Group轨道左边的大纲
	TSharedRef<class SWidget> GenerateContainerWidgetForOutliner(const TSharedRef<SOutlinerItem>& InRow) override;

	bool CanRename() const override { return true; }

	FText GetLabel() const override;

	void OnCommitCurveName(const FText& InText, ETextCommit::Type CommitInfo);

	void AddTaskTemplate(FName TemplateName);

private:
	TSharedRef<class SWidget> BuildGroupSubMenu();

	void AddNewTask(UClass* InTaskClass);

	void PasteTasks();

	void AddTaskGroup();

	void DeleteTaskGroup();

	void FillNewTaskMenu(class FMenuBuilder& MenuBuilder);

	void FillNewTaskTemplateMenu(class FMenuBuilder& MenuBuilder);

private:
	FBXTLTaskGroup* GroupData = nullptr;

};