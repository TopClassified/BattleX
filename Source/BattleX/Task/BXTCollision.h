#pragma once

#include "CoreMinimal.h"

#include "BXTask.h"
#include "BXCollision.h"
#include "BXGameplayTags.h"

#include "BXTCollision.generated.h"



UCLASS(BlueprintType, Blueprintable)
class BATTLEX_API UBXTCollision : public UBXTask
{
	GENERATED_BODY()
	
public:
	// 碰撞检测间隔
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Collision", Meta = (ClampMin = "0.001"))
	float Interval = 0.1f;

	// 碰撞检测次数(小于等于0时，代表次数不定)
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Collision")
	int32 Count = 0;
	
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
	// 碰撞盒名称
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Collision")
	FGameplayTagContainer HitBoxTags;

	// 碰撞检测角度步进(当路径上点的角度差异过大时，数值越小，步进次数越多，性能消耗越大)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Collision", AdvancedDisplay)
	float SweepAngleStep = 20.0f;

	
	/*
	 * 对烘焙轨迹进行冗余点去除的规则
	 * X:坐标共线判定误差(角度)，把在同一条直线上的位置进行排除
	 * Y:方向判定误差(角度)，把相同方向的数据进行排除
	 * Z:缩放判定误差(倍率)，把相同大小的数据进行排除
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Bake")
	FVector BakedTrajectoryOptimization = FVector(10.0f, 15.0f, 0.1f);
	
	// 烘焙的碰撞盒轨迹列表
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Bake", AdvancedDisplay)
	TMap<FGameplayTag, FBXTrajectoryPoints> BakedHBTrajectoryPoints;


#pragma region Editor
public:
	virtual void CleanBakedData_Implementation() override;
	
	virtual void BakingData_Implementation(const FBXTLRunTimeData& InOutRTData, const FBXTLSectionRTData& InOutRTSData, const FBXTLTaskRTData& InOutRTTData) override;

	virtual void PostBakeData_Implementation() override;
	
#if WITH_EDITOR
public:
	virtual void PreSave(FObjectPreSaveContext SaveContext) override;
	
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
	
	virtual bool EnablePassiveTrigger() override;
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
public:
	virtual void BakingData_Implementation(const FBXTLRunTimeData& InOutRTData, const FBXTLSectionRTData& InOutRTSData, const FBXTLTaskRTData& InOutRTTData) override;
#pragma endregion Editor
};
