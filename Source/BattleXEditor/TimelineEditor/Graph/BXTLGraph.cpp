#include "BXTLGraph.h"
#include "EdGraph/EdGraphSchema.h"

#include "BXTLAsset.h"

#include "BXTLGraphNode.h"



UBXTLGraph::UBXTLGraph()
{

}

UBXTLGraph::~UBXTLGraph()
{

}

void UBXTLGraph::RefreshGraph()
{
	UBXTLAsset* Asset = Cast<UBXTLAsset>(GetOuter());
	if (!Asset)
	{
		return;
	}

	// 删除无效节点
	for (int32 i = Nodes.Num() - 1; i >= 0; --i)
	{
		if (UBXTLGraphNode* Node = Cast<UBXTLGraphNode>(Nodes[i]))
		{
			if (!Node->CachedTask)
			{
				Node->DestroyNode();
			}
		}
	}

	// 刷新已有节点的Pin
	for (int32 i = 0; i < Nodes.Num(); ++i)
	{
		if (UBXTLGraphNode* Node = Cast<UBXTLGraphNode>(Nodes[i]))
		{
			Node->UpdatePins();
		}
	}

	// 根据现在的数据/事件传输关系，重建节点和连接关系
	for (int32 i = 0; i < Nodes.Num(); i++)
	{
		for (int32 j = i + 1; j < Nodes.Num(); j++)
		{
			UBXTLGraphNode* Node1 = Cast<UBXTLGraphNode>(Nodes[i]);
			UBXTLGraphNode* Node2 = Cast<UBXTLGraphNode>(Nodes[j]);
			if (Node1 && Node2)
			{
				TryAutoConnectPin(Node1, Node2);
				TryAutoConnectPin(Node2, Node1);
			}
		}
	}
}

void UBXTLGraph::GenerateGraphNodeByTask(UBXTask* InTask)
{
	if (!InTask)
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
	PosY += 400.0f;

	GenerateGraphNodeByTask(InTask, 0.0f, PosY);
}

void UBXTLGraph::GenerateGraphNodeByTask(UBXTask* InTask, float InPosX, float InPosY)
{
	if (!InTask)
	{
		return;
	}

	if (UBXTLGraphNode* ResultNode = NewObject<UBXTLGraphNode>(this))
	{
		AddNode(ResultNode, true, true);

		ResultNode->CachedTask = InTask;

		ResultNode->CreateNewGuid();
		ResultNode->PostPlacedNewNode();
		ResultNode->AllocateDefaultPins();
		ResultNode->UpdatePins();

		ResultNode->NodePosX = InPosX;
		ResultNode->NodePosY = InPosY;

		ResultNode->SetFlags(RF_Transactional);
	}

	Modify();
}

void UBXTLGraph::TryAutoConnectPin(UBXTLGraphNode* Node1, UBXTLGraphNode* Node2)
{
	if (!Node1 || !Node1->CachedTask || !Node2 || Node2->CachedTask)
	{
		return;
	}

	UEdGraphPin* Pin1 = nullptr;
	UEdGraphPin* Pin2 = nullptr;
	
	// 更新Task事件触发列表
	for (TMap<FName, FBXTEvent>::TIterator It(Node1->CachedTask->Events); It; ++It)
	{
		TMap<TSoftObjectPtr<UBXTask>, float> Events = It->Value.Event;
		for (TMap<TSoftObjectPtr<UBXTask>, float>::TIterator EIt(Events); EIt; ++EIt)
		{
			if (EIt->Key.Get() == Node2->CachedTask)
			{
				Pin1 = Node1->GetPinByName(It->Key);
				Pin2 = Node2->GetPinByName(FName("Exe"));

				if (Pin1 && Pin2 && !Pin1->LinkedTo.Contains(Pin2))
				{
					GetSchema()->TryCreateConnection(Pin1, Pin2);
				}
			}
		}
	}

	// 更新碰撞信息列表
	for (TArray<FBXTInputInfo>::TConstIterator It(Node1->CachedTask->CollisionInputDatas); It; ++It)
	{
		if (It->DataTask.Get() == Node2->CachedTask)
		{
			Pin1 = Node1->GetPinByName(It->DisplayName);
			Pin2 = Node2->GetPinByName(It->DataDesc);

			if (Pin1 && Pin2 && !Pin1->LinkedTo.Contains(Pin2))
			{
				GetSchema()->TryCreateConnection(Pin1, Pin2);
			}
		}
	}

	// 修复InputDatas
	for (TArray<FBXTInputInfo>::TConstIterator It(Node1->CachedTask->InputDatas); It; ++It)
	{
		if (It->DataTask.Get() == Node2->CachedTask)
		{
			Pin1 = Node1->GetPinByName(It->DisplayName);
			Pin2 = Node2->GetPinByName(It->DataDesc);

			if (Pin1 && Pin2 && !Pin1->LinkedTo.Contains(Pin2))
			{
				GetSchema()->TryCreateConnection(Pin1, Pin2);
			}
		}
	}
}
