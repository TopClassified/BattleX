#pragma once

#include "CoreMinimal.h"
#include "Input/Reply.h"
#include "Input/DragAndDrop.h"
#include "Misc/Attribute.h"

#include "SNodePanel.h"
#include "SCurveEditor.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

#include "BXTLEditorDelegates.h"
#include "BXTLController.h"
#include "SBXTLTaskTrack.h"
#include "SBXTLTaskTrackNode.h"
#include "SBXTLExtraTrack.h"
#include "SBXTLExtraTrackNode.h" 



class FBXTLTaskTrackDragDropOp : public FDragDropOperation
{
public:
	DRAG_DROP_OPERATOR_TYPE(FBXTLTaskTrackDragDropOp, FDragDropOperation);

	FBXTLTaskTrackDragDropOp();

	void Construct() override;

	static TSharedRef<FBXTLTaskTrackDragDropOp> New(const TSharedPtr<FBXTLController>& InController, class UBXTask* InTask, TSharedPtr<SWidget> Decorator);

	void OnDrop(bool bDropWasHandled, const FPointerEvent& MouseEvent) override;

	void OnDragged(const class FDragDropEvent& DragDropEvent) override;

	TSharedPtr<SWidget> GetDefaultDecorator() const override;

	void SetCanDropHere(bool bCanDropHere);

	void ChangeTaskTrackPosition(class UBXTask* TargetTask);

public:
	TSharedPtr<SWidget>	Decorator = nullptr;

	TWeakObjectPtr<class UBXTask> CachedTask = nullptr;

	TWeakPtr<FBXTLController> Controller = nullptr;

};



class FBXTLTaskTrackNodeDragDropOp : public FDragDropOperation
{
public:
	struct FTrackClampInfo
	{
		int32 TrackPos;
		int32 TrackSnapTestPos;
		TSharedPtr<SBXTLTaskTrack> NotifyTrack;
	};
	
	DRAG_DROP_OPERATOR_TYPE(FBXTLTaskTrackNodeDragDropOp, FDragDropOperation);

	FBXTLTaskTrackNodeDragDropOp(float& InCurrentDragXPosition);

	static TSharedRef<FBXTLTaskTrackNodeDragDropOp> New
	(
		TArray<TSharedPtr<SBXTLTaskTrackNode>> NotifyNodes, TSharedPtr<SWidget> Decorator, const TArray<TSharedPtr<SBXTLTaskTrack>>& NotifyTracks, float InViewPlayLength, 
		const FVector2D& CursorPosition, const FVector2D& SelectionScreenPosition, const FVector2D& SelectionSize, float& CurrentDragXPosition, FBXTLESlateRefreshPanel& RefreshPanelEvent
	);
	
	void OnDrop(bool bDropWasHandled, const FPointerEvent& MouseEvent) override;

	void OnDragged(const class FDragDropEvent& DragDropEvent) override;

	TSharedPtr<SWidget> GetDefaultDecorator() const override
	{
		return Decorator;
	}

	FText GetHoverText() const;

	FTrackClampInfo& GetTrackClampInfo(const FVector2D NodePos);

public:
	FVector2D DragOffset;
	TArray<FTrackClampInfo> ClampInfos;	

	TSharedPtr<SWidget> Decorator;
	TArray<TSharedPtr<SBXTLTaskTrackNode>> SelectedNodes;

	TArray<float> NodeTimes;
	TArray<float> NodeTimeOffsets;
	TArray<float> NodeXOffsets;
	FVector2D NodeGroupPosition;
	FVector2D NodeGroupSize;

	int32 TrackSpan;
	float ViewPlayLength;
	float SelectionTimeLength;
	float CurrentDragXPosition;

	FBXTLESlateRefreshPanel RefreshPanelEvent;

};



class FBXTLExtraTrackNodeDragDropOp : public FDragDropOperation
{
public:
	struct FTrackClampInfo
	{
		int32 TrackPos;
		int32 TrackSnapTestPos;
		TSharedPtr<SBXTLExtraTrack> NotifyTrack;
	};

	DRAG_DROP_OPERATOR_TYPE(FBXTLExtraTrackNodeDragDropOp, FDragDropOperation);

	FBXTLExtraTrackNodeDragDropOp(float& InCurrentDragXPosition);

	static TSharedRef<FBXTLExtraTrackNodeDragDropOp> New
	(
		TArray<TSharedPtr<SBXTLExtraTrackNode>> NotifyNodes, TSharedPtr<SWidget> Decorator, const TArray<TSharedPtr<SBXTLExtraTrack>>& NotifyTracks, float InViewPlayLength, 
		const FVector2D& CursorPosition, const FVector2D& SelectionScreenPosition, const FVector2D& SelectionSize, float& CurrentDragXPosition, FBXTLESlateRefreshPanel& RefreshPanelEvent
	);

	void OnDrop(bool bDropWasHandled, const FPointerEvent& MouseEvent) override;

	void OnDragged(const class FDragDropEvent& DragDropEvent) override;

	TSharedPtr<SWidget> GetDefaultDecorator() const override
	{
		return Decorator;
	}

	FText GetHoverText() const;

	FTrackClampInfo& GetTrackClampInfo(const FVector2D NodePos);

public:
	FVector2D DragOffset;
	TArray<FTrackClampInfo> ClampInfos;

	TSharedPtr<SWidget> Decorator;
	TArray<TSharedPtr<SBXTLExtraTrackNode>> SelectedNodes;

	TArray<float> NodeTimes;
	TArray<float> NodeTimeOffsets;
	TArray<float> NodeXOffsets;
	FVector2D NodeGroupPosition;
	FVector2D NodeGroupSize;

	int32 TrackSpan;
	float ViewPlayLength;
	float SelectionTimeLength;
	float CurrentDragXPosition;

	FBXTLESlateRefreshPanel RefreshPanelEvent;

};
