#pragma once

#include "CoreMinimal.h"
#include "ITimeSlider.h"
#include "AnimatedRange.h"
#include "Misc/FrameNumber.h"
#include "Framework/MultiBox/MultiBoxBuilder.h" 



class BATTLEXEDITOR_API FTimelineController : public TSharedFromThis<FTimelineController>
{
#pragma region Important
public:
	FTimelineController() {}

	virtual ~FTimelineController() {}

	/** Refresh the tracks we have using our underlying asset */
	virtual void RefreshTracks();

	/** Build a context menu for selected items */
	virtual void BuildContextMenu(FMenuBuilder& InMenuBuilder);

#pragma endregion Important



#pragma region Parameter
public:
	/** Get the root tracks representing the tree */
	virtual TArray<TSharedRef<class FTimelineTrack>>& GetRootTracks();
	virtual const TArray<TSharedRef<class FTimelineTrack>>& GetRootTracks() const;

	/** Get the current view range */
	virtual FAnimatedRange GetViewRange() const;

	/** Set the current view range */
	virtual void SetViewRange(TRange<double> InRange);

	/** Snap ViewRange to PlayRange */
	virtual void SnapViewToPlayRange();

	/** Get the working range of the model's data */
	virtual FAnimatedRange GetWorkingRange() const;

	/** Get the playback range of the model's data */
	virtual TRange<FFrameNumber> GetPlaybackRange() const;

	/** Get the current scrub position */
	virtual FFrameNumber GetScrubPosition() const;

	/** Set the current scrub position */
	virtual void SetScrubPosition(FFrameTime NewScrubPostion) const;

	/** Get the sequence length of the object */
	virtual float GetPlayLength() const;

	/** Get the framerate specified by the anim sequence */
	virtual double GetFrameRate() const;

	/** Get the tick resolution we are displaying at */
	virtual int32 GetTickResolution() const;

	/** Handle the view range being changed */
	virtual void HandleViewRangeChanged(TRange<double> InRange, EViewRangeInterpolation InInterpolation);

	/** Handle the working range being changed */
	virtual void HandleWorkingRangeChanged(TRange<double> InRange);

	/** Get whether a track is selected */
	virtual bool IsTrackSelected(const TSharedRef<class FTimelineTrack>& InTrack) const;

	/** Clear all track selection */
	virtual void ClearTrackSelection();

	/** Set whether a track is selected */
	virtual void SetTrackSelected(const TSharedRef<class FTimelineTrack>& InTrack, bool bIsSelected);

	/** 'Selects' objects and shows them in the details view */
	virtual void SelectObjects(const TArray<UObject*>& Objects);

	/** Clears the detail view of whatever we displayed last */
	virtual void PostClearTrackSelection();

protected:
	/** Tracks used to generate a tree */
	TArray<TSharedRef<class FTimelineTrack>> RootTracks;

	/** Tracks that are selected */
	TSet<TSharedRef<class FTimelineTrack>> SelectedTracks;

	/** The range we are currently viewing */
	FAnimatedRange ViewRange;

	/** The working range of this model, encompassing the view range */
	FAnimatedRange WorkingRange;

	/** Recursion guard for selection */
	bool bIsSelecting;

#pragma endregion Parameter



#pragma region Event
public:
	DECLARE_EVENT(FTimelineController, FOnTracksChanged)

	FOnTracksChanged& OnTracksChanged() { return TracksChangedEvent; }

protected:
	FOnTracksChanged TracksChangedEvent;

#pragma endregion Event

};

