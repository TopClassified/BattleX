#include "BXProjectileComputeWorker.h"

#include "BXProjectileSolver.h"

#include "Async/ParallelFor.h"



namespace
{
	// 分片描述(全桶统一切片,跨桶汇总分片解决桶间串行)
	struct FBXProjectileComputeShard
	{
		// Job桶条目索引
		int32 BucketEntryIndex = INDEX_NONE;

		// 分片起始子弹索引(含)
		int32 BulletStart = INDEX_NONE;

		// 分片结束子弹索引(不含)
		int32 BulletEnd = INDEX_NONE;
	};
}



FBXProjectileComputeWorker::FBXProjectileComputeWorker()
{
	WakeEvent = FPlatformProcess::GetSynchEventFromPool();
}

FBXProjectileComputeWorker::~FBXProjectileComputeWorker()
{
	RequestExit();

	if (Thread)
	{
		Thread->Kill(true);
		delete Thread;
		Thread = nullptr;
	}

	if (WakeEvent)
	{
		FPlatformProcess::ReturnSynchEventToPool(WakeEvent);
		WakeEvent = nullptr;
	}

	// 清空队列(节点值逐个析构,Job/Result含TArray堆内存)
	JobQueue.Empty();
	ResultQueue.Empty();
}

bool FBXProjectileComputeWorker::EnqueueJob(FBXProjectileComputeJob&& InJob)
{
	if (bStopRequested.load(std::memory_order_acquire))
	{
		return false;
	}

	SubmittedSerial.fetch_add(1, std::memory_order_release);
	JobQueue.Enqueue(MoveTemp(InJob));

	// 入队后确认线程存活:自回收竞态窗口内线程可能已标记消亡,重启消费滞留Job(否则IsBusy永真冻结模拟)
	EnsureThread();

	WakeEvent->Trigger();
	return true;
}

bool FBXProjectileComputeWorker::DequeueResult(FBXProjectileComputeResult& OutResult)
{
	return ResultQueue.Dequeue(OutResult);
}

bool FBXProjectileComputeWorker::IsBusy() const
{
	return SubmittedSerial.load(std::memory_order_acquire) > CompletedSerial.load(std::memory_order_acquire);
}

void FBXProjectileComputeWorker::RequestExit()
{
	bStopRequested.store(true, std::memory_order_release);
	if (WakeEvent)
	{
		WakeEvent->Trigger();
	}
}

uint32 FBXProjectileComputeWorker::Run()
{
	bThreadAlive.store(true, std::memory_order_release);

	const double IdleTimeoutSeconds = 10.0;
	double IdleSeconds = 0.0;

	while (!bStopRequested.load(std::memory_order_acquire))
	{
		FBXProjectileComputeJob Job;
		if (JobQueue.Dequeue(Job))
		{
			FBXProjectileComputeResult Result;
			ProcessJob(Job, Result);
			ResultQueue.Enqueue(MoveTemp(Result));
			CompletedSerial.fetch_add(1, std::memory_order_release);
			IdleSeconds = 0.0;
			continue;
		}

		WakeEvent->Wait(100);
		IdleSeconds += 0.1;
		if (IdleSeconds >= IdleTimeoutSeconds)
		{
			// 自回收退出协议:先标记消亡再终查队列;终查非空且未被Kill等待则复活消费,否则退出由提交方重启新线程消费滞留Job
			bThreadAlive.store(false, std::memory_order_release);
			if (!JobQueue.IsEmpty() && !bStopRequested.load(std::memory_order_acquire))
			{
				bThreadAlive.store(true, std::memory_order_release);
				IdleSeconds = 0.0;
				continue;
			}
			break;
		}
	}

	bThreadAlive.store(false, std::memory_order_release);
	return 0;
}

void FBXProjectileComputeWorker::Stop()
{
	// Kill(true)等待前由框架调用:置停机标记使超时复活分支放弃消费直接退出,防止GT Join与复活消费互相等待死锁
	bStopRequested.store(true, std::memory_order_release);
}

void FBXProjectileComputeWorker::ProcessJob(FBXProjectileComputeJob& InJob, FBXProjectileComputeResult& OutResult)
{
	OutResult.StepSerial = InJob.StepSerial;

	// 全桶统一切片:每桶64发一片,汇总所有桶的分片一次性提交
	const int32 BulletsPerShard = 64;
	TArray<FBXProjectileComputeShard> Shards;
	for (int32 EntryIndex = 0; EntryIndex < InJob.BucketEntries.Num(); ++EntryIndex)
	{
		const int32 BulletCount = InJob.BucketEntries[EntryIndex].Bullets.Num();
		const int32 EntryShardCount = FMath::Max(FMath::DivideAndRoundUp(BulletCount, BulletsPerShard), 1);
		for (int32 ShardIndex = 0; ShardIndex < EntryShardCount; ++ShardIndex)
		{
			FBXProjectileComputeShard Shard;
			Shard.BucketEntryIndex = EntryIndex;
			Shard.BulletStart = ShardIndex * BulletsPerShard;
			Shard.BulletEnd = FMath::Min(Shard.BulletStart + BulletsPerShard, BulletCount);
			Shards.Add(Shard);
		}
	}

	// 分片候选独写数组(零锁)
	const bool bPerBulletCollect = !InJob.bAuthority;
	const FBXProjectileTargetSnapshot* Snapshots = InJob.TargetSnapshots.GetData();
	const int32 SnapshotCount = InJob.TargetSnapshots.Num();
	TArray<TArray<FBXProjectileHitCandidate>> ShardCandidates;
	ShardCandidates.SetNum(Shards.Num());

	auto ProcessShard = [&InJob, &Shards, &ShardCandidates, bPerBulletCollect, Snapshots, SnapshotCount](int32 ShardIndex)
	{
		const FBXProjectileComputeShard& Shard = Shards[ShardIndex];
		FBXProjectileComputeBucketEntry& Entry = InJob.BucketEntries[Shard.BucketEntryIndex];
		TArray<FBXProjectileHitCandidate>& Candidates = ShardCandidates[ShardIndex];
		Candidates.Reset();

		for (int32 Index = Shard.BulletStart; Index < Shard.BulletEnd; ++Index)
		{
			FBXProjectileSimData& Data = Entry.Bullets[Index];
			const FBXProjectileTargetSnapshot* Snapshot = (Data.TargetSnapshotIndex >= 0 && Data.TargetSnapshotIndex < SnapshotCount) ? &Snapshots[Data.TargetSnapshotIndex] : nullptr;
			FBXProjectileSolver::IntegrateStep(Entry.Baked, Snapshot, InJob.FixedStep, Data);
			if (Entry.bCollectMath && (!bPerBulletCollect || Data.bLocalDetectable))
			{
				FBXProjectileSolver::MathCheckBullet(Entry.Baked, Data, Index, Snapshots, SnapshotCount, Candidates);
			}
		}
	};

	const int32 ShardCount = Shards.Num();
	if (ShardCount > 1)
	{
		ParallelFor(ShardCount, ProcessShard);
	}
	else if (ShardCount == 1)
	{
		ProcessShard(0);
	}

	// 候选聚合:分片候选转异步形态(以ID与弱引用定位,跨步有效;须在桶结果搬运前读取副本)
	for (int32 ShardIndex = 0; ShardIndex < ShardCount; ++ShardIndex)
	{
		const FBXProjectileComputeShard& Shard = Shards[ShardIndex];
		const FBXProjectileComputeBucketEntry& Entry = InJob.BucketEntries[Shard.BucketEntryIndex];
		for (const FBXProjectileHitCandidate& Candidate : ShardCandidates[ShardIndex])
		{
			if (!Entry.Bullets.IsValidIndex(Candidate.BulletIndex) || !InJob.TargetSnapshots.IsValidIndex(Candidate.TargetIndex))
			{
				continue;
			}

			const FBXProjectileTargetSnapshot& Snapshot = InJob.TargetSnapshots[Candidate.TargetIndex];

			FBXProjectileAsyncHitCandidate AsyncCandidate;
			AsyncCandidate.ProjectileID = Entry.Bullets[Candidate.BulletIndex].ProjectileID;
			AsyncCandidate.BucketType = Entry.BucketType;
			AsyncCandidate.Target = Snapshot.Target;
			AsyncCandidate.TargetUID = Snapshot.TargetUID;
			AsyncCandidate.HitLocation = Candidate.HitLocation;
			AsyncCandidate.HitNormal = Candidate.HitNormal;
			AsyncCandidate.HitBoxTag = Candidate.HitBoxTag;
			OutResult.Candidates.Add(AsyncCandidate);
		}
	}

	// 桶结果搬运:积分后副本与基线自Job移交Result(收割守卫与弹簧合并用)
	OutResult.BucketResults.SetNum(InJob.BucketEntries.Num());
	for (int32 EntryIndex = 0; EntryIndex < InJob.BucketEntries.Num(); ++EntryIndex)
	{
		FBXProjectileComputeBucketEntry& Entry = InJob.BucketEntries[EntryIndex];
		FBXProjectileComputeBucketResult& BucketResult = OutResult.BucketResults[EntryIndex];
		BucketResult.BucketType = Entry.BucketType;
		BucketResult.Bullets = MoveTemp(Entry.Bullets);
		BucketResult.BaseStates = MoveTemp(Entry.BaseStates);
		BucketResult.BaseSpringOffsets = MoveTemp(Entry.BaseSpringOffsets);
	}
}

void FBXProjectileComputeWorker::EnsureThread()
{
	if (Thread && !bThreadAlive.load(std::memory_order_acquire))
	{
		// 空闲自回收后的旧句柄回收
		Thread->Kill(true);
		delete Thread;
		Thread = nullptr;
	}

	if (!Thread)
	{
		bStopRequested.store(false, std::memory_order_release);
		Thread = FRunnableThread::Create(this, TEXT("BXProjectileComputeWorker"), 0, TPri_Normal);
	}
}
