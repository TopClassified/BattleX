// BUFF运行状态复制快照。
// UE属性复制时序、运行数据新鲜度问题与选型说明见 Net/BXSkillReplicated.h 文件头注释。
// BUFF快照为无条件复制(非COND_InitialOnly):低频变化(增删/层级/时长刷新)承担
// 初始重建 + 已重建客户端的层/级/到期持续同步,与Unreliable的MulticastBuffLayerChanged互为兜底。

#pragma once

#include "CoreMinimal.h"

#include "BXBuffReplicated.generated.h"



// BUFF复制条目:运行状态投影(层/级/计时,本质即BUFF运行数据的可复制形态)
USTRUCT()
struct FBXBuffReplicatedState
{
	GENERATED_USTRUCT_BODY()

public:
	// BUFF实例ID(与BuffRTDatas键一致)
	UPROPERTY()
	int64 BuffID = 0;

	// 资产ID(GetTimelineAssetByID恢复)
	UPROPERTY()
	int32 BuffAssetID = 0;

	// 当前层数(重建+已重建客户端持续同步)
	UPROPERTY()
	int32 Layer = 0;

	// 当前等级(持续同步)
	UPROPERTY()
	int32 Level = 0;

	UPROPERTY()
	AActor* Instigator = nullptr;

	// BLL_Shared到期时刻(服务器世界时间域毫秒;Add/刷新/层变化时写,Tick不更新)
	UPROPERTY()
	int64 SharedExpireServerTimestamp = 0;

	// BLL_Independent各层到期时刻(与当前层数一一对应)
	UPROPERTY()
	TArray<int64> LayerExpireServerTimestamps;
};
