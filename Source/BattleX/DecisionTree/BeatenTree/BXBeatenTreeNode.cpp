#include "BXBeatenTreeNode.h"



UBXBeatenTreeNode::UBXBeatenTreeNode()
{

}

#if WITH_EDITOR
void UBXBeatenTreeNode::SetNodeTitle(const FText& NewTitle)
{
	NodeTitle = NewTitle;
}

void UBXBeatenTreeNode::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{


	Super::PostEditChangeProperty(PropertyChangedEvent);
}

void UBXBeatenTreeNode::PostEditChangeChainProperty(struct FPropertyChangedChainEvent& PropertyChangedEvent)
{


	Super::PostEditChangeChainProperty(PropertyChangedEvent);
}
#endif
