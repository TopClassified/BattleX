#include "BXDTEditorGraphNode.h"
#include "Kismet2/Kismet2NameValidators.h"

#include "BXDTEditorGraph.h"
#include "BXDTEditorGraphEdge.h"

#include "SBXDTEditorGraphNode.h"



#define LOCTEXT_NAMESPACE "BXDTEditorGraphNode"

UBXDTEditorGraphNode::UBXDTEditorGraphNode()
{
	bCanRenameNode = true;
}

UBXDTEditorGraphNode::~UBXDTEditorGraphNode()
{
	if (GraphNode && GraphNode->IsValidLowLevel() && !IsValid(GraphNode))
	{
		GraphNode->MarkAsGarbage();
	}
}

void UBXDTEditorGraphNode::AllocateDefaultPins()
{
	if (GraphNode)
	{
		const TArray<FBXDTNodePin>* List = GraphNode->GetNodePinList();
		for (int32 i = 0; i < List->Num(); ++i)
		{
			const FBXDTNodePin& CurPin = (*List)[i];
			if (CurPin.bOutput)
			{
				CreatePin(EGPD_Output, TEXT("DecisionTree"), TEXT("DTNode"), CurPin.PinName);
			}
			else
			{
				CreatePin(EGPD_Input, TEXT("DecisionTree"), TEXT("DTNode"), CurPin.PinName);
			}
		}
	}
}

UBXDTEditorGraph* UBXDTEditorGraphNode::GetDecisionTreeEdGraph()
{
	return Cast<UBXDTEditorGraph>(GetGraph());
}

FText UBXDTEditorGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	if (!GraphNode)
	{
		return Super::GetNodeTitle(TitleType);
	}
	else
	{
		return GraphNode->GetNodeTitle();
	}
}

void UBXDTEditorGraphNode::PrepareForCopying()
{
	
}

void UBXDTEditorGraphNode::SetDecisionTreeNode(UBXDecisionTreeNode* InNode)
{
	GraphNode = InNode;
}

FLinearColor UBXDTEditorGraphNode::GetBackgroundColor() const
{
	return FLinearColor::Black;
}

UEdGraphPin* UBXDTEditorGraphNode::GetPinByIndex(int32 InIndex) const
{
	if (Pins.IsValidIndex(InIndex))
	{
		return Pins[InIndex];
	}

	return nullptr;
}

int32 UBXDTEditorGraphNode::GetPinIndex(const UEdGraphPin* InPin) const
{
	for (int32 i = 0; i < Pins.Num(); ++i)
	{
		if (Pins[i] == InPin)
		{
			return i;
		}
	}

	return -1;
}

UEdGraphPin* UBXDTEditorGraphNode::GetPinByName(FName InName) const
{
	for (int32 i = 0; i < Pins.Num(); ++i)
	{
		if (Pins[i]->GetFName().IsEqual(InName))
		{
			return Pins[i];
		}
	}

	return nullptr;
}

bool UBXDTEditorGraphNode::CanUserDeleteNode() const
{
	if (GraphNode)
	{
		return GraphNode->CanUserDelete();
	}

	return true;
}

void UBXDTEditorGraphNode::PostEditUndo()
{
	UEdGraphNode::PostEditUndo();
}

void UBXDTEditorGraphNode::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	if (SEdNode.IsValid())
	{
		SEdNode.Pin()->UpdateGraphNode();
	}

	UEdGraphNode::PostEditChangeProperty(PropertyChangedEvent);
}

int32 UBXDTEditorGraphNode::GetNodeDepth()
{
	if (Pins[0]->LinkedTo.Num() == 0)
	{
		return 0;
	}

	int32 Depth = 10000;
	for (int32 i = 0; i < Pins[0]->LinkedTo.Num(); ++i)
	{
		int32 TmpDepth = 10000;
		UBXDTEditorGraphEdge* Edge = Cast<UBXDTEditorGraphEdge>(Pins[0]->LinkedTo[i]->GetOwningNode());
		if (UBXDTEditorGraphNode* FatherNode = Edge->GetStartNode())
		{
			TmpDepth = FatherNode->GetNodeDepth();
			if (TmpDepth < Depth)
			{
				Depth = TmpDepth;
			}
		}
	}

	return Depth + 1;
}

void UBXDTEditorGraphNode::DestroyNode()
{
	if (GraphNode)
	{
		GraphNode->MarkAsGarbage();
		GraphNode = nullptr;
	}

	Super::DestroyNode();
}

#undef LOCTEXT_NAMESPACE
