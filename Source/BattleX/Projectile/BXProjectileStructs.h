#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "StructUtils/InstancedStruct.h"

#include "BXEnums.h"
#include "BXNetEnums.h"

#include "BXProjectileStructs.generated.h"



// 跟踪目标失效策略
UENUM(BlueprintType)
enum class EBXProjectileTargetInvalidPolicy : uint8
{
	TIP_KeepDirection                  = 0   UMETA(DisplayName = "保持当前方向"),
	TIP_FlyToLastKnownLocation               UMETA(DisplayName = "直飞最后已知位置"),
	TIP_SelfDestruct                         UMETA(DisplayName = "自毁"),

	TIP_TMax                                 UMETA(Hidden)
};



// 子弹碰撞形状(数学判定按形状膨胀线段,物理检测按形状生成Sweep几何体)
UENUM(BlueprintType)
enum class EBXProjectileShape : uint8
{
	BS_Point                  = 0             UMETA(DisplayName = "点"),
	BS_Sphere                                UMETA(DisplayName = "球体"),
	BS_Box                                   UMETA(DisplayName = "长方体"),

	BS_TMax                                  UMETA(Hidden)
};



// 子弹运行状态
UENUM(BlueprintType)
enum class EBXProjectileState : uint8
{
	PS_Active                 = 0             UMETA(DisplayName = "飞行中"),
	PS_Residual                               UMETA(DisplayName = "残留中"),
	PS_Dead                                   UMETA(Hidden)

	// PS_Dead为内部待回收标记,不暴露蓝图
};



// 命中类型
UENUM(BlueprintType)
enum class EBXProjectileHitType : uint8
{
	HT_Unit                   = 0             UMETA(DisplayName = "单位命中"),
	HT_World                                  UMETA(DisplayName = "场景命中"),

	HT_TMax                                   UMETA(Hidden)
};



// 命中效果类型(配置于子弹资产,每次命中在权威端逐条执行)
UENUM(BlueprintType)
enum class EBXProjectileHitEffectType : uint8
{
	HE_Damage                 = 0             UMETA(DisplayName = "伤害"),
	HE_Buff                                   UMETA(DisplayName = "施加BUFF"),
	HE_Skill                                  UMETA(DisplayName = "播放技能"),

	HE_TMax                                   UMETA(Hidden)
};



// 命中效果条目(伤害待伤害/属性框架接入后实现;BUFF与技能资产桶创建时同步加载缓存)
USTRUCT(BlueprintType)
struct FBXProjectileHitEffect
{
	GENERATED_USTRUCT_BODY()

	// 效果类型
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HitEffect")
	EBXProjectileHitEffectType EffectType = EBXProjectileHitEffectType::HE_Damage;

	// 施加的BUFF资产(对命中目标施加)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HitEffect", Meta = (EditCondition = "EffectType == EBXProjectileHitEffectType::HE_Buff", EditConditionHides))
	TSoftObjectPtr<class UBXBuffAsset> BuffAsset;

	// 播放的技能资产(在命中目标身上播放)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HitEffect", Meta = (EditCondition = "EffectType == EBXProjectileHitEffectType::HE_Skill", EditConditionHides))
	TSoftObjectPtr<class UBXSkillAsset> SkillAsset;
};



// 服务器否认原因
UENUM(BlueprintType)
enum class EBXProjectileDenyReason : uint8
{
	DR_DuplicateID            = 0             UMETA(DisplayName = "ID重复"),
	DR_AssetMissing                           UMETA(DisplayName = "子弹种类不存在"),
	DR_RequestExpired                         UMETA(DisplayName = "请求过老"),
	DR_InvalidOwnership                       UMETA(DisplayName = "归属校验失败"),

	DR_TMax                                   UMETA(Hidden)
};



// 显式停止原因(自然结束不广播,不在此列)
UENUM(BlueprintType)
enum class EBXProjectileStopReason : uint8
{
	SR_Interrupted            = 0             UMETA(DisplayName = "拦截中断"),

	SR_TMax                                   UMETA(Hidden)
};



// 子弹结束原因(事件广播用)
UENUM(BlueprintType)
enum class EBXProjectileFinishReason : uint8
{
	FR_Hit                    = 0             UMETA(DisplayName = "命中终止"),
	FR_Lifetime                               UMETA(DisplayName = "生命结束"),
	FR_ResidualEnd                            UMETA(DisplayName = "残留结束"),
	FR_Interrupted                            UMETA(DisplayName = "显式中断"),
	FR_PredictDenied                          UMETA(DisplayName = "预测否认回滚"),

	FR_TMax                                   UMETA(Hidden)
};



// 发射方向类型(Task配置用)
UENUM(BlueprintType)
enum class EBXProjectileFireDirection : uint8
{
	FD_OwnerForward           = 0             UMETA(DisplayName = "所有者朝向"),
	FD_ToTarget                               UMETA(DisplayName = "指向目标"),

	FD_TMax                                   UMETA(Hidden)
};



// 贝塞尔中间控制点配置(以起始点指向目标点线段为基准的百分比+偏移)
USTRUCT(BlueprintType)
struct FBXBezierControlPoint
{
	GENERATED_USTRUCT_BODY()

public:
	// 起始点指向目标点线段上的百分比位置(0~1)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float Percent = 0.5f;

	// 偏移向量(X沿线段方向 Y侧向 Z垂直向,基于发射者UpVector构建的局部坐标系)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Offset = FVector::ZeroVector;
};



// 子弹发射上下文(兼作发射参数与RPC传输体)
USTRUCT(BlueprintType)
struct FBXProjectileFireContext
{
	GENERATED_USTRUCT_BODY()

public:
	// 始作俑者
	UPROPERTY(BlueprintReadWrite)
	AActor* Instigator = nullptr;

	// 触发者
	UPROPERTY(BlueprintReadWrite)
	AActor* Triggerer = nullptr;

	// 发射位置
	UPROPERTY(BlueprintReadWrite)
	FVector StartLocation = FVector::ZeroVector;

	// 发射朝向(单位向量)
	UPROPERTY(BlueprintReadWrite)
	FVector FireDirection = FVector::ForwardVector;

	// 发射者UpVector(贝塞尔局部坐标系Z轴基准)
	UPROPERTY(BlueprintReadWrite)
	FVector FireUpVector = FVector::UpVector;

	// 目标(跟踪/贝塞尔弹使用,可为空)
	UPROPERTY(BlueprintReadWrite)
	AActor* Target = nullptr;

	// 发起端时间戳(毫秒,服务器世界时间域,经引擎时间同步校正)
	UPROPERTY(BlueprintReadWrite)
	int64 ClientTimestamp = 0;

	// 附带上下文数据(命中事件原样带回,需为可序列化USTRUCT)
	UPROPERTY(BlueprintReadWrite)
	FInstancedStruct ContextData;
};



// 子弹同步头(所有RPC共用)
USTRUCT(BlueprintType)
struct FBXProjectileSyncHeader
{
	GENERATED_USTRUCT_BODY()

public:
	// 子弹运行时唯一ID(客户端或服务器生成,高3位为发起端标识)
	UPROPERTY(BlueprintReadWrite)
	int64 ProjectileID = 0;

	// 子弹种类标签
	UPROPERTY(BlueprintReadWrite)
	FGameplayTag ProjectileType;

	// 发起端类型
	UPROPERTY(BlueprintReadWrite)
	EBXSyncInitiator Initiator = EBXSyncInitiator::Client;

	// 发起端时间戳(毫秒,服务器世界时间域)
	UPROPERTY(BlueprintReadWrite)
	int64 ClientTimestamp = 0;
};



// 命中事件载荷
USTRUCT(BlueprintType)
struct FBXProjectileHitPayload
{
	GENERATED_USTRUCT_BODY()

public:
	// 子弹ID
	UPROPERTY(BlueprintReadWrite)
	int64 ProjectileID = 0;

	// 子弹种类标签
	UPROPERTY(BlueprintReadWrite)
	FGameplayTag ProjectileType;

	// 命中类型
	UPROPERTY(BlueprintReadWrite)
	EBXProjectileHitType HitType = EBXProjectileHitType::HT_Unit;

	// 命中时的子弹时刻(秒,服务器原样回传客户端上报值,代劳端以此做组播回声去重)
	UPROPERTY(BlueprintReadWrite)
	float HitTime = 0.0f;

	// 命中点
	UPROPERTY(BlueprintReadWrite)
	FVector HitLocation = FVector::ZeroVector;

	// 命中法线
	UPROPERTY(BlueprintReadWrite)
	FVector HitNormal = FVector::UpVector;

	// 命中目标(单位命中时有值)
	UPROPERTY(BlueprintReadWrite)
	AActor* HitTarget = nullptr;

	// 命中受击盒标签(多受击盒目标展开后标识命中部位)
	UPROPERTY(BlueprintReadWrite)
	FGameplayTag HitBoxTag;

	// 始作俑者
	UPROPERTY(BlueprintReadWrite)
	AActor* Instigator = nullptr;

	// 触发者
	UPROPERTY(BlueprintReadWrite)
	AActor* Triggerer = nullptr;

	// 附带上下文数据(发射时原样带回)
	UPROPERTY(BlueprintReadWrite)
	FInstancedStruct ContextData;
};



// 子弹事件参数(发射/结束/否认广播用)
USTRUCT(BlueprintType)
struct FBXProjectileEventParameter
{
	GENERATED_USTRUCT_BODY()

public:
	// 子弹ID
	UPROPERTY(BlueprintReadWrite)
	int64 ProjectileID = 0;

	// 子弹种类标签
	UPROPERTY(BlueprintReadWrite)
	FGameplayTag ProjectileType;

	// 始作俑者
	UPROPERTY(BlueprintReadWrite)
	AActor* Instigator = nullptr;

	// 触发者
	UPROPERTY(BlueprintReadWrite)
	AActor* Triggerer = nullptr;

	// 结束原因(仅Finished事件有效)
	UPROPERTY(BlueprintReadWrite)
	EBXProjectileFinishReason FinishReason = EBXProjectileFinishReason::FR_Hit;

	// 附带上下文数据(发射时原样带回)
	UPROPERTY(BlueprintReadWrite)
	FInstancedStruct ContextData;
};



// 子弹复制状态投影(Late Join重建用,COND_InitialOnly仅新连接初始同步发送)
USTRUCT()
struct FBXProjectileReplicatedState
{
	GENERATED_USTRUCT_BODY()

public:
	// 子弹ID
	UPROPERTY()
	int64 ProjectileID = 0;

	// 子弹种类标签
	UPROPERTY()
	FGameplayTag ProjectileType;

	// 发射上下文
	UPROPERTY()
	FBXProjectileFireContext FireContext;

	// 已飞行时长(秒)
	UPROPERTY()
	float ElapsedTime = 0.0f;

	// 发送时刻位置(重放后校正锚点)
	UPROPERTY()
	FVector Location = FVector::ZeroVector;

	// 运行状态
	UPROPERTY()
	EBXProjectileState State = EBXProjectileState::PS_Active;
};



// 子弹快照条目(服务器定时下发,客户端弹簧矫正浮点误差累计)
USTRUCT()
struct FBXProjectileSnapshotEntry
{
	GENERATED_USTRUCT_BODY()

public:
	// 子弹ID
	UPROPERTY()
	int64 ProjectileID = 0;

	// 服务器模拟位置
	UPROPERTY()
	FVector Location = FVector::ZeroVector;

	// 服务器模拟速度(客户端外推传输延迟用)
	UPROPERTY()
	FVector Velocity = FVector::ZeroVector;

	// 已飞行时长(秒)
	UPROPERTY()
	float ElapsedTime = 0.0f;
};



// 子弹快照批次(单次组播载荷,服务器世界时间域时间戳)
USTRUCT()
struct FBXProjectileSnapshotBatch
{
	GENERATED_USTRUCT_BODY()

public:
	// 发送时刻服务器世界时间(毫秒)
	UPROPERTY()
	int64 ServerTimestamp = 0;

	// 快照条目
	UPROPERTY()
	TArray<FBXProjectileSnapshotEntry> Entries;
};



// 曲线烘焙查找表(等间隔采样,双端因Asset一致而一致)
struct FBXProjectileBakedCurve
{
	// 采样值数组
	TArray<float> Samples;

	// 采样区间时长(定义域长度)
	float TimeRange = 1.0f;

	// 采样(时间钳制到定义域,线性插值)
	float Sample(float InTime) const
	{
		if (Samples.Num() <= 1)
		{
			return Samples.Num() > 0 ? Samples[0] : 0.0f;
		}

		const float ClampedTime = FMath::Clamp(InTime, 0.0f, TimeRange);
		const float Alpha = TimeRange > 0.0f ? ClampedTime / TimeRange : 0.0f;
		const float ScaledIndex = Alpha * (Samples.Num() - 1);
		const int32 LowerIndex = FMath::Min(FMath::FloorToInt(ScaledIndex), Samples.Num() - 2);
		const float Blend = ScaledIndex - LowerIndex;
		return FMath::Lerp(Samples[LowerIndex], Samples[LowerIndex + 1], Blend);
	}
};



// 烘焙配置(Asset加载后构建,并行阶段只读)
struct FBXProjectileBakedConfig
{
	// 速度查找表
	FBXProjectileBakedCurve SpeedCurve;

	// 转向角速度查找表(deg/s)
	FBXProjectileBakedCurve TurnRateCurve;

	// 贝塞尔中间控制点配置(烘焙副本,避免并行阶段访问UObject)
	TArray<FBXBezierControlPoint> BezierControlPoints;

	// 最大生命周期(秒)
	float MaxLifetime = 1.0f;

	// 命中或寿命尽后的残留时长(秒)
	float ResidualTime = 0.0f;

	// 重力向量(cm/s²)
	FVector Gravity = FVector::ZeroVector;

	// 子弹碰撞形状
	EBXProjectileShape BulletShape = EBXProjectileShape::BS_Point;

	// 子弹碰撞半径(cm,球体形状)
	float BulletRadius = 10.0f;

	// 子弹长方体半尺寸(cm,局部空间X沿飞行方向,长方体形状)
	FVector BulletBoxExtent = FVector(10.0f, 10.0f, 10.0f);

	// 最大穿透次数(总命中预算,含冷却后对同一目标的再次命中,1为命中即毁)
	int32 MaxPenetrationCount = 1;

	// 同目标命中冷却(秒,命中后经过该时长才允许再次命中同一目标,0为仅同帧去重)
	float HitCooldown = 0.0f;

	// 是否跟踪目标
	bool bHoming = false;

	// 跟踪目标失效策略
	EBXProjectileTargetInvalidPolicy TargetInvalidPolicy = EBXProjectileTargetInvalidPolicy::TIP_KeepDirection;

	// 是否贝塞尔移动(优先级高于跟踪)
	bool bUseBezier = false;

	// 贝塞尔目标移动重建阈值(cm)
	float TargetRefreshDistance = 100.0f;

	// 贝塞尔弧长表采样段数
	int32 ArcSampleCount = 64;

	// 是否执行数学判定(注册目标重叠检测,与运动积分同并行阶段,开启时才收集数学候选)
	bool bMathCheck = true;

	// 是否执行物理检测(场景Sweep,GameThread,与数学判定独立开关可任意组合)
	bool bPhysicsCheck = false;
};



// 贝塞尔运行时上下文(每发子弹独立持有)
struct FBXProjectileBezierRuntime
{
	// 世界空间控制点(首尾为发射点与目标点)
	TArray<FVector> ControlPoints;

	// 弧长累积表(按参数u等间隔采样,单调递增)
	TArray<float> ArcLength;

	// 曲线总弧长
	float TotalArcLength = 0.0f;

	// 当前参数u(0~1,弧长推进)
	float ParamU = 0.0f;
};



// 目标快照(每步GameThread拷贝,并行阶段只读,禁止解引用Target)
struct FBXProjectileTargetSnapshot
{
	// 目标位置
	FVector Location = FVector::ZeroVector;

	// 目标朝向
	FQuat Rotation = FQuat::Identity;

	// 目标形状大小(球体=X 胶囊体=X,Y 长方体=X,Y,Z)
	FVector ShapeExtent = FVector(50.0f, 50.0f, 50.0f);

	// 目标形状类型
	EBXShapeType ShapeType = EBXShapeType::ST_Box;

	// 受击盒标签(多受击盒展开后标识部位,单形状注册为空)
	FGameplayTag HitBoxTag;

	// 目标UID(命中去重)
	uint32 TargetUID = 0;

	// 目标是否有效(失效目标按策略处理)
	bool bValid = false;

	// 目标Actor引用(GameThread专用,并行阶段禁止解引用)
	TWeakObjectPtr<AActor> Target;
};



// 目标命中冷却条目(以子弹时刻为基准的绝对截止时刻,免逐帧递减;命中时刷新,过期即允许再次命中)
struct FBXProjectileTargetCooldown
{
	// 目标UID
	uint32 TargetUID = 0;

	// 冷却截止的子弹时刻(秒,命中时刻+冷却时长)
	float CooldownEndTime = 0.0f;
};



// 子弹模拟数据(纯POD,并行阶段仅允许Solver访问数值成员,禁止解引用FireContext内的对象指针)
struct FBXProjectileSimData
{
	// 子弹ID
	int64 ProjectileID = 0;

	// 发射上下文(GameThread专用,并行阶段禁止解引用内部对象指针)
	FBXProjectileFireContext FireContext;

	// 贝塞尔运行时上下文
	FBXProjectileBezierRuntime Bezier;

	// 上一步位置(本步碰撞线段起点)
	FVector PrevLocation = FVector::ZeroVector;

	// 当前位置
	FVector Location = FVector::ZeroVector;

	// 上次物理检测位置(物理模式Sweep线段起点,预算轮转续检)
	FVector LastSweptLocation = FVector::ZeroVector;

	// 当前速度
	FVector Velocity = FVector::ZeroVector;

	// 已飞行时长(秒,残留阶段继续累积供Niagara淡出)
	float ElapsedTime = 0.0f;

	// 残留倒计时(秒)
	float ResidualTimer = 0.0f;

	// 运行状态
	EBXProjectileState State = EBXProjectileState::PS_Active;

	// 结束原因(终态流转时写入,清理阶段广播Finished事件用)
	EBXProjectileFinishReason FinishReason = EBXProjectileFinishReason::FR_Lifetime;

	// 已穿透次数
	int32 PenetrationCount = 0;

	// 目标命中冷却表(穿透模式:对同一目标命中后进入冷却,冷却结束允许再次命中;GameThread读写,worker不参与判定)
	TArray<FBXProjectileTargetCooldown> TargetHitCooldowns;

	// 本步解析出的目标快照索引(INDEX_NONE代表无有效目标,每步GameThread重建)
	int32 TargetSnapshotIndex = INDEX_NONE;

	// 本端是否对该子弹执行命中检测(GameThread解析:单机/LS/服务器检测开启全量,客户端代劳模式仅本连接Instigator的子弹)
	bool bLocalDetectable = false;

	// 始作俑者UID(命中排除自身)
	uint32 InstigatorUID = 0;

	// 最后已知目标位置(跟踪失效策略与贝塞尔重建判定基准)
	FVector LastTargetLocation = FVector::ZeroVector;

	// 弹簧矫正偏移(渲染与检测位置=模拟位置+偏移,临界阻尼弹簧收敛)
	FVector SpringOffset = FVector::ZeroVector;

	// 弹簧速度
	FVector SpringVelocity = FVector::ZeroVector;

	// 弹簧矫正目标(快照外推位置-本地模拟位置,0代表期望偏移归零)
	FVector SpringTarget = FVector::ZeroVector;

	// 广播载体组件(GameThread专用,命中/停止组播通道,并行阶段禁止访问)
	TWeakObjectPtr<class UBXProjectileComponent> Carrier;

	// 归属载体拥有者(归属表键,载体销毁后仍可正确移除登记)
	TWeakObjectPtr<AActor> Owner = nullptr;

	// 是否本地预测(等待服务器确认/否认)
	bool bPredicted = false;

	// 最近本地上报命中的子弹时刻列表(代劳端组播回声去重:服务器回声按命中时刻精确匹配,保留多条防连续上报间回声错配)
	TArray<float> RecentReportedHitTimes;

	// 拷贝异步计算所需字段(积分/数学判定/候选聚合/收割回写;跳过ContextData实例克隆与GT专用字段,弹幕量级下消除每步每发的堆分配)
	void CopyComputeFieldsTo(FBXProjectileSimData& OutData) const
	{
		OutData.ProjectileID = ProjectileID;
		OutData.FireContext.StartLocation = FireContext.StartLocation;
		OutData.FireContext.FireDirection = FireContext.FireDirection;
		OutData.FireContext.FireUpVector = FireContext.FireUpVector;
		OutData.Bezier = Bezier;
		OutData.PrevLocation = PrevLocation;
		OutData.Location = Location;
		OutData.Velocity = Velocity;
		OutData.ElapsedTime = ElapsedTime;
		OutData.ResidualTimer = ResidualTimer;
		OutData.State = State;
		OutData.LastTargetLocation = LastTargetLocation;
		OutData.SpringOffset = SpringOffset;
		OutData.TargetSnapshotIndex = TargetSnapshotIndex;
		OutData.bLocalDetectable = bLocalDetectable;
		OutData.InstigatorUID = InstigatorUID;
	}
};



// 命中候选(并行阶段产出,GameThread验证结算)
struct FBXProjectileHitCandidate
{
	// 桶内子弹索引
	int32 BulletIndex = INDEX_NONE;

	// 目标快照索引
	int32 TargetIndex = INDEX_NONE;

	// 命中点
	FVector HitLocation = FVector::ZeroVector;

	// 命中法线(目标中心指向命中点)
	FVector HitNormal = FVector::UpVector;

	// 受击盒标签(命中快照条目携带)
	FGameplayTag HitBoxTag;
};



// 异步计算Job的桶条目(GameThread拷贝提交,worker线程只读基准+读写副本)
struct FBXProjectileComputeBucketEntry
{
	// 桶种类标签
	FGameplayTag BucketType;

	// 烘焙配置副本(避免worker访问主容器)
	FBXProjectileBakedConfig Baked;

	// 子弹模拟数据副本(worker积分写,收割时按基线守卫回写)
	TArray<FBXProjectileSimData> Bullets;

	// 提交时运行状态基线(收割守卫:主容器状态与基线一致才回写,在途期间被GT事件改动的子弹丢弃)
	TArray<EBXProjectileState> BaseStates;

	// 提交时弹簧偏移基线(贝塞尔重建增量=结果弹簧偏移-基线,收割时合并入主容器弹簧)
	TArray<FVector> BaseSpringOffsets;

	// 该桶是否收集数学命中候选(bLocalDetect && bMathCheck)
	bool bCollectMath = false;
};



// 异步计算Job(GameThread构建拷贝提交,worker线程消费,全POD无锁队列传输)
struct FBXProjectileComputeJob
{
	// 步序号(递增,结果配对与流水线深度观测)
	uint64 StepSerial = 0;

	// 固定步长(秒)
	float FixedStep = 0.033334f;

	// 本端是否权威(客户端代劳模式逐子弹过滤检测职责)
	bool bAuthority = false;

	// 桶条目(空则Job无效)
	TArray<FBXProjectileComputeBucketEntry> BucketEntries;

	// 目标快照表副本(worker只读,禁止解引用Target)
	TArray<FBXProjectileTargetSnapshot> TargetSnapshots;
};



// 异步命中候选(跨步有效,以ID与弱引用定位,不依赖提交步的容器索引)
struct FBXProjectileAsyncHitCandidate
{
	// 子弹ID
	int64 ProjectileID = 0;

	// 桶种类标签
	FGameplayTag BucketType;

	// 命中目标弱引用(worker仅拷贝,GameThread收割时解引用复查)
	TWeakObjectPtr<AActor> Target;

	// 目标UID(命中去重)
	uint32 TargetUID = 0;

	// 命中点
	FVector HitLocation = FVector::ZeroVector;

	// 命中法线
	FVector HitNormal = FVector::UpVector;

	// 受击盒标签
	FGameplayTag HitBoxTag;
};



// 异步计算结果的桶条目(worker产出,GameThread收割回写)
struct FBXProjectileComputeBucketResult
{
	// 桶种类标签
	FGameplayTag BucketType;

	// 积分后的子弹模拟数据(收割时选择性回写数值字段)
	TArray<FBXProjectileSimData> Bullets;

	// 提交时运行状态基线(自Job搬运,收割守卫用)
	TArray<EBXProjectileState> BaseStates;

	// 提交时弹簧偏移基线(自Job搬运,弹簧增量合并用)
	TArray<FVector> BaseSpringOffsets;
};



// 异步计算结果(worker聚合产出,GameThread收割结算)
struct FBXProjectileComputeResult
{
	// 步序号(与Job配对)
	uint64 StepSerial = 0;

	// 桶结果条目
	TArray<FBXProjectileComputeBucketResult> BucketResults;

	// 数学命中候选(全桶聚合,收割时幂等复查)
	TArray<FBXProjectileAsyncHitCandidate> Candidates;
};
