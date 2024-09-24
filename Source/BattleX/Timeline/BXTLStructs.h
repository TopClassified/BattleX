#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "StructUtils/Public/InstancedStruct.h"

#include "BXStructs.h"
#include "BXTLEnums.h"

#include "BXTLStructs.generated.h" 



// 时间轴预览动态对象数据
USTRUCT(BlueprintType)
struct FBXTLPreviewObjectData
{
	GENERATED_USTRUCT_BODY()

public:
	// 所对应的任务
	UPROPERTY(Transient, BlueprintReadWrite)
	class UBXTask* Task = nullptr;

	// 无相对偏移坐标
	UPROPERTY(Transient, BlueprintReadWrite)
	FTransform NoOffsetTransform;

};



// 关键帧数据
USTRUCT(BlueprintType)
struct FBXTLKeyFrame
{
	GENERATED_USTRUCT_BODY()

public:
	FBXTLKeyFrame() {}
	FBXTLKeyFrame(float InTime) : Time(InTime) {}

public:
	UPROPERTY(BlueprintReadWrite)
	float Time = 0.0f;

	UPROPERTY(BlueprintReadWrite)
	TArray<int32> Tasks;
};



// 待处理任务信息
USTRUCT(BlueprintType)
struct FBXTLPendingTaskInfo
{
	GENERATED_USTRUCT_BODY()

public:
	FBXTLPendingTaskInfo() {}
	FBXTLPendingTaskInfo(int32 InIndex, float InTime) : LocalIndex(InIndex), Time(InTime){}

public:
	// 任务局部索引
	UPROPERTY(BlueprintReadWrite)
	int32 LocalIndex = -1;

	// 触发时间
	UPROPERTY(BlueprintReadWrite)
	float Time = 0.0f;
};



// 待广播任务信息
USTRUCT(BlueprintType)
struct FBXTLBroadcastTaskInfo
{
	GENERATED_USTRUCT_BODY()

public:
	FBXTLBroadcastTaskInfo() {}
	FBXTLBroadcastTaskInfo(int32 InIndex, const FName& InName) : FullIndex(InIndex), Name(InName) {}

public:
	// 任务全量索引
	UPROPERTY(BlueprintReadWrite)
	int32 FullIndex = -1;

	// 事件名称
	UPROPERTY(BlueprintReadWrite)
	FName Name;
};



// 动态数据集查询索引
USTRUCT(BlueprintType)
struct FBXTLDynamicDataSearchKey
{
	GENERATED_USTRUCT_BODY()

public:
	FBXTLDynamicDataSearchKey() {}
	FBXTLDynamicDataSearchKey(int32 InIndex, const FName& InName) : Index(InIndex), Name(InName) {}

	friend bool operator==(const FBXTLDynamicDataSearchKey& Val1, const FBXTLDynamicDataSearchKey& Val2)
	{
		return Val1.Index == Val2.Index && Val1.Name.IsEqual(Val2.Name);
	}

	friend uint32 GetTypeHash(const FBXTLDynamicDataSearchKey& Val)
	{
		return HashCombine(GetTypeHash(Val.Index), GetTypeHash(Val.Name));
	}

public:
	// 索引
	UPROPERTY(BlueprintReadWrite)
	int32 Index = 0;

	// 名称
	UPROPERTY(BlueprintReadWrite)
	FName Name;
};



// 任务运行数据
USTRUCT(BlueprintType)
struct FBXTLTaskRTData
{
	GENERATED_USTRUCT_BODY()

public:
	FBXTLTaskRTData() {}

	FBXTLTaskRTData(const UScriptStruct* InScriptStruct) : DynamicData(InScriptStruct) {}

	FBXTLTaskRTData(const FBXTLTaskRTData& InOther)
	{
		Index = InOther.Index;
		StartTimestamp = InOther.StartTimestamp;
		RunTime = InOther.RunTime;
		bEarlyFinish = InOther.bEarlyFinish;
		DynamicData = InOther.DynamicData;
	}

	FBXTLTaskRTData(FBXTLTaskRTData&& InOther)
	{
		Index = InOther.Index;
		StartTimestamp = InOther.StartTimestamp;
		RunTime = InOther.RunTime;
		bEarlyFinish = InOther.bEarlyFinish;
		DynamicData = InOther.DynamicData;
	}

	FBXTLTaskRTData& operator=(const FBXTLTaskRTData& InOther)
	{
		if (this != &InOther)
		{
			Index = InOther.Index;
			StartTimestamp = InOther.StartTimestamp;
			RunTime = InOther.RunTime;
			bEarlyFinish = InOther.bEarlyFinish;
			DynamicData = InOther.DynamicData;
		}

		return *this;
	}

	FBXTLTaskRTData& operator=(FBXTLTaskRTData&& InOther)
	{
		if (this != &InOther)
		{
			Index = InOther.Index;
			StartTimestamp = InOther.StartTimestamp;
			RunTime = InOther.RunTime;
			bEarlyFinish = InOther.bEarlyFinish;
			DynamicData = InOther.DynamicData;
		}

		return *this;
	}

	void Reset()
	{
		Index = -1;
		StartTimestamp = 0;
		RunTime = 0.0f;
		bEarlyFinish = true;
		DynamicData.Reset();
	}

public:
	// 静态数据索引
	UPROPERTY(Transient, BlueprintReadOnly)
	int32 Index = -1;

	// 开始时间戳
	UPROPERTY(Transient, BlueprintReadWrite)
	int64 StartTimestamp = 0;

	// 运行时长
	UPROPERTY(Transient, BlueprintReadWrite)
	float RunTime = 0.0f;

	// 是否提前结束
	UPROPERTY(Transient, BlueprintReadWrite)
	bool bEarlyFinish = false;

	// 自定义动态数据
	UPROPERTY(Transient, BlueprintReadWrite)
	FInstancedStruct DynamicData;
};



// 任务托管数据
USTRUCT(BlueprintType)
struct FBXTLTaskHostingData
{
	GENERATED_USTRUCT_BODY()

public:
	FBXTLTaskHostingData() {}

public:
	// 任务静态数据
	UPROPERTY(Transient, BlueprintReadWrite)
	UBXTask* Task = nullptr;

	// 拥有者
	UPROPERTY(Transient, BlueprintReadWrite)
	AActor* Owner = nullptr;

	// 始作俑者
	UPROPERTY(Transient, BlueprintReadWrite)
	AActor* Instigator = nullptr;

	// 触发者
	UPROPERTY(Transient, BlueprintReadWrite)
	AActor* Triggerer = nullptr;

	// 锁定目标列表
	UPROPERTY(Transient, BlueprintReadWrite)
	TArray<FBXBodyPartSelection> LockParts;

	// 运行数据
	UPROPERTY(Transient, BlueprintReadWrite)
	FBXTLTaskRTData TaskRTData;
};



// 时间片段运行数据
USTRUCT(BlueprintType)
struct FBXTLSectionRTData
{
	GENERATED_USTRUCT_BODY()

public:
	FBXTLSectionRTData() {}

	FBXTLSectionRTData(const FBXTLSectionRTData& InOther)
	{
		Index = InOther.Index;
		KeyFrameIndex = InOther.KeyFrameIndex;
		Timestamp = InOther.Timestamp;
		RunTime = InOther.RunTime;
		LoopCount = InOther.LoopCount;
		RunningTasks.Reset();
		RunningTasks.Append(InOther.RunningTasks);

		PendingTasks.Reset();
		PendingTasks.Append(InOther.PendingTasks);

		FramePendingTasks.Reset();
		FramePendingTasks.Append(InOther.FramePendingTasks);

		BroadcastTasks.Reset();
		BroadcastTasks.Append(InOther.BroadcastTasks);
	}

	FBXTLSectionRTData(FBXTLSectionRTData&& InOther)
	{
		Index = InOther.Index;
		KeyFrameIndex = InOther.KeyFrameIndex;
		Timestamp = InOther.Timestamp;
		RunTime = InOther.RunTime;
		LoopCount = InOther.LoopCount;
		RunningTasks.Reset();
		RunningTasks.Append(InOther.RunningTasks);
		InOther.RunningTasks.Reset();

		PendingTasks.Reset();
		PendingTasks.Append(InOther.PendingTasks);
		InOther.PendingTasks.Reset();

		FramePendingTasks.Reset();
		FramePendingTasks.Append(InOther.FramePendingTasks);
		InOther.FramePendingTasks.Reset();

		BroadcastTasks.Reset();
		BroadcastTasks.Append(InOther.BroadcastTasks);
		InOther.BroadcastTasks.Reset();
	}

	FBXTLSectionRTData& operator=(const FBXTLSectionRTData& InOther)
	{
		if (this != &InOther)
		{
			Index = InOther.Index;
			KeyFrameIndex = InOther.KeyFrameIndex;
			Timestamp = InOther.Timestamp;
			RunTime = InOther.RunTime;
			LoopCount = InOther.LoopCount;
			RunningTasks.Reset();
			RunningTasks.Append(InOther.RunningTasks);

			PendingTasks.Reset();
			PendingTasks.Append(InOther.PendingTasks);

			FramePendingTasks.Reset();
			FramePendingTasks.Append(InOther.FramePendingTasks);

			BroadcastTasks.Reset();
			BroadcastTasks.Append(InOther.BroadcastTasks);
		}

		return *this;
	}

	FBXTLSectionRTData& operator=(FBXTLSectionRTData&& InOther)
	{
		if (this != &InOther)
		{
			Index = InOther.Index;
			KeyFrameIndex = InOther.KeyFrameIndex;
			Timestamp = InOther.Timestamp;
			RunTime = InOther.RunTime;
			LoopCount = InOther.LoopCount;
			RunningTasks.Reset();
			RunningTasks.Append(InOther.RunningTasks);
			InOther.RunningTasks.Reset();

			PendingTasks.Reset();
			PendingTasks.Append(InOther.PendingTasks);
			InOther.PendingTasks.Reset();

			FramePendingTasks.Reset();
			FramePendingTasks.Append(InOther.FramePendingTasks);
			InOther.FramePendingTasks.Reset();

			BroadcastTasks.Reset();
			BroadcastTasks.Append(InOther.BroadcastTasks);
			InOther.BroadcastTasks.Reset();
		}

		return *this;
	}

	void Reset()
	{
		Index = -1;
		KeyFrameIndex = 0;
		Timestamp = 0;
		RunTime = 0.0f;
		LoopCount = 0;
		RunningTasks.Empty();
		PendingTasks.Empty();
		FramePendingTasks.Empty();
		BroadcastTasks.Empty();
	}

public:
	// 时间轴索引
	UPROPERTY(Transient, BlueprintReadWrite)
	int32 Index = -1;

	// 当前的关键帧索引
	UPROPERTY(Transient, BlueprintReadWrite)
	int32 KeyFrameIndex = 0;

	// 开始时间戳
	UPROPERTY(Transient, BlueprintReadWrite)
	int64 Timestamp = 0;

	// 运行时长
	UPROPERTY(Transient, BlueprintReadWrite)
	float RunTime = 0.0f;

	// 循环次数
	UPROPERTY(Transient, BlueprintReadWrite)
	int32 LoopCount = 0;

	// 正在运行的Task数据
	UPROPERTY(Transient, BlueprintReadWrite)
	TArray<FBXTLTaskRTData> RunningTasks;

	// 待执行Task列表 Task索引 触发时间点
	UPROPERTY(Transient, BlueprintReadWrite)
	TArray<FBXTLPendingTaskInfo> PendingTasks;

	// 帧内待执行Task索引列表
	UPROPERTY(Transient, BlueprintReadWrite)
	TArray<int32> FramePendingTasks;

	// 需要广播的Task数据 时间轴索引*1000+Task索引 触发时间点
	UPROPERTY(Transient, BlueprintReadWrite)
	TArray<FBXTLBroadcastTaskInfo> BroadcastTasks;
};



// 时间轴运行数据
USTRUCT(BlueprintType)
struct FBXTLRunTimeData
{
	GENERATED_USTRUCT_BODY()

public:
	FBXTLRunTimeData() {}

	FBXTLRunTimeData(const FBXTLRunTimeData& InOther)
	{
		ID = InOther.ID;
		StaticData = InOther.StaticData;
		Owner = InOther.Owner;
		Instigator = InOther.Instigator;
		Triggerer = InOther.Triggerer;
		LockParts.Append(InOther.LockParts);
		Timestamp = InOther.Timestamp;
		RunTime = InOther.RunTime;
		RunRate = InOther.RunRate;
		bEarlyFinish = InOther.bEarlyFinish;
		RunningSections.Reset();
		RunningSections.Append(InOther.RunningSections);

		DynamicDatas.Reset();
		DynamicDatas.Append(InOther.DynamicDatas);
	}

	FBXTLRunTimeData(FBXTLRunTimeData&& InOther)
	{
		ID = InOther.ID;
		StaticData = InOther.StaticData;
		Owner = InOther.Owner;
		Instigator = InOther.Instigator;
		Triggerer = InOther.Triggerer;
		LockParts.Append(InOther.LockParts);
		InOther.LockParts.Reset();
		Timestamp = InOther.Timestamp;
		RunTime = InOther.RunTime;
		RunRate = InOther.RunRate;
		bEarlyFinish = InOther.bEarlyFinish;
		RunningSections.Reset();
		RunningSections.Append(InOther.RunningSections);
		InOther.RunningSections.Reset();

		DynamicDatas.Reset();
		DynamicDatas.Append(InOther.DynamicDatas);
		InOther.DynamicDatas.Reset();
	}

	FBXTLRunTimeData& operator=(const FBXTLRunTimeData& InOther)
	{
		if (this != &InOther)
		{
			ID = InOther.ID;
			StaticData = InOther.StaticData;
			Owner = InOther.Owner;
			Instigator = InOther.Instigator;
			Triggerer = InOther.Triggerer;
			LockParts.Append(InOther.LockParts);
			Timestamp = InOther.Timestamp;
			RunTime = InOther.RunTime;
			RunRate = InOther.RunRate;
			bEarlyFinish = InOther.bEarlyFinish;
			RunningSections.Reset();
			RunningSections.Append(InOther.RunningSections);

			DynamicDatas.Reset();
			DynamicDatas.Append(InOther.DynamicDatas);
		}

		return *this;
	}

	FBXTLRunTimeData& operator=(FBXTLRunTimeData&& InOther)
	{
		if (this != &InOther)
		{
			ID = InOther.ID;
			StaticData = InOther.StaticData;
			Owner = InOther.Owner;
			Instigator = InOther.Instigator;
			Triggerer = InOther.Triggerer;
			LockParts.Append(InOther.LockParts);
			InOther.LockParts.Reset();
			Timestamp = InOther.Timestamp;
			RunTime = InOther.RunTime;
			RunRate = InOther.RunRate;
			bEarlyFinish = InOther.bEarlyFinish;
			RunningSections.Reset();
			RunningSections.Append(InOther.RunningSections);
			InOther.RunningSections.Reset();

			DynamicDatas.Reset();
			DynamicDatas.Append(InOther.DynamicDatas);
			InOther.DynamicDatas.Reset();
		}

		return *this;
	}

	void Reset()
	{
		ID = 0;
		StaticData = nullptr;
		Owner = nullptr;
		Instigator = nullptr;
		Triggerer = nullptr;
		LockParts.Empty();
		Timestamp = 0;
		RunTime = 0.0f;
		RunRate = 1.0f;
		bEarlyFinish = false;
		RunningSections.Empty();
		DynamicDatas.Empty();
	}

public:
	// 唯一ID
	UPROPERTY(Transient, BlueprintReadWrite)
	int64 ID = 0;

	// 静态数据
	UPROPERTY(Transient, BlueprintReadWrite)
	class UBXTLAsset* StaticData = nullptr;

	// 拥有者
	UPROPERTY(Transient, BlueprintReadWrite)
	AActor* Owner = nullptr;

	// 始作俑者
	UPROPERTY(Transient, BlueprintReadWrite)
	AActor* Instigator = nullptr;

	// 触发者
	UPROPERTY(Transient, BlueprintReadWrite)
	AActor* Triggerer = nullptr;

	// 锁定目标列表
	UPROPERTY(Transient, BlueprintReadWrite)
	TArray<FBXBodyPartSelection> LockParts;

	// 开始时间戳
	UPROPERTY(Transient, BlueprintReadWrite)
	int64 Timestamp = 0;

	// 运行时长
	UPROPERTY(Transient, BlueprintReadWrite)
	float RunTime = 0.0f;

	// 运行速率
	UPROPERTY(Transient, BlueprintReadWrite)
	float RunRate = 1.0f;

	// 是否提前结束
	UPROPERTY(Transient, BlueprintReadWrite)
	bool bEarlyFinish = false;

	// 片段运行信息
	UPROPERTY(Transient, BlueprintReadWrite)
	TArray<FBXTLSectionRTData> RunningSections;

	// 动态数据集
	UPROPERTY(Transient, BlueprintReadWrite)
	TMap<FBXTLDynamicDataSearchKey, FInstancedStruct> DynamicDatas;

#if WITH_EDITORONLY_DATA
	// 预览对象到任务全量索引的映射表
	UPROPERTY(Transient)
	TMap<UObject*, FBXTLPreviewObjectData> PreviewObjects;
#endif

};



// 时间轴运行上下文
USTRUCT(BlueprintType)
struct FBXTLPlayContext
{
	GENERATED_USTRUCT_BODY()

public:
	FBXTLPlayContext() {}

	FBXTLPlayContext(const FBXTLPlayContext& InOther)
	{
		Instigator = InOther.Instigator;
		Triggerer = InOther.Triggerer;
		LockParts.Append(InOther.LockParts);
		Timestamp = InOther.Timestamp;
		InputDatas.Append(InOther.InputDatas);
	}

	FBXTLPlayContext(FBXTLPlayContext&& InOther)
	{
		Instigator = InOther.Instigator;
		Triggerer = InOther.Triggerer;
		LockParts.Append(InOther.LockParts);
		InOther.LockParts.Reset();
		Timestamp = InOther.Timestamp;
		InputDatas.Append(InOther.InputDatas);
		InOther.InputDatas.Reset();
	}

	FBXTLPlayContext& operator=(const FBXTLPlayContext& InOther)
	{
		if (this != &InOther)
		{
			Instigator = InOther.Instigator;
			Triggerer = InOther.Triggerer;
			LockParts.Append(InOther.LockParts);
			Timestamp = InOther.Timestamp;
			InputDatas.Append(InOther.InputDatas);
		}

		return *this;
	}

	FBXTLPlayContext& operator=(FBXTLPlayContext&& InOther)
	{
		if (this != &InOther)
		{
			Instigator = InOther.Instigator;
			Triggerer = InOther.Triggerer;
			LockParts.Append(InOther.LockParts);
			InOther.LockParts.Reset();
			Timestamp = InOther.Timestamp;
			InputDatas.Append(InOther.InputDatas);
			InOther.InputDatas.Reset();
		}

		return *this;
	}

	void Reset()
	{
		Instigator = nullptr;
		Triggerer = nullptr;
		LockParts.Empty();
		Timestamp = 0;
		InputDatas.Empty();
	}

public:
	// 始作俑者
	UPROPERTY(BlueprintReadWrite)
	AActor* Instigator = nullptr;

	// 触发者
	UPROPERTY(BlueprintReadWrite)
	AActor* Triggerer = nullptr;

	// 锁定目标列表
	UPROPERTY(BlueprintReadWrite)
	TArray<FBXBodyPartSelection> LockParts;

	// 开始时间戳
	UPROPERTY(BlueprintReadWrite)
	int64 Timestamp = 0;

	// 动态数据集
	UPROPERTY(BlueprintReadWrite)
	TMap<FName, FInstancedStruct> InputDatas;

};
