#include "BXTPCollision.h"

#include "BXColdWeapon.h"
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

AActor* UBXTPCollision::GetCollisionRequester(UActorComponent* InComponent)
{
	if (!IsValid(InComponent))
	{
		return nullptr;
	}
	
	return InComponent->GetOwner();
}






void UBXTPTrackHitBox::Start(FBXTLRunTimeData& InOutRTData, FBXTLSectionRTData& InOutRTSData, FBXTLTaskRTData& InOutRTTData)
{
	UBXTTrackHitBox* Task = Cast<UBXTTrackHitBox>(InOutRTTData.Task);
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
	FBXTPTrackHitBoxContext& TPC = InOutRTTData.DynamicData.GetMutable<FBXTPTrackHitBoxContext>();
	TPC.CurrentCount = 0;
	TPC.StartTime = Targets[0]->GetWorld()->GetTimeSeconds() - InOutRTTData.RunTime;

	// 当前时间
	float CurrentTime = FPlatformTime::Seconds();
	
	// 获取角色的碰撞盒组件
	FTransform HBTransform;
 	TPC.ShapeComponents.Reset();
	for (TArray<AActor*>::TIterator It(Targets); It; ++It)
	{
		UBXShapeComponent* ShapeComponent = (*It)->FindComponentByClass<UBXShapeComponent>();
		if (!IsValid(ShapeComponent))
		{
			continue;
		}

		// 添加碰撞盒信息
		TPC.ShapeComponents.AddUnique(ShapeComponent);
	}
	
	// 设置下一次更新时间
	InOutRTTData.NextTick = FMath::Max(0.0f, Task->Interval - InOutRTTData.RunTime);
}

void UBXTPTrackHitBox::Update(FBXTLRunTimeData& InOutRTData, FBXTLSectionRTData& InOutRTSData, FBXTLTaskRTData& InOutRTTData, float InDeltaTime)
{
	UBXTTrackHitBox* Task = Cast<UBXTTrackHitBox>(InOutRTTData.Task);
	if (!Task)
	{
		return;
	}

	// 获取任务的自定义数据结构
	FBXTPTrackHitBoxContext& TPC = InOutRTTData.DynamicData.GetMutable<FBXTPTrackHitBoxContext>();
	if (TPC.ShapeComponents.Num() <= 0 || TPC.CurrentCount >= Task->Count)
	{
		// 不再更新
		InOutRTTData.NextTick = -1.0f;
		return;
	}
	
	// 计算理想检测次数
	int32 TargetCount = FMath::Min(FMath::FloorToInt(InOutRTTData.RunTime / Task->Interval), Task->Count);

	// 碰撞检测 并 触发事件
	CollisionCheck(InOutRTData, InOutRTSData, InOutRTTData, TargetCount);

	// 设置下一次更新间隔
	InOutRTTData.NextTick = Task->Interval - (InOutRTTData.RunTime - TargetCount * Task->Interval);
}
	
void UBXTPTrackHitBox::End(FBXTLRunTimeData& InOutRTData, FBXTLSectionRTData& InOutRTSData, FBXTLTaskRTData& InOutRTTData, EBXTLFinishReason InReason)
{
	UBXTTrackHitBox* Task = Cast<UBXTTrackHitBox>(InOutRTTData.Task);
	if (!Task)
	{
		return;
	}

	// 补全碰撞检测次数 并 触发事件
	if (InReason == EBXTLFinishReason::FR_EndOfLife && Task->Count > 0)
	{
		CollisionCheck(InOutRTData, InOutRTSData, InOutRTTData, Task->Count);
	}
	else
	{
		CollisionCheck(InOutRTData, InOutRTSData, InOutRTTData, FMath::Min(FMath::FloorToInt(InOutRTTData.RunTime / Task->Interval), Task->Count));
	}
}

void UBXTPTrackHitBox::CollisionCheck(FBXTLRunTimeData& InOutRTData, FBXTLSectionRTData& InOutRTSData, FBXTLTaskRTData& InOutRTTData, int32 InTargetCheckCount)
{
	UBXTTrackHitBox* Task = Cast<UBXTTrackHitBox>(InOutRTTData.Task);
	if (!Task)
	{
		return;
	}

	// 获取任务的自定义数据结构
	FBXTPTrackHitBoxContext& TPC = InOutRTTData.DynamicData.GetMutable<FBXTPTrackHitBoxContext>();

	// 根据次数以及冷却，来触发事件
	int32 FullIndex = UBXFunctionLibrary::GetTaskFullIndex(InOutRTData.Timeline, Task);
	
	// 临时变量声明
	FBXCParameter Parameter;
	FBXTHitResults FinalResults;
	FTransform WorldTransform;
	TArray<FHitResult> HitResults;
	TArray<FHitResult> TempHitResults;
	TArray<FTransform> HitBoxTransforms;
	int32 Step = FMath::FloorToInt(Task->Interval / 0.1f) + 1;
	float StepTime = Task->Interval / Step;

	// 碰撞查询
	float StartTime = TPC.CurrentCount * Task->Interval;
	float StopTime = InTargetCheckCount * Task->Interval;
	for (float CurrentTime = StartTime; CurrentTime < StopTime - 1e-6; CurrentTime = CurrentTime + Task->Interval)
	{
		for (TArray<UBXShapeComponent*>::TIterator SComp(TPC.ShapeComponents); SComp; ++SComp)
		{
			UBXShapeComponent* ShapeComponent = (*SComp);
			if (!IsValid(ShapeComponent))
			{
				continue;
			}
		
			// 获取碰撞请求者
			Parameter.Requester = GetCollisionRequester(ShapeComponent);
	
			// 进行碰撞检测
			for (TMap<FGameplayTag, FBXTrajectoryPoints>::TIterator It(Task->BakedHBTrajectoryPoints); It; ++It)
			{
				FBXShapeInformation* SInfo = ShapeComponent->ShapeInformations.Find(It->Key);
				if (!SInfo)
				{
					continue;
				}

				for (int32 CurrentStep = 0; CurrentStep < Step; ++CurrentStep)
				{
					float STime = CurrentTime + CurrentStep * StepTime;
					float ETime = CurrentTime + (CurrentStep + 1) * StepTime;

					// 获取碰撞盒位置列表
					HitBoxTransforms.Reset();
					TArray<FBXTrajectoryPoint>& Points = It->Value.List;
					for (int32 i = 0; i < Points.Num(); ++i)
					{
						if (HitBoxTransforms.IsEmpty())
						{
							if (Points[i].Time >= STime)
							{
								UBXTProcessor::GetTargetTransformByWorldTime(Parameter.Requester, TPC.StartTime + STime, WorldTransform);
								if (i == 0)
								{
									HitBoxTransforms.Add(Points[i].Transform * WorldTransform);
								}
								else
								{
									FVector Location = FMath::Lerp(Points[i - 1].Transform.GetLocation(), Points[i].Transform.GetLocation(), (STime - Points[i - 1].Time) / (Points[i].Time - Points[i - 1].Time));
									FQuat Rotation = FQuat::Slerp(Points[i - 1].Transform.GetRotation(), Points[i].Transform.GetRotation(), (STime - Points[i - 1].Time) / (Points[i].Time - Points[i - 1].Time));
									FVector Scale = FMath::Lerp(Points[i - 1].Transform.GetScale3D(), Points[i].Transform.GetScale3D(), (STime - Points[i - 1].Time) / (Points[i].Time - Points[i - 1].Time));
									HitBoxTransforms.Add(FTransform(Rotation, Location, Scale) * WorldTransform);
								}
							}	
						}
						else
						{
							if (Points[i].Time >= ETime)
							{
								UBXTProcessor::GetTargetTransformByWorldTime(Parameter.Requester, TPC.StartTime + ETime, WorldTransform);
								FVector Location = FMath::Lerp(Points[i - 1].Transform.GetLocation(), Points[i].Transform.GetLocation(), (ETime - Points[i - 1].Time) / (Points[i].Time - Points[i - 1].Time));
								FQuat Rotation = FQuat::Slerp(Points[i - 1].Transform.GetRotation(), Points[i].Transform.GetRotation(), (ETime - Points[i - 1].Time) / (Points[i].Time - Points[i - 1].Time));
								FVector Scale = FMath::Lerp(Points[i - 1].Transform.GetScale3D(), Points[i].Transform.GetScale3D(), (ETime - Points[i - 1].Time) / (Points[i].Time - Points[i - 1].Time));
								HitBoxTransforms.Add(FTransform(Rotation, Location, Scale) * WorldTransform);
								break;
							}
							else
							{
								UBXTProcessor::GetTargetTransformByWorldTime(Parameter.Requester, TPC.StartTime + Points[i].Time, WorldTransform);
								HitBoxTransforms.Add(Points[i].Transform * WorldTransform);
							}
						}
					}
					if (HitBoxTransforms.IsEmpty())
					{
						continue;
					}

					// 进行碰撞检测
					if (HitBoxTransforms.Num() == 1)
					{
						TempHitResults.Reset();
						Parameter.StartLocation = HitBoxTransforms[0].GetLocation();
						Parameter.StartRotation = HitBoxTransforms[0].GetRotation().Rotator();
						Parameter.EndLocation = FVector::ZeroVector;
						Parameter.EndRotation = FRotator::ZeroRotator;
						Parameter.Scale = HitBoxTransforms[0].GetScale3D();
				
						switch (SInfo->ShapeType)
						{
						case EBXShapeType::ST_Sphere:
							TempHitResults = UBXCollisionLibrary::SphereCheck(Parameter, Task->ObjectTypes, SInfo->ShapeSize.X, Task->EngineFilter);
							break;
						case EBXShapeType::ST_Capsule:
							TempHitResults = UBXCollisionLibrary::CapsuleCheck(Parameter, Task->ObjectTypes, FVector2D(SInfo->ShapeSize.X), Task->EngineFilter);
							break;
						case EBXShapeType::ST_Box:
							TempHitResults = UBXCollisionLibrary::BoxCheck(Parameter, Task->ObjectTypes, SInfo->ShapeSize, Task->EngineFilter);
							break;
						default:
							break;
						}
					}
					else
					{
						for (int32 i = 1; i < HitBoxTransforms.Num(); ++i)
						{
							HitResults.Reset();
							Parameter.StartLocation = HitBoxTransforms[i - 1].GetLocation();
							Parameter.StartRotation = HitBoxTransforms[i - 1].GetRotation().Rotator();
							Parameter.EndLocation = HitBoxTransforms[i].GetLocation();
							Parameter.EndRotation = HitBoxTransforms[i].GetRotation().Rotator();
							Parameter.Scale = (HitBoxTransforms[i - 1].GetScale3D() + HitBoxTransforms[i].GetScale3D()) * 0.5f;

							switch (SInfo->ShapeType)
							{
							case EBXShapeType::ST_Sphere:
								TempHitResults = UBXCollisionLibrary::SphereCheck(Parameter, Task->ObjectTypes, SInfo->ShapeSize.X, Task->EngineFilter);
								break;
							case EBXShapeType::ST_Capsule:
								TempHitResults = UBXCollisionLibrary::CapsuleCheck(Parameter, Task->ObjectTypes, FVector2D(SInfo->ShapeSize.X), Task->EngineFilter, Task->SweepAngleStep);
								break;
							case EBXShapeType::ST_Box:
								TempHitResults = UBXCollisionLibrary::BoxCheck(Parameter, Task->ObjectTypes, SInfo->ShapeSize, Task->EngineFilter, Task->SweepAngleStep);
								break;
							default:
								break;
							}
						}	
					}

					// 碰撞结果合并
					UBXCollisionLibrary::CombineCollisionResults(TempHitResults, HitResults);	
				}
			}
		}
		
		// 根据冷却，决定是否合法
		FinalResults.Results.Reset();
		for (TArray<FHitResult>::TIterator It(HitResults); It; ++It)
		{
			if (CheckCoolDownCompleted(Task, *It, CurrentTime, TPC))
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

	TPC.CurrentCount = InTargetCheckCount;
}






void UBXTPTrackWeaponHitBox::Start(FBXTLRunTimeData& InOutRTData, FBXTLSectionRTData& InOutRTSData, FBXTLTaskRTData& InOutRTTData)
{
	UBXTTrackWeaponHitBox* Task = Cast<UBXTTrackWeaponHitBox>(InOutRTTData.Task);
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
	FBXTPTrackHitBoxContext& TPC = InOutRTTData.DynamicData.GetMutable<FBXTPTrackHitBoxContext>();
	TPC.CurrentCount = 0;
	TPC.StartTime = Targets[0]->GetWorld()->GetTimeSeconds() - InOutRTTData.RunTime;
	
	// 当前时间
	float CurrentTime = FPlatformTime::Seconds();
	
	// 获取角色的碰撞盒组件
	FTransform HBTransform;
	TPC.ShapeComponents.Reset();
	for (TArray<AActor*>::TIterator It(Targets); It; ++It)
	{
		UBXGearComponent* GearComponent = (*It)->FindComponentByClass<UBXGearComponent>();
		if (!IsValid(GearComponent))
		{
			continue;
		}

		ABXColdWeapon* Gear = Cast<ABXColdWeapon>(GearComponent->GetUsingGear(Task->WeaponSlot));
		if (!IsValid(Gear))
		{
			continue;
		}

		// 添加碰撞盒信息
		TPC.ShapeComponents.AddUnique(Gear->GetHitBoxComponent());
	}
	
	// 设置下一次更新时间
	InOutRTTData.NextTick = FMath::Max(0.0f, Task->Interval - InOutRTTData.RunTime);
}

AActor* UBXTPTrackWeaponHitBox::GetCollisionRequester(UActorComponent* InComponent)
{
	if (!IsValid(InComponent))
	{
		return nullptr;
	}

	ABXGear* Gear = Cast<ABXGear>(InComponent->GetOwner());
	if (!IsValid(Gear) || !IsValid(Gear->OwnerComponent))
	{
		return nullptr;
	}

	return Gear->OwnerComponent->GetOwner();
}
