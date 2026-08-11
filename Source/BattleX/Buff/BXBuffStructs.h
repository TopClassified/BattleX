#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "GameplayTagContainer.h"
#include "InstancedStruct.h"

#include "BXBuffEnums.h"
#include "BXTLStructs.h"

#include "BXBuffStructs.generated.h"



class UBXBuffAsset;
class UBXTask;



// BUFF运行时数据(纯数据结构,内嵌Timeline运行时数据以复用Task执行链路)
USTRUCT(BlueprintType)
struct FBXBuffRuntimeData
{
	GENERATED_USTRUCT_BODY()

	// 静态配置
	UPROPERTY(Transient, BlueprintReadWrite)
	UBXBuffAsset* BuffAsset = nullptr;

	// 唯一ID(防ABA)
	UPROPERTY(Transient, BlueprintReadWrite)
	int64 BuffID = 0;

	// Timeline运行时数据(直接复用,Task执行链路所需)
	UPROPERTY(Transient, BlueprintReadWrite)
	FBXTLRunTimeData TLRunTimeData;

	// 当前层数
	UPROPERTY(Transient, BlueprintReadWrite)
	int32 CurrentLayer = 1;

	// 当前等级
	UPROPERTY(Transient, BlueprintReadWrite)
	int32 CurrentLevel = 1;

	// 共享生命周期已运行时间
	UPROPERTY(Transient, BlueprintReadWrite)
	float RunTime = 0.0f;

	// 独立层级模式每层已运行时间
	UPROPERTY(Transient)
	TArray<float> LayerRunTimes;

	// 是否提前结束
	UPROPERTY(Transient, BlueprintReadWrite)
	bool bEarlyFinish = false;
};



// BUFF施放上下文
USTRUCT(BlueprintType)
struct FBXBuffPlayContext
{
	GENERATED_USTRUCT_BODY()

	// 始作俑者
	UPROPERTY(BlueprintReadWrite)
	AActor* Instigator = nullptr;

	// 触发者
	UPROPERTY(BlueprintReadWrite)
	AActor* Triggerer = nullptr;

	// 初始层数
	UPROPERTY(BlueprintReadWrite)
	int32 InitLayer = 1;

	// 初始等级
	UPROPERTY(BlueprintReadWrite)
	int32 InitLevel = 1;

	// 输入数据
	UPROPERTY(BlueprintReadWrite)
	TMap<FGameplayTag, FInstancedStruct> InputDatas;
};



// Task层级绑定配置
USTRUCT(BlueprintType)
struct FBXBuffTaskBinding
{
	GENERATED_USTRUCT_BODY()

	// Task引用(关联到Sections[0].TaskList中的Task)
	UPROPERTY(EditAnywhere, Instanced)
	UBXTask* Task = nullptr;

	// 层级执行区间下限,0代表不限制
	UPROPERTY(EditAnywhere, meta = (ClampMin = 0))
	int32 MinLayer = 0;

	// 层级执行区间上限,0代表不限制
	UPROPERTY(EditAnywhere, meta = (ClampMin = 0))
	int32 MaxLayer = 0;
};



// BUFF事件参数
USTRUCT(BlueprintType)
struct FBXEventBuffChanged
{
	GENERATED_USTRUCT_BODY()

	// BUFF实例ID
	UPROPERTY(BlueprintReadWrite)
	int64 BuffInstanceID = 0;

	// BUFF资产
	UPROPERTY(BlueprintReadWrite)
	UBXBuffAsset* BuffAsset = nullptr;

	// 拥有者
	UPROPERTY(BlueprintReadWrite)
	AActor* Owner = nullptr;

	// 始作俑者
	UPROPERTY(BlueprintReadWrite)
	AActor* Instigator = nullptr;

	// 触发者
	UPROPERTY(BlueprintReadWrite)
	AActor* Triggerer = nullptr;

	// 旧层数
	UPROPERTY(BlueprintReadWrite)
	int32 OldLayer = 0;

	// 新层数
	UPROPERTY(BlueprintReadWrite)
	int32 NewLayer = 0;

	// 旧等级
	UPROPERTY(BlueprintReadWrite)
	int32 OldLevel = 0;

	// 新等级
	UPROPERTY(BlueprintReadWrite)
	int32 NewLevel = 0;

	// 移除原因
	UPROPERTY(BlueprintReadWrite)
	uint8 RemoveReason = 0;
};
