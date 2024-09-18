#pragma once

#include "TimelineTrack.h"
#include "SOutlinerItem.h"



class FBXTLTaskTrackPanel : public FTimelineTrack
{
	TIMELINE_DECLARE_TRACK(FBXTLTaskTrackPanel, FTimelineTrack);

public:
	FBXTLTaskTrackPanel(const TSharedRef<class FBXTLController>& InController, class UBXTask* InTask, const FText& InDisplayName, const FText& InToolTipText);

	void UpdateLayout();

	// 绘制Task时间轨道
	TSharedRef<SWidget> GenerateContainerWidgetForTimeline() override;

	// 绘制Task轨道左边的大纲
	TSharedRef<SWidget> GenerateContainerWidgetForOutliner(const TSharedRef<SOutlinerItem>& InRow) override;

private:
	void OnCommitTrackName(const FText& InText, ETextCommit::Type CommitInfo);

	void RefreshOutlinerWidget();

	void InputViewRangeChanged(float ViewMin, float ViewMax);

	void OnSelectTask();

	void OnDeselectAllTask();

	void RemoveSelectedTasks();

	void AddNewTask(UClass* InTaskClass, float InStartTime);

	void CopySelectedTasks();

	void PasteSelectedTasks();

	void ExportTaskTemplate();

private:
	TWeakPtr<FBXTLController> Controller = nullptr;

	TWeakObjectPtr<class UBXTask> CachedTask = nullptr;

	TSharedPtr<class SBXTLTaskTrack> TrackWidget = nullptr;
	
};