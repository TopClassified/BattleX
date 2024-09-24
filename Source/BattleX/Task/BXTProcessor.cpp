#include "BXTProcessor.h"

#include "BXSettings.h"
#include "BXTLAsset.h"
#include "BXTLStructs.h"
#include "BXTask.h" 



#pragma region Important
UWorld* UBXTProcessor::GetWorld() const
{
	if (!GetOuter())
	{
		return nullptr;
	}

	return GetOuter()->GetWorld();
}

bool UBXTProcessor::StartTask(UPARAM(ref) FBXTLRunTimeData& InOutRTData, UPARAM(ref) FBXTLSectionRTData& InOutRTSData, UPARAM(ref) FBXTLTaskRTData& InOutRTTData, UBXTask* InTask)
{
	bool Result = false;

	if ((ExecuteFunctions & 1) > 0)
	{
		Result &= Start(InOutRTData, InOutRTSData, InOutRTTData, InTask);
	}

	if ((ExecuteFunctions & 2) > 0)
	{
		Result &= ScriptStart(InOutRTData, InOutRTSData, InOutRTTData, InTask);
	}

	return Result;
}

bool UBXTProcessor::UpdateTask(UPARAM(ref) FBXTLRunTimeData& InOutRTData, UPARAM(ref) FBXTLSectionRTData& InOutRTSData, UPARAM(ref) FBXTLTaskRTData& InOutRTTData, UBXTask* InTask, float InDeltaTime, float InTimeRate)
{
	bool Result = false;

	if ((ExecuteFunctions & 4) > 0)
	{
		Result &= Update(InOutRTData, InOutRTSData, InOutRTTData, InTask, InDeltaTime, InTimeRate);
	}

	if ((ExecuteFunctions & 8) > 0)
	{
		Result &= ScriptUpdate(InOutRTData, InOutRTSData, InOutRTTData, InTask, InDeltaTime, InTimeRate);
	}

	return Result;
}

bool UBXTProcessor::EndTask(UPARAM(ref) FBXTLRunTimeData& InOutRTData, UPARAM(ref) FBXTLSectionRTData& InOutRTSData, UPARAM(ref) FBXTLTaskRTData& InOutRTTData, UBXTask* InTask, EBXTLFinishReason InReason)
{
	bool Result = false;

	if ((ExecuteFunctions & 16) > 0)
	{
		Result &= End(InOutRTData, InOutRTSData, InOutRTTData, InTask, InReason);
	}

	if ((ExecuteFunctions & 32) > 0)
	{
		Result &= ScriptEnd(InOutRTData, InOutRTSData, InOutRTTData, InTask, InReason);
	}

	return Result;
}

#pragma endregion Important



#pragma region GlobalAPI
bool UBXTProcessor::IsTaskCompleted(UBXTask* InTask, const FBXTLTaskRTData& InTaskData, EBXTLFinishReason& OutReason)
{
	OutReason = EBXTLFinishReason::FR_Interrupt;
	if (!InTask)
	{
		return true;
	}

	if (InTaskData.bEarlyFinish)
	{
		return true;
	}

	OutReason = EBXTLFinishReason::FR_EndOfLife;
	switch (InTask->LifeType)
	{
	case EBXTLifeType::L_Instant:
		return true;
	case EBXTLifeType::L_Duration:
		return InTaskData.RunTime >= InTask->Duration;
	case EBXTLifeType::L_Timeline:
		return false;
	case EBXTLifeType::L_DurationTimeline:
		return InTaskData.RunTime >= InTask->Duration;
	}

	OutReason = EBXTLFinishReason::FR_Interrupt;
	return true;
}

bool UBXTProcessor::AddPendingTask(UPARAM(ref) FBXTLRunTimeData& InOutRTData, UPARAM(ref) FBXTLSectionRTData& InOutRTSData, UPARAM(ref) FBXTLTaskRTData& InOutRTTData, const FName& InEventName)
{
	const UBXSettings* Settings = GetDefault<UBXSettings>();
	if (!Settings)
	{
		return false;
	}

	UBXTLAsset* Asset = InOutRTData.StaticData;
	if (!Asset)
	{
		return false;
	}

	if (!Asset->Sections.IsValidIndex(InOutRTSData.Index))
	{
		return false;
	}

	const FBXTLSection& Section = Asset->Sections[InOutRTSData.Index];
	if (!Section.TaskList.IsValidIndex(InOutRTTData.Index))
	{
		return false;
	}

	UBXTask* Task = Section.TaskList[InOutRTTData.Index];
	if (!Task)
	{
		return false;
	}

	FBXTEvent* EventPointer = Task->Events.Find(InEventName);
	if (!EventPointer)
	{
		return false;
	}

	// 需要广播，则添加到广播列表
	if (EventPointer->bMulticast)
	{
		InOutRTSData.BroadcastTasks.Add(FBXTLBroadcastTaskInfo(InOutRTSData.Index * 1000 + InOutRTTData.Index, InEventName));
	}

	TArray<int32>& List = InOutRTSData.FramePendingTasks;
	for (TMap<TSoftObjectPtr<UBXTask>, float>::TIterator It(EventPointer->Event); It; ++It)
	{
		// 任务全量索引
		int32 FullIndex = UBXFunctionLibrary::GetSoftTaskFullIndex(Asset, It->Key);
		if (FullIndex < 0)
		{
			continue;
		}

		// 任务局部索引
		int32 LocalIndex = FullIndex % 1000;

		// 延迟触发的任务，添加到待处理列表
		if (It->Value > 0.0f)
		{
			InOutRTSData.PendingTasks.Add(FBXTLPendingTaskInfo(LocalIndex, InOutRTSData.RunTime + It->Value));

			continue;
		}

		// 如果开启了帧内待处理列表，则添加到该列表
		if (List.Num() > 0)
		{
			if (!List.Contains(LocalIndex) && Settings->FramePendingTaskLimit > List.Num())
			{
				List.Add(LocalIndex);
			}
		}
		else
		{
			InOutRTSData.PendingTasks.Add(FBXTLPendingTaskInfo(LocalIndex, InOutRTSData.RunTime));
		}
	}

	return true;
}

#pragma endregion GlobalAPI
