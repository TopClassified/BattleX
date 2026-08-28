#include "BXProxyMove.h"

#include "Movement/BXCharacterMovementComponent.h"



// 开始/停止事件由UBXBehaviorComponent管线统一广播(BXEvent.Behavior.Enter/Exit)
// 代理只负责基层组件的执行逻辑,不再手动发事件
bool UBXProxyMove::NativeEnableProxy()
{
	if (AActor* ProxyOwner = GetOwner())
	{
		if (UBXCharacterMovementComponent* MovementComp = ProxyOwner->FindComponentByClass<UBXCharacterMovementComponent>())
		{
			MovementComp->SetBehaviorMoveBlocked(false);
		}
	}

	return true;
}

bool UBXProxyMove::NativeDisableProxy()
{
	if (AActor* ProxyOwner = GetOwner())
	{
		if (UBXCharacterMovementComponent* MovementComp = ProxyOwner->FindComponentByClass<UBXCharacterMovementComponent>())
		{
			MovementComp->SetBehaviorMoveBlocked(true);
		}
	}

	return true;
}

bool UBXProxyMove::NativeStartBehavior(const FInstancedStruct& InParameter)
{
	return true;
}

bool UBXProxyMove::NativeStopBehavior(const FInstancedStruct& InParameter)
{
	return true;
}

bool UBXProxyMove::NativeCheckStartBehavior(const FInstancedStruct& InParameter)
{
	return true;
}
