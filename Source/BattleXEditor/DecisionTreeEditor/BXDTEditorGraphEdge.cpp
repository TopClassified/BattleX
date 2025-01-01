#include "BXDTEditorGraphEdge.h"

#include "EdGraph/EdGraph.h"
#include "EdGraph/EdGraphPin.h"

#include "BXDTEditorGraphNode.h"



#define LOCTEXT_NAMESPACE "BXDTEditorGraphEdge"

UBXDTEditorGraphEdge::UBXDTEditorGraphEdge()
{
	
}

UBXDTEditorGraphEdge::~UBXDTEditorGraphEdge()
{
	if (GraphEdge && GraphEdge->IsValidLowLevel() && !IsValid(GraphEdge))
	{
		GraphEdge->MarkAsGarbage();
	}
}

void UBXDTEditorGraphEdge::AllocateDefaultPins()
{
	UEdGraphPin* Inputs = CreatePin(EGPD_Input, TEXT("Edge"), FName(), TEXT("In"));
	Inputs->bHidden = true;

	UEdGraphPin* Outputs = CreatePin(EGPD_Output, TEXT("Edge"), FName(), TEXT("Out"));
	Outputs->bHidden = true;
}

FText UBXDTEditorGraphEdge::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	if (!GraphEdge)
	{
		return Super::GetNodeTitle(TitleType);
	}
	else
	{
		return GraphEdge->GetEdgeTitle();
	}
}

void UBXDTEditorGraphEdge::PinConnectionListChanged(UEdGraphPin* Pin)
{
	if (Pin->LinkedTo.Num() == 0)
	{
		Modify();

		if (UEdGraph* ParentGraph = GetGraph())
		{
			ParentGraph->Modify();
		}

		DestroyNode();
	}
}

void UBXDTEditorGraphEdge::PrepareForCopying()
{
	
}

void UBXDTEditorGraphEdge::CreateConnections(UEdGraphPin* StartPin, UEdGraphPin* EndPin)
{
	Pins[0]->Modify();
	Pins[0]->LinkedTo.Empty();
	Pins[0]->MakeLinkTo(StartPin);

	Pins[1]->Modify();
	Pins[1]->LinkedTo.Empty();
	Pins[1]->MakeLinkTo(EndPin);
}

UBXDTEditorGraphNode* UBXDTEditorGraphEdge::GetStartNode()
{
	if (Pins[0]->LinkedTo.Num() > 0)
	{
		return Cast<UBXDTEditorGraphNode>(Pins[0]->LinkedTo[0]->GetOwningNode());
	}
	else
	{
		return nullptr;
	}
}

UBXDTEditorGraphNode* UBXDTEditorGraphEdge::GetEndNode()
{
	if (Pins[1]->LinkedTo.Num() > 0)
	{
		return Cast<UBXDTEditorGraphNode>(Pins[1]->LinkedTo[0]->GetOwningNode());
	}
	else
	{
		return nullptr;
	}
}

int32 UBXDTEditorGraphEdge::GetNodeDepth()
{
	UBXDTEditorGraphNode* StartNode = GetStartNode();
	if (StartNode)
	{
		return StartNode->GetNodeDepth();
	}

	return -1;
}

void UBXDTEditorGraphEdge::ResetEdgeMessage()
{
	
}

void UBXDTEditorGraphEdge::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	if (SEdEdge.IsValid())
	{
		SEdEdge.Pin()->UpdateGraphNode();
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);
}

#undef LOCTEXT_NAMESPACE
