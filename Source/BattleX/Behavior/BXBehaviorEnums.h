#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"

#include "BXBehaviorEnums.generated.h"

// 行为系统日志(模块级声明,Define在BXBehaviorComponent.cpp)
DECLARE_LOG_CATEGORY_EXTERN(BXBEHAVIOR, Log, All);



// 行为结束原因
UENUM(BlueprintType)
enum class EBXBehaviorEndReason : uint8
{
	BER_Manual           = 0             UMETA(DisplayName = "手动停止"),
	BER_Expelled                         UMETA(DisplayName = "被挤出"),
	BER_Interrupted                      UMETA(DisplayName = "被中断(一次性Stop)"),
	BER_PredictRollback                  UMETA(DisplayName = "预测回滚"),
	BER_Cleared                          UMETA(DisplayName = "组件清空"),

	BER_TMax                             UMETA(Hidden)
};





// 行为关系类型(矩阵单元格=两开关组合:禁止/接管,空=天然共存不落数据)
UENUM(BlueprintType)
enum class EBXBehaviorRelation : uint8
{
	BR_None             = 0             UMETA(DisplayName = "空(天然共存)"),
	BR_Expel                            UMETA(DisplayName = "接管(挤出)"),
	BR_Forbid                           UMETA(DisplayName = "禁止(挡入)"),
	BR_ForbidExpel                      UMETA(DisplayName = "禁止+接管"),

	BR_TMax                             UMETA(Hidden)
};
