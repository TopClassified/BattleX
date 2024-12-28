#include "BXDecisionTreeTemplate.h"



#if WITH_EDITOR
void UBXDecisionTreeTemplate::InitByEditor()
{
	return;
}

void UBXDecisionTreeTemplate::InitSaver()
{
	EditedNodes.Empty();

	RootNodes.Empty(50);
	TreeEdges.Empty(100);
}

void UBXDecisionTreeTemplate::AddEdgeMessage(UBXDecisionTreeNode* InFromNode, UBXDecisionTreeNode* InToNode, UBXDecisionTreeEdge* InEdge)
{
	if (!InFromNode || !InToNode || !InEdge)
		return;

	if (!EditedNodes.Contains(InFromNode))
	{
		InFromNode->OutEdges.Empty(5);
		InFromNode->InEdges.Empty(5);

		EditedNodes.Add(InFromNode);
	}

	if (!EditedNodes.Contains(InToNode))
	{
		InToNode->OutEdges.Empty(5);
		InToNode->InEdges.Empty(5);

		EditedNodes.Add(InToNode);
	}

	InEdge->StartNode = InFromNode;
	InEdge->EndNode = InToNode;

	// 将该条边添加到数组中
	TreeEdges.Add(InEdge);

	InFromNode->OutEdges.Add(TreeEdges.Num() - 1);
	InToNode->InEdges.Add(TreeEdges.Num() - 1);
	InEdge->RefreshEdgeTitle();
}

void UBXDecisionTreeTemplate::RefreshLogicMessage()
{
	return;
}
#endif