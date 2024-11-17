#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimMontage.h"

#include "Task/BXTask.h"
#include "Animation/BXAnimationLibrary.h"

#include "BXTAnimation.generated.h"



UCLASS(BlueprintType, Blueprintable)
class BATTLEX_API UBXTPlayAnimation : public UBXTask
{
	GENERATED_BODY()

public:
	UBXTPlayAnimation();
	
public:
	// 动画资源类型
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EBXTAnimationAssetType AssetType;

	// 动画蒙太奇
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Meta = (EditCondition = "AssetType == EBXTAnimationAssetType::AAT_Montage", EditConditionHides))
	UAnimMontage* Montage = nullptr;

	// 动画库中资源标签(暂未开放)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Meta = (EditCondition = "AssetType == EBXTAnimationAssetType::AAT_Library", EditConditionHides))
	FGameplayTag Tag;

	// 播放速率
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float PlayRate = 1.0f;
	
	// 被中断时的淡出时间
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float InterruptedBlendOut = 0.2f;

	// 播放N秒后，可以被移动中断（小于零代表不会被移动中断）
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float DelayInterruptedByMove = -1.0f;

	// 播放动画的组件名称
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, AdvancedDisplay)
	FName PlayComponentName = FName(TEXT("Mesh"));

	// 动画行为标签
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, AdvancedDisplay)
	FGameplayTag PlayAnimBehaviorTag;
	
	// 播放优先级
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, AdvancedDisplay)
	int32 PlayPriority = 100;
	
	// 终止同组其他蒙太奇的播放
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, AdvancedDisplay)
	bool bStopGroup = true;
	
	// 启用Rootmotion
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, AdvancedDisplay)
	bool bEnableRootmotion = true;


	
#pragma region Editor
#if WITH_EDITOR
public:
	virtual bool EnablePassiveTrigger() override;
	
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
#pragma endregion Editor
	
};
