#pragma once

#include "UObject/ObjectMacros.h"

#include "BXNetEnums.generated.h"



// 同步发起端类型
UENUM(BlueprintType)
enum class EBXSyncInitiator : uint8
{
	// 客户端发起
	Client = 0,

	// 服务器发起
	Server = 1,

	EBXSyncInitiator_TMax UMETA(Hidden)
};



// 同步ID中发起端标识的起始位(高3位: bit 61~63, 共3位)
#define BX_SYNC_ID_INITIATOR_SHIFT 61

// 同步ID中发起端标识的位掩码(0x7 << 61, 覆盖bit 61~63)
#define BX_SYNC_ID_INITIATOR_MASK  ((int64)0x7 << BX_SYNC_ID_INITIATOR_SHIFT)

// 同步ID中ID数值部分的位掩码(低61位, 用于屏蔽高3位发起端标识)
#define BX_SYNC_ID_VALUE_MASK      (~BX_SYNC_ID_INITIATOR_MASK)
