#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Engine/DataAsset.h"
#include "Curves/CurveFloat.h"
#include "GameplayTagContainer.h"

#include "BXCollision.h"
#include "BXProjectileStructs.h"

#include "BXProjectileAsset.generated.h"



class UNiagaraSystem;
class USoundBase;



// 子弹资源(数据与逻辑分离:本类只存配置,运行数据由UBXProjectileManager维护)
UCLASS(BlueprintType)
class BATTLEX_API UBXProjectileAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

#pragma region Important
public:
	// 子弹种类标签(Manager按此查找,约定命名空间BXProjectile.*)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Important")
	FGameplayTag ProjectileType;

	// 最大生命周期(秒)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Important", Meta = (ClampMin = "0.1"))
	float MaxLifetime = 3.0f;

	// 命中或寿命尽后的残留时长(秒,表现层淡出用)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Important", Meta = (ClampMin = "0.0"))
	float ResidualTime = 1.0f;

#pragma endregion Important



#pragma region Movement
public:
	// 是否跟踪目标(始终跟随直到命中或目标失效或生命结束)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	bool bHoming = false;

	// 跟踪目标失效策略
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement", Meta = (EditCondition = "bHoming"))
	EBXProjectileTargetInvalidPolicy TargetInvalidPolicy = EBXProjectileTargetInvalidPolicy::TIP_KeepDirection;

	// 速度曲线(定义域为生命时长,值域为速度cm/s)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	FRuntimeFloatCurve SpeedCurve;

	// 转向角速度曲线(定义域为生命时长,值域为deg/s)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement", Meta = (EditCondition = "bHoming"))
	FRuntimeFloatCurve TurnRateCurve;

	// 重力向量(cm/s²,以速度增量方式积分)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	FVector Gravity = FVector::ZeroVector;

	// 是否贝塞尔移动(优先级高于跟踪,忽略重力与转向)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	bool bUseBezier = false;

	// 贝塞尔中间控制点(2~4个,以发射点指向目标点线段为基准的百分比+偏移)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement", Meta = (EditCondition = "bUseBezier"))
	TArray<FBXBezierControlPoint> BezierControlPoints;

#pragma endregion Movement



#pragma region Collision
public:
	// 是否执行数学判定(注册目标重叠检测,与运动积分同并行阶段计算)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Collision")
	bool bMathCheck = true;

	// 是否执行物理检测(场景Sweep,与数学判定独立开关可任意组合)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Collision")
	bool bPhysicsCheck = false;

	// 物理检测碰撞策略(ObjectTypes+筛选器,数学判定同样使用其筛选器做GameThread过滤)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Collision")
	FBXCStrategy PhysicsStrategy;

	// 子弹碰撞形状(数学判定按形状膨胀线段,物理检测按形状生成Sweep几何体)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Collision")
	EBXProjectileShape BulletShape = EBXProjectileShape::BS_Point;

	// 子弹碰撞半径(cm,球体形状)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Collision", Meta = (EditCondition = "BulletShape == EBXProjectileShape::BS_Sphere", ClampMin = "0.1"))
	float BulletRadius = 10.0f;

	// 子弹长方体半尺寸(cm,局部空间X沿飞行方向,长方体形状)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Collision", Meta = (EditCondition = "BulletShape == EBXProjectileShape::BS_Box", ClampMin = "0.0"))
	FVector BulletBoxExtent = FVector(10.0f, 10.0f, 10.0f);

	// 最大穿透次数(命中后继续飞行,1为命中即毁)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Collision", Meta = (ClampMin = "1"))
	int32 MaxPenetrationCount = 1;

#pragma endregion Collision



#pragma region Presentation
public:
	// 创建特效
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Presentation")
	TSoftObjectPtr<UNiagaraSystem> FireSystem;

	// 飞行特效(需包含UserArray:ProjectilePositions/ProjectileVelocities/ProjectileAges)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Presentation")
	TSoftObjectPtr<UNiagaraSystem> FlightSystem;

	// 命中特效
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Presentation")
	TSoftObjectPtr<UNiagaraSystem> HitSystem;

	// 创建音效
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Presentation")
	TSoftObjectPtr<USoundBase> FireSound;

	// 飞行音效(按桶聚合,音量随数量缩放)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Presentation")
	TSoftObjectPtr<USoundBase> FlightSound;

	// 命中音效
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Presentation")
	TSoftObjectPtr<USoundBase> HitSound;

#pragma endregion Presentation



#pragma region Bake
public:
	// 构建烘焙配置(速度/转向曲线等间隔LUT+参数副本,桶创建时调用一次)
	void BuildBakedConfig(FBXProjectileBakedConfig& OutBaked, int32 InSampleCount, int32 InArcSampleCount, float InTargetRefreshDistance) const;

#pragma endregion Bake
};
