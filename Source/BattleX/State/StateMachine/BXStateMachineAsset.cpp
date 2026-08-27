#include "BXStateMachineAsset.h"

#include "DecisionTree/BXDecisionTreeEdge.h"



UBXStateMachineType::UBXStateMachineType()
{
#if WITH_EDITORONLY_DATA
	// 状态机允许环(硬直链回环)
	bAllowCycle = true;
	NodeTypes.Add(UBXSMStateNode::StaticClass());
	EdgeType = UBXSMTransitionEdge::StaticClass();
#endif
}



UBXSMStateNode* UBXStateMachineAsset::FindStateNode(const FGameplayTag& InStateTag) const
{
	for (UBXDecisionTreeNode* Node : RootNodes)
	{
		if (UBXSMStateNode* SMNode = Cast<UBXSMStateNode>(Node))
		{
			if (SMNode->StateTag == InStateTag)
			{
				return SMNode;
			}
		}
	}

	return nullptr;
}

UBXSMStateNode* UBXStateMachineAsset::FindDefaultStateNode() const
{
	for (UBXDecisionTreeNode* Node : RootNodes)
	{
		if (UBXSMStateNode* SMNode = Cast<UBXSMStateNode>(Node))
		{
			if (SMNode->bIsDefaultNode)
			{
				return SMNode;
			}
		}
	}

	return nullptr;
}

void UBXStateMachineAsset::CollectStateTags(FGameplayTagContainer& OutTags) const
{
	OutTags.Reset();

	for (UBXDecisionTreeNode* Node : RootNodes)
	{
		if (UBXSMStateNode* SMNode = Cast<UBXSMStateNode>(Node))
		{
			if (SMNode->StateTag.IsValid())
			{
				OutTags.AddTag(SMNode->StateTag);
			}
		}
	}
}

#if WITH_EDITOR
void UBXStateMachineAsset::RefreshLogicMessage()
{
	Super::RefreshLogicMessage();

	// 重建状态Tag→节点缓存(逐Tag检测重复:仅统计有效Tag的SM节点,旧实现Tags.Num()!=RootNodes.Num()会被非SM节点/空Tag节点误报)
	StateNodeMap.Reset();

	for (UBXDecisionTreeNode* Node : RootNodes)
	{
		if (UBXSMStateNode* SMNode = Cast<UBXSMStateNode>(Node))
		{
			if (SMNode->StateTag.IsValid())
			{
				// 状态Tag冲突诊断(同资产内重复Tag会导致转移歧义,TMap覆盖后者保留首个)
				if (StateNodeMap.Contains(SMNode->StateTag))
				{
					UE_LOG(BXSTATE, Warning, TEXT("UBXStateMachineAsset: duplicate StateTag %s in asset %s"), *SMNode->StateTag.ToString(), *GetName());
				}

				StateNodeMap.Add(SMNode->StateTag, SMNode);
			}
		}
	}
}
#endif



UBXSMStateNode::UBXSMStateNode()
{

}

#if WITH_EDITOR
void UBXSMStateNode::RefreshNodeTitle()
{
	NodeTitle = FText::FromString(FString::Printf(TEXT("%s\n时长: %.1f%s"), *StateTag.ToString(), Duration, bIsDefaultNode ? TEXT("\n[默认]") : TEXT("")));
}

void UBXSMStateNode::SetNodeTitle(const FText& NewTitle)
{
	// 状态节点标题由StateTag/Duration派生,禁止外部覆盖
	RefreshNodeTitle();
}

void UBXSMStateNode::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	RefreshNodeTitle();

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif



#if WITH_EDITOR
void UBXSMTransitionEdge::ConstructEdge()
{
	RefreshEdgeTitle();
}

void UBXSMTransitionEdge::RefreshEdgeTitle()
{
	// 空条件=无条件通过(评估侧约定),标题只显示评估时机
	const FString TypeString = (EvaluateType == EBXTransitionEvaluate::TE_OnExpired) ? TEXT("到期") : TEXT("每帧");
	if (IsValid(Condition))
	{
		EdgeTitle = FText::FromString(FString::Printf(TEXT("[%s] %s"), *TypeString, *Condition->GetDescription()));
	}
	else
	{
		EdgeTitle = FText::FromString(FString::Printf(TEXT("[%s] 无条件"), *TypeString));
	}
}
#endif
