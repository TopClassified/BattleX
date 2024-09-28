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

	virtual void TickActor(float DeltaTime, enum ELevelTick TickType, FActorTickFunction& ThisTickFunction) override;

protected:
	// 垃圾清理间隔
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GCInterval = 10.0f;
	// 上一次清理垃圾的时间戳
	float LastGCTS = 0.0f;

	// 碰撞信息组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UBXShapeComponent* HitBoxComponent = nullptr;

#pragma endregion Important



#pragma region State
protected:
	virtual void InternalChangeState(EBXGearState NewState) override;

#pragma endregion State



#pragma region Collision
public:
	// 获取所有的碰撞盒名称
	UFUNCTION(BlueprintCallable, Category = "Collision")
	virtual void GetAllHitBoxNames(TArray<FName>& OutBoxNames);

	// 开始记录位置
	UFUNCTION(BlueprintCallable, Category = "Collision")
	virtual void StartRecordLocation(const TArray<FName>& BoxNames);

	// 停止记录位置
	UFUNCTION(BlueprintCallable, Category = "Collision")
	virtual void StopRecordLocation(const TArray<FName>& BoxNames);

	// 获取N秒内，某个碰撞盒的碰撞结果信息
	UFUNCTION(BlueprintCallable, Category = "Collision")
	virtual void GetHitResultsInSeconds(AActor* Requester, float Seconds, const FName& BoxName, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, const FBXCFilter& Filter, TArray<FHitResult>& OutResults);

protected:
	virtual void AddNewHitBoxRecord(const TArray<FName>& BoxNames);

protected:
	// 要记录的碰撞盒名称
	UPROPERTY(Transient, BlueprintReadWrite, Category = "Collision")
	TArray<FName> RecordBoxNames;

	// 1秒内碰撞盒位置信息
	UPROPERTY(Transient, BlueprintReadWrite, Category = "Collision")
	TMap<FName, FBXGHitBoxRecords> HitBoxRecords;

	// 辅助容器
	UPROPERTY(Transient)
	TMap<FName, USceneComponent*> HelpMap1;

#pragma endregion Collision

};