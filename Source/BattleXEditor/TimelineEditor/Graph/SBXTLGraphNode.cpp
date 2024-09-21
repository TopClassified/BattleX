#include "SBXTLGraphNode.h"

#include "SGraphPin.h"
#include "NodeFactory.h"

#include "BXTLGraphNode.h"



#define LOCTEXT_NAMESPACE "SGraphNode"

void SBXTLGraphNode::Construct(const FArguments& InArgs, UBXTLGraphNode* InNode)
{
	GraphNode = InNode;

	UpdateGraphNode();

	InNode->NodeWidget = this;
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

#undef LOCTEXT_NAMESPACE
