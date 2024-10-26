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
	virtual void GetHitResults(float InStartTime, FGameplayTagContainer& BoxTags, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, const FBXCFilter& Filter, TArray<FHitResult>& OutResults) override;

protected:
	// 添加新的碰撞盒位置信息，一般情况下在辅助线程调用
	virtual void AddNewHitBoxRecord();

protected:
	// 位置记录间隔
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Collision")
	float UpdateHitBoxRecordInterval = 0.02f;

	// 1秒内碰撞盒位置信息
	UPROPERTY(Transient, BlueprintReadWrite, Category = "Collision")
	TMap<FGameplayTag, FBXGHitBoxRecords> HitBoxRecords;

	// 辅助容器
	TMap<FGameplayTag, TWeakObjectPtr<USceneComponent>> HBRHelpMap;

private:
	// 正在添加碰撞盒位置信息
	bool bAddingBoxRecords = false;

	// 正在清理碰撞盒位置信息
	bool bCleaningBoxRecords = false;

	// 清理碰撞盒信息时间戳
	int64 CleanBoxRecordsTS = 0;
	
#pragma endregion Collision

};