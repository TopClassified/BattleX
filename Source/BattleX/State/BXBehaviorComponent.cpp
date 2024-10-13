#include "BXBehaviorComponent.h"

#include "BXCharacterMovementComponent.h"
#include "BXFunctionLibrary.h"


#pragma region Important
UBXBehaviorComponent::UBXBehaviorComponent()
{

}

void UBXBehaviorComponent::BeginPlay()
{
	// 监听移动相关的事件
	if (UBXCharacterMovementComponent* MoveComp = GetOwner()->FindComponentByClass<UBXCharacterMovementComponent>())
	{
		MoveComp->DoJumpEvent.AddDynamic(this, &UBXBehaviorComponent::ReceiveDoJump);
		MoveComp->StartProactiveMoveEvent.AddDynamic(this, &UBXBehaviorComponent::ReceiveStartProactiveMoving);
		MoveComp->StopProactiveMoveEvent.AddDynamic(this, &UBXBehaviorComponent::ReceiveStopProactiveMoving);
		MoveComp->StartProactiveRotateEvent.AddDynamic(this, &UBXBehaviorComponent::ReceiveStartProactiveRotating);
		MoveComp->StopProactiveRotateEvent.AddDynamic(this, &UBXBehaviorComponent::ReceiveStopProactiveRotating);
	}

	Super::BeginPlay();
}

void UBXBehaviorComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UBXCharacterMovementComponent* MoveComp = GetOwner()->FindComponentByClass<UBXCharacterMovementComponent>())
	{
		MoveComp->DoJumpEvent.RemoveAll(this);
		MoveComp->StartProactiveMoveEvent.RemoveAll(this);
		MoveComp->StopProactiveMoveEvent.RemoveAll(this);
		MoveComp->StartProactiveRotateEvent.RemoveAll(this);
		MoveComp->StopProactiveRotateEvent.RemoveAll(this);
	}
	
	Super::EndPlay(EndPlayReason);
}

#pragma endregion Important



#pragma region MovementCallback
void UBXBehaviorComponent::ReceiveDoJump()
{
	
}
	
void UBXBehaviorComponent::ReceiveStartProactiveMoving()
{
	PerformingBehaviors.AddTag(FGameplayTag::RequestGameplayTag(TEXT("BXBehavior.ProactiveMoving")));
}

void UBXBehaviorComponent::ReceiveStopProactiveMoving()
{
	PerformingBehaviors.RemoveTag(FGameplayTag::RequestGameplayTag(TEXT("BXBehavior.ProactiveMoving")));
}
	
void UBXBehaviorComponent::ReceiveStartProactiveRotating()
{
	PerformingBehaviors.AddTag(FGameplayTag::RequestGameplayTag(TEXT("BXBehavior.ProactiveRotating")));
}
    	
void UBXBehaviorComponent::ReceiveStopProactiveRotating()
{
	PerformingBehaviors.RemoveTag(FGameplayTag::RequestGameplayTag(TEXT("BXBehavior.ProactiveRotating")));
}
	
#pragma endregion MovementCallback



#pragma region API
bool UBXBehaviorComponent::IsCurrentlyPerformingBehavior(AActor* InActor, const FGameplayTag& InBehaviorTag)
{
	if (!IsValid(InActor))
	{
		return false;
	}

	if (UBXBehaviorComponent* BXBComp = InActor->FindComponentByClass<UBXBehaviorComponent>())
	{
		return BXBComp->PerformingBehaviors.HasTag(InBehaviorTag);
	}
	
	return false;
}

int64 UBXBehaviorComponent::GetBehaviorPermission(AActor* InActor, const FGameplayTag& InBehaviorTag, int32 InPriority)
{
	int64 Result = -1;
	if (!IsValid(InActor))
	{
		return Result;
	}

	UBXBehaviorComponent* BComp = InActor->FindComponentByClass<UBXBehaviorComponent>();
	if (!IsValid(BComp))
	{
		return UBXFunctionLibrary::GetUniqueID();
	}
	
	if (FInt64Vector2* FindResult = BComp->BehaviorPermissionMap.Find(InBehaviorTag))
	{
		if (FindResult->X <= InPriority)
		{
			Result = UBXFunctionLibrary::GetUniqueID();
			FindResult->X = InPriority;
			FindResult->Y = Result;
		}
	}
	else
	{
		Result = UBXFunctionLibrary::GetUniqueID();
		BComp->BehaviorPermissionMap.Add(InBehaviorTag, FInt64Vector2(InPriority, Result));
	}

	return Result;
}

void UBXBehaviorComponent::RevokeBehaviorPermission(AActor* InActor, const FGameplayTag& InBehaviorTag, int64 InPermission)
{
	if (!IsValid(InActor))
	{
		return;
	}

	UBXBehaviorComponent* BComp = InActor->FindComponentByClass<UBXBehaviorComponent>();
	if (!IsValid(BComp))
	{
		return;
	}
	
	if (FInt64Vector2* FindResult = BComp->BehaviorPermissionMap.Find(InBehaviorTag))
	{
		if (FindResult->Y == InPermission)
		{
			BComp->BehaviorPermissionMap.Remove(InBehaviorTag);
		}
	}
}
	
#pragma endregion API
