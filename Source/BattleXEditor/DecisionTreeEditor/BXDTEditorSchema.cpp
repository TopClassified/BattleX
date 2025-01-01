#include "BXDTEditorSchema.h"

#include "CoreMinimal.h"
#include "UObject/UObjectIterator.h"
#include "ScopedTransaction.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "EdGraph/EdGraph.h"
#include "EdGraph/EdGraphSchema.h"

#include "BXDecisionTreeTemplate.h"
#include "BXDTEditorGraphNode.h"
#include "BXDTEditorGraphEdge.h"
#include "BXDTConnectionDrawingPolicy.h"
#include "BXDTEditorGraph.h"


#define LOCTEXT_NAMESPACE "BXDTEditorSchema"

// 循环连接检查器
class FNodeCycleChecker
{
public:
	bool CheckForLoop(UEdGraphNode* StartNode, UEdGraphNode* EndNode)
	{
		CacheStartNode = StartNode;

		return TraverseInputNodesToRoot(EndNode);
	}

private:
	bool TraverseInputNodesToRoot(UEdGraphNode* Node)
	{
		for (int32 PinIndex = 0; PinIndex < Node->Pins.Num(); ++PinIndex)
		{
			UEdGraphPin* MyPin = Node->Pins[PinIndex];

			if (MyPin->Direction == EGPD_Output)
			{
				for (int32 LinkedPinIndex = 0; LinkedPinIndex < MyPin->LinkedTo.Num(); ++LinkedPinIndex)
				{
					if (UEdGraphPin* OtherPin = MyPin->LinkedTo[LinkedPinIndex])
					{
						if (CacheStartNode == OtherPin->GetOwningNode())
							return false;
						else if (!TraverseInputNodesToRoot(OtherPin->GetOwningNode()))
							return false;
					}
				}
			}
		}

		return true;
	}

	UEdGraphNode* CacheStartNode;
};






UEdGraphNode* FBXDTEditorSchemaAction_NewNode::PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode)
{
	UEdGraphNode* ResultNode = nullptr;

	if (NodeTemplate.IsValid())
	{
		const FScopedTransaction Transaction(LOCTEXT("DecisionTreeEditorNewNode", "DecisionTree Editor: New Node"));
		ParentGraph->Modify();
		if (FromPin)
		{
			FromPin->Modify();
		}

		NodeTemplate->Rename(nullptr, ParentGraph);
		ParentGraph->AddNode(NodeTemplate.Get(), true, bSelectNewNode);

		NodeTemplate->CreateNewGuid();
		NodeTemplate->PostPlacedNewNode();
		NodeTemplate->AllocateDefaultPins();
		NodeTemplate->AutowireNewNode(FromPin);

		NodeTemplate->NodePosX = Location.X;
		NodeTemplate->NodePosY = Location.Y;

		NodeTemplate->GraphNode->SetFlags(RF_Transactional);
		NodeTemplate->SetFlags(RF_Transactional);

		ResultNode = NodeTemplate.Get();
	}

	return ResultNode;
}

void FBXDTEditorSchemaAction_NewNode::AddReferencedObjects(FReferenceCollector& Collector)
{
	FEdGraphSchemaAction::AddReferencedObjects(Collector);
	Collector.AddReferencedObject(NodeTemplate);
}






UEdGraphNode* FBXDTEditorSchemaAction_NewEdge::PerformAction
(
	class UEdGraph* ParentGraph, UEdGraphPin* FromPin, 
	const FVector2D Location, bool bSelectNewNode /*= true*/
)
{
	UEdGraphNode* ResultNode = nullptr;

	if (NodeTemplate.IsValid())
	{
		const FScopedTransaction Transaction(LOCTEXT("DecisionTreeEditorNewEdge", "DecisionTree Editor: New Edge"));
		ParentGraph->Modify();
		if (FromPin != nullptr)
		{
			FromPin->Modify();
		}

		NodeTemplate->Rename(nullptr, ParentGraph);
		ParentGraph->AddNode(NodeTemplate.Get(), true, bSelectNewNode);

		NodeTemplate->CreateNewGuid();
		NodeTemplate->PostPlacedNewNode();
		NodeTemplate->AllocateDefaultPins();
		NodeTemplate->AutowireNewNode(FromPin);

		NodeTemplate->NodePosX = Location.X;
		NodeTemplate->NodePosY = Location.Y;

		NodeTemplate->GraphEdge->SetFlags(RF_Transactional);
		NodeTemplate->SetFlags(RF_Transactional);

		ResultNode = NodeTemplate.Get();
	}

	return ResultNode;
}

void FBXDTEditorSchemaAction_NewEdge::AddReferencedObjects(FReferenceCollector& Collector)
{
	FEdGraphSchemaAction::AddReferencedObjects(Collector);
	Collector.AddReferencedObject(NodeTemplate);
}






void UBXDTEditorSchema::GetBreakLinkToSubMenuActions(class FMenuBuilder& MenuBuilder, class UEdGraphPin* InGraphPin)
{
	TMap< FString, uint32 > LinkTitleCount;

	for (TArray<class UEdGraphPin*>::TConstIterator Links(InGraphPin->LinkedTo); Links; ++Links)
	{
		UEdGraphPin* Pin = *Links;
		FString TitleString = Pin->GetOwningNode()->GetNodeTitle(ENodeTitleType::ListView).ToString();
		FText Title = FText::FromString(TitleString);
		if (Pin->PinName != TEXT(""))
		{
			TitleString = FString::Printf(TEXT("%s (%s)"), *TitleString, *Pin->PinName.ToString());

			FFormatNamedArguments Args;
			Args.Add(TEXT("NodeTitle"), Title);
			Args.Add(TEXT("PinName"), Pin->GetDisplayName());
			Title = FText::Format(LOCTEXT("BreakDescPin", "{NodeTitle} ({PinName})"), Args);
		}

		uint32 &Count = LinkTitleCount.FindOrAdd(TitleString);

		FText Description;
		FFormatNamedArguments Args;
		Args.Add(TEXT("NodeTitle"), Title);
		Args.Add(TEXT("NumberOfNodes"), Count);

		if (Count == 0)
		{
			Description = FText::Format(LOCTEXT("BreakDesc", "Break link to {NodeTitle}"), Args);
		}
		else
		{
			Description = FText::Format(LOCTEXT("BreakDescMulti", "Break link to {NodeTitle} ({NumberOfNodes})"), Args);
		}
		++Count;

		MenuBuilder.AddMenuEntry
		(
			Description, Description, FSlateIcon(), 
			FUIAction
			(
				FExecuteAction::CreateUObject(this, &UBXDTEditorSchema::BreakSinglePinLink, const_cast<UEdGraphPin*>(InGraphPin), *Links)
			)
		);
	}
}

EGraphType UBXDTEditorSchema::GetGraphType(const UEdGraph* TestEdGraph) const
{
	return GT_StateMachine;
}

void UBXDTEditorSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	// 获取节点类型信息
	TArray<TSubclassOf<UBXDecisionTreeNode>> NodeTypes;
	if (UBXDecisionTreeTemplate* TreeTemplate = Cast<UBXDecisionTreeTemplate>(ContextMenuBuilder.CurrentGraph->GetOuter()))
	{
		if (TreeTemplate->TreeType)
		{
			if (UBXDecisionTreeType* CurMsg = Cast<UBXDecisionTreeType>(TreeTemplate->TreeType.GetDefaultObject()))
			{
				NodeTypes.Append(CurMsg->NodeTypes);
			}
		}
	}
	
	if (NodeTypes.Num() > 0)
	{
		const FText AddToolTip = LOCTEXT("NewDecisionTreeNodeTooltip", "Add node here");

		for (int32 i = 0; i < NodeTypes.Num(); ++i)
		{
			if (!NodeTypes[i]->HasAnyClassFlags(CLASS_Abstract))
			{
				FString Title = NodeTypes[i]->GetName();
				Title.RemoveFromEnd("_C");
				FText Desc = FText::FromString(Title);

				TSharedPtr<FBXDTEditorSchemaAction_NewNode> NewNodeAction(new FBXDTEditorSchemaAction_NewNode(LOCTEXT("DecisionTreeNodeAction", "DecisionTree Node"), Desc, AddToolTip, 0));

				NewNodeAction->NodeTemplate = NewObject<UBXDTEditorGraphNode>(ContextMenuBuilder.OwnerOfTemporaries);
				NewNodeAction->NodeTemplate->GraphNode = NewObject<UBXDecisionTreeNode>(ContextMenuBuilder.CurrentGraph->GetOuter(), NodeTypes[i]);
				NewNodeAction->NodeTemplate->GraphNode->ConstructNode();
				ContextMenuBuilder.AddAction(NewNodeAction);
			}
		}
	}

}

void UBXDTEditorSchema::GetContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const
{
	Super::GetContextMenuActions(Menu, Context);
}

const FPinConnectionResponse UBXDTEditorSchema::CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const
{
	UBXDTEditorGraphNode* Node = Cast<UBXDTEditorGraphNode>(A->GetOwningNode());
	UBXDTEditorGraphNode* EndNode = Cast<UBXDTEditorGraphNode>(B->GetOwningNode());

	if (A->GetOwningNode() == B->GetOwningNode())
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, LOCTEXT("PinErrorSameNode", "Both are on the same node"));
	}

	if ((A->Direction == EGPD_Input) && (B->Direction == EGPD_Input))
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, LOCTEXT("PinErrorInput", "Can't connect input node to input node"));
	}
	else if ((A->Direction == EGPD_Output) && (B->Direction == EGPD_Output))
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, LOCTEXT("PinErrorOutput", "Can't connect output node to output node"));
	}
	
	// 检查环路
	UBXDecisionTreeType* TreeType = nullptr;
	if (UBXDecisionTreeTemplate* TreeTemplate = Cast<UBXDecisionTreeTemplate>(Node->GetGraph()->GetOuter()))
	{
		if (TreeTemplate->TreeType)
		{
			TreeType = Cast<UBXDecisionTreeType>(TreeTemplate->TreeType.GetDefaultObject());
		}
	}
	if (!TreeType->bAllowCycle)
	{
		// 检查是否出现环路
		FNodeCycleChecker CycleChecker;
		if (!CycleChecker.CheckForLoop(A->GetOwningNode(), B->GetOwningNode()))
		{
			return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, LOCTEXT("PinErrorCycle", "Can't create a graph cycle"));
		}
	}
	
	UBXDTEditorGraphNode* EdNode_A = Cast<UBXDTEditorGraphNode>(A->GetOwningNode());
	UBXDTEditorGraphNode* EdNode_B = Cast<UBXDTEditorGraphNode>(B->GetOwningNode());
	if (!EdNode_A || !EdNode_B)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, LOCTEXT("PinError", "Not a valid UGenericGraphEdNode"));
	}
	
	FText ErrorMessage;
	if (A->Direction == EGPD_Input)
	{
		if (!EdNode_A->GraphNode->CanCreateConnection(EdNode_A->GetPinIndex(A), EdNode_B->GraphNode, EdNode_B->GetPinIndex(B), ErrorMessage))
		{
			return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, ErrorMessage);
		}
	}
	else
	{
		if (!EdNode_B->GraphNode->CanCreateConnection(EdNode_B->GetPinIndex(B), EdNode_A->GraphNode, EdNode_A->GetPinIndex(A), ErrorMessage))
		{
			return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, ErrorMessage);
		}
	}

	return FPinConnectionResponse(CONNECT_RESPONSE_MAKE_WITH_CONVERSION_NODE, LOCTEXT("PinConnect", "Connect nodes with edge"));
}

bool UBXDTEditorSchema::CreateAutomaticConversionNodeAndConnections(UEdGraphPin* A, UEdGraphPin* B) const
{
	UBXDTEditorGraphNode* NodeA = Cast<UBXDTEditorGraphNode>(A->GetOwningNode());
	UBXDTEditorGraphNode* NodeB = Cast<UBXDTEditorGraphNode>(B->GetOwningNode());
	if (!NodeA || !NodeB)
	{
		return false;
	}

	FVector2D InitPos((NodeA->NodePosX + NodeB->NodePosX) / 2, (NodeA->NodePosY + NodeB->NodePosY) / 2);
	
	// 获取边类型信息
	TSubclassOf<UBXDecisionTreeEdge> EdgeType;
	if (UBXDecisionTreeTemplate* TreeTemplate = Cast<UBXDecisionTreeTemplate>(NodeA->GetGraph()->GetOuter()))
	{
		if (TreeTemplate->TreeType)
		{
			if (UBXDecisionTreeType* TreeType = Cast<UBXDecisionTreeType>(TreeTemplate->TreeType.GetDefaultObject()))
			{
				EdgeType = TreeType->EdgeType;
			}
		}
	}
	
	if (EdgeType)
	{
		FBXDTEditorSchemaAction_NewEdge Action;
		Action.NodeTemplate = NewObject<UBXDTEditorGraphEdge>(NodeA->GetGraph());
		Action.NodeTemplate->GraphEdge = NewObject<UBXDecisionTreeEdge>(NodeA->GetGraph()->GetOuter(), EdgeType);

		UBXDTEditorGraphEdge* EdgeNode = Cast<UBXDTEditorGraphEdge>(Action.PerformAction(NodeA->GetGraph(), nullptr, InitPos, false));

		if (A->Direction == EGPD_Output)
		{
			EdgeNode->CreateConnections(A, B);
		}
		else
		{
			EdgeNode->CreateConnections(B, A);
		}
		
		Action.NodeTemplate->GraphEdge->ConstructEdge();
	}

	return true;
}

class FConnectionDrawingPolicy* UBXDTEditorSchema::CreateConnectionDrawingPolicy(int32 InBackLayerID, int32 InFrontLayerID, float InZoomFactor, const FSlateRect& InClippingRect, class FSlateWindowElementList& InDrawElements, class UEdGraph* InGraphObj) const
{
	return new FBXDTConnectionDrawingPolicy(InBackLayerID, InFrontLayerID, InZoomFactor, InClippingRect, InDrawElements, InGraphObj);
}

FLinearColor UBXDTEditorSchema::GetPinTypeColor(const FEdGraphPinType& PinType) const
{
	return FColor::White;
}

void UBXDTEditorSchema::BreakNodeLinks(UEdGraphNode& TargetNode) const
{
	UBXDTEditorGraphNode* Node = Cast<UBXDTEditorGraphNode>(&TargetNode);

	const FScopedTransaction Transaction(NSLOCTEXT("UnrealEd", "GraphEd_BreakNodeLinks", "Break Node Links"));

	Super::BreakNodeLinks(TargetNode);
}

void UBXDTEditorSchema::BreakPinLinks(UEdGraphPin& TargetPin, bool bSendsNodeNotifcation) const
{
	UBXDTEditorGraphNode* Node = Cast<UBXDTEditorGraphNode>(TargetPin.GetOwningNode());

	const FScopedTransaction Transaction(NSLOCTEXT("UnrealEd", "GraphEd_BreakPinLinks", "Break Pin Links"));

	Super::BreakPinLinks(TargetPin, bSendsNodeNotifcation);
}

void UBXDTEditorSchema::BreakSinglePinLink(UEdGraphPin* SourcePin, UEdGraphPin* TargetPin) const
{
	UBXDTEditorGraphNode* Node = Cast<UBXDTEditorGraphNode>(SourcePin->GetOwningNode());

	const FScopedTransaction Transaction(NSLOCTEXT("UnrealEd", "GraphEd_BreakSinglePinLink", "Break Pin Link"));

	Super::BreakSinglePinLink(SourcePin, TargetPin);
}

UEdGraphPin* UBXDTEditorSchema::DropPinOnNode(UEdGraphNode* InTargetNode, const FName& InSourcePinName, const FEdGraphPinType& InSourcePinType, EEdGraphPinDirection InSourcePinDirection) const
{
	UBXDTEditorGraphNode* EdNode = Cast<UBXDTEditorGraphNode>(InTargetNode);

	return EdNode->GetPinByName(InSourcePinName);
}

bool UBXDTEditorSchema::SupportsDropPinOnNode(UEdGraphNode* InTargetNode, const FEdGraphPinType& InSourcePinType, EEdGraphPinDirection InSourcePinDirection, FText& OutErrorMessage) const
{
	return Cast<UBXDTEditorGraphNode>(InTargetNode) != nullptr;
}

#undef LOCTEXT_NAMESPACE
