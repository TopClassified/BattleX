#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "GameplayTagContainer.h"

#include "BXFunctionLibrary.generated.h" 



UCLASS(Blueprintable)
class BATTLEX_API UBXFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

#pragma region Misc
public:
	// 63位的唯一ID
	UFUNCTION(BlueprintPure, Category = "Misc")
	static int64 GetUniqueID();

	// 获取当前客户端时间
	UFUNCTION(BlueprintCallable, Category = "Time")
	static float GetClientTimeSeconds(UObject* InWorldContext);
	
	// 获取当前时间戳（毫秒）
	UFUNCTION(BlueprintCallable, Category = "Time")
	static int64 GetUtcMillisecond();

	// 获取服务器世界时间（毫秒）。服务器/单机返回本地世界时间；客户端返回经引擎时间同步校正的估算值。
	// 网络时间戳校验与延迟估算统一使用本函数，禁止跨机器直接比较本地墙钟。
	UFUNCTION(BlueprintCallable, Category = "Time", meta = (WorldContext = "InWorldContext"))
	static int64 GetServerWorldTimeMilliseconds(UObject* InWorldContext);

	// 获取游戏运行时间（微秒）
	UFUNCTION(BlueprintCallable, Category = "Time")
	static int64 GetGameMicrosecond();

	// 时间对齐
	UFUNCTION(BlueprintPure, Category = "Misc")
	static float AlignTime(float InTime, float InAlign);

protected:
	static int32 IDCreater;
	static int32 ShortIDCreater;

#pragma endregion Misc



#pragma region GameplayTag
public:
	// 获取GameplayTag的最后子标签的名称
	UFUNCTION(BlueprintCallable, Category = "GameplayTag")
	static FName GetLastGameplayTagSubName(const FGameplayTag& InTag);
	
#pragma endregion GameplayTag



#pragma region Property
public:
	static void CopyStruct(void* DestAddress, void* SrcAddress, UScriptStruct* StructType, UObject* Dest, UObject* Src);

	UFUNCTION(BlueprintCallable, Category = "Property")
	static void CopyObject(UObject* DestObject, UObject* SrcObject);

	static void CopyData(void* DestAddress, void* SrcAddress, FProperty* DataType, UObject* Dest, UObject* Src);

#pragma endregion Property



#pragma region Timeline
public:
	// 获取时间轴任务的索引值
	UFUNCTION(BlueprintPure, Category = "Timeline")
	static int32 GetTaskFullIndex(class UBXTLAsset* InAsset, class UBXTask* InTask);

	// 获取时间轴任务的索引值
	UFUNCTION(BlueprintPure, Category = "Timeline")
	static int32 GetSoftTaskFullIndex(class UBXTLAsset* InAsset, TSoftObjectPtr<UBXTask> InTask);

#pragma endregion Timeline



#pragma region Math
public:
	// 判断三点是否共线
	UFUNCTION(BlueprintCallable, Category = "Math")
	static bool AreCollinear(const FVector& A, const FVector& B, const FVector& C, float AngleTolerance);
	
	// 计算点到线段的最短距离
	UFUNCTION(BlueprintCallable, Category = "Math")
	static float PointToSegment(const FVector& InP, const FVector& InLS, const FVector& InLE);

	// 计算点到圆形的最短距离
	UFUNCTION(BlueprintCallable, Category = "Math")
	static float PointToCircle(const FVector& InP, const FVector& InCenter, const FRotator& InRotation, const float& InRadius);

	// 计算点到矩形的最短距离
	UFUNCTION(BlueprintCallable, Category = "Math")
	static float PointToRectangle(const FVector& InP, const FVector& InCenter, const FRotator& InRotation, const FVector2D& InExtent);

	// 计算点到球体的最短距离
	UFUNCTION(BlueprintCallable, Category = "Math")
	static float PointToSphere(const FVector& InP, const FVector& InCenter, const float& InRadius);

	// 计算点到胶囊体的最短距离
	UFUNCTION(BlueprintCallable, Category = "Math")
	static float PointToCapsule(const FVector& InP, const FVector& InCenter, const FRotator& InRotation, const float& InRadius, const float& InHalfHeight);

	// 计算点到圆柱体的最短距离
	UFUNCTION(BlueprintCallable, Category = "Math")
	static float PointToCylinder(const FVector& InP, const FVector& InCenter, const FRotator& InRotation, const float& InRadius, const float& InHalfHeight);

	// 计算点到长方体的最短距离
	UFUNCTION(BlueprintCallable, Category = "Math")
	static float PointToBox(const FVector& InP, const FVector& InCenter, const FRotator& InRotation, const FVector& InExtent);

	// 计算线段到线段之间的最短距离
	UFUNCTION(BlueprintCallable, Category = "Math")
	static float SegmentToSegment(const FVector& InL1S, const FVector& InL1E, const FVector& InL2S, const FVector& InL2E);

	// 计算线段到球体之间的最短距离
	UFUNCTION(BlueprintCallable, Category = "Math")
	static float SegmentToSphere(const FVector& InL1S, const FVector& InL1E, const FVector& InCenter, const float& InRadius);

	// 计算线段到胶囊体之间的最短距离
	UFUNCTION(BlueprintCallable, Category = "Math")
	static float SegmentToCapsule(const FVector& InL1S, const FVector& InL1E, const FVector& InCenter, const FRotator& InRotation, const float& InRadius, const float& InHalfHeight);

	// 计算线段到长方体之间的最短距离
	UFUNCTION(BlueprintCallable, Category = "Math")
	static float SegmentToBox(const FVector& InL1S, const FVector& InL1E, const FVector& InCenter, const FRotator& InRotation, const FVector& InExtent);

	// 判定长方体沿线段扫掠与球体是否相交(精确:等价转化为球心反向轨迹线段到静止盒的距离判定)
	UFUNCTION(BlueprintCallable, Category = "Math")
	static bool SweptBoxToSphere(const FVector& InBoxStart, const FVector& InBoxEnd, const FRotator& InBoxRotation, const FVector& InBoxExtent, const FVector& InSphereCenter, const float& InSphereRadius);

	// 判定长方体沿线段扫掠与胶囊体是否相交(精确:7轴SAT粗筛拒绝后,对凸距离函数f(t)=轴线段反向平移到盒距离在[0,1]三分搜索最小值)
	UFUNCTION(BlueprintCallable, Category = "Math")
	static bool SweptBoxToCapsule(const FVector& InBoxStart, const FVector& InBoxEnd, const FRotator& InBoxRotation, const FVector& InBoxExtent, const FVector& InCapsuleCenter, const FRotator& InCapsuleRotation, const float& InCapsuleRadius, const float& InCapsuleHalfHeight);

	// 判定长方体沿线段扫掠与长方体是否相交(精确:15轴SAT重叠时段区间交集)
	UFUNCTION(BlueprintCallable, Category = "Math")
	static bool SweptBoxToBox(const FVector& InBoxStart, const FVector& InBoxEnd, const FRotator& InBoxRotation, const FVector& InBoxExtent, const FVector& InTargetCenter, const FRotator& InTargetRotation, const FVector& InTargetExtent);

protected:
	// 单轴扫掠区间测试:|中心投影差+t·扫掠投影|≤半径和在[0,1]上的t区间与已有时段求交,确定分离或区间已空返回false
	static bool InternalSweptAxisOverlap(const FVector& InAxis, const FVector& InSweepDelta, float InCenterDelta, float InRadiusSum, float& InOutMinTime, float& InOutMaxTime);

#pragma endregion Math



#pragma region Component
public:
	// 根据骨骼名称，获取组件
	UFUNCTION(BlueprintCallable, Category = "Component")
	static USceneComponent* GetSceneComponentBySocketName(AActor* InActor, const FName& InSocketName, bool bCheckChild = true);
	
	// 根据名称，获取组件
	UFUNCTION(BlueprintCallable, Category = "Component")
	static USceneComponent* GetSceneComponentByNameAndClass(AActor* InActor, const FName& InName, UClass* InClass, bool bCheckChild = true);
	
protected:
	static USceneComponent* InternalGetSceneComponentBySocketName(AActor* InActor, const FName& InSocketName);
	
	static USceneComponent* InternalGetSceneComponentByNameAndClass(AActor* InActor, const FName& InName, UClass* InClass);

#pragma endregion Component



#pragma region DrawDebug
#if WITH_EDITOR
public:
	static void DrawDebugBoxInEditor(UObject* WorldContextObject, const FVector Center, FVector Extent, FLinearColor LineColor, const FRotator Rotation = FRotator::ZeroRotator, float Duration = 0.f, float Thickness = 0.f);

	static void DrawDebugSphereInEditor(UObject* WorldContextObject, const FVector Center, float Radius = 100.f, int32 Segments = 12, FLinearColor LineColor = FLinearColor::White, float Duration = 0.f, float Thickness = 0.f);

	static void DrawDebugSweepBoxInEditor(const UWorld* InWorld, FVector const& Start, FVector const& End, FRotator const& Orientation, FVector const& HalfSize, FColor const& Color, bool bPersistentLines, float LifeTime, uint8 DepthPriority);

	static void DrawDebugSweepSphereInEditor(const UWorld* InWorld, FVector const& Start, FVector const& End, float Radius, FColor const& Color, bool bPersistentLines, float LifeTime, uint8 DepthPriority);

	static void DrawDebugSweepCapsuleInEditor(const UWorld* InWorld, FVector const& Start, FVector const& End, float HalfHeight, float Radius, FColor const& Color, bool bPersistentLines, float LifeTime, uint8 DepthPriority);
#endif
#pragma endregion DrawDebug

};
