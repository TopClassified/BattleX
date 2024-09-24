#include "BXTLGraphSchema.h"
#include "EdGraph/EdGraph.h"
#include "ScopedTransaction.h"
#include "GraphEditorActions.h"

#include "BXTLAsset.h"
#include "BXTLGraphNode.h"
#include "BXTLGraphTransitionNode.h"

#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Framework/Commands/GenericCommands.h" 



#define LOCTEXT_NAMESPACE "BXTLGraphSchema"

UEdGraphNode* FBXTLGCreatTaskNode::PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode)
{
	if (TaskPointer.IsValid())
	{
		ParentGraph->Modify();

		if (FromPin)
		{
			FromPin->Modify();
		}

		if (UBXTLGraphNode* ResultNode = NewObject<UBXTLGraphNode>(ParentGraph))
		{
			ResultNode->CachedTask = TaskPointer.Get();

			ParentGraph->AddNode(ResultNode, true, bSelectNewNode);

			ResultNode->CreateNewGuid();
			ResultNode->PostPlacedNewNode();
			ResultNode->AllocateDefaultPins();
			ResultNode->AutowireNewNode(FromPin);

			ResultNode->NodePosX = Location.X;
			ResultNode->NodePosY = Location.Y;

			ResultNode->SetFlags(RF_Transactional);

			return ResultNode;
		}
	}

	return nullptr;
}

void FBXTLGCreatTaskNode::AddReferencedObjects(FReferenceCollector& Collector)
{
	FEdGraphSchemaAction::AddReferencedObjects(Collector);

	Collector.AddReferencedObject(TaskPointer);
}






// 循环连接检查器
class FBXTLGraphCycleChecker
{
public:
	bool CheckForLoop(UEdGraphNode* InputNode, UEdGraphNode* OutputNode)
	{
		CacheOutputNode = OutputNode;

		return TraverseInputNodesToRoot(InputNode);
	}

private:
	bool TraverseInputNodesToRoot(UEdGraphNode* Node)
	{
		for (int32 PinIndex = 0; PinIndex < Node->Pins.Num(); ++PinIndex)
		{
			UEdGraphPin* MyPin = Node->Pins[PinIndex];

			if (MyPin->Direction == EGPD_Output)
			{
				for (int32 LinkedPinIndex = 0; LinkedPinIndex < MyPin->LinkedTo.Num(); ++LinkedPinIndex)
				{
					if (UEdGraphPin* OtherPin = MyPin->LinkedTo[LinkedPinIndex])
					{
						if (CacheOutputNode == OtherPin->GetOwningNode())
							return false;
						else if (!TraverseInputNodesToRoot(OtherPin->GetOwningNode()))
							return false;
					}
				}
			}
		}

		return true;
	}

	UEdGraphNode* CacheOutputNode;
};






FBXTLGConnectionDrawingPolicy::FBXTLGConnectionDrawingPolicy(int32 InBackLayerID, int32 InFrontLayerID, float ZoomFactor, const FSlateRect& InClippingRect, FSlateWindowElementList& InDrawElements) : FConnectionDrawingPolicy(InBackLayerID, InFrontLayerID, ZoomFactor, InClippingRect, InDrawElements)
{

}

void FBXTLGConnectionDrawingPolicy::DetermineWiringStyle(UEdGraphPin* OutputPin, UEdGraphPin* InputPin, FConnectionParams& Params)
{
	FConnectionDrawingPolicy::DetermineWiringStyle(OutputPin, InputPin, Params);

	if (UBXTLGraphNode* LGNode = Cast<UBXTLGraphNode>(OutputPin->GetOwningNode()))
	{
		Params.WireColor = LGNode->GetPinColor(OutputPin);
	}
}






EGraphType UBXTLGraphSchema::GetGraphType(const UEdGraph* TestEdGraph) const
{
	return GT_StateMachine;
}

const FPinConnectionResponse UBXTLGraphSchema::CanCreateConnection(const UEdGraphPin* InPinA, const UEdGraphPin* InPinB) const
{
	// 首先检查两个Pin的类型是不是一个输入一个输出
	if (!InPinA || !InPinB)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, LOCTEXT("Connect Error", "Invalid Pin!"));
	}
	if (InPinA->Direction == InPinB->Direction)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, LOCTEXT("Connect Error", "Direction Type Is Same!"));
	}

	const UEdGraphPin* InputPin = (InPinA->Direction == EEdGraphPinDirection::EGPD_Input) ? InPinA : InPinB;
	const UEdGraphPin* OutputPin = (InPinA->Direction == EEdGraphPinDirection::EGPD_Output) ? InPinA : InPinB;

	// 其次检查是否会出现环路
	UBXTLGraphNode* InputNode = Cast<UBXTLGraphNode>(InputPin->GetOwningNode());
	UBXTLGraphNode* OutputNode = Cast<UBXTLGraphNode>(OutputPin->GetOwningNode());
	if (!InputNode || !OutputNode)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, LOCTEXT("Connect Error", "Invalid Node!"));
	}
	if (UBXTLAsset* Asset = Cast<UBXTLAsset>(InputNode->CachedTask->GetOuter()))
	{
		int32 InputSectionID;
		int32 InputGroupID;
		Asset->GetSectionIDAndGroupID(InputNode->CachedTask, InputSectionID, InputGroupID);

		int32 OutputSectionID;
		int32 OutputGroupID;
		Asset->GetSectionIDAndGroupID(OutputNode->CachedTask, OutputSectionID, OutputGroupID);

		if (InputSectionID != OutputSectionID)
		{
			return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, LOCTEXT("Connect Error", "Not Same Task Section!"));
		}
	}
	FBXTLGraphCycleChecker CycleChecker;
	if (!CycleChecker.CheckForLoop(InputNode, OutputNode))
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, LOCTEXT("Connect Error", "Can't Create A Graph Cycle!"));
	}

	// 获取Pin的信息
	FBXTLGNodePin* InputInfo = InputNode->GetPinInformation(InputPin);
	FBXTLGNodePin* OutputInfo = OutputNode->GetPinInformation(OutputPin);
	if (!InputInfo || !OutputInfo)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, LOCTEXT("Connect Error", "Invalid Pin!"));
	}

	// 如果是事件输入/输出相关
	if (InputInfo->PinType == 0)
	{
		if (OutputInfo->PinType == 1)
		{
			return FPinConnectionResponse(CONNECT_RESPONSE_MAKE_WITH_CONVERSION_NODE, LOCTEXT("Connect Sucess", "Add Task Event To Start New Task!"));
		}
		else
		{
			return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, LOCTEXT("Connect Error", "Pin Type Error!"));
		}
	}
	else if (InputInfo->PinType == 2 || InputInfo->PinType == 3)
	{
		EBXDataType InputDataType = EBXDataType::DT_TMax;
		UScriptStruct* InputDataStructType = nullptr;
		InputNode->GetPinDataType(InputPin, InputDataType, InputDataStructType);

		EBXDataType OutputDataType = EBXDataType::DT_TMax;
		UScriptStruct* OutputDataStructType = nullptr;
		OutputNode->GetPinDataType(OutputPin, OutputDataType, OutputDataStructType);

		if (InputDataType != EBXDataType::DT_TMax && InputDataType == OutputDataType)
		{
			ECanCreateConnectionResponse ConnectResponse;
			if (InputPin == InPinA)
			{
				ConnectResponse = CONNECT_RESPONSE_BREAK_OTHERS_A;
			}
			else
			{
				ConnectResponse = CONNECT_RESPONSE_BREAK_OTHERS_B;
			}

			if (InputDataType == EBXDataType::DT_Struct)
			{
				if (InputDataStructType == OutputDataStructType)
				{
					return FPinConnectionResponse(ConnectResponse, LOCTEXT("Connect Sucess", "Transfer Task Struct Data!"));
				}
			}
			else
			{
				return FPinConnectionResponse(ConnectResponse, LOCTEXT("Connect Sucess", "Transfer Task Data!"));
			}
		}

		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, LOCTEXT("Connect Error", "Data Type Mismatch!"));
	}


	return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, LOCTEXT("Connect Error", "No Reason."));
}

FConnectionDrawingPolicy* UBXTLGraphSchema::CreateConnectionDrawingPolicy(int32 InBackLayerID, int32 InFrontLayerID, float InZoomFactor, const FSlateRect& InClippingRect, class FSlateWindowElementList& InDrawElements, class UEdGraph* InGraphObj) const
{
	return new FBXTLGConnectionDrawingPolicy(InBackLayerID, InFrontLayerID, InZoomFactor, InClippingRect, InDrawElements);
}

bool UBXTLGraphSchema::CreateAutomaticConversionNodeAndConnections(UEdGraphPin* A, UEdGraphPin* B) const
{
	UBXTLGraphNode* NodeA = Cast<UBXTLGraphNode>(A->GetOwningNode());
	UBXTLGraphNode* NodeB = Cast<UBXTLGraphNode>(B->GetOwningNode());

	UEdGraph* Graph = NodeA->GetGraph();
	if (!Graph)
	{
		return Super::CreateAutomaticConversionNodeAndConnections(A, B);
	}

	if (!NodeA || !NodeB)
	{
		return Super::CreateAutomaticConversionNodeAndConnections(A, B);
	}

	int32 PinAType = -1;
	if (FBXTLGNodePin* PinAInfo = NodeA->GetPinInformation(A))
	{
		PinAType = PinAInfo->PinType;
	}

	int32 PinBType = -1;
	if (FBXTLGNodePin* PinBInfo = NodeB->GetPinInformation(B))
	{
		PinBType = PinBInfo->PinType;
	}

	if (PinAType != 1 || PinBType != 0)
	{
		return Super::CreateAutomaticConversionNodeAndConnections(A, B);
	}

	if (UBXTLGraphTransitionNode* TransitNode = NewObject<UBXTLGraphTransitionNode>(NodeA->GetGraph()))
	{
		Graph->AddNode(TransitNode, true, false);

		TransitNode->CreateNewGuid();
		TransitNode->PostPlacedNewNode();
		TransitNode->AllocateDefaultPins();
		TransitNode->AutowireNewNode(A);
		TransitNode->SetFlags(RF_Transactional);
		TransitNode->NodePosX = (NodeA->NodePosX + NodeB->NodePosX) * 0.5f;
		TransitNode->NodePosY = (NodeA->NodePosY + NodeB->NodePosY) * 0.5f;
		
		if (A->Direction == EGPD_Output)
		{
			TransitNode->CreateConnections(A, B);
		}
		else
		{
			TransitNode->CreateConnections(B, A);
		}
	}

	return true;
}

FLinearColor UBXTLGraphSchema::GetPinTypeColor(const FEdGraphPinType& PinType) const
{
	return FColor::White;
}

void UBXTLGraphSchema::BreakSinglePinLink(UEdGraphPin* SourcePin, UEdGraphPin* TargetPin) const
{
	Super::BreakSinglePinLink(SourcePin, TargetPin);

	if (SourcePin)
	{
		if (UBXTLGraphTransitionNode* TransitNode = Cast<UBXTLGraphTransitionNode>(SourcePin->GetOwningNode()))
		{
			TransitNode->DestroyNode();
		}
	}

	if (TargetPin)
	{
		if (UBXTLGraphTransitionNode* TransitNode = Cast<UBXTLGraphTransitionNode>(TargetPin->GetOwningNode()))
		{
			TransitNode->DestroyNode();
		}
	}
}

#undef LOCTEXT_NAMESPACE
