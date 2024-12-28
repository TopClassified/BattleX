#include "BXDecisionTreeNode.h"

#include "BXFunctionLibrary.h"
#include "BXConditionManager.h"



UBXDecisionTreeNode::UBXDecisionTreeNode()
{
#if WITH_EDITORONLY_DATA
	PinInformations.Empty();
	PinInformations.Add(FBXDTNodePin(false, TEXT("In")));
	PinInformations.Add(FBXDTNodePin(true, TEXT("Out")));
#endif

}

bool UBXDecisionTreeNode::CheckCondition(UObject* InWorldContext, UScriptStruct* InParameterType, void* InParameterAddress)
{
	if (!IsValid(InWorldContext))
	{
		return false;
	}

	UBXConditionManager* ConditionManager = UBXConditionManager::Get(InWorldContext);
	if (!IsValid(ConditionManager))
	{
		return false;
	}

	return ConditionManager->CheckCondition(Condition, InParameterType, InParameterAddress);
}

#if WITH_EDITOR
void UBXDecisionTreeNode::ConstructNode()
{
	RefreshNodeTitle();
}

void UBXDecisionTreeNode::CopyData(UBXDecisionTreeNode* OtherNode)
{
	if (!OtherNode)
		return;

	if (OtherNode->Condition)
	{
		Condition = NewObject<UBXCondition>(this, OtherNode->Condition->GetClass());
		UBXFunctionLibrary::CopyObject(Condition, OtherNode->Condition);
	}

	RefreshNodeTitle();
}

void UBXDecisionTreeNode::RefreshNodeTitle()
{
	if (IsValid(Condition))
	{
		NodeTitle = FText::FromString(Condition->GetDescription());
	}
}

FText UBXDecisionTreeNode::GetNodeTitle() const
{
	return NodeTitle;
}

void UBXDecisionTreeNode::SetNodeTitle(const FText& InText)
{
	NodeTitle = InText;
}

bool UBXDecisionTreeNode::CanCreateConnection(int32 SelfPinID, UBXDecisionTreeNode* Other, int32 OtherPinID, FText& ErrorMessage)
{
	return true;
}

bool UBXDecisionTreeNode::CanUserDelete() const
{
	return true;
}

const TArray<FBXDTNodePin>* UBXDecisionTreeNode::GetNodePinList()
{
	return &PinInformations;
}

void UBXDecisionTreeNode::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	RefreshNodeTitle();

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif
