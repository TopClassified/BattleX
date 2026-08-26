#include "SBXTLGraphNode.h"

#include "SGraphPin.h"
#include "NodeFactory.h"
#include "Framework/Application/SlateApplication.h"
#include "Styling/AppStyle.h"

#include "BXTLGraphNode.h"
#include "BXTLEditor.h"
#include "BXTask.h"



#define LOCTEXT_NAMESPACE "SGraphNode"

void SBXTLGraphNode::Construct(const FArguments& InArgs, UBXTLGraphNode* InNode)
{
	GraphNode = InNode;

	UpdateGraphNode();
}

void SBXTLGraphNode::UpdateGraphNode()
{
	SGraphNode::UpdateGraphNode();

	// 覆盖Pin脚的颜色
	if (UBXTLGraphNode* GNode = Cast<UBXTLGraphNode>(GraphNode))
	{
		for (int32 i = 0; i < InputPins.Num(); ++i)
		{
			InputPins[i].Get().SetPinColorModifier(GNode->GetPinColor(InputPins[i]->GetPinObj()));
		}

		for (int32 i = 0; i < OutputPins.Num(); ++i)
		{
			OutputPins[i].Get().SetPinColorModifier(GNode->GetPinColor(OutputPins[i]->GetPinObj()));
		}
	}
}

bool SBXTLGraphNode::IsNameReadOnly() const
{
	return true;
}

int32 SBXTLGraphNode::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	// 先让基类正常绘制节点
	int32 FinalLayerId = SGraphNode::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

	// Debug:若当前节点对应的Task正在执行或处于结束残留期,叠加黄色高亮框(残留期内透明度衰减)
	UBXTLGraphNode* TLGraphNode = Cast<UBXTLGraphNode>(GraphNode);
	if (TLGraphNode && TLGraphNode->CachedTask)
	{
		const float HighlightAlpha = FBXTLEditor::GetTaskHighlightAlpha(TLGraphNode->CachedTask);
		if (HighlightAlpha > 0.0f)
		{
			const FLinearColor HighlightColor(1.0f, 0.85f, 0.0f, HighlightAlpha);

			const FVector2f NodeSize = AllottedGeometry.GetLocalSize();
			const float BorderThickness = 2.0f;
			const FSlateBrush* WhiteBrush = FAppStyle::GetBrush(TEXT("WhiteBrush"));

			// 上边
			FSlateDrawElement::MakeBox(OutDrawElements, FinalLayerId + 1, AllottedGeometry.ToPaintGeometry(FVector2f(NodeSize.X, BorderThickness), FSlateLayoutTransform(FVector2f(0.0f, 0.0f))), WhiteBrush, ESlateDrawEffect::None, HighlightColor);
			// 下边
			FSlateDrawElement::MakeBox(OutDrawElements, FinalLayerId + 1, AllottedGeometry.ToPaintGeometry(FVector2f(NodeSize.X, BorderThickness), FSlateLayoutTransform(FVector2f(0.0f, NodeSize.Y - BorderThickness))), WhiteBrush, ESlateDrawEffect::None, HighlightColor);
			// 左边
			FSlateDrawElement::MakeBox(OutDrawElements, FinalLayerId + 1, AllottedGeometry.ToPaintGeometry(FVector2f(BorderThickness, NodeSize.Y), FSlateLayoutTransform(FVector2f(0.0f, 0.0f))), WhiteBrush, ESlateDrawEffect::None, HighlightColor);
			// 右边
			FSlateDrawElement::MakeBox(OutDrawElements, FinalLayerId + 1, AllottedGeometry.ToPaintGeometry(FVector2f(BorderThickness, NodeSize.Y), FSlateLayoutTransform(FVector2f(NodeSize.X - BorderThickness, 0.0f))), WhiteBrush, ESlateDrawEffect::None, HighlightColor);

			return FinalLayerId + 2;
		}
	}

	return FinalLayerId;
}

#undef LOCTEXT_NAMESPACE
