#pragma once

#include "TimelineTrack.h"

#include "BXTLEnums.h" 



class FBXTLExtraTrackPanel : public FTimelineTrack
{
	TIMELINE_DECLARE_TRACK(FBXTLExtraTrackPanel, FTimelineTrack);

#pragma region Important
public:
	FBXTLExtraTrackPanel
	(
		const TSharedRef<class FBXTLController>& InController, 
		class UBXTLAsset* InAsset, int32 InSectionIndex, EBXTLExtraType InTrackType, 
		const FText& InDisplayName, const FText& InToolTipText
	);

	// 绘制Task时间轨道
	TSharedRef<SWidget> GenerateContainerWidgetForTimeline() override;

private:
	TWeakObjectPtr<class UBXTLAsset> CachedAsset = nullptr;

	int32 SectionIndex = -1;

	EBXTLExtraType TrackType = EBXTLExtraType::ET_TMax;

	TSharedPtr<class SBXTLExtraTrack> TrackWidget = nullptr;

#pragma endregion Important



#pragma region Callback
protected:
	void OnRefreshPanel();

	void OnInputViewRangeChanged(float ViewMin, float ViewMax);

#pragma endregion Callback

};