# BattleX 技能同步框架设计方案

> 高性能动作游戏技能系统 · 网络同步与运行时架构
> 仓库根：`BattleX/`  ·  文档生成日期：2026-08-13

---

## 目录

1. [设计目标与约束](#1-设计目标与约束)
2. [架构分层](#2-架构分层)
3. [核心数据结构](#3-核心数据结构)
4. [SkillID 设计](#4-skillid-设计)
5. [播放流程与网络路径](#5-播放流程与网络路径)
6. [性能优化策略](#6-性能优化策略)
7. [冷却职责归属](#7-冷却职责归属)
8. [加速弥补与卡顿保护](#8-加速弥补与卡顿保护)
9. [预测与回滚](#9-预测与回滚)
10. [拷贝次数分析](#10-拷贝次数分析)
11. [技能/BUFF 状态复制与重建（Late Join）](#11-技能buff状态复制与重建late-join)

---

## 1. 设计目标与约束

### 设计目标

| 目标 | 实现手段 |
|---|---|
| **低延迟反馈** | 客户端预测启动 + 服务器确认/回滚 |
| **低带宽占用** | 单一 RPC 合并 Header + Payload，按需广播 |
| **零冗余拷贝** | MoveTemp 移动语义 + && 重载 + 统一数据载体 |
| **职责清晰** | Component 管私有状态，Manager 管全局实例，TLManager 管执行管线 |
| **抗卡顿** | 加速弥补带卡顿保护，避免单帧跳过大量 KeyFrame |

### 约束

- 技能输入字段不固定（释放位置/朝向/瞄准/锁定等），需变长容器承载
- Task 执行链路由 `UBXTLManager` 统一维护，技能层复用而非重写
- 冷却等 Actor 私有状态不应由全局单例 Manager 持有

---

## 2. 架构分层

```
┌─────────────────────────────────────────────────────────┐
│               UBXSkillComponent (Actor级)                │
│  职责: BP友好API / RPC收发 / 冷却(私有) / OwnedSkillIDs  │
└──────────────────────────┬──────────────────────────────┘
                           │  Payload (引用/移动)
                           ▼
┌─────────────────────────────────────────────────────────┐
│                UBXSkillManager (全局单例)                │
│  职责: 技能实例生命周期 / 预测框架 / 加速弥补 / 释放校验   │
│  持有: SkillRTDatas (TMap<SkillID, FBXSkillRuntimeData>) │
└──────────────────────────┬──────────────────────────────┘
                           │  TLRunTimeData (引用)
                           ▼
┌─────────────────────────────────────────────────────────┐
│                UBXTLManager (全局单例)                   │
│  职责: Task执行管线 / KeyFrame触发 / Section生命周期      │
│  复用: StartTimelineSections / InternalUpdateTimeline    │
└─────────────────────────────────────────────────────────┘
```

### 分层原则

- **Component 层**：持有 Actor 私有状态（冷却、OwnedSkillIDs），提供 BP API，承载 RPC。不持有技能运行时数据。
- **Manager 层**：持有全局技能实例（`SkillRTDatas`），管理预测/加速/校验等跨 Actor 逻辑。
- **TLManager 层**：提供 Task 执行管线的公共接口（`StartTimelineSections`、`InternalUpdateTimeline`），供 SkillManager 和直接 PlayTimeline 路径复用。

---

## 3. 核心数据结构

### 3.1 FBXSkillSyncHeader（同步头）

所有 RPC 共用的轻量元数据，跨端一致。

```cpp
USTRUCT(BlueprintType)
struct FBXSkillSyncHeader
{
    int64 SkillID = 0;              // 运行时唯一ID(含发起端标识)
    int32 SkillAssetID = 0;         // 技能资源ID(UBXTLAsset.ID)
    EBXSyncInitiator Initiator;     // 发起端类型(Client/Server)
    int64 ClientTimestamp = 0;      // 客户端上传时间戳(毫秒,服务器世界时间域)
    int64 ServerTimestamp = 0;      // 服务器接收时间戳(毫秒,服务器世界时间域,回传用)
};
```

**时间戳域约定**：所有网络时间戳统一通过 `UBXFunctionLibrary::GetServerWorldTimeMilliseconds` 获取 —— 服务器/单机返回本地世界时间，客户端返回经引擎时间同步（`AGameStateBase::GetServerWorldTimeDelta`）校正的估算值。**禁止跨机器直接比较本地墙钟**（`FDateTime::UtcNow`），玩家机器时钟偏差会导致校验误杀与加速量失真。

### 3.2 FBXSkillSyncPayload（同步载荷 / 施放上下文）

**统一数据载体**，兼作施放上下文与 RPC 传输体，消除 Context 与 Payload 的字段冗余。

```cpp
USTRUCT(BlueprintType)
struct FBXSkillInputEntry
{
    FGameplayTag Tag;               // 输入标签(BXSkillInput.*)
    FInstancedStruct Value;         // 输入数据
};

USTRUCT(BlueprintType)
struct FBXSkillSyncPayload
{
    AActor* Instigator = nullptr;   // 始作俑者
    AActor* Triggerer = nullptr;    // 触发者
    int64 ClientTimestamp = 0;      // 客户端上传时间戳
    TArray<FBXSkillInputEntry> InputDatas;  // 所有技能输入(释放位置/朝向/瞄准/锁定等)
};
```

**设计要点**：
- 输入字段变长，通过"标签+数据"条目承载，Task 统一通过 Tag 查询。
- **RPC 传输形态为条目数组而非 TMap**：UE 网络栈不支持 TMap 作 RPC 参数；`PlaySkillWithInputData` 的 BP 入参仍为 TMap（BlueprintCallable 允许），组件内逐条移动转换，无深拷贝。
- 不再单独存储 `LockParts`、`AimLocation` 等字段，全部通过 `InputDatas` 传递，数据来源唯一。

### 3.3 FBXSkillRuntimeData（技能运行时数据）

内嵌 Timeline 运行时数据以复用 Task 执行链路。

```cpp
USTRUCT(BlueprintType)
struct FBXSkillRuntimeData
{
    UBXSkillAsset* SkillAsset = nullptr;
    int64 SkillID = 0;
    FBXTLRunTimeData TLRunTimeData;     // 复用Task执行链路
    EBXPredictState PredictState;       // 预测状态
    float PredictStartTime;             // 预测超时清理用
    int64 ClientTimestamp = 0;
    int64 ServerTimestamp = 0;
    EBXSyncInitiator Initiator;
    float AccelerateRemainTime = 0.0f;  // 加速剩余时间
    float AccelerateRate = 1.0f;        // 加速倍率
    bool bEarlyFinish = false;
};
```

---

## 4. SkillID 设计

SkillID 为 64 位整数，由 RawID（低 61 位）和 Initiator（高 3 位）组成。

```
 63       61 60                                                      0
┌──────────┬─────────────────────────────────────────────────────────┐
│ Initiator│                    RawID                                 │
│ (3 bits) │                  (61 bits)                               │
└──────────┴─────────────────────────────────────────────────────────┘
```

### 关键宏

```cpp
#define BX_SYNC_ID_VALUE_MASK       ((1ULL << 61) - 1)   // 低61位掩码
#define BX_SYNC_ID_INITIATOR_SHIFT  61                    // Initiator起始位
#define BX_SYNC_ID_INITIATOR_MASK   (0x7ULL << 61)       // 高3位掩码
```

### 生成规则

- **RawID**：由 `UBXFunctionLibrary::GetUniqueID` 生成，基于 UTC 时间戳（毫秒精度）+ 循环计数器（1~2,000,000）。
- **Initiator**：`Client`（客户端预测生成）或 `Server`（服务器权威生成），通过 `BXMakeSyncID` 组合。

```cpp
inline int64 BXMakeSyncID(int64 InRawID, EBXSyncInitiator InInitiator)
{
    return (InRawID & BX_SYNC_ID_VALUE_MASK) | ((int64)InInitiator << BX_SYNC_ID_INITIATOR_SHIFT);
}
```

**作用**：预测确认/回滚时通过 Initiator 位区分技能来源，防止客户端预测 ID 与服务器 ID 冲突。跨客户端 RawID 碰撞（两客户端同毫秒同计数器）与伪造 ID 由服务器侧防重校验兜底：`HandleServerPlaySkill` 校验 `BXGetSyncIDInitiator(SkillID) == Client` 且 ID 未存在于 SkillRTDatas；`InternalPlaySkill` 对重复 ID 直接拒绝，不会覆盖正在运行的技能。

---

## 5. 播放流程与网络路径

### 5.1 入口：PlaySkillWithInputData

所有 `PlayXxx` API 收敛到 `PlaySkillWithInputData`，构造 Header + Payload 后按网络角色分流。

```
PlaySkill / PlaySkillWithTarget / PlaySkillWithLocation / PlaySkillWithDirection
                                    │
                                    ▼
                        PlaySkillWithInputData
                                    │
                    ┌───────────────┼───────────────┐
                    │               │               │
              Authority      AutonomousProxy    SimulatedProxy
                    │               │               │
                    ▼               ▼               ▼
            PlaySkill       StartPrediction      不主动释放
            (直接播放)       (客户端预测)          (等Multicast)
                    │               │
                    ▼               ▼
            MulticastPlaySkill  ServerPlaySkill
            (广播给所有客户端)   (上传到服务器)
```

### 5.2 服务器处理：HandleServerPlaySkill

```
ServerPlaySkill RPC
       │
       ▼
HandleServerPlaySkill
       │
       ├─ 查找技能资产
       ├─ SkillID校验(Initiator==Client且未存在,防伪造/防碰撞)
       ├─ 冷却校验 (Component.IsCooldownReady)     ← 私有状态
       ├─ 释放校验 (Manager.ServerValidateRelease)  ← 时间戳+条件
       ├─ InternalPlaySkill (播放)
       ├─ RecordCooldown (Component)                ← 记录冷却
       ├─ OwnedSkillIDs.Add                         ← 服务器登记(EndPlay时可停止)
       ├─ ClientPredictResult (通知发起端)
       ├─ ServerAccelerate (服务器加速弥补)
       └─ MulticastPlaySkill (Reliable广播给所有客户端)
```

`ServerValidateRelease` 的时间戳校验在服务器世界时间域进行：请求年龄（服务器当前 − 客户端时间戳）超过 `UBXSettings::SkillRequestMaxAgeMs`（默认 500ms）拒绝；小幅负值视为客户端时间同步误差放行。

### 5.3 客户端处理：HandleClientPlaySkill

```
MulticastPlaySkill RPC (Reliable)
       │
       ▼
HandleClientPlaySkill (值传递,支持MoveTemp)
       │
       ├─ 查找技能资产
       ├─ InternalPlaySkill(&&重载,移动Payload)    ← 零拷贝
       ├─ OwnedSkillIDs.Add
       └─ ClientAccelerate (客户端加速弥补)
```

Multicast 使用 Reliable：技能可见性关键，丢包无补偿机制会导致远端客户端永远看不到该技能。

### 5.4 碰撞结果上报（bServerCollisionCheck = false 时）

```
客户端(AutonomousProxy) CollisionCheck命中
       │
       ▼
ServerReportCollisionResults(SkillID, TaskFullIndex, DataTag, HitResults)
       │
       ▼
服务器 ReceiveCollisionResults
       │
       ├─ 写入DynamicDatas[(TaskFullIndex, DataTag)]   ← 与客户端本地写入键一致
       ├─ 按TaskFullIndex匹配Task,清除bAwaitingClientCollision
       └─ 广播BXEvent.Collision.Reported
```

- 服务器端 TrackHitBox 跳过实际检测，标记 `bAwaitingClientCollision` + `ServerExtraLifeTimer`（0.15s 兜底）。
- 存在 awaiting Task 的 Section 延迟自然结束（中断/强跳不受影响），保证上报仍能命中 RunningSections 中的 Task。
- 服务器开启 `bServerCollisionCheck` 时忽略客户端上报（服务器权威，防止重复结果）。

---

## 6. 性能优化策略

### 6.1 统一数据载体

合并 `FBXSkillPlayContext` 到 `FBXSkillSyncPayload`，消除 Context 与 Payload 的字段冗余。Payload 既作施放上下文，又作 RPC 传输体，避免中间 Context 构造。

### 6.2 MoveTemp 移动语义

在 Component 层用 MoveTemp 转移 InputDatas 所有权：

```cpp
TMap<FGameplayTag, FInstancedStruct> MergedInputs = MoveTemp(InInputDatas);
// ... 补充 ReleaseLocation/Rotation
Payload.InputDatas = MoveTemp(MergedInputs);
```

### 6.3 InternalPlaySkill && 重载

提供 `const&` 和 `&&` 两个重载，`const&` 委托给 `&&`（拷贝一次后移动）：

```cpp
// const& 版本:拷贝一份后委托给&&版本
int64 InternalPlaySkill(..., const FBXSkillSyncPayload& InPayload, ...);
// && 版本:移动InputDatas到DynamicDatas,零深拷贝
int64 InternalPlaySkill(..., FBXSkillSyncPayload&& InPayload, ...);
```

**调用点**：
- `&&` 版本：`HandleClientPlaySkill`（Payload 用完即弃）
- `const&` 版本：`HandleServerPlaySkill`、`PlaySkill`、`StartPrediction`（后续需转发 Payload）

### 6.4 StartTimelineSections 抽取

将 RunningSections 初始化 + 首帧 KeyFrame 触发的逻辑抽取到 `UBXTLManager::StartTimelineSections`，供 `PlayTimeline` 和 `SkillManager::StartSkillTasks` 复用：

```cpp
// BXTLManager
void StartTimelineSections(FBXTLRunTimeData& InOutData);

// SkillManager 薄委托
void StartSkillTasks(FBXSkillRuntimeData& InOutData)
{
    TLMgr->StartTimelineSections(InOutData.TLRunTimeData);
}
```

Start/Stop 对称：`StartSkillTasks` → `StartTimelineSections`，`StopSkillTasks` → `FinishTimelineSection`。

---

## 7. 冷却职责归属

### 设计决策

冷却数据迁移到 `UBXSkillComponent`，作为 Actor 私有状态管理。

### 理由

| 维度 | Manager 持有（旧） | Component 持有（新） |
|---|---|---|
| 数据结构 | `TMap<TWeakObjectPtr<AActor>, TMap<int32,float>>`（二级映射） | `TMap<int32, float>`（一级映射） |
| 生命周期 | 需 WeakObjectPtr 防 dangling，手动 Deinitialize 清理 | Component 销毁自动清理 |
| 调用链 | Component → Manager.RecordCooldown | Component.RecordCooldown（无跨容器调用） |
| 职责边界 | Manager 混合全局实例与 Actor 私有状态 | Manager 只管全局实例 |

### 校验顺序

```
冷却校验(Component私有) → ServerValidateRelease(Manager全局) → InternalPlaySkill → RecordCooldown
```

Component 负责冷却校验（私有状态），Manager 负责时间戳 + 释放条件校验（全局规则），职责分离。

---

## 8. 加速弥补与卡顿保护

### 8.1 加速机制

- **服务器加速**（`ServerAccelerate`）：服务器处理 RPC 后，对自身技能实例加速，弥补 RPC 上行延迟。
- **客户端加速**（`ClientAccelerate`）：其他客户端收到 Multicast 后加速，弥补广播延迟。
- 延迟差值统一在服务器世界时间域计算（见 3.1 时间戳域约定），负值视为时间同步误差丢弃。
- 加速时长 = 延迟 / 加速倍率，服务器与客户端均钳制上限为技能总时长的 50%（`ClampAccelerateDuration`）。

### 8.2 卡顿保护

**问题**：卡顿帧（如 `InDeltaTime = 2.0s`）时，加速倍率被乘到大 DeltaTime 上，导致跳过大量 KeyFrame。

**修复**：`UpdateAccelerate` 返回等效推进 Delta，将本帧 DeltaTime 拆分为加速部分和正常部分：

```cpp
float UpdateAccelerate(FBXSkillRuntimeData& InOutData, float InDeltaTime)
{
    if (InOutData.AccelerateRemainTime <= 0.0f)
        return InDeltaTime;

    float RunRate = InOutData.AccelerateRate;
    float AcceleratedTime = FMath::Min(InDeltaTime, InOutData.AccelerateRemainTime);
    float NormalTime = InDeltaTime - AcceleratedTime;

    InOutData.AccelerateRemainTime -= AcceleratedTime;
    if (InOutData.AccelerateRemainTime <= 0.0f)
    {
        // 恢复速率,返回等效Delta(加速部分按倍率折算+正常部分)
        ChangeTimelineRunTimeDataTickRate(InOutData.TLRunTimeData, 1.0f);
        return AcceleratedTime * RunRate + NormalTime;
    }

    // 仍在加速中,返回原始Delta(由InternalUpdateTimeline中RunRate放大)
    return InDeltaTime;
}
```

`InternalUpdateSkill` 使用返回的 `EffectiveDelta` 推进 Timeline：

```cpp
float EffectiveDelta = UpdateAccelerate(InOutData, InDeltaTime);
TLMgr->UpdateTimelineRunTimeData(InOutData.TLRunTimeData, EffectiveDelta);
```

### 场景验证

| 场景 | InDeltaTime | RemainTime | RunRate | 返回值 | 实际推进 |
|---|---|---|---|---|---|
| 未加速 | 0.016 | 0 | 1.0 | 0.016 | 0.016 |
| 加速中 | 0.016 | 0.1 | 2.0 | 0.016 | 0.032 |
| 加速结束帧 | 0.016 | 0.008 | 2.0 | 0.024 | 0.024 |
| **卡顿帧** | **2.0** | **0.1** | **2.0** | **2.1** | **2.1** |

卡顿帧从修复前的 2.0s（丢失 0.1s 加速）变为 2.1s（正确弥补）。

---

## 9. 预测与回滚

### 预测启动

客户端（AutonomousProxy）调用 `StartPrediction`：
1. 生成 Client Initiator 的 SkillID
2. `InternalPlaySkill` 本地播放
3. 标记 `PredictState = Predicting`，记录 `PredictStartTime`
4. `ServerPlaySkill` RPC 上传到服务器

### 服务器确认

服务器校验通过后 `ClientPredictResult(true, ServerTimestamp)`：
- 客户端 `ConfirmPrediction`：`PredictState = Confirmed`，记录 `ServerTimestamp`
- 客户端本地实例自预测起即按真实时间推进，服务器通过加速收敛到相同进度，无需客户端额外对齐

### 服务器否认

服务器校验失败 `ClientPredictResult(false, ...)`：
- 客户端 `RollbackPrediction`：`InternalRollbackPredictedSkill` 回滚技能效果
- 从 `OwnedSkillIDs` 移除

### 预测超时

`CheckPredictTimeout` 每帧检查，超过阈值（`SkillPredictMaxDuration`，默认 0.3s）未收到服务器响应的预测技能自动回滚。超时 ID 先收集后统一回滚（回滚会执行 Task 的 End 逻辑，可能同步释放新技能导致 SkillRTDatas 扩容，禁止在 TMap 迭代中直接触发）。

### 冷却的假冷却机制（客户端侧配合）

客户端预测成功后本地立即开始**假冷却**（`RecordCooldown` + `PendingCooldownAssetIDs` 标记），用于挡住连点，无需等待服务器往返。

服务器结果经 `ClientPredictResult(InSkillID, bSuccess, InServerTimestamp, InCooldownRemaining)` 返回后结算：

| 服务器结果 | InCooldownRemaining | 客户端动作 |
|---|---|---|
| 预测确认 | 权威冷却剩余时长 | 假冷却**转正**：`本地时间 + 服务器剩余` 覆盖 CooldownMap |
| 冷却拒绝 | 服务器真冷却剩余时长 | 假冷却**同步为真冷却**（同上覆盖） |
| 条件/时间戳等否认 | -1 | **移除**假冷却，允许立即重试 |

- 转正以"服务器冷却剩余时长"（相对量）传输，避免跨机器时间域换算误差；下行延迟会使客户端冷却略晚于服务器结束（保守方向，宁可晚不可早）。
- `IsCooldownPending(Asset)` 可查询冷却是否处于假冷却状态（等待服务器确认）。
- 两个 RPC 均为 Reliable，结果必然到达：预测超时回滚不清算假冷却，留待结果 RPC 到达时统一结算。

### 遍历安全

`UBXSkillManager::Tick` 对 SkillRTDatas 采用 ID 快照遍历：Task 执行链（连招）可能同步释放新技能触发 TMap rehash，直接迭代器遍历会失效。

---

## 10. 拷贝次数分析

### 各路径总拷贝次数

| 路径 | Component 层 | Manager InternalPlay | RPC 序列化 | 总计 |
|---|---|---|---|---|
| **Authority（服务器/单机）** | 0 | 1 | 1（Multicast） | **2** |
| **AutonomousProxy（客户端预测）** | 0 | 1 | 1（Server RPC） | **2** |
| **服务器处理 ServerPlaySkill** | 0 | 1 | 1（Multicast 转发） | **2** |
| **SimulatedProxy 收到 Multicast** | 0 | 0（&&重载） | 1（反序列化） | **1** |

### 优化前后对比

| 路径 | 优化前 | 优化后 | 降幅 |
|---|---|---|---|
| Authority | 5（Component 3 + Manager 1 + RPC 1） | 2 | 60% |
| AutonomousProxy | 5 | 2 | 60% |
| SimulatedProxy | 3（Component 1 + Manager 1 + RPC 1） | 1 | 67% |

### 剩余拷贝说明

- **RPC 序列化（1次）**：UE 网络栈固有开销，无法避免。
- **Manager InternalPlay（const& 路径 1次）**：后续需转发 Payload 故不能 Move，开销等价于原逐元素 Add。
- **SimulatedProxy 路径 0 次额外拷贝**：`HandleClientPlaySkill` 值传递 + `&&` 重载 + `MoveTemp` 写入 DynamicDatas，全链路零拷贝。

---

## 11. 技能/BUFF 状态复制与重建（Late Join）

### 11.1 问题定义

Multicast RPC 是瞬时消息，只对"当时已在监听"的连接生效。两类场景需要不同处理：

| 场景 | 处理通道 |
|---|---|
| 新复制到客户端的对象（Late Join / 相关性进入），创建初始化时 | **状态属性快照复制 + 一次性重建** |
| 服务器中断技能/BUFF（主动 Stop / Actor 死亡 / 生命周期到期） | **显式 Multicast RPC 广播**（当前缺失，需补） |

设计原则（与 GAS 一致的基本同步处理）：**显式 RPC 承载增量事件（开始/结束），复制属性承载初始快照（重建用）**。两条通道职责分明，不做隐式 diff 推导结束。

### 11.2 总体架构

```
【增量通道】(在场对象实时同步)
  MulticastPlaySkill(已有) / MulticastAddBuff(已有)
  MulticastStopSkill(新增) / Buff到期移除广播(补)
        │
        ▼
【快照通道】(新复制体创建时初始化)
  服务器: 技能播放/结束 ──维护──▶ RunningSkillStates (Replicated)
                                  RunningBuffStates  (Replicated)
        │ 属性复制(随ActorChannel初始bunch到达)
        ▼
  客户端: 组件OnRep(首次) ──▶ 逐条重建技能/BUFF(带进度对齐)
          bInitialRebuildDone标记,此后OnRep不再全量重建
```

### 11.3 数据结构

```cpp
// 技能复制状态(挂在UBXSkillComponent)
USTRUCT()
struct FBXSkillReplicatedState
{
    int64 SkillID = 0;              // 与SkillRTDatas键一致(幂等去重用)
    int32 SkillAssetID = 0;
    EBXSyncInitiator Initiator;
    int64 StartServerTimestamp = 0; // 服务器世界时间域播放时刻(进度对齐用)
    AActor* Instigator = nullptr;
    AActor* Triggerer = nullptr;
    TMap<FGameplayTag, FInstancedStruct> InputDatas;  // 原样复制(重建DynamicDatas)
};

// BUFF复制状态(挂在UBXBuffComponent)
USTRUCT()
struct FBXBuffReplicatedState
{
    int64 BuffID = 0;
    int32 BuffAssetID = 0;
    int32 Layer = 0;
    int32 Level = 0;
    float RemainingLifetime = -1.0f; // 剩余生命时长(-1=永久),相对量避免时间域换算
    AActor* Instigator = nullptr;
};

// 组件容器(仅初始快照用,不承载持续diff语义)
UPROPERTY(ReplicatedUsing=OnRep_RunningSkillStates)
TArray<FBXSkillReplicatedState> RunningSkillStates;
```

规模：技能并发个位数、BUFF几十条，普通复制数组足够；数量级增长后再升级 FFastArraySerializer。

### 11.4 服务器维护与显式中断广播

**技能状态数组**（SkillComponent，服务器侧维护）：
- `HandleServerPlaySkill` / 本地 Authority `PlaySkill` 成功 → `RunningSkillStates.Add`（含 StartServerTimestamp）
- 技能结束（自然结束/中断/Stop）→ 移除条目。**需要新增 `BXEvent.Skill.Finished` 事件**（Manager 在 bEarlyFinish 置位处广播，Component 订阅移除），保证后续进入相关性的对象拿到准确快照

**MulticastStopSkill（新增 RPC，Reliable）**：
```cpp
UFUNCTION(NetMulticast, Reliable)
void MulticastStopSkill(int64 InSkillID, uint8 InFinishReason);
```
- 发送时机：`UBXSkillManager::StopSkill` 中 `Reason == FR_Interrupt` 时（覆盖 API 调用、Actor 死亡 EndPlay、条件强制中断），经 `Data.TLRunTimeData.Owner` 的 SkillComponent 广播
- 客户端收到：停止本地技能 + `OwnedSkillIDs.Remove` + 修剪本地状态副本
- **不发送**：自然结束（客户端实例同步推进会自行结束）、预测回滚（服务器无该实例）

**BUFF 侧对应补齐**：Manager 内生命周期到期移除处补发 `MulticastRemoveBuff`（当前只有 Component 入口发过）。

### 11.5 客户端重建流程（对象创建后初始化，一次性）

```
OnRep_RunningSkillStates
  ├─ 首次(bInitialRebuildDone==false):
  │     for State in RunningSkillStates:
  │        ├─ 跳过: SkillRTDatas已存在该ID(Multicast先到/本地预测)
  │        ├─ 查资产 → InternalPlaySkill(&&, SkillID, Initiator)
  │        └─ 进度对齐(见11.6)
  │     bInitialRebuildDone = true
  ├─ 后续OnRep: 不再全量重建(增量由显式RPC驱动)
  │     新增条目 → 快照滞后,可安全重建(有ID去重兜底)
  │     消失条目 → 本地若仍存在该ID则停止(兜底RPC与属性乱序竞态,见11.7)
  └─ 同步本地副本
```

BUFF 重建复用 `AddBuffWithID`（已有 Find 防重），重建后写入 RemainingLifetime；层级重建走已有 `RebuildEffect`。

### 11.6 进度对齐（核心难点）

错过的 KeyFrame 通过 `RunTime` 前移 + 现有 while 追赶循环一次性触发，Task 以已有 `OffsetTime` 参数自补偿：

| Task 类型 | OffsetTime 补偿方式 |
|---|---|
| 动画 | Montage StartAt(OffsetTime) 从中段播 |
| 特效/声音 | 起播或减寿命 |
| 碰撞/伤害 | SimulatedProxy 上本就 NetMode 门控跳过，无副作用 |

新增 `UBXTLManager::SeekTimelineRunTimeData(RTData, InTime)`：逐 Section 前移 RunTime 并触发追赶 KeyFrame。

```
ElapsedMs = Clamp(GetServerWorldTimeMilliseconds() - StartServerTimestamp, 0, 技能总时长)
```

保守备选：复用现有 `AccelerateRemainTime` 高倍率平滑追赶（视觉更顺，对齐慢）。

### 11.7 顺序与竞态

| 竞态 | 后果 | 处理 |
|---|---|---|
| Stop RPC 先于属性快照到达 | 重建出一个已死技能 | OnRep 消失条目兜底停止（本地存在才停，不存在忽略） |
| 属性快照先到、技能已在服务器结束但 Stop RPC 未到 | 重建后随即被 Stop RPC 停止 | 正常时序，无需处理 |
| Multicast 与快照重建并发到达同一技能 | 双份播放 | InternalPlaySkill 的 SkillID 去重守卫（已有） |
| 重建时 Actor 表现层未就绪（动画/网格） | 闪断 | P1 观察后决定是否延迟一帧重建 |

### 11.8 实施拆分

| 阶段 | 内容 | 依赖 |
|---|---|---|
| P1 | MulticastStopSkill + Buff到期广播补齐 + BXEvent.Skill.Finished | 无（独立小改动） |
| P2 | 复制状态结构 + 服务器维护 + 首次OnRep重建(不快进,从头播) | P1 |
| P3 | SeekTimelineRunTimeData 进度快进 + OffsetTime 审计 | P2 |
| P4 | FastArray升级、按需复制(数据驱动) | P2/P3稳定后 |

### 11.9 风险点

- `TMap<FGameplayTag, FInstancedStruct>` 复制：FInstancedStruct 按结构路径序列化，两端类型定义需一致。
- 短技能高频增删数组：属性复制按元素 diff，开销可控。
- 快进密集触发 Task：审计各 Task 的 OffsetTime 使用（框架已传参，个别未适配需补）。
- 服务器数组与 Manager 生命周期不一致（Finished 事件丢失→快照残留）：OnRep 消失兜底 + Finished 可靠广播双保险。

### 11.10 P2 详细设计：快照结构（运行数据投影方案）

**核心思路**：不推演进度，直接下发**服务器当前的运行数据**。客户端反投影恢复运行数据后从真实状态无缝续跑——动态分支、循环计数、Task 内部状态全部保真。

#### 为什么不能原样复制 FBXTLRunTimeData

| 字段 | 类型 | 处理 |
|---|---|---|
| `Timeline` | `UBXTLAsset*`（资产指针） | 投影为 `TimelineID`，客户端 `GetSkillAssetByID` 恢复 |
| `RunningTasks[].Task` | `UBXTask*`（资产子对象指针） | 投影为已有 `Index`（Section.TaskList 索引），客户端按 (SectionIdx, TaskIdx) 恢复 |
| `Owner/Instigator/Triggerer` | `AActor*` | Actor 引用直接复制（NetGUID）✓ |
| 其余全部 | 值类型/值容器 | 直接复制 ✓（SectionRT/TaskRT 的 Index、RunTime、LoopCount、KeyFrameIndex、NextTick、DynamicDatas、ScopeGraph 等均为值类型，DynamicDataSearchKey 已有 GetTypeHash） |

结论：值字段占绝大多数，投影层很薄——只换两个指针字段。

#### 公共投影结构（同时服务于录像回放、断线重连等未来需求）

```cpp
// FBXTLRunTimeData的可复制投影
USTRUCT()
struct FBXTLRunTimeDataReplicated
{
    int32 TimelineID = 0;
    int64 ID = 0;
    AActor* Owner = nullptr;
    AActor* Instigator = nullptr;
    AActor* Triggerer = nullptr;
    TArray<FBXBodyPartSelection> LockParts;
    float RunTime = 0.0f;
    float RunRate = 1.0f;
    TArray<FBXTLSectionRTDataReplicated> RunningSections;
    TMap<FBXTLDynamicDataSearchKey, FInstancedStruct> DynamicDatas;   // 含运行期写入的数据(比仅InputDatas更完整)
    TArray<FInt64Vector2> ScopeGraph;
};

USTRUCT()
struct FBXTLSectionRTDataReplicated
{
    int32 Index = -1;  int32 KeyFrameIndex = 0;  float RunTime = 0.0f;
    int32 LoopCount = 0;  int32 ForceJumpSection = -1;  bool bEarlyFinish = false;
    TArray<FBXTLTaskRTDataReplicated> RunningTasks;
    // PendingTasks/BroadcastTasks按需投影(FBXTLPendingTaskInfo均为值类型)
};

USTRUCT()
struct FBXTLTaskRTDataReplicated
{
    int32 Index = -1;          // Task指针→Section.TaskList索引
    float RunTime = 0.0f;  float NextTick = 0.0f;  int64 ParentScope = 0;
    FInstancedStruct DynamicData;
};
```

投影/反投影函数对（建议放 UBXFunctionLibrary 或 TLManager）：
```cpp
void ToReplicatedProjection(const FBXTLRunTimeData& InSource, FBXTLRunTimeDataReplicated& OutTarget);
void FromReplicatedProjection(const FBXTLRunTimeDataReplicated& InSource, UBXTLAsset* InAsset, FBXTLRunTimeData& OutTarget);
// 反投影时按(Index→TaskList[Index])恢复Task指针;NetTypes过滤见11.12
```

#### 技能条目（FastArray载体）

```cpp
USTRUCT()
struct FBXSkillReplicatedState
{
    GENERATED_USTRUCT_BODY()

    int64 SkillID = 0;              // 与SkillRTDatas键一致(去重/停止定位)
    EBXSyncInitiator Initiator = 0;
    FBXTLRunTimeDataReplicated TLRunTimeData;   // 发送时刻的运行数据投影
};

// FastArray而非普通TArray:PreReplicatedAdd提供"新接收连接打开通道时刻现场投影"钩子
USTRUCT()
struct FBXSkillReplicatedArray : public FFastArraySerializer
{
    TArray<FBXSkillReplicatedState> Items;

    bool NetDeltaSerialize(FNetDeltaSerializeSerializeInfo...)  // 标准FastArray样板
    void PreReplicatedAdd(const TArrayView<int32>& ChangedIndices, int32 FinalSize);
    // ↑服务器端、条目即将首次发送给某个客户端时回调:此时从SkillRTDatas现场投影最新运行数据写入条目
};
```

FastArray增量的含义天然满足需求：**每个新连接/新相关客户端获得"它打开通道那一刻"的最新状态**；已有条目的客户端不重发（不产生持续运行数据同步的带宽）。普通TArray无此钩子，故技能侧直接采用FastArray。

#### BUFF快照（保持简单形态，本质也是运行数据投影）

```cpp
USTRUCT()
struct FBXBuffReplicatedState
{
    int64 BuffID = 0;
    int32 BuffAssetID = 0;
    int32 Layer = 0;  int32 Level = 0;               // 持续同步(重建+已重建客户端都响应)
    AActor* Instigator = nullptr;
    int64 SharedExpireServerTimestamp = 0;            // BLL_Shared到期(服务器世界时间域ms,Add/刷新时写)
    TArray<int64> LayerExpireServerTimestamps;        // BLL_Independent各层到期
};
```
BUFF运行数据（层/级/计时）已全部覆盖，无Task时序问题，无需FastArray现场投影。

### 11.11 P2 详细设计：重建流程

```
OnRep(首次) → for State in Items:
  ├─ SkillRTDatas已有该SkillID → 跳过(Multicast先到/本地预测,幂等)
  ├─ GetSkillAssetByID + LoadSynchronous → 资产
  ├─ FromReplicatedProjection(State.TLRunTimeData, 资产, NewTLData)
  │     恢复Timeline指针、逐Task恢复(SectionIdx,TaskIdx)→Task对象指针
  ├─ InternalPlaySkill变体:跳过StartTimelineSections,直接采用恢复的FBXTLRunTimeData入SkillRTDatas
  └─ OwnedSkillIDs.Add
```

**续跑即追赶**：恢复的RunTime/RunningSections/KeyFrameIndex/NextTick就是真实进度，客户端下一帧起与服务器并行推进（两侧都按真实时间走，偏差为常量RTT/2，无害）。

后续OnRep（FastArray增量回调）：新增条目同上；消失条目仅兜底MulticastStop乱序（本地存在且非Predicting才停）。

### 11.12 P2 详细设计：进度与Task上下文问题

#### 进度——不再需要推演

原时间戳推演方案的三大限制（动态分支不追溯/加速误差/Section链推演）全部消失：运行数据记录的就是服务器实际路径。`SeekTimelineRunTimeData` 不再需要。

#### Task上下文（FInstancedStruct内的UObject指针）处理

投影时按 `Task->NetTypes` **过滤**：仅保留声明了模拟端位的Task的RunningTasks条目。
- 碰撞/伤害类Task（权威端位）本就不在SimulatedProxy执行，投影丢弃其条目——省带宽且避开其上下文内的组件指针（如TrackHitBoxContext.ShapeComponents不可复制）。
- 表现类Task（动画/特效）上下文若含UObject引用：属性级NetGUID可解析（如Montage）；确实不可解析的由Task在首个Update时检测DynamicData有效性并自愈重初始化（需审计，与Multicast延迟场景同一套自愈逻辑）。

#### Task迟到状态语义

恢复的Task带着半程RunTime/NextTick直接进入Update——Task内部状态（动画进度由Montage自身持有、粒子寿命由DynamicData持有）与框架时间一致。个别Task若依赖"Start曾执行"的初始化，投影里DynamicData已带过半程状态，Start缺失部分由Task自愈（同上审计项）。

#### BUFF计时对齐（不变）

```
剩余 = Clamp((ExpireServerTimestamp - ServerNowMs)/1000, 0, BuffDuration)
BLL_Shared: RunTime = BuffDuration - 剩余;  BLL_Independent: LayerRunTimes[i] = BuffDuration - 各层剩余
```
Task(TaskBindings常驻)从头启动，不快进。

### 11.13 P2 竞态与边界

| 场景 | 处理 |
|---|---|
| Multicast先于快照到 | 重建跳过(去重守卫),正常路径 |
| 快照先于Multicast到 | 快照先重建,Multicast到时去重跳过 |
| Stop RPC先于快照 | 重建出已死技能→下次FastArray增量消失条目兜底停止 |
| 预测技能出现在服务器快照 | 预测端本地已有同ID实例→重建跳过 |
| 投影时技能恰在服务器上结束 | PreReplicatedAdd回调里Find不到→条目写空标记,客户端跳过;或该帧先移除条目则不发送 |
| 重建时资产未加载 | LoadSynchronous阻塞一次(冷启动);P4可改异步+延迟重建 |
| FInstancedStruct内不可复制指针 | NetTypes过滤+Task自愈(见11.12) |

### 11.14 P2 实施拆分与验证用例

**实施顺序**：
1. 投影结构 + To/From投影函数（纯数据转换，可单测）
2. FBXSkillReplicatedArray(FastArray) + PreReplicatedAdd现场投影 + 服务器条目生命周期(播放Add/Finished移除)
3. OnRep重建(反投影+跳过StartTimelineSections直接续跑)
4. Task自愈审计(表现类Task的DynamicData恢复性检查)
5. BUFF快照数组 + OnRep(重建+层数/等级持续同步)

**验证用例**：
1. 客户端B播放中,新客户端C加入→C立即看到B进行中的技能,进度与B一致(±RTT)。
2. 分支技能(Switch)播放中C加入→重建实例与服务器同分支（对比原推演方案的关键优势）。
3. C远离(超相关性)再返回→重建正确。
4. 重建后服务器StopSkill→C的重建实例同步停止。
5. BUFF重建后服务器改层→已重建客户端RebuildEffect生效。
6. 预测回滚技能不出现在任何远端重建中。
7. 带宽对比:在场客户端在技能播放期间不应收到重复的运行数据(FastArray增量语义)。
