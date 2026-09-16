# BattleX 项目指令与记忆

（用户偏好与通用代码规范见全局指令 `~/.zcode/AGENTS.md`，此处仅记录项目特定规则。）

## 工作流约定
- 编译、构建（UBT/IDE 重建）由用户自己执行，agent 不要尝试定位引擎路径或运行编译命令；代码修改完成后静态自查即可，由用户负责编译验证。
- `UBXMiscManager`（BXMiscManager.h/.cpp，辅助线程管理器）是**预留功能，不可删除**——当前无调用方、未注册 ManagerClasses 属预期状态，后续会接入使用。删除任何"看似无引用"的类之前必须先询问用户。

## 接口设计规则
- `UBXSkillComponent::PlayXxx` 接口的业务参数（如 InLockParts、InAimLocation）必须放在参数列表前面且无默认值，调用方需显式传递；InInstigator 和 InTriggerer 放在参数列表最后，默认值为 nullptr。

## 代码格式规范
- 多行注释使用 `/* */` 包裹；单行注释仍用 `//`。

## Task 系统规则
- 每一个 Task 都有两个默认事件：Start（BXTEvent.Start）和 End（BXTEvent.End），`UBXTProcessor::StartTask/EndTask` 末尾无条件调用 AddPendingTask 触发；Task 的 Events 映射表应预置这两个条目。
- 仅注册配置文件中声明的蓝图派生 Task 类（如 BP_BXT_PlayAnimation）会显示在编辑器创建菜单中，原生 C++ Task 类不显示。
- Task 类需在蓝图 Class Settings 中设置 `Blueprint Display Name` 和 `Blueprint Namespace`，编辑器通过 `UClass::GetMetaData("DisplayName")` / `GetMetaData("Namespace")` 读取，节点标题格式为 "Namespace:DisplayName"，创建窗口按 Namespace 分组。
- Pin 刷新通过 `UBXTask::BuildPinRelatedSignature()` 签名比对机制在基类统一处理，子类在 PostEditChangeProperty 中修改属性后调用 Super 即可。

## 条件系统要点
- 同帧计算结果缓存：UBXConditionManager 的 DerivedCache（Int/Float/Struct 三张表），按条件类 + 参数哈希（GetTypeHash 基于 Owner/Instigator 指针）复用。
- `BuildTaskConditionParameter` 位于 UBXConditionManager；未找到映射配置时回退 `NativeBuildDefaultTaskParam` 填充基础参数（Owner/Instigator）。
- 函数注册采用宏驱动（IMPLEMENT_NATIVE_PARAM_BUILDER、IMPLEMENT_NATIVE_CHECK），通过静态变量初始化自动注册。
- BXTSwitch：Cases 变更时自动重建 Events 保持一一对应，DefaultEventTag 使用 BXTEvent_BranchDefault。

## BUFF 系统要点
- 数据与逻辑分离：UBXBuffAsset 继承 UBXTLAsset，运行时数据 FBXBuffRuntimeData 内嵌 FBXTLRunTimeData，Task 执行完全复用 UBXTLManager；BUFF 特有逻辑（层级、共存策略、生命时长）由 UBXBuffManager 处理。
- 层级变化仅走高层 RebuildEffect 方案（Processor->RebuildEffectTask，Task 需重写 RebuildEffect 响应）。
- BUFF 编辑器复用技能编辑器（FBXBuffEditor 继承 FBXTLEditor），节点带 MinLayer/MaxLayer 配置。
- 需在插件 `Config/DefaultBattleX.ini` 注册：`+ManagerClasses=/Script/BattleX.BXBuffManager`。UBXSettings / UBattleXEditorSettings 均使用 `Config=BattleX`（与插件同名的自定义 ini，引擎经 GPluginLayers 自动加载插件 Config 目录），勿改回 Game/Editor。

## 技能同步框架要点
- 多个技能 RPC 已合并为单一 ServerPlaySkill；ReleaseLocation/ReleaseRotation/AimLocation/AimRotation/LockParts 等业务参数统一移入 InputDatas（BXSkillInput.* 标签）。**RPC 传输形态为 TArray<FBXSkillInputEntry>（UE 不支持 TMap 做 RPC 参数）**，PlaySkillWithInputData 的 BP 入参仍为 TMap，组件内逐条移动转换。
- 五个 Play 接口（PlaySkill、PlaySkillWithTarget、PlaySkillWithLocation、PlaySkillWithDirection、PlaySkillWithInputData），PlaySkillWithInputData 为统一入口，按值收 Map 参数 + 内部 MoveTemp（蓝图调用方数据不受影响，C++ 可 MoveTemp 传入）。
- SkillID 为 64 位：低 61 位 RawID（UTC 时间戳毫秒 + 1~2,000,000 循环计数器，UBXFunctionLibrary::GetUniqueID），高 3 位 Initiator（Server/Client，BXMakeSyncID），用于客户端预测确认/回滚时区分来源。相关宏：BX_SYNC_ID_VALUE_MASK、BX_SYNC_ID_INITIATOR_SHIFT、BX_SYNC_ID_INITIATOR_MASK。
- SkillID 防重：服务器 HandleServerPlaySkill 校验 Initiator 必须为 Client 且 ID 未存在于 SkillRTDatas；InternalPlaySkill 有去重守卫，重复 ID 直接拒绝（防止跨客户端 ID 碰撞/伪造覆盖正在运行的技能）。
- 网络时间戳统一使用服务器世界时间域（UBXFunctionLibrary::GetServerWorldTimeMilliseconds，客户端经 AGameStateBase 时间同步校正），禁止跨机器直接比较本地墙钟（FDateTime::UtcNow）。请求最大年龄走 UBXSettings::SkillRequestMaxAgeMs。
- 性能优化：FBXSkillPlayContext 与 FBXSkillSyncPayload 已合并，使用 MoveTemp 移动语义，InputDatas 深拷贝次数从 3 次降为 0 次；InternalPlaySkill 有 const& 和 && 两个重载（const& 版本通过拷贝 + MoveTemp 委托给 && 版本）。
- StartSkillTasks 负责初始化 RunningSections 和首帧触发；公共初始化已抽取为 `UBXTLManager::StartTimelineSections`，Start/Stop 操作对称。
- 冷却逻辑位于 BXSkillComponent（CooldownMap / RecordCooldown / ClearCooldown / IsCooldownReady / GetCooldownRemainingSeconds，Transient 不复制），HandleServerPlaySkill 在调用 ServerValidateRelease 前做冷却检查。
- 假冷却机制：客户端预测成功后本地立即 RecordCooldown + 标记 PendingCooldownAssetIDs（挡连点）；ClientPredictResult 携带服务器权威冷却剩余（InCooldownRemaining，-1 代表无）：确认时假冷却转正（本地时间 + 服务器剩余），冷却拒绝时同步为真冷却，其他否认（条件/时间戳失败）移除假冷却允许重试。IsCooldownPending 可查询假冷却状态。
- 客户端预测最长 0.3s（SkillPredictMaxDuration），失败回滚；服务端/客户端加速补偿延迟均钳制为技能总时长 50%（ClampAccelerateDuration）；MulticastPlaySkill 为 Reliable（防丢包导致远端看不到技能）。
- UBXSkillManager::Tick 用 ID 快照遍历 SkillRTDatas、CheckPredictTimeout 先收集超时 ID 再回滚：Task 执行链中同步释放新技能（连招链）会导致 TMap rehash，禁止直接持迭代器遍历时触发 Add。
- 碰撞客户端上报：bServerCollisionCheck=false 时服务器 TrackHitBox 只标记等待（bAwaitingClientCollision + ServerExtraLifeTimer 0.15s），拥有连接的客户端（AutonomousProxy）在 CollisionCheck 命中后经 ServerReportCollisionResults 上报，服务器按 TaskFullIndex + DataTag 匹配 Task 清除等待标记，生成服务器侧作用域双键写入 DynamicDatas（无作用域键+作用域键，与客户端 WriteContextData 一致）并触发 BXTEvent_Success 驱动服务器侧反应分支；存在 awaiting Task 的 Section 延迟自然结束。
- 显式中断广播：UBXSkillManager::StopSkill 在 FR_Interrupt 时经 Owner 的 SkillComponent 发 MulticastStopSkill（Reliable）；自然结束/预测回滚不广播。技能结束统一在 CleanSkillTrash 移除数据时广播 BXEvent.Skill.Finished（参数 FBXTLEventParameter 含 FinishReason，原因记录于 FBXSkillRuntimeData::FinishReason，StopSkillTasks/自然结束/回滚三路写入）。
- BUFF 移除广播统一收束在 UBXBuffManager::InternalRemoveBuff（服务器端经 Owner 的 BuffComponent 发 MulticastRemoveBuff，覆盖手动/到期/层级耗尽全部路径），Component 侧不再手动发送；MulticastRemoveBuff_Implementation 用 RemoveBuffWithReason 保证本地事件 Reason 与服务器一致。
- BXBuffManager::Tick 与 UBXSkillManager::Tick 同样使用 ID 快照遍历（移除广播/事件回调可能同步添加 BUFF/技能触发 TMap rehash）。
- P2 状态复制重建（Late Join）：新复制到客户端的对象经复制快照重建进行中的技能/BUFF。**UE5.4 已移除 FastArray 虚回调机制（FFastArraySerializer 非多态）且无服务器端发送前钩子，不可用 FastArray 回调方案**。技能侧：`RunningSkillStates` 为普通 TArray + COND_InitialOnly（仅新连接初始同步发送，已有连接零属性流量，技能动态由显式 RPC 维护），服务器在组件 PreReplication 检测远程连接数增加（新客户端连入）时从 SkillRTDatas 重建快照（常规帧零投影开销，详见 BXSkillComponent::LastProjectedConnectionCount），客户端 OnRep(带旧值)差分重建/兜底停止。BUFF 侧与技能对齐：`RunningBuffStates` 普通 TArray + COND_InitialOnly（仅新连接初始同步发送，已有连接零属性流量），服务器在组件 PreReplication 检测远程连接数增加时从 OwnedBuffIDs 全量重建快照（无增量维护点）；已有连接的 BUFF 动态由显式 RPC 维护——增删走 MulticastAddBuff/MulticastRemoveBuff，层/级/到期变化走 MulticastBuffStateChanged（服务器在 ChangeBuffLayer/ChangeBuffLevel/RefreshBuffLifetime/独立层级到期/手动按层移除收束点广播，客户端 ApplyBuffStateChange 差分应用）。运行数据投影结构见 BXTLReplicated.h（Timeline/Task 指针→ID/Index，按 Task.NetTypes 过滤只保留模拟端位 Task）；重建入口 UBXSkillManager::RebuildSkillFromProjection（反投影恢复 Task 指针后跳过 StartTimelineSections 直接入 SkillRTDatas 续跑）。
- 组件启用复制用 `SetIsReplicatedByDefault(true)`（UE5.4+ 的 bReplicates 是 private，5.8 已验证仍如此）；网络时间戳客户端侧用 `AGameStateBase::GetServerWorldTimeSeconds()`（Base 即有，勿用不存在的 GetServerWorldTimeDelta）。

## 行为/状态网络同步要点（P5，详见 StateBehaviorSystemDesign.md §4.6/§5.7/§11）
- 双组件同步与技能/BUFF 同模型：`RunningBehaviorStates` / `RunningStateStates` 普通 TArray + COND_InitialOnly（PreReplication 远程连接数增加才投影快照），已有连接动态走 Reliable 显式 RPC；OnRep(带旧值) 差分做 Late Join 静默重建 + 消失条目兜底清理。条目结构见 Net/BXStateBehaviorReplicated.h（状态来源投影为剩余时长，重建以接收时刻为计时零点）。
- **预测只走显式 Net 入口**：`StartBehaviorNet/StopBehaviorNet`、`EnterStateNet/ExitStateNet`——权威端直执行、AutonomousProxy 本地执行+预测缓冲+Server RPC、SimulatedProxy 拒绝（纯多播跟随）。非 Client 签名自动生成全新 ClientSyncID 并返回生效 Sign（Sign=0 无法定位回滚）。**CMC 高频路径（FunctionLibrary→StartBehavior）与技能链路（BXSkillManager 调公开本地 API）绝不能切 Net 入口**：技能链路 Sign=SkillID 随技能预测携带无独立 RPC（文档 Q2）。启动参数不上传 RPC，Agent 从基层组件现场取参。
- 确认=多播匹配（MulticastBehaviorEnter 到达即从预测缓冲移除）；拒绝=`ClientReject*` 单独回包回滚；超时=`BehaviorPredictMaxDuration/StatePredictMaxDuration`（0.3s）Tick 快照收集回滚（回滚后迟到的确认多播经跟随路径重建条目自愈）；请求年龄=`BehaviorRequestMaxAgeMs/StateRequestMaxAgeMs`（服务器世界时间域毫秒；客户端 GameState 未校时到位时时间戳为 0 会被拒绝=fail-safe）。防重：同(Tag,Sign)已存在静默忽略不回 Reject（避免误删已确认条目）。缓冲上限 32 条仅告警。
- **Exit 上报仅允许 Client 签名来源**：`ServerExitBehavior/ServerExitState` 校验 Initiator==Client 且 Sign!=0，非 Client 一律忽略（防客户端伪造退出技能驱动/常驻来源）；`StopBehaviorNet/ExitStateNet` 客户端侧同校验前置拦截（否则本地已退服务器拒收造成双端漂移）。停止必须使用 Net Start 返回的生效 Sign（BP 默认参数已移除）。
- **条目已存在时来源级新增须补发确认多播**：服务器 ServerEnter* 前置记录 bEntryExisted，条目新建场景多播由 Broadcast 收束门发出，已存在场景（管线内 bNewEntry=false 不走事件门）执行后显式补发 `MulticastBehaviorEnter/MulticastStateEnter`——否则发起端预测缓冲超时误回滚造成双端漂移。
- **中断/恢复不走通用 Enter/Exit 多播**：Interrupted/Resumed 事件在通用多播的权威门被剔除，由 Tag 粒度控制包 `MulticastControlBehavior(Op)` 精确镜像服务器"代理单次停转/重启"操作粒度，接收端重放代理动作+逐来源本地事件流（客户端无账本 Suspend 镜像，控制包与快照 Flags(bit0) 是中断终态仅有的两个事实源，两序到达均收敛）。
- 表现跟随通道：`TriggerPresentation` 是表现触发唯一收束点，权威端转发 `MulticastStatePresentation(StateTag, FBXStatePresentation)` 跟随端本播（转移边/裸状态进出场全部三通道自动覆盖）；族内 SM 服务器评估转移（Tick 权威门控），SimulatedProxy 不评估边。
- 族内 SM 状态拒绝客户端自主请求（ServerEnterState 内拒绝回 Reject）：族内互斥/转移必须权威驱动，防伪造硬直等处境作弊。
- **裸状态 Net 进入须在 StateConfigs 存在**（ServerEnterState 存在性校验拒绝；技能链路走本地 API 不受此限）；族内 SM 状态 Net 入口**客户端侧预检早退**（EnterStateNet 与服务器对称——否则本地预测先顶掉旧状态，拒绝回滚后旧状态在客户端丢失造成双端漂移）。
- **客户端调用 NetMulticast 会被引擎本地同步执行**（Actor.cpp GetFunctionCallspace 多播分支对客户端返回 Local）——组件内所有 RPC 发起点必须 ROLE_Authority 门控，否则与端内直发逻辑叠加造成事件双发（实例：MulticastControlBehavior 曾在预测端双发 BER_Interrupted/BER_Resumed）。
- **中断/恢复事件流与代理配置解耦**：Interrupt/ResumeBehavior 的事件循环按活跃行为表族匹配（含未配置代理的纯事实行为——互锁事件不依赖代理配置），命令循环按 BehaviorProxyConfigs；控制包目标=命令∪事件目标去重（跟随端纯事实行为也收事件重放；同 Tag 双包会致跟随端事件重放双发）。
- **PT_Skill 表现仅权威端本播**：技能经自身多播同步到达各端，跟随端经 MulticastStatePresentation 再本播会在自主端走预测+ServerPlaySkill 造成服务器二次播放（全端双份）；PT_Timeline/PT_Animation 保持各端本播。
- **外部进入顶掉路径门控解除延迟**：InternalEnterState 的 SM 顶掉分支与 ExecuteTransition 同型——退出 bDeferBehaviorRelease=true，新状态登记账本后统一 ReleaseBehaviorGates（共享禁用 Tag 无 Resume→Interrupt 抖动）。
- 跟随端 SM CurrentNode 双路径镜像：多播跟随进入（HandleClientStateEnter）与 LateJoin 重建（RebuildStateFromState）均按条目 Tag 反查资产置节点，Exit 管线置空——与服务器 InternalEnterState/ExecuteTransition 置节点语义对齐（CurrentNode 在跟随端仅影响查询一致性，转移评估不跑）。
- 广播多播收束点两组件各一处：BroadcastBehaviorEvent / BroadcastStateEvent 内按 ROLE_Authority 门转发，BER_Cleared/SER_Cleared 排除（EndPlay 销毁场景），Interrupted/Resumed 由控制包通道负责（行为侧）。
- 多播 _Implementation 一律首行 Authority 早退（服务器发起 Multicast 时 UE 会本地回环执行实现，早退同时兼作跟随处理器重入防护）。
- SM 转移上下文 ServerTimeMs 使用服务器世界时间域（UBXFunctionLibrary::GetServerWorldTimeMilliseconds），与技能系统一致，勿用 World->GetTimeSeconds。

## 行为 Proxy 体系要点（P9，详见 StateBehaviorSystemDesign.md §13）
- **UBXBehaviorAgent 已改名 UBXBehaviorProxy**（目录 BehaviorAgent/→BehaviorProxy/，UBXBADefault*→UBXProxyMove/Rotate/Jump/Landed，枚举 BAF_*→BPF_*；**2026-09-15 派生类二次改名 UBXProxyXxx→UBXBProxyXxx**，与状态代理派生类 UBXSProxyXxx 前缀区分——基类 UBXBehaviorProxy/UBXStateProxy 名字不变，仅派生类带 B/S 中缀）。双轴命令：EnableProxy/DisableProxy（权限轴，持有基层开关）+ Start/Stop（活动轴）+ UpdateProxy（bWantsProxyUpdate 时组件 Tick 转发）；权限/活动簿记（bEnabled/bStarted/bArmedResume 恢复重放武装）在基类，派生类只重写 Native/Script 槽位。BehaviorFunctions 默认值 341→5461（追加 Native/BP 启用·禁用·更新三对槽位）。
- 组件配置 `BehaviorProxyConfigs: TMap<Tag, FBXBehaviorProxyConfig{ProxyClass, bEnabledByDefault}>`：常驻门控代理（Move/Rotate/Jump）默认启用；事件型（Attack 姿态/Landed）默认禁用、管线 Start 隐式启用（失败回退 Disable）、最后来源退出隐式禁用（InternalStart/Stop 尾部 RefreshProxyGates 归一化）。
- **门控下推（CMC 不再反查行为组件）**：RefreshProxyGates 双禁用位差分命令——中断位落盘（服务器账本中断来源翻转逐代理 SetProxyInterruptBit(true)/客户端控制包 MulticastControlBehavior 直控）、禁止位求值即算 → 差分 → Proxy Enable/Disable → CMC 禁止开关是**基础服务，命名不带 Behavior**：`bMoveBlocked/bRotateBlocked/bJumpBlocked` + 三条 `TBXOperateStack<bool>` 操作记录栈（OperateStack/BXOperateStack.h），`AddMoveBlocked(bInBlocked, InModifier)` 登记修改记录返回句柄（生效值=栈顶自动刷新开关）、`RemoveMoveBlocked(InID)` 按句柄移除、`ClearMoveBlocked(InModifier)` 按修改者注销（代理 Enable=Clear 自己的记录不踩其他系统、Disable=Add(true)，修改者名固定 "ProxyMove/ProxyRotate/ProxyJump"，外部系统同样按自己的标识登记/注销）；CMC 四处（CalcVelocity 加速度清零/ComputeSlideVector/PhysicsRotation 停转向/CanAttemptJump）只读本地开关，事实上报方向（Start/Stop 上报）不变。
- **客户端无账本 Suspend 镜像**：中断事实由控制包按代理粒度承载，无活跃条目的常驻门控同样生效（这是与 P5 时代的关键差异——控制包从"镜像条目停转"升格为"代理命令"）；CanStart 对默认启用代理含 ProxyDisabled 检查保证预测两端一致；LateJoin 中断条目置位禁用代理，OnRep 兜底清理配对清中断位（防镜像残留永久禁挡）。
- 蓝图迁移：旧 BP Agent 父类需手动重定向到 UBXBehaviorProxy 派生类（类已改名，旧资产会加载报错）；组件 Details 面板 BehaviorAgentConfigs 数据随类型变更丢弃，需按新 FBXBehaviorProxyConfig 结构重新配置（bEnabledByDefault=Move/Rotate/Jump 三项）。

- **行为族 Tag 树（2026-09-01 二次整理，平铺）**：`BXBehavior.*` 全部直接挂根下共 15 个——Walk/Run/Sprint/Rotate/Jump/Landed/Falling/LowSkill/HighSkill/Defense/Block/Parry/Dodge/PerfectDodge/ParallelSkill；Locomotion/Attack 分层已删（移动系禁用需逐条列出 Tag）；动画 Tag 移族 `BXMontage.Default`（原 BXBehavior.PlayMontage.Default）；CMC 主动移动事实暂统一报 `BXBehavior.Walk`（走路/跑步/冲刺速度分档待移动状态设计落地；**Root Motion/RootMotionSource 期间不算主动移动**——CalcVelocity 早退前在位即收束，尾部边沿检测两侧均排除（RMS 不走引擎早退，所以启动侧也要排除），RM/RMS 结束由输入边沿恢复），主动转向事实上报已随整理移除（bProactiveRotating 删除；**2026-09-15 补 `BXBehavior.Rotate` 纯门控域 Tag**——无 CMC 事实上报，仅作 UBXBProxyRotate 的注册域键，Forbid/矩阵经此路由到 bRotateBlocked，BP_BXC_Behavior 的 BehaviorProxyConfigs 需挂此键）；**下坠事实 `BXBehavior.Falling`：滞空∧速度不逆重力方向才算下坠**（`Dot(Velocity,GetGravityDirection())>=0`，零速/水平离地即算，不能只看 MOVE_Falling——跳跃/击飞上升段不算下坠）；Tick 每帧物理后边沿判定（`UpdateFallingBehavior`），下坠开始边沿同时收束跳跃行为；离开 MOVE_Falling（OnMovementModeChanged）或滞空中速度转向上时停止。**跳跃行为 `BXBehavior.Jump` 生命周期归 CMC**：DoJump 开始→因重力下坠边沿结束→离开 MOVE_Falling 收束（OnMovementModeChanged 与 Falling 同点，覆盖上升段入水/切飞行等不经下坠边沿与落地的离场）→ProcessLanded 再兜底收束，Sign=0 常驻来源，两者均有停止驱动方可正常配矩阵关系。资产侧需按新 Tag 重配：BP_BXC_Behavior 的 BehaviorProxyConfigs、技能资产 BehaviorTag、矩阵轴。**行为命名约定：显示名（省略 `BXBehavior.` 前缀后）必须少于 16 个字符**——矩阵编辑器表格按 16 字符统一列宽、文字四向居中；新产行为 Tag 起名时遵守。**事实型行为禁止在矩阵配自禁/持久禁用（无停止驱动方者）**：CMC 自动上报的行为中 Walk 无停止驱动方（行为无时长、生命周期归驱动方），自禁=Walk 刹车抖动循环；Jump/Falling/Landed 均有确定死亡点（下坠边沿/落地收束/计时器，见上）可正常配矩阵关系；**Landed 可配时长自动停止**（CMC `LandedBehaviorDuration` 默认 0.1s，**下限钳 0.05s 禁止常驻**——Landed 在位禁用列过广，常驻=落地后全锁死；计时器到期 Stop、每次落地重置）可正常配矩阵关系；这类"带时长的期间禁 X"也可使用状态系统（ForbidBehaviors，状态到期自动解除）。`RebuildRelationIndex` 自动丢弃不在轴内的键与列（改名残留自愈）；**索引为懒重建**——CDO 创建阶段（PostInitProperties）原生 GameplayTag 尚未注册，启动/ini 重载只置脏标记 `bRelationIndexStale`，查询入口（GetRelation/GetExpelTargets/FindForbidDomains）前置 `EnsureRelationIndexFresh` 首查才重建，编辑器 Commit 仍显式重建；重建中未注册 Tag 先经 `FindTagNode` 短路按"与轴无亲缘"处理（5.8 `MatchesAny` 对有效但未注册的 Tag 触发 ensureMsgf 中断）。

## 状态-行为关系矩阵要点（State/BXStateBehaviorSettings + 矩阵编辑器，2026-09-15）
- **状态对行为门控的唯一配置面**（行=状态 Tag，列=行为 Tag，两轴独立数组 StateRelationTags/BehaviorRelationTags）：`InterruptRelations`（状态进入时一次性停运列中在位行为）+ `ForbidRelations`（存续期禁用列中行为，状态退出自动解除），键=状态行 Tag 精确直查（无懒重建索引——运行时只在状态进出时查一次，非行为矩阵的每帧裁决）；行为列可含族 Tag（如 BXBehavior 根=全禁），族语义由行为组件侧匹配承担。
- **状态 Tag 树迁移（2026-09-16）**：`BXStunState.*` 全族改名 **`BXState.*`**（原生 Tag 宏 BXStunState_X→BXState_X、矩阵根串、BeatenTree 默认值同步；ini 资产侧 Tag 与关系矩阵 ini 数据需用户自行替换，旧名残留由轴自动补全的未注册清理兜底——**先改 ini 再开设置页**，否则旧关系数据会被清理）。表头悬停 tooltip 显示完整名+原生 Tag 注释中文名（`FGameplayTagNode::GetDevComment`）。
- **打开设置页零写入（2026-09-16 数据损毁事故后铁律）**：矩阵控件 Construct 期的轴同步（自动补齐+未注册轴移除）**纯内存**，绝不调 SaveToPluginConfig；ini 只在用户显式编辑（单元格点击/拖拽排序）的 Commit 落盘。曾发生：打开页面触发自动补齐→落盘，行为关系整节从插件 ini 消失（GConfig 对该文件的写路径不可靠），用户数据靠 git 恢复。`RebuildRelationIndex` 另加空轴防御：RelationTags 为空（加载失败形态）时跳过残留清理，防关系表连带清盘。
- **轴自动补全（2026-09-16 起，两个矩阵同规）**：打开 BattleX 设置页时（矩阵控件 Construct）按已注册 Tag 同步轴——行为矩阵轴=BXBehavior 根下全部注册后代、状态矩阵行=BXState 根（根串硬编码在 SBXStateRelationMatrix::EnsureAxesComplete，Tag 树迁移时同步改）+列=BXBehavior 根；缺失追加到轴数组**末尾**（不打乱现有顺序，拖拽排序成果保留），未注册 Tag 的轴移除；状态矩阵另清两表行键/列引用中的未注册残留（无运行时索引，自愈收在编辑器侧；行为矩阵的残留清理由 SaveToPluginConfig→RebuildRelationIndex 承担）。有变更才落盘一次，零变更零写入。**轴编辑按钮/表头删轴交互已删**（自动补全之下手动增删无意义），表头纯展示悬停完整名。
- 页面位置（2026-09-16 起）：**并入 BattleX 单页**——本类 `SupportsAutoRegistration=false` 无独立页面，矩阵控件 SBXStateRelationMatrix（BattleXEditor/CustomLayout/）经 FBXSettingsCustomization 注入 UBXSettings 的 BattleX 页面 "Relations" 分类；**全部沿用行为关系矩阵既定规则**：任何变更不走 ForceRefreshDetails/视图重建（单元格 SetText 直改、增删轴 SBox 容器 SetContent 换网格）、冻结表头条+悬停行列黄底高亮、单元格四态循环 空→禁用→中断→禁用并中断、空格=天然共存不落数据。**未做行头拖拽排序**（需要时从 SBXBehaviorRelationMatrix 同法移植——注意状态矩阵两轴是独立数组，行头重排只动 StateRelationTags）。行头显示省略命名空间首段（BXState.Knockback→Knockback，未来 BXState.X→X），列头省略 BXBehavior. 前缀。
- **持久化直连插件 Config/DefaultBattleX.ini（与行为关系配置同文件不同节）**：PostInitProperties 直读 `LoadConfig(nullptr, 插件ini路径)`、编辑器 Commit 直写 `SaveConfig(CPF_Config, 路径)` + Flush + 清项目层残留旧节——**绝不能走 Config=BattleX 默认链**（裸键数组跨层合并退化为末值，行为矩阵踩过的坑）。状态轴选择器 Filter="BXState" 硬编码当前状态命名空间，**状态 Tag 树迁移（如 BXState.*）时需同步更新过滤器**。
- 状态侧无对角线自关系概念（状态×行为不对称矩阵）；行为列中的 Rotate 是纯门控域（无事实上报），作被禁列有效。

## 状态代理要点（State/StateProxy/，2026-09-14）
- **状态系统的物理执行臂**，与行为代理同构但**仅活动轴**（StartState/StopState）——状态没有禁止账本语义（挡启动归行为侧 Forbid），无权限轴 Enable/Disable。基类 UBXStateProxy：槽位 Native/BP × Init/Deinit/Start/Stop/Update，`StateFunctions` 位掩码默认 85（C++ 四件套）；帧更新走 bWantsStateUpdate + 组件 Tick 转发（同行为代理）。**启动无参数**——参数不上网络，从基层组件现场取参（P5 先例）。
- **职责边界**：状态代理只做门控链表达不了的物理效果（移动模式切换/定身/朝向锁定）；"锁移动/跳跃"归状态 Forbid 列表+行为代理，勿在状态代理里重复登记移动封锁。
- **配置面两处**：SM 节点 `UBXSMStateNode::StateProxyClass`（族内状态）与裸状态 `FBXStateConfig::StateProxyClass`；组件 BeginPlay 预建（SM 节点优先、裸状态兜底，与 GetStateBehaviorConfig 查询序一致），键=状态 Tag 一一对应，无族匹配。
- **生命周期挂钩点（全部收束在条目生死，缺一即物理悬挂）**：条目从无到有→Start（InternalEnterState bNewEntry 块、先于表现——表现依赖物理就位）；最后来源退出→Stop（InternalExitState 条目移除后、**先于表现**——表现监听者重入状态时按"先还原再重启"次序，放表现之后会误停新实例）；**所有退出原因含预测回滚均停**（物理必须对称还原）。跟随端：HandleClientStateEnter 条目新建即 Start（代理无控制包通道，随条目直接驱动——与行为门控走控制包原子重放不同）；LateJoin RebuildStateFromState 静默 Start（不启动则新连入端滞空角色直接落地双端漂移）；OnRep 消失条目兜底清理配对 Stop。EndPlay 先 Stop 再 Deinitialize（与行为组件对称）。
- **同 Sign 刷新/来源级新增不重启代理**（与门控/表现/事件同语义）；**Stop 不随 bDeferBehaviorRelease 延迟**——物理直调无账本，转移路径先还原旧再设新同帧完成无抖动（与 Forbid 解除延迟语义不同的原因）。
- 现有 4 个派生类（**2026-09-15 裁剪：原 Knockback/Knockdown/Prone 三个锁转向代理已删**——`BXBehavior.Rotate` 域 Tag 建立后锁转向走状态 Forbid 列表配置，不再代理直登记；Execution 代理的三开关直登记同批移除，只剩瞬时清速度），**命名约定：派生类前缀 UBXSProxy*（状态）/UBXBProxy*（行为）区分系统，基类不带中缀**：BXSProxyAscending/Falling=地面系模式切 MOVE_Falling（挑空速度在地面模式被地面约束清零；退出不还原，落地归 CMC）；BXSProxyFloating=进 Flying 自持悬浮+速度清零、出还原 Falling（仅仍在 Flying 时还原防强踩外部切换）；BXSProxyExecution=定身（仅进入时速度清零，移动/跳跃/转向封锁归 Forbid 列表）。Knockback/Knockdown/Prone 为纯姿态状态无代理（物理=Forbid 列表+表现通道）。代理物理操作一律幂等（重入即重启语义）。
- Floating 的 Flying 切换会经 OnMovementModeChanged 联动 CMC 下坠事实上报（Falling 行为）——离开 MOVE_Falling 收束、还原 Falling 模式后速度向下才重新报，语义正确勿特判。

## 编辑器设置页与 Slate 定制要点
- **UDeveloperSettings 由引擎自动发现注册**（SettingsEditorModule 扫描全部派生类 CDO，模块加载后重建）——页面位置用 `GetContainerName/GetCategoryName/GetSectionName/GetSectionText` 虚函数重写定位；**禁止再手动 ISettingsModule::RegisterSettings，会出双页面**。`GetSectionText/GetSectionDescription` 在基类是 WITH_EDITOR only，重写必须包 `#if WITH_EDITOR`（否则游戏构建编译失败）。**`SupportsAutoRegistration()` 同样在 WITH_EDITOR 内（重写须包裹）**，返回 false 的类不出设置页但配置读写不受影响（唯一消费点 SettingsEditorModule.cpp 的 CDO 扫描；引擎先例 UEditorKeyboardShortcutSettings）。
- **编辑器设置统一收口 `UBattleXEditorSettings`**（2026-09-16 由 UBattleXTimelineEditor 改名扩展，文件迁至模块根 BattleXEditorSettings.h/.cpp，ini 节随之变更为 [/Script/BattleXEditor.BattleXEditorSettings]）——页面 "BattleXEditor"，现含时间轴编辑器 Preview/PreviewScene/Edit 配置，**后续新增编辑器相关设置项一律入此类**，勿再建独立设置页。
- **多个设置类不能靠共用 SectionName 合并页面**——引擎 `FSettingsCategory::AddSection` 对同名 Section 挂不同对象是**整节替换**不是合并（Section 只持一个对象）。**合并页面的正确路径（2026-09-16 已落地）**：两个关系设置类（UBXBehaviorSettings/UBXStateBehaviorSettings）`SupportsAutoRegistration=false` 关掉独立页面，`FBXSettingsCustomization`（UBXSettings 的定制）在 BattleX 页面的 "Relations" 分类（Important 优先级，排 UBXSettings 的 Important 属性之后）注入 `SBXBehaviorRelationMatrix`/`SBXStateRelationMatrix` 两个矩阵控件——控件直接操作关系类 CDO，变更经各自 `SaveToPluginConfig()`（直写插件 ini+Flush+清项目层残留节，行为侧另重建运行时索引）落盘；UBXSettings 保持对象式 Section，导入/导出/恢复默认按钮保留。代价：设置页搜索只索引 UBXSettings 属性，矩阵行搜索时折叠。
- **HideCategory 会把整个分类从构建中剔除**（BuildCategories 对隐藏分类连 GenerateLayout 都不调，AddCustomRow 的自定义行一并消失）——要"隐藏属性+显示自定义行"必须用 `HideProperty(GET_MEMBER_NAME_CHECKED(...))` 逐属性隐藏，分类保留可见。
- Slate 定制网格是构建期一次性生成：**结构变化**（增删行列/属性布局变化）后需要重建网格，通用做法是 `IDetailLayoutBuilder::ForceRefreshDetails()`（否则 UI 不反映；缓存 Builder 用裸指针，定制实例由视图持有先于视图析构）。**纯数据变更不走 ForceRefreshDetails，直改控件**。行为关系矩阵两档都不用 ForceRefreshDetails（整视图重建要重跑全部属性反射枚举+定制+设置页重排，是"每次操作都卡"的根源）：矩阵装在控件持有的 SBox 容器里，单元格点击 SetText 直改单元格文本缓存，增删轴 `MatrixContainer->SetContent(MakeMatrixWidget())` 只换网格本体。**行头拖拽排序（2026-09-03，2026-09-16 随矩阵控件化迁入 SBXBehaviorRelationMatrix.cpp 文件局部类）**：行头为 SBXDraggableAxisHeader（DetectDrag→阈值→FBXAxisDragDropOp，继承 FDecoratedDragDropOp 光标跟随轴名），拖到目标行头上/下半场点亮其顶/底 3px 蓝色插入指示线（下半场=插到其后，支持移到最后一行），拖回源行头不显示指示线且原位短路不落盘；松手 MoveAxis（槽位钳制+原位前后短路）重排 RelationTags 后 Commit+换网格——行头与列头同轴自动同步（两轴共用 RelationTags 数组、关系按 Tag 键存储与顺序无关零迁移）；ESC 取消/丢别处安全（引擎取消路径向悬停控件补发 OnDragLeave，指示线不残留）；行高 30 为文件级常量（行为矩阵 `MatrixRowHeight`/状态矩阵 `StateMatrixRowHeight`，unity build 防撞名）供行头控件与三面板共用防跨面板错位。
- **行为关系配置落插件 Config 目录，读写均直连文件、不经链合并**：`UBXBehaviorSettings` 的 `PostInitProperties` 用 `LoadConfig(nullptr, *插件ini路径)` 直读、编辑器落盘用 `SaveToPluginConfig()`（`SaveConfig(CPF_Config, *插件ini路径)` 直写 `插件 Config/DefaultBattleX.ini` + `GConfig->Flush` + 清理项目层残留旧节，行为侧另重建运行时索引；路径经 `GetPluginConfigIniPath()`，IPluginManager 解析）。**绝不能走 `Config=BattleX` 默认链读写**：自定义名 ini 作为层级文件参与链合并时，裸键数组（无 `+` 前缀的 TArray 逐行）跨层合并会退化为末值——曾致重启后矩阵只剩最后添加的一个轴；同机制是 Buff 注册必须写 `+ManagerClasses`（带 + 前缀跨层追加）的原因。UBXSettings 等其余设置仍走默认链读写项目目录不变。
- `IDetailsView` 不继承 `IDetailLayoutBuilder`；lambda 内调外部成员函数记得捕 `this`。

## 禁止账本要点（v4.5，原保护机制/遮蔽表/中断记账已全部退役）

**术语契约**：**禁止（Forbid）**=挡启动的持续禁令（账本唯一事实，来源解除即失效）；**中断（Interrupt）**=一次性停运在跑实例的动作（就是 Stop——不记账、不恢复、不挡启动）；**接管（Expel）**=进入时踢掉（动作非状态，对在位者执行 Stop）。**设计原则：账本只记持续事实，一次性动作不记账；高级需求全部显式组合**。

- 原子操作：`ForbidBehavior/UnforbidBehavior(域, 来源, Sign)`（账本登记 + **直调域覆盖代理 DisableProxy/EnableProxy**，Proxy 内部幂等）、`InterruptBehavior(域)`（一次性：对域覆盖代理直调 `Proxy->StopBehavior()` + 逐来源 BER_Interrupted，不记账）、`UnforbidBySign(Sign)`（动态禁止来源收束）。原 `SuspendByForbiddenTag/ResumeByForbiddenTag/ResumeBehavior/SuspendMasks/EvaluateBehaviorDisable` 已删除。
- **组合表**：硬直=Forbid（刹车+锁启动）；需打断攻击蒙太奇→追加 `InterruptBehavior[Attack]`；打断（可立刻再出招）=仅中断；聚气只锁=仅 Forbid；矩阵禁止贡献=在位方生死经 `RefreshForbidSources` 自动组合；接管=对在位者执行 Stop（事实表生命周期）。
- **幂等语义**：重复 `ForbidBehavior` 只短路账本登记（`Contains` 检查），**代理调用与控制包必达**——重复禁止期间代理可能已被解禁，跳过会造成裁决与物理脱节；`DisableProxy/EnableProxy` 自身幂等，重复调用无害。中断不记账 → 无启动覆盖规则、无恢复配对。
- **记账收束（七记账点，缺一即鬼影）**：贡献=条目生死入口（InternalStart bNewEntry/InternalStop/多播跟随进入/LateJoin 重建/OnRep 兜底清理，统一走 `RefreshForbidSources(在位Tag)`：先注销旧贡献再按"在位∧未豁免"沿列索引 `ForbidDomainsBySource` 重推导，Lifted/Added 差分直调代理）+ 豁免翻转刷新 + 动态 Forbid/Unforbid（Sign 配对）。
- **豁免写入期折算**：`BehaviorWaivers` 只被 `RefreshForbidSources` 消费（被豁免的在位方不贡献禁止条目），读路径零豁免依赖——账本命中即被禁；翻转时刷新匹配域的全部活跃在位方。
- 状态配置面（2026-09-15 起唯一配置面=全局状态矩阵，见下节）：`ApplyBehaviorGates` 组合中断/禁止两原子，`ReleaseBehaviorGates` 只解除 Forbid（中断是一次性动作无解除），重入保护内置；SM 节点/裸状态上的两列表字段已删除（查询源全部切换到 UBXStateBehaviorSettings，六处：GetStateBehaviorConfig/DoesStateDisableBehavior/进入顶掉快照/退出解除/转移旧禁用快照均经 GetGlobalBehaviorGates 收口）。
- 矩阵单元格=两开关组合（**UI 命名与语义（用户定稿）：禁用=RejectRelations（行在位期间禁用其列）/ 中断=ExpelRelations 停运列中在位者 / 禁用并中断=同格双配置——行=该行为开始时中断哪些行为+在位期间禁用哪些行为，列=被作用方无独立意义；用户已弃用"接管"一词，UI/文档对用户一律说"中断"**），**空单元格=天然共存不落数据**；**对角线自关系可配**：自禁用=挡同 Tag 重入（条目死亡经 RefreshForbidSources 自动解除），自中断=新实例顶掉旧实例（仅技能链 CanStart→清场→登记 路径执行），双配置同格在对角线上实际只生效禁用（CanStart 先于清场）；单元格点击循环 空→禁用→中断→禁用并中断；矩阵网格用 SGridPanel 内容自适应列宽（轴名完整显示）+ 横向 SScrollBox；静态表后处理双索引（`RelationRowIndex` 行索引清场求值 + `ForbidDomainsBySource` 列索引贡献计算——键=行(禁用来源)、值=其禁用列，`RebuildRelationIndex` 懒重建：配置加载/重载只标脏、首查（EnsureRelationIndexFresh 前置于三个查询入口）或编辑器变更时才重建，并自动丢弃不在轴内的键与列与未注册 Tag）——ini 运行期只读，索引物化零漂移。
- **代理查找族匹配**：`FindProxyForBehavior(Tag)` 精确命中优先、沿父链族匹配——CanStart 的权限/CheckStart 检查、启停均支持按族 Tag 操作（族内多代理命中首个）；行为事实键仍为精确 Tag。
- **中断对纯事实行为=纯事件**（显式决策）：无代理配置的行为被中断时只发 BER_Interrupted（互锁链路会停技能→收束→条目死亡），不直接终止事实——事实终止归事实表生命周期（StopBehavior），中断不代劳。
- 控制包=原子重放：`MulticastForbidBehavior/UnforbidBehavior(域,来源,Sign)` + `MulticastInterruptBehavior(域)`，跟随端收到后执行同一个原子函数（账本+代理，与服务器同构）；LateJoin 快照条目按代理 `IsStarted()` 推导 `BX_SYNC_FLAG_BEHAVIOR_STOPPED`——标记条目重建时不自动 Start。
- 更名清单：`SuspendByForbiddenTag→InterruptBehavior`（单参数）、`ResumeByForbiddenTag/ResumeBehavior` 删除、`IsBehaviorSuspended/IsBehaviorInterrupted` 删除、`CheckForbiddenBehavior→IsBehaviorDisabled`、`bSuspendBit→删除`（无门控位）、`BR_Reject→BR_Forbid`/`BR_ExpelReject→BR_ForbidExpel`、`BER_Suspended→BER_Interrupted`、`BER_Resumed` 删除、`BX_SYNC_FLAG_BEHAVIOR_SUSPENDED→BX_SYNC_FLAG_BEHAVIOR_STOPPED`（语义=代理未启动）、`Proxy::StopBehavior` 去参化（真停语义置 bStarted=false）、`StopBehaviorWithParameter/FunctionLibrary 参数停止模板` 删除、事实表 `LastStartParameter` 删除（Proxy 自记参数）。
- 技能侧：`bWaiveOnCancelWindow` 语义不变；互锁监听 Reason 为 `BER_Expelled/BER_Interrupted`；技能开始不登记任何禁用（禁止由矩阵承担）。

## 操作记录栈要点
- `OperateStack/BXOperateStack.h` 为纯 C++ 模板核心（无 UHT、无 .cpp，头文件即全部）；蓝图包装与类型库在 `OperateStack/BXOperateStackTypes.h/.cpp`（含 bool/float/int32/struct 四种包装）。
- 蓝图双轨：**基础类型**（bool/float/int）每类一个薄壳 USTRUCT + 独立库类承载同名四节点（Initialize/Push/RemoveByID/GetEffectiveValue，Category=OperateStack；BP 无重载不可同库，新基础类型=复制薄壳样板）；**结构体**走 `FBXOperateStackStruct`（核心=`TBXOperateStack<FInstancedStruct>`，类型擦除由 FInstancedStruct 承载）+ CustomThunk 通配节点（CustomStructureParam，先例 KGCore execInitOS / UKismetArrayLibrary 内联 DECLARE_FUNCTION），一个包装覆盖全部结构体类型。
- 一个栈一种值类型：基线（Initialize）锚定类型，Push 错型拒绝（返回 0）、GetEffectiveValue 错型返回 false 不写出参。
- UFUNCTION 不可挂 USTRUCT（UhtFunctionParser 的 UFUNCTIONKeyword 只注册 Class/Interface 等作用域）；5.8 的 DECLARE_FUNCTION/DEFINE_FUNCTION 签名已含 UObject* Context 首参（用宏即可）。
- 首个使用者：CMC 门控开关 `bMoveBlocked/bRotateBlocked/bJumpBlocked` + 三条 `TBXOperateStack<bool>`（Add/Remove/Clear*Blocked 三件套，见 P9 门控下推）。

## 工程经验
- 事件系统（BXEventManager）使用 TSet 替代 TArray 存储 GlobalTargetMap/SingleKeyMap/SingleTargetMap，提升注册/注销性能。
- **同模块 unity build（Module.X.cpp）把全部 cpp 拼进同一编译单元——文件级常量/类跨 cpp 撞名即重定义**（实例：行为矩阵与状态矩阵编辑器各自的文件级 `MatrixRowHeight`，状态侧已改名 `StateMatrixRowHeight`）。同目录新增功能相似文件时，文件级符号要带自身前缀。
- UFunction 查找结果以 (Class, FunctionName) 为键缓存，减少反射开销。
- **staging 挂起区机制（Buff/Skill Manager）**：更新窗口（bUpdatingBuff/bUpdatingSkill 为 true 的 Tick 期间）内 Task 回调中的 AddBuff/PlaySkill 一律入 `PendingAddBuffs/PendingAddSkills`（TUniquePtr 容器，对象地址稳定），Tick 末 `MergePendingAddBuffs/MergePendingAddSkills` 合并入主容器（纯数据搬移无回调）。这是根治"TMap 扩容 rehash 使遍历中数据引用悬空"的机制——更新窗口内主容器结构完全冻结。**规则：① 所有按 ID 查询/变更必须走 GetBuffRuntimeDataByID/GetSkillRuntimeDataByID 双查入口（主容器 miss 后查挂起区），禁止直接 BuffRTDatas.Find/SkillRTDatas.Find（Tick 快照遍历除外）；② 新增 Manager 数据查询 API 必须双查；③ OwnerBuffMap/OwnerSkillMap 登记延迟到合并时（更新窗口内对其 Add 新键同样有 rehash 风险）**。TL 侧维持 bUpdatingTimeline 拒绝策略（PlayTimeline 返回 0）。
- BroadcastTasks（FBXTLSectionRTData）为网络多播功能预留：当前仅 AddPendingTask 收集不消费，接入网络多播前请勿移除该机制。
- UBXMiscManager 辅助线程模型：辅助线程仅计时，到期调用经 HTExpiredFunctions 无锁队列投递回游戏线程（Manager::Tick 消费执行 ProcessEvent）——UObject/蓝图反射体系非线程安全，禁止辅助线程直接 ProcessEvent；队列裸指针所有权约定：生产方 new 入队，消费方 Pop 后接管并 delete。
- 迭代器安全三条铁律：① UBXTProcessor::EndTask 内部会按地址从 RunningTasks 移除当前条目（RemoveAt 左移），外层循环禁止再 RemoveCurrent（会误删左移进来的相邻条目；末尾元素场景越界崩溃），遍历 RunningTasks 调 EndTask 必须用索引循环且移除后不推进索引；② TMap 容器在 Task/BP 回调执行期间可能被同步 Remove（如 StopTimeline/StopSkill），遍历必须用 ID 快照（先收集 Key 再 Find）；③ TArray 的 TIterator 为索引式，遍历中 Add/RemoveCurrent 自身安全，但 RemoveAt 会左移元素，索引循环需手动控制推进。
- 广播重入安全通过 BroadcastDepth 计数器与 PendingMutations 延迟变更队列实现。
- 使用 UID Generation 机制防止对象 UID 复用导致的 ABA 问题。
- **编辑器 UID 全局严格唯一（FBXTInputInfo/FBXTOutputInfo/FBXTTransformCreater）**：拷贝构造/拷贝赋值不携带 UID（新实例经 GetUniqueID 惰性分配新值），移动构造/移动赋值保留 UID（TArray 扩容搬迁不失身份，否则 RefreshTransformCreaters 配对断裂、BuildPinRelatedSignature 漂移）；任务复制链（CopyDataFromOther→RestoreTasksRelation）中 CopyDataFromOther 负责重置坐标系创建器 UID（CopyObject 属性搬运会拷入源任务 UID，经 SetUniqueID 注入新任务造成跨任务重复），RestoreTasksRelation 按源创建器 UID 重映射变换输入条目到目标创建器的新 UID（拷贝构造不携带 UID，不重映射则 PreSave 重建时条目被删除重建、DataTask/DataTag 连线丢失）；资产整体复制（内容浏览器 Ctrl+D/SaveAs 等 DuplicateObject 路径，序列化原样携带 UID）经 UBXTask::PostDuplicate（仅 EDuplicateMode::Normal，PIE 瞬态拷贝跳过）重分配：创建器先换新 UID 并记录旧→新映射，配对的变换输入条目按映射重配对（连线保持），其余 Input/Collision/Output 条目 ResetUniqueID 置 0 惰性分配全新值。
- 日志规范：清理频繁打印的正常流程日志，仅保留异常诊断（LogTemp Warning / 模块专属日志类别如 BXMGR_Buff、BXMGR_Timeline、BX_TP）。
- UE 5.8 注意事项（已对照 F:\UE-5.8.1 源码验证）：`CreateBelowWidgetControls`（非 CreateBelowNodeControls，后者已移除）、`AddDefaulted()`（非 AddDefaulted_Get；返回引用变体为 AddDefaulted_GetRef）、呼吸动画用 `FPlatformTime::Seconds()`（非 FSlateApplication::GetTime()；5.5+ 新形式 UE::Platform::Time::Seconds 亦可用，旧形式未弃用）、FastArraySerializer.h 已从 Engine 模块迁至 Net\Core 模块。
