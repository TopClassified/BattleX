#include "BXTLTaskTrackPanel.h"
#include "Misc/MessageDialog.h"
#include "HAL/PlatformApplicationMisc.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"

#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Views/SExpanderArrow.h"
#include "Widgets/Text/SInlineEditableTextBlock.h"

#include "TimelineTrack.h"
#include "SBXTLTaskTrack.h"
#include "SBXTLTaskTrackOutliner.h"
#include "BXTLController.h"
#include "BXTLEditorUtilities.h"

#include "BXTask.h"
#include "BXTLAsset.h"



#define LOCTEXT_NAMESPACE "FBXTLTaskTrackPanel"

TIMELINE_IMPLEMENT_TRACK(FBXTLTaskTrackPanel);

#pragma region Important
FBXTLTaskTrackPanel::FBXTLTaskTrackPanel
(
	const TSharedRef<FBXTLController>& InController, UBXTask* InTask, const FText& InToolTipText
) : FTimelineTrack(InController, InTask->GetDisplayName(), InToolTipText), CachedTask(InTask)
{
	Controller = InController;

	SetHeight(FTimelineTrack::TimelineTrackHeight);
}

TSharedRef<SWidget> FBXTLTaskTrackPanel::GenerateContainerWidgetForTimeline()
{
	if (!TrackWidget.IsValid())
	{
		TrackWidget = SNew(SBXTLTaskTrack)
			.Task(CachedTask.Get())
			.Controller(Controller)
			.InputMin(this, &FBXTLTaskTrackPanel::GetMinInput)
			.InputMax(this, &FBXTLTaskTrackPanel::GetMaxInput)
			.ViewInputMin(this, &FBXTLTaskTrackPanel::GetViewMinInput)
			.ViewInputMax(this, &FBXTLTaskTrackPanel::GetViewMaxInput)
			.TimelinePlayLength(GetEditorTimelineController()->GetPlayLength())
			.FrameRate(GetEditorTimelineController()->GetFrameRate())
			.RefreshPanelEvent(this, &FBXTLTaskTrackPanel::OnRefreshPanel)
			.SetInputViewRangeEvent(this, &FBXTLTaskTrackPanel::OnInputViewRangeChanged)
			.SelectNodeEvent(this, &FBXTLTaskTrackPanel::OnSelectTask)
			.DeselectNodesEvent(this, &FBXTLTaskTrackPanel::OnDeselectTasks)
			.AddTaskEvent(this, &FBXTLTaskTrackPanel::OnAddTask)
			.DeleteTaskEvent(this, &FBXTLTaskTrackPanel::OnDeleteTasks)
			.CopyTasksEvent(this, &FBXTLTaskTrackPanel::OnCopyTasks)
			.PasteTasksEvent(this, &FBXTLTaskTrackPanel::OnPasteTasks)
			.ExportTemplateEvent(this, &FBXTLTaskTrackPanel::OnExportTaskTemplate);
	}

	return TrackWidget.ToSharedRef();
}

TSharedRef<SWidget> FBXTLTaskTrackPanel::GenerateContainerWidgetForOutliner(const TSharedRef<SOutlinerItem>& InRow)
{
	TSharedPtr<SBorder> OuterBorder = SNew(SBorder)
		.ToolTipText(this, &FTimelineTrack::GetToolTipText)
		.BorderImage(FAppStyle::GetBrush("Sequencer.Section.BackgroundTint"))
		.BorderBackgroundColor(FAppStyle::GetColor("AnimTimeline.Outliner.ItemColor"));

	TSharedRef<SWidget> TaskTrackPanel = SNew(SBXTLTaskTrackOutliner, GetEditorTimelineController(), CachedTask.Get())
		.TrackPanelArea(OuterBorder)
		.InlineEditableTextBlock
		(
			SNew(STextBlock)
			.Text_Lambda([this]() { return CachedTask.IsValid() ? CachedTask->GetDisplayName() : DisplayName; })
		);

	return TaskTrackPanel;
}

void FBXTLTaskTrackPanel::RefreshOutlinerPanel()
{
	
}

#pragma endregion Important



#pragma region Callback
void FBXTLTaskTrackPanel::OnRefreshPanel()
{
	SetHeight(FTimelineTrack::TimelineTrackHeight);

	RefreshOutlinerPanel();

	if (TrackWidget.IsValid())
	{
		TrackWidget->UpdateLayout();
	}
}

void FBXTLTaskTrackPanel::OnInputViewRangeChanged(float ViewMin, float ViewMax)
{

}

void FBXTLTaskTrackPanel::OnSelectTask()
{
	if (FBXTLController* TC = static_cast<FBXTLController*>(TimelineController.Pin().Get()))
	{
		TArray<UBXTask*> TaskObjects;
		TaskObjects.Add(CachedTask.Get());

		TC->ChangeTaskSelection(TaskObjects);
	}
}

void FBXTLTaskTrackPanel::OnDeselectTasks()
{
	if (FBXTLController* TC = static_cast<FBXTLController*>(TimelineController.Pin().Get()))
	{
		TC->ChangeTaskSelection(TArray<UBXTask*>{});
	}
}

void FBXTLTaskTrackPanel::OnAddTask(UClass* InTaskClass, float InStartTime)
{
	if (!CachedTask.IsValid())
	{
		return;
	}

	if (FBXTLController* TC = static_cast<FBXTLController*>(TimelineController.Pin().Get()))
	{
		if (FBXTLTaskGroup* Group = TC->GetTaskGroup(CachedTask.Get()))
		{
			TC->AddNewTask(*Group, InTaskClass, InStartTime);
		}
	}
}

void FBXTLTaskTrackPanel::OnDeleteTasks()
{
	if (FBXTLController* TC = static_cast<FBXTLController*>(TimelineController.Pin().Get()))
	{
		TC->DeleteSelectedTasks();
	}
}

void FBXTLTaskTrackPanel::OnCopyTasks()
{
	if (FBXTLController* TC = static_cast<FBXTLController*>(TimelineController.Pin().Get()))
	{
		TC->CopySelectedTasks();
	}
}

void FBXTLTaskTrackPanel::OnPasteTasks()
{
	if (FBXTLController* TC = static_cast<FBXTLController*>(TimelineController.Pin().Get()))
	{
		FString PasteString;
		FPlatformApplicationMisc::ClipboardPaste(PasteString);

		TC->PasteSelectedTasks(PasteString, TC->GetTaskGroupIndex(CachedTask.Get()));
	}
}

void FBXTLTaskTrackPanel::OnExportTaskTemplate()
{
	if (FBXTLController* TC = static_cast<FBXTLController*>(TimelineController.Pin().Get()))
	{
		TC->ExportSelectedTaskTemplate();
	}
}
#pragma endregion Callback

#undef LOCTEXT_NAMESPACE