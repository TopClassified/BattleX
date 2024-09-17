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
	SetInputViewRangeEvent = InArgs._OnSetInputViewRange;
	RefreshPanelEvent = InArgs._OnRefreshPanel;

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
}

#pragma endregion Important



#pragma region Render
int32 SBXTLExtraTrack::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
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

	if (!TrackNode->BeingDragged())
	{
		TrackNode->UpdateSizeAndPosition(AllottedGeometry);
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
	.OnRefreshPanel(RefreshPanelEvent)
	.OnStartDragNode(this, &SBXTLExtraTrack::OnNotifyNodeDragStarted, 0);

	NodeSlots->AddSlot()
	.Padding(TAttribute<FMargin>::Create(TAttribute<FMargin>::FGetter::CreateSP(this, &SBXTLExtraTrack::GetNotifyTrackPadding)))
	[
		TrackNode->AsShared()
	];
}

#pragma endregion Render



#pragma region Widget
float SBXTLExtraTrack::GetLastClickedTime() const
{ 
	return LastClickedTime; 
}

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
	Size.Y = FBXTLExtraTrackPanel::NotificationTrackHeight;
	return Size;
}

bool SBXTLExtraTrack::SupportsKeyboardFocus() const
{
	return true;
}

FReply SBXTLExtraTrack::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	FVector2D CursorPos = MouseEvent.GetScreenSpacePosition();
	CursorPos = MyGeometry.AbsoluteToLocal(CursorPos);

	if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		TrackNode->SetLastMouseDownPosition(CursorPos);

		return FReply::Handled().DetectDrag(TrackNode.ToSharedRef(), EKeys::LeftMouseButton);
	}

	return FReply::Handled();
}

FReply SBXTLExtraTrack::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	bool bLeftMouseButton = MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton;
	bool bRightMouseButton = MouseEvent.GetEffectingButton() == EKeys::RightMouseButton;

	if (bLeftMouseButton)
	{
		FVector2D CursorPos = MouseEvent.GetScreenSpacePosition();
		CursorPos = MyGeometry.AbsoluteToLocal(CursorPos);

		LastClickedTime = CalculateTime(MyGeometry, MouseEvent.GetScreenSpacePosition());

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

float SBXTLExtraTrack::CalculateTime(const FGeometry& MyGeometry, FVector2D NodePos, bool bInputIsAbsolute)
{
	if (bInputIsAbsolute)
	{
		NodePos = MyGeometry.AbsoluteToLocal(NodePos);
	}

	FTrackScaleInfo ScaleInfo(ViewInputMin.Get(), ViewInputMax.Get(), 0, 0, MyGeometry.Size);
	return FMath::Clamp<float>(ScaleInfo.LocalXToInput(NodePos.X), 0.f, TimelinePlayLength);
}

FMargin SBXTLExtraTrack::GetNotifyTrackPadding() const
{
	float LeftMargin = TrackNode->GetWidgetPosition().X;
	float RightMargin = CachedGeometry.GetLocalSize().X - TrackNode->GetWidgetPosition().X - TrackNode->GetSize().X;

	return FMargin(LeftMargin, 0, RightMargin, 0);
}

FReply SBXTLExtraTrack::OnNotifyNodeDragStarted(TSharedRef<SBXTLExtraTrackNode> NotifyNode, const FPointerEvent& MouseEvent, const FVector2D& ScreenNodePosition, const bool bDragOnMarker, int32 NotifyIndex)
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

#pragma endregion Widget

#undef LOCTEXT_NAMESPACE
