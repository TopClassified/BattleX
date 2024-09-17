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
		, _OnSetInputViewRange()
		, _OnRefreshPanel()
		, _OnSelectNode()
		, _OnDeselectAllNodes()
		, _OnDeleteTask()
		, _OnAddNewTask()
		, _OnCopyTasks()
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

	SLATE_EVENT(FOnSetInputViewRange, OnSetInputViewRange)
	SLATE_EVENT(FBXTLRefreshPanel, OnRefreshPanel)
	SLATE_EVENT(FBXTLSelectNode, OnSelectNode)
	SLATE_EVENT(FBXTLDeselectNodes, OnDeselectAllNodes)
	SLATE_EVENT(FBXTLDeleteTask, OnDeleteTask)
	SLATE_EVENT(FBXTLAddTask, OnAddNewTask)
	SLATE_EVENT(FBXTLCopyTask, OnCopyTasks)
	SLATE_EVENT(FBXTLPasteTask, OnPasteTasks)
	SLATE_EVENT(FBXTLExportTemplate, OnExportTemplate)

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
	int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

	void UpdateLayout();

private:
	TAttribute<FLinearColor> TrackColor;

#pragma endregion Render



#pragma region Menu
private:
	TSharedPtr<SWidget> SummonContextMenu(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent);

	void AddNewTaskMenu(FMenuBuilder& MenuBuilder);

	void CreateNewTaskNodeAtCursor(UClass* NotifyClass);

	void CopyTaskMenu();

	void PasteTaskMenu();

	void ExportTemplateMenu();

private:
	bool bIsCtrlDown = false;

#pragma endregion Menu



#pragma region Widget
public:
	float GetLastClickedTime() const;

	const FGeometry& GetCachedGeometry() const;

	void UpdateCachedGeometry(const FGeometry& InGeometry);

	FTrackScaleInfo GetCachedScaleInfo() const;

	FVector2D ComputeDesiredSize(float) const override;

	bool SupportsKeyboardFocus() const override;

	FReply OnKeyUp(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;

	FReply OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;

	FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	FCursorReply OnCursorQuery(const FGeometry& MyGeometry, const FPointerEvent& CursorEvent) const override;

	void HandleNodeDrop(TSharedPtr<SBXTLTaskTrackNode> Node, float Offset = 0.0f);

private:
	void SelectTaskNode();

	void DeselectAllTaskNodes();

	void DeleteSelectedTask();

	void OnDeleteKeyPressed();

	float CalculateTime(const FGeometry& MyGeometry, FVector2D NodePos, bool bInputIsAbsolute = true);

	FMargin GetNotifyTrackPadding() const;

	FReply OnNotifyNodeDragStarted(TSharedRef<SBXTLTaskTrackNode> NotifyNode, const FPointerEvent& MouseEvent, const FVector2D& ScreenNodePosition, const bool bDragOnMarker, int32 NotifyIndex);

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

	FBXTLStartDragTTN StartDragTTNEvent;

	FBXTLCopyTask CopyTaskEvent;

	FBXTLPasteTask PasteTaskEvent;

	FBXTLExportTemplate ExportTemplateEvent;

#pragma endregion Event

};