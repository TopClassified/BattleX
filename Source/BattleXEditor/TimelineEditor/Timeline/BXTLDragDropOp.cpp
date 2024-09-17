#include "BXTLDragDropOp.h"
#include "Framework/Application/SlateApplication.h"

#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SBorder.h"

#include "SBXTLTaskTrack.h"
#include "SBXTLTaskTrackNode.h"
#include "SBXTLTaskTrackOutliner.h"

#include "BXTask.h"



#define LOCTEXT_NAMESPACE "FBXTLDragDropOp"

FBXTLTaskTrackDragDropOp::FBXTLTaskTrackDragDropOp()
{

}

void FBXTLTaskTrackDragDropOp::Construct()
{
	MouseCursor = EMouseCursor::GrabHandClosed;

	FDragDropOperation::Construct();
}

TSharedRef<FBXTLTaskTrackDragDropOp> FBXTLTaskTrackDragDropOp::New(const TSharedPtr<FBXTLController>& InController, UBXTask* InTask, TSharedPtr<SWidget> Decorator)
{
	TSharedRef<FBXTLTaskTrackDragDropOp> Operation = MakeShareable(new FBXTLTaskTrackDragDropOp);
	Operation->Controller = InController;
	Operation->CachedTask = InTask;
	Operation->Decorator = Decorator;
	Operation->Construct();

	return Operation;
}

void FBXTLTaskTrackDragDropOp::OnDrop(bool bDropWasHandled, const FPointerEvent& MouseEvent)
{

}

void FBXTLTaskTrackDragDropOp::OnDragged(const class FDragDropEvent& DragDropEvent)
{
	if (CursorDecoratorWindow.IsValid())
	{
		CursorDecoratorWindow->MoveWindowTo(DragDropEvent.GetScreenSpacePosition());
	}
}

TSharedPtr<SWidget> FBXTLTaskTrackDragDropOp::GetDefaultDecorator() const
{
	return Decorator;
}

void FBXTLTaskTrackDragDropOp::SetCanDropHere(bool bCanDropHere)
{
	MouseCursor = bCanDropHere ? EMouseCursor::TextEditBeam : EMouseCursor::SlashedCircle;
}

void FBXTLTaskTrackDragDropOp::ChangeTaskTrackPosition(UBXTask* TargetTask)
{
	Controller.Pin()->ChangeTaskPosition(CachedTask.Get(), TargetTask);
}






FBXTLTaskTrackNodeDragDropOp::FBXTLTaskTrackNodeDragDropOp(float& InCurrentDragXPosition) :SelectionTimeLength(0.0f), CurrentDragXPosition(InCurrentDragXPosition)
{

}

TSharedRef<FBXTLTaskTrackNodeDragDropOp> FBXTLTaskTrackNodeDragDropOp::New
(
	TArray<TSharedPtr<SBXTLTaskTrackNode>> NotifyNodes, TSharedPtr<SWidget> Decorator, 
	const TArray<TSharedPtr<SBXTLTaskTrack>>& NotifyTracks, float InViewPlayLength, 
	const FVector2D& CursorPosition, const FVector2D& SelectionScreenPosition, 
	const FVector2D& SelectionSize, float& CurrentDragXPosition, FBXTLRefreshPanel& RefreshPanel
)
{
	TSharedRef<FBXTLTaskTrackNodeDragDropOp> Operation = MakeShareable(new FBXTLTaskTrackNodeDragDropOp(CurrentDragXPosition));
	Operation->ViewPlayLength = MAX_FLT;
	Operation->RefreshPanelEvent = RefreshPanel;
	Operation->NodeGroupPosition = SelectionScreenPosition;
	Operation->NodeGroupSize = SelectionSize;
	Operation->DragOffset = SelectionScreenPosition - CursorPosition;
	Operation->Decorator = Decorator;
	Operation->SelectedNodes = NotifyNodes;
	Operation->TrackSpan = NotifyTracks.Last()->GetTrackIndex() - NotifyTracks[0]->GetTrackIndex();

	float BeginTime = MAX_flt;
	for (TSharedPtr<SBXTLTaskTrackNode> Node : NotifyNodes)
	{
		float NotifyTime = Node->GetTaskNodeData().GetStartTime();

		if (NotifyTime < BeginTime)
		{
			BeginTime = NotifyTime;
		}
	}

	for (TSharedPtr<SBXTLTaskTrackNode> Node : NotifyNodes)
	{
		float NotifyTime = Node->GetTaskNodeData().GetStartTime();

		Operation->NodeTimeOffsets.Add(NotifyTime - BeginTime);
		Operation->NodeTimes.Add(NotifyTime);
		Operation->NodeXOffsets.Add(Node->GetNotifyPositionOffset().X);

		Operation->SelectionTimeLength = FMath::Max(Operation->SelectionTimeLength, NotifyTime + Node->GetTaskNodeData().GetDuration() - BeginTime);
	}

	Operation->Construct();

	for (int32 i = 0; i < NotifyTracks.Num(); ++i)
	{
		FTrackClampInfo Info;
		Info.NotifyTrack = NotifyTracks[i];
		const FGeometry& CachedGeometry = Info.NotifyTrack->GetCachedGeometry();
		Info.TrackPos = CachedGeometry.AbsolutePosition.Y;
		Info.TrackSnapTestPos = Info.TrackPos + (CachedGeometry.Size.Y / 2);
		Operation->ClampInfos.Add(Info);
	}

	Operation->CursorDecoratorWindow->SetOpacity(0.5f);
	return Operation;
}

void FBXTLTaskTrackNodeDragDropOp::OnDrop(bool bDropWasHandled, const FPointerEvent& MouseEvent)
{
	if (bDropWasHandled == false)
	{
		int32 NumNodes = SelectedNodes.Num();

		for (int32 CurrentNode = 0; CurrentNode < NumNodes; ++CurrentNode)
		{
			TSharedPtr<SBXTLTaskTrackNode> Node = SelectedNodes[CurrentNode];
			float NodePositionOffset = NodeXOffsets[CurrentNode];
			const FTrackClampInfo& ClampInfo = GetTrackClampInfo(Node->GetScreenPosition());
			ClampInfo.NotifyTrack->HandleNodeDrop(Node, NodePositionOffset);
			Node->DragCancelled();
		}

		RefreshPanelEvent.ExecuteIfBound();
	}

	FDragDropOperation::OnDrop(bDropWasHandled, MouseEvent);
}

void FBXTLTaskTrackNodeDragDropOp::OnDragged(const class FDragDropEvent& DragDropEvent)
{
	NodeGroupPosition = DragDropEvent.GetScreenSpacePosition() + DragOffset;

	FTrackClampInfo* SelectionPositionClampInfo = &GetTrackClampInfo(DragDropEvent.GetScreenSpacePosition());
	if ((SelectionPositionClampInfo->NotifyTrack->GetTrackIndex() + TrackSpan) >= ClampInfos.Num())
	{
		SelectionPositionClampInfo = &ClampInfos[ClampInfos.Num() - TrackSpan - 1];
	}

	const FGeometry& TrackGeom = SelectionPositionClampInfo->NotifyTrack->GetCachedGeometry();
	const FTrackScaleInfo& TrackScaleInfo = SelectionPositionClampInfo->NotifyTrack->GetCachedScaleInfo();

	FVector2D SelectionBeginPosition = TrackGeom.LocalToAbsolute(TrackGeom.AbsoluteToLocal(NodeGroupPosition) + SelectedNodes[0]->GetNotifyPositionOffset());

	float LocalTrackMin = TrackScaleInfo.InputToLocalX(0.0f);
	float LocalTrackMax = TrackScaleInfo.InputToLocalX(ViewPlayLength);
	float LocalTrackWidth = LocalTrackMax - LocalTrackMin;

	float SnapMovement = 0.0f;
	float SelectionBeginLocalPositionX = TrackGeom.AbsoluteToLocal(SelectionBeginPosition).X;
	const float ClampedEnd = FMath::Clamp(SelectionBeginLocalPositionX + NodeGroupSize.X, LocalTrackMin, LocalTrackMax);
	const float ClampedBegin = FMath::Clamp(SelectionBeginLocalPositionX, LocalTrackMin, LocalTrackMax);
	if (ClampedBegin > SelectionBeginLocalPositionX)
	{
		SelectionBeginLocalPositionX = ClampedBegin;
	}
	else if (ClampedEnd < SelectionBeginLocalPositionX + NodeGroupSize.X)
	{
		SelectionBeginLocalPositionX = ClampedEnd - NodeGroupSize.X;
	}

	SelectionBeginPosition.X = TrackGeom.LocalToAbsolute(FVector2D(SelectionBeginLocalPositionX, 0.0f)).X;

	bool bSnapped = false;
	for (int32 NodeIdx = 0; NodeIdx < SelectedNodes.Num() && !bSnapped; ++NodeIdx)
	{
		TSharedPtr<SBXTLTaskTrackNode> CurrentNode = SelectedNodes[NodeIdx];

		const FTrackClampInfo& NodeClamp = GetTrackClampInfo(CurrentNode->GetScreenPosition());

		FVector2D EventPosition = SelectionBeginPosition + FVector2D(TrackScaleInfo.PixelsPerInput * NodeTimeOffsets[NodeIdx], 0.0f);

		SelectionBeginPosition.Y = SelectionPositionClampInfo->TrackPos - 1.0f;
	}

	SelectionBeginPosition.X += SnapMovement;

	CurrentDragXPosition = TrackGeom.AbsoluteToLocal(FVector2D(SelectionBeginPosition.X, 0.0f)).X;

	CursorDecoratorWindow->MoveWindowTo(TrackGeom.LocalToAbsolute(TrackGeom.AbsoluteToLocal(SelectionBeginPosition) - SelectedNodes[0]->GetNotifyPositionOffset()));
	NodeGroupPosition = SelectionBeginPosition;
}

FText FBXTLTaskTrackNodeDragDropOp::GetHoverText() const
{
	FText HoverText = LOCTEXT("Invalid", "Invalid");

	if (SelectedNodes[0].IsValid())
	{
		HoverText = FText::FromString(SelectedNodes[0]->GetTaskNodeData().GetTaskName());
	}

	return HoverText;
}

FBXTLTaskTrackNodeDragDropOp::FTrackClampInfo& FBXTLTaskTrackNodeDragDropOp::GetTrackClampInfo(const FVector2D NodePos)
{
	int32 ClampInfoIndex = 0;
	int32 SmallestNodeTrackDist = FMath::Abs(ClampInfos[0].TrackSnapTestPos - NodePos.Y);
	for (int32 i = 0; i < ClampInfos.Num(); ++i)
	{
		int32 Dist = FMath::Abs(ClampInfos[i].TrackSnapTestPos - NodePos.Y);
		if (Dist < SmallestNodeTrackDist)
		{
			SmallestNodeTrackDist = Dist;
			ClampInfoIndex = i;
		}
	}

	return ClampInfos[ClampInfoIndex];
}






FBXTLExtraTrackNodeDragDropOp::FBXTLExtraTrackNodeDragDropOp(float& InCurrentDragXPosition) :SelectionTimeLength(0.0f), CurrentDragXPosition(InCurrentDragXPosition)
{

}

TSharedRef<FBXTLExtraTrackNodeDragDropOp> FBXTLExtraTrackNodeDragDropOp::New
(
	TArray<TSharedPtr<SBXTLExtraTrackNode>> NotifyNodes, TSharedPtr<SWidget> Decorator,
	const TArray<TSharedPtr<SBXTLExtraTrack>>& NotifyTracks, float InViewPlayLength,
	const FVector2D& CursorPosition, const FVector2D& SelectionScreenPosition,
	const FVector2D& SelectionSize, float& CurrentDragXPosition, FBXTLRefreshPanel& RefreshPanel
)
{
	TSharedRef<FBXTLExtraTrackNodeDragDropOp> Operation = MakeShareable(new FBXTLExtraTrackNodeDragDropOp(CurrentDragXPosition));
	Operation->ViewPlayLength = MAX_FLT;
	Operation->RefreshPanelEvent = RefreshPanel;
	Operation->NodeGroupPosition = SelectionScreenPosition;
	Operation->NodeGroupSize = SelectionSize;
	Operation->DragOffset = SelectionScreenPosition - CursorPosition;
	Operation->Decorator = Decorator;
	Operation->SelectedNodes = NotifyNodes;
	Operation->TrackSpan = 0;

	float BeginTime = MAX_flt;
	for (TSharedPtr<SBXTLExtraTrackNode> Node : NotifyNodes)
	{
		float NotifyTime = Node->GetNodeStartTime();

		if (NotifyTime < BeginTime)
		{
			BeginTime = NotifyTime;
		}
	}

	for (TSharedPtr<SBXTLExtraTrackNode> Node : NotifyNodes)
	{
		float NotifyTime = Node->GetNodeStartTime();

		Operation->NodeTimeOffsets.Add(NotifyTime - BeginTime);
		Operation->NodeTimes.Add(NotifyTime);
		Operation->NodeXOffsets.Add(Node->GetNotifyPositionOffset().X);

		Operation->SelectionTimeLength = FMath::Max(Operation->SelectionTimeLength, NotifyTime + Node->GetNodeDuration() - BeginTime);
	}

	Operation->Construct();

	for (int32 i = 0; i < NotifyTracks.Num(); ++i)
	{
		FTrackClampInfo Info;
		Info.NotifyTrack = NotifyTracks[i];
		const FGeometry& CachedGeometry = Info.NotifyTrack->GetCachedGeometry();
		Info.TrackPos = CachedGeometry.AbsolutePosition.Y;
		Info.TrackSnapTestPos = Info.TrackPos + (CachedGeometry.Size.Y / 2);
		Operation->ClampInfos.Add(Info);
	}

	Operation->CursorDecoratorWindow->SetOpacity(0.5f);
	return Operation;
}

void FBXTLExtraTrackNodeDragDropOp::OnDrop(bool bDropWasHandled, const FPointerEvent& MouseEvent)
{
	if (bDropWasHandled == false)
	{
		int32 NumNodes = SelectedNodes.Num();

		for (int32 CurrentNode = 0; CurrentNode < NumNodes; ++CurrentNode)
		{
			TSharedPtr<SBXTLExtraTrackNode> Node = SelectedNodes[CurrentNode];
			float NodePositionOffset = NodeXOffsets[CurrentNode];
			const FTrackClampInfo& ClampInfo = GetTrackClampInfo(Node->GetScreenPosition());
			ClampInfo.NotifyTrack->HandleNodeDrop(Node, NodePositionOffset);
			Node->DragCancelled();
		}

		RefreshPanelEvent.ExecuteIfBound();
	}

	FDragDropOperation::OnDrop(bDropWasHandled, MouseEvent);
}

void FBXTLExtraTrackNodeDragDropOp::OnDragged(const class FDragDropEvent& DragDropEvent)
{
	NodeGroupPosition = DragDropEvent.GetScreenSpacePosition() + DragOffset;

	FTrackClampInfo* SelectionPositionClampInfo = &GetTrackClampInfo(DragDropEvent.GetScreenSpacePosition());
	if (TrackSpan >= ClampInfos.Num())
	{
		SelectionPositionClampInfo = &ClampInfos[ClampInfos.Num() - TrackSpan - 1];
	}

	const FGeometry& TrackGeom = SelectionPositionClampInfo->NotifyTrack->GetCachedGeometry();
	const FTrackScaleInfo& TrackScaleInfo = SelectionPositionClampInfo->NotifyTrack->GetCachedScaleInfo();

	FVector2D SelectionBeginPosition = TrackGeom.LocalToAbsolute(TrackGeom.AbsoluteToLocal(NodeGroupPosition) + SelectedNodes[0]->GetNotifyPositionOffset());

	float LocalTrackMin = TrackScaleInfo.InputToLocalX(0.0f);
	float LocalTrackMax = TrackScaleInfo.InputToLocalX(ViewPlayLength);
	float LocalTrackWidth = LocalTrackMax - LocalTrackMin;

	float SnapMovement = 0.0f;
	float SelectionBeginLocalPositionX = TrackGeom.AbsoluteToLocal(SelectionBeginPosition).X;
	const float ClampedEnd = FMath::Clamp(SelectionBeginLocalPositionX + NodeGroupSize.X, LocalTrackMin, LocalTrackMax);
	const float ClampedBegin = FMath::Clamp(SelectionBeginLocalPositionX, LocalTrackMin, LocalTrackMax);
	if (ClampedBegin > SelectionBeginLocalPositionX)
	{
		SelectionBeginLocalPositionX = ClampedBegin;
	}
	else if (ClampedEnd < SelectionBeginLocalPositionX + NodeGroupSize.X)
	{
		SelectionBeginLocalPositionX = ClampedEnd - NodeGroupSize.X;
	}

	SelectionBeginPosition.X = TrackGeom.LocalToAbsolute(FVector2D(SelectionBeginLocalPositionX, 0.0f)).X;

	bool bSnapped = false;
	for (int32 NodeIdx = 0; NodeIdx < SelectedNodes.Num() && !bSnapped; ++NodeIdx)
	{
		TSharedPtr<SBXTLExtraTrackNode> CurrentNode = SelectedNodes[NodeIdx];

		const FTrackClampInfo& NodeClamp = GetTrackClampInfo(CurrentNode->GetScreenPosition());

		FVector2D EventPosition = SelectionBeginPosition + FVector2D(TrackScaleInfo.PixelsPerInput * NodeTimeOffsets[NodeIdx], 0.0f);

		SelectionBeginPosition.Y = SelectionPositionClampInfo->TrackPos - 1.0f;
	}

	SelectionBeginPosition.X += SnapMovement;

	CurrentDragXPosition = TrackGeom.AbsoluteToLocal(FVector2D(SelectionBeginPosition.X, 0.0f)).X;

	CursorDecoratorWindow->MoveWindowTo(TrackGeom.LocalToAbsolute(TrackGeom.AbsoluteToLocal(SelectionBeginPosition) - SelectedNodes[0]->GetNotifyPositionOffset()));
	NodeGroupPosition = SelectionBeginPosition;
}

FText FBXTLExtraTrackNodeDragDropOp::GetHoverText() const
{
	FText HoverText = LOCTEXT("Invalid", "Invalid");

	if (SelectedNodes[0].IsValid())
	{
		HoverText = SelectedNodes[0]->GetNodeName();
	}

	return HoverText;
}

FBXTLExtraTrackNodeDragDropOp::FTrackClampInfo& FBXTLExtraTrackNodeDragDropOp::GetTrackClampInfo(const FVector2D NodePos)
{
	int32 ClampInfoIndex = 0;
	int32 SmallestNodeTrackDist = FMath::Abs(ClampInfos[0].TrackSnapTestPos - NodePos.Y);
	for (int32 i = 0; i < ClampInfos.Num(); ++i)
	{
		int32 Dist = FMath::Abs(ClampInfos[i].TrackSnapTestPos - NodePos.Y);
		if (Dist < SmallestNodeTrackDist)
		{
			SmallestNodeTrackDist = Dist;
			ClampInfoIndex = i;
		}
	}

	return ClampInfos[ClampInfoIndex];
}

#undef LOCTEXT_NAMESPACE
