#pragma once

#include "CoreMinimal.h"

#include "BXDecisionTreeNode.h"

#include "BXDecisionTreeActuator.generated.h"



UCLASS(Blueprintable, BlueprintType)
class BATTLEX_API UBXDecisionTreeActuator : public UObject
{
	GENERATED_BODY()

public:
	template<typename T>
	UBXDecisionTreeNode* GetBestNode(UObject* InWorldContext, UBXDecisionTreeTemplate* InTemplate, const T& InParameter)
	{
		return InternalGetBestNode(InWorldContext, InTemplate, T::StaticStruct(), &InParameter);
	}

	UFUNCTION(BlueprintCallable, CustomThunk, meta = (CustomStructureParam = "InParameter"))
	UBXDecisionTreeNode* GetBestNode(UObject* InWorldContext, UBXDecisionTreeTemplate* InTemplate, int32 InParameter);
	DECLARE_FUNCTION(execGetBestNode);

protected:
	// 获取最佳节点
	virtual UBXDecisionTreeNode* InternalGetBestNode(UObject* InWorldContext, UBXDecisionTreeTemplate* InTemplate, UScriptStruct* InParameterType, void* InParameterAddress);
	
	// 决策树遍历接口
	virtual UBXDecisionTreeNode* TravelDecisionTree(UObject* InWorldContext, UBXDecisionTreeNode* StartNode, UScriptStruct* InParameterType, void* InParameterAddress);

};

