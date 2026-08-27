#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#include "BXBehaviorEnums.h"
#include "BXStateEnums.h"

#include "BXEventStructs.generated.h"



// 空事件参数
USTRUCT(BlueprintType)
struct FBXEventEmpty
{
	GENERATED_USTRUCT_BODY()

};



// 行为事件参数(进入/退出)
USTRUCT(BlueprintType)
struct FBXEventBehaviorChanged
{
	GENERATED_USTRUCT_BODY()

public:
	FBXEventBehaviorChanged() {}
	FBXEventBehaviorChanged(FGameplayTag InBehaviorTag, int64 InSign, EBXBehaviorEndReason InReason)
		: BehaviorTag(InBehaviorTag), Sign(InSign), Reason(InReason) {}

public:
	// 行为Tag
	UPROPERTY(Transient, BlueprintReadWrite)
	FGameplayTag BehaviorTag;

	// 来源签名(技能SkillID/时间轴TLID/0系统常驻)
	UPROPERTY(Transient, BlueprintReadWrite)
	int64 Sign = 0;

	// 原因(Exit事件:结束原因;Enter事件:BER_TMax=新进入/BER_Resumed=挂起恢复)
	UPROPERTY(Transient, BlueprintReadWrite)
	EBXBehaviorEndReason Reason = EBXBehaviorEndReason::BER_TMax;

};



// 状态事件参数(进入/退出)
USTRUCT(BlueprintType)
struct FBXEventStateChanged
{
	GENERATED_USTRUCT_BODY()

public:
	FBXEventStateChanged() {}
	FBXEventStateChanged(FGameplayTag InStateTag, int64 InSign, float InDuration, EBXStateEndReason InReason)
		: StateTag(InStateTag), Sign(InSign), Duration(InDuration), Reason(InReason) {}

public:
	// 状态Tag
	UPROPERTY(Transient, BlueprintReadWrite)
	FGameplayTag StateTag;

	// 来源签名
	UPROPERTY(Transient, BlueprintReadWrite)
	int64 Sign = 0;

	// 进入时长(≤0无限,仅Enter事件有效)
	UPROPERTY(Transient, BlueprintReadWrite)
	float Duration = -1.0f;

	// 结束原因(仅Exit事件有效)
	UPROPERTY(Transient, BlueprintReadWrite)
	EBXStateEndReason Reason = EBXStateEndReason::SER_TMax;

};
