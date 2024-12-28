#include "BXDecisionTreeActuator.h"
#include "BXDecisionTreeNode.h"
#include "BXDecisionTreeEdge.h"
#include "BXDecisionTreeTemplate.h"



DEFINE_FUNCTION(UBXDecisionTreeActuator::execGetBestNode)
{
	Stack.MostRecentProperty = nullptr;
		
	// 更新蓝图虚拟机栈顶指针
	Stack.StepCompiledIn<FProperty>(nullptr);
	// 获取第一个数据的参数的地址
	UObject* InContext = reinterpret_cast<UObject*>(Stack.MostRecentPropertyAddress);

	// 更新蓝图虚拟机栈顶指针
	Stack.StepCompiledIn<FProperty>(nullptr);
	// 获取第二个数据的参数的地址
	UBXDecisionTreeTemplate* InTemplate = reinterpret_cast<UBXDecisionTreeTemplate*>(Stack.MostRecentPropertyAddress);
		
	// 更新蓝图虚拟机栈顶指针
	Stack.StepCompiledIn<FProperty>(nullptr);
	// 获取第三个无类型参数的内存地址
	uint8* ParameterPointer = Stack.MostRecentPropertyAddress;
	// 获取第三个参数的反射信息
	FStructProperty* ParameterProperty = CastField<FStructProperty>(Stack.MostRecentProperty);

	// 停止对蓝图栈的使用
	P_FINISH;

	if (!IsValid(InTemplate) || !InTemplate->IsValidLowLevelFast() || !ParameterPointer || !ParameterProperty)
	{
		return;
	}

	UBXDecisionTreeActuator* Actuator = P_THIS_CAST(UBXDecisionTreeActuator);
	if (!IsValid(Actuator) || !Actuator->IsValidLowLevel())
	{
		return;
	}

	UBXDecisionTreeNode* CheckResult = nullptr;
		
	P_NATIVE_BEGIN;
	CheckResult = Actuator->InternalGetBestNode(InContext, InTemplate, ParameterProperty->Struct, ParameterPointer);
	P_NATIVE_END;

	*(UBXDecisionTreeNode**)RESULT_PARAM = CheckResult;
}

UBXDecisionTreeNode* UBXDecisionTreeActuator::InternalGetBestNode(UObject* InWorldContext, UBXDecisionTreeTemplate* InTemplate, UScriptStruct* InParameterType, void* InParameterAddress)
{
	if (!IsValid(InTemplate) || !IsValid(InParameterType))
	{
		return nullptr;
	}

	TArray<UBXDecisionTreeNode*> RootList;
	RootList.Append(InTemplate->RootNodes);

	UBXDecisionTreeNode* ResultNode = nullptr;
	for (int32 i = 0; i < RootList.Num(); ++i)
	{
		if (!IsValid(RootList[i]))
		{
			continue;
		}
		
		// 遍历该根节点的决策树
		ResultNode = TravelDecisionTree(InWorldContext, RootList[i], InParameterType, InParameterAddress);
		if (ResultNode)
		{
			return ResultNode;
		}
	}

	return nullptr;
}

UBXDecisionTreeNode* UBXDecisionTreeActuator::TravelDecisionTree(UObject* InWorldContext, UBXDecisionTreeNode* StartNode, UScriptStruct* InParameterType, void* InParameterAddress)
{
	if (!IsValid(StartNode) || !StartNode->CheckCondition(InWorldContext, InParameterType, InParameterAddress))
	{
		return nullptr;
	}

	UBXDecisionTreeTemplate* Template = Cast<UBXDecisionTreeTemplate>(StartNode->GetOuter());
	if (!IsValid(Template))
	{
		return nullptr;
	}

	TArray<UBXDecisionTreeEdge*> EdgeList;
	EdgeList.Append(Template->TreeEdges);

	UBXDecisionTreeNode* Result = StartNode;
	// 遍历从该节点出发的边
	for (int32 i = 0; i < StartNode->OutEdges.Num(); ++i)
	{
		if (EdgeList.IsValidIndex(StartNode->OutEdges[i]))
		{
			UBXDecisionTreeEdge* CurEdge = EdgeList[StartNode->OutEdges[i]];
			if (CurEdge && CurEdge->CheckCondition(InWorldContext, InParameterType, InParameterAddress))
			{
				Result = TravelDecisionTree(InWorldContext, CurEdge->EndNode, InParameterType, InParameterAddress);
				if (Result)
				{
					break;
				}
			}
		}
	}

	return Result;
}
