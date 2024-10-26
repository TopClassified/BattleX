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
	FBXTLPendingTaskInfo(int32 InIndex, int32 InParentScope, float InTime) : LocalIndex(InIndex), ParentScope(InParentScope), Time(InTime){}

public:
	// 任务局部索引
	UPROPERTY(Transient, BlueprintReadWrite)
	int32 LocalIndex = -1;

	// 父作用域
	UPROPERTY(Transient, BlueprintReadWrite)
	int32 ParentScope = 0;
	
	// 触发时间
	UPROPERTY(Transient, BlueprintReadWrite)
	float Time = 0.0f;
};



// 待广播任务信息
USTRUCT(BlueprintType)
struct FBXTLBroadcastTaskInfo
{
	GENERATED_USTRUCT_BODY()

public:
	FBXTLBroadcastTaskInfo() {}
	FBXTLBroadcastTaskInfo(int32 InIndex, const FGameplayTag& InTag) : FullIndex(InIndex), Tag(InTag) {}

public:
	// 任务全量索引
	UPROPERTY(Transient, BlueprintReadWrite)
	int32 FullIndex = -1;

	// 事件名称
	UPROPERTY(Transient, BlueprintReadWrite)
	FGameplayTag Tag;
};



// 动态数据集查询索引
USTRUCT(BlueprintType)
struct FBXTLDynamicDataSearchKey
{
	GENERATED_USTRUCT_BODY()

public:
	FBXTLDynamicDataSearchKey() {}
	FBXTLDynamicDataSearchKey(int32 InIndex, const FGameplayTag& InTag) : Index(InIndex), Tag(InTag) {}
	FBXTLDynamicDataSearchKey(int32 InIndex, const FGameplayTag& InTag, int64 InScope) : Index(InIndex), Tag(InTag), Scope(InScope) {}

	friend bool operator==(const FBXTLDynamicDataSearchKey& Val1, const FBXTLDynamicDataSearchKey& Val2)
	{
		return Val1.Index == Val2.Index && Val1.Tag == Val2.Tag && Val1.Scope == Val2.Scope;
	}

	friend uint32 GetTypeHash(const FBXTLDynamicDataSearchKey& Val)
	{
		return HashCombine(GetTypeHash(Val.Index), GetTypeHash(Val.Tag));
	}

public:
	// 索引
	UPROPERTY(Transient, BlueprintReadWrite)
	int32 Index = 0;

	// 名称
	UPROPERTY(Transient, BlueprintReadWrite)
	FGameplayTag Tag;
	
	// 任务作用域
	UPROPERTY(Transient, BlueprintReadWrite)
	int64 Scope = 0;
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
		Task = InOther.Task;
		Index = InOther.Index;
		ParentScope = InOther.ParentScope;
		RunTime = InOther.RunTime;
		bEarlyFinish = InOther.bEarlyFinish;
		DynamicData = InOther.DynamicData;
	}

	FBXTLTaskRTData(FBXTLTaskRTData&& InOther)
	{
		Task = InOther.Task;
		Index = InOther.Index;
		ParentScope = InOther.ParentScope;
		RunTime = InOther.RunTime;
		bEarlyFinish = InOther.bEarlyFinish;
		DynamicData = InOther.DynamicData;
	}

	FBXTLTaskRTData& operator=(const FBXTLTaskRTData& InOther)
	{
		if (this != &InOther)
		{
			Task = InOther.Task;
			Index = InOther.Index;
			ParentScope = InOther.ParentScope;
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
			Task = InOther.Task;
			Index = InOther.Index;
			ParentScope = InOther.ParentScope;
			RunTime = InOther.RunTime;
			bEarlyFinish = InOther.bEarlyFinish;
			DynamicData = InOther.DynamicData;
		}

		return *this;
	}

	void Reset()
	{
		Task = nullptr;
		Index = -1;
		ParentScope = 0;
		RunTime = 0.0f;
		bEarlyFinish = true;
		DynamicData.Reset();
	}

public:
	// 静态数据
	UPROPERTY(Transient, BlueprintReadOnly)
	UBXTask* Task = nullptr;
	
	// 静态数据索引
	UPROPERTY(Transient, BlueprintReadOnly)
	int32 Index = -1;

	// 父作用域
	UPROPERTY(Transient, BlueprintReadOnly)
	int64 ParentScope = 0;
	
	// 运行时长
	UPROPERTY(Transient, BlueprintReadWrite)
	float RunTime = 0.0f;

	// 下次更新间隔（-1代表无穷大）
	UPROPERTY(Transient, BlueprintReadWrite)
	float NextTick = 0.0f;
	
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
		RunTime = InOther.RunTime;
		LoopCount = InOther.LoopCount;
		RunningTasks.Reset();
		RunningTasks.Append(InOther.RunningTasks);

		PendingTasks.Reset();
		PendingTasks.Append(InOther.PendingTasks);

		BroadcastTasks.Reset();
		BroadcastTasks.Append(InOther.BroadcastTasks);
	}

	FBXTLSectionRTData(FBXTLSectionRTData&& InOther)
	{
		Index = InOther.Index;
		KeyFrameIndex = InOther.KeyFrameIndex;
		RunTime = InOther.RunTime;
		LoopCount = InOther.LoopCount;
		RunningTasks.Reset();
		RunningTasks.Append(InOther.RunningTasks);
		InOther.RunningTasks.Reset();

		PendingTasks.Reset();
		PendingTasks.Append(InOther.PendingTasks);
		InOther.PendingTasks.Reset();

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
			RunTime = InOther.RunTime;
			LoopCount = InOther.LoopCount;
			RunningTasks.Reset();
			RunningTasks.Append(InOther.RunningTasks);

			PendingTasks.Reset();
			PendingTasks.Append(InOther.PendingTasks);

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
			RunTime = InOther.RunTime;
			LoopCount = InOther.LoopCount;
			RunningTasks.Reset();
			RunningTasks.Append(InOther.RunningTasks);
			InOther.RunningTasks.Reset();

			PendingTasks.Reset();
			PendingTasks.Append(InOther.PendingTasks);
			InOther.PendingTasks.Reset();

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
		RunTime = 0.0f;
		LoopCount = 0;
		RunningTasks.Empty();
		PendingTasks.Empty();
		BroadcastTasks.Empty();
	}

public:
	// 时间轴索引
	UPROPERTY(Transient, BlueprintReadWrite)
	int32 Index = -1;

	// 当前的关键帧索引
	UPROPERTY(Transient, BlueprintReadWrite)
	int32 KeyFrameIndex = 0;

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

	// 需要广播的Task数据 时间轴索引*1000+Task索引 触发时间点
	UPROPERTY(Transient, BlueprintReadWrite)
	TArray<FBXTLBroadcastTaskInfo> BroadcastTasks;

	// 帧内任务触发栈
	TArray<FInt64Vector2> TaskStackInFrame;
	
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
		Timeline = InOther.Timeline;
		TimelineID = InOther.TimelineID;
		ID = InOther.ID;
		Owner = InOther.Owner;
		Instigator = InOther.Instigator;
		Triggerer = InOther.Triggerer;
		LockParts.Append(InOther.LockParts);
		RunTime = InOther.RunTime;
		RunRate = InOther.RunRate;
		bEarlyFinish = InOther.bEarlyFinish;
		RunningSections.Reset();
		RunningSections.Append(InOther.RunningSections);

		DynamicDatas.Reset();
		DynamicDatas.Append(InOther.DynamicDatas);
		
		ScopeGraph.Reset();
		ScopeGraph.Append(InOther.ScopeGraph);
	}

	FBXTLRunTimeData(FBXTLRunTimeData&& InOther)
	{
		Timeline = InOther.Timeline;
		TimelineID = InOther.TimelineID;
		ID = InOther.ID;
		Owner = InOther.Owner;
		Instigator = InOther.Instigator;
		Triggerer = InOther.Triggerer;
		LockParts.Append(InOther.LockParts);
		InOther.LockParts.Reset();
		RunTime = InOther.RunTime;
		RunRate = InOther.RunRate;
		bEarlyFinish = InOther.bEarlyFinish;
		RunningSections.Reset();
		RunningSections.Append(InOther.RunningSections);
		InOther.RunningSections.Reset();

		DynamicDatas.Reset();
		DynamicDatas.Append(InOther.DynamicDatas);
		InOther.DynamicDatas.Reset();
		
		ScopeGraph.Reset();
		ScopeGraph.Append(InOther.ScopeGraph);
		InOther.ScopeGraph.Reset();
	}

	FBXTLRunTimeData& operator=(const FBXTLRunTimeData& InOther)
	{
		if (this != &InOther)
		{
			Timeline = InOther.Timeline;
			TimelineID = InOther.TimelineID;
			ID = InOther.ID;
			Owner = InOther.Owner;
			Instigator = InOther.Instigator;
			Triggerer = InOther.Triggerer;
			LockParts.Append(InOther.LockParts);
			RunTime = InOther.RunTime;
			RunRate = InOther.RunRate;
			bEarlyFinish = InOther.bEarlyFinish;
			RunningSections.Reset();
			RunningSections.Append(InOther.RunningSections);

			DynamicDatas.Reset();
			DynamicDatas.Append(InOther.DynamicDatas);
			
			ScopeGraph.Reset();
			ScopeGraph.Append(InOther.ScopeGraph);
		}

		return *this;
	}

	FBXTLRunTimeData& operator=(FBXTLRunTimeData&& InOther)
	{
		if (this != &InOther)
		{
			Timeline = InOther.Timeline;
			TimelineID = InOther.TimelineID;
			ID = InOther.ID;
			Owner = InOther.Owner;
			Instigator = InOther.Instigator;
			Triggerer = InOther.Triggerer;
			LockParts.Append(InOther.LockParts);
			InOther.LockParts.Reset();
			RunTime = InOther.RunTime;
			RunRate = InOther.RunRate;
			bEarlyFinish = InOther.bEarlyFinish;
			RunningSections.Reset();
			RunningSections.Append(InOther.RunningSections);
			InOther.RunningSections.Reset();

			DynamicDatas.Reset();
			DynamicDatas.Append(InOther.DynamicDatas);
			InOther.DynamicDatas.Reset();

			ScopeGraph.Reset();
			ScopeGraph.Append(InOther.ScopeGraph);
			InOther.ScopeGraph.Reset();
		}

		return *this;
	}

	void Reset()
	{
		Timeline = nullptr;
		TimelineID = 0;
		ID = 0;
		Owner = nullptr;
		Instigator = nullptr;
		Triggerer = nullptr;
		LockParts.Empty();
		RunTime = 0.0f;
		RunRate = 1.0f;
		bEarlyFinish = false;
		RunningSections.Empty();
		DynamicDatas.Empty();
		ScopeGraph.Empty();
	}

public:
	// 静态数据
	UPROPERTY(Transient, BlueprintReadWrite)
	class UBXTLAsset* Timeline = nullptr;

	// 静态数据ID
	UPROPERTY(Transient, BlueprintReadWrite)
	int64 TimelineID = 0;
	
	// 唯一ID
	UPROPERTY(Transient, BlueprintReadWrite)
	int64 ID = 0;

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

	// 作用域图 Key:作用域ID  Value:X:父作用域ID Y:Task的全量索引
	TMap<int64, FInt64Vector2> ScopeGraph;
	
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
		InputDatas.Append(InOther.InputDatas);
	}

	FBXTLPlayContext(FBXTLPlayContext&& InOther)
	{
		Instigator = InOther.Instigator;
		Triggerer = InOther.Triggerer;
		LockParts.Append(InOther.LockParts);
		InOther.LockParts.Reset();
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

	// 动态数据集
	UPROPERTY(BlueprintReadWrite)
	TMap<FGameplayTag, FInstancedStruct> InputDatas;

};
