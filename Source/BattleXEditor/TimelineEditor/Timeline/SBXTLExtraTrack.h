#pragma once

#include "CoreMinimal.h"
#include "Input/Reply.h"
#include "Misc/Attribute.h"

#include "SNodePanel.h"
#include "SCurveEditor.h"
#include "Layout/Geometry.h"

#include "Widgets/SOverlay.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SScrollBar.h"

#include "SBXTLExtraTrackNode.h"



class SBXTLExtraTrack : public SCompoundWidget
{
#pragma region Important
public:
	SLATE_BEGIN_ARGS(SBXTLExtraTrack) : _ViewInputMin(), _ViewInputMax(), _TimelinePlayLength(), _OnSetInputViewRange(), _OnRefreshPanel() {}
	SLATE_ARGUMENT(class UBXTLAsset*, Asset)
	SLATE_ARGUMENT(int32, SectionIndex)
	SLATE_ARGUMENT(EBXTLExtraType, TrackType)
	SLATE_ATTRIBUTE(float, ViewInputMin)
	SLATE_ATTRIBUTE(float, ViewInputMax)
	SLATE_ATTRIBUTE(float, InputMin)
	SLATE_ATTRIBUTE(float, InputMax)
	SLATE_ATTRIBUTE(float, TimelinePlayLength)
	SLATE_ATTRIBUTE(double, FrameRate)
	SLATE_EVENT(FOnSetInputViewRange, OnSetInputViewRange)
	SLATE_EVENT(FBXTLRefreshPanel, OnRefreshPanel)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

private:
	float TimelinePlayLength = 0.0f;

	int32 SectionIndex = 0;

	EBXTLExtraType TrackType = EBXTLExtraType::ET_TMax;

	TWeakObjectPtr<class UBXTLAsset> CachedAsset = nullptr;

	TSharedPtr<SBXTLExtraTrackNode> TrackNode = nullptr;

#pragma endregion Important



#pragma region Render
public:
	int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

	void UpdateLayout();

private:
	TAttribute<FLinearColor> TrackColor;

#pragma endregion Render



#pragma region Widget
public:
	float GetLastClickedTime() const;

	const FGeometry& GetCachedGeometry() const;

	void UpdateCachedGeometry(const FGeometry& InGeometry);

	FTrackScaleInfo GetCachedScaleInfo() const;

	FVector2D ComputeDesiredSize(float) const override;

	bool SupportsKeyboardFocus() const override;

	FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	FCursorReply OnCursorQuery(const FGeometry& MyGeometry, const FPointerEvent& CursorEvent) const override;

	void HandleNodeDrop(TSharedPtr<SBXTLExtraTrackNode> Node, float Offset = 0.0f);

private:
	float CalculateTime(const FGeometry& MyGeometry, FVector2D NodePos, bool bInputIsAbsolute = true);

	FMargin GetNotifyTrackPadding() const;

	FReply OnNotifyNodeDragStarted(TSharedRef<SBXTLExtraTrackNode> NotifyNode, const FPointerEvent& MouseEvent, const FVector2D& ScreenNodePosition, const bool bDragOnMarker, int32 NotifyIndex);

	float CalculateDraggedNodePos() const { return CurrentDragXPosition; }

private:
	TAttribute<float> ViewInputMin;

	TAttribute<float> ViewInputMax;

	TAttribute<float> InputMin;

	TAttribute<float> InputMax;

	float LastClickedTime;

	float CurrentDragXPosition;

	FGeometry CachedGeometry;

	TSharedPtr<SBorder> TrackArea;

	TSharedPtr<SOverlay> NodeSlots;

	TSharedPtr<SScrollBar> NotifyTrackScrollBar;

#pragma endregion Widget



#pragma region Event
private:
	FOnSetInputViewRange SetInputViewRangeEvent;

	FBXTLRefreshPanel RefreshPanelEvent;

	FBXTLSelectNode SelectNodeEvent;

	FBXTLDeselectNodes DeselectNodesEvent;

	FBXTLDeleteTask DeleteTaskEvent;

	FBXTLAddTask AddTaskEvent;

	FBXTLStartDragETN StartDragETNEvent;

	FBXTLCopyTask CopyTaskEvent;

	FBXTLPasteTask PasteTaskEvent;

	FBXTLExportTemplate ExportTemplateEvent;

#pragma endregion Event

};