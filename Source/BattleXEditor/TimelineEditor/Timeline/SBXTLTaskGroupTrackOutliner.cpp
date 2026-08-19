#include "SBXTLTaskGroupTrackOutliner.h"
#include "Framework/Application/SlateApplication.h"

#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SBorder.h"

#include "BXTLEditorUtilities.h"
#include "BXTLDragDropOp.h"
#include "BXTLController.h"
#include "SBXTLTaskTrackOutliner.h"

#include "BXTask.h"
#include "BXTLAsset.h"



#define LOCTEXT_NAMESPACE "SBXTLTaskGroupTrackOutliner"

void SBXTLTaskGroupTrackOutliner::Construct(const FArguments& InArgs, const TSharedPtr<FTimelineController>& InTimelineController, int32 InGroupID)
{
	TimelineController = InTimelineController;
	GroupID = InGroupID;

	TSharedPtr<SWidget> MainWidget = InArgs._MainWidget;

	this->ChildSlot[MainWidget.ToSharedRef()];
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
			// 按索引解析目标组(数组重排后裸指针悬垂,越界时放弃本次拖放)
			UBXTLAsset* Asset = TC->GetAsset();
			const int32 SectionID = TC->GetSectionID();
			if (Asset && Asset->Sections.IsValidIndex(SectionID))
			{
				TArray<FBXTLTaskGroup>& Groups = Asset->Sections[SectionID].Groups;
				if (Groups.IsValidIndex(GroupID))
				{
					TC->ChangeTaskGroup(FrameDragDropOp->CachedTask.Get(), Groups[GroupID]);
				}
			}
		}

		bWasDropHandled = true;
	}

	return bWasDropHandled ? FReply::Handled() : FReply::Unhandled();
}

#undef LOCTEXT_NAMESPACE
