#pragma once

#include "CoreMinimal.h"

#include "BXCondition.h"
#include "BXConditionEnums.h"

#include "BXTaskCondition.generated.h"



// Task条件参数结构体（按具体条件需求后续补充字段）
USTRUCT(BlueprintType)
struct BATTLEX_API FBXTaskConditionParameter
{
	GENERATED_USTRUCT_BODY()

public:
	// 预留：Task执行上下文字段按需补充
};






// Task系列条件基类（抽象，不可直接实例化）
UCLASS(Abstract, Blueprintable, EditInlineNew)
class BATTLEX_API UBXTaskCondition : public UBXCondition
{
	GENERATED_BODY()
};






// Task组合条件（支持AND/OR嵌套与NOT取反）
UCLASS(Blueprintable, EditInlineNew)
class BATTLEX_API UBXTaskConditionComposite : public UBXTaskCondition
{
	GENERATED_BODY()

public:
	// 逻辑运算类型
	UPROPERTY(EditAnywhere)
	EBXLogicOperator Logic = EBXLogicOperator::And;

	// 子条件列表（Instanced内联编辑，限定Task系列）
	UPROPERTY(EditAnywhere, Instanced)
	TArray<UBXTaskCondition*> Children;
};
