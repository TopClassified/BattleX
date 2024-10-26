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
