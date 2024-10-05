#pragma once

#include "CoreMinimal.h" 



// 比较符号
UENUM(BlueprintType)
enum class EBXCompareSign : uint8
{
	CS_Less               = 0               UMETA(DisplayName = "小于"),
	CS_LessOrEqual                          UMETA(DisplayName = "小于等于"),
	CS_Equal                                UMETA(DisplayName = "等于"),
	CS_GreaterOrEqual                       UMETA(DisplayName = "大于等于"),
	CS_Greater                              UMETA(DisplayName = "大于"),

	CS_TMax                                 UMETA(Hidden)
};



// 伤害类型
UENUM(BlueprintType)
enum class EBXDamageType : uint8
{
	DT_Physics               = 0               UMETA(DisplayName = "物理伤害"),
	DT_Magic                                   UMETA(DisplayName = "魔法伤害"),
	DT_Purely                                  UMETA(DisplayName = "纯粹伤害"),

	DT_TMax                                    UMETA(Hidden)
};



// 角色受击硬直状态枚举
UENUM(BlueprintType, Meta = (Bitflags))
enum class EBXStaggerState : uint8
{
	SS_Normal               = 0               UMETA(DisplayName = "无硬直"),
	SS_Repel                                  UMETA(DisplayName = "击退"),
	SS_LieDown                                UMETA(DisplayName = "躺倒"),
	SS_Crawl                                  UMETA(DisplayName = "趴下"),
	SS_Rise                                   UMETA(DisplayName = "上浮"),
	SS_Fall                                   UMETA(DisplayName = "坠落"),
	SS_Float                                  UMETA(DisplayName = "漂浮"),
	SS_Execution                              UMETA(DisplayName = "处决"),

	SS_008                                    UMETA(Hidden),
	SS_009                                    UMETA(Hidden),
	SS_010                                    UMETA(Hidden),
	SS_011                                    UMETA(Hidden),
	SS_012                                    UMETA(Hidden),
	SS_013                                    UMETA(Hidden),
	SS_014                                    UMETA(Hidden),
	SS_015                                    UMETA(Hidden),

	SS_TMax                                   UMETA(Hidden)
};
ENUM_CLASS_FLAGS(EBXStaggerState);



// 攻击类型
UENUM(BlueprintType, Meta = (Bitflags))
enum class EBXAttackType : uint8
{
	AT_Light               = 0               UMETA(DisplayName = "轻攻击"),
	AT_Repel                                 UMETA(DisplayName = "击退"),
	AT_HitDown                               UMETA(DisplayName = "击倒"),
	AT_BlowFly                               UMETA(DisplayName = "击飞"),
	AT_LiftUp                                UMETA(DisplayName = "挑空"),
	AT_BlowDown                              UMETA(DisplayName = "击落"),
	AT_Drag                                  UMETA(DisplayName = "拖拽"),

	AT_TMax                                  UMETA(Hidden)
};
ENUM_CLASS_FLAGS(EBXAttackType);



// 攻击力度
UENUM(BlueprintType, Meta = (Bitflags))
enum class EBXAttackForce : uint8
{
	AF_0     = 0		UMETA(DisplayName = "力度0"),
	AF_1 				UMETA(DisplayName = "力度1"),
	AF_2 				UMETA(DisplayName = "力度2"),
	AF_3				UMETA(DisplayName = "力度3"),
	AF_4				UMETA(DisplayName = "力度4"),
	AF_5				UMETA(DisplayName = "力度5"),

	AF_TMax				UMETA(Hidden)
};
ENUM_CLASS_FLAGS(EBXAttackForce);



// 攻击者相对于受击者的方向
UENUM(BlueprintType, Meta = (Bitflags))
enum class EBXAttackerLocationType : uint8
{
	ALT_Front    = 0		UMETA(DisplayName = "前方"),
	ALT_Back 				UMETA(DisplayName = "后方"),
	ALT_Left				UMETA(DisplayName = "左方"),
	ALT_Right				UMETA(DisplayName = "右方"),

	ALT_TMax
};
ENUM_CLASS_FLAGS(EBXAttackerLocationType);



// 形状类型
UENUM(BlueprintType)
enum class EBXShapeType : uint8
{
	ST_Sphere               = 0               UMETA(DisplayName = "球体"),
	ST_Capsule                                UMETA(DisplayName = "胶囊体"),
	ST_Box                                    UMETA(DisplayName = "长方体"),

	ST_TMax                                   UMETA(Hidden)
};



// 动态材质参数类型
UENUM(BlueprintType)
enum class EBXDynamicMaterialParamType : uint8
{
	DMPT_Scalar = 0               UMETA(DisplayName = "标量"),
	DMPT_Vector                   UMETA(DisplayName = "向量"),
	DMPT_Texture                  UMETA(DisplayName = "纹理"),
	DMPT_LinearColor              UMETA(DisplayName = "颜色"),

	DMPT_TMax                     UMETA(Hidden)
};



// 目标锁定类型
UENUM(BlueprintType)
enum class EBXLockTargetType : uint8
{
	TT_Skill                = 0               UMETA(DisplayName = "技能目标"),

	TT_ExtraOne                               UMETA(DisplayName = "扩展目标一"),
	TT_ExtraTwo                               UMETA(DisplayName = "扩展目标二"),
	TT_ExtraThree                             UMETA(DisplayName = "扩展目标三"),
	TT_ExtraFour                              UMETA(DisplayName = "扩展目标四"),

	TT_TMax                                   UMETA(DisplayName = "无效值"),
};



// 身体部位
UENUM(BlueprintType)
enum class EBXBodyPartType : uint8
{
	BPT_Default              = 0               UMETA(DisplayName = "默认"),
	BPT_Head                                   UMETA(DisplayName = "头部"),
	BPT_UpperBody                              UMETA(DisplayName = "上半身"),
	BPT_LowerBody                              UMETA(DisplayName = "下半身"),
	BPT_Tail                                   UMETA(DisplayName = "尾巴"),
	BPT_Limbs1                                 UMETA(DisplayName = "肢体1"),
	BPT_Limbs2                                 UMETA(DisplayName = "肢体2"),
	BPT_Limbs3                                 UMETA(DisplayName = "肢体3"),
	BPT_Limbs4                                 UMETA(DisplayName = "肢体4"),
	BPT_Limbs5                                 UMETA(DisplayName = "肢体5"),
	BPT_Limbs6                                 UMETA(DisplayName = "肢体6"),
	BPT_Limbs7                                 UMETA(DisplayName = "肢体7"),
	BPT_Limbs8                                 UMETA(DisplayName = "肢体8"),

	BPT_TMax                                   UMETA(DisplayName = "无效值"),
};
