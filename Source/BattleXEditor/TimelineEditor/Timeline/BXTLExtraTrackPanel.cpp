#include "BXTLExtraTrackPanel.h"

#include "BXTLController.h"
#include "SBXTLExtraTrack.h" 



#define LOCTEXT_NAMESPACE "FBXTLExtraTrackPanel"

TIMELINE_IMPLEMENT_TRACK(FBXTLExtraTrackPanel);

#pragma region Important
FBXTLExtraTrackPanel::FBXTLExtraTrackPanel
(
	const TSharedRef<FBXTLController>& InModel, UBXTLAsset* InAsset, int32 InSectionIndex,
	EBXTLExtraType InTrackType, const FText& InDisplayName, const FText& InToolTipText
) : FTimelineTrack(InModel, InDisplayName, InToolTipText), CachedAsset(InAsset), SectionIndex(InSectionIndex), TrackType(InTrackType)
{
	SetHeight(FTimelineTrack::TimelineSubTrackHeight);
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
			.RefreshPanelEvent(this, &FBXTLExtraTrackPanel::OnRefreshPanel)
			.SetInputViewRangeEvent(this, &FBXTLExtraTrackPanel::OnInputViewRangeChanged);
	}

	return TrackWidget.ToSharedRef();
}

#pragma endregion Important



#pragma region Callback
void FBXTLExtraTrackPanel::OnRefreshPanel()
{
	SetHeight(FTimelineTrack::TimelineSubTrackHeight);

	if (TrackWidget.IsValid())
	{
		TrackWidget->UpdateLayout();
	}
}

void FBXTLExtraTrackPanel::OnInputViewRangeChanged(float ViewMin, float ViewMax)
{

}

#pragma endregion Callback

#undef LOCTEXT_NAMESPACE