#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "BXFunctionLibrary.generated.h" 



UCLASS(Blueprintable)
class BATTLEX_API UBXFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

#pragma region Misc
public:
	// 63位的唯一
	UFUNCTION(BlueprintPure, Category = "Misc")
	static int64 GetUniqueID();

	// 31位的唯一
	UFUNCTION(BlueprintPure, Category = "Misc")
	static int32 GetShortUniqueID();

	// 获取当前时间戳（毫秒）
	UFUNCTION(BlueprintCallable, Category = "Time")
	static int64 GetUtcMillisecond();

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
