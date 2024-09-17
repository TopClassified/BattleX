#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "StructUtils/Public/InstancedStruct.h"

#include "BXStructs.h"
#include "BXTLEnums.h"

#include "BXTLStructs.generated.h"



// ʱ����Ԥ����̬��������
USTRUCT(BlueprintType)
struct FBXTLPreviewObjectData
{
	GENERATED_USTRUCT_BODY()

public:
	// ����Ӧ������
	UPROPERTY(Transient, BlueprintReadWrite)
	class UBXTask* Task = nullptr;

	// �����ƫ������
	UPROPERTY(Transient, BlueprintReadWrite)
	FTransform NoOffsetTransform;

};



// �ؼ�֡����
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



// ������������Ϣ
USTRUCT(BlueprintType)
struct FBXTLPendingTaskInfo
{
	GENERATED_USTRUCT_BODY()

public:
	FBXTLPendingTaskInfo() {}
	FBXTLPendingTaskInfo(int32 InIndex, float InTime) : LocalIndex(InIndex), Time(InTime){}

public:
	// ����ֲ�����
	UPROPERTY(BlueprintReadWrite)
	int32 LocalIndex = -1;

	// ����ʱ��
	UPROPERTY(BlueprintReadWrite)
	float Time = 0.0f;
};



// ���㲥������Ϣ
USTRUCT(BlueprintType)
struct FBXTLBroadcastTaskInfo
{
	GENERATED_USTRUCT_BODY()

public:
	FBXTLBroadcastTaskInfo() {}
	FBXTLBroadcastTaskInfo(int32 InIndex, const FName& InName) : FullIndex(InIndex), Name(InName) {}

public:
	// ����ȫ������
	UPROPERTY(BlueprintReadWrite)
	int32 FullIndex = -1;

	// �¼�����
	UPROPERTY(BlueprintReadWrite)
	FName Name;
};



// ��̬���ݼ���ѯ����
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
	// ����
	UPROPERTY(BlueprintReadWrite)
	int32 Index = 0;

	// ����
	UPROPERTY(BlueprintReadWrite)
	FName Name;
};



// ������������
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
	// ��̬��������
	UPROPERTY(Transient, BlueprintReadOnly)
	int32 Index = -1;

	// ��ʼʱ���
	UPROPERTY(Transient, BlueprintReadWrite)
	int64 StartTimestamp = 0;

	// ����ʱ��
	UPROPERTY(Transient, BlueprintReadWrite)
	float RunTime = 0.0f;

	// �Ƿ���ǰ����
	UPROPERTY(Transient, BlueprintReadWrite)
	bool bEarlyFinish = false;

	// �Զ��嶯̬����
	UPROPERTY(Transient, BlueprintReadWrite)
	FInstancedStruct DynamicData;
};



// �����й�����
USTRUCT(BlueprintType)
struct FBXTLTaskHostingData
{
	GENERATED_USTRUCT_BODY()

public:
	FBXTLTaskHostingData() {}

public:
	// ����̬����
	UPROPERTY(Transient, BlueprintReadWrite)
	UBXTask* Task = nullptr;

	// ӵ����
	UPROPERTY(Transient, BlueprintReadWrite)
	AActor* Owner = nullptr;

	// ʼ��ٸ��
	UPROPERTY(Transient, BlueprintReadWrite)
	AActor* Instigator = nullptr;

	// ������
	UPROPERTY(Transient, BlueprintReadWrite)
	AActor* Triggerer = nullptr;

	// ����Ŀ���б�
	UPROPERTY(Transient, BlueprintReadWrite)
	TArray<FBXBodyPartSelection> LockParts;

	// ��������
	UPROPERTY(Transient, BlueprintReadWrite)
	FBXTLTaskRTData TaskRTData;
};



// ʱ��Ƭ����������
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
	// ʱ��������
	UPROPERTY(Transient, BlueprintReadWrite)
	int32 Index = -1;

	// ��ǰ�Ĺؼ�֡����
	UPROPERTY(Transient, BlueprintReadWrite)
	int32 KeyFrameIndex = 0;

	// ��ʼʱ���
	UPROPERTY(Transient, BlueprintReadWrite)
	int64 Timestamp = 0;

	// ����ʱ��
	UPROPERTY(Transient, BlueprintReadWrite)
	float RunTime = 0.0f;

	// ѭ������
	UPROPERTY(Transient, BlueprintReadWrite)
	int32 LoopCount = 0;

	// �������е�Task����
	UPROPERTY(Transient, BlueprintReadWrite)
	TArray<FBXTLTaskRTData> RunningTasks;

	// ��ִ��Task�б� Task���� ����ʱ���
	UPROPERTY(Transient, BlueprintReadWrite)
	TArray<FBXTLPendingTaskInfo> PendingTasks;

	// ֡�ڴ�ִ��Task�����б�
	UPROPERTY(Transient, BlueprintReadWrite)
	TArray<int32> FramePendingTasks;

	// ��Ҫ�㲥��Task���� ʱ��������*1000+Task���� ����ʱ���
	UPROPERTY(Transient, BlueprintReadWrite)
	TArray<FBXTLBroadcastTaskInfo> BroadcastTasks;
};



// ʱ������������
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
	// ΨһID
	UPROPERTY(Transient, BlueprintReadWrite)
	int64 ID = 0;

	// ��̬����
	UPROPERTY(Transient, BlueprintReadWrite)
	class UBXTLAsset* StaticData = nullptr;

	// ӵ����
	UPROPERTY(Transient, BlueprintReadWrite)
	AActor* Owner = nullptr;

	// ʼ��ٸ��
	UPROPERTY(Transient, BlueprintReadWrite)
	AActor* Instigator = nullptr;

	// ������
	UPROPERTY(Transient, BlueprintReadWrite)
	AActor* Triggerer = nullptr;

	// ����Ŀ���б�
	UPROPERTY(Transient, BlueprintReadWrite)
	TArray<FBXBodyPartSelection> LockParts;

	// ��ʼʱ���
	UPROPERTY(Transient, BlueprintReadWrite)
	int64 Timestamp = 0;

	// ����ʱ��
	UPROPERTY(Transient, BlueprintReadWrite)
	float RunTime = 0.0f;

	// ��������
	UPROPERTY(Transient, BlueprintReadWrite)
	float RunRate = 1.0f;

	// �Ƿ���ǰ����
	UPROPERTY(Transient, BlueprintReadWrite)
	bool bEarlyFinish = false;

	// Ƭ��������Ϣ
	UPROPERTY(Transient, BlueprintReadWrite)
	TArray<FBXTLSectionRTData> RunningSections;

	// ��̬���ݼ�
	UPROPERTY(Transient, BlueprintReadWrite)
	TMap<FBXTLDynamicDataSearchKey, FInstancedStruct> DynamicDatas;

#if WITH_EDITORONLY_DATA
	// Ԥ����������ȫ��������ӳ���
	UPROPERTY(Transient)
	TMap<UObject*, FBXTLPreviewObjectData> PreviewObjects;
#endif

};



// ʱ��������������
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
	// ʼ��ٸ��
	UPROPERTY(BlueprintReadWrite)
	AActor* Instigator = nullptr;

	// ������
	UPROPERTY(BlueprintReadWrite)
	AActor* Triggerer = nullptr;

	// ����Ŀ���б�
	UPROPERTY(BlueprintReadWrite)
	TArray<FBXBodyPartSelection> LockParts;

	// ��ʼʱ���
	UPROPERTY(BlueprintReadWrite)
	int64 Timestamp = 0;

	// ��̬���ݼ�
	UPROPERTY(BlueprintReadWrite)
	TMap<FName, FInstancedStruct> InputDatas;

};
