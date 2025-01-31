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



// 行为事件默认参数
USTRUCT(BlueprintType)
struct FBXEventBehaviorDefault
{
	GENERATED_USTRUCT_BODY()

public:
	FBXEventBehaviorDefault() {}
	FBXEventBehaviorDefault(bool InActive): bActive(InActive) {}
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bActive = false;

};
