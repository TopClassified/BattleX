#pragma once

#include "CoreMinimal.h"
#include "Misc/Attribute.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SCompoundWidget.h"
#include "TimeSliderController.h" 

class FPaintArgs;
class FSlateWindowElementList;
class FTimeSliderController;

/**
 * An overlay that displays global information in the track area
 */
class BATTLEXEDITOR_API STimelineOverlay : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(STimelineOverlay) : _DisplayTickLines(true), _DisplayScrubPosition(false) {}
		SLATE_ATTRIBUTE(bool, DisplayTickLines)
		SLATE_ATTRIBUTE(bool, DisplayScrubPosition)
		SLATE_ATTRIBUTE(FPaintPlaybackRangeArgs, PaintPlaybackRangeArgs)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, TSharedRef<FTimeSliderController> InTimeSliderController)
	{
		bDisplayScrubPosition = InArgs._DisplayScrubPosition;
		bDisplayTickLines = InArgs._DisplayTickLines;
		PaintPlaybackRangeArgs = InArgs._PaintPlaybackRangeArgs;
		TimeSliderController = InTimeSliderController;
	}

private:
	/** SWidget Interface */
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

private:
	/** Controller for manipulating time */
	TSharedPtr<FTimeSliderController> TimeSliderController;

	/** Whether or not to display the scrub position */
	TAttribute<bool> bDisplayScrubPosition;

	/** Whether or not to display tick lines */
	TAttribute<bool> bDisplayTickLines;

	/** User-supplied options for drawing playback range */
	TAttribute<FPaintPlaybackRangeArgs> PaintPlaybackRangeArgs;

};
