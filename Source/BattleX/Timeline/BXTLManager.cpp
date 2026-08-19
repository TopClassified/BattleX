#include "BXTLManager.h"
#include "AssetRegistry/AssetRegistryModule.h"

#include "BXSubSystem.h"
#include "BXSettings.h"
#include "BXStructs.h"
#include "BXTLAsset.h"
#include "BXSkillAsset.h"
#include "BXTask.h"
#include "BXTProcessor.h"
#include "BXTLComponent.h"



DEFINE_LOG_CATEGORY(BXMGR_Timeline);



#pragma region Important
UBXTLManager* UBXTLManager::Get(UObject* InWorldContext)
{
	UBXTLManager* Result = nullptr;

	if (InWorldContext)
	{
		if (UWorld* World = InWorldContext->GetWorld())
		{
			if (UGameInstance* GI = World->GetGameInstance())
			{
				if (UBXSubSystem* BXSS = GI->GetSubsystem<UBXSubSystem>())
				{
					Result = BXSS->GetManagerByClass<UBXTLManager>();
				}
			}

#if WITH_EDITOR
			if (!IsValid(Result))
			{
				for (TObjectIterator<UBXTLManager> It; It; ++It)
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

void UBXTLManager::Initialize()
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

	// 收集所有的时间轴资源路径
	CollectTimelineAssetPath();
	
	FWorldDelegates::OnWorldCleanup.AddUObject(this, &UBXTLManager::OnWorldCleanupStart);
}

void UBXTLManager::Deinitialize()
{
	
	FWorldDelegates::OnWorldCleanup.RemoveAll(this);
}

#pragma endregion Important



#pragma region Tick
UWorld* UBXTLManager::GetTickableGameObjectWorld() const
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

ETickableTickType UBXTLManager::GetTickableTickType() const
{
	return IsTemplate() ? ETickableTickType::Never : ETickableTickType::Always;
}

bool UBXTLManager::IsAllowedToTick() const
{
	return IsValid(this) && !IsUnreachable();
}

TStatId UBXTLManager::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UBXTLManager, STATGROUP_Tickables);
}

void UBXTLManager::Tick(float DeltaTime)
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

		CleanTimelineTrash();
	}
}

#pragma endregion Tick



#pragma region Asset
TSoftObjectPtr<UBXTLAsset> UBXTLManager::GetTimelineAssetByID(int32 InID)
{
	if (FBXTimelineAssetInformation* Result = TimelineAssetMap.Find(InID))
	{
		return Result->AssetPath;
	}

	return nullptr;
}

TSoftObjectPtr<UBXTLAsset> UBXTLManager::GetSkillAssetByID(int32 InID)
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

TArray<int32> UBXTLManager::GetTimelineAssetIDs()
{
	TArray<int32> IDs;
	TimelineAssetMap.GenerateKeyArray(IDs);

	return IDs;
}

TArray<int32> UBXTLManager::GetSkillAssetIDs()
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

void UBXTLManager::CollectTimelineAssetPath()
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
bool UBXTLManager::IsUpdatingTimeline() const
{
	return bUpdatingTimeline;
}

int64 UBXTLManager::PlayTimeline(UBXTLAsset* InAsset, AActor* InOwner, UPARAM(ref) FBXTLPlayContext& InContext)
{
	// 严禁更新时间轴时，播放新的时间轴
	if (bUpdatingTimeline)
	{
		UE_LOG(BXMGR_Timeline, Warning, TEXT("UBXTLManager::PlayTimeline failed: bUpdatingTimeline=true, rejecting new timeline. Owner=%s Asset=%s."), InOwner ? *InOwner->GetName() : TEXT("null"), InAsset ? *InAsset->GetName() : TEXT("null"));
		return 0;
	}

	if (!IsValid(InAsset))
	{
		UE_LOG(BXMGR_Timeline, Warning, TEXT("UBXTLManager::PlayTimeline failed: InAsset is null, Owner=%s."), InOwner ? *InOwner->GetName() : TEXT("null"));
		return 0;
	}

	// 创建时间轴运行时数据
	int64 NewID = UBXFunctionLibrary::GetUniqueID();
	FBXTLRunTimeData& NewData = TimelineRTDatas.Add(NewID);
	NewData.Timeline = InAsset;
	NewData.TimelineID = InAsset->ID;
	NewData.ID = NewID;
	NewData.Owner = InOwner;
	NewData.Instigator = InContext.Instigator;
	NewData.Triggerer = InContext.Triggerer;
	NewData.LockParts.Append(InContext.LockParts);
	for (TMap<FGameplayTag, FInstancedStruct>::TIterator It(InContext.InputDatas); It; ++It)
	{
		NewData.DynamicDatas.Add(FBXTLDynamicDataSearchKey(-1, It->Key), It->Value);
	}

	// 初始化RunningSections并触发首帧KeyFrame执行
	StartTimelineSections(NewData);

	return NewID;
}

void UBXTLManager::StopTimeline(int64 InID, EBXTLFinishReason InReason)
{
	if (FBXTLRunTimeData* RTData = TimelineRTDatas.Find(InID))
	{
		for (int32 i = 0; i < RTData->RunningSections.Num(); ++i)
		{
			FinishTimelineSection(*RTData, RTData->RunningSections[i], InReason);
		}
		
		// 尝试直接调用拥有者的组件函数
		if (IsValid(RTData->Owner))
		{
			if (UBXTLComponent* BXTLC = RTData->Owner->FindComponentByClass<UBXTLComponent>())
			{
				BXTLC->ReceiveTimelineWillFinish(InID, InReason);
			}
		}

		// 从容器中移除
		TimelineRTDatas.Remove(InID);
	}
}

void UBXTLManager::ChangeTimelineTickRate(int64 InID, float InRate)
{
	InRate = FMath::Max(InRate, 0.0001f);
	
	if (FBXTLRunTimeData* RTData = TimelineRTDatas.Find(InID))
	{
		RTData->RunRate = InRate;
		UBXTLAsset* Asset = RTData->Timeline;
		if (!Asset)
		{
			return;
		}

		// 遍历所有的片段
		for (int32 i = 0; i < RTData->RunningSections.Num(); ++i)
		{
			FBXTLSectionRTData& SectionData = RTData->RunningSections[i];
			if (!Asset->Sections.IsValidIndex(SectionData.Index))
			{
				continue;
			}
			
			const FBXTLSection& Section = Asset->Sections[SectionData.Index];
			// 遍历所有的任务，调整其更新频率
			for (int32 j = 0; j < SectionData.RunningTasks.Num(); ++j)
			{
				FBXTLTaskRTData& TaskData = SectionData.RunningTasks[j];
				if (!Section.TaskList.IsValidIndex(TaskData.Index))
				{
					continue;
				}
				
				UBXTask* Task = Section.TaskList[TaskData.Index];
				if (!Task)
				{
					continue;
				}
				
				UBXTProcessor* Processor = GetTLTProcessorByTLTClass(Task->GetClass());
				if (!Processor)
				{
					continue;
				}

				Processor->ChangeTaskTickRate(TaskData, InRate);
			}
		}
	}
}

void UBXTLManager::ChangeTimelineRunTimeDataTickRate(FBXTLRunTimeData& InOutData, float InRate)
{
	InRate = FMath::Max(InRate, 0.0001f);

	InOutData.RunRate = InRate;
	UBXTLAsset* Asset = InOutData.Timeline;
	if (!Asset)
	{
		return;
	}

	for (int32 i = 0; i < InOutData.RunningSections.Num(); ++i)
	{
		FBXTLSectionRTData& SectionData = InOutData.RunningSections[i];
		if (!Asset->Sections.IsValidIndex(SectionData.Index))
		{
			continue;
		}

		const FBXTLSection& Section = Asset->Sections[SectionData.Index];
		for (int32 j = 0; j < SectionData.RunningTasks.Num(); ++j)
		{
			FBXTLTaskRTData& TaskData = SectionData.RunningTasks[j];
			if (!Section.TaskList.IsValidIndex(TaskData.Index))
			{
				continue;
			}

			UBXTask* Task = Section.TaskList[TaskData.Index];
			if (!Task)
			{
				continue;
			}

			UBXTProcessor* Processor = GetTLTProcessorByTLTClass(Task->GetClass());
			if (!Processor)
			{
				continue;
			}

			Processor->ChangeTaskTickRate(TaskData, InRate);
		}
	}
}

void UBXTLManager::UpdateTimeline(float InDeltaTime)
{
	// 快照遍历:Task/BP回调可能同步StopTimeline移除条目(含当前条目),避免TMap迭代器失效与双重移除
	TArray<int64> TimelineIDs;
	TimelineIDs.Reserve(TimelineRTDatas.Num());
	for (const TPair<int64, FBXTLRunTimeData>& Pair : TimelineRTDatas)
	{
		TimelineIDs.Add(Pair.Key);
	}

	for (int64 TimelineID : TimelineIDs)
	{
		FBXTLRunTimeData* RTData = TimelineRTDatas.Find(TimelineID);
		if (!RTData)
		{
			continue;
		}

		InternalUpdateTimeline(*RTData, InDeltaTime);

		// 检查是否要结束这个时间轴
		bool bNeedFinish = RTData->bEarlyFinish;
		if (!bNeedFinish)
		{
			bNeedFinish = true;
			for (int32 j = 0; j < RTData->RunningSections.Num(); ++j)
			{
				if (RTData->RunningSections[j].Index >= 0)
				{
					bNeedFinish = false;
					break;
				}
			}
		}

		if (bNeedFinish)
		{
			EBXTLFinishReason StopReason = RTData->bEarlyFinish ? EBXTLFinishReason::FR_Interrupt : EBXTLFinishReason::FR_EndOfLife;

			// 尝试直接调用拥有者的组件函数
			if (IsValid(RTData->Owner))
			{
				if (UBXTLComponent* BXTLC = RTData->Owner->FindComponentByClass<UBXTLComponent>())
				{
					BXTLC->ReceiveTimelineWillFinish(RTData->ID, StopReason);
				}
			}

			// 从容器中移除(ReceiveTimelineWillFinish内可能已StopTimeline同ID,Remove对不存在的键为空操作)
			TimelineRTDatas.Remove(TimelineID);
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
		HelpHostingData1.RunRate = 1.0f;
		HelpHostingData1.Owner = It->Owner;
		HelpHostingData1.Instigator = It->Instigator;
		HelpHostingData1.Triggerer = It->Triggerer;
		HelpHostingData1.LockParts.Append(It->LockParts);
		HelpHostingData2.Reset();

		// 更新
		if (!UBXTProcessor::IsTaskCompleted(It->TaskRTData, FinishReason))
		{
			Processor->UpdateTask(HelpHostingData1, HelpHostingData2, It->TaskRTData, InDeltaTime);
		}

		// 结束
		if (UBXTProcessor::IsTaskCompleted(It->TaskRTData, FinishReason))
		{
			Processor->EndTask(HelpHostingData1, HelpHostingData2, It->TaskRTData, FinishReason);
			It.RemoveCurrent();
		}
	}
}

void UBXTLManager::UpdateTimelineRunTimeData(FBXTLRunTimeData& InOutData, float InDeltaTime)
{
	InternalUpdateTimeline(InOutData, InDeltaTime);
}

void UBXTLManager::StartTimelineSections(FBXTLRunTimeData& InOutData)
{
	UBXTLAsset* Asset = InOutData.Timeline;
	if (!Asset)
	{
		return;
	}

	// 初始Section填充RunningSections
	for (int32 i = 0; i < Asset->StartSectionIndexes.Num(); ++i)
	{
		FBXTLSectionRTData& NewSectionData = InOutData.RunningSections.AddDefaulted_GetRef();
		NewSectionData.Index = Asset->StartSectionIndexes[i];

		// LoopCount为1基计数(跳转/循环分支均以1表示"第1次运行"),初始0会导致LoopTime=1的Section多跑一周期
		NewSectionData.LoopCount = 1;
	}

	// 立刻更新一次,通过KeyFrame触发Task首帧执行
	InternalUpdateTimeline(InOutData, 0.0f);
}

void UBXTLManager::FinishTimelineSection(FBXTLRunTimeData& InOutData, FBXTLSectionRTData& InOutSectionData, EBXTLFinishReason InReason)
{
	// 时间轴静态数据
	UBXTLAsset* Asset = InOutData.Timeline;
	if (!Asset || !Asset->Sections.IsValidIndex(InOutSectionData.Index))
	{
		return;
	}

	// 时间片段
	const FBXTLSection& Section = Asset->Sections[InOutSectionData.Index];

	// 提炼常用数据
	int32 SectionEndMask = 1 << (int32)EBXTTriggerType::T_SectionEnd;
	int32 SectionInterruptMask = 1 << (int32)EBXTTriggerType::T_SectionInterrupt;
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
			if ((Task->TriggerTypes & SectionEndMask) > 0)
			{
				ExecuteTimelineTask(InOutData, InOutSectionData, It.GetIndex(), NetMode, LocalRole, 0.0f);
			}
		}
		else if (InReason == EBXTLFinishReason::FR_Interrupt)
		{
			if ((Task->TriggerTypes & SectionInterruptMask) > 0)
			{
				ExecuteTimelineTask(InOutData, InOutSectionData, It.GetIndex(), NetMode, LocalRole, 0.0f);
			}
		}
	}

	// 执行被动触发的Task
	ProcessTimelineSectionPendingTasks(InOutData, InOutSectionData);

	// 结束正在运行的任务
	// EndTask内部会按地址从RunningTasks移除当前条目(数组左移),此处用索引循环且移除后不推进索引,否则会跳过相邻任务导致其End永久丢失
	int32 TaskIndex = 0;
	while (TaskIndex < InOutSectionData.RunningTasks.Num())
	{
		FBXTLTaskRTData& TaskData = InOutSectionData.RunningTasks[TaskIndex];
		if (!Section.TaskList.IsValidIndex(TaskData.Index))
		{
			++TaskIndex;
			continue;
		}

		UBXTask* Task = Section.TaskList[TaskData.Index];
		if (!Task)
		{
			++TaskIndex;
			continue;
		}

		// 找到任务处理器，结束任务
		UBXTProcessor* Processor = GetTLTProcessorByTLTClass(Task->GetClass());
		if (!Processor)
		{
			++TaskIndex;
			continue;
		}

		// 固定时长需要托管(条目保留在RunningTasks,由末尾Reset统一清空)
		if (Task->LifeType == EBXTLifeType::L_Duration)
		{
			FBXTLTaskHostingData& NewHosting = TimelineTaskHostingDatas.AddDefaulted_GetRef();
			NewHosting.Task = Task;
			NewHosting.Owner = InOutData.Owner;
			NewHosting.Instigator = InOutData.Instigator;
			NewHosting.Triggerer = InOutData.Triggerer;
			NewHosting.LockParts.Append(InOutData.LockParts);
			NewHosting.TaskRTData = TaskData;

			Processor->ChangeTaskTickRate(TaskData, 1.0f);

			++TaskIndex;
			continue;
		}

		// EndTask内部移除该条目,数组左移,索引不推进即可处理下一个
		Processor->EndTask(InOutData, InOutSectionData, TaskData, InReason);
	}

	// 时间片段数据重置
	InOutSectionData.Reset();
}

void UBXTLManager::ProcessTimelineSectionPendingTasks(FBXTLRunTimeData& InOutData, FBXTLSectionRTData& InOutSectionData)
{
	if (!InOutData.Timeline || !InOutData.Timeline->Sections.IsValidIndex(InOutSectionData.Index))
	{
		return;
	}

	const FBXTLSection& Section = InOutData.Timeline->Sections[InOutSectionData.Index];

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
		
		// 判断触发时间
		float DeltaTime = InOutSectionData.RunTime - Information.Time;
		if (DeltaTime >= 0.0f)
		{
			// 将根Task加入当前帧待执行队列
			InOutSectionData.TaskStackInFrame.Reset();
			InOutSectionData.TaskStackInFrame.Add(FInt64Vector2(Information.LocalIndex, Information.ParentScope));

			// 执行Task
			ExecuteTimelineTask(InOutData, InOutSectionData, Information.LocalIndex, NetMode, LocalRole, DeltaTime, Information.ParentScope);

			// 标记为已执行
			Information.LocalIndex = -1;

			// 帧内触发的其他Task(从1起始:栈[0]是根Task自身且已在上方执行过,从0起始会把根Task再执行一遍,效果翻倍)
			int32 CurrentIndex = 1;
			while (CurrentIndex < InOutSectionData.TaskStackInFrame.Num())
			{
				FInt64Vector2& Stack = InOutSectionData.TaskStackInFrame[CurrentIndex];
				ExecuteTimelineTask(InOutData, InOutSectionData, Stack.X, NetMode, LocalRole, 0.0f, Stack.Y);

				CurrentIndex += 1;
			}
		}
	}

	// 清空帧内执行队列:残留条目会让后续在更新管线外触发的AddPendingTask误入堆栈(永不执行且阻塞同索引任务入队)
	InOutSectionData.TaskStackInFrame.Reset();

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
}

bool UBXTLManager::ExecuteTimelineTask(FBXTLRunTimeData& InOutData, FBXTLSectionRTData& InOutSectionData, int32 InTaskIndex, ENetMode InNetMode, ENetRole InRoleType, float InStartOffset, int64 InParentScope)
{
	if (!InOutData.Timeline || !InOutData.Timeline->Sections.IsValidIndex(InOutSectionData.Index) || !InOutData.Timeline->Sections[InOutSectionData.Index].TaskList.IsValidIndex(InTaskIndex))
	{
		UE_LOG(BXMGR_Timeline, Warning, TEXT("UBXTLManager::ExecuteTimelineTask failed: Invalid index. Timeline=%s SectionIdx=%d TaskIdx=%d."), InOutData.Timeline ? *InOutData.Timeline->GetName() : TEXT("null"), InOutSectionData.Index, InTaskIndex);
		return false;
	}

	if (!InOutData.Timeline->Sections.IsValidIndex(InOutSectionData.Index))
	{
		return false;
	}
	FBXTLSection& Section = InOutData.Timeline->Sections[InOutSectionData.Index];

	UBXTask* InTask = Section.TaskList[InTaskIndex];
	if (!InTask)
	{
		UE_LOG(BXMGR_Timeline, Warning, TEXT("UBXTLManager::ExecuteTimelineTask failed: Task is null at SectionIdx=%d TaskIdx=%d, Asset=%s."), InOutSectionData.Index, InTaskIndex, *InOutData.Timeline->GetName());
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
		UE_LOG(BXMGR_Timeline, Warning, TEXT("UBXTLManager::ExecuteTimelineTask failed: NetType check failed. Task=%s NetTypes=%d NetMode=%d LocalRole=%d."), *InTask->GetName(), InTask->NetTypes, (int32)InNetMode, (int32)InRoleType);
		return false;
	}

	UScriptStruct* CustomDataType = TimelineTaskTypeMap.FindRef(InTask->GetClass());
	if (!CustomDataType)
	{
		UE_LOG(BXMGR_Timeline, Warning, TEXT("UBXTLManager::ExecuteTimelineTask failed: CustomDataType not registered in TaskCustomDataMap. Task=%s Class=%s."), *InTask->GetName(), *InTask->GetClass()->GetName());
		return false;
	}

	if (InTask->LifeType == EBXTLifeType::L_Instant)
	{
		// 创建Task运行时数据
		FBXTLTaskRTData NewTaskData(CustomDataType);
		NewTaskData.Task = InTask;
		NewTaskData.Index = InTaskIndex;
		NewTaskData.ParentScope = InParentScope;
		NewTaskData.DynamicData.InitializeAs(CustomDataType);

		// 找到Task处理器
		if (UBXTProcessor* Processor = GetTLTProcessorByTLTClass(InTask->GetClass()))
		{
			// 开始
			Processor->StartTask(InOutData, InOutSectionData, NewTaskData);

			// 结束
			Processor->EndTask(InOutData, InOutSectionData, NewTaskData, EBXTLFinishReason::FR_EndOfLife);
		}
		else
		{
			UE_LOG(BXMGR_Timeline, Warning, TEXT("UBXTLManager::ExecuteTimelineTask failed: Processor not registered in TaskProcessorMap. Task=%s Class=%s."), *InTask->GetName(), *InTask->GetClass()->GetName());
		}
	}
	else
	{
		// 创建Task运行时数据
		FBXTLTaskRTData& NewTaskData = InOutSectionData.RunningTasks.AddDefaulted_GetRef();
		NewTaskData.Task = InTask;
		NewTaskData.Index = InTaskIndex;
		NewTaskData.ParentScope = InParentScope;
		NewTaskData.RunTime = InStartOffset;
		NewTaskData.DynamicData.InitializeAs(CustomDataType);

		// 找到Task处理器
		if (UBXTProcessor* Processor = GetTLTProcessorByTLTClass(InTask->GetClass()))
		{
			// 开始
			Processor->StartTask(InOutData, InOutSectionData, NewTaskData);
		}
		else
		{
			UE_LOG(BXMGR_Timeline, Warning, TEXT("UBXTLManager::ExecuteTimelineTask failed: Processor not registered in TaskProcessorMap. Task=%s Class=%s."), *InTask->GetName(), *InTask->GetClass()->GetName());
		}
	}

	return true;
}

UBXTProcessor* UBXTLManager::GetTLTProcessorByTLTClass(UClass* TaskClass)
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

FBXTLRunTimeData* UBXTLManager::GetTimelineRunTimeDataByID(int64 InID)
{
	return TimelineRTDatas.Find(InID);
}

void UBXTLManager::CloseSectionJump(bool InClose)
{
	bCloseSectionJump = InClose;
}

void UBXTLManager::InternalUpdateTimeline(FBXTLRunTimeData& InOutData, float InDeltaTime)
{
	UBXTLAsset* Asset = InOutData.Timeline;
	if (!Asset)
	{
		return;
	}

	// 标记为更新中(空资产提前返回,禁止在置标志后return否则PlayTimeline被永久拒绝)
	bUpdatingTimeline = true;

	// 调整更新时间
	float FixedDeltaTime = InDeltaTime * InOutData.RunRate;
	
	// 提炼常用数据
	ENetMode NetMode = ENetMode::NM_Standalone;
	ENetRole LocalRole = ENetRole::ROLE_Authority;
	if (AActor* Owner = Cast<AActor>(InOutData.Owner))
	{
		NetMode = Owner->GetNetMode();
		LocalRole = Owner->GetLocalRole();
	}
	
	// 更新运行时间
	InOutData.RunTime += FixedDeltaTime;

	// 更新时间片段
	for (TArray<FBXTLSectionRTData>::TIterator It(InOutData.RunningSections); It; ++It)
	{
		FBXTLSectionRTData& SectionData = *It;
		if (!Asset->Sections.IsValidIndex(SectionData.Index))
		{
			UE_LOG(BXMGR_Timeline, Warning, TEXT("UBXTLManager::InternalUpdateTimeline: SectionData.Index=%d invalid(Sections.Num=%d), removed. Asset=%s."), SectionData.Index, Asset->Sections.Num(), *Asset->GetName());
			It.RemoveCurrent();
			continue;
		}

		// 时间片段静态数据
		const FBXTLSection& Section = Asset->Sections[SectionData.Index];
		// 缓存本帧起始索引(跳转/循环重开后Section引用与Index脱节,旧Section的首帧任务不应在跳转当帧触发)
		const int32 CachedSectionIndex = SectionData.Index;

		// 更新正在运行的Task
		UpdateTimelineSectionTasks(InOutData, SectionData, FixedDeltaTime);

		// 更新时间片段运行时间
		SectionData.RunTime += FixedDeltaTime;

		// 是否存在等待客户端碰撞结果的Task(服务器端):延迟自然结束,等待上报或ExtraLife超时
		bool bHasAwaitingCollisionTask = false;
		for (const FBXTLTaskRTData& TaskData : SectionData.RunningTasks)
		{
			if (TaskData.bAwaitingClientCollision)
			{
				bHasAwaitingCollisionTask = true;
				break;
			}
		}

		// 结束时间片段
		if (bCloseSectionJump)
		{
			if ((SectionData.RunTime >= Section.Duration && !bHasAwaitingCollisionTask) || SectionData.bEarlyFinish)
			{
				FinishTimelineSection(InOutData, SectionData, EBXTLFinishReason::FR_EndOfLife);
			}
		}
		else
	{
		if ((SectionData.RunTime >= Section.Duration && !bHasAwaitingCollisionTask) || SectionData.bEarlyFinish || SectionData.ForceJumpSection >= 0)
		{
			// 缓存跳转判定所需的原始状态(FinishTimelineSection内部的Reset会重置ForceJumpSection/LoopCount/bEarlyFinish,
			// 重置后再读取判断会导致循环/链跳转/强制跳转全部失效——Section一律在首周期后死亡)
			const int32 CachedForceJumpSection = SectionData.ForceJumpSection;
			const int32 CachedLoopCount = SectionData.LoopCount;
			const bool bCachedEarlyFinish = SectionData.bEarlyFinish;

			if (bCachedEarlyFinish || CachedForceJumpSection >= 0)
			{
				FinishTimelineSection(InOutData, SectionData, EBXTLFinishReason::FR_Interrupt);
			}
			else
			{
				FinishTimelineSection(InOutData, SectionData, EBXTLFinishReason::FR_EndOfLife);
			}

			// 跳转到强制执行片段(原实现条件写反:ForceJumpSection>=0才应跳转,却写成了<0)
			if (CachedForceJumpSection >= 0)
			{
				SectionData.LoopCount = 1;
				SectionData.Index = CachedForceJumpSection;
			}
			// 开始下一次循环 或 下一个时间片段
			else
			{
				// LoopTime<=0为无限循环语义(资产注释声明,加速钳制ClampAccelerateDuration已按此假设处理)
				if ((Section.LoopTime <= 0 || CachedLoopCount < Section.LoopTime) && !bCachedEarlyFinish)
				{
					SectionData.LoopCount = CachedLoopCount + 1;

					// 循环分支须恢复Index:FinishTimelineSection内部的Reset已将Index置-1,
					// 不恢复则Section立即被判为已结束并在下一帧移除,循环功能失效
					SectionData.Index = CachedSectionIndex;
				}
				else if (Section.NextIndex >= 0)
				{
					SectionData.LoopCount = 1;
					SectionData.Index = Section.NextIndex;
				}
			}
		}
	}

		// 旧时间片段已经结束，且没有开启新的时间片段
		// 本帧发生跳转时Section引用仍是旧片段,跳过当帧任务触发,下一帧按新Index取新Section
		if (SectionData.Index < 0 || SectionData.Index != CachedSectionIndex)
		{
			continue;
		}

		// 触发时间片段的任务
		while(SectionData.KeyFrameIndex < Section.KeyFrames.Num())
		{
			const FBXTLKeyFrame& KeyFrame = Section.KeyFrames[SectionData.KeyFrameIndex];

			// 计算偏移时间
			float OffsetTime = SectionData.RunTime - KeyFrame.Time;
			if (OffsetTime < 0.0f)
			{
				break;
			}

			// 执行新的Task
			for (TArray<int32>::TConstIterator It3(KeyFrame.Tasks); It3; ++It3)
			{
				ExecuteTimelineTask(InOutData, SectionData, *It3, NetMode, LocalRole, OffsetTime);
			}

			// 前进到下一个关键帧
			SectionData.KeyFrameIndex += 1;
		}

		// 处理待执行任务队列
		ProcessTimelineSectionPendingTasks(InOutData, SectionData);
	}
	bUpdatingTimeline = false;
}

void UBXTLManager::UpdateTimelineSectionTasks(FBXTLRunTimeData& InOutData, FBXTLSectionRTData& InOutSectionData, float InDeltaTime)
{
	UBXTLAsset* Asset = InOutData.Timeline;
	if (!Asset || !Asset->Sections.IsValidIndex(InOutSectionData.Index))
	{
		return;
	}

	const FBXTLSection& Section = Asset->Sections[InOutSectionData.Index];
	EBXTLFinishReason FinishReason;

	// EndTask内部会按地址从RunningTasks移除当前条目(数组左移),此处用索引循环且移除后不推进索引,否则同帧完成的相邻任务被跳过(延迟一帧结束)
	int32 TaskIndex = 0;
	while (TaskIndex < InOutSectionData.RunningTasks.Num())
	{
		FBXTLTaskRTData& TaskData = InOutSectionData.RunningTasks[TaskIndex];
		if (!Section.TaskList.IsValidIndex(TaskData.Index))
		{
			InOutSectionData.RunningTasks.RemoveAt(TaskIndex);
			continue;
		}

		UBXTask* Task = Section.TaskList[TaskData.Index];
		if (!Task)
		{
			InOutSectionData.RunningTasks.RemoveAt(TaskIndex);
			continue;
		}

		UBXTProcessor* Processor = GetTLTProcessorByTLTClass(Task->GetClass());
		if (!Processor)
		{
			InOutSectionData.RunningTasks.RemoveAt(TaskIndex);
			continue;
		}

		if (!UBXTProcessor::IsTaskCompleted(TaskData, FinishReason))
		{
			Processor->UpdateTask(InOutData, InOutSectionData, TaskData, InDeltaTime);
		}

		if (UBXTProcessor::IsTaskCompleted(TaskData, FinishReason))
		{
			// 服务器端等待客户端碰撞结果的Task不立即结束,递减额外生命计时
			if (TaskData.bAwaitingClientCollision)
			{
				TaskData.ServerExtraLifeTimer -= InDeltaTime;
				if (TaskData.ServerExtraLifeTimer <= 0.0f)
				{
					// EndTask内部移除该条目,数组左移,索引不推进即可处理下一个
					Processor->EndTask(InOutData, InOutSectionData, TaskData, FinishReason);
					continue;
				}
			}
			else
			{
				// EndTask内部移除该条目,数组左移,索引不推进即可处理下一个
				Processor->EndTask(InOutData, InOutSectionData, TaskData, FinishReason);
				continue;
			}
		}

		++TaskIndex;
	}
}

void UBXTLManager::CleanTimelineTrash()
{

}

void UBXTLManager::CleanTimeline()
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
		Processor->EndTask(HelpHostingData1, HelpHostingData2, It->TaskRTData, EBXTLFinishReason::FR_Interrupt);
	}
	TimelineTaskHostingDatas.Empty();
}

#pragma endregion Timeline



#pragma region Callback
void UBXTLManager::OnWorldCleanupStart(UWorld* World, bool bSessionEnded, bool bCleanupResources)
{
	CleanTimeline();
}

#pragma endregion Callback



#pragma region Debug
bool UBXTLManager::GetShowCollision() const
{
	return bShowCollision;
}

void UBXTLManager::ChangeShowCollision(bool InShow)
{
	bShowCollision = InShow;
}

#pragma endregion Debug
