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

	// 获取任务的自定义数据结构
	FBXTPTrackWeaponCollisionContext& TPC = InOutRTTData.DynamicData.GetMutable<FBXTPTrackWeaponCollisionContext>();
	TPC.CurrentCount = 0;
	TPC.UpdateInterval = (Task->Duration - 0.005f) / FMath::Max(Task->Count, 1);

	// 获取任务目标
	TArray<AActor*> Targets;
	UBXTProcessor::GetTargetActorList(InOutRTData, InOutRTTData, Targets);
	if (Targets.Num() <= 0)
	{
		return;
	}
	
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

	// 记录时间点
	TPC.LastCheckTime = UBXFunctionLibrary::GetClientTimeSeconds(Targets[0]) - InOutRTTData.RunTime;
	
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
	if (TPC.Gears.Num() <= 0)
	{
		// 不再更新
		InOutRTTData.NextTick = -1.0f;
		return;
	}
	
	// 计算理想检测次数
	float ExpectTime = TPC.LastCheckTime;
	int32 TargetCount = FMath::FloorToInt(InOutRTTData.RunTime / TPC.UpdateInterval);
	
	// 进行碰撞检测
	FBXTHitResults FinalResults;
	TArray<FHitResult> HitResults;
	CollisionCheck(TPC, Task, HitResults);

	// 根据次数以及冷却，来触发事件
	int32 FullIndex = UBXFunctionLibrary::GetTaskFullIndex(InOutRTData.Timeline, Task);
	while (TPC.CurrentCount < TargetCount)
	{
		TPC.CurrentCount += 1;
		ExpectTime += TPC.UpdateInterval;

		// 根据冷却，决定是否合法
		FinalResults.Results.Reset();
		for (TArray<FHitResult>::TIterator It(HitResults); It; ++It)
		{
			if (CheckCoolDownCompleted(Task, *It, ExpectTime, TPC))
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

	// 计算理想检测次数
	float ExpectTime = TPC.LastCheckTime;
	int32 TargetCount = FMath::FloorToInt(InOutRTTData.RunTime / TPC.UpdateInterval);
	
	// 进行碰撞检测
	FBXTHitResults FinalResults;
	TArray<FHitResult> HitResults;
	CollisionCheck(TPC, Task, HitResults);

	// 根据次数以及冷却，来触发事件
	int32 FullIndex = UBXFunctionLibrary::GetTaskFullIndex(InOutRTData.Timeline, Task);
	while (TPC.CurrentCount < TargetCount)
	{
		TPC.CurrentCount += 1;
		ExpectTime += TPC.UpdateInterval;

		// 根据冷却，决定是否合法
		FinalResults.Results.Reset();
		for (TArray<FHitResult>::TIterator It(HitResults); It; ++It)
		{
			if (CheckCoolDownCompleted(Task, *It, ExpectTime, TPC))
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

void UBXTPTrackWeaponCollision::CollisionCheck(FBXTPTrackWeaponCollisionContext& InOutContext, UBXTTrackWeaponCollision* InTask, TArray<FHitResult>& OutHitResults)
{
	OutHitResults.Reset();

	if (InOutContext.Gears.Num() <= 0)
	{
		return;
	}
	
	// 如果使用缓存数据
	if (InTask->bCacheRTransform && InTask->CacheRTransfomList.Num() > 0)
	{
		
	}
	// 从武器中获取
	else
	{
		// 遍历所有的装备对象
		TArray<FHitResult> TempResults;
		for (TArray<ABXGear*>::TIterator It(InOutContext.Gears); It; ++It)
		{
			ABXGear* Gear = *It;
			if (!IsValid(Gear))
			{
				continue;
			}

			// 获取武器碰撞盒的命中数据
			Gear->GetHitResults(InOutContext.LastCheckTime, InTask->WeaponHitBoxTags, InTask->ObjectTypes, InTask->EngineFilter, TempResults, InTask->CollisionOptimizationRules);

			// 检查角色合法性
			for (TArray<FHitResult>::TIterator HR(TempResults); HR; ++HR)
			{
				if (!CheckActor(InTask, Gear->OwnerComponent->GetOwner(), *HR))
				{
					HR.RemoveCurrentSwap();
				}
			}
			
			// 结果去重合并
			UBXCollisionLibrary::CombineCollisionResults(TempResults, OutHitResults);
		}

		// 进行数量限制
		if (OutHitResults.Num() > InTask->Limit)
		{
			LimitHitResults(InTask->LimitLogic, InTask->Limit, OutHitResults);
		}
	}
	
	// 更新检测时间点
	InOutContext.LastCheckTime = UBXFunctionLibrary::GetClientTimeSeconds(InOutContext.Gears[0]);
}
