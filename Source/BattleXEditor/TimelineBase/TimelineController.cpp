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

void FTimelineController::SetScrubPosition(FFrameTime NewScrubPosition) const
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

	// SnapValue默认0或极小值时取整为0,FFrameRate(0,1)会令时间轴全部坐标换算产生inf/0(scrub/缩放全失效)
	return FMath::Max(FMath::RoundToInt(Resolution), 1);
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

	// guard须覆盖PostClearTrackSelection(其内部可重入调用本函数):原实现guard在if块末尾即析构,重入保护完全失效
	if (!bIsSelecting)
	{
		TGuardValue<bool> GuardValue(bIsSelecting, true);
		PostClearTrackSelection();
	}
	else
	{
		PostClearTrackSelection();
	}
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
