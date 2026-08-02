#pragma once

#include "CoreMinimal.h"

#include "BXConditionEnums.generated.h"



// 条件逻辑运算类型
UENUM(BlueprintType)
enum class EBXLogicOperator : uint8
{
	// 所有子条件都满足时为真
	And UMETA(DisplayName = "AND (All)"),

	// 任一子条件满足时为真
	Or UMETA(DisplayName = "OR (Any)"),
};
