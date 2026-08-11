#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"

#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"

#include "BXTLAsset.h"
#include "BXBuffEnums.h"
#include "BXBuffStructs.h"

#include "BXBuffAsset.generated.h"



// BUFF资产(继承Timeline资产,复用Sections/TaskList)
UCLASS(BlueprintType, Blueprintable)
class BATTLEX_API UBXBuffAsset : public UBXTLAsset
{
	GENERATED_BODY()

#pragma region Important
public:
	// BUFF Tag集合
	UPROPERTY(EditDefaultsOnly, Category = "Important")
	FGameplayTagContainer BuffTags;

#pragma endregion Important



#pragma region Lifetime
public:
	// 生命周期类型
	UPROPERTY(EditDefaultsOnly, Category = "Lifetime")
	EBXBuffLifeType LifeType = EBXBuffLifeType::BL_Duration;

	// 持续时长,-1代表无限
	UPROPERTY(EditDefaultsOnly, Category = "Lifetime", meta = (EditCondition = "LifeType==EBXBuffLifeType::BL_Duration", ClampMin = "-1.0"))
	float BuffDuration = 5.0f;

	// 层级生命周期模式
	UPROPERTY(EditDefaultsOnly, Category = "Lifetime")
	EBXBuffLayerLifeMode LayerLifeMode = EBXBuffLayerLifeMode::BLL_Shared;

#pragma endregion Lifetime



#pragma region Stack
public:
	// 最大层数,默认1
	UPROPERTY(EditDefaultsOnly, Category = "Stack", meta = (ClampMin = "1"))
	int32 MaxLayer = 1;

	// 最大等级
	UPROPERTY(EditDefaultsOnly, Category = "Stack", meta = (ClampMin = "1"))
	int32 MaxLevel = 1;

#pragma endregion Stack



#pragma region Policy
public:
	// 共存策略
	UPROPERTY(EditDefaultsOnly, Category = "Policy")
	EBXBuffCoexistPolicy CoexistPolicy = EBXBuffCoexistPolicy::BC_Coexist;

	// 添加已有BUFF时是否刷新生命时长
	UPROPERTY(EditDefaultsOnly, Category = "Policy")
	bool bRefreshLifetimeOnAdd = true;

#pragma endregion Policy



#pragma region Tasks
public:
	// Task层级绑定列表
	UPROPERTY(EditDefaultsOnly, Category = "Tasks")
	TArray<FBXBuffTaskBinding> TaskBindings;

#pragma endregion Tasks
};
