#include "BXCombatTreeNode.h"



UBXCombatTreeRootNode::UBXCombatTreeRootNode()
{
	// NodeTitle为基类WITH_EDITORONLY_DATA属性,cooked打包构建下访问会编译失败
#if WITH_EDITORONLY_DATA
	NodeTitle = FText::FromString(TEXT("Start"));
#endif
}

#if WITH_EDITOR
void UBXCombatTreeRootNode::SetNodeTitle(const FText& NewTitle)
{
	
}

bool UBXCombatTreeRootNode::CanCreateConnection(int32 SelfPinID, UBXDecisionTreeNode* Other, int32 OtherPinID, FText& ErrorMessage)
{
	if (PinInformations.IsValidIndex(SelfPinID) && !PinInformations[SelfPinID].bOutput)
	{
		return false;
	}

	return true;
}

bool UBXCombatTreeRootNode::CanUserDelete() const
{
	return false;
}
#endif






UBXCombatTreeNode::UBXCombatTreeNode()
{

}

#if WITH_EDITOR
void UBXCombatTreeNode::SetNodeTitle(const FText& NewTitle)
{
	NodeTitle = NewTitle;
}

void UBXCombatTreeNode::CopyData(UBXDecisionTreeNode* OtherNode)
{
	Super::CopyData(OtherNode);

	if (UBXCombatTreeNode* Other = Cast<UBXCombatTreeNode>(OtherNode))
	{
		ReleaseSkillAsset = Other->ReleaseSkillAsset;
	}
}

void UBXCombatTreeNode::RefreshNodeTitle()
{
	FString Title;

	if (bCheckReleaseCondition)
	{
		Title += TEXT("Need Check Release Condition !\n");
	}
	
	Title += TEXT("Release ");
	Title += ReleaseSkillAsset.GetAssetName();
	if (UBXSkillAsset* Skill = ReleaseSkillAsset.LoadSynchronous())
	{
		Title += TEXT("  ");
		Title += Skill->Name.ToString();
	}

	NodeTitle = FText::FromString(Title);
}

void UBXCombatTreeNode::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	RefreshNodeTitle();

	Super::PostEditChangeProperty(PropertyChangedEvent);
}

void UBXCombatTreeNode::PostEditChangeChainProperty(struct FPropertyChangedChainEvent& PropertyChangedEvent)
{
	Super::PostEditChangeChainProperty(PropertyChangedEvent);
}
#endif
