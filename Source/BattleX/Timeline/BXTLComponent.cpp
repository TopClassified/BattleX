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
		// 每次取末尾元素停止:StopTimeline会同步回调ReceiveTimelineWillFinish对数组RemoveSwap(元素左移),
		// 索引遍历(正序跳过元素/倒序回调联动移除多条时越界)均不安全,末尾取值对任意移除模式健壮
		while (TimelineRunTimeDataIDs.Num() > 0)
		{
			const int64 StoppedTimelineID = TimelineRunTimeDataIDs.Last();
			BXMgr->StopTimeline(StoppedTimelineID, EBXTLFinishReason::FR_Interrupt);

			// StopTimeline找不到数据时不触发回调移除,手动弹出防止死循环
			if (TimelineRunTimeDataIDs.Num() > 0 && TimelineRunTimeDataIDs.Last() == StoppedTimelineID)
			{
				TimelineRunTimeDataIDs.Pop();
			}
		}
	}

	TimelineRunTimeDataIDs.Empty();

	Super::EndPlay(EndPlayReason);
}

#pragma endregion Important



#pragma region Timeline
int64 UBXTLComponent::PlayTimeline(UBXTLAsset* InAsset, UPARAM(ref) FBXTLPlayContext& InContext)
{
	UBXTLManager* BXMgr = UBXTLManager::Get(GetOwner());
	if (!IsValid(BXMgr))
	{
		// 组件无Owner时Get返回null走到此分支,GetOwner()为空,原日志直接解引用崩溃
		UE_LOG(BXMGR_Timeline, Warning, TEXT("UBXTLComponent::PlayTimeline failed: BXTLManager is null, Owner=%s Asset=%s."), *GetNameSafe(GetOwner()), InAsset ? *InAsset->GetName() : TEXT("null"));
		return 0;
	}

	int64 TimelineID = BXMgr->PlayTimeline(InAsset, GetOwner(), InContext);

	// 播放失败返回0(更新中拒绝/资产无效),禁止入列否则形成永久垃圾条目
	if (TimelineID > 0)
	{
		TimelineRunTimeDataIDs.Add(TimelineID);
	}

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
