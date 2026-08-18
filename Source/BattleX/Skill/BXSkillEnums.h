#pragma once

#include "UObject/ObjectMacros.h"

#include "BXSkillEnums.generated.h"



// 技能预测状态
UENUM(BlueprintType)
enum class EBXPredictState : uint8
{
	// 非预测,正常播放
	None = 0,

	// 客户端预测中,等待服务器结果
	Predicting,

	// 服务器已确认
	Confirmed,

	// 正在回滚
	RollingBack,

	EBXPredictState_TMax UMETA(Hidden)
};



// 技能锁定类型(互斥,决定释放时需要附带的数据)
UENUM(BlueprintType)
enum class EBXSkillLockType : uint8
{
	// 无锁定
	None = 0,

	// 锁定目标(附带LockParts)
	Target,

	// 锁定位置(附带AimLocation)
	Location,

	// 锁定朝向(附带AimRotation)
	Direction,

	EBXSkillLockType_TMax UMETA(Hidden)
};
