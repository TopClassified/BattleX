#include "SOutliner.h"
#include "TimelineController.h"
#include "TimelineTrack.h"
#include "SOutlinerItem.h"
#include "STrackArea.h"
#include "Widgets/Input/SButton.h"
#include "STrack.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Misc/TextFilterExpressionEvaluator.h"



#define LOCTEXT_NAMESPACE "SOutliner"

SOutliner::~SOutliner()
{
	if(EditorTimelineController.IsValid())
	{
		EditorTimelineController.Pin()->OnTracksChanged().Remove(TracksChangedDelegateHandle);
	}
}

void SOutliner::Construct(const FArguments& InArgs, const TSharedRef<FTimelineController>& InTimelineController, const TSharedRef<STrackArea>& InTrackArea)
{	
	EditorTimelineController = InTimelineController;
	TrackArea = InTrackArea;
	FilterText = InArgs._FilterText;
	bPhysicalTracksNeedUpdate = false;

	TracksChangedDelegateHandle = InTimelineController->OnTracksChanged().AddSP(this, &SOutliner::HandleTracksChanged);

	TextFilter = MakeShareable(new FTextFilterExpressionEvaluator(ETextFilterExpressionEvaluatorMode::BasicString));

	HeaderRow = SNew(SHeaderRow).Visibility(EVisibility::Collapsed);

	HeaderRow->AddColumn(SHeaderRow::Column(TEXT("Outliner")).FillWidth(1.0f));

	STreeView::Construct
	(
		STreeView::FArguments()
		.TreeItemsSource(&InTimelineController->GetRootTracks())
		.SelectionMode(ESelectionMode::Multi)
		.OnGenerateRow(this, &SOutliner::HandleGenerateRow) 
		.OnGetChildren(this, &SOutliner::HandleGetChildren)
		.HeaderRow(HeaderRow)
		.ExternalScrollbar(InArgs._ExternalScrollbar)
		.OnExpansionChanged(this, &SOutliner::HandleExpansionChanged)
		.AllowOverscroll(EAllowOverscroll::No)
		.OnContextMenuOpening(this, &SOutliner::HandleContextMenuOpening)
	);

	// expand all
	for(const TSharedRef<FTimelineTrack>& RootTrack : InTimelineController->GetRootTracks())
	{
		RootTrack->Traverse_ParentFirst([this](FTimelineTrack& InTrack){ SetItemExpansion(InTrack.AsShared(), InTrack.IsExpanded()); return true; });
	}
}

void SOutliner::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	STreeView::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

	// These are updated in both tick and paint since both calls can cause changes to the cached rows and the data needs
	// to be kept synchronized so that external measuring calls get correct and reliable results.
	if (bPhysicalTracksNeedUpdate)
	{
		PhysicalTracks.Reset();
		CachedTrackGeometry.GenerateValueArray(PhysicalTracks);

		PhysicalTracks.Sort
		(
			[](const FCachedGeometry& A, const FCachedGeometry& B)
			{
				return A.Top < B.Top;
			}
		);

		bPhysicalTracksNeedUpdate = false;
	}
}

int32 SOutliner::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	LayerId = STreeView::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

	// These are updated in both tick and paint since both calls can cause changes to the cached rows and the data needs
	// to be kept synchronized so that external measuring calls get correct and reliable results.
	if (bPhysicalTracksNeedUpdate)
	{
		PhysicalTracks.Reset();
		CachedTrackGeometry.GenerateValueArray(PhysicalTracks);

		PhysicalTracks.Sort
		(
			[](const FCachedGeometry& A, const FCachedGeometry& B) 
			{
				return A.Top < B.Top;
			}
		);

		bPhysicalTracksNeedUpdate = false;
	}

	return LayerId + 1;
}

TSharedRef<ITableRow> SOutliner::HandleGenerateRow(TSharedRef<FTimelineTrack> InTrack, const TSharedRef<STableViewBase>& OwnerTable)
{
	TSharedRef<SOutlinerItem> Row = SNew(SOutlinerItem, OwnerTable, InTrack).OnGenerateWidgetForColumn(this, &SOutliner::GenerateWidgetForColumn).HighlightText(FilterText);

	// Ensure the track area is kept up to date with the virtualized scroll of the tree view
	TSharedPtr<STrack> TrackWidget = TrackArea->FindTrackSlot(InTrack);

	if (!TrackWidget.IsValid())
	{
		// Add a track slot for the row
		TrackWidget = SNew(STrack, InTrack, SharedThis(this))[InTrack->GenerateContainerWidgetForTimeline()];

		TrackArea->AddTrackSlot(InTrack, TrackWidget);
	}

	if (ensure(TrackWidget.IsValid()))
	{
		Row->AddTrackAreaReference(TrackWidget);
	}

	return Row;
}

TSharedRef<SWidget> SOutliner::GenerateWidgetForColumn(const TSharedRef<FTimelineTrack>& InTrack, const FName& ColumnId, const TSharedRef<SOutlinerItem>& Row) const
{
	return InTrack->GenerateContainerWidgetForOutliner(Row);
}

void SOutliner::HandleGetChildren(TSharedRef<FTimelineTrack> Item, TArray<TSharedRef<FTimelineTrack>>& OutChildren)
{
	if(!FilterText.Get().IsEmpty())
	{
		for(const TSharedRef<FTimelineTrack>& Child : Item->GetChildren())
		{
			if(!Child->SupportsFiltering() || TextFilter->TestTextFilter(FBasicStringFilterExpressionContext(Child->GetLabel().ToString())))
			{
				OutChildren.Add(Child);
			}
		}
	}
	else
	{
		OutChildren.Append(Item->GetChildren());
	}
}

void SOutliner::HandleExpansionChanged(TSharedRef<FTimelineTrack> InTrack, bool bIsExpanded)
{
	InTrack->SetExpanded(bIsExpanded);
	
	// Expand any children that are also expanded
	for (const TSharedRef<FTimelineTrack>& Child : InTrack->GetChildren())
	{
		if (Child->IsExpanded())
		{
			SetItemExpansion(Child, true);
		}
	}
}

TSharedPtr<SWidget> SOutliner::HandleContextMenuOpening()
{
	const bool bShouldCloseWindowAfterMenuSelection = true;
	FMenuBuilder MenuBuilder(bShouldCloseWindowAfterMenuSelection, nullptr);

	EditorTimelineController.Pin()->BuildContextMenu(MenuBuilder);

	// > 1 because the search widget is always added
	return MenuBuilder.GetMultiBox()->GetBlocks().Num() > 1 ? MenuBuilder.MakeWidget() : TSharedPtr<SWidget>();
}

void SOutliner::HandleTracksChanged()
{
	RequestTreeRefresh();

	// 自动展开
	for (const TSharedRef<FTimelineTrack>& RootTrack : EditorTimelineController.Pin()->GetRootTracks())
	{
		if (RootTrack->IsExpanded())
		{
			RootTrack->Traverse_ParentFirst([this](FTimelineTrack& InTrack) { SetItemExpansion(InTrack.AsShared(), InTrack.IsExpanded()); return true; });
		}
	}
}

void SOutliner::ReportChildRowGeometry(const TSharedRef<FTimelineTrack>& InTrack, const FGeometry& InGeometry)
{
	float ChildOffset = TransformPoint(
		Concatenate
		(
			InGeometry.GetAccumulatedLayoutTransform(),
			GetCachedGeometry().GetAccumulatedLayoutTransform().Inverse()
		),
		FVector2D(0,0)
	).Y;

	FCachedGeometry* ExistingGeometry = CachedTrackGeometry.Find(InTrack);
	if(ExistingGeometry == nullptr || (ExistingGeometry->Top != ChildOffset || ExistingGeometry->Height != InGeometry.Size.Y))
	{
		CachedTrackGeometry.Add(InTrack, FCachedGeometry(InTrack, ChildOffset, InGeometry.Size.Y));
		bPhysicalTracksNeedUpdate = true;
	}
}

void SOutliner::OnChildRowRemoved(const TSharedRef<FTimelineTrack>& InTrack)
{
	CachedTrackGeometry.Remove(InTrack);
	bPhysicalTracksNeedUpdate = true;
}

TOptional<SOutliner::FCachedGeometry> SOutliner::GetCachedGeometryForTrack(const TSharedRef<FTimelineTrack>& InTrack) const
{
	if (const FCachedGeometry* FoundGeometry = CachedTrackGeometry.Find(InTrack))
	{
		return *FoundGeometry;
	}

	return TOptional<FCachedGeometry>();
}

TOptional<float> SOutliner::ComputeTrackPosition(const TSharedRef<FTimelineTrack>& InTrack) const
{
	// Positioning strategy:
	// Attempt to root out any visible track in the specified track's sub-hierarchy, and compute the track's offset from that
	float NegativeOffset = 0.f;
	TOptional<float> Top;
	
	// Iterate parent first until we find a tree view row we can use for the offset height
	auto Iter = [&](FTimelineTrack& InTrack)
	{		
		TOptional<FCachedGeometry> ChildRowGeometry = GetCachedGeometryForTrack(InTrack.AsShared());
		if (ChildRowGeometry.IsSet())
		{
			Top = ChildRowGeometry->Top;
			// Stop iterating
			return false;
		}

		NegativeOffset -= InTrack.GetHeight() + InTrack.GetPadding().Combined();
		return true;
	};

	InTrack->TraverseVisible_ParentFirst(Iter);

	if (Top.IsSet())
	{
		return NegativeOffset + Top.GetValue();
	}

	return Top;
}

void SOutliner::ScrollByDelta(float DeltaInSlateUnits)
{
	ScrollBy(GetCachedGeometry(), DeltaInSlateUnits, EAllowOverscroll::No);
}

void SOutliner::Private_SetItemSelection( TSharedRef<FTimelineTrack> TheItem, bool bShouldBeSelected, bool bWasUserDirected )
{
	if(TheItem->SupportsSelection())
	{
		EditorTimelineController.Pin()->SetTrackSelected(TheItem, bShouldBeSelected);

		STreeView::Private_SetItemSelection(TheItem, bShouldBeSelected, bWasUserDirected);
	}
}

void SOutliner::Private_ClearSelection()
{
	EditorTimelineController.Pin()->ClearTrackSelection();

	STreeView::Private_ClearSelection();
}

void SOutliner::Private_SelectRangeFromCurrentTo( TSharedRef<FTimelineTrack> InRangeSelectionEnd )
{
	STreeView::Private_SelectRangeFromCurrentTo(InRangeSelectionEnd);

	for(TSet<TSharedRef<FTimelineTrack>>::TIterator Iter = SelectedItems.CreateIterator(); Iter; ++Iter)
	{
		if(!(*Iter)->SupportsSelection())
		{
			Iter.RemoveCurrent();
		}
	}

	for(const TSharedRef<FTimelineTrack>& SelectedItem : SelectedItems)
	{
		EditorTimelineController.Pin()->SetTrackSelected(SelectedItem, true);
	}
}

void SOutliner::RefreshFilter()
{
	TextFilter->SetFilterText(FilterText.Get());

	RequestTreeRefresh();
}

#undef LOCTEXT_NAMESPACE