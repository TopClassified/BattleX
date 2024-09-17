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
	bBeingDragged = false;
	CurrentDragHandle = ENotifyStateHandleHit::None;
	bDrawTooltipToRight = true;
	DragMarkerTransactionIdx = INDEX_NONE;
	DragMarkerPositionIdx = INDEX_NONE;

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

	// 根据时长创建一个Box
	if (NotifyDurationSizeX > 0.0f)
	{
		FVector2f DurationBoxSize = FVector2f(NotifyDurationSizeX, TextSize.Y + TextBorderSize.Y * 2.0f);
		FVector2f DurationBoxPosition = FVector2f(NotifyScrubHandleCenter, (NotifyHeight - TextSize.Y) * 0.5f + DrawBoxHeightOffset);
		FSlateDrawElement::MakeBox(OutDrawElements, LayerId, AllottedGeometry.ToPaintGeometry(DurationBoxSize, FSlateLayoutTransform(DurationBoxPosition)), StyleInfo, ESlateDrawEffect::None, BoxColor);
	}
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

	if (CurrentDragHandle != ENotifyStateHandleHit::None || bBeingDragged)
	{
		NotifyTimePositionX = ScaleInfo.InputToLocalX(NodeStartTime);
		NotifyDurationSizeX = ScaleInfo.PixelsPerInput * NodeDuration;
	}
	else if (!bBeingDragged)
	{
		NotifyTimePositionX = ScaleInfo.InputToLocalX(TaskNodeData.GetStartTime());
		NotifyDurationSizeX = ScaleInfo.PixelsPerInput * TaskNodeData.GetDuration();
	}

	const TSharedRef< FSlateFontMeasure > FontMeasureService = FSlateApplication::Get().GetRenderer()->GetFontMeasureService();
	TextSize = FontMeasureService->Measure(GetNotifyText(), Font);
	LabelWidth = TextSize.X + (TextBorderSize.X * 2.f) + (ScrubHandleSize.X / 2.f);

	float NotifyHandleBoxWidth = FMath::Max(ScrubHandleSize.X, AlignmentMarkerSize.X * 2);

	FVector2D Size = GetSize();
	float LeftEdgeToNotify = NotifyTimePositionX;
	float RightEdgeToNotify = AllottedGeometry.Size.X - NotifyTimePositionX;
	bDrawTooltipToRight = NotifyDurationSizeX > 0.0f || ((RightEdgeToNotify > LabelWidth) || (RightEdgeToNotify > LeftEdgeToNotify));

	WidgetX = bDrawTooltipToRight ? (NotifyTimePositionX - (NotifyHandleBoxWidth / 2.f)) : (NotifyTimePositionX - LabelWidth);
	float MaxSizeX = FMath::Max(NotifyDurationSizeX, TextSize.X);
	WidgetSize = bDrawTooltipToRight ? FVector2D(MaxSizeX, NotifyHeight) : FVector2D(LabelWidth + MaxSizeX, NotifyHeight);
	WidgetSize.X += NotifyHandleBoxWidth;

	NotifyScrubHandleCenter = bDrawTooltipToRight ? NotifyHandleBoxWidth / 2.f : LabelWidth;
}

#pragma endregion Parameter



#pragma region Widget
FReply SBXTLTaskTrackNode::OnFocusReceived(const FGeometry& MyGeometry, const FFocusEvent& InFocusEvent)
{
	return FReply::Handled().SetUserFocus(AsShared(), EFocusCause::SetDirectly, true);
}

void SBXTLTaskTrackNode::OnFocusLost(const FFocusEvent& InFocusEvent)
{
	if (CurrentDragHandle != ENotifyStateHandleHit::None)
	{
		CurrentDragHandle = ENotifyStateHandleHit::None;

		if (DragMarkerTransactionIdx != INDEX_NONE)
		{
			GEditor->EndTransaction();
			DragMarkerTransactionIdx = INDEX_NONE;
		}

		if (DragMarkerPositionIdx != INDEX_NONE)
		{
			GEditor->EndTransaction();
			DragMarkerPositionIdx = INDEX_NONE;
		}
	}
}

bool SBXTLTaskTrackNode::SupportsKeyboardFocus() const
{
	return true;
}

ENotifyStateHandleHit::Type SBXTLTaskTrackNode::DurationHandleHitTest(const FVector2D& CursorTrackPosition) const
{
	ENotifyStateHandleHit::Type MarkerHit = ENotifyStateHandleHit::None;

	// 计算点击位置与TaskNode的相对位置，来判断能否对Task产生影响，以及产生什么影响
	if (NotifyDurationSizeX > 0.0f)
	{
		float ScrubHandleHalfWidth = ScrubHandleSize.X / 2.0f;

		FVector2D NotifyNodePosition(NotifyScrubHandleCenter - ScrubHandleHalfWidth, 0.0f);
		FVector2D NotifyNodeSize(NotifyDurationSizeX + ScrubHandleHalfWidth * 2.0f, NotifyHeight);

		FVector2D MouseRelativePosition(CursorTrackPosition - GetWidgetPosition());

		if (MouseRelativePosition.ComponentwiseAllGreaterThan(NotifyNodePosition) && MouseRelativePosition.ComponentwiseAllLessThan(NotifyNodePosition + NotifyNodeSize))
		{
			// 该次拖动想要修改开始时间
			if (MouseRelativePosition.X <= (NotifyNodePosition.X + ScrubHandleSize.X))
			{
				MarkerHit = ENotifyStateHandleHit::Start;
			}
			// 该次拖动想要修改时长
			else if (MouseRelativePosition.X >= (NotifyNodePosition.X + NotifyNodeSize.X - ScrubHandleSize.X))
			{
				MarkerHit = ENotifyStateHandleHit::End;
			}
		}
	}

	return MarkerHit;
}

bool SBXTLTaskTrackNode::BeingDragged() const
{
	return CurrentDragHandle == ENotifyStateHandleHit::Start || CurrentDragHandle == ENotifyStateHandleHit::End;
}

FReply SBXTLTaskTrackNode::OnDragDetected(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	ENotifyStateHandleHit::Type MarkerHit = DurationHandleHitTest(LastMouseDownPosition);
	if (!bBeingDragged)
	{
		NodeStartTime = TaskNodeData.GetStartTime();
		NodeDuration = TaskNodeData.GetDuration();
		if (MarkerHit == ENotifyStateHandleHit::None)
		{
			DragMarkerPositionIdx = GEditor->BeginTransaction(NSLOCTEXT("Node", "Drag Postion", "Drag State Node Postion"));
		}
	}

	FVector2D ScreenNodePosition = FVector2D(MyGeometry.AbsolutePosition);

	bool bDragOnMarker = false;
	bBeingDragged = true;

	if (GetDurationSize() > 0.0f)
	{
		if (MarkerHit == ENotifyStateHandleHit::Start || MarkerHit == ENotifyStateHandleHit::End)
		{
			bDragOnMarker = true;
			bBeingDragged = false;
			CurrentDragHandle = MarkerHit;

			DragMarkerTransactionIdx = GEditor->BeginTransaction(NSLOCTEXT("Node", "Drag Transation", "Drag State Node Marker"));
		}

	}

	return StartDragTTNEvent.Execute(SharedThis(this), MouseEvent, ScreenNodePosition, bDragOnMarker);
}

void SBXTLTaskTrackNode::DragCancelled()
{
	if (bBeingDragged && CurrentDragHandle == ENotifyStateHandleHit::None)
	{
		TaskNodeData.SetStartTime(TaskNodeData.GetStartTime());
		check(DragMarkerPositionIdx != INDEX_NONE);
		GEditor->EndTransaction();
		DragMarkerPositionIdx = INDEX_NONE;
	}
	bBeingDragged = false;
}

void SBXTLTaskTrackNode::SetLastMouseDownPosition(const FVector2D& CursorPosition)
{ 
	LastMouseDownPosition = CursorPosition; 
}

FReply SBXTLTaskTrackNode::OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (CurrentDragHandle == ENotifyStateHandleHit::None)
	{
		FSlateApplication::Get().ReleaseAllPointerCapture();
		return FReply::Handled();
	}

	FTrackScaleInfo ScaleInfo(ViewInputMin.Get(), ViewInputMax.Get(), 0, 0, CachedAllotedGeometrySize);

	float XPositionInTrack = MyGeometry.AbsolutePosition.X - CachedTrackGeometry.AbsolutePosition.X;
	float TrackScreenSpaceXPosition = MyGeometry.AbsolutePosition.X - XPositionInTrack;
	float TrackScreenSpaceOrigin = CachedTrackGeometry.LocalToAbsolute(FVector2D(ScaleInfo.InputToLocalX(0.0f), 0.0f)).X;
	float TrackScreenSpaceLimit = CachedTrackGeometry.LocalToAbsolute(FVector2D(ScaleInfo.InputToLocalX(TimelinePlayLength), 0.0f)).X;

	if (CurrentDragHandle == ENotifyStateHandleHit::Start || bBeingDragged)
	{
		float NewDisplayTime = ScaleInfo.LocalXToInput((FVector2f(MouseEvent.GetScreenSpacePosition()) - MyGeometry.AbsolutePosition + XPositionInTrack).X);
		NodeStartTime = NewDisplayTime;
	}
	else if (CurrentDragHandle == ENotifyStateHandleHit::End)
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


	if (bLeftButton && CurrentDragHandle != ENotifyStateHandleHit::None)
	{
		// 对Task的起始时间进行修改
		if (CurrentDragHandle == ENotifyStateHandleHit::Start)
		{
			float NewDisplayTime = ScaleInfo.LocalXToInput((FVector2f(MouseEvent.GetScreenSpacePosition()) - MyGeometry.AbsolutePosition + XPositionInTrack).X);

			TaskNodeData.SetStartTime(NewDisplayTime);
		}
		// 对Task的时长进行修改
		else
		{
			float NewDuration = ScaleInfo.LocalXToInput((FVector2f(MouseEvent.GetScreenSpacePosition()) - MyGeometry.AbsolutePosition + XPositionInTrack).X) - TaskNodeData.GetStartTime();

			TaskNodeData.SetDuration(NewDuration);
		}

		CurrentDragHandle = ENotifyStateHandleHit::None;

		// End drag transaction before handing mouse back
		check(DragMarkerTransactionIdx != INDEX_NONE);
		GEditor->EndTransaction();
		DragMarkerTransactionIdx = INDEX_NONE;

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
		const float DistFromFirstHandle = FMath::Abs(RelMouseLocation.X - NotifyScrubHandleCenter);
		const float DistFromSecondHandle = FMath::Abs(RelMouseLocation.X - (NotifyScrubHandleCenter + NotifyDurationSizeX));

		if (DistFromFirstHandle < HandleHalfWidth || DistFromSecondHandle < HandleHalfWidth || CurrentDragHandle != ENotifyStateHandleHit::None)
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
