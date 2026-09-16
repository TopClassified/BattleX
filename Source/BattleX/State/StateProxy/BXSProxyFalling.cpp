#include "BXSProxyFalling.h"

#include "Movement/BXCharacterMovementComponent.h"



bool UBXSProxyFalling::NativeStartState()
{
	if (AActor* ProxyOwner = GetOwner())
	{
		if (UBXCharacterMovementComponent* MovementComp = ProxyOwner->FindComponentByClass<UBXCharacterMovementComponent>())
		{
			// 仅地面系模式需要切滞空(已在Falling/Flying则不动,防重入覆盖悬浮等特殊模式)
			if (MovementComp->MovementMode != MOVE_Falling && MovementComp->MovementMode != MOVE_Flying)
			{
				MovementComp->SetMovementMode(EMovementMode::MOVE_Falling);
			}
		}
	}

	return true;
}
