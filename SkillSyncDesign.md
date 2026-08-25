# BattleX 技能同步框架设计方案

> 高性能动作游戏技能系统 · 网络同步与运行时架构
> 仓库根：`BattleX/`  ·  初版 2026-08-13  ·  最后更新 2026-08-19（P1/P2 实施 + UE5.4 适配）

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
- **LockParts 需 USTRUCT 包装**（`FBXSkillLockParts{ TArray<FBXBodyPartSelection> Parts }`）：FInstancedStruct 不可直接承载容器。`InternalPlaySkill` 解包 LockParts 条目写入 `TLRunTimeData.LockParts` 专用字段（Task 消费方读 `InRTData.LockParts`），不进 DynamicDatas。
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
    int64 ClientTimestamp = 0;          // 服务器世界时间域(毫秒)
    int64 ServerTimestamp = 0;          // 服务器世界时间域(毫秒,仅服务器填写)
    EBXSyncInitiator Initiator;
    float AccelerateRemainTime = 0.0f;  // 加速剩余时间
    float AccelerateRate = 1.0f;        // 加速倍率
    bool bEarlyFinish = false;
    EBXTLFinishReason FinishReason;     // 结束原因(bEarlyFinish置位时记录,Finished事件携带)
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
       ├─ 按TaskFullIndex匹配Task,清除bAwaitingClientCollision
       ├─ 生成服务器侧作用域,双键写入DynamicDatas[(FullIndex,Tag)]+[(FullIndex,Tag,Scope)]   ← 与客户端WriteContextData行为一致
       ├─ 触发BXTEvent.Success → 反应分支Task在服务器侧启动(事件作用域读取键可达)
       └─ 广播BXEvent.Collision.Reported
```

- 服务器端 TrackHitBox 跳过实际检测，标记 `bAwaitingClientCollision` + `ServerExtraLifeTimer`（0.15s 兜底）。
- 存在 awaiting Task 的 Section 延迟自然结束（中断/强跳不受影响），保证上报仍能命中 RunningSections 中的 Task。
- Task 已结束的迟到上报退化为单键写入（无作用域键），供后续时间片段按索引读取兜底。
- 服务器开启 `bServerCollisionCheck` 时忽略客户端上报（服务器权威，防止重复结果）。
- 帧内执行队列（TaskStackInFrame）在待处理任务处理后清空：残留条目会让更新管线外触发的 AddPendingTask（碰撞上报的 Success）误入堆栈永不执行，且阻塞同索引任务入队。

---

## 6. 性能优化策略

### 6.1 统一数据载体

合并 `FBXSkillPlayContext` 到 `FBXSkillSyncPayload`，消除 Context 与 Payload 的字段冗余。Payload 既作施放上下文，又作 RPC 传输体，避免中间 Context 构造。

### 6.2 MoveTemp 移动语义

在 Component 层用 MoveTemp 转移 InputDatas 所有权（TMap 入参逐条移动进条目数组，无深拷贝）：

```cpp
// BP入参为TMap,逐条MoveTemp进RPC条目数组
TArray<FBXSkillInputEntry> MergedInputs;
for (TPair<FGameplayTag, FInstancedStruct>& Pair : InInputDatas)
{
    FBXSkillInputEntry& Entry = MergedInputs.AddDefaulted_GetRef();
    Entry.Tag = Pair.Key;
    Entry.Value = MoveTemp(Pair.Value);
}
// ... 补充 ReleaseLocation/Rotation 条目
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

**服务器处理 ServerPlaySkill（HandleServerPlaySkill）**：

```
SkillID校验(Initiator必须为Client且服务器无重复) -> 冷却校验(Component私有) -> ServerValidateRelease(Manager全局) -> InternalPlaySkill -> RecordCooldown -> ClientPredictResult
```

冷却拒绝与校验失败的 `ClientPredictResult` 均携带结果：前者带服务器权威冷却剩余（假冷却同步为真冷却），后者带 -1（移除假冷却允许重试）。

**客户端发起预测（PlaySkillWithInputData）**：本地冷却检查（含假冷却，防连点）通过后 `StartPrediction` + 记录假冷却 + 上行 RPC。本地检查仅是快速失败，服务器仍会权威校验。

**Standalone/服务器权威端**：本地冷却检查 -> 播放 -> `RecordCooldown` -> `MulticastPlaySkill`。

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
| 新复制到客户端的对象（Late Join / 相关性进入），创建初始化时 | **复制属性快照 + OnRep 重建** |
| 服务器中断技能/BUFF（主动 Stop / Actor 死亡 / 生命周期到期） | **显式 Multicast RPC 广播** |

设计原则（与 GAS 一致的基本同步处理）：**显式 RPC 承载增量事件（开始/结束），复制属性承载初始快照（重建用）**。

### 11.2 总体架构（最终实现：COND_InitialOnly + PreReplication）

**UE5.4 已移除 FastArray 虚回调机制（FFastArraySerializer 非多态，static_assert 禁止），且无服务器端发送前钩子，故不采用 FastArray 方案**（选型分析见 `Net/BXSkillReplicated.h` 文件头注释）。

```
【增量通道】(在场对象实时同步)
  MulticastPlaySkill(Reliable) / MulticastAddBuff(Reliable)
  MulticastStopSkill(Reliable, 服务器FR_Interrupt时) / MulticastRemoveBuff(Reliable, InternalRemoveBuff收束)
        │
        ▼
【快照通道】(新复制体创建时初始化)
  服务器: 组件PreReplication检测远程连接数增加(新客户端连入)时从SkillRTDatas重建快照数组
        │   ← 常规帧零投影开销(避免每帧全量深拷贝DynamicDatas);时序:连接加入ClientConnections后
        │     才在后续ServerReplicateActors打开通道,PreReplication在同一次flush序列化前运行,当帧重建即被消费
        │ 属性复制(COND_InitialOnly:仅新连接初始bunch发送,已有连接零属性流量)
        ▼
  客户端: OnRep(带旧值)差分 → 新增条目反投影重建 → 续跑
```

技能快照不可变、已有连接的技能动态完全由显式 RPC 维护；BUFF 快照为无条件复制（低频变化），额外承担已重建客户端的层/级/到期持续同步。

### 11.3 数据结构

#### 运行数据投影（公共基建，`Timeline/BXTLReplicated.h`）

```cpp
USTRUCT()
struct FBXTLRunTimeDataReplicated      // FBXTLRunTimeData的可复制投影
{
    int32 TimelineID;                  // Timeline指针→GetTimelineAssetByID恢复
    int64 ID;
    AActor* Owner/Instigator/Triggerer; // Actor引用直接复制(NetGUID)
    TArray<FBXBodyPartSelection> LockParts;
    float RunTime; float RunRate;
    TArray<FBXTLSectionRTDataReplicated> RunningSections;
    TMap<FBXTLDynamicDataSearchKey, FInstancedStruct> DynamicDatas;  // 含运行期写入数据
};

USTRUCT()
struct FBXTLSectionRTDataReplicated    // Index/KeyFrameIndex/RunTime/LoopCount/ForceJumpSection/bEarlyFinish + RunningTasks
USTRUCT()
struct FBXTLTaskRTDataReplicated       // Index(Task指针→TaskList[Index]) /ParentScope/RunTime/NextTick/bEarlyFinish/DynamicData
```

转换函数对：`BXToTLRunTimeProjection`（投影时按 `Task.NetTypes` 过滤，仅保留模拟端位 Task——权威/自主端 Task 不在远端执行，同时规避其上下文中的不可复制指针）；`BXFromTLRunTimeProjection`（按 Index 恢复 Task 指针，索引失效条目跳过）。ScopeGraph/TaskStackInFrame 含无反射类型不投影。

#### 复制条目

```cpp
USTRUCT()
struct FBXSkillReplicatedState         // 挂UBXSkillComponent,COND_InitialOnly
{
    int64 SkillID;                     // 与SkillRTDatas键一致(去重/停止定位)
    EBXSyncInitiator Initiator;
    FBXTLRunTimeDataReplicated TLRunTimeData;
};

// UBXSkillComponent
UPROPERTY(ReplicatedUsing=OnRep_RunningSkillStates)
TArray<FBXSkillReplicatedState> RunningSkillStates;
// DOREPLIFETIME_CONDITION(..., COND_InitialOnly)

USTRUCT()
struct FBXBuffReplicatedState          // 挂UBXBuffComponent,无条件复制
{
    int64 BuffID; int32 BuffAssetID;
    int32 Layer; int32 Level;          // 持续同步
    AActor* Instigator;
    int64 SharedExpireServerTimestamp;         // BLL_Shared到期(服务器世界时间域ms,Add/刷新时写)
    TArray<int64> LayerExpireServerTimestamps; // BLL_Independent各层到期
};

// UBXBuffComponent
UPROPERTY(ReplicatedUsing=OnRep_RunningBuffStates)
TArray<FBXBuffReplicatedState> RunningBuffStates;
```

### 11.4 服务器维护与显式中断广播

**技能快照（PreReplication 连接计数触发重建，无手动维护点）**：

```cpp
void UBXSkillComponent::PreReplication(IRepChangedPropertyTracker&)
{
    // 仅当远程连接数增加(新客户端连入)时调用RebuildRunningSkillStates重建快照数组
    // 常规帧零投影开销:COND_InitialOnly对已有连接零流量,每帧深拷贝DynamicDatas无意义
    // 断线回落仅同步计数不重建(InitialOnly已发收不回);回落保证"断N再连N"后重新触发
    // 初值-1保证组件首个复制周期(角色中途spawn等)必建一次基线快照
}
```

**技能结束**：CleanSkillTrash 移除数据时统一广播 `BXEvent.Skill.Finished`（参数含 FinishReason，自然结束/中断/回滚三路写入），本地事件供 P2 后续及 UI 消费。

**MulticastStopSkill**：`UBXSkillManager::StopSkill` 在 `FR_Interrupt` 时经 Owner 的 SkillComponent 广播（覆盖 API 调用、Actor 死亡 EndPlay、条件强制中断）。自然结束/预测回滚不广播。

**BUFF 快照（收束点维护）**：
- `AddBuff` 成功 → 条目加入（到期时刻 = now + 剩余，Tick 不更新，带宽友好）
- `InternalRemoveBuff` → 条目移除 + `MulticastRemoveBuff`（同收束点，覆盖手动/到期/层级耗尽）
- `ChangeBuffLayer` / `ChangeBuffLevel` / `RefreshBuffLifetime` → 条目原地刷新（`FillBuffReplicatedState` 从运行数据统一反推）

### 11.5 客户端重建流程（OnRep 差分）

```
OnRep_RunningSkillStates(旧值)
  ├─ 新增条目(SkillID旧值无) → RebuildSkillFromProjection:
  │     ├─ SkillRTDatas已有该ID → 跳过(Multicast先到/本地预测,幂等)
  │     ├─ GetTimelineAssetByID + LoadSynchronous → 资产
  │     ├─ BXFromTLRunTimeProjection 恢复运行数据(含Task指针)
  │     ├─ 跳过StartTimelineSections → 直接入SkillRTDatas续跑
  │     └─ 广播BXEvent.Skill.Released(本地可见)
  └─ 消失条目 → StopSkillIfNotPredicting(本地存在且非Predicting才停,兜底RPC与属性乱序)

OnRep_RunningBuffStates(旧值)
  ├─ 新增条目 → AddBuffWithID(已有防重) + 到期时间戳回填计时
  │            (BLL_Shared: RunTime=Duration-剩余; BLL_Independent: LayerRunTimes[i]逐层回填)
  ├─ 变化条目(层/级/到期字段diff) → ChangeBuffLayer/ChangeBuffLevel(触发RebuildEffect) + 计时对齐
  └─ 消失条目 → RemoveBuffIfLocalExists兜底(主通道MulticastRemoveBuff)
```

### 11.6 进度对齐（核心：续跑即追赶）

**不推演进度，直接下发服务器当前的运行数据**：恢复的 RunTime/RunningSections/KeyFrameIndex/NextTick 就是服务器实际状态（含动态分支、循环计数、Task 内部状态），客户端下一帧起 1x 续跑，与服务器恒差 RTT/2（常量偏移，无害）。

原时间戳推演方案的三大限制（动态分支不追溯/加速误差/Section 链推演）全部消失。

**Task 上下文**：投影按 NetTypes 过滤后，仅表现类 Task 进入重建——动画/特效 Task 上下文若含 UObject 引用，由属性级 NetGUID 解析；不可解析的由 Task 在首个 Update 检测 DynamicData 有效性自愈（与 Multicast 延迟场景同一套逻辑）。

### 11.7 顺序与竞态

| 竞态 | 处理 |
|---|---|
| Multicast 先于快照到 | 重建跳过(去重守卫)，正常路径 |
| 快照先于 Multicast 到 | 快照先重建，Multicast 到时去重跳过，等价 |
| Stop RPC 先于快照 | 重建出已死技能→下次属性复制消失条目兜底停止（本地存在且非 Predicting） |
| 预测技能出现在服务器快照 | 预测端本地已有同 ID 实例→重建跳过 |
| 重建时资产未加载 | LoadSynchronous 阻塞一次(冷启动)；后续可改异步+延迟重建 |
| 新客户端连入(快照重建触发) | PreReplication检测ClientConnections计数增加→当帧重建(序列化前运行无窗口期);断线回落不重建(已发收不回) |
| 断线重连 | 断开:计数回落仅同步记录值;重连:计数超记录值→正常触发重建 |
| BUFF 层变化同步 | 层级变化仅经 RunningBuffStates 无条件复制同步（原 MulticastBuffLayerChanged RPC 已删除，与快照通道冗余），属性复制频率低于 RPC，瞬时层级变化感知略慢 |

### 11.8 验证用例与观测日志

测试期在关键节点埋有 `BXMGR_Skill` / `BXMGR_Buff` / `BX_TP` 类别日志（Log 级别，稳定后清理）：

| 用例 | 验证方式 |
|---|---|
| 1. 基础重建：客户端B播放中，新客户端C加入 | C 端 `OnRep: Rebuild entry` + `Rebuilt`，Rebuilt 的 RunTime≈服务器已播时长（**新鲜度方案的直接判据**） |
| 2. 分支技能(Switch)重建同分支 | C 端重建实例与服务器同分支（运行数据记录实际路径） |
| 3. 相关性进出(NetCullDistance) | 远离再返回后重建正确 |
| 4. 重建后服务器中断 | C 端 `MulticastStopSkill: Received` 停止重建实例 |
| 5. BUFF 状态同步 | 重建后服务器改层→`Layer changed` + RebuildEffect 生效 |
| 6. 预测回滚不泄漏 | 回滚技能不出现在任何远端重建中 |
| 7. 已有连接零冗余 | 在场客户端技能播放期间无快照流量（COND_InitialOnly） |
| 8. 碰撞上报链路 | 客户端 `Client reporting collision` ↔ 服务器 `Received`，Hits 数一致 |
