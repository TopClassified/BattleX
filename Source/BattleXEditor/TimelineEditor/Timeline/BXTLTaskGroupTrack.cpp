#include "BXTLTaskGroupTrack.h"
#include "ScopedTransaction.h"
#include "HAL/PlatformApplicationMisc.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"

#include "Widgets/SBoxPanel.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Text/SInlineEditableTextBlock.h"

#include "BXTLController.h"
#include "BXTLEditorUtilities.h"
#include "SOutlinerItem.h"
#include "BXTLTaskTrackPanel.h"
#include "SBXTLTaskGroupTrackOutliner.h"
#include "SBXTLTaskGroupTemplatePicker.h" 



#define LOCTEXT_NAMESPACE "FBXTLTaskGroupTrack"

TIMELINE_IMPLEMENT_TRACK(FBXTLTaskGroupTrack);

#pragma region Important
FBXTLTaskGroupTrack::FBXTLTaskGroupTrack
(
	const TSharedRef<FBXTLController>& InController, FBXTLTaskGroup& InGroupData,
	const FText& InDisplayName, const FText& InToolTipText
) : FTimelineTrack(InController, InDisplayName, InToolTipText, true)
{
	// 构造时指针有效,解析出组索引供后续按需定位(数组重排后索引可能漂移,
	// 但悬垂指针读写已释放内存的风险远小于索引漂移:漂移表现为操作错组,可通过RefreshTracks重建轨道收敛)
	if (const FBXTLController* TC = &InController.Get())
	{
		if (UBXTLAsset* Asset = TC->GetAsset())
		{
			const int32 SectionID = TC->GetSectionID();
			if (Asset->Sections.IsValidIndex(SectionID))
			{
				const TArray<FBXTLTaskGroup>& Groups = Asset->Sections[SectionID].Groups;
				for (int32 i = 0; i < Groups.Num(); ++i)
				{
					if (&Groups[i] == &InGroupData)
					{
						GroupID = i;
						break;
					}
				}
			}
		}
	}

	SetHeight(30.0f);
}

TSharedRef<SWidget> FBXTLTaskGroupTrack::GenerateContainerWidgetForOutliner(const TSharedRef<SOutlinerItem>& InRow)
{
	TSharedPtr<SBorder> OuterBorder;
	TSharedPtr<SHorizontalBox> InnerHorizontalBox;
	TSharedRef<SWidget> OutlinerWidget = GenerateStandardOutlinerWidget(InRow, false, OuterBorder, InnerHorizontalBox);

	OuterBorder->SetBorderBackgroundColor(FAppStyle::GetColor("AnimTimeline.Outliner.HeaderColor"));

	// Group的名称
	InnerHorizontalBox->AddSlot()
	.VAlign(VAlign_Center)
	.HAlign(HAlign_Left)
	.Padding(2.0f, 1.0f)
	.FillWidth(1.0f)
	[
		SNew(SInlineEditableTextBlock)
		.IsReadOnly(false)
		.Text(this, &FBXTLTaskGroupTrack::GetLabel)
		.IsSelected(FIsSelected::CreateLambda([]() { return false; }))
		.OnTextCommitted(this, &FBXTLTaskGroupTrack::OnCommitName)
		.HighlightText(InRow->GetHighlightText())
	];

	// Group的下拉菜单
	InnerHorizontalBox->AddSlot()
	.AutoWidth()
	.HAlign(HAlign_Right)
	.VAlign(VAlign_Center)
	.Padding(OutlinerRightPadding, 1.0f)
	[
		FBXTLEditorUtilities::MakeTrackButton(LOCTEXT("BXTLTaskGroupTrack", "Group Menu"),FOnGetContent::CreateSP(this, &FBXTLTaskGroupTrack::OnBuildSubMenu))
	];

	TSharedRef<SWidget> Outliner = SNew(SBXTLTaskGroupTrackOutliner, GetEditorTimelineController(), GroupID).MainWidget(OutlinerWidget);

	return Outliner;
}

bool FBXTLTaskGroupTrack::CanRename() const
{
	return true;
}

FText FBXTLTaskGroupTrack::GetLabel() const
{
	// 绘制路径每帧调用,按索引解析防悬垂(组被删时返回空文本)
	if (const FBXTLTaskGroup* Group = GetGroupData())
	{
		return Group->Name;
	}

	return FText::GetEmpty();
}

int32 FBXTLTaskGroupTrack::GetGroupIndex() const
{
	return GroupID;
}

FBXTLTaskGroup* FBXTLTaskGroupTrack::GetGroupData() const
{
	FBXTLController* TC = StaticCast<FBXTLController*>(TimelineController.Pin().Get());
	if (!TC)
	{
		return nullptr;
	}

	UBXTLAsset* Asset = TC->GetAsset();
	const int32 SectionID = TC->GetSectionID();
	if (!Asset || !Asset->Sections.IsValidIndex(SectionID))
	{
		return nullptr;
	}

	TArray<FBXTLTaskGroup>& Groups = Asset->Sections[SectionID].Groups;
	return Groups.IsValidIndex(GroupID) ? &Groups[GroupID] : nullptr;
}

#pragma endregion Important



#pragma region Callback
void FBXTLTaskGroupTrack::OnCommitName(const FText& InText, ETextCommit::Type CommitInfo)
{
	// 写路径按索引解析(组被删后改名静默跳过,原悬垂指针写已释放内存)
	if (FBXTLTaskGroup* Group = GetGroupData())
	{
		Group->Name = InText;
	}
}

TSharedRef<SWidget> FBXTLTaskGroupTrack::OnBuildSubMenu()
{
	FMenuBuilder MenuBuilder(true, nullptr);

	MenuBuilder.BeginSection("Group Actions", LOCTEXT("NotifiesMenuSection", "Group"));
	{
		MenuBuilder.AddMenuEntry
		(
			LOCTEXT("Action_NewGroup_Lable", "New Group"),
			LOCTEXT("Action_NewGroup_Tooltip", "Add a new task group"),
			FSlateIcon(),
			FUIAction(FExecuteAction::CreateSP(this, &FBXTLTaskGroupTrack::OnAddGroup)),
			NAME_None,
			EUserInterfaceActionType::Button
		);

		MenuBuilder.AddMenuEntry
		(
			LOCTEXT("Action_DeleteGroup_Lable", "Delete Group"),
			LOCTEXT("Action_DeleteGroup_Tooltip", "Delete group"),
			FSlateIcon(),
			FUIAction(FExecuteAction::CreateSP(this, &FBXTLTaskGroupTrack::OnDeleteGroup)),
			NAME_None,
			EUserInterfaceActionType::Button
		);
	}
	MenuBuilder.EndSection();

	MenuBuilder.BeginSection("Track Actions", LOCTEXT("NotifiesMenuSection", "Tracks"));
	{
		MenuBuilder.AddSubMenu
		(
			LOCTEXT("Action_AddTaskTrack_Lable", "Add Task Track"),
			LOCTEXT("Action_AddTaskTrack_Tooltip", "Add A New Task Track"),
			FNewMenuDelegate::CreateRaw(this, &FBXTLTaskGroupTrack::OnFillTaskMenu)
		);
		MenuBuilder.AddMenuEntry
		(
			LOCTEXT("Action_PasteTasks_Lable", "Paste Tasks"),
			LOCTEXT("Action_PasteTasks_Tooltip", "Paste Tasks"),
			FSlateIcon(),
			FUIAction(FExecuteAction::CreateSP(this, &FBXTLTaskGroupTrack::OnPasteTasks)),
			NAME_None,
			EUserInterfaceActionType::Button
		);
		MenuBuilder.AddSubMenu
		(
			LOCTEXT("Action_AddTaskTemplate_Lable", "Add Task Template"),
			LOCTEXT("Action_AddTaskTemplate_Tooltip", "Add A New Task Template"),
			FNewMenuDelegate::CreateRaw(this, &FBXTLTaskGroupTrack::OnFillTaskTemplateMenu)
		);
	}
	MenuBuilder.EndSection();

	return MenuBuilder.MakeWidget();
}

void FBXTLTaskGroupTrack::OnFillTaskMenu(class FMenuBuilder& MenuBuilder)
{
	FBXTLEditorUtilities::MakeNewTaskPicker(MenuBuilder, FOnClassPicked::CreateSP(this, &FBXTLTaskGroupTrack::OnAddTask));
}

void FBXTLTaskGroupTrack::OnFillTaskTemplateMenu(class FMenuBuilder& MenuBuilder)
{
	FBXTLEditorUtilities::MakeNewTaskTemplatePicker(MenuBuilder, FAddTaskGroupTemplateDelegate::CreateSP(this, &FBXTLTaskGroupTrack::OnAddTaskTemplate));
}

void FBXTLTaskGroupTrack::OnAddTask(UClass* InTaskClass)
{
	if (FBXTLController* TC = StaticCast<FBXTLController*>(TimelineController.Pin().Get()))
	{
		if (FBXTLTaskGroup* Group = GetGroupData())
		{
			TC->AddNewTask(*Group, InTaskClass);
		}
	}
}

void FBXTLTaskGroupTrack::OnPasteTasks()
{
	if (FBXTLController* TC = StaticCast<FBXTLController*>(TimelineController.Pin().Get()))
	{
		FString PasteString;
		FPlatformApplicationMisc::ClipboardPaste(PasteString);

		TC->PasteSelectedTasks(PasteString, GetGroupIndex());
	}
}

void FBXTLTaskGroupTrack::OnAddGroup()
{
	if (FBXTLController* TC = StaticCast<FBXTLController*>(TimelineController.Pin().Get()))
	{
		TC->AddNewTaskGroup(FText());
	}
}

void FBXTLTaskGroupTrack::OnDeleteGroup()
{
	if (FBXTLController* TC = StaticCast<FBXTLController*>(TimelineController.Pin().Get()))
	{
		if (FBXTLTaskGroup* Group = GetGroupData())
		{
			TC->DeleteTaskGroup(*Group);
		}
	}
}

void FBXTLTaskGroupTrack::OnAddTaskTemplate(FName TemplateName)
{
	if (FBXTLController* TC = StaticCast<FBXTLController*>(TimelineController.Pin().Get()))
	{
		TC->ImportTaskTemplate(TemplateName, GetGroupIndex());
	}
}

#pragma endregion Callback

#undef LOCTEXT_NAMESPACE