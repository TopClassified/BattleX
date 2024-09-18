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

FBXTLTaskTrackPanel::FBXTLTaskTrackPanel
(
	const TSharedRef<FBXTLController>& InController, UBXTask* InTask,
	const FText& InDisplayName, const FText& InToolTipText
) : FTimelineTrack(InController, InDisplayName, InToolTipText), CachedTask(InTask)
{
	Controller = InController;

	SetHeight(FTimelineTrack::TimelineTrackHeight);
}

void FBXTLTaskTrackPanel::UpdateLayout()
{
	SetHeight(FTimelineTrack::TimelineTrackHeight);

	RefreshOutlinerWidget();

	if (TrackWidget.IsValid())
	{
		TrackWidget->UpdateLayout();
	}
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
			.OnSetInputViewRange(this, &FBXTLTaskTrackPanel::InputViewRangeChanged)
			.OnRefreshPanel(this, &FBXTLTaskTrackPanel::UpdateLayout)
			.OnSelectNode(this, &FBXTLTaskTrackPanel::OnSelectTask)
			.OnDeselectAllNodes(this, &FBXTLTaskTrackPanel::OnDeselectAllTask)
			.OnDeleteTask(this, &FBXTLTaskTrackPanel::RemoveSelectedTasks)
			.OnAddNewTask(this, &FBXTLTaskTrackPanel::AddNewTask)
			.OnCopyTasks(this, &FBXTLTaskTrackPanel::CopySelectedTasks)
			.OnPasteTasks(this, &FBXTLTaskTrackPanel::PasteSelectedTasks)
			.OnExportTemplate(this, &FBXTLTaskTrackPanel::ExportTaskTemplate);
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
			SNew(SInlineEditableTextBlock)
			.Text_Lambda([this]() { return DisplayName; })
			.IsSelected(FIsSelected::CreateLambda([]() { return false; }))
			.OnTextCommitted(this, &FBXTLTaskTrackPanel::OnCommitTrackName)
		);

	return TaskTrackPanel;
}

void FBXTLTaskTrackPanel::OnCommitTrackName(const FText& InText, ETextCommit::Type CommitInfo)
{
	FText TrimText = FText::TrimPrecedingAndTrailing(InText);
	if (FBXTLController* TC = static_cast<FBXTLController*>(TimelineController.Pin().Get()))
	{
		TC->ChangeTaskName(CachedTask.Get(), TrimText);
	}

	this->UpdateLayout();
}

void FBXTLTaskTrackPanel::RefreshOutlinerWidget()
{
	
}

void FBXTLTaskTrackPanel::InputViewRangeChanged(float ViewMin, float ViewMax)
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

void FBXTLTaskTrackPanel::OnDeselectAllTask()
{
	if (FBXTLController* TC = static_cast<FBXTLController*>(TimelineController.Pin().Get()))
	{
		TC->ChangeTaskSelection(TArray<UBXTask*>{});
	}
}

void FBXTLTaskTrackPanel::RemoveSelectedTasks()
{
	if (FBXTLController* TC = static_cast<FBXTLController*>(TimelineController.Pin().Get()))
	{
		TC->DeleteSelectedTasks();
	}
}

void FBXTLTaskTrackPanel::AddNewTask(UClass* InTaskClass, float InStartTime)
{
	if (FBXTLController* TC = static_cast<FBXTLController*>(TimelineController.Pin().Get()))
	{
		if (CachedTask.IsValid())
		{
			if (UBXTLAsset* Asset = Cast<UBXTLAsset>(CachedTask->GetOuter()))
			{
				int32 SectionID = -1;
				int32 GroupID = -1;

				if (Asset->GetSectionIDAndGroupID(CachedTask.Get(), SectionID, GroupID))
				{
					TC->AddNewTask(Asset->Sections[SectionID].Groups[GroupID], InTaskClass, InStartTime);
				}
			}
		}
	}
}

void FBXTLTaskTrackPanel::CopySelectedTasks()
{
	if (FBXTLController* TC = static_cast<FBXTLController*>(TimelineController.Pin().Get()))
	{
		if (TC->GetSelectedTasks().Num() == 0)
		{
			if (TC->GetSelectedTasks().Num() == 0)
			{
				FText DialogText = LOCTEXT("提示", "请先选中Task再复制");
				FMessageDialog::Open(EAppMsgType::Ok, DialogText);
				return;
			}
		}

		TC->CopySelectedTasks();
	}
}

void FBXTLTaskTrackPanel::PasteSelectedTasks()
{
	if (FBXTLController* TC = static_cast<FBXTLController*>(TimelineController.Pin().Get()))
	{
		FString SubString = TEXT("BEGIN Copy UTask!\n");

		FString PasteString;
		FPlatformApplicationMisc::ClipboardPaste(PasteString);
		if (PasteString.Contains(SubString))
		{
			PasteString = PasteString.Replace(*SubString, TEXT(""));

			int32 GroupID = -1;
			if (CachedTask.IsValid() && TC->GetAsset())
			{
				int32 SectionID;
				TC->GetAsset()->GetSectionIDAndGroupID(CachedTask.Get(), SectionID, GroupID);
			}

			TC->PasteSelectedTasks(PasteString, GroupID);
		}
		else
		{
			FText DialogText = LOCTEXT("提示", "目前没有复制任何Task");
			FMessageDialog::Open(EAppMsgType::Ok, DialogText);
		}
	}
}

void FBXTLTaskTrackPanel::ExportTaskTemplate()
{
	if (FBXTLController* TC = static_cast<FBXTLController*>(TimelineController.Pin().Get()))
	{
		if (TC->GetSelectedTasks().Num() == 0)
		{
			TC = static_cast<FBXTLController*>(TimelineController.Pin().Get());
			if (TC->GetSelectedTasks().Num() == 0)
			{
				FText DialogText = LOCTEXT("提示", "请先选中Task");
				FMessageDialog::Open(EAppMsgType::Ok, DialogText);
				return;
			}
		}

		TC->ExportSelectedTaskTemplate();
	}
}

#undef LOCTEXT_NAMESPACE