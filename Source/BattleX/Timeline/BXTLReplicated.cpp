#include "BXTLReplicated.h"

#include "BXTask.h"
#include "BXTLAsset.h"



#pragma region Projection
void BXToTLRunTimeProjection(const FBXTLRunTimeData& InSource, FBXTLRunTimeDataReplicated& OutTarget, bool bSimulatedOnly)
{
	OutTarget.TimelineID = InSource.TimelineID;
	OutTarget.ID = InSource.ID;
	OutTarget.Owner = InSource.Owner;
	OutTarget.Instigator = InSource.Instigator;
	OutTarget.Triggerer = InSource.Triggerer;
	OutTarget.LockParts = InSource.LockParts;
	OutTarget.RunTime = InSource.RunTime;
	OutTarget.RunRate = InSource.RunRate;
	OutTarget.DynamicDatas = InSource.DynamicDatas;

	OutTarget.RunningSections.Reset();
	for (const FBXTLSectionRTData& SectionRT : InSource.RunningSections)
	{
		FBXTLSectionRTDataReplicated& SectionProjection = OutTarget.RunningSections.AddDefaulted_GetRef();
		SectionProjection.Index = SectionRT.Index;
		SectionProjection.KeyFrameIndex = SectionRT.KeyFrameIndex;
		SectionProjection.RunTime = SectionRT.RunTime;
		SectionProjection.LoopCount = SectionRT.LoopCount;
		SectionProjection.ForceJumpSection = SectionRT.ForceJumpSection;
		SectionProjection.bEarlyFinish = SectionRT.bEarlyFinish;

		UBXTLAsset* Asset = InSource.Timeline;
		for (const FBXTLTaskRTData& TaskRT : SectionRT.RunningTasks)
		{
			// 按NetTypes过滤:仅保留重建端(模拟端)会执行的Task,避免权威/自主端Task在远端Update
			if (bSimulatedOnly && Asset && Asset->Sections.IsValidIndex(SectionRT.Index) && Asset->Sections[SectionRT.Index].TaskList.IsValidIndex(TaskRT.Index))
			{
				UBXTask* Task = Asset->Sections[SectionRT.Index].TaskList[TaskRT.Index];
				const int32 SimulatedMask = 1 << 2;
				if (!Task || (Task->NetTypes & SimulatedMask) == 0)
				{
					continue;
				}
			}

			FBXTLTaskRTDataReplicated& TaskProjection = SectionProjection.RunningTasks.AddDefaulted_GetRef();
			TaskProjection.Index = TaskRT.Index;
			TaskProjection.ParentScope = TaskRT.ParentScope;
			TaskProjection.RunTime = TaskRT.RunTime;
			TaskProjection.NextTick = TaskRT.NextTick;
			TaskProjection.bEarlyFinish = TaskRT.bEarlyFinish;
			TaskProjection.DynamicData = TaskRT.DynamicData;
		}
	}
}

void BXFromTLRunTimeProjection(const FBXTLRunTimeDataReplicated& InSource, UBXTLAsset* InAsset, FBXTLRunTimeData& OutTarget)
{
	OutTarget.Reset();

	OutTarget.Timeline = InAsset;
	OutTarget.TimelineID = InSource.TimelineID;
	OutTarget.ID = InSource.ID;
	OutTarget.Owner = InSource.Owner;
	OutTarget.Instigator = InSource.Instigator;
	OutTarget.Triggerer = InSource.Triggerer;
	OutTarget.LockParts = InSource.LockParts;
	OutTarget.RunTime = InSource.RunTime;
	OutTarget.RunRate = InSource.RunRate;
	OutTarget.DynamicDatas = InSource.DynamicDatas;

	for (const FBXTLSectionRTDataReplicated& SectionProjection : InSource.RunningSections)
	{
		if (!InAsset || !InAsset->Sections.IsValidIndex(SectionProjection.Index))
		{
			continue;
		}

		FBXTLSectionRTData& SectionRT = OutTarget.RunningSections.AddDefaulted_GetRef();
		SectionRT.Index = SectionProjection.Index;
		SectionRT.KeyFrameIndex = SectionProjection.KeyFrameIndex;
		SectionRT.RunTime = SectionProjection.RunTime;
		SectionRT.LoopCount = SectionProjection.LoopCount;
		SectionRT.ForceJumpSection = SectionProjection.ForceJumpSection;
		SectionRT.bEarlyFinish = SectionProjection.bEarlyFinish;

		for (const FBXTLTaskRTDataReplicated& TaskProjection : SectionProjection.RunningTasks)
		{
			if (!InAsset->Sections[SectionProjection.Index].TaskList.IsValidIndex(TaskProjection.Index))
			{
				continue;
			}

			FBXTLTaskRTData& TaskRT = SectionRT.RunningTasks.AddDefaulted_GetRef();
			TaskRT.Task = InAsset->Sections[SectionProjection.Index].TaskList[TaskProjection.Index];
			TaskRT.Index = TaskProjection.Index;
			TaskRT.ParentScope = TaskProjection.ParentScope;
			TaskRT.RunTime = TaskProjection.RunTime;
			TaskRT.NextTick = TaskProjection.NextTick;
			TaskRT.bEarlyFinish = TaskProjection.bEarlyFinish;
			TaskRT.DynamicData = TaskProjection.DynamicData;
		}
	}
}

#pragma endregion Projection
