#include "BXBehaviorFunctionLibrary.h"

#include "Behavior/BXBehaviorComponent.h"
#include "State/BXStateComponent.h"



bool UBXBehaviorFunctionLibrary::CheckActiveBehavior(AActor* InTarget, const FGameplayTag& InBehaviorTag)
{
	if (!IsValid(InTarget))
	{
		return false;
	}

	if (UBXBehaviorComponent* BehaviorComp = InTarget->FindComponentByClass<UBXBehaviorComponent>())
	{
		return BehaviorComp->CheckActiveBehavior(InBehaviorTag);
	}

	return false;
}

bool UBXBehaviorFunctionLibrary::IsBehaviorDisabled(AActor* InTarget, const FGameplayTag& InBehaviorTag)
{
	if (!IsValid(InTarget))
	{
		return false;
	}

	if (UBXBehaviorComponent* BehaviorComp = InTarget->FindComponentByClass<UBXBehaviorComponent>())
	{
		return BehaviorComp->IsBehaviorDisabled(InBehaviorTag);
	}

	return false;
}

bool UBXBehaviorFunctionLibrary::CanStartBehavior(AActor* InTarget, const FGameplayTag& InBehaviorTag)
{
	if (!IsValid(InTarget))
	{
		return false;
	}

	if (UBXBehaviorComponent* BehaviorComp = InTarget->FindComponentByClass<UBXBehaviorComponent>())
	{
		FBXBehaviorStartCheck Check;
		return BehaviorComp->CanStartBehavior(InBehaviorTag, Check);
	}

	return false;
}

bool UBXBehaviorFunctionLibrary::IsBehaviorWaived(AActor* InTarget, const FGameplayTag& InBehaviorTag)
{
	if (!IsValid(InTarget))
	{
		return false;
	}

	if (UBXBehaviorComponent* BehaviorComp = InTarget->FindComponentByClass<UBXBehaviorComponent>())
	{
		return BehaviorComp->IsBehaviorWaived(InBehaviorTag);
	}

	return false;
}

bool UBXBehaviorFunctionLibrary::StartBehavior(AActor* InTarget, const FGameplayTag& InBehaviorTag, int64 InSign)
{
	if (!IsValid(InTarget))
	{
		return false;
	}

	if (UBXBehaviorComponent* BehaviorComp = InTarget->FindComponentByClass<UBXBehaviorComponent>())
	{
		return BehaviorComp->StartBehavior(InBehaviorTag, InSign);
	}

	return false;
}

bool UBXBehaviorFunctionLibrary::StopBehavior(AActor* InTarget, const FGameplayTag& InBehaviorTag, int64 InSign)
{
	if (!IsValid(InTarget))
	{
		return false;
	}

	if (UBXBehaviorComponent* BehaviorComp = InTarget->FindComponentByClass<UBXBehaviorComponent>())
	{
		return BehaviorComp->StopBehavior(InBehaviorTag, InSign);
	}

	return false;
}

bool UBXBehaviorFunctionLibrary::CheckStateActive(AActor* InTarget, const FGameplayTag& InStateTag)
{
	if (!IsValid(InTarget))
	{
		return false;
	}

	if (UBXStateComponent* StateComp = InTarget->FindComponentByClass<UBXStateComponent>())
	{
		return StateComp->CheckStateActive(InStateTag);
	}

	return false;
}

float UBXBehaviorFunctionLibrary::GetStateRemainingSeconds(AActor* InTarget, const FGameplayTag& InStateTag)
{
	if (!IsValid(InTarget))
	{
		return -1.0f;
	}

	if (UBXStateComponent* StateComp = InTarget->FindComponentByClass<UBXStateComponent>())
	{
		return StateComp->GetStateRemainingSeconds(InStateTag);
	}

	return -1.0f;
}
