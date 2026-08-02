#pragma once

#include "CoreMinimal.h"

#include "BXCondition.h"
#include "BXConditionEnums.h"

#include "BXDecisionTreeCondition.generated.h"



// 决策树系列条件基类（抽象，不可直接实例化）
UCLASS(Abstract, Blueprintable, EditInlineNew)
class BATTLEX_API UBXDecisionTreeCondition : public UBXCondition
{
	GENERATED_BODY()
};



// 决策树组合条件（支持AND/OR嵌套与NOT取反）
UCLASS(Blueprintable, EditInlineNew)
class BATTLEX_API UBXDecisionTreeConditionComposite : public UBXDecisionTreeCondition
{
	GENERATED_BODY()

public:
	// 逻辑运算类型
	UPROPERTY(EditAnywhere)
	EBXLogicOperator Logic = EBXLogicOperator::And;

	// 子条件列表（Instanced内联编辑，限定决策树系列）
	UPROPERTY(EditAnywhere, Instanced)
	TArray<UBXDecisionTreeCondition*> Children;
};
