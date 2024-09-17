#pragma once

#include "CoreMinimal.h"
#include "SlotBase.h"
#include "Input/CursorReply.h"
#include "Input/Reply.h"

#include "Layout/Geometry.h"
#include "Layout/Children.h"
#include "Widgets/SPanel.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

class FPaintArgs;
class FArrangedChildren;
class FSlateWindowElementList;
class STrack;
class SAnimOutliner;
class FTimelineTrack;
class FTimelineController;
class FTimeSliderController;



/**
 * Structure representing a slot in the track area.
 */
class BATTLEXEDITOR_API FTrackAreaSlot : public TSlotBase<FTrackAreaSlot>, public TAlignmentWidgetSlotMixin<FTrackAreaSlot>
{
public:
	/** Construction from a track lane */
	FTrackAreaSlot(const TSharedPtr<STrack>& InSlotContent);

	/** Get the vertical position of this slot inside its parent. */
	float GetVerticalOffset() const;

public:
	/** Horizontal/Vertical alignment for the slot. */
	EHorizontalAlignment HAlignment;

	EVerticalAlignment VAlignment;

	/** The track that we represent. */
	TWeakPtr<STrack> TrackWidget;
};



/**
 * The area where tracks are displayed.
 */
class STrackArea : public SPanel
{
public:

	SLATE_BEGIN_ARGS(STrackArea)
	{
		_Clipping = EWidgetClipping::ClipToBounds;
	}
	SLATE_END_ARGS()

	STrackArea() : Children(this) {}

	/** Construct this widget */
	void Construct(const FArguments& InArgs, const TSharedRef<FTimelineController>& InTimelineController, const TSharedRef<FTimeSliderController>& InTimeSliderController);

public:
	/** Empty the track area */
	void Empty();

	/** Add a new track slot to this area for the given node. The slot will be automatically cleaned up when all external references to the supplied slot are removed. */
	void AddTrackSlot(const TSharedRef<FTimelineTrack>& InTrack, const TSharedPtr<STrack>& InTrackWidget);

	/** Attempt to find an existing slot relating to the given node */
	TSharedPtr<STrack> FindTrackSlot(const TSharedRef<FTimelineTrack>& InTrack);

	/** Access the cached geometry for this track area */
	const FGeometry& GetCachedGeometry() const
	{
		return CachedGeometry;
	}

	/** Assign an outliner to this track area. */
	void SetOutliner(const TSharedPtr<SOutliner>& InOutliner);

public:
	/** SWidget interface */
	virtual FReply OnMouseButtonDown( const FGeometry& MyGeometry, const FPointerEvent& MouseEvent ) override;
	virtual FReply OnMouseButtonUp( const FGeometry& MyGeometry, const FPointerEvent& MouseEvent ) override;
	virtual FReply OnMouseMove( const FGeometry& MyGeometry, const FPointerEvent& MouseEvent ) override;
	virtual void OnMouseLeave(const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseWheel( const FGeometry& MyGeometry, const FPointerEvent& MouseEvent ) override;
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled ) const override;
	virtual FCursorReply OnCursorQuery( const FGeometry& MyGeometry, const FPointerEvent& CursorEvent ) const override;
	virtual void Tick( const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime ) override;
	virtual void OnArrangeChildren( const FGeometry& AllottedGeometry, FArrangedChildren& ArrangedChildren ) const override;
	virtual FVector2D ComputeDesiredSize(float) const override;
	virtual FChildren* GetChildren() override;

protected:
	/** Update any hover state required for the track area */
	void UpdateHoverStates(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent);

private:
	/** The track area's children. */
	TPanelChildren<FTrackAreaSlot> Children;

private:
	/** Cached geometry. */
	FGeometry CachedGeometry;

	/** A map of child slot content that exists in our view. */
	TMap<TWeakPtr<FTimelineTrack>, TWeakPtr<STrack>> TrackSlots;

	/** Weak pointer to the Timeline Controller. */
	TWeakPtr<FTimelineController> WeakEditorTimelineController;

	/** Weak pointer to the time slider. */
	TWeakPtr<FTimeSliderController> WeakTimeSliderController;

	/** Weak pointer to the outliner (used for scrolling interactions). */
	TWeakPtr<SOutliner> WeakOutliner;

};
