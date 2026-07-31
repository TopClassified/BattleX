#include "BXTPCollision.h"

#include "Components/SkeletalMeshComponent.h"

#include "BXMeleeWeapon.h"
#include "BXGearComponent.h"
#include "BXCharacterMovementComponent.h"



bool UBXTPCollision::CheckCoolDownCompleted(UBXTCollision* InTask, const FHitResult& InHitResult, float InCheckTime, UPARAM(ref) FBXTPCollisionContext& InOutRTData)
{
	if (!IsValid(InTask))
	{
		return false;
	}

	// 先执行所有检查,全部通过后再写入冷却信息,避免脚本检查失败时已写入冷却导致下次误判
	if (bUseNativeCheckCoolDown && !CheckCoolDown(InTask, InHitResult, InCheckTime, InOutRTData))
	{
		return false;
	}

	if (bUseScriptCheckCoolDown && !ScriptCheckCoolDown(InTask, InHitResult, InCheckTime, InOutRTData))
	{
		return false;
	}

	// 所有检查通过后写入冷却信息
	if (bUseNativeCheckCoolDown && IsValid(InHitResult.GetActor()))
	{
		InOutRTData.CoolDownInformations.Add(InHitResult.GetActor(), InCheckTime);
	}

	return true;
}

bool UBXTPCollision::CheckCoolDown(UBXTCollision* InTask, const FHitResult& InHitResult, float InCheckTime, FBXTPCollisionContext& InOutRTData)
{
	if (!IsValid(InTask) || !IsValid(InHitResult.GetActor()))
	{
		return false;
	}

	// 仅检查冷却,不写入,由CheckCoolDownCompleted统一写入
	if (float* FindResult = InOutRTData.CoolDownInformations.Find(InHitResult.GetActor()))
	{
		if (InCheckTime - (*FindResult) < InTask->CoolDown)
		{
			return false;
		}
	}

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
	TPC.LastCheckTime = 0.0f;
	TPC.StartTime = Targets[0]->GetWorld()->GetTimeSeconds() - InOutRTTData.RunTime;

	// 获取角色的碰撞盒组件
	TPC.ShapeComponents.Reset();
	TPC.CachedPolylineFrameLinks.Reset();
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

	// 每帧更新
	InOutRTTData.NextTick = 0.0f;
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
	if (TPC.ShapeComponents.Num() <= 0)
	{
		// 不再更新
		InOutRTTData.NextTick = -1.0f;
		return;
	}

	// 碰撞检测 并 触发事件
	CollisionCheck(InOutRTData, InOutRTSData, InOutRTTData, InOutRTTData.RunTime);

	// 每帧更新
	InOutRTTData.NextTick = 0.0f;
}
	
void UBXTPTrackHitBox::End(FBXTLRunTimeData& InOutRTData, FBXTLSectionRTData& InOutRTSData, FBXTLTaskRTData& InOutRTTData, EBXTLFinishReason InReason)
{
	UBXTTrackHitBox* Task = Cast<UBXTTrackHitBox>(InOutRTTData.Task);
	if (!Task)
	{
		return;
	}

	// 结束时覆盖剩余时间范围
	CollisionCheck(InOutRTData, InOutRTSData, InOutRTTData, InOutRTTData.RunTime);
}

void UBXTPTrackHitBox::CollisionCheck(FBXTLRunTimeData& InOutRTData, FBXTLSectionRTData& InOutRTSData, FBXTLTaskRTData& InOutRTTData, float InTargetCheckTime)
{
	UBXTTrackHitBox* Task = Cast<UBXTTrackHitBox>(InOutRTTData.Task);
	if (!Task)
	{
		return;
	}

	// 获取任务的自定义数据结构
	FBXTPTrackHitBoxContext& TPC = InOutRTTData.DynamicData.GetMutable<FBXTPTrackHitBoxContext>();

	// 时间范围(RunTime空间,相对于任务开始)
	float StartTime = TPC.LastCheckTime;
	float StopTime = InTargetCheckTime;
	if (StopTime <= StartTime)
	{
		return;
	}

	int32 FullIndex = UBXFunctionLibrary::GetTaskFullIndex(InOutRTData.Timeline, Task);

	// 临时变量声明
	FBXCParameter Parameter;
	FBXTHitResults FinalResults;
	FTransform WorldTransform;
	TArray<FHitResult> HitResults;
	TArray<FHitResult> TempHitResults;
	TArray<FTransform> HitBoxTransforms;

	if (Task->BoneSampledTrajectory.List.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("[BXTrackColl] CollisionCheck skip: empty trajectory, Task=%s"), *Task->GetName());
		return;
	}

	// 映射到烘焙点时间轴(section time,烘焙点Time含Task->StartTime偏移)
	TArray<FBXTrajectoryPoint>& Points = Task->BoneSampledTrajectory.List;
	float SearchStartTime = StartTime + Task->StartTime;
	float SearchStopTime = StopTime + Task->StartTime;

	UE_LOG(LogTemp, Log, TEXT("[BXTrackColl] CollisionCheck: Task=%s Trajectory=%d TimeRange=[%.4f,%.4f]"),
		*Task->GetName(), Points.Num(), StartTime, StopTime);

	// 遍历每个ShapeComponent
	for (TArray<UBXShapeComponent*>::TIterator SComp(TPC.ShapeComponents); SComp; ++SComp)
	{
		UBXShapeComponent* ShapeComponent = (*SComp);
		if (!IsValid(ShapeComponent))
		{
			continue;
		}

		// 获取碰撞请求者
		Parameter.Requester = GetCollisionRequester(ShapeComponent);

		// 缓存Requester的CharacterMovementComponent,避免每个采样点都FindComponentByClass
		UBXCharacterMovementComponent* RequesterCMC = IsValid(Parameter.Requester) ? Parameter.Requester->FindComponentByClass<UBXCharacterMovementComponent>() : nullptr;

		// RequesterCMC无效时回退到Requester当前Transform,避免WorldTransform残留旧值
		auto GetRequesterFallback = [&]() -> FTransform
		{
			return IsValid(Parameter.Requester) ? Parameter.Requester->GetActorTransform() : FTransform::Identity;
		};

		// 按RunTime相对时间获取历史Mesh Transform
		auto GetWorldTransformAt = [&](float RunTimeRelative) -> FTransform
		{
			if (IsValid(RequesterCMC))
			{
				return RequesterCMC->GetHistoryMeshTransformByTime(TPC.StartTime + RunTimeRelative);
			}
			return GetRequesterFallback();
		};

		// 在烘焙轨迹上按浮点时间采样
		auto SampleTrajectoryAt = [&](float SearchTime) -> FTransform
		{
			if (Points.Num() <= 0)
			{
				return FTransform::Identity;
			}
			if (SearchTime <= Points[0].Time)
			{
				return Points[0].Transform;
			}
			if (SearchTime >= Points.Last().Time)
			{
				return Points.Last().Transform;
			}
			int32 Idx = 0;
			while (Idx < Points.Num() && Points[Idx].Time < SearchTime)
			{
				++Idx;
			}
			if (Idx <= 0 || Idx >= Points.Num())
			{
				return Points[FMath::Clamp(Idx, 0, Points.Num() - 1)].Transform;
			}
			float Alpha = FMath::Clamp((SearchTime - Points[Idx - 1].Time) / (Points[Idx].Time - Points[Idx - 1].Time + 1e-8f), 0.0f, 1.0f);
			FVector Location = FMath::Lerp(Points[Idx - 1].Transform.GetLocation(), Points[Idx].Transform.GetLocation(), Alpha);
			FQuat Rotation = FQuat::Slerp(Points[Idx - 1].Transform.GetRotation(), Points[Idx].Transform.GetRotation(), Alpha);
			FVector Scale = FMath::Lerp(Points[Idx - 1].Transform.GetScale3D(), Points[Idx].Transform.GetScale3D(), Alpha);
			return FTransform(Rotation, Location, Scale);
		};

		// 进行碰撞检测
		for (TMap<FGameplayTag, FBXShapeInformation>::TIterator It(ShapeComponent->ShapeInformations); It; ++It)
		{
			// 按HitBoxTag过滤,未设置表示匹配任意碰撞盒
			if (Task->HitBoxTag.IsValid() && Task->HitBoxTag != It->Key)
			{
				continue;
			}

			FBXShapeInformation* SInfo = &It->Value;

			// 收集本帧时间范围内的轨迹采样点
			HitBoxTransforms.Reset();

			// 添加起始采样点
			{
				FTransform SampledTransform = SampleTrajectoryAt(SearchStartTime);
				WorldTransform = GetWorldTransformAt(StartTime);
				HitBoxTransforms.Add(ComputeSampledHitBoxTransform(SampledTransform, WorldTransform, ShapeComponent, *SInfo));
			}

			// 添加范围内烘焙点(严格介于起止时间之间)
			for (int32 i = 0; i < Points.Num(); ++i)
			{
				if (Points[i].Time <= SearchStartTime || Points[i].Time >= SearchStopTime)
				{
					continue;
				}
				float PointRelTime = Points[i].Time - Task->StartTime;
				WorldTransform = GetWorldTransformAt(PointRelTime);
				HitBoxTransforms.Add(ComputeSampledHitBoxTransform(Points[i].Transform, WorldTransform, ShapeComponent, *SInfo));
			}

			// 添加结束采样点
			{
				FTransform SampledTransform = SampleTrajectoryAt(SearchStopTime);
				WorldTransform = GetWorldTransformAt(StopTime);
				HitBoxTransforms.Add(ComputeSampledHitBoxTransform(SampledTransform, WorldTransform, ShapeComponent, *SInfo));
			}

			if (HitBoxTransforms.Num() <= 0)
			{
				continue;
			}

			// 获取或创建跨帧衔接信息
			FBXCPolylineFrameLink& FrameLink = TPC.CachedPolylineFrameLinks.FindOrAdd(ShapeComponent).FindOrAdd(It->Key);

			// 调用Library曲线扫描接口
			switch (SInfo->ShapeType)
			{
			case EBXShapeType::ST_Sphere:
				TempHitResults = UBXCollisionLibrary::SphereSweepAlongCurve(Parameter.Requester, Task->ObjectTypes, SInfo->ShapeSize.X, Task->EngineFilter, HitBoxTransforms, Task->PolylineConfig, FrameLink);
				break;
			case EBXShapeType::ST_Capsule:
				TempHitResults = UBXCollisionLibrary::CapsuleSweepAlongCurve(Parameter.Requester, Task->ObjectTypes, FVector2D(SInfo->ShapeSize.X), Task->EngineFilter, HitBoxTransforms, Task->PolylineConfig, FrameLink);
				break;
			case EBXShapeType::ST_Box:
				TempHitResults = UBXCollisionLibrary::BoxSweepAlongCurve(Parameter.Requester, Task->ObjectTypes, SInfo->ShapeSize, Task->EngineFilter, HitBoxTransforms, Task->PolylineConfig, FrameLink);
				break;
			default:
				break;
			}
			UBXCollisionLibrary::CombineCollisionResults(TempHitResults, HitResults);
		}
	}

	// 根据冷却，决定是否合法
	FinalResults.Results.Reset();
	for (TArray<FHitResult>::TIterator It(HitResults); It; ++It)
	{
		if (CheckCoolDownCompleted(Task, *It, StopTime, TPC))
		{
			FinalResults.Results.Add(*It);
		}
	}

	UE_LOG(LogTemp, Log, TEXT("[BXTrackColl] CollisionCheck result: Task=%s TimeRange=[%.4f,%.4f] Hits=%d (after cooldown from %d)"),
		*Task->GetName(), StartTime, StopTime, FinalResults.Results.Num(), HitResults.Num());

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

	TPC.LastCheckTime = StopTime;
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
	TPC.LastCheckTime = 0.0f;
	TPC.StartTime = Targets[0]->GetWorld()->GetTimeSeconds() - InOutRTTData.RunTime;

	// 获取角色的碰撞盒组件
	TPC.ShapeComponents.Reset();
	TPC.CachedPolylineFrameLinks.Reset();
	for (TArray<AActor*>::TIterator It(Targets); It; ++It)
	{
		UBXGearComponent* GearComponent = (*It)->FindComponentByClass<UBXGearComponent>();
		if (!IsValid(GearComponent))
		{
			continue;
		}

		ABXMeleeWeapon* Gear = Cast<ABXMeleeWeapon>(GearComponent->GetUsingGear(Task->WeaponSlot));
		if (!IsValid(Gear))
		{
			continue;
		}

		// 添加碰撞盒信息
		TPC.ShapeComponents.AddUnique(Gear->GetHitBoxComponent());
	}

	// 每帧更新
	InOutRTTData.NextTick = 0.0f;
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

FTransform UBXTPTrackHitBox::ComputeSampledHitBoxTransform
(
	const FTransform& BakedPoint,
	const FTransform& CharMeshWorld,
	UBXShapeComponent* InShapeComp,
	const FBXShapeInformation& InShapeInfo
)
{
	// 默认实现:烘焙的角色骨骼CS轨迹 × 角色Mesh历史世界Transform
	return BakedPoint * CharMeshWorld;
}

FTransform UBXTPTrackWeaponHitBox::ComputeSampledHitBoxTransform
(
	const FTransform& BakedPoint,
	const FTransform& CharMeshWorld,
	UBXShapeComponent* InShapeComp,
	const FBXShapeInformation& InShapeInfo
)
{
	// UE A*B为"先A后B",偏移链从内到外: HitBoxRelation*WeaponBoneCS*WeaponAttachRelation*BakedPoint*CharMeshWorld
	ABXGear* Gear = IsValid(InShapeComp) ? Cast<ABXGear>(InShapeComp->GetOwner()) : nullptr;
	if (!IsValid(Gear))
	{
		UE_LOG(LogTemp, Warning, TEXT("ComputeSampledHitBoxTransform: Gear invalid, fallback to BakedPoint*CharMeshWorld"));
		return BakedPoint * CharMeshWorld;
	}

	USkeletalMeshComponent* WeaponMeshComp = nullptr;
	for (UActorComponent* Comp : Gear->GetComponents())
	{
		if (USkeletalMeshComponent* SkMeshComp = Cast<USkeletalMeshComponent>(Comp))
		{
			WeaponMeshComp = SkMeshComp;
			break;
		}
	}
	if (!IsValid(WeaponMeshComp))
	{
		// StaticMesh武器无骨骼,WeaponBoneCS用Identity
		WeaponMeshComp = Gear->AttachTarget;
	}

	FTransform WeaponBoneCS = FTransform::Identity;
	if (IsValid(WeaponMeshComp) && !InShapeInfo.Socket.BoneName.IsNone())
	{
		WeaponBoneCS = WeaponMeshComp->GetSocketTransform(InShapeInfo.Socket.BoneName, RTS_Component);
	}

	FTransform WeaponAttachRelation = FTransform::Identity;
	if (IsValid(Gear->OwnerComponent))
	{
		FGameplayTag GearSlot = Gear->OwnerComponent->GetUsingGearSlot(Gear);
		if (GearSlot == BXGameplayTags::BXGearSlot_Default)
		{
			UE_LOG(LogTemp, Warning, TEXT("ComputeSampledHitBoxTransform: GetUsingGearSlot returned Default, Gear=%s, WeaponAttachRelation falls back to Identity"),
				*Gear->GetName());
		}
		for (const FBXGearAttachmentConfig& Config : Gear->AttachmentConfigs)
		{
			if (Config.Slot == GearSlot)
			{
				WeaponAttachRelation = Config.Relation;
				break;
			}
		}
	}

	return InShapeInfo.Relation * WeaponBoneCS * WeaponAttachRelation * BakedPoint * CharMeshWorld;
}
