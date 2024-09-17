#include "SBXTLTaskGroupTrackOutliner.h"
#include "Framework/Application/SlateApplication.h"

#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SBorder.h"

#include "BXTLEditorUtilities.h"
#include "BXTLDragDropOp.h"
#include "BXTLController.h"
#include "SBXTLTaskTrackOutliner.h"

#include "BXTask.h"



#define LOCTEXT_NAMESPACE "SBXTLTaskGroupTrackOutliner"

void SBXTLTaskGroupTrackOutliner::Construct(const FArguments& InArgs, const TSharedPtr<FTimelineController>& InTimelineController, FBXTLTaskGroup& InGroupData)
{
	TimelineController = InTimelineController;
	GroupData = &InGroupData;

	TSharedPtr<SWidget> MainWidget = InArgs._MainWidget;

	this->ChildSlot
	[
		MainWidget.ToSharedRef()
	];
}

FReply SBXTLTaskGroupTrackOutliner::OnDrop(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent)
{
	bool bWasDropHandled = false;

	TSharedPtr<FDragDropOperation> Operation = DragDropEvent.GetOperation();

	if (!Operation.IsValid())
	{

	}
	else if (Operation->IsOfType<FBXTLTaskTrackDragDropOp>())
	{
		const auto& FrameDragDropOp = StaticCastSharedPtr<FBXTLTaskTrackDragDropOp>(Operation);

		if (FBXTLController* TC = static_cast<FBXTLController*>(TimelineController.Pin().Get()))
		{
			TC->ChangeTaskGroup(FrameDragDropOp->CachedTask.Get(), *GroupData);
		}

		bWasDropHandled = true;
	}

	return bWasDropHandled ? FReply::Handled() : FReply::Unhandled();
}

#undef LOCTEXT_NAMESPACE
