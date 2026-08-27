# BattleX 状态/行为系统架构设计

> 版本：v4.0（双系统分立：行为组件统筹基层组件 + 状态组件驱动可视化自动状态机；状态单向禁用行为；状态机资产化复用决策树编辑器）
> 范围：`Source/BattleX/State/**` 重写（BehaviorComponent / StateComponent 双组件）+ 决策树派生状态机资产 + 技能/Task/出招表集成 + 网络同步
> 复用既有：UBXDecisionTree*（图框架）、BXBAMove/Rotate/Jump/Landed（行为Agent，包装 CharacterMovement）、技能系统同步/预测模型（SyncID/防重/超时/服务器世界时间）
> 关联模块：UBXSkillComponent / UBXTLManager / UBXTask / UBXEventManager / UBXGameplayTags

---

## 1. 概念模型：双系统 + 单向依赖

### 1.1 两个系统

| 系统 | 组件 | 回答的问题 | 生命周期 |
|------|------|-----------|---------|
| **行为系统** | UBXBehaviorComponent | "角色正在**做**什么"（移动/旋转/攻击…） | 驱动方控制（技能/输入/AI 启停），**无时长概念** |
| **状态系统** | UBXStateComponent | "角色处于**什么处境**"（硬直/浮空/灼烧…） | 时长驱动（≤0 无限）+ 状态机自动转移 |

**行为 = 中间层统筹者**：每个行为挂一个 Agent（`UBXBehaviorAgent`），包装基层组件（CharacterMovement / 输入 / 动画接口）——行为被启动时开启基层能力，被停止/挂起时关闭。现有 `BXBAMove/BXBARotate/BXBAJump/BXBALanded` 即此类，全部保留复用。

**状态 = 处境标记 + 自动状态机**：状态条目有时长、有进出场表现、可禁用行为；同族状态由可视化状态机资产（决策树派生）编排自动跳转。

### 1.2 单向依赖（核心规则）

```
┌──────────────────────────────────────────────────────────────┐
│ UBXBehaviorComponent（行为系统）                                │
│   ActiveBehaviors: Tag → {Agent, Sources(Sign)}               │
│   行为关系矩阵（并行/挤出/拒绝）          ← 行为间自动权衡        │
│   Agent 统筹基层组件                    ← 中间层定位            │
└───────────────▲──────────────────────────────────────────────┘
                │ 唯一交互：禁用（单向，立即中断 + 挂起恢复）
┌───────────────┴──────────────────────────────────────────────┐
│ UBXStateComponent（状态系统）                                   │
│   ActiveStates: Tag → {Sources(Sign), Duration}               │
│   StateMachines: 状态机资产实例（族内自动跳转 + 跳转表现）         │
│   StateConfigs: 裸状态配置（非族状态：Buff/标记类）               │
└──────────────────────────────────────────────────────────────┘
        ▲ StartBehavior/StopBehavior、EnterState/ExitState
┌───────┴──────────────────────────────────────────────────────┐
│ 驱动层：技能（EnterBehaviors/EnterStates/CancelWindows）、       │
│        BUFF（预留）、输入/AI/出招表（直接调用）                    │
└──────────────────────────────────────────────────────────────┘
```

- **状态 → 行为：单向禁用**。状态存在期间其 `ForbiddenBehaviors` 被禁用：未启动的拒绝启动，运行中的**立即中断**（挂起，状态退出后自动恢复）。
- **行为 → 状态：零影响**。行为被挤出/停止不产生任何状态变化。
- **进出行为/进出状态的决策权在驱动层**（技能/BUFF 配置决定），系统间不互相触发——杜绝交叉配置表。

### 1.3 v3.x → v4.0 决策演进

| 变更 | 理由 |
|------|------|
| 统一状态模型（v3.x）→ 双系统分立 | 行为与状态生命周期模型不同（驱动方控制 vs 时长驱动）；"技能姿态=行为"（出招表确认）；行为间关系与状态间互斥的配置面不同（矩阵 vs 状态机图） |
| 状态互斥矩阵删除 | **族内互斥由状态机图结构表达**（SM 单当前节点=族内单活天然语义）；跨族并存天然；无跨族跳转需求（浮空硬直/地面硬直同属硬直状态机内部状态） |
| AutoTransitions/ExitTransition 配置上收（v3.2）→ 状态机资产化 | 用户原始意图即自动状态机；可视化编辑（决策树框架现成）；表达力补齐条件分支与物理驱动转移 |
| 子状态所有权传播/回收机制删除 | 行为/状态分立后状态间无父子关系；技能 EnterStates 退出由 CleanSkillTrash 按 Sign 逐条收束 |
| 出招表章节合入 | 攻击姿态=行为（确认），取消窗口保护行为，互锁监听行为 Exit 事件 |

## 2. 设计目标与映射

| # | 需求 | 机制 | 主责 |
|---|------|------|------|
| 1 | 行为并行、禁用、中断 | 并行=表内多条目；禁用=矩阵拒绝 + 状态禁用；中断=矩阵挤出 | Behavior |
| 2 | 状态并存、互斥 | 并存=跨族天然；族内互斥=状态机图结构（单当前节点） | State |
| 3 | 技能/BUFF 决定进出行为 | SkillAsset.EnterBehaviors（Sign=SkillID），结束收束退出 | 驱动层 |
| 4 | 状态结束触发表现（含行为） | 状态机转移 → 边 TransitionPresentation；裸状态到期 → StateConfig.ExitPresentation（可配技能/时间轴/动画；技能本身可为行为） | State |
| 5 | 状态时长（≤0 无限） | 节点 Duration / 外部携带，来源独立计时 | State |
| 6 | 技能状态集 | SkillAsset.EnterStates（Tag→时长，Sign=SkillID） | 驱动层 |
| 7 | 技能时间段解除互斥（取消窗口） | CancelWindows：默认保护技能行为不被矩阵挤出，窗口内解除 → 互斥行为挤出 → 行为 Exit(Sign=SkillID) → 技能互锁中断 | 双方 |
| 8 | 状态自动切换 | 状态机资产：节点+条件边，服务器评估 | State |
| 9 | 状态跳转表现 | 转移边 TransitionPresentation 唯一入口（状态节点不配表现——节点进出场与边过渡冗余；技能/时间轴/纯动画三通道，详情面板按 Type 显隐对应资产） | State |
| 10 | 状态Task | BXT_EnterState（时长+可回退）/ BXT_ExitState（可控表现） | 驱动层 |
| 11 | 状态、行为同步 + 预测回滚 | 双组件各自：COND_InitialOnly 快照 + Enter/Exit 多播 + 统一预测 + 拒绝/超时回滚 | 双方 |
| 12 | 出招表 | UBXComboComponent：输入缓冲 + 双触发查询 + 招式图数据资产 | Combo |

## 3. 总体架构与数据流

```
┌────────────────────────────────────────────────────────────────────┐
│ 驱动层                                                             │
│  技能：EnterBehaviors(保护) + EnterStates + CancelWindows + 互锁监听 │
│  Task：BXT_EnterState / BXT_ExitState（Sign=TL运行时ID）             │
│  出招表：UBXComboComponent（输入缓冲→匹配边→PlaySkill）              │
│  输入/AI：StartBehavior/StopBehavior（Sign=ClientSyncID/0）          │
│  BUFF：预留（AddBuff/RemoveBuff 收束，Sign=BuffID）                  │
└──────────────┬───────────────────────────┬────────────────────────┘
               ▼                           ▼
┌──────────────────────────┐  ┌────────────────────────────────────┐
│ UBXBehaviorComponent      │  │ UBXStateComponent                   │
│ ───────────────────────── │  │ ──────────────────────────────────  │
│ ActiveBehaviors(事实)     │  │ ActiveStates(事实)                  │
│ 行为矩阵(全局ini)          │  │ StateMachines(资产实例,族内转移)     │
│ ProtectionEntries(取消窗) │◄─│ StateConfigs(裸状态配置)             │
│ SuspendedBehaviors(挂起区)│  │ 禁用门控:ForbiddenBehaviors→挂起      │
│ BehaviorAgents(预建)      │  │ Tick:服务器转移评估+到期快照          │
│ 复制+预测(§4.6)           │  │ 复制+预测(§5.7)                     │
└──────────┬───────────────┘  └───────────────┬────────────────────┘
           │ Start/Stop(指令)                  │ Enter/Exit/Presentation(通知)
           ▼                                  ▼
   UBXBehaviorAgent(执行体)           UBXStateMachineAsset(数据,决策树派生)
   BXBAMove 包装CMC…                  SM_Stun:节点图+条件边
                                      (服务器评估,客户端跟随)
```

**调用方向**：驱动层 → 两组件公开 API；BehaviorComponent → Agent（Start/Stop）；StateComponent → 状态机资产（转移评估，纯数据）；StateComponent → BehaviorComponent（禁用/恢复，唯一跨系统调用）。Agent/状态机资产不反向持有事实，全部现查。

## 4. 行为系统：UBXBehaviorComponent

### 4.1 职责

| 职责 | 说明 |
|------|------|
| 唯一行为事实表 | ActiveBehaviors：Tag → Sources(Sign) + Agent |
| 进入裁决 | 矩阵拒绝、状态禁用拒绝、Agent CheckStart、保护检查 + 挤出 |
| 停止裁决 | 驱动方 Stop（Sign 匹配）、矩阵挤出、状态挂起、回滚 |
| Agent 管理 | BeginPlay 预建（现有 BXBAMove 等机制不变） |
| 取消窗口保护 | ProtectionEntries（技能驱动，§6.1） |
| 挂起/恢复 | 状态禁用的中断与自动恢复（§4.5） |
| 复制与预测 | §4.6 |

### 4.2 数据结构

```cpp
// 行为结束原因
UENUM(BlueprintType)
enum class EBXBehaviorEndReason : uint8
{
	BER_Manual = 0,        // 驱动方显式停止
	BER_Expelled,          // 被行为矩阵挤出（被中断/取代）
	BER_Suspended,         // 被状态禁用挂起（可恢复）
	BER_PredictRollback,   // 客户端预测回滚
	BER_Cleared,           // 组件清空
	BER_TMax UMETA(Hidden)
};

// 行为来源（无时长：生命周期归驱动方）
USTRUCT(BlueprintType)
struct FBXBehaviorSource
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(Transient, BlueprintReadOnly) int64 Sign = 0;  // SkillID/TLID/ClientSyncID/0(系统常驻)
};

USTRUCT(BlueprintType)
struct FBXBehaviorRuntimeData
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(Transient, BlueprintReadOnly) FGameplayTag Tag;
	UPROPERTY(Transient, BlueprintReadOnly) TArray<FBXBehaviorSource> Sources;  // 空=不存在
};

// UBXBehaviorComponent（重写）：
TMap<FGameplayTag, FBXBehaviorRuntimeData> ActiveBehaviors;          // 唯一事实
TMap<FGameplayTag, TArray<FBXProtectionRecord>> ProtectionEntries;   // 取消窗口保护
TMap<FGameplayTag, FBXSuspendedBehavior> SuspendedBehaviors;         // 挂起区(§4.5)
TMap<FGameplayTag, UBXBehaviorAgent*> BehaviorAgents;                // BeginPlay预建复用
```

### 4.3 行为关系矩阵（全局，可视化配置）

```cpp
// 全局行为关系（Config=BattleX，各端一致的 ini）
UCLASS(Config=BattleX, DefaultConfig, meta=(DisplayName="BattleX Behavior Relations"))
class UBXBehaviorSettings : public UDeveloperSettings
{
	UPROPERTY(EditAnywhere, Config, Category="Matrix")
	TArray<FGameplayTag> RelationTags;   // 矩阵轴（支持族Tag如 BXBehavior.Locomotion 整族参与）
	UPROPERTY(EditAnywhere, Config, Category="Matrix")
	TMap<FGameplayTag, FGameplayTagContainer> ExpelRelations;   // 挤出：行进入时踢掉列（中断/取代）
	UPROPERTY(EditAnywhere, Config, Category="Matrix")
	TMap<FGameplayTag, FGameplayTagContainer> RejectRelations;  // 拒绝：列存在时挡住行（禁用/互斥）
};
```

| 单元格 | 语义 | 行为 |
|--------|------|------|
| 并存（默认空） | 互不干扰 | 双活（移动+瞄准并行） |
| 挤出 | 行踢掉列 | 进入时 InternalStop(列, BER_Expelled)，Agent 正常 Stop（需求1中断） |
| 拒绝 | 列挡住行 | 进入失败（需求1禁用） |

- 行=想要进入，列=已存在；非对称是特性：(A,B)/(B,A) 独立配置；
- 层级匹配：轴注册族 Tag（`BXBehavior.Locomotion`）一条覆盖整族；
- **即时求值不物化**：关系激活时现查，零登记簿记；
- 编辑器：BattleXEditor DetailCustomization 渲染为矩阵网格（单元格循环 并存→挤出→拒绝，对角线禁用=重复启动语义，图例如碰撞通道矩阵）。

### 4.4 激活与停止管线

```
InternalStartBehavior(Tag, Sign, Param):
  0. 链深守卫（>8 阻断 + Warning）
  1. 挂起检查：SuspendedBehaviors 含 Tag（仍被状态禁用）→ false
  2. 矩阵拒绝：RejectRelations[Tag] 命中任一活跃行为 → false
  3. Agent->CheckStart(Param) 失败 → false
  4. 保护检查 + 挤出：活跃行为命中 ExpelRelations[Tag] 且存在
     bProtected=true 记录 → 激活拒绝（霸体/取消窗口外）
     否则逐个 InternalStop(该条目, BER_Expelled)
  5. 表更新（新 Sign 追加 / 同 Sign 刷新）；Agent->Start(Param)
  6. 广播 BXEvent.Behavior.Enter {Tag, Sign}
  7. 服务器：MulticastBehaviorEnter；连接数变化时投影快照

InternalStopBehavior(Tag, Sign, Reason, Param):
  1. 查无该 Sign → false；移除来源；仍有其他来源 → true（先停先退）
  2. Agent->Stop(Param)；移除条目
  3. 广播 BXEvent.Behavior.Exit {Tag, Sign, Reason}   ← 技能互锁监听点
  4. 服务器：MulticastBehaviorExit
```

```cpp
// 对外 API：
bool StartBehavior(const FGameplayTag& Tag, int64 Sign = 0, const FInstancedStruct& Param = {});
bool StopBehavior(const FGameplayTag& Tag, int64 Sign = 0, const FInstancedStruct& Param = {});
bool StopBehaviorAllSources(const FGameplayTag& Tag, ...);            // 全来源退出
// 重复 Start = Agent 重启 + 来源刷新；Agent 主动结束（动画完/位移到位）→ StopBehavior(Tag, AgentKey)
```

**所有停止路径（手动/挤出/挂起/回滚/清空）都走 InternalStopBehavior**——Agent Stop、事件一个不漏（机制性根治 Stop 链路失效类问题）。

### 4.5 挂起/恢复（状态禁用通道，规则落地）

状态禁用 = 立即中断，但**不销毁驱动方来源、不移事实表条目**——状态退出后自动恢复。

实现为**遮蔽表（SuspendMasks）**方案：挂起 = 查询层遮蔽，而非条目搬运：

```cpp
// 挂起遮蔽（key=状态禁用Tag，可为族Tag如 BXBehavior.Locomotion）
struct FBXSuspendMask
{
	TSet<FGameplayTag> ByStates;   // 禁用它的状态集合（最后一个退出才解除遮蔽）
};
TMap<FGameplayTag, FBXSuspendMask> SuspendMasks;
```

```
状态进入（携带 ForbiddenBehaviors，§5.5 门控）→ SuspendByForbiddenTag(禁用Tag, 状态Tag):
  遮蔽键已存在 → 仅 ByStates 追加状态（遮蔽已生效，无重复处理）
  新遮蔽生效:
    匹配该 Tag 且未被其他遮蔽键覆盖的活跃行为：
      Agent->Stop（条目留在 ActiveBehaviors，事实保留）
      每来源广播 BXEvent.Behavior.Exit {Reason=BER_Suspended}   ← 技能互锁同样触发（受击中断攻击）
    登记遮蔽键

状态退出 → ResumeByForbiddenTag(禁用Tag, 状态Tag):
  **重入保护先行**：状态退出后至解除登记期间被重新进入（延迟解除窗口内
  Exit/Enter 事件监听者同步 EnterState 重入旧状态；或 Exit 表现触发的技能
  EnterStates 重入自身）→ 跳过解除（登记仍有效，活跃状态的禁用不被误清）
  从遮蔽键 ByStates 移除该状态；集合非空 → 遮蔽保持（多状态叠加禁用最后一个退出才解除）
  遮蔽解除:
    匹配该 Tag 且不再被任何活跃遮蔽键覆盖的活跃行为：
      Agent->Start（来源 Sign 原样，所有权不变）
      广播 BXEvent.Behavior.Enter

查询遮蔽 IsBehaviorSuspended:
  任一活跃遮蔽键为该 Tag 的祖先或自身 → 挂起（族 Tag 遮蔽整族）
  CanStartBehavior 的挂起检查走此判定 → 挂起期间 StartBehavior 该 Tag → 拒绝
```

- 条目不移表 → ActiveBehaviors 始终是"活跃行为全集"，挂起态由查询遮蔽表达（CheckActiveBehavior 不受影响，GetActiveBehaviors 含挂起行为）；
- 挂起/恢复不改变来源 Sign——技能驱动的行为（冲锋）恢复后仍归技能管；
- 恢复顺序与挂起顺序无关（Tag 独立判定）；
- 遮蔽键与解除按（禁用Tag, 状态Tag）配对登记，天然支持族 Tag 禁用与多状态叠加禁用无竞态。

### 4.6 复制与预测（Q2/Q3 决议，同技能系统模型）

**同步通道（COND_InitialOnly + 显式RPC，同 RunningSkillStates 形态）**：

- `RunningBehaviorStates`：TArray + COND_InitialOnly——仅新客户端连入初始重建（PreReplication 连接数检测投影，LastProjectedConnectionCount 模式），已有连接零属性流量；
- 已有连接动态：`MulticastBehaviorEnter(Tag, Sign)` / `MulticastBehaviorExit(Tag, Sign, Reason)`（Reliable，按值传参）；挂起/恢复复用 Exit/Enter 多播（Reason 区分）；
- Late Join：OnRep(带旧值) 差分重建（Agent Start + 通知，跳过矩阵）。

**统一预测（AutonomousProxy）**：

```
本地启动:
  1. 本地走完整 InternalStartBehavior（矩阵/挂起用本地判定——全局ini+多播到达的状态,安全域内）
  2. PredictedBehaviors.Add({Tag, Sign(ClientSyncID), 时间})
  3. ServerEnterBehaviors RPC（仅直接调用类；技能链路 Sign=SkillID 随技能预测携带，无独立RPC）

服务器 ServerEnterBehaviors:
  校验 Initiator=Client + Sign 未存在（防重防伪造）+ 请求年龄<=StateRequestMaxAgeMs（服务器世界时间域）
  → 权威裁决 → InternalStartBehavior + Multicast 确认 / ClientRejectBehaviors 拒绝回包
```

**回滚——确认与回滚触发严格分离**：

| 事件 | 动作 |
|------|------|
| MulticastBehaviorEnter 匹配 (Tag,Sign) | **确认**：移出预测缓冲（幂等：已存在仅视为确认） |
| ClientRejectBehaviors | **回滚**：InternalStop(Tag, Sign, BER_PredictRollback) |
| 预测超时（BehaviorPredictMaxDuration=0.3s，UBXSettings） | **回滚**（Tick 快照收集） |
| 驱动方先行退出 | 移除预测条目（回滚即完成） |

- 本地退出预测：`StopBehavior` 本地执行 + `ServerExitBehaviors` RPC 上报（服务器权威退出 + 多播）；
- SimulatedProxy 纯多播跟随；Agent 各端本地运行（表现层）；Listen Server/Standalone 单事实源；
- **Move 行为实现注记**：位移事实已由 CMC 复制（移动模式/SavedMove），Agent 只同步行为开关语义（启停/挂起状态），禁止与 CMC 重复同步位移数据；
- 流量注记：若高频行为场景实测超标，可切常规复制（RunningBuffStates 形态）——仅换传输通道。

## 5. 状态系统：UBXStateComponent

### 5.1 职责

| 职责 | 说明 |
|------|------|
| 唯一状态事实表 | ActiveStates：Tag → Sources(Sign+时长) |
| 状态机实例管理 | 按资产创建实例，服务器评估转移 |
| 时长与到期 | 来源独立计时、先到先退；到期触发转移评估（族内）/自然退出（裸状态） |
| 禁用行为门控 | 进入/退出状态时向 BehaviorComponent 挂起/恢复（唯一跨系统调用） |
| 表现触发 | 族内：转移边 TransitionPresentation；裸状态：StateConfig 进出场表现——共用三通道 |
| 复制与预测 | §5.7 |

### 5.2 数据结构

```cpp
UENUM(BlueprintType)
enum class EBXStateEndReason : uint8
{
	SER_Expired = 0,      // 时长耗尽（裸状态自然退出 / 族内触发转移评估）
	SER_Manual,           // 显式退出（API/Task）
	SER_Transition,       // 状态机转移离开
	SER_External,         // 被外部进入顶掉（族内单活：受击直接进 Knockdown 顶掉 Knockback）
	SER_PredictRollback,  // 预测回滚（不触发表现）
	SER_Cleared,
	SER_TMax UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FBXStateSource
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(Transient, BlueprintReadOnly) int64 Sign = 0;
	UPROPERTY(Transient, BlueprintReadOnly) float Duration = -1.0f;   // ≤0 无限
	UPROPERTY(Transient, BlueprintReadOnly) float ElapsedTime = 0.0f;
};

USTRUCT(BlueprintType)
struct FBXStateRuntimeData
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(Transient, BlueprintReadOnly) FGameplayTag Tag;
	UPROPERTY(Transient, BlueprintReadOnly) TArray<FBXStateSource> Sources;
};

// 表现（转移边/裸状态进出场共用，三通道；详情面板按 Type 显隐对应资产——EditConditionHides）
USTRUCT(BlueprintType)
struct FBXStatePresentation
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) EBXPresentationType Type = PT_None;  // PT_None/PT_Skill/PT_Timeline/PT_Animation
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) TSoftObjectPtr<UBXSkillAsset> SkillAsset;     // 自带同步体系（Type==PT_Skill 显示）
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) TSoftObjectPtr<UBXTLAsset> TimelineAsset;     // 不同步的技能(纯表现)（Type==PT_Timeline 显示）
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) TSoftObjectPtr<UAnimMontage> MontageAsset;    // 纯动画（Type==PT_Animation 显示）
};

// 裸状态配置（非族状态：Buff/标记类——无状态机，Detail 面板配置）
USTRUCT(BlueprintType)
struct FBXStateConfig
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) float Duration = -1.0f;                // 默认时长（外部携带优先）
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) FGameplayTagContainer ForbiddenBehaviors;  // 存续期禁用（Tag层级：禁 BXBehavior 根=全禁）
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) FBXStatePresentation EntryPresentation;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) FBXStatePresentation ExitPresentation;  // SER_PredictRollback 强制不触发
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) bool bPresentationOnServerOnly = false;
};

// UBXStateComponent（重写）：
TMap<FGameplayTag, FBXStateRuntimeData> ActiveStates;          // 唯一事实
TMap<FGameplayTag, FBXStateConfig> StateConfigs;               // 裸状态配置
TArray<TObjectPtr<UBXStateMachineInstance>> StateMachineInstances;  // 资产实例
TArray<FBXPredictedEntry> PredictedStates;                     // 预测缓冲
UPROPERTY(ReplicatedUsing=OnRep_RunningStateStates)
TArray<FBXStateSyncEntry> RunningStateStates;                  // COND_InitialOnly
```

### 5.3 状态机资产（决策树派生，可视化编辑）

复用决策树框架（`UBXDecisionTreeTemplate` + Node/Edge/Condition/Actuator + EdGraph 编辑器管线）：

```cpp
// 状态机资产（bAllowCycle=true：硬直链回环）
UCLASS(BlueprintType)
class UBXStateMachineAsset : public UBXDecisionTreeTemplate
{
	GENERATED_BODY()
public:
	// 状态Tag → 节点 运行时索引（转移评估用）
	TMap<FGameplayTag, int32> StateNodeIndex;
};

// 状态节点（表现统一由转移边承载——节点进出场与边过渡冗余，已删除）
UCLASS(BlueprintType)
class UBXSMStateNode : public UBXDecisionTreeNode
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) FGameplayTag StateTag;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) float Duration = -1.0f;                    // 驻留时长(≤0无限)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) FGameplayTagContainer ForbiddenBehaviors;  // 存续期禁用
};

// 转移边（条件=决策树 Instanced Condition：属性阈值/事件/到期）
UCLASS(BlueprintType)
class UBXSMTransitionEdge : public UBXDecisionTreeEdge
{
	GENERATED_BODY()
public:
	// 转移表现（状态间过渡表现的唯一配置入口；未配置则无表现）
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) FBXStatePresentation TransitionPresentation;
	// 触发时机：TE_OnExpired(时长耗尽评估) / TE_OnTick(每帧评估,物理驱动) / TE_OnEvent(事件驱动,预留)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) EBXTransitionEvaluate EvaluateType = TE_OnExpired;
};
```

**转移评估（服务器，StateComponent::Tick）**：

```
对每个状态机实例（有当前节点且节点状态条目活跃）:
  1. 到期驱动：条目时长耗尽 → 评估 TE_OnExpired 边（按 OutEdges 顺序）
  2. 帧驱动：评估 TE_OnTick 边（浮空连招 SM 看速度/高度请求转移——CheckCondition
     复用决策树条件机制，参数为 FBXSMTransitionContext{Owner, FromTag, ServerTime}）
  3. 首个 CheckCondition 通过的边命中 → 执行转移（自环除外）：
     ExecuteTransition = ExitStateAllSourcesInternal(当前, SER_Transition, 抑制表现, **延迟禁用解除**)
     → InternalEnterState(目标, 节点时长, Sign=0)（族内进入无内置表现，不涉及抑制）
     → 解除旧状态禁用登记 → 各自多播 → 转移边 TransitionPresentation 触发（未配置则无表现）
     *延迟禁用解除*：退出时不解除禁用遮蔽，待新状态登记遮蔽后再解除旧登记——
     新旧共享禁用 Tag 经遮蔽 ByStates 多重登记保持挂起（无 Resume→Suspend 抖动），
     独占 Tag 解除恢复；进入失败同样解除（禁用不残留）
  3a. **自环边**（目标 StateTag == 当前 StateTag）= 驻留刷新：重置全部有限来源计时
     （无限来源不动），不触发表现/事件——守卫语义：条件成立期间保持驻留；
     旧实现跳过自环令到期来源永不退出（状态卡死），已修复
  4. 到期且无 OnExpired 边命中 → 族内状态自然退出（条目移除，SM 空转）
     ——"无硬直"用"无条目"表达（BXStunState.Normal 废弃）
  5. 空转回退：曾激活（bActivatedOnce）且无当前节点 → 自动进入默认节点
     （bIsDefaultNode 标记，全图至多一个；经 EnterState 走链深度守卫，失败下帧重试；
     默认态条目仍活跃时直接归位）
```

- **服务器评估、多播结果**：SimulatedProxy/客户端不评估边（无同步分叉）；转移触发的表现随 Enter/Exit 多播在各端本地执行；
- SM 资产是纯数据（节点+边），运行时实例（UBXStateMachineInstance）只存 `{Asset, CurrentNodeIndex}`——不持有状态事实。

### 5.4 外部进入与状态机整合（规则一/二）

**规则一：外部进入 = 外部转移**。技能 EnterStates / Task / 表现技能进入族内状态时：

```
InternalEnterState(族内Tag, 外部时长, Sign):
  1. 定位拥有该 Tag 的 SM 实例
  2. SM 有当前节点且 ≠ 目标 → InternalExitState(当前, SER_External)（族内单活，顶掉）
  3. SM.CurrentNodeIndex = 目标节点；状态条目建立
  4. **不评估出边**——进入由外部决策，离开只走边评估（受击链：外部进、边出）
  5. 表现：无内置表现（表现唯一入口是转移边，外部进入不走边；需要表现由驱动方技能自带）
```

**规则二：外部携带时长优先**。外部进入带时长（技能 EnterStates 配置的受击时长）用携带值；未带（-1）用节点默认。同 AutoTransition 语义，规则统一。

进入裸状态（无 SM 归属）：直接建条目（StateConfigs 配置生效，外部时长同样优先）。

### 5.5 禁用行为门控（规则三）

- 配置面：SM 节点 `ForbiddenBehaviors` 与裸状态 `StateConfig.ForbiddenBehaviors`；
- **Tag 层级缓解配置量**：禁 `BXBehavior` 根 = 全行为禁（处决/变形）；禁 `BXBehavior.Locomotion` 族 = 禁移动族（保留攻击）；禁具体 Tag = 单禁；
- 状态进入管线末步：对命中 Tag 的活跃行为执行挂起（§4.5）；状态条目彻底移除时执行恢复判定；
- 多来源状态：任一来源存续即禁用持续（先到先退语义顺延）。

### 5.6 状态进入/退出管线

```
InternalEnterState(Tag, Duration, Sign, bExternal):
  0. 链深守卫
  1. 族内定位（规则一：外部转移顶掉 SER_External）/ 裸状态直入
  2. 时长解析（规则二：携带优先，-1 用节点/配置默认）
  3. 表更新（同 Sign 刷新 / 新 Sign 追加）
  4. 禁用门控：ForbiddenBehaviors → BehaviorComponent 挂起
  5. 裸状态：StateConfig.EntryPresentation 触发（族内无内置表现，由转移边在 ExecuteTransition 统一触发；各端本地，PredictRollback 路径除外）
  6. 广播 BXEvent.State.Enter {Tag, Sign, Duration}
  7. 服务器：MulticastStateEnter；连接数变化投影快照

InternalExitState(Tag, Sign, Reason):
  1. 查无该 Sign → false；移除来源；仍有其他来源 → true（先到先退）
  2. 移除条目；裸状态：StateConfig.ExitPresentation 触发（SER_PredictRollback 除外；族内无内置退场表现）
  3. 禁用解除判定 → BehaviorComponent 恢复（§4.5）
     （bDeferForbiddenRelease=true 时延迟：转移路径由 ExecuteTransition 在新状态登记后统一解除）
  4. 广播 BXEvent.State.Exit {Tag, Sign, Reason}
  5. 服务器：MulticastStateExit
  （族内退出后 SM CurrentNode 置空——下次外部进入/转移再激活）
```

**剩余时间语义**：`GetStateRemainingSeconds` 多来源取最小；含无限来源返回浮点最大值（数值比较安全，任何"剩余<X"判定对无限态恒假），无条目返回 -1。

### 5.7 复制与预测

同 §4.6 模型（组件独立一套）：

- `RunningStateStates` COND_InitialOnly + `MulticastStateEnter(Tag, Sign, Duration, EnterServerTime)` / `MulticastStateExit(Tag, Sign, Reason)`；
- 预测：`PredictedStates` + `ServerEnterStates`/`ServerExitStates`/`ClientRejectStates`；技能链路（EnterStates / 预测技能时间轴内 BXT_EnterState）Sign=SkillID 随技能预测携带，服务器时间轴执行到同一 Task 处进入，经多播确认（确认延迟 ≈1.5×RTT，超时预算覆盖）；
- SM 转移不预测（服务器权威，多播到达跟随）；
- Late Join OnRep 差分重建（条目 + Agent 无关 + SM CurrentNode 由条目 Tag 反查资产索引恢复）。

## 6. 驱动层集成

### 6.1 技能（需求3/6/7）

```cpp
// UBXSkillAsset 新增
UPROPERTY(EditDefaultsOnly, Category="Behavior")
TMap<FGameplayTag, bool> EnterBehaviors;      // 技能行为集（Tag→bProtected，默认全保护）
UPROPERTY(EditDefaultsOnly, Category="State")
TMap<FGameplayTag, float> EnterStates;        // 技能状态集（Tag→时长，-1用默认）
UPROPERTY(EditDefaultsOnly, Category="Behavior")
TArray<FBXSkillCancelWindow> CancelWindows;   // 取消窗口 {TimeWindow}
```

- **开始**（InternalPlaySkill 成功，含预测路径）：EnterBehaviors 逐条 `StartBehavior(Tag, SkillID)`（保护默认开）；EnterStates 逐条 `EnterState(Tag, 时长, SkillID)`；
- **结束**（CleanSkillTrash 收束，覆盖自然/中断/回滚全部 FinishReason）：`StopBehavior(Tag, SkillID)` + `ExitState(Tag, SkillID)` 逐条退出——回滚路径必须退出，否则客户端残留；
- **取消窗口**：技能开始 EnterBehaviors Keys 各 Tag `SetBehaviorProtection(Tag, SkillID, true)`；SkillManager Tick 窗口边界切换 true/false；结束移除该 SkillID 全部保护记录；
- **技能互锁**：技能组件监听 `BXEvent.Behavior.Exit`（Sign=本技能 SkillID 且 Reason∈{BER_Expelled, BER_Suspended}）→ StopSkill(FinishReason=Interrupt)——技能行为被矩阵挤出或被状态禁用挂起，技能即中断；
- **霸体**对硬直：在驱动层解决（受击方计算决定不施加硬直状态/技能），门控层不加豁免——硬直状态一旦进入必禁用其列表内行为。

### 6.2 状态Task（需求10）

蓝图派生 BP_BXT_EnterState / BP_BXT_ExitState 注册 `Config/DefaultBattleX.ini` 后显示于编辑器创建菜单（Class Settings 设 Blueprint Display Name / Namespace，如 "State:EnterState"）。Sign=Timeline 运行时 ID（技能场景=SkillID，纯时间轴=TLID）。

```cpp
UCLASS(DisplayName="Enter State")
class UBXT_EnterState : public UBXTask
{
	GENERATED_BODY()
public:
	// 进入的状态集：Tag → 时长（≤0 无限/用默认）
	UPROPERTY(EditAnywhere, Category="State")
	TMap<FGameplayTag, float> EnterStates;
	// Task 结束（End 事件）时是否回退本Task进入的状态
	UPROPERTY(EditAnywhere, Category="State")
	bool bRollbackOnEnd = true;

	virtual void StartTask(...) override;  // 逐条 EnterState(Tag, Duration, TLID)
	virtual void EndTask(...) override;    // bRollbackOnEnd → 逐条 ExitState(Tag, TLID)
};

UCLASS(DisplayName="Exit State")
class UBXT_ExitState : public UBXTask
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category="State")
	FGameplayTagContainer ExitStates;            // 要退出的状态（层级匹配）
	UPROPERTY(EditAnywhere, Category="State")
	bool bTriggerExitPresentation = true;        // 退出时是否触发配置的出场表现

	virtual void StartTask(...) override;  // 命中 Tag 的全部活跃条目 → InternalExit(全部来源, SER_Manual,
	                                       //   bSuppressPresentation=!bTriggerExitPresentation)
};
```

- bRollbackOnEnd=false：状态存续超出 Task（"技能前摇进姿态、结束后姿态再持续 N 秒"）；
- BXT_ExitState 语义为显式退出（全部来源）；
- 行为操控 Task（BXT_EnterBehavior 等）预留：技能内行为进出首选 EnterBehaviors 配置，时间轴中途操控需求出现时再加（避免首期膨胀）；
- 预测：技能时间轴内 BXT_EnterState 由统一预测覆盖（§5.7）；纯本地时间轴无同步语义。

### 6.3 BUFF（预留）

`UBXBuffAsset` 同构（AddBuff/InternalRemoveBuff 收束点，Sign=Buff 运行时 ID；Buff 层/级配置映射 EnterStates 时长刷新）。首期不实现。

## 7. 出招表系统：UBXComboComponent（需求12）

出招表 = 输入驱动的招式图遍历器。决策与执行分离：**组件只做"输入→招式"翻译，从不直接中断技能**——中断是行为矩阵挤出+技能互锁的自动结果。

### 7.1 逻辑归属

| 逻辑 | 归属 |
|------|------|
| 输入捕获（EnhancedInput→语义Tag `BXInput.*`） | PlayerController 映射层 |
| 输入缓冲（InputTag+时间戳，超时清理） | UBXComboComponent |
| 出招表数据（招式图：招式+输入边+条件） | UBXComboAsset（DataAsset，首期 Detail 面板，图编辑器后置） |
| 匹配决策 | UBXComboComponent（AI 走 GetAvailableMoves，不依赖 PC） |
| "是否取消窗口内"查询 | BehaviorComponent.IsBehaviorProtected(Tag) |
| 窗口边界事件 | 技能系统广播 BXEvent.Skill.CancelWindowChanged（推送触发源） |
| 播放技能 | UBXSkillComponent（既有，含预测） |
| 攻击姿态查询 | BehaviorComponent.IsBehaviorActive（姿态=行为） |

### 7.2 双触发时序

**方向A：窗口先开、输入后到**（输入到达时拉取）：

```
OnInput(Light)（EnhancedInput 回调，帧初）:
  1. 缓冲区记录 {Light, 服务器世界时间}
  2. TryResolve：
     a. BehaviorComponent 查攻击行为活跃？（如 BXBehavior.Attack.SlashA）
     b. 活跃 且 IsBehaviorProtected=false → 窗口内 → 匹配边 → PlaySkill(SlashB)
     c. 活跃 且 保护中 → 留缓冲，等方向B
     d. 不活跃 → 出基础招（RootEdges）
```

**方向B：输入先到、窗口后开**（窗口边界推送）：

```
SkillManager Tick 跨入取消窗口边界:
  SetBehaviorProtection(SlashA行为, SkillID, false)
  → 广播 CancelWindowChanged(true)
  → Combo.OnCancelWindowOpened: 缓冲有未超时输入？→ 匹配边 → PlaySkill(SlashB)  ← 同帧出招
```

**连招切换执行链**（组件只调 PlaySkill，其余全自动）：

```
PlaySkill(SlashB)（预测 + ServerPlaySkill）
  → StartBehavior(BXBehavior.Attack.SlashB, SkillID_B)
      → 行为矩阵 (SlashB, SlashA)=挤出 且 SlashA 保护已解除 → 挤出成功
      → BXEvent.Behavior.Exit {SlashA, Sign=SkillID_A, BER_Expelled}
      → 技能互锁 → StopSkill(A, FinishReason=Interrupt)
  → SlashB 时间轴开始
服务器同链路：校验 → InternalStartBehavior → MulticastBehaviorEnter → 确认预测
```

### 7.3 数据结构与服务器校验

```cpp
USTRUCT()
struct FBXComboEdge
{
	FGameplayTag InputTag;                        // BXInput.Light / 组合Tag
	TSoftObjectPtr<UBXSkillAsset> NextSkill;
	FGameplayTagContainer RequiredStates;         // 状态条件（StateComponent 查询）
	FGameplayTagContainer ForbiddenBehaviors;     // 行为条件
};
// UBXComboAsset: RootEdges（空闲出招） + Nodes（Skill + Edges 招式图）
```

- **服务器校验**（HandleServerPlaySkill 扩展）：B 的前置边要求 A 运行中 **或 A 结束距今 < RTT 余量（200ms 宽限）**——严格窗口校验会误杀两端时间轴毫秒偏差下的合法连招；
- 连招链式回滚：A 预测被拒 → B 前置失效 → B 校验失败 → 自然链式回滚，无需特判；
- 边界处理：攻击行为被挤出/挂起（BXEvent.Behavior.Exit 攻击族 + BXEvent.State.Enter 受击族）→ 清空输入缓冲（连招意图作废）；冷却拒绝 → PlaySkill 假冷却链路生效，缓冲已消费即丢弃。

## 8. 事件、调试与日志

| Tag | 参数 | 时机 |
|-----|------|------|
| BXEvent.Behavior.Enter | {Tag, Sign} | 条目从无到有（含挂起恢复） |
| BXEvent.Behavior.Exit | {Tag, Sign, Reason} | 最后来源退出（挤出/挂起/回滚） |
| BXEvent.State.Enter | {Tag, Sign, Duration} | 条目从无到有 |
| BXEvent.State.Exit | {Tag, Sign, Reason} | 最后来源退出 |
| BXEvent.State.PredictResult / BXEvent.Behavior.PredictResult | {Sign, bSuccess} | 预测确认/回滚（客户端本地） |
| BXEvent.Skill.CancelWindowChanged | {SkillID, bOpen} | 取消窗口边界（新增，技能系统） |

```cpp
// FunctionLibrary（重写）：
TArray<FBXBehaviorRuntimeData> GetActiveBehaviors(const AActor* Target);
bool CheckBehaviorActive(const AActor* Target, const FGameplayTag& Tag);
TArray<FBXStateRuntimeData> GetActiveStates(const AActor* Target);
bool CheckStateActive(const AActor* Target, const FGameplayTag& Tag);
float GetStateRemainingSeconds(const AActor* Target, const FGameplayTag& Tag);   // 最小剩余；含无限来源=浮点最大值，无条目=-1
bool IsBehaviorProtected(const AActor* Target, const FGameplayTag& Tag);
TArray<FBXPredictedEntry> GetPredictedEntries(const AActor* Target);            // 调试
```

日志：正常流程 Verbose；保护拒绝/链守卫/挂起冲突/Agent 失败/预测回滚 → `BXCOMP_Behavior` / `BXCOMP_State` Warning（仅异常诊断）。

## 9. Tag 树整理（Q6，双树）

现状问题：`BXBehavior_*` 与 `BXImmBehavior_*` 两前缀并存；`BXStunState_*` 扁平无族结构；`BXEvent_Behavior_*` 旧语义。迁移与旧组件删除同批提交（P8），不留别名——消费方少，零兼容窗口。

```
BXBehavior                             // 行为根（收编 BXImmBehavior_*）
  BXBehavior.Locomotion                // 移动族（矩阵轴/禁用族Tag）
    BXBehavior.Locomotion.Move         //   ← BXBehavior_Locomotion_Move
    BXBehavior.Locomotion.Rotate       //   ← BXBehavior_Locomotion_Rotate
    BXBehavior.Locomotion.Jump         //   ← BXImmBehavior_Locomotion_Jump
    BXBehavior.Locomotion.Landed       //   ← BXImmBehavior_Locomotion_Landed
  BXBehavior.Attack                    // 攻击族（技能姿态行为：SlashA/SlashB…随内容扩展）
  （BXBehavior_Root 删除：全行为查询用 BXBehavior 根）

BXState                                // 状态根
  BXState.Stun                         // 硬直族（SM_Stun 资产节点域）
    BXState.Stun.Knockback             //   ← BXStunState_Knockback
    BXState.Stun.Knockdown             //   ← BXStunState_Knockdown
    BXState.Stun.Prone / Ascending / Falling / Floating / Execution
    （BXStunState_Normal 删除："无硬直"=无条目）
  BXState.Buff.*                       // 裸状态域（灼烧/中毒…，StateConfigs 配置）

BXEvent                                // 事件根（沿用 BXEvent.* 惯例）
  BXEvent.Behavior.Enter / Exit        // 新增
  BXEvent.State.Enter / Exit / PredictResult
  BXEvent.Behavior.PredictResult
  BXEvent.Skill.CancelWindowChanged    // 新增
  （BXEvent_Behavior_Move/Rotate/Jump/Landed、BXEvent_ChangeForbiddenBehavior 随旧体系废弃）

新增：BXInput.*（出招表语义输入：Light/Heavy/方向组合，PC 映射层产出）
不动：BXAttackType/Force/RPos、BXBodyPart、BXGear*、BXTEvent/Data、BXEvent_Buff/Skill/Projectile_*、
      BXSkillInput_*、BXRelationShip、BXActorType
```

迁移要点：Native Tag 改名 = 宏参数 + 全局替换；`Config/Tags/BXGameplayTags.ini` 资产侧 Tag 无冲突不动；行为矩阵轴注册 `BXBehavior.Locomotion` / `BXBehavior.Attack` 族 Tag。

## 10. 配置示例：受击硬直链 + 攻击取消连招

```
行为矩阵（UBXBehaviorSettings，行=想进入，列=已存在）：
  行＼列                    Locomotion(族)   Attack.SlashA
  BXBehavior.Attack.SlashB       —               挤出
  BXBehavior.Stun.Knockback      挤出             挤出
  Locomotion                     —                —

SM_Stun 资产（状态机图，决策树编辑器产出）：
  [Knockback 节点] Duration=1.0  Forbidden=[BXBehavior.Locomotion]
      │ 边(TE_OnExpired, 无条件, TransitionPresentation=Montage(AM_KnockdownLoop)) → [Knockdown]
  [Knockdown 节点] Duration=2.0  Forbidden=[BXBehavior.Locomotion, BXBehavior.Attack]
      │ 边(TE_OnExpired, TransitionPresentation=Montage(AM_GetUp)) → [Recover]
  [Recover 节点] Duration=1.5  Forbidden=[BXBehavior.Locomotion]
      │ 边(TE_OnExpired) → 无出边 → 自然退出(SM空转)

攻击技能资产（SlashA）：
  EnterBehaviors = { BXBehavior.Attack.SlashA: true }     // 姿态行为，默认保护
  EnterStates    = { }                                     // 攻击不进状态（硬直才进）
  CancelWindows  = [ { TimeWindow=(0.4, 0.6) } ]           // 收招取消窗口

受击技能资产（命中受击方）：
  EnterStates = { BXState.Stun.Knockback: 1.0 }            // 外部进入硬直SM（规则一/二）

出招表资产：
  Root: [输入 Light] → SlashA
  SlashA 节点: [输入 Light, 窗口内] → SlashB
```

**执行链（受击）**：受击技能命中 → EnterState(Knockback, 1.0s, SkillID) → 外部转移进 SM_Stun → 挂起移动族（移动 Agent Stop）→ 1s 到期 → 服务器评估 OnExpired 边 → 转移 Knockdown（顶掉式退出+进入，各端多播跟随，倒地循环动画）→ 2s 到期 → 转移 Recover（起身动画）→ 1.5s 自然退出 → 移动族恢复（Agent Start）→ SM 空转。

**执行链（取消连招）**：SlashA 播放中（Attack.SlashA 行为受保护）→ 前摇期敌方击退到达 → 矩阵(击退,SlashA)=挤出但保护中 → 拒绝（霸体）→ 0.4~0.6s 窗口 → 玩家按 Light → 缓冲+窗口查询通过 → PlaySkill(SlashB) → SlashB 挤出 SlashA → 技能互锁中断 A → B 开始。

## 11. 实施计划

| 阶段 | 状态 | 内容 | 涉及文件 |
|------|------|------|----------|
| P1 行为系统 | ✅ 完成 | UBXBehaviorComponent 重写：事实表/矩阵/Agent 接入(BXBAMove等复用)/挂起恢复(遮蔽表方案§4.5)/事件 | Behavior/ 新目录：BXBehaviorComponent.h/cpp、BXBehaviorStructs.h、BXBehaviorEnums.h、BXBehaviorSettings.h/cpp、BXBehaviorFunctionLibrary、BehaviorAgent/ 迁移 |
| P1.5 矩阵编辑器 | ✅ 完成 | UBXBehaviorSettings DetailCustomization（矩阵网格/对角线禁用/图例） | BattleXEditor/CustomLayout/BXBehaviorMatrixCustomization.*，注册于 BattleXEditor.cpp |
| P2 状态系统 | ✅ 完成 | UBXStateComponent 重写：事实表/时长到期/裸状态配置/禁用门控对接/表现三通道 | State/ 新目录：BXStateComponent.h/cpp、BXStateStructs.h、BXStateEnums.h |
| P3 状态机资产 | ✅ 完成 | UBXStateMachineAsset/SMStateNode/SMTransitionEdge（决策树派生）+ 实例管理 + 转移评估（OnTick/OnExpired）+ 外部进入整合（族内单活顶掉） | State/StateMachine/ 三件套；决策树编辑器复用（BXStateMachineType 注册节点/边类型） |
| P4 状态Task | ⬜ 未实施 | BXT_EnterState / BXT_ExitState + 蓝图派生 + ini 注册 | Task/ 新增；Config/DefaultBattleX.ini |
| P5 网络 | ✅ 完成（代码侧） | 双组件 COND_InitialOnly 投影（PreReplication 连接数检测）/ Enter·Exit 多播 / OnRep 差分 LateJoin / 预测缓冲 + Server·Client RPC + 超时回滚。实现注记：① CMC 高频路径与技能链路保持本地 API 不入网，预测走显式 `StartBehaviorNet/StopBehaviorNet`、`EnterStateNet/ExitStateNet`（非 Client 签名自动生成 ClientSyncID 并返回生效 Sign）；② 挂起/恢复以 Tag 粒度控制包 `MulticastControlBehavior` 镜像服务器 Agent 单次停转/重启，Suspended/Resumed 事件流在控制包处理器内本地重放，通用 Enter/Exit 多播剔除这两类原因防 Agent 双停双启；③ 表现三通道经 `TriggerPresentation` 唯一收束点权威转发 `MulticastStatePresentation` 跟随端本播；④ 族内 SM 状态拒绝客户端自主请求（仅权威驱动）；⑤ 客户端无遮蔽表，挂起终态由快照 Flags + 镜像集合编码，配合运动门控下推 | 两组件 + UBXSettings + Net/BXStateBehaviorReplicated.h |
| P6 技能集成 | ✅ 完成 | 五步链（CanStart判定→保护→清场→登记→首帧Task）/EnterStates 收束/CancelWindows 保护切换/互锁监听(Behavior.Exit 按Reason过滤)/CleanSkillTrash 收束 | BXSkillAsset.h、BXSkillManager.cpp、BXSkillComponent.cpp |
| P7 出招表 | ⏸ 暂缓 | UBXComboComponent/UBXComboAsset + 输入缓冲 + 双触发 + 服务器宽限校验（决议：出招组件后置，CanPlayNextSkill 归 ComboComponent） | Combo/ 新增 |
| P8 迁移清理 | ✅ 完成（代码侧） | 删旧 BXSMStun/旧状态机类、FunctionLibrary/CMC 适配；Tag 树：BXBehavior.* 族已建，BXStunState_* 等旧 Tag 保留，随资产制作批量迁移（§9） | State/StateMachine 清理、BXGameplayTags.h/cpp |

- P1-P4 纯本地独立编译验证；P5 需 PIE 双端（确认/拒绝/超时三路径 × 行为/状态两组）；P6 末做互锁联调；P7 依赖 P6 的 CancelWindowChanged；
- RPC 参数全部**按值传递**（UHT 约束，已验证教训）；后续可按技能系统合并经验将 Server Enter/Exit RPC 合并瘦身；
- 组件启用复制用 `SetIsReplicatedByDefault(true)`；网络时间戳统一服务器世界时间域；
- 迭代器安全：两组件 Tick 到期/超时评估一律**先收集快照再执行**（铁律②），回调内同步增删走挂起区/下轮快照。

## 12. 决议记录（全部关闭）

| # | 决议 |
|---|------|
| Q1 互锁粒度 | 技能行为（EnterBehaviors Keys 任一）被挤出或挂起 → 技能中断（姿态破即中断） |
| Q2 预测 | 行为/状态均由各自组件统一预测回滚；技能链路 Sign=SkillID 无独立 RPC |
| Q3 同步通道 | COND_InitialOnly + 显式 Enter/Exit 多播（RunningSkillStates 形态）；确认走多播匹配（非 OnRep 差分）；预测因此可行 |
| Q4 外部进入时长 | 携带优先，-1 用节点/配置默认（规则二） |
| Q5 表现被打断 | 自然截断（动画混合/技能既有中断规则），无需特判 |
| Q6 Tag 树 | 双树整理（§9：BXBehavior.* / BXState.*），P8 批量迁移 |
| Q7 状态禁用强度 | **立即中断**（挂起）+ 状态退出自动恢复（§4.5） |
| Q8 跨族互斥 | **不需要**：不跨族跳转；浮空/地面硬直同为硬直 SM 内部状态；族内互斥=SM 图结构（单当前节点），跨族并存 |
| Q9 行为复制 | **需要**：走 §4.6 模型；Move 行为只同步开关语义，位移归 CMC |
| Q10 出招表姿态 | 攻击姿态=**行为**（技能 EnterBehaviors），取消窗口保护行为，互锁监听 Behavior.Exit |
| R1 外部进入整合 | 外部进入=外部转移：SM 跳节点、重置时长、**不评估出边**；离开只走边评估 |
| R2 时长归属 | 同 Q4（规则二） |
| R3 禁用配置面 | SM 节点 + 裸状态 StateConfig 的 ForbiddenBehaviors；Tag 层级缓解配置量（禁根=全禁） |
| R4 Agent 定位 | 行为中间层执行体（统筹基层组件，现有 BXBAMove 等保留）；战斗编排走技能时间轴（技能即执行体），不挂 Agent |
| R5 Normal 语义 | "无硬直"=无状态条目（SM 空转），BXStunState_Normal 废弃 |
| R6 子状态回收 | 删除所有权传播机制：技能 EnterStates 退出由 CleanSkillTrash 按 Sign 逐条收束 |
