#include "SBXTLExtraTrackNode.h"
#include "Editor.h"
#include "Fonts/FontMeasure.h"
#include "Styling/CoreStyle.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SBorder.h"
#include "Preferences/PersonaOptions.h"
#include "Framework/Application/SlateApplication.h"

#include "STimeline.h"
#include "BXTLExtraTrackPanel.h"
#include "BXTLEditorDelegates.h"

#include "BXTLAsset.h"



#define LOCTEXT_NAMESPACE "SBXTLExtraTrackNode"

namespace BXTLExtraNodeParam
{
	const float NotifyHeightOffset = 0.f;
	const float DrawBoxHeightOffset = 0.0f;
	const float NotifyHeight = FBXTLExtraTrackPanel::NotificationTrackHeight;
	const FVector2D ScrubHandleSize(10.0f, 10.0f);
	const FVector2D AlignmentMarkerSize(10.f, 4.f);
	const FVector2D TextBorderSize(1.f, 1.f);
}



#pragma region Important
void SBXTLExtraTrackNode::Construct(const FArguments& InArgs)
{
	Font = FCoreStyle::GetDefaultFontStyle("Regular", 8);
	bBeingDragged = false;
	CurrentDragHandle = EExtraTrackHandleHit::None;
	bDrawTooltipToRight = true;
	DragMarkerTransactionIdx = INDEX_NONE;

	CachedAsset = InArgs._Asset;
	SectionIndex = InArgs._SectionIndex;
	NodeType = InArgs._NodeType;
	ViewInputMin = InArgs._ViewInputMin;
	ViewInputMax = InArgs._ViewInputMax;
	TimelinePlayLength = InArgs._TimelinePlayLength.Get();
	RefreshPanelEvent = InArgs._OnRefreshPanel;
	StartDragNodeEvent = InArgs._OnStartDragNode;

	SetClipping(EWidgetClipping::ClipToBounds);

	SetToolTipText(TAttribute<FText>(this, &SBXTLExtraTrackNode::GetNodeTooltip));
}

SBXTLExtraTrackNode::~SBXTLExtraTrackNode()
{
	
}

void SBXTLExtraTrackNode::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	ScreenPosition = FVector2D(AllottedGeometry.AbsolutePosition);
}

#pragma endregion Important



#pragma region Render
int32 SBXTLExtraTrackNode::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	int32 MarkerLayer = LayerId + 1;
	int32 ScrubHandleID = MarkerLayer + 1;
	int32 TextLayerID = ScrubHandleID + 1;
	int32 BranchPointLayerID = TextLayerID + 1;

	const FSlateBrush* StyleInfo = FAppStyle::GetBrush(TEXT("SpecialEditableTextImageNormal"));

	FText Text = GetNotifyText();
	FLinearColor BoxColor = FLinearColor::Blue;

	// 根据时长创建一个Box
	if (NotifyDurationSizeX > 0.f)
	{
		FVector2D DurationBoxSize = FVector2D(NotifyDurationSizeX, TextSize.Y + BXTLExtraNodeParam::TextBorderSize.Y * 2.f);
		FVector2f DurationBoxPosition = FVector2f(NotifyScrubHandleCentre, (BXTLExtraNodeParam::NotifyHeight - TextSize.Y) * 0.5f + BXTLExtraNodeParam::DrawBoxHeightOffset);
		FSlateDrawElement::MakeBox
		(
			OutDrawElements, LayerId,
			AllottedGeometry.ToPaintGeometry(DurationBoxSize, FSlateLayoutTransform(1.0f, TransformPoint(1.0f, UE::Slate::CastToVector2f(DurationBoxPosition)))),
			StyleInfo, ESlateDrawEffect::None, BoxColor
		);
	}

	// 背景
	float HalfScrubHandleWidth = BXTLExtraNodeParam::ScrubHandleSize.X / 2.0f;
	FVector2D LabelSize = TextSize + BXTLExtraNodeParam::TextBorderSize * 2.f;
	LabelSize.X += HalfScrubHandleWidth;
	FVector2D LabelPosition(bDrawTooltipToRight ? NotifyScrubHandleCentre : NotifyScrubHandleCentre - LabelSize.X, (BXTLExtraNodeParam::NotifyHeight - TextSize.Y) * 0.5f);
	if (NotifyDurationSizeX == 0.f)
	{
		FSlateDrawElement::MakeBox
		(
			OutDrawElements, LayerId,
			AllottedGeometry.ToPaintGeometry(LabelSize, FSlateLayoutTransform(1.0f, TransformPoint(1.0f, UE::Slate::CastToVector2f(LabelPosition)))),
			StyleInfo, ESlateDrawEffect::None, BoxColor
		);
	}

	// 文字
	FVector2D TextPosition = LabelPosition + BXTLExtraNodeParam::TextBorderSize;
	if (bDrawTooltipToRight)
	{
		TextPosition.X += HalfScrubHandleWidth;
	}
	TextPosition.Y += BXTLExtraNodeParam::DrawBoxHeightOffset;

	FVector2D DrawTextSize;
	DrawTextSize.X = TextSize.X;
	DrawTextSize.Y = TextSize.Y;
	FSlateDrawElement::MakeText
	(
		OutDrawElements, TextLayerID, 
		AllottedGeometry.ToPaintGeometry(DrawTextSize, FSlateLayoutTransform(1.0f, TransformPoint(1.0f, UE::Slate::CastToVector2f(TextPosition)))),
		Text, Font, ESlateDrawEffect::None, FLinearColor::White
	);

	return TextLayerID;
}

void SBXTLExtraTrackNode::DrawHandleOffset(const float& Offset, const float& HandleCentre, FSlateWindowElementList& OutDrawElements, int32 MarkerLayer, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FLinearColor NodeColor) const
{
	FVector2D MarkerPosition;
	FVector2D MarkerSize = BXTLExtraNodeParam::AlignmentMarkerSize;

	if (Offset < 0.f)
	{
		MarkerPosition.Set(HandleCentre - BXTLExtraNodeParam::AlignmentMarkerSize.X, (BXTLExtraNodeParam::NotifyHeight - BXTLExtraNodeParam::AlignmentMarkerSize.Y) * 0.5f);
	}
	else
	{
		MarkerPosition.Set(HandleCentre + BXTLExtraNodeParam::AlignmentMarkerSize.X, (BXTLExtraNodeParam::NotifyHeight - BXTLExtraNodeParam::AlignmentMarkerSize.Y) * 0.5f);
		MarkerSize.X = BXTLExtraNodeParam::AlignmentMarkerSize.X * -1.0f;
	}

	FSlateDrawElement::MakeBox
	(
		OutDrawElements, MarkerLayer,
		AllottedGeometry.ToPaintGeometry(MarkerSize, FSlateLayoutTransform(1.0f, TransformPoint(1.0f, UE::Slate::CastToVector2f(MarkerPosition)))),
		FAppStyle::GetBrush(TEXT("BSA.Timeline.NotifyAlignmentMarker")),
		ESlateDrawEffect::None, NodeColor
	);
}

#pragma endregion Render



#pragma region Parameter
FText SBXTLExtraTrackNode::GetNotifyText() const
{
	return GetNodeName();
}

FText SBXTLExtraTrackNode::GetNodeTooltip() const
{
	return GetNodeName();
}

FVector2D SBXTLExtraTrackNode::GetSize() const
{
	return WidgetSize;
}

const FVector2D& SBXTLExtraTrackNode::GetScreenPosition() const
{
	return ScreenPosition;
}

float SBXTLExtraTrackNode::GetDurationSize() const
{ 
	return NotifyDurationSizeX; 
}

FVector2D SBXTLExtraTrackNode::GetWidgetPosition() const
{
	return FVector2D(WidgetX, BXTLExtraNodeParam::NotifyHeightOffset);
}

FVector2D SBXTLExtraTrackNode::GetNotifyPosition() const
{
	return FVector2D(NotifyTimePositionX, BXTLExtraNodeParam::NotifyHeightOffset);
}

FVector2D SBXTLExtraTrackNode::GetNotifyPositionOffset() const
{
	return GetNotifyPosition() - GetWidgetPosition();
}

FVector2D SBXTLExtraTrackNode::ComputeDesiredSize(float) const
{
	return GetSize();
}

void SBXTLExtraTrackNode::UpdateSizeAndPosition(const FGeometry& AllottedGeometry)
{
	FTrackScaleInfo ScaleInfo(ViewInputMin.Get(), ViewInputMax.Get(), 0.0f, 0.0f, AllottedGeometry.Size);

	CachedAllotedGeometrySize = AllottedGeometry.Size * AllottedGeometry.Scale;

	NotifyTimePositionX = ScaleInfo.InputToLocalX(GetNodeStartTime());
	NotifyDurationSizeX = ScaleInfo.PixelsPerInput * GetNodeDuration();

	const TSharedRef< FSlateFontMeasure > FontMeasureService = FSlateApplication::Get().GetRenderer()->GetFontMeasureService();
	TextSize = FontMeasureService->Measure(GetNotifyText(), Font);
	LabelWidth = TextSize.X + (BXTLExtraNodeParam::TextBorderSize.X * 2.0f) + (BXTLExtraNodeParam::ScrubHandleSize.X / 2.0f);

	float NotifyHandleBoxWidth = FMath::Max(BXTLExtraNodeParam::ScrubHandleSize.X, BXTLExtraNodeParam::AlignmentMarkerSize.X * 2.0f);

	FVector2D Size = GetSize();
	float LeftEdgeToNotify = NotifyTimePositionX;
	float RightEdgeToNotify = AllottedGeometry.Size.X - NotifyTimePositionX;
	bDrawTooltipToRight = NotifyDurationSizeX > 0.0f || ((RightEdgeToNotify > LabelWidth) || (RightEdgeToNotify > LeftEdgeToNotify));

	WidgetX = bDrawTooltipToRight ? (NotifyTimePositionX - (NotifyHandleBoxWidth / 2.0f)) : (NotifyTimePositionX - LabelWidth);
	float MaxSizeX = FMath::Max(NotifyDurationSizeX, TextSize.X);
	WidgetSize = bDrawTooltipToRight ? FVector2D(MaxSizeX, BXTLExtraNodeParam::NotifyHeight) : FVector2D(LabelWidth + MaxSizeX, BXTLExtraNodeParam::NotifyHeight);
	WidgetSize.X += NotifyHandleBoxWidth;

	NotifyScrubHandleCentre = bDrawTooltipToRight ? NotifyHandleBoxWidth / 2.0f : LabelWidth;
}

FText SBXTLExtraTrackNode::GetNodeName() const
{
	FString Result;

	if (NodeType == EBXTLExtraType::ET_Anticipation)
	{
		Result = TEXT("Anticipation");
	}
	else if (NodeType == EBXTLExtraType::ET_Recovery)
	{
		Result = TEXT("Recovery");
	}
	else if (NodeType >= EBXTLExtraType::ET_ComboWindow1 && NodeType <= EBXTLExtraType::ET_ComboWindow8)
	{
		Result = TEXT("ComboWindow");
	}

	return FText::FromString(Result);
}

float SBXTLExtraTrackNode::GetNodeStartTime()
{
	float Result = 0.0f;

	if (!CachedAsset.IsValid() || !CachedAsset.Get()->Sections.IsValidIndex(SectionIndex))
	{
		return Result;
	}

	const FBXTLSection& Section = CachedAsset.Get()->Sections[SectionIndex];

	if (const FVector2f* Find = Section.ExtraTime.Find(NodeType))
	{
		Result = Find->X;
	}

	return Result;
}

float SBXTLExtraTrackNode::GetNodeDuration()
{
	float Result = 0.0f;

	if (!CachedAsset.IsValid() || !CachedAsset.Get()->Sections.IsValidIndex(SectionIndex))
	{
		return Result;
	}

	const FBXTLSection& Section = CachedAsset.Get()->Sections[SectionIndex];

	if (const FVector2f* Find = Section.ExtraTime.Find(NodeType))
	{
		Result = Find->Y;
	}

	return Result;
}

void SBXTLExtraTrackNode::SetNodeStartTime(float InStartTime)
{
	if (!CachedAsset.IsValid() || !CachedAsset.Get()->Sections.IsValidIndex(SectionIndex))
	{
		return;
	}

	FBXTLSection& Section = CachedAsset.Get()->Sections[SectionIndex];

	if (FVector2f* Find = Section.ExtraTime.Find(NodeType))
	{
		Find->X = InStartTime;
	}
}

void SBXTLExtraTrackNode::SetNodeDuration(float InDuration)
{
	if (!CachedAsset.IsValid() || !CachedAsset.Get()->Sections.IsValidIndex(SectionIndex))
	{
		return;
	}

	FBXTLSection& Section = CachedAsset.Get()->Sections[SectionIndex];

	if (FVector2f* Find = Section.ExtraTime.Find(NodeType))
	{
		Find->Y = InDuration;
	}
}

#pragma endregion Parameter



#pragma region Widget
bool SBXTLExtraTrackNode::HitTest(const FGeometry& AllottedGeometry, FVector2D MouseLocalPose) const
{
	return true;
}

FReply SBXTLExtraTrackNode::OnFocusReceived(const FGeometry& MyGeometry, const FFocusEvent& InFocusEvent)
{
	return FReply::Handled().SetUserFocus(AsShared(), EFocusCause::SetDirectly, true);
}

void SBXTLExtraTrackNode::OnFocusLost(const FFocusEvent& InFocusEvent)
{
	if (CurrentDragHandle != EExtraTrackHandleHit::None)
	{
		CurrentDragHandle = EExtraTrackHandleHit::None;

		check(DragMarkerTransactionIdx != INDEX_NONE);
		GEditor->EndTransaction();
		DragMarkerTransactionIdx = INDEX_NONE;
	}
}

bool SBXTLExtraTrackNode::SupportsKeyboardFocus() const
{
	return true;
}

EExtraTrackHandleHit::Type SBXTLExtraTrackNode::DurationHandleHitTest(const FVector2D& CursorTrackPosition) const
{
	EExtraTrackHandleHit::Type MarkerHit = EExtraTrackHandleHit::Start;//NodeType == 2 ? EExtraTrackHandleHit::End : EExtraTrackHandleHit::Start;

	// 计算点击位置与TaskNode的相对位置，来判断能否对Task产生影响，以及产生什么影响
	if (NotifyDurationSizeX > 0.0f)
	{
		float ScrubHandleHalfWidth = BXTLExtraNodeParam::ScrubHandleSize.X / 2.0f;

		FVector2D NotifyNodePosition(NotifyScrubHandleCentre - ScrubHandleHalfWidth, 0.0f);
		FVector2D NotifyNodeSize(NotifyDurationSizeX + ScrubHandleHalfWidth * 2.0f, BXTLExtraNodeParam::NotifyHeight);

		FVector2D MouseRelativePosition(CursorTrackPosition - GetWidgetPosition());

		if (MouseRelativePosition.ComponentwiseAllGreaterThan(NotifyNodePosition) && MouseRelativePosition.ComponentwiseAllLessThan(NotifyNodePosition + NotifyNodeSize))
		{
			// 该次拖动想要修改开始时间
			if (MouseRelativePosition.X <= (NotifyNodePosition.X + BXTLExtraNodeParam::ScrubHandleSize.X))
			{
				MarkerHit = EExtraTrackHandleHit::Start;
			}
			// 该次拖动想要修改时长
			else if (MouseRelativePosition.X >= (NotifyNodePosition.X + NotifyNodeSize.X - BXTLExtraNodeParam::ScrubHandleSize.X))
			{
				MarkerHit = EExtraTrackHandleHit::End;
			}
		}
	}

	return MarkerHit;
}

bool SBXTLExtraTrackNode::BeingDragged() const
{
	return bBeingDragged;
}

FReply SBXTLExtraTrackNode::OnDragDetected(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	FVector2D ScreenNodePosition = FVector2D(MyGeometry.AbsolutePosition);

	bool bDragOnMarker = false;
	bBeingDragged = true;

	if (GetDurationSize() >= 0.0f)
	{
		EExtraTrackHandleHit::Type MarkerHit = DurationHandleHitTest(LastMouseDownPosition);
		bDragOnMarker = true;
		bBeingDragged = false;
		CurrentDragHandle = MarkerHit;

		DragMarkerTransactionIdx = GEditor->BeginTransaction(NSLOCTEXT("BXTLExtraNode", "ExtraNodeDragTransation", "Extra State Node Marker"));
	}

	return StartDragNodeEvent.Execute(SharedThis(this), MouseEvent, ScreenNodePosition, bDragOnMarker);
}

void SBXTLExtraTrackNode::DragCancelled()
{
	bBeingDragged = false;

}

void SBXTLExtraTrackNode::SetLastMouseDownPosition(const FVector2D& CursorPosition)
{ 
	LastMouseDownPosition = CursorPosition; 
}

FReply SBXTLExtraTrackNode::OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (CurrentDragHandle == EExtraTrackHandleHit::None)
	{
		FSlateApplication::Get().ReleaseAllPointerCapture();
		return FReply::Handled();
	}

	FTrackScaleInfo ScaleInfo(ViewInputMin.Get(), ViewInputMax.Get(), 0, 0, CachedAllotedGeometrySize);

	float XPositionInTrack = MyGeometry.AbsolutePosition.X - CachedTrackGeometry.AbsolutePosition.X;
	float TrackScreenSpaceXPosition = MyGeometry.AbsolutePosition.X - XPositionInTrack;
	float TrackScreenSpaceOrigin = CachedTrackGeometry.LocalToAbsolute(FVector2D(ScaleInfo.InputToLocalX(0.0f), 0.0f)).X;
	float TrackScreenSpaceLimit = CachedTrackGeometry.LocalToAbsolute(FVector2D(ScaleInfo.InputToLocalX(TimelinePlayLength), 0.0f)).X;

	// 对起始时间进行修改
	if (CurrentDragHandle != EExtraTrackHandleHit::End)
	{
		float NewDisplayTime = ScaleInfo.LocalXToInput((FVector2f(MouseEvent.GetScreenSpacePosition()) - MyGeometry.AbsolutePosition + XPositionInTrack).X);

		SetNodeStartTime(NewDisplayTime);
	}
	// 对时长进行修改
	else
	{
		float NewDuration = ScaleInfo.LocalXToInput((FVector2f(MouseEvent.GetScreenSpacePosition()) - MyGeometry.AbsolutePosition + XPositionInTrack).X) - GetNodeStartTime();

		SetNodeDuration(NewDuration);
	}

	return FReply::Handled();
}

FReply SBXTLExtraTrackNode::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	bool bLeftButton = MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton;

	if (bLeftButton && CurrentDragHandle != EExtraTrackHandleHit::None)
	{
		CurrentDragHandle = EExtraTrackHandleHit::None;

		// End drag transaction before handing mouse back
		check(DragMarkerTransactionIdx != INDEX_NONE);
		GEditor->EndTransaction();
		DragMarkerTransactionIdx = INDEX_NONE;

		return FReply::Handled().ReleaseMouseCapture();
	}

	return FReply::Unhandled();
}

FCursorReply SBXTLExtraTrackNode::OnCursorQuery(const FGeometry& MyGeometry, const FPointerEvent& CursorEvent) const
{
	if (IsHovered() && GetDurationSize() > 0.0f)
	{
		FVector2D RelMouseLocation = MyGeometry.AbsoluteToLocal(CursorEvent.GetScreenSpacePosition());

		const float HandleHalfWidth = BXTLExtraNodeParam::ScrubHandleSize.X / 2.0f;
		const float DistFromFirstHandle = FMath::Abs(RelMouseLocation.X - NotifyScrubHandleCentre);
		const float DistFromSecondHandle = FMath::Abs(RelMouseLocation.X - (NotifyScrubHandleCentre + NotifyDurationSizeX));

		if (DistFromFirstHandle < HandleHalfWidth || DistFromSecondHandle < HandleHalfWidth || CurrentDragHandle != EExtraTrackHandleHit::None)
		{
			return FCursorReply::Cursor(EMouseCursor::ResizeLeftRight);
		}
	}

	return FCursorReply::Unhandled();
}

float SBXTLExtraTrackNode::HandleOverflowPan(const FVector2D& ScreenCursorPos, float TrackScreenSpaceXPosition, float TrackScreenSpaceMin, float TrackScreenSpaceMax)
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

#pragma  endregion Widget

#undef LOCTEXT_NAMESPACE
