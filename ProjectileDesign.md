# BattleX 子弹系统设计方案

> 高性能 · 服务器权威 · 双端一致模拟 · 大规模 Niagara 表现
> 仓库根：`BattleX/`  ·  初版 2026-08-19

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
- 命中判定与伤害投递为服务器权威；客户端模拟仅为表现，不产生逻辑结果
- 表现层只消费模拟层产出的数组，Niagara/音效可整体替换不影响模拟与同步
- `Source/BattleX/Unit/` 下的空壳文件（BXProjectile/BXMissile/BXUnit 等）为未来"单位实体"体系预留，本系统不启用、不删除

---

## 2. 核心决策

| 决策 | 内容 | 理由 |
|---|---|---|
| 无 Actor 化 | 子弹是 Manager 内的纯 POD 数据 | 数千发并发的性能根基；数据与逻辑分离的前提 |
| 确定性模拟 + 服务器权威 | 客户端模拟仅表现，命中/伤害只信服务器 | 不追求 lockstep 级 bit 一致（浮点/物理状态跨机不可保证），而是输入相同 + 算法相同 → 轨迹收敛，正确性由服务器兜底 |
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
│  服务器权威命中 · 客户端预测发射 · Multicast 命中表现             │
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
| 碰撞 | `EBXCollisionMode CollisionMode` | Math（数学库重叠判定）/ Physics（物理 Sweep）/ Both |
| 碰撞 | `FBXCStrategy PhysicsStrategy` | Physics 模式：ObjectTypes + FBXCFilter 筛选 |
| 碰撞 | `FBXShapeInformation TargetShape` | Math 模式：目标注册时的形状判定基准 |
| 碰撞 | `float BulletRadius` | 子弹自身碰撞半径（球），Sweep 与数学判定共用 |
| 碰撞 | `int32 MaxPenetrationCount` | 最大穿透次数（命中后继续飞行），1 = 命中即毁 |
| 贝塞尔 | `bool bUseBezier` | 是否贝塞尔移动（与 Homing 互斥） |
| 贝塞尔 | `TArray<FBXBezierControlPoint> BezierControlPoints` | 2~4 个中间控制点，见 §7 |
| 表现 | `TSoftObjectPtr<UNiagaraSystem> FireSystem / FlightSystem / HitSystem` | 创建 / 飞行 / 命中特效 |
| 表现 | `TSoftObjectPtr<USoundBase> FireSound / FlightSound / HitSound` | 创建 / 飞行 / 命中音效 |
| 表现 | `float FlightSoundRange` | 飞行音效聚合阈值相关（§9.3） |
| 附带 | `FInstancedStruct ContextTemplate` | 发射方可附带的上下文数据模板，命中事件原样带回（结构需为可序列化 USTRUCT） |

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
    FVector PrevLocation;        // 上一步位置（碰撞线段起点）
    FVector Location;            // 当前位置
    FVector Velocity;            // 当前速度
    float LifeTime;              // 已飞行时间
    float ResidualTimer;         // 残留倒计时
    EBXProjectileState State;    // Active / Residual
    int64 HomingTargetID;        // 跟踪目标句柄（索引目标快照表）
    int32 PredictFlags;          // 预测标记（IsPredicted 等）
    FBXBezierRuntimeContext Bezier;  // 控制点缓存 + 弧长表 + 当前参数 u + 误差偏移
};

// 目标快照（Tick 开始时 GameThread 拷贝，供并行阶段只读）
struct FBXProjectileTargetSnapshot
{
    FVector Location;
    FQuat Rotation;
    FVector ShapeExtent;         // 按形状类型解释
    uint8 ShapeType;
    bool bValid;                 // 目标失效标记
};

// 分桶
struct FBXProjectileBucket
{
    UBXProjectileAsset* Asset;
    FBXProjectileBakedConfig Baked;       // LUT、钳制后的参数
    TArray<FBXProjectileSimData> Bullets; // 连续内存，swap-remove 回收
    TArray<int32> FreeSlots;              // 可选：回收链
    UNiagaraComponent* FlightComponent;   // 每桶一个常驻飞行渲染组件
    UAudioComponent* FlightSoundComponent; // 飞行音效聚合组件（可选）
};
```

**Manager**：`UBXProjectileManager : UBXManager, FTickableGameObject`，注册进 `Config/DefaultBattleX.ini` 的 ManagerClasses。持有 `TMap<FGameplayTag, FBXProjectileBucket> Buckets`、`TMap<int64, FBXProjectileIndex> IDToBullet`（ID → 桶Tag + 槽位）、目标注册表（`TMap<AActor*, int32>` + 快照数组）。

**Tick 流水线**：

1. **Accumulate**：累积帧时间 → N 个固定步；每帧步数上限 `ProjectileMaxStepsPerFrame`（防螺旋），超出丢弃（子弹时间轻微放慢但不雪崩）
2. **Snapshot**：GameThread 将注册目标的 Transform/形状拷贝进快照数组（并行阶段不触碰 UObject）
3. **Integrate（ParallelFor）**：按桶分片并行运动积分（桶内子弹数 > 阈值时二次分片）。调用 `BXProjectileSolver` 纯函数，输出新位置/速度；Math 碰撞模式的距离判定（SegmentToXxx）在本阶段并行完成，只产出命中候选索引
4. **Collide（GameThread）**：Physics 模式对 `PrevLocation→Location` 线段做 Sweep + `FBXCFilter` 过滤（预算分帧，§8.2）；Math 候选在此验证结算
5. **Commit**：命中事件广播、穿透计数、状态流转（→Residual）、残留计时结束回收
6. **RenderCommit**：每桶 `SetNiagaraArrayVector` 批量提交（可配置隔帧）

**重入安全**：命中事件回调可能同步发射新子弹（连发链）导致容器变更——与 SkillManager 同策略：Tick 先收集到期/命中 ID 快照，回调在容器稳定后统一处理，禁止迭代中 Add。

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

### 7.4 脱离处理（方案 A：投影 + 误差渐进消除）

新曲线构建后子弹实际位置可能脱离曲线，处理：

1. 在新曲线上找距当前实际位置最近的参数 u*（旧 u 附近局部搜索 + 全局粗采样兜底）
2. 记录 `ErrorOffset = ActualLocation - CurvePoint(u*)`，u 推进从 u* 继续
3. 后续每步：逻辑与渲染位置 = `CurvePoint(u) + ErrorOffset`，同时 `ErrorOffset *= exp(-ProjectileBezierErrorDecayRate * FixedStep)`（默认衰减率 10，约 0.2~0.3s 内消除）
4. 误差消除期间碰撞检测使用含误差的实际位置，确保正确性

无跳变、实现简单、成本为一次最近点搜索（仅在重建时发生）。

---

## 8. 碰撞检测

### 8.1 Math 模式（默认，逻辑目标判定）

- 目标（单位/角色）经 `RegisterProjectileTarget(AActor*, Shape)` 注册进 Manager，快照含形状信息
- 并行阶段对 `PrevLocation→Location` 线段 + BulletRadius 与目标形状做 `SegmentToSphere / SegmentToCapsule / SegmentToBox` 判定（`UBXFunctionLibrary` 已有）
- 完全确定性、可并行、双端行为一致
- 第一版遍历注册目标表（项目规模可控）；进阶加均匀网格 hash 空间划分
- 穿透去重：per-bullet 已命中目标 ID 集合

### 8.2 Physics 模式（场景交互）

- GameThread 对线段做 `SweepSingleByChannel/ObjectType` + `FBXCFilter` 过滤（复用 `UBXCollisionLibrary::CheckCollisionResult` 逻辑）
- 每帧 Sweep 预算 `ProjectileSweepBudgetPerFrame`（默认 128），超出的子弹标记延迟到下帧检测（预算轮转，避免尖峰）
- 命中 WorldStatic/WorldDynamic（墙/障碍）→ 弹道结束（反弹留作后续扩展，结构预留 EBXProjectileHitType）
- Both 模式：先 Math 判定逻辑目标，未命中再 Physics 判定场景

### 8.3 命中结算（服务器权威）

- 服务器命中 → 状态流转（穿透计数耗尽或 MaxPenetration=1 → Active→Residual 或直接销毁）→ `MulticastProjectileHit` 全端表现 + `BXEvent.Projectile.Hit` 事件广播（`FBXProjectileHitPayload`：ID、命中点、法线、目标、部位 Tag、Asset Context 原样带回）
- 伤害由现有战斗系统订阅事件处理，子弹系统不感知伤害数值
- 客户端预测命中只提前播本地表现，不产生逻辑结果

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
             ◀── 校验失败: ServerDenyProjectile → 回滚(移除预测弹与表现)
```

### 10.2 RPC 设计

| RPC | 方向 | 参数 | 说明 |
|---|---|---|---|
| `ServerFireProjectile` | C→S Reliable | ProjectileID + TypeTag + FBXProjectileFireContext | 客户端预测发射后的服务器确认请求 |
| `MulticastFireProjectile` | S→All Reliable | ProjectileID + TypeTag + FireContext + ServerFireTimeMs | 全端启动模拟；已预测客户端按 ID 匹配保留本地弹 |
| `ServerDenyProjectile` | S→C Reliable | ProjectileID + DenyReason | 校验失败，预测弹回滚 |
| `MulticastProjectileHit` | S→All Reliable | ProjectileID + HitPoint + HitNormal + Target + PartTag + bTerminal | 全端命中表现；服务器侧同时广播 BXEvent.Projectile.Hit |
| `MulticastStopProjectile` | S→All Reliable | ProjectileID + Reason | 显式中断（拦截/技能打断）；自然死亡不广播，双端自行模拟一致 |

- FireContext：Owner/Instigator（Actor 引用）、StartLocation、FireDirection、Target（可选）、ContextData（InstancedStruct，需可序列化）
- **ProjectileID**：`GetUniqueID` + 高 3 位 Initiator（与 SkillID 同方案），服务器校验 Initiator=Client 且 ID 不存在，防伪造/碰撞
- **请求年龄校验**：`SkillRequestMaxAgeMs` 同款，基于 `GetServerWorldTimeMilliseconds`
- **时间对齐**：Multicast 携带服务器发射时间戳，未预测的客户端按本地服务器时间域估算已飞行时长，从该时刻起模拟（与技能加速补偿同思路）；已预测客户端保留本地时间线（预测弹更早、命中权威在服务器，无需强一致）

### 10.3 跟踪目标位置

双端各自读复制后的目标位置（天然 RTT 级偏差）——命中权威在服务器，客户端轨迹只需"接近"不需"一致"。

---

## 11. Late Join 投影重建

新复制到客户端的对象（迟加入玩家）经复制快照重建进行中的子弹，与技能 `RunningSkillStates` 同模式：

- **通道**：`RunningProjectileStates` 普通 TArray + `COND_InitialOnly`（仅新连接初始同步发送，已有连接零属性流量；飞行中子弹的动态由显式 RPC 维护）
- **服务器维护时机**：组件 PreReplication（每帧、收集前）从活跃桶重建快照（数量大时按可见性/距离裁剪，进阶项）
- **快照结构** `FBXProjectileSnapshot`：ProjectileID、TypeTag、FireContext、当前 LifeTime、State
- **重建入口** `UBXProjectileManager::RebuildProjectileFromSnapshot`：
  1. 按 Tag 找到/创建桶
  2. 从 FireContext 用烘焙 LUT 与固定步长**确定性重放**至快照 LifeTime（快速重放：只算位置，不触发命中/表现）
  3. 跟踪/贝塞尔弹重放时使用当前目标位置（轨迹与历史略有偏差，表现可接受）
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
| `ProjectileBezierErrorDecayRate` | 10.0 | 脱离误差指数衰减率 |
| `ProjectileSweepBudgetPerFrame` | 128 | Physics 模式每帧 Sweep 预算 |
| `ProjectileAssetPaths` | `/Game/BattleX/Projectile` | Asset 扫描目录 |

---

## 13. 文件规划与集成

```
Source/BattleX/Projectile/
    BXProjectileAsset.h/.cpp        // DataAsset 配置定义 + 曲线烘焙
    BXProjectileStructs.h           // 枚举/SimData/贝塞尔结构/快照/FireContext（纯头文件）
    BXProjectileManager.h/.cpp      // Tick 流水线 / 分桶 / 目标注册 / 同步 RPC / Late Join
    BXProjectileSolver.h/.cpp       // 纯函数运动求解（积分/LUT采样/贝塞尔弧长），无状态可多线程
Source/BattleX/Task/
    BXTFireProjectile.h/.cpp        // 发射 Task（技能时间轴内发射，蓝图派生后注册 ini）
```

- `BattleX.Build.cs`：PrivateDependencyModuleNames 增加 `"Niagara"`
- `Config/DefaultBattleX.ini`：`+ManagerClasses=/Script/BattleX.BXProjectileManager`
- `BXGameplayTags.h/.cpp`：Native Tag 新增 `BXEvent_Projectile_Fired / Hit / Finished / Denied`
- `Config/Tags/BXGameplayTags.ini`：子弹类型命名空间 `BXProjectile.*` 的基础 Tag 声明（具体种类由内容侧扩展）
- 子弹系统承载 RPC 的组件归属：`UBXSkillComponent` 已有 Multicast 通道先例；子弹第一版由 Manager 持有（Manager 为 UObject 非 Actor，RPC 需 Actor 通道）——**通过 Owner Actor 上的 SkillComponent 转发 Multicast**（与技能 MulticastPlaySkill 同模式），Server RPC 由发射方 Component 直接调用。落地时若引入独立 `UBXProjectileComponent` 承载，结构预留，Phase 3 决定
- `Source/BattleX/Unit/` 空壳文件为未来"单位实体"体系预留，本系统不启用不删除

---

## 14. 已确认决策记录

| 编号 | 决策 | 结论 |
|---|---|---|
| 1 | 贝塞尔脱离处理 | **A：投影回曲线 + 误差渐进消除**（§7.4） |
| 2 | 固定步长 | **1/30s，UBXSettings 可配置**（§12） |
| 3 | Late Join 投影重建 | **第一版需要**（§11） |
| 4 | 物理碰撞模式 | **第一版需要**（场景交互如撞墙，§8.2） |
| 5 | Unit/ 空壳文件 | **未来"单位实体"体系预留，不启用不删除** |

---

## 15. 实施阶段划分

### Phase 1：单机可玩

Asset 定义 + 分桶结构 + 直线/重力运动（Solver + LUT 烘焙）+ Math 碰撞 + Niagara 飞行批量渲染 + Manager 注册。

验收：单机发射 1000 发直线弹，帧率稳定，命中事件正确广播。

### Phase 2：网络同步 + 物理碰撞

预测发射/确认/回滚 + Multicast 命中 + Physics Sweep 撞墙终止 + 创建/命中特效音效池化 + 飞行音效聚合。

验收：PIE 双端，客户端预测弹无可见回滚闪烁，服务器权威命中全端表现一致，撞墙正确终止。

### Phase 3：跟踪 + 贝塞尔 + Late Join

Homing（转向 LUT + 失效策略）+ 贝塞尔（弧长表 + 目标重建 + 脱离处理 A）+ 快照投影确定性重放 + BXTFireProjectile Task 集成。

验收：跟踪弹命中移动目标；贝塞尔弹目标位移后平滑贴合（A 方案无跳变）；迟加入客户端重建后轨迹与已存在客户端接近。

### Phase 4：进阶优化（按需）

GPU 插值平滑表现、目标候选空间划分（均匀网格）、Sweep 预算自适应、反弹扩展、快照按可见性裁剪。

---

> 本文档为实现依据，落地过程中如有设计变更需同步更新。与 `CodeWiki.md` 的关系：CodeWiki 记录代码现状，本文档记录设计意图与决策。
