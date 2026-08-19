#include "BXDecisionTreeActuator.h"
#include "BXDecisionTreeNode.h"
#include "BXDecisionTreeEdge.h"
#include "BXDecisionTreeTemplate.h"



DEFINE_FUNCTION(UBXDecisionTreeActuator::execGetBestNode)
{
	Stack.MostRecentProperty = nullptr;

	// 更新蓝图虚拟机栈顶指针
	Stack.StepCompiledIn<FProperty>(nullptr);
	// 获取第一个数据的参数的地址(参数槽内存放指针值,须解引用取对象,原实现把槽地址当对象用)
	UObject* InContext = *(UObject**)Stack.MostRecentPropertyAddress;

	// 更新蓝图虚拟机栈顶指针
	Stack.StepCompiledIn<FProperty>(nullptr);
	// 获取第二个数据的参数的地址(同上须解引用)
	UBXDecisionTreeTemplate* InTemplate = *(UBXDecisionTreeTemplate**)Stack.MostRecentPropertyAddress;

	// 更新蓝图虚拟机栈顶指针
	Stack.StepCompiledIn<FProperty>(nullptr);
	// 获取第三个无类型参数的内存地址
	uint8* ParameterPointer = Stack.MostRecentPropertyAddress;
	// 获取第三个参数的反射信息
	FStructProperty* ParameterProperty = CastField<FStructProperty>(Stack.MostRecentProperty);

	// 停止对蓝图栈的使用
	P_FINISH;

	// 所有提前返回路径必须写入返回值槽(否则蓝图拿到未初始化栈内存当节点指针使用即崩溃)
	if (!IsValid(InTemplate) || !InTemplate->IsValidLowLevelFast() || !ParameterPointer || !ParameterProperty)
	{
		*(UBXDecisionTreeNode**)RESULT_PARAM = nullptr;
		return;
	}

	UBXDecisionTreeActuator* Actuator = P_THIS_CAST(UBXDecisionTreeActuator);
	if (!IsValid(Actuator) || !Actuator->IsValidLowLevel())
	{
		*(UBXDecisionTreeNode**)RESULT_PARAM = nullptr;
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

	// 访问集防环(CombatTree的bAllowCycle=true允许环形连招链,无访问集的递归遍历会栈溢出)
	TSet<UBXDecisionTreeNode*> VisitedNodes;

	UBXDecisionTreeNode* ResultNode = nullptr;
	for (int32 i = 0; i < RootList.Num(); ++i)
	{
		if (!IsValid(RootList[i]))
		{
			continue;
		}

		// 遍历该根节点的决策树
		ResultNode = TravelDecisionTree(InWorldContext, RootList[i], InParameterType, InParameterAddress, VisitedNodes);
		if (ResultNode)
		{
			return ResultNode;
		}
	}

	return nullptr;
}

UBXDecisionTreeNode* UBXDecisionTreeActuator::TravelDecisionTree(UObject* InWorldContext, UBXDecisionTreeNode* StartNode, UScriptStruct* InParameterType, void* InParameterAddress, TSet<UBXDecisionTreeNode*>& InOutVisitedNodes)
{
	if (!IsValid(StartNode) || !StartNode->CheckCondition(InWorldContext, InParameterType, InParameterAddress))
	{
		return nullptr;
	}

	// 已访问节点直接返回(环防护)
	if (InOutVisitedNodes.Contains(StartNode))
	{
		return nullptr;
	}
	InOutVisitedNodes.Add(StartNode);

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
				// 子树失败时保持Result=StartNode(本节点条件已通过,是有效落点);
				// 原实现直接覆盖Result,边通过但子节点失败时本节点成功结果被null覆盖丢失
				UBXDecisionTreeNode* SubResult = TravelDecisionTree(InWorldContext, CurEdge->EndNode, InParameterType, InParameterAddress, InOutVisitedNodes);
				if (SubResult)
				{
					Result = SubResult;
					break;
				}
			}
		}
	}

	return Result;
}
