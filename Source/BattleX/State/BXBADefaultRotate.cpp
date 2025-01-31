#include "BXBADefaultRotate.h"

#include "BXEventStructs.h"
#include "BXEventManager.h"
#include "BXGameplayTags.h"



bool UBXBADefaultRotate::NativeStartBehavior(const FInstancedStruct& InParameter)
{
	UBXEventManager* BXEMgr = UBXEventManager::Get(GetOwner());
	if (!IsValid(BXEMgr))
	{
		return false;
	}

	// 广播开始事件
	FBXEventBehaviorDefault Parameter(true);
	BXEMgr->BroadcastSingleEvent<FBXEventBehaviorDefault>(BXGameplayTags::BXEvent_Behavior_Rotate, GetOwner(), Parameter);
	
	return true;
}

bool UBXBADefaultRotate::NativeStopBehavior(const FInstancedStruct& InParameter)
{
	UBXEventManager* BXEMgr = UBXEventManager::Get(GetOwner());
	if (!IsValid(BXEMgr))
	{
		return false;
	}

	// 广播结束事件
	FBXEventBehaviorDefault Parameter(false);
	BXEMgr->BroadcastSingleEvent<FBXEventBehaviorDefault>(BXGameplayTags::BXEvent_Behavior_Rotate, GetOwner(), Parameter);
	
	return true;
}

bool UBXBADefaultRotate::NativeCheckStartBehavior(const FInstancedStruct& InParameter)
{
	return true;
}
