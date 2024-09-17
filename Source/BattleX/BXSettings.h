#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DeveloperSettings.h"

#include "Internationalization/StringTable.h"

#include "BXSettings.generated.h"



UCLASS(Config = Game, DefaultConfig, Meta = (DisplayName = "BattleX Settings"))
class UBXSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UBXSettings(const FObjectInitializer& ObjectInitializer);

public:
	// 受击类型类型
	UPROPERTY(Config, EditDefaultsOnly, Category = "Collision")
	TEnumAsByte<ECollisionChannel> CollisionChannel;

	// 碰撞Debug时长
	UPROPERTY(Config, EditDefaultsOnly, Category = "Collision", Meta = (ClampMin = "0.1"))
	float CollisionDebugDuration = 1.0f;



	// 任务处理器映射表
	UPROPERTY(Config, EditDefaultsOnly, Category = "Task")
	TMap<TSoftClassPtr<UObject>, TSoftClassPtr<UObject>> TaskProcessorMap;

	// 任务自定义数据集映射表
	UPROPERTY(Config, EditDefaultsOnly, Category = "Task")
	TMap<TSoftClassPtr<UObject>, TSoftObjectPtr<UScriptStruct>> TaskCustomDataMap;

	// 处理待执行任务的深度
	UPROPERTY(Config, EditDefaultsOnly, Category = "Task")
	int32 FramePendingTaskLimit = 6;

};
