// BUFF运行状态复制快照。
// UE属性复制时序、运行数据新鲜度问题与选型说明见 Net/BXSkillReplicated.h 文件头注释。
// BUFF快照为COND_InitialOnly(与技能RunningSkillStates对齐):仅新连接初始同步发送,
// 已有连接零属性流量;增删由MulticastAddBuff/MulticastRemoveBuff维护,
// 层/级/到期变化由MulticastBuffStateChanged维护(服务器在Manager收束点广播)。

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

	// 当前层数
	UPROPERTY()
	int32 Layer = 0;

	// 当前等级
	UPROPERTY()
	int32 Level = 0;

	UPROPERTY()
	AActor* Instigator = nullptr;

	// BLL_Shared到期时刻(服务器世界时间域毫秒;投影时按"当前剩余"换算)
	UPROPERTY()
	int64 SharedExpireServerTimestamp = 0;

	// BLL_Independent各层到期时刻(与当前层数一一对应)
	UPROPERTY()
	TArray<int64> LayerExpireServerTimestamps;
};
