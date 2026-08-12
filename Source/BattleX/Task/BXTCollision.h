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
	// 碰撞盒标签,用于从ShapeComponent上匹配对应的碰撞盒信息
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Collision")
	FGameplayTag HitBoxTag;

	// 折线Sweep配置: X=最大段数 Y=共线检测角度阈值 Z=旋转分段角度阈值
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Collision")
	FIntVector PolylineConfig = FIntVector(3, 10, 15);

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
