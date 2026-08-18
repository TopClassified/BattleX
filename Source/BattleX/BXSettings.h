#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"

#include "BXSettings.generated.h" 



UCLASS(Config = BattleX, DefaultConfig, Meta = (DisplayName = "BattleX Settings"))
class BATTLEX_API UBXSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UBXSettings(const FObjectInitializer& ObjectInitializer);

public:
	// 按顺序创建并初始化的管理器
	UPROPERTY(Config, EditDefaultsOnly, Category = "Important")
	TArray<TSubclassOf<class UBXManager>> ManagerClasses;
	
	
	// 碰撞Debug时长
	UPROPERTY(Config, EditDefaultsOnly, Category = "Collision", Meta = (ClampMin = "0.1"))
	float CollisionDebugDuration = 1.0f;
	

	// 任务处理器映射表
	UPROPERTY(Config, EditDefaultsOnly, Category = "Task")
	TMap<TSoftClassPtr<UObject>, TSoftClassPtr<UObject>> TaskProcessorMap;

	// 任务自定义数据集映射表
	UPROPERTY(Config, EditDefaultsOnly, Category = "Task")
	TMap<TSoftClassPtr<UObject>, TSoftObjectPtr<UScriptStruct>> TaskCustomDataMap;


	// 客户端预测最长时长(秒)
	UPROPERTY(Config, EditDefaultsOnly, Category = "Sync", meta = (ClampMin = "0.05"))
	float SkillPredictMaxDuration = 0.3f;

	// 服务器收到客户端释放请求时是否加速弥补
	UPROPERTY(Config, EditDefaultsOnly, Category = "Sync")
	bool bServerAccelerateOnReceive = true;

	// 服务器加速倍率
	UPROPERTY(Config, EditDefaultsOnly, Category = "Sync", meta = (ClampMin = "1.0"))
	float ServerAccelerateRate = 2.0f;

	// 其他客户端收到广播时是否加速弥补
	UPROPERTY(Config, EditDefaultsOnly, Category = "Sync")
	bool bClientAccelerateOnBroadcast = true;

	// 客户端加速倍率
	UPROPERTY(Config, EditDefaultsOnly, Category = "Sync", meta = (ClampMin = "1.0"))
	float ClientAccelerateRate = 2.0f;

	// 服务器是否做碰撞检测(关闭时等待客户端上报)
	UPROPERTY(Config, EditDefaultsOnly, Category = "Sync")
	bool bServerCollisionCheck = false;

	// 服务器等待客户端碰撞结果的额外延迟销毁时长(秒)
	UPROPERTY(Config, EditDefaultsOnly, Category = "Sync", meta = (ClampMin = "0.0"))
	float CollisionTaskServerExtraLife = 0.15f;

	// 服务器校验释放请求的最大年龄(服务器世界时间域毫秒,超过视为过老请求拒绝)
	UPROPERTY(Config, EditDefaultsOnly, Category = "Sync", meta = (ClampMin = "0"))
	int32 SkillRequestMaxAgeMs = 500;

};
