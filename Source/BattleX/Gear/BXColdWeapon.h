#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ShapeComponent.h"

#include "BXGear.h"
#include "BXShapeComponent.h"

#include "BXColdWeapon.generated.h" 



UCLASS(Abstract, Blueprintable, BlueprintType)
class BATTLEX_API ABXColdWeapon : public ABXGear
{
	GENERATED_BODY()

#pragma region Important
public:
	ABXColdWeapon(const FObjectInitializer& ObjectInitializer);

	virtual ~ABXColdWeapon();

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
	// 碰撞信息组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UBXShapeComponent* HitBoxComponent = nullptr;

#pragma endregion Important



#pragma region Use
public:
	void InternalPostUsing(UPARAM(ref) FBXUsingGearInformation& UsingInfo) override;

	void InternalPreUnusing(UPARAM(ref) FBXUsingGearInformation& UnusingInfo) override;
	
#pragma endregion Use



#pragma region State
protected:
	virtual void InternalChangeState(FGameplayTag NewState) override;

#pragma endregion State



#pragma region Collision
public:
	// 开始记录位置
	UFUNCTION(BlueprintCallable, Category = "Collision")
	virtual void StartRecordLocation();

	// 停止记录位置
	UFUNCTION(BlueprintCallable, Category = "Collision")
	virtual void StopRecordLocation();

	// 获取N秒内，某个碰撞盒的碰撞结果信息
	virtual void GetHitResults(float InStartTime, FGameplayTagContainer& BoxTags, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, const FBXCFilter& Filter, TArray<FHitResult>& OutResults, FVector4 OverrideRules) override;

protected:
	// 添加新的碰撞盒位置信息，一般情况下在辅助线程调用
	UFUNCTION()
	virtual void AddNewHitBoxRecord();

protected:
	// 位置记录间隔
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Collision", Meta = (ClampMin = 0.02))
	float UpdateHitBoxRecordInterval = 0.028f;

	// 位置记录失效时间
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Collision", Meta = (ClampMin = 0.5f, ClampMax = 5.0f))
	float HitBoxRecordExpiryTime = 2.0f;
	
	/*
	 * 路径追踪碰撞查询时，对冗余路径点进行合并的默认参数集
	 * X:坐标共线判定误差(角度)，把在同一条直线上的位置进行排除
	 * Y:方向判定误差(角度)，把相同方向的数据进行排除
	 * Z:缩放判定误差(倍率)，把相同大小的数据进行排除
	 * W:碰撞检测步进角度，数值越小误差越小
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Collision")
	FVector4 TrackOptimizationRules = FVector4(10.0f, 30.0f, 0.1f, 20.0f);
	
	// 1秒内碰撞盒位置信息
	UPROPERTY(Transient, BlueprintReadWrite, Category = "Collision")
	TMap<FGameplayTag, FBXGHitBoxRecords> HitBoxRecords;

	// 预碰撞盒位置信息
	UPROPERTY(Transient, BlueprintReadWrite, Category = "Collision")
	TMap<FGameplayTag, FBXGHitBoxRecords> PendingHitBoxRecords;

	// 辅助容器
	TMap<FGameplayTag, TWeakObjectPtr<USceneComponent>> HBRHelpMap;

private:
	// 正在使用碰撞盒位置信息
	bool bUsingHitBoxRecords = false;

	// 清理碰撞盒信息时间戳
	int64 CleanBoxRecordsTS = 0;
	
#pragma endregion Collision

};