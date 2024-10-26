#include "BXTimelineComponent.h" 



#pragma region Important
UBXTimelineComponent::UBXTimelineComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UBXTimelineComponent::BeginPlay()
{
	SetComponentTickEnabled(false);

	Super::BeginPlay();	
}

void UBXTimelineComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UBXManager* BXMgr = UBXManager::Get(GetOwner()))
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
int64 UBXTimelineComponent::PlayTimeline(UBXTLAsset* InAsset, UPARAM(ref) FBXTLPlayContext& InContext)
{
	UBXManager* BXMgr = UBXManager::Get(GetOwner());
	if (!IsValid(BXMgr))
	{
		return 0;
	}

	int64 TimelineID = BXMgr->PlayTimeline(InAsset, GetOwner(), InContext);

	TimelineRunTimeDataIDs.Add(TimelineID);

	// 广播时间轴开始事件
	TimelineStarted.Broadcast(TimelineID);

	return TimelineID;
}

void UBXTimelineComponent::StopTimeline(int64 InID, EBXTLFinishReason InReason)
{
	UBXManager* BXMgr = UBXManager::Get(GetOwner());
	if (!IsValid(BXMgr))
	{
		return;
	}

	BXMgr->StopTimeline(InID, InReason);
}

void UBXTimelineComponent::ReceiveTimelineWillFinish(int64 InID, EBXTLFinishReason InReason)
{
	ScriptReceiveTimelineWillFinish(InID, InReason);

	// 广播时间轴即将结束事件
	TimelineWillEnd.Broadcast(InID, InReason);

	// 从容器中移除
	TimelineRunTimeDataIDs.RemoveSwap(InID);
}

bool UBXTimelineComponent::IsTimelineRunning(UBXTLAsset* InAsset)
{
	UBXManager* BXMgr = UBXManager::Get(GetOwner());
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

bool UBXTimelineComponent::IsTimelineRunningByID(int64 InID)
{
	UBXManager* BXMgr = UBXManager::Get(GetOwner());
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
