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





// 行为关系类型(矩阵单元格=禁用/中断两开关组合,空=天然共存不落数据;
// 对角线自关系可配:自禁用挡同Tag重入,自中断=新实例顶掉旧实例)
UENUM(BlueprintType)
enum class EBXBehaviorRelation : uint8
{
	BR_None             = 0             UMETA(DisplayName = "空(天然共存)"),
	BR_Expel                            UMETA(DisplayName = "中断"),
	BR_Forbid                           UMETA(DisplayName = "禁用"),
	BR_ForbidExpel                      UMETA(DisplayName = "禁用并中断"),

	BR_TMax                             UMETA(Hidden)
};
