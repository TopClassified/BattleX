#include "BXSProxyFloating.h"

#include "Movement/BXCharacterMovementComponent.h"



bool UBXSProxyFloating::NativeStartState()
{
	if (AActor* ProxyOwner = GetOwner())
	{
		if (UBXCharacterMovementComponent* MovementComp = ProxyOwner->FindComponentByClass<UBXCharacterMovementComponent>())
		{
			// 残留速度清零:悬浮位置锚定在进入时刻,否则挑空的上升速度会带着角色继续漂移
			MovementComp->Velocity = FVector::ZeroVector;
			MovementComp->SetMovementMode(EMovementMode::MOVE_Flying);
		}
	}

	return true;
}

bool UBXSProxyFloating::NativeStopState()
{
	if (AActor* ProxyOwner = GetOwner())
	{
		if (UBXCharacterMovementComponent* MovementComp = ProxyOwner->FindComponentByClass<UBXCharacterMovementComponent>())
		{
			// 仅仍在Flying时还原(状态退出时可能已被外部切模式,不强踩);落地收归移动组件
			if (MovementComp->MovementMode == MOVE_Flying)
			{
				MovementComp->SetMovementMode(EMovementMode::MOVE_Falling);
			}
		}
	}

	return true;
}
