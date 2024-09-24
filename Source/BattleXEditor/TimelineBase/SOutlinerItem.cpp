#include "SOutlinerItem.h"
#include "TimelineTrack.h"
#include "Widgets/Text/STextBlock.h"
#include "SOutliner.h"
#include "Widgets/SOverlay.h"



SOutlinerItem::~SOutlinerItem()
{
	TSharedPtr<SOutliner> Outliner = StaticCastSharedPtr<SOutliner>(OwnerTablePtr.Pin());
	TSharedPtr<FTimelineTrack> PinnedTrack = Track.Pin();
	if (Outliner.IsValid() && PinnedTrack.IsValid())
	{
		Outliner->OnChildRowRemoved(PinnedTrack.ToSharedRef());
	}
}

void SOutlinerItem::Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView, const TSharedRef<FTimelineTrack>& InTrack)
{
	Track = InTrack;
	OnGenerateWidgetForColumn = InArgs._OnGenerateWidgetForColumn;
	HighlightText = InArgs._HighlightText;

	SMultiColumnTableRow::Construct
	(
		SMultiColumnTableRow::FArguments().ShowSelection(true).OnCanAcceptDrop(this, &SOutlinerItem::OnCanAcceptDrop).OnAcceptDrop(this, &SOutlinerItem::OnAcceptDrop),
		InOwnerTableView
	);
}

TSharedRef<SWidget> SOutlinerItem::GenerateWidgetForColumn(const FName& ColumnId)
{
	TSharedPtr<FTimelineTrack> PinnedTrack = Track.Pin();
	if (PinnedTrack.IsValid())
	{
		TSharedPtr<SWidget> ColumnWidget = SNullWidget::NullWidget;
		if(OnGenerateWidgetForColumn.IsBound())
		{
			ColumnWidget = OnGenerateWidgetForColumn.Execute(PinnedTrack.ToSharedRef(), ColumnId, SharedThis(this));
		}

		return SNew(SOverlay)
		+SOverlay::Slot()
		[
			ColumnWidget.ToSharedRef()
		];
	}

	return SNullWidget::NullWidget;
}

TOptional<EItemDropZone> SOutlinerItem::OnCanAcceptDrop(const FDragDropEvent& DragDropEvent, EItemDropZone InItemDropZone, TSharedRef<FTimelineTrack> InItem)
{
	return Track.Pin()->OnCanAcceptDrop(DragDropEvent, InItemDropZone, InItem);
}

FReply SOutlinerItem::OnAcceptDrop(const FDragDropEvent& DragDropEvent, EItemDropZone DropZone, TSharedRef<FTimelineTrack> TargetItem)
{
	return Track.Pin()->OnAcceptDrop(DragDropEvent, DropZone, TargetItem);
}

void SOutlinerItem::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	StaticCastSharedPtr<SOutliner>(OwnerTablePtr.Pin())->ReportChildRowGeometry(Track.Pin().ToSharedRef(), AllottedGeometry);
}

FVector2D SOutlinerItem::ComputeDesiredSize(float LayoutScaleMultiplier) const
{
	TSharedPtr<FTimelineTrack> PinnedTrack = Track.Pin();
	if(PinnedTrack.IsValid())
	{
		return FVector2D(100.0f, PinnedTrack->GetHeight() + PinnedTrack->GetPadding().Combined());
	}

	return FVector2D(100.0f, 16.0f);
}

void SOutlinerItem::AddTrackAreaReference(const TSharedPtr<STrack>& InTrackWidget)
{
	TrackWidget = InTrackWidget;
}

void SOutlinerItem::OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	bHovered = true;
	SMultiColumnTableRow<TSharedRef<FTimelineTrack>>::OnMouseEnter(MyGeometry, MouseEvent);

	TSharedPtr<FTimelineTrack> PinnedTrack = Track.Pin();
	if(PinnedTrack.IsValid())
	{
		PinnedTrack->SetHovered(true);
	}
}

void SOutlinerItem::OnMouseLeave(const FPointerEvent& MouseEvent)
{
	bHovered = false;

	SMultiColumnTableRow<TSharedRef<FTimelineTrack>>::OnMouseLeave(MouseEvent);

	TSharedPtr<FTimelineTrack> PinnedTrack = Track.Pin();
	if(PinnedTrack.IsValid())
	{
		PinnedTrack->SetHovered(false);
	}
}