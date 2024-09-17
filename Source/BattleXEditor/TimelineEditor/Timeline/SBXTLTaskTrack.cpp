#include "SBXTLTaskTrack.h"
#include "Widgets/Input/SNumericEntryBox.h"
#include "Framework/Application/SlateApplication.h"

#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SBorder.h"
#include "BXTLDragDropOp.h"
#include "BXTLTaskTrackPanel.h"

#include "BXTLEditorUtilities.h"



#define LOCTEXT_NAMESPACE "SBXTLTaskTrack"

#pragma region Important
void SBXTLTaskTrack::Construct(const FArguments& InArgs)
{
	SetClipping(EWidgetClipping::ClipToBounds);

	CachedTask = InArgs._Task;
	Controller = InArgs._Controller;
	ViewInputMin = InArgs._ViewInputMin;
	ViewInputMax = InArgs._ViewInputMax;
	InputMin = InArgs._InputMin;
	InputMax = InArgs._InputMax;
	TrackIndex = InArgs._TrackIndex;
	TimelinePlayLength = InArgs._TimelinePlayLength.Get();
	SetInputViewRangeEvent = InArgs._OnSetInputViewRange;
	RefreshPanelEvent = InArgs._OnRefreshPanel;
	SelectNodeEvent = InArgs._OnSelectNode;
	DeselectNodesEvent = InArgs._OnDeselectAllNodes;
	DeleteTaskEvent = InArgs._OnDeleteTask;
	AddTaskEvent = InArgs._OnAddNewTask;
	CopyTaskEvent = InArgs._OnCopyTasks;
	PasteTaskEvent = InArgs._OnPasteTasks;
	ExportTemplateEvent = InArgs._OnExportTemplate;

	this->ChildSlot
	[
		SAssignNew(TrackArea, SBorder)
		.Visibility(EVisibility::SelfHitTestInvisible)
		.BorderImage(FAppStyle::GetBrush("NoBorder"))
		.Padding(FMargin(0.f, 0.f))
		.ColorAndOpacity(FLinearColor::White)
	];

	UpdateLayout();
}

void SBXTLTaskTrack::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{ 
	UpdateCachedGeometry(AllottedGeometry); 
}

int32 SBXTLTaskTrack::GetTrackIndex() const
{ 
	return TrackIndex; 
}

#pragma endregion Important



#pragma region Render
int32 SBXTLTaskTrack::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	const FSlateBrush* StyleInfo = FAppStyle::GetBrush(TEXT("Persona.NotifyEditor.NotifyTrackBackground"));
	FLinearColor Color = TrackColor.Get();

	FPaintGeometry MyGeometry = AllottedGeometry.ToPaintGeometry();

	int32 CustomLayerId = LayerId + 1;
	FTrackScaleInfo ScaleInfo(ViewInputMin.Get(), ViewInputMax.Get(), 0.f, 0.f, AllottedGeometry.Size);

	FSlateDrawElement::MakeLines
	(
		OutDrawElements, CustomLayerId, AllottedGeometry.ToPaintGeometry(),
		TArray<FVector2D>({ FVector2D(0.0f, AllottedGeometry.GetLocalSize().Y), FVector2D(AllottedGeometry.GetLocalSize().X, AllottedGeometry.GetLocalSize().Y) }),
		ESlateDrawEffect::None, FLinearColor(0.1f, 0.1f, 0.1f, 0.3f)
	);

	++CustomLayerId;

	if (!TaskNode->BeingDragged())
	{
		TaskNode->UpdateSizeAndPosition(AllottedGeometry);
	}
	else 
	{
		float Value = CalculateDraggedNodePos();
		if (Value >= 0.0f)
		{
			float XPos = Value;
			TArray<FVector2D> LinePoints;
			LinePoints.Add(FVector2D(XPos, 0.f));
			LinePoints.Add(FVector2D(XPos, AllottedGeometry.Size.Y));

			FSlateDrawElement::MakeLines
			(
				OutDrawElements, CustomLayerId, MyGeometry, LinePoints, 
				ESlateDrawEffect::None, FLinearColor(1.0f, 0.5f, 0.0f)
			);
		}
	}

	return SCompoundWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, CustomLayerId, InWidgetStyle, bParentEnabled);
}

void SBXTLTaskTrack::UpdateLayout()
{
	TrackArea->SetContent
	(
		SAssignNew(NodeSlots, SOverlay)
	);

	SAssignNew(TaskNode, SBXTLTaskTrackNode)
	.Task(CachedTask.Get())
	.Controller(Controller)
	.ViewInputMin(ViewInputMin)
	.ViewInputMax(ViewInputMax)
	.TimelinePlayLength(TimelinePlayLength)
	.OnRefreshPanel(RefreshPanelEvent)
	.OnStartDragTTN(this, &SBXTLTaskTrack::OnNotifyNodeDragStarted, 0);

	NodeSlots->AddSlot()
	.Padding(TAttribute<FMargin>::Create(TAttribute<FMargin>::FGetter::CreateSP(this, &SBXTLTaskTrack::GetNotifyTrackPadding)))
	[
		TaskNode->AsShared()
	];
}

#pragma endregion Render



#pragma region Menu
TSharedPtr<SWidget> SBXTLTaskTrack::SummonContextMenu(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	FVector2D CursorPos = MouseEvent.GetScreenSpacePosition();
	LastClickedTime = CalculateTime(MyGeometry, MouseEvent.GetScreenSpacePosition());

	FMenuBuilder MenuBuilder(true, nullptr);
	MenuBuilder.BeginSection("Track", LOCTEXT("NotifyHeading", "Track Actions"));
	{
		MenuBuilder.AddSubMenu
		(
			NSLOCTEXT("AddTaskSubMenu", "AddTaskSubMenuAddNotify", "Add Task"),
			NSLOCTEXT("AddTaskSubMenu", "AddTaskSubMenuAddNotifyToolTip", "Add Task"),
			FNewMenuDelegate::CreateRaw(this, &SBXTLTaskTrack::AddNewTaskMenu)
		);
		MenuBuilder.AddMenuEntry
		(
			NSLOCTEXT("DeleteTaskSubMenu", "DeleteTaskSubMenuAddNotify", "Delete Tasks"),
			NSLOCTEXT("DeleteTaskSubMenu", "DeleteTaskSubMenuAddNotifyToolTip", "Delete Tasks"),
			FSlateIcon(),
			FUIAction
			(
				FExecuteAction::CreateSP(this, &SBXTLTaskTrack::DeleteSelectedTask)
			),
			NAME_None,
			EUserInterfaceActionType::Button
		);
		MenuBuilder.AddMenuEntry
		(
			NSLOCTEXT("CopyTaskSubMenu", "CopyTaskSubMenuAddNotify", "Copy Tasks"),
			NSLOCTEXT("CopyTaskSubMenu", "CopyTaskSubMenuAddNotifyToolTip", "Copy Selected Tasks"),
			FSlateIcon(),
			FUIAction
			(
				FExecuteAction::CreateSP(this, &SBXTLTaskTrack::CopyTaskMenu)
			),
			NAME_None,
			EUserInterfaceActionType::Button
		);
		MenuBuilder.AddMenuEntry
		(
			NSLOCTEXT("PasteTaskSubMenu", "PasteTaskSubMenuAddNotify", "Paste Tasks"),
			NSLOCTEXT("PasteTaskSubMenu", "PasteTaskSubMenuAddNotifyToolTip", "Paste Selected Tasks into a New Group"),
			FSlateIcon(),
			FUIAction
			(
				FExecuteAction::CreateSP(this, &SBXTLTaskTrack::PasteTaskMenu)
			),
			NAME_None,
			EUserInterfaceActionType::Button
		);
		MenuBuilder.AddMenuEntry
		(
			NSLOCTEXT("ExportTaskTemplateSubMenu", "ExportTaskTemplateSubMenuAddNotify", "Export Tasks To Template"),
			NSLOCTEXT("ExportTaskTemplateSubMenu", "ExportTaskTemplateSubMenuAddNotifyToolTip", "Export Selected Tasks To Template"),
			FSlateIcon(),
			FUIAction
			(
				FExecuteAction::CreateSP(this, &SBXTLTaskTrack::ExportTemplateMenu)
			),
			NAME_None,
			EUserInterfaceActionType::Button
		);
	}
	MenuBuilder.EndSection();

	FWidgetPath WidgetPath = MouseEvent.GetEventPath() != nullptr ? *MouseEvent.GetEventPath() : FWidgetPath();
	FSlateApplication::Get().PushMenu(SharedThis(this), WidgetPath, MenuBuilder.MakeWidget(), CursorPos, FPopupTransitionEffect(FPopupTransitionEffect::ContextMenu));

	return TSharedPtr<SWidget>();
}

void SBXTLTaskTrack::AddNewTaskMenu(FMenuBuilder& MenuBuilder)
{
	FBXTLEditorUtilities::MakeNewTaskPicker(MenuBuilder, FOnClassPicked::CreateSP(this, &SBXTLTaskTrack::CreateNewTaskNodeAtCursor));
}

void SBXTLTaskTrack::CreateNewTaskNodeAtCursor(UClass* NotifyClass)
{
	FSlateApplication::Get().DismissAllMenus();

	AddTaskEvent.ExecuteIfBound(NotifyClass, LastClickedTime);
}

void SBXTLTaskTrack::CopyTaskMenu()
{
	CopyTaskEvent.ExecuteIfBound();
}

void SBXTLTaskTrack::PasteTaskMenu()
{
	PasteTaskEvent.ExecuteIfBound();
}

void SBXTLTaskTrack::ExportTemplateMenu()
{
	ExportTemplateEvent.ExecuteIfBound();
}

#pragma endregion Menu



#pragma region Widget
float SBXTLTaskTrack::GetLastClickedTime() const
{ 
	return LastClickedTime; 
}

const FGeometry& SBXTLTaskTrack::GetCachedGeometry() const
{ 
	return CachedGeometry; 
}

void SBXTLTaskTrack::UpdateCachedGeometry(const FGeometry& InGeometry)
{
	CachedGeometry = InGeometry;

	TaskNode->CachedTrackGeometry = InGeometry;
}

FTrackScaleInfo SBXTLTaskTrack::GetCachedScaleInfo() const
{ 
	return FTrackScaleInfo(ViewInputMin.Get(), ViewInputMax.Get(), 0.f, 0.f, CachedGeometry.GetLocalSize()); 
}

FVector2D SBXTLTaskTrack::ComputeDesiredSize(float) const
{
	FVector2D Size;
	Size.X = 200;
	Size.Y = FBXTLTaskTrackPanel::NotificationTrackHeight;
	return Size;
}

bool SBXTLTaskTrack::SupportsKeyboardFocus() const
{
	return true;
}

FReply SBXTLTaskTrack::OnKeyUp(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == EKeys::LeftControl || InKeyEvent.GetKey() == EKeys::RightControl)
	{
		// 不阻碍Ctrl多选逻辑
		bIsCtrlDown = false;
		return FReply::Unhandled();
	}

	return FReply::Unhandled();
}

FReply SBXTLTaskTrack::OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == EKeys::LeftControl || InKeyEvent.GetKey() == EKeys::RightControl)
	{
		// 不阻碍Ctrl多选逻辑
		bIsCtrlDown = true;
		return FReply::Unhandled();
	}

	if (InKeyEvent.GetKey() == EKeys::Platform_Delete)
	{
		OnDeleteKeyPressed();
		return FReply::Handled();
	}

	if (bIsCtrlDown && InKeyEvent.GetKey() == EKeys::C)
	{
		CopyTaskMenu();
		return FReply::Handled();
	}

	return FReply::Unhandled();
}

FReply SBXTLTaskTrack::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	FVector2D CursorPos = MouseEvent.GetScreenSpacePosition();
	CursorPos = MyGeometry.AbsoluteToLocal(CursorPos);

	if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		TaskNode->SetLastMouseDownPosition(CursorPos);

		return FReply::Handled().DetectDrag(TaskNode.ToSharedRef(), EKeys::LeftMouseButton);
	}
	else if (MouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		return FReply::Handled();
	}

	return FReply::Handled();
}

FReply SBXTLTaskTrack::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	bool bLeftMouseButton = MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton;
	bool bRightMouseButton = MouseEvent.GetEffectingButton() == EKeys::RightMouseButton;

	if (bRightMouseButton)
	{
		TSharedPtr<SWidget> WidgetToFocus;
		WidgetToFocus = SummonContextMenu(MyGeometry, MouseEvent);

		return (WidgetToFocus.IsValid())
			? FReply::Handled().ReleaseMouseCapture().SetUserFocus(WidgetToFocus.ToSharedRef(), EFocusCause::SetDirectly)
			: FReply::Handled().ReleaseMouseCapture();
	}
	else if (bLeftMouseButton)
	{
		SelectTaskNode();

		FVector2D CursorPos = MouseEvent.GetScreenSpacePosition();
		CursorPos = MyGeometry.AbsoluteToLocal(CursorPos);

		LastClickedTime = CalculateTime(MyGeometry, MouseEvent.GetScreenSpacePosition());

		return FReply::Handled();
	}

	return FReply::Unhandled();
}

FCursorReply SBXTLTaskTrack::OnCursorQuery(const FGeometry& MyGeometry, const FPointerEvent& CursorEvent) const
{
	return FCursorReply::Unhandled();
}

void SBXTLTaskTrack::HandleNodeDrop(TSharedPtr<SBXTLTaskTrackNode> Node, float Offset)
{
	float LocalX = GetCachedGeometry().AbsoluteToLocal(Node->GetScreenPosition() + Offset).X;
	float Time = GetCachedScaleInfo().LocalXToInput(LocalX);

	Node->GetTaskNodeDataPtr()->SetStartTime(Time);
}

void SBXTLTaskTrack::SelectTaskNode()
{
	SelectNodeEvent.ExecuteIfBound();
}

void SBXTLTaskTrack::DeselectAllTaskNodes()
{
	DeselectNodesEvent.ExecuteIfBound();
}

void SBXTLTaskTrack::DeleteSelectedTask()
{
	DeleteTaskEvent.ExecuteIfBound();

	DeselectAllTaskNodes();

	UpdateLayout();
}

void SBXTLTaskTrack::OnDeleteKeyPressed()
{
	if (HasKeyboardFocus() || HasFocusedDescendants())
	{
		DeleteSelectedTask();
	}
}

float SBXTLTaskTrack::CalculateTime(const FGeometry& MyGeometry, FVector2D NodePos, bool bInputIsAbsolute)
{
	if (bInputIsAbsolute)
	{
		NodePos = MyGeometry.AbsoluteToLocal(NodePos);
	}

	FTrackScaleInfo ScaleInfo(ViewInputMin.Get(), ViewInputMax.Get(), 0.0f, 0.0f, MyGeometry.Size);
	return FMath::Clamp<float>(ScaleInfo.LocalXToInput(NodePos.X), 0.0f, TimelinePlayLength);
}

FMargin SBXTLTaskTrack::GetNotifyTrackPadding() const
{
	float LeftMargin = TaskNode->GetWidgetPosition().X;
	float RightMargin = CachedGeometry.GetLocalSize().X - TaskNode->GetWidgetPosition().X - TaskNode->GetSize().X;

	return FMargin(LeftMargin, 0, RightMargin, 0);
}

FReply SBXTLTaskTrack::OnNotifyNodeDragStarted(TSharedRef<SBXTLTaskTrackNode> NotifyNode, const FPointerEvent& MouseEvent, const FVector2D& ScreenNodePosition, const bool bDragOnMarker, int32 NotifyIndex)
{
	if (!bDragOnMarker)
	{
		SelectTaskNode();

		TArray<TSharedPtr<SBXTLTaskTrackNode>> NodesToDrag;
		NodesToDrag.Add(NotifyNode);

		FVector2D DecoratorPosition = NodesToDrag[0]->GetWidgetPosition();
		DecoratorPosition = CachedGeometry.LocalToAbsolute(DecoratorPosition);

		TSharedRef<SOverlay> NodeDragDecoratorOverlay = SNew(SOverlay);
		TSharedRef<SBorder> NodeDragDecorator = SNew(SBorder).BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))[NodeDragDecoratorOverlay];

		FBox2D OverlayBounds(NodesToDrag[0]->GetScreenPosition(), NodesToDrag[0]->GetScreenPosition() + FVector2D(NodesToDrag[0]->GetDurationSize(), 0.0f));
		for (int32 Idx = 1; Idx < NodesToDrag.Num(); ++Idx)
		{
			TSharedPtr<SBXTLTaskTrackNode> Node = NodesToDrag[Idx];
			FVector2D NodePosition = Node->GetScreenPosition();
			float NodeDuration = Node->GetDurationSize();

			OverlayBounds += FBox2D(NodePosition, NodePosition + FVector2D(NodeDuration, 0.0f));
		}

		FVector2D OverlayOrigin = OverlayBounds.Min;
		FVector2D OverlayExtents = OverlayBounds.GetSize();

		for (TSharedPtr<SBXTLTaskTrackNode> Node : NodesToDrag)
		{
			FVector2D OffsetFromFirst(Node->GetScreenPosition() - OverlayOrigin);

			NodeDragDecoratorOverlay->AddSlot().Padding(FMargin(OffsetFromFirst.X, OffsetFromFirst.Y, 0.0f, 0.0f))[Node->AsShared()];
		}

		TArray<TSharedPtr<SBXTLTaskTrack>> NotifyAnimTracks;
		NotifyAnimTracks.Add(SharedThis(this));

		return FReply::Handled().BeginDragDrop(FBXTLTaskTrackNodeDragDropOp::New(NodesToDrag, NodeDragDecorator, NotifyAnimTracks, TimelinePlayLength, MouseEvent.GetScreenSpacePosition(), OverlayOrigin, OverlayExtents, CurrentDragXPosition, RefreshPanelEvent));
	}
	else
	{
		return FReply::Handled().CaptureMouse(NotifyNode).UseHighPrecisionMouseMovement(NotifyNode);
	}
}

#pragma endregion Widget

#undef LOCTEXT_NAMESPACE
