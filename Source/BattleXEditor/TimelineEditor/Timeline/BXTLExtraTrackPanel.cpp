#include "BXTLExtraTrackPanel.h"

#include "BXTLController.h"
#include "SBXTLExtraTrack.h"



#define LOCTEXT_NAMESPACE "FBXTLExtraTrackPanel"

TIMELINE_IMPLEMENT_TRACK(FBXTLExtraTrackPanel);

FBXTLExtraTrackPanel::FBXTLExtraTrackPanel
(
	const TSharedRef<FBXTLController>& InModel, UBXTLAsset* InAsset, int32 InSectionIndex,
	EBXTLExtraType InTrackType, const FText& InDisplayName, const FText& InToolTipText
) : FTimelineTrack(InModel, InDisplayName, InToolTipText), CachedAsset(InAsset), SectionIndex(InSectionIndex), TrackType(InTrackType)
{
	SetHeight(FTimelineTrack::TimelineSubTrackHeight);
}

void FBXTLExtraTrackPanel::UpdateLayout()
{
	SetHeight(FTimelineTrack::TimelineSubTrackHeight);

	if (TrackWidget.IsValid())
	{
		TrackWidget->UpdateLayout();
	}
}

TSharedRef<SWidget> FBXTLExtraTrackPanel::GenerateContainerWidgetForTimeline()
{
	if (!TrackWidget.IsValid())
	{
		TrackWidget = SNew(SBXTLExtraTrack)
			.Asset(CachedAsset.Get())
			.SectionIndex(SectionIndex)
			.TrackType(TrackType)
			.InputMin(this, &FBXTLExtraTrackPanel::GetMinInput)
			.InputMax(this, &FBXTLExtraTrackPanel::GetMaxInput)
			.ViewInputMin(this, &FBXTLExtraTrackPanel::GetViewMinInput)
			.ViewInputMax(this, &FBXTLExtraTrackPanel::GetViewMaxInput)
			.TimelinePlayLength(GetEditorTimelineController()->GetPlayLength())
			.FrameRate(GetEditorTimelineController()->GetFrameRate())
			.OnSetInputViewRange(this, &FBXTLExtraTrackPanel::InputViewRangeChanged)
			.OnRefreshPanel(this, &FBXTLExtraTrackPanel::UpdateLayout);
	}

	return TrackWidget.ToSharedRef();
}

void FBXTLExtraTrackPanel::InputViewRangeChanged(float ViewMin, float ViewMax)
{

}

#undef LOCTEXT_NAMESPACE