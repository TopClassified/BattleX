#include "BXTPCollision.h"

#include "BXGearComponent.h"



bool UBXTPCollision::CheckCoolDownCompleted(UBXTCollision* InTask, const FHitResult& InHitResult, float InCheckTime, UPARAM(ref) FBXTPCollisionContext& InOutRTData)
{
	if (!IsValid(InTask))
	{
		return false;
	}

	if (bUseNativeCheckCoolDown && !CheckCoolDown(InTask, InHitResult, InCheckTime, InOutRTData))
	{
		return false;
	}

	if (bUseScriptCheckCoolDown && !ScriptCheckCoolDown(InTask, InHitResult, InCheckTime, InOutRTData))
	{
		return false;
	}

	return true;
}

bool UBXTPCollision::CheckCoolDown(UBXTCollision* InTask, const FHitResult& InHitResult, float InCheckTime, FBXTPCollisionContext& InOutRTData)
{
	if (!IsValid(InTask) || !IsValid(InHitResult.GetActor()))
	{
		return false;
	}

	// 检查冷却
	if (float* FindResult = InOutRTData.CoolDownInformations.Find(InHitResult.GetActor()))
	{
		if (InCheckTime - (*FindResult) < InTask->CoolDown)
		{
			return false;
		}
	}

	// 添加冷却信息
	InOutRTData.CoolDownInformations.Add(InHitResult.GetActor(), InCheckTime);

	return true;
}







void UBXTPTrackWeaponCollision::Start(FBXTLRunTimeData& InOutRTData, FBXTLSectionRTData& InOutRTSData, FBXTLTaskRTData& InOutRTTData)
{
	UBXTTrackWeaponCollision* Task = Cast<UBXTTrackWeaponCollision>(InOutRTTData.Task);
	if (!Task)
	{
		return;
	}

	// 获取任务目标
	TArray<AActor*> Targets;
	UBXTProcessor::GetTargetActorList(InOutRTData, InOutRTTData, Targets);
	if (Targets.Num() <= 0)
	{
		return;
	}

	// 获取任务的自定义数据结构
	FBXTPTrackWeaponCollisionContext& TPC = InOutRTTData.DynamicData.GetMutable<FBXTPTrackWeaponCollisionContext>();
	TPC.CurrentCount = 0;
	TPC.StartCheckTime = UBXFunctionLibrary::GetClientTimeSeconds(Targets[0]) - InOutRTTData.RunTime;
	TPC.UpdateInterval = (Task->Duration - 0.005f) / FMath::Max(Task->Count, 1);
	
	// 获取对应的装备
	TPC.Gears.Reset();
	for (TArray<AActor*>::TIterator It(Targets); It; ++It)
	{
		UBXGearComponent* GearComponent = (*It)->FindComponentByClass<UBXGearComponent>();
		if (!IsValid(GearComponent))
		{
			continue;
		}

		ABXGear* Gear = GearComponent->GetUsingGear(Task->WeaponSlot);
		if (!IsValid(Gear))
		{
			continue;
		}

		TPC.Gears.AddUnique(Gear);
	}
	
	// 设置下一次更新时间
	InOutRTTData.NextTick = FMath::Max(0.0f, TPC.UpdateInterval - InOutRTTData.RunTime);
}

void UBXTPTrackWeaponCollision::Update(FBXTLRunTimeData& InOutRTData, FBXTLSectionRTData& InOutRTSData, FBXTLTaskRTData& InOutRTTData, float InDeltaTime)
{
	UBXTTrackWeaponCollision* Task = Cast<UBXTTrackWeaponCollision>(InOutRTTData.Task);
	if (!Task)
	{
		return;
	}

	// 获取任务的自定义数据结构
	FBXTPTrackWeaponCollisionContext& TPC = InOutRTTData.DynamicData.GetMutable<FBXTPTrackWeaponCollisionContext>();
	if (TPC.Gears.Num() <= 0 || TPC.CurrentCount >= Task->Count)
	{
		// 不再更新
		InOutRTTData.NextTick = -1.0f;
		return;
	}
	
	// 计算理想检测次数
	int32 TargetCount = FMath::Min(FMath::FloorToInt(InOutRTTData.RunTime / TPC.UpdateInterval), Task->Count);
	float StartTime = TPC.StartCheckTime + TPC.CurrentCount * TPC.UpdateInterval;
	float EndTime = TPC.StartCheckTime + TargetCount * TPC.UpdateInterval;
	
	// 进行碰撞检测
	FBXTHitResults FinalResults;
	TArray<FHitResult> HitResults;
	CollisionCheck(TPC.Gears, StartTime, EndTime, Task, HitResults);

	// 根据次数以及冷却，来触发事件
	int32 FullIndex = UBXFunctionLibrary::GetTaskFullIndex(InOutRTData.Timeline, Task);
	while (TPC.CurrentCount < TargetCount)
	{
		TPC.CurrentCount += 1;
		StartTime += TPC.UpdateInterval;

		// 根据冷却，决定是否合法
		FinalResults.Results.Reset();
		for (TArray<FHitResult>::TIterator It(HitResults); It; ++It)
		{
			if (CheckCoolDownCompleted(Task, *It, StartTime, TPC))
			{
				FinalResults.Results.Add(*It);
			}
		}

		int64 Scope = UBXTProcessor::GenerateContextScope(InOutRTData, InOutRTTData);
		// 触发成功事件，并写入碰撞数据
		if (FinalResults.Results.Num() > 0)
		{
			UBXTProcessor::AddPendingTask(InOutRTData, InOutRTSData, InOutRTTData, Scope, BXGameplayTags::BXTEvent_Success);
			UBXTProcessor::WriteContextData<FBXTHitResults>(InOutRTData, FullIndex, BXGameplayTags::BXTData_ColResults1, Scope, FinalResults);
		}
		// 触发失败事件
		else
		{
			UBXTProcessor::AddPendingTask(InOutRTData, InOutRTSData, InOutRTTData, Scope, BXGameplayTags::BXTEvent_Failure);
		}
	}

	// 设置下一次更新间隔
	InOutRTTData.NextTick = TPC.UpdateInterval - (InOutRTTData.RunTime - TargetCount * TPC.UpdateInterval);
}
	
void UBXTPTrackWeaponCollision::End(FBXTLRunTimeData& InOutRTData, FBXTLSectionRTData& InOutRTSData, FBXTLTaskRTData& InOutRTTData, EBXTLFinishReason InReason)
{
	UBXTTrackWeaponCollision* Task = Cast<UBXTTrackWeaponCollision>(InOutRTTData.Task);
	if (!Task)
	{
		return;
	}

	// 获取任务的自定义数据结构
	FBXTPTrackWeaponCollisionContext& TPC = InOutRTTData.DynamicData.GetMutable<FBXTPTrackWeaponCollisionContext>();
	if (TPC.CurrentCount >= Task->Count)
	{
		return;
	}

	float StartTime = TPC.StartCheckTime + TPC.CurrentCount * TPC.UpdateInterval;
	float EndTime = TPC.StartCheckTime + Task->Count * TPC.UpdateInterval;
	
	// 进行碰撞检测
	FBXTHitResults FinalResults;
	TArray<FHitResult> HitResults;
	CollisionCheck(TPC.Gears, StartTime, EndTime, Task, HitResults);

	// 根据次数以及冷却，来触发事件
	int32 FullIndex = UBXFunctionLibrary::GetTaskFullIndex(InOutRTData.Timeline, Task);
	while (TPC.CurrentCount < Task->Count)
	{
		TPC.CurrentCount += 1;
		StartTime += TPC.UpdateInterval;

		// 根据冷却，决定是否合法
		FinalResults.Results.Reset();
		for (TArray<FHitResult>::TIterator It(HitResults); It; ++It)
		{
			if (CheckCoolDownCompleted(Task, *It, StartTime, TPC))
			{
				FinalResults.Results.Add(*It);
			}
		}

		int64 Scope = UBXTProcessor::GenerateContextScope(InOutRTData, InOutRTTData);
		// 触发成功事件，并写入碰撞数据
		if (FinalResults.Results.Num() > 0)
		{
			UBXTProcessor::AddPendingTask(InOutRTData, InOutRTSData, InOutRTTData, Scope, BXGameplayTags::BXTEvent_Success);
			UBXTProcessor::WriteContextData<FBXTHitResults>(InOutRTData, FullIndex, BXGameplayTags::BXTData_ColResults1, Scope, FinalResults);
		}
		// 触发失败事件
		else
		{
			UBXTProcessor::AddPendingTask(InOutRTData, InOutRTSData, InOutRTTData, Scope, BXGameplayTags::BXTEvent_Failure);
		}
	}
}

void UBXTPTrackWeaponCollision::CollisionCheck(const TArray<ABXGear*>& InGears, float InStartTime, float InEndTime, UBXTTrackWeaponCollision* InTask, TArray<FHitResult>& OutHitResults)
{
	OutHitResults.Reset();
	
	if (InGears.Num() <= 0)
	{
		return;
	}
	
	
}
