#include "BXSProxyExecution.h"

#include "Movement/BXCharacterMovementComponent.h"



bool UBXSProxyExecution::NativeStartState()
{
	if (AActor* ProxyOwner = GetOwner())
	{
		if (UBXCharacterMovementComponent* MovementComp = ProxyOwner->FindComponentByClass<UBXCharacterMovementComponent>())
		{
			// 处决演出定身:残留速度清零(Forbid链的封锁是挡新启动+摩擦滑停,给不了瞬停)
			MovementComp->Velocity = FVector::ZeroVector;
		}
	}

	return true;
}
