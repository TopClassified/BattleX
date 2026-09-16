#include "BXBProxyJump.h"

#include "Movement/BXCharacterMovementComponent.h"



// 开始/停止事件由UBXBehaviorComponent管线统一广播(BXEvent.Behavior.Enter/Exit)
bool UBXBProxyJump::NativeEnableProxy()
{
	if (AActor* ProxyOwner = GetOwner())
	{
		if (UBXCharacterMovementComponent* MovementComp = ProxyOwner->FindComponentByClass<UBXCharacterMovementComponent>())
		{
			MovementComp->ClearJumpBlocked(TEXT("ProxyJump"));
		}
	}

	return true;
}

bool UBXBProxyJump::NativeDisableProxy()
{
	if (AActor* ProxyOwner = GetOwner())
	{
		if (UBXCharacterMovementComponent* MovementComp = ProxyOwner->FindComponentByClass<UBXCharacterMovementComponent>())
		{
			MovementComp->AddJumpBlocked(true, TEXT("ProxyJump"));
		}
	}

	return true;
}

bool UBXBProxyJump::NativeStartBehavior(const FInstancedStruct& InParameter)
{
	return true;
}

bool UBXBProxyJump::NativeCheckStartBehavior(const FInstancedStruct& InParameter)
{
	return true;
}
