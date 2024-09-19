#pragma once

#include "CoreMinimal.h"
#include "Input/Reply.h"
#include "Misc/Attribute.h"

#include "SNodePanel.h"
#include "SCurveEditor.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

#include "BXTLEditorDelegates.h"
#include "SBXTLTaskTrackNode.h"

#include "BXTask.h"



class SBXTLTaskTrack : public SCompoundWidget
{
#pragma region Important
public:
	SLATE_BEGIN_ARGS(SBXTLTaskTrack)
		: _ViewInputMin()
		, _ViewInputMax()
		, _TimelinePlayLength()
		, _TrackIndex()
		, _SetInputViewRangeEvent()
		, _RefreshPanelEvent()
		, _SelectNodeEvent()
		, _DeselectNodesEvent()
		, _AddTaskEvent()
		, _DeleteTaskEvent()
		, _CopyTasksEvent()
		, _PasteTasksEvent()
		, _ExportTemplateEvent()
	{}
	SLATE_ARGUMENT(class UBXTask*, Task)
	SLATE_ARGUMENT(TWeakPtr<class FBXTLController> , Controller)
	SLATE_ATTRIBUTE(float, ViewInputMin)
	SLATE_ATTRIBUTE(float, ViewInputMax)
	SLATE_ATTRIBUTE(float, InputMin)
	SLATE_ATTRIBUTE(float, InputMax)
	SLATE_ATTRIBUTE(float, TimelinePlayLength)
	SLATE_ATTRIBUTE(double, FrameRate)
	SLATE_ARGUMENT(int32, TrackIndex)

	SLATE_EVENT(FOnSetInputViewRange, SetInputViewRangeEvent)
	SLATE_EVENT(FBXTLESlateRefreshPanel, RefreshPanelEvent)
	SLATE_EVENT(FBXTLESlateSelectNode, SelectNodeEvent)
	SLATE_EVENT(FBXTLESlateDeselectNodes, DeselectNodesEvent)
	SLATE_EVENT(FBXTLESlateAddTask, AddTaskEvent)
	SLATE_EVENT(FBXTLESlateDeleteTask, DeleteTaskEvent)
	SLATE_EVENT(FBXTLESlateCopyTask, CopyTasksEvent)
	SLATE_EVENT(FBXTLESlatePasteTask, PasteTasksEvent)
	SLATE_EVENT(FBXTLESlateExportTemplate, ExportTemplateEvent)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

	int32 GetTrackIndex() const;

private:
	float TimelinePlayLength;

	int32 TrackIndex;

	TWeakPtr<FBXTLController> Controller = nullptr;

	TWeakObjectPtr<class UBXTask> CachedTask = nullptr;

	TSharedPtr<SBXTLTaskTrackNode> TaskNode = nullptr;

#pragma endregion Important



#pragma region Render
public:
	void UpdateLayout();

private:
	TAttribute<FLinearColor> TrackColor;

#pragma endregion Render



#pragma region Menu
private:
	TSharedPtr<SWidget> SummonContextMenu(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent);

	void AddNewTaskMenu(FMenuBuilder& MenuBuilder);

	void CreateGraphNode();

	void CreateNewTaskNodeAtCursor(UClass* NotifyClass);

	void CopyTaskMenu();

	void PasteTaskMenu();

	void ExportTemplateMenu();

private:
	bool bIsCtrlDown = false;

#pragma endregion Menu



#pragma region Widget
public:
	const FGeometry& GetCachedGeometry() const;

	void UpdateCachedGeometry(const FGeometry& InGeometry);

	FTrackScaleInfo GetCachedScaleInfo() const;

	FVector2D ComputeDesiredSize(float) const override;

	bool SupportsKeyboardFocus() const override;

	FReply OnKeyUp(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;

	FReply OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;

	FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	FReply OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	void HandleNodeDrop(TSharedPtr<SBXTLTaskTrackNode> Node, float Offset = 0.0f);

private:
	void SelectTaskNode();

	void DeselectAllTaskNodes();

	void DeleteSelectedTask();

	void OnDeleteKeyPressed();

	FMargin GetNotifyTrackPadding() const;

private:
	TAttribute<float> ViewInputMin;

	TAttribute<float> ViewInputMax;

	TAttribute<float> InputMin;

	TAttribute<float> InputMax;

	float CurrentDragXPosition;

	FGeometry CachedGeometry;

	TSharedPtr<SOverlay> TrackArea;

#pragma endregion Widget



#pragma region Callback
public:
	FReply OnNodeDragged(TSharedRef<SBXTLTaskTrackNode> NotifyNode, const FPointerEvent& MouseEvent, const FVector2D& ScreenNodePosition, const bool bDragOnMarker, int32 NotifyIndex);

#pragma endregion Callback



#pragma region Event
private:
	FOnSetInputViewRange SetInputViewRangeEvent;

	FBXTLESlateRefreshPanel RefreshPanelEvent;

	FBXTLESlateSelectNode SelectNodeEvent;

	FBXTLESlateDeselectNodes DeselectNodesEvent;

	FBXTLESlateAddTask AddTaskEvent;

	FBXTLESlateDeleteTask DeleteTaskEvent;

	FBXTLESlateCopyTask CopyTaskEvent;

	FBXTLESlatePasteTask PasteTaskEvent;

	FBXTLESlateExportTemplate ExportTemplateEvent;

#pragma endregion Event

};