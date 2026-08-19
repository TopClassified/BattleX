# BattleX 项目指令与记忆

（用户偏好与通用代码规范见全局指令 `~/.zcode/AGENTS.md`，此处仅记录项目特定规则。）

## 工作流约定
- 编译、构建（UBT/IDE 重建）由用户自己执行，agent 不要尝试定位引擎路径或运行编译命令；代码修改完成后静态自查即可，由用户负责编译验证。
- `UBXMiscManager`（BXMiscManager.h/.cpp，辅助线程管理器）是**预留功能，不可删除**——当前无调用方、未注册 ManagerClasses 属预期状态，后续会接入使用。删除任何"看似无引用"的类之前必须先询问用户。

## 接口设计规则
- `UBXSkillComponent::PlayXxx` 接口的业务参数（如 InLockParts、InAimLocation）必须放在参数列表前面且无默认值，调用方需显式传递；InInstigator 和 InTriggerer 放在参数列表最后，默认值为 nullptr。

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
- 需在插件 `Config/DefaultBattleX.ini` 注册：`+ManagerClasses=/Script/BattleX.BXBuffManager`。UBXSettings / UBXTLEditorSettings 均使用 `Config=BattleX`（与插件同名的自定义 ini，引擎经 GPluginLayers 自动加载插件 Config 目录），勿改回 Game/Editor。

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
- P2 状态复制重建（Late Join）：新复制到客户端的对象经复制快照重建进行中的技能/BUFF。**UE5.4 已移除 FastArray 虚回调机制（FFastArraySerializer 非多态）且无服务器端发送前钩子，不可用 FastArray 回调方案**。技能侧：`RunningSkillStates` 为普通 TArray + COND_InitialOnly（仅新连接初始同步发送，已有连接零属性流量，技能动态由显式 RPC 维护），服务器在组件 PreReplication（每帧、收集前）从 SkillRTDatas 重建快照解决运行数据新鲜度，客户端 OnRep(带旧值)差分重建/兜底停止。BUFF 侧：`RunningBuffStates` 普通 TArray 无条件复制（低频变化承担初始重建+层/级/到期持续同步，层级变化仅经此通道，原 MulticastBuffLayerChanged RPC 已删除），服务器在 AddBuff/InternalRemoveBuff/ChangeBuffLayer/ChangeBuffLevel/RefreshBuffLifetime 收束点维护。运行数据投影结构见 BXTLReplicated.h（Timeline/Task 指针→ID/Index，按 Task.NetTypes 过滤只保留模拟端位 Task）；重建入口 UBXSkillManager::RebuildSkillFromProjection（反投影恢复 Task 指针后跳过 StartTimelineSections 直接入 SkillRTDatas 续跑）。
- 组件启用复制用 `SetIsReplicatedByDefault(true)`（UE5.4 的 bReplicates 是 private）；网络时间戳客户端侧用 `AGameStateBase::GetServerWorldTimeSeconds()`（Base 即有，勿用不存在的 GetServerWorldTimeDelta）。

## 工程经验
- 事件系统（BXEventManager）使用 TSet 替代 TArray 存储 GlobalTargetMap/SingleKeyMap/SingleTargetMap，提升注册/注销性能。
- UFunction 查找结果以 (Class, FunctionName) 为键缓存，减少反射开销。
- **staging 挂起区机制（Buff/Skill Manager）**：更新窗口（bUpdatingBuff/bUpdatingSkill 为 true 的 Tick 期间）内 Task 回调中的 AddBuff/PlaySkill 一律入 `PendingAddBuffs/PendingAddSkills`（TUniquePtr 容器，对象地址稳定），Tick 末 `MergePendingAddBuffs/MergePendingAddSkills` 合并入主容器（纯数据搬移无回调）。这是根治"TMap 扩容 rehash 使遍历中数据引用悬空"的机制——更新窗口内主容器结构完全冻结。**规则：① 所有按 ID 查询/变更必须走 GetBuffRuntimeDataByID/GetSkillRuntimeDataByID 双查入口（主容器 miss 后查挂起区），禁止直接 BuffRTDatas.Find/SkillRTDatas.Find（Tick 快照遍历除外）；② 新增 Manager 数据查询 API 必须双查；③ OwnerBuffMap/OwnerSkillMap 登记延迟到合并时（更新窗口内对其 Add 新键同样有 rehash 风险）**。TL 侧维持 bUpdatingTimeline 拒绝策略（PlayTimeline 返回 0）。
- BroadcastTasks（FBXTLSectionRTData）为网络多播功能预留：当前仅 AddPendingTask 收集不消费，接入网络多播前请勿移除该机制。
- UBXMiscManager 辅助线程模型：辅助线程仅计时，到期调用经 HTExpiredFunctions 无锁队列投递回游戏线程（Manager::Tick 消费执行 ProcessEvent）——UObject/蓝图反射体系非线程安全，禁止辅助线程直接 ProcessEvent；队列裸指针所有权约定：生产方 new 入队，消费方 Pop 后接管并 delete。
- 迭代器安全三条铁律：① UBXTProcessor::EndTask 内部会按地址从 RunningTasks 移除当前条目（RemoveAt 左移），外层循环禁止再 RemoveCurrent（会误删左移进来的相邻条目；末尾元素场景越界崩溃），遍历 RunningTasks 调 EndTask 必须用索引循环且移除后不推进索引；② TMap 容器在 Task/BP 回调执行期间可能被同步 Remove（如 StopTimeline/StopSkill），遍历必须用 ID 快照（先收集 Key 再 Find）；③ TArray 的 TIterator 为索引式，遍历中 Add/RemoveCurrent 自身安全，但 RemoveAt 会左移元素，索引循环需手动控制推进。
- 广播重入安全通过 BroadcastDepth 计数器与 PendingMutations 延迟变更队列实现。
- 使用 UID Generation 机制防止对象 UID 复用导致的 ABA 问题。
- **编辑器 UID 全局严格唯一（FBXTInputInfo/FBXTOutputInfo/FBXTTransformCreater）**：拷贝构造/拷贝赋值不携带 UID（新实例经 GetUniqueID 惰性分配新值），移动构造/移动赋值保留 UID（TArray 扩容搬迁不失身份，否则 RefreshTransformCreaters 配对断裂、BuildPinRelatedSignature 漂移）；任务复制链（CopyDataFromOther→RestoreTasksRelation）中 CopyDataFromOther 负责重置坐标系创建器 UID（CopyObject 属性搬运会拷入源任务 UID，经 SetUniqueID 注入新任务造成跨任务重复），RestoreTasksRelation 按源创建器 UID 重映射变换输入条目到目标创建器的新 UID（拷贝构造不携带 UID，不重映射则 PreSave 重建时条目被删除重建、DataTask/DataTag 连线丢失）；资产整体复制（内容浏览器 Ctrl+D/SaveAs 等 DuplicateObject 路径，序列化原样携带 UID）经 UBXTask::PostDuplicate（仅 EDuplicateMode::Normal，PIE 瞬态拷贝跳过）重分配：创建器先换新 UID 并记录旧→新映射，配对的变换输入条目按映射重配对（连线保持），其余 Input/Collision/Output 条目 ResetUniqueID 置 0 惰性分配全新值。
- 日志规范：清理频繁打印的正常流程日志，仅保留异常诊断（LogTemp Warning / 模块专属日志类别如 BXMGR_Buff、BXMGR_Timeline、BX_TP）。
- UE 5.4.2 注意事项：`CreateBelowWidgetControls`（非 CreateBelowNodeControls）、`AddDefaulted()`（非 AddDefaulted_Get）、呼吸动画用 `FPlatformTime::Seconds()`（非 FSlateApplication::GetTime()）。
