#include "BXBProxyMove.h"

#include "Movement/BXCharacterMovementComponent.h"



// 开始/停止事件由UBXBehaviorComponent管线统一广播(BXEvent.Behavior.Enter/Exit)
// 代理只负责基层组件的执行逻辑,不再手动发事件
bool UBXBProxyMove::NativeEnableProxy()
{
	if (AActor* ProxyOwner = GetOwner())
	{
		if (UBXCharacterMovementComponent* MovementComp = ProxyOwner->FindComponentByClass<UBXCharacterMovementComponent>())
		{
			MovementComp->ClearMoveBlocked(TEXT("ProxyMove"));
		}
	}

	return true;
}

bool UBXBProxyMove::NativeDisableProxy()
{
	if (AActor* ProxyOwner = GetOwner())
	{
		if (UBXCharacterMovementComponent* MovementComp = ProxyOwner->FindComponentByClass<UBXCharacterMovementComponent>())
		{
			MovementComp->AddMoveBlocked(true, TEXT("ProxyMove"));
		}
	}

	return true;
}

bool UBXBProxyMove::NativeStartBehavior(const FInstancedStruct& InParameter)
{
	return true;
}

bool UBXBProxyMove::NativeStopBehavior()
{
	return true;
}

bool UBXBProxyMove::NativeCheckStartBehavior(const FInstancedStruct& InParameter)
{
	return true;
}
