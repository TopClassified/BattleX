#pragma once

#include "CoreMinimal.h"

#include "BXStateEnums.h"

#include "BXEventStructs.generated.h"



// 空事件参数
USTRUCT(BlueprintType)
struct FBXEventEmpty
{
	GENERATED_USTRUCT_BODY()

};



// 行为禁止事件参数
USTRUCT(BlueprintType)
struct FBXEventForbiddenBehavior
{
	GENERATED_USTRUCT_BODY()

public:
	FBXEventForbiddenBehavior() {}
	FBXEventForbiddenBehavior(FGameplayTag InBehavior, bool InForbidden): BehaviorTag(InBehavior), bForbidden(InForbidden) {}
	FBXEventForbiddenBehavior(FGameplayTag InBehavior, bool InForbidden, EBXForbiddenBehaviorReason InReason): BehaviorTag(InBehavior), bForbidden(InForbidden), Reason(InReason) {}
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag BehaviorTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bForbidden = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EBXForbiddenBehaviorReason Reason = EBXForbiddenBehaviorReason::FB_OtherBehavior;

};



// 行为开始事件参数
USTRUCT(BlueprintType)
struct FBXEventStartBehavior
{
	GENERATED_USTRUCT_BODY()

public:
	FBXEventStartBehavior() {}
	FBXEventStartBehavior(FGameplayTag InBehavior): BehaviorTag(InBehavior) {}
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag BehaviorTag;

};



// 行为停止事件参数
USTRUCT(BlueprintType)
struct FBXEventStopBehavior
{
	GENERATED_USTRUCT_BODY()

public:
	FBXEventStopBehavior() {}
	FBXEventStopBehavior(FGameplayTag InBehavior): BehaviorTag(InBehavior) {}
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag BehaviorTag;

};
