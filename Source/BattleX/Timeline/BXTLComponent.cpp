#include "BXTLComponent.h"
#include "BXTLManager.h"
#include "BXEventManager.h"
#include "BXGameplayTags.h"



#pragma region Important
UBXTLComponent::UBXTLComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UBXTLComponent::BeginPlay()
{
	SetComponentTickEnabled(false);

	Super::BeginPlay();	
}

void UBXTLComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UBXTLManager* BXMgr = UBXTLManager::Get(GetOwner()))
	{
		for (int32 i = 0; i < TimelineRunTimeDataIDs.Num(); i++)
		{
			BXMgr->StopTimeline(TimelineRunTimeDataIDs[i], EBXTLFinishReason::FR_Interrupt);
		}
	}

	Super::EndPlay(EndPlayReason);
}

#pragma endregion Important



#pragma region Timeline
int64 UBXTLComponent::PlayTimeline(UBXTLAsset* InAsset, UPARAM(ref) FBXTLPlayContext& InContext)
{
	UBXTLManager* BXMgr = UBXTLManager::Get(GetOwner());
	if (!IsValid(BXMgr))
	{
		return 0;
	}

	int64 TimelineID = BXMgr->PlayTimeline(InAsset, GetOwner(), InContext);

	TimelineRunTimeDataIDs.Add(TimelineID);

	// 广播时间轴开始事件
	if (UBXEventManager* BXEMgr = UBXEventManager::Get(GetOwner()))
	{
		FBXTLEventParameter Parameter(TimelineID, EBXTLFinishReason::FR_TMax);
		BXEMgr->BroadcastSingleEvent<FBXTLEventParameter>(BXGameplayTags::BXEvent_TimelineStarted, GetOwner(), Parameter);
	}

	return TimelineID;
}

void UBXTLComponent::StopTimeline(int64 InID, EBXTLFinishReason InReason)
{
	UBXTLManager* BXMgr = UBXTLManager::Get(GetOwner());
	if (!IsValid(BXMgr))
	{
		return;
	}

	BXMgr->StopTimeline(InID, InReason);
}

void UBXTLComponent::ReceiveTimelineWillFinish(int64 InID, EBXTLFinishReason InReason)
{
	ScriptReceiveTimelineWillFinish(InID, InReason);

	// 广播时间轴即将结束事件
	if (UBXEventManager* BXEMgr = UBXEventManager::Get(GetOwner()))
	{
		FBXTLEventParameter Parameter(InID, InReason);
		BXEMgr->BroadcastSingleEvent<FBXTLEventParameter>(BXGameplayTags::BXEvent_TimelineClosing, GetOwner(), Parameter);
	}

	// 从容器中移除
	TimelineRunTimeDataIDs.RemoveSwap(InID);
}

bool UBXTLComponent::IsTimelineRunning(UBXTLAsset* InAsset)
{
	UBXTLManager* BXMgr = UBXTLManager::Get(GetOwner());
	if (!IsValid(BXMgr))
	{
		return false;
	}

	for (TArray<int64>::TIterator It(TimelineRunTimeDataIDs); It; ++It)
	{
		if (FBXTLRunTimeData* RTData = BXMgr->GetTimelineRunTimeDataByID(*It))
		{
			if (RTData->Timeline == InAsset)
			{
				return true;
			}
		}
	}

	return false;
}

bool UBXTLComponent::IsTimelineRunningByID(int64 InID)
{
	UBXTLManager* BXMgr = UBXTLManager::Get(GetOwner());
	if (!IsValid(BXMgr))
	{
		return false;
	}

	if (TimelineRunTimeDataIDs.Contains(InID) && BXMgr->GetTimelineRunTimeDataByID(InID))
	{
		return true;
	}

	return false;
}

#pragma endregion Timeline
