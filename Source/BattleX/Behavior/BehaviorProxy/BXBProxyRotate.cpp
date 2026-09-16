#include "BXBProxyRotate.h"

#include "Movement/BXCharacterMovementComponent.h"



// 开始/停止事件由UBXBehaviorComponent管线统一广播(BXEvent.Behavior.Enter/Exit)
bool UBXBProxyRotate::NativeEnableProxy()
{
	if (AActor* ProxyOwner = GetOwner())
	{
		if (UBXCharacterMovementComponent* MovementComp = ProxyOwner->FindComponentByClass<UBXCharacterMovementComponent>())
		{
			MovementComp->ClearRotateBlocked(TEXT("ProxyRotate"));
		}
	}

	return true;
}

bool UBXBProxyRotate::NativeDisableProxy()
{
	if (AActor* ProxyOwner = GetOwner())
	{
		if (UBXCharacterMovementComponent* MovementComp = ProxyOwner->FindComponentByClass<UBXCharacterMovementComponent>())
		{
			MovementComp->AddRotateBlocked(true, TEXT("ProxyRotate"));
		}
	}

	return true;
}

bool UBXBProxyRotate::NativeStartBehavior(const FInstancedStruct& InParameter)
{
	return true;
}

bool UBXBProxyRotate::NativeStopBehavior()
{
	return true;
}

bool UBXBProxyRotate::NativeCheckStartBehavior(const FInstancedStruct& InParameter)
{
	return true;
}
