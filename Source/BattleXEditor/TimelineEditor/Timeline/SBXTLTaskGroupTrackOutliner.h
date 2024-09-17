#pragma once

#include "CoreMinimal.h"
#include "Input/Reply.h"
#include "Misc/Attribute.h"

#include "Widgets/SBoxPanel.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"



class SBXTLTaskGroupTrackOutliner : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SBXTLTaskGroupTrackOutliner){}
	SLATE_ARGUMENT(TSharedPtr<class SWidget>, MainWidget)
	SLATE_END_ARGS()

public:
	void Construct(const FArguments& InArgs, const TSharedPtr<class FTimelineController>& InTimelineController, struct FBXTLTaskGroup& InGroupData);

	FReply OnDrop(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent) override;

protected:
	TWeakPtr<class FTimelineController> TimelineController = nullptr;

	struct FBXTLTaskGroup* GroupData = nullptr;

};