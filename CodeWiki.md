# BattleX Code Wiki

> 高性能动作游戏技能系统 · Unreal Engine 5 插件
> 仓库根：`BattleX/`  ·  版本：1.0 (Beta)  ·  文档生成日期：2026-07-25

---

## 目录

1. [项目概述](#1-项目概述)
2. [整体架构](#2-整体架构)
3. [模块职责与目录结构](#3-模块职责与目录结构)
4. [核心运行时系统](#4-核心运行时系统)
   - 4.1 [基础设施层 (Manager / SubSystem / Settings / Tags)](#41-基础设施层)
   - 4.2 [Timeline 时间轴系统（技能核心）](#42-timeline-时间轴系统技能核心)
   - 4.3 [Task 任务系统（技能原子操作）](#43-task-任务系统技能原子操作)
   - 4.4 [State 状态系统（行为代理与状态机）](#44-state-状态系统行为代理与状态机)
   - 4.5 [DecisionTree 决策树系统（受击 / 连段选择）](#45-decisiontree-决策树系统受击--连段选择)
   - 4.6 [Gear 装备系统（含冷兵器）](#46-gear-装备系统含冷兵器)
   - 4.7 [Movement 移动系统](#47-movement-移动系统)
   - 4.8 [Collision 碰撞系统](#48-collision-碰撞系统)
   - 4.9 [Event 事件系统](#49-event-事件系统)
   - 4.10 [Condition 条件系统](#410-condition-条件系统)
   - 4.11 [Animation 动画系统](#411-animation-动画系统)
   - 4.12 [Unit / Lock 系统（占位）](#412-unit--lock-系统占位)
5. [编辑器模块](#5-编辑器模块)
6. [依赖关系](#6-依赖关系)
7. [项目运行方式](#7-项目运行方式)
8. [关键数据流与执行流程](#8-关键数据流与执行流程)
9. [优化建议](#9-优化建议)

---

## 1. 项目概述

**BattleX** 是一个面向 **高性能动作游戏** 的技能系统插件，基于 Unreal Engine 5 构建。它提供了一套完整的、数据驱动的技能开发框架，目标是让策划/技术美术能在不修改 C++ 代码的前提下，通过编辑器资产编排复杂的动作游戏技能（连段、受击反应、命中检测、位移、特效等）。

### 设计目标

| 目标 | 实现手段 |
|---|---|
| **高性能** | 单点 `UBXTLManager` 集中 Tick；任务处理器实例缓存；InstancedStruct 零拷贝传递动态数据；按 Bitmask 选择性启用 C++/BP 执行路径 |
| **数据驱动** | 技能 = `UBXTLAsset` 资产；任务 = `UBXTask` 内联实例；决策树 = `UBXDecisionTreeTemplate` 资产 |
| **可扩展** | Task / Processor / BehaviorAgent / StateMachine / Condition 均为 `Abstract + Blueprintable + EditInlineNew`，支持 C++ 与蓝图双轨扩展 |
| **网络友好** | Task 通过 `EBXTNetType` Bitmask 区分权威端/自主端/模拟端执行；RootMotion 支持网络复制 |
| **编辑器集成** | 自定义资产类型 + 工作流应用 (WorkflowOrientedApp) 编辑器 + 自绘时间轴控件 + 图编辑器 |

### 插件清单（`BattleX.uplugin`）

- 模块 `BattleX`：Runtime 模块，`Default` 加载阶段
- 模块 `BattleXEditor`：`UncookedOnly` 模块，`PostEngineInit` 加载阶段
- 依赖插件：`StructUtils`、`Niagara`、`Chooser`、`AnimationWarping`、`PoseSearch`、`MotionTrajectory`、`MotionWarping`、`EnhancedInput`

---

## 2. 整体架构

```
┌─────────────────────────────────────────────────────────────────────┐
│                          游戏项目 (Project)                          │
│   ┌─────────────────────────────────────────────────────────────┐  │
│   │                     BattleX (Runtime)                       │  │
│   │                                                              │  │
│   │  ┌─────────────┐  ┌──────────────┐  ┌──────────────────┐   │  │
│   │  │ 基础设施    │  │  Timeline    │  │   Task (原子)    │   │  │
│   │  │ Manager/    │  │  (技能核心)  │←→│  Processor       │   │  │
│   │  │ SubSystem/  │  └──────┬───────┘  └────────┬─────────┘   │  │
│   │  │ Settings/   │         │                   │             │  │
│   │  │ Tags/Func   │         ↓                   ↓             │  │
│   │  └──────┬──────┘  ┌──────────────┐  ┌──────────────────┐   │  │
│   │         │         │   State      │  │  DecisionTree    │   │  │
│   │         │         │ (行为/状态机)│  │ (受击/连段决策)  │   │  │
│   │         │         └──────┬───────┘  └────────┬─────────┘   │  │
│   │         │                │                   │             │  │
│   │         ↓                ↓                   ↓             │  │
│   │  ┌──────────────────────────────────────────────────────┐  │  │
│   │  │      支撑系统: Gear / Movement / Collision /         │  │  │
│   │  │                Event / Condition / Animation / Unit  │  │  │
│   │  └──────────────────────────────────────────────────────┘  │  │
│   └─────────────────────────────────────────────────────────────┘  │
│   ┌─────────────────────────────────────────────────────────────┐  │
│   │                  BattleXEditor (UncookedOnly)               │  │
│   │   TimelineEditor │ DecisionTreeEditor │ TimelineBase │     │  │
│   │   ComponentVisualizers │ SimpleEditor │ CustomLayout       │  │
│   └─────────────────────────────────────────────────────────────┘  │
└─────────────────────────────────────────────────────────────────────┘
```

**核心抽象**：
- **技能 = 时间轴资产**：一个技能由若干 `FBXTLSection`（时间片段）组成，每个片段包含若干 `UBXTask`（任务）按时间排布。
- **任务 = 原子操作 + 处理器**：`UBXTask` 是配置数据（蓝图可编辑），`UBXTProcessor` 是运行时执行器（C++/蓝图双实现）。每个 Task 类对应一个 Processor 类（通过 `UBXSettings::TaskProcessorMap` 配置）。
- **数据流 = 上下文数据作用域图**：任务通过 `WriteContextData/ReadContextData` 在 `FBXTLRunTimeData::DynamicDatas` 中以 `(Task全量索引, 数据Tag, 作用域)` 为键存取动态数据，作用域通过 `ScopeGraph` 形成父子链，支持跨任务数据传递与作用域隔离。

---

## 3. 模块职责与目录结构

```
BattleX/
├── BattleX.uplugin              # 插件清单
├── Config/Tags/BXGameplayTags.ini  # GameplayTag 定义
├── Content/                     # 美术资源（动画、MotionMatching 数据库等）
└── Source/
    ├── BattleX/                 # 运行时模块
    │   ├── BattleX.{h,cpp}      # 模块入口（注册 GameplayTag 搜索路径）
    │   ├── BattleX.Build.cs     # 构建配置
    │   ├── BXManager.{h,cpp}          # Manager 基类
    │   ├── BXSubSystem.{h,cpp}        # GameInstanceSubsystem（管理 Manager）
    │   ├── BXSettings.{h,cpp}         # DeveloperSettings（项目配置）
    │   ├── BXGameplayTags.{h,cpp}     # 原生 GameplayTag 声明
    │   ├── BXFunctionLibrary.{h,cpp}  # 工具函数库
    │   ├── BXEnums.h / BXStructs.h / BXCurves.h
    │   ├── Animation/           # 动画实例与动画库
    │   ├── Collision/           # 碰撞检测、形状组件、受击反应
    │   ├── Condition/           # 条件系统（决策树用）
    │   ├── DecisionTree/        # 决策树（含 BeatenTree / CombatTree）
    │   ├── Event/               # 事件系统
    │   ├── Gear/                # 装备系统（含冷兵器）
    │   ├── Lock/                # 锁定系统（占位）
    │   ├── Movement/            # 角色移动与 RootMotion
    │   ├── State/               # 状态机与行为代理
    │   ├── Task/                # 任务系统（Task + Processor + 具体任务）
    │   ├── Timeline/            # 时间轴系统（资产、管理器、组件）
    │   └── Unit/                # 投射物/法术场（占位）
    └── BattleXEditor/           # 编辑器模块
        ├── BattleXEditor.{h,cpp}     # 模块入口（注册资产类型/可视化器）
        ├── BattleXEditor.Build.cs
        ├── ComponentVisualizers/ # 形状组件可视化
        ├── CustomLayout/        # 自定义 Slate 控件
        ├── DecisionTreeEditor/  # 决策树图编辑器
        ├── SimpleEditor/        # 通用编辑器视口基类
        ├── TimelineBase/        # 通用时间轴 Slate 控件框架
        └── TimelineEditor/      # 时间轴资产编辑器（完整工具包）
```

---

## 4. 核心运行时系统

### 4.1 基础设施层

#### `UBXManager` ([BXManager.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/BXManager.h))

所有全局管理器的基类，提供 `Initialize()` / `Deinitialize()` 生命周期钩子。子类包括 `UBXTLManager`、`UBXEventManager`、`UBXConditionManager` 等。

#### `UBXSubSystem` ([BXSubSystem.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/BXSubSystem.h))

继承自 `UGameInstanceSubsystem`。在 `Initialize` 时按 `UBXSettings::ManagerClasses` 列表顺序实例化所有 Manager，存入 `ManagerMap`。提供模板方法 `GetManagerByClass<T>()`，支持按基类查询。所有 Manager 通过 `UBXManager::Get(UObject* WorldContext)` 静态方法访问。

#### `UBXSettings` ([BXSettings.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/BXSettings.h))

继承自 `UDeveloperSettings`（`Config=Game`）。关键配置项：

| 字段 | 作用 |
|---|---|
| `ManagerClasses` | 启动时按顺序创建的 Manager 类列表 |
| `CollisionDebugDuration` | 碰撞调试绘制时长（默认 1.0s） |
| `TaskProcessorMap` | Task 类 → Processor 类的映射（驱动任务执行） |
| `TaskCustomDataMap` | Task 类 → 自定义动态数据 UScriptStruct 的映射 |

#### `BXGameplayTags` ([BXGameplayTags.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/BXGameplayTags.h))

使用 `UE_DECLARE_GAMEPLAY_TAG_EXTERN` 声明的原生 GameplayTag 命名空间，是整个系统的" vocabulary"。主要包括：

- **事件**：`BXEvent_TimelineStarted/Closing`、`BXEvent_ChangeForbiddenBehavior`、`BXEvent_Behavior_Move/Rotate/Jump/Landed`
- **行为**：`BXBehavior_Locomotion_Move/Rotate`、`BXImmBehavior_Locomotion_Jump/Landed`
- **硬直状态**：Normal / Knockback / Knockdown / Prone / Ascending / Falling / Floating / Execution
- **攻击类型**：Light / KnockBack / KnockDown / Launch / Uppercut / AirKnockdown / Drag
- **攻击力度**：`BXAttackForce_0` ~ `BXAttackForce_5`
- **受击方位**：Front / Back / Left / Right
- **身体部位**：Head / UpperBody / LowerBody / Tail / Limbs1~8
- **装备类型/插槽/状态**：Weapon/Armor/Ornament；LeftHand/RightHand/Head/Chest...；Close/Open
- **关系/角色类型**：Self/Friendly/Neutral/Hostile；Player/NPC/Monster/BOSS
- **时间轴数据标签**：`BXTData_Transform`、`BXTData_ColResults1~5`
- **时间轴事件标签**：`BXTEvent_Start/End/Success/Failure/Trigger`

Tag 的 ini 搜索路径在 `FBattleXModule::StartupModule` 中注册为 `BattleX/Config/Tags`。

#### `UBXFunctionLibrary` ([BXFunctionLibrary.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/BXFunctionLibrary.h))

通用工具函数库，分类组织：
- **时间/ID**：`GetUniqueID`、`GetClientTimeSeconds`、`GetUtcMillisecond`、`GetGameMicrosecond`、`AlignTime`
- **GameplayTag**：`GetLastGameplayTagSubName`
- **反射拷贝**：`CopyStruct`、`CopyObject`、`CopyData`
- **时间轴**：`GetTaskFullIndex`、`GetSoftTaskFullIndex`（计算任务在资产中的全量索引）
- **几何数学**：`AreCollinear`、`PointToSegment/Circle/Rectangle/Sphere/Capsule/Cylinder/Box`、`SegmentToSegment/Sphere/Capsule/Box`
- **组件查找**：`GetSceneComponentBySocketName`、`GetSceneComponentByNameAndClass`
- **调试绘制（编辑器）**：`DrawDebugBoxInEditor` 等

---

### 4.2 Timeline 时间轴系统（技能核心）

时间轴系统是 BattleX 的核心。一个 **技能** 就是一个 `UBXSkillAsset`（派生自 `UBXTLAsset`），它由若干 **Section（时间片段）** 组成，每个 Section 内按时间排布若干 **Task（任务）**。

#### `UBXTLAsset` ([BXTLAsset.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/Timeline/BXTLAsset.h))

继承自 `UPrimaryDataAsset`。技能/时间轴资产的静态数据容器。

| 字段 | 类型 | 说明 |
|---|---|---|
| `ID` | `int32` | 资源 ID（用于运行时按 ID 查找） |
| `Sections` | `TArray<FBXTLSection>` | 运行片段列表 |
| `StartSectionIndexes` | `TArray<int32>` | 开始时执行哪些片段 |
| `AllResources` | `TArray<FSoftObjectPath>` | 收集的所有引用资源（用于异步加载） |
| `Graph` | `UEdGraph*` | 编辑器逻辑图（WITH_EDITORONLY_DATA） |

编辑器方法：`AddTask` / `RemoveTask` / `AddGroup` / `AddSection` / `ChangeSectionLoopTime` / `AlignTimeProperty` / `CleanInvalidTaskDependency` 等。

#### `FBXTLSection`（时间片段）

| 字段 | 说明 |
|---|---|
| `Duration` | 片段持续时间（≥0.5） |
| `LoopTime` | 循环次数（≤0 代表无限循环） |
| `ExtraTime` | 额外时间信息（前摇/后摇/连段窗口1~8，见 `EBXTLExtraType`） |
| `NextIndex` | 下一个时间轴索引（≥0 起效，用于片段跳转） |
| `TaskList` | 该片段的 Task 列表（Instanced） |
| `KeyFrames` | 关键帧列表（用于优化任务触发） |
| `Groups` | 编辑器内的任务分组（WITH_EDITORONLY_DATA） |

#### `UBXSkillAsset` ([BXSkillAsset.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/Timeline/BXSkillAsset.h))

继承自 `UBXTLAsset`，技能资产的语义别名。当前未额外扩展字段，但作为类型标识便于资产分类与编辑器注册。

#### `UBXTLManager` ([BXTLManager.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/Timeline/BXTLManager.h)) ★

**整个技能系统的运行时中枢**。继承自 `UBXManager` 与 `FTickableGameObject`，是单例（通过 `UBXSubSystem` 管理）。

**核心职责**：
- **资产索引**：`CollectTimelineAssetPath()` 启动时扫描 AssetRegistry，建立 ID → 资源路径映射表 `TimelineAssetMap`；提供 `GetTimelineAssetByID` / `GetSkillAssetByID`
- **运行时驱动**：每帧 `Tick` 调用 `UpdateTimeline(DeltaTime)`，遍历所有 `TimelineRTDatas` 更新运行数据
- **任务处理器缓存**：`TimelineTaskProcessors` 缓存 Task 类 → Processor 实例的映射（`GetTLTProcessorByTLTClass`）
- **垃圾清理**：`GCInterval`（默认 20s）周期性调用 `CleanTimelineTrash`

**关键方法**：

| 方法 | 说明 |
|---|---|
| `PlayTimeline(Asset, Owner, Context)` | 播放时间轴，生成唯一 ID，创建 `FBXTLRunTimeData`，立即更新一次 |
| `StopTimeline(ID, Reason)` | 停止指定时间轴，通知拥有者组件 |
| `ChangeTimelineTickRate(ID, Rate)` | 改变运行速率（影响所有运行中任务） |
| `UpdateTimeline(DeltaTime)` | 遍历更新所有运行数据，检查是否需要结束 |
| `FinishTimelineSection(...)` | 结束时间片段，处理待执行任务 |
| `ProcessTimelineSectionPendingTasks(...)` | 处理片段内待执行任务队列 |
| `ExecuteTimelineTask(...)` | 执行单个任务（创建 Processor、StartTask） |

#### `UBXTLComponent` ([BXTLComponent.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/Timeline/BXTLComponent.h))

挂载在 Actor 上的时间轴组件，是对 `UBXTLManager` 的薄封装。持有 `TimelineRunTimeDataIDs` 列表，提供 `PlayTimeline` / `StopTimeline` / `IsTimelineRunning` 等蓝图接口，并通过 `ScriptReceiveTimelineWillFinish` 暴露结束回调给蓝图。

#### 关键运行时数据结构（[BXTLStructs.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/Timeline/BXTLStructs.h)）

| 结构体 | 说明 |
|---|---|
| `FBXTLPlayContext` | 播放上下文：Instigator / Triggerer / LockParts / InputDatas |
| `FBXTLRunTimeData` | 时间轴运行时数据：Timeline / ID / Owner / RunningSections / **DynamicDatas** / **ScopeGraph** |
| `FBXTLSectionRTData` | 片段运行时数据：Index / RunTime / LoopCount / RunningTasks / PendingTasks / BroadcastTasks |
| `FBXTLTaskRTData` | 任务运行时数据：Task / Index / ParentScope / RunTime / NextTick / **DynamicData (FInstancedStruct)** |
| `FBXTLTaskHostingData` | 托管任务数据（脱离时间轴独立运行的任务） |
| `FBXTLDynamicDataSearchKey` | 动态数据查询键：(Index, Tag, Scope)，含哈希函数 |
| `FBXTLKeyFrame` | 关键帧：Time + Tasks 索引列表 |
| `FBXTLPendingTaskInfo` | 待执行任务信息：LocalIndex / ParentScope / Time |
| `FBXTLBroadcastTaskInfo` | 待广播任务信息：FullIndex / Tag |

**作用域图（ScopeGraph）** 是数据流的核心机制：`TMap<int64, FInt64Vector2>`，Key 是作用域 ID，Value 的 X 是父作用域 ID、Y 是 Task 全量索引。读取数据时沿父链向上查找，实现作用域隔离与数据继承。

#### `EBXTLFinishReason`（结束原因）

- `FR_EndOfLife`：寿终正寝（正常结束）
- `FR_Interrupt`：被中断
- `FR_PredictionFailure`：预测失败（网络回滚场景）

#### `EBXTLExtraType`（额外时间类型）

前摇 `ET_Anticipation`、后摇 `ET_Recovery`、连段窗口1~8 `ET_ComboWindow1~8`。

---

### 4.3 Task 任务系统（技能原子操作）

任务系统采用 **数据/执行分离** 设计：`UBXTask` 是配置数据（随资产序列化），`UBXTProcessor` 是运行时执行器（Transient，由 Manager 缓存）。

#### `UBXTask` ([BXTask.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/Task/BXTask.h)) ★

所有任务的抽象基类。`Abstract, Blueprintable, EditInlineNew`，可在细节面板内联编辑。

| 区域 | 字段 | 说明 |
|---|---|---|
| **Important** | `NetTypes` | 网络类型 Bitmask（Authority/Autonomous/Simulated，默认 7=全部） |
| | `TriggerTypes` | 触发方式 Bitmask（Timeline/SectionEnd/SectionInterrupt，默认 1=时间轴触发） |
| | `StartTime` | 开始时间 |
| | `LifeType` | 生命周期（Instant/Duration/Timeline/DurationTimeline） |
| | `Duration` | 持续时长（≥0.05） |
| | `TargetTypes` | 目标类型 Bitmask（Owner/Instigator/Trigger/LockTargets/CollisionResults） |
| | `CollisionInputDatas` | 碰撞数据来源（当目标含"碰撞目标"时） |
| **RunTimeData** | `InputDatas` | 数据读取声明：从哪个 Task 读取哪个 Tag 的数据 |
| | `OutputDatas` | 数据输出声明：生产名为 xxx 的 xxx 类型数据 |
| **Event** | `Events` | 事件触发表：Tag → `FBXTEvent`（含 bMulticast 与延迟触发） |
| **Editor** | `DisplayName` / `Annotation` | 编辑器显示 |
| | `TriggeredByList` | 被哪些任务触发（反向索引） |
| | `bNeedCollisionInput` | 是否需要碰撞输入 |

#### `UBXTProcessor` ([BXTProcessor.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/Task/BXTProcessor.h)) ★

任务执行器基类。`Transient, Abstract, Blueprintable`。每个 Task 子类对应一个 Processor 子类（通过 `UBXSettings::TaskProcessorMap` 配置）。

**生命周期**（由 `UBXTLManager::ExecuteTimelineTask` 调用）：
1. `StartTask(RTData, RTSData, RTTData)` → 按 Bitmask 调用 `Start`（C++）和 `ScriptStart`（BP），并自动广播 `BXTEvent_Start` 事件
2. `UpdateTask(...)` 每帧调用 → 累积 RunTime，按 `NextTick` 节流后调用 `Update` / `ScriptUpdate`
3. `EndTask(...)` → 调用 `End` / `ScriptEnd`

**`ExecuteFunctions` Bitmask**（默认 85 = NativeStart|NativeUpdate|NativeEnd|NativeChangeRate）：位 0~7 分别对应 C++/BP 的 Start/Update/End/ChangeRate，用于精细控制每个 Processor 启用哪些执行路径。

**核心全局 API**：

| 方法 | 说明 |
|---|---|
| `IsTaskCompleted(RTTData, OutReason)` | 判断任务是否结束 |
| `AddPendingTask(...)` | 添加被动触发任务（事件驱动） |
| `GenerateContextScope(...)` | 创建上下文数据作用域，返回新 Scope ID |
| `GetTargetComponentList / GetTargetActorList` | 根据 `TargetTypes` Bitmask 解析目标列表 |
| `AnalyzeTransformCreater / AnalyzeTransformCreaterList` | 解析坐标系创建器（见下） |
| `WriteContextData<T>` / `ReadContextData<T>` | 上下文数据读写（模板版 + BP CustomThunk 版） |
| `GetTargetTransformByWorldTime` | 按世界时间（20s 内）查询目标历史 Transform |

**`WriteContextData` / `ReadContextData`** 是任务间数据流的核心。写入时以 `(FullIndex, Tag)` 和 `(FullIndex, Tag, ParentScope)` 双键存入 `DynamicDatas`；读取时沿 `ScopeGraph` 父链向上查找，实现作用域隔离与继承。蓝图版本通过 `CustomThunk` + `execWriteContextData/execReadContextData` 直接操作蓝图 VM 栈，支持任意结构体。

#### `FBXTTransformCreater`（坐标系创建器）

强大的坐标系统构建工具，用于在任务中指定"在哪个坐标系下执行"。配置项：
- `OriginType`：原点选取（Owner/Instigator/Trigger/LockTarget/LockPart/Special/World）
- `OriginBoneName`：骨骼选择器（当原点是骨骼时）
- `OriginDelta`：原点偏移
- `XAxisType`：X 轴选取规则（默认使用原点轴向）
- `ConnectionType`：连线类型（2D/3D，从原点→X轴 或 反向）
- `PostOffset`：创建后的总体偏移

#### 具体任务类型

##### `UBXTPlayAnimation` ([BXTAnimation.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/Task/BXTAnimation.h))

播放动画任务。支持蒙太奇或动画库（按 Tag）。配置：`PlayRate`、`InterruptedBlendOut`、`DelayInterruptedByMove`（N 秒后可被移动中断）、`PlayComponentName`、`PlayAnimBehaviorTag`、`PlayPriority`、`bStopGroup`（终止同组蒙太奇）、`bEnableRootmotion`。

##### `UBXTCollision` / `UBXTTrackHitBox` / `UBXTTrackWeaponHitBox` ([BXTCollision.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/Task/BXTCollision.h))

碰撞检测任务族：
- **`UBXTCollision`**：基类，配置 `Interval`（检测间隔）、`CoolDown`（冷却）、`Limit`（数量限制）、`CharacterTags`/`RelationshipTags`（筛选）、`ObjectTypes`、`EngineFilter`（`FBXCFilter`）
- **`UBXTTrackHitBox`**：轨迹碰撞盒检测。`HitBoxTags`（碰撞盒标签）、`SweepAngleStep`（角度步进，越小越精确越耗性能）、`TrajectoryBone`（采样骨骼）、`TrajectoryOptimization`（冗余点去除规则：共线误差/方向误差/缩放误差）、`BoneSampledTrajectory`（骨骼模型空间轨迹）
- **`UBXTTrackWeaponHitBox`**：武器轨迹碰撞，增加 `WeaponSlot`（默认右手）

#### `EBXTLifeType`（生命周期）

- `L_Instant`：立即结束
- `L_Duration`：固定时长
- `L_Timeline`：时间轴控制
- `L_DurationTimeline`：固定时长且时间轴控制

#### `EBXTNetType` / `EBXTTriggerType` / `EBXTTargetType` / `EBXTProcessorFunction`

均为 Bitflags 枚举，分别控制网络端、触发时机、目标选取、Processor 执行函数的开关。这种 Bitmask 设计是 BattleX 性能可控的关键——每个任务可精确指定在哪些端、哪些时机、执行哪些函数。

---

### 4.4 State 状态系统（行为代理与状态机）

状态系统分为 **行为代理（BehaviorAgent）** 与 **状态机（StateMachine）** 两层，由 `UBXBehaviorComponent` 与 `UBXStateComponent` 分别承载。

#### `UBXBehaviorComponent` ([BXBehaviorComponent.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/State/BXBehaviorComponent.h))

行为管理组件。通过 GameplayTag 标识行为（如 `BXBehavior_Locomotion_Move`）。

| 配置 | 说明 |
|---|---|
| `ForbiddenBehaviorConfigs` | 行为禁用表：A 行为执行时禁用哪些行为 |
| `InterruptBehaviorConfigs` | 行为中断表：A 行为开始前中断哪些行为 |
| `BehaviorAgentConfigs` | 行为代理类：Tag → `UBXBehaviorAgent` 子类 |

**API**：`CheckForbiddenBehavior`、`ChangeForbiddenBehavior`、`CheckActiveBehavior`、`StartBehavior` / `StartBehaviorWithParameter`、`StopBehavior` / `StopBehaviorWithParameter`。

`FBXForbiddenBehaviorInformation` 记录禁用签名 `Sign` 与原因 `EBXForbiddenBehaviorReason`，支持多重禁用计数。

#### `UBXBehaviorAgent` ([BXBehaviorAgent.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/State/BehaviorAgent/BXBehaviorAgent.h))

行为代理基类。`Abstract, Blueprintable`。生命周期：`Initialize` → `StartBehavior` → `StopBehavior` → `Deinitialize`，并有 `CheckStartBehavior` 前置检查。每对方法都有 Native/Script 双版本，由 `BehaviorFunctions` Bitmask（默认 341）控制。

**具体代理**：
- `UBXBAMove` / `UBXBARotate`：移动/旋转行为
- `UBXBAJump` / `UBXBALanded`：跳跃/着陆行为

#### `UBXStateComponent` ([BXStateComponent.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/State/BXStateComponent.h))

状态机管理组件。`StateMachineConfig` 配置每个状态机类负责哪些状态 Tag。每帧 `TickComponent` 调用 `UpdateStateMachine` 驱动所有状态机。API：`EnterState` / `EnterStateWithParameter` / `ExitState` / `ExitStateWithParameter`。

#### `UBXStateMachine` ([BXStateMachine.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/State/StateMachine/BXStateMachine.h))

状态机基类。生命周期：`Initialize` / `Deinitialize` / `Update` / `EnterState(Tag, Param)` / `ExitState(Tag, Param)`。`StateMachineFunctions` Bitmask（默认 341）控制 Native/BP 执行路径。

**具体状态机**：`UBXSMStun`（硬直状态机），处理硬直状态的进入/退出。

#### `EBXForbiddenBehaviorReason`

行为禁用原因枚举（在 `BXStateEnums.h`），用于追踪为何某行为被禁用。

---

### 4.5 DecisionTree 决策树系统（受击 / 连段选择）

数据驱动的图评估框架，用于"应该播哪个受击反应"、"应该接哪个连段技能"等决策。

#### 类层次

```
UObject
 ├── UBXCondition                 (条件基类，Condition/BXCondition.h)
 ├── UBXDecisionTreeNode          (决策节点)
 ├── UBXDecisionTreeEdge          (决策边)
 ├── UBXDecisionTreeType          (树类型配置：允许的节点/边/执行器类)
 └── UBXDecisionTreeActuator      (运行时评估器)

UPrimaryDataAsset
 └── UBXDecisionTreeTemplate      (决策树资产：RootNodes + TreeEdges + TreeType)
      ├── UBXBeatenTreeTemplate   (受击树)
      └── UBXCombatTreeTemplate   (战斗连段树)
```

#### `UBXDecisionTreeTemplate` ([BXDecisionTreeTemplate.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/DecisionTree/BXDecisionTreeTemplate.h))

资产容器。`RootNodes`（根节点列表）、`TreeEdges`（所有边的扁平数组）、节点通过 `OutEdges`/`InEdges` 的整数索引引用边。编辑器方法 `InitByEditor` / `InitSaver` / `AddEdgeMessage` / `RefreshLogicMessage` 维护图结构。

#### `UBXDecisionTreeActuator` ([BXDecisionTreeActuator.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/DecisionTree/BXDecisionTreeActuator.h))

运行时评估器。核心方法 `GetBestNode<T>(WorldContext, Template, Param)`：
- C++ 模板版：转发 `T::StaticStruct()` + `&Param` 到 `InternalGetBestNode`
- 蓝图版：`CustomThunk` + `execGetBestNode` 从 BP VM 栈提取结构体

**执行流程（`TravelDecisionTree`，深度优先 DFS）**：
1. 若节点条件不满足，返回 nullptr
2. 当前节点作为候选结果
3. 遍历 `OutEdges`，找到第一条条件满足的边
4. 递归向下，若子节点返回非空则替换结果
5. 返回最深的有效匹配（或当前节点本身）

#### `UBXCondition` / `UBXConditionManager`

条件基类有 `bNot` 反转标志。`UBXConditionManager` 通过反射将条件类映射到 UFunction 进行评估，配置 `ConditionToFunctionConfig`，运行时缓存 `ConditionToFunctionMap`。

#### BeatenTree（受击树）

- **参数** `FBXBeatenTreeCondParam`：`StunState` / `AttackType` / `AttackForce` / `AttackRPos`（全为 GameplayTag）
- **条件** `UBXBeatenTreeCondition`：四个可选 TagContainer 匹配器，每个由 `bCheck*` 开关启用
- **节点** `UBXBeatenTreeNode`：受击反应输出——新 `StunState`、`AnimLibraryTag`、`MotionDistance`（X=时间, Y=平面距离, Z=高度）、`MotionCurve`
- **模板** `UBXBeatenTreeTemplate`：编辑期计算 `EdgeShortestDepth` 与 `TotalSkillNum` 用于可视化

#### CombatTree（战斗连段树）

- **节点**：
  - `UBXCombatTreeRootNode`：不可删除的 "Start" 根节点，仅输出引脚可连
  - `UBXCombatTreeNode`：输出 `ReleaseSkillAsset`（软引用 `UBXSkillAsset`）+ `bCheckReleaseCondition`
- **边** `UBXCombatTreeEdge`：携带 `ComboWindows`（连段窗口对应的技能资产列表），表示"从技能 A 满足条件可过渡到技能 B"
- **模板** `UBXCombatTreeTemplate`：薄封装，仅设置 `TreeType = UBXCombatTreeType`

---

### 4.6 Gear 装备系统（含冷兵器）

#### `ABXGear` ([BXGear.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/Gear/BXGear.h))

装备 Actor 基类（Abstract）。持有 `GearType`（Tag）、`GearData`（资产）、`OwnerComponent`。四个生命周期阶段（每阶段 Pre/Post + C++/BP 双实现）：
- **Equip/Unequip**：装备/卸载
- **Using/Unusing**：使用/收起
- **ChangeState**：状态切换（如开/合）

`EquipFunctions`/`UseFunctions`/`ChangeStateFunctions` Bitmask（默认 85）控制 Native/BP 执行。通过 `AttachToSocket()` 与 `AttachmentConfigs` 管理附着。

#### `UBXGearComponent` ([BXGearComponent.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/Gear/BXGearComponent.h))

每 Actor 装备库存组件。`SlotMaxSize`、`EquipGears`（插槽→装备列表映射）、`UsingGearIndexs`。9 个多播委托（Pre/PostEquip/Unequip、Pre/PostUsing/Unusing、ChangeGearState）。API：`GetEquipGearList`、`SwitchUsingGear`、`ChangeUsingGearState`、`ChangeEquipGear` / `ByClass` / `ByData`、`UnequipAllGears`。

#### `ABXColdWeapon` ([BXColdWeapon.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/Gear/BXColdWeapon.h))

冷兵器（近战武器）特化。持有 `UBXShapeComponent`（HitBoxComponent）用于命中检测。提供 `GetHitBoxComponent` / `GetHitBoxTransform`。

#### `BXGearEnums.h`

Bitflags 枚举：`EBXEquipGearFunction`、`EBXUseGearFunction`、`EBXChangeGearStateFunction`，每对 Native/BP 变体对应一个生命周期阶段。

---

### 4.7 Movement 移动系统

#### `UBXCharacterMovementComponent` ([BXCharacterMovementComponent.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/Movement/BXCharacterMovementComponent.h))

继承 `UCharacterMovementComponent`。新增：
- `bProactiveMoving` / `bProactiveRotating`：主动移动/旋转标志
- **轨迹历史缓冲**：`TrajectoryPoints`（默认记录 20s），`RecordTime`，`CleanInterval`/`CleanTimer` 周期清理
- `GetHistoryTransformByTime(InTime)`：按时间查询历史 Transform（供 `UBXTProcessor::GetTargetTransformByWorldTime` 使用）

重写：`CalcVelocity`、`ComputeSlideVector`、`PhysicsRotation`、`ProcessLanded`、`CanAttemptJump`、`DoJump`。

#### `FBXRootMotionSource_Uppercut` ([BXRootMotionSource.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/Movement/BXRootMotionSource.h))

自定义网络复制的 RootMotion 源，用于升龙拳等弧线位移技能。配置：`Direction`、`Distance`、`Height`、`MoveTimeMappingCurve`，可选朝向插值（`bChangeOrientation`、`StartOrientation`/`EndOrientation`、`RotateTimeMappingCurve`）。实现 `PrepareRootMotion` / `Clone` / `Matches` / `UpdateStateFrom` / `NetSerialize`。

---

### 4.8 Collision 碰撞系统

#### `UBXCollisionLibrary` ([BXCollision.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/Collision/BXCollision.h))

蓝图函数库，提供静态碰撞查询 API：`SphereCheck`、`CapsuleCheck`、`CylinderCheck`、`HollowCylinderCheck`、`BoxCheck`、`SectorCheck`、`CheckCollisionResult`、`CombineCollisionResults`。

#### 关键结构体

- **`FBXCFilter`**：过滤器。`bIgnoreSelf`、`ClassTypes`/`IgnoreClassTypes`、`ActorTags`/`IgnoreActorTags`、`ComponentTags`/`IgnoreComponentTags`
- **`FBXCParameter`**：请求者、起止位置/旋转、缩放
- **`FBXCStrategy`**（基类）+ 子类：`FBXCSSphere`/`FBXCSCapsule`/`FBXCSCylinder`/`FBXCSHollowCylinder`/`FBXCSBox`/`FBXCSSector`。Sweep 型形状含 `AngleStep`（越小越精确越耗性能）与 `bUseSweep`
- **`EBXCDirection`**：扇形精细检测方向（RightToLeft/LeftToRight/InToOut）

#### `UBXShapeComponent` ([BXShapeComponent.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/Collision/BXShapeComponent.h))

持有 `ShapeInformations`（Tag → `FBXShapeInformation`），提供 `GetShapeTransformByTag`。用于在武器/角色上定义具名形状区域。

#### `UBXHitReactionComponent` ([BXHitReactionComponent.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/Collision/BXHitReactionComponent.h))

基于身体部位的受击反应组件。`BodyPartConfigs`（Tag → `FBXBodyPartConfig`）配置、`BodyPartRTDatas`（Tag → `FBXBodyPartRTInformation`）运行时数据。API：`GetBodyPartByType`、`GetBodyPartByComponentAndBone`。

---

### 4.9 Event 事件系统

#### `UBXEventManager` ([BXEventManager.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/Event/BXEventManager.h))

继承 `UBXManager`。轻量级事件系统，以 GameplayTag 为事件名，支持两种事件：
- **全局事件**：仅以 `FGameplayTag` 为键
- **单播事件**：以 `FBXESingleKey`（EventName + InitiatorUniqueID）为键，支持按发起者定向

通过反射将事件参数结构体分发到 UFunction。`FBXEventManagerODL` 实现 `FUObjectDeleteListener`，在注册的 UObject 被销毁时自动清理关联。`DefinedEvents`（Tag → UScriptStruct*）声明事件参数类型。

API：`RegisterGlobalEvent` / `UnregisterGlobalEvent` / `BroadcastGlobalEvent`（模板/UScriptStruct/BP CustomThunk 三种重载）；单播版同构。

#### 事件参数结构体（[BXEventStructs.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/Event/BXEventStructs.h)）

- `FBXEventEmpty`：无载荷
- `FBXEventForbiddenBehavior`：`BehaviorTag` + `bForbidden` + `Reason`（`EBXForbiddenBehaviorReason`）
- `FBXEventBehaviorDefault`：`bActive`

---

### 4.10 Condition 条件系统

#### `UBXCondition` ([BXCondition.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/Condition/BXCondition.h))

条件基类。`Abstract, Blueprintable, EditInlineNew`。`bNot` 反转标志。编辑器 `GetDescription()` 用于显示。

#### `UBXConditionManager` ([BXConditionManager.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/Condition/BXConditionManager.h))

单例管理器。`CheckCondition<T>(Condition, Param)` 模板版（转发 `UScriptStruct*` + 指针到 `InternalCheckCondition`）、BP CustomThunk 版、结构体/类型+地址版。配置 `ConditionToFunctionConfig`（条件类 → UFunction + 参数名），运行时缓存 `ConditionToFunctionMap`。`FBXConditionFunctionParameter` 持有反射的 `UFunction*` 与参数名列表。

---

### 4.11 Animation 动画系统

#### `UBXAnimInstance` ([BXAnimInstance.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/Animation/BXAnimInstance.h))

自定义 AnimInstance，对接技能系统的动画播放需求。

#### `UBXAnimationLibrary` ([BXAnimationLibrary.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/Animation/BXAnimationLibrary.h))

动画库，支持按 GameplayTag 标签检索动画（`UBXTPlayAnimation` 的 `AAT_Library` 模式使用）。

---

### 4.12 Unit / Lock 系统（占位）

`Source/BattleX/Unit/`（`BXUnit`、`BXProjectile`、`BXMissile`、`BXSpellField`、`BXPMovementComponent`）与 `Source/BattleX/Lock/`（`BXLockComponent`）目前为**空文件占位**，仅保留目录与文件骨架，尚未实现。计划用于投射物、追踪导弹、法术场、目标锁定等功能。

---

## 5. 编辑器模块

`BattleXEditor` 模块（`UncookedOnly`，`PostEngineInit` 加载）提供完整的资产编辑器与可视化工具。

### 5.1 模块入口

`FBattleXEditorModule::StartupModule` 完成三类注册：
1. **组件可视化器**：`GUnrealEd->RegisterComponentVisualizer` 注册 `UBXShapeComponent` 的 `FBXShapeComponentVisualizer`
2. **资产类型动作**（注册到 `EAssetTypeCategories::Gameplay`）：
   - `FAsssetTypeActions_BXTLAsset`（时间轴资产）
   - `FAsssetTypeActions_BXSkillAsset`（技能资产）
   - `FBXDecisionTreeAssetTypeActions` / `FBXBeatenTreeAssetTypeActions` / `FBXCombatTreeAssetTypeActions`（三种决策树）
3. **自定义缩略图渲染器**：`UBXTLAssetThumbnailRenderer`、`UBXDTAssetThumbnailRenderers`

### 5.2 时间轴编辑器（TimelineEditor）

基于 `FWorkflowCentricApplication` 的完整资产编辑器工具包。

**`FBXTLEditor`**（[BXTLEditor.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleXEditor/TimelineEditor/BXTLEditor.h)）：
- 继承 `FWorkflowCentricApplication`、`FGCObject`、`FEditorUndoClient`
- 持有编辑的 `UBXTLAsset`、预览场景、图编辑器、工具栏、命令绑定、任务选择、Manager 缓存

**标签页**（`BXTLEditorTabs`）：
| Tab | 内容 |
|---|---|
| `Graph` | `SGraphEditor` 显示 `UBXTLGraph`（任务节点 + 过渡节点） |
| `Viewport` | `SBXTLEditorViewport`（基于 `SSimpleEditorViewport`）承载预览场景 |
| `Timeline` | `SBXTLAssetTimelineTab`，每个 Section 持有一个 `STimeline` + `FBXTLController` |
| `TaskDetails` | 任务细节面板（`IDetailsView`） |
| `AssetDetails` | 资产细节面板（`SSingleObjectDetailsPanel`） |

**图子系统**（`Graph/`）：
- `UBXTLGraph : UEdGraph`：镜像资产的 Task 列表，自动生成/连接节点
- `UBXTLGraphNode : UEdGraphNode`：代表一个 `UBXTask`，含 5 类引脚（执行/事件/碰撞输入/输入数据/输出数据）的 `FBXTLGNodePin`
- `UBXTLGraphSchema`：连接规则、引脚颜色、`FBXTLGCreatTaskNode` 上下文菜单动作
- 过渡节点/引脚：表示 Task 间的事件流

**预览子系统**（`Preview/`）：
- `FBXTLPreviewScene : FAdvancedPreviewScene`：预览世界，可加载外部地图、生成玩家/目标 Actor
- `FBXTLPreviewProxy`：运行时驱动，通过 `UBXTLComponent` 实际播放/暂停/停止时间轴资产
- `UBXTLPreviewActor`：描述生成配置（类、Transform、装备、锁定部位）
- `BXTLPreviewGameMode` / `BXTLPreviewGameState`：预览专用 GameMode

**命令**（`BXTLEditorCommands`）：Play / Pause / Stop / Step / Bake / ResetWorld / ShowCollision / RefreshTimelineAssetProperty / AutoOptimize

**事件**（`BXTLEditorDelegates.h`）：`RefreshPanelEvent`、`PreviewChangedEvent`、`TaskSelectedEvent`

### 5.3 决策树编辑器（DecisionTreeEditor）

**`FBXDTEditor : FAssetEditorToolkit, FNotifyHook, FGCObject`**（[BXDTEditor.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleXEditor/DecisionTreeEditor/BXDTEditor.h)）：
- 管理 `SGraphEditor` 视口栈（支持钻入子图并返回）
- 属性/编辑器设置细节面板
- 复制/剪切/粘贴/复制/重命名命令
- 类过滤器限制节点为 `UBXDecisionTreeType` 子类
- 标签页：Viewport / Details / EditorSettings

**图**：`UBXDTEditorGraph`（容器）、`UBXDTEditorSchema`（含 `NewNode`/`NewEdge` 动作、断链菜单、落点引脚）、`UBXDTEditorGraphNode`/`UBXDTEditorGraphEdge` 及其 `SGraphNode`/`SGraphEdge`。

### 5.4 时间轴基础控件（TimelineBase）

**资产无关的通用时间轴 Slate 框架**，TimelineEditor 通过子类化复用：

- **`FTimelineController`**（[TimelineController.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleXEditor/TimelineBase/TimelineController.h)）：抽象控制器。管理 `RootTracks`/`SelectedTracks`、`ViewRange`/`WorkingRange`、播放范围、播放头位置、帧率、Track 选择、`OnTracksChanged` 事件
- **`FTimelineTrack`**（[TimelineTrack.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleXEditor/TimelineBase/TimelineTrack.h)）：Track 基类，树形结构，含 RTTI 宏系统
- **`STimeline`**（[STimeline.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleXEditor/TimelineBase/STimeline.h)）：时间轴 UI，持有 `SOutliner` + `STrackArea` + `FTimeSliderController` + 顶部 `ITimeSlider`，支持网格、吸附、显示格式切换（帧/百分比/次要）
- **`STrack`**：Track 在 TrackArea 中的包装控件
- 辅助控件：`SOutliner`、`SOutlinerItem`、`STrackArea`、`STimelineOverlay`、`STimelineSplitterOverlay`、`STimelineTransportControls`、`TimeSliderController`

**TL 特化**：`FBXTLController : FTimelineController` 重写 `RefreshTracks`、`GetScrubPosition`、`SetScrubPosition`、`GetPlayLength`、`GetFrameRate` 等，添加任务选择、任务分组、复制粘贴、模板导入导出、图节点创建等 TL 专属操作。TL 专属 Track 子类位于 `TimelineEditor/Timeline/`（`BXTLTaskGroupTrack`、`BXTLTaskTrackPanel`、`SBXTLTaskTrack`、`SBXTLTaskTrackNode`、`SBXTLExtraTrack` 等）。

### 5.5 其他编辑器子模块

- **ComponentVisualizers**：`FBXShapeComponentVisualizer`，在视口中绘制 `UBXShapeComponent` 形状（`DrawCircle` / `DrawHalfCircle`）
- **SimpleEditor**：`SSimpleEditorViewport : SEditorViewport` + `FSimpleEditorViewportClient`，通用可复用的编辑器视口基类，接收 `FEditorViewportParameter`（预览场景 + 资产工具包 + 视口索引）
- **CustomLayout**：共享自定义 Slate 控件，如 `SBXTLTaskGroupPicker`

---

## 6. 依赖关系

### 6.1 模块依赖（`BattleX.Build.cs`）

**PublicDependencyModuleNames**（运行时核心）：
- `Core` / `CoreUObject` / `Engine` / `InputCore` / `EnhancedInput`
- `Slate` / `SlateCore` / `DeveloperSettings`

**PrivateDependencyModuleNames**：
- `GameplayTags` / `StructUtils`

**PrivateIncludePaths**：暴露了所有子目录（`Task`、`Unit`、`Lock`、`Gear`、`State`、`Event`、`Movement`、`Timeline`、`Animation`、`Collision`、`Condition`、`DecisionTree`）

### 6.2 插件依赖

`StructUtils`（InstancedStruct）、`Niagara`（特效）、`Chooser`（选择器）、`AnimationWarping`/`MotionWarping`（动画扭曲）、`PoseSearch`/`MotionTrajectory`（PoseSearch 动画匹配）、`EnhancedInput`

### 6.3 系统间依赖

```
Timeline (核心) ──依赖──> Task (原子操作)
    │                        │
    │                        ├──依赖──> Collision (碰撞检测)
    │                        ├──依赖──> Animation (动画播放)
    │                        ├──依赖──> Movement (RootMotion)
    │                        └──依赖──> Event (事件广播)
    │
    ├──依赖──> State (行为/状态机) ──依赖──> Event
    │
    └──被依赖──< DecisionTree (决策) ──依赖──> Condition
                                          └──> Skill (引用技能资产)

Gear (装备) ──依赖──> Collision (形状组件)
Movement ──被依赖──< Task (RootMotion 任务)
            └──被依赖──< BXFunctionLibrary (历史轨迹查询)

基础设施 (Manager/SubSystem/Settings/Tags/Func) <──所有系统依赖
```

### 6.4 关键映射配置

`UBXSettings` 中的两张映射表是系统的"接线图"：
- **`TaskProcessorMap`**：Task 类 → Processor 类。`UBXTLManager::Initialize` 时加载并缓存为 `TimelineTaskProcessorTypeMap`，运行时通过 `GetTLTProcessorByTLTClass` 查询 Processor 实例
- **`TaskCustomDataMap`**：Task 类 → 自定义动态数据 UScriptStruct。缓存为 `TimelineTaskTypeMap`

---

## 7. 项目运行方式

### 7.1 环境要求

- **Unreal Engine 5**（推荐 5.3+，因使用 `PoseSearch`、`MotionTrajectory` 等 UE5 特性）
- **C++ 编译环境**：Windows 上需 Visual Studio 2022 + Windows SDK
- **依赖插件**：`StructUtils`、`Niagara`、`Chooser`、`AnimationWarping`、`PoseSearch`、`MotionTrajectory`、`MotionWarping`、`EnhancedInput`

### 7.2 集成到游戏项目

1. 将整个 `BattleX` 文件夹放入项目的 `Plugins/` 目录
2. 重启项目，在插件浏览器中确认 `BattleX` 已启用
3. 在 `DefaultGame.ini` 中配置 `UBXSettings`（或在项目设置面板的 "BattleX Settings" 中编辑）：
   - `ManagerClasses`：添加 `UBXTLManager`、`UBXEventManager`、`UBXConditionManager` 等
   - `TaskProcessorMap`：为每个 Task 子类指定 Processor 子类
   - `TaskCustomDataMap`：为需要自定义动态数据的 Task 指定 UScriptStruct

### 7.3 创建技能资产

1. 在内容浏览器中右键 → Miscellaneous → **BattleX Skill Asset**（或 Timeline Asset）
2. 双击打开自定义编辑器（见 [5.2](#52-时间轴编辑器timelineeditor)）
3. 在 Timeline 标签页添加 Section，配置时长/循环/连段窗口
4. 从任务面板拖入 Task（如 `BXTPlayAnimation`、`BXTTrackHitBox`），配置 StartTime/Duration/目标类型
5. 在 Graph 标签页连接任务的事件引脚，建立数据流
6. 在 Viewport 标签页预览播放效果

### 7.4 运行时触发技能

在角色 Actor 上挂载 `UBXTLComponent`，蓝图调用：

```cpp
// C++ 示例
FBXTLPlayContext Context;
Context.Instigator = this;
Context.Triggerer = this;
// Context.LockParts / Context.InputDatas 可选
UBXTLComponent* TLComp = FindComponentByClass<UBXTLComponent>();
int64 TimelineID = TLComp->PlayTimeline(SkillAsset, Context);
```

```blueprint
// 蓝图示例
Play Timeline (InAsset: SkillAsset, InContext: Context) → Returns TimelineID
```

### 7.5 决策树使用

1. 创建 `BXBeatenTreeTemplate` 或 `BXCombatTreeTemplate` 资产
2. 在图编辑器中构建节点（条件 + 输出）与边（条件 + 过渡）
3. 运行时调用 `UBXDecisionTreeActuator::GetBestNode<T>(WorldContext, Template, Param)` 获取最佳节点，读取其输出（如 `AnimLibraryTag` 或 `ReleaseSkillAsset`）

---

## 8. 关键数据流与执行流程

### 8.1 技能播放完整流程

```
1. 游戏代码: TLComp->PlayTimeline(Asset, Context)
       │
       ↓
2. UBXTLManager::PlayTimeline
   ├─ 生成 NewID = GetUniqueID()
   ├─ 创建 FBXTLRunTimeData，填充 Owner/Instigator/Triggerer/LockParts/InputDatas
   ├─ 按 Asset->StartSectionIndexes 创建 FBXTLSectionRTData
   └─ 立即调用 InternalUpdateTimeline(NewData, 0.0f)
       │
       ↓
3. UBXTLManager::Tick (每帧)
   └─ UpdateTimeline(DeltaTime) → 遍历 TimelineRTDatas
       │
       ↓
4. InternalUpdateTimeline(RTData, DeltaTime)
   ├─ 遍历 RunningSections
   │   ├─ 累积 SectionData.RunTime
   │   ├─ ProcessTimelineSectionPendingTasks (触发到时间的待执行任务)
   │   ├─ 遍历 RunningTasks → Processor->UpdateTask
   │   │   └─ UpdateTask 内部按 NextTick 节流，调用 Update/ScriptUpdate
   │   ├─ 检查 Section 是否结束（RunTime >= Duration 且 LoopCount 达标）
   │   │   └─ FinishTimelineSection → Processor->EndTask → 跳转 NextIndex
   │   └─ 检查 ForceJumpSection（强制跳转）
   └─ 检查整个时间轴是否结束（bEarlyFinish 或所有 Section 已结束）
       └─ 通知 Owner 的 UBXTLComponent::ReceiveTimelineWillFinish
```

### 8.2 任务执行流程

```
ExecuteTimelineTask(RTData, RTSData, TaskIndex, NetMode, RoleType, StartOffset, ParentScope)
   │
   ├─ 获取 Task = Section.TaskList[TaskIndex]
   ├─ 检查 NetTypes Bitmask 是否匹配当前 NetMode/Role
   ├─ 检查 TriggerTypes Bitmask 是否匹配触发时机
   ├─ 创建 FBXTLTaskRTData，初始化 DynamicData (FInstancedStruct)
   ├─ 加入 RTSData.RunningTasks
   │
   ├─ Processor = GetTLTProcessorByTLTClass(Task->GetClass())
   │       └─ 查缓存 TimelineTaskProcessors，未命中则从 TaskProcessorMap 创建
   │
   └─ Processor->StartTask(RTData, RTSData, RTTData)
       ├─ 按 ExecuteFunctions Bitmask 调用 Start (C++) 和 ScriptStart (BP)
       └─ 自动 AddPendingTask 广播 BXTEvent_Start 事件
           └─ 触发其他 Task 的 Events 表中匹配 Tag 的被动任务
```

### 8.3 任务间数据流

```
Task A (输出数据)                  Task B (输入数据)
─────────────────                  ─────────────────
OutputDatas:                       InputDatas:
  Tag: "BXTData_Transform"           DataTask: Task A
  StructType: FTransform             DataTag: "BXTData_Transform"
        │                                  │
        └──────────────┬───────────────────┘
                       ↓
              WriteContextData<T>(RTData, FullIndexA, Tag, ParentScope, Data)
                       │
                       ↓
              RTData.DynamicDatas.Add(
                  Key=(FullIndexA, Tag, ParentScope),
                  Value=FInstancedStruct::Make(Data))
                       │
                       ↓
              ReadContextData<T>(RTData, FullIndexA, Tag, ParentScopeB)
                  └─ 沿 ScopeGraph 父链查找匹配的作用域
                  └─ 返回 T* 或 nullptr
```

### 8.4 决策树评估流程

```
GetBestNode<FBXBeatenTreeCondParam>(WorldCtx, Template, Param)
   │
   ↓
InternalGetBestNode(WorldCtx, Template, StructType, ParamAddr)
   ├─ 遍历 Template->RootNodes
   └─ TravelDecisionTree(WorldCtx, RootNode, StructType, ParamAddr)  [DFS]
       │
       ├─ 1. RootNode->CheckCondition(Param) ?
       │      └─ false → return nullptr
       │
       ├─ 2. Result = RootNode  (当前节点作为候选)
       │
       └─ 3. 遍历 RootNode->OutEdges
            ├─ Edge->CheckCondition(Param) ?
            │   └─ true → childResult = TravelDecisionTree(Edge->EndNode, ...)
            │       └─ childResult 非空 → Result = childResult; break
            │
            └─ return Result  (最深有效匹配，或当前节点)
```

---

## 9. 优化建议

### 9.1 性能优化

#### 9.1.1 时间轴 Manager 的 Tick 优化

**现状**：`UBXTLManager` 每帧遍历所有 `TimelineRTDatas`，每个 Section 遍历所有 `RunningTasks`。

**建议**：
- **引入 Task 调度优先队列**：将 `PendingTasks` 改为按触发时间排序的最小堆，避免每帧线性扫描。当前 `FBXTLSectionRTData::PendingTasks` 是 `TArray`，可改为 `TArray` + `Heapify` 或自定义二叉堆。
- **空时间轴早退**：`UpdateTimeline` 开头若 `TimelineRTDatas.Num() == 0` 直接返回，避免无意义 Tick 开销。
- **按需 Tick 分级**：对 `NextTick` 较大的任务，可考虑分组 Tick（如每 N 帧处理一次低频任务），减少每帧 Processor 调用次数。

#### 9.1.2 任务处理器缓存

**现状**：`TimelineTaskProcessors` 按 Task **类** 缓存单例 Processor，所有同类任务共享一个 Processor 实例。

**建议**：
- **确认 Processor 无状态**：确保所有 Processor 子类不持有任务级状态（动态数据应通过 `FBXTLTaskRTData::DynamicData` 传递，而非 Processor 成员变量）。当前设计已遵循此原则，但应在代码规范中明确约束。
- **Processor 池化**：若有 Processor 需要任务级临时缓冲，考虑对象池而非成员变量。

#### 9.1.3 动态数据查询优化

**现状**：`FBXTLRunTimeData::DynamicDatas` 是 `TMap<FBXTLDynamicDataSearchKey, FInstancedStruct>`，每次 `ReadContextData` 沿 `ScopeGraph` 父链查找。

**建议**：
- **缓存查找结果**：对高频读取的数据，可在 `FBXTLTaskRTData` 中缓存最近一次读取的 `(Key, Value)`，命中则跳过 ScopeGraph 遍历。
- **`FBXTLDynamicDataSearchKey` 哈希优化**：当前 `GetTypeHash` 只 combine 了 Index 和 Tag，未包含 Scope。若冲突频繁，可加入 Scope 哈希；但需权衡——当前设计允许按 (Index, Tag) 快速定位再筛选 Scope，可能是有意为之。建议加 profiling 验证。

#### 9.1.4 碰撞检测优化

**现状**：`UBXTTrackHitBox` 的 `SweepAngleStep` 控制轨迹采样精度，`TrajectoryOptimization` 控制冗余点去除。

**建议**：
- **LOD 化碰撞检测**：根据目标距离/重要性动态调整 `SweepAngleStep`（远处用大步长，近处用小步长）。
- **多线程化**：将碰撞检测任务分派到 Task Graph 并行执行（当前在 `UBXTLManager::Tick` 主线程串行）。可利用 UE 的 `ParallelFor`。
- **HitResult 对象池**：`FBXTHitResults::Results` 预分配容量（已 `Empty(10)`），可考虑跨任务复用避免反复分配。

#### 9.1.5 GameplayTag 查询优化

**现状**：系统大量使用 GameplayTag 作为键，`BXGameplayTags` 命名空间提供原生 Tag 缓存。

**建议**：
- **避免运行时字符串构造 Tag**：所有 Tag 应通过 `BXGameplayTags::XXX` 原生句柄引用，避免 `FGameplayTag::RequestGameplayTag(TEXT("..."))` 运行时查找。
- **Tag 容器预排序**：对频繁匹配的 `FGameplayTagContainer`（如 `CharacterTags`、`RelationshipTags`），可在编辑期排序以加速 `HasTag` 查询。

### 9.2 架构优化

#### 9.2.1 Unit / Lock 系统实现

**现状**：`Unit/`（`BXUnit`、`BXProjectile`、`BXMissile`、`BXSpellField`、`BXPMovementComponent`）与 `Lock/`（`BXLockComponent`）为空占位。

**建议**：
- **优先实现 `BXLockComponent`**：动作游戏的锁定目标是核心玩法，应尽早实现。可参考 `UBXHitReactionComponent` 的身体部位配置模式。
- **Unit 系统分层设计**：`BXUnit` 作为基类持有生命周期与碰撞，`BXProjectile`（直线）、`BXMissile`（追踪）、`BXSpellField`（区域）作为特化。`BXPMovementComponent` 实现 ProjectileMovement 的技能定制版（支持曲线速度、引力修正）。
- **与 Task 系统集成**：创建 `UBXTSpawnUnit` 任务类型，让技能能数据驱动地生成投射物。

#### 9.2.2 网络同步完善

**现状**：Task 通过 `NetTypes` Bitmask 区分端，`FBXRootMotionSource_Uppercut` 支持 `NetSerialize`。

**建议**：
- **预测与回滚**：`EBXTLFinishReason::FR_PredictionFailure` 已预留，但缺少完整的预测/回滚实现。建议：
  - 权威端推送 Section 跳转事件
  - 自主端预测执行，收到权威端纠正时触发 `FR_PredictionFailure` 回滚
  - 模拟端仅播放动画与特效，不执行碰撞判定
- **时间轴状态同步**：考虑将 `FBXTLRunTimeData` 的关键字段（当前 Section、RunTime）纳入网络同步，避免各端漂移。
- **Task 执行结果同步**：碰撞命中结果应权威端判定后广播，避免各端重复判定导致不一致。

#### 9.2.3 调试与可视化增强

**现状**：`UBXTLManager` 有 `bShowCollision` 调试开关，`UBXFunctionLibrary` 有编辑器调试绘制。

**建议**：
- **运行时技能调试 HUD**：实现 `UBXSettings` 中的调试开关，运行时显示当前播放的时间轴 ID、Section、RunTime、活跃任务数。
- **时间轴录制回放**：录制一段时间轴执行快照，离线回放分析性能瓶颈。
- **决策树可视化调试**：运行时高亮当前评估路径，显示每个节点条件的判定结果。

### 9.3 工程质量优化

#### 9.3.1 代码规范

**建议**：
- **统一 Bitmask 默认值文档化**：`ExecuteFunctions=85`、`BehaviorFunctions=341`、`StateMachineFunctions=341` 等魔法数字应在注释中说明哪些位被启用（如 341 = 1|4|16|64|256 = NativeInit|NativeCleanup|NativeUpdate|NativeEnter|NativeExit）。
- **`UBXFunctionLibrary::GetUniqueID` 实现**：当前被广泛用于 ID 生成，应确认其线程安全性（若 Manager Tick 与异步任务并发调用需加锁或使用原子操作）。
- **空文件清理**：`Unit/` 与 `Lock/` 的空文件应至少包含类声明骨架或 `// TODO` 注释，避免被误认为已完成。

#### 9.3.2 测试覆盖

**建议**：
- **核心系统单元测试**：为 `UBXTLManager` 的 Section 跳转逻辑、`UBXDecisionTreeActuator` 的 DFS 评估、`UBXTProcessor` 的 ContextData 读写编写自动化测试。
- **性能回归测试**：建立标准技能资产集，CI 中监控 Tick 耗时、内存占用、GC 频率。
- **网络一致性测试**：自动化测试权威端/自主端/模拟端三端执行同一技能的状态一致性。

#### 9.3.3 资源管理

**现状**：`UBXTLAsset::CollectResources` 收集所有引用资源路径。

**建议**：
- **异步加载**：在 `PlayTimeline` 前检查 `AllResources` 是否已加载，未加载则异步加载并延迟播放，避免卡顿。
- **资源卸载**：时间轴结束后，主动卸载不再使用的资源（如动画蒙太奇），降低内存峰值。
- **资源引用计数**：多个时间轴共享同一动画时，应引用计数避免提前卸载。

#### 9.3.4 编辑器体验

**建议**：
- **任务模板库**：`FBXTLController` 已有"任务组模板导入导出"，可扩展为全局模板库（如"标准近战攻击"、"范围 AOE"模板），加速技能制作。
- **时间轴性能分析面板**：编辑器内显示每个 Task 的预估 CPU 开销，辅助策划优化。
- **决策树节点搜索**：`FBXDTEditor` 支持按节点名称/条件搜索定位，大型决策树时提升导航效率。

### 9.4 文档与可维护性

**建议**：
- **Task 类型清单**：维护一份所有内置 Task 子类及其用途的文档（当前需读源码）。
- **GameplayTag 命名规范**：`BXGameplayTags.h` 已分类，但应在 `Config/Tags/BXGameplayTags.ini` 中补充注释说明每类 Tag 的用途与扩展规则。
- **资产命名约定**：建议技能资产以 `SK_` 前缀、决策树以 `DT_Beaten_` / `DT_Combat_` 前缀，便于过滤与组织。

---

## 附录 A：关键文件速查表

| 系统 | 核心文件 |
|---|---|
| 模块入口 | [BattleX.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/BattleX.h) / [BattleX.cpp](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/BattleX.cpp) |
| 基础设施 | [BXManager.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/BXManager.h) / [BXSubSystem.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/BXSubSystem.h) / [BXSettings.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/BXSettings.h) |
| Tag 词汇表 | [BXGameplayTags.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/BXGameplayTags.h) |
| 工具函数 | [BXFunctionLibrary.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/BXFunctionLibrary.h) |
| 时间轴核心 | [BXTLAsset.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/Timeline/BXTLAsset.h) / [BXTLManager.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/Timeline/BXTLManager.h) / [BXTLComponent.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/Timeline/BXTLComponent.h) / [BXTLStructs.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/Timeline/BXTLStructs.h) |
| 任务系统 | [BXTask.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/Task/BXTask.h) / [BXTProcessor.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/Task/BXTProcessor.h) |
| 具体任务 | [BXTAnimation.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/Task/BXTAnimation.h) / [BXTCollision.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/Task/BXTCollision.h) |
| 状态系统 | [BXBehaviorComponent.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/State/BXBehaviorComponent.h) / [BXBehaviorAgent.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/State/BehaviorAgent/BXBehaviorAgent.h) / [BXStateMachine.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/State/StateMachine/BXStateMachine.h) |
| 决策树 | [BXDecisionTreeTemplate.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/DecisionTree/BXDecisionTreeTemplate.h) / [BXDecisionTreeActuator.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/DecisionTree/BXDecisionTreeActuator.h) |
| 装备系统 | [BXGear.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/Gear/BXGear.h) / [BXGearComponent.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/Gear/BXGearComponent.h) / [BXColdWeapon.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/Gear/BXColdWeapon.h) |
| 移动系统 | [BXCharacterMovementComponent.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/Movement/BXCharacterMovementComponent.h) / [BXRootMotionSource.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/Movement/BXRootMotionSource.h) |
| 碰撞系统 | [BXCollision.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/Collision/BXCollision.h) / [BXShapeComponent.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/Collision/BXShapeComponent.h) / [BXHitReactionComponent.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/Collision/BXHitReactionComponent.h) |
| 事件系统 | [BXEventManager.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/Event/BXEventManager.h) / [BXEventStructs.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/Event/BXEventStructs.h) |
| 条件系统 | [BXCondition.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/Condition/BXCondition.h) / [BXConditionManager.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/Condition/BXConditionManager.h) |
| 编辑器入口 | [BattleXEditor.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleXEditor/BattleXEditor.h) |
| 时间轴编辑器 | [BXTLEditor.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleXEditor/TimelineEditor/BXTLEditor.h) |
| 决策树编辑器 | [BXDTEditor.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleXEditor/DecisionTreeEditor/BXDTEditor.h) |
| 时间轴控件 | [STimeline.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleXEditor/TimelineBase/STimeline.h) / [TimelineController.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleXEditor/TimelineBase/TimelineController.h) |

---

## 附录 B：核心枚举速查

| 枚举 | 定义文件 | 说明 |
|---|---|---|
| `EBXTLFinishReason` | BXTLEnums.h | EndOfLife / Interrupt / PredictionFailure |
| `EBXTLExtraType` | BXTLEnums.h | 前摇/后摇/连段窗口1~8 |
| `EBXTLTargetSelection` | BXTLEnums.h | Self/Owner/Instigator/Trigger/Target |
| `EBXTLifeType` | BXTEnums.h | Instant/Duration/Timeline/DurationTimeline |
| `EBXTNetType` | BXTEnums.h | Authority/Autonomous/Simulated (Bitflags) |
| `EBXTTriggerType` | BXTEnums.h | Timeline/SectionEnd/SectionInterrupt (Bitflags) |
| `EBXTTargetType` | BXTEnums.h | Owner/Instigator/Trigger/LockTargets/CollisionResults (Bitflags) |
| `EBXTCoordinateType` | BXTEnums.h | Owner/Instigator/Trigger/LockTarget/LockPart/Special/World |
| `EBXTProcessorFunction` | BXTEnums.h | Native/Script × Start/Update/End/ChangeRate (Bitflags) |
| `EBXStateMachineFunction` | BXStateMachine.h | Native/BP × Init/Cleanup/Update/EnterState/ExitState (Bitflags) |
| `EBXBehaviorAgentFunction` | BXBehaviorAgent.h | Native/BP × Init/Cleanup/Start/Stop/CheckStart (Bitflags) |
| `EBXForbiddenBehaviorReason` | BXStateEnums.h | 行为禁用原因 |

---

*本文档基于源码静态分析生成，涵盖 BattleX 插件运行时模块（BattleX）与编辑器模块（BattleXEditor）的完整架构、关键类与函数说明、依赖关系、运行方式与优化建议。*
