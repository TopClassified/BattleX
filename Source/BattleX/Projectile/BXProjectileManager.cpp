#include "BXProjectileManager.h"

#include "BXProjectileAsset.h"
#include "BXProjectileComponent.h"
#include "BXProjectileComputeWorker.h"
#include "BXProjectileSolver.h"
#include "BXSubSystem.h"
#include "BXSettings.h"
#include "BXFunctionLibrary.h"
#include "BXGameplayTags.h"
#include "BXEventManager.h"
#include "BXCollision.h"
#include "BXNetStructs.h"
#include "BXShapeComponent.h"
#include "BXBuffManager.h"
#include "BXBuffStructs.h"
#include "BXBuffAsset.h"
#include "BXSkillComponent.h"
#include "BXSkillAsset.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "Async/ParallelFor.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "GameFramework/GameStateBase.h"



DEFINE_LOG_CATEGORY(BXMGR_Projectile);



#pragma region Important
UBXProjectileManager* UBXProjectileManager::Get(UObject* InWorldContext)
{
	if (!InWorldContext)
	{
		return nullptr;
	}

	if (UWorld* World = InWorldContext->GetWorld())
	{
		if (UGameInstance* GI = World->GetGameInstance())
		{
			if (UBXSubSystem* BXSS = GI->GetSubsystem<UBXSubSystem>())
			{
				return BXSS->GetManagerByClass<UBXProjectileManager>();
			}
		}
	}

	return nullptr;
}

void UBXProjectileManager::Initialize()
{
	Super::Initialize();

	CollectProjectileAssets();

	FWorldDelegates::OnWorldCleanup.AddUObject(this, &UBXProjectileManager::OnWorldCleanupStart);

	// 定义子弹全局事件
	if (UBXEventManager* EventMgr = UBXEventManager::Get(this))
	{
		EventMgr->DefineEvent(BXGameplayTags::BXEvent_Projectile_Fired, FBXProjectileEventParameter::StaticStruct());
		EventMgr->DefineEvent(BXGameplayTags::BXEvent_Projectile_Hit, FBXProjectileHitPayload::StaticStruct());
		EventMgr->DefineEvent(BXGameplayTags::BXEvent_Projectile_Finished, FBXProjectileEventParameter::StaticStruct());
		EventMgr->DefineEvent(BXGameplayTags::BXEvent_Projectile_Denied, FBXProjectileEventParameter::StaticStruct());
	}
}

void UBXProjectileManager::Deinitialize()
{
	FWorldDelegates::OnWorldCleanup.RemoveAll(this);

	// 异步计算线程停机(请求退出+Join+清空队列,防析构后收割悬空)
	ComputeWorker.Reset();

	Buckets.Empty();
	ProjectileIndexMap.Empty();
	OwnerProjectileMap.Empty();
	RegisteredTargets.Empty();
	TargetSnapshots.Empty();
	TargetActorToIndex.Empty();
	PendingProjectiles.Empty();
	PresentationHost = nullptr;
	SnapshotChannel = nullptr;

	Super::Deinitialize();
}

void UBXProjectileManager::OnWorldCleanupStart(UWorld* InWorld, bool bSessionEnded, bool bCleanupResources)
{
	// 只清理自己所属世界(PIE多实例并行时避免误清其它实例的运行数据)
	if (InWorld != GetWorld())
	{
		return;
	}

	// 世界销毁(PIE结束/关卡切换):清空全部运行数据,桶表现组件随宿主Actor销毁
	Buckets.Empty();
	ProjectileIndexMap.Empty();
	OwnerProjectileMap.Empty();
	RegisteredTargets.Empty();
	TargetSnapshots.Empty();
	TargetActorToIndex.Empty();
	PendingProjectiles.Empty();
	PresentationHost = nullptr;
	SnapshotChannel = nullptr;
	StepAccumulator = 0.0f;
	SnapshotTimer = 0.0f;
}

#pragma endregion Important



#pragma region Tick
UWorld* UBXProjectileManager::GetTickableGameObjectWorld() const
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

ETickableTickType UBXProjectileManager::GetTickableTickType() const
{
	return IsTemplate() ? ETickableTickType::Never : ETickableTickType::Always;
}

bool UBXProjectileManager::IsAllowedToTick() const
{
	return IsValid(this) && !IsUnreachable();
}

void UBXProjectileManager::Tick(float DeltaTime)
{
	const UBXSettings* Settings = GetDefault<UBXSettings>();
	const bool bAsyncCompute = Settings->bProjectileAsyncCompute;

	// 无子弹时清空累积器避免空转(丢弃在途结果防驻留:世界清理后ID全miss)
	if (Buckets.Num() == 0 && PendingProjectiles.Num() == 0)
	{
		StepAccumulator = 0.0f;
		SnapshotTimer = 0.0f;
		if (bAsyncCompute)
		{
			HarvestComputeResults();
		}
		return;
	}

	const float FixedStep = FMath::Max(Settings->ProjectileFixedStep, 0.005f);
	const int32 MaxSteps = FMath::Max(Settings->ProjectileMaxStepsPerFrame, 1);

	// 先收割上帧异步步结果(顺序协议:终态/去重集更新后才提交新步)
	if (bAsyncCompute)
	{
		HarvestComputeResults();
	}

	// 固定步长累积:与帧率解耦保证双端步进序列一致
	StepAccumulator += DeltaTime;
	int32 StepCount = 0;
	while (StepAccumulator >= FixedStep && StepCount < MaxSteps)
	{
		// 异步在途:本帧不再步进,子弹按worker完成速率推进(避免本地推进与收割回写竞态)
		if (bAsyncCompute && ComputeWorker.IsValid() && ComputeWorker->IsBusy())
		{
			break;
		}

		StepAccumulator -= FixedStep;
		++StepCount;

		// 首步异步提交,本帧步进预算耗尽于异步步(流水线深度1,收割延后一帧)
		if (bAsyncCompute && TrySubmitComputeJob(FixedStep))
		{
			break;
		}

		StepSimulation(FixedStep);
	}

	// 步数超限:丢弃欠账防螺旋(子弹时间轻微放慢但不雪崩)
	if (StepAccumulator > FixedStep)
	{
		StepAccumulator = FMath::Fmod(StepAccumulator, FixedStep);
	}

	// 弹簧矫正每帧步进(表现层平滑,非固定步长)
	UpdateSprings(DeltaTime);

	// 服务器快照定时下发
	InternalUpdateSnapshots(DeltaTime);

	// 表现提交每帧一次(非每步)
	CommitRender();
}

void UBXProjectileManager::StepSimulation(float InFixedStep)
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	const ENetMode NetMode = World->GetNetMode();
	const bool bAuthority = NetMode != NM_Client;
	const UBXSettings* Settings = GetDefault<UBXSettings>();

	// 本端命中检测职责:单机/LS本端检测;DS开启检测时本端检测;DS关闭检测时等待客户端上报;客户端关闭检测时代劳本连接Instigator的子弹
	const bool bLocalDetect = bAuthority ? (NetMode == NM_Standalone || NetMode == NM_ListenServer || (Settings && Settings->bServerCollisionCheck)) : !(Settings && Settings->bServerCollisionCheck);

	BuildTargetSnapshots();

	// 更新窗口:期间事件回调中的发射/停止入挂起区或仅标记死亡,主容器结构冻结
	bInSimulation = true;

	// ID快照遍历桶:命中/结束事件回调可能同步发射新种类子弹触发Buckets扩容rehash
	TArray<FGameplayTag> BucketTypes;
	Buckets.GetKeys(BucketTypes);
	for (const FGameplayTag& BucketType : BucketTypes)
	{
		FBXProjectileBucket* Bucket = Buckets.Find(BucketType);
		if (!Bucket || Bucket->Bullets.Num() == 0)
		{
			continue;
		}

		InternalResolveTargetSnapshotIndices(*Bucket, bAuthority);
		ParallelIntegrateBucket(*Bucket, InFixedStep, bLocalDetect);

		if (bLocalDetect)
		{
			// Math优先结算:命中单位终态后物理检测自然跳过(双开时语义)
			if (Bucket->Baked.bMathCheck)
			{
				ProcessBucketHits(BucketType, *Bucket);
			}

			if (Bucket->Baked.bPhysicsCheck)
			{
				PhysicsSweepBucket(BucketType, *Bucket);
			}
		}

		ProcessBucketLifecycle(BucketType, *Bucket);
	}

	bInSimulation = false;
	MergePendingProjectiles();
}

void UBXProjectileManager::BuildTargetSnapshots()
{
	TargetSnapshots.Reset(RegisteredTargets.Num());
	TargetActorToIndex.Reset();

	for (TMap<TWeakObjectPtr<AActor>, FBXProjectileTargetRegistration>::TIterator It(RegisteredTargets); It; ++It)
	{
		AActor* Target = It->Key.Get();
		if (!IsValid(Target))
		{
			// 目标已销毁:顺手清理注册表
			It.RemoveCurrent();
			continue;
		}

		// 受击盒组件存在时逐盒展开(每盒独立快照条目,同目标UID用于穿透去重,索引映射登记首盒)
		UBXShapeComponent* ShapeComponent = It->Value.ShapeComponent.Get();
		if (IsValid(ShapeComponent) && ShapeComponent->ShapeInformations.Num() > 0)
		{
			for (const TPair<FGameplayTag, FBXShapeInformation>& Pair : ShapeComponent->ShapeInformations)
			{
				FTransform BoxTransform;
				if (!ShapeComponent->GetShapeTransformByTag(Pair.Key, BoxTransform))
				{
					continue;
				}

				FBXProjectileTargetSnapshot Snapshot;
				Snapshot.Location = BoxTransform.GetLocation();
				Snapshot.Rotation = BoxTransform.GetRotation();
				Snapshot.ShapeType = Pair.Value.ShapeType;
				Snapshot.ShapeExtent = Pair.Value.ShapeSize;
				Snapshot.HitBoxTag = Pair.Key;
				Snapshot.TargetUID = static_cast<uint32>(Target->GetUniqueID());
				Snapshot.bValid = true;
				Snapshot.Target = Target;

				// 多受击盒目标仅登记首盒索引(跟踪/贝塞尔以首盒为瞄准锚点而非Actor根位置;命中检测不受影响,逐盒快照全量参与)
			if (!TargetActorToIndex.Contains(Target))
			{
				TargetActorToIndex.Add(Target, TargetSnapshots.Num());
			}
			TargetSnapshots.Add(MoveTemp(Snapshot));
			}
			continue;
		}

		// 单形状回退(注册参数+根变换)
		FBXProjectileTargetSnapshot Snapshot;
		Snapshot.Location = Target->GetActorLocation();
		Snapshot.Rotation = Target->GetActorQuat();
		Snapshot.ShapeType = It->Value.ShapeType;
		Snapshot.ShapeExtent = It->Value.ShapeExtent;
		Snapshot.TargetUID = static_cast<uint32>(Target->GetUniqueID());
		Snapshot.bValid = true;
		Snapshot.Target = Target;

		TargetActorToIndex.Add(Target, TargetSnapshots.Num());
		TargetSnapshots.Add(MoveTemp(Snapshot));
	}
}

void UBXProjectileManager::InternalResolveTargetSnapshotIndices(FBXProjectileBucket& InOutBucket, bool bAuthority)
{
	for (FBXProjectileSimData& Data : InOutBucket.Bullets)
	{
		Data.TargetSnapshotIndex = INDEX_NONE;

		AActor* Target = Data.FireContext.Target;
		if (IsValid(Target))
		{
			if (int32* SnapshotIndex = TargetActorToIndex.Find(Target))
			{
				Data.TargetSnapshotIndex = *SnapshotIndex;
			}
		}

		// 检测职责解析:权威端全量,客户端代劳模式仅本连接Instigator(本地AutonomousProxy)的子弹
		if (bAuthority)
		{
			Data.bLocalDetectable = true;
		}
		else
		{
			AActor* Instigator = Data.FireContext.Instigator;
			Data.bLocalDetectable = IsValid(Instigator) && Instigator->GetLocalRole() == ROLE_AutonomousProxy;
		}
	}
}

void UBXProjectileManager::ParallelIntegrateBucket(FBXProjectileBucket& InOutBucket, float InFixedStep, bool bLocalDetect)
{
	const int32 BulletCount = InOutBucket.Bullets.Num();
	if (BulletCount <= 0)
	{
		return;
	}

	UWorld* World = GetWorld();
	const bool bAuthority = World && World->GetNetMode() != NM_Client;

	// 数学候选收集:权威检测端全量,客户端代劳端逐子弹(bLocalDetectable);关闭数学判定的桶无候选
	const bool bCollectMath = bLocalDetect && InOutBucket.Baked.bMathCheck;
	const bool bPerBulletCollect = bCollectMath && !bAuthority;

	const FBXProjectileTargetSnapshot* Snapshots = TargetSnapshots.GetData();
	const int32 SnapshotCount = TargetSnapshots.Num();

	// 分片:64发一片,小规模直跑避免并行调度开销
	const int32 ShardCount = BulletCount > 64 ? FMath::Clamp(BulletCount / 64, 2, 16) : 1;
	InOutBucket.ShardCandidates.SetNum(ShardCount);

	auto ProcessShard = [&InOutBucket, InFixedStep, bCollectMath, bPerBulletCollect, Snapshots, SnapshotCount, BulletCount, ShardCount](int32 ShardIndex)
	{
		TArray<FBXProjectileHitCandidate>& Candidates = InOutBucket.ShardCandidates[ShardIndex];
		Candidates.Reset();

		const int32 PerShard = FMath::DivideAndRoundUp(BulletCount, ShardCount);
		const int32 StartIndex = ShardIndex * PerShard;
		const int32 EndIndex = FMath::Min(StartIndex + PerShard, BulletCount);
		for (int32 Index = StartIndex; Index < EndIndex; ++Index)
		{
			FBXProjectileSimData& Data = InOutBucket.Bullets[Index];
			const FBXProjectileTargetSnapshot* Snapshot = (Data.TargetSnapshotIndex >= 0 && Data.TargetSnapshotIndex < SnapshotCount) ? &Snapshots[Data.TargetSnapshotIndex] : nullptr;
			FBXProjectileSolver::IntegrateStep(InOutBucket.Baked, Snapshot, InFixedStep, Data);
			if (bCollectMath && (!bPerBulletCollect || Data.bLocalDetectable))
			{
				FBXProjectileSolver::MathCheckBullet(InOutBucket.Baked, Data, Index, Snapshots, SnapshotCount, Candidates);
			}
		}
	};

	if (ShardCount > 1)
	{
		ParallelFor(ShardCount, ProcessShard);
	}
	else
	{
		ProcessShard(0);
	}
}

void UBXProjectileManager::UpdateSprings(float InDeltaTime)
{
	// 全端步进:客户端收敛快照矫正偏差,服务器/单机收敛贝塞尔重建偏差(否则权威检测线段与快照锚点长期带偏差)
	const float Frequency = GetDefault<UBXSettings>()->ProjectileSpringFrequency;
	for (TPair<FGameplayTag, FBXProjectileBucket>& Pair : Buckets)
	{
		for (FBXProjectileSimData& Data : Pair.Value.Bullets)
		{
			if (Data.State != EBXProjectileState::PS_Active)
			{
				// 残留态冻结矫正(表现淡出跟随最终位置)
				continue;
			}

			FBXProjectileSolver::SpringStep(Frequency, InDeltaTime, Data);
		}
	}
}

bool UBXProjectileManager::TrySubmitComputeJob(float InFixedStep)
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return false;
	}

	const ENetMode NetMode = World->GetNetMode();
	const bool bAuthority = NetMode != NM_Client;
	const UBXSettings* Settings = GetDefault<UBXSettings>();

	// 本端命中检测职责:单机/LS本端检测;DS开启检测时本端检测;DS关闭检测时等待客户端上报;客户端关闭检测时代劳本连接Instigator的子弹
	const bool bLocalDetect = bAuthority ? (NetMode == NM_Standalone || NetMode == NM_ListenServer || (Settings && Settings->bServerCollisionCheck)) : !(Settings && Settings->bServerCollisionCheck);

	BuildTargetSnapshots();

	// 全桶拷贝构建Job(纯数据深拷贝,worker期间主容器变更由基线守卫兜底)
	FBXProjectileComputeJob Job;
	Job.StepSerial = ++ComputeStepSerial;
	Job.FixedStep = InFixedStep;
	Job.bAuthority = bAuthority;
	Job.TargetSnapshots = TargetSnapshots;

	TArray<FGameplayTag> BucketTypes;
	Buckets.GetKeys(BucketTypes);
	for (const FGameplayTag& BucketType : BucketTypes)
	{
		FBXProjectileBucket* Bucket = Buckets.Find(BucketType);
		if (!Bucket || Bucket->Bullets.Num() == 0)
		{
			continue;
		}

		InternalResolveTargetSnapshotIndices(*Bucket, bAuthority);

		FBXProjectileComputeBucketEntry Entry;
		Entry.BucketType = BucketType;
		Entry.Baked = Bucket->Baked;
		Entry.bCollectMath = bLocalDetect && Bucket->Baked.bMathCheck;

		// 轻量计算副本:仅拷贝积分/判定/回写所需字段(跳过ContextData实例克隆与GT专用字段)
		Entry.Bullets.SetNum(Bucket->Bullets.Num());
		for (int32 BulletIndex = 0; BulletIndex < Bucket->Bullets.Num(); ++BulletIndex)
		{
			const FBXProjectileSimData& Data = Bucket->Bullets[BulletIndex];
			Data.CopyComputeFieldsTo(Entry.Bullets[BulletIndex]);
			Entry.BaseStates.Add(Data.State);
			Entry.BaseSpringOffsets.Add(Data.SpringOffset);
		}
		Job.BucketEntries.Add(MoveTemp(Entry));
	}

	if (Job.BucketEntries.Num() == 0)
	{
		return false;
	}

	if (!ComputeWorker.IsValid())
	{
		ComputeWorker = MakeUnique<FBXProjectileComputeWorker>();
	}

	return ComputeWorker->EnqueueJob(MoveTemp(Job));
}

void UBXProjectileManager::HarvestComputeResults()
{
	if (!ComputeWorker.IsValid())
	{
		return;
	}

	FBXProjectileComputeResult Result;
	while (ComputeWorker->DequeueResult(Result))
	{
		UWorld* World = GetWorld();
		if (!World)
		{
			break;
		}

		const ENetMode NetMode = World->GetNetMode();
		const bool bAuthority = NetMode != NM_Client;
		const UBXSettings* Settings = GetDefault<UBXSettings>();
		const bool bLocalDetect = bAuthority ? (NetMode == NM_Standalone || NetMode == NM_ListenServer || (Settings && Settings->bServerCollisionCheck)) : !(Settings && Settings->bServerCollisionCheck);

		// 更新窗口:结算事件回调中的发射/停止入挂起区,主容器结构冻结
		bInSimulation = true;

		TArray<FGameplayTag> TouchedBucketTypes;

		// 数值回写:按ID定位,主容器状态与提交时基线一致才回写(在途期间被GT事件改动的子弹丢弃)
		for (FBXProjectileComputeBucketResult& BucketResult : Result.BucketResults)
		{
			TouchedBucketTypes.Add(BucketResult.BucketType);
			FBXProjectileBucket* Bucket = Buckets.Find(BucketResult.BucketType);
			if (!Bucket)
			{
				continue;
			}

			for (int32 Index = 0; Index < BucketResult.Bullets.Num(); ++Index)
			{
				FBXProjectileSimData& ResultData = BucketResult.Bullets[Index];
				const FBXProjectileIndexRef* Ref = ProjectileIndexMap.Find(ResultData.ProjectileID);
				if (!Ref || Ref->BucketType != BucketResult.BucketType)
				{
					continue;
				}

				if (!Bucket->Bullets.IsValidIndex(Ref->Index) || Bucket->BulletIDs[Ref->Index] != ResultData.ProjectileID)
				{
					continue;
				}

				FBXProjectileSimData& MainData = Bucket->Bullets[Ref->Index];
				if (MainData.State != BucketResult.BaseStates[Index])
				{
					continue;
				}

				MainData.PrevLocation = ResultData.PrevLocation;
				MainData.Location = ResultData.Location;
				MainData.Velocity = ResultData.Velocity;
				MainData.ElapsedTime = ResultData.ElapsedTime;
				MainData.ResidualTimer = ResultData.ResidualTimer;
				MainData.State = ResultData.State;
				MainData.LastTargetLocation = ResultData.LastTargetLocation;
				MainData.Bezier = MoveTemp(ResultData.Bezier);

				// 弹簧增量合并:worker期间贝塞尔重建偏差叠加进主容器当前弹簧(主容器弹簧已同步步进不可覆盖)
				MainData.SpringOffset += ResultData.SpringOffset - BucketResult.BaseSpringOffsets[Index];
			}
		}

		// 候选结算(幂等复查按当前主容器状态:终态/去重/目标有效)
		for (const FBXProjectileAsyncHitCandidate& Candidate : Result.Candidates)
		{
			InternalResolveAsyncCandidate(Candidate);
		}

		// 涉及桶的物理检测与生命周期(收割后位置已更新)
		for (const FGameplayTag& BucketType : TouchedBucketTypes)
		{
			FBXProjectileBucket* Bucket = Buckets.Find(BucketType);
			if (!Bucket || Bucket->Bullets.Num() == 0)
			{
				continue;
			}

			if (bLocalDetect && Bucket->Baked.bPhysicsCheck)
			{
				PhysicsSweepBucket(BucketType, *Bucket);
			}

			ProcessBucketLifecycle(BucketType, *Bucket);
		}

		bInSimulation = false;
		MergePendingProjectiles();
	}
}

#pragma endregion Tick



#pragma region API
void UBXProjectileManager::RegisterProjectileTarget(AActor* InTarget, EBXShapeType InShapeType, FVector InShapeExtent)
{
	if (!IsValid(InTarget))
	{
		return;
	}

	FBXProjectileTargetRegistration Registration;
	Registration.ShapeType = InShapeType;
	Registration.ShapeExtent = InShapeExtent;

	// 挂有受击盒组件时以组件的多受击盒信息为准(逐盒展开检测,命中载荷携带受击盒标签)
	Registration.ShapeComponent = InTarget->FindComponentByClass<UBXShapeComponent>();

	RegisteredTargets.Add(InTarget, Registration);
}

void UBXProjectileManager::UnregisterProjectileTarget(AActor* InTarget)
{
	if (InTarget)
	{
		RegisteredTargets.Remove(InTarget);
	}
}

int64 UBXProjectileManager::FireProjectile(FGameplayTag InProjectileType, const FBXProjectileFireContext& InContext, UBXProjectileComponent* InCarrier)
{
	UWorld* World = GetWorld();
	if (!World || World->GetNetMode() == NM_Client)
	{
		UE_LOG(BXMGR_Projectile, Warning, TEXT("FireProjectile: 非权威端禁止权威发射"));
		return 0;
	}

	// 载体解析:未指定时从始作俑者惰性获取
	UBXProjectileComponent* Carrier = InCarrier ? InCarrier : GetOrCreateCarrier(InContext.Instigator);

	const int64 NewID = BXMakeSyncID(UBXFunctionLibrary::GetUniqueID(), EBXSyncInitiator::Server);

	FBXProjectileSyncHeader Header;
	Header.ProjectileID = NewID;
	Header.ProjectileType = InProjectileType;
	Header.Initiator = EBXSyncInitiator::Server;
	Header.ClientTimestamp = UBXFunctionLibrary::GetServerWorldTimeMilliseconds(this);

	return InternalAuthorityFire(Header, InContext, Carrier) ? NewID : 0;
}

void UBXProjectileManager::StopProjectile(int64 InProjectileID, EBXProjectileStopReason InReason)
{
	UWorld* World = GetWorld();
	const bool bAuthority = !World || World->GetNetMode() != NM_Client;

	if (bAuthority)
	{
		// 权威端组播停止:本端经组播本地执行移除,远端同步移除
		if (const FBXProjectileIndexRef* Ref = ProjectileIndexMap.Find(InProjectileID))
		{
			if (const FBXProjectileBucket* Bucket = Buckets.Find(Ref->BucketType))
			{
				if (Bucket->Bullets.IsValidIndex(Ref->Index) && Bucket->BulletIDs[Ref->Index] == InProjectileID)
				{
					if (UBXProjectileComponent* Carrier = Bucket->Bullets[Ref->Index].Carrier.Get())
					{
						Carrier->MulticastStopProjectile(InProjectileID, static_cast<uint8>(InReason));
						return;
					}
				}
			}
		}

		// 无载体兜底:仅本端移除
		InternalRemoveProjectile(InProjectileID, EBXProjectileFinishReason::FR_Interrupted);
		return;
	}

	// 客户端本地移除(无广播权)
	InternalRemoveProjectile(InProjectileID, EBXProjectileFinishReason::FR_Interrupted);
}

bool UBXProjectileManager::IsProjectileRunning(int64 InProjectileID) const
{
	if (InProjectileID == 0)
	{
		return false;
	}

	const FBXProjectileIndexRef* Ref = ProjectileIndexMap.Find(InProjectileID);
	if (!Ref)
	{
		return false;
	}

	const FBXProjectileBucket* Bucket = Buckets.Find(Ref->BucketType);
	return Bucket && Bucket->Bullets.IsValidIndex(Ref->Index) && Bucket->BulletIDs[Ref->Index] == InProjectileID;
}

int32 UBXProjectileManager::GetProjectileCount(FGameplayTag InProjectileType) const
{
	const FBXProjectileBucket* Bucket = Buckets.Find(InProjectileType);
	return Bucket ? Bucket->Bullets.Num() : 0;
}

#pragma endregion API



#pragma region Asset
void UBXProjectileManager::CollectProjectileAssets()
{
	const UBXSettings* Settings = GetDefault<UBXSettings>();

	FARFilter Filter;
	for (const FDirectoryPath& AssetPath : Settings->ProjectileAssetPaths)
	{
		if (!AssetPath.Path.IsEmpty())
		{
			Filter.PackagePaths.Add(FName(*AssetPath.Path));
		}
	}
	if (Filter.PackagePaths.Num() == 0)
	{
		Filter.PackagePaths.Add("/Game");
	}
	Filter.ClassPaths.Add(UBXProjectileAsset::StaticClass()->GetClassPathName());
	Filter.bRecursivePaths = true;
	Filter.bRecursiveClasses = true;

	TArray<FAssetData> AssetDataList;
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::GetModuleChecked<FAssetRegistryModule>("AssetRegistry");
	AssetRegistryModule.Get().GetAssets(Filter, AssetDataList);

	// 子弹种类数量级小:启动时同步加载,规避运行时首次发射卡顿
	for (const FAssetData& AssetData : AssetDataList)
	{
		UBXProjectileAsset* Asset = Cast<UBXProjectileAsset>(AssetData.GetAsset());
		if (!Asset)
		{
			continue;
		}

		if (!Asset->ProjectileType.IsValid())
		{
			UE_LOG(BXMGR_Projectile, Warning, TEXT("CollectProjectileAssets: 资产%s的ProjectileType未配置"), *AssetData.GetName().ToString());
			continue;
		}

		ProjectileAssetMap.Add(Asset->ProjectileType, TSoftObjectPtr<UBXProjectileAsset>(AssetData.ToSoftObjectPath()));
		LoadedAssets.AddUnique(Asset);
	}
}

UBXProjectileAsset* UBXProjectileManager::GetOrLoadProjectileAsset(const FGameplayTag& InProjectileType)
{
	if (!InProjectileType.IsValid())
	{
		return nullptr;
	}

	// 已加载资源直接命中
	for (const TObjectPtr<UBXProjectileAsset>& Asset : LoadedAssets)
	{
		if (Asset && Asset->ProjectileType == InProjectileType)
		{
			return Asset;
		}
	}

	// 软引用加载
	const TSoftObjectPtr<UBXProjectileAsset>* SoftAssetPtr = ProjectileAssetMap.Find(InProjectileType);
	if (!SoftAssetPtr)
	{
		return nullptr;
	}

	UBXProjectileAsset* Asset = SoftAssetPtr->LoadSynchronous();
	if (!Asset)
	{
		return nullptr;
	}

	LoadedAssets.AddUnique(Asset);
	return Asset;
}

#pragma endregion Asset



#pragma region Bucket
FBXProjectileBucket* UBXProjectileManager::GetOrCreateBucket(const FGameplayTag& InProjectileType)
{
	if (FBXProjectileBucket* Existing = Buckets.Find(InProjectileType))
	{
		return Existing;
	}

	UBXProjectileAsset* Asset = GetOrLoadProjectileAsset(InProjectileType);
	if (!Asset)
	{
		return nullptr;
	}

	const UBXSettings* Settings = GetDefault<UBXSettings>();

	FBXProjectileBucket NewBucket;
	NewBucket.Asset = Asset;
	Asset->BuildBakedConfig(NewBucket.Baked, Settings->ProjectileCurveBakeSamples, Settings->ProjectileBezierArcSamples, Settings->ProjectileTargetRefreshDistance);

	// 表现资源与筛选器一次性解析缓存(避免每帧/每命中重复加载)
	NewBucket.FireSystem = Asset->FireSystem.LoadSynchronous();
	NewBucket.HitSystem = Asset->HitSystem.LoadSynchronous();
	NewBucket.FireSound = Asset->FireSound.LoadSynchronous();
	NewBucket.HitSound = Asset->HitSound.LoadSynchronous();
	NewBucket.FlightSound = Asset->FlightSound.LoadSynchronous();

	const FBXCFilter& Filter = Asset->PhysicsStrategy.Filter;
	NewBucket.bFilterIgnoreSelf = Filter.bIgnoreSelf;

	// 筛选器类型解析并登记GC强引用(桶内裸指针不可见GC,蓝图类无强引用会被回收致悬空)
	for (const TSoftClassPtr<UObject>& SoftClass : Filter.ClassTypes)
	{
		if (UClass* ResolvedClass = SoftClass.LoadSynchronous())
		{
			NewBucket.FilterClassTypes.Add(ResolvedClass);
			PinnedFilterClasses.AddUnique(ResolvedClass);
		}
	}
	for (const TSoftClassPtr<UObject>& SoftClass : Filter.IgnoreClassTypes)
	{
		if (UClass* ResolvedClass = SoftClass.LoadSynchronous())
		{
			NewBucket.FilterIgnoreClassTypes.Add(ResolvedClass);
			PinnedFilterClasses.AddUnique(ResolvedClass);
		}
	}

	// 命中效果资产解析并登记GC强引用(桶内弱引用,失效则命中时静默跳过)
	for (const FBXProjectileHitEffect& Effect : Asset->HitEffects)
	{
		FBXProjectileHitEffectRuntime Runtime;
		Runtime.EffectType = Effect.EffectType;

		switch (Effect.EffectType)
		{
		case EBXProjectileHitEffectType::HE_Buff:
			if (UBXBuffAsset* BuffAsset = Effect.BuffAsset.LoadSynchronous())
			{
				Runtime.BuffAsset = BuffAsset;
				PinnedHitEffectAssets.AddUnique(BuffAsset);
			}
			break;

		case EBXProjectileHitEffectType::HE_Skill:
			if (UBXSkillAsset* SkillAsset = Effect.SkillAsset.LoadSynchronous())
			{
				Runtime.SkillAsset = SkillAsset;
				PinnedHitEffectAssets.AddUnique(SkillAsset);
			}
			break;

		default:
			break;
		}

		NewBucket.HitEffects.Add(Runtime);
	}

	return &Buckets.Add(InProjectileType, MoveTemp(NewBucket));
}

void UBXProjectileManager::DestroyBucketPresentation(FBXProjectileBucket& InOutBucket)
{
	if (UNiagaraComponent* FlightComp = InOutBucket.FlightComponent.Get())
	{
		FlightComp->DestroyComponent();
	}
	InOutBucket.FlightComponent = nullptr;

	if (UAudioComponent* FlightSoundComp = InOutBucket.FlightSoundComponent.Get())
	{
		FlightSoundComp->DestroyComponent();
	}
	InOutBucket.FlightSoundComponent = nullptr;
}

#pragma endregion Bucket



#pragma region Simulation
bool UBXProjectileManager::InternalAddProjectile(const FGameplayTag& InProjectileType, int64 InProjectileID, const FBXProjectileFireContext& InContext, bool bPredicted, UBXProjectileComponent* InCarrier)
{
	if (InProjectileID == 0)
	{
		return false;
	}

	if (ProjectileIndexMap.Contains(InProjectileID))
	{
		UE_LOG(BXMGR_Projectile, Warning, TEXT("InternalAddProjectile: 子弹ID重复 %lld"), InProjectileID);
		return false;
	}

	// 更新窗口内入挂起区(桶TMap新增/数组追加触发rehash会令遍历中引用悬空)
	if (bInSimulation)
	{
		FBXProjectilePendingEntry Entry;
		Entry.Header.ProjectileID = InProjectileID;
		Entry.Header.ProjectileType = InProjectileType;
		Entry.Header.Initiator = bPredicted ? EBXSyncInitiator::Client : EBXSyncInitiator::Server;
		Entry.Header.ClientTimestamp = InContext.ClientTimestamp;
		Entry.FireContext = InContext;
		Entry.bPredicted = bPredicted;
		Entry.Carrier = InCarrier;
		PendingProjectiles.Add(MoveTemp(Entry));
		return true;
	}

	FBXProjectileBucket* Bucket = GetOrCreateBucket(InProjectileType);
	if (!Bucket)
	{
		UE_LOG(BXMGR_Projectile, Warning, TEXT("InternalAddProjectile: 子弹种类不存在 %s"), *InProjectileType.ToString());
		return false;
	}

	FBXProjectileSimData NewData;
	InternalInitSimData(*Bucket, InContext, InProjectileID, bPredicted, InCarrier, NewData);
	InternalAppendBullet(InProjectileType, *Bucket, MoveTemp(NewData));

	// Fired事件广播
	InternalBroadcastFired(InProjectileType, InContext, InProjectileID);

	// 发射表现
	PresentationFire(*Bucket, InContext.StartLocation);

	return true;
}

void UBXProjectileManager::InternalInitSimData(const FBXProjectileBucket& InBucket, const FBXProjectileFireContext& InContext, int64 InProjectileID, bool bPredicted, UBXProjectileComponent* InCarrier, FBXProjectileSimData& OutData)
{
	OutData = FBXProjectileSimData();
	OutData.ProjectileID = InProjectileID;
	OutData.FireContext = InContext;
	OutData.bPredicted = bPredicted;
	OutData.Carrier = InCarrier;
	OutData.Owner = InCarrier ? InCarrier->GetOwner() : nullptr;
	OutData.Location = InContext.StartLocation;
	OutData.PrevLocation = InContext.StartLocation;
	OutData.LastSweptLocation = InContext.StartLocation;
	OutData.InstigatorUID = IsValid(InContext.Instigator) ? static_cast<uint32>(InContext.Instigator->GetUniqueID()) : 0;
	OutData.TargetSnapshotIndex = INDEX_NONE;

	if (InBucket.Baked.bUseBezier)
	{
		// 无目标时以发射方向远点为伪目标
		FVector TargetLocation = InContext.StartLocation + InContext.FireDirection * 1000.0f;
		if (IsValid(InContext.Target))
		{
			TargetLocation = InContext.Target->GetActorLocation();
		}
		FBXProjectileSolver::InitBezierRuntime(InBucket.Baked, InContext.StartLocation, TargetLocation, InContext.FireUpVector, InBucket.Baked.ArcSampleCount, OutData);
	}
	else if (IsValid(InContext.Target))
	{
		OutData.LastTargetLocation = InContext.Target->GetActorLocation();
	}
}

void UBXProjectileManager::InternalAppendBullet(const FGameplayTag& InProjectileType, FBXProjectileBucket& InOutBucket, FBXProjectileSimData&& InData)
{
	const int64 ProjectileID = InData.ProjectileID;

	// 归属登记先于移动(复制快照构建用,载体组件拥有者维度)
	if (AActor* Owner = InData.Owner.Get())
	{
		OwnerProjectileMap.FindOrAdd(Owner).Add(ProjectileID);
	}

	// 结构变更通知载体组件即时刷新复制快照
	if (UBXProjectileComponent* Carrier = InData.Carrier.Get())
	{
		Carrier->MarkReplicatedStatesDirty();
	}

	const int32 NewIndex = InOutBucket.Bullets.Add(MoveTemp(InData));
	InOutBucket.BulletIDs.Add(ProjectileID);

	FBXProjectileIndexRef NewRef;
	NewRef.BucketType = InProjectileType;
	NewRef.Index = NewIndex;
	ProjectileIndexMap.Add(ProjectileID, NewRef);

	// 飞行渲染组件惰性创建
	InternalEnsureFlightPresentation(InOutBucket);
}

void UBXProjectileManager::PhysicsSweepBucket(const FGameplayTag& InBucketType, FBXProjectileBucket& InOutBucket)
{
	UWorld* World = GetWorld();
	if (!World || !InOutBucket.Asset || InOutBucket.Bullets.Num() == 0)
	{
		return;
	}

	const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes = InOutBucket.Asset->PhysicsStrategy.ObjectTypes;
	if (ObjectTypes.Num() == 0)
	{
		return;
	}

	const bool bAuthority = World->GetNetMode() != NM_Client;
	const UBXSettings* Settings = GetDefault<UBXSettings>();

	// 预算为每桶独立(各桶每次调用各自满额,非帧级共享;实际帧上限=该值×活跃桶数)
	int32 RemainingBudget = FMath::Max(Settings->ProjectileSweepBudgetPerFrame, 1);

	// Sweep几何体按子弹形状:点=线检测 球体=半径球 长方体=半尺寸盒(旋转对齐飞行方向)
	FCollisionShape SweepShape;
	bool bBoxRotated = false;
	switch (InOutBucket.Baked.BulletShape)
	{
	case EBXProjectileShape::BS_Sphere:
		SweepShape = FCollisionShape::MakeSphere(InOutBucket.Baked.BulletRadius);
		break;

	case EBXProjectileShape::BS_Box:
		SweepShape = FCollisionShape::MakeBox(InOutBucket.Baked.BulletBoxExtent);
		bBoxRotated = true;
		break;

	default:
		break;
	}

	const FCollisionObjectQueryParams ObjectQueryParams(ObjectTypes);
	const FBXCFilter& Filter = InOutBucket.Asset->PhysicsStrategy.Filter;

	const int32 BulletCount = InOutBucket.Bullets.Num();
	int32 BulletIndex = InOutBucket.SweepCursor % BulletCount;

	// 预算轮转:一圈内每发至少受检一次,超预算子弹下帧从游标处继续(线段跨度累积不漏检)
	int32 CheckedCount = 0;
	while (CheckedCount < BulletCount && RemainingBudget > 0)
	{
		FBXProjectileSimData& Data = InOutBucket.Bullets[BulletIndex];

		// 客户端代劳模式仅检测本连接Instigator的子弹;Sweep线段叠加弹簧偏移与渲染位置一致(服务器偏移恒零)
		const FVector SweepEnd = Data.Location + Data.SpringOffset;
		const bool bNeedsSweep = Data.bLocalDetectable && Data.State == EBXProjectileState::PS_Active && FVector::DistSquared(Data.LastSweptLocation, SweepEnd) > KINDA_SMALL_NUMBER;
		if (bNeedsSweep)
		{
			FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(BXProjectileSweep), false);
			if (Filter.bIgnoreSelf && IsValid(Data.FireContext.Instigator))
			{
				QueryParams.AddIgnoredActor(Data.FireContext.Instigator);
			}

			// 长方体旋转对齐飞行方向(局部X朝前),点/球体旋转不参与
			FQuat SweepRotation = FQuat::Identity;
			if (bBoxRotated)
			{
				const FVector SweepDirection = (SweepEnd - Data.LastSweptLocation).GetSafeNormal();
				if (!SweepDirection.IsNearlyZero())
				{
					SweepRotation = FQuat::FindBetweenNormals(FVector::ForwardVector, SweepDirection);
				}
			}

			FHitResult Hit;
			if (World->SweepSingleByObjectType(Hit, Data.LastSweptLocation, SweepEnd, SweepRotation, ObjectQueryParams, SweepShape, QueryParams) && UBXCollisionLibrary::CheckCollisionResult(Data.FireContext.Instigator, Hit, Filter))
			{
				// 场景命中:组播终态(本端经本地执行表现/事件/终态流转)
				FBXProjectileHitPayload Payload;
				Payload.ProjectileID = Data.ProjectileID;
				Payload.ProjectileType = InBucketType;
				Payload.HitType = EBXProjectileHitType::HT_World;
				Payload.HitTime = Data.ElapsedTime;
				Payload.HitLocation = Hit.Location;
				Payload.HitNormal = Hit.Normal;
				Payload.HitTarget = Hit.GetActor();
				Payload.Instigator = Data.FireContext.Instigator;
				Payload.Triggerer = Data.FireContext.Triggerer;
				Payload.ContextData = Data.FireContext.ContextData;

				if (bAuthority)
				{
					if (UBXProjectileComponent* Carrier = Data.Carrier.Get())
					{
						Carrier->MulticastProjectileHit(Payload, true);
					}
					else
					{
						HandleHitMulticast(Payload, true);
					}
				}
				else
				{
					// 客户端代劳:本地预测结算+上报服务器权威校验
					HandleHitMulticast(Payload, true);
					InternalRecordReportedHitTime(Data, Payload.HitTime);
					InternalReportClientHit(Payload, Data);
				}
			}
			else
			{
				Data.LastSweptLocation = SweepEnd;
			}

			--RemainingBudget;
		}

		++CheckedCount;
		BulletIndex = (BulletIndex + 1) % BulletCount;
	}

	InOutBucket.SweepCursor = BulletIndex;
}

void UBXProjectileManager::ProcessBucketHits(const FGameplayTag& InBucketType, FBXProjectileBucket& InOutBucket)
{
	// 分片候选逐条结算(各分片独写无锁,此处GameThread串行)
	for (TArray<FBXProjectileHitCandidate>& Shard : InOutBucket.ShardCandidates)
	{
		for (const FBXProjectileHitCandidate& Candidate : Shard)
		{
			InternalResolveHitCandidate(InBucketType, InOutBucket, Candidate);
		}
	}
}

void UBXProjectileManager::InternalResolveHitCandidate(const FGameplayTag& InBucketType, FBXProjectileBucket& InOutBucket, const FBXProjectileHitCandidate& InCandidate)
{
	if (!InOutBucket.Bullets.IsValidIndex(InCandidate.BulletIndex))
	{
		return;
	}

	FBXProjectileSimData& Data = InOutBucket.Bullets[InCandidate.BulletIndex];
	if (Data.State != EBXProjectileState::PS_Active)
	{
		return;
	}

	if (!TargetSnapshots.IsValidIndex(InCandidate.TargetIndex))
	{
		return;
	}

	const FBXProjectileTargetSnapshot& Snapshot = TargetSnapshots[InCandidate.TargetIndex];
	if (!Snapshot.bValid)
	{
		return;
	}

	// GameThread目标有效性复查+筛选器过滤
	AActor* TargetActor = Snapshot.Target.Get();
	if (!IsValid(TargetActor) || !InternalPassesMathFilter(InOutBucket, Data.FireContext.Instigator, TargetActor))
	{
		return;
	}

	// 命中冷却判定(含同帧同目标去重:多受击盒展开/冷却期内重复候选由此过滤)
	if (InternalIsTargetInCooldown(Data, Snapshot.TargetUID))
	{
		return;
	}

	InternalExecuteHitResolution(InBucketType, InOutBucket, Data, TargetActor, InCandidate.HitLocation, InCandidate.HitNormal, InCandidate.HitBoxTag);
}

void UBXProjectileManager::InternalResolveAsyncCandidate(const FBXProjectileAsyncHitCandidate& InCandidate)
{
	// ID定位(在途期间索引漂移/子弹移除由定位失败兜底)
	const FBXProjectileIndexRef* Ref = ProjectileIndexMap.Find(InCandidate.ProjectileID);
	if (!Ref)
	{
		return;
	}

	FBXProjectileBucket* Bucket = Buckets.Find(Ref->BucketType);
	if (!Bucket || !Bucket->Bullets.IsValidIndex(Ref->Index) || Bucket->BulletIDs[Ref->Index] != InCandidate.ProjectileID)
	{
		return;
	}

	FBXProjectileSimData& Data = Bucket->Bullets[Ref->Index];
	if (Data.State != EBXProjectileState::PS_Active)
	{
		return;
	}

	// 目标有效性复查+筛选器过滤(当前帧状态,非提交时快照)
	AActor* TargetActor = InCandidate.Target.Get();
	if (!IsValid(TargetActor) || !InternalPassesMathFilter(*Bucket, Data.FireContext.Instigator, TargetActor))
	{
		return;
	}

	// 命中冷却判定(跨步重复候选由当前冷却表过滤,冷却结束允许再次命中)
	if (InternalIsTargetInCooldown(Data, InCandidate.TargetUID))
	{
		return;
	}

	InternalExecuteHitResolution(Ref->BucketType, *Bucket, Data, TargetActor, InCandidate.HitLocation, InCandidate.HitNormal, InCandidate.HitBoxTag);
}

void UBXProjectileManager::InternalExecuteHitResolution(const FGameplayTag& InBucketType, FBXProjectileBucket& InOutBucket, FBXProjectileSimData& InOutData, AActor* InTargetActor, const FVector& InHitLocation, const FVector& InHitNormal, const FGameplayTag& InHitBoxTag)
{
	// 客户端代劳模式:本地预测结算+上报服务器权威校验
	UWorld* World = GetWorld();
	if (World && World->GetNetMode() == NM_Client)
	{
		InternalResolveClientHitCandidate(InBucketType, InOutBucket, InOutData, InTargetActor, InHitLocation, InHitNormal, InHitBoxTag);
		return;
	}

	// 穿透判定:命中次数达上限则终态(冷却后再命中同一目标同样消耗预算)
	InternalMarkTargetHitCooldown(InOutData, static_cast<uint32>(InTargetActor->GetUniqueID()), InOutBucket.Baked.HitCooldown);
	const bool bTerminal = InOutData.PenetrationCount >= InOutBucket.Baked.MaxPenetrationCount - 1;
	if (!bTerminal)
	{
		++InOutData.PenetrationCount;
	}

	// 命中效果(权威端执行:伤害占位/施加BUFF/播放技能)
	InternalApplyHitEffects(InOutBucket, InOutData, InTargetActor);

	FBXProjectileHitPayload Payload;
	Payload.ProjectileID = InOutData.ProjectileID;
	Payload.ProjectileType = InBucketType;
	Payload.HitType = EBXProjectileHitType::HT_Unit;
	Payload.HitTime = InOutData.ElapsedTime;
	Payload.HitLocation = InHitLocation;
	Payload.HitNormal = InHitNormal;
	Payload.HitTarget = InTargetActor;
	Payload.HitBoxTag = InHitBoxTag;
	Payload.Instigator = InOutData.FireContext.Instigator;
	Payload.Triggerer = InOutData.FireContext.Triggerer;
	Payload.ContextData = InOutData.FireContext.ContextData;

	// 组播(本端经本地执行表现/事件/终态流转,无载体兜底仅本端)
	if (UBXProjectileComponent* Carrier = InOutData.Carrier.Get())
	{
		Carrier->MulticastProjectileHit(Payload, bTerminal);
	}
	else
	{
		HandleHitMulticast(Payload, bTerminal);
	}
}

void UBXProjectileManager::InternalResolveClientHitCandidate(const FGameplayTag& InBucketType, FBXProjectileBucket& InOutBucket, FBXProjectileSimData& InOutData, AActor* InTargetActor, const FVector& InHitLocation, const FVector& InHitNormal, const FGameplayTag& InHitBoxTag)
{
	// 穿透计数与冷却(本地预测,服务器校验通过后权威覆写)
	InternalMarkTargetHitCooldown(InOutData, static_cast<uint32>(InTargetActor->GetUniqueID()), InOutBucket.Baked.HitCooldown);
	const bool bTerminal = InOutData.PenetrationCount >= InOutBucket.Baked.MaxPenetrationCount - 1;
	if (!bTerminal)
	{
		++InOutData.PenetrationCount;
	}

	FBXProjectileHitPayload Payload;
	Payload.ProjectileID = InOutData.ProjectileID;
	Payload.ProjectileType = InBucketType;
	Payload.HitType = EBXProjectileHitType::HT_Unit;
	Payload.HitTime = InOutData.ElapsedTime;
	Payload.HitLocation = InHitLocation;
	Payload.HitNormal = InHitNormal;
	Payload.HitTarget = InTargetActor;
	Payload.HitBoxTag = InHitBoxTag;
	Payload.Instigator = InOutData.FireContext.Instigator;
	Payload.Triggerer = InOutData.FireContext.Triggerer;
	Payload.ContextData = InOutData.FireContext.ContextData;

	// 本地预测表现+事件+终态流转
	HandleHitMulticast(Payload, bTerminal);

	// 上报服务器权威校验(每次命中均上报,服务器按命中冷却校验)
	InternalRecordReportedHitTime(InOutData, Payload.HitTime);
	InternalReportClientHit(Payload, InOutData);
}

void UBXProjectileManager::InternalReportClientHit(const FBXProjectileHitPayload& InPayload, const FBXProjectileSimData& InData)
{
	// 上报通道:预测弹载体优先,回退始作俑者载体(服务器按连接归属校验)
	UBXProjectileComponent* Carrier = InData.Carrier.Get();
	if (!Carrier)
	{
		AActor* Instigator = InData.FireContext.Instigator;
		Carrier = IsValid(Instigator) ? Instigator->FindComponentByClass<UBXProjectileComponent>() : nullptr;
	}

	if (Carrier)
	{
		Carrier->ServerReportProjectileHit(InPayload);
	}
}

void UBXProjectileManager::InternalApplyHitEffects(const FBXProjectileBucket& InBucket, const FBXProjectileSimData& InData, AActor* InTargetActor)
{
	// 仅权威端调用(服务器检测路径与客户端上报校验通过路径);客户端代劳端不执行,等待服务器权威结算
	for (const FBXProjectileHitEffectRuntime& Effect : InBucket.HitEffects)
	{
		switch (Effect.EffectType)
		{
		case EBXProjectileHitEffectType::HE_Damage:
			// TODO: 伤害/属性框架接入后实现(命中目标+部位+子弹资产结算伤害)
			break;

		case EBXProjectileHitEffectType::HE_Buff:
		{
			// 对命中目标施加BUFF(始作俑者/触发者随子弹上下文传递)
			UBXBuffAsset* BuffAsset = Effect.BuffAsset.Get();
			UBXBuffManager* BuffManager = UBXBuffManager::Get(this);
			if (BuffAsset && BuffManager && IsValid(InTargetActor))
			{
				FBXBuffPlayContext Context;
				Context.Instigator = InData.FireContext.Instigator;
				Context.Triggerer = InData.FireContext.Triggerer;
				BuffManager->AddBuff(BuffAsset, InTargetActor, Context);
			}
			break;
		}

		case EBXProjectileHitEffectType::HE_Skill:
		{
			// 在命中目标身上播放技能(目标须持有技能组件)
			UBXSkillAsset* SkillAsset = Effect.SkillAsset.Get();
			if (SkillAsset && IsValid(InTargetActor))
			{
				if (UBXSkillComponent* TargetSkillComponent = InTargetActor->FindComponentByClass<UBXSkillComponent>())
				{
					TargetSkillComponent->PlaySkill(SkillAsset, InData.FireContext.Instigator, InData.FireContext.Triggerer);
				}
			}
			break;
		}

		default:
			break;
		}
	}
}

bool UBXProjectileManager::InternalIsTargetInCooldown(const FBXProjectileSimData& InData, uint32 InTargetUID) const
{
	for (const FBXProjectileTargetCooldown& Entry : InData.TargetHitCooldowns)
	{
		if (Entry.TargetUID == InTargetUID)
		{
			// 含等值比较:冷却0时命中当步(同帧多受击盒重复候选)同样被抑制,下一步即放行
			return InData.ElapsedTime <= Entry.CooldownEndTime;
		}
	}
	return false;
}

void UBXProjectileManager::InternalMarkTargetHitCooldown(FBXProjectileSimData& InData, uint32 InTargetUID, float InCooldownSeconds)
{
	// 先清理过期条目(倒序RemoveAtSwap,换入元素来自已访问的高位索引;表规模以不同目标数为上界)
	for (int32 Index = InData.TargetHitCooldowns.Num() - 1; Index >= 0; --Index)
	{
		if (InData.ElapsedTime > InData.TargetHitCooldowns[Index].CooldownEndTime)
		{
			InData.TargetHitCooldowns.RemoveAtSwap(Index);
		}
	}

	// 已有条目刷新截止时刻,新目标追加
	for (FBXProjectileTargetCooldown& Entry : InData.TargetHitCooldowns)
	{
		if (Entry.TargetUID == InTargetUID)
		{
			Entry.CooldownEndTime = InData.ElapsedTime + InCooldownSeconds;
			return;
		}
	}

	FBXProjectileTargetCooldown NewEntry;
	NewEntry.TargetUID = InTargetUID;
	NewEntry.CooldownEndTime = InData.ElapsedTime + InCooldownSeconds;
	InData.TargetHitCooldowns.Add(NewEntry);
}

void UBXProjectileManager::InternalRecordReportedHitTime(FBXProjectileSimData& InOutData, float InHitTime)
{
	// 记录本地上报时刻并清理过期记录(回声匹配窗口需覆盖一次网络往返;上限8条防极端刷屏)
	for (int32 Index = InOutData.RecentReportedHitTimes.Num() - 1; Index >= 0; --Index)
	{
		if (InOutData.RecentReportedHitTimes[Index] < InHitTime - 2.0f || InOutData.RecentReportedHitTimes[Index] > InHitTime)
		{
			InOutData.RecentReportedHitTimes.RemoveAtSwap(Index);
		}
	}

	InOutData.RecentReportedHitTimes.Add(InHitTime);
	if (InOutData.RecentReportedHitTimes.Num() > 8)
	{
		InOutData.RecentReportedHitTimes.RemoveAt(0, InOutData.RecentReportedHitTimes.Num() - 8);
	}
}

bool UBXProjectileManager::InternalIsLocalReportedEcho(const FBXProjectileSimData& InData, float InHitTime) const
{
	// 服务器原样回传本地上报的子弹时刻(RPC浮点精确传输),步进粒度远大于容差防误匹配
	for (const float ReportedTime : InData.RecentReportedHitTimes)
	{
		if (FMath::Abs(ReportedTime - InHitTime) < 0.001f)
		{
			return true;
		}
	}
	return false;
}

void UBXProjectileManager::ProcessBucketLifecycle(const FGameplayTag& InBucketType, FBXProjectileBucket& InOutBucket)
{
	// 倒序遍历+交换移除:回收时尾元素左移入当前位,已处理索引不受影响
	for (int32 Index = InOutBucket.Bullets.Num() - 1; Index >= 0; --Index)
	{
		FBXProjectileSimData& Data = InOutBucket.Bullets[Index];

		if (Data.State == EBXProjectileState::PS_Active)
		{
			// 寿命尽:残留或直接待回收
			if (Data.ElapsedTime >= InOutBucket.Baked.MaxLifetime)
			{
				if (InOutBucket.Baked.ResidualTime > 0.0f)
				{
					Data.State = EBXProjectileState::PS_Residual;
					Data.ResidualTimer = InOutBucket.Baked.ResidualTime;
				}
				else
				{
					Data.State = EBXProjectileState::PS_Dead;
				}
				Data.FinishReason = EBXProjectileFinishReason::FR_Lifetime;
			}
		}
		else if (Data.State == EBXProjectileState::PS_Residual)
		{
			// 残留到期:待回收(计时由Solver积分阶段推进)
			if (Data.ResidualTimer <= 0.0f)
			{
				Data.State = EBXProjectileState::PS_Dead;
				Data.FinishReason = EBXProjectileFinishReason::FR_ResidualEnd;
			}
		}

		if (Data.State == EBXProjectileState::PS_Dead)
		{
			InternalRecycleBullet(InBucketType, InOutBucket, Index);
		}
	}

	if (InOutBucket.Bullets.Num() == 0)
	{
		DestroyBucketPresentation(InOutBucket);
	}
}

void UBXProjectileManager::InternalTerminateBullet(FBXProjectileBucket& InOutBucket, int32 InBulletIndex)
{
	if (!InOutBucket.Bullets.IsValidIndex(InBulletIndex))
	{
		return;
	}

	FBXProjectileSimData& Data = InOutBucket.Bullets[InBulletIndex];
	if (Data.State != EBXProjectileState::PS_Active)
	{
		return;
	}

	Data.FinishReason = EBXProjectileFinishReason::FR_Hit;
	if (InOutBucket.Baked.ResidualTime > 0.0f)
	{
		Data.State = EBXProjectileState::PS_Residual;
		Data.ResidualTimer = InOutBucket.Baked.ResidualTime;
	}
	else
	{
		Data.State = EBXProjectileState::PS_Dead;
	}
}

void UBXProjectileManager::InternalRecycleBullet(const FGameplayTag& InBucketType, FBXProjectileBucket& InOutBucket, int32 InBulletIndex)
{
	if (!InOutBucket.Bullets.IsValidIndex(InBulletIndex) || !InOutBucket.BulletIDs.IsValidIndex(InBulletIndex))
	{
		return;
	}

	FBXProjectileSimData Data = MoveTemp(InOutBucket.Bullets[InBulletIndex]);
	const int64 ProjectileID = Data.ProjectileID;

	// 交换移除+索引表维护
	const bool bHasSwap = InBulletIndex < InOutBucket.Bullets.Num() - 1;
	const int64 SwappedID = bHasSwap ? InOutBucket.BulletIDs.Last() : 0;
	InOutBucket.Bullets.RemoveAtSwap(InBulletIndex);
	InOutBucket.BulletIDs.RemoveAtSwap(InBulletIndex);
	ProjectileIndexMap.Remove(ProjectileID);
	if (bHasSwap)
	{
		if (FBXProjectileIndexRef* SwappedRef = ProjectileIndexMap.Find(SwappedID))
		{
			SwappedRef->Index = InBulletIndex;
		}
	}

	// 归属表维护(以弹内Owner弱引用为键,载体已销毁仍可正确移除防泄漏)
	if (TSet<int64>* OwnedIDs = OwnerProjectileMap.Find(Data.Owner))
	{
		OwnedIDs->Remove(ProjectileID);
		if (OwnedIDs->Num() == 0)
		{
			OwnerProjectileMap.Remove(Data.Owner);
		}
	}

	// 结构变更通知载体组件即时刷新复制快照
	if (UBXProjectileComponent* Carrier = Data.Carrier.Get())
	{
		Carrier->MarkReplicatedStatesDirty();
	}

	// Finished事件广播(移除后容器已稳定,回调内查询一致)
	FBXProjectileEventParameter Param;
	Param.ProjectileID = ProjectileID;
	Param.ProjectileType = InBucketType;
	Param.Instigator = Data.FireContext.Instigator;
	Param.Triggerer = Data.FireContext.Triggerer;
	Param.FinishReason = Data.FinishReason;
	Param.ContextData = MoveTemp(Data.FireContext.ContextData);

	if (UBXEventManager* EventMgr = UBXEventManager::Get(this))
	{
		EventMgr->BroadcastGlobalEvent(BXGameplayTags::BXEvent_Projectile_Finished, Param);
	}
}

void UBXProjectileManager::InternalRemoveProjectile(int64 InProjectileID, EBXProjectileFinishReason InReason)
{
	FBXProjectileIndexRef* Ref = ProjectileIndexMap.Find(InProjectileID);
	if (!Ref)
	{
		return;
	}

	const FGameplayTag BucketType = Ref->BucketType;
	FBXProjectileBucket* Bucket = Buckets.Find(BucketType);
	if (!Bucket)
	{
		ProjectileIndexMap.Remove(InProjectileID);
		return;
	}

	const int32 Index = Ref->Index;
	if (!Bucket->Bullets.IsValidIndex(Index) || !Bucket->BulletIDs.IsValidIndex(Index) || Bucket->BulletIDs[Index] != InProjectileID)
	{
		// 索引漂移防御(不应发生):按残留ID清理
		ProjectileIndexMap.Remove(InProjectileID);
		return;
	}

	// 更新窗口内仅标记死亡,生命周期阶段统一回收(避免遍历中数组结构变更)
	if (bInSimulation)
	{
		Bucket->Bullets[Index].State = EBXProjectileState::PS_Dead;
		Bucket->Bullets[Index].FinishReason = InReason;
		return;
	}

	Bucket->Bullets[Index].FinishReason = InReason;
	InternalRecycleBullet(BucketType, *Bucket, Index);

	if (Bucket->Bullets.Num() == 0)
	{
		DestroyBucketPresentation(*Bucket);
	}
}

void UBXProjectileManager::InternalReplaySimData(FBXProjectileBucket& InOutBucket, FBXProjectileSimData& InOutData, float InTargetElapsedTime, float InFixedStep)
{
	// 重放使用当前目标位置(轨迹与历史略有偏差,表现可接受)
	FBXProjectileTargetSnapshot TempSnapshot;
	const FBXProjectileTargetSnapshot* Snapshot = nullptr;
	AActor* Target = InOutData.FireContext.Target;
	if (IsValid(Target))
	{
		TempSnapshot.Location = Target->GetActorLocation();
		TempSnapshot.bValid = true;
		Snapshot = &TempSnapshot;
	}

	// 步数钳制:寿命内步数+2(残留推进余量)防御异常时长
	const int32 MaxStepCount = FMath::CeilToInt(InOutBucket.Baked.MaxLifetime / InFixedStep) + 2;
	const int32 StepCount = FMath::Min(FMath::FloorToInt(FMath::Max(InTargetElapsedTime, 0.0f) / InFixedStep), MaxStepCount);
	for (int32 StepIndex = 0; StepIndex < StepCount; ++StepIndex)
	{
		FBXProjectileSolver::IntegrateStep(InOutBucket.Baked, Snapshot, InFixedStep, InOutData);
		if (InOutData.State == EBXProjectileState::PS_Dead)
		{
			break;
		}
	}
}

bool UBXProjectileManager::InternalAuthorityFire(const FBXProjectileSyncHeader& InHeader, const FBXProjectileFireContext& InContext, UBXProjectileComponent* InCarrier)
{
	// 更新窗口内入挂起区,合并时统一入桶+组播
	if (bInSimulation)
	{
		FBXProjectilePendingEntry Entry;
		Entry.Header = InHeader;
		Entry.FireContext = InContext;
		Entry.bPredicted = false;
		Entry.Carrier = InCarrier;
		PendingProjectiles.Add(MoveTemp(Entry));
		return true;
	}

	if (!InternalAddProjectile(InHeader.ProjectileType, InHeader.ProjectileID, InContext, false, InCarrier))
	{
		return false;
	}

	if (InCarrier)
	{
		InCarrier->MulticastFireProjectile(InHeader, InContext);
	}
	return true;
}

void UBXProjectileManager::MergePendingProjectiles()
{
	if (PendingProjectiles.Num() == 0)
	{
		return;
	}

	UWorld* World = GetWorld();
	const bool bAuthority = !World || World->GetNetMode() != NM_Client;

	// 纯数据搬移入桶+Fired事件+发射表现+服务器侧补组播(回调中再发射走直加,不回挂起区)
	TArray<FBXProjectilePendingEntry> Entries = MoveTemp(PendingProjectiles);
	for (FBXProjectilePendingEntry& Entry : Entries)
	{
		if (!InternalAddProjectile(Entry.Header.ProjectileType, Entry.Header.ProjectileID, Entry.FireContext, Entry.bPredicted, Entry.Carrier.Get()))
		{
			continue;
		}

		// 服务器权威发射补组播(客户端预测弹不发)
		if (!Entry.bPredicted && bAuthority)
		{
			if (UBXProjectileComponent* Carrier = Entry.Carrier.Get())
			{
				Carrier->MulticastFireProjectile(Entry.Header, Entry.FireContext);
			}
		}
	}
}

void UBXProjectileManager::InternalBroadcastFired(const FGameplayTag& InProjectileType, const FBXProjectileFireContext& InContext, int64 InProjectileID)
{
	FBXProjectileEventParameter Param;
	Param.ProjectileID = InProjectileID;
	Param.ProjectileType = InProjectileType;
	Param.Instigator = InContext.Instigator;
	Param.Triggerer = InContext.Triggerer;
	Param.ContextData = InContext.ContextData;

	if (UBXEventManager* EventMgr = UBXEventManager::Get(this))
	{
		EventMgr->BroadcastGlobalEvent(BXGameplayTags::BXEvent_Projectile_Fired, Param);
	}
}

bool UBXProjectileManager::InternalPassesMathFilter(const FBXProjectileBucket& InBucket, AActor* InInstigator, AActor* InTarget) const
{
	if (!IsValid(InTarget) || !InBucket.Asset)
	{
		return false;
	}

	// 忽略自身
	if (InBucket.bFilterIgnoreSelf && InInstigator == InTarget)
	{
		return false;
	}

	const FBXCFilter& Filter = InBucket.Asset->PhysicsStrategy.Filter;

	// 检查Class
	const UClass* TargetClass = InTarget->GetClass();
	if (InBucket.FilterClassTypes.Num() > 0)
	{
		bool bClassPassed = false;
		for (const UClass* PassClass : InBucket.FilterClassTypes)
		{
			if (TargetClass->IsChildOf(PassClass))
			{
				bClassPassed = true;
				break;
			}
		}
		if (!bClassPassed)
		{
			return false;
		}
	}

	// 检查需要忽略的Class
	for (const UClass* IgnoreClass : InBucket.FilterIgnoreClassTypes)
	{
		if (TargetClass->IsChildOf(IgnoreClass))
		{
			return false;
		}
	}

	// 检查Actor的Tag
	if (Filter.ActorTags.Num() > 0)
	{
		bool bTagPassed = false;
		for (const FName& ActorTag : Filter.ActorTags)
		{
			if (InTarget->ActorHasTag(ActorTag))
			{
				bTagPassed = true;
				break;
			}
		}
		if (!bTagPassed)
		{
			return false;
		}
	}

	// 检查需要忽略的Actor Tag
	for (const FName& IgnoreActorTag : Filter.IgnoreActorTags)
	{
		if (InTarget->ActorHasTag(IgnoreActorTag))
		{
			return false;
		}
	}

	return true;
}

#pragma endregion Simulation



#pragma region Net
bool UBXProjectileManager::HandleServerFireProjectile(const FBXProjectileSyncHeader& InHeader, const FBXProjectileFireContext& InContext, UBXProjectileComponent* InCarrier, EBXProjectileDenyReason& OutDenyReason)
{
	// 发起端必须为Client(防服务器ID段伪造/碰撞)
	if (InHeader.Initiator != EBXSyncInitiator::Client || InHeader.ProjectileID == 0)
	{
		OutDenyReason = EBXProjectileDenyReason::DR_DuplicateID;
		return false;
	}

	// 连接归属校验:发射请求的Instigator须属于发起RPC的连接(防Instigator伪造嫁祸他人/绕过归属约束)
	AActor* CarrierOwner = IsValid(InCarrier) ? InCarrier->GetOwner() : nullptr;
	if (!IsValid(InContext.Instigator) || !CarrierOwner || CarrierOwner->GetNetConnection() != InContext.Instigator->GetNetConnection())
	{
		OutDenyReason = EBXProjectileDenyReason::DR_InvalidOwnership;
		return false;
	}

	// ID查重(防跨客户端ID碰撞覆盖正在运行的子弹)
	if (ProjectileIndexMap.Contains(InHeader.ProjectileID))
	{
		OutDenyReason = EBXProjectileDenyReason::DR_DuplicateID;
		return false;
	}

	// 资源校验
	if (!GetOrLoadProjectileAsset(InHeader.ProjectileType))
	{
		OutDenyReason = EBXProjectileDenyReason::DR_AssetMissing;
		return false;
	}

	// 请求年龄校验(服务器世界时间域,防陈旧请求重放)
	const UBXSettings* Settings = GetDefault<UBXSettings>();
	const int64 NowMs = UBXFunctionLibrary::GetServerWorldTimeMilliseconds(this);
	if (InHeader.ClientTimestamp > 0 && FMath::Abs(NowMs - InHeader.ClientTimestamp) > Settings->SkillRequestMaxAgeMs)
	{
		OutDenyReason = EBXProjectileDenyReason::DR_RequestExpired;
		return false;
	}

	// 权威发射(时间戳改用服务器当前,组播携带供远端补偿)
	FBXProjectileSyncHeader ServerHeader = InHeader;
	ServerHeader.ClientTimestamp = NowMs;

	UBXProjectileComponent* Carrier = InCarrier ? InCarrier : GetOrCreateCarrier(InContext.Instigator);
	return InternalAuthorityFire(ServerHeader, InContext, Carrier);
}

int64 UBXProjectileManager::StartPredictedProjectile(FGameplayTag InProjectileType, const FBXProjectileFireContext& InContext, int64 InProjectileID, UBXProjectileComponent* InCarrier)
{
	if (InProjectileID == 0)
	{
		return 0;
	}

	// 模拟期间入挂起区(合并时统一入桶,表现延迟至步末不可感知)
	if (bInSimulation)
	{
		FBXProjectilePendingEntry Entry;
		Entry.Header.ProjectileID = InProjectileID;
		Entry.Header.ProjectileType = InProjectileType;
		Entry.Header.Initiator = EBXSyncInitiator::Client;
		Entry.Header.ClientTimestamp = InContext.ClientTimestamp;
		Entry.FireContext = InContext;
		Entry.bPredicted = true;
		Entry.Carrier = InCarrier;
		PendingProjectiles.Add(MoveTemp(Entry));
		return InProjectileID;
	}

	return InternalAddProjectile(InProjectileType, InProjectileID, InContext, true, InCarrier) ? InProjectileID : 0;
}

void UBXProjectileManager::StartRemoteProjectile(const FBXProjectileSyncHeader& InHeader, const FBXProjectileFireContext& InContext, UBXProjectileComponent* InCarrier)
{
	if (IsProjectileRunning(InHeader.ProjectileID))
	{
		return;
	}

	FBXProjectileBucket* Bucket = GetOrCreateBucket(InHeader.ProjectileType);
	if (!Bucket)
	{
		UE_LOG(BXMGR_Projectile, Warning, TEXT("StartRemoteProjectile: 子弹种类不存在 %s"), *InHeader.ProjectileType.ToString());
		return;
	}

	// 时间对齐:按服务器世界时间域估算已飞行时长(RPC传输延迟补偿)
	const int64 NowMs = UBXFunctionLibrary::GetServerWorldTimeMilliseconds(this);
	const float ElapsedSeconds = InHeader.ClientTimestamp > 0 ? FMath::Max(NowMs - InHeader.ClientTimestamp, 0LL) / 1000.0f : 0.0f;

	const UBXSettings* Settings = GetDefault<UBXSettings>();
	const float FixedStep = FMath::Max(Settings->ProjectileFixedStep, 0.005f);

	FBXProjectileSimData NewData;
	InternalInitSimData(*Bucket, InContext, InHeader.ProjectileID, false, InCarrier, NewData);
	InternalReplaySimData(*Bucket, NewData, ElapsedSeconds, FixedStep);
	InternalAppendBullet(InHeader.ProjectileType, *Bucket, MoveTemp(NewData));

	InternalBroadcastFired(InHeader.ProjectileType, InContext, InHeader.ProjectileID);

	// 发射表现:仅低延迟场景播放(高延迟远端弹中途出现,起点特效已过时)
	if (ElapsedSeconds < 0.25f)
	{
		PresentationFire(*Bucket, InContext.StartLocation);
	}
}

void UBXProjectileManager::HandleProjectileDenied(int64 InProjectileID)
{
	// Denied事件广播(移除前构建参数)
	if (const FBXProjectileIndexRef* Ref = ProjectileIndexMap.Find(InProjectileID))
	{
		if (const FBXProjectileBucket* Bucket = Buckets.Find(Ref->BucketType))
		{
			if (Bucket->Bullets.IsValidIndex(Ref->Index) && Bucket->BulletIDs[Ref->Index] == InProjectileID)
			{
				const FBXProjectileSimData& Data = Bucket->Bullets[Ref->Index];

				FBXProjectileEventParameter Param;
				Param.ProjectileID = InProjectileID;
				Param.ProjectileType = Ref->BucketType;
				Param.Instigator = Data.FireContext.Instigator;
				Param.Triggerer = Data.FireContext.Triggerer;
				Param.ContextData = Data.FireContext.ContextData;

				if (UBXEventManager* EventMgr = UBXEventManager::Get(this))
				{
					EventMgr->BroadcastGlobalEvent(BXGameplayTags::BXEvent_Projectile_Denied, Param);
				}
			}
		}
	}

	// 预测弹回滚移除
	InternalRemoveProjectile(InProjectileID, EBXProjectileFinishReason::FR_PredictDenied);
}

void UBXProjectileManager::HandleHitMulticast(const FBXProjectileHitPayload& InPayload, bool bTerminal)
{
	FBXProjectileIndexRef* Ref = ProjectileIndexMap.Find(InPayload.ProjectileID);
	FBXProjectileBucket* Bucket = Ref ? Buckets.Find(Ref->BucketType) : nullptr;
	const bool bIndexValid = Bucket && Bucket->Bullets.IsValidIndex(Ref->Index) && Bucket->BulletIDs.IsValidIndex(Ref->Index) && Bucket->BulletIDs[Ref->Index] == InPayload.ProjectileID;

	// 代劳端回声去重:服务器组播与本地上报命中同时刻(原样回传的子弹时刻)则跳过重复表现与事件,其余命中正常表现
	if (!bIndexValid || !InternalIsLocalReportedEcho(Bucket->Bullets[Ref->Index], InPayload.HitTime))
	{
		if (FBXProjectileBucket* PresentationBucket = Buckets.Find(InPayload.ProjectileType))
		{
			PresentationHit(*PresentationBucket, InPayload.HitLocation, InPayload.HitNormal);
		}

		// Hit事件广播(服务器结算与客户端组播统一入口)
		FBXProjectileHitPayload Payload = InPayload;
		if (UBXEventManager* EventMgr = UBXEventManager::Get(this))
		{
			EventMgr->BroadcastGlobalEvent(BXGameplayTags::BXEvent_Projectile_Hit, Payload);
		}
	}

	// 终态流转
	if (!bTerminal || !bIndexValid)
	{
		return;
	}

	InternalTerminateBullet(*Bucket, Ref->Index);
}

void UBXProjectileManager::HandleStopMulticast(int64 InProjectileID)
{
	InternalRemoveProjectile(InProjectileID, EBXProjectileFinishReason::FR_Interrupted);
}

void UBXProjectileManager::HandleServerReportProjectileHit(const FBXProjectileHitPayload& InPayload, UBXProjectileComponent* InReporter)
{
	UWorld* World = GetWorld();
	if (!World || World->GetNetMode() == NM_Client)
	{
		return;
	}

	// 服务器自检模式下客户端上报通道无效(命中由服务器权威Sweep判定,拒绝上报防伪造命中作弊)
	if (GetDefault<UBXSettings>()->bServerCollisionCheck)
	{
		return;
	}

	// 子弹存在且仍在飞行
	FBXProjectileIndexRef* Ref = ProjectileIndexMap.Find(InPayload.ProjectileID);
	if (!Ref)
	{
		return;
	}

	FBXProjectileBucket* Bucket = Buckets.Find(Ref->BucketType);
	if (!Bucket || !Bucket->Bullets.IsValidIndex(Ref->Index) || !Bucket->BulletIDs.IsValidIndex(Ref->Index) || Bucket->BulletIDs[Ref->Index] != InPayload.ProjectileID)
	{
		return;
	}

	FBXProjectileSimData& Data = Bucket->Bullets[Ref->Index];
	if (Data.State != EBXProjectileState::PS_Active)
	{
		return;
	}

	// 连接归属校验:上报组件拥有者与始作俑者须同一客户端连接(防替他人伪造上报)
	AActor* Instigator = Data.FireContext.Instigator;
	AActor* ReporterOwner = IsValid(InReporter) ? InReporter->GetOwner() : nullptr;
	if (!IsValid(Instigator) || !ReporterOwner || ReporterOwner->GetNetConnection() != Instigator->GetNetConnection())
	{
		return;
	}

	// 权威字段以服务器数据为准(不信任客户端载荷;HitTime刻意保留客户端原值,组播回声去重按此匹配)
	FBXProjectileHitPayload Payload = InPayload;
	Payload.ProjectileType = Ref->BucketType;
	Payload.Instigator = Instigator;
	Payload.Triggerer = Data.FireContext.Triggerer;
	Payload.ContextData = Data.FireContext.ContextData;

	bool bTerminal = true;
	if (InPayload.HitType == EBXProjectileHitType::HT_Unit)
	{
		// 单位命中:目标有效+筛选器+穿透去重
		AActor* Target = InPayload.HitTarget;
		if (!IsValid(Target) || !InternalPassesMathFilter(*Bucket, Instigator, Target))
		{
			return;
		}

		const uint32 TargetUID = static_cast<uint32>(Target->GetUniqueID());
		if (InternalIsTargetInCooldown(Data, TargetUID))
		{
			return;
		}

		InternalMarkTargetHitCooldown(Data, TargetUID, Bucket->Baked.HitCooldown);
		bTerminal = Data.PenetrationCount >= Bucket->Baked.MaxPenetrationCount - 1;
		if (!bTerminal)
		{
			++Data.PenetrationCount;
		}

		// 命中效果(客户端代劳模式,校验通过后权威端执行)
		InternalApplyHitEffects(*Bucket, Data, Target);
	}

	// 权威结算组播(本端经本地执行表现/事件/终态流转,无载体兜底仅本端)
	if (UBXProjectileComponent* Carrier = Data.Carrier.Get())
	{
		Carrier->MulticastProjectileHit(Payload, bTerminal);
	}
	else
	{
		HandleHitMulticast(Payload, bTerminal);
	}
}

void UBXProjectileManager::HandleSnapshotMulticast(const FBXProjectileSnapshotBatch& InBatch)
{
	// 仅客户端矫正(服务器/单机模拟即权威)
	UWorld* World = GetWorld();
	if (!World || World->GetNetMode() != NM_Client || InBatch.ServerTimestamp <= 0)
	{
		return;
	}

	// 传输延迟外推(服务器世界时间域)
	const int64 NowMs = UBXFunctionLibrary::GetServerWorldTimeMilliseconds(this);
	const float LatencySeconds = FMath::Max(NowMs - InBatch.ServerTimestamp, 0LL) / 1000.0f;

	for (const FBXProjectileSnapshotEntry& Entry : InBatch.Entries)
	{
		FBXProjectileIndexRef* Ref = ProjectileIndexMap.Find(Entry.ProjectileID);
		if (!Ref)
		{
			continue;
		}

		FBXProjectileBucket* Bucket = Buckets.Find(Ref->BucketType);
		if (!Bucket || !Bucket->Bullets.IsValidIndex(Ref->Index) || !Bucket->BulletIDs.IsValidIndex(Ref->Index) || Bucket->BulletIDs[Ref->Index] != Entry.ProjectileID)
		{
			continue;
		}

		FBXProjectileSimData& Data = Bucket->Bullets[Ref->Index];
		if (Data.State != EBXProjectileState::PS_Active)
		{
			continue;
		}

		// 弹簧矫正目标:快照外推位置与本地模拟位置的偏差(弹簧平滑收敛,模拟锚点不跳变)
		const FVector ExpectedLocation = Entry.Location + Entry.Velocity * LatencySeconds;
		const FVector Delta = ExpectedLocation - Data.Location;
		if (Delta.IsNearlyZero(1.0f))
		{
			Data.SpringTarget = FVector::ZeroVector;
			continue;
		}

		Data.SpringTarget = Delta;
	}
}

void UBXProjectileManager::InternalUpdateSnapshots(float InDeltaTime)
{
	// 仅服务器下发(客户端无广播权)
	UWorld* World = GetWorld();
	if (!World || World->GetNetMode() == NM_Client)
	{
		return;
	}

	const UBXSettings* Settings = GetDefault<UBXSettings>();
	const float Interval = Settings->ProjectileSnapshotInterval;
	if (Interval <= 0.0f)
	{
		return;
	}

	SnapshotTimer += InDeltaTime;
	if (SnapshotTimer < Interval)
	{
		return;
	}
	SnapshotTimer = FMath::Fmod(SnapshotTimer, Interval);

	// 收集长寿命子弹快照(仅飞行中,残留态表现淡出无需矫正)
	FBXProjectileSnapshotBatch Batch;
	Batch.ServerTimestamp = UBXFunctionLibrary::GetServerWorldTimeMilliseconds(this);
	for (TPair<FGameplayTag, FBXProjectileBucket>& Pair : Buckets)
	{
		for (const FBXProjectileSimData& Data : Pair.Value.Bullets)
		{
			if (Data.State != EBXProjectileState::PS_Active || Data.ElapsedTime < Settings->ProjectileSnapshotMinAge)
			{
				continue;
			}

			FBXProjectileSnapshotEntry Entry;
			Entry.ProjectileID = Data.ProjectileID;
			Entry.Location = Data.Location;
			Entry.Velocity = Data.Velocity;
			Entry.ElapsedTime = Data.ElapsedTime;
			Batch.Entries.Add(Entry);
		}
	}

	if (Batch.Entries.Num() == 0)
	{
		return;
	}

	// 经全局快照通道组播(独立于具体发射者,全部客户端可见)
	if (UBXProjectileComponent* Channel = GetOrCreateSnapshotChannel())
	{
		Channel->MulticastProjectileSnapshots(Batch);
	}
}

UBXProjectileComponent* UBXProjectileManager::GetOrCreateSnapshotChannel()
{
	if (UBXProjectileComponent* Existing = SnapshotChannel.Get())
	{
		return Existing;
	}

	UWorld* World = GetWorld();
	if (!World || World->GetNetMode() == NM_Client)
	{
		return nullptr;
	}

	// 全局通道挂靠GameState(常驻复制且AlwaysRelevant,不随具体发射者销毁)
	AActor* Host = World->GetGameState();
	if (!IsValid(Host))
	{
		return nullptr;
	}

	UBXProjectileComponent* Channel = NewObject<UBXProjectileComponent>(Host, NAME_None, RF_Transient);
	Channel->SetIsReplicatedByDefault(true);
	Host->AddInstanceComponent(Channel);
	Channel->RegisterComponent();
	SnapshotChannel = Channel;
	return Channel;
}

void UBXProjectileManager::ConfirmPredictedProjectile(int64 InProjectileID)
{
	FBXProjectileIndexRef* Ref = ProjectileIndexMap.Find(InProjectileID);
	if (!Ref)
	{
		return;
	}

	FBXProjectileBucket* Bucket = Buckets.Find(Ref->BucketType);
	if (Bucket && Bucket->Bullets.IsValidIndex(Ref->Index) && Bucket->BulletIDs[Ref->Index] == InProjectileID)
	{
		Bucket->Bullets[Ref->Index].bPredicted = false;
	}
}

void UBXProjectileManager::BuildReplicatedStates(AActor* InOwner, TArray<FBXProjectileReplicatedState>& OutStates)
{
	OutStates.Reset();

	const TSet<int64>* OwnedIDs = OwnerProjectileMap.Find(InOwner);
	if (!OwnedIDs)
	{
		return;
	}

	OutStates.Reserve(OwnedIDs->Num());
	for (const int64 ProjectileID : *OwnedIDs)
	{
		const FBXProjectileIndexRef* Ref = ProjectileIndexMap.Find(ProjectileID);
		if (!Ref)
		{
			continue;
		}

		const FBXProjectileBucket* Bucket = Buckets.Find(Ref->BucketType);
		if (!Bucket || !Bucket->Bullets.IsValidIndex(Ref->Index) || !Bucket->BulletIDs.IsValidIndex(Ref->Index) || Bucket->BulletIDs[Ref->Index] != ProjectileID)
		{
			continue;
		}

		const FBXProjectileSimData& Data = Bucket->Bullets[Ref->Index];

		FBXProjectileReplicatedState State;
		State.ProjectileID = ProjectileID;
		State.ProjectileType = Ref->BucketType;
		State.FireContext = Data.FireContext;
		State.ElapsedTime = Data.ElapsedTime;
		State.Location = Data.Location;
		State.State = Data.State;
		OutStates.Add(MoveTemp(State));
	}
}

void UBXProjectileManager::RebuildProjectileFromState(const FBXProjectileReplicatedState& InState, UBXProjectileComponent* InCarrier)
{
	// 仅客户端重建(服务器/单机自持运行数据)
	UWorld* World = GetWorld();
	if (!World || World->GetNetMode() != NM_Client || IsProjectileRunning(InState.ProjectileID))
	{
		return;
	}

	FBXProjectileBucket* Bucket = GetOrCreateBucket(InState.ProjectileType);
	if (!Bucket)
	{
		return;
	}

	const UBXSettings* Settings = GetDefault<UBXSettings>();
	const float FixedStep = FMath::Max(Settings->ProjectileFixedStep, 0.005f);

	FBXProjectileSimData NewData;
	InternalInitSimData(*Bucket, InState.FireContext, InState.ProjectileID, false, InCarrier, NewData);

	// 确定性重放至快照时长(只算位置,不触发命中与表现)
	InternalReplaySimData(*Bucket, NewData, InState.ElapsedTime, FixedStep);

	// 残留态直接放置(剩余时长快照未携带,按完整残留近似)
	if (InState.State == EBXProjectileState::PS_Residual && NewData.State == EBXProjectileState::PS_Active)
	{
		NewData.State = EBXProjectileState::PS_Residual;
		NewData.ResidualTimer = Bucket->Baked.ResidualTime;
	}

	// 位置锚点校正:重放轨迹与快照位置的偏差(目标移动等)以快照为准,偏差记入弹簧偏移由弹簧平滑收敛(渲染位置无跳变)
	const FVector Delta = InState.Location - NewData.Location;
	if (!Delta.IsNearlyZero())
	{
		NewData.Location = InState.Location;
		NewData.SpringOffset = -Delta;
	}
	NewData.PrevLocation = NewData.Location;
	NewData.LastSweptLocation = NewData.Location;

	InternalAppendBullet(InState.ProjectileType, *Bucket, MoveTemp(NewData));

	// Fired事件广播(不播发射表现:中途加入)
	InternalBroadcastFired(InState.ProjectileType, InState.FireContext, InState.ProjectileID);
}

void UBXProjectileManager::RemoveProjectileIfNotPredicted(int64 InProjectileID)
{
	// 预测弹保留:等待服务器命中/停止/否认结论(组播可能晚于快照到达)
	const FBXProjectileIndexRef* Ref = ProjectileIndexMap.Find(InProjectileID);
	if (!Ref)
	{
		return;
	}

	const FBXProjectileBucket* Bucket = Buckets.Find(Ref->BucketType);
	if (Bucket && Bucket->Bullets.IsValidIndex(Ref->Index) && Bucket->BulletIDs[Ref->Index] == InProjectileID && Bucket->Bullets[Ref->Index].bPredicted)
	{
		return;
	}

	InternalRemoveProjectile(InProjectileID, EBXProjectileFinishReason::FR_Interrupted);
}

UBXProjectileComponent* UBXProjectileManager::GetOrCreateCarrier(AActor* InOwner)
{
	if (!IsValid(InOwner))
	{
		return nullptr;
	}

	if (UBXProjectileComponent* Existing = InOwner->FindComponentByClass<UBXProjectileComponent>())
	{
		return Existing;
	}

	// 权威端惰性创建广播载体(客户端由复制通道建立,不做主)
	UWorld* World = GetWorld();
	if (!World || World->GetNetMode() == NM_Client)
	{
		return nullptr;
	}

	UBXProjectileComponent* NewCarrier = NewObject<UBXProjectileComponent>(InOwner, NAME_None, RF_Transient);
	NewCarrier->SetIsReplicatedByDefault(true);
	InOwner->AddInstanceComponent(NewCarrier);
	NewCarrier->RegisterComponent();

	if (!InOwner->GetIsReplicated())
	{
		UE_LOG(BXMGR_Projectile, Warning, TEXT("GetOrCreateCarrier: 始作俑者%s未启用复制,子弹无法同步"), *InOwner->GetName());
	}
	return NewCarrier;
}

#pragma endregion Net



#pragma region Presentation
void UBXProjectileManager::PresentationFire(const FBXProjectileBucket& InBucket, const FVector& InLocation)
{
	UWorld* World = GetWorld();
	if (!World || !InternalIsPresentationAllowed())
	{
		return;
	}

	if (UNiagaraSystem* System = InBucket.FireSystem.Get())
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(World, System, InLocation);
	}

	if (USoundBase* Sound = InBucket.FireSound.Get())
	{
		UGameplayStatics::PlaySoundAtLocation(this, Sound, InLocation);
	}
}

void UBXProjectileManager::PresentationHit(const FBXProjectileBucket& InBucket, const FVector& InLocation, const FVector& InNormal)
{
	UWorld* World = GetWorld();
	if (!World || !InternalIsPresentationAllowed())
	{
		return;
	}

	if (UNiagaraSystem* System = InBucket.HitSystem.Get())
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(World, System, InLocation, InNormal.Rotation());
	}

	if (USoundBase* Sound = InBucket.HitSound.Get())
	{
		UGameplayStatics::PlaySoundAtLocation(this, Sound, InLocation);
	}
}

void UBXProjectileManager::CommitRender()
{
	if (!InternalIsPresentationAllowed())
	{
		return;
	}

	for (TPair<FGameplayTag, FBXProjectileBucket>& Pair : Buckets)
	{
		FBXProjectileBucket& Bucket = Pair.Value;
		const int32 BulletCount = Bucket.Bullets.Num();
		if (BulletCount == 0)
		{
			continue;
		}

		// 批量提交用户数组(单组件支撑数千实例)
		if (UNiagaraComponent* FlightComp = Bucket.FlightComponent.Get())
		{
			Bucket.ScratchPositions.Reset(BulletCount);
			Bucket.ScratchVelocities.Reset(BulletCount);
			Bucket.ScratchAges.Reset(BulletCount);

			for (const FBXProjectileSimData& Data : Bucket.Bullets)
			{
				// 渲染位置叠加弹簧偏移(与检测线段一致,客户端矫正平滑无跳变)
				Bucket.ScratchPositions.Add(Data.Location + Data.SpringOffset);
				Bucket.ScratchVelocities.Add(Data.Velocity);
				Bucket.ScratchAges.Add(Data.ElapsedTime);
			}

			UNiagaraFunctionLibrary::SetNiagaraArrayVector(FlightComp, TEXT("ProjectilePositions"), Bucket.ScratchPositions);
			UNiagaraFunctionLibrary::SetNiagaraArrayVector(FlightComp, TEXT("ProjectileVelocities"), Bucket.ScratchVelocities);
			UNiagaraFunctionLibrary::SetNiagaraArrayFloat(FlightComp, TEXT("ProjectileAges"), Bucket.ScratchAges);
		}

		// 飞行音效聚合:组件置于桶内子弹质心,音量随数量缩放
		if (UAudioComponent* FlightSoundComp = Bucket.FlightSoundComponent.Get())
		{
			FVector Centroid = FVector::ZeroVector;
			for (const FBXProjectileSimData& Data : Bucket.Bullets)
			{
				Centroid += Data.Location;
			}
			Centroid /= static_cast<float>(BulletCount);

			FlightSoundComp->SetWorldLocation(Centroid);
			FlightSoundComp->SetVolumeMultiplier(FMath::Clamp(static_cast<float>(BulletCount) / 16.0f, 0.0f, 1.0f));
		}
	}
}

bool UBXProjectileManager::InternalIsPresentationAllowed() const
{
	const UWorld* World = GetWorld();
	return World && World->GetNetMode() != NM_DedicatedServer;
}

AActor* UBXProjectileManager::InternalGetPresentationHost()
{
	if (AActor* Host = PresentationHost.Get())
	{
		return Host;
	}

	UWorld* World = GetWorld();
	if (!World || !InternalIsPresentationAllowed())
	{
		return nullptr;
	}

	// 隐藏瞬态宿主Actor:持有各桶飞行渲染组件,随世界销毁
	FActorSpawnParameters SpawnParams;
	SpawnParams.ObjectFlags |= RF_Transient;
	AActor* Host = World->SpawnActor<AActor>(AActor::StaticClass(), FTransform::Identity, SpawnParams);
	PresentationHost = Host;
	return Host;
}

void UBXProjectileManager::InternalEnsureFlightPresentation(FBXProjectileBucket& InOutBucket)
{
	if (!InternalIsPresentationAllowed() || InOutBucket.FlightComponent.IsValid() || !InOutBucket.Asset)
	{
		return;
	}

	UNiagaraSystem* FlightSystem = InOutBucket.Asset->FlightSystem.LoadSynchronous();
	if (!FlightSystem)
	{
		return;
	}

	AActor* Host = InternalGetPresentationHost();
	if (!Host)
	{
		return;
	}

	UNiagaraComponent* FlightComp = NewObject<UNiagaraComponent>(Host);
	FlightComp->SetAsset(FlightSystem);
	FlightComp->SetAbsolute(true, true, true);
	FlightComp->bAutoDestroy = false;
	FlightComp->RegisterComponent();
	InOutBucket.FlightComponent = FlightComp;

	// 飞行音效聚合组件(可空)
	if (USoundBase* Sound = InOutBucket.FlightSound.Get())
	{
		UAudioComponent* FlightSoundComp = NewObject<UAudioComponent>(Host);
		FlightSoundComp->SetSound(Sound);
		FlightSoundComp->bAutoDestroy = false;
		FlightSoundComp->SetVolumeMultiplier(0.0f);
		FlightSoundComp->RegisterComponent();
		FlightSoundComp->Play(0.0f);
		InOutBucket.FlightSoundComponent = FlightSoundComp;
	}
}

#pragma endregion Presentation
