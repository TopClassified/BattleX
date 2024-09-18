#pragma once

#include "TimelineTrack.h"

#include "BXTLEnums.h"



class FBXTLExtraTrackPanel : public FTimelineTrack
{
	TIMELINE_DECLARE_TRACK(FBXTLExtraTrackPanel, FTimelineTrack);

public:
	FBXTLExtraTrackPanel
	(
		const TSharedRef<class FBXTLController>& InController, 
		class UBXTLAsset* InAsset, int32 InSectionIndex, EBXTLExtraType InTrackType, 
		const FText& InDisplayName, const FText& InToolTipText
	);

	void UpdateLayout();

	// 绘制Task时间轨道
	TSharedRef<SWidget> GenerateContainerWidgetForTimeline() override;

private:
	void InputViewRangeChanged(float ViewMin, float ViewMax);

private:
	TWeakObjectPtr<class UBXTLAsset> CachedAsset = nullptr;

	int32 SectionIndex = -1;

	EBXTLExtraType TrackType = EBXTLExtraType::ET_TMax;

	TSharedPtr<class SBXTLExtraTrack> TrackWidget = nullptr;
	
};