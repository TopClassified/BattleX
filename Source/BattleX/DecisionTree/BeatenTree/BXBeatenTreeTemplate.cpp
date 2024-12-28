#include "BXBeatenTreeTemplate.h"

#include "BXBeatenTreeEdge.h"



UBXBeatenTreeType::UBXBeatenTreeType()
{
#if WITH_EDITORONLY_DATA
	NodeTypes.Add(UBXBeatenTreeNode::StaticClass());
	EdgeType = UBXBeatenTreeEdge::StaticClass();
#endif
}






#if WITH_EDITOR
void UBXBeatenTreeTemplate::RefreshLogicMessage()
{
	Super::RefreshLogicMessage();

	TotalSkillNum = 0;
	VisitedNodes.Empty();

	for (int32 i = 0; i < TreeEdges.Num(); ++i)
	{
		if (UBXBeatenTreeEdge* CurBattleEdge = Cast<UBXBeatenTreeEdge>(TreeEdges[i]))
		{
			CurBattleEdge->EdgeShortestDepth = 100000;
		}
	}

	for (int32 i = 0; i < RootNodes.Num(); ++i)
	{
		UBXBeatenTreeNode* CurNode = Cast<UBXBeatenTreeNode>(RootNodes[i]);
		RefreshNodeMessage(CurNode, 1);
	}

	for (int32 i = 0; i < TreeEdges.Num(); ++i)
	{
		if (UBXBeatenTreeEdge* CurBattleEdge = Cast<UBXBeatenTreeEdge>(TreeEdges[i]))
		{
			CurBattleEdge->CheckConditionConfig();
		}
	}
}

void UBXBeatenTreeTemplate::RefreshNodeMessage(UBXBeatenTreeNode* InCurNode, int32 CurrentDepth)
{
	if (InCurNode)
	{
		if (!VisitedNodes.Contains(InCurNode))
		{
			TotalSkillNum += 1;
			VisitedNodes.Add(InCurNode);

			// 遍历该节点的边
			for (int32 i = 0; i < InCurNode->OutEdges.Num(); ++i)
			{
				UBXBeatenTreeEdge* CurEdge = Cast<UBXBeatenTreeEdge>(TreeEdges[InCurNode->OutEdges[i]]);
				if (!CurEdge)
					continue;

				if (CurEdge->EdgeShortestDepth > CurrentDepth)
					CurEdge->EdgeShortestDepth = CurrentDepth;

				UBXBeatenTreeNode* CurNode = Cast<UBXBeatenTreeNode>(CurEdge->EndNode);
				RefreshNodeMessage(CurNode, CurrentDepth + 1);
			}
		}
	}
}
#endif
