#include "SBXTLExtraTrack.h"
#include "Framework/Application/SlateApplication.h"

#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Input/SNumericEntryBox.h"

#include "BXTLDragDropOp.h"
#include "BXTLExtraTrackPanel.h"
#include "BXTLEditorUtilities.h"

#include "BXTLAsset.h" 



#define LOCTEXT_NAMESPACE "SBXTLExtraTrack"

#pragma region Important
void SBXTLExtraTrack::Construct(const FArguments& InArgs)
{
	SetClipping(EWidgetClipping::ClipToBounds);

	CachedAsset = InArgs._Asset;
	SectionIndex = InArgs._SectionIndex;
	TrackType = InArgs._TrackType;
	ViewInputMin = InArgs._ViewInputMin;
	ViewInputMax = InArgs._ViewInputMax;
	InputMin = InArgs._InputMin;
	InputMax = InArgs._InputMax;
	TimelinePlayLength = InArgs._TimelinePlayLength.Get();
	SetInputViewRangeEvent = InArgs._SetInputViewRangeEvent;
	RefreshPanelEvent = InArgs._RefreshPanelEvent;

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

void SBXTLExtraTrack::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{ 
	UpdateCachedGeometry(AllottedGeometry); 

	if (TrackNode.IsValid())
	{
		TrackNode->CachedTrackGeometry = AllottedGeometry;

		if (!TrackNode->BeingDragged())
		{
			TrackNode->UpdateSizeAndPosition(AllottedGeometry);
		}
	}
}

#pragma endregion Important



#pragma region Render
void SBXTLExtraTrack::UpdateLayout()
{
	TrackArea->SetContent
	(
		SAssignNew(NodeSlots, SOverlay)
	);

	SAssignNew(TrackNode, SBXTLExtraTrackNode)
	.Asset(CachedAsset.Get())
	.SectionIndex(SectionIndex)
	.NodeType(TrackType)
	.ViewInputMin(ViewInputMin)
	.ViewInputMax(ViewInputMax)
	.TimelinePlayLength(TimelinePlayLength)
	.RefreshPanelEvent(RefreshPanelEvent)
	.DragETNEvent(this, &SBXTLExtraTrack::OnNodeDragged, 0);

	NodeSlots->AddSlot()
	.Padding(TAttribute<FMargin>::Create(TAttribute<FMargin>::FGetter::CreateSP(this, &SBXTLExtraTrack::GetNotifyTrackPadding)))
	[
		TrackNode->AsShared()
	];
}

#pragma endregion Render



#pragma region Widget
const FGeometry& SBXTLExtraTrack::GetCachedGeometry() const
{ 
	return CachedGeometry; 
}

void SBXTLExtraTrack::UpdateCachedGeometry(const FGeometry& InGeometry)
{
	CachedGeometry = InGeometry;

	TrackNode->CachedTrackGeometry = InGeometry;
}

FTrackScaleInfo SBXTLExtraTrack::GetCachedScaleInfo() const
{ 
	return FTrackScaleInfo(ViewInputMin.Get(), ViewInputMax.Get(), 0.f, 0.f, CachedGeometry.GetLocalSize()); 
}

FVector2D SBXTLExtraTrack::ComputeDesiredSize(float) const
{
	FVector2D Size;
	Size.X = 200;
	Size.Y = FTimelineTrack::TimelineSubTrackHeight;
	return Size;
}

bool SBXTLExtraTrack::SupportsKeyboardFocus() const
{
	return true;
}

FReply SBXTLExtraTrack::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		TrackNode->SetLastMouseDownPosition(MyGeometry.AbsoluteToLocal(MouseEvent.GetScreenSpacePosition()));

		return FReply::Handled().DetectDrag(TrackNode.ToSharedRef(), EKeys::LeftMouseButton);
	}

	return FReply::Handled();
}

FReply SBXTLExtraTrack::OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	return FReply::Handled();
}

FReply SBXTLExtraTrack::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		return FReply::Handled();
	}

	return FReply::Unhandled();
}

FCursorReply SBXTLExtraTrack::OnCursorQuery(const FGeometry& MyGeometry, const FPointerEvent& CursorEvent) const
{
	return FCursorReply::Unhandled();
}

void SBXTLExtraTrack::HandleNodeDrop(TSharedPtr<SBXTLExtraTrackNode> Node, float Offset)
{
	float LocalX = GetCachedGeometry().AbsoluteToLocal(Node->GetScreenPosition() + Offset).X;
	float Time = GetCachedScaleInfo().LocalXToInput(LocalX);

	Node->SetNodeStartTime(Time);
}

FMargin SBXTLExtraTrack::GetNotifyTrackPadding() const
{
	float LeftMargin = TrackNode->GetWidgetPosition().X;
	float RightMargin = CachedGeometry.GetLocalSize().X - TrackNode->GetWidgetPosition().X - TrackNode->GetSize().X;

	return FMargin(LeftMargin, 0, RightMargin, 0);
}

#pragma endregion Widget



#pragma region Callback
FReply SBXTLExtraTrack::OnNodeDragged(TSharedRef<SBXTLExtraTrackNode> NotifyNode, const FPointerEvent& MouseEvent, const FVector2D& ScreenNodePosition, const bool bDragOnMarker, int32 NotifyIndex)
{
	if (!bDragOnMarker)
	{
		TArray<TSharedPtr<SBXTLExtraTrackNode>> NodesToDrag;
		NodesToDrag.Add(NotifyNode);

		FVector2D DecoratorPosition = NodesToDrag[0]->GetWidgetPosition();
		DecoratorPosition = CachedGeometry.LocalToAbsolute(DecoratorPosition);

		TSharedRef<SOverlay> NodeDragDecoratorOverlay = SNew(SOverlay);
		TSharedRef<SBorder> NodeDragDecorator = SNew(SBorder).BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))[NodeDragDecoratorOverlay];

		FBox2D OverlayBounds(NodesToDrag[0]->GetScreenPosition(), NodesToDrag[0]->GetScreenPosition() + FVector2D(NodesToDrag[0]->GetDurationSize(), 0.0f));
		for (int32 Idx = 1; Idx < NodesToDrag.Num(); ++Idx)
		{
			TSharedPtr<SBXTLExtraTrackNode> Node = NodesToDrag[Idx];
			FVector2D NodePosition = Node->GetScreenPosition();
			float NodeDuration = Node->GetDurationSize();

			OverlayBounds += FBox2D(NodePosition, NodePosition + FVector2D(NodeDuration, 0.0f));
		}

		FVector2D OverlayOrigin = OverlayBounds.Min;
		FVector2D OverlayExtents = OverlayBounds.GetSize();

		for (TSharedPtr<SBXTLExtraTrackNode> Node : NodesToDrag)
		{
			FVector2D OffsetFromFirst(Node->GetScreenPosition() - OverlayOrigin);

			NodeDragDecoratorOverlay->AddSlot().Padding(FMargin(OffsetFromFirst.X, OffsetFromFirst.Y, 0.0f, 0.0f))[Node->AsShared()];
		}

		TArray<TSharedPtr<SBXTLExtraTrack>> NotifyAnimTracks;
		NotifyAnimTracks.Add(SharedThis(this));

		return FReply::Handled().BeginDragDrop(FBXTLExtraTrackNodeDragDropOp::New(NodesToDrag, NodeDragDecorator, NotifyAnimTracks, TimelinePlayLength, MouseEvent.GetScreenSpacePosition(), OverlayOrigin, OverlayExtents, CurrentDragXPosition, RefreshPanelEvent));
	}
	else
	{
		return FReply::Handled().CaptureMouse(NotifyNode).UseHighPrecisionMouseMovement(NotifyNode);
	}
}

#pragma endregion Callback

#undef LOCTEXT_NAMESPACE
