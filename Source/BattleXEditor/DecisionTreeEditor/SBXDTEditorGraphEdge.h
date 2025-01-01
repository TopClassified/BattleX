#pragma once

#include "CoreMinimal.h"
#include "SNodePanel.h"
#include "SGraphNode.h"
#include "Styling/SlateColor.h"
#include "Widgets/SWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"



class SBXDTEditorGraphEdge : public SGraphNode
{
public:
	SLATE_BEGIN_ARGS(SBXDTEditorGraphEdge) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, class UBXDTEditorGraphEdge* InNode);

	virtual bool RequiresSecondPassLayout() const override;
	
	virtual void PerformSecondPassLayout(const TMap< UObject*, TSharedRef<SNode> >& NodeToWidgetLookup) const override;

	virtual void UpdateGraphNode() override;

	void PositionBetweenTwoNodesWithOffset(const FGeometry& StartGeom, const FGeometry& EndGeom, int32 NodeIndex, int32 MaxNodes) const;

protected:
	TSharedPtr<SBorder> NodeBody;

	FSlateColor GetEdgeColor() const;

	virtual bool IsNameReadOnly() const override
	{
		return true;
	}

	virtual FVector2D ComputeDesiredSize(float LayoutScaleMultiplier) const;

private:
	FText GetNodeTitle() const;

	TSharedPtr<STextEntryPopup> TextEntryWidget;
};
