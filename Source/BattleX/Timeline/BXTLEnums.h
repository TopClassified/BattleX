#pragma once

#include "UObject/ObjectMacros.h"

#include "BXTLEnums.generated.h" 



// 结束类型
UENUM(BlueprintType)
enum class EBXTLFinishReason : uint8
{
	FR_EndOfLife               = 0               UMETA(DisplayName = "寿终正寝"),
	FR_Interrupt                                 UMETA(DisplayName = "被中断"),
	FR_PredictionFailure                         UMETA(DisplayName = "预测失败"),

	FR_TMax                                      UMETA(Hidden)
};



// 目标选择类型
UENUM(BlueprintType, Meta = (Bitflags))
enum class EBXTLTargetSelection : uint8
{
	ST_Self                = 0               UMETA(DisplayName = "当前执行目标"),
	ST_Owner                                 UMETA(DisplayName = "拥有者"),
	ST_Instigator                            UMETA(DisplayName = "始作俑者"),
	ST_Trigger                               UMETA(DisplayName = "触发者"),
	ST_Target                                UMETA(DisplayName = "锁定目标"),

	ST_TMax                                  UMETA(DisplayName = "无效选项")
};
ENUM_CLASS_FLAGS(EBXTLTargetSelection);



// 额外时间轴类型
UENUM(BlueprintType)
enum class EBXTLExtraType : uint8
{
	ET_Anticipation            = 0               UMETA(DisplayName = "前摇"),
	ET_Recovery                                  UMETA(DisplayName = "后摇"),
	ET_ComboWindow1                              UMETA(DisplayName = "连段窗口1"),
	ET_ComboWindow2                              UMETA(DisplayName = "连段窗口2"),
	ET_ComboWindow3                              UMETA(DisplayName = "连段窗口3"),
	ET_ComboWindow4                              UMETA(DisplayName = "连段窗口4"),
	ET_ComboWindow5                              UMETA(DisplayName = "连段窗口5"),
	ET_ComboWindow6                              UMETA(DisplayName = "连段窗口6"),
	ET_ComboWindow7                              UMETA(DisplayName = "连段窗口7"),
	ET_ComboWindow8                              UMETA(DisplayName = "连段窗口8"),

	ET_TMax                                      UMETA(Hidden)
};
