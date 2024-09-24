#pragma once

#include "CoreMinimal.h"
#include "Input/Reply.h"
#include "Misc/Attribute.h"
#include "ScopedTransaction.h"

#include "Widgets/SBoxPanel.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Layout/SBorder.h" 



class SBXTLTaskTrackOutliner : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SBXTLTaskTrackOutliner) {};
	SLATE_ARGUMENT(TSharedPtr<SBorder>, TrackPanelArea)
	SLATE_ARGUMENT(TSharedPtr<SWidget>, InlineEditableTextBlock)
	SLATE_END_ARGS()

public:
	void Construct(const FArguments& InArgs, const TSharedPtr<class FTimelineController>& InController, class UBXTask* InTask);

	void UpdateLayout();

	virtual FReply OnDrop(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent) override;

	virtual FReply OnDragDetected(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

protected:
	TWeakPtr<class FBXTLController> Controller = nullptr;

	TWeakObjectPtr<class UBXTask> CachedTask = nullptr;

	TSharedPtr<SBorder>	TrackPanelArea;

	TSharedPtr<SWidget>	InlineEditableTextBlock;

};