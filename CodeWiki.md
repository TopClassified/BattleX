# BattleX Code Wiki

> 高性能动作游戏技能系统 · Unreal Engine 5 插件
> 仓库根：`BattleX/`  ·  版本：1.3 (Beta)  ·  文档更新日期：2026-08-19

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
   - 4.14 [Skill / Net 网络同步系统（客户端预测与服务器权威）](#414-skill--net-网络同步系统客户端预测与服务器权威)
5. [编辑器模块](#5-编辑器模块)
6. [依赖关系](#6-依赖关系)
7. [项目运行方式](#7-项目运行方式)
8. [关键数据流与执行流程](#8-关键数据流与执行流程)
9. [优化建议](#9-优化建议)
10. [附录 A：关键文件速查表](#附录-a关键文件速查表)
11. [附录 B：核心枚举速查](#附录-b核心枚举速查)

---

## 1. 项目概述

**BattleX** 是一个面向 **高性能动作游戏** 的技能系统插件，基于 Unreal Engine 5 构建。它提供了一套完整的、数据驱动的技能开发框架，目标是让策划/技术美术能在不修改 C++ 代码的前提下，通过编辑器资产编排复杂的动作游戏技能（连段、受击反应、命中检测、位移、特效等）。

### 设计目标

| 目标 | 实现手段 |
|---|---|
| **高性能** | 单点 `UBXTLManager` 集中 Tick；任务处理器实例缓存；InstancedStruct 零拷贝传递动态数据；按 Bitmask 选择性启用 C++/BP 执行路径 |
| **数据驱动** | 技能 = `UBXTLAsset` 资产；任务 = `UBXTask` 内联实例；决策树 = `UBXDecisionTreeTemplate` 资产 |
| **可扩展** | Task / Processor / BehaviorAgent / StateMachine / Condition 均为 `Abstract + Blueprintable + EditInlineNew`，支持 C++ 与蓝图双轨扩展 |
| **网络友好** | Skill/Buff 客户端预测 + 服务器权威校验 + Reliable 广播 + 加速弥补；Task 通过 `EBXTNetType` Bitmask 区分权威端/自主端/模拟端执行；`COND_InitialOnly` 快照支持 Late Join 重建；服务器可关闭碰撞检测改为客户端上报 |
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
│   │                                                             │
│   │  ┌──────────────────────────────────────────────────────┐  │  │
│   │  │   Skill (技能系统) ──复用──> Timeline / Task         │  │  │
│   │  │   (释放条件 / 冷却 / 预测 / 加速)                      │  │  │
│   │  └──────────────────────────────────────────────────────┘  │  │
│   │                                                             │  │
│   │  ┌──────────────────────────────────────────────────────┐  │  │
│   │  │   Net (同步框架) ──服务于──> Skill / Buff            │  │  │
│   │  │   (预测头/载荷 / 复制快照 / 资产ID注册表)              │  │  │
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
- **技能 = 时间轴资产**：一个技能由若干 `FBXTLSection`（时间片段）组成，每个片段包含若干 `UBXTask`（任务）按时间排布。技能资产 `UBXSkillAsset` 与 BUFF 资产 `UBXBuffAsset` 均派生自 `UBXTLAsset`，分别叠加释放配置（冷却/锁定/释放条件）与 BUFF 配置（层级/共存/生命时长）。
- **任务 = 原子操作 + 处理器**：`UBXTask` 是配置数据（蓝图可编辑），`UBXTProcessor` 是运行时执行器（C++/蓝图双实现）。每个 Task 类对应一个 Processor 类（通过 `UBXSettings::TaskProcessorMap` 配置）。
- **数据流 = 上下文数据作用域图**：任务通过 `WriteContextData/ReadContextData` 在 `FBXTLRunTimeData::DynamicDatas` 中以 `(Task全量索引, 数据Tag, 作用域)` 为键存取动态数据，作用域通过 `ScopeGraph` 形成父子链，支持跨任务数据传递与作用域隔离。
- **同步 = 服务器权威 + 客户端预测**：技能由 `UBXSkillComponent` 承载 RPC（预测→校验→确认/回滚→广播），BUFF 由 `UBXBuffComponent` 承载（请求→钳制校验→广播）；跨端资产引用一律传资产 ID，由 `UBXTLManager` 注册表解析；运行状态通过可复制投影快照支持 Late Join 重建（见 [4.14](#414-skill--net-网络同步系统客户端预测与服务器权威)）。

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
    │   ├── Buff/                # BUFF状态系统（资产、管理器、组件、复制快照，复用Timeline/Task）
    │   ├── Collision/           # 碰撞检测、形状组件、受击反应
    │   ├── Condition/           # 条件系统（基类、枚举、管理器、派生缓存）
    │   ├── DecisionTree/        # 决策树（含 BeatenTree / CombatTree + 决策树系列条件）
    │   ├── Event/               # 事件系统
    │   ├── Gear/                # 装备系统（含冷兵器）
    │   ├── Lock/                # 锁定系统（占位）
    │   ├── Movement/            # 角色移动与 RootMotion
    │   ├── Net/                 # 网络同步基础（同步枚举、RPC结构、技能复制快照）
    │   ├── Skill/               # 技能系统（资产、管理器、组件、枚举、结构，复用Timeline/Task）
    │   ├── State/               # 状态机与行为代理
    │   ├── Task/                # 任务系统（Task + Processor + 具体任务 + FlowControl + Task系列条件）
    │   ├── Timeline/            # 时间轴系统（资产、管理器、组件、复制投影）
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

#### `UBXManager` ([BXManager.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/BXManager.h))

所有全局管理器的基类，提供 `Initialize()` / `Deinitialize()` 生命周期钩子。子类包括 `UBXTLManager`、`UBXEventManager`、`UBXConditionManager` 等。

#### `UBXSubSystem` ([BXSubSystem.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/BXSubSystem.h))

继承自 `UGameInstanceSubsystem`。在 `Initialize` 时按 `UBXSettings::ManagerClasses` 列表顺序实例化所有 Manager，存入 `ManagerMap`。提供模板方法 `GetManagerByClass<T>()`，支持按基类查询。所有 Manager 通过 `UBXManager::Get(UObject* WorldContext)` 静态方法访问。

#### `UBXSettings` ([BXSettings.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/BXSettings.h))

继承自 `UDeveloperSettings`（`Config=BattleX`）。关键配置项：

| 字段 | 作用 |
|---|---|
| `ManagerClasses` | 启动时按顺序创建的 Manager 类列表 |
| `CollisionDebugDuration` | 碰撞调试绘制时长（默认 1.0s） |
| `TaskProcessorMap` | Task 类 → Processor 类的映射（驱动任务执行） |
| `TaskCustomDataMap` | Task 类 → 自定义动态数据 UScriptStruct 的映射 |
| `SkillPredictMaxDuration` | 客户端预测最长时长（秒，默认 0.3，超时自动回滚） |
| `bServerAccelerateOnReceive` / `ServerAccelerateRate` | 服务器收到释放请求时是否加速弥补及倍率（默认开 / 2.0） |
| `bClientAccelerateOnBroadcast` / `ClientAccelerateRate` | 其他客户端收到广播时是否加速弥补及倍率（默认开 / 2.0） |
| `bServerCollisionCheck` | 服务器是否做碰撞检测（默认关，由 Autonomous 客户端上报） |
| `CollisionTaskServerExtraLife` | 服务器等待客户端碰撞结果的额外延迟销毁时长（秒，默认 0.15） |
| `SkillRequestMaxAgeMs` | 释放请求最大年龄（服务器世界时间域毫秒，默认 500，双向校验防重放/伪造） |

配置文件为插件自带的 `Config/DefaultBattleX.ini`（`Config=BattleX`），示例：`TaskProcessorMap` 注册蓝图 Task（`BP_BXT_PlayAnimation` → `BXTPPlayAnimation` 等）、`TaskCustomDataMap` 注册对应上下文结构（`BXTPPlayAnimationContext` 等）。

#### `BXGameplayTags` ([BXGameplayTags.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/BXGameplayTags.h))

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
- **技能事件标签**：`BXEvent_Skill_Released`（释放）、`BXEvent_Skill_Finished`（结束）、`BXEvent_Skill_PredictSuccess` / `BXEvent_Skill_PredictFailed`（预测成功/失败）
- **碰撞事件标签**：`BXEvent_Collision_Reported`（碰撞结果上报）
- **技能输入标签**：`BXSkillInput_ReleaseLocation` / `BXSkillInput_ReleaseRotation`（释放位置/朝向，自动补充）、`BXSkillInput_AimLocation` / `BXSkillInput_AimRotation`（瞄准位置/朝向）、`BXSkillInput_LockParts`（锁定部位）

Tag 的 ini 搜索路径在 `FBattleXModule::StartupModule` 中注册为 `BattleX/Config/Tags`。

#### `UBXFunctionLibrary` ([BXFunctionLibrary.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/BXFunctionLibrary.h))

通用工具函数库，分类组织：
- **时间/ID**：`GetUniqueID`、`GetClientTimeSeconds`、`GetUtcMillisecond`、`GetGameMicrosecond`、`AlignTime`、`GetServerWorldTimeMilliseconds`（服务器世界时间域毫秒，网络同步时间戳统一入口，依赖引擎时间同步）
- **GameplayTag**：`GetLastGameplayTagSubName`
- **反射拷贝**：`CopyStruct`、`CopyObject`、`CopyData`
- **时间轴**：`GetTaskFullIndex`、`GetSoftTaskFullIndex`（计算任务在资产中的全量索引）
- **几何数学**：`AreCollinear`、`PointToSegment/Circle/Rectangle/Sphere/Capsule/Cylinder/Box`、`SegmentToSegment/Sphere/Capsule/Box`
- **组件查找**：`GetSceneComponentBySocketName`、`GetSceneComponentByNameAndClass`
- **调试绘制（编辑器）**：`DrawDebugBoxInEditor` 等

---

### 4.2 Timeline 时间轴系统（技能核心）

时间轴系统是 BattleX 的核心。一个 **技能** 就是一个 `UBXSkillAsset`（派生自 `UBXTLAsset`），它由若干 **Section（时间片段）** 组成，每个 Section 内按时间排布若干 **Task（任务）**。

#### `UBXTLAsset` ([BXTLAsset.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Timeline/BXTLAsset.h))

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

#### `UBXSkillAsset` ([BXSkillAsset.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Skill/BXSkillAsset.h))

继承自 `UBXTLAsset`，技能资产。复用父类 Sections/TaskList，叠加释放配置：

| 字段 | 类型 | 说明 |
|---|---|---|
| `ReleaseConditions` | `TArray<UBXTaskCondition*>` | 释放条件列表（服务器校验用，复用 TaskCondition 体系） |
| `Cooldown` | `float` | 默认冷却时长（秒，-1 代表无冷却） |
| `LockType` | `EBXSkillLockType` | 锁定类型（None/Target/Location/Direction，决定释放时附带的数据） |

运行时由 `UBXSkillManager` 驱动（见 [4.14](#414-skill--net-网络同步系统客户端预测与服务器权威)）。

#### `UBXTLManager` ([BXTLManager.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Timeline/BXTLManager.h)) ★

**整个技能系统的运行时中枢**。继承自 `UBXManager` 与 `FTickableGameObject`，是单例（通过 `UBXSubSystem` 管理）。

**核心职责**：
- **资产注册表**：`CollectTimelineAssetPath()` 启动时扫描 AssetRegistry，建立 ID → 资源路径映射表 `TimelineAssetMap`；提供 `GetTimelineAssetByID` / `GetSkillAssetByID`（网络同步中资产引用一律传 ID，由注册表解析，防客户端伪造对象引用）
- **运行时驱动**：每帧 `Tick` 调用 `UpdateTimeline(DeltaTime)`，遍历所有 `TimelineRTDatas` 更新运行数据
- **外部容器复用**：`UpdateTimelineRunTimeData` / `StartTimelineSections` / `ChangeTimelineRunTimeDataTickRate` 等接口供 `UBXSkillManager` / `UBXBuffManager` 复用完整时间轴更新管线（运行数据存于外部容器）
- **任务处理器缓存**：`TimelineTaskProcessors` 缓存 Task 类 → Processor 实例的映射（`GetTLTProcessorByTLTClass`）
- **垃圾清理**：`GCInterval`（默认 20s）周期性调用 `CleanTimelineTrash`

**关键方法**：

| 方法 | 说明 |
|---|---|
| `PlayTimeline(Asset, Owner, Context)` | 播放时间轴，生成唯一 ID，创建 `FBXTLRunTimeData`，立即更新一次 |
| `StopTimeline(ID, Reason)` | 停止指定时间轴，通知拥有者组件 |
| `GetTimelineAssetByID(ID)` / `GetSkillAssetByID(ID)` | 资产注册表查询（返回 TSoftObjectPtr，未加载时调用方 LoadSynchronous） |
| `ChangeTimelineTickRate(ID, Rate)` | 改变运行速率（影响所有运行中任务） |
| `ChangeTimelineRunTimeDataTickRate(RTData, Rate)` | 改变外部持有运行数据的速率（加速弥补用） |
| `UpdateTimeline(DeltaTime)` | 遍历更新所有运行数据，检查是否需要结束 |
| `UpdateTimelineRunTimeData(RTData, DeltaTime)` | 更新外部持有的运行数据（Skill/Buff Manager 复用） |
| `StartTimelineSections(RTData)` | 初始化 RunningSections 并触发首帧 KeyFrame（外部容器复用） |
| `FinishTimelineSection(...)` | 结束时间片段，处理待执行任务 |
| `ProcessTimelineSectionPendingTasks(...)` | 处理片段内待执行任务队列 |
| `ExecuteTimelineTask(...)` | 执行单个任务（创建 Processor、StartTask，Skill/Buff 复用） |

#### `UBXTLComponent` ([BXTLComponent.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Timeline/BXTLComponent.h))

挂载在 Actor 上的时间轴组件，是对 `UBXTLManager` 的薄封装。持有 `TimelineRunTimeDataIDs` 列表，提供 `PlayTimeline` / `StopTimeline` / `IsTimelineRunning` 等蓝图接口，并通过 `ScriptReceiveTimelineWillFinish` 暴露结束回调给蓝图。

#### 关键运行时数据结构（[BXTLStructs.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Timeline/BXTLStructs.h)）

| 结构体 | 说明 |
|---|---|
| `FBXTLPlayContext` | 播放上下文：Instigator / Triggerer / LockParts / InputDatas |
| `FBXTLRunTimeData` | 时间轴运行时数据：Timeline / ID / Owner / RunningSections / **DynamicDatas** / **ScopeGraph** |
| `FBXTLSectionRTData` | 片段运行时数据：Index / RunTime / LoopCount / RunningTasks / PendingTasks / BroadcastTasks |
| `FBXTLTaskRTData` | 任务运行时数据：Task / Index / ParentScope / RunTime / NextTick / **DynamicData (FInstancedStruct)** / ServerExtraLifeTimer / bAwaitingClientCollision（服务器等待客户端碰撞上报的额外生命） |
| `FBXTLTaskHostingData` | 托管任务数据（脱离时间轴独立运行的任务） |
| `FBXTLDynamicDataSearchKey` | 动态数据查询键：(Index, Tag, Scope)，含哈希函数 |
| `FBXTLKeyFrame` | 关键帧：Time + Tasks 索引列表 |
| `FBXTLPendingTaskInfo` | 待执行任务信息：LocalIndex / ParentScope / Time |
| `FBXTLBroadcastTaskInfo` | 待广播任务信息：FullIndex / Tag |
| `FBXTLRunTimeDataReplicated` 系列 | 运行数据可复制投影（见下） |

**作用域图（ScopeGraph）** 是数据流的核心机制：`TMap<int64, FInt64Vector2>`，Key 是作用域 ID，Value 的 X 是父作用域 ID、Y 是 Task 全量索引。读取数据时沿父链向上查找，实现作用域隔离与数据继承。

#### 运行数据可复制投影（[BXTLReplicated.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Timeline/BXTLReplicated.h)）

`FBXTLTaskRTDataReplicated` / `FBXTLSectionRTDataReplicated` / `FBXTLRunTimeDataReplicated` 将运行数据中的指针（Timeline→TimelineID、Task→Index）与不可复制类型转换为可序列化字段，用于 Late Join 快照：

- `BXToTLRunTimeProjection(Source, Target, bSimulatedOnly)`：运行数据 → 投影（服务器 PreReplication 调用；`bSimulatedOnly=true` 时按 `Task.NetTypes` 过滤，仅保留含模拟端位的 Task，避免权威/自主端 Task 上下文中的不可复制指针被复制）
- `BXFromTLRunTimeProjection(Source, Asset, Target)`：投影 → 运行数据（客户端重建调用，按 (资产, Index) 恢复 Task 指针；内部队列 PendingTasks/BroadcastTasks 不投影，由重建端自行产生）

**投影已知限制**（Late Join 重建的近似性，均不影响续跑正确性）：

- **ScopeGraph 不投影**：作用域图（运行期事件作用域链）不随快照复制，重建端 ScopeGraph 为空——`ReadContextData` 的作用域链查找全部回退到无作用域键（scope=0）；DynamicDatas 投影保留了双键条目，作用域隔离在重建端退化为全局键读取
- **内部瞬态队列不投影**：PendingTasks / BroadcastTasks / TaskStackInFrame 由重建端自行产生，重建时刻在途的事件链（如碰撞 Success 反应分支）在重建端不会补触发
- **权威/自主端专属 Task 被过滤**：按 `Task.NetTypes` 过滤后仅保留含模拟端位的 Task，重建端（SimulatedProxy）缺此类 Task 属预期（模拟端不执行）
- **服务器内部状态不投影**：`bAwaitingClientCollision` / `ServerExtraLifeTimer` 等服务器侧标记不复制（重建端为客户端，无此语义）
- **快照时点性**：技能侧 `COND_InitialOnly` 快照取 PreReplication 时刻值，重建后与服务器存在一个传播间隔的进度偏差（以 RunTime 直接续跑，不做延迟弥补）

#### `EBXTLFinishReason`（结束原因）

- `FR_EndOfLife`：寿终正寝（正常结束）
- `FR_Interrupt`：被中断
- `FR_PredictionFailure`：预测失败（网络回滚场景）

#### `EBXTLExtraType`（额外时间类型）

前摇 `ET_Anticipation`、后摇 `ET_Recovery`、连段窗口1~8 `ET_ComboWindow1~8`。

---

### 4.3 Task 任务系统（技能原子操作）

任务系统采用 **数据/执行分离** 设计：`UBXTask` 是配置数据（随资产序列化），`UBXTProcessor` 是运行时执行器（Transient，由 Manager 缓存）。

#### `UBXTask` ([BXTask.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Task/BXTask.h)) ★

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
| **Editor** | `Annotation` | 编辑器注释 |
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

**显示名与命名空间机制**（WITH_EDITOR）：

Task 的显示名和分类使用引擎自带的蓝图元数据字段，**不使用自定义 UPROPERTY**。在蓝图 Class Settings 面板设置 "Blueprint Display Name" 和 "Blueprint Namespace"，蓝图编译时引擎自动写入 `UClass` 的 Metadata（参考 `KismetCompiler.cpp:3077-3090`）。

| Getter | 读取方式 | 回退值 |
|---|---|---|
| `GetBlueprintNamespace()` | `GetClass()->GetMetaData("Namespace")` | 空字符串 |
| `GetBlueprintDisplayName()` | `GetClass()->GetMetaData("DisplayName")` | 去除 `BP_BXT_`/`_C` 后缀的类名 |
| `GetDisplayName()` | 组合 `"Namespace:DisplayName"` + 实例后缀（如 `_0`） | 去除前缀的实例名 |

> **注意**：Metadata key 名是 `"DisplayName"` / `"Namespace"`，**不是** `"BlueprintDisplayName"` / `"BlueprintNamespace"`。不要在 C++ 中用 UPROPERTY 重新定义这两个字段——`WITH_EDITORONLY_DATA` 块内的 UPROPERTY 会被自动加上 `CPF_Transient` 标志导致不序列化。

#### `UBXTProcessor` ([BXTProcessor.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Task/BXTProcessor.h)) ★

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

##### `UBXTPlayAnimation` ([BXTAnimation.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Task/BXTAnimation.h))

播放动画任务。支持蒙太奇或动画库（按 Tag）。配置：`PlayRate`、`InterruptedBlendOut`、`DelayInterruptedByMove`（N 秒后可被移动中断）、`PlayComponentName`、`PlayAnimBehaviorTag`、`PlayPriority`、`bStopGroup`（终止同组蒙太奇）、`bEnableRootmotion`。

##### `UBXTCollision` / `UBXTTrackHitBox` / `UBXTTrackWeaponHitBox` ([BXTCollision.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Task/BXTCollision.h))

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

#### `UBXBehaviorComponent` ([BXBehaviorComponent.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/State/BXBehaviorComponent.h))

行为管理组件。通过 GameplayTag 标识行为（如 `BXBehavior_Locomotion_Move`）。

| 配置 | 说明 |
|---|---|
| `ForbiddenBehaviorConfigs` | 行为禁用表：A 行为执行时禁用哪些行为 |
| `InterruptBehaviorConfigs` | 行为中断表：A 行为开始前中断哪些行为 |
| `BehaviorAgentConfigs` | 行为代理类：Tag → `UBXBehaviorAgent` 子类 |

**API**：`CheckForbiddenBehavior`、`ChangeForbiddenBehavior`、`CheckActiveBehavior`、`StartBehavior` / `StartBehaviorWithParameter`、`StopBehavior` / `StopBehaviorWithParameter`。

`FBXForbiddenBehaviorInformation` 记录禁用签名 `Sign` 与原因 `EBXForbiddenBehaviorReason`，支持多重禁用计数。

#### `UBXBehaviorAgent` ([BXBehaviorAgent.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/State/BehaviorAgent/BXBehaviorAgent.h))

行为代理基类。`Abstract, Blueprintable`。生命周期：`Initialize` → `StartBehavior` → `StopBehavior` → `Deinitialize`，并有 `CheckStartBehavior` 前置检查。每对方法都有 Native/Script 双版本，由 `BehaviorFunctions` Bitmask（默认 341）控制。

**具体代理**：
- `UBXBAMove` / `UBXBARotate`：移动/旋转行为
- `UBXBAJump` / `UBXBALanded`：跳跃/着陆行为

#### `UBXStateComponent` ([BXStateComponent.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/State/BXStateComponent.h))

状态机管理组件。`StateMachineConfig` 配置每个状态机类负责哪些状态 Tag。每帧 `TickComponent` 调用 `UpdateStateMachine` 驱动所有状态机。API：`EnterState` / `EnterStateWithParameter` / `ExitState` / `ExitStateWithParameter`。

#### `UBXStateMachine` ([BXStateMachine.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/State/StateMachine/BXStateMachine.h))

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

#### `UBXDecisionTreeTemplate` ([BXDecisionTreeTemplate.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/DecisionTree/BXDecisionTreeTemplate.h))

资产容器。`RootNodes`（根节点列表）、`TreeEdges`（所有边的扁平数组）、节点通过 `OutEdges`/`InEdges` 的整数索引引用边。编辑器方法 `InitByEditor` / `InitSaver` / `AddEdgeMessage` / `RefreshLogicMessage` 维护图结构。

#### `UBXDecisionTreeActuator` ([BXDecisionTreeActuator.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/DecisionTree/BXDecisionTreeActuator.h))

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

#### `ABXGear` ([BXGear.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Gear/BXGear.h))

装备 Actor 基类（Abstract）。持有 `GearType`（Tag）、`GearData`（资产）、`OwnerComponent`。四个生命周期阶段（每阶段 Pre/Post + C++/BP 双实现）：
- **Equip/Unequip**：装备/卸载
- **Using/Unusing**：使用/收起
- **ChangeState**：状态切换（如开/合）

`EquipFunctions`/`UseFunctions`/`ChangeStateFunctions` Bitmask（默认 85）控制 Native/BP 执行。通过 `AttachToSocket()` 与 `AttachmentConfigs` 管理附着。

#### `UBXGearComponent` ([BXGearComponent.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Gear/BXGearComponent.h))

每 Actor 装备库存组件。`SlotMaxSize`、`EquipGears`（插槽→装备列表映射）、`UsingGearIndexs`。9 个多播委托（Pre/PostEquip/Unequip、Pre/PostUsing/Unusing、ChangeGearState）。API：`GetEquipGearList`、`SwitchUsingGear`、`ChangeUsingGearState`、`ChangeEquipGear` / `ByClass` / `ByData`、`UnequipAllGears`。

#### `ABXMeleeWeapon` ([BXMeleeWeapon.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Gear/BXMeleeWeapon.h))

近战武器基类。持有 `UBXShapeComponent`（HitBoxComponent）用于命中检测。提供 `GetHitBoxComponent` / `GetHitBoxTransform`。

#### `BXGearEnums.h`

Bitflags 枚举：`EBXEquipGearFunction`、`EBXUseGearFunction`、`EBXChangeGearStateFunction`，每对 Native/BP 变体对应一个生命周期阶段。

---

### 4.7 Movement 移动系统

#### `UBXCharacterMovementComponent` ([BXCharacterMovementComponent.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Movement/BXCharacterMovementComponent.h))

继承 `UCharacterMovementComponent`。新增：
- `bProactiveMoving` / `bProactiveRotating`：主动移动/旋转标志
- **轨迹历史缓冲**：`TrajectoryPoints`（默认记录 20s），`RecordTime`，`CleanInterval`/`CleanTimer` 周期清理
- `GetHistoryTransformByTime(InTime)`：按时间查询历史 Transform（供 `UBXTProcessor::GetTargetTransformByWorldTime` 使用）

重写：`CalcVelocity`、`ComputeSlideVector`、`PhysicsRotation`、`ProcessLanded`、`CanAttemptJump`、`DoJump`。

#### `FBXRootMotionSource_Uppercut` ([BXRootMotionSource.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Movement/BXRootMotionSource.h))

自定义网络复制的 RootMotion 源，用于升龙拳等弧线位移技能。配置：`Direction`、`Distance`、`Height`、`MoveTimeMappingCurve`，可选朝向插值（`bChangeOrientation`、`StartOrientation`/`EndOrientation`、`RotateTimeMappingCurve`）。实现 `PrepareRootMotion` / `Clone` / `Matches` / `UpdateStateFrom` / `NetSerialize`。

---

### 4.8 Collision 碰撞系统

#### `UBXCollisionLibrary` ([BXCollision.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Collision/BXCollision.h))

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

#### `UBXShapeComponent` ([BXShapeComponent.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Collision/BXShapeComponent.h))

持有 `ShapeInformations`（Tag → `FBXShapeInformation`），提供 `GetShapeTransformByTag`。用于在武器/角色上定义具名形状区域。

#### `UBXHitReactionComponent` ([BXHitReactionComponent.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Collision/BXHitReactionComponent.h))

基于身体部位的受击反应组件。`BodyPartConfigs`（Tag → `FBXBodyPartConfig`）配置、`BodyPartRTDatas`（Tag → `FBXBodyPartRTInformation`）运行时数据。API：`GetBodyPartByType`、`GetBodyPartByComponentAndBone`。

---

### 4.9 Event 事件系统

#### `UBXEventManager` ([BXEventManager.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Event/BXEventManager.h))

继承 `UBXManager`。轻量级事件系统，以 GameplayTag 为事件名，支持两种事件：
- **全局事件**：仅以 `FGameplayTag` 为键
- **单播事件**：以 `FBXESingleKey`（EventName + InitiatorUniqueID）为键，支持按发起者定向

通过反射将事件参数结构体分发到 UFunction。`FBXEventManagerODL` 实现 `FUObjectDeleteListener`，在注册的 UObject 被销毁时自动清理关联。`DefinedEvents`（Tag → UScriptStruct*）声明事件参数类型。

API：`RegisterGlobalEvent` / `UnregisterGlobalEvent` / `BroadcastGlobalEvent`（模板/UScriptStruct/BP CustomThunk 三种重载）；单播版同构。

#### 事件参数结构体（[BXEventStructs.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Event/BXEventStructs.h)）

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

#### `UBXAnimInstance` ([BXAnimInstance.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Animation/BXAnimInstance.h))

自定义 AnimInstance，对接技能系统的动画播放需求。

#### `UBXAnimationLibrary` ([BXAnimationLibrary.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Animation/BXAnimationLibrary.h))

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
| `BXBuffManager.h/.cpp` | BUFF 管理器，继承 `UBXManager` + `FTickableGameObject`。管理 BUFF 生命周期、层级、共存策略、事件广播、服务器校验（`ServerValidateAddBuff`） |
| `BXBuffComponent.h/.cpp` | Actor 组件，提供 BP 友好 API + 网络 RPC + 复制快照维护，EndPlay 时自动移除所有 BUFF |
| `BXBuffReplicated.h` | `FBXBuffReplicatedState`（BUFF 运行状态复制快照：BuffID/BuffAssetID/Layer/Level/到期时间戳） |
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
| 服务器校验（钳制） | UBXBuffManager | `ServerValidateAddBuff`（层级/等级钳制到资产范围） |
| 指定ID添加 | UBXBuffManager | `AddBuffWithID`（网络同步路径用，保证跨端 BuffID 一致） |
| 网络同步 | UBXBuffComponent | RPC + 复制快照（见 [4.14](#414-skill--net-网络同步系统客户端预测与服务器权威)） |

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

在 `Config/DefaultBattleX.ini` 的 `[/Script/BattleX.BXSettings]` 段中注册 BUFF 管理器（当前项目注册的是蓝图派生类，便于项目侧扩展）：
```
+ManagerClasses=/Script/Engine.BlueprintGeneratedClass'/BattleX/BP/Manager/BP_BXM_Buff.BP_BXM_Buff_C'
```

#### 网络同步

BUFF 的客户端请求/服务器校验/广播/快照重建机制见 [4.14 Skill / Net 网络同步系统](#414-skill--net-网络同步系统客户端预测与服务器权威)（BUFF 侧子节）。

---

### 4.14 Skill / Net 网络同步系统（客户端预测与服务器权威）

`Source/BattleX/Net/` 与 `Source/BattleX/Skill/` 实现了完整的技能/BUFF 网络同步框架。**核心思路**：服务器权威 + 自主端客户端预测 + 显式 Reliable RPC 维护动态 + 属性复制快照兜底 Late Join。Task 执行链路完全复用 Timeline（技能运行数据 `FBXSkillRuntimeData` 内嵌 `FBXTLRunTimeData`，由 `UBXSkillManager::InternalUpdateSkill` 委托 `UBXTLManager::UpdateTimelineRunTimeData` 推进）。

#### 核心文件

| 文件 | 说明 |
|---|---|
| `BXNetEnums.h` | `EBXSyncInitiator`（Client/Server 发起端）+ 同步 ID 位布局宏（bit 61~63 为发起端标识，低 61 位为 ID 数值） |
| `BXNetStructs.h` | `FBXSkillSyncHeader`（同步头）、`FBXSkillInputEntry`（输入条目，RPC 不支持 TMap）、`FBXSkillSyncPayload`（同步载荷）、`BXMakeSyncID` / `BXGetSyncIDInitiator` |
| `BXSkillReplicated.h` | `FBXSkillReplicatedState`（技能复制快照条目，含 `FBXTLRunTimeDataReplicated` 投影）；文件头注释完整记录了 UE 属性复制时序与快照方案选型 |
| `BXSkillAsset.h/.cpp` | 技能资产（见 [4.2](#42-timeline-时间轴系统技能核心)）：释放条件 / 冷却 / 锁定类型 |
| `BXSkillComponent.h/.cpp` | 技能组件：BP 友好 API + 同步 RPC + 假冷却管理 + 复制快照 |
| `BXSkillManager.h/.cpp` | 技能管理器：运行数据容器 + 预测生命周期 + 加速弥补 + 服务器校验 + 碰撞结果接收 |
| `BXSkillEnums.h` | `EBXPredictState`（None/Predicting/Confirmed/RollingBack）、`EBXSkillLockType`（None/Target/Location/Direction） |
| `BXSkillStructs.h` | `FBXSkillRuntimeData`（技能运行时数据） |

#### 同步 ID 设计（防 ABA 与跨端碰撞）

`SkillID`（int64）由 `BXMakeSyncID(RawID, Initiator)` 构造：高 3 位（bit 61~63）编码发起端类型，低 61 位为 `UBXFunctionLibrary::GetUniqueID()` 生成的递增 ID。服务器校验时通过 `BXGetSyncIDInitiator` 检查发起端标识，拒绝"客户端伪造服务器 ID"或"ID 已存在"的请求，防止跨端 ID 碰撞覆盖正在运行的技能。

#### RPC 传输结构

- **`FBXSkillSyncHeader`**（所有技能 RPC 共用）：`SkillID`（跨端一致的运行时唯一 ID）、`SkillAssetID`（资产 ID，服务器经注册表解析）、`Initiator`、`ClientTimestamp` / `ServerTimestamp`（服务器世界时间域毫秒，经 `GetServerWorldTimeMilliseconds` 取值，禁止跨机器比较本地墙钟）
- **`FBXSkillSyncPayload`**（兼作施放上下文与 RPC 传输体）：`Instigator` / `Triggerer` / `ClientTimestamp` / `InputDatas`（`TArray<FBXSkillInputEntry>`，承载释放位置/朝向/瞄准/锁定等全部技能输入；释放位置与朝向由 `PlaySkillWithInputData` 自动补充）

#### UBXSkillComponent（组件层）

**RPC 列表**：

| RPC | 可靠性 | 职责 |
|---|---|---|
| `ServerPlaySkill(Header, Payload)` | Server, Reliable | 自主端上传释放请求（单一 RPC，所有数据经 Payload） |
| `ClientPredictResult(SkillID, bSuccess, ServerTimestamp, CooldownRemaining)` | Client, Reliable | 服务器回传预测结果；`CooldownRemaining>=0` 为权威冷却剩余（确认时假冷却转正/拒绝时同步真冷却），-1 代表无服务器冷却（否认时移除假冷却） |
| `MulticastPlaySkill(Header, Payload)` | NetMulticast, Reliable | 广播完整播放数据（丢包会导致远端永远看不到该技能） |
| `MulticastStopSkill(SkillID, FinishReason)` | NetMulticast, Reliable | 广播技能中断（服务器主动 Stop / Actor 死亡 / 条件强制中断） |
| `ServerReportCollisionResults(SkillID, TaskFullIndex, DataTag, HitResults)` | Server, Unreliable | 碰撞结果上报（见下） |

**关键设计**：
- **服务器校验编排**：`HandleServerPlaySkill`（组件内部方法）串联完整校验链——注册表解析（`UBXTLManager::GetSkillAssetByID`）→ SkillID 校验 → 冷却校验（组件私有状态）→ `ServerValidateRelease`（Manager，时间戳+释放条件）→ 播放 + 回传预测结果 + 加速弥补 + 广播
- **假冷却**：自主端预测启动时本地立即记录冷却防连点（`PendingCooldownAssetIDs` 标记），服务器确认时转正为权威冷却，否认且无服务器冷却时移除允许重试
- **复制快照**：`RunningSkillStates` 标记 `COND_InitialOnly`，配合 `PreReplication` 每帧从 `SkillRTDatas` 重建快照——新连接初始同步拿到"打开通道时刻"的最新运行数据投影，已有连接零属性流量（技能动态完全由 Reliable RPC 维护）
- **OnRep 差分**：新增条目 `RebuildSkillFromState` 重建续跑（已存在 ID 幂等跳过）；消失条目 `StopSkillIfNotPredicting` 兜底停止（仅处理 RPC 与属性乱序竞态，预测中实例不因快照消失而停止）
- **EndPlay**：停止 `OwnedSkillIDs` 中所有技能（含客户端发起、服务器登记的技能）

#### UBXSkillManager（管理器层）

| 职责 | 关键函数 | 说明 |
|---|---|---|
| 播放 | `PlaySkill` / `InternalPlaySkill`（含移动语义重载） / `StopSkill` | Standalone/服务器直接播放；`InternalPlaySkill` 支持外部传入指定 ID（同步用） |
| 预测生命周期 | `StartPrediction` / `ConfirmPrediction` / `RollbackPrediction` | 预测中实例超时由 `CheckPredictTimeout`（`SkillPredictMaxDuration`）自动回滚；回滚以 `FR_PredictionFailure` 停止所有 Task 并广播 `BXEvent_Skill_PredictFailed` |
| 加速弥补 | `ServerAccelerate` / `ClientAccelerate` / `UpdateAccelerate` | 服务器按"RPC 上行延迟"、远端客户端按"广播传播延迟"加速播放（`延迟/倍率` 时长内倍速推进）；`ClampAccelerateDuration` 钳制加速时长上限为技能总时长 50%（含无限循环片段的技能总时长无界，跳过钳制） |
| 释放校验 | `ServerValidateRelease` | 时间戳双向校验（`|RequestAge| > SkillRequestMaxAgeMs` 拒绝，防重放/伪造）+ 释放条件校验（复用 `UBXConditionManager`） |
| 碰撞接收 | `ReceiveCollisionResults` | 服务器关闭自身碰撞检测时接收客户端上报（开启时忽略，防重复结果）；匹配碰撞 Task 后双键写入数据并触发 `BXTEvent_Success` 驱动服务器侧反应分支 |
| 快照重建 | `RebuildSkillFromProjection` | 反投影恢复运行数据直接续跑；ID 已存在时幂等返回 |
| 数据容器 | `SkillRTDatas`（ID→运行数据）/ `OwnerSkillMap`（Owner 反向索引） | 运行数据内嵌 TLRunTimeData，Task 执行复用 `UBXTLManager` |

**技能事件**：`BXEvent_Skill_Released`（释放，InternalPlaySkill 末尾）、`BXEvent_Skill_PredictSuccess` / `BXEvent_Skill_PredictFailed`（预测确认/回滚）、`BXEvent_Skill_Finished`（结束，携带 FinishReason）。均单体+全局双广播。

#### BUFF 侧同步（UBXBuffComponent）

| RPC | 可靠性 | 职责 |
|---|---|---|
| `ServerRequestAddBuff(BuffAssetID, Instigator, Layer, Level)` | Server, Reliable | **传资产 ID 而非对象引用**，服务器经 `GetTimelineAssetByID` 注册表解析，防客户端伪造对象引用；`ServerValidateAddBuff` 钳制 Layer/Level 到资产 `[1, MaxLayer]` / `[1, MaxLevel]` 范围（越界 InitLayer 会撑爆 LayerRunTimes 数组） |
| `ServerRequestRemoveBuff(BuffID, LayerDelta)` | Server, Reliable | 请求移除 |
| `MulticastAddBuff(...)` / `MulticastRemoveBuff(...)` | NetMulticast, Reliable | 广播增删（移除广播由 `InternalRemoveBuff` 统一收束发送，本地事件 Reason 与服务器一致） |

**复制策略差异**（与技能侧对比）：`RunningBuffStates` 为**无条件复制**（非 COND_InitialOnly）——BUFF 变化低频（增删/层级/时长刷新），快照同时承担初始重建与已重建客户端的层/级/到期持续同步（层级变化唯一同步通道，增删由 Reliable RPC 主通道承担）。到期时间戳使用**服务器世界时间域毫秒**（`SharedExpireServerTimestamp` / `LayerExpireServerTimestamps`），客户端按时间戳差值回填运行时长，Late Join 重建的 BUFF 剩余时长与服务器对齐。

#### 碰撞结果上报

`bServerCollisionCheck=false`（默认）时服务器不执行碰撞检测（省 DedicatedServer 开销），由拥有连接的 Autonomous 客户端执行并经 `ServerReportCollisionResults`（Unreliable）上报。服务器 `ReceiveCollisionResults` 按 TaskFullIndex 匹配碰撞 Task 后：清除等待标记 → 以服务器侧作用域**双键写入** DynamicDatas（无作用域键+作用域键，与客户端 `CollisionCheck` 的 `WriteContextData` 行为一致）→ **触发 `BXTEvent_Success` 事件**（反应分支 Task 在服务器侧启动，事件作用域读取键可达，实现"由客户端结果驱动服务器侧流程"）→ 广播 `BXEvent.Collision.Reported`。Task 已结束时退化为单键写入供后续时间片段按索引读取兜底。服务器端碰撞 Task 以 `ServerExtraLifeTimer`（`CollisionTaskServerExtraLife`）延长生命等待上报，超时销毁。已知缺口：Listen Server 本地客户端的归属判定待测试确认。

#### 端到端流程

- 技能：客户端预测 → 服务器校验 → 确认/回滚 → 广播 → 加速弥补 → Late Join 快照重建，详见 [8.6](#86-技能网络同步与客户端预测流程)
- BUFF：客户端请求 → 服务器解析钳制 → 广播 → 快照重建，详见 [8.7](#87-buff-网络同步流程)

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

**`FBXTLEditor`**（[BXTLEditor.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleXEditor/TimelineEditor/BXTLEditor.h)）：
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
- `FBXTLPreviewProxy`：运行时驱动，通过 `UBXTLComponent` 实际播放/暂停/停止时间轴资产。提供 `GetRunningTasks(OutTasks)` 接口收集当前正在执行的 Task 集合（Debug 高亮使用）
- `UBXTLPreviewActor`：描述生成配置（类、Transform、装备、锁定部位）
- `BXTLPreviewGameMode` / `BXTLPreviewGameState`：预览专用 GameMode

**命令**（`BXTLEditorCommands`）：Play / Pause / Stop / Step / Bake / ResetWorld / ShowCollision / RefreshTimelineAssetProperty / AutoOptimize

**事件**（`BXTLEditorDelegates.h`）：`RefreshPanelEvent`、`PreviewChangedEvent`、`TaskSelectedEvent`、`RunningTasksChangedEvent`（Debug：正在执行的 Task 集合变化）

**Debug 高亮系统**：预览播放时,`FBXTLEditor::Tick` 每帧收集当前 RunningTasks 并更新静态缓存 `DebugRunningTasksCache`,提供 `IsTaskRunning(UBXTask*)` 静态查询接口。Graph 节点（`SBXTLGraphNode::OnPaint`）和时间轴节点（`SBXTLTaskTrackNode::OnPaint`）在绘制末尾查询该接口,若命中则在最顶层叠加黄色呼吸高亮框（4 条 2px 边,用 `FPlatformTime::Seconds()` 驱动 sin 呼吸 alpha,范围 0.35~0.8）。由于 `SBXBuffGraphNode` 继承 `SBXTLGraphNode` 且 `FBXBuffEditor` 继承 `FBXTLEditor`,BUFF 编辑器自动复用整套高亮机制。

**Task 创建菜单**（[BXTLEditorUtilities.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleXEditor/TimelineEditor/BXTLEditorUtilities.h)）：

三个编辑器（时间轴 Graph 右键菜单、时间轴 Task Track 添加菜单、BUFF Graph 右键菜单）统一的 Task 创建入口：
- `FBXTLEditorUtilities::CollectBPTaskClasses()`：通过 Asset Registry 收集所有 `BP_BXT_` 前缀的非抽象蓝图 Task 类（过滤掉原生 C++ Task 类，避免显示未注册到 `TaskProcessorMap`/`TaskCustomDataMap` 的类）
- `FBXTLEditorUtilities::MakeNewTaskPicker()`：构建按 `BlueprintNamespace` 分组的可折叠子菜单，每个子菜单内列出该命名空间下的 Task，显示 `BlueprintDisplayName`
- `UBXBuffGraphSchema::GetGraphContextActions()`：BUFF Graph 右键菜单，以 `BlueprintNamespace` 作为 `FEdGraphSchemaAction` 的 Category 分组，显示 `BlueprintDisplayName`

节点标题显示格式为 `"Namespace:DisplayName"` + 实例后缀（如 `"动画:播放动画_0"`），由 `UBXTask::GetDisplayName()` 提供。

### 5.3 决策树编辑器（DecisionTreeEditor）

**`FBXDTEditor : FAssetEditorToolkit, FNotifyHook, FGCObject`**（[BXDTEditor.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleXEditor/DecisionTreeEditor/BXDTEditor.h)）：
- 管理 `SGraphEditor` 视口栈（支持钻入子图并返回）
- 属性/编辑器设置细节面板
- 复制/剪切/粘贴/复制/重命名命令
- 类过滤器限制节点为 `UBXDecisionTreeType` 子类
- 标签页：Viewport / Details / EditorSettings

**图**：`UBXDTEditorGraph`（容器）、`UBXDTEditorSchema`（含 `NewNode`/`NewEdge` 动作、断链菜单、落点引脚）、`UBXDTEditorGraphNode`/`UBXDTEditorGraphEdge` 及其 `SGraphNode`/`SGraphEdge`。

### 5.4 时间轴基础控件（TimelineBase）

**资产无关的通用时间轴 Slate 框架**，TimelineEditor 通过子类化复用：

- **`FTimelineController`**（[TimelineController.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleXEditor/TimelineBase/TimelineController.h)）：抽象控制器。管理 `RootTracks`/`SelectedTracks`、`ViewRange`/`WorkingRange`、播放范围、播放头位置、帧率、Track 选择、`OnTracksChanged` 事件
- **`FTimelineTrack`**（[TimelineTrack.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleXEditor/TimelineBase/TimelineTrack.h)）：Track 基类，树形结构，含 RTTI 宏系统
- **`STimeline`**（[STimeline.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleXEditor/TimelineBase/STimeline.h)）：时间轴 UI，持有 `SOutliner` + `STrackArea` + `FTimeSliderController` + 顶部 `ITimeSlider`，支持网格、吸附、显示格式切换（帧/百分比/次要）
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
- `GameplayTags` / `StructUtils` / `NetCore`

**PrivateIncludePaths**：暴露了所有子目录（`Task`、`Unit`、`Lock`、`Gear`、`State`、`Event`、`Movement`、`Timeline`、`Animation`、`Collision`、`Condition`、`DecisionTree`、`Buff`、`Skill`、`Net`）

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

Skill (技能系统) ──复用──> Timeline (数据载体 + Task执行链路)
    │                └──复用──> Task (Processor)
    └──依赖──> Event (技能事件广播) / Condition (释放条件校验)

Buff (状态系统) ──复用──> Timeline (数据载体 + Task执行链路)
    │              └──复用──> Task (Processor)
    └──依赖──> Event (BUFF生命周期事件广播)

Net (同步框架) ──被依赖──< Skill / Buff (RPC结构与复制快照)
    └──依赖──> Timeline (BXTLReplicated 投影 + 资产ID注册表)

Gear (装备) ──依赖──> Collision (形状组件)
Movement ──被依赖──< Task (RootMotion 任务)
            └──被依赖──< BXFunctionLibrary (历史轨迹查询)

基础设施 (Manager/SubSystem/Settings/Tags/Func) <──所有系统依赖
```

### 6.4 关键映射配置

`UBXSettings` 中的两张映射表是系统的"接线图"：
- **`TaskProcessorMap`**：Task 类 → Processor 类。`UBXTLManager::Initialize` 时加载并缓存为 `TimelineTaskProcessorTypeMap`，运行时通过 `GetTLTProcessorByTLTClass` 查询 Processor 实例
- **`TaskCustomDataMap`**：Task 类 → 自定义动态数据 UScriptStruct。缓存为 `TimelineTaskTypeMap`

映射键为蓝图 Task 类的软类路径（蓝图 Task 需显式注册，原生 C++ Task 类不显示在编辑器创建菜单中），`Config/DefaultBattleX.ini` 当前注册：

```ini
TaskProcessorMap=(("/BattleX/BP/Task/Animation/BP_BXT_PlayAnimation.BP_BXT_PlayAnimation_C", "/Script/BattleX.BXTPPlayAnimation"),("/BattleX/BP/Task/Collision/BP_BXT_TrackWeaponHitBox.BP_BXT_TrackWeaponHitBox_C", "/Script/BattleX.BXTPTrackWeaponHitBox"),("/BattleX/BP/Task/FlowControl/BP_BXT_Switch.BP_BXT_Switch_C", "/Script/BattleX.BXTPSwitch"))
TaskCustomDataMap=(("/BattleX/BP/Task/Animation/BP_BXT_PlayAnimation.BP_BXT_PlayAnimation_C", "/Script/BattleX.BXTPPlayAnimationContext"),("/BattleX/BP/Task/Collision/BP_BXT_TrackWeaponHitBox.BP_BXT_TrackWeaponHitBox_C", "/Script/BattleX.BXTPTrackHitBoxContext"),("/BattleX/BP/Task/FlowControl/BP_BXT_Switch.BP_BXT_Switch_C", "/Script/BattleX.BXTPSwitchContext"))
```

查找使用**精确类匹配**：新建蓝图 Task（如 `BP_BXT_TrackHitBox`）必须在此注册，否则 `GetTLTProcessorByTLTClass` 无法命中。

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

**推荐方式**：在角色 Actor 上挂载 `UBXSkillComponent`，走技能系统（含冷却/释放条件/网络同步/客户端预测）：

```cpp
// C++ 示例（网络下自主端自动走客户端预测，服务器端直接权威播放）
UBXSkillComponent* SkillComp = FindComponentByClass<UBXSkillComponent>();
int64 SkillID = SkillComp->PlaySkill(SkillAsset, /*Instigator*/ this);
// 定向变体：PlaySkillWithTarget / PlaySkillWithLocation / PlaySkillWithDirection / PlaySkillWithInputData
```

```blueprint
// 蓝图示例
Play Skill (InAsset: SkillAsset, InInstigator: Self) → Returns SkillID
```

**免网络的单机路径**：直接挂 `UBXTLComponent` 播放时间轴（无冷却/预测逻辑）：

```cpp
FBXTLPlayContext Context;
Context.Instigator = this;
Context.Triggerer = this;
// Context.LockParts / Context.InputDatas 可选
UBXTLComponent* TLComp = FindComponentByClass<UBXTLComponent>();
int64 TimelineID = TLComp->PlayTimeline(SkillAsset, Context);
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
   └─ UpdateTimeline(DeltaTime) → ID 快照遍历 TimelineRTDatas（Task/BP 回调可能同步 StopTimeline 移除条目）
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
1.5 网络分叉 (UBXBuffComponent::AddBuff)
   ├─ 服务器权威端: 直接走 2，成功后 MulticastAddBuff 广播
   ├─ 自主端: ServerRequestAddBuff(Asset->ID, ...) → 服务器走 2 (注册表解析+钳制校验后)
   └─ 模拟端: 返回 INDEX_NONE (由广播/快照驱动)
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

### 8.6 技能网络同步与客户端预测流程

```
【自主端 (AutonomousProxy)】
1. SkillComp->PlaySkillWithInputData(Asset, Inputs, ...)
   ├─ 补充释放位置/朝向到 InputDatas (条目数组形态, 逐条移动无深拷贝)
   ├─ 构造 FBXSkillSyncHeader (SkillAssetID + ClientTimestamp 服务器世界时间域)
   ├─ 本地冷却检查 (防连点) → StartPrediction:
   │     SkillID = BXMakeSyncID(GetUniqueID(), Client)
   │     InternalPlaySkill(..., InSkillID, Client) + PredictState=Predicting
   ├─ 立即记录"假冷却" (PendingCooldownAssetIDs 标记)
   └─ ServerPlaySkill(Header, Payload)                      [Server, Reliable]
       │
       ↓
【服务器 (Authority)】
2. SkillComp::HandleServerPlaySkill (组件内编排服务器校验链)
   ├─ GetSkillAssetByID(SkillAssetID) 注册表解析资产 (UBXTLManager)  (失败→否认)
   ├─ SkillID 校验: 发起端=Client 且服务器上不存在           (防伪造/防ID碰撞覆盖)
   ├─ 冷却校验 (组件私有状态 IsCooldownReady)                 (失败→否认+回传权威冷却剩余)
   ├─ ServerValidateRelease (UBXSkillManager):
   │     时间戳双向校验 (|RequestAge| ≤ SkillRequestMaxAgeMs, 防重放/伪造)
   │     释放条件校验 (ReleaseConditions × ConditionManager)
   ├─ 校验通过: InternalPlaySkill(指定 SkillID, UBXSkillManager) + RecordCooldown
   ├─ ClientPredictResult(SkillID, true, ServerTimestamp, 权威冷却剩余)  [Client, Reliable]
   ├─ ServerAccelerate: 按上行延迟(ServerTimestamp-ClientTimestamp)加速弥补
   │     加速时长 = 延迟/ServerAccelerateRate, 钳制 ≤ 技能总时长50%
   └─ MulticastPlaySkill(Header, Payload)                    [NetMulticast, Reliable]
       │
       ↓
【自主端收到预测结果】
3. ClientPredictResult
   ├─ 成功: ConfirmPrediction (PredictState→Confirmed, 广播 BXEvent_Skill_PredictSuccess)
   │        假冷却转正 (本地时间+服务器剩余时长)
   └─ 失败: RollbackPrediction → InternalRollbackPredictedSkill
            StopSkillTasks(FR_PredictionFailure) + bEarlyFinish
            广播 BXEvent_Skill_PredictFailed; 冷却按服务器返回结算
            (预测超时未收到结果: CheckPredictTimeout 自动回滚; 运行数据清理时
             InternalOnSkillFinished 释放假冷却允许重试, 迟到的结果无登记直接跳过)

【其他客户端 (SimulatedProxy) 收到广播】
4. MulticastPlaySkill → 已有实例跳过 → HandleClientPlaySkill
   └─ InternalPlaySkill(广播的 SkillID) + ClientAccelerate (按广播传播延迟加速弥补)

【Late Join / 新复制对象】
5. RunningSkillStates (COND_InitialOnly) 初始同步
   └─ OnRep 差分 → 新增条目 RebuildSkillFromProjection (反投影恢复运行数据直接续跑)
                   → 消失条目 StopSkillIfNotPredicting (乱序兜底)

【技能中断】
6. MulticastStopSkill (服务器主动Stop/Actor死亡) → 各端 StopSkill + 广播 BXEvent_Skill_Finished

【碰撞结果上报 (bServerCollisionCheck=false)】
7. 拥有连接的客户端 CollisionCheck 命中
   └─ ServerReportCollisionResults(SkillID, TaskFullIndex, DataTag, HitResults)  [Server, Unreliable]
       → 服务器 ReceiveCollisionResults:
         ├─ 匹配碰撞 Task → 清除 bAwaitingClientCollision/ServerExtraLifeTimer
         ├─ 生成服务器侧作用域, 双键写入 DynamicDatas (与客户端 WriteContextData 一致)
         ├─ 触发 BXTEvent_Success → 反应分支 Task 在服务器侧启动 (客户端结果驱动服务器侧流程)
         └─ 广播 BXEvent.Collision.Reported
```

### 8.7 BUFF 网络同步流程

```
【自主端】
1. BuffComp->AddBuff(Asset, Instigator, Layer, Level)
   └─ ServerRequestAddBuff(Asset->ID, Instigator, Layer, Level)   [Server, Reliable]
       │
       ↓
【服务器】
2. BuffComp::ServerRequestAddBuff_Implementation
   ├─ GetTimelineAssetByID(BuffAssetID) 注册表解析 (UBXTLManager, 防客户端伪造对象引用)
   ├─ ServerValidateAddBuff: InitLayer→[1,MaxLayer], InitLevel→[1,MaxLevel] 钳制
   ├─ AddBuff (UBXBuffManager, 服务器生成 BuffID; 内部经组件 AddBuffReplicatedState 登记快照)
   └─ MulticastAddBuff(BuffID, Asset, ...)                      [NetMulticast, Reliable]
       │
       ↓
【所有客户端】
3. MulticastAddBuff → AddBuffWithID(服务器下发的 BuffID) 本地重建
   └─ 层/级变化: RunningBuffStates 快照持续同步 (无条件复制,层级变化唯一同步通道)

【Late Join / 新复制对象】
4. RunningBuffStates (无条件复制) OnRep 差分
   ├─ 新增条目: RebuildBuffFromState → GetTimelineAssetByID 解析资产
   │             + AddBuffWithID 重建 + 按服务器到期时间戳回填剩余时长
   ├─ 变化条目: ApplyBuffStateChange (层/级/到期对齐)
   └─ 消失条目: RemoveBuffIfLocalExists (兜底移除)

【移除】
5. 服务器 RemoveBuffWithReason → MulticastRemoveBuff(BuffID, Reason)  [Reliable]
   (本地事件 Reason 与服务器一致) + RemoveBuffReplicatedState (快照删除)
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

**现状**：技能/BUFF 同步框架已落地（客户端预测/假冷却/服务器校验/回滚/加速弥补/Reliable 广播/Late Join 快照重建，见 [4.14](#414-skill--net-网络同步系统客户端预测与服务器权威)），Task 通过 `NetTypes` Bitmask 区分端，`FBXRootMotionSource_Uppercut` 支持 `NetSerialize`。

**待完善**（按优先级）：
- **PIE 网络测试收尾**：跑通 8 个同步用例；关键判据为 Late Join 重建日志 `RunTime ≈ 服务器已播时长`、无 "Entry vanished" 日志（该日志指示 RPC 与属性复制乱序竞态）。
- **Listen Server 碰撞归属**：`bServerCollisionCheck=false` 时服务器自身视角的碰撞检测依赖 AutonomousProxy 客户端上报，Listen Server 本地客户端（NM_ListenServer 且 Authority）当前被 `IsServerWithoutCollisionCheck` 判为"服务器无碰撞检测"，其本地视角碰撞由谁执行待测试确认。
- **层级同步时延**：层级变化仅经 `RunningBuffStates` 无条件复制同步（原 `MulticastBuffLayerChanged` RPC 已删除，空实现且与快照通道冗余），属性复制频率低于 RPC，瞬时层级变化感知略慢，视测试反馈决定是否需要恢复 Reliable 推送通道。
- **测试期日志清理**：`BXMGR_Skill` / `BXMGR_Buff` / `BX_TP` 的 Log 级日志测试稳定后降为 Verbose 或删除，保留 Warning 诊断。
- **Section 跳转事件同步**：权威端的 ForceJumpSection / 连段跳转目前随快照投影同步，可考虑推送显式跳转事件减少重建偏差。
- **根运动同步收口**：RootMotion 任务在预测/回滚/加速场景下的位置对账（PredictFailure 回滚时位移是否需要修正）待验证。

#### 9.2.3 调试与可视化增强

**现状**：`UBXTLManager` 有 `bShowCollision` 调试开关，`UBXFunctionLibrary` 有编辑器调试绘制。编辑器预览时已实现 **Debug 节点高亮**：正在执行的 Task 对应的 Graph 节点和时间轴节点叠加黄色呼吸高亮框（见 [5.2 Debug 高亮系统](#52-时间轴编辑器timelineeditor)）。

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
| 模块入口 | [BattleX.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/BattleX.h) / [BattleX.cpp](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/BattleX.cpp) |
| 基础设施 | [BXManager.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/BXManager.h) / [BXSubSystem.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/BXSubSystem.h) / [BXSettings.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/BXSettings.h) |
| Tag 词汇表 | [BXGameplayTags.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/BXGameplayTags.h) |
| 工具函数 | [BXFunctionLibrary.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/BXFunctionLibrary.h) |
| 时间轴核心 | [BXTLAsset.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Timeline/BXTLAsset.h) / [BXTLManager.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Timeline/BXTLManager.h) / [BXTLComponent.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Timeline/BXTLComponent.h) / [BXTLStructs.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Timeline/BXTLStructs.h) |
| 技能系统 | [BXSkillAsset.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Skill/BXSkillAsset.h) / [BXSkillManager.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Skill/BXSkillManager.h) / [BXSkillComponent.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Skill/BXSkillComponent.h) / [BXSkillStructs.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Skill/BXSkillStructs.h) / [BXSkillEnums.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Skill/BXSkillEnums.h) |
| 网络同步 | [BXNetEnums.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Net/BXNetEnums.h) / [BXNetStructs.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Net/BXNetStructs.h) / [BXSkillReplicated.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Net/BXSkillReplicated.h) / [BXTLReplicated.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Timeline/BXTLReplicated.h) / [BXBuffReplicated.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Buff/BXBuffReplicated.h) |
| 任务系统 | [BXTask.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Task/BXTask.h) / [BXTProcessor.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Task/BXTProcessor.h) |
| 具体任务 | [BXTAnimation.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Task/BXTAnimation.h) / [BXTCollision.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Task/BXTCollision.h) / [BXTFlowControl.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Task/BXTFlowControl.h) |
| 状态系统 | [BXBehaviorComponent.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/State/BXBehaviorComponent.h) / [BXBehaviorAgent.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/State/BehaviorAgent/BXBehaviorAgent.h) / [BXStateMachine.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/State/StateMachine/BXStateMachine.h) |
| 决策树 | [BXDecisionTreeTemplate.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/DecisionTree/BXDecisionTreeTemplate.h) / [BXDecisionTreeActuator.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/DecisionTree/BXDecisionTreeActuator.h) |
| 装备系统 | [BXGear.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Gear/BXGear.h) / [BXGearComponent.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Gear/BXGearComponent.h) / [BXMeleeWeapon.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Gear/BXMeleeWeapon.h) |
| 移动系统 | [BXCharacterMovementComponent.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Movement/BXCharacterMovementComponent.h) / [BXRootMotionSource.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Movement/BXRootMotionSource.h) |
| 碰撞系统 | [BXCollision.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Collision/BXCollision.h) / [BXShapeComponent.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Collision/BXShapeComponent.h) / [BXHitReactionComponent.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Collision/BXHitReactionComponent.h) |
| 事件系统 | [BXEventManager.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Event/BXEventManager.h) / [BXEventStructs.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Event/BXEventStructs.h) |
| 条件系统 | [BXCondition.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Condition/BXCondition.h) / [BXConditionEnums.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Condition/BXConditionEnums.h) / [BXConditionManager.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Condition/BXConditionManager.h) / [BXTaskCondition.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Task/BXTaskCondition.h) / [BXDecisionTreeCondition.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/DecisionTree/BXDecisionTreeCondition.h) |
| BUFF 系统 | [BXBuffAsset.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Buff/BXBuffAsset.h) / [BXBuffManager.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Buff/BXBuffManager.h) / [BXBuffComponent.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Buff/BXBuffComponent.h) / [BXBuffStructs.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Buff/BXBuffStructs.h) / [BXBuffEnums.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Buff/BXBuffEnums.h) / [BXBuffFunctionLibrary.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleX/Buff/BXBuffFunctionLibrary.h) |
| 编辑器入口 | [BattleXEditor.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleXEditor/BattleXEditor.h) |
| 时间轴编辑器 | [BXTLEditor.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleXEditor/TimelineEditor/BXTLEditor.h) / [BXTLEditorDelegates.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleXEditor/TimelineEditor/BXTLEditorDelegates.h) / [BXTLPreviewProxy.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleXEditor/TimelineEditor/Preview/BXTLPreviewProxy.h) |
| 决策树编辑器 | [BXDTEditor.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleXEditor/DecisionTreeEditor/BXDTEditor.h) |
| 时间轴控件 | [STimeline.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleXEditor/TimelineBase/STimeline.h) / [TimelineController.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleXEditor/TimelineBase/TimelineController.h) |
| Graph 节点 | [SBXTLGraphNode.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleXEditor/TimelineEditor/Graph/SBXTLGraphNode.h) / [SBXBuffGraphNode.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleXEditor/TimelineEditor/Graph/SBXBuffGraphNode.h) / [BXTLGraphNode.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleXEditor/TimelineEditor/Graph/BXTLGraphNode.h) |
| 时间轴 Task 节点 | [SBXTLTaskTrackNode.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleXEditor/TimelineEditor/Timeline/SBXTLTaskTrackNode.h) / [SBXTLTaskTrack.h](file:///f:/BXTest/Plugins/BattleX/Source/BattleXEditor/TimelineEditor/Timeline/SBXTLTaskTrack.h) |
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
| `EBXSyncInitiator` | BXNetEnums.h | Client / Server（同步发起端，编码在同步 ID 高 3 位） |
| `EBXPredictState` | BXSkillEnums.h | None / Predicting / Confirmed / RollingBack（技能预测状态） |
| `EBXSkillLockType` | BXSkillEnums.h | None / Target / Location / Direction（技能锁定类型） |

---

*本文档基于源码静态分析生成，涵盖 BattleX 插件运行时模块（BattleX）与编辑器模块（BattleXEditor）的完整架构、关键类与函数说明、依赖关系、运行方式与优化建议。*
