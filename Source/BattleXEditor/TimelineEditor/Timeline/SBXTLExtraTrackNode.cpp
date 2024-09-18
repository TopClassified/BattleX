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

#pragma region Important
namespace BXTLETN
{
	const float ScrubSizeX = 10.0f;
	const float TextOffsetX = 4.0f;
	const float TextExpandSizeY = 4.0f;
}

void SBXTLExtraTrackNode::Construct(const FArguments& InArgs)
{
	Font = FCoreStyle::GetDefaultFontStyle("Regular", 6);
	DragIndex = INDEX_NONE;
	DragType = EDragType::None;

	CachedAsset = InArgs._Asset;
	SectionIndex = InArgs._SectionIndex;
	NodeType = InArgs._NodeType;
	ViewInputMin = InArgs._ViewInputMin;
	ViewInputMax = InArgs._ViewInputMax;
	TimelinePlayLength = InArgs._TimelinePlayLength.Get();
	RefreshPanelEvent = InArgs._OnRefreshPanel;
	StartDragETNEvent = InArgs._OnStartDragNode;

	SetClipping(EWidgetClipping::ClipToBounds);
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

	FText Text = GetNotifyText();
	FLinearColor BoxColor = FLinearColor::Blue;

	float BoxHeightPosition = FTimelineTrack::TimelineSubTrackHeight * 0.125f;

	// 根据时长创建一个Box
	if (ExtraDurationSizeX > 0.0f)
	{
		FVector2D DurationBoxSize = FVector2D(ExtraDurationSizeX, TextSize.Y + BXTLETN::TextExpandSizeY * 2.0f);
		FVector2f DurationBoxPosition = FVector2f(0.0f, BoxHeightPosition);
		
		const FSlateBrush* StyleInfo = FAppStyle::GetBrush(TEXT("SpecialEditableTextImageNormal"));

		FSlateDrawElement::MakeBox(OutDrawElements, LayerId, AllottedGeometry.ToPaintGeometry(DurationBoxSize, FSlateLayoutTransform(DurationBoxPosition)), StyleInfo, ESlateDrawEffect::None, BoxColor);
	}

	// 文字
	FVector2f TextPosition(BXTLETN::TextOffsetX, FTimelineTrack::TimelineSubTrackHeight * 0.5f - TextSize.Y * 0.5f);
	FSlateDrawElement::MakeText(OutDrawElements, TextLayerID, AllottedGeometry.ToPaintGeometry(TextSize, FSlateLayoutTransform(TextPosition)), Text, Font, ESlateDrawEffect::None, FLinearColor::White);

	return TextLayerID;
}

void SBXTLExtraTrackNode::DrawHandleOffset(const float& Offset, const float& HandleCentre, FSlateWindowElementList& OutDrawElements, int32 MarkerLayer, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FLinearColor NodeColor) const
{
	FVector2D MarkerPosition;
	FVector2D MarkerSize(10.0f, 10.0f);

	if (Offset < 0.f)
	{
		MarkerPosition.Set(HandleCentre - MarkerSize.X, (FTimelineTrack::TimelineSubTrackHeight - MarkerSize.Y) * 0.5f);
	}
	else
	{
		MarkerPosition.Set(HandleCentre + MarkerSize.X, (FTimelineTrack::TimelineSubTrackHeight - MarkerSize.Y) * 0.5f);
		MarkerSize.X *= -1.0f;
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
	return ExtraDurationSizeX;
}

FVector2D SBXTLExtraTrackNode::GetWidgetPosition() const
{
	return FVector2D(WidgetX, 0.0f);
}

FVector2D SBXTLExtraTrackNode::GetNotifyPosition() const
{
	return FVector2D(ExtraTimePositionX, 0.0f);
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

	ExtraTimePositionX = ScaleInfo.InputToLocalX(GetNodeStartTime());
	ExtraDurationSizeX = ScaleInfo.PixelsPerInput * GetNodeDuration();

	const TSharedRef<FSlateFontMeasure> FontMeasureService = FSlateApplication::Get().GetRenderer()->GetFontMeasureService();
	TextSize = FontMeasureService->Measure(GetNotifyText(), Font);
	TextSize.Y = FTimelineTrack::TimelineSubTrackHeight * 0.75f - BXTLETN::TextExpandSizeY * 2.0f;

	WidgetX = ExtraTimePositionX;
	WidgetSize = FVector2D(FMath::Max3(ExtraDurationSizeX, TextSize.X * 1.2f, BXTLETN::ScrubSizeX * 2.0f), FTimelineTrack::TimelineSubTrackHeight);
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
		Result = TEXT("ComboWindow:") + FString::FromInt((int32)NodeType - (int32)EBXTLExtraType::ET_ComboWindow1 + 1);
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
FReply SBXTLExtraTrackNode::OnFocusReceived(const FGeometry& MyGeometry, const FFocusEvent& InFocusEvent)
{
	return FReply::Handled().SetUserFocus(AsShared(), EFocusCause::SetDirectly, true);
}

void SBXTLExtraTrackNode::OnFocusLost(const FFocusEvent& InFocusEvent)
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

bool SBXTLExtraTrackNode::SupportsKeyboardFocus() const
{
	return true;
}

void SBXTLExtraTrackNode::RefreshDragType(const FVector2D& CursorTrackPosition)
{
	DragType = EDragType::None;

	FVector2D NodePosition(0.0f, 0.0f);
	FVector2D NodeSize(ExtraDurationSizeX > 0.0f ? ExtraDurationSizeX : WidgetSize.X, FTimelineTrack::TimelineSubTrackHeight);

	FVector2D MouseRelativePosition(CursorTrackPosition - GetWidgetPosition());
	if (MouseRelativePosition.ComponentwiseAllGreaterThan(NodePosition) && MouseRelativePosition.ComponentwiseAllLessThan(NodePosition + NodeSize))
	{
		// 该次拖动想要修改开始时间
		if (MouseRelativePosition.X <= (NodePosition.X + NodeSize.X - BXTLETN::ScrubSizeX))
		{
			DragType = EDragType::StartTime;
		}
		// 该次拖动想要修改时长
		else
		{
			DragType = EDragType::Duration;
		}
	}
}

bool SBXTLExtraTrackNode::BeingDragged() const
{
	return (DragType == EDragType::StartTime) || (DragType == EDragType::Duration);
}

FReply SBXTLExtraTrackNode::OnDragDetected(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (DragType == EDragType::None)
	{
		RefreshDragType(LastMouseDownPosition);

		if (DragType != EDragType::None)
		{
			DragIndex = GEditor->BeginTransaction(NSLOCTEXT("Node", "Drag Postion", "Drag State Node Postion"));

			return StartDragETNEvent.Execute(SharedThis(this), MouseEvent, FVector2D(MyGeometry.AbsolutePosition), false);
		}
	}

	return FReply::Unhandled();
}

void SBXTLExtraTrackNode::DragCancelled()
{
	if (DragType != EDragType::None)
	{
		GEditor->EndTransaction();
	}

	DragIndex = INDEX_NONE;
	DragType = EDragType::None;
}

void SBXTLExtraTrackNode::SetLastMouseDownPosition(const FVector2D& CursorPosition)
{ 
	LastMouseDownPosition = CursorPosition; 
}

FCursorReply SBXTLExtraTrackNode::OnCursorQuery(const FGeometry& MyGeometry, const FPointerEvent& CursorEvent) const
{
	float DurationSize = GetDurationSize();

	if (IsHovered() && DurationSize > 0.0f)
	{
		FVector2D MouseLocation = MyGeometry.AbsoluteToLocal(CursorEvent.GetScreenSpacePosition());

		const float DistFromFirstHandle = FMath::Abs(MouseLocation.X);
		const float DistFromSecondHandle = FMath::Abs(MouseLocation.X - DurationSize);

		if (DistFromFirstHandle < BXTLETN::ScrubSizeX || DistFromSecondHandle < BXTLETN::ScrubSizeX)
		{
			return FCursorReply::Cursor(EMouseCursor::ResizeLeftRight);
		}
	}

	return FCursorReply::Unhandled();
}

#pragma  endregion Widget

#undef LOCTEXT_NAMESPACE
