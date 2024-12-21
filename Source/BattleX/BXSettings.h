#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"

#include "BXSettings.generated.h" 



UCLASS(Config = Game, DefaultConfig, Meta = (DisplayName = "BattleX Settings"))
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

};
