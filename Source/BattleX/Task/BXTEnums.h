#pragma once

#include "CoreMinimal.h"



// 目标类型
UENUM(BlueprintType, Meta = (Bitflags))
enum class EBXTTargetType : uint8
{
	T_Owner               = 0               UMETA(DisplayName = "拥有者"),
	T_Instigator                            UMETA(DisplayName = "始作俑者"),
	T_Trigger                               UMETA(DisplayName = "触发者"),
	T_LockTargets                           UMETA(DisplayName = "锁定的目标"),
	T_CollisionResults                      UMETA(DisplayName = "碰撞检测结果"),

	T_TMax                                  UMETA(DisplayName = "无效选项")
};
ENUM_CLASS_FLAGS(EBXTTargetType);



// 生命周期
UENUM(BlueprintType)
enum class EBXTLifeType : uint8
{
	L_Instant               = 0               UMETA(DisplayName = "立即结束"),
	L_Duration                                UMETA(DisplayName = "固定时长"),
	L_Timeline                                UMETA(DisplayName = "时间轴控制"),
	L_DurationTimeline                        UMETA(DisplayName = "固定时长且时间轴控制"),
	
	L_TMax                                    UMETA(Hidden)
};



// 网络类型
UENUM(BlueprintType, Meta = (Bitflags))
enum class EBXTNetType : uint8
{
	N_Authority          = 0               UMETA(DisplayName = "权威端"),
	N_Autonomous                           UMETA(DisplayName = "自主端"),
	N_Simulated                            UMETA(DisplayName = "模拟端"),

	N_TMax                                 UMETA(Hidden),
};
ENUM_CLASS_FLAGS(EBXTNetType);



// 触发类型
UENUM(BlueprintType, Meta = (Bitflags))
enum class EBXTTriggerType : uint8
{
	T_Timeline                 = 0               UMETA(DisplayName = "时间轴触发"),
	T_SectionEnd                                 UMETA(DisplayName = "时间轴结束时触发"),
	T_SectionInterrupt                           UMETA(DisplayName = "时间轴中断时触发"),

	T_TMax                                       UMETA(Hidden),
};
ENUM_CLASS_FLAGS(EBXTTriggerType);



// 坐标系类型
UENUM(BlueprintType)
enum class EBXTCoordinateType : uint8
{
	C_Target               = 0               UMETA(DisplayName = "执行者的坐标系"),
	C_Owner                                  UMETA(DisplayName = "拥有者的坐标系"),
	C_Instigator                             UMETA(DisplayName = "始作俑者的坐标系"),
	C_Trigger                                UMETA(DisplayName = "触发者的坐标系"),
	C_LockTarget                             UMETA(DisplayName = "锁定目标的坐标系"),
	C_LockPart                               UMETA(DisplayName = "锁定部位的坐标系"),
	C_Special                                UMETA(DisplayName = "特定对象的坐标系"),
	C_World                                  UMETA(DisplayName = "世界坐标系"),

	C_TMax                                   UMETA(DisplayName = "无效值")
};



// 连线类型
UENUM(BlueprintType)
enum class EBXTConnectionType : uint8
{
	C_FromOriginIn2D               = 0               UMETA(DisplayName = "从原点出发指向X轴坐标系原点(2D)"),
	C_FromOriginIn3D                                 UMETA(DisplayName = "从原点出发指向X轴坐标系原点(3D)"),
	C_FromXAxisIn2D                                  UMETA(DisplayName = "从X轴坐标系出发指向原点(2D)"),
	C_FromXAxisIn3D                                  UMETA(DisplayName = "从X轴坐标系出发指向原点(3D)"),

	C_TMax                                           UMETA(DisplayName = "无效值")
};



// 任务处理器执行函数类型
UENUM(BlueprintType, Meta = (Bitflags))
enum class EBXTProcessorFunction : uint8
{
	PF_NativeStart          = 0              UMETA(DisplayName = "C++开始"),
	PF_BPStart                               UMETA(DisplayName = "蓝图开始"),
	PF_NativeUpdate                          UMETA(DisplayName = "C++更新"),
	PF_BPUpdate                              UMETA(DisplayName = "蓝图更新"),
	PF_NativeEnd                             UMETA(DisplayName = "C++结束"),
	PF_BPEnd                                 UMETA(DisplayName = "蓝图结束"),

	PF_TMax                                  UMETA(Hidden)
};
ENUM_CLASS_FLAGS(EBXTProcessorFunction);
