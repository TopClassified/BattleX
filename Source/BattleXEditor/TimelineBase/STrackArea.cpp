#include "STrackArea.h"
#include "Types/PaintArgs.h"
#include "EditorStyleSet.h"
#include "Rendering/DrawElements.h"

#include "Layout/LayoutUtils.h"
#include "Layout/ArrangedChildren.h"
#include "Widgets/SWeakWidget.h"

#include "STrack.h"
#include "SOutliner.h"
#include "TimelineTrack.h"
#include "TimelineController.h"
#include "TimeSliderController.h"



FTrackAreaSlot::FTrackAreaSlot(const TSharedPtr<STrack>& InSlotContent)
{
	TrackWidget = InSlotContent;

	HAlignment = HAlign_Fill;
	VAlignment = VAlign_Top;

	AttachWidget(SNew(SWeakWidget).Clipping(EWidgetClipping::ClipToBounds).PossiblyNullContent(InSlotContent));
}

float FTrackAreaSlot::GetVerticalOffset() const
{
	TSharedPtr<STrack> PinnedTrackWidget = TrackWidget.Pin();

	return PinnedTrackWidget.IsValid() ? PinnedTrackWidget->GetPhysicalPosition() : 0.f;
}






void STrackArea::Construct(const FArguments& InArgs, const TSharedRef<FTimelineController>& InTimelineController, const TSharedRef<FTimeSliderController>& InTimeSliderController)
{
	WeakEditorTimelineController = InTimelineController;
	WeakTimeSliderController = InTimeSliderController;
}

void STrackArea::SetOutliner(const TSharedPtr<SOutliner>& InOutliner)
{
	WeakOutliner = InOutliner;
}

void STrackArea::Empty()
{
	TrackSlots.Empty();
	Children.Empty();
}

void STrackArea::AddTrackSlot(const TSharedRef<FTimelineTrack>& InTrack, const TSharedPtr<STrack>& InSlot)
{
	TrackSlots.Add(InTrack, InSlot);
	Children.AddSlot(FTrackAreaSlot::FSlotArguments(MakeUnique<FTrackAreaSlot>(InSlot)));
}

TSharedPtr<STrack> STrackArea::FindTrackSlot(const TSharedRef<FTimelineTrack>& InTrack)
{
	// Remove stale entries
	TrackSlots.Remove(TWeakPtr<FTimelineTrack>());

	return TrackSlots.FindRef(InTrack).Pin();
}

void STrackArea::OnArrangeChildren(const FGeometry& AllottedGeometry, FArrangedChildren& ArrangedChildren) const
{
	struct Temp
	{
		Temp(int32 InID, EVisibility InVis, const AlignmentArrangeResult& InX, const AlignmentArrangeResult& InY) : Index(InID), Vis(InVis), XResult(InX), YResult(InY) {}

		int32 Index;
		EVisibility Vis;
		AlignmentArrangeResult XResult;
		AlignmentArrangeResult YResult;
	};

	TArray<Temp> TempMessages;
	for (int32 ChildIndex = 0; ChildIndex < Children.Num(); ++ChildIndex)
	{
		const FTrackAreaSlot& CurChild = Children[ChildIndex];

		const EVisibility ChildVisibility = CurChild.GetWidget()->GetVisibility();
		if (!ArrangedChildren.Accepts(ChildVisibility))
		{
			continue;
		}

		const FMargin Padding(0, CurChild.GetVerticalOffset(), 0, 0);
		AlignmentArrangeResult XResult = AlignChild<Orient_Horizontal>(AllottedGeometry.GetLocalSize().X, CurChild, Padding, 1.0f, false);
		AlignmentArrangeResult YResult = AlignChild<Orient_Vertical>(AllottedGeometry.GetLocalSize().Y, CurChild, Padding, 1.0f, false);
		YResult.Size = FTimelineTrack::TimelineTrackHeight;

		TempMessages.Add(Temp(ChildIndex, ChildVisibility, XResult, YResult));
	}
	TempMessages.Sort
	(
		[&](const Temp& A, const Temp& B)
		{
			return A.YResult.Offset < B.YResult.Offset;
		}
	);

	for (int32 i = 0; i < TempMessages.Num(); ++i)
	{
		Temp& TT = TempMessages[i];
		ArrangedChildren.AddWidget(TT.Vis, AllottedGeometry.MakeChild(Children[TT.Index].GetWidget(), FVector2D(TT.XResult.Offset, TT.YResult.Offset), FVector2D(TT.XResult.Size, TT.YResult.Size)));
	}
}

FVector2D STrackArea::ComputeDesiredSize(float) const
{
	FVector2D MaxSize(0.0f, 0.0f);
	for (int32 ChildIndex = 0; ChildIndex < Children.Num(); ++ChildIndex)
	{
		const FTrackAreaSlot& CurChild = Children[ChildIndex];

		const EVisibility ChildVisibilty = CurChild.GetWidget()->GetVisibility();
		if (ChildVisibilty != EVisibility::Collapsed)
		{
			FVector2D ChildDesiredSize = CurChild.GetWidget()->GetDesiredSize();
			MaxSize.X = FMath::Max(MaxSize.X, ChildDesiredSize.X);
			MaxSize.Y = FMath::Max(MaxSize.Y, ChildDesiredSize.Y);
		}
	}

	return MaxSize;
}

FChildren* STrackArea::GetChildren()
{
	return &Children;
}

int32 STrackArea::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled ) const
{
	// paint the child widgets
	FArrangedChildren ArrangedChildren(EVisibility::Visible);
	ArrangeChildren(AllottedGeometry, ArrangedChildren);

	const FPaintArgs NewArgs = Args.WithNewParent(this);

	for (int32 ChildIndex = 0; ChildIndex < ArrangedChildren.Num(); ++ChildIndex)
	{
		FArrangedWidget& CurWidget = ArrangedChildren[ChildIndex];
		FSlateRect ChildClipRect = MyCullingRect.IntersectionWith(CurWidget.Geometry.GetLayoutBoundingRect());
		const int32 ThisWidgetLayerId = CurWidget.Widget->Paint(NewArgs, CurWidget.Geometry, ChildClipRect, OutDrawElements, LayerId + 2, InWidgetStyle, bParentEnabled);

		LayerId = FMath::Max(LayerId, ThisWidgetLayerId);
	}

	return LayerId;
}

void STrackArea::UpdateHoverStates(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{

}

FReply STrackArea::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	TSharedPtr<FTimeSliderController> TimeSliderController = WeakTimeSliderController.Pin();
	if(TimeSliderController.IsValid())
	{
		WeakOutliner.Pin()->ClearSelection();
		WeakEditorTimelineController.Pin()->ClearTrackSelection();

		TimeSliderController->OnMouseButtonDown(*this, MyGeometry, MouseEvent);
	}

	return FReply::Unhandled();
}

FReply STrackArea::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	TSharedPtr<FTimeSliderController> TimeSliderController = WeakTimeSliderController.Pin();
	if(TimeSliderController.IsValid())
	{
		return WeakTimeSliderController.Pin()->OnMouseButtonUp(*this, MyGeometry, MouseEvent);
	}

	return FReply::Unhandled();
}

FReply STrackArea::OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	UpdateHoverStates(MyGeometry, MouseEvent);

	TSharedPtr<FTimeSliderController> TimeSliderController = WeakTimeSliderController.Pin();
	if(TimeSliderController.IsValid())
	{
		FReply Reply = WeakTimeSliderController.Pin()->OnMouseMove(*this, MyGeometry, MouseEvent);

		// Handle right click scrolling on the track area
		if (Reply.IsEventHandled())
		{
			if (MouseEvent.IsMouseButtonDown(EKeys::RightMouseButton) && HasMouseCapture())
			{
				WeakOutliner.Pin()->ScrollByDelta(-MouseEvent.GetCursorDelta().Y);
			}
		}

		return Reply;
	}

	return FReply::Unhandled();
}

FReply STrackArea::OnMouseWheel(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	TSharedPtr<FTimeSliderController> TimeSliderController = WeakTimeSliderController.Pin();
	if(TimeSliderController.IsValid())
	{
		FReply Reply = WeakTimeSliderController.Pin()->OnMouseWheel(*this, MyGeometry, MouseEvent);
		if (Reply.IsEventHandled())
		{
			return Reply;
		}

		const float ScrollAmount = -MouseEvent.GetWheelDelta() * GetGlobalScrollAmount();
		WeakOutliner.Pin()->ScrollByDelta(ScrollAmount);

		return FReply::Handled();
	}

	return FReply::Unhandled();
}

void STrackArea::OnMouseLeave(const FPointerEvent& MouseEvent)
{

}

FCursorReply STrackArea::OnCursorQuery(const FGeometry& MyGeometry, const FPointerEvent& CursorEvent) const
{
	if (CursorEvent.IsMouseButtonDown(EKeys::RightMouseButton) && HasMouseCapture())
	{
		return FCursorReply::Cursor(EMouseCursor::GrabHandClosed);
	}
	else
	{
		TSharedPtr<FTimeSliderController> TimeSliderController = WeakTimeSliderController.Pin();
		if(TimeSliderController.IsValid())
		{
			return TimeSliderController->OnCursorQuery(SharedThis(this), MyGeometry, CursorEvent);
		}
	}

	return FCursorReply::Unhandled();
}

void STrackArea::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	CachedGeometry = AllottedGeometry;

	for (int32 Index = 0; Index < Children.Num();)
	{
		if (!StaticCastSharedRef<SWeakWidget>(Children[Index].GetWidget())->ChildWidgetIsValid())
		{
			Children.RemoveAt(Index);
		}
		else
		{
			++Index;
		}
	}
}
