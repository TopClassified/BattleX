#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "StructUtils/Public/InstancedStruct.h"

#include "BXStructs.h"
#include "BXNetEnums.h"

#include "BXNetStructs.generated.h"



// 技能同步头(所有RPC共用)
USTRUCT(BlueprintType)
struct FBXSkillSyncHeader
{
	GENERATED_USTRUCT_BODY()

public:
	// 技能运行时唯一ID(客户端或服务器生成,跨端一致)
	UPROPERTY(BlueprintReadWrite)
	int64 SkillID = 0;

	// 技能资源ID(UBXTLAsset.ID,跨端通用)
	UPROPERTY(BlueprintReadWrite)
	int32 SkillAssetID = 0;

	// 发起端类型
	UPROPERTY(BlueprintReadWrite)
	EBXSyncInitiator Initiator = EBXSyncInitiator::Client;

	// 客户端上传时间戳(毫秒,服务器世界时间域:服务器/单机为本地世界时间,客户端经引擎时间同步校正)
	// 统一通过UBXFunctionLibrary::GetServerWorldTimeMilliseconds获取,禁止跨机器直接比较本地墙钟
	UPROPERTY(BlueprintReadWrite)
	int64 ClientTimestamp = 0;

	// 服务器接收时间戳(毫秒,服务器世界时间域,仅服务器填写后回传)
	UPROPERTY(BlueprintReadWrite)
	int64 ServerTimestamp = 0;
};



// 锁定部位列表输入(FInstancedStruct不可直接承载TArray,需USTRUCT包装)
USTRUCT(BlueprintType)
struct FBXSkillLockParts
{
	GENERATED_USTRUCT_BODY()

public:
	FBXSkillLockParts() {}
	FBXSkillLockParts(const TArray<FBXBodyPartSelection>& InParts) : Parts(InParts) {}

	UPROPERTY(BlueprintReadWrite)
	TArray<FBXBodyPartSelection> Parts;
};



// 技能输入条目(键值对)
// RPC参数不支持TMap(UE网络栈限制),InputDatas的RPC传输形态为条目数组
USTRUCT(BlueprintType)
struct FBXSkillInputEntry
{
	GENERATED_USTRUCT_BODY()

public:
	// 输入标签(BXSkillInput.*)
	UPROPERTY(BlueprintReadWrite)
	FGameplayTag Tag;

	// 输入数据
	UPROPERTY(BlueprintReadWrite)
	FInstancedStruct Value;
};



// 技能同步载荷(兼作施放上下文与RPC传输体)
USTRUCT(BlueprintType)
struct FBXSkillSyncPayload
{
	GENERATED_USTRUCT_BODY()

public:
	// 始作俑者
	UPROPERTY(BlueprintReadWrite)
	AActor* Instigator = nullptr;

	// 触发者
	UPROPERTY(BlueprintReadWrite)
	AActor* Triggerer = nullptr;

	// 客户端上传时间戳(毫秒,服务器世界时间域)
	UPROPERTY(BlueprintReadWrite)
	int64 ClientTimestamp = 0;

	// 动态输入数据(包含释放位置/朝向/瞄准/锁定等所有技能输入)
	UPROPERTY(BlueprintReadWrite)
	TArray<FBXSkillInputEntry> InputDatas;
};



// 构造带发起端标识的同步ID
inline int64 BXMakeSyncID(int64 InRawID, EBXSyncInitiator InInitiator)
{
	return (InRawID & BX_SYNC_ID_VALUE_MASK) | ((int64)InInitiator << BX_SYNC_ID_INITIATOR_SHIFT);
}

// 从同步ID提取发起端类型
inline EBXSyncInitiator BXGetSyncIDInitiator(int64 InSyncID)
{
	return (EBXSyncInitiator)((InSyncID >> BX_SYNC_ID_INITIATOR_SHIFT) & 0x7);
}
