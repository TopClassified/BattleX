#pragma once

#include "CoreMinimal.h"
#include "HAL/Runnable.h"
#include "HAL/RunnableThread.h"
#include "HAL/PlatformProcess.h"
#include "Containers/Queue.h"

#include "BXProjectileStructs.h"

#include <atomic>



// 子弹异步计算驻留线程(生产者-消费者:消费计算Job全桶统一切片分发TaskGraph并聚合结果,GT收割;惰性创建空闲自回收)
class FBXProjectileComputeWorker : public FRunnable
{
public:
	FBXProjectileComputeWorker();

	virtual ~FBXProjectileComputeWorker();

	// 提交计算Job(GameThread专用,入队后确认线程存活并唤醒,自回收竞态窗口内重启线程消费滞留Job)
	bool EnqueueJob(FBXProjectileComputeJob&& InJob);

	// 收割计算结果(队列空返回false,GameThread专用)
	bool DequeueResult(FBXProjectileComputeResult& OutResult);

	// 是否有在途计算(提交序号大于完成序号)
	bool IsBusy() const;

	// 请求停机(置停止标记并唤醒,析构时Join)
	void RequestExit();

private:
	// 线程主循环(消费Job→分发计算→产出Result,空闲超时先标记消亡再终查队列后自回收)
	virtual uint32 Run() override;

	// 停机通知(框架调用)
	virtual void Stop() override;

	// 处理单个Job(全桶统一切片ParallelFor分发,分片候选聚合为异步形态)
	void ProcessJob(FBXProjectileComputeJob& InJob, FBXProjectileComputeResult& OutResult);

	// 确保线程存在(未创建或自回收后重启;须在Job入队后调用,消亡标记先于终查队列构成闭环;GameThread专用)
	void EnsureThread();



	// 计算Job无锁队列(GameThread生产→worker消费)
	TQueue<FBXProjectileComputeJob, EQueueMode::Spsc> JobQueue;

	// 计算结果无锁队列(worker生产→GameThread消费)
	TQueue<FBXProjectileComputeResult, EQueueMode::Spsc> ResultQueue;

	// 唤醒事件(池化)
	FEvent* WakeEvent = nullptr;

	// 线程句柄
	FRunnableThread* Thread = nullptr;

	// 停机标记
	std::atomic<bool> bStopRequested{false};

	// 线程存活标记(空闲自回收置false)
	std::atomic<bool> bThreadAlive{false};

	// 已提交Job序号计数(GameThread递增)
	std::atomic<uint64> SubmittedSerial{0};

	// 已完成Result序号计数(worker递增)
	std::atomic<uint64> CompletedSerial{0};
};
