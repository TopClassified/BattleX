#include "SBXTLTaskTrackNode.h"
#include "Fonts/FontMeasure.h"
#include "Styling/CoreStyle.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SBorder.h"
#include "Preferences/PersonaOptions.h"
#include "Framework/Application/SlateApplication.h"

#include "STimeline.h"
#include "BXTLEditor.h"
#include "BXTLTaskTrackPanel.h"
#include "BXTLEditorDelegates.h"

#include "BXTask.h"
#include "BXTLController.h"

#if WITH_EDITOR
#include "Editor.h"
#endif



#define LOCTEXT_NAMESPACE "SBXTLTaskTrackNode"

const float NotifyHeightOffset = 0.0f;
const float DrawBoxHeightOffset = -4.0f;
const float NotifyHeight = FBXTLTaskTrackPanel::NotificationTrackHeight;
const FVector2D ScrubHandleSize(12.0f, 12.0f);
const FVector2D AlignmentMarkerSize(10.0f, 20.0f);
const FVector2D TextBorderSize(1.0f, 4.0f);

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

FText FBXTLTaskNodeData::GetNodeTooltip() const
{
	if (!CachedTask.IsValid())
	{
		return FText();
	}

	FString ToolTipString = FString::Format
	(
		TEXT("@ start:{0},dura: {1}\n {2} "), 
		{ 
			FString::FormatAsNumber(GetStartTime()),  
			FString::FormatAsNumber(GetDuration()),
			*CachedTask->GetClass()->GetToolTipText().ToString()
		}
	);

	return FText::FromString(ToolTipString);
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

	SetToolTipText(TAttribute<FText>(this, &SBXTLTaskTrackNode::GetNodeTooltip));

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

FBXTLTaskNodeData* SBXTLTaskTrackNode::GetTaskNodeDataPtr()
{ 
	return &TaskNodeData; 
}

const FBXTLTaskNodeData& SBXTLTaskTrackNode::GetTaskNodeData() const
{ 
	return TaskNodeData; 
}

#pragma endregion Important



#pragma region Render
int32 SBXTLTaskTrackNode::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	int32 MarkerLayer = LayerId + 1;
	int32 ScrubHandleID = MarkerLayer + 1;
	int32 TextLayerID = ScrubHandleID + 1;
	int32 BranchPointLayerID = TextLayerID + 1;

	const FSlateBrush* StyleInfo = FAppStyle::GetBrush(TEXT("SpecialEditableTextImageNormal"));

	FText Text = GetNotifyText();
	FLinearColor BoxColor = TaskNodeData.GetColor().GetValue();
	if (bSelected)
	{
		BoxColor = FLinearColor(0.73f, 0.36f, 0.0f);
	}

	// 持续任务
	if (NotifyDurationSizeX > 0.0f)
	{
		FVector2f DurationBoxSize = FVector2f(NotifyDurationSizeX, TextSize.Y + TextBorderSize.Y * 2.0f);
		FVector2f DurationBoxPosition = FVector2f(NotifyScrubHandleCenter, (NotifyHeight - TextSize.Y) * 0.5f + DrawBoxHeightOffset);
		FSlateDrawElement::MakeBox(OutDrawElements, LayerId, AllottedGeometry.ToPaintGeometry(DurationBoxSize, FSlateLayoutTransform(DurationBoxPosition)), StyleInfo, ESlateDrawEffect::None, BoxColor);
	}
	// 瞬时任务
	else
	{
		float SizeY = TextSize.Y + TextBorderSize.Y * 2.0f;
		FVector2f DurationBoxPosition = FVector2f(NotifyScrubHandleCenter, (NotifyHeight - TextSize.Y) * 0.5f + DrawBoxHeightOffset);

		TArray<FVector2f> Points;
		Points.Add(FVector2f(DurationBoxPosition.X, DurationBoxPosition.Y));
		Points.Add(FVector2f(DurationBoxPosition.X, DurationBoxPosition.Y + SizeY));
		Points.Add(FVector2f(DurationBoxPosition.X + SizeY, DurationBoxPosition.Y + SizeY * 0.5f));
		Points.Add(FVector2f(DurationBoxPosition.X, DurationBoxPosition.Y));

		FSlateDrawElement::MakeLines(OutDrawElements, LayerId, AllottedGeometry.ToPaintGeometry(), Points, ESlateDrawEffect::None, BoxColor);
	}

	// 文字
	float HalfScrubHandleWidth = ScrubHandleSize.X / 2.0f;
	FVector2f LabelSize(TextSize);
	LabelSize.X += TextBorderSize.X * 2.0f + HalfScrubHandleWidth;
	LabelSize.Y += TextBorderSize.Y * 2.0f;
	FVector2f TextPosition(TextBorderSize);
	TextPosition.X += HalfScrubHandleWidth + NotifyScrubHandleCenter;
	TextPosition.Y += DrawBoxHeightOffset + (NotifyHeight - TextSize.Y) * 0.5f;
	FVector2f DrawTextSize;
	DrawTextSize.X = TextSize.X;
	DrawTextSize.Y = TextSize.Y;
	FSlateDrawElement::MakeText(OutDrawElements, TextLayerID, AllottedGeometry.ToPaintGeometry(DrawTextSize, FSlateLayoutTransform(TextPosition)), Text, Font, ESlateDrawEffect::None, FLinearColor::White);

	return TextLayerID;
}

void SBXTLTaskTrackNode::DrawHandleOffset(const float& Offset, const float& HandleCentre, FSlateWindowElementList& OutDrawElements, int32 MarkerLayer, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FLinearColor NodeColor) const
{
	FVector2f MarkerPosition;
	FVector2f MarkerSize(AlignmentMarkerSize);

	if (Offset < 0.f)
	{
		MarkerPosition.Set(HandleCentre - AlignmentMarkerSize.X, (NotifyHeight - AlignmentMarkerSize.Y) / 2.f);
	}
	else
	{
		MarkerPosition.Set(HandleCentre + AlignmentMarkerSize.X, (NotifyHeight - AlignmentMarkerSize.Y) / 2.f);
		MarkerSize.X = -AlignmentMarkerSize.X;
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

FText SBXTLTaskTrackNode::GetNodeTooltip() const
{
	return TaskNodeData.GetNodeTooltip();
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
	return NotifyDurationSizeX;
}

FVector2D SBXTLTaskTrackNode::GetWidgetPosition() const
{
	return FVector2D(WidgetX, NotifyHeightOffset);
}

FVector2D SBXTLTaskTrackNode::GetNotifyPosition() const
{
	return FVector2D(NotifyTimePositionX, NotifyHeightOffset);
}

FVector2D SBXTLTaskTrackNode::GetNotifyPositionOffset() const
{
	return GetNotifyPosition() - GetWidgetPosition();
}

FVector2D SBXTLTaskTrackNode::ComputeDesiredSize(float) const
{
	return GetSize();
}

void SBXTLTaskTrackNode::UpdateSizeAndPosition(const FGeometry& AllottedGeometry)
{
	FTrackScaleInfo ScaleInfo(ViewInputMin.Get(), ViewInputMax.Get(), 0, 0, AllottedGeometry.Size);

	CachedAllotedGeometrySize = AllottedGeometry.Size * AllottedGeometry.Scale;

	if (DragType != EDragType::None)
	{
		NotifyTimePositionX = ScaleInfo.InputToLocalX(NodeStartTime);
		NotifyDurationSizeX = ScaleInfo.PixelsPerInput * NodeDuration;
	}
	else
	{
		NotifyTimePositionX = ScaleInfo.InputToLocalX(TaskNodeData.GetStartTime());
		NotifyDurationSizeX = ScaleInfo.PixelsPerInput * TaskNodeData.GetDuration();
	}

	const TSharedRef< FSlateFontMeasure > FontMeasureService = FSlateApplication::Get().GetRenderer()->GetFontMeasureService();
	TextSize = FontMeasureService->Measure(GetNotifyText(), Font);

	WidgetX = NotifyTimePositionX;
	WidgetSize = FVector2D(FMath::Max(NotifyDurationSizeX, TextSize.X), NotifyHeight);
	WidgetSize.X += FMath::Max(ScrubHandleSize.X, AlignmentMarkerSize.X * 2);
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

	float ScrubHandleHalfWidth = ScrubHandleSize.X / 2.0f;
	FVector2D NotifyNodePosition(ScrubHandleHalfWidth, 0.0f);
	FVector2D NotifyNodeSize(NotifyDurationSizeX + ScrubHandleHalfWidth * 2.0f, NotifyHeight);

	FVector2D MouseRelativePosition(CursorTrackPosition - GetWidgetPosition());
	if (MouseRelativePosition.ComponentwiseAllGreaterThan(NotifyNodePosition) && MouseRelativePosition.ComponentwiseAllLessThan(NotifyNodePosition + NotifyNodeSize))
	{
		// 持续任务需要区分
		if (NotifyDurationSizeX > 0.0f)
		{
			// 该次拖动想要修改开始时间
			if (MouseRelativePosition.X <= (NotifyNodePosition.X + ScrubHandleSize.X))
			{
				DragType = EDragType::StartTime;
			}
			// 该次拖动想要修改时长
			else if (MouseRelativePosition.X >= (NotifyNodePosition.X + NotifyNodeSize.X - ScrubHandleSize.X))
			{
				DragType = EDragType::Duration;
			}
		}
		// 瞬时任务默认修改开始时间
		else
		{
			DragType = EDragType::StartTime;
		}
	}
}

bool SBXTLTaskTrackNode::BeingDragged() const
{
	return (DragType == EDragType::StartTime) || (DragType == EDragType::Duration);
}

FReply SBXTLTaskTrackNode::OnDragDetected(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (DragType == EDragType::None)
	{
		RefreshDragType(LastMouseDownPosition);
	
		if (DragType != EDragType::None)
		{
			NodeStartTime = TaskNodeData.GetStartTime();
			NodeDuration = TaskNodeData.GetDuration();
			DragIndex = GEditor->BeginTransaction(NSLOCTEXT("Node", "Drag Postion", "Drag State Node Postion"));

			return StartDragTTNEvent.Execute(SharedThis(this), MouseEvent, FVector2D(MyGeometry.AbsolutePosition), false);
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

void SBXTLTaskTrackNode::SetLastMouseDownPosition(const FVector2D& CursorPosition)
{ 
	LastMouseDownPosition = CursorPosition; 
}

FReply SBXTLTaskTrackNode::OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (DragType == EDragType::None)
	{
		FSlateApplication::Get().ReleaseAllPointerCapture();
		return FReply::Handled();
	}

	FTrackScaleInfo ScaleInfo(ViewInputMin.Get(), ViewInputMax.Get(), 0, 0, CachedAllotedGeometrySize);

	float XPositionInTrack = MyGeometry.AbsolutePosition.X - CachedTrackGeometry.AbsolutePosition.X;
	float TrackScreenSpaceXPosition = MyGeometry.AbsolutePosition.X - XPositionInTrack;
	float TrackScreenSpaceOrigin = CachedTrackGeometry.LocalToAbsolute(FVector2D(ScaleInfo.InputToLocalX(0.0f), 0.0f)).X;
	float TrackScreenSpaceLimit = CachedTrackGeometry.LocalToAbsolute(FVector2D(ScaleInfo.InputToLocalX(TimelinePlayLength), 0.0f)).X;

	if (DragType == EDragType::StartTime)
	{
		float NewDisplayTime = ScaleInfo.LocalXToInput((FVector2f(MouseEvent.GetScreenSpacePosition()) - MyGeometry.AbsolutePosition + XPositionInTrack).X);
		NodeStartTime = NewDisplayTime;
	}
	else if (DragType == EDragType::Duration)
	{
		float NewDuration = ScaleInfo.LocalXToInput((FVector2f(MouseEvent.GetScreenSpacePosition()) - MyGeometry.AbsolutePosition + XPositionInTrack).X) - TaskNodeData.GetStartTime();
		NodeDuration = NewDuration;
	}

	return FReply::Handled();
}

FReply SBXTLTaskTrackNode::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	bool bLeftButton = MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton;

	FTrackScaleInfo ScaleInfo(ViewInputMin.Get(), ViewInputMax.Get(), 0, 0, CachedAllotedGeometrySize);

	float XPositionInTrack = MyGeometry.AbsolutePosition.X - CachedTrackGeometry.AbsolutePosition.X;
	float TrackScreenSpaceXPosition = MyGeometry.AbsolutePosition.X - XPositionInTrack;
	float TrackScreenSpaceOrigin = CachedTrackGeometry.LocalToAbsolute(FVector2D(ScaleInfo.InputToLocalX(0.0f), 0.0f)).X;
	float TrackScreenSpaceLimit = CachedTrackGeometry.LocalToAbsolute(FVector2D(ScaleInfo.InputToLocalX(TimelinePlayLength), 0.0f)).X;


	if (bLeftButton && DragType != EDragType::None)
	{
		// 对Task的起始时间进行修改
		if (DragType == EDragType::StartTime)
		{
			float NewDisplayTime = ScaleInfo.LocalXToInput((FVector2f(MouseEvent.GetScreenSpacePosition()) - MyGeometry.AbsolutePosition + XPositionInTrack).X);
			TaskNodeData.SetStartTime(NewDisplayTime);
		}
		// 对Task的时长进行修改
		else if (DragType == EDragType::Duration)
		{
			float NewDuration = ScaleInfo.LocalXToInput((FVector2f(MouseEvent.GetScreenSpacePosition()) - MyGeometry.AbsolutePosition + XPositionInTrack).X) - TaskNodeData.GetStartTime();
			TaskNodeData.SetDuration(NewDuration);
		}

		GEditor->EndTransaction();
		DragIndex = INDEX_NONE;
		DragType = EDragType::None;

		return FReply::Handled().ReleaseMouseCapture();
	}

	return FReply::Unhandled();
}

FCursorReply SBXTLTaskTrackNode::OnCursorQuery(const FGeometry& MyGeometry, const FPointerEvent& CursorEvent) const
{
	if (IsHovered() && GetDurationSize() > 0.0f)
	{
		FVector2D RelMouseLocation = MyGeometry.AbsoluteToLocal(CursorEvent.GetScreenSpacePosition());

		const float HandleHalfWidth = ScrubHandleSize.X / 2.0f;
		const float DistFromFirstHandle = FMath::Abs(RelMouseLocation.X);
		const float DistFromSecondHandle = FMath::Abs(RelMouseLocation.X - NotifyDurationSizeX);

		if (DistFromFirstHandle < HandleHalfWidth || DistFromSecondHandle < HandleHalfWidth || DragType != EDragType::None)
		{
			return FCursorReply::Cursor(EMouseCursor::ResizeLeftRight);
		}
	}

	return FCursorReply::Unhandled();
}

float SBXTLTaskTrackNode::HandleOverflowPan(const FVector2D& ScreenCursorPos, float TrackScreenSpaceXPosition, float TrackScreenSpaceMin, float TrackScreenSpaceMax)
{
	float Overflow = 0.0f;

	if (ScreenCursorPos.X < TrackScreenSpaceXPosition && TrackScreenSpaceXPosition > TrackScreenSpaceMin - 10.0f)
	{
		Overflow = FMath::Min(ScreenCursorPos.X - TrackScreenSpaceXPosition, -10.0f);
	}
	else if (ScreenCursorPos.X > CachedAllotedGeometrySize.X && (TrackScreenSpaceXPosition + CachedAllotedGeometrySize.X) < TrackScreenSpaceMax + 10.0f)
	{
		Overflow = FMath::Max(ScreenCursorPos.X - (TrackScreenSpaceXPosition + CachedAllotedGeometrySize.X), 10.0f);
	}

	return Overflow;
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
