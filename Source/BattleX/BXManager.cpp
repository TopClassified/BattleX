#include "BXManager.h"
#include "AssetRegistry/AssetRegistryModule.h"

#include "BXSubSystem.h"
#include "BXSettings.h"
#include "BXStructs.h"
#include "BXTLAsset.h"
#include "BXSkillAsset.h"
#include "BXTask.h"
#include "BXTProcessor.h"
#include "BXTimelineComponent.h" 



DEFINE_LOG_CATEGORY(BXMGR);



#pragma region Important
UBXManager* UBXManager::Get(UObject* InWorldContext)
{
	UBXManager* Result = nullptr;

	if (InWorldContext)
	{
		if (UWorld* World = InWorldContext->GetWorld())
		{
			if (UGameInstance* GI = World->GetGameInstance())
			{
				if (UBXSubSystem* BXSS = GI->GetSubsystem<UBXSubSystem>())
				{
					Result = BXSS->GetBXManager();
				}
			}

#if WITH_EDITOR
			if (!IsValid(Result))
			{
				for (TObjectIterator<UBXManager> It; It; ++It)
				{
					if (It->GetOuter() == World)
					{
						Result = *It;
						break;
					}
				}
			}
#endif
		}
	}

	return Result;
}

void UBXManager::Initialize()
{
	if (const UBXSettings* Settings = GetDefault<UBXSettings>())
	{
		TimelineTaskProcessorTypeMap.Reset();
		for (TMap<TSoftClassPtr<UObject>, TSoftClassPtr<UObject>>::TConstIterator It(Settings->TaskProcessorMap); It; ++It)
		{
			TimelineTaskProcessorTypeMap.Add(It->Key.LoadSynchronous(), It->Value.LoadSynchronous());
		}

		TimelineTaskTypeMap.Reset();
		for (TMap<TSoftClassPtr<UObject>, TSoftObjectPtr<UScriptStruct>>::TConstIterator It(Settings->TaskCustomDataMap); It; ++It)
		{
			TimelineTaskTypeMap.Add(It->Key.LoadSynchronous(), It->Value.LoadSynchronous());
		}
	}

	// 受击所有的时间轴资源路径
	CollectTimelineAssetPath();

	FWorldDelegates::OnWorldCleanup.AddUObject(this, &UBXManager::OnWorldCleanupStart);
}

void UBXManager::Deinitialize()
{
	FWorldDelegates::OnWorldCleanup.RemoveAll(this);
}

#pragma endregion Important



#pragma region Tick
UWorld* UBXManager::GetTickableGameObjectWorld() const
{
	if (!GetOuter())
	{
		return nullptr;
	}

	if (UGameInstanceSubsystem* GIS = Cast<UGameInstanceSubsystem>(GetOuter()))
	{
		if (UGameInstance* GI = GIS->GetGameInstance())
		{
			return GI->GetWorld();
		}

		return nullptr;
	}

	return GetOuter()->GetWorld();
}

ETickableTickType UBXManager::GetTickableTickType() const
{
	return IsTemplate() ? ETickableTickType::Never : ETickableTickType::Always;
}

bool UBXManager::IsAllowedToTick() const
{
	return IsValid(this) && !IsUnreachable();
}

TStatId UBXManager::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UBXManager, STATGROUP_Tickables);
}

void UBXManager::Tick(float DeltaTime)
{
	// 更新垃圾回收计时器
	GCTimer -= DeltaTime;

	// 更新时间轴运行数据
	UpdateTimeline(DeltaTime);

	// 执行脚本Tick
	ScriptTick(DeltaTime);

	// 垃圾清理
	if (GCTimer <= 0.0f)
	{
		GCTimer = GCInterval;
	}
}

#pragma endregion Tick



#pragma region Asset
TSoftObjectPtr<UBXTLAsset> UBXManager::GetTimelineAssetByID(int32 InID)
{
	if (FBXTimelineAssetInformation* Result = TimelineAssetMap.Find(InID))
	{
		return Result->AssetPath;
	}

	return nullptr;
}

TSoftObjectPtr<UBXTLAsset> UBXManager::GetSkillAssetByID(int32 InID)
{
	if (FBXTimelineAssetInformation* Result = TimelineAssetMap.Find(InID))
	{
		if (Result->AssetType == UBXSkillAsset::StaticClass())
		{
			return Result->AssetPath;
		}
	}

	return nullptr;
}

TArray<int32> UBXManager::GetTimelineAssetIDs()
{
	TArray<int32> IDs;
	TimelineAssetMap.GenerateKeyArray(IDs);

	return IDs;
}

TArray<int32> UBXManager::GetSkillAssetIDs()
{
	TArray<int32> IDs;
	for (TMap<int32, FBXTimelineAssetInformation>::TIterator It(TimelineAssetMap); It; ++It)
	{
		if (It->Value.AssetType == UBXSkillAsset::StaticClass())
		{
			IDs.Add(It->Key);
		}
	}

	return IDs;
}

void UBXManager::CollectTimelineAssetPath()
{
	FARFilter Filter;
	Filter.PackagePaths.Add("/Game");
	Filter.ClassPaths.Add(UBXTLAsset::StaticClass()->GetClassPathName());
	Filter.bRecursivePaths = true;
	Filter.bRecursiveClasses = true;

	TArray<FAssetData> AssetData;
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::GetModuleChecked<FAssetRegistryModule>("AssetRegistry");
	AssetRegistryModule.Get().GetAssets(Filter, AssetData);

	FBXTimelineAssetInformation Information;
	for (int32 i = 0; i < AssetData.Num(); ++i)
	{
		Information.AssetType = AssetData[i].GetClass();
		Information.AssetPath = TSoftObjectPtr<UBXTLAsset>(AssetData[i].ToSoftObjectPath());

		TimelineAssetMap.Add(FCString::Atoi(*AssetData[i].AssetName.ToString()), Information);
	}
}

#pragma endregion Asset



#pragma region Timeline
bool UBXManager::IsUpdatingTimeline() const
{
	return bUpdatingTimeline;
}

int64 UBXManager::PlayTimeline(UBXTLAsset* InAsset, AActor* InOwner, UPARAM(ref) FBXTLPlayContext& InContext)
{
	// 严禁更新时间轴时，播放新的时间轴
	if (bUpdatingTimeline)
	{
		return 0;
	}

	if (!IsValid(InAsset))
	{
		return 0;
	}

	// 创建时间轴运行时数据
	int64 NewID = UBXFunctionLibrary::GetUniqueID();
	FBXTLRunTimeData& NewData = TimelineRTDatas.Add(NewID);
	NewData.ID = NewID;
	NewData.StaticData = InAsset;
	NewData.Owner = InOwner;
	NewData.Instigator = InContext.Instigator;
	NewData.Triggerer = InContext.Triggerer;
	NewData.LockParts.Append(InContext.LockParts);
	NewData.Timestamp = InContext.Timestamp;
	for (TMap<FName, FInstancedStruct>::TIterator It(InContext.InputDatas); It; ++It)
	{
		NewData.DynamicDatas.Add(FBXTLDynamicDataSearchKey(-1, It->Key), It->Value);
	}

	// 开始这些时间片段
	for (int32 i = 0; i < InAsset->StartSectionIndexes.Num(); ++i)
	{
		FBXTLSectionRTData& NewSectionData = NewData.RunningSections.AddDefaulted_GetRef();
		NewSectionData.Index = InAsset->StartSectionIndexes[i];
		NewSectionData.Timestamp = InContext.Timestamp;
	}

	// 立刻更新一次
	InternalUpdateTimeline(NewData, 0.0f);

	// 广播时间轴开始事件
	TimelineStarted.Broadcast(NewID);

	return NewID;
}

void UBXManager::StopTimeline(int64 InID, EBXTLFinishReason InReason)
{
	if (FBXTLRunTimeData* RTData = TimelineRTDatas.Find(InID))
	{
		for (int32 i = 0; i < RTData->RunningSections.Num(); ++i)
		{
			FinishTimelineSection(*RTData, RTData->RunningSections[i], InReason);
		}
	
		// 广播时间轴即将结束事件
		TimelineWillEnd.Broadcast(InID, InReason);

		// 尝试直接调用拥有者的组件函数
		if (IsValid(RTData->Owner))
		{
			if (UBXTimelineComponent* BXTLC = RTData->Owner->FindComponentByClass<UBXTimelineComponent>())
			{
				BXTLC->ReceiveTimelineWillFinish(InID, InReason);
			}
		}

		// 从容器中移除
		TimelineRTDatas.Remove(InID);
	}
}

void UBXManager::UpdateTimeline(float InDeltaTime)
{
	for (TMap<int64, FBXTLRunTimeData>::TIterator It(TimelineRTDatas); It; ++It)
	{
		InternalUpdateTimeline(It->Value, InDeltaTime);

		// 检查是否要结束这个时间轴
		bool bNeedFinish = It->Value.bEarlyFinish;
		if (!bNeedFinish)
		{
			bNeedFinish = true;
			for (int32 j = 0; j < It->Value.RunningSections.Num(); ++j)
			{
				if (It->Value.RunningSections[j].Index >= 0)
				{
					bNeedFinish = false;
					break;
				}
			}
		}

		if (bNeedFinish)
		{
			EBXTLFinishReason StopReason = It->Value.bEarlyFinish ? EBXTLFinishReason::FR_Interrupt : EBXTLFinishReason::FR_EndOfLife;

			// 广播时间轴即将结束事件
			TimelineWillEnd.Broadcast(It->Value.ID, StopReason);

			// 尝试直接调用拥有者的组件函数
			if (IsValid(It->Value.Owner))
			{
				if (UBXTimelineComponent* BXTLC = It->Value.Owner->FindComponentByClass<UBXTimelineComponent>())
				{
					BXTLC->ReceiveTimelineWillFinish(It->Value.ID, StopReason);
				}
			}

			// 从容器中移除
			It.RemoveCurrent();
		}
	}

	// 更新托管的任务
	EBXTLFinishReason FinishReason;
	for (TArray<FBXTLTaskHostingData>::TIterator It(TimelineTaskHostingDatas); It; ++It)
	{
		UBXTask* Task = It->Task;
		if (!Task)
		{
			It.RemoveCurrent();
			continue;
		}

		// 找到Task处理器
		UBXTProcessor* Processor = GetTLTProcessorByTLTClass(Task->GetClass());
		if (!Processor)
		{
			It.RemoveCurrent();
			continue;
		}

		// 填充辅助结构体
		HelpHostingData1.Reset();
		HelpHostingData1.Owner = It->Owner;
		HelpHostingData1.Instigator = It->Instigator;
		HelpHostingData1.Triggerer = It->Triggerer;
		HelpHostingData1.LockParts.Append(It->LockParts);
		HelpHostingData2.Reset();

		// 更新
		if (!UBXTProcessor::IsTaskCompleted(Task, It->TaskRTData, FinishReason))
		{
			Processor->UpdateTask(HelpHostingData1, HelpHostingData2, It->TaskRTData, Task, InDeltaTime, 1.0f);
		}

		// 结束
		if (UBXTProcessor::IsTaskCompleted(Task, It->TaskRTData, FinishReason))
		{
			Processor->EndTask(HelpHostingData1, HelpHostingData2, It->TaskRTData, Task, FinishReason);
			It.RemoveCurrent();
		}
	}
}

void UBXManager::FinishTimelineSection(FBXTLRunTimeData& InOutData, FBXTLSectionRTData& InOutSectionData, EBXTLFinishReason InReason)
{
	// 时间轴静态数据
	UBXTLAsset* Asset = InOutData.StaticData;
	if (!Asset || !Asset->Sections.IsValidIndex(InOutSectionData.Index))
	{
		return;
	}

	// 时间片段
	const FBXTLSection& Section = Asset->Sections[InOutSectionData.Index];

	// 提炼常用数据
	int32 SectionEndMask = (int32)EBXTTriggerType::T_SectionEnd;
	SectionEndMask = 1 << SectionEndMask;
	int32 SectionInterruptMask = (int32)EBXTTriggerType::T_SectionInterrupt;
	SectionInterruptMask = 1 << SectionInterruptMask;
	ENetMode NetMode = ENetMode::NM_Standalone;
	ENetRole LocalRole = ENetRole::ROLE_Authority;
	if (AActor* Owner = Cast<AActor>(InOutData.Owner))
	{
		NetMode = Owner->GetNetMode();
		LocalRole = Owner->GetLocalRole();
	}

	// 执行部分任务
	for (TArray<UBXTask*>::TConstIterator It(Section.TaskList); It; ++It)
	{
		UBXTask* Task = *It;
		if (!Task)
		{
			continue;
		}

		if (InReason == EBXTLFinishReason::FR_EndOfLife)
		{
			if (SectionEndMask > 0)
			{
				ExecuteTimelineTask(InOutData, InOutSectionData, It.GetIndex(), NetMode, LocalRole, 0.0f);
			}
		}
		else if (InReason == EBXTLFinishReason::FR_Interrupt)
		{
			if (SectionInterruptMask > 0)
			{
				ExecuteTimelineTask(InOutData, InOutSectionData, It.GetIndex(), NetMode, LocalRole, 0.0f);
			}
		}
	}

	// 执行被动触发的Task
	ProcessTimelineSectionPendingTasks(InOutData, InOutSectionData);

	// 结束正在运行的任务
	for (TArray<FBXTLTaskRTData>::TIterator It(InOutSectionData.RunningTasks); It; ++It)
	{
		FBXTLTaskRTData& TaskData = *It;
		if (!Section.TaskList.IsValidIndex(TaskData.Index))
		{
			continue;
		}

		UBXTask* Task = Section.TaskList[TaskData.Index];
		if (!Task)
		{
			continue;
		}

		// 固定时长需要托管
		if (Task->LifeType == EBXTLifeType::L_Duration)
		{
			FBXTLTaskHostingData& NewHosting = TimelineTaskHostingDatas.AddDefaulted_GetRef();
			NewHosting.Task = Task;
			NewHosting.Owner = InOutData.Owner;
			NewHosting.Instigator = InOutData.Instigator;
			NewHosting.Triggerer = InOutData.Triggerer;
			NewHosting.LockParts.Append(InOutData.LockParts);
			NewHosting.TaskRTData = TaskData;

			continue;
		}

		// 找到任务处理器，结束任务
		UBXTProcessor* Processor = GetTLTProcessorByTLTClass(Task->GetClass());
		if (!Processor)
		{
			continue;
		}
		Processor->EndTask(InOutData, InOutSectionData, TaskData, Task, InReason);
	}

	// 时间片段数据重置
	InOutSectionData.Reset();
}

void UBXManager::ProcessTimelineSectionPendingTasks(FBXTLRunTimeData& InOutData, FBXTLSectionRTData& InOutSectionData)
{
	if (!InOutData.StaticData || !InOutData.StaticData->Sections.IsValidIndex(InOutSectionData.Index))
	{
		return;
	}

	const FBXTLSection& Section = InOutData.StaticData->Sections[InOutSectionData.Index];

	ENetMode NetMode = ENetMode::NM_Standalone;
	ENetRole LocalRole = ENetRole::ROLE_Authority;
	if (AActor* Owner = Cast<AActor>(InOutData.Owner))
	{
		NetMode = Owner->GetNetMode();
		LocalRole = Owner->GetLocalRole();
	}

	for (TArray<FBXTLPendingTaskInfo>::TIterator It(InOutSectionData.PendingTasks); It; ++It)
	{
		// 检测数据有效性
		FBXTLPendingTaskInfo& Information = *It;
		if (!Section.TaskList.IsValidIndex(Information.LocalIndex))
		{
			Information.LocalIndex = -1;
			continue;
		}

		// 清理当前帧待执行队列
		InOutSectionData.FramePendingTasks.Reset();

		// 判断触发时间
		float DeltaTime = InOutSectionData.RunTime - Information.Time;
		if (DeltaTime >= 0.0f)
		{
			// 将根Task加入当前帧待执行队列
			InOutSectionData.FramePendingTasks.Add(Information.LocalIndex);

			// 执行Task
			ExecuteTimelineTask(InOutData, InOutSectionData, Information.LocalIndex, NetMode, LocalRole, DeltaTime);

			// 标记为已执行
			Information.LocalIndex = -1;

			int32 FramePendingIndex = 1;
			while (FramePendingIndex < InOutSectionData.FramePendingTasks.Num())
			{
				FramePendingIndex += 1;
				int32 Index = InOutSectionData.FramePendingTasks[FramePendingIndex];

				// 执行Task
				ExecuteTimelineTask(InOutData, InOutSectionData, Index, NetMode, LocalRole, 0.0f);
			}
		}
	}

	// 移除已完成的任务
	if (GCTimer <= 0.0f)
	{
		InOutSectionData.PendingTasks.RemoveAllSwap
		(
			[](const FBXTLPendingTaskInfo& Value)
			{
				return Value.LocalIndex < 0;
			}
		);
	}

	// 清空帧内待执行列表
	InOutSectionData.FramePendingTasks.Reset();
}

bool UBXManager::ExecuteTimelineTask(FBXTLRunTimeData& InOutData, FBXTLSectionRTData& InOutSectionData, int32 InTaskIndex, ENetMode InNetMode, ENetRole InRoleType, float InStartOffset)
{
	if (!InOutData.StaticData || !InOutData.StaticData->Sections.IsValidIndex(InOutSectionData.Index) || !InOutData.StaticData->Sections[InOutSectionData.Index].TaskList.IsValidIndex(InTaskIndex))
	{
		return false;
	}

	if (!InOutData.StaticData->Sections.IsValidIndex(InOutSectionData.Index))
	{
		return false;
	}
	FBXTLSection& Section = InOutData.StaticData->Sections[InOutSectionData.Index];

	UBXTask* InTask = Section.TaskList[InTaskIndex];
	if (!InTask)
	{
		return false;
	}

	bool bResult = (InNetMode == ENetMode::NM_Standalone);
	// 判断是否是权威端
	if (!bResult && (InTask->NetTypes & 1) > 0)
	{
		if (InRoleType == ENetRole::ROLE_Authority)
		{
			bResult |= true;
		}
	}
	// 判断是否是自主端
	if (!bResult && (InTask->NetTypes & 2) > 0)
	{
		if (InNetMode == ENetMode::NM_ListenServer && (InRoleType == ENetRole::ROLE_Authority || InRoleType == ENetRole::ROLE_AutonomousProxy))
		{
			bResult |= true;
		}

		if (InNetMode == ENetMode::NM_Client && InRoleType == ENetRole::ROLE_AutonomousProxy)
		{
			bResult |= true;
		}
	}
	// 判断是否是模拟端
	if (!bResult && (InTask->NetTypes & 4) > 0)
	{
		if (InNetMode == ENetMode::NM_Client && InRoleType == ENetRole::ROLE_SimulatedProxy)
		{
			bResult |= true;
		}
	}
	if (!bResult)
	{
		return false;
	}

	UScriptStruct* CustomDataType = TimelineTaskTypeMap.FindRef(InTask->GetClass());
	if (!CustomDataType)
	{
		return false;
	}

	if (InTask->LifeType == EBXTLifeType::L_Instant)
	{
		// 创建Task运行时数据
		FBXTLTaskRTData NewTaskData(CustomDataType);
		NewTaskData.Index = InTaskIndex;

		// 找到Task处理器
		if (UBXTProcessor* Processor = GetTLTProcessorByTLTClass(InTask->GetClass()))
		{
			// 开始
			Processor->StartTask(InOutData, InOutSectionData, NewTaskData, InTask);

			// 结束
			Processor->EndTask(InOutData, InOutSectionData, NewTaskData, InTask, EBXTLFinishReason::FR_EndOfLife);
		}
	}
	else
	{
		// 创建Task运行时数据
		FBXTLTaskRTData& NewTaskData = InOutSectionData.RunningTasks.AddDefaulted_GetRef();
		NewTaskData.Index = InTaskIndex;
		NewTaskData.DynamicData.InitializeAs(CustomDataType);

		// 找到Task处理器
		if (UBXTProcessor* Processor = GetTLTProcessorByTLTClass(InTask->GetClass()))
		{
			// 开始
			Processor->StartTask(InOutData, InOutSectionData, NewTaskData, InTask);
		}
	}

	return true;
}

UBXTProcessor* UBXManager::GetTLTProcessorByTLTClass(UClass* TaskClass)
{
	UBXTProcessor* Processor = TimelineTaskProcessors.FindRef(TaskClass);
	if (!Processor)
	{
		if (UClass* ProcessorType = TimelineTaskProcessorTypeMap.FindRef(TaskClass))
		{
			Processor = NewObject<UBXTProcessor>(this, ProcessorType);
			TimelineTaskProcessors.Add(TaskClass, Processor);
		}
	}

	return Processor;
}

FBXTLRunTimeData* UBXManager::GetTimelineRunTimeDataByID(int64 InID)
{
	return TimelineRTDatas.Find(InID);
}

void UBXManager::InternalUpdateTimeline(FBXTLRunTimeData& InOutData, float InDeltaTime)
{
	// 标记为更新中
	bUpdatingTimeline = true;

	EBXTLFinishReason FinishReason;

	UBXTLAsset* Asset = InOutData.StaticData;
	if (!Asset)
	{
		return;
	}

	// 提炼常用数据
	float UpdateRate = InOutData.RunRate;
	ENetMode NetMode = ENetMode::NM_Standalone;
	ENetRole LocalRole = ENetRole::ROLE_Authority;
	if (AActor* Owner = Cast<AActor>(InOutData.Owner))
	{
		NetMode = Owner->GetNetMode();
		LocalRole = Owner->GetLocalRole();
	}

	// 更新运行时间
	InOutData.RunTime += InDeltaTime * UpdateRate;

	// 更新时间片段
	for (TArray<FBXTLSectionRTData>::TIterator It(InOutData.RunningSections); It; ++It)
	{
		FBXTLSectionRTData& SectionData = *It;
		if (!Asset->Sections.IsValidIndex(SectionData.Index))
		{
			It.RemoveCurrent();
			continue;
		}

		// 时间片段静态数据
		const FBXTLSection& Section = Asset->Sections[SectionData.Index];

		// 更新正在运行的Task
		for (TArray<FBXTLTaskRTData>::TIterator It2(SectionData.RunningTasks); It2; ++It2)
		{
			FBXTLTaskRTData& TaskData = *It2;
			if (!Section.TaskList.IsValidIndex(TaskData.Index))
			{
				It2.RemoveCurrent();
				continue;
			}

			UBXTask* Task = Section.TaskList[TaskData.Index];
			if (!Task)
			{
				It2.RemoveCurrent();
				continue;
			}

			// 找到Task处理器
			UBXTProcessor* Processor = GetTLTProcessorByTLTClass(Task->GetClass());
			if (!Processor)
			{
				It2.RemoveCurrent();
				continue;
			}

			// 更新
			if (!UBXTProcessor::IsTaskCompleted(Task, TaskData, FinishReason))
			{
				Processor->UpdateTask(InOutData, SectionData, TaskData, Task, InDeltaTime, UpdateRate);
			}

			// 结束
			if (UBXTProcessor::IsTaskCompleted(Task, TaskData, FinishReason))
			{
				Processor->EndTask(InOutData, SectionData, TaskData, Task, FinishReason);
			}
		}

		// 更新时间片段运行时间
		SectionData.RunTime += InDeltaTime * UpdateRate;

		// 结束时间片段
		if (SectionData.RunTime >= Section.Duration)
		{
			FinishTimelineSection(InOutData, SectionData, EBXTLFinishReason::FR_EndOfLife);

			// 开始下一次循环 或 下一个时间片段
			if (SectionData.LoopCount < Section.LoopTime)
			{
				SectionData.LoopCount += 1;
			}
			else if (Section.NextIndex >= 0)
			{
				SectionData.LoopCount = 1;
				SectionData.Timestamp = UBXFunctionLibrary::GetUtcMillisecond();
				SectionData.Index = Section.NextIndex;
			}
		}

		// 触发时间片段的任务
		for (TArray<FBXTLKeyFrame>::TConstIterator It2(Section.KeyFrames); It2; ++It2)
		{
			const FBXTLKeyFrame& KeyFrame = *It2;

			if (SectionData.RunTime < KeyFrame.Time)
			{
				SectionData.KeyFrameIndex = It2.GetIndex() + 1;
				break;
			}

			// 计算偏移时间
			float OffsetTime = SectionData.RunTime - KeyFrame.Time;

			// 执行新的Task
			for (TArray<int32>::TConstIterator It3(KeyFrame.Tasks); It3; ++It3)
			{
				ExecuteTimelineTask(InOutData, SectionData, *It3, NetMode, LocalRole, OffsetTime);
			}
		}

		// 处理待执行任务队列
		ProcessTimelineSectionPendingTasks(InOutData, SectionData);
	}
	bUpdatingTimeline = false;
}

void UBXManager::CleanTimeline()
{
	// 结束所有时间轴
	for (TMap<int64, FBXTLRunTimeData>::TIterator It(TimelineRTDatas); It; ++It)
	{
		for (TArray<FBXTLSectionRTData>::TIterator It2(It->Value.RunningSections); It2; ++It2)
		{
			FinishTimelineSection(It->Value, *It2, EBXTLFinishReason::FR_Interrupt);
		}
	}
	TimelineRTDatas.Empty();

	// 结束托管的任务
	for (TArray<FBXTLTaskHostingData>::TIterator It(TimelineTaskHostingDatas); It; ++It)
	{
		UBXTask* Task = It->Task;
		if (!Task)
		{
			continue;
		}

		// 找到Task处理器
		UBXTProcessor* Processor = GetTLTProcessorByTLTClass(Task->GetClass());
		if (!Processor)
		{
			continue;
		}

		// 填充辅助结构体
		HelpHostingData1.Reset();
		HelpHostingData1.Owner = It->Owner;
		HelpHostingData1.Instigator = It->Instigator;
		HelpHostingData1.Triggerer = It->Triggerer;
		HelpHostingData1.LockParts.Append(It->LockParts);
		HelpHostingData2.Reset();

		// 结束任务
		Processor->EndTask(HelpHostingData1, HelpHostingData2, It->TaskRTData, Task, EBXTLFinishReason::FR_Interrupt);
	}
	TimelineTaskHostingDatas.Empty();
}

#pragma endregion Timeline



#pragma region Callback
void UBXManager::OnWorldCleanupStart(UWorld* World, bool bSessionEnded, bool bCleanupResources)
{
	CleanTimeline();
}

#pragma endregion Callback



#pragma region Debug
bool UBXManager::GetShowCollision() const
{
	return bShowCollision;
}

void UBXManager::ChangeShowCollision(bool InShow)
{
	bShowCollision = InShow;
}

#pragma endregion Debug
