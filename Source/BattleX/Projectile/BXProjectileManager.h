#pragma once

#include "CoreMinimal.h"
#include "Tickable.h"
#include "GameplayTagContainer.h"

#include "BXManager.h"
#include "BXProjectileStructs.h"

#include "BXProjectileManager.generated.h"

class UBXProjectileAsset;
class UBXProjectileComponent;
class UBXShapeComponent;
class UNiagaraComponent;
class UNiagaraSystem;
class UAudioComponent;
class USoundBase;
class FBXProjectileComputeWorker;



DECLARE_LOG_CATEGORY_EXTERN(BXMGR_Projectile, Log, All);



// 子弹桶索引引用(全局索引表值)
struct FBXProjectileIndexRef
{
	// 桶种类标签
	FGameplayTag BucketType;

	// 桶内索引
	int32 Index = INDEX_NONE;
};



// 挂起的新增子弹条目(模拟期间事件回调同步发射的暂存)
struct FBXProjectilePendingEntry
{
	// 同步头(含ID/种类/发起端/发射时间戳)
	FBXProjectileSyncHeader Header;

	// 发射上下文
	FBXProjectileFireContext FireContext;

	// 是否本地预测
	bool bPredicted = false;

	// 广播载体组件(服务器权威发射时非空)
	TWeakObjectPtr<class UBXProjectileComponent> Carrier;
};



// 注册目标的形状信息
struct FBXProjectileTargetRegistration
{
	// 目标形状类型(无受击盒组件时的回退形状)
	EBXShapeType ShapeType = EBXShapeType::ST_Box;

	// 目标形状大小(球体=X 胶囊体=X,Y 长方体=X,Y,Z)
	FVector ShapeExtent = FVector(50.0f, 50.0f, 50.0f);

	// 受击盒组件(优先以其多受击盒信息逐盒展开,可空)
	TWeakObjectPtr<UBXShapeComponent> ShapeComponent;
};



// 子弹桶(同类型子弹连续内存,共享烘焙配置与飞行渲染组件)
struct FBXProjectileBucket
{
	// 子弹资源
	UBXProjectileAsset* Asset = nullptr;

	// 烘焙配置(桶创建时构建,并行阶段只读)
	FBXProjectileBakedConfig Baked;

	// 子弹模拟数据(连续内存,swap-remove回收)
	TArray<FBXProjectileSimData> Bullets;

	// 子弹ID(与Bullets平行,swap-remove同步维护)
	TArray<int64> BulletIDs;

	// 并行分片命中候选(各分片任务独写,GameThread合并读)
	TArray<TArray<FBXProjectileHitCandidate>> ShardCandidates;

	// 飞行Niagara组件(每桶一个常驻,空桶时销毁)
	TWeakObjectPtr<UNiagaraComponent> FlightComponent;

	// 飞行聚合音效组件(可空,音量随数量缩放)
	TWeakObjectPtr<UAudioComponent> FlightSoundComponent;

	// 创建特效缓存(桶创建时加载)
	TWeakObjectPtr<UNiagaraSystem> FireSystem;

	// 命中特效缓存(桶创建时加载)
	TWeakObjectPtr<UNiagaraSystem> HitSystem;

	// 创建音效缓存(桶创建时加载)
	TWeakObjectPtr<USoundBase> FireSound;

	// 命中音效缓存(桶创建时加载)
	TWeakObjectPtr<USoundBase> HitSound;

	// 飞行音效缓存(桶创建时加载)
	TWeakObjectPtr<USoundBase> FlightSound;

	// 筛选器自忽略标记(桶创建时从资产解析)
	bool bFilterIgnoreSelf = true;

	// 筛选器可通过类型解析(桶创建时从软引用同步加载)
	TArray<UClass*> FilterClassTypes;

	// 筛选器不可通过类型解析(桶创建时从软引用同步加载)
	TArray<UClass*> FilterIgnoreClassTypes;

	// 物理检测轮转游标(预算分帧,子弹轮流受检)
	int32 SweepCursor = 0;

	// Niagara提交暂存(避免每帧分配)
	TArray<FVector> ScratchPositions;

	// Niagara提交暂存
	TArray<FVector> ScratchVelocities;

	// Niagara提交暂存
	TArray<float> ScratchAges;
};



// 子弹管理器(统一运动更新/碰撞检测/数据传递Niagara/网络同步)
UCLASS()
class BATTLEX_API UBXProjectileManager : public UBXManager, public FTickableGameObject
{
	GENERATED_BODY()

#pragma region Important
public:
	static UBXProjectileManager* Get(UObject* InWorldContext);

	virtual void Initialize() override;

	virtual void Deinitialize() override;

	// 世界清理回调(PIE结束/关卡切换,清空运行数据)
	UFUNCTION()
	void OnWorldCleanupStart(UWorld* InWorld, bool bSessionEnded, bool bCleanupResources);

#pragma endregion Important



#pragma region Tick
public:
	virtual void Tick(float DeltaTime) override;

	virtual TStatId GetStatId() const override { RETURN_QUICK_DECLARE_CYCLE_STAT(UBXProjectileManager, STATGROUP_Tickables); }

	virtual UWorld* GetTickableGameObjectWorld() const override;

	virtual ETickableTickType GetTickableTickType() const override;

	virtual bool IsAllowedToTick() const override;

protected:
	// 单个固定步长模拟(快照→并行积分→命中结算→物理检测→生命周期,异步关闭或降级时本地执行)
	void StepSimulation(float InFixedStep);

	// 重建目标快照数组与Actor到索引映射(GameThread,受击盒组件逐盒展开,顺手清理失效注册)
	void BuildTargetSnapshots();

	// 解析桶内子弹的目标快照索引与检测职责(GameThread,并行阶段前调用)
	void InternalResolveTargetSnapshotIndices(FBXProjectileBucket& InOutBucket, bool bAuthority);

	// 并行分片运动积分(含数学命中候选收集,权威检测端全量/客户端代劳端逐子弹)
	void ParallelIntegrateBucket(FBXProjectileBucket& InOutBucket, float InFixedStep, bool bLocalDetect);

	// 弹簧矫正步进(全端每帧:客户端收敛快照偏差,服务器/单机收敛贝塞尔重建偏差)
	void UpdateSprings(float InDeltaTime);

	// 提交异步计算Job(拷贝全桶子弹与快照入队,失败返回false降级本地步进)
	bool TrySubmitComputeJob(float InFixedStep);

	// 收割异步计算结果(数值回写+候选结算+涉及桶物理检测与生命周期,顺序协议:先收割后提交)
	void HarvestComputeResults();

#pragma endregion Tick



#pragma region API
public:
	// 注册碰撞目标(数学判定模式,形状信息用于重叠检测)
	UFUNCTION(BlueprintCallable, Category = "BattleX|Projectile")
	void RegisterProjectileTarget(AActor* InTarget, EBXShapeType InShapeType, FVector InShapeExtent);

	// 注销碰撞目标
	UFUNCTION(BlueprintCallable, Category = "BattleX|Projectile")
	void UnregisterProjectileTarget(AActor* InTarget);

	// 权威端发射子弹(服务器/单机,经载体组件广播)
	UFUNCTION(BlueprintCallable, Category = "BattleX|Projectile")
	int64 FireProjectile(FGameplayTag InProjectileType, const FBXProjectileFireContext& InContext, UBXProjectileComponent* InCarrier);

	// 权威端显式停止子弹(拦截/打断,广播全端)
	UFUNCTION(BlueprintCallable, Category = "BattleX|Projectile")
	void StopProjectile(int64 InProjectileID, EBXProjectileStopReason InReason);

	// 查询子弹是否存在
	UFUNCTION(BlueprintPure, Category = "BattleX|Projectile")
	bool IsProjectileRunning(int64 InProjectileID) const;

	// 获取指定种类子弹数量
	UFUNCTION(BlueprintPure, Category = "BattleX|Projectile")
	int32 GetProjectileCount(FGameplayTag InProjectileType) const;

#pragma endregion API



#pragma region Asset
protected:
	// 收集子弹资源(扫描配置目录,按ProjectileType注册)
	void CollectProjectileAssets();

	// 按种类标签获取或加载子弹资源(失败返回空)
	UBXProjectileAsset* GetOrLoadProjectileAsset(const FGameplayTag& InProjectileType);

#pragma endregion Asset



#pragma region Bucket
protected:
	// 获取或创建桶(资源加载+烘焙+渲染组件惰性创建)
	FBXProjectileBucket* GetOrCreateBucket(const FGameplayTag& InProjectileType);

	// 销毁桶的飞行渲染组件(空桶时)
	void DestroyBucketPresentation(FBXProjectileBucket& InOutBucket);

#pragma endregion Bucket



#pragma region Simulation
protected:
	// 添加子弹到桶(模拟期间自动入挂起区,失败返回false)
	bool InternalAddProjectile(const FGameplayTag& InProjectileType, int64 InProjectileID, const FBXProjectileFireContext& InContext, bool bPredicted, UBXProjectileComponent* InCarrier);

	// 初始化子弹模拟数据(直线/跟踪/贝塞尔统一入口)
	void InternalInitSimData(const FBXProjectileBucket& InBucket, const FBXProjectileFireContext& InContext, int64 InProjectileID, bool bPredicted, UBXProjectileComponent* InCarrier, FBXProjectileSimData& OutData);

	// 子弹入桶(连续内存追加+索引表/归属表维护+飞行渲染组件惰性创建)
	void InternalAppendBullet(const FGameplayTag& InProjectileType, FBXProjectileBucket& InOutBucket, FBXProjectileSimData&& InData);

	// 物理模式Sweep检测(GameThread,预算分帧轮转,命中组播终态)
	void PhysicsSweepBucket(const FGameplayTag& InBucketType, FBXProjectileBucket& InOutBucket);

	// 命中结算(分片候选验证/筛选/穿透计数/组播,仅权威端)
	void ProcessBucketHits(const FGameplayTag& InBucketType, FBXProjectileBucket& InOutBucket);

	// 单个命中候选结算(索引验证/筛选器过滤/穿透判定/命中组播,权威端与客户端代劳端内部分流)
	void InternalResolveHitCandidate(const FGameplayTag& InBucketType, FBXProjectileBucket& InOutBucket, const FBXProjectileHitCandidate& InCandidate);

	// 异步命中候选结算(ID定位/状态复查/目标有效复查/筛选器/穿透去重,同步与异步候选共用结算核心)
	void InternalResolveAsyncCandidate(const FBXProjectileAsyncHitCandidate& InCandidate);

	// 命中结算共享核心(穿透判定/Payload构建/组播或客户端代劳分流)
	void InternalExecuteHitResolution(const FGameplayTag& InBucketType, FBXProjectileBucket& InOutBucket, FBXProjectileSimData& InOutData, AActor* InTargetActor, const FVector& InHitLocation, const FVector& InHitNormal, const FGameplayTag& InHitBoxTag);

	// 客户端代劳命中结算(本地预测表现+终态流转+上报服务器权威校验)
	void InternalResolveClientHitCandidate(const FGameplayTag& InBucketType, FBXProjectileBucket& InOutBucket, FBXProjectileSimData& InOutData, AActor* InTargetActor, const FVector& InHitLocation, const FVector& InHitNormal, const FGameplayTag& InHitBoxTag);

	// 客户端命中上报(经Instigator的载体组件发往服务器)
	void InternalReportClientHit(const FBXProjectileHitPayload& InPayload, const FBXProjectileSimData& InData);

	// 生命周期流转与死亡清理(寿命尽/残留到期/swap-remove回收/Finished事件广播)
	void ProcessBucketLifecycle(const FGameplayTag& InBucketType, FBXProjectileBucket& InOutBucket);

	// 单发子弹命中终态流转(残留或待回收标记)
	void InternalTerminateBullet(FBXProjectileBucket& InOutBucket, int32 InBulletIndex);

	// 回收桶内子弹(交换移除+索引表/归属表维护+Finished事件广播)
	void InternalRecycleBullet(const FGameplayTag& InBucketType, FBXProjectileBucket& InOutBucket, int32 InBulletIndex);

	// 移除子弹(模拟期间仅标记死亡,生命周期阶段统一回收)
	void InternalRemoveProjectile(int64 InProjectileID, EBXProjectileFinishReason InReason);

	// 确定性重放(从发射参数推进到指定时长,不触发命中与表现)
	void InternalReplaySimData(FBXProjectileBucket& InOutBucket, FBXProjectileSimData& InOutData, float InTargetElapsedTime, float InFixedStep);

	// 权威端发射统一入口(模拟期间入挂起区,合并时补组播)
	bool InternalAuthorityFire(const FBXProjectileSyncHeader& InHeader, const FBXProjectileFireContext& InContext, UBXProjectileComponent* InCarrier);

	// 合并模拟期间挂起的新增子弹(事件回调可能同步发射,避免容器迭代中变更)
	void MergePendingProjectiles();

	// Fired事件广播(Fired/Remote/Rebuild统一入口)
	void InternalBroadcastFired(const FGameplayTag& InProjectileType, const FBXProjectileFireContext& InContext, int64 InProjectileID);

	// 数学命中GameThread筛选器过滤(类别与Actor标签,语义同UBXCollisionLibrary::CheckCollisionResult)
	bool InternalPassesMathFilter(const FBXProjectileBucket& InBucket, AActor* InInstigator, AActor* InTarget) const;

#pragma endregion Simulation



#pragma region Net
public:
	// 服务器处理客户端发射RPC(校验ID/年龄/资源,通过则权威发射并广播)
	bool HandleServerFireProjectile(const FBXProjectileSyncHeader& InHeader, const FBXProjectileFireContext& InContext, UBXProjectileComponent* InCarrier, EBXProjectileDenyReason& OutDenyReason);

	// 客户端预测发射(立即本地模拟,由组件发送RPC,InCarrier兼作命中上报通道)
	int64 StartPredictedProjectile(FGameplayTag InProjectileType, const FBXProjectileFireContext& InContext, int64 InProjectileID, UBXProjectileComponent* InCarrier);

	// 客户端收到广播后的远端模拟启动(按时间戳估算已飞行时长重放)
	void StartRemoteProjectile(const FBXProjectileSyncHeader& InHeader, const FBXProjectileFireContext& InContext, UBXProjectileComponent* InCarrier);

	// 客户端收到预测否认(移除本地预测子弹+Denied事件广播)
	void HandleProjectileDenied(int64 InProjectileID);

	// 客户端收到命中广播(表现+Hit事件广播+终态流转,服务器命中组播本地执行同一入口)
	void HandleHitMulticast(const FBXProjectileHitPayload& InPayload, bool bTerminal);

	// 客户端收到停止广播(移除本地子弹)
	void HandleStopMulticast(int64 InProjectileID);

	// 服务器处理客户端命中上报(连接/目标/筛选器校验,通过则权威结算并组播)
	void HandleServerReportProjectileHit(const FBXProjectileHitPayload& InPayload, UBXProjectileComponent* InReporter);

	// 客户端收到快照批次(传输延迟外推后设置弹簧矫正目标)
	void HandleSnapshotMulticast(const FBXProjectileSnapshotBatch& InBatch);

	// 服务器定时收集长寿命子弹快照并经全局通道组播
	void InternalUpdateSnapshots(float InDeltaTime);

	// 获取或创建全局快照通道组件(隐藏瞬态复制Actor持有,服务器惰性创建)
	UBXProjectileComponent* GetOrCreateSnapshotChannel();

	// 预测确认(收到广播ID匹配时清除预测标记)
	void ConfirmPredictedProjectile(int64 InProjectileID);

	// 服务器构建复制状态投影(组件PreReplication调用)
	void BuildReplicatedStates(AActor* InOwner, TArray<FBXProjectileReplicatedState>& OutStates);

	// 客户端从复制状态重建子弹(Late Join,确定性重放续跑)
	void RebuildProjectileFromState(const FBXProjectileReplicatedState& InState, UBXProjectileComponent* InCarrier);

	// 移除非预测子弹(快照条目消失的乱序兜底)
	void RemoveProjectileIfNotPredicted(int64 InProjectileID);

	// 获取或创建归属Actor的载体组件(权威端惰性创建,广播通道)
	UBXProjectileComponent* GetOrCreateCarrier(AActor* InOwner);

#pragma endregion Net



#pragma region Presentation
protected:
	// 播放发射表现(特效+音效)
	void PresentationFire(const FBXProjectileBucket& InBucket, const FVector& InLocation);

	// 播放命中表现(特效+音效)
	void PresentationHit(const FBXProjectileBucket& InBucket, const FVector& InLocation, const FVector& InNormal);

	// 提交Niagara用户数组(每桶批量提交位置/速度/时长)+飞行音效聚合
	void CommitRender();

	// 当前世界是否允许表现(专用服务器跳过)
	bool InternalIsPresentationAllowed() const;

	// 获取表现宿主Actor(持有各桶飞行渲染组件,惰性创建)
	AActor* InternalGetPresentationHost();

	// 确保桶的飞行渲染组件与聚合音效组件存在(惰性创建,空桶时销毁)
	void InternalEnsureFlightPresentation(FBXProjectileBucket& InOutBucket);

#pragma endregion Presentation



#pragma region Data
protected:
	// 子弹资源注册表(种类标签→软引用)
	UPROPERTY(Transient)
	TMap<FGameplayTag, TSoftObjectPtr<UBXProjectileAsset>> ProjectileAssetMap;

	// 已加载资源列表(GC引用持有)
	UPROPERTY(Transient)
	TArray<TObjectPtr<UBXProjectileAsset>> LoadedAssets;

	// 筛选器解析类型GC强引用登记(桶内裸UClass指针不可见GC,防蓝图类无强引用被回收悬空)
	UPROPERTY(Transient)
	TArray<TObjectPtr<UClass>> PinnedFilterClasses;

	// 子弹桶(种类标签→桶)
	TMap<FGameplayTag, FBXProjectileBucket> Buckets;

	// 全局子弹索引表(ID→桶+索引)
	TMap<int64, FBXProjectileIndexRef> ProjectileIndexMap;

	// 归属者的子弹ID登记表(复制快照构建用)
	TMap<TWeakObjectPtr<AActor>, TSet<int64>> OwnerProjectileMap;

	// 注册目标表(Actor→形状信息)
	TMap<TWeakObjectPtr<AActor>, FBXProjectileTargetRegistration> RegisteredTargets;

	// 目标快照数组(每步重建,并行阶段只读)
	TArray<FBXProjectileTargetSnapshot> TargetSnapshots;

	// Actor到快照索引映射(每步重建,GameThread解析用)
	TMap<TWeakObjectPtr<AActor>, int32> TargetActorToIndex;

	// 挂起的新增子弹(模拟期间事件回调同步发射的暂存)
	TArray<FBXProjectilePendingEntry> PendingProjectiles;

	// 表现宿主Actor(持有各桶飞行渲染组件,世界切换时随世界销毁)
	TWeakObjectPtr<AActor> PresentationHost;

	// 全局快照通道组件(服务器惰性创建,长寿命子弹快照组播)
	TWeakObjectPtr<UBXProjectileComponent> SnapshotChannel;

	// 固定步长累积器
	float StepAccumulator = 0.0f;

	// 异步计算步序号(递增,Job/Result配对)
	uint64 ComputeStepSerial = 0;

	// 异步计算驻留线程(惰性创建,Deinitialize停机)
	TUniquePtr<FBXProjectileComputeWorker> ComputeWorker;

	// 快照下发计时器
	float SnapshotTimer = 0.0f;

	// 是否处于模拟阶段(命中事件回调可能同步发射,入挂起区)
	bool bInSimulation = false;

#pragma endregion Data
};
