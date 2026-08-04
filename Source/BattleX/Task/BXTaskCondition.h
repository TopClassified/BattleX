#pragma once

#include "CoreMinimal.h"

#include "BXCondition.h"
#include "BXConditionEnums.h"

#include "BXTaskCondition.generated.h"



class AActor;

// Task条件参数结构体基类
USTRUCT(BlueprintType)
struct BATTLEX_API FBXTaskConditionParameter
{
	GENERATED_BODY()

public:
	// 技能拥有者
	UPROPERTY(BlueprintReadWrite)
	AActor* Owner = nullptr;

	// 技能始作俑者
	UPROPERTY(BlueprintReadWrite)
	AActor* Instigator = nullptr;
};

uint32 GetTypeHash(const FBXTaskConditionParameter& InParam);



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
	UPROPERTY(EditAnywhere, Category = "Condition")
	EBXLogicOperator Logic = EBXLogicOperator::And;

	// 子条件列表（Instanced内联编辑，限定Task系列）
	UPROPERTY(EditAnywhere, Instanced, Category = "Condition")
	TArray<UBXTaskCondition*> Children;
};
