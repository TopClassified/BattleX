#include "BXProxyRotate.h"

#include "Movement/BXCharacterMovementComponent.h"



// 开始/停止事件由UBXBehaviorComponent管线统一广播(BXEvent.Behavior.Enter/Exit)
bool UBXProxyRotate::NativeEnableProxy()
{
	if (AActor* ProxyOwner = GetOwner())
	{
		if (UBXCharacterMovementComponent* MovementComp = ProxyOwner->FindComponentByClass<UBXCharacterMovementComponent>())
		{
			MovementComp->SetBehaviorRotateBlocked(false);
		}
	}

	return true;
}

bool UBXProxyRotate::NativeDisableProxy()
{
	if (AActor* ProxyOwner = GetOwner())
	{
		if (UBXCharacterMovementComponent* MovementComp = ProxyOwner->FindComponentByClass<UBXCharacterMovementComponent>())
		{
			MovementComp->SetBehaviorRotateBlocked(true);
		}
	}

	return true;
}

bool UBXProxyRotate::NativeStartBehavior(const FInstancedStruct& InParameter)
{
	return true;
}

bool UBXProxyRotate::NativeStopBehavior()
{
	return true;
}

bool UBXProxyRotate::NativeCheckStartBehavior(const FInstancedStruct& InParameter)
{
	return true;
}
