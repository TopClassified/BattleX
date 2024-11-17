#include "BXTLGraph.h"
#include "EdGraph/EdGraphSchema.h"

#include "BXTLAsset.h"
#include "BXFunctionLibrary.h"

#include "BXTLGraphNode.h"
#include "BXTLGraphTransitionNode.h" 



UBXTLGraph::UBXTLGraph()
{

}

UBXTLGraph::~UBXTLGraph()
{

}

void UBXTLGraph::Init()
{
	for (int32 i = 0; i < Nodes.Num(); ++i)
	{
		if (UBXTLGraphNode* Node = Cast<UBXTLGraphNode>(Nodes[i]))
		{
			Node->Init();
		}
	}
}

void UBXTLGraph::Uninit()
{
	for (int32 i = 0; i < Nodes.Num(); ++i)
	{
		if (UBXTLGraphNode* Node = Cast<UBXTLGraphNode>(Nodes[i]))
		{
			Node->Uninit();
		}
	}
}

void UBXTLGraph::PreSave(class FObjectPreSaveContext ObjectSaveContext)
{
	RefreshGraphInformation();

	Super::PreSave(ObjectSaveContext);
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
	for (int32 i = 0; i < Nodes.Num(); ++i)
	{
		for (int32 j = i + 1; j < Nodes.Num(); ++j)
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

	RefreshGraphInformation();
}

void UBXTLGraph::RefreshGraphInformation()
{
	for (int32 i = 0; i < Nodes.Num(); ++i)
	{
		if (UBXTLGraphNode* Node = Cast<UBXTLGraphNode>(Nodes[i]))
		{
			if (Node->CachedTask)
			{
				Node->CachedTask->TriggeredByList.Reset();
			}
		}
	}

	// 刷新任务信息
	for (int32 i = 0; i < Nodes.Num(); ++i)
	{
		if (UBXTLGraphNode* Node = Cast<UBXTLGraphNode>(Nodes[i]))
		{
			Node->RefreshGraphNodeInformation();
		}
	}

	Modify();
}

bool UBXTLGraph::CheckTaskNodeValid(UBXTask* InTask)
{
	for (int32 i = 0; i < Nodes.Num(); ++i)
	{
		if (UBXTLGraphNode* Node = Cast<UBXTLGraphNode>(Nodes[i]))
		{
			if (Node->CachedTask == InTask)
			{
				return true;
			}
		}
	}

	return false;
}

UEdGraphNode* UBXTLGraph::GetGraphNodeByTask(UBXTask* InTask)
{
	for (int32 i = 0; i < Nodes.Num(); ++i)
	{
		if (UBXTLGraphNode* Node = Cast<UBXTLGraphNode>(Nodes[i]))
		{
			if (Node->CachedTask == InTask)
			{
				return Nodes[i];
			}
		}
	}

	return nullptr;
}

void UBXTLGraph::GenerateGraphNodeByTask(UBXTask* InTask, float InX, float InY)
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

	if (UBXTLGraphNode* ResultNode = NewObject<UBXTLGraphNode>(this))
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

void UBXTLGraph::GenerateGraphNodesByTasks(TArray<UBXTask*>& InTaskList)
{
	if (InTaskList.Num() <= 0)
	{
		return;
	}

	int32 NodeY = 0;
	for (int32 i = 0; i < Nodes.Num(); ++i)
	{
		UEdGraphNode* Node = Nodes[i];
		if (!Node)
		{
			continue;
		}

		NodeY = FMath::Max(NodeY, Node->NodePosY);
	}
	NodeY += 300;

	int32 NodeX = 0;
	for (int32 i = 0; i < InTaskList.Num(); ++i)
	{
		UBXTask* Task = InTaskList[i];
		if (!Task)
		{
			continue;
		}

		bool bGenerateNode = false;

		// 判断有没有必要创建逻辑节点
		TArray<FBXTInputInfo>& CollisionInputs = Task->CollisionInputDatas;
		for (int32 j = 0; j < CollisionInputs.Num(); ++j)
		{
			if (CollisionInputs[j].DataTask.IsValid())
			{
				bGenerateNode = true;
				break;
			}
		}

		if (!bGenerateNode)
		{
			for (int32 j = 0; j < Task->InputDatas.Num(); ++j)
			{
				if (Task->InputDatas[j].DataTask.IsValid())
				{
					bGenerateNode = true;
					break;
				}
			}
		}

		if (!bGenerateNode)
		{
			// 自身有连接到外部的节点，需要生成自身节点
			for (TMap<FGameplayTag, FBXTEvent>::TIterator It(Task->Events); It; ++It)
			{
				if (It->Value.Event.Num() > 0)
				{
					bGenerateNode = true;
					break;
				}
			}
		}

		if (!bGenerateNode)
		{
			// 有其他节点连接到自身，也需要生成自身节点
			for (int32 k = 0; k < InTaskList.Num(); ++k)
			{
				UBXTask* OtherTask = InTaskList[k];
				if (!OtherTask || OtherTask == Task)
				{
					continue;
				}

				for (TMap<FGameplayTag, FBXTEvent>::TIterator It(OtherTask->Events); It; ++It)
				{
					if (It->Value.Event.Contains(Task))
					{
						bGenerateNode = true;
						break;
					}
				}

				if (bGenerateNode)
				{
					break;
				}
			}
		}

		if (!bGenerateNode)
		{
			continue;
		}

		GenerateGraphNodeByTask(InTaskList[i], NodeX, NodeY);

		NodeX += 300;
	}

	RefreshGraph();
}

void UBXTLGraph::DeleteGraphNodes(TArray<UEdGraphNode*>& InNodes)
{
	for (TArray<UEdGraphNode*>::TIterator It(InNodes); It; ++It)
	{
		if (UEdGraphNode* EdNode = Cast<UEdGraphNode>(*It))
		{
			EdNode->DestroyNode();
		}
	}

	RefreshGraph();
}

void UBXTLGraph::DeleteGraphNodeByTask(UBXTask* InTask)
{
	if (!InTask)
	{
		return;
	}

	for (int32 i = 0; i < Nodes.Num(); ++i)
	{
		if (UBXTLGraphNode* Node = Cast<UBXTLGraphNode>(Nodes[i]))
		{
			if (Node->CachedTask == InTask)
			{
				Node->DestroyNode();
				break;
			}
		}
	}

	RefreshGraph();
}

void UBXTLGraph::TryAutoConnectPin(UBXTLGraphNode* Node1, UBXTLGraphNode* Node2)
{
	if (!Node1 || !Node1->CachedTask || !Node2 || !Node2->CachedTask)
	{
		return;
	}

	UEdGraphPin* Pin1 = nullptr;
	UEdGraphPin* Pin2 = nullptr;
	
	// 更新Task事件触发列表
	for (TMap<FGameplayTag, FBXTEvent>::TIterator It(Node1->CachedTask->Events); It; ++It)
	{
		TMap<TSoftObjectPtr<UBXTask>, float> Events = It->Value.Event;
		for (TMap<TSoftObjectPtr<UBXTask>, float>::TIterator EIt(Events); EIt; ++EIt)
		{
			if (EIt->Key.Get() == Node2->CachedTask)
			{
				Pin1 = Node1->GetPinByName(It->Key.GetTagName());
				Pin2 = Node2->GetPinByName(FName("Exe"));

				if (Pin1 && Pin2 && !Pin1->LinkedTo.Contains(Pin2))
				{
					GetSchema()->TryCreateConnection(Pin1, Pin2);
				}
			}
		}
	}

	// 更新碰撞信息列表
	for (TArray<FBXTInputInfo>::TIterator It(Node1->CachedTask->CollisionInputDatas); It; ++It)
	{
		if (It->DataTask.Get() == Node2->CachedTask)
		{
			Pin1 = Node1->GetPinByName(It->GetDisplayName());
			Pin2 = Node2->GetPinByName(It->DataTag.GetTagName());

			if (Pin1 && Pin2 && !Pin1->LinkedTo.Contains(Pin2))
			{
				GetSchema()->TryCreateConnection(Pin1, Pin2);
			}
		}
	}

	// 修复InputDatas
	for (TArray<FBXTInputInfo>::TIterator It(Node1->CachedTask->InputDatas); It; ++It)
	{
		if (It->DataTask.Get() == Node2->CachedTask)
		{
			Pin1 = Node1->GetPinByName(It->GetDisplayName());
			Pin2 = Node2->GetPinByName(It->DataTag.GetTagName());

			if (Pin1 && Pin2 && !Pin1->LinkedTo.Contains(Pin2))
			{
				GetSchema()->TryCreateConnection(Pin1, Pin2);
			}
		}
	}
}

void UBXTLGraph::AlignTimeProperty(float InAlign)
{
	for (int32 i = 0; i < Nodes.Num(); ++i)
	{
		if (UBXTLGraphTransitionNode* Node = Cast<UBXTLGraphTransitionNode>(Nodes[i]))
		{
			Node->Delay = UBXFunctionLibrary::AlignTime(Node->Delay, InAlign);
		}
	}
}
