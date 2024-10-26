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
	// 碰撞检测次数(开始一次，结束一次，检测频率=时长/次数)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Collision", Meta = (ClampMin = "1"))
	float Count = 1;

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
class BATTLEX_API UBXTTrackWeaponCollision : public UBXTCollision
{
	GENERATED_BODY()
	
public:
	// 武器插槽
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag WeaponSlot = BXGameplayTags::BXGearSlot_RightHand;

	// 武器碰撞盒
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTagContainer WeaponHitBoxTags;
	
	// 是否缓存武器相对位置(非多目标时才有效)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bCacheRTransform = false;

	// 相对位置组件名称
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Meta = (EditCondition = "bCacheRTransform", EditConditionHides))
	FName CacheRTransfomName = NAME_None;

	// 相对位置插槽名称
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Meta = (EditCondition = "bCacheRTransform", EditConditionHides))
	FBXBoneSelector CacheRTransfomBone;

	// 相对位置列表(记录第0~第Count次)
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Meta = (EditCondition = "bCacheRTransform", EditConditionHides))
	TArray<FTransform> CacheRTransfomList;
	
#if WITH_EDITOR
public:
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
	
#endif
	
};
