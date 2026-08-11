# BattleX Code Wiki

> 高性能动作游戏技能系统 · Unreal Engine 5 插件
> 仓库根：`BattleX/`  ·  版本：1.1 (Beta)  ·  文档生成日期：2026-08-06

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
   - 4.13 [Buff 状态系统（复用 Timeline）](#413-buff-状态系统复用-timeline)
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
│   │                                                             │  │
│   │  ┌──────────────────────────────────────────────────────┐  │  │
│   │  │   Buff (状态系统) ──复用──> Timeline / Task          │  │  │
│   │  │   (生命时长 / 层级 / 共存 / 事件广播)                  │  │  │
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
    │   ├── Buff/                # BUFF状态系统（资产、管理器、组件，复用Timeline/Task）
    │   ├── Collision/           # 碰撞检测、形状组件、受击反应
    │   ├── Condition/           # 条件系统（基类、枚举、管理器、派生缓存）
    │   ├── DecisionTree/        # 决策树（含 BeatenTree / CombatTree + 决策树系列条件）
    │   ├── Event/               # 事件系统
    │   ├── Gear/                # 装备系统（含冷兵器）
    │   ├── Lock/                # 锁定系统（占位）
    │   ├── Movement/            # 角色移动与 RootMotion
    │   ├── State/               # 状态机与行为代理
    │   ├── Task/                # 任务系统（Task + Processor + 具体任务 + FlowControl + Task系列条件）
    │   ├── Timeline/            # 时间轴系统（资产、管理器、组件）
    │   └── Unit/                # 投射物/法术场（占位）
    └── BattleXEditor/           # 编辑器模块
        ├── BattleXEditor.{h,cpp}     # 模块入口（注册资产类型/可视化器）
        ├── BattleXEditor.Build.cs
        ├── ComponentVisualizers/ # 形状组件可视化
        ├── CustomLayout/        # 自定义 Slate 控件（骨骼选择器、函数选择器、任务组选择器）
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
- **行为**：`BXBehavior_Locomotion_Move/Rotate`、`BXBehavior_PlayMontage_Default`、`BXImmBehavior_Locomotion_Jump/Landed`
- **硬直状态**：Normal / Knockback / Knockdown / Prone / Ascending / Falling / Floating / Execution
- **攻击类型**：Light / KnockBack / KnockDown / Launch / Uppercut / AirKnockdown / Drag
- **攻击力度**：`BXAttackForce_0` ~ `BXAttackForce_5`
- **受击方位**：Front / Back / Left / Right
- **身体部位**：Head / UpperBody / LowerBody / Tail / Limbs1~8
- **装备类型/插槽/状态**：Weapon/Armor/Ornament；LeftHand/RightHand/Head/Chest...；Close/Open
- **关系/角色类型**：Self/Friendly/Neutral/Hostile；Player/NPC/Monster/BOSS
- **时间轴数据标签**：`BXTData_Transform`、`BXTData_ColResults1~5`
- **时间轴事件标签**：`BXTEvent_Start/End/Success/Failure/Trigger`
- **Switch 分支事件标签**：`BXTEvent_Branch01~16`（16 个编号分支）、`BXTEvent_BranchDefault`（默认分支，用于 `UBXTSwitch` 流程控制任务）

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
| **PinChangeDetection** | `CachedPinSignature` | 上次检测时的 Pin 相关属性签名（WITH_EDITOR） |

**Pin 变更签名检测机制**（WITH_EDITOR）：

`UBXTask::PostEditChangeProperty` 末尾通过 `BuildPinRelatedSignature()` 构建 Pin 相关属性的字符串签名，与 `CachedPinSignature` 比较来决定是否广播 `RefreshInputOutput`。签名编码了以下属性：

- `EnablePassiveTrigger()` 返回值
- `Events` 映射表的所有 Key
- `CollisionInputDatas` 每个条目的 GUID + DisplayName
- `InputDatas` 每个条目的 GUID + DisplayName
- `OutputDatas` 每个条目的 GUID + DataTag

子类只需在 `PostEditChangeProperty` 中修改属性后调用 `Super::PostEditChangeProperty`，基类自动通过签名比较触发 Pin 刷新，避免子类遗漏 `RefreshInputOutput.Broadcast()` 调用。

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
- **`UBXTCollision`**：基类，配置 `CoolDown`（冷却）、`Limit`（数量限制）、`LimitLogic`（数量限制逻辑）、`CharacterTags`/`RelationshipTags`（筛选）、`ObjectTypes`、`EngineFilter`（`FBXCFilter`）
- **`UBXTIntervalCollision`**：间隔检测基类，增加 `Interval`（检测间隔）、`Count`（检测次数，编辑器自动计算）
- **`UBXTTrackHitBox`**：轨迹碰撞盒检测。`HitBoxTag`（碰撞盒标签，未设置则匹配任意）、`PolylineConfig`（折线 Sweep 配置 `FIntVector`：X=最大段数 1~10、Y=共线检测角度阈值 1~60、Z=旋转分段角度阈值 1~180）、`BoneSampledTrajectory`（烘焙的骨骼模型空间轨迹）。运行时按帧时间范围在烘焙轨迹上采样，调用 `UBXCollisionLibrary::SphereSweepAlongCurve/CapsuleSweepAlongCurve/BoxSweepAlongCurve` 执行折线 Sweep
- **`UBXTTrackWeaponHitBox`**：武器轨迹碰撞，增加 `WeaponSlot`（默认右手）

##### `UBXTSwitch` ([BXTFlowControl.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Task/BXTFlowControl.h)) ★

流程控制 Switch 任务（`L_Instant` 生命周期）。按顺序评估分支条件，首个匹配的分支触发对应事件后结束。

| 字段 | 说明 |
|---|---|
| `Cases` | 分支列表（`FBXTSwitchCase` 数组），每个 Case 含 `Condition`（`UBXTaskCondition*`，Instanced 内联编辑）与 `EventTag`（满足时触发的事件 Tag） |
| `DefaultEventTag` | 所有分支都不匹配时触发的事件 Tag（默认 `BXTEvent_BranchDefault`） |

**执行流程**（`UBXTPSwitch::Start`）：
1. 首次使用时通过 `BuildTaskConditionParameter<FBXTaskConditionParameter>` 构建参数（填充 Owner/Instigator），后续 Case 复用
2. 逐个 Case 调用 `CheckCondition` 评估，首个匹配的 Case 通过 `AddPendingTask` 触发其 `EventTag` 后返回
3. 全部不匹配时触发 `DefaultEventTag`

**编辑器行为**（`PostEditChangeProperty`）：
- Cases 变更时自动为 `EventTag` 无效的 Case 分配未使用的 `BXTEvent_Branch01~16`（从小序号开始）
- 重建 `Events` 映射表：所有 Case 的 `EventTag` + `DefaultEventTag`，供 GraphNode Pin 显示

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

决策树系列条件继承自 `UBXDecisionTreeCondition`（详见 [4.10 Condition 条件系统](#410-condition-条件系统)）。条件基类有 `bNot` 反转标志；`UBXConditionManager` 支持 Native 快速路径（绕过 `ProcessEvent`）与反射慢速路径双路求值，并提供同帧派生结果缓存。

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

#### `ABXMeleeWeapon` ([BXMeleeWeapon.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/Gear/BXMeleeWeapon.h))

近战武器基类。持有 `UBXShapeComponent`（HitBoxComponent）用于命中检测。提供 `GetHitBoxComponent` / `GetHitBoxTransform`。

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

蓝图函数库，提供静态碰撞查询 API：

- **单次形状查询**：`SphereCheck`、`CapsuleCheck`、`CylinderCheck`、`HollowCylinderCheck`、`BoxCheck`、`SectorCheck`
- **曲线折线 Sweep**（轨迹命中优化）：`SphereSweepAlongCurve`、`CapsuleSweepAlongCurve`、`BoxSweepAlongCurve`，将一帧内的曲线 `TArray<FTransform>` 折线化为 N 段（默认 3），每段单次 Sweep，并通过反向延伸填补段间/跨帧旋转缺口
- **结果处理**：`CheckCollisionResult`（按 Filter 校验单条 Hit）、`CombineCollisionResults`（按 (Component, BoneName) 去重合并）

#### 关键结构体

- **`FBXCFilter`**：过滤器。`bIgnoreSelf`、`ClassTypes`/`IgnoreClassTypes`、`ActorTags`/`IgnoreActorTags`、`ComponentTags`/`IgnoreComponentTags`
- **`FBXCParameter`**：请求者、起止位置/旋转、缩放
- **`FBXCStrategy`**（基类）+ 子类：`FBXCSSphere`/`FBXCSCapsule`/`FBXCSCylinder`/`FBXCSHollowCylinder`/`FBXCSBox`/`FBXCSSector`。Sweep 型形状含 `AngleStep`（越小越精确越耗性能）与 `bUseSweep`
- **`EBXCDirection`**：扇形精细检测方向（RightToLeft/LeftToRight/InToOut）
- **`FBXCPolylineFrameLink`**：折线 Sweep 跨帧衔接信息。`LastSegDir`（上一帧末段方向）、`LastSegRotation`（上一帧末段姿态）、`bValid`（是否有效）。Library 内部维护，调用方持久化存储并每帧传入，用于跨帧首段的反向延伸

#### 折线 Sweep 分段算法（`BuildPolylineSegments`）

将曲线降采样为有限段折线，控制单帧 Sweep 次数，核心步骤：

1. **累积弧长**：按相邻点位置距离累加，得到每点弧长坐标
2. **旋转优先分段**：累积所有相邻点旋转差（`FQuat::AngularDistance`），按 `PolylineConfig.Z`（旋转阈值）计算段数 `RotSegments`，受 `PolylineConfig.X`（最大段数）上限约束。使用累积差而非首末差，避免 S 形曲线首末旋转接近但中间扭曲导致的欠分段
3. **共线拐点补充**：剩余段额度内，用 `UBXFunctionLibrary::AreCollinear` 检测三点不共线的位置拐点，按 `PolylineConfig.Y`（共线角度阈值）补充分段点
4. **弧长等分插值**：按目标弧长位置在相邻烘焙点间插值生成关键点 Transform（位置 Lerp + 旋转 Slerp + 缩放 Lerp），均衡各段长度
5. **反向延伸填补缺口**：每段起点沿前一段方向反向延伸 `BackOff = ShapeMaxRadius * tan(θ/2)`（θ 为相邻段旋转差，clamp 到 170° 防止跨帧突变时 BackOff 爆炸），填补旋转缺口。首段使用 `FBXCPolylineFrameLink` 提供的上一帧末段信息
6. **更新跨帧衔接**：将本帧末段方向与姿态写回 `OutFrameLink`，供下一帧使用

调用旧接口（`CapsuleCheck`/`BoxCheck`）时传 `AngleStep=360`，防止旧接口对段内旋转差二次拆分。`PolylineConfig` 在 `UBXTTrackHitBox::PostEditChangeProperty` 与 `BuildPolylineSegments` 双重 Clamp 范围。

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

条件系统提供数据驱动的条件判定框架，支持系列隔离、组合嵌套（AND/OR/NOT）、Native 快速路径与同帧运算结果缓存。

#### `EBXLogicOperator` ([BXConditionEnums.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Condition/BXConditionEnums.h))

逻辑运算枚举，用于组合条件的短路求值：
- `And`：所有子条件都满足时为真
- `Or`：任一子条件满足时为真

#### `UBXCondition` ([BXCondition.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Condition/BXCondition.h))

条件基类。`Abstract, Blueprintable, EditInlineNew`。
- `bNot`：结果取反标志
- `GetDescription()`：编辑器显示用描述（WITH_EDITOR）

#### 系列隔离机制

条件系统通过 **抽象基类 + Instanced 属性** 实现不同业务模块的条件子类互不可见，防止子系统间条件类混用：

| 系列 | 基类 | 组合条件 | 参数结构体 | 文件 |
|---|---|---|---|---|
| **Task 系列** | `UBXTaskCondition` | `UBXTaskConditionComposite` | `FBXTaskConditionParameter` | [BXTaskCondition.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Task/BXTaskCondition.h) |
| **决策树系列** | `UBXDecisionTreeCondition` | `UBXDecisionTreeConditionComposite` | （按需补充） | [BXDecisionTreeCondition.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/DecisionTree/BXDecisionTreeCondition.h) |

每个系列的 `Composite.Children` 数组通过 `Instanced` 属性限定为该系列基类指针，编辑器细节面板只能选取同系列子类。

#### 组合条件（Composite 模式）

`UBXTaskConditionComposite` / `UBXDecisionTreeConditionComposite` 支持嵌套条件判断，可实现 `((A&&B&&C)||(D&&E))` 等复杂表达式：

| 字段 | 说明 |
|---|---|
| `Logic` | 逻辑运算类型（`EBXLogicOperator::And` / `Or`） |
| `Children` | 子条件列表（Instanced 内联编辑，限定同系列） |

求值规则（`NativeCheckTaskComposite` / `NativeCheckDecisionTreeComposite`）：
- **空 Children** 永远返回 `True`
- **AND 模式**：短路求值，遇到 `false` 立即返回 `False`
- **OR 模式**：短路求值，遇到 `true` 立即返回 `True`
- **NOT 取反**：继承自 `UBXCondition::bNot`，在 `CheckCondition` 最外层统一处理

#### `UBXConditionManager` ([BXConditionManager.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Condition/BXConditionManager.h))

单例管理器，核心职责包括条件求值、条件参数构造、Native 函数注册与派生结果缓存。

**条件求值**（三种重载）：
- `CheckCondition<T>(Condition, Param)`：模板版，转发 `UScriptStruct*` + `const void*`（const 正确性从模板层传递）
- `CheckCondition(Condition, int32)`：BP CustomThunk 版（`execCheckCondition` 从蓝图 VM 栈提取结构体）
- `CheckCondition(Condition, UScriptStruct*, const void*)`：类型擦除版，实际执行

**求值路径（双路）**：
1. **快速路径（Native）**：若条件类在 `NativeCheckMap` 中注册了原生函数，直接 C++ 调用，绕过 `ProcessEvent`
2. **慢速路径（反射）**：通过 `ConditionToFunctionMap` 查找 `UFunction`，分配 ParmsSize 缓冲区，`Memcpy` 参数后 `ProcessEvent` 调用

最终结果统一经 `bNot` 取反后返回。

**Native 函数签名与 int64 指针承载**：

由于 UHT 不支持 `void*` 作为 UFUNCTION 参数，Native 函数签名使用 `int64` 承载指针地址：
```cpp
using FBXNativeCheckFunc = bool(UBXConditionManager::*)(UBXCondition* InCondition, int64 InParameterTypeAddress, int64 InParameterAddress);
```
Native 函数入口通过 `check` 断言校验指针非零，`reinterpret_cast` 还原后使用。调用方在转发时通过 `const_cast` 去除 const（仅绕过签名限制，Native 函数内部不应修改入参）。

**Native 函数 Registry 自动注册机制**：

Native 函数通过宏驱动自动注册到 Registry，避免手动维护静态表导致的遗漏：

```cpp
// 宏：定义Native条件检查函数并自动注册到Registry
#define IMPLEMENT_NATIVE_CHECK(Class, FuncName) \
    static bool GRegistered_##FuncName = (AccessNativeCheckRegistry().Add(TEXT(#FuncName), &Class::FuncName), true); \
    bool Class::FuncName(UBXCondition* InCondition, int64 InParameterTypeAddress, int64 InParameterAddress)
```

- `AccessNativeCheckRegistry()` / `AccessNativeTaskParamBuilderRegistry()`：函数内静态变量，避免跨翻译单元初始化顺序问题
- 漏写宏 = 函数未定义 = 链接错误（编译期暴露问题）
- `Initialize()` 时遍历 `ConditionToFunctionConfig`，Registry 命中则注册到 `NativeCheckMap`（Native 通道），否则走蓝图 UFunction 通道

**条件参数构造（TaskConditionParamBuilder）**：

为 Task 条件提供参数构造机制，支持 Native 与蓝图双通道：

| 方法 | 说明 |
|---|---|
| `BuildTaskConditionParameter<TParam>(Condition, RTData, RTSData, RTTData)` | C++ 模板版，返回 `TOptional<TParam>` |
| `BuildTaskConditionParameter(Condition, RTData, RTSData, RTTData, OutParam)` | BP CustomThunk 版，`OutParameter` 为通配符输出引脚 |
| `InternalBuildTaskConditionParameter(Condition, RTData, RTSData, RTTData, OutParamType, OutParamAddress)` | 运行时实现，向上回溯查找 Builder |

**Builder 查找流程**（`InternalBuildTaskConditionParameter`）：
1. 向上回溯 `InCondition` 的类继承链，查找 Native Builder → 命中则直接 C++ 调用写入内存
2. 未命中 Native 则查找蓝图 Builder → 通过 `ProcessEvent` 调用 UFunction（签名：Condition, OutParam&）
3. 均未命中则回退到 `NativeBuildDefaultTaskParam`（填充 Owner/Instigator 基础参数）

**类型安全校验**：
- 蓝图 Builder 通道校验 `ConditionProperty->PropertyClass` 是否为当前类的派生
- 校验 `InOutParamType` 是否为 `ParamProperty->Struct` 的派生（避免越界写入）
- 校验 `InOutParamType` 是否为 `FBXTaskConditionParameter` 的派生（确保可预填 Owner/Instigator）
- Native 函数入口 `check` 断言指针非零

**Native ParamBuilder 宏**：
```cpp
#define IMPLEMENT_NATIVE_TASK_PARAM_BUILDER(Class, FuncName) \
    static bool GRegistered_##FuncName = (AccessNativeTaskParamBuilderRegistry().Add(TEXT(#FuncName), &Class::FuncName), true); \
    void Class::FuncName(UBXTaskCondition* InCondition, const FBXTLRunTimeData& InRTData, ...)
```

**帧时间戳机制**：
- `OnWorldTickStart` 回调在 World Tick 最早时机刷新 `CurrentFrameTime`（`FPlatformTime::Seconds()`）
- 帧内所有 `CheckCondition` / 派生缓存共用同一时间戳，确保同帧一致性

**派生结果缓存系统**：

针对"同一种条件 + 相同参数"在同一帧内重复进行复杂运算的场景，提供三类型缓存（int32 / float / FInstancedStruct），由调用方自行决定是否缓存。每类缓存均提供 C++ 模板版本与 Blueprint 可调用版本：

| 缓存类型 | C++ 模板查询 | Blueprint 查询 | Blueprint 写入 |
|---|---|---|---|
| int32 | `GetDerivedInt<TParam>(Class, Param)` → `const int32*` | `GetDerivedInt(Class, InParam, OutValue)` → `bool` | `SetDerivedInt(Class, InParam, InValue)` |
| float | `GetDerivedFloat<TParam>(Class, Param)` → `const float*` | `GetDerivedFloat(Class, InParam, OutValue)` → `bool` | `SetDerivedFloat(Class, InParam, InValue)` |
| Struct | `GetDerivedStruct<TResult,TParam>(Class, Param)` → `const TResult*` | `GetDerivedStruct(Class, InParam, OutResult)` → `bool` | `SetDerivedStruct(Class, InParam, InResult)` |

- Blueprint 版本通过 `CustomThunk` + `CustomStructureParam` 实现通配符结构体参数，`exec*` 函数从蓝图 VM 栈提取结构体地址与类型
- Internal Get 系列函数标记 `const`（仅读操作）
- 缓存键 `FBXDerivedKey` 由 `ConditionClass` + `ParamType` + `ParamHash` 组成
- 缓存项 `TBXDerivedEntry<T>` 持有 `Value` 与 `CachedTime`，查询时校验 `CachedTime == CurrentFrameTime` 确保同帧有效
- 超过 `MaxDerivedEntries`（1024）时整体清空，防止内存膨胀

**配置表（使用 `FBXFunctionSelector`）**：

`FBXFunctionSelector`（[BXStructs.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/BXStructs.h)）是统一的函数选择器结构体，包含 `TargetClass`（目标类，为空时使用所属对象的类）与 `FunctionName`（函数名）。编辑器中通过自定义属性布局（`FBXFunctionSelectorCustomization`）提供可搜索的函数列表下拉。

| 配置表 | 说明 |
|---|---|
| `ConditionToFunctionConfig` | 条件检查函数配置（`TSubclassOf<UBXCondition>` → `FBXFunctionSelector`） |
| `ConditionToFunctionMap` | 条件检查运行时映射（蓝图通道，含 `UFunction*` 与参数名列表） |
| `NativeCheckMap` | Native 条件检查函数映射（直接 C++ 调用） |
| `TaskConditionParamBuilderConfig` | 条件参数构造函数配置（`TSubclassOf<UBXTaskCondition>` → `FBXFunctionSelector`） |
| `TaskConditionParamBuilderMap` | 条件参数构造运行时映射（蓝图通道） |
| `NativeTaskParamBuilderMap` | Native 参数构造函数映射 |

`FBXConditionFunctionParameter` 持有反射的 `UFunction*` 与参数名列表，`Initialize` 时校验参数签名（条件检查为 3 参数：条件对象 / 参数结构体 / bool 返回值；参数构造为 2 参数：条件对象 / OutParam&）。

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

### 4.13 Buff 状态系统（复用 Timeline）

`Source/BattleX/Buff/` 模块实现了一套数据/逻辑分离的 BUFF 状态系统。**核心思路**：BUFF 本质是简化版 Timeline（单 Section、无 KeyFrame 时间推进、按 BUFF 规则调度），因此直接复用 Timeline 的数据载体与 Task 执行链路，避免重写一套等价的执行机制。

#### 设计原则

| 原则 | 说明 |
|---|---|
| 数据/逻辑分离 | 运行时状态存于纯数据结构 `FBXBuffRuntimeData`，逻辑收归 `UBXBuffManager` |
| Task 执行完全复用 | BUFF 内的 Task 通过 `UBXTLManager` 的方法执行，与技能完全同路径 |
| 职责清晰分层 | `UBXTLManager` 管所有 Task 的执行，`UBXBuffManager` 只管 BUFF 特有逻辑 |
| 事件系统复用 | BUFF 生命周期事件通过 `BXEventManager` 单体+全局广播 |

#### 架构对齐

| 层 | 技能系统 | BUFF 系统 | 职责 |
|---|---|---|---|
| 静态配置 | `UBXTLAsset` | `UBXBuffAsset : UBXTLAsset` | 继承，复用 Sections/TaskList，额外加 BUFF 字段 |
| 运行时数据 | `FBXTLRunTimeData` + `FBXTLSectionRTData` | 同上（直接复用） | Timeline 字段指向 BuffAsset |
| 编排逻辑 | `UBXTLManager` | `UBXBuffManager` | BUFF 特有：生命时长/层级/共存/事件广播 |
| Task 执行 | `UBXTLManager` | `UBXTLManager`（复用） | Task 启停/更新/事件链/数据传递/作用域 |

#### 核心文件

| 文件 | 说明 |
|---|---|
| `BXBuffEnums.h` | 枚举：`EBXBuffLifeType`（Duration/Infinite/Manual）、`EBXBuffLayerLifeMode`（Shared/Independent）、`EBXBuffCoexistPolicy`（Coexist/Replace）、`EBXBuffRemoveReason` |
| `BXBuffStructs.h` | `FBXBuffRuntimeData`（运行时数据，内嵌 `FBXTLRunTimeData`）、`FBXBuffPlayContext`（施放上下文）、`FBXBuffTaskBinding`（Task 层级区间绑定）、`FBXEventBuffChanged`（事件参数） |
| `BXBuffAsset.h/.cpp` | BUFF 资产，继承 `UBXTLAsset`。额外字段：`BuffTags`、`LifeType`、`BuffDuration`（-1 代表无限）、`LayerLifeMode`、`MaxLayer`（默认1）、`MaxLevel`、`CoexistPolicy`、`bRefreshLifetimeOnAdd`、`TaskBindings` |
| `BXBuffManager.h/.cpp` | BUFF 管理器，继承 `UBXManager` + `FTickableGameObject`。管理 BUFF 生命周期、层级、共存策略、事件广播 |
| `BXBuffComponent.h/.cpp` | Actor 组件，提供 BP 友好 API，EndPlay 时自动移除所有 BUFF |
| `BXBuffFunctionLibrary.h/.cpp` | 静态 BP API（AddBuff/RemoveBuff/HasBuff 等） |

#### UBXBuffAsset（资产配置）

继承 `UBXTLAsset`，Task 列表配置在 `Sections[0].TaskList`（复用父类字段）。`TaskBindings` 仅存储层级区间配置（MinLayer/MaxLayer），通过 Task 指针与 Section 内的 Task 关联。

**关键设计**：`UBXTProcessor` 执行链路硬依赖 `Asset->Sections[SectionIndex].TaskList[TaskIndex]`，Task 必须在 Sections 里才能被索引和执行。因此 BUFF 资产继承 Timeline 资产，Task 直接放在 Sections[0] 中。

#### UBXBuffManager（管理器）

**职责分层**：

| 职责 | 归属 | 调用方式 |
|---|---|---|
| Task 启动 | UBXTLManager | `ExecuteTimelineTask` |
| Task 更新 | UBXTLManager | `UpdateTimelineSectionTasks`（从 InternalUpdateTimeline 提取） |
| Task 结束 | UBXTLManager | `FinishTimelineSection` |
| 事件链处理 | UBXTLManager | `ProcessTimelineSectionPendingTasks` |
| Processor 缓存 | UBXTLManager | `GetTLTProcessorByTLTClass` |
| 数据传递/作用域 | UBXTLManager（内嵌于 Task 执行） | 自动 |
| 生命时长推进 | UBXBuffManager | 自己 |
| 层数/等级管理 | UBXBuffManager | 自己 |
| 共存策略 | UBXBuffManager | 自己 |
| 层级区间 Task 切换 | UBXBuffManager | 调用 TLManager 启停 |
| 事件广播 | UBXBuffManager | 调用 BXEventManager（单体+全局） |
| Owner→Buff 索引 | UBXBuffManager | 自己 |

**Tick 流程**：

```
Tick(DeltaTime)
  ├ 遍历 BuffRTDatas (UBXBuffManager)
  │   ├ ① UpdateTimelineSectionTasks (UBXTLManager)     // Task 执行
  │   ├ ② ProcessTimelineSectionPendingTasks (UBXTLManager) // 事件链
  │   ├ ③ 生命时长推进 (UBXBuffManager)                  // 共享/独立
  │   ├ ④ 到期 → FinishTimelineSection (UBXTLManager) + 移除
  │   └ ⑤ 层级变化 → 区间 Task 切换 + RebuildEffect      // 需求 11
  └ CleanBuffTrash (UBXBuffManager)
```

#### Task 层级区间执行（需求 10）

`FBXBuffTaskBinding` 配置每个 Task 的层级执行区间：
- `MinLayer=0, MaxLayer=0`：无层级限制（所有层级执行）
- `MinLayer=1, MaxLayer=3`：仅 1~3 层执行
- 层级变化时，`InternalRefreshBuffTasksByLayer` 自动停止离开区间的 Task、启动进入区间的 Task

#### Task 效果随层级变化（需求 11）

采用**高级层方案（RebuildEffect）**：层级变化时 Task 不重启，而是调用 `RebuildEffect` 接口通知 Task 层级变化，Task 在内部增量调整效果。

扩展 `UBXTProcessor`：
- `RebuildEffect`（protected virtual）：C++ 重写入口
- `ScriptRebuildEffect`（BlueprintImplementableEvent）：蓝图重写入口
- `RebuildEffectTask`（public UFUNCTION）：公开 wrapper，内部调用上述两个函数

调用路径：`UBXBuffManager::InternalRefreshBuffTasksByLayer` → `RebuildBuffTaskEffect` → `Processor->RebuildEffectTask`

**适用场景**：Task 有运行时累积状态（如已造成伤害计数、已播放动画进度），重启会丢失状态，需增量调整效果。

#### 事件广播（需求 9）

5 个 BUFF 生命周期事件（定义在 `BXGameplayTags`）：

| Tag | 触发时机 |
|---|---|
| `BXEvent.Buff.Added` | BUFF 添加 |
| `BXEvent.Buff.Removed` | BUFF 移除（含原因） |
| `BXEvent.Buff.LayerChanged` | 层数变化 |
| `BXEvent.Buff.LevelChanged` | 等级变化 |
| `BXEvent.Buff.LifetimeRefreshed` | 生命时长刷新 |

事件参数结构 `FBXEventBuffChanged`：包含 BuffInstanceID、BuffAsset、三角色（Owner/Instigator/Triggerer）、Old/NewLayer、Old/NewLevel、RemoveReason。

广播方式（单体+全局）：
- `BroadcastSingleEvent`：以 Owner 为 Initiator，仅注册了 (EventTag, Owner) 的监听者收到
- `BroadcastGlobalEvent`：所有监听者都能收到

#### 共存策略（需求 5）

| 策略 | 行为 |
|---|---|
| `BC_Coexist` | 不同始作俑者共存（各自独立实例），相同 Instigator 加层 |
| `BC_Replace` | 不共存，取等级最高 + 剩余时长最长的实例替换 |

#### 层级生命周期（需求 3）

| 模式 | 行为 |
|---|---|
| `BLL_Shared` | 所有层共享一个 `RunTime`，到期整体移除 |
| `BLL_Independent` | 每层独立 `LayerRunTimes[i]`，单层到期仅移除该层，触发层级变化 |

#### 注册配置

在 `DefaultGame.ini` 的 `[/Script/BattleX.BXSettings]` 段中添加：
```
+ManagerClasses=/Script/BattleX.BXBuffManager
```

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
- **CustomLayout**：共享自定义 Slate 控件
  - `SBXTLTaskGroupPicker`：任务组选择器
  - `FBXFunctionSelectorCustomization`：`FBXFunctionSelector` 的自定义属性布局，ComboButton 弹出可搜索的函数列表（按 TargetClass 过滤 UFunction），选中后自动关闭下拉
  - `FBXBoneSelectorCustomization`：`FBXBoneSelector` 的自定义属性布局，ComboButton 弹出 `SBXBonePicker` 骨骼树选择窗口（基于 `STreeView`，支持搜索过滤），用于 `FBXTTransformCreater::OriginBoneName` 等骨骼选取场景。依赖 `AnimGraphRuntime` 模块

---

## 6. 依赖关系

### 6.1 模块依赖（`BattleX.Build.cs`）

**PublicDependencyModuleNames**（运行时核心）：
- `Core` / `CoreUObject` / `Engine` / `InputCore` / `EnhancedInput`
- `Slate` / `SlateCore` / `DeveloperSettings`

**PrivateDependencyModuleNames**：
- `GameplayTags` / `StructUtils`

**PrivateIncludePaths**：暴露了所有子目录（`Task`、`Unit`、`Lock`、`Gear`、`State`、`Event`、`Movement`、`Timeline`、`Animation`、`Collision`、`Condition`、`DecisionTree`、`Buff`）

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

Buff (状态系统) ──复用──> Timeline (数据载体 + Task执行链路)
    │              └──复用──> Task (Processor)
    └──依赖──> Event (BUFF生命周期事件广播)

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

### 8.5 BUFF 施加与更新流程

```
1. 游戏代码: BuffComp->AddBuff(Asset, Instigator, Layer, Level)  或  UBXBuffFunctionLibrary::AddBuff
       │
       ↓
2. UBXBuffManager::AddBuff(Asset, Owner, Context)
   ├─ 共存策略判断 (Coexist/Replace)
   │   ├─ BC_Coexist: 查找 (Owner, Asset, Instigator) 唯一实例 → 存在则 ChangeBuffLayer + RefreshBuffLifetime
   │   └─ BC_Replace: 查找等级最高+剩余时长最长实例 → 替换 Instigator/Triggerer/Layer/Level
   │
   ├─ 创建新 FBXBuffRuntimeData，TLRunTimeData.Timeline = BuffAsset
   ├─ InternalAddBuff: 填充三角色 + 初始化单 SectionRTData + StartBuffTasks + 广播 Added 事件
   │   └─ StartBuffTasks: 遍历 TaskBindings, 对符合当前层级的 Task 调用 ExecuteBuffTask
   │       └─ ExecuteBuffTask → UBXTLManager::ExecuteTimelineTask (完全复用技能 Task 启动)
   │
   └─ 返回 BuffID

3. UBXBuffManager::Tick(DeltaTime) (每帧)
   └─ 遍历 BuffRTDatas → InternalUpdateBuff(Data, DeltaTime)
       ├─ ① UBXTLManager::UpdateTimelineSectionTasks (Task 执行)
       ├─ ② UBXTLManager::ProcessTimelineSectionPendingTasks (事件链)
       └─ ③ 生命时长推进
           ├─ BLL_Shared: RunTime += DeltaTime → 到期则 FinishTimelineSection + RemoveBuff
           └─ BLL_Independent: 每层独立计时 → 单层到期移除该层 → InternalRefreshBuffTasksByLayer
               └─ InternalRefreshBuffTasksByLayer:
                   ├─ 离开区间: StopBuffTask
                   ├─ 进入区间: ExecuteBuffTask
                   └─ 仍在区间: RebuildBuffTaskEffect → Processor->RebuildEffectTask (需求11)
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

**现状**：`UBXTTrackHitBox` 通过 `PolylineConfig`（`FIntVector`：X=最大段数 1~10、Y=共线角度阈值 1~60、Z=旋转角度阈值 1~180）控制折线 Sweep 分段。一帧内曲线被折线化为 ≤X 段，每段单次 Sweep；段间与跨帧的旋转缺口由反向延伸（`BackOff = ShapeMaxRadius * tan(θ/2)`，θ clamp 到 170°）填补。`UBXCollisionLibrary::SphereSweepAlongCurve/CapsuleSweepAlongCurve/BoxSweepAlongCurve` 是通用曲线扫描接口，`FBXCPolylineFrameLink` 承载跨帧衔接信息。

**建议**：
- **LOD 化碰撞检测**：根据目标距离/重要性动态调整 `PolylineConfig.X`（远处用更少段数，近处用更多段数）。
- **多线程化**：将碰撞检测任务分派到 Task Graph 并行执行（当前在 `UBXTLManager::Tick` 主线程串行）。可利用 UE 的 `ParallelFor`。
- **HitResult 对象池**：`FBXTHitResults::Results` 预分配容量，可考虑跨任务复用避免反复分配。
- **ShapeMaxRadius 精度**：`BoxSweepAlongCurve` 当前用 `BoxSize.Size()`（半对角线）作为形状最大半径，会略微高估 BackOff，可改为 `BoxSize.GetMax()` 更精确。

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
| 具体任务 | [BXTAnimation.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/Task/BXTAnimation.h) / [BXTCollision.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/Task/BXTCollision.h) / [BXTFlowControl.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Task/BXTFlowControl.h) |
| 状态系统 | [BXBehaviorComponent.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/State/BXBehaviorComponent.h) / [BXBehaviorAgent.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/State/BehaviorAgent/BXBehaviorAgent.h) / [BXStateMachine.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/State/StateMachine/BXStateMachine.h) |
| 决策树 | [BXDecisionTreeTemplate.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/DecisionTree/BXDecisionTreeTemplate.h) / [BXDecisionTreeActuator.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/DecisionTree/BXDecisionTreeActuator.h) |
| 装备系统 | [BXGear.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/Gear/BXGear.h) / [BXGearComponent.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/Gear/BXGearComponent.h) / [BXMeleeWeapon.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/Gear/BXMeleeWeapon.h) |
| 移动系统 | [BXCharacterMovementComponent.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/Movement/BXCharacterMovementComponent.h) / [BXRootMotionSource.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/Movement/BXRootMotionSource.h) |
| 碰撞系统 | [BXCollision.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/Collision/BXCollision.h) / [BXShapeComponent.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/Collision/BXShapeComponent.h) / [BXHitReactionComponent.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/Collision/BXHitReactionComponent.h) |
| 事件系统 | [BXEventManager.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/Event/BXEventManager.h) / [BXEventStructs.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleX/Event/BXEventStructs.h) |
| 条件系统 | [BXCondition.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Condition/BXCondition.h) / [BXConditionEnums.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Condition/BXConditionEnums.h) / [BXConditionManager.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Condition/BXConditionManager.h) / [BXTaskCondition.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Task/BXTaskCondition.h) / [BXDecisionTreeCondition.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/DecisionTree/BXDecisionTreeCondition.h) |
| BUFF 系统 | [BXBuffAsset.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Buff/BXBuffAsset.h) / [BXBuffManager.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Buff/BXBuffManager.h) / [BXBuffComponent.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Buff/BXBuffComponent.h) / [BXBuffStructs.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Buff/BXBuffStructs.h) / [BXBuffEnums.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Buff/BXBuffEnums.h) / [BXBuffFunctionLibrary.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Buff/BXBuffFunctionLibrary.h) |
| 编辑器入口 | [BattleXEditor.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleXEditor/BattleXEditor.h) |
| 时间轴编辑器 | [BXTLEditor.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleXEditor/TimelineEditor/BXTLEditor.h) |
| 决策树编辑器 | [BXDTEditor.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleXEditor/DecisionTreeEditor/BXDTEditor.h) |
| 时间轴控件 | [STimeline.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleXEditor/TimelineBase/STimeline.h) / [TimelineController.h](file:///c:/Users/xiewe/.trae-cn/worktrees/BattleX/feat-code-wiki-action-game-skill-system-OiNOVn/Source/BattleXEditor/TimelineBase/TimelineController.h) |
| 自定义属性布局 | [BXFunctionSelectorCustomization.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleXEditor/CustomLayout/BXFunctionSelectorCustomization.h) / [BXBoneSelectorCustomization.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleXEditor/CustomLayout/BXBoneSelectorCustomization.h) / [SBXBonePicker.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleXEditor/CustomLayout/SBXBonePicker.h) |

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
| `EBXLogicOperator` | BXConditionEnums.h | And / Or（组合条件逻辑运算） |
| `EBXBuffLifeType` | BXBuffEnums.h | Duration / Infinite / Manual（BUFF 生命周期类型） |
| `EBXBuffLayerLifeMode` | BXBuffEnums.h | Shared / Independent（层级生命周期模式） |
| `EBXBuffCoexistPolicy` | BXBuffEnums.h | Coexist / Replace（BUFF 共存策略） |
| `EBXBuffRemoveReason` | BXBuffEnums.h | Manual / Expired / OverStack / OwnerLost（BUFF 移除原因） |

---

*本文档基于源码静态分析生成，涵盖 BattleX 插件运行时模块（BattleX）与编辑器模块（BattleXEditor）的完整架构、关键类与函数说明、依赖关系、运行方式与优化建议。*
