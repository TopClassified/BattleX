#include "BXProjectileSolver.h"

#include "BXFunctionLibrary.h"



namespace
{
	// 弧长表采样:参数u处弧长
	float InternalArcAt(const TArray<float>& InArcLength, float InU)
	{
		if (InArcLength.Num() < 2)
		{
			return 0.0f;
		}

		const float ClampedU = FMath::Clamp(InU, 0.0f, 1.0f);
		const float ScaledIndex = ClampedU * (InArcLength.Num() - 1);
		const int32 LowerIndex = FMath::Min(FMath::FloorToInt(ScaledIndex), InArcLength.Num() - 2);
		const float Blend = ScaledIndex - LowerIndex;
		return FMath::Lerp(InArcLength[LowerIndex], InArcLength[LowerIndex + 1], Blend);
	}

	// 弧长表反查:弧长处参数u(二分查找,弧长表单调递增)
	float InternalParamAt(const TArray<float>& InArcLength, float InArcLengthValue)
	{
		if (InArcLength.Num() < 2)
		{
			return 0.0f;
		}

		if (InArcLengthValue <= 0.0f)
		{
			return 0.0f;
		}

		if (InArcLengthValue >= InArcLength.Last())
		{
			return 1.0f;
		}

		int32 LowIndex = 0;
		int32 HighIndex = InArcLength.Num() - 1;
		while (HighIndex - LowIndex > 1)
		{
			const int32 MidIndex = (LowIndex + HighIndex) / 2;
			if (InArcLength[MidIndex] <= InArcLengthValue)
			{
				LowIndex = MidIndex;
			}
			else
			{
				HighIndex = MidIndex;
			}
		}

		const float Span = InArcLength[HighIndex] - InArcLength[LowIndex];
		const float Alpha = Span > KINDA_SMALL_NUMBER ? (InArcLengthValue - InArcLength[LowIndex]) / Span : 0.0f;
		return (LowIndex + Alpha) / static_cast<float>(InArcLength.Num() - 1);
	}
}



#pragma region Bezier
void FBXProjectileSolver::BuildBezierControlPoints(const FVector& InStartLocation, const FVector& InTargetLocation, const FVector& InUpVector, const TArray<FBXBezierControlPoint>& InControlPointConfigs, TArray<FVector>& OutControlPoints)
{
	OutControlPoints.Reset(InControlPointConfigs.Num() + 2);
	OutControlPoints.Add(InStartLocation);

	const FVector Segment = InTargetLocation - InStartLocation;
	FVector AxisX = Segment.GetSafeNormal();
	if (AxisX.IsNearlyZero())
	{
		AxisX = FVector::ForwardVector;
	}

	// Z轴 = UpVector对X轴正交化,Y轴 = Z × X
	FVector AxisZ = InUpVector - AxisX * FVector::DotProduct(InUpVector, AxisX);
	if (AxisZ.IsNearlyZero())
	{
		AxisZ = FVector::UpVector - AxisX * FVector::DotProduct(FVector::UpVector, AxisX);
		if (AxisZ.IsNearlyZero())
		{
			AxisZ = FVector::CrossProduct(AxisX, FVector::RightVector).GetSafeNormal();
		}
		else
		{
			AxisZ = AxisZ.GetSafeNormal();
		}
	}
	else
	{
		AxisZ = AxisZ.GetSafeNormal();
	}

	const FVector AxisY = FVector::CrossProduct(AxisZ, AxisX).GetSafeNormal();

	for (const FBXBezierControlPoint& Config : InControlPointConfigs)
	{
		const FVector Point = InStartLocation + Segment * Config.Percent + AxisX * Config.Offset.X + AxisY * Config.Offset.Y + AxisZ * Config.Offset.Z;
		OutControlPoints.Add(Point);
	}

	OutControlPoints.Add(InTargetLocation);
}

FVector FBXProjectileSolver::EvaluateBezier(const TArray<FVector>& InControlPoints, float InU)
{
	const int32 PointCount = InControlPoints.Num();
	if (PointCount <= 0)
	{
		return FVector::ZeroVector;
	}

	if (PointCount == 1)
	{
		return InControlPoints[0];
	}

	// de Casteljau逐阶降阶(控制点上限8,栈缓冲无堆分配)
	const int32 ActiveCount = FMath::Min(PointCount, 8);
	FVector TempPoints[8];
	for (int32 Index = 0; Index < ActiveCount; ++Index)
	{
		TempPoints[Index] = InControlPoints[Index];
	}

	const float ClampedU = FMath::Clamp(InU, 0.0f, 1.0f);
	int32 Remaining = ActiveCount;
	while (Remaining > 1)
	{
		--Remaining;
		for (int32 Index = 0; Index < Remaining; ++Index)
		{
			TempPoints[Index] = FMath::Lerp(TempPoints[Index], TempPoints[Index + 1], ClampedU);
		}
	}

	return TempPoints[0];
}

FVector FBXProjectileSolver::EvaluateBezierTangent(const TArray<FVector>& InControlPoints, float InU)
{
	if (InControlPoints.Num() < 2)
	{
		return FVector::ForwardVector;
	}

	const float Epsilon = 1.0f / 1024.0f;
	const float LowerU = FMath::Max(InU - Epsilon, 0.0f);
	const float UpperU = FMath::Min(InU + Epsilon, 1.0f);
	const FVector Tangent = EvaluateBezier(InControlPoints, UpperU) - EvaluateBezier(InControlPoints, LowerU);
	if (Tangent.IsNearlyZero())
	{
		// 端点处中心差分退化,改用单侧差分
		const FVector OneSideTangent = EvaluateBezier(InControlPoints, FMath::Min(InU + 0.01f, 1.0f)) - EvaluateBezier(InControlPoints, InU);
		return OneSideTangent.GetSafeNormal();
	}

	return Tangent.GetSafeNormal();
}

float FBXProjectileSolver::BuildArcLengthTable(const TArray<FVector>& InControlPoints, int32 InSampleCount, TArray<float>& OutArcLength)
{
	const int32 SampleCount = FMath::Max(InSampleCount, 2);
	OutArcLength.SetNumZeroed(SampleCount);

	FVector PreviousPoint = EvaluateBezier(InControlPoints, 0.0f);
	float TotalArcLength = 0.0f;
	for (int32 Index = 1; Index < SampleCount; ++Index)
	{
		const FVector CurrentPoint = EvaluateBezier(InControlPoints, static_cast<float>(Index) / static_cast<float>(SampleCount - 1));
		TotalArcLength += FVector::Dist(PreviousPoint, CurrentPoint);
		OutArcLength[Index] = TotalArcLength;
		PreviousPoint = CurrentPoint;
	}

	return TotalArcLength;
}

float FBXProjectileSolver::AdvanceArcLengthParam(const TArray<float>& InArcLength, float InTotalArcLength, float InU, float InDeltaS)
{
	if (InArcLength.Num() < 2 || InTotalArcLength <= KINDA_SMALL_NUMBER)
	{
		// 退化曲线(零长度)按参数比例直推,推进量按DeltaS钳制
		return FMath::Clamp(InU + InDeltaS, 0.0f, 1.0f);
	}

	const float CurrentArc = InternalArcAt(InArcLength, InU);
	const float NewArc = FMath::Clamp(CurrentArc + InDeltaS, 0.0f, InTotalArcLength);
	return InternalParamAt(InArcLength, NewArc);
}

float FBXProjectileSolver::FindClosestBezierParam(const TArray<FVector>& InControlPoints, const FVector& InLocation, float InHintU)
{
	if (InControlPoints.Num() < 2)
	{
		return 0.0f;
	}

	// 全局粗采样
	const int32 CoarseSampleCount = 16;
	float BestU = 0.0f;
	float BestDistanceSquared = TNumericLimits<float>::Max();
	for (int32 Index = 0; Index < CoarseSampleCount; ++Index)
	{
		const float TestU = static_cast<float>(Index) / static_cast<float>(CoarseSampleCount - 1);
		const float DistanceSquared = FVector::DistSquared(EvaluateBezier(InControlPoints, TestU), InLocation);
		if (DistanceSquared < BestDistanceSquared)
		{
			BestDistanceSquared = DistanceSquared;
			BestU = TestU;
		}
	}

	// Hint附近细化(重建场景下Hint通常已是较优起点)
	const float HintDistanceSquared = FVector::DistSquared(EvaluateBezier(InControlPoints, InHintU), InLocation);
	if (HintDistanceSquared < BestDistanceSquared)
	{
		BestDistanceSquared = HintDistanceSquared;
		BestU = InHintU;
	}

	// BestU邻域16步精搜
	const float Window = 1.0f / static_cast<float>(CoarseSampleCount);
	const float LowerBound = FMath::Max(BestU - Window, 0.0f);
	const float UpperBound = FMath::Min(BestU + Window, 1.0f);
	for (int32 Index = 0; Index < CoarseSampleCount; ++Index)
	{
		const float TestU = FMath::Lerp(LowerBound, UpperBound, static_cast<float>(Index) / static_cast<float>(CoarseSampleCount - 1));
		const float DistanceSquared = FVector::DistSquared(EvaluateBezier(InControlPoints, TestU), InLocation);
		if (DistanceSquared < BestDistanceSquared)
		{
			BestDistanceSquared = DistanceSquared;
			BestU = TestU;
		}
	}

	return BestU;
}

void FBXProjectileSolver::InitBezierRuntime(const FBXProjectileBakedConfig& InBaked, const FVector& InStartLocation, const FVector& InTargetLocation, const FVector& InUpVector, int32 InArcSamples, FBXProjectileSimData& InOutData)
{
	BuildBezierControlPoints(InStartLocation, InTargetLocation, InUpVector, InBaked.BezierControlPoints, InOutData.Bezier.ControlPoints);
	InOutData.Bezier.TotalArcLength = BuildArcLengthTable(InOutData.Bezier.ControlPoints, InArcSamples, InOutData.Bezier.ArcLength);
	InOutData.Bezier.ParamU = 0.0f;
	InOutData.LastTargetLocation = InTargetLocation;
}

void FBXProjectileSolver::RebuildBezierRuntime(const FBXProjectileBakedConfig& InBaked, const FVector& InStartLocation, const FVector& InTargetLocation, const FVector& InUpVector, const FVector& InCurrentLocation, int32 InArcSamples, FBXProjectileSimData& InOutData)
{
	const float OldParamU = InOutData.Bezier.ParamU;

	BuildBezierControlPoints(InStartLocation, InTargetLocation, InUpVector, InBaked.BezierControlPoints, InOutData.Bezier.ControlPoints);
	InOutData.Bezier.TotalArcLength = BuildArcLengthTable(InOutData.Bezier.ControlPoints, InArcSamples, InOutData.Bezier.ArcLength);

	// 新曲线上找最近参数u*继续推进,旧位置与新曲线点的偏差记入弹簧偏移(渲染位置重建瞬间不变,由弹簧收敛消除)
	InOutData.Bezier.ParamU = FindClosestBezierParam(InOutData.Bezier.ControlPoints, InCurrentLocation, OldParamU);
	InOutData.SpringOffset += InCurrentLocation - EvaluateBezier(InOutData.Bezier.ControlPoints, InOutData.Bezier.ParamU);
	InOutData.LastTargetLocation = InTargetLocation;
}

#pragma endregion Bezier



#pragma region Integrate
void FBXProjectileSolver::IntegrateStep(const FBXProjectileBakedConfig& InBaked, const FBXProjectileTargetSnapshot* InSnapshot, float InFixedStep, FBXProjectileSimData& InOutData)
{
	// 残留阶段只推进计时
	if (InOutData.State == EBXProjectileState::PS_Residual)
	{
		InOutData.ElapsedTime += InFixedStep;
		InOutData.ResidualTimer -= InFixedStep;
		return;
	}

	if (InOutData.State == EBXProjectileState::PS_Dead)
	{
		return;
	}

	InOutData.PrevLocation = InOutData.Location;
	InOutData.ElapsedTime += InFixedStep;

	const bool bTargetValid = InSnapshot && InSnapshot->bValid;
	const float Speed = InBaked.SpeedCurve.Sample(InOutData.ElapsedTime);

	if (InBaked.bUseBezier)
	{
		// 贝塞尔:目标移动超阈值时重建曲线
		if (bTargetValid)
		{
			const bool bNeedRebuild = FVector::DistSquared(InSnapshot->Location, InOutData.LastTargetLocation) > InBaked.TargetRefreshDistance * InBaked.TargetRefreshDistance;
			if (bNeedRebuild)
			{
				RebuildBezierRuntime(InBaked, InOutData.FireContext.StartLocation, InSnapshot->Location, InOutData.FireContext.FireUpVector, InOutData.Location, InBaked.ArcSampleCount, InOutData);
			}
			else
			{
				InOutData.LastTargetLocation = InSnapshot->Location;
			}
		}

		const float DeltaS = Speed * InFixedStep;
		InOutData.Bezier.ParamU = AdvanceArcLengthParam(InOutData.Bezier.ArcLength, InOutData.Bezier.TotalArcLength, InOutData.Bezier.ParamU, DeltaS);
		InOutData.Location = EvaluateBezier(InOutData.Bezier.ControlPoints, InOutData.Bezier.ParamU);

		const FVector Tangent = EvaluateBezierTangent(InOutData.Bezier.ControlPoints, InOutData.Bezier.ParamU);
		InOutData.Velocity = Tangent * Speed;

		// 抵达曲线终点视为生命结束
		if (InOutData.Bezier.ParamU >= 1.0f - KINDA_SMALL_NUMBER)
		{
			if (InBaked.ResidualTime > 0.0f)
			{
				InOutData.State = EBXProjectileState::PS_Residual;
				InOutData.ResidualTimer = InBaked.ResidualTime;
			}
			else
			{
				InOutData.State = EBXProjectileState::PS_Dead;
			}
		}

		return;
	}

	FVector DesiredDirection = InOutData.Velocity.GetSafeNormal();

	if (InBaked.bHoming)
	{
		if (bTargetValid)
		{
			InOutData.LastTargetLocation = InSnapshot->Location;
			DesiredDirection = (InSnapshot->Location - InOutData.Location).GetSafeNormal();
		}
		else
		{
			switch (InBaked.TargetInvalidPolicy)
			{
			case EBXProjectileTargetInvalidPolicy::TIP_KeepDirection:
				DesiredDirection = InOutData.Velocity.GetSafeNormal();
				break;

			case EBXProjectileTargetInvalidPolicy::TIP_FlyToLastKnownLocation:
				DesiredDirection = (InOutData.LastTargetLocation - InOutData.Location).GetSafeNormal();
				break;

			case EBXProjectileTargetInvalidPolicy::TIP_SelfDestruct:
				InOutData.State = EBXProjectileState::PS_Dead;
				return;

			default:
				break;
			}
		}
	}

	FVector CurrentDirection = InOutData.Velocity.GetSafeNormal();
	if (CurrentDirection.IsNearlyZero())
	{
		CurrentDirection = InOutData.FireContext.FireDirection.GetSafeNormal();
	}

	FVector NewDirection = CurrentDirection;
	if (InBaked.bHoming && !DesiredDirection.IsNearlyZero())
	{
		const float TurnRate = InBaked.TurnRateCurve.Sample(InOutData.ElapsedTime);
		const float MaxAngleRad = FMath::DegreesToRadians(TurnRate * InFixedStep);
		NewDirection = RotateToward(CurrentDirection, DesiredDirection, MaxAngleRad);
	}

	InOutData.Velocity = NewDirection * Speed + InBaked.Gravity * InFixedStep;
	InOutData.Location += InOutData.Velocity * InFixedStep;
}

FVector FBXProjectileSolver::RotateToward(const FVector& InCurrentDirection, const FVector& InDesiredDirection, float InMaxAngleRad)
{
	if (InCurrentDirection.IsNearlyZero() || InDesiredDirection.IsNearlyZero())
	{
		return InDesiredDirection.IsNearlyZero() ? InCurrentDirection : InDesiredDirection;
	}

	const float DotResult = FMath::Clamp(FVector::DotProduct(InCurrentDirection, InDesiredDirection), -1.0f, 1.0f);
	const float Angle = FMath::Acos(DotResult);
	if (Angle <= KINDA_SMALL_NUMBER)
	{
		return InDesiredDirection;
	}

	if (InMaxAngleRad >= Angle)
	{
		return InDesiredDirection;
	}

	const FQuat FullRotation = FQuat::FindBetweenNormals(InCurrentDirection, InDesiredDirection);
	const FQuat PartialRotation = FQuat::Slerp(FQuat::Identity, FullRotation, InMaxAngleRad / Angle);
	return PartialRotation.RotateVector(InCurrentDirection);
}

void FBXProjectileSolver::SpringStep(float InFrequency, float InDeltaTime, FBXProjectileSimData& InOutData)
{
	if (InDeltaTime <= 0.0f || (InOutData.SpringOffset.IsNearlyZero() && InOutData.SpringVelocity.IsNearlyZero() && InOutData.SpringTarget.IsNearlyZero()))
	{
		return;
	}

	// 临界阻尼弹簧:加速度 = ω²·(目标-偏移) - 2ω·速度,无超调平滑收敛
	const float Omega = 2.0f * PI * FMath::Max(InFrequency, 0.1f);
	const FVector Acceleration = (InOutData.SpringTarget - InOutData.SpringOffset) * (Omega * Omega) - InOutData.SpringVelocity * (2.0f * Omega);
	InOutData.SpringVelocity += Acceleration * InDeltaTime;
	InOutData.SpringOffset += InOutData.SpringVelocity * InDeltaTime;

	// 收敛末端清零避免永续微小抖动
	if (InOutData.SpringOffset.IsNearlyZero(0.01f) && InOutData.SpringVelocity.IsNearlyZero(0.01f) && InOutData.SpringTarget.IsNearlyZero(0.01f))
	{
		InOutData.SpringOffset = FVector::ZeroVector;
		InOutData.SpringVelocity = FVector::ZeroVector;
		InOutData.SpringTarget = FVector::ZeroVector;
	}
}

void FBXProjectileSolver::MathCheckBullet(const FBXProjectileBakedConfig& InBaked, const FBXProjectileSimData& InData, int32 InBulletIndex, const FBXProjectileTargetSnapshot* InSnapshots, int32 InSnapshotCount, TArray<FBXProjectileHitCandidate>& InOutCandidates)
{
	if (InData.State != EBXProjectileState::PS_Active)
	{
		return;
	}

	// 检测线段叠加弹簧偏移:与渲染位置一致(服务器偏移恒零无影响)
	const FVector SegmentStart = InData.PrevLocation + InData.SpringOffset;
	const FVector SegmentEnd = InData.Location + InData.SpringOffset;

	// 盒形子弹:整盒沿线段扫掠判定,姿态局部X对齐飞行方向(与物理检测姿态一致)
	const bool bBoxBullet = InBaked.BulletShape == EBXProjectileShape::BS_Box;
	FRotator BoxRotation = FRotator::ZeroRotator;
	if (bBoxBullet)
	{
		const FVector SweepDirection = (SegmentEnd - SegmentStart).GetSafeNormal();
		if (!SweepDirection.IsNearlyZero())
		{
			BoxRotation = FQuat::FindBetweenNormals(FVector::ForwardVector, SweepDirection).Rotator();
		}
	}

	// 点/球体子弹的线段膨胀量:点=0 球体=半径(均为精确Minkowski膨胀)
	float ShapeInflation = 0.0f;
	if (InBaked.BulletShape == EBXProjectileShape::BS_Sphere)
	{
		ShapeInflation = InBaked.BulletRadius;
	}

	for (int32 SnapshotIndex = 0; SnapshotIndex < InSnapshotCount; ++SnapshotIndex)
	{
		const FBXProjectileTargetSnapshot& Snapshot = InSnapshots[SnapshotIndex];
		if (!Snapshot.bValid)
		{
			continue;
		}

		// 排除自身与已穿透命中过的目标
		if (Snapshot.TargetUID == InData.InstigatorUID || InData.HitTargetUIDs.Contains(Snapshot.TargetUID))
		{
			continue;
		}

		const FRotator ShapeRotation = Snapshot.Rotation.Rotator();
		bool bHit = false;
		if (bBoxBullet)
		{
			// 盒扫掠按目标形状分流:球=反向轨迹距离转化(精确) 盒=15轴SAT区间交集(精确) 胶囊=保守SAT(角部可能多判不漏检)
			switch (Snapshot.ShapeType)
			{
			case EBXShapeType::ST_Sphere:
				bHit = UBXFunctionLibrary::SweptBoxToSphere(SegmentStart, SegmentEnd, BoxRotation, InBaked.BulletBoxExtent, Snapshot.Location, Snapshot.ShapeExtent.X);
				break;

			case EBXShapeType::ST_Capsule:
				bHit = UBXFunctionLibrary::SweptBoxToCapsule(SegmentStart, SegmentEnd, BoxRotation, InBaked.BulletBoxExtent, Snapshot.Location, ShapeRotation, Snapshot.ShapeExtent.X, Snapshot.ShapeExtent.Y);
				break;

			case EBXShapeType::ST_Box:
				bHit = UBXFunctionLibrary::SweptBoxToBox(SegmentStart, SegmentEnd, BoxRotation, InBaked.BulletBoxExtent, Snapshot.Location, ShapeRotation, Snapshot.ShapeExtent);
				break;

			default:
				break;
			}
		}
		else
		{
			// 点/球体子弹:线段对目标形状距离判定(膨胀量为0或半径)
			float Distance = TNumericLimits<float>::Max();
			switch (Snapshot.ShapeType)
			{
			case EBXShapeType::ST_Sphere:
				Distance = UBXFunctionLibrary::SegmentToSphere(SegmentStart, SegmentEnd, Snapshot.Location, Snapshot.ShapeExtent.X);
				break;

			case EBXShapeType::ST_Capsule:
				Distance = UBXFunctionLibrary::SegmentToCapsule(SegmentStart, SegmentEnd, Snapshot.Location, ShapeRotation, Snapshot.ShapeExtent.X, Snapshot.ShapeExtent.Y);
				break;

			case EBXShapeType::ST_Box:
				Distance = UBXFunctionLibrary::SegmentToBox(SegmentStart, SegmentEnd, Snapshot.Location, ShapeRotation, Snapshot.ShapeExtent);
				break;

			default:
				break;
			}

			bHit = Distance <= ShapeInflation;
		}

		if (bHit)
		{
			// 命中点:线段上距目标中心最近点(近似)
			const FVector Segment = SegmentEnd - SegmentStart;
			const float SegmentSizeSquared = Segment.SizeSquared();
			const float Alpha = SegmentSizeSquared > KINDA_SMALL_NUMBER ? FMath::Clamp(FVector::DotProduct(Snapshot.Location - SegmentStart, Segment) / SegmentSizeSquared, 0.0f, 1.0f) : 0.0f;

			FBXProjectileHitCandidate Candidate;
			Candidate.BulletIndex = InBulletIndex;
			Candidate.TargetIndex = SnapshotIndex;
			Candidate.HitLocation = SegmentStart + Segment * Alpha;
			Candidate.HitNormal = (Candidate.HitLocation - Snapshot.Location).GetSafeNormal();
			if (Candidate.HitNormal.IsNearlyZero())
			{
				Candidate.HitNormal = FVector::UpVector;
			}
			Candidate.HitBoxTag = Snapshot.HitBoxTag;
			InOutCandidates.Add(Candidate);

			// 非穿透弹首个命中即终态,无需继续检测
			if (InBaked.MaxPenetrationCount <= 1)
			{
				break;
			}
		}
	}
}

#pragma endregion Integrate
