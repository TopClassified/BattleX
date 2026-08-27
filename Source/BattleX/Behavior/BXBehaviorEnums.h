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
	BER_Suspended                        UMETA(DisplayName = "被状态挂起"),
	BER_Resumed                          UMETA(DisplayName = "挂起恢复(仅Enter事件携带)"),
	BER_PredictRollback                  UMETA(DisplayName = "预测回滚"),
	BER_Cleared                          UMETA(DisplayName = "组件清空"),

	BER_TMax                             UMETA(Hidden)
};



// 行为关系类型(行为矩阵单元格三态)
UENUM(BlueprintType)
enum class EBXBehaviorRelation : uint8
{
	BR_None             = 0             UMETA(DisplayName = "并存"),
	BR_Expel                            UMETA(DisplayName = "挤出"),
	BR_Reject                           UMETA(DisplayName = "拒绝"),

	BR_TMax                             UMETA(Hidden)
};
