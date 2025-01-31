#include "BXBADefaultJump.h"

#include "BXEventStructs.h"
#include "BXEventManager.h"
#include "BXGameplayTags.h"



bool UBXBADefaultJump::NativeStartBehavior(const FInstancedStruct& InParameter)
{
	UBXEventManager* BXEMgr = UBXEventManager::Get(GetOwner());
	if (!IsValid(BXEMgr))
	{
		return false;
	}

	// 广播开始事件
	FBXEventBehaviorDefault Parameter(true);
	BXEMgr->BroadcastSingleEvent<FBXEventBehaviorDefault>(BXGameplayTags::BXEvent_Behavior_Jump, GetOwner(), Parameter);
	
	return true;
}

bool UBXBADefaultJump::NativeCheckStartBehavior(const FInstancedStruct& InParameter)
{
	return true;
}
