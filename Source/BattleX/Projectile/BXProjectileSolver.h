#pragma once

#include "CoreMinimal.h"

#include "BXProjectileStructs.h"



// 子弹运动求解器(纯函数无状态,并行阶段安全调用,禁止访问UObject)
class BATTLEX_API FBXProjectileSolver
{
public:
	// 构建贝塞尔世界空间控制点(以起始点指向目标点线段为X轴,发射者UpVector正交化为Z轴)
	static void BuildBezierControlPoints(const FVector& InStartLocation, const FVector& InTargetLocation, const FVector& InUpVector, const TArray<FBXBezierControlPoint>& InControlPointConfigs, TArray<FVector>& OutControlPoints);

	// 贝塞尔求值(de Casteljau,栈缓冲无堆分配)
	static FVector EvaluateBezier(const TArray<FVector>& InControlPoints, float InU);

	// 贝塞尔切线(中心差分)
	static FVector EvaluateBezierTangent(const TArray<FVector>& InControlPoints, float InU);

	// 构建弧长累积表,返回总弧长
	static float BuildArcLengthTable(const TArray<FVector>& InControlPoints, int32 InSampleCount, TArray<float>& OutArcLength);

	// 弧长推进参数u(总弧长非正值时按参数比例直推)
	static float AdvanceArcLengthParam(const TArray<float>& InArcLength, float InTotalArcLength, float InU, float InDeltaS);

	// 曲线上最近参数搜索(全局粗采样+Hint附近细化)
	static float FindClosestBezierParam(const TArray<FVector>& InControlPoints, const FVector& InLocation, float InHintU);

	// 初始化贝塞尔运行时上下文(发射时调用)
	static void InitBezierRuntime(const FBXProjectileBakedConfig& InBaked, const FVector& InStartLocation, const FVector& InTargetLocation, const FVector& InUpVector, int32 InArcSamples, FBXProjectileSimData& InOutData);

	// 目标点变化时重建贝塞尔运行时上下文(投影回曲线,偏差统一记入弹簧偏移由弹簧收敛)
	static void RebuildBezierRuntime(const FBXProjectileBakedConfig& InBaked, const FVector& InStartLocation, const FVector& InTargetLocation, const FVector& InUpVector, const FVector& InCurrentLocation, int32 InArcSamples, FBXProjectileSimData& InOutData);

	// 单步运动积分(直线/跟踪/贝塞尔统一入口,快照为空代表无有效目标)
	static void IntegrateStep(const FBXProjectileBakedConfig& InBaked, const FBXProjectileTargetSnapshot* InSnapshot, float InFixedStep, FBXProjectileSimData& InOutData);

	// 以最大角速度向期望方向旋转当前方向
	static FVector RotateToward(const FVector& InCurrentDirection, const FVector& InDesiredDirection, float InMaxAngleRad);

	// 临界阻尼弹簧步进(快照矫正偏移平滑收敛,InFrequency为自然频率Hz)
	static void SpringStep(float InFrequency, float InDeltaTime, FBXProjectileSimData& InOutData);

	// 数学命中判定(点/球体线段膨胀与盒扫掠按目标形状分流,候选写入InOutCandidates,禁止访问UObject)
	static void MathCheckBullet(const FBXProjectileBakedConfig& InBaked, const FBXProjectileSimData& InData, int32 InBulletIndex, const FBXProjectileTargetSnapshot* InSnapshots, int32 InSnapshotCount, TArray<FBXProjectileHitCandidate>& InOutCandidates);
};
