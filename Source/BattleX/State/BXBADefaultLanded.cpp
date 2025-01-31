#include "BXBADefaultLanded.h"

#include "BXEventManager.h"
#include "BXGameplayTags.h"



bool UBXBADefaultLanded::NativeStartBehavior(const FInstancedStruct& InParameter)
{
	UBXEventManager* BXEMgr = UBXEventManager::Get(GetOwner());
	if (!IsValid(BXEMgr))
	{
		return false;
	}

	// 广播开始事件
	FHitResult EventParameter = InParameter.Get<FHitResult>();
	BXEMgr->BroadcastSingleEvent<FHitResult>(BXGameplayTags::BXEvent_Behavior_Landed, GetOwner(), EventParameter);
	
	return true;
}

bool UBXBADefaultLanded::NativeCheckStartBehavior(const FInstancedStruct& InParameter)
{
	return true;
}
