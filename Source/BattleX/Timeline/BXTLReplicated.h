#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "StructUtils/Public/InstancedStruct.h"

#include "BXStructs.h"
#include "BXTLStructs.h"

#include "BXTLReplicated.generated.h"



// FBXTLTaskRTData的可复制投影(Task指针→Index定位,服务器内部状态不投影)
USTRUCT()
struct FBXTLTaskRTDataReplicated
{
	GENERATED_USTRUCT_BODY()

public:
	// Task在Section.TaskList中的索引(反投影恢复Task指针)
	UPROPERTY()
	int32 Index = -1;

	// 父作用域
	UPROPERTY()
	int64 ParentScope = 0;

	// 运行时间
	UPROPERTY()
	float RunTime = 0.0f;

	// 下次执行倒计时
	UPROPERTY()
	float NextTick = 0.0f;

	// 是否提前结束
	UPROPERTY()
	bool bEarlyFinish = false;

	// 任务自定义上下文(FInstancedStruct按结构路径序列化)
	UPROPERTY()
	FInstancedStruct DynamicData;
};



// FBXTLSectionRTData的可复制投影(内部队列PendingTasks/BroadcastTasks不投影,重建端自行产生)
USTRUCT()
struct FBXTLSectionRTDataReplicated
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY()
	int32 Index = -1;

	UPROPERTY()
	int32 KeyFrameIndex = 0;

	UPROPERTY()
	float RunTime = 0.0f;

	UPROPERTY()
	int32 LoopCount = 0;

	UPROPERTY()
	int32 ForceJumpSection = -1;

	UPROPERTY()
	bool bEarlyFinish = false;

	UPROPERTY()
	TArray<FBXTLTaskRTDataReplicated> RunningTasks;
};



// FBXTLRunTimeData的可复制投影(Timeline指针→TimelineID;ScopeGraph/TaskStackInFrame含无反射类型,不投影)
USTRUCT()
struct FBXTLRunTimeDataReplicated
{
	GENERATED_USTRUCT_BODY()

public:
	// 资产ID(Timeline指针→GetTimelineAssetByID恢复)
	UPROPERTY()
	int32 TimelineID = 0;

	// 实例ID
	UPROPERTY()
	int64 ID = 0;

	// Actor引用直接复制(NetGUID)
	UPROPERTY()
	AActor* Owner = nullptr;

	UPROPERTY()
	AActor* Instigator = nullptr;

	UPROPERTY()
	AActor* Triggerer = nullptr;

	UPROPERTY()
	TArray<FBXBodyPartSelection> LockParts;

	UPROPERTY()
	float RunTime = 0.0f;

	UPROPERTY()
	float RunRate = 1.0f;

	UPROPERTY()
	TArray<FBXTLSectionRTDataReplicated> RunningSections;

	// 动态数据集(含运行期写入的数据,比播放时刻的InputDatas更完整)
	UPROPERTY()
	TMap<FBXTLDynamicDataSearchKey, FInstancedStruct> DynamicDatas;
};



#pragma region Projection
// 运行数据→可复制投影(服务器端调用)
// bSimulatedOnly=true时按Task.NetTypes过滤,仅保留含模拟端位的Task(重建端为SimulatedProxy,
// 权威/自主端Task不在远端执行,投影丢弃可避免其上下文中的不可复制指针,如碰撞盒组件引用)
BATTLEX_API void BXToTLRunTimeProjection(const FBXTLRunTimeData& InSource, FBXTLRunTimeDataReplicated& OutTarget, bool bSimulatedOnly = true);

// 可复制投影→运行数据(客户端重建调用,按(资产,Index)恢复Task指针;资产未加载或索引失效的条目跳过)
BATTLEX_API void BXFromTLRunTimeProjection(const FBXTLRunTimeDataReplicated& InSource, UBXTLAsset* InAsset, FBXTLRunTimeData& OutTarget);

#pragma endregion Projection
