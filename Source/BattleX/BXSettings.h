#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
// UE5.8: FDirectoryPath 移至 UObject/SoftObjectPath.h(原 Misc/DirectoryPath.h 已删除)
#include "UObject/SoftObjectPath.h"

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

	// 行为预测最长时长(秒,客户端预测启动行为后超时未确认即回滚)
	UPROPERTY(Config, EditDefaultsOnly, Category = "Sync", meta = (ClampMin = "0.05"))
	float BehaviorPredictMaxDuration = 0.3f;

	// 状态预测最长时长(秒,客户端预测进入状态后超时未确认即回滚)
	UPROPERTY(Config, EditDefaultsOnly, Category = "Sync", meta = (ClampMin = "0.05"))
	float StatePredictMaxDuration = 0.3f;

	// 服务器校验行为请求的最大年龄(服务器世界时间域毫秒)
	UPROPERTY(Config, EditDefaultsOnly, Category = "Sync", meta = (ClampMin = "0"))
	int32 BehaviorRequestMaxAgeMs = 500;

	// 服务器校验状态请求的最大年龄(服务器世界时间域毫秒)
	UPROPERTY(Config, EditDefaultsOnly, Category = "Sync", meta = (ClampMin = "0"))
	int32 StateRequestMaxAgeMs = 500;


#pragma region Projectile
public:
	// 子弹模拟固定步长(秒,与帧率解耦保证双端步进序列一致)
	UPROPERTY(Config, EditDefaultsOnly, Category = "Projectile", meta = (ClampMin = "0.005"))
	float ProjectileFixedStep = 0.033334f;

	// 子弹每帧最大步数(超出丢弃欠账防螺旋)
	UPROPERTY(Config, EditDefaultsOnly, Category = "Projectile", meta = (ClampMin = "1"))
	int32 ProjectileMaxStepsPerFrame = 2;

	// 子弹速度/转向曲线烘焙采样数
	UPROPERTY(Config, EditDefaultsOnly, Category = "Projectile", meta = (ClampMin = "8"))
	int32 ProjectileCurveBakeSamples = 128;

	// 贝塞尔弧长表采样段数
	UPROPERTY(Config, EditDefaultsOnly, Category = "Projectile", meta = (ClampMin = "8"))
	int32 ProjectileBezierArcSamples = 64;

	// 贝塞尔目标移动重建阈值(cm)
	UPROPERTY(Config, EditDefaultsOnly, Category = "Projectile", meta = (ClampMin = "1.0"))
	float ProjectileTargetRefreshDistance = 100.0f;

	// 长寿命子弹快照下发间隔(秒,0关闭)
	UPROPERTY(Config, EditDefaultsOnly, Category = "Projectile", meta = (ClampMin = "0.0"))
	float ProjectileSnapshotInterval = 0.5f;

	// 快照下发的子弹最低存活时长(秒,低于此视为短寿命子弹不下发)
	UPROPERTY(Config, EditDefaultsOnly, Category = "Projectile", meta = (ClampMin = "0.0"))
	float ProjectileSnapshotMinAge = 2.0f;

	// 快照矫正临界阻尼弹簧自然频率(Hz,越大收敛越快)
	UPROPERTY(Config, EditDefaultsOnly, Category = "Projectile", meta = (ClampMin = "0.1"))
	float ProjectileSpringFrequency = 4.0f;

	// 物理检测模式每桶每帧Sweep预算(各桶独立预算,预算轮转避免尖峰)
	UPROPERTY(Config, EditDefaultsOnly, Category = "Projectile", meta = (ClampMin = "1"))
	int32 ProjectileSweepBudgetPerFrame = 128;

	// 是否启用子弹异步计算驻留线程(积分与数学判定移交专职线程分发,命中结算延后一帧;关闭则GameThread同步并行)
	UPROPERTY(Config, EditDefaultsOnly, Category = "Projectile")
	bool bProjectileAsyncCompute = true;

	// 子弹资源扫描目录
	UPROPERTY(Config, EditDefaultsOnly, Category = "Projectile")
	TArray<FDirectoryPath> ProjectileAssetPaths;

#pragma endregion Projectile

};
