#include "BXBuffGraph.h"

#include "UObject/ObjectSaveContext.h"

#include "BXBuffAsset.h"
#include "BXBuffGraphNode.h"



#pragma region Important
UBXBuffGraph::UBXBuffGraph()
{

}

UBXBuffGraph::~UBXBuffGraph()
{

}

void UBXBuffGraph::GenerateGraphNodeByTask(UBXTask* InTask, float InX, float InY)
{
	if (!InTask || CheckTaskNodeValid(InTask))
	{
		return;
	}

	float PosY = 0.0f;
	for (int32 i = 0; i < Nodes.Num(); ++i)
	{
		UEdGraphNode* Node = Nodes[i];
		if (!Node)
		{
			continue;
		}

		PosY = FMath::Max(PosY, Node->NodePosY);
	}
	PosY += 200.0f;

	if (UBXBuffGraphNode* ResultNode = NewObject<UBXBuffGraphNode>(this))
	{
		AddNode(ResultNode, true, true);

		ResultNode->CachedTask = InTask;
		ResultNode->Init();
		ResultNode->CreateNewGuid();
		ResultNode->PostPlacedNewNode();
		ResultNode->AllocateDefaultPins();
		ResultNode->UpdatePins();

		ResultNode->NodePosX = FMath::IsNearlyZero(InX) ? 0.0f : InX;
		ResultNode->NodePosY = FMath::IsNearlyZero(InY) ? PosY : InY;

		ResultNode->SetFlags(RF_Transactional);
	}
}

void UBXBuffGraph::PreSave(FObjectPreSaveContext ObjectSaveContext)
{
	Super::PreSave(ObjectSaveContext);

	UBXBuffAsset* BuffAsset = Cast<UBXBuffAsset>(GetOuter());
	if (!BuffAsset)
	{
		return;
	}

	// 从图表节点同步层级配置到TaskBindings
	BuffAsset->TaskBindings.Empty();

	for (int32 i = 0; i < Nodes.Num(); ++i)
	{
		if (UBXBuffGraphNode* BuffNode = Cast<UBXBuffGraphNode>(Nodes[i]))
		{
			if (BuffNode->CachedTask)
			{
				int32 Idx = BuffAsset->TaskBindings.AddDefaulted();
				FBXBuffTaskBinding& Binding = BuffAsset->TaskBindings[Idx];
				Binding.Task = BuffNode->CachedTask;
				Binding.MinLayer = BuffNode->MinLayer;
				Binding.MaxLayer = BuffNode->MaxLayer;
			}
		}
	}

	BuffAsset->Modify();
}

#pragma endregion Important
