#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"

#include "BXStateEnums.generated.h"

// 状态系统日志(模块级声明,Define在BXStateComponent.cpp)
DECLARE_LOG_CATEGORY_EXTERN(BXSTATE, Log, All);



// 状态结束原因
UENUM(BlueprintType)
enum class EBXStateEndReason : uint8
{
	SER_Expired          = 0            UMETA(DisplayName = "时长耗尽"),
	SER_Manual                          UMETA(DisplayName = "手动退出"),
	SER_Transition                      UMETA(DisplayName = "状态机转移"),
	SER_External                        UMETA(DisplayName = "被外部进入顶掉"),
	SER_PredictRollback                 UMETA(DisplayName = "预测回滚"),
	SER_Cleared                         UMETA(DisplayName = "组件清空"),

	SER_TMax                            UMETA(Hidden)
};



// 状态转移边的评估时机
UENUM(BlueprintType)
enum class EBXTransitionEvaluate : uint8
{
	TE_OnExpired         = 0            UMETA(DisplayName = "驻留时长耗尽时"),
	TE_OnTick                           UMETA(DisplayName = "每帧评估"),

	TE_TMax                             UMETA(Hidden)
};



// 状态表现通道
UENUM(BlueprintType)
enum class EBXPresentationType : uint8
{
	PT_None              = 0            UMETA(DisplayName = "无表现"),
	PT_Skill                            UMETA(DisplayName = "技能(自带同步)"),
	PT_Timeline                         UMETA(DisplayName = "时间轴(不同步)"),
	PT_Animation                        UMETA(DisplayName = "纯动画Montage"),

	PT_TMax                             UMETA(Hidden)
};
