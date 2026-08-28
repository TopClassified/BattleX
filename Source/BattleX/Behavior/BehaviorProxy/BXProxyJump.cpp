#include "BXProxyJump.h"

#include "Movement/BXCharacterMovementComponent.h"



// 开始/停止事件由UBXBehaviorComponent管线统一广播(BXEvent.Behavior.Enter/Exit)
bool UBXProxyJump::NativeEnableProxy()
{
	if (AActor* ProxyOwner = GetOwner())
	{
		if (UBXCharacterMovementComponent* MovementComp = ProxyOwner->FindComponentByClass<UBXCharacterMovementComponent>())
		{
			MovementComp->SetBehaviorJumpBlocked(false);
		}
	}

	return true;
}

bool UBXProxyJump::NativeDisableProxy()
{
	if (AActor* ProxyOwner = GetOwner())
	{
		if (UBXCharacterMovementComponent* MovementComp = ProxyOwner->FindComponentByClass<UBXCharacterMovementComponent>())
		{
			MovementComp->SetBehaviorJumpBlocked(true);
		}
	}

	return true;
}

bool UBXProxyJump::NativeStartBehavior(const FInstancedStruct& InParameter)
{
	return true;
}

bool UBXProxyJump::NativeCheckStartBehavior(const FInstancedStruct& InParameter)
{
	return true;
}
