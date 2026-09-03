# BattleX 状态/行为系统架构设计

> 版本：v4.5（v4.5：中断=一次性 Stop 动作——不记账、不恢复，账本只剩禁止，Proxy 四函数与四操作严格一一对应，详见 §4.5/§4.8；已实施+审查修复：Stop 去参化、事实表 LastStartParameter 删除、FindProxyForBehavior 族匹配、幂等语义（重复 Forbid 只短路账本,代理/控制包必达）、中断对纯事实行为=纯事件。v4.4：禁用原子化+显式组合。v4.3：统一禁用账本、静态索引。v4.2：取消窗口豁免。v4.1：双系统分立 + §13 行为代理升级。均已完成（代码侧）；设置页命名统一:UBXSettings→页面"BattleX"、UBXBehaviorSettings→"BattleXBehaviorRelations"、UBattleXTimelineEditor(原 UBXTLEditorSettings)——三者均在 项目设置→插件 分类）
> 范围：`Source/BattleX/State/**` 重写（BehaviorComponent / StateComponent 双组件）+ 决策树派生状态机资产 + 技能/Task/出招表集成 + 网络同步
> 复用既有：UBXDecisionTree*（图框架）、UBXProxyMove/Rotate/Jump/Landed（行为代理，统筹 CharacterMovement）、技能系统同步/预测模型（SyncID/防重/超时/服务器世界时间）
> 关联模块：UBXSkillComponent / UBXTLManager / UBXTask / UBXEventManager / UBXGameplayTags

---

## 1. 概念模型：双系统 + 单向依赖

### 1.1 两个系统

| 系统 | 组件 | 回答的问题 | 生命周期 |
|------|------|-----------|---------|
| **行为系统** | UBXBehaviorComponent | "角色正在**做**什么"（移动/旋转/攻击…） | 驱动方控制（技能/输入/AI 启停），**无时长概念** |
| **状态系统** | UBXStateComponent | "角色处于**什么处境**"（硬直/浮空/灼烧…） | 时长驱动（≤0 无限）+ 状态机自动转移 |

**行为 = 中间层统筹者**：每个行为域挂一个代理（`UBXBehaviorProxy`，v4.1 由 Agent 升格），以双轴命令接管基层组件（CharacterMovement / 输入 / 动画接口）——Enable/Disable 为权限轴（持有基层开关，门控下推），Start/Stop 为活动轴（事实与姿态）。现有 `UBXProxyMove/Rotate/Jump/Landed` 即此类。

**状态 = 处境标记 + 自动状态机**：状态条目有时长、有进出场表现、可禁用行为；同族状态由可视化状态机资产（决策树派生）编排自动跳转。

### 1.2 单向依赖（核心规则）

```
┌──────────────────────────────────────────────────────────────┐
│ UBXBehaviorComponent（行为系统）                                │
│   ActiveBehaviors: Tag → Sources(Sign)                        │
│   行为关系矩阵（并存/中断/禁用）          ← 行为间自动权衡        │
│   禁止原子直调代理（账本覆盖→Disable/Enable）← 中间层定位          │
└───────────────▲──────────────────────────────────────────────┘
                │ 唯一交互：禁用（单向，立即中断 + 恢复）
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

- **状态 → 行为：单向禁用**。状态存在期间其 `InterruptBehaviors` 列表被中断：未启动的禁止启动，运行中的**立即中断**（状态退出后自动恢复）。
- **行为 → 状态：零影响**。行为被中断/停止不产生任何状态变化。
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
| 1 | 行为并行、禁用、中断 | 并行=表内多条目；禁用=矩阵禁用 + 状态禁止（统一账本 §4.8）；中断=矩阵中断（停运在位者） | Behavior |
| 2 | 状态并存、互斥 | 并存=跨族天然；族内互斥=状态机图结构（单当前节点） | State |
| 3 | 技能/BUFF 决定进出行为 | SkillAsset.BehaviorTag（Sign=SkillID），结束收束退出 | 驱动层 |
| 4 | 状态结束触发表现（含行为） | 状态机转移 → 边 TransitionPresentation；裸状态到期 → StateConfig.ExitPresentation（可配技能/时间轴/动画；技能本身可为行为） | State |
| 5 | 状态时长（≤0 无限） | 节点 Duration / 外部携带，来源独立计时 | State |
| 6 | 技能状态集 | SkillAsset.EnterStates（Tag→时长，Sign=SkillID） | 驱动层 |
| 7 | 技能时间段解除互斥（取消窗口） | 矩阵配置技能行禁移动列+移动行中断技能列（技能在位禁移动、移动进入顶掉技能）；窗口内 SetBehaviorWaiver 豁免在位方禁用 → 放行进入 → 中断在位者 → 行为 Exit(Sign=SkillID) → 技能互锁中断（§4.7） | 双方 |
| 8 | 状态自动切换 | 状态机资产：节点+条件边，服务器评估 | State |
| 9 | 状态跳转表现 | 转移边 TransitionPresentation 唯一入口（状态节点不配表现——节点进出场与边过渡冗余；技能/时间轴/纯动画三通道，详情面板按 Type 显隐对应资产） | State |
| 10 | 状态Task | BXT_EnterState（时长+可回退）/ BXT_ExitState（可控表现） | 驱动层 |
| 11 | 状态、行为同步 + 预测回滚 | 双组件各自：COND_InitialOnly 快照 + Enter/Exit 多播 + 统一预测 + 拒绝/超时回滚 | 双方 |
| 12 | 出招表 | UBXComboComponent：输入缓冲 + 双触发查询 + 招式图数据资产 | Combo |

## 3. 总体架构与数据流

```
┌────────────────────────────────────────────────────────────────────┐
│ 驱动层                                                             │
│  技能：EnterBehaviors + EnterStates + CancelWindows(豁免) + 互锁监听 │
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
│ DisableLedger(禁用账本)   │◄─│ StateConfigs(裸状态配置)             │
│ ForbidLedger(禁止账本)    │  │ 禁用门控:ForbidBehaviors→禁止        │
│ BehaviorProxies(预建)     │  │ Tick:服务器转移评估+到期快照          │
│ 复制+预测(§4.6)           │  │ 复制+预测(§5.7)                     │
└──────────┬───────────────┘  └───────────────┬────────────────────┘
           │ Start/Stop(指令)                  │ Enter/Exit/Presentation(通知)
           ▼                                  ▼
   UBXBehaviorProxy(总代理,门控下推)   UBXStateMachineAsset(数据,决策树派生)
   UBXProxyMove 推CMC开关…             SM_Stun:节点图+条件边
                                      (服务器评估,客户端跟随)
```

**调用方向**：驱动层 → 两组件公开 API；BehaviorComponent → Proxy（Enable/Disable/Start/Stop 命令，Proxy 推基层开关）；StateComponent → 状态机资产（转移评估，纯数据）；StateComponent → BehaviorComponent（禁用/恢复，唯一跨系统调用）。Proxy/状态机资产不反向持有事实，全部现查。

## 4. 行为系统：UBXBehaviorComponent

> **术语契约（v4.5；2026-09-01 命名修订+语义定稿）**：**禁止（Forbid）**=挡启动的持续禁令（账本唯一事实，可被豁免，来源解除即失效）；**中断**=一次性停运在跑实例的动作（就是 Stop——不记账、不恢复、不挡启动；需要"期间不许启动"就组合禁止原子），两个来源：状态/手动 `InterruptBehavior(域)` 与**矩阵中断轴**（行进入时停运列中的在位者，`BER_Expelled`）。**矩阵配置面（UI）语义（用户定稿：行=该行为开始时中断哪些行为+在位期间禁用哪些行为，列=被作用方）**：禁用=禁止关系（RejectRelations，行在位期间禁用其列）/ 中断=停运在位者（ExpelRelations，行进入时停运列中在位者）/ 禁用并中断=同格双配置——"接管（挤出）"一词已弃用（用户决策），旧段落中的"接管/挤出"一律读作矩阵"中断"。**设计原则：账本只记持续事实，一次性动作不记账；高级需求全部显式组合**——硬直=禁止（+可选中断停蒙太奇）、打断=中断、聚气只锁=禁止（§4.8 组合表）。

### 4.1 职责

| 职责 | 说明 |
|------|------|
| 唯一行为事实表 | ActiveBehaviors：Tag → Sources(Sign)（启动参数由 Proxy 自记,事实表不存） |
| 进入裁决 | 禁止裁决（账本唯一判据，豁免感知 §4.8）、代理权限与 CheckStart、清场中断（矩阵中断不豁免） |
| 停止裁决 | 驱动方 Stop（Sign 匹配）、矩阵清场中断（BER_Expelled）、状态中断（BER_Interrupted）、回滚 |
| 代理管理 | BeginPlay 预建（BehaviorProxyConfigs：常驻门控代理默认启用，事件型随管线隐式启停）；门控差分命令（§4.5/§13.3） |
| 取消窗口豁免 | BehaviorWaivers（技能驱动，§4.7/§6.1） |
| 中断/恢复 | 状态禁用的中断与自动恢复（§4.5） |
| 复制与预测 | §4.6 |

### 4.2 数据结构

```cpp
// 行为结束原因
UENUM(BlueprintType)
enum class EBXBehaviorEndReason : uint8
{
	BER_Manual = 0,        // 驱动方显式停止
	BER_Expelled,          // 被矩阵中断（停运/取代）
	BER_Interrupted,       // 被状态中断（可恢复）
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

// 禁止账本来源（账本唯一事实：挡启动的持续禁令；多来源叠加，最后一个移除才失效）
struct FBXBehaviorForbidSource
{
	FGameplayTag SourceTag;   // 状态Tag / 在位行为Tag / 系统来源Tag
	int64 Sign;               // 来源签名（配对解除）
};

// 禁止裁决：账本覆盖查询（唯一挡启动判据）
bool IsBehaviorForbidden(const FGameplayTag& InBehaviorTag, FGameplayTag* OutBy = nullptr) const;

// UBXBehaviorComponent（重写）：
TMap<FGameplayTag, FBXBehaviorRuntimeData> ActiveBehaviors;          // 唯一事实
TMap<FGameplayTag, TArray<FBXBehaviorForbidSource>> ForbidLedger;    // 禁止账本(域键→来源列表,§4.8)
TMap<FGameplayTag, TArray<int64>> BehaviorWaivers;                   // 豁免登记(禁止通道写入期输入,§4.7)
TMap<FGameplayTag, FBXBehaviorProxyConfig> BehaviorProxyConfigs;     // 代理配置(仅ProxyClass)
TMap<FGameplayTag, UBXBehaviorProxy*> BehaviorProxies;               // 代理实例(BeginPlay预建,出生即启用)
```

### 4.3 行为关系矩阵（全局，可视化配置 + 静态后处理索引）

```cpp
// 全局行为关系（Config=BattleX，各端一致的 ini；序列化格式与编辑器数据源）
// 写入落点=插件 Config/DefaultBattleX.ini（随插件分发；编辑器侧 SaveConfig 显式传 Filename，
// 默认落点是项目 Config 会造成两份漂移且项目层遮蔽插件层；读取经 GPluginLayers 合并两端）
UCLASS(Config=BattleX, DefaultConfig, meta=(DisplayName="BattleX Behavior Relations"))
class UBXBehaviorSettings : public UDeveloperSettings
{
	UPROPERTY(EditAnywhere, Config, Category="Matrix")
	TArray<FGameplayTag> RelationTags;   // 矩阵轴（支持族Tag分层匹配；当前行为轴为 BXBehavior.* 平铺Tag）
	UPROPERTY(EditAnywhere, Config, Category="Matrix")
	TMap<FGameplayTag, FGameplayTagContainer> ExpelRelations;    // 中断：行进入时停运列中的在位行为
	UPROPERTY(EditAnywhere, Config, Category="Matrix")
	TMap<FGameplayTag, FGameplayTagContainer> RejectRelations;   // 禁用：行在位期间禁用列中的行为（用户语义：行=开始时中断列+在位期间禁用列）

	// ── 静态后处理索引（Transient，懒重建：配置加载只标脏，首次查询/编辑器变更时 RebuildRelationIndex，运行时只读）──
	TMap<FGameplayTag, FBXBehaviorRelationRow> RelationRowIndex;      // 行键→{中断列,禁用列}（清场求值/诊断）
	TMap<FGameplayTag, FGameplayTagContainer> ForbidDomainsBySource;  // 来源→它禁止的域集合（禁止贡献计算,§4.8）
};
```

| 单元格（两开关组合，UI 命名） | 语义 | 行为 |
|--------|------|------|
| 空（默认，不落数据） | 天然共存 | 双活（移动+瞄准并行）——"并存"是关系的缺席，不是一种关系 |
| 中断 | 行停运列中的在位者 | 进入时 InternalStop(列, BER_Expelled)，代理正常 Stop（需求1中断） |
| 禁用 | 行在位期间禁住列 | 列的行为进入失败（需求1禁用） |
| 禁用并中断 | 同行为自关系：开始中断自己+期间禁自己（连招自重启）；跨行为的禁用+中断拆两格 | 两张表同格配置仅用于自关系等同主语场景（§4.7） |

- 行=想要进入，列=已存在；非对称是特性：(A,B)/(B,A) 独立配置；
- **对角线自关系可配（2026-09-01）**：自禁用=挡同 Tag 重入（第一次攻击期间禁用攻击，直到攻击结束——条目死亡经 `RefreshForbidSources` 自动解除）；自中断=新实例顶掉旧实例（重启语义）；同格双配置在对角线只生效禁用（进入判定先于清场）；族 Tag 轴的自关系=族内互斥（在位成员挡住整族）；
- 层级匹配：轴可注册族 Tag 一条覆盖整族（当前行为轴为 `BXBehavior.*` 平铺 Tag，族语义保留给未来分层）；
- **静态后处理不漂移**：ini 运行期只读，索引是只读投影——物化零一致性成本（与动态规则"即时求值不物化"原则不冲突）；
- **索引懒重建（2026-09-03）**：启动时序约束——PostInitProperties 运行于 CDO 创建阶段（ProcessNewlyLoadedUObjects），此刻原生 GameplayTag 尚未注册进管理器，亲缘匹配不可用且清残留会把"族相关但非精确"的合法条目误删（之后编辑器落盘即真实数据丢失）；因此配置加载/ini 重载只置脏标记 `bRelationIndexStale`，三个查询入口（GetRelation/GetExpelTargets/FindForbidDomains）前置 `EnsureRelationIndexFresh` 首查懒重建（一次 bool 判断开销）；编辑器 Commit 仍走显式 RebuildRelationIndex；**未注册 Tag 先短路**（`FindTagNode` 无效直接按"与轴无亲缘"返回 false）——5.8 的 `FGameplayTag::MatchesAny` 对有效但未注册的 Tag 触发 ensureMsgf 中断，而改名残留恰是清理逻辑要删的对象，短路等价旧匹配语义；
- 查询沿父链精确键命中：O(链深)，替代 O(R) 全表扫；
- 编辑器：BattleXEditor DetailCustomization 渲染为矩阵网格——**冻结行头/列头（常驻可见）**：表头条/标签列/网格体三面板用统一列宽/行高（按行为命名约定显示名 ≤16 字符量宽定宽+余量，网格文本统一 9pt，文字四向居中）跨面板对齐，轴名显示省略 `BXBehavior.` 父族前缀（`BXBehavior.PerfectDodge`→`PerfectDodge`，悬停提示完整名）；表头条横向位移由网格体横向滚动回调驱动（`OnUserScrolled`→`SetRenderTransform` 反向平移+裁剪容器），标签列与网格体同处纵向滚动器纵向天然同步、横向钉住；**纵横双向滚动**（外纵向+内横向嵌套 SScrollBox，滚动条经 `ExternalScrollbar` 外接钉在视口右缘/底缘；内层 `ConsumeMouseWheel::Never` 让纵向滚轮穿透给外层；轴数 ≥10 时给 400px 固定高度视口，行少保持自然高度随设置页滚动）；单元格四态循环 空→禁用→中断→禁+中（UI 缩写，含对角线自关系），按钮按关系着色（禁用=蓝/中断=红/禁+中=紫，`SetButtonStyle` + 按关系缓存的着色样式副本，空=默认样式）；单元格悬停时行头/列头/单元格三者全部**黄底黑字**（表头条与标签列为"交互层+黄底高亮层+文字顶层"三层结构，`OnHovered/OnUnhovered` 联动高亮层 `SetVisibility` 与文字 `SetColorAndOpacity`；SButton 无颜色 setter、5.8 无 SetRenderTranslate/HitTestInvisible 参数——见 ue58 迁移记忆），一眼定位当前配置的两个行为；**行头拖拽排序（2026-09-03）**：行头为可拖拽控件 SBXDraggableAxisHeader（按下 DetectDrag 过阈值生成 `FBXAxisDragDropOp`，继承 FDecoratedDragDropOp，光标跟随轴名），悬停拖拽按落点在目标行头上/下半场点亮其顶部/底部 3px 蓝色插入指示线（下半场=插到其后，支持"移到最后一行"），拖回源行头自身不显示指示线且 Drop 原位短路不落盘——松手 `MoveAxis`：插入槽位钳制+原位前后短路，RemoveAt/Insert 重排 `RelationTags` 后 Commit+换网格，**行头与列头同轴自动同步**（两轴共用 RelationTags 数组、关系按 Tag 键存储与顺序无关零迁移）；ESC 取消/丢别处安全（引擎取消路径向悬停控件补发 OnDragLeave，指示线不残留）；行高 30 提升为文件级 `MatrixRowHeight` 常量供行头控件与三面板共用防错位；轴选择器 `SGameplayTagCombo.Filter` 仅列 `BXBehavior.*` 行为族（根名串经 `GetFilteredGameplayRootTags` 裁剪树根，不显示全量 Tag）；**任何矩阵变更都不走 ForceRefreshDetails 整视图重建（设置页卡顿根源）**——单元格点击 SetText 直改单元格文本，增删轴经 SBox 容器 SetContent 只换网格本体。

### 4.4 激活与停止管线

```
InternalStartBehavior(Tag, Sign, Param):
  0. 链深守卫（>8 阻断 + Warning）
  1. 禁止裁决：IsBehaviorForbidden(Tag) → false（FailReason=ForbiddenBy:域）
  1a. 代理权限检查：代理存在且未启用（被禁止原子的 DisableProxy 关掉）→ false
  2. Proxy->CheckStart(Param) 失败 → false
  3. 清场：活跃行为命中的中断列（沿父链查行索引）→ 逐个 InternalStop(该条目, BER_Expelled)
     （矩阵中断是动作不是状态，不受豁免影响，无条件执行）
  4. 表更新（新 Sign 追加 / 同 Sign 刷新）；Proxy->StartBehavior(Param)
     （代理出生即启用,无隐式 Enable/Disable;bEnabledByDefault 字段已删除）
  5. 条目诞生边沿 → RefreshForbidSources(Tag)（禁止贡献登记，§4.8）
  6. 广播 BXEvent.Behavior.Enter {Tag, Sign}
  7. 服务器：MulticastBehaviorEnter；连接数变化时投影快照

InternalStopBehavior(Tag, Sign, Reason, Param):
  1. 查无该 Sign → false；移除来源；仍有其他来源 → true（先停先退）
  2. Proxy->StopBehavior()（无参,真停语义置 bStarted=false）；移除条目
  3. 条目死亡边沿 → RefreshForbidSources(Tag)（禁止贡献注销）
  4. 广播 BXEvent.Behavior.Exit {Tag, Sign, Reason}   ← 技能互锁监听点
  5. 服务器：MulticastBehaviorExit
```

```cpp
// 对外 API：
bool StartBehavior(const FGameplayTag& Tag, int64 Sign = 0, const FInstancedStruct& Param = {});
bool StopBehavior(const FGameplayTag& Tag, int64 Sign = 0);
bool StopBehaviorAllSources(const FGameplayTag& Tag, ...);            // 全来源退出
// 重复 Start = 代理重启 + 来源刷新；代理主动结束（动画完/位移到位）→ StopBehavior(Tag, ProxyKey)
// Stop 参数通道已退役:StopBehavior 组件侧与 Proxy 侧均无参(真停语义,无需参数)
```

**所有停止路径（手动/矩阵中断/状态中断/回滚/清空）都走 InternalStopBehavior**——代理 Stop、事件一个不漏（机制性根治 Stop 链路失效类问题）。

### 4.5 中断（一次性动作，不记账）

中断 = 对域内代理执行 **StopBehavior**——停运在跑实例的一次性动作。**不记账（无持续状态）、不恢复、不挡启动**（需要"期间不许启动"就组合禁止原子，§4.8）：

```cpp
// 中断：对域覆盖的全部代理执行 StopBehavior（子类未实现停止槽位=无操作）
void InterruptBehavior(const FGameplayTag& InDomainTag);
```

```
InterruptBehavior(域):
  对域覆盖的全部代理执行 Proxy->StopBehavior（幂等:未启动=无操作）
  活跃条目逐来源广播 BXEvent.Behavior.Exit {Reason=BER_Interrupted}
                                        ← 技能互锁同样触发（受击中断攻击）
  权威端控制包 MulticastInterruptBehavior(域) → 跟随端同构执行 Stop
```

- 中断不涉及事实表条目（条目留在表中，由事实表自身的生命周期管理）——技能姿态被中断 → BER_Interrupted → 互锁 → StopSkill → CleanSkillTrash → StopBehavior(Sign) → 条目死亡；
- 无监听者的行为（AI 驱动姿态）：Stop 直接生效，不依赖事件监听；
- 中断后想锁启动 → 组合 `ForbidBehavior`（§4.8）；想恢复 → 没有恢复，重新 Start 即可（一次性动作没有需要撤销的状态）。

### 4.6 复制与预测（Q2/Q3 决议，同技能系统模型）

**同步通道（COND_InitialOnly + 显式RPC，同 RunningSkillStates 形态）**：

- `RunningBehaviorStates`：TArray + COND_InitialOnly——仅新客户端连入初始重建（PreReplication 连接数检测投影，LastProjectedConnectionCount 模式），已有连接零属性流量；
- 已有连接动态：`MulticastBehaviorEnter(Tag, Sign)` / `MulticastBehaviorExit(Tag, Sign, Reason)`（Reliable，按值传参）；**中断与禁止不走这两条通用多播**——以原子重放控制包承载（`MulticastForbidBehavior/UnforbidBehavior(域,来源,Sign)`、`MulticastInterruptBehavior(域)`，跟随端收到后执行同一个原子函数，与服务器同构；权威端转发、客户端 handler 均幂等；常驻门控代理无活跃条目时同样生效）；
- Late Join：OnRep(带旧值) 差分重建（静默：仅事实表 + 未停运条目自动 Start，不发事件不触表现——时机与监听者就绪次序不定；"条目在但代理未启动"由代理 IsStarted() 推导编码于条目 Flags(bit0)，标记条目重建时不自动 Start（服务器上它就是被停着的）；OnRep 兜底清理路径同理）。

**统一预测（AutonomousProxy，显式 Net 入口 `StartBehaviorNet`/`StopBehaviorNet`）**：

```
本地启动:
  1. 本地走完整 InternalStartBehavior（账本/矩阵用本地判定——全局ini+多播到达的状态,安全域内）
  2. PredictedBehaviors.Add({Tag, Sign(ClientSyncID), 时间})（非Client签名自动生成全新ClientSyncID并返回生效Sign）
  3. ServerEnterBehavior RPC（仅直接调用类；技能链路 Sign=SkillID 随技能预测携带，无独立RPC）

服务器 ServerEnterBehavior:
  校验 Initiator=Client + Sign 未存在（防重防伪造）+ 请求年龄<=BehaviorRequestMaxAgeMs（服务器世界时间域）
  → 权威裁决 → InternalStartBehavior + Multicast 确认 / ClientRejectBehavior 拒绝回包
  * 来源级新增补发确认：条目已存在时管线内不走Enter广播（bNewEntry=false），须显式补发MulticastBehaviorEnter，
    否则发起端预测缓冲超时误回滚造成双端漂移
```

**回滚——确认与回滚触发严格分离**：

| 事件 | 动作 |
|------|------|
| MulticastBehaviorEnter 匹配 (Tag,Sign) | **确认**：移出预测缓冲（幂等：已存在仅视为确认） |
| ClientRejectBehavior | **回滚**：InternalStop(Tag, Sign, BER_PredictRollback) |
| 预测超时（BehaviorPredictMaxDuration=0.3s，UBXSettings） | **回滚**（Tick 快照收集；回滚后迟到的确认多播经跟随路径重建条目，自愈） |
| 驱动方先行退出 | 移除预测条目（回滚即完成） |

- 本地退出预测：`StopBehaviorNet` 本地执行 + `ServerExitBehavior` RPC 上报（服务器权威退出 + 多播）；**仅允许退出 Client 签名来源**——Server/系统 Sign（SkillID/0常驻）的生命周期归权威管线，客户端伪造上报与 Net 入口误用双端均拒绝；
- SimulatedProxy 纯多播跟随；代理各端本地运行（表现层）；Listen Server/Standalone 单事实源；
- **Move 行为实现注记**：位移事实已由 CMC 复制（移动模式/SavedMove），代理只同步行为开关语义（启停/中断状态），禁止与 CMC 重复同步位移数据；
- 流量注记：若高频行为场景实测超标，可切常规复制（RunningBuffStates 形态）——仅换传输通道。

### 4.7 取消窗口与禁止豁免（禁止通道的写入期修饰）

**动机**：取消窗口的本意是"技能运行到某些时刻，**临时取消对互斥行为的禁用**"——这是互斥规则系统应有的原生能力（对规则的豁免），而非"受害方免疫标志"的对偶编码（原 ProtectionEntries：姿态行为默认免疫矩阵中断、窗口期解除免疫——语义等价但概念归属错层，已退役）。

**机制**：豁免登记在**在位方**，不在进入方——技能只声明"豁免我自己造成的禁止"，放行谁由矩阵的禁止关系决定（技能零新增配置面）。**豁免在写入期折算**：被豁免的在位方不贡献禁止条目（`RefreshForbidSources` 推导时跳过），读取路径零豁免依赖——**账本命中严格等于被禁**：

```cpp
// 豁免表：在位域Tag → 来源Sign列表（多来源叠加，最后一个移除才失效）
TMap<FGameplayTag, TArray<int64>> BehaviorWaivers;

void SetBehaviorWaiver(const FGameplayTag& InSubjectTag, int64 InSign, bool bWaived); // 窗口开=加,窗口关=删
void RemoveWaiversBySign(int64 InSign);                                               // 来源收束(技能结束必走)
bool IsBehaviorWaived(const FGameplayTag& InSubjectTag) const;                        // 族匹配(诊断用)
```

- **语义边界一句话：豁免只豁免"禁用"，不豁免"中断"**——禁用关系放行，中断执行照旧，窗口内谁进来谁顶掉在位者，窗口外谁也进不来；
- **豁免翻转是第三个记账时刻**：翻转时对匹配豁免域的全部活跃在位方重跑 `RefreshForbidSources`（窗口开=撤回贡献，窗口关=重新贡献），§4.8 收束点⑥；
- **豁免不产生网络流量**：服务器权威裁决，客户端事实经多播镜像后自然一致；
- 技能侧参与（§6.1）：窗口边界 `SetBehaviorWaiver(BehaviorTag, SkillID, bInWindow)`，结束 `RemoveWaiversBySign(SkillID)`；`bWaiveOnCancelWindow=false` 关闭豁免语义。

**时序示例**（矩阵 `(S→A/B/C)=禁用`、`(A/B/C→S)=中断`——技能 S 在位禁 A/B/C，A/B/C 进入顶掉 S；技能 S 配置窗口 [1s,2s]）：

| 时刻 | 事件 | 账本 S 类来源 | 行为系统 |
|------|------|--------|---------|
| t=0 | 技能开始，S 活跃 | 空 | A/B/C 被矩阵禁用挡住——禁用来自静态规则，无动态登记 |
| 1s | 窗口开 | `[(S域, SkillID)]` | S 的禁止贡献撤回，A/B/C 放行 |
| 1.2s | A 准备开始 | 不变 | CanStart 通过 → 中断停运 S（Exit, Expelled）→ 技能互锁 → A 激活 |
| 收束 | 技能结束 | RemoveWaiversBySign → 空 | 来源死亡豁免跟走，零残留 |
| 2s | （若技能存活）窗口关 | 空 | S 重新贡献禁止，恢复禁用状态 |

### 4.8 禁止账本与记账收束（v4.5）

**结构**：账本唯一事实=禁止（挡启动的持续禁令）：`ForbidLedger`（域键 → 来源列表，多来源叠加、最后一个移除才失效）。**中断是一次性动作不记账**（§4.5）——账本条目零边沿动作，Enable/Disable 由禁止原子直接调用（Proxy 内部幂等），无门控重算。

**禁止裁决**：

```cpp
bool IsBehaviorForbidden(const FGameplayTag& InBehaviorTag, FGameplayTag* OutBy = nullptr) const;
// 遍历账本键,InBehaviorTag.MatchesTag(键) → O(账本键数,实战个位数)
```

消费点全部收敛：`CanStartBehaviorInternal` 步骤1（FailReason=`ForbiddenBy:域`）、`IsBehaviorDisabled`（原 CheckForbiddenBehavior）、FunctionLibrary 查询转发。

**记账收束——配平契约**：

> 账本中条目 ≡ { (域R, 来源B) : B 活跃 ∧ B 的禁用列覆盖 R ∧ B 未被豁免 } ∪ { 动态禁止来源（未解除） }

**记账点（完整枚举，缺一即鬼影）**：

| # | 入口 | 端 | 时机 |
|---|------|----|------|
| 1 | `InternalStartBehavior`（bNewEntry 边沿） | 双端 | 贡献登记 |
| 2 | `InternalStopBehavior`（最后来源退出） | 双端 | 贡献注销 |
| 3 | `HandleClientBehaviorEnter`（多播跟随直改表） | 客户端 | 条目诞生 |
| 4 | `RebuildBehaviorFromState`（LateJoin 重建直改表） | 客户端 | 条目诞生 |
| 5 | `OnRep` 兜底清理（直改表） | 客户端 | 条目死亡 |
| 6 | `SetBehaviorWaiver` 翻转 | 双端 | 豁免态变化，刷新匹配域的全部活跃在位方 |
| 7 | `ForbidBehavior / UnforbidBehavior` | 双端 | 动态禁止来源（Sign 配对；`UnforbidBySign` 收束） |

（中断不记账——一次性动作无配平义务，§4.5。）

**组合表（原子 → 需求）**：

| 需求 | 组合 |
|------|------|
| 硬直（刹车+锁启动） | Forbid[Walk/Run/Sprint/Jump/Landed]（移动系平铺，逐条列出）；若需打断攻击蒙太奇 → 追加 `InterruptBehavior[HighSkill]`（一次性） |
| 打断（可立刻再出招） | `InterruptBehavior[Attack]`（一次性 Stop；没有恢复，重新 Start 即可） |
| 聚气只锁（当前打完之后不能打） | `ForbidBehavior[Attack]` |
| 矩阵禁用贡献 | Forbid（在位方生死自动组合，1~6 号记账点） |
| 矩阵中断 | 对在位者执行 Stop（事实表生命周期，非账本概念） |

**代理调用与客户端同步**：禁止原子直接调 `Proxy->DisableProxy/EnableProxy`（幂等）；中断原子直接调 `Proxy->StopBehavior`。控制包=原子重放：`MulticastForbidBehavior(域,来源,Sign) / MulticastUnforbidBehavior(域,来源,Sign) / MulticastInterruptBehavior(域)`，跟随端收到后执行同一个原子函数（账本+代理调用，与服务器同构）。Late Join：快照条目按代理 `IsStarted()` 推导 Flags(bit0)——条目在但代理未启动 → 标记，重建时不自动 Start；Forbid 贡献由重建路径补齐。

**性能**：读侧 O(账本键数)（近 O(1)）；写侧 +O(父链深×命中行数) 精确增量贡献维护；静态查询（清场/诊断）O(父链深)。代数戳缓存的挂点在裁决函数上，帧级轮询需求出现时再加。

## 5. 状态系统：UBXStateComponent

### 5.1 职责

| 职责 | 说明 |
|------|------|
| 唯一状态事实表 | ActiveStates：Tag → Sources(Sign+时长) |
| 状态机实例管理 | 按资产创建实例，服务器评估转移 |
| 时长与到期 | 来源独立计时、先到先退；到期触发转移评估（族内）/自然退出（裸状态） |
| 禁用行为门控 | 进入/退出状态时向 BehaviorComponent 中断/恢复（唯一跨系统调用） |
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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) FGameplayTagContainer InterruptBehaviors;  // 存续期中断（停运在跑，不挡启动；Tag层级：禁 BXBehavior 根=全禁）
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) FGameplayTagContainer ForbidBehaviors;     // 存续期禁止（挡启动）
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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) FGameplayTagContainer InterruptBehaviors;  // 存续期中断（停运在跑）
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) FGameplayTagContainer ForbidBehaviors;     // 存续期禁止（挡启动）
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
     ExecuteTransition = ExitStateAllSourcesInternal(当前, SER_Transition, 抑制表现, **延迟中断解除**)
     → InternalEnterState(目标, 节点时长, Sign=0)（族内进入无内置表现，不涉及抑制）
     → 解除旧状态中断登记 → 各自多播 → 转移边 TransitionPresentation 触发（未配置则无表现）
     *延迟中断解除*：退出时不解除中断登记，待新状态登记中断来源后再解除旧登记——
     新旧共享禁用 Tag 经账本多来源登记保持中断（无 Resume→Interrupt 抖动），
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

- 配置面：SM 节点与裸状态各持 `InterruptBehaviors`（停运在跑）+ `ForbidBehaviors`（挡启动）两个列表——**原子显式组合**（§4.8 组合表）：硬直两列都配，"打断不锁"只配中断，"聚气只锁"只配禁止；
- **Tag 层级缓解配置量**：禁 `BXBehavior` 根 = 全行为禁（处决/变形）；禁移动系 = 逐条列出 Walk/Run/Sprint/Jump/Landed（Tag 已平铺，无移动族层）；禁具体 Tag = 单禁；
- 状态进入管线末步：对命中域的活跃行为执行中断（§4.5）；状态条目彻底移除时执行恢复判定；
- 多来源状态：任一来源存续即禁用持续（先到先退语义顺延）。

### 5.6 状态进入/退出管线

```
InternalEnterState(Tag, Duration, Sign, bExternal):
  0. 链深守卫
  1. 族内定位（规则一：外部转移顶掉 SER_External）/ 裸状态直入
  2. 时长解析（规则二：携带优先，-1 用节点/配置默认）
  3. 表更新（同 Sign 刷新 / 新 Sign 追加）
  4. 禁用门控：InterruptBehaviors 逐域 → BehaviorComponent 中断；ForbidBehaviors 逐域 → 禁止
  5. 裸状态：StateConfig.EntryPresentation 触发（族内无内置表现，由转移边在 ExecuteTransition 统一触发；各端本地，PredictRollback 路径除外）
  6. 广播 BXEvent.State.Enter {Tag, Sign, Duration}
  7. 服务器：MulticastStateEnter；连接数变化投影快照

InternalExitState(Tag, Sign, Reason):
  1. 查无该 Sign → false；移除来源；仍有其他来源 → true（先到先退）
  2. 移除条目；裸状态：StateConfig.ExitPresentation 触发（SER_PredictRollback 除外；族内无内置退场表现）
  3. 禁止解除判定 → BehaviorComponent 解禁（§4.8；中断是一次性动作无解除）
     （bDeferBehaviorRelease=true 时延迟：转移路径由 ExecuteTransition 在新状态登记后统一解除）
  4. 广播 BXEvent.State.Exit {Tag, Sign, Reason}
  5. 服务器：MulticastStateExit
  （族内退出后 SM CurrentNode 置空——下次外部进入/转移再激活）
```

**剩余时间语义**：`GetStateRemainingSeconds` 多来源取最小；含无限来源返回浮点最大值（数值比较安全，任何"剩余<X"判定对无限态恒假），无条目返回 -1。

### 5.7 复制与预测

同 §4.6 模型（组件独立一套）：

- `RunningStateStates` COND_InitialOnly + `MulticastStateEnter(Tag, Sign, Duration)` / `MulticastStateExit(Tag, Sign, Reason)`；来源级新增补发确认同 §4.6 注记；
- 表现专用通道：`TriggerPresentation` 唯一收束点权威转发 `MulticastStatePresentation(StateTag, FBXStatePresentation)` 跟随端本播（转移边/裸状态进出场三通道全覆盖，不依赖 Enter 包携带来源边）；
- 预测：`PredictedStates` + `ServerEnterState`/`ServerExitState`/`ClientRejectState`（显式 Net 入口 `EnterStateNet`/`ExitStateNet`，语义与 §4.6 对称：仅 Client 签名可退出、非 Client 签名自动生成）；技能链路（EnterStates / 预测技能时间轴内 BXT_EnterState）Sign=SkillID 随技能预测携带，服务器时间轴执行到同一 Task 处进入，经多播确认（确认延迟 ≈1.5×RTT，超时预算覆盖）；
- SM 转移不预测（服务器权威，多播到达跟随）；族内 SM 状态拒绝客户端自主请求（`ServerEnterState` 内拒绝回 Reject）；
- Late Join OnRep 差分重建（条目 + SM CurrentNode 由条目 Tag 反查资产恢复；多播跟随进入路径同样镜像 CurrentNode，与服务器 `InternalEnterState` 置节点语义对齐）。

## 6. 驱动层集成

### 6.1 技能（需求3/6/7）

```cpp
// UBXSkillAsset（实现形态：单姿态行为 Tag，非行为集）
UPROPERTY(EditDefaultsOnly, Category="Behavior")
FGameplayTag BehaviorTag;                     // 技能姿态行为（空=无行为纯技能；禁止由矩阵承担，无保护登记）
UPROPERTY(EditDefaultsOnly, Category="Behavior")
bool bWaiveOnCancelWindow = true;             // 取消窗口期豁免自身禁止（放行被拒的接招行为，§4.7）
UPROPERTY(EditDefaultsOnly, Category="State")
TMap<FGameplayTag, float> EnterStates;        // 技能状态集（Tag→时长，-1用默认）
UPROPERTY(EditDefaultsOnly, Category="Behavior")
TArray<FBXSkillCancelWindow> CancelWindows;   // 取消窗口 {TimeWindow}
```

- **开始**（InternalPlaySkill 成功，含预测路径）：`StartBehavior(BehaviorTag, SkillID)`；EnterStates 逐条 `EnterState(Tag, 时长, SkillID)`；
- **结束**（CleanSkillTrash 收束，覆盖自然/中断/回滚全部 FinishReason）：`StopBehavior(BehaviorTag, SkillID)` + `ExitState(Tag, SkillID)` 逐条退出 + `RemoveWaiversBySign(SkillID)`——回滚路径必须退出，否则客户端残留；
- **取消窗口**：SkillComponent Tick 窗口边界 `SetBehaviorWaiver(BehaviorTag, SkillID, bInWindow)`（边界切换才写豁免表，常规帧零开销）+ 广播 CancelWindowChanged（出招表推送触发源）；结束按 SkillID 收束全部豁免；
- **技能互锁**：技能组件监听 `BXEvent.Behavior.Exit`（Sign=本技能 SkillID 且 Reason∈{BER_Expelled, BER_Interrupted}）→ StopSkill(FinishReason=Interrupt)——技能行为被矩阵清场或被状态中断，技能即中断；
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

出招表 = 输入驱动的招式图遍历器。决策与执行分离：**组件只做"输入→招式"翻译，从不直接中断技能**——技能中断是行为矩阵清场（停运在位者）+技能互锁的自动结果。

### 7.1 逻辑归属

| 逻辑 | 归属 |
|------|------|
| 输入捕获（EnhancedInput→语义Tag `BXInput.*`） | PlayerController 映射层 |
| 输入缓冲（InputTag+时间戳，超时清理） | UBXComboComponent |
| 出招表数据（招式图：招式+输入边+条件） | UBXComboAsset（DataAsset，首期 Detail 面板，图编辑器后置） |
| 匹配决策 | UBXComboComponent（AI 走 GetAvailableMoves，不依赖 PC） |
| "能否接招"查询 | BehaviorComponent.CanStartBehavior(NextTag)（聚合全部约束：中断/禁止/豁免/代理，§4.8） |
| 窗口边界事件 | 技能系统广播 BXEvent.Skill.CancelWindowChanged（推送触发源） |
| 播放技能 | UBXSkillComponent（既有，含预测） |
| 攻击姿态查询 | BehaviorComponent.IsBehaviorActive（姿态=行为） |

### 7.2 双触发时序

**方向A：窗口先开、输入后到**（输入到达时拉取）：

```
OnInput(Light)（EnhancedInput 回调，帧初）:
  1. 缓冲区记录 {Light, 服务器世界时间}
  2. TryResolve：
     a. BehaviorComponent 查攻击行为活跃？（如 BXBehavior.HighSkill）
     b. 活跃 且 CanStartBehavior(SlashB)=true（窗口豁免已放行禁止）→ 匹配边 → PlaySkill(SlashB)
     c. 活跃 且 CanStartBehavior=false（禁止生效）→ 留缓冲，等方向B
     d. 不活跃 → 出基础招（RootEdges）
```

**方向B：输入先到、窗口后开**（窗口边界推送）：

```
SkillComponent Tick 跨入取消窗口边界:
  SetBehaviorWaiver(SlashA行为, SkillID, true)   // 豁免自身造成的禁止
  → 广播 CancelWindowChanged(true)
  → Combo.OnCancelWindowOpened: 缓冲有未超时输入？→ 匹配边 → PlaySkill(SlashB)  ← 同帧出招
```

**连招切换执行链**（组件只调 PlaySkill，其余全自动）：

```
PlaySkill(SlashB)（预测 + ServerPlaySkill）
  → StartBehavior(BXBehavior.HighSkill, SkillID_B)
      → 行为矩阵 (HighSkill,HighSkill)=禁用并中断（自关系），窗口内 HighSkill 已豁免禁用 → 放行进入，中断停运旧实例
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
- 边界处理：攻击行为被中断（矩阵清场/状态中断，BXEvent.Behavior.Exit 攻击族 + BXEvent.State.Enter 受击族）→ 清空输入缓冲（连招意图作废）；冷却拒绝 → PlaySkill 假冷却链路生效，缓冲已消费即丢弃。

## 8. 事件、调试与日志

| Tag | 参数 | 时机 |
|-----|------|------|
| BXEvent.Behavior.Enter | {Tag, Sign} | 条目从无到有（含中断恢复） |
| BXEvent.Behavior.Exit | {Tag, Sign, Reason} | 最后来源退出（矩阵中断/状态中断/回滚） |
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
bool IsBehaviorWaived(const AActor* Target, const FGameplayTag& Tag);            // 查询在位方豁免态(§4.7)
bool CanStartBehavior(const AActor* Target, const FGameplayTag& Tag);            // 聚合判定(中断/禁止豁免感知/代理)
TArray<FBXPredictedEntry> GetPredictedEntries(const AActor* Target);            // 调试
```

日志：正常流程 Verbose；裁决拒绝/链守卫/中断冲突/代理失败/预测回滚 → `BXCOMP_Behavior` / `BXCOMP_State` Warning（仅异常诊断）。

## 9. Tag 树整理（Q6，双树）

现状问题：`BXBehavior_*` 与 `BXImmBehavior_*` 两前缀并存；`BXStunState_*` 扁平无族结构；`BXEvent_Behavior_*` 旧语义。迁移与旧组件删除同批提交（P8），不留别名——消费方少，零兼容窗口。

```
BXBehavior                             // 行为根（收编 BXImmBehavior_*）
  （2026-09-01 二次整理：Locomotion/Attack 分层移除，13 个行为 Tag 全部平铺挂在根下）
  BXBehavior.Walk / Run / Sprint       // 走路 / 跑步 / 冲刺（CMC 主动移动事实暂统一报 Walk，速度分档待移动状态设计）
  BXBehavior.Jump / Landed             // 跳跃 / 落地（瞬间行为，CMC 上报）
  BXBehavior.LowSkill          // 低优先技能（低优先姿态行为域）
  BXBehavior.HighSkill         // 高优先技能（高优先姿态行为域，原 Attack 族语义；连招姿态在此）
  BXBehavior.Defense / Block / Parry   // 防御 / 格挡 / 弹反
  BXBehavior.Dodge / PerfectDodge      // 闪避 / 极限闪避
  BXBehavior.ParallelSkill             // 可并行技能（不与其它姿态互斥）
  （转向不再作为行为：CMC 主动转向事实上报随整理移除，旋转门控仍可经代理注册域实现）
  （动画 Tag 移出行为族：BXBehavior.PlayMontage.Default → BXMontage.Default）
  （BXBehavior_Root 保留：全行为查询/矩阵轴选择器过滤用 BXBehavior 根）

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
新增：BXMontage.*（动画域，自行为族迁出：BXMontage.Default）
不动：BXAttackType/Force/RPos、BXBodyPart、BXGear*、BXTEvent/Data、BXEvent_Buff/Skill/Projectile_*、
      BXSkillInput_*、BXRelationShip、BXActorType
```

迁移要点：Native Tag 改名 = 宏参数 + 全局替换；`Config/Tags/BXGameplayTags.ini` 资产侧 Tag 无冲突不动；行为矩阵轴按需注册 `BXBehavior.*` 行为 Tag（已平铺）。

迁移进度：2026-08-29 Jump/Landed 原生 Tag 已迁移，BXImmBehavior 前缀退役；**2026-09-01 二次整理**——Locomotion/Attack 分层移除，行为 Tag 平铺为 13 个（Walk/Run/Sprint/Jump/Landed/LowSkill/HighSkill/Defense/Block/Parry/Dodge/PerfectDodge/ParallelSkill），动画 Tag 移族至 `BXMontage.Default`，CMC 主动转向事实上报移除、主动移动事实暂统一报 Walk。资产侧需同步：`BP_BXC_Behavior` 的 BehaviorProxyConfigs 按新 Tag 重配（Move/Jump/Landed 代理），技能资产 BehaviorTag 改用 LowSkill/HighSkill/ParallelSkill，矩阵轴在设置页重选。

## 10. 配置示例：受击硬直链 + 攻击取消连招

```
行为矩阵（UBXBehaviorSettings，行=该行为：开始时中断列+在位期间禁用列，列=被作用的行为；空=天然共存不落数据；对角线自关系可配）：
  行＼列                          HighSkill   Landed
  BXBehavior.HighSkill       禁用并中断         —
  BXBehavior.Landed                       —             —

SM_Stun 资产（状态机图，决策树编辑器产出；原子组合示例——硬直需要"停运在跑+锁启动"，两列都配；
              移动系 Tag 已平铺，禁移动需逐条列出）：
  [Knockback 节点] Duration=1.0  Forbid=[BXBehavior.Walk, BXBehavior.Run, BXBehavior.Sprint, BXBehavior.Jump]
      │ 边(TE_OnExpired, 无条件, TransitionPresentation=Montage(AM_KnockdownLoop)) → [Knockdown]
  [Knockdown 节点] Duration=2.0  Forbid=[BXBehavior.Walk, BXBehavior.Run, BXBehavior.Sprint, BXBehavior.Jump, BXBehavior.HighSkill]  Interrupt=[高优先技能蒙太奇停运,可选]
      │ 边(TE_OnExpired, TransitionPresentation=Montage(AM_GetUp)) → [Recover]
  [Recover 节点] Duration=1.5  Forbid=[BXBehavior.Walk, BXBehavior.Run, BXBehavior.Sprint, BXBehavior.Jump]
      │ 边(TE_OnExpired) → 无出边 → 自然退出(SM空转)
  （Forbid=存续期禁止挡启动,状态退出自动解除;Interrupt=进入时一次性停运,不挡启动不恢复;§4.8 组合表）

攻击技能资产（SlashA）：
  BehaviorTag          = BXBehavior.HighSkill      // 姿态行为（禁止由矩阵承担，无保护登记）
  EnterStates          = { }                                // 攻击不进状态（硬直才进）
  CancelWindows        = [ { TimeWindow=(0.4, 0.6) } ]      // 取消窗口
  bWaiveOnCancelWindow = true                               // 窗口期豁免自身互斥（放行被拒的接招行为）

受击技能资产（命中受击方）：
  EnterStates = { BXState.Stun.Knockback: 1.0 }            // 外部进入硬直SM（规则一/二）

出招表资产：
  Root: [输入 Light] → SlashA
  SlashA 节点: [输入 Light, 窗口内] → SlashB
```

**执行链（受击）**：受击技能命中 → EnterState(Knockback, 1.0s, SkillID) → 外部转移进 SM_Stun → 禁止移动系各 Tag（DisableProxy→CMC 开关刹车+锁启动）→ 1s 到期 → 服务器评估 OnExpired 边 → 转移 Knockdown（顶掉式退出+进入，各端多播跟随，倒地循环动画）→ 2s 到期 → 转移 Recover（起身动画）→ 1.5s 自然退出 → 移动系解禁（EnableProxy→开关放行）→ SM 空转。

**执行链（取消连招）**：SlashA 播放中（矩阵 (HighSkill→HighSkill)=禁用并中断——**对角线自关系**：前摇期同 Tag 新技能被自禁用挡住）→ 0.4~0.6s 窗口 → SetBehaviorWaiver(HighSkill域) 豁免禁用（撤回 S 的禁止贡献）→ 玩家按 Light → CanStartBehavior(HighSkill) 通过 → 缓冲+匹配边 → PlaySkill(SlashB) → 清场中断停运 SlashA（自中断=新实例顶掉旧实例）→ 技能互锁中断 A → B 开始。

## 11. 实施计划

| 阶段 | 状态 | 内容 | 涉及文件 |
|------|------|------|----------|
| P1 行为系统 | ✅ 完成（遮蔽表/挂起机制已被 v4.5 取代,见 P11） | UBXBehaviorComponent 重写：事实表/矩阵/Agent 接入(BXBAMove等复用)/挂起恢复(遮蔽表方案)/事件 | Behavior/ 新目录：BXBehaviorComponent.h/cpp、BXBehaviorStructs.h、BXBehaviorEnums.h、BXBehaviorSettings.h/cpp、BXBehaviorFunctionLibrary、BehaviorAgent/ 迁移 |
| P1.5 矩阵编辑器 | ✅ 完成 | UBXBehaviorSettings DetailCustomization（矩阵网格/对角线禁用/图例） | BattleXEditor/CustomLayout/BXBehaviorMatrixCustomization.*，注册于 BattleXEditor.cpp |
| P2 状态系统 | ✅ 完成 | UBXStateComponent 重写：事实表/时长到期/裸状态配置/禁用门控对接/表现三通道 | State/ 新目录：BXStateComponent.h/cpp、BXStateStructs.h、BXStateEnums.h |
| P3 状态机资产 | ✅ 完成 | UBXStateMachineAsset/SMStateNode/SMTransitionEdge（决策树派生）+ 实例管理 + 转移评估（OnTick/OnExpired）+ 外部进入整合（族内单活顶掉） | State/StateMachine/ 三件套；决策树编辑器复用（BXStateMachineType 注册节点/边类型） |
| P4 状态Task | ⬜ 未实施 | BXT_EnterState / BXT_ExitState + 蓝图派生 + ini 注册 | Task/ 新增；Config/DefaultBattleX.ini |
| P5 网络 | ✅ 完成（代码侧，三轮审查通过） | 双组件 COND_InitialOnly 投影（PreReplication 连接数检测）/ Enter·Exit 多播 / OnRep 差分 LateJoin / 预测缓冲 + Server·Client RPC + 超时回滚。实现注记：① CMC 高频路径与技能链路保持本地 API 不入网，预测走显式 `StartBehaviorNet/StopBehaviorNet`、`EnterStateNet/ExitStateNet`（非 Client 签名自动生成 ClientSyncID 并返回生效 Sign；退出仅允许 Client 签名来源，Server/系统 Sign 双端拒绝防伪造退出与双端漂移）；② 挂起/恢复以 Tag 粒度控制包 `MulticastControlBehavior` 镜像服务器 Agent 单次停转/重启，Suspended/Resumed 事件流在控制包处理器内本地重放，通用 Enter/Exit 多播剔除这两类原因防 Agent 双停双启；③ 表现三通道经 `TriggerPresentation` 唯一收束点权威转发 `MulticastStatePresentation` 跟随端本播；④ 族内 SM 状态拒绝客户端自主请求（仅权威驱动）；⑤ 条目已存在时来源级新增须补发 Enter 多播作预测确认（事件门只在条目新建时广播）；⑥ 跟随端 SM CurrentNode 双路径镜像（多播进入 + LateJoin 重建），与服务器置节点语义对齐；⑦ 乱序自愈：超时回滚后迟到的确认经跟随路径重建、控制包与快照 Flags 两序收敛 | 两组件 + UBXSettings + Net/BXStateBehaviorReplicated.h |
| P6 技能集成 | ✅ 完成 | 四步链（CanStart判定→清场→登记→首帧Task）/EnterStates 收束/CancelWindows 豁免切换(v4.2)/互锁监听(Behavior.Exit 按Reason过滤)/CleanSkillTrash 收束 | BXSkillAsset.h、BXSkillManager.cpp、BXSkillComponent.cpp |
| P7 出招表 | ⏸ 暂缓 | UBXComboComponent/UBXComboAsset + 输入缓冲 + 双触发 + 服务器宽限校验（决议：出招组件后置，CanPlayNextSkill 归 ComboComponent） | Combo/ 新增 |
| P8 迁移清理 | ✅ 完成（代码侧） | 删旧 BXSMStun/旧状态机类、FunctionLibrary/CMC 适配；Tag 树：BXBehavior.* 族已建，BXStunState_* 等旧 Tag 保留，随资产制作批量迁移（§9） | State/StateMachine 清理、BXGameplayTags.h/cpp |
| P9 行为代理升级 | ✅ 完成（代码侧） | Agent→Proxy：命令模型（Enable/Disable/Start/Stop/Update，§13.1）/默认启用配置与两种代理形态（§13.2）/双禁用位门控下推（§13.3）/CMC 本地开关改造/改名迁移（§13.5）。实现注记：① 挂起位落盘、拒绝位求值即算（RefreshProxyGates 统一差分命令）；② 控制包升格为代理粒度命令——服务器遮蔽新覆盖逐代理发包（含无活跃条目的常驻门控），客户端 HandleClientControlBehavior 置/清挂起位+活跃条目事件流重放，无条目场景（门控常态）位同样生效；③ CanStart 对默认启用代理含 ProxyDisabled 检查（客户端无遮蔽表的预测两端一致性）；④ InternalStart 失败回退隐式启用；⑤ OnRep 兜底清理配对清挂起位 | BXBehaviorProxy.*、BXProxyMove/Rotate/Jump/Landed、BXBehaviorComponent、BXCharacterMovementComponent、蓝图父类重定向 |
| P10 取消窗口豁免 | ✅ 完成（代码侧） | 取消窗口机制重构（§4.7）：BehaviorWaivers 豁免表替代 ProtectionEntries 保护表（在位方登记——技能传自身 BehaviorTag，放行集合由矩阵拒绝关系决定；挡入豁免/接管不豁免）；矩阵单元格两开关四组合（编辑器四态循环，"并存"正名为空单元格）；FindRelation 按轴可查（新增 BR_ExpelReject）；CanStart 拒绝检查豁免感知（EvaluateRejectRelations，CanStart/CheckForbiddenBehavior/RefreshProxyGates 共用）；技能层 bProtectedBehavior→bWaiveOnCancelWindow、窗口边界 SetBehaviorWaiver、技能开始不再登记保护；霸体不受影响（驱动层语义 §6.1）。豁免不产生网络流量（服务器权威裁决） | BXBehaviorComponent.h/cpp、BXBehaviorEnums/Settings、矩阵编辑器、BXSkillAsset/Manager/Component、BXBehaviorFunctionLibrary |
| P11 统一禁用账本 | ✅ 完成（代码侧+审查修复） | v4.5（§4.8）：**禁止账本**——中断=一次性 Stop 动作（`InterruptBehavior(域)`：对域覆盖代理执行 StopBehavior + 逐来源 BER_Interrupted），不记账不恢复；账本唯一事实=禁止（`ForbidLedger`：`ForbidBehavior/UnforbidBehavior/UnforbidBySign`，矩阵贡献经 `RefreshForbidSources` 九... 六收束点自动组合，豁免写入期折算）；静态矩阵后处理双索引（行索引清场 + 列索引贡献）；Proxy 四函数与操作严格一一对应（禁止→DisableProxy/EnableProxy、中断→StopBehavior、开始→StartBehavior、停止→StopBehavior），无门控重算（`RefreshProxyGates/ProxyGateStates/bEnabledByDefault/bArmedResume` 删除）；控制包=原子重放（MulticastForbid/Unforbid/InterruptBehavior）；CanStart 只看禁止覆盖（`IsBehaviorForbidden`）；状态配置面两列表 Interrupt/Forbid；更名：EvaluateBehaviorDisable→IsBehaviorForbidden(IsForbiddenByLedger)、ResumeBehavior 删除、bEnabledByDefault 删除（代理出生即启用）。审查修复：① 幂等语义——重复 Forbid 只短路账本 Contains,**代理 DisableProxy 调用与控制包必达**（重复禁止期间代理可能已被解禁,跳过致裁决与物理脱节）;② `Proxy::StopBehavior()` 去参化（真停语义置 bStarted=false,停止参数通道随恢复语义退役,StopBehaviorWithParameter/FunctionLibrary 参数停止模板删除）;③ 事实表 LastStartParameter 删除（无读者,Proxy 自记参数）;④ `FindProxyForBehavior` 精确优先沿父链族匹配（CanStart 权限/CheckStart 支持按族 Tag）;⑤ 中断对纯事实行为=纯事件（显式决策,无代理配置的行为中断只发事件不代劳终止） | BXBehaviorEnums/Structs/Settings/Component/BehaviorProxy、矩阵编辑器、BXStateComponent、FunctionLibrary、BXSkillComponent（互锁 Reason）、状态资产 |
| P12 矩阵编辑器修订 | ✅ 完成（2026-09-01 二次重施+增删轴优化） | 四问题修复：① 矩阵网格 SGridPanel 内容自适应列宽（行/列轴名完整显示，废弃固定 140/90px）+ 横向 SScrollBox；轴选择器 `SGameplayTagCombo.Filter=BXBehavior` 仅列行为族 Tag（不显示全量）；② 对角线自关系放开（UI 去掉对角线禁用 + `GetRelation` 删同 Tag 早退）——自禁用=挡同 Tag 重入（条目死亡自动解除），自中断=新实例顶掉旧实例（重启语义），双配置同格在对角线只生效禁用（进入判定先于清场）；③ **任何矩阵变更都不再 ForceRefreshDetails 整视图重建（每次操作卡顿的根源）**——单元格点击 Commit(SaveConfig+RebuildRelationIndex) 后 SetText 直改单元格，增删轴经定制持有的 SBox 容器 `SetContent(MakeMatrixWidget())` 只换网格本体；④ 关系命名修订为**禁用/中断/禁用并中断**（"接管"一词弃用，UMETA DisplayName/单元格文本/页面描述同步；C++ 标识符 BR_Expel/ExpelRelations 未动以保 ini 序列化兼容）；⑤ 行为关系配置写入**插件 Config/DefaultBattleX.ini**（编辑器 Commit 用 `SaveConfig(CPF_Config, *插件ini路径)` + `GConfig->Flush` + 清理项目层残留旧节；SaveConfig 默认落点是项目 Config，会与插件层副本两份漂移且项目层遮蔽插件层；读取仍走 Config=BattleX 的 GPluginLayers 合并链）。**回退史**：本行修订曾完成后被 v4.5 会话以旧版方案覆盖一次（SButton 网格/ForceRefreshDetails 每击重建/对角线禁用/命名 接管→禁止→禁止+接管/页面位于 插件→BattleXBehaviorRelations），2026-09-01 已在其上重新落地并加档③——后续任何会话整段重写矩阵编辑器时，以本行为准校验 | BattleXEditor/CustomLayout/BXBehaviorMatrixCustomization.*、BXBehaviorSettings.h/cpp、BXBehaviorEnums.h |

- P1-P4 纯本地独立编译验证；P5 需 PIE 双端（确认/拒绝/超时三路径 × 行为/状态两组）；P6 末做互锁联调；P7 依赖 P6 的 CancelWindowChanged；
- RPC 参数全部**按值传递**（UHT 约束，已验证教训）；后续可按技能系统合并经验将 Server Enter/Exit RPC 合并瘦身；
- 组件启用复制用 `SetIsReplicatedByDefault(true)`；网络时间戳统一服务器世界时间域；
- 迭代器安全：两组件 Tick 到期/超时评估一律**先收集快照再执行**（铁律②），回调内同步增删走挂起区/下轮快照。

## 12. 决议记录（全部关闭）

| # | 决议 |
|---|------|
| Q1 互锁粒度 | 技能行为（EnterBehaviors Keys 任一）被矩阵中断或状态中断 → 技能中断（姿态破即中断） |
| Q2 预测 | 行为/状态均由各自组件统一预测回滚；技能链路 Sign=SkillID 无独立 RPC |
| Q3 同步通道 | COND_InitialOnly + 显式 Enter/Exit 多播（RunningSkillStates 形态）；确认走多播匹配（非 OnRep 差分）；预测因此可行 |
| Q4 外部进入时长 | 携带优先，-1 用节点/配置默认（规则二） |
| Q5 表现被打断 | 自然截断（动画混合/技能既有中断规则），无需特判 |
| Q6 Tag 树 | 双树整理（§9：BXBehavior.* / BXState.*），P8 批量迁移 |
| Q7 状态禁用强度 | **立即中断**（一次性 Stop，不恢复）+ **组合禁止原子**存续期挡启动、状态退出自动解禁（§4.5/§4.8） |
| Q8 跨族互斥 | **不需要**：不跨族跳转；浮空/地面硬直同为硬直 SM 内部状态；族内互斥=SM 图结构（单当前节点），跨族并存 |
| Q9 行为复制 | **需要**：走 §4.6 模型；Move 行为只同步开关语义，位移归 CMC |
| Q10 出招表姿态 | 攻击姿态=**行为**（技能 BehaviorTag）；取消窗口=v4.2 豁免机制（原保护表退役，§4.7），互锁监听 Behavior.Exit |
| R1 外部进入整合 | 外部进入=外部转移：SM 跳节点、重置时长、**不评估出边**；离开只走边评估 |
| R2 时长归属 | 同 Q4（规则二） |
| R3 禁用配置面 | SM 节点 + 裸状态 StateConfig 的 `InterruptBehaviors`（v4.3 前名 ForbiddenBehaviors）；Tag 层级缓解配置量（禁根=全禁） |
| R4 Agent 定位 | 行为中间层执行体（统筹基层组件，现有 BXBAMove 等保留）；战斗编排走技能时间轴（技能即执行体），不挂 Agent——**v4.1 P9 升格为 Proxy 双轴命令模型，见 §13** |
| R5 Normal 语义 | "无硬直"=无状态条目（SM 空转），BXStunState_Normal 废弃 |
| R6 子状态回收 | 删除所有权传播机制：技能 EnterStates 退出由 CleanSkillTrash 按 Sign 逐条收束 |

## 13. 行为代理升级规划：Agent → Proxy（v4.1，✅ 完成（代码侧），实现注记见 §11 P9）

> **动机**：① 门控交互方向翻转——CMC 不再反查行为组件（现状：CalcVelocity/ComputeSlideVector/PhysicsRotation/CanAttemptJump 四处 `CheckForbiddenBehavior` 反查），由行为组件裁决后**单向推送开关**；② R4"Agent 统筹基层组件"的落地形态升级——Agent 从"启停回调执行体"升格为**行为域总代理**，权限（启用/禁用）与活动（开始/结束）双轴分家。
>
> **否决项备查**：组件直推 CMC（行为系统硬编码移动组件类型，模块耦合）；开关全放 Agent（语义错位——现 Agent 启停时机=事实上报，静止即 Stop 会反向禁用；拒绝关系是聚合判断，无 Agent 回调时机；族级禁令需跨轴分发）。
>
> **分层原则**：组件负责"何时算、算什么"（聚合求值 + 差分），Proxy 负责"怎么执行"（持有基层开关）。事实表语义不变——ActiveBehaviors 仍是"正在做什么"，与 Proxy 启用态解耦。

### 13.1 Proxy 命令模型

| 命令 | 轴 | 语义 |
|------|----|------|
| EnableProxy / DisableProxy | 权限 | 禁用：置 bEnabled=false → 若活动中先收 Stop → 推基层开关 off；启用：置 true → 禁用期间被停转的按快照参数重放 Start → 推开关 on |
| StartBehavior / StopBehavior | 活动 | 语义同现状（CMC 事实上报 / 技能姿态），仅在 bEnabled 时生效 |
| UpdateProxy(DeltaSeconds) | 帧驱动 | bWantsProxyUpdate=true 的代理由组件 Tick 转发（转向插值/轨迹类代理用） |
| CheckStartBehavior | 判定 | 沿用 |

- Proxy 状态：`bEnabled` / `LastStartParameter`（仅作最近启动参数记录,无恢复重放消费方）/ `bWantsProxyUpdate`；`BehaviorFunctions` 位掩码扩位（Native/BP × Enable/Disable/Update 三对新槽位）；`StopBehavior()` 已去参化（真停语义,重复 Stop 幂等）；
- **组件是唯一命令源**：Proxy 不自治、不持有禁用来源逻辑——多状态叠加禁用由组件聚合（遮蔽表），最后一个禁用来源退出才发一次 Enable；幂等由组件侧"最后命令值"差分保证。

### 13.2 组件配置与两种代理形态

```cpp
// 单表配置（避免 ProxyClass 表与启用表两份漂移）
USTRUCT() FBXBehaviorProxyConfig
{
    TSubclassOf<UBXBehaviorProxy> ProxyClass;
};
TMap<FGameplayTag, FBXBehaviorProxyConfig> BehaviorProxyConfigs;  // v4.5:仅ProxyClass,代理出生即启用
```

BeginPlay 流程：创建全部实例 → Initialize（代理出生即启用,无逐个 Enable）。

| 形态 | 例子 | 默认态 | 权限轴驱动方 |
|------|------|--------|--------------|
| 常驻型 | Move/Rotate/Jump | 启用 | 门控通道（§13.3） |
| 事件型 | Attack 姿态、Landed | 启用（出生即启用） | 无隐式启停——条目存在即运行,Start/Stop 由事实表生命周期驱动（v4.5:两种形态合并,`bEnabledByDefault` 字段删除） |

### 13.3 门控通道：双禁用位，一个出口

v4.5 起门控下推简化为**禁止原子直调**：`ForbidBehavior/UnforbidBehavior` 在账本登记/解除后直接对域覆盖代理调 `Proxy->DisableProxy/EnableProxy`（Proxy 内部 bEnabled 幂等兜底），无门控重算、无位落盘；子类未实现 Disable 槽位=纯裁决无物理效果（效果由子类实现决定，§4.8）：

- **中断原子**：对域覆盖代理直接调 `Proxy->StopBehavior`（活动轴停，控制包同构重放）；中断不驱动门控（门控归禁止独占）；
- **覆盖求值**：禁止原子对域覆盖代理逐个直调（幂等）——无求值时机问题；跟随端账本经控制包同构重放,两端一致；
- **出口唯一**：`Proxy->EnableProxy/DisableProxy` → Proxy 原生实现推 `UBXCharacterMovementComponent::SetBehaviorMoveBlocked/RotateBlocked/JumpBlocked`；组件不再 `FindComponentByClass` 移动组件，行为系统与移动模块解耦，三轴开关由三个 Proxy 各自认领；
- **CMC 侧改造**：四处反查改读本地布尔开关（CalcVelocity 加速度清零 / ComputeSlideVector 跳过滑动修正 / PhysicsRotation 停转向 / CanAttemptJump 挡跳跃）；事实上报方向（Start/Stop 上报）原样保留；
- **静止不抖动**：Move 事实上报 Stop（条目移除）不影响 Proxy 启用态，开关不随事实抖动；
- 历史注记：早期方案 MirroredSuspensions 镜像集合→"中断位"（v4.1~v4.4）→ v4.5 起门控位整体删除（禁止原子直调代理，无位可落盘）。

### 13.4 网络衔接

- Late Join：快照条目按代理 IsStarted() 推导 STOPPED 标志 → 重建时不自动 Start（服务器上它就是被停着的）；
- Update 通道：组件 Tick 已开（P5 预测超时用），追加一循环遍历 `bEnabled && bWantsProxyUpdate` 代理转发，无新 Tick 成本；
- 预测/多播/快照模型零变更（§4.6 不受影响）。

### 13.5 改名与迁移清单

| 项 | 现 | 新 |
|----|----|----|
| 基类 | UBXBehaviorAgent | UBXBehaviorProxy |
| 目录 | Behavior/BehaviorAgent/ | Behavior/BehaviorProxy/ |
| 组件配置 | BehaviorAgentConfigs（Tag→Class） | BehaviorProxyConfigs（Tag→FBXBehaviorProxyConfig） |
| 默认实现 | UBXBADefaultMove/Rotate/Jump/Landed | UBXProxyMove/Rotate/Jump/Landed（避开 BXBP≈蓝图 BP_ 前缀混淆） |

- 蓝图 Agent 父类改绑需重定向（项目内数量少，手改即可）；
- Suspend/Resume 管线内部 `Agent->Stop/Start` 调用点改为命令式 `Proxy->Disable/Enable`；BER_Suspended/Resumed 事件广播与技能互锁不变；
- **不动**：事实表结构、矩阵裁决、取消窗口豁免（v4.2 重构后形态 §4.7）、CanStart 链、事件系统、技能集成（P1-P8 成果全保留）。
