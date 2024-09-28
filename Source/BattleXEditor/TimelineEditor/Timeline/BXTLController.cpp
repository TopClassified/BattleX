#include "BXTLController.h"
#include "FileHelpers.h"
#include "IDesktopPlatform.h"
#include "SourceControlHelpers.h"
#include "EditorUtilityLibrary.h"
#include "DesktopPlatformModule.h"
#include "Misc/FrameNumber.h"
#include "Misc/MessageDialog.h"
#include "Animation/AnimSequence.h"
#include "Preferences/PersonaOptions.h"
#include "HAL/PlatformApplicationMisc.h"
#include "Framework/Application/SlateApplication.h"

#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SEditableText.h"
#include "Widgets/Text/SInlineEditableTextBlock.h"

#include "TimelineTrack.h"
#include "SOutlinerItem.h"
#include "BXTLEditor.h"
#include "BXTLEditorDelegates.h"
#include "BXTLEditorSettings.h"
#include "BXTLTaskGroupTrack.h"
#include "BXTLTaskTrackPanel.h"
#include "BXTLExtraTrackPanel.h"
#include "Graph/BXTLGraph.h"
#include "Preview/BXTLPreviewProxy.h"
#include "Preview/BXTLPreviewScene.h"

#include "BXTask.h"
#include "BXTLAsset.h"
#include "BXSkillAsset.h"
#include "BXTLEditorTemplate.h"
#include "BXTLEditorUtilities.h" 



#define LOCTEXT_NAMESPACE "FBXTLController"

#pragma region Important
FBXTLController::FBXTLController()
{
	bIsSelecting = false;
}

FBXTLController::~FBXTLController()
{
	
}

void FBXTLController::Initialize(const TSharedPtr<FBXTLEditor>& InAssetEditorToolkit, TSharedPtr<FBXTLPreviewProxy> InPreviewProxy,  int32 InSectionIndex)
{
	CachedEditor = InAssetEditorToolkit;
	CachedPreviewProxy = InPreviewProxy;
	SectionIndex = InSectionIndex;

	SnapViewToPlayRange();
}

void FBXTLController::RefreshTracks()
{
	if (!CachedEditor.IsValid() || !CachedPreviewProxy.IsValid())
	{
		return;
	}

	UBXTLAsset* Asset = CachedEditor.Pin()->GetEditingAsset();
	if (!Asset)
	{
		return;
	}

	if (!Asset->Sections.IsValidIndex(SectionIndex))
	{
		return;
	}
	FBXTLSection& Section = Asset->Sections[SectionIndex];

	// 清除轨道选取信息
	ClearTrackSelection();
	
	// 记录轨道的展开信息
	TMap<int32, int32> GroupExpandStateMap;
	for (int32 i = 0; i < RootTracks.Num(); i++)
	{
		if (FTimelineTrack* Track = &RootTracks[i].Get())
		{
			GroupExpandStateMap.Add(i, Track->IsExpanded() ? 1 : 0);
		}
	}

	// 清除所有轨道
	RootTracks.Empty();

	// 创建额外轨道
	FText TrackName;
	FText TrackContext;
	for (TMap<EBXTLExtraType, FVector2f>::TConstIterator It(Section.ExtraTime); It; ++It)
	{
		if (It->Key == EBXTLExtraType::ET_Anticipation)
		{
			TrackName = FText::FromString(TEXT("Anticipation"));
			TrackContext = LOCTEXT("ExtraTrack", "Anticipation");
		}
		else if (It->Key == EBXTLExtraType::ET_Recovery)
		{
			TrackName = FText::FromString(TEXT("Recovery"));
			TrackContext = LOCTEXT("ExtraTrack", "Recovery");
		}
		else if (It->Key >= EBXTLExtraType::ET_ComboWindow1 && It->Key <= EBXTLExtraType::ET_ComboWindow8)
		{
			TrackName = FText::FromString(TEXT("ComboWindow"));
			TrackContext = LOCTEXT("ExtraTrack", "ComboWindow");
		}

		RootTracks.Add(MakeShareable(new FBXTLExtraTrackPanel(SharedThis(this), Asset, SectionIndex, It->Key, TrackName, TrackContext)));
	}

	// 重新创建各个轨道
	for (int32 i = 0; i < Section.Groups.Num(); ++i)
	{
		FBXTLTaskGroup& Group = Section.Groups[i];

		TSharedRef<FBXTLTaskGroupTrack> NewTrackGroup = MakeShareable(new FBXTLTaskGroupTrack(SharedThis(this), Group, Group.Name, LOCTEXT("TaskGroup_Tooltip", "Task Group")));

		// 恢复历史的展开状况
		if (GroupExpandStateMap.Contains(RootTracks.Num()))
		{
			NewTrackGroup->SetExpanded(*GroupExpandStateMap.Find(RootTracks.Num()) > 0);
		}
		// 无法恢复历史展开状况时，默认设置展开
		else
		{
			NewTrackGroup->SetExpanded(true);
		}

		for (int32 j = 0; j < Group.TaskList.Num(); ++j)
		{
			UBXTask* Task = Group.TaskList[j].Get();
			if (!IsValid(Task))
			{
				continue;
			}

			TSharedRef<FBXTLTaskTrackPanel> NewTrack = MakeShareable(new FBXTLTaskTrackPanel(SharedThis(this), Task, LOCTEXT("Task_Tooltip", "Task")));
			NewTrack->GenerateContainerWidgetForTimeline();
			NewTrackGroup->AddChild(NewTrack);
		}

		RootTracks.Add(NewTrackGroup);
	}

	TracksChangedEvent.Broadcast();
	
	SnapViewToPlayRange();
}

void FBXTLController::OnAssetPropertyChange(FName PropertyName)
{
	if (PropertyName.IsNone())
	{
		RefreshTracks();
	}
}

void FBXTLController::PostClearTrackSelection()
{
	CachedEditor.Pin()->SetTaskSelection(TArray<UBXTask*>{});
}

#pragma endregion Important



#pragma region Parameter
int32 FBXTLController::GetSectionID()
{
	return SectionIndex;
}

FFrameNumber FBXTLController::GetScrubPosition() const
{
	if (!CachedPreviewProxy.IsValid())
	{
		return FFrameNumber();
	}

	if (CachedPreviewProxy->IsStopped())
	{
		return FFrameNumber(FMath::RoundToInt(ScrubPosition * GetTickResolution()));
	}
	else
	{
		return FFrameNumber(FMath::RoundToInt(CachedPreviewProxy->GetCurrentTime(SectionIndex) * GetTickResolution()));
	}
}

void FBXTLController::SetScrubPosition(FFrameTime NewScrubPosition) const
{
	if (CachedPreviewProxy.IsValid())
	{
		if (!CachedPreviewProxy->IsStopped())
		{
			CachedPreviewProxy->Stop();
		}
		
		float PositionValue = static_cast<float>(NewScrubPosition.AsDecimal() / static_cast<double>(GetTickResolution()));
		PositionValue = FMath::Clamp(PositionValue, 0.0f, GetPlayLength());

		//CachedPreviewProxy->TimeLineClick(ScrubPosition, PositionValue);
		
		ScrubPosition = PositionValue;
	}
}

float FBXTLController::GetPlayLength() const
{
	if (UBXTLAsset* Asset = GetAsset())
	{
		if (Asset->Sections.IsValidIndex(SectionIndex))
		{
			const FBXTLSection& Section = Asset->Sections[SectionIndex];
			return Section.Duration;
		}
	}

	return 0.f;
}

double FBXTLController::GetFrameRate() const
{
	if (const UBXTLEditorSettings* Setting = GetDefault<UBXTLEditorSettings>())
	{
		return Setting->PreviewTickRate;
	}

	return 60.0f;
}

UBXTLAsset* FBXTLController::GetAsset() const
{
	return CachedPreviewProxy->GetPreviewAsset();
}

TWeakPtr<FBXTLEditor> FBXTLController::GetEditor() const
{
	return CachedEditor;
}

#pragma endregion Parameter



#pragma region Input
void FBXTLController::ChangeCtrlState(bool bPress)
{
	bPressCtrl = bPress;
}

#pragma endregion Input



#pragma region Task
FString FBXTLController::PasteHead = TEXT("BEGIN Copy UTask!\n");

void FBXTLController::ClearTaskSelection()
{
	CachedEditor.Pin()->SetTaskSelection(TArray<UBXTask*>{});
}

void FBXTLController::ChangeTaskSelection(const TArray<UBXTask*>& Tasks)
{
	CachedEditor.Pin()->SetTaskSelection(Tasks, !bPressCtrl);
}

void FBXTLController::ChangeTaskPosition(UBXTask* SrcTask, UBXTask* DestTask)
{
	if (!CachedEditor.Pin()->IsStopped())
	{
		return;
	}

	if (SrcTask == DestTask)
	{
		return;
	}

	UBXTLAsset* Asset = GetAsset();
	if (!Asset)
	{
		return;
	}

	if (!Asset->Sections.IsValidIndex(SectionIndex))
	{
		return;
	}
	FBXTLSection& Section = Asset->Sections[SectionIndex];

	int32 SrcGroupIndex = -1;
	int32 DestGroupIndex = -1;
	int32 DestIndex = -1;

	for (int32 GroupIndex = 0; GroupIndex < Section.Groups.Num(); ++GroupIndex)
	{
		FBXTLTaskGroup& Group = Section.Groups[GroupIndex];
		if (Group.TaskList.Contains(SrcTask))
		{
			SrcGroupIndex = GroupIndex;
			break;
		}
	}

	for (int32 GroupIndex = 0; GroupIndex < Section.Groups.Num(); ++GroupIndex)
	{
		FBXTLTaskGroup& Group = Section.Groups[GroupIndex];
		for (int32 TaskIndex = 0; TaskIndex < Group.TaskList.Num(); ++TaskIndex)
		{
			if (Group.TaskList[TaskIndex] == DestTask)
			{
				DestGroupIndex = GroupIndex;
				DestIndex = TaskIndex;
				break;
			}
		}
	}

	if (SrcGroupIndex < 0 || DestGroupIndex < 0)
	{
		return;
	}

	// 从原先的Group中删除，然后添加到新的Group中
	Section.Groups[SrcGroupIndex].TaskList.Remove(SrcTask);
	Section.Groups[DestGroupIndex].TaskList.Insert(SrcTask, DestIndex);

	RefreshTracks();

	Asset->GetPackage()->MarkPackageDirty();
}

int32 FBXTLController::GetTaskGroupIndex(UBXTask* TheTask) const
{
	int32 SectionID = -1, GroupID = -1;

	UBXTLAsset* Asset = GetAsset();
	if (!Asset)
	{
		return GroupID;
	}

	Asset->GetSectionIDAndGroupID(TheTask, SectionID, GroupID);

	return GroupID;
}

FBXTLTaskGroup* FBXTLController::GetTaskGroup(UBXTask* TheTask) const
{
	UBXTLAsset* Asset = GetAsset();
	if (!Asset)
	{
		return nullptr;
	}

	int32 SectionID = -1, GroupID = -1;
	if (Asset->GetSectionIDAndGroupID(TheTask, SectionID, GroupID))
	{
		return &(Asset->Sections[SectionID].Groups[GroupID]);
	}

	return nullptr;
}

void FBXTLController::ChangeTaskGroup(UBXTask* SrcTask, FBXTLTaskGroup& DestGroupData)
{
	if (!CachedEditor.Pin()->IsStopped())
	{
		return;
	}

	UBXTLAsset* Asset = GetAsset();
	if (!Asset)
	{
		return;
	}

	if (!Asset->Sections.IsValidIndex(SectionIndex))
	{
		return;
	}
	FBXTLSection& Section = Asset->Sections[SectionIndex];

	int32 SrcGroupIndex = -1;
	int32 DestGroupIndex = FindGroupID(DestGroupData);

	for (int32 GroupIndex = 0; GroupIndex < Section.Groups.Num(); ++GroupIndex)
	{
		if (Section.Groups[GroupIndex].TaskList.Contains(SrcTask))
		{
			SrcGroupIndex = GroupIndex;
			break;
		}
	}

	if (SrcGroupIndex < 0 || DestGroupIndex < 0)
	{
		return;
	}

	// 从原先的Group中删除，然后添加到新的Group中
	Section.Groups[SrcGroupIndex].TaskList.Remove(SrcTask);
	Section.Groups[DestGroupIndex].TaskList.Add(SrcTask);

	RefreshTracks();

	Asset->GetPackage()->MarkPackageDirty();
}

void FBXTLController::AddNewTaskGroup(const FText& Name)
{
	if (!CachedEditor.Pin()->IsStopped())
	{
		return;
	}

	if (UBXTLAsset* Asset = GetAsset())
	{
		Asset->AddGroup(SectionIndex, Name);

		RefreshTracks();

		Asset->GetPackage()->MarkPackageDirty();
	}
}

void FBXTLController::DeleteTaskGroup(FBXTLTaskGroup& InGroup)
{
	if (!CachedEditor.Pin()->IsStopped())
	{
		return;
	}

	UBXTLAsset* Asset = GetAsset();
	if (!Asset)
	{
		return;
	}

	if (!Asset->Sections.IsValidIndex(SectionIndex))
	{
		return;
	}
	FBXTLSection& Section = Asset->Sections[SectionIndex];

	int32 GroupID = FindGroupID(InGroup);
	if (GroupID >= 0)
	{
		Asset->RemoveGroup(SectionIndex, GroupID);

		RefreshTracks();

		Asset->GetPackage()->MarkPackageDirty();
	}
}

UBXTask* FBXTLController::AddNewTask(FBXTLTaskGroup& InGroup, UClass* InTaskClass, float StartTime)
{
	if (!CachedEditor.Pin()->IsStopped())
	{
		return nullptr;
	}

	UBXTLAsset* Asset = GetAsset();
	if (!Asset)
	{
		return nullptr;
	}

	if (UBXTask* NewTask = NewObject<UBXTask>(Asset, InTaskClass, NAME_None, RF_Transactional))
	{
		int32 GroupID = FindGroupID(InGroup);
		if (GroupID >= 0)
		{
			NewTask->StartTime = StartTime;

			Asset->AddTask(SectionIndex, GroupID, NewTask);

			RefreshTracks();

			Asset->GetPackage()->MarkPackageDirty();
		}

		return NewTask;
	}

	return nullptr;
}

void FBXTLController::CreateTaskGraphNode(UBXTask* TheTask)
{
	UBXTLAsset* Asset = GetAsset();
	if (!Asset)
	{
		return;
	}

	UBXTLGraph* Graph = Cast<UBXTLGraph>(Asset->Graph);
	if (!Graph)
	{
		return;
	}

	if (Graph->CheckTaskNodeValid(TheTask))
	{
		if (CachedEditor.IsValid())
		{
			CachedEditor.Pin()->SetGraphEditorViewLocationByTask(TheTask);
		}
	}
	else
	{
		Graph->GenerateGraphNodeByTask(TheTask, 0.0f, 0.0f);
	}
}

void FBXTLController::DeleteTask(UBXTask* TheTask)
{
	if (!CachedEditor.Pin()->IsStopped())
	{
		return;
	}

	UBXTLAsset* Asset = GetAsset();
	if (!Asset)
	{
		return;
	}

	if (UBXTLGraph* Graph = Cast<UBXTLGraph>(Asset->Graph))
	{
		Graph->DeleteGraphNodeByTask(TheTask);
	}

	int32 SectionID = -1, GroupID = -1;
	Asset->GetSectionIDAndGroupID(TheTask, SectionID, GroupID);
	Asset->RemoveTask(SectionID, GroupID, TheTask);

	RefreshTracks();

	Asset->GetPackage()->MarkPackageDirty();
}

void FBXTLController::DeleteSelectedTasks()
{
	if (!CachedEditor.Pin()->IsStopped())
	{
		return;
	}

	UBXTLAsset* Asset = GetAsset();
	if (!Asset)
	{
		return;
	}

	UBXTLGraph* Graph = Cast<UBXTLGraph>(Asset->Graph);

	int32 SectionID = -1, GroupID = -1;
	TArray<UBXTask*> Tasks = GetSelectedTasks();
	for (int32 i = 0; i < Tasks.Num(); ++i)
	{
		if (Graph)
		{
			Graph->DeleteGraphNodeByTask(Tasks[i]);
		}

		Asset->GetSectionIDAndGroupID(Tasks[i], SectionID, GroupID);
		Asset->RemoveTask(SectionID, GroupID, Tasks[i]);
	}

	RefreshTracks();

	Asset->GetPackage()->MarkPackageDirty();
}

int32 FBXTLController::FindGroupID(FBXTLTaskGroup& InGroup)
{
	int32 GroupID = -1;

	UBXTLAsset* Asset = GetAsset();
	if (!Asset)
	{
		return GroupID;
	}

	if (!Asset->Sections.IsValidIndex(SectionIndex))
	{
		return GroupID;
	}
	FBXTLSection& Section = Asset->Sections[SectionIndex];

	for (int32 i = 0; i < Section.Groups.Num(); ++i)
	{
		const FBXTLTaskGroup& Group = Section.Groups[i];
		if (&Group == &InGroup)
		{
			GroupID = i;
			break;
		}
	}

	return GroupID;
}

TArray<UBXTask*> FBXTLController::GetSelectedTasks()
{
	return CachedEditor.Pin()->GetTaskSelection();
}

void FBXTLController::CopySelectedTasks()
{
	if (!CachedEditor.Pin()->IsStopped())
	{
		return;
	}

	TArray<UBXTask*> Tasks = GetSelectedTasks();

	if (Tasks.Num() <= 0)
	{
		FText DialogText = LOCTEXT("提示", "请先选中Task再复制");
		FMessageDialog::Open(EAppMsgType::Ok, DialogText);
		return;
	}
	
	Tasks.Sort
	(
		[this](const UBXTask& A, const UBXTask& B) 
		{
			if (UBXTLAsset* Asset = GetAsset())
			{
				if (Asset->Sections.IsValidIndex(SectionIndex))
				{
					FBXTLSection& Section = Asset->Sections[SectionIndex];

					int32 IndexA = -1;
					int32 IndexB = -1;

					for (int32 i = 0; i < Section.TaskList.Num(); ++i)
					{
						if (Section.TaskList[i] == &A)
						{
							IndexA = i;
							break;
						}
					}

					for (int32 i = 0; i < Section.TaskList.Num(); ++i)
					{
						if (Section.TaskList[i] == &B)
						{
							IndexB = i;
							break;
						}
					}

					return IndexA < IndexB;
				}
			}

			return A.StartTime < B.StartTime;
		}
	);

	FString CopyString;
	CopyString += TEXT("BEGIN Copy UTask!\n");
	for (int32 i = 0; i < Tasks.Num(); ++i)
	{
		if (Tasks[i])
		{
			TSoftObjectPtr<UBXTask> ObjectPath(Tasks[i]);
			CopyString += ObjectPath.ToString() + TEXT("!!!!");
		}
	}

	FPlatformApplicationMisc::ClipboardCopy(*CopyString);
}

void FBXTLController::PasteSelectedTasks(FString InPasteMsg, int32 InGroupID)
{
	if (!InPasteMsg.Contains(FBXTLController::PasteHead))
	{
		FText DialogText = LOCTEXT("提示", "目前没有复制任何Task");
		FMessageDialog::Open(EAppMsgType::Ok, DialogText);
		return;
	}
	InPasteMsg = InPasteMsg.Replace(*FBXTLController::PasteHead, TEXT(""));

	if (!CachedEditor.Pin()->IsStopped())
	{
		return;
	}

	UBXTLAsset* Asset = GetAsset();
	if (!Asset)
	{
		return;
	}

	if (!Asset->Sections.IsValidIndex(SectionIndex))
	{
		return;
	}
	FBXTLSection& Section = Asset->Sections[SectionIndex];

	TArray<UBXTask*> OldTasks;
	TArray<FString> AllPaths;
	InPasteMsg.ParseIntoArray(AllPaths, TEXT("!!!!"));
	for (int32 i = 0; i < AllPaths.Num(); ++i)
	{
		TSoftObjectPtr<UBXTask> ObjectPath(AllPaths[i]);

		if (ObjectPath.IsNull())
		{
			continue;
		}

		UBXTask* OldTask = Cast<UBXTask>(ObjectPath.LoadSynchronous());
		if (OldTask)
		{
			OldTasks.Add(OldTask);
		}
	}

	if (Section.Groups.Num() > 0)
	{
		if (!Section.Groups.IsValidIndex(InGroupID))
		{
			InGroupID = Section.Groups.Num() - 1;
		}

		// 获取新建TaskGroup的地址
		FBXTLTaskGroup& Group = Section.Groups[InGroupID];

		// 复制所选的Tasks
		TArray<UBXTask*> NewTasks;
		for (UBXTask* OldTask : OldTasks)
		{
			UBXTask* NewTask = AddNewTask(Group, OldTask->GetClass(), 0.0f);
			NewTask->CopyDataFromOther(OldTask);
			NewTasks.Add(NewTask);
		}

		// 更新Task之间的关系
		FBXTLEditorUtilities::RestoreTasksRelation(NewTasks, OldTasks);

		// 对更新了LogicPin关系的节点，要生成LogicGraph的对应节点
		CachedEditor.Pin()->GenerateGraphNodes(NewTasks);

		RefreshTracks();

		Asset->GetPackage()->MarkPackageDirty();
	}
}

void FBXTLController::ExportSelectedTaskTemplate()
{
	if (GetSelectedTasks().Num() <= 0)
	{
		FText DialogText = LOCTEXT("提示", "请先选中Task");
		FMessageDialog::Open(EAppMsgType::Ok, DialogText);
		return;
	}

	//创建模板命名弹窗
	TemplateNameWindow = SNew(SWindow)
		.AutoCenter(EAutoCenter::PreferredWorkArea)
		.Title(LOCTEXT("TaskTemplateNameTitle", "设置TaskTemplate名称"))
		.SizingRule(ESizingRule::FixedSize)
		.SupportsMaximize(false)
		.SupportsMinimize(false).HasCloseButton(true)
		.ClientSize(FVector2D(350, 120))
		.CreateTitleBar(true);
	
	//创建弹窗内容
	TSharedPtr<SWidget> content = SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SBox)
			.HeightOverride(90)
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.Padding(0.0f, 12.0f, 0.0f, 12.0f)
				.AutoHeight()
				[
					SNew(SEditableText)	
					.HintText(LOCTEXT("TaskTemplateNameInputHint", "请输入TaskTemplate名称"))
					.OnTextChanged_Lambda([this](const FText& InText) { TemplateName = InText;})
					.Font(FCoreStyle::GetDefaultFontStyle("Regular", 20))
				]

				+ SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.AutoHeight()
				[
					SNew(SButton)
					.Text(LOCTEXT("TaskTemplateNameConfirm", "确认"))
					.OnClicked_Lambda
					(
						[this]()
						{
							if (TemplateName.IsEmpty())
							{
								FText DialogText = LOCTEXT("空Task模板错误", "请勿设置TaskTemplate名称为空...");
								FMessageDialog::Open(EAppMsgType::Ok, DialogText);
							}
							else
							{
								InternalExportSelectedTaskTemplate();
							}

							TemplateNameWindow.ToSharedRef().Get().RequestDestroyWindow();

							return FReply::Handled();
						}
					)
				]
			]
		];

	//将弹窗内容添加到弹框中
	TemplateNameWindow->SetContent(content.ToSharedRef());

	//显示弹窗
	FSlateApplication::Get().AddWindow(TemplateNameWindow.ToSharedRef());
}

void FBXTLController::InternalExportSelectedTaskTemplate()
{
	TArray<UBXTask*> Tasks = GetSelectedTasks();

	UBXTLAsset* Asset = GetAsset();
	if (!Asset)
	{
		return;
	}

	const UBXTLEditorSettings* Setting = GetDefault<UBXTLEditorSettings>();
	if (!Setting)
	{
		return;
	}

	UBXTLTaskGroupTemplate* Template = Setting->Template.LoadSynchronous();
	if (!Template)
	{
		return;
	}

	// 避免出现同名模板
	for (FBXTLTaskGroupInformation Group : Template->Groups)
	{
		if (Group.Name == FName(TemplateName.ToString()))
		{
			FText DialogText = LOCTEXT("同名Task模板错误", "存在同名Task模板...");
			FMessageDialog::Open(EAppMsgType::Ok, DialogText);
			return;
		}
	}

	// 新建TaskGroup
	FBXTLTaskGroupInformation Group = FBXTLTaskGroupInformation(FName(TemplateName.ToString()));

	// 复制所选的Tasks
	for (UBXTask* Task : Tasks)
	{
		UBXTask* NewTask = NewObject<UBXTask>(Template, Task->GetClass());
		NewTask->CopyDataFromOther(Task);
		Group.TaskList.Add(NewTask);
	}

	// 更新Task之间的关系
	FBXTLEditorUtilities::RestoreTasksRelation(Group.TaskList, Tasks);

	// 添加至TaskTemplate
	Template->Groups.Add(Group);

	// Save
	Template->MarkPackageDirty();
	TArray<UPackage*> PackagesToSave;
	PackagesToSave.AddUnique(Template->GetOutermost());
	FEditorFileUtils::PromptForCheckoutAndSave(PackagesToSave, true, true);

	//生成原文件路径
	FString DefaultLocation = FPaths::GetPath(Template->GetPathName()); 
	DefaultLocation.Split(TEXT("/Game/"), nullptr, &DefaultLocation);
	DefaultLocation = FPaths::ProjectContentDir() + DefaultLocation + TEXT("/") + Template->GetName() + TEXT(".uasset");

	FText ErrorMessage;
	SourceControlHelpers::CheckoutOrMarkForAdd(DefaultLocation, FText::FromString(DefaultLocation), NULL, ErrorMessage);
}

void FBXTLController::ImportTaskTemplate(FName InTemplateName, int32 InGroupID)
{
	if (!CachedEditor.Pin()->IsStopped())
	{
		return;
	}

	UBXTLAsset* Asset = GetAsset();
	if (!Asset)
	{
		return;
	}

	const UBXTLEditorSettings* Setting = GetDefault<UBXTLEditorSettings>();
	if (!Setting)
	{
		return;
	}

	UBXTLTaskGroupTemplate* Template = Setting->Template.LoadSynchronous();
	if (!Template)
	{
		return;
	}

	if (!Asset->Sections.IsValidIndex(SectionIndex))
	{
		return;
	}
	FBXTLSection& Section = Asset->Sections[SectionIndex];

	// 找到Template中的同名模板
	for (FBXTLTaskGroupInformation Group : Template->Groups)
	{
		if (Group.Name == FName(InTemplateName.ToString()))
		{
			if (!Section.Groups.IsValidIndex(InGroupID))
			{
				InGroupID = Section.Groups.Num() - 1;
			}

			FBXTLTaskGroup& TaskGoup = Section.Groups[InGroupID];

			// 复制所选的Tasks
			TArray<UBXTask*> CoppiedTasks;
			for (TWeakObjectPtr<UBXTask> OriginTask : Group.TaskList)
			{
				UBXTask* NewTask = AddNewTask(TaskGoup, OriginTask->GetClass(), 0.0f);
				NewTask->CopyDataFromOther(OriginTask.Get());
				CoppiedTasks.Add(NewTask);
			}

			// 更新Task之间的关系
			FBXTLEditorUtilities::RestoreTasksRelation(CoppiedTasks, Group.TaskList);

			// 对更新了LogicPin关系的节点，要生成LogicGraph的对应节点
			CachedEditor.Pin()->GenerateGraphNodes(CoppiedTasks);

			RefreshTracks();

			break;
		}
	}
}

#pragma endregion Task



#pragma region Callback
void FBXTLController::ReceiveAutoOptimizeDone(UBXTLAsset* Asset)
{
	if (GetAsset() == Asset)
	{
		RefreshTracks();
	}
}
#pragma endregion Callback

#undef LOCTEXT_NAMESPACE