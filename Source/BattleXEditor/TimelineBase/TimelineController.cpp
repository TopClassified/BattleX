#include "TimelineController.h"
#include "TimelineTrack.h"
#include "Preferences/PersonaOptions.h"



#pragma region Important
void FTimelineController::RefreshTracks()
{

}

void FTimelineController::BuildContextMenu(FMenuBuilder& InMenuBuilder)
{
	TSet<FName> ExistingMenuTypes;
	for (const TSharedRef<FTimelineTrack>& SelectedItem : SelectedTracks)
	{
		SelectedItem->AddToContextMenu(InMenuBuilder, ExistingMenuTypes);
	}
}

#pragma endregion Important



#pragma region Parameter
TArray<TSharedRef<FTimelineTrack>>& FTimelineController::GetRootTracks()
{ 
	return RootTracks; 
}

const TArray<TSharedRef<FTimelineTrack>>& FTimelineController::GetRootTracks() const
{ 
	return RootTracks; 
}

FAnimatedRange FTimelineController::GetViewRange() const
{
	return ViewRange;
}

void FTimelineController::SetViewRange(TRange<double> InRange)
{
	ViewRange = InRange;

	if (WorkingRange.HasLowerBound() && WorkingRange.HasUpperBound())
	{
		WorkingRange = TRange<double>::Hull(WorkingRange, ViewRange);
	}
	else
	{
		WorkingRange = ViewRange;
	}
}

void FTimelineController::SnapViewToPlayRange()
{
	const double Resolution = GetTickResolution();

	TRange<FFrameNumber> PlaybackRange = GetPlaybackRange();

	SetViewRange(TRange<double>(PlaybackRange.GetLowerBoundValue().Value / Resolution, PlaybackRange.GetUpperBoundValue().Value / Resolution));
}

FAnimatedRange FTimelineController::GetWorkingRange() const
{
	return WorkingRange;
}

TRange<FFrameNumber> FTimelineController::GetPlaybackRange() const
{
	auto PlaybackRange = FAnimatedRange(0.0f, (double)GetPlayLength());

	const int32 Resolution = GetTickResolution();

	return TRange<FFrameNumber>(FFrameNumber((int32)FMath::RoundToInt(PlaybackRange.GetLowerBoundValue() * Resolution)), FFrameNumber((int32)FMath::RoundToInt(PlaybackRange.GetUpperBoundValue() * Resolution)));
}

FFrameNumber FTimelineController::GetScrubPosition() const
{
	return FFrameNumber();
}

void FTimelineController::SetScrubPosition(FFrameTime NewScrubPostion) const
{

}

float FTimelineController::GetPlayLength() const
{
	return 0.0f;
}

double FTimelineController::GetFrameRate() const
{
	return 0.1f;
}

int32 FTimelineController::GetTickResolution() const
{
	float Resolution = GetDefault<UPersonaOptions>()->TimelineScrubSnapValue * GetFrameRate();

	return FMath::RoundToInt(Resolution);
}

void FTimelineController::HandleViewRangeChanged(TRange<double> InRange, EViewRangeInterpolation InInterpolation)
{
	SetViewRange(InRange);
}

void FTimelineController::HandleWorkingRangeChanged(TRange<double> InRange)
{
	WorkingRange = InRange;
}

bool FTimelineController::IsTrackSelected(const TSharedRef<FTimelineTrack>& InTrack) const
{
	return SelectedTracks.Find(InTrack) != nullptr;
}

void FTimelineController::ClearTrackSelection()
{
	SelectedTracks.Empty();

	if (!bIsSelecting)
	{
		TGuardValue<bool> GuardValue(bIsSelecting, true);
	}

	PostClearTrackSelection();

}

void FTimelineController::SetTrackSelected(const TSharedRef<FTimelineTrack>& InTrack, bool bIsSelected)
{
	if (bIsSelected)
	{
		SelectedTracks.Add(InTrack);
	}
	else
	{
		SelectedTracks.Remove(InTrack);
	}
}

void FTimelineController::SelectObjects(const TArray<UObject*>& Objects)
{
	
}

void FTimelineController::PostClearTrackSelection()
{

}

#pragma endregion Parameter
