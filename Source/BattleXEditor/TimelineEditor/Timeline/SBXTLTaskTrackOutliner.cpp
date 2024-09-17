#include "SBXTLTaskTrackOutliner.h"
#include "Framework/Application/SlateApplication.h"

#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SBorder.h"
#include "BXTLDragDropOp.h"
#include "BXTLTaskTrackPanel.h"

#include "BXTask.h"
#include "BXTLEditorUtilities.h"
#include "BXTLController.h"



#define LOCTEXT_NAMESPACE "SBXTLTaskTrackOutliner"

void SBXTLTaskTrackOutliner::Construct(const FArguments& InArgs, const TSharedPtr<FTimelineController>& InController, UBXTask* InTask)
{
	Controller = StaticCastSharedPtr<FBXTLController>(InController);

	CachedTask = InTask;

	TrackPanelArea = InArgs._TrackPanelArea;
	InlineEditableTextBlock = InArgs._InlineEditableTextBlock;

	this->ChildSlot
	[
		TrackPanelArea.ToSharedRef()
	];

	UpdateLayout();
}


void SBXTLTaskTrackOutliner::UpdateLayout()
{
	TSharedPtr<SHorizontalBox> ParentOutlinerWidget = SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.FillWidth(1.0f)
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Left)
		.Padding(20.0f, 0.0f, 0.0f, 0.0f)
		[
			InlineEditableTextBlock.ToSharedRef()
		];
	
	TrackPanelArea->SetContent(ParentOutlinerWidget.ToSharedRef());
}

FReply SBXTLTaskTrackOutliner::OnDrop(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent)
{
	bool bWasDropHandled = false;

	TSharedPtr<FDragDropOperation> Operation = DragDropEvent.GetOperation();
	if (!Operation.IsValid())
	{

	}
	else if (Operation->IsOfType<FBXTLTaskTrackDragDropOp>())
	{
		const auto& FrameDragDropOp = StaticCastSharedPtr<FBXTLTaskTrackDragDropOp>(Operation);
		FrameDragDropOp->ChangeTaskTrackPosition(CachedTask.Get());
		bWasDropHandled = true;
	}
	
	return bWasDropHandled ? FReply::Handled() : FReply::Unhandled();
}

FReply SBXTLTaskTrackOutliner::OnDragDetected(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (MouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		TSharedRef<SOverlay> NodeDragDecoratorOverlay = SNew(SOverlay);
		TSharedRef<SBorder> NodeDragDecorator = SNew(SBorder)
			.BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
			.BorderBackgroundColor(FLinearColor(.0f, 1.f, .0f))
			[
				NodeDragDecoratorOverlay
			];

		FVector2D OffsetFromFirst(2, 2);

		NodeDragDecoratorOverlay->AddSlot().Padding(FMargin(OffsetFromFirst.X, OffsetFromFirst.Y, 0.0f, 0.0f))[SharedThis(this)];

		return FReply::Handled().BeginDragDrop(FBXTLTaskTrackDragDropOp::New(Controller.Pin().ToSharedRef(), CachedTask.Get(), NodeDragDecorator));
	}

	return FReply::Unhandled();
}

FReply SBXTLTaskTrackOutliner::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		return FReply::Handled().DetectDrag(SharedThis(this), EKeys::LeftMouseButton);
	}

	return FReply::Unhandled();
}

#undef LOCTEXT_NAMESPACE
