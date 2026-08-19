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
	void Construct(const FArguments& InArgs, const TSharedPtr<class FTimelineController>& InTimelineController, int32 InGroupID);

	FReply OnDrop(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent) override;

protected:
	TWeakPtr<class FTimelineController> TimelineController = nullptr;

	// 组在Section.Groups内的索引(Undo/属性面板修改Groups会使数组重排,裸指针悬垂,OnDrop时按索引解析)
	int32 GroupID = INDEX_NONE;

};