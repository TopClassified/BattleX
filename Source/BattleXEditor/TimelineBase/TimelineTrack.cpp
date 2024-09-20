#include "TimelineTrack.h"
#include "SOutlinerItem.h"
#include "EditorStyleSet.h"
#include "Preferences/PersonaOptions.h"

#include "Widgets/SOverlay.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Input/SCheckBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Views/SExpanderArrow.h"
#include "Widgets/Views/STableViewBase.h"
 


#define LOCTEXT_NAMESPACE "FTimelineTrack"

const float FTimelineTrack::OutlinerRightPadding = 10.0f;
const float FTimelineTrack::TimelineTrackHeight = 32.0f;
const float FTimelineTrack::TimelineSubTrackHeight = 16.0f;

TIMELINE_IMPLEMENT_TRACK(FTimelineTrack);

FText FTimelineTrack::GetLabel() const
{
	return DisplayName;
}

FText FTimelineTrack::GetToolTipText() const
{
	return ToolTipText;
}

bool FTimelineTrack::Traverse_ChildFirst(const TFunctionRef<bool(FTimelineTrack&)>& InPredicate, bool bIncludeThisTrack)
{
	for (TSharedRef<FTimelineTrack>& Child : Children)
	{
		if (!Child->Traverse_ChildFirst(InPredicate, true))
		{
			return false;
		}
	}

	return bIncludeThisTrack ? InPredicate(*this) : true;
}

bool FTimelineTrack::Traverse_ParentFirst(const TFunctionRef<bool(FTimelineTrack&)>& InPredicate, bool bIncludeThisTrack)
{
	if (bIncludeThisTrack && !InPredicate(*this))
	{
		return false;
	}

	for (TSharedRef<FTimelineTrack>& Child : Children)
	{
		if (!Child->Traverse_ParentFirst(InPredicate, true))
		{
			return false;
		}
	}

	return true;
}

bool FTimelineTrack::TraverseVisible_ChildFirst(const TFunctionRef<bool(FTimelineTrack&)>& InPredicate, bool bIncludeThisTrack)
{
	// If the item is not expanded, its children ain't visible
	if (IsExpanded())
	{
		for (TSharedRef<FTimelineTrack>& Child : Children)
		{
			if (Child->IsVisible() && !Child->TraverseVisible_ChildFirst(InPredicate, true))
			{
				return false;
			}
		}
	}

	if (bIncludeThisTrack && IsVisible())
	{
		return InPredicate(*this);
	}

	// Continue iterating regardless of visibility
	return true;
}

bool FTimelineTrack::TraverseVisible_ParentFirst(const TFunctionRef<bool(FTimelineTrack&)>& InPredicate, bool bIncludeThisTrack)
{
	if (bIncludeThisTrack && IsVisible() && !InPredicate(*this))
	{
		return false;
	}

	// If the item is not expanded, its children ain't visible
	if (IsExpanded())
	{
		for (TSharedRef<FTimelineTrack>& Child : Children)
		{
			if (Child->IsVisible() && !Child->TraverseVisible_ParentFirst(InPredicate, true))
			{
				return false;
			}
		}
	}

	return true;
}

TSharedRef<SWidget> FTimelineTrack::GenerateContainerWidgetForOutliner(const TSharedRef<SOutlinerItem>& InRow)
{
	TSharedPtr<SBorder> OuterBorder;
	TSharedPtr<SHorizontalBox> InnerHorizontalBox;
	TSharedRef<SWidget> Widget = GenerateStandardOutlinerWidget(InRow, true, OuterBorder, InnerHorizontalBox);

	if(bIsHeaderTrack)
	{
		OuterBorder->SetBorderBackgroundColor(FAppStyle::GetColor("AnimTimeline.Outliner.HeaderColor"));
	}

	return Widget;
}

TSharedRef<SWidget> FTimelineTrack::GenerateStandardOutlinerWidget(const TSharedRef<SOutlinerItem>& InRow, bool bWithLabelText, TSharedPtr<SBorder>& OutOuterBorder, TSharedPtr<SHorizontalBox>& OutInnerHorizontalBox)
{
	TSharedRef<SWidget> Widget = SAssignNew(OutOuterBorder, SBorder)
		.ToolTipText(this, &FTimelineTrack::GetToolTipText)
		.BorderImage(FAppStyle::GetBrush("Sequencer.Section.BackgroundTint"))
		.BorderBackgroundColor(FAppStyle::GetColor("AnimTimeline.Outliner.ItemColor"))
		[
			SAssignNew(OutInnerHorizontalBox, SHorizontalBox)
			+SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			.AutoWidth()
			.Padding(4.0f, 1.0f)
			[
				SNew(SExpanderArrow, InRow)
			]
		];

	if(bWithLabelText)
	{
		OutInnerHorizontalBox->AddSlot()
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Left)
		.Padding(2.0f, 1.0f)
		.FillWidth(1.0f)
		[
			SNew(STextBlock)
			.TextStyle(&FAppStyle::Get().GetWidgetStyle<FTextBlockStyle>("AnimTimeline.Outliner.Label"))
			.Text(this, &FTimelineTrack::GetLabel)
			.HighlightText(InRow->GetHighlightText())
		];
	}

	return Widget;
}

TOptional<EItemDropZone> FTimelineTrack::OnCanAcceptDrop(const FDragDropEvent& DragDropEvent, EItemDropZone InItemDropZone, TSharedRef<FTimelineTrack> InItem)
{
	return TOptional<EItemDropZone>();
}

FReply FTimelineTrack::OnAcceptDrop(const FDragDropEvent& DragDropEvent, EItemDropZone DropZone, TSharedRef<FTimelineTrack> TargetItem)
{
	return FReply::Unhandled();
}

TSharedRef<SWidget> FTimelineTrack::GenerateContainerWidgetForTimeline()
{
	return SNullWidget::NullWidget;
}

void FTimelineTrack::AddToContextMenu(FMenuBuilder& InMenuBuilder, TSet<FName>& InOutExistingMenuTypes) const
{

}

float FTimelineTrack::GetMaxInput() const
{
	return GetEditorTimelineController()->GetPlayLength();
}

float FTimelineTrack::GetViewMinInput() const
{
	return GetEditorTimelineController()->GetViewRange().GetLowerBoundValue();
}

float FTimelineTrack::GetViewMaxInput() const
{
	return GetEditorTimelineController()->GetViewRange().GetUpperBoundValue();
}

float FTimelineTrack::GetScrubValue() const
{
	const int32 Resolution = FMath::RoundToInt((double)GetDefault<UPersonaOptions>()->TimelineScrubSnapValue * GetEditorTimelineController()->GetFrameRate());
	return (float)((double)GetEditorTimelineController()->GetScrubPosition().Value / (double)Resolution);
}

void FTimelineTrack::SelectObjects(const TArray<UObject*>& SelectedItems)
{
	GetEditorTimelineController()->SelectObjects(SelectedItems);
}

void FTimelineTrack::OnSetInputViewRange(float ViewMin, float ViewMax)
{
	GetEditorTimelineController()->SetViewRange(TRange<double>(ViewMin, ViewMax));
}

#undef LOCTEXT_NAMESPACE