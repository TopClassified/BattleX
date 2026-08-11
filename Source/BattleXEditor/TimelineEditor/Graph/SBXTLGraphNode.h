#pragma once

#include "CoreMinimal.h"
#include "SGraphNode.h" 



class SBXTLGraphNode : public SGraphNode
{
public:
	SLATE_BEGIN_ARGS(SBXTLGraphNode) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, class UBXTLGraphNode* InNode);

	void UpdateGraphNode() override;

	bool IsNameReadOnly() const override;

	// Debug:重写OnPaint,在节点最上层叠加黄色呼吸高亮框(正在执行的Task)
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

};
