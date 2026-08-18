#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "StructUtils/Public/InstancedStruct.h"

#include "BXStructs.h"
#include "BXTLStructs.h"
#include "BXSkillEnums.h"
#include "BXNetEnums.h"

#include "BXSkillStructs.generated.h"



// 技能运行时数据(内嵌Timeline运行时数据以复用Task执行链路)
USTRUCT(BlueprintType)
struct FBXSkillRuntimeData
{
	GENERATED_USTRUCT_BODY()

public:
	// 技能资产
	UPROPERTY(Transient, BlueprintReadWrite)
	class UBXSkillAsset* SkillAsset = nullptr;

	// 唯一ID(防ABA,客户端或服务器生成)
	UPROPERTY(Transient, BlueprintReadWrite)
	int64 SkillID = 0;

	// Timeline运行时数据(复用Task执行链路)
	UPROPERTY(Transient, BlueprintReadWrite)
	FBXTLRunTimeData TLRunTimeData;

	// 预测状态
	UPROPERTY(Transient, BlueprintReadWrite)
	EBXPredictState PredictState = EBXPredictState::None;

	// 预测起始时间(用于超时清理)
	UPROPERTY(Transient)
	float PredictStartTime = 0.0f;

	// 客户端上传时间戳(毫秒)
	UPROPERTY(Transient, BlueprintReadWrite)
	int64 ClientTimestamp = 0;

	// 服务器接收时间戳(毫秒,仅服务器填写)
	UPROPERTY(Transient, BlueprintReadWrite)
	int64 ServerTimestamp = 0;

	// 发起端类型
	UPROPERTY(Transient, BlueprintReadWrite)
	EBXSyncInitiator Initiator = EBXSyncInitiator::Client;

	// 加速剩余时间(服务器/客户端加速弥补用,<=0代表未加速)
	UPROPERTY(Transient)
	float AccelerateRemainTime = 0.0f;

	// 加速倍率(恢复时用)
	UPROPERTY(Transient)
	float AccelerateRate = 1.0f;

	// 是否提前结束
	UPROPERTY(Transient, BlueprintReadWrite)
	bool bEarlyFinish = false;

	// 结束原因(bEarlyFinish置位时记录,BXEvent.Skill.Finished事件携带)
	UPROPERTY(Transient)
	EBXTLFinishReason FinishReason = EBXTLFinishReason::FR_TMax;
};
