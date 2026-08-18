#pragma once

#include "CoreMinimal.h"

#include "Timeline/BXTLAsset.h"
#include "BXSkillEnums.h"

#include "BXSkillAsset.generated.h"



class UBXTaskCondition;



// 技能资源(对应技能系统,继承Timeline资产复用Sections/TaskList)
UCLASS()
class BATTLEX_API UBXSkillAsset : public UBXTLAsset
{
	GENERATED_BODY()

#pragma region Release
public:
	// 释放条件列表(服务器校验用,复用UBXTaskCondition体系)
	UPROPERTY(EditDefaultsOnly, Instanced, Category = "Release")
	TArray<UBXTaskCondition*> ReleaseConditions;

	// 默认冷却时长(秒,-1代表无冷却)
	UPROPERTY(EditDefaultsOnly, Category = "Release", meta = (ClampMin = "-1.0"))
	float Cooldown = -1.0f;

	// 锁定类型(决定释放时附带的数据,见EBXSkillLockType)
	UPROPERTY(EditDefaultsOnly, Category = "Release")
	EBXSkillLockType LockType = EBXSkillLockType::None;

#pragma endregion Release

};
