# BattleX 子弹系统设计方案

> 高性能 · 服务器权威 · 双端一致模拟 · 大规模 Niagara 表现
> 仓库根：`BattleX/`  ·  初版 2026-08-19  ·  更新 2026-08-20（客户端代劳命中检测 + 长寿命快照弹簧矫正 + 异步计算驻留线程管线）

---

## 目录

1. [设计目标与约束](#1-设计目标与约束)
2. [核心决策](#2-核心决策)
3. [架构分层](#3-架构分层)
4. [数据定义：UBXProjectileAsset](#4-数据定义ubxprojectileasset)
5. [运行时结构与分桶](#5-运行时结构与分桶)
6. [运动模型](#6-运动模型)
7. [贝塞尔移动](#7-贝塞尔移动)
8. [碰撞检测](#8-碰撞检测)
9. [Niagara 表现层](#9-niagara-表现层)
10. [网络同步](#10-网络同步)
11. [Late Join 投影重建](#11-late-join-投影重建)
12. [配置项](#12-配置项)
13. [文件规划与集成](#13-文件规划与集成)
14. [已确认决策记录](#14-已确认决策记录)
15. [实施阶段划分](#15-实施阶段划分)

---

## 1. 设计目标与约束

### 设计目标

| 目标 | 实现手段 |
|---|---|
| **大规模** | 无 Actor 化：纯 POD 数据 + 按类型分桶连续内存 + Niagara 批量渲染 |
| **高性能** | 固定步长累积 + 并行运动积分 + 曲线烘焙 LUT + 池化表现资源 |
| **双端一致** | 确定性模拟（相同输入 + 相同算法 + 相同数据 = 相同轨迹）+ 服务器权威命中 |
| **通用性** | 直线 / 跟踪 / 贝塞尔三模式互斥配置，DataAsset 按 GameplayTag 定义与查找 |
| **数据与逻辑分离** | Asset（配置）/ SimData（状态）/ Solver（纯函数）/ 表现层（只读数组）四层解耦 |

### 约束

- 子弹不生成 AActor/Component，全部由 `UBXProjectileManager` 统一持有与更新
- 命中判定与伤害投递为服务器权威；客户端模拟仅为表现，不产生逻辑结果（客户端代劳检测模式下客户端可先行预测结算表现，但权威结论以服务器校验为准，§8.4）
- 表现层只消费模拟层产出的数组，Niagara/音效可整体替换不影响模拟与同步
- `Source/BattleX/Unit/` 下的空壳文件（BXProjectile/BXMissile/BXUnit 等）为未来"单位实体"体系预留，本系统不启用、不删除

---

## 2. 核心决策

| 决策 | 内容 | 理由 |
|---|---|---|
| 无 Actor 化 | 子弹是 Manager 内的纯 POD 数据 | 数千发并发的性能根基；数据与逻辑分离的前提 |
| 确定性模拟 + 服务器权威 | 客户端模拟仅表现，命中/伤害只信服务器 | 不追求 lockstep 级 bit 一致（浮点/物理状态跨机不可保证），而是输入相同 + 算法相同 → 轨迹收敛，正确性由服务器兜底 |
| 客户端代劳命中检测 | `bServerCollisionCheck=false` 时由发射客户端执行检测并上报，服务器只做合法性校验（§10.4） | 与技能碰撞上报同模式，省 DS 检测开销；连接归属 + 筛选器 + 穿透去重三重校验兜底 |
| 长寿命快照弹簧矫正 | 服务器定时下发快照，客户端临界阻尼弹簧平滑收敛浮点误差（§10.5） | 浮点误差随寿命累积不可逆；快照锚定权威位置，弹簧保证渲染无跳变 |
| 按类型分桶 | 同类型子弹连续排列，共享烘焙 LUT 与 Niagara 组件 | 缓存友好、并行分片对齐、Niagara 批量提交按桶进行 |
| 固定步长 | 默认 1/30s，`UBXSettings` 可配置 | 与帧率解耦，双端步进序列一致；碰撞用线段 Sweep，步长增大不漏检，仅轨迹弯曲采样精度降低 |
| 曲线烘焙 LUT | Asset 加载时将速度/转向曲线烘焙为等间隔查找表 | 运行时 O(1) 查表；双端 Asset 相同 → LUT 相同，确定性的一环 |

**复用现有设施**：`FBXCStrategy/FBXCFilter`（碰撞筛选）、`UBXCollisionLibrary`（碰撞过滤逻辑）、`UBXFunctionLibrary::Math`（SegmentToXxx 距离函数）、`FBXRemapFloatCurve`（曲线）、NativeGameplayTags、`GetUniqueID + BXMakeSyncID`（同步 ID）、技能系统已验证的预测/回滚与投影重建模式、`GetServerWorldTimeMilliseconds`（统一时间域）。

---

## 3. 架构分层

```
┌──────────────────────────────────────────────────────────────┐
│                    配置层 Data (UPrimaryDataAsset)             │
│  UBXProjectileAsset: TypeTag / 生命 / 跟踪 / 曲线 / 重力 /      │
│  碰撞策略 / 贝塞尔控制点 / Niagara与音效资源                     │
│  加载时烘焙: 速度LUT / 转向LUT / 弧长表                          │
└────────────────────────────┬─────────────────────────────────┘
                             │ 加载 · 烘焙
                             ▼
┌──────────────────────────────────────────────────────────────┐
│                  运行时数据 State (纯 POD)                      │
│  ProjectileBucket × N: 按类型分桶                              │
│  TArray<FBXProjectileSimData> 连续内存 · 零 Actor 开销          │
└────────────────────────────┬─────────────────────────────────┘
                             │
                             ▼
┌──────────────────────────────────────────────────────────────┐
│              模拟层 Logic — UBXProjectileManager               │
│  1. Accumulate   固定步长累积（防螺旋钳制）                      │
│  2. Integrate    ParallelFor 并行运动积分（纯 POD + LUT）        │
│  3. Collide      碰撞检测（Math 并行候选 + Physics GameThread）  │
│  4. Commit       命中结算 / 状态流转 / 回收（GameThread）         │
│  5. RenderCommit 数组批量提交 Niagara（GameThread，可隔帧）       │
└────────────────────────────┬─────────────────────────────────┘
                             │ 位置 / 状态数组
                             ▼
┌──────────────────────────────────────────────────────────────┐
│                 表现层 Presentation                            │
│  飞行: 每桶一个 NiagaraComponent · UserArray 批量提交           │
│  创建/命中: 池化 NiagaraComponent + SoundBase                   │
└──────────────────────────────────────────────────────────────┘

┌──────────────────────────────────────────────────────────────┐
│                    同步层 Net（横切模拟层）                       │
│  服务器权威 · 客户端预测发射 · 命中可客户端代劳上报(服务器校验)     │
│  长寿命子弹快照下发 + 临界阻尼弹簧矫正                            │
│  Late Join: 快照投影 + 确定性重放续跑                            │
└──────────────────────────────────────────────────────────────┘
```

---

## 4. 数据定义：UBXProjectileAsset

继承 `UPrimaryDataAsset`（与 `UBXTLAsset` 同级），每种子弹一个实例。

| 分组 | 字段 | 说明 |
|---|---|---|
| 标识 | `FGameplayTag ProjectileType` | 子弹种类标识，Manager 按 Tag 查找 |
| 生命 | `float MaxLifetime` | 最大生命周期（秒） |
| 生命 | `float ResidualTime` | 命中或寿命尽后的表现残留时间（如插在墙上的箭），期间不运动不碰撞 |
| 跟踪 | `bool bHoming` | 是否跟踪目标 |
| 跟踪 | `EBXTargetInvalidPolicy TargetInvalidPolicy` | 目标失效策略：直飞最后已知位置 / 自毁 / 保持当前方向 |
| 速度 | `FRuntimeFloatCurve SpeedCurve` | 定义域为生命时长 [0, MaxLifetime]，值域为速度 cm/s |
| 转向 | `FRuntimeFloatCurve TurnRateCurve` | 定义域为生命时长，值域为转向角速度 deg/s，仅 bHoming 时可编辑 |
| 重力 | `FVector Gravity` | 重力向量（cm/s²），以速度增量方式积分 |
| 碰撞 | `bool bMathCheck` | 是否执行数学判定（注册目标重叠检测，与运动积分同并行阶段），默认开 |
| 碰撞 | `bool bPhysicsCheck` | 是否执行物理检测（场景 Sweep，GameThread），与数学判定独立开关可任意组合（双关=纯表现弹） |
| 碰撞 | `FBXCStrategy PhysicsStrategy` | Physics 模式：ObjectTypes + FBXCFilter 筛选；Math 模式复用其筛选器做 GameThread 过滤 |
| 碰撞 | `EBXProjectileShape BulletShape` | 子弹碰撞形状：点 / 球体 / 长方体（§8.0） |
| 碰撞 | `float BulletRadius` | 球体形状半径（cm），Sweep 与数学判定共用 |
| 碰撞 | `FVector BulletBoxExtent` | 长方体半尺寸（cm），局部 X 沿飞行方向，仅长方体形状 |
| 碰撞 | `int32 MaxPenetrationCount` | 最大穿透次数（命中后继续飞行），1 = 命中即毁 |
| 贝塞尔 | `bool bUseBezier` | 是否贝塞尔移动（与 Homing 互斥） |
| 贝塞尔 | `TArray<FBXBezierControlPoint> BezierControlPoints` | 2~4 个中间控制点，见 §7 |
| 表现 | `TSoftObjectPtr<UNiagaraSystem> FireSystem / FlightSystem / HitSystem` | 创建 / 飞行 / 命中特效 |
| 表现 | `TSoftObjectPtr<USoundBase> FireSound / FlightSound / HitSound` | 创建 / 飞行 / 命中音效 |
| 表现 | `float FlightSoundRange` | 飞行音效聚合阈值相关（§9.3） |
| 附带 | `FInstancedStruct ContextTemplate` | 发射方可附带的上下文数据模板，命中事件原样带回（结构需为可序列化 USTRUCT） |

目标形状不配置在 Asset 上：注册目标时从其 `UBXShapeComponent` 的受击信息（`ShapeInformations`）逐盒展开，一个多受击盒目标展开为多条目标快照，每条携带 `HitBoxTag` 标识命中部位（§8.1）。

### Tag 查找

Manager 初始化时扫描配置目录（`UBXSettings` 配置的路径，默认 `/Game/BattleX/Projectile`）收集 `Tag → TSoftObjectPtr<UBXProjectileAsset>` 注册表（与 `TimelineAssetMap` 同模式）。子弹类型命名空间约定为 `BXProjectile.*`（如 `BXProjectile.Arrow.Basic`），具体种类 Tag 由项目内容侧配置，事件 Tag 走 NativeGameplayTags（§13）。

### 曲线烘焙

Asset 加载完成后（异步加载回调中）将 SpeedCurve / TurnRateCurve 按 `ProjectileCurveBakeSamples`（默认 128）等间隔烘焙为 LUT；贝塞尔弹不烘焙曲线（用弧长表，§7）。运行时采样即数组寻址，双端因 Asset 一致而 LUT 一致。

---

## 5. 运行时结构与分桶

```cpp
// 模拟数据（纯 POD，无 UObject 引用，可安全多线程读写）
struct FBXProjectileSimData
{
    int64 ProjectileID;          // 跨端一致运行时 ID（GetUniqueID + 高3位Initiator）
    FVector PrevLocation;        // 上一步位置（碰撞线段起点）
    FVector Location;            // 当前模拟锚点位置
    FVector Velocity;            // 当前速度
    float ElapsedTime;           // 已飞行时间
    float ResidualTimer;         // 残留倒计时
    EBXProjectileState State;    // Active / Residual / Dead(内部)
    uint32 InstigatorUID;        // 始作俑者UID(排除自身命中)
    TArray<uint32> HitTargetUIDs; // 穿透去重
    int32 PenetrationCount;      // 已穿透计数
    FBXProjectileFireContext FireContext;  // 发射上下文(Owner/Instigator/方向/附带数据)
    TWeakObjectPtr<UBXProjectileComponent> Carrier;  // 发射载体组件(RPC通道)
    FBXBezierRuntimeContext Bezier;  // 控制点缓存 + 弧长表 + 当前参数 u

    // 弹簧矫正(渲染与检测位置 = 模拟位置 + SpringOffset,§10.5)
    FVector SpringOffset;        // 弹簧矫正偏移(客户端,服务器恒零)
    FVector SpringVelocity;      // 弹簧速度
    FVector SpringTarget;        // 弹簧矫正目标(快照外推偏差,0代表期望偏移归零)

    bool bLocalDetectable;       // 本端是否对该子弹执行命中检测(GameThread解析,§8.4)
    bool bLocalHitReported;      // 客户端代劳已本地结算标记(组播去重)
};

// 目标快照（Tick 开始时 GameThread 拷贝，供并行阶段只读；多受击盒目标逐盒展开为多条）
struct FBXProjectileTargetSnapshot
{
    TWeakObjectPtr<AActor> Target;   // 目标弱引用(GameThread复查)
    uint32 TargetUID;                // 目标UID(穿透去重/排除自身)
    FGameplayTag HitBoxTag;          // 受击盒标签(命中部位)
    FVector Location;
    FQuat Rotation;
    FVector ShapeExtent;             // 按形状类型解释
    EBXShapeType ShapeType;          // 球/胶囊/盒
    bool bValid;                     // 目标失效标记
};

// 分桶
struct FBXProjectileBucket
{
    UBXProjectileAsset* Asset;
    FBXProjectileBakedConfig Baked;       // LUT、钳制后的参数、碰撞形状
    TArray<FBXProjectileSimData> Bullets; // 连续内存，swap-remove 回收
    TArray<int64> BulletIDs;              // 与Bullets同索引的ID表(索引校验)
    int32 SweepCursor;                    // Sweep预算轮转游标
    UNiagaraComponent* FlightComponent;   // 每桶一个常驻飞行渲染组件
    TArray<FVector> ScratchPositions;     // Niagara提交暂存(UserArray)
    TArray<FVector> ScratchVelocities;
    TArray<float> ScratchAges;
};
```

**Manager**：`UBXProjectileManager : UBXManager, FTickableGameObject`，注册进 `Config/DefaultBattleX.ini` 的 ManagerClasses。持有 `TMap<FGameplayTag, FBXProjectileBucket> Buckets`、`TMap<int64, FBXProjectileIndex> IDToBullet`（ID → 桶Tag + 槽位）、目标注册表（`TMap<AActor*, int32>` + 快照数组）。

**Tick 流水线**：

1. **Accumulate**：累积帧时间 → N 个固定步；每帧步数上限 `ProjectileMaxStepsPerFrame`（防螺旋），超出丢弃（子弹时间轻微放慢但不雪崩）；每固定步执行 StepSimulation
2. **StepSimulation（每固定步，`bProjectileAsyncCompute=false` 或提交失败时本地执行）**：
   - **Snapshot**：GameThread 将注册目标（含多受击盒逐盒展开）的 Transform/形状拷贝进快照数组，并解析每发子弹的 `bLocalDetectable`（检测职责分流，§8.4）
   - **Integrate（ParallelFor）**：按桶分片并行运动积分。调用 `FBXProjectileSolver` 纯函数，输出新位置/速度；`bMathCheck` 开启时数学判定在本阶段并行完成（点/球体子弹 SegmentToXxx 距离判定、盒形子弹 SweptBoxToXxx 扫掠判定，§8.0），只产出命中候选索引；模拟期间新发射的子弹进入挂起区，步末合并入桶
   - **Collide/Commit（GameThread）**：数学候选复查结算（目标有效性 + 筛选器 + 穿透去重）；`bPhysicsCheck` 开启时按预算轮转对线段做 Sweep + `FBXCFilter` 过滤（§8.2）；命中事件广播、状态流转（→Residual）、残留计时结束回收
3. **UpdateSprings（每帧）**：客户端弹簧矫正步进（表现层平滑，非固定步长，§10.5）
4. **UpdateSnapshots（每帧，服务器）**：长寿命子弹快照定时收集与组播（§10.5）
5. **RenderCommit（每帧）**：每桶 `SetNiagaraArrayVector` 批量提交（位置叠加弹簧偏移）

### 5.1 异步计算管线（bProjectileAsyncCompute，默认开启）

积分与数学判定移交驻留计算线程 `FBXProjectileComputeWorker`（FRunnable，非 Actor 化），GameThread 只做提交与收割，命中结算延后一帧：

```cpp
【GameThread 帧 N】
1. HarvestComputeResults(先收割):
   Drain 输出队列 → 逐桶数值回写(按ID定位,基线守卫:主容器State与提交时一致才回写,
   在途期间被GT事件改动的子弹丢弃回写) → 候选结算(InternalResolveAsyncCandidate幂等复查:
   ID定位/State复查/目标有效/筛选器/穿透去重) → 涉及桶物理Sweep+生命周期 → 合并挂起区
2. TrySubmitComputeJob(后提交):
   BuildTargetSnapshots(GT) → 全桶轻量计算副本入Job(CopyComputeFieldsTo仅拷贝积分/判定/
   回写字段,跳过ContextData实例克隆与GT专用字段,消除每步每发堆分配;每桶记录
   BaseStates/BaseSpringOffsets基线) → SPSC队列入队唤醒worker

【Worker 线程(生产者)】
3. ProcessJob: 全桶统一切片(64发/片,跨桶汇总分片解决桶间串行) → ParallelFor分发
   (积分+数学判定,复用Solver纯函数) → 分片候选聚合为异步形态(以ID+弱引用定位,
   不依赖提交步索引) → Result入SPSC输出队列

【协议要点】
- 顺序协议:每帧先收割后提交(收割更新终态/去重集后才拷贝新Job)
- 深度≤1:在途时本帧不步进不提交(本地推进与收割回写会竞态),子弹按worker完成速率推进
- 弹簧增量合并:worker期间贝塞尔重建偏差(结果弹簧偏移-基线)叠加进主容器当前弹簧,
  主容器弹簧已被UpdateSprings步进不可覆盖
- 幂等复查兜底全部异步竞态:在途时被物理拦截/显式停止/预测否认/组播命中的子弹,
  其过期候选或过期回写自动丢弃
- 惰性创建+空闲自回收:首个Job到达起线程,连续10s无Job撤线程,无弹幕时零线程存在
- 自回收闭环协议:worker空闲超时先置消亡标记再终查队列,非空且未被Kill等待则复活消费;提交方入队后确认
  线程存活,见消亡标记则重启线程消费滞留Job(双侧闭环,防止Job滞留导致IsBusy永真冻结模拟)。
  Stop()在Kill(true)等待前置停机标记,复活分支见标记放弃消费直接退出——防复活消费与GT Join互相等待死锁
- 优雅停机:Deinitialize请求退出+Join+清空队列(节点值逐个析构)
- 降级路径完整保留:关闭开关或提交失败时StepSimulation本地同步并行(原逻辑不变)
```

**重入安全**：命中事件回调可能同步发射新子弹（连发链）导致容器变更——模拟期间（bInSimulation）新子弹一律入 `PendingProjectiles` 挂起区（TUniquePtr 容器，地址稳定），步末合并入桶，禁止迭代中 Add；命中结算按候选索引批处理，容器结构在回调前稳定。

---

## 6. 运动模型

每固定步求解顺序（三模式互斥：bUseBezier > bHoming > Line）：

```
t += FixedStep
Speed = SpeedLUT.Sample(t)                     // 定义域 [0, MaxLifetime] 钳制

方向求解:
  Bezier: 位置 = 曲线求值(弧长推进)，方向 = 切线   // 忽略重力与转向
  Homing: 期望方向 = normalize(目标快照位置 - 位置)
          当前方向按 TurnRateLUT.Sample(t) 的最大角速度向期望方向旋转
          目标失效则按 TargetInvalidPolicy 处理
  Line:   方向不变

Velocity = 方向 * Speed + Gravity * FixedStep  // Line / Homing 模式
Location += Velocity * FixedStep
```

- 转向旋转用四元数 Slerp 限定最大角步进，避免欧拉角万向节问题
- 步长 1/30 下高速直线弹每步位移可能较大，碰撞检测用整段线段（Prev→Location）保证不漏检
- 贝塞尔模式速度由弧长表控制（§7），重力与转向曲线对其无效

---

## 7. 贝塞尔移动

### 7.1 控制点定义

`FBXBezierControlPoint`：`float Percent`（0~1）+ `FVector Offset`。局部坐标系：

- X 轴 = normalize(TargetPoint - StartPoint)
- Z 轴 = 发射者 UpVector 对 X 轴正交化
- Y 轴 = Z × X

中间控制点世界坐标 = `Start + Percent * (Target - Start) + Offset.Y * Y轴 + Offset.Z * Z轴`（Offset.X 沿线段方向附加偏移）。端点 P0 = Start、PN = Target，曲线为 2~4 个控制点的贝塞尔（N 个中间点 → N+1 次）。

### 7.2 弧长参数化

曲线建立时按 u 等间隔采样 `ProjectileBezierArcSamples`（默认 64）段构建弧长表（累积长度数组）。每步用 `Speed * FixedStep` 换算 Δs → 反查 u 递进 → 位置与切线。保证速度可控、双端一致、不因曲线陡峭段出现速度突变。

### 7.3 目标点变化

目标位置移动超过 `ProjectileTargetRefreshDistance`（默认 100cm）阈值时触发重建：重算控制点、重建弧长表（从当前 u 映射到新表）。非每步重建，避免高频抖动。

### 7.4 脱离处理（投影 + 临界阻尼弹簧收敛）

新曲线构建后子弹实际位置可能脱离曲线，处理：

1. 在新曲线上找距当前实际位置最近的参数 u*（旧 u 附近局部搜索 + 全局粗采样兜底）
2. 脱离偏差记入弹簧矫正偏移：`SpringOffset += ActualLocation - CurvePoint(u*)`，u 推进从 u* 继续（重建瞬间渲染位置不变）
3. 后续每步：渲染与检测位置 = `CurvePoint(u) + SpringOffset`，弹簧目标为 0，临界阻尼弹簧平滑收敛消除偏差（§10.5，无超调）
4. 收敛期间碰撞检测使用含偏移的实际位置，确保正确性

无跳变、实现简单、成本为一次最近点搜索（仅在重建时发生）。使用者仍可通过配置不同控制点（Percent/Offset）让轨迹呈现多样性，矫正只影响贴合过程不影响曲线形状。

**与初版差异**：初版使用独立的 `ErrorOffset` 指数衰减（`ProjectileBezierErrorDecayRate`），已统一收编进弹簧偏移系统——贝塞尔重建偏差、Late Join 重放偏差（§11）、快照矫正（§10.5）三处共用一套弹簧状态，行为一致、参数统一（`ProjectileSpringFrequency`）。

---

## 8. 碰撞检测

### 8.0 子弹碰撞形状

子弹自身碰撞体由 Asset 定义（`EBXProjectileShape`）：

| 形状 | 数学判定（单位命中） | 物理检测（场景 Sweep） |
|---|---|---|
| 点 `BS_Point` | 线段与目标形状相交判定（膨胀 0，精确） | 线检测（Line trace） |
| 球体 `BS_Sphere` | 线段按 `BulletRadius` 膨胀（Minkowski 精确） | `MakeSphere(BulletRadius)` |
| 长方体 `BS_Box` | 整盒沿线段扫掠判定（局部 X 对齐飞行方向）：目标盒 = 15 轴 SAT 区间交集（精确）、目标球 = 球心反向轨迹距离转化（精确）、目标胶囊 = SAT 粗筛 + 凸距离函数三分搜索（精确） | `MakeBox(BulletBoxExtent)`，旋转对齐飞行方向（局部 X 朝前） |

盒形子弹的数学判定不再使用膨胀近似：扫掠盒 vs 盒经 15 轴 SAT 把"存在 t∈[0,1] 双盒重叠"转化为各轴重叠时段的区间交集（每轴投影差是 t 的线性函数）；扫掠盒 vs 球经等价转化（盒沿线段扫掠命中球 ⟺ 球心沿反向线段到静止盒距离 ≤ 半径）直接复用库的精确距离函数；扫掠盒 vs 胶囊经距离归约（胶囊 = 轴线段 ⊕ 球，命中 ⟺ 存在 t 使轴线段反向平移 t·扫掠量后到盒距离 ≤ 半径），f(t) = dist(t·扫掠量, 轴线段 ⊖ 盒) 为凸函数，7 轴 SAT 粗筛拒绝后三分搜索最小值（GTE 库无胶囊-盒查询，此为其胶囊问题归约为"线段距离+半径"惯例的扫掠推广）。判定函数位于 `UBXFunctionLibrary::SweptBoxToSphere / SweptBoxToCapsule / SweptBoxToBox`（纯函数可并行、固定迭代次数保证确定性）。

### 8.1 数学判定（逻辑目标命中，`bMathCheck`）

- 目标（单位/角色）经 `RegisterProjectileTarget(AActor*)` 注册进 Manager；快照从目标 `UBXShapeComponent` 的受击信息逐盒展开——一个多受击盒目标展开为多条快照，每条携带 `HitBoxTag`（命中部位标签，无形状组件的目标用默认整包形状）
- **与运动积分同并行阶段执行**（ParallelFor 分片内逐弹判定）：对 `PrevLocation→Location` 线段（叠加弹簧偏移，与渲染位置一致）判定——点/球体子弹按膨胀量做 `SegmentToSphere / SegmentToCapsule / SegmentToBox` 距离判定，盒形子弹整盒扫掠走 `SweptBoxToSphere / SweptBoxToCapsule / SweptBoxToBox`（§8.0）
- 完全确定性、可并行、双端行为一致；GameThread 仅做候选复查结算（目标有效性 + 筛选器 + 穿透去重，需访问 UObject）
- 第一版遍历注册目标表（项目规模可控）；进阶加均匀网格 hash 空间划分
- 穿透去重：per-bullet 已命中目标 UID 集合（多受击盒展开后同帧命中同目标多条快照按目标 UID 去重，首个命中的 HitBoxTag 生效）

### 8.2 物理检测（场景交互，`bPhysicsCheck`）

- GameThread 对线段按子弹形状生成 Sweep 几何体（§8.0）做 `SweepSingleByObjectType` + `FBXCFilter` 过滤（复用 `UBXCollisionLibrary::CheckCollisionResult` 逻辑）
- 每帧 Sweep 预算 `ProjectileSweepBudgetPerFrame`（默认 128），超出的子弹标记延迟到下帧检测（预算轮转，避免尖峰；线段跨度累积不漏检）
- 命中 WorldStatic/WorldDynamic（墙/障碍）→ 弹道结束（反弹留作后续扩展，结构预留 EBXProjectileHitType）
- 数学/物理为独立开关：双开时先数学结算逻辑目标，命中终态后物理检测自然跳过；双关为纯表现弹（仅飞行不检测，如装饰性弹幕）

### 8.3 命中结算（权威端）

- 服务器（或代劳客户端，§8.4）命中 → 状态流转（穿透计数耗尽或 MaxPenetration=1 → Active→Residual 或直接销毁）→ `MulticastProjectileHit` 全端表现 + `BXEvent.Projectile.Hit` 事件广播（`FBXProjectileHitPayload`：ID、命中点、法线、目标、受击盒标签、Instigator/Triggerer、Context 原样带回）
- 伤害由现有战斗系统订阅事件处理，子弹系统不感知伤害数值
- 客户端预测命中只提前播本地表现，权威确认前不产生逻辑结果

### 8.4 检测职责分流（客户端代劳）

`bServerCollisionCheck=false`（默认）时 DedicatedServer 不执行命中检测（省开销），检测职责按端分流：

| 端 | 检测范围 |
|---|---|
| 单机 / Listen Server | 全量检测 |
| DS（`bServerCollisionCheck=true`） | 全量检测 |
| DS（`bServerCollisionCheck=false`） | 不检测，等待客户端上报 |
| 客户端（`bServerCollisionCheck=false`） | **代劳**：仅本连接 Instigator（AutonomousProxy 拥有）的子弹 |

- 客户端代劳命中：本地预测结算（表现/事件/终态流转立即执行）→ `ServerReportProjectileHit` 上报服务器权威校验
- 服务器校验：连接归属（上报组件拥有者与始作俑者同一连接，防替他人伪造）+ 目标有效性 + 筛选器 + 穿透去重；权威字段（类型/Instigator/Triggerer/ContextData）以服务器数据覆写，不信任客户端载荷
- 校验通过 → 服务器权威结算 + `MulticastProjectileHit` 组播；代劳端经 `bLocalHitReported` 去重（跳过重复表现/事件，仅应用权威终态）
- 其他客户端（非发射者）：不检测，等待组播结果

---

## 9. Niagara 表现层

### 9.1 飞行渲染（大规模核心）

- 每桶一个常驻 `UNiagaraComponent`，Niagara 系统读 **User Array 参数**（PositionArray / VelocityArray / AgeArray / ColorArray 等），每帧 `SetNiagaraArrayVector` 批量提交
- GPU 粒子按数组长度 burst 渲染，单组件支撑数千实例
- 进阶（Phase 4）：传 PrevPos/Pos/PrevTime/CurTime 四元组，GPU 内插值实现帧率无关平滑

### 9.2 创建 / 命中特效与音效

- 特效：池化（World 的 NiagaraComponent 池）+ 按生命周期自动回收
- 音效：`PlaySoundAtLocation`（自带并发管理）

### 9.3 飞行音效

大规模逐发循环音不现实，按桶聚合：一个循环 AudioComponent 置于桶内子弹质心，音量随数量缩放；或仅前 N 发挂循环音（Asset 的 FlightSoundRange 可配）。

### 9.4 分离原则

模拟层只产出位置/状态数组；表现层只消费数组。整套 Niagara/音效可替换不影响模拟与同步。

---

## 10. 网络同步

### 10.1 发射链路（复用技能系统已验证模式）

```
自主客户端:  本地预测发射(立即模拟+表现) ──ServerFireProjectile──▶ 服务器
             ◀── ID匹配: 预测弹保留 ── MulticastFireProjectile (Reliable)
             ◀── 校验失败: ClientDenyProjectile → 回滚(移除预测弹与表现)
```

RPC 通道由发射者 Actor 上的 `UBXProjectileComponent` 承载（发射时 GetOrCreate，与技能 SkillComponent 同模式）。

### 10.2 RPC 设计

| RPC | 方向 | 可靠性 | 参数 | 说明 |
|---|---|---|---|---|
| `ServerFireProjectile` | C→S | Reliable | `FBXProjectileSyncHeader` + `FBXProjectileFireContext` | 客户端预测发射后的服务器确认请求 |
| `ClientDenyProjectile` | S→C | Reliable | ProjectileID + DenyReason | 校验失败，预测弹回滚（仅发往发起端） |
| `MulticastFireProjectile` | S→All | Reliable | Header + FireContext | 全端启动模拟；已预测客户端按 ID 匹配保留本地弹 |
| `ServerReportProjectileHit` | C→S | Reliable | `FBXProjectileHitPayload` | 客户端代劳检测命中后上报服务器权威校验（§10.4） |
| `MulticastProjectileHit` | S→All | Reliable | Payload + bTerminal | 全端命中表现；服务器侧同时广播 BXEvent.Projectile.Hit |
| `MulticastStopProjectile` | S→All | Reliable | ProjectileID + Reason | 显式中断（拦截/技能打断）；自然死亡不广播，双端自行模拟一致 |
| `MulticastProjectileSnapshots` | S→All | Unreliable | `FBXProjectileSnapshotBatch` | 长寿命子弹定时位置快照，客户端弹簧矫正（§10.5）；丢包由下周期补 |

- Header：ProjectileID + TypeTag + Initiator + 时间戳；FireContext：Owner/Instigator（Actor 引用）、StartLocation、FireDirection、FireUpVector、Target（可选）、ContextData（InstancedStruct，需可序列化）
- **ProjectileID**：`GetUniqueID` + 高 3 位 Initiator（与 SkillID 同方案），服务器校验 Initiator=Client 且 ID 不存在，防伪造/碰撞
- **请求年龄校验**：`SkillRequestMaxAgeMs` 同款，基于 `GetServerWorldTimeMilliseconds`
- **时间对齐**：Multicast 携带服务器发射时间戳，未预测的客户端按本地服务器时间域估算已飞行时长，从该时刻起模拟；已预测客户端保留本地时间线（预测弹更早、命中权威在服务器，无需强一致）

### 10.3 跟踪目标位置

双端各自读复制后的目标位置（天然 RTT 级偏差）——命中权威在服务器，客户端轨迹只需"接近"不需"一致"。

### 10.4 客户端代劳命中上报（`bServerCollisionCheck=false`）

```
发射客户端(AutonomousProxy):
  本连接Instigator子弹命中(数学判定/物理检测) 
  → 本地预测结算(表现+事件+终态流转, bLocalHitReported=true)
  → ServerReportProjectileHit(Payload)          [Server, Reliable]
       │
       ↓
服务器 HandleServerReportProjectileHit:
  ├─ 子弹存在且Active + 连接归属校验(上报组件Owner与Instigator同连接)
  ├─ 单位命中: 目标有效 + 筛选器 + 穿透去重(HitTargetUIDs)
  ├─ 权威字段覆写(Type/Instigator/Triggerer/ContextData以服务器为准)
  └─ 通过 → 权威结算 + MulticastProjectileHit
       │
       ↓
代劳端收到组播回声: bLocalHitReported去重 → 跳过重复表现/事件,仅应用权威终态
其他客户端: 正常表现+事件
```

与技能碰撞上报（`ServerReportCollisionResults`）同模式，差异：子弹上报携带完整命中载荷（客户端已本地预测结算，服务器校验失败直接丢弃——本地表现已播出，接受单次误差）；物理检测命中的场景命中同样经此通道上报。

### 10.5 长寿命子弹快照矫正（临界阻尼弹簧）

**问题**：确定性模拟不保证 bit 一致——浮点运算顺序差异（并行分片边界、FMA 差异）随步数累积，长寿命子弹（>2s，如贝塞尔抛射、慢速跟踪弹）双端位置偏差会逐渐放大，命中权威在服务器但客户端表现可能明显偏离。

**方案**：服务器定时下发位置快照，客户端用临界阻尼弹簧把偏差平滑收敛到零：

```
服务器 InternalUpdateSnapshots (每 ProjectileSnapshotInterval=0.5s):
  收集 Active 且 ElapsedTime ≥ ProjectileSnapshotMinAge=2s 的子弹
  → {ID, Location, Velocity, ElapsedTime} 批次 + ServerTimestamp
  → 经全局快照通道(挂靠GameState的ProjectileComponent,常驻复制) MulticastProjectileSnapshots  [Unreliable]

客户端 HandleSnapshotMulticast:
  Latency = 当前服务器世界时间 - ServerTimestamp
  ExpectedLocation = Location + Velocity × Latency    (传输延迟外推)
  SpringTarget = ExpectedLocation - 本地模拟Location   (弹簧矫正目标,模拟锚点不动)

客户端 UpdateSprings (每帧):
  ω = 2π × ProjectileSpringFrequency                  (默认 4Hz)
  加速度 = ω²·(SpringTarget - SpringOffset) - 2ω·SpringVelocity   (临界阻尼,无超调)
  渲染/检测位置 = 模拟Location + SpringOffset
  收敛末端(三者均近似零)清零避免永续微抖
```

**设计要点**：

- **模拟锚点不动**：快照矫正不回写模拟位置（模拟状态保持确定性推进），仅叠加表现偏移；数学检测线段、物理 Sweep 线段、Niagara 渲染位置三处统一叠加 `SpringOffset`（服务器恒零，行为无影响）
- **临界阻尼**：阻尼比 ζ=1，无超调不震荡，视觉上是"平滑贴合"而非"跳变/橡皮筋"
- **三源统一**：贝塞尔曲线重建偏差（§7.4）、Late Join 重放锚点偏差（§11）、快照矫正（本节）共用同一套弹簧状态，任何来源的偏差都是"记入偏移、弹簧收敛到零"
- **Unreliable + 周期补**：快照是收敛性数据非事件性数据，丢包下个周期补，无需 Reliable
- **下发门槛**：短寿命子弹（<2s）双端自然收敛域内偏差可忽略，不下发省流量；仅长寿命子弹进入快照通道

---

## 11. Late Join 投影重建

新复制到客户端的对象（迟加入玩家）经复制快照重建进行中的子弹，与技能 `RunningSkillStates` 同模式：

- **通道**：`RunningProjectileStates` 普通 TArray + `COND_InitialOnly`（仅新连接初始同步发送，已有连接零属性流量；飞行中子弹的动态由显式 RPC 维护）
- **服务器维护时机**：组件 PreReplication 从 Manager 活跃桶重建快照（脏标记即时重建：发射/回收结构变更时 Manager 通知载体组件置脏；低频 0.5s 定时兜底数值新鲜度——InitialOnly 仅新连接消费，无需每帧重建）
- **快照结构** `FBXProjectileReplicatedState`：ProjectileID、TypeTag、FireContext、当前 ElapsedTime、State（服务器模拟到当前时刻的最新状态）
- **重建入口** `UBXProjectileManager::RebuildProjectileFromState`：
  1. 按 Tag 找到/创建桶
  2. 从 FireContext 用烘焙 LUT 与固定步长**确定性重放**至快照 ElapsedTime（快速重放：只算位置，不触发命中/表现）
  3. 重放轨迹与快照位置的偏差（目标移动等）以快照为准：位置校正到快照值，偏差记入 `SpringOffset = -Delta`（弹簧平滑收敛，重建瞬间渲染无跳变）
  4. 入桶续跑（Residual 态直接放置残留）
- **客户端 OnRep 差分**：与技能侧一致，旧值对比避免重复重建；服务器侧移除的子弹由残留到期/自然死亡兜底停止

确定性模拟的红利：无需传输轨迹，只传发射参数 + 时间。

---

## 12. 配置项

`UBXSettings`（Config=BattleX）新增：

| 配置 | 默认值 | 说明 |
|---|---|---|
| `ProjectileFixedStep` | 0.033334 | 固定步长（1/30s） |
| `ProjectileMaxStepsPerFrame` | 2 | 每帧步数上限（防螺旋钳制） |
| `ProjectileCurveBakeSamples` | 128 | 速度/转向曲线烘焙采样数 |
| `ProjectileBezierArcSamples` | 64 | 贝塞尔弧长表采样段数 |
| `ProjectileTargetRefreshDistance` | 100.0 | 贝塞尔目标移动重建阈值（cm） |
| `ProjectileSnapshotInterval` | 0.5 | 长寿命子弹快照下发间隔（秒，0 关闭） |
| `ProjectileSnapshotMinAge` | 2.0 | 快照下发的子弹最低存活时长（秒） |
| `ProjectileSpringFrequency` | 4.0 | 快照矫正临界阻尼弹簧自然频率（Hz，越大收敛越快） |
| `ProjectileSweepBudgetPerFrame` | 128 | Physics 模式每帧 Sweep 预算 |
| `bProjectileAsyncCompute` | true | 异步计算管线开关（关闭则 GameThread 同步并行，§5.1） |
| `ProjectileAssetPaths` | `/Game/BattleX/Projectile` | Asset 扫描目录 |

子弹碰撞形状（点/球体/长方体）为 Asset 级配置（§8.0），非全局设置。

---

## 13. 文件规划与集成

```
Source/BattleX/Projectile/
    BXProjectileAsset.h/.cpp        // DataAsset 配置定义 + 曲线烘焙
    BXProjectileStructs.h           // 枚举/SimData/贝塞尔结构/快照/FireContext（纯头文件）
    BXProjectileManager.h/.cpp      // Tick 流水线 / 分桶 / 目标注册 / 命中结算 / 快照下发 / Late Join
    BXProjectileComponent.h/.cpp    // RPC 通道组件（发射/命中上报/组播/复制快照,挂发射者Actor或GameState）
    BXProjectileSolver.h/.cpp       // 纯函数运动求解（积分/LUT采样/贝塞尔弧长/弹簧步进/数学命中判定），无状态可多线程
    BXProjectileComputeWorker.h/.cpp // 异步计算驻留线程（FRunnable+双SPSC队列,惰性创建/空闲自回收/优雅停机,§5.1）
Source/BattleX/Task/
    BXTFireProjectile.h             // 发射 Task（配置:种类Tag/方向类型/枪口偏移/逐目标/附带数据）
    BXTPFireProjectile.h/.cpp       // 发射 Processor（解析Task配置→组装FireContext→FireProjectile）
```

- `BattleX.Build.cs`：PrivateDependencyModuleNames 含 `"Niagara"`
- `Config/DefaultBattleX.ini`：`+ManagerClasses=/Script/BattleX.BXProjectileManager`
- `BXGameplayTags.h/.cpp`：Native Tag：`BXEvent_Projectile_Fired / Hit / Finished / Denied`
- `Config/Tags/BXGameplayTags.ini`：子弹类型命名空间 `BXProjectile.*` 的基础 Tag 声明（具体种类由内容侧扩展）
- RPC 承载：`UBXProjectileComponent`（已落地，替代初版"经 SkillComponent 转发"方案）——发射时 GetOrCreate 挂到发射者 Actor；快照组播经挂靠 GameState 的常驻通道组件（AlwaysRelevant，全端可见）
- `Source/BattleX/Unit/` 空壳文件为未来"单位实体"体系预留，本系统不启用不删除

---

## 14. 已确认决策记录

| 编号 | 决策 | 结论 |
|---|---|---|
| 1 | 贝塞尔脱离处理 | **投影回曲线 + 弹簧收敛**（初版为独立 ErrorOffset 指数衰减，已统一收编进弹簧系统，§7.4） |
| 2 | 固定步长 | **1/30s，UBXSettings 可配置**（§12） |
| 3 | Late Join 投影重建 | **第一版需要**（§11） |
| 4 | 物理碰撞模式 | **第一版需要**（场景交互如撞墙，§8.2） |
| 5 | Unit/ 空壳文件 | **未来"单位实体"体系预留，不启用不删除** |
| 6 | 命中检测职责 | **客户端代劳 + 服务器校验**：`bServerCollisionCheck=false` 时发射客户端检测上报，服务器做连接归属/筛选器/穿透去重校验后权威结算（§8.4 / §10.4） |
| 7 | 长寿命浮点误差 | **服务器定时快照 + 客户端临界阻尼弹簧矫正**：`ProjectileSnapshotInterval/MinAge/SpringFrequency` 三参数（§10.5） |
| 8 | 误差矫正统一 | **单一弹簧系统**：贝塞尔重建偏差、Late Join 重放偏差、快照矫正共用 SpringOffset/Velocity/Target，删除独立 ErrorOffset 机制 |
| 9 | 子弹碰撞形状 | **点/球体/长方体三形状**（`EBXProjectileShape`）：数学判定点=膨胀0/球体=半径膨胀（均精确）、长方体=整盒扫掠精确判定（目标盒 15 轴 SAT 区间交集、目标球反向轨迹距离转化、目标胶囊 SAT 粗筛+凸距离三分搜索）；物理检测按形状生成 Sweep 几何体（长方体旋转对齐飞行方向）（§8.0） |
| 10 | 目标形状来源 | **从目标 UBXShapeComponent 受击信息展开**（多受击盒目标逐盒展开为多条快照，携带 HitBoxTag），不配置在子弹 Asset 上 |
| 11 | 检测开关 | **数学/物理双独立布尔**（`bMathCheck` / `bPhysicsCheck`）替代互斥枚举：可任意组合（双关=纯表现弹）；数学判定与运动积分同并行阶段（ParallelFor 分片内逐弹判定），GameThread 仅做候选复查结算 |
| 12 | 异步计算管线 | **驻留线程 + 生产者-消费者**（`FBXProjectileComputeWorker`，`bProjectileAsyncCompute` 默认开启）：GT 每帧先收割后提交、流水线深度≤1、全桶统一切片 ParallelFor、按 ID 回写 + 基线守卫 + 幂等候选复查兜底全部在途竞态；关闭或提交失败降级本地同步并行（§5.1） |

---

## 15. 实施阶段划分

### Phase 1：单机可玩 ✅ 已完成

Asset 定义 + 分桶结构 + 直线/重力运动（Solver + LUT 烘焙）+ Math 碰撞 + Niagara 飞行批量渲染 + Manager 注册。

验收：单机发射 1000 发直线弹，帧率稳定，命中事件正确广播。

### Phase 2：网络同步 + 物理碰撞 ✅ 已完成

预测发射/确认/回滚 + Multicast 命中 + Physics Sweep 撞墙终止 + 创建/命中特效音效 + 飞行音效聚合。

验收：PIE 双端，客户端预测弹无可见回滚闪烁，服务器权威命中全端表现一致，撞墙正确终止。

### Phase 3：跟踪 + 贝塞尔 + Late Join ✅ 已完成

Homing（转向 LUT + 失效策略）+ 贝塞尔（弧长表 + 目标重建 + 弹簧收敛贴合）+ 快照投影确定性重放 + BXTFireProjectile Task 集成。

验收：跟踪弹命中移动目标；贝塞尔弹目标位移后平滑贴合；迟加入客户端重建后轨迹与已存在客户端接近。

### Phase 3.5：客户端代劳 + 快照矫正 + 碰撞形状 ✅ 已完成（2026-08-20）

客户端代劳命中检测（检测职责分流 + ServerReportProjectileHit 上报 + 服务器三重校验 + 组播去重）+ 长寿命子弹快照弹簧矫正（快照通道 + 临界阻尼弹簧 + 三源偏差统一）+ 子弹碰撞形状（点/球体/长方体）+ 多受击盒目标展开（HitBoxTag）。

验收：DS + 双客户端，`bServerCollisionCheck=false` 时代劳命中只表现一次；超过 2s 的贝塞尔弹在客户端可见平滑收敛过程；不同形状子弹的物理检测几何体与配置一致。

### Phase 3.6：异步计算管线 ✅ 已完成（2026-08-20）

驻留计算线程（FBXProjectileComputeWorker：双 SPSC 队列 + 惰性创建 + 空闲自回收闭环协议）+ 全桶统一切片 ParallelFor + 按 ID 回写基线守卫 + 异步候选幂等复查 + 弹簧增量合并 + 降级本地同步。

验收：数千发弹幕下 GameThread 帧耗显著下降；关闭开关行为与原同步路径一致；无弹幕 10s 后零驻留线程。

### Phase 4：进阶优化（按需）

GPU 插值平滑表现、目标候选空间划分（均匀网格）、Sweep 预算自适应、反弹扩展、快照按可见性裁剪。

---

> 本文档为实现依据，落地过程中如有设计变更需同步更新。与 `CodeWiki.md` 的关系：CodeWiki 记录代码现状，本文档记录设计意图与决策。
