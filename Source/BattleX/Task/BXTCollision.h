#pragma once

#include "CoreMinimal.h"

#include "BXTask.h"
#include "BXStructs.h"
#include "BXCollision.h"
#include "BXGameplayTags.h"

#include "BXTCollision.generated.h"

class UBXTLAsset;
class USkeletalMesh;


UCLASS(BlueprintType, Blueprintable)
class BATTLEX_API UBXTCollision : public UBXTask
{
	GENERATED_BODY()

public:
	// 碰撞检测冷却
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Collision", Meta = (ClampMin = "0.1"))
	float CoolDown = 10.0f;

	// 数量限制
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Filter", Meta = (ClampMin = "1"))
	int32 Limit = 100;

	// 数量限制逻辑(不填则锁定的优先级高)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Filter")
	FGameplayTag LimitLogic;

	// 角色类型筛选
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Filter")
	FGameplayTagContainer CharacterTags;

	// 关系筛选
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Filter")
	FGameplayTagContainer RelationshipTags;

	// 碰撞类型
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Filter")
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;

	// 碰撞结果引擎层筛选
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Filter")
	FBXCFilter EngineFilter;
};






UCLASS(BlueprintType, Blueprintable)
class BATTLEX_API UBXTIntervalCollision : public UBXTCollision
{
	GENERATED_BODY()

public:
	// 碰撞检测间隔
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Collision", Meta = (ClampMin = "0.001"))
	float Interval = 0.1f;

	// 碰撞检测次数(小于等于0时，代表次数不定)
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Collision")
	int32 Count = 0;


#pragma region Editor
#if WITH_EDITOR
public:
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
#pragma endregion Editor
};






UCLASS(BlueprintType, Blueprintable)
class BATTLEX_API UBXTTrackHitBox : public UBXTCollision
{
	GENERATED_BODY()

public:
	UBXTTrackHitBox();

public:
	// 碰撞盒标签,用于从ShapeComponent上匹配对应的碰撞盒信息
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Collision")
	FGameplayTag HitBoxTag;

	/*
	 * 碰撞检测角度步进
	 * X: 最小步进角度(帧率>=60时使用,精度高)
	 * Y: 最大步进角度(帧率<=20时使用,性能好)
	 * 帧率在20~60之间时线性插值
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Collision")
	FVector2D SweepAngleStep = FVector2D(15.0f, 30.0f);

	/*
	 * 对轨迹进行冗余点去除的规则
	 * X: 坐标共线判定误差(角度)，把在同一条直线上的位置进行排除
	 * Y: 方向判定误差(角度)，把相同方向的数据进行排除
	 * Z: 缩放判定误差(倍率)，把相同大小的数据进行排除
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Trajectory")
	FVector TrajectoryOptimization = FVector(10.0f, 15.0f, 0.1f);

	// 存储的骨骼模型空间轨迹
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Trajectory", AdvancedDisplay)
	FBXTrajectoryPoints BoneSampledTrajectory;


#pragma region Editor
#if WITH_EDITOR
public:
	virtual void PreSave(FObjectPreSaveContext SaveContext) override;

	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

	virtual void RefreshDataBeforePreview();

	virtual bool EnablePassiveTrigger() override;

	// 解析采样骨骼来源,返回采样骨骼名和SkeletalMesh,子类可重写以提供不同来源(如武器挂接Socket)
	virtual bool ResolveSampleBoneSource(UBXTLAsset* OwnerAsset, int32 SectionIndex, FName& OutBoneName, USkeletalMesh*& OutSkeletalMesh);
#endif
#pragma endregion Editor
};






UCLASS(BlueprintType, Blueprintable)
class BATTLEX_API UBXTTrackWeaponHitBox : public UBXTTrackHitBox
{
	GENERATED_BODY()

public:
	UBXTTrackWeaponHitBox();

public:
	// 武器插槽
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Collision")
	FGameplayTag WeaponSlot = BXGameplayTags::BXGearSlot_RightHand;


#pragma region Editor
#if WITH_EDITOR
public:
	virtual bool ResolveSampleBoneSource(UBXTLAsset* OwnerAsset, int32 SectionIndex, FName& OutBoneName, USkeletalMesh*& OutSkeletalMesh) override;
#endif
#pragma endregion Editor
};
