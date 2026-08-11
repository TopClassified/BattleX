#pragma once

#include "CoreMinimal.h"

#include "BXBuffEnums.generated.h"



// BUFF生命周期类型
UENUM(BlueprintType)
enum class EBXBuffLifeType : uint8
{
	// 限时
	BL_Duration,

	// 无限
	BL_Infinite,

	// 手动控制
	BL_Manual,
};

// 层级生命周期模式
UENUM(BlueprintType)
enum class EBXBuffLayerLifeMode : uint8
{
	// 所有层共享一个生命周期
	BLL_Shared,

	// 每层独立生命周期
	BLL_Independent,
};

// BUFF共存策略
UENUM(BlueprintType)
enum class EBXBuffCoexistPolicy : uint8
{
	// 不同始作俑者共存
	BC_Coexist,

	// 不共存,替换为等级最高且剩余时长最长
	BC_Replace,
};

// BUFF移除原因
UENUM(BlueprintType)
enum class EBXBuffRemoveReason : uint8
{
	// 手动移除
	BRR_Manual,

	// 生命到期
	BRR_Expired,

	// 超过最大层数
	BRR_OverStack,

	// Owner丢失
	BRR_OwnerLost,
};
