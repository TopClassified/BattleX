#include "BXTLGraphTransitionNode.h"

#include "SBXTLGraphTransitionNode.h"



#define LOCTEXT_NAMESPACE "BXTLGraphTransitionNode" 

#pragma region Important
UBXTLGraphTransitionNode::UBXTLGraphTransitionNode()
{
	bCanRenameNode = false;
}

UBXTLGraphTransitionNode::~UBXTLGraphTransitionNode()
{

}

UEdGraphNode* UBXTLGraphTransitionNode::GetLinkTargetNode()
{
	if (Pins.IsValidIndex(1) && Pins[1]->LinkedTo.Num() > 0)
	{
		return Pins[1]->LinkedTo[0]->GetOwningNode();
	}

	return nullptr;
}

#pragma endregion Important



#pragma region Pin
void UBXTLGraphTransitionNode::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, TEXT("Transition"), TEXT("Delay"));
	CreatePin(EGPD_Output, TEXT("Transition"), TEXT("Trigger"));
}

void UBXTLGraphTransitionNode::CreateConnections(UEdGraphPin* StartPin, UEdGraphPin* EndPin)
{
	Pins[0]->Modify();
	Pins[0]->LinkedTo.Empty();
	Pins[0]->MakeLinkTo(StartPin);

	Pins[1]->Modify();
	Pins[1]->LinkedTo.Empty();
	Pins[1]->MakeLinkTo(EndPin);
}

void UBXTLGraphTransitionNode::PinConnectionListChanged(UEdGraphPin* Pin)
{
	if (Pin->LinkedTo.Num() <= 0)
	{
		if (UEdGraph* ParentGraph = GetGraph())
		{
			ParentGraph->Modify();
		}

		DestroyNode();
	}
}

#pragma endregion Pin

#undef LOCTEXT_NAMESPACE
