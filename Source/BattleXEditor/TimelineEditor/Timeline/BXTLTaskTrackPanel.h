#pragma once

#include "TimelineTrack.h"
#include "SOutlinerItem.h"



class FBXTLTaskTrackPanel : public FTimelineTrack
{
	TIMELINE_DECLARE_TRACK(FBXTLTaskTrackPanel, FTimelineTrack);

#pragma region Important
public:
	FBXTLTaskTrackPanel(const TSharedRef<class FBXTLController>& InController, class UBXTask* InTask, const FText& InDisplayName, const FText& InToolTipText);

	// 绘制Task时间轨道
	TSharedRef<SWidget> GenerateContainerWidgetForTimeline() override;

	// 绘制Task轨道左边的大纲
	TSharedRef<SWidget> GenerateContainerWidgetForOutliner(const TSharedRef<SOutlinerItem>& InRow) override;

private:
	void RefreshOutlinerPanel();

private:
	TWeakPtr<FBXTLController> Controller = nullptr;

	TWeakObjectPtr<class UBXTask> CachedTask = nullptr;

	TSharedPtr<class SBXTLTaskTrack> TrackWidget = nullptr;

#pragma endregion Important



#pragma region Callback
protected:
	void OnRefreshPanel();

	void OnInputViewRangeChanged(float ViewMin, float ViewMax);

	void OnSelectTask();

	void OnDeselectTasks();

	void OnAddTask(UClass* InTaskClass, float InStartTime);

	void OnDeleteTasks();

	void OnCopyTasks();

	void OnPasteTasks();

	void OnExportTaskTemplate();

#pragma endregion Callback
	
};