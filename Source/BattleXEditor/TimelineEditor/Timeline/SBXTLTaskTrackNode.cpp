#include "SBXTLTaskTrackNode.h"
#include "Editor.h"
#include "Fonts/FontMeasure.h"
#include "Styling/CoreStyle.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SBorder.h"
#include "Preferences/PersonaOptions.h"
#include "Framework/Application/SlateApplication.h"

#include "STimeline.h"
#include "TimelineTrack.h"
#include "BXTLEditor.h"
#include "BXTLTaskTrackPanel.h"
#include "BXTLEditorDelegates.h"

#include "BXTask.h"
#include "BXTLController.h"



#define LOCTEXT_NAMESPACE "SBXTLTaskTrackNode"

void FBXTLTaskNodeData::SetTaskNodeData(UBXTask* InTask)
{
	CachedTask = InTask;
}

UBXTask* FBXTLTaskNodeData::GetTask() const
{
	return CachedTask.Get();
}

FString FBXTLTaskNodeData::GetTaskName() const
{
	return CachedTask.IsValid() ? CachedTask->GetDisplayName().ToString() : FString(TEXT("Error Task!"));
}

TOptional<FLinearColor> FBXTLTaskNodeData::GetColor() const
{
	if (!CachedTask.IsValid())
	{
		return TOptional<FLinearColor>(FLinearColor::Red);
	}

	return ((CachedTask->TriggerTypes & 1) > 0) ? TOptional<FLinearColor>(FColor(0, 160, 160)) : TOptional<FLinearColor>(FLinearColor::Gray);
}

float FBXTLTaskNodeData::GetStartTime() const
{
	return CachedTask.IsValid() ? CachedTask->StartTime : 0.f;
}

void FBXTLTaskNodeData::SetStartTime(float Time)
{
	if (CachedTask.IsValid())
	{
		CachedTask->StartTime = Time;
		CachedTask->Modify();
	}
}

float FBXTLTaskNodeData::GetDuration() const
{
	if (!CachedTask.IsValid())
	{
		return 0.0f;
	}

	if (CachedTask->LifeType == EBXTLifeType::L_Instant)
	{
		return 0.0f;
	}
	else
	{
		return CachedTask->Duration;
	}
}

void FBXTLTaskNodeData::SetDuration(float InDuration)
{
	if (CachedTask.IsValid())
	{
		CachedTask->Duration = InDuration;
		CachedTask->Modify();
	}
}






#pragma region Important
namespace BXTLTTN
{
	const float ScrubSizeX = 10.0f;
	const float TextOffsetX = 4.0f;
	const float TextExpandSizeY = 4.0f;
	const float WidgetExpandSizeX = 200.0f;
}

void SBXTLTaskTrackNode::Construct(const FArguments& InArgs)
{
	Font = FCoreStyle::GetDefaultFontStyle("Regular", 10);
	bSelected = false;
	DragIndex = INDEX_NONE;
	DragType = EDragType::None;

	TaskNodeData.SetTaskNodeData(InArgs._Task);
	Controller = InArgs._Controller;
	RefreshPanelEvent = InArgs._OnRefreshPanel;
	StartDragTTNEvent = InArgs._OnStartDragTTN;
	ViewInputMin = InArgs._ViewInputMin;
	ViewInputMax = InArgs._ViewInputMax;
	TimelinePlayLength = InArgs._TimelinePlayLength.Get();

	SetClipping(EWidgetClipping::ClipToBounds);

	// 注册事件
	if (Controller.IsValid() && Controller.Pin()->GetEditor().IsValid())
	{
		Controller.Pin()->GetEditor().Pin()->OnTaskSelectionChanged.AddRaw(this, &SBXTLTaskTrackNode::OnEditorTaskSelectionChanged);
	}
}

SBXTLTaskTrackNode::~SBXTLTaskTrackNode()
{
	// 注销事件
	if (Controller.IsValid() && Controller.Pin()->GetEditor().IsValid())
	{
		Controller.Pin()->GetEditor().Pin()->OnTaskSelectionChanged.RemoveAll(this);
	}
}

void SBXTLTaskTrackNode::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	ScreenPosition = FVector2D(AllottedGeometry.AbsolutePosition);
}

#pragma endregion Important



#pragma region Render
int32 SBXTLTaskTrackNode::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	int32 MarkerLayer = LayerId + 1;
	int32 ScrubHandleID = MarkerLayer + 1;
	int32 TextLayerID = ScrubHandleID + 1;
	int32 BranchPointLayerID = TextLayerID + 1;

	FText Text = GetNotifyText();
	FLinearColor BoxColor = TaskNodeData.GetColor().GetValue();
	if (bSelected)
	{
		BoxColor = FLinearColor(0.73f, 0.36f, 0.0f);
	}

	float BoxHeightPosition = FTimelineTrack::TimelineTrackHeight * 0.125f;

	// 持续任务
	if (TaskDurationSizeX > 0.0f)
	{
		FVector2f DurationBoxSize = FVector2f(TaskDurationSizeX, TextSize.Y + BXTLTTN::TextExpandSizeY * 2.0f);
		FVector2f DurationBoxPosition = FVector2f(0.0f, BoxHeightPosition);

		const FSlateBrush* StyleInfo = FAppStyle::GetBrush(TEXT("SpecialEditableTextImageNormal"));
		FSlateDrawElement::MakeBox(OutDrawElements, LayerId, AllottedGeometry.ToPaintGeometry(DurationBoxSize, FSlateLayoutTransform(DurationBoxPosition)), StyleInfo, ESlateDrawEffect::None, BoxColor);
	}
	// 瞬时任务
	else
	{
		float BoxSize = TextSize.Y + BXTLTTN::TextExpandSizeY * 2.0f;
		const FSlateBrush* StyleInfo = FAppStyle::GetBrush(TEXT("SpecialEditableTextImageNormal"));

		FSlateDrawElement::MakeBox
		(
			OutDrawElements, LayerId, 
			AllottedGeometry.ToPaintGeometry(FVector2f(2.0f, BoxSize), FSlateLayoutTransform(FVector2f(0.0f, BoxHeightPosition))),
			StyleInfo, ESlateDrawEffect::None, BoxColor
		);

		FSlateDrawElement::MakeBox
		(
			OutDrawElements, LayerId,
			AllottedGeometry.ToPaintGeometry(FVector2f(BoxSize * 0.75f, BoxSize * 0.5f), FSlateLayoutTransform(FVector2f(0.0f, BoxHeightPosition))),
			StyleInfo, ESlateDrawEffect::None, BoxColor
		);
	}

	// 文字
	FVector2f TextPosition(BXTLTTN::TextOffsetX, FTimelineTrack::TimelineTrackHeight * 0.5f - TextSize.Y * 0.5f);
	FSlateDrawElement::MakeText(OutDrawElements, TextLayerID, AllottedGeometry.ToPaintGeometry(TextSize, FSlateLayoutTransform(TextPosition)), Text, Font, ESlateDrawEffect::None, FLinearColor::White);

	return TextLayerID;
}

void SBXTLTaskTrackNode::DrawHandleOffset(const float& Offset, const float& HandleCenter, FSlateWindowElementList& OutDrawElements, int32 MarkerLayer, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FLinearColor NodeColor) const
{
	FVector2f MarkerPosition;
	FVector2f MarkerSize(10.0f, 20.0f);

	if (Offset < 0.f)
	{
		MarkerPosition.Set(HandleCenter - MarkerSize.X, (FTimelineTrack::TimelineTrackHeight - MarkerSize.Y) * 0.5f);
	}
	else
	{
		MarkerPosition.Set(HandleCenter + MarkerSize.X, (FTimelineTrack::TimelineTrackHeight - MarkerSize.Y) * 0.5f);
		MarkerSize.X *= -1.0f;
	}

	FSlateDrawElement::MakeBox
	(
		OutDrawElements, MarkerLayer,
		AllottedGeometry.ToPaintGeometry(MarkerSize, FSlateLayoutTransform(MarkerPosition)),
		FAppStyle::GetBrush(TEXT("BX.Timeline.NotifyAlignmentMarker")),
		ESlateDrawEffect::None, NodeColor
	);
}

#pragma endregion Render



#pragma region Parameter
bool SBXTLTaskTrackNode::IsSelected() const
{ 
	return bSelected; 
}

FText SBXTLTaskTrackNode::GetNotifyText() const
{
	FString Temp = TaskNodeData.GetTaskName();
	if (TaskNodeData.GetTask() && !TaskNodeData.GetTask()->GetAnnotation().IsEmpty())
	{
		Temp = Temp + TEXT("    注释:");
		Temp = Temp + TaskNodeData.GetTask()->GetAnnotation().ToString();
	}

	return FText::FromString(Temp);
}

FVector2D SBXTLTaskTrackNode::GetSize() const
{
	return WidgetSize;
}

const FVector2D& SBXTLTaskTrackNode::GetScreenPosition() const
{
	return ScreenPosition;
}

float SBXTLTaskTrackNode::GetDurationSize() const
{ 
	return TaskDurationSizeX;
}

FVector2D SBXTLTaskTrackNode::GetWidgetPosition() const
{
	return FVector2D(WidgetX, 0.0f);
}

FVector2D SBXTLTaskTrackNode::GetNotifyPosition() const
{
	return FVector2D(TaskTimePositionX, 0.0f);
}

FVector2D SBXTLTaskTrackNode::GetNotifyPositionOffset() const
{
	return GetNotifyPosition() - GetWidgetPosition();
}

FBXTLTaskNodeData& SBXTLTaskTrackNode::GetTaskNodeData()
{
	return TaskNodeData;
}

FVector2D SBXTLTaskTrackNode::ComputeDesiredSize(float) const
{
	return GetSize();
}

void SBXTLTaskTrackNode::UpdateSizeAndPosition(const FGeometry& AllottedGeometry)
{
	FTrackScaleInfo ScaleInfo(ViewInputMin.Get(), ViewInputMax.Get(), 0, 0, AllottedGeometry.Size);

	CachedAllotedGeometrySize = AllottedGeometry.Size * AllottedGeometry.Scale;
	CachedAllotedGeometrySize.Y *= 0.5f;

	TaskTimePositionX = ScaleInfo.InputToLocalX(TaskNodeData.GetStartTime());
	TaskDurationSizeX = ScaleInfo.PixelsPerInput * (DragType == EDragType::Duration ? NodeDuration : TaskNodeData.GetDuration());

	const TSharedRef<FSlateFontMeasure> FontMeasureService = FSlateApplication::Get().GetRenderer()->GetFontMeasureService();
	TextSize = FontMeasureService->Measure(GetNotifyText(), Font);
	TextSize.Y = FTimelineTrack::TimelineTrackHeight * 0.75f - BXTLTTN::TextExpandSizeY * 2.0f;

	WidgetX = TaskTimePositionX;
	WidgetSize = FVector2D(FMath::Max3(TaskDurationSizeX, TextSize.X * 1.2f, BXTLTTN::ScrubSizeX) + BXTLTTN::WidgetExpandSizeX, FTimelineTrack::TimelineTrackHeight);
}

#pragma endregion Parameter



#pragma region Widget
FReply SBXTLTaskTrackNode::OnFocusReceived(const FGeometry& MyGeometry, const FFocusEvent& InFocusEvent)
{
	return FReply::Handled().SetUserFocus(AsShared(), EFocusCause::SetDirectly, true);
}

void SBXTLTaskTrackNode::OnFocusLost(const FFocusEvent& InFocusEvent)
{
	if (DragType != EDragType::None)
	{
		if (DragIndex != INDEX_NONE)
		{
			GEditor->EndTransaction();
		}

		DragIndex = INDEX_NONE;
		DragType = EDragType::None;
	}
}

bool SBXTLTaskTrackNode::SupportsKeyboardFocus() const
{
	return true;
}

void SBXTLTaskTrackNode::RefreshDragType(const FVector2D& CursorTrackPosition)
{
	DragType = EDragType::None;

	FVector2D NodePosition(0.0f, 0.0f);
	FVector2D NodeSize(WidgetSize.X, FTimelineTrack::TimelineTrackHeight);

	FVector2D MouseRelativePosition(CursorTrackPosition - GetWidgetPosition());
	if (MouseRelativePosition.ComponentwiseAllGreaterThan(NodePosition) && MouseRelativePosition.ComponentwiseAllLessThan(NodePosition + NodeSize))
	{
		// 持续任务需要区分
		if (TaskDurationSizeX > 0.0f)
		{
			// 该次拖动想要修改开始时间
			if (MouseRelativePosition.X < (NodePosition.X + TaskDurationSizeX - BXTLTTN::ScrubSizeX))
			{
				DragType = EDragType::StartTime;
			}
			// 该次拖动想要修改时长
			else if (MouseRelativePosition.X < (NodePosition.X + TaskDurationSizeX + BXTLTTN::ScrubSizeX))
			{
				DragType = EDragType::Duration;
			}
		}
		// 瞬时任务默认修改开始时间
		else if (MouseRelativePosition.X < (NodePosition.X + TextSize.X))
		{
			DragType = EDragType::StartTime;
		}
	}
}

bool SBXTLTaskTrackNode::BeingDragged() const
{
	return DragType == EDragType::StartTime;
}

FReply SBXTLTaskTrackNode::OnDragDetected(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (DragType == EDragType::None)
	{
		RefreshDragType(LastMouseDownPosition);
	
		if (DragType == EDragType::StartTime)
		{
			DragIndex = GEditor->BeginTransaction(NSLOCTEXT("Node", "Drag Postion", "Drag State Node Postion"));

			return StartDragTTNEvent.Execute(SharedThis(this), MouseEvent, FVector2D(MyGeometry.AbsolutePosition), false);
		}
		else if (DragType == EDragType::Duration)
		{
			return FReply::Handled();
		}
	}

	return FReply::Unhandled();
}

void SBXTLTaskTrackNode::DragCancelled()
{
	if (DragType != EDragType::None)
	{
		GEditor->EndTransaction();
	}

	DragIndex = INDEX_NONE;
	DragType = EDragType::None;
}

FReply SBXTLTaskTrackNode::OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (DragType == EDragType::Duration)
	{
		FTrackScaleInfo ScaleInfo(ViewInputMin.Get(), ViewInputMax.Get(), 0, 0, CachedAllotedGeometrySize);

		float XPositionInTrack = MyGeometry.AbsolutePosition.X - CachedTrackGeometry.AbsolutePosition.X;
		float NewDuration = ScaleInfo.LocalXToInput((FVector2f(MouseEvent.GetScreenSpacePosition()) - MyGeometry.AbsolutePosition + XPositionInTrack).X) - TaskNodeData.GetStartTime();
		
		NodeDuration = NewDuration;

		return FReply::Handled();
	}

	return FReply::Unhandled();
}

void SBXTLTaskTrackNode::OnMouseLeave(const FPointerEvent& MouseEvent)
{
	if (DragType == EDragType::Duration)
	{
		TaskNodeData.SetDuration(NodeDuration);
		DragType = EDragType::None;
	}
}

FReply SBXTLTaskTrackNode::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (DragType == EDragType::Duration)
	{
		TaskNodeData.SetDuration(NodeDuration);
		DragType = EDragType::None;

		return FReply::Handled();
	}

	return FReply::Unhandled();
}

void SBXTLTaskTrackNode::SetLastMouseDownPosition(const FVector2D& CursorPosition)
{ 
	LastMouseDownPosition = CursorPosition; 
}

FCursorReply SBXTLTaskTrackNode::OnCursorQuery(const FGeometry& MyGeometry, const FPointerEvent& CursorEvent) const
{
	if (IsHovered() && TaskDurationSizeX > 0.0f)
	{
		FVector2D MouseLocation = MyGeometry.AbsoluteToLocal(CursorEvent.GetScreenSpacePosition());

		const float DistFromFirstHandle = FMath::Abs(MouseLocation.X);
		const float DistFromSecondHandle = FMath::Abs(MouseLocation.X - TaskDurationSizeX);

		if (DistFromFirstHandle < BXTLTTN::ScrubSizeX || DistFromSecondHandle < BXTLTTN::ScrubSizeX)
		{
			return FCursorReply::Cursor(EMouseCursor::ResizeLeftRight);
		}
	}

	return FCursorReply::Unhandled();
}

#pragma endregion Widget



#pragma region Callback
void SBXTLTaskTrackNode::OnEditorTaskSelectionChanged(TArray<UBXTask*>& InTaskSelection)
{
	if (InTaskSelection.Contains(TaskNodeData.GetTask()))
	{
		bSelected = true;
	}
	else
	{
		bSelected = false;
	}
}

#pragma endregion Callback

#undef LOCTEXT_NAMESPACE
