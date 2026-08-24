#include "BXProjectileAsset.h"



#pragma region Bake
void UBXProjectileAsset::BuildBakedConfig(FBXProjectileBakedConfig& OutBaked, int32 InSampleCount, int32 InArcSampleCount, float InTargetRefreshDistance) const
{
	OutBaked.MaxLifetime = FMath::Max(MaxLifetime, 0.1f);
	OutBaked.ResidualTime = FMath::Max(ResidualTime, 0.0f);
	OutBaked.Gravity = Gravity;
	OutBaked.BulletShape = BulletShape;
	OutBaked.BulletRadius = FMath::Max(BulletRadius, 0.1f);
	OutBaked.BulletBoxExtent = BulletBoxExtent.ComponentMax(FVector::ZeroVector);
	OutBaked.MaxPenetrationCount = FMath::Max(MaxPenetrationCount, 1);
	OutBaked.HitCooldown = FMath::Max(HitCooldown, 0.0f);
	OutBaked.bHoming = bHoming;
	OutBaked.TargetInvalidPolicy = TargetInvalidPolicy;
	OutBaked.bUseBezier = bUseBezier;

	// 中间控制点钳制(求值器栈缓冲上限8=首尾+6中间,超出丢弃尾部并告警防静默轨迹错误)
	OutBaked.BezierControlPoints = BezierControlPoints;
	if (OutBaked.BezierControlPoints.Num() > 6)
	{
		UE_LOG(LogTemp, Warning, TEXT("BuildBakedConfig: 资产%s贝塞尔中间控制点超过6个,超出部分被丢弃"), *GetName());
		OutBaked.BezierControlPoints.SetNum(6);
	}
	OutBaked.TargetRefreshDistance = FMath::Max(InTargetRefreshDistance, 1.0f);
	OutBaked.ArcSampleCount = FMath::Max(InArcSampleCount, 8);
	OutBaked.bMathCheck = bMathCheck;
	OutBaked.bPhysicsCheck = bPhysicsCheck;

	// 曲线烘焙为等间隔LUT:运行时O(1)查表,双端因Asset一致而LUT一致
	const int32 SampleCount = FMath::Max(InSampleCount, 2);
	auto BakeCurve = [&OutBaked, SampleCount](const FRuntimeFloatCurve& InSourceCurve, FBXProjectileBakedCurve& OutCurve)
	{
		OutCurve.TimeRange = OutBaked.MaxLifetime;
		OutCurve.Samples.Reset(SampleCount);
		for (int32 Index = 0; Index < SampleCount; ++Index)
		{
			const float Alpha = static_cast<float>(Index) / static_cast<float>(SampleCount - 1);
			OutCurve.Samples.Add(InSourceCurve.GetFloatValue(Alpha * OutBaked.MaxLifetime));
		}
	};

	BakeCurve(SpeedCurve, OutBaked.SpeedCurve);
	BakeCurve(TurnRateCurve, OutBaked.TurnRateCurve);
}

#pragma endregion Bake
