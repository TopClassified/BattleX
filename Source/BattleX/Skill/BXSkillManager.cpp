#include "BXSkillManager.h"

#include "BXSkillAsset.h"
#include "BXSkillComponent.h"
#include "BXSubSystem.h"
#include "BXSettings.h"
#include "BXFunctionLibrary.h"
#include "BXGameplayTags.h"
#include "BXTask.h"
#include "BXTProcessor.h"
#include "BXTLManager.h"
#include "BXTLAsset.h"
#include "BXTLStructs.h"
#include "BXTLEnums.h"
#include "BXConditionManager.h"
#include "BXTaskCondition.h"
#include "BXEventManager.h"
#include "BXTStructs.h"
#include "BXNetStructs.h"
#include "Behavior/BXBehaviorComponent.h"
#include "State/BXStateComponent.h"



DEFINE_LOG_CATEGORY(BXMGR_Skill);



#pragma region Important
UBXSkillManager* UBXSkillManager::Get(UObject* InWorldContext)
{
	if (!InWorldContext)
	{
		return nullptr;
	}

	if (UWorld* World = InWorldContext->GetWorld())
	{
		if (UGameInstance* GI = World->GetGameInstance())
		{
			if (UBXSubSystem* BXSS = GI->GetSubsystem<UBXSubSystem>())
			{
				return BXSS->GetManagerByClass<UBXSkillManager>();
			}
		}
	}

	return nullptr;
}

void UBXSkillManager::Initialize()
{
	Super::Initialize();

	UBXEventManager* EventMgr = UBXEventManager::Get(this);
	if (EventMgr)
	{
		EventMgr->DefineEvent(BXGameplayTags::BXEvent_Skill_Released, FBXTLEventParameter::StaticStruct());
		EventMgr->DefineEvent(BXGameplayTags::BXEvent_Skill_Finished, FBXTLEventParameter::StaticStruct());
		EventMgr->DefineEvent(BXGameplayTags::BXEvent_Skill_PredictSuccess, FBXTLEventParameter::StaticStruct());
		EventMgr->DefineEvent(BXGameplayTags::BXEvent_Skill_PredictFailed, FBXTLEventParameter::StaticStruct());
		EventMgr->DefineEvent(BXGameplayTags::BXEvent_Collision_Reported, FBXTLEventParameter::StaticStruct());
	}
}

void UBXSkillManager::Deinitialize()
{
	SkillRTDatas.Empty();
	OwnerSkillMap.Empty();

	Super::Deinitialize();
}

#pragma endregion Important



#pragma region Tick
UWorld* UBXSkillManager::GetTickableGameObjectWorld() const
{
	if (!GetOuter())
	{
		return nullptr;
	}

	if (UGameInstanceSubsystem* GIS = Cast<UGameInstanceSubsystem>(GetOuter()))
	{
		if (UGameInstance* GI = GIS->GetGameInstance())
		{
			return GI->GetWorld();
		}

		return nullptr;
	}

	return GetOuter()->GetWorld();
}

ETickableTickType UBXSkillManager::GetTickableTickType() const
{
	return IsTemplate() ? ETickableTickType::Never : ETickableTickType::Always;
}

bool UBXSkillManager::IsAllowedToTick() const
{
	return IsValid(this) && !IsUnreachable();
}

void UBXSkillManager::Tick(float DeltaTime)
{
	// 快照遍历:Task执行中可能同步释放新技能(连招链),避免TMap迭代器因扩容rehash失效
	TArray<int64> TickSkillIDs;
	TickSkillIDs.Reserve(SkillRTDatas.Num());
	for (const TPair<int64, FBXSkillRuntimeData>& Pair : SkillRTDatas)
	{
		TickSkillIDs.Add(Pair.Key);
	}

	// 更新窗口:期间Task回调中的PlaySkill入挂起区(直接Add触发TMap扩容rehash会令遍历中的InOutData引用悬空)
	bUpdatingSkill = true;

	for (int64 SkillID : TickSkillIDs)
	{
		FBXSkillRuntimeData* Data = SkillRTDatas.Find(SkillID);
		if (Data && !Data->bEarlyFinish)
		{
			InternalUpdateSkill(*Data, DeltaTime);
		}
	}

	CleanSkillTrash();

	CheckPredictTimeout();

	bUpdatingSkill = false;

	// 合并挂起的新增技能(合并点在全部遍历外,此处TMap扩容安全;staging条目下帧起参与更新,与同步路径下帧可见一致)
	MergePendingAddSkills();
}

void UBXSkillManager::InternalUpdateSkill(FBXSkillRuntimeData& InOutData, float InDeltaTime)
{
	UBXTLManager* TLMgr = UBXTLManager::Get(this);
	if (!TLMgr)
	{
		return;
	}

	// 更新加速状态,获取本帧等效推进Delta(含加速弥补与卡顿保护)
	float EffectiveDelta = UpdateAccelerate(InOutData, InDeltaTime);

	// Task执行交给UBXTLManager(完整时间轴更新管线)
	TLMgr->UpdateTimelineRunTimeData(InOutData.TLRunTimeData, EffectiveDelta);

	// 检查是否提前结束(非预测状态才检查)
	if (InOutData.PredictState != EBXPredictState::Predicting)
	{
		bool bNeedFinish = InOutData.TLRunTimeData.bEarlyFinish;
		if (!bNeedFinish)
		{
			bNeedFinish = true;
			for (int32 j = 0; j < InOutData.TLRunTimeData.RunningSections.Num(); ++j)
			{
				if (InOutData.TLRunTimeData.RunningSections[j].Index >= 0)
				{
					bNeedFinish = false;
					break;
				}
			}
		}

		if (bNeedFinish)
		{
			InOutData.bEarlyFinish = true;
			InOutData.FinishReason = EBXTLFinishReason::FR_EndOfLife;
		}
	}
}

void UBXSkillManager::CleanSkillTrash()
{
	// 先收集待清理ID:事件回调可能同步释放新技能(结束链招),避免TMap迭代器因扩容rehash失效
	TArray<int64> TrashIDs;
	for (const TPair<int64, FBXSkillRuntimeData>& Pair : SkillRTDatas)
	{
		if (Pair.Value.bEarlyFinish)
		{
			TrashIDs.Add(Pair.Key);
		}
	}

	for (int64 SkillID : TrashIDs)
	{
		FBXSkillRuntimeData TrashData;
		SkillRTDatas.RemoveAndCopyValue(SkillID, TrashData);

		if (IsValid(TrashData.TLRunTimeData.Owner))
		{
			OwnerSkillMap.FindOrAdd(TrashData.TLRunTimeData.Owner).Remove(SkillID);

			// ===== 技能-行为/状态收束(覆盖自然/中断/回滚全部FinishReason;回滚路径必须退出,否则客户端残留) =====
			if (UBXSkillAsset* TrashAsset = Cast<UBXSkillAsset>(TrashData.SkillAsset))
			{
				AActor* TrashOwner = TrashData.TLRunTimeData.Owner;

				if (UBXBehaviorComponent* BehaviorComp = TrashOwner->FindComponentByClass<UBXBehaviorComponent>())
				{
					// 姿态行为退出(全部来源中本技能的Sign)
					if (TrashAsset->BehaviorTag.IsValid())
					{
						BehaviorComp->StopBehavior(TrashAsset->BehaviorTag, SkillID);
					}

					// 取消窗口保护记录移除
					BehaviorComp->RemoveProtectionBySign(SkillID);
				}

				if (UBXStateComponent* StateComp = TrashOwner->FindComponentByClass<UBXStateComponent>())
				{
					// 退出原因映射(中断→被外部顶掉,预测失败→回滚,自然结束→手动)
					EBXStateEndReason StateExitReason = EBXStateEndReason::SER_Manual;
					switch (TrashData.FinishReason)
					{
					case EBXTLFinishReason::FR_Interrupt:
						StateExitReason = EBXStateEndReason::SER_External;
						break;
					case EBXTLFinishReason::FR_PredictionFailure:
						StateExitReason = EBXStateEndReason::SER_PredictRollback;
						break;
					default:
						break;
					}

					for (const FBXSkillEnterState& StateEntry : TrashAsset->EnterStates)
					{
						StateComp->ExitState(StateEntry.StateTag, SkillID, StateExitReason);
					}
				}
			}

			// 通知组件清理本地登记(OwnedSkillIDs/预测假冷却),否则自然结束的技能ID永久残留
			if (UBXSkillComponent* SkillComponent = TrashData.TLRunTimeData.Owner->FindComponentByClass<UBXSkillComponent>())
			{
				SkillComponent->InternalOnSkillFinished(SkillID);
			}
		}

		// 广播技能结束事件(自然结束/中断/回滚统一收束点,参数含结束原因)
		UE_LOG(BXMGR_Skill, Log, TEXT("UBXSkillManager::CleanSkillTrash: Skill finished. SkillID=%lld Reason=%d."), SkillID, (int32)TrashData.FinishReason);
		if (UBXEventManager* EventMgr = UBXEventManager::Get(this))
		{
			FBXTLEventParameter Param(SkillID, TrashData.FinishReason);
			EventMgr->BroadcastSingleEvent<FBXTLEventParameter>(BXGameplayTags::BXEvent_Skill_Finished, TrashData.TLRunTimeData.Owner, Param);
			EventMgr->BroadcastGlobalEvent<FBXTLEventParameter>(BXGameplayTags::BXEvent_Skill_Finished, Param);
		}
	}
}

void UBXSkillManager::MergePendingAddSkills()
{
	for (TUniquePtr<FBXSkillRuntimeData>& Pending : PendingAddSkills)
	{
		if (!Pending.IsValid())
		{
			continue;
		}

		// 更新中被停止的条目:不并入主容器,仅清理组件本地登记(与CleanSkillTrash对bEarlyFinish条目的处理对称;
		// Finished广播与MulticastStopSkill已在StopSkill/回滚路径执行过,此处禁止重复)
		if (Pending->bEarlyFinish)
		{
			if (AActor* Owner = Pending->TLRunTimeData.Owner)
			{
				if (UBXSkillComponent* SkillComponent = Owner->FindComponentByClass<UBXSkillComponent>())
				{
					SkillComponent->InternalOnSkillFinished(Pending->SkillID);
				}
			}

			continue;
		}

		// 纯数据搬移:OwnerSkillMap登记延迟至此(更新窗口内对其Add新键同样有rehash风险),合并点在全部遍历外扩容安全
		OwnerSkillMap.FindOrAdd(Pending->TLRunTimeData.Owner).Add(Pending->SkillID);
		SkillRTDatas.Add(Pending->SkillID, MoveTemp(*Pending));
	}

	PendingAddSkills.Reset();
}

void UBXSkillManager::CheckPredictTimeout()
{
	const UBXSettings* Settings = GetDefault<UBXSettings>();
	if (!Settings)
	{
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	float CurrentTime = World->GetTimeSeconds();

	// 先收集超时ID,循环外统一回滚:回滚会执行Task的End逻辑,可能同步释放新技能导致TMap扩容rehash
	TArray<int64> TimeoutSkillIDs;
	for (const TPair<int64, FBXSkillRuntimeData>& Pair : SkillRTDatas)
	{
		if (Pair.Value.PredictState != EBXPredictState::Predicting)
		{
			continue;
		}

		if (CurrentTime - Pair.Value.PredictStartTime >= Settings->SkillPredictMaxDuration)
		{
			TimeoutSkillIDs.Add(Pair.Key);
		}
	}

	for (int64 SkillID : TimeoutSkillIDs)
	{
		UE_LOG(BXMGR_Skill, Warning, TEXT("UBXSkillManager::CheckPredictTimeout: SkillID=%lld predict timeout, rolling back."), SkillID);
		RollbackPrediction(SkillID);
	}
}

#pragma endregion Tick



#pragma region Play
int64 UBXSkillManager::PlaySkill(UBXSkillAsset* InAsset, AActor* InOwner, const FBXSkillSyncPayload& InPayload)
{
	if (!InAsset || !InOwner)
	{
		UE_LOG(BXMGR_Skill, Warning, TEXT("UBXSkillManager::PlaySkill failed: InAsset=%s InOwner=%s."), InAsset ? TEXT("valid") : TEXT("null"), InOwner ? TEXT("valid") : TEXT("null"));
		return INDEX_NONE;
	}

	int64 NewID = UBXFunctionLibrary::GetUniqueID();
	NewID = BXMakeSyncID(NewID, EBXSyncInitiator::Server);

	return InternalPlaySkill(InAsset, InOwner, InPayload, NewID, EBXSyncInitiator::Server);
}

void UBXSkillManager::StopSkill(int64 InSkillID, EBXTLFinishReason InReason)
{
	// 双查(挂起区条目可被同帧停止:标记bEarlyFinish后合并时跳过入表)
	FBXSkillRuntimeData* Data = GetSkillRuntimeDataByID(InSkillID);
	if (!Data || Data->bEarlyFinish)
	{
		return;
	}

	StopSkillTasks(*Data, InReason);

	Data->bEarlyFinish = true;

	// 服务器主动中断时显式广播,远端客户端同步停止
	// (自然结束不广播:客户端实例同步推进会自行结束;预测回滚不广播:服务器无该实例)
	if (InReason == EBXTLFinishReason::FR_Interrupt)
	{
		AActor* Owner = Data->TLRunTimeData.Owner;
		if (Owner && Owner->GetNetMode() != NM_Client)
		{
			if (UBXSkillComponent* SkillComponent = Owner->FindComponentByClass<UBXSkillComponent>())
			{
				UE_LOG(BXMGR_Skill, Log, TEXT("UBXSkillManager::StopSkill: Broadcasting interrupt. SkillID=%lld."), InSkillID);
				SkillComponent->MulticastStopSkill(InSkillID, static_cast<uint8>(InReason));
			}
		}
	}
}

FBXSkillRuntimeData* UBXSkillManager::GetSkillRuntimeDataByID(int64 InID)
{
	if (FBXSkillRuntimeData* Data = SkillRTDatas.Find(InID))
	{
		return Data;
	}

	// 挂起区双查(更新窗口内新增的技能尚未合并入主容器,漏查会令同帧查询/去重守卫/预测状态写入失效)
	for (const TUniquePtr<FBXSkillRuntimeData>& Pending : PendingAddSkills)
	{
		if (Pending.IsValid() && Pending->SkillID == InID)
		{
			return Pending.Get();
		}
	}

	return nullptr;
}

int64 UBXSkillManager::RebuildSkillFromProjection(const FBXSkillReplicatedState& InState, AActor* InOwner)
{
	if (!InOwner)
	{
		return INDEX_NONE;
	}

	// 幂等:已存在(Multicast先到/本地预测,含挂起区)直接返回
	if (GetSkillRuntimeDataByID(InState.SkillID))
	{
		return InState.SkillID;
	}

	// 查找技能资产
	UBXTLManager* TLMgr = UBXTLManager::Get(this);
	if (!TLMgr)
	{
		return INDEX_NONE;
	}

	TSoftObjectPtr<UBXTLAsset> AssetPtr = TLMgr->GetTimelineAssetByID(InState.TLRunTimeData.TimelineID);
	UBXTLAsset* Asset = AssetPtr.Get();
	if (!Asset)
	{
		Asset = AssetPtr.LoadSynchronous();
	}

	UBXSkillAsset* SkillAsset = Cast<UBXSkillAsset>(Asset);
	if (!SkillAsset)
	{
		UE_LOG(BXMGR_Skill, Warning, TEXT("UBXSkillManager::RebuildSkillFromProjection: SkillAsset not found. TimelineID=%d SkillID=%lld."), InState.TLRunTimeData.TimelineID, InState.SkillID);
		return INDEX_NONE;
	}

	FBXSkillRuntimeData& Data = SkillRTDatas.Add(InState.SkillID);
	Data.SkillAsset = SkillAsset;
	Data.SkillID = InState.SkillID;
	Data.Initiator = InState.Initiator;

	// 反投影恢复运行数据(含Task指针按Index恢复),跳过StartTimelineSections直接续跑
	BXFromTLRunTimeProjection(InState.TLRunTimeData, SkillAsset, Data.TLRunTimeData);
	UE_LOG(BXMGR_Skill, Log, TEXT("UBXSkillManager::RebuildSkillFromProjection: Rebuilt. SkillID=%lld Asset=%s RunTime=%.2fs Sections=%d."), InState.SkillID, *SkillAsset->GetName(), Data.TLRunTimeData.RunTime, Data.TLRunTimeData.RunningSections.Num());

	// 记录Owner反向索引
	OwnerSkillMap.FindOrAdd(InOwner).Add(InState.SkillID);

	// 广播技能释放事件(与正常播放一致,本地系统可见)
	if (UBXEventManager* EventMgr = UBXEventManager::Get(this))
	{
		FBXTLEventParameter Param(InState.SkillID, EBXTLFinishReason::FR_TMax);
		EventMgr->BroadcastSingleEvent<FBXTLEventParameter>(BXGameplayTags::BXEvent_Skill_Released, InOwner, Param);
		EventMgr->BroadcastGlobalEvent<FBXTLEventParameter>(BXGameplayTags::BXEvent_Skill_Released, Param);
	}

	return InState.SkillID;
}

int64 UBXSkillManager::InternalPlaySkill(UBXSkillAsset* InAsset, AActor* InOwner, const FBXSkillSyncPayload& InPayload, int64 InSkillID, EBXSyncInitiator InInitiator)
{
	// const&版本:拷贝一份后委托给&&版本(拷贝开销与原逐元素Add等价)
	FBXSkillSyncPayload PayloadCopy = InPayload;
	return InternalPlaySkill(InAsset, InOwner, MoveTemp(PayloadCopy), InSkillID, InInitiator);
}

int64 UBXSkillManager::InternalPlaySkill(UBXSkillAsset* InAsset, AActor* InOwner, FBXSkillSyncPayload&& InPayload, int64 InSkillID, EBXSyncInitiator InInitiator)
{
	if (!InAsset || !InOwner)
	{
		return INDEX_NONE;
	}

	// ID去重守卫:重复ID会静默覆盖正在运行的技能(旧实例Task不走End,外部资源泄漏)
	// 双查:挂起区条目同样占用该ID
	if (GetSkillRuntimeDataByID(InSkillID))
	{
		UE_LOG(BXMGR_Skill, Warning, TEXT("UBXSkillManager::InternalPlaySkill: duplicate SkillID=%lld rejected."), InSkillID);
		return INDEX_NONE;
	}

	// 先在堆上完整构建(含Task启动):回调链中递归PlaySkill(连招链)时本对象地址稳定,
	// 且递归条目同样按时机入挂起区或主容器,均无TMap扩容风险
	TUniquePtr<FBXSkillRuntimeData> NewData = MakeUnique<FBXSkillRuntimeData>();

	NewData->SkillAsset = InAsset;
	NewData->SkillID = InSkillID;
	NewData->Initiator = InInitiator;
	NewData->ClientTimestamp = InPayload.ClientTimestamp;

	// 填充Timeline运行时数据
	NewData->TLRunTimeData.Timeline = InAsset;
	NewData->TLRunTimeData.TimelineID = InAsset->ID;
	NewData->TLRunTimeData.ID = InSkillID;
	NewData->TLRunTimeData.Owner = InOwner;
	NewData->TLRunTimeData.Instigator = InPayload.Instigator;
	NewData->TLRunTimeData.Triggerer = InPayload.Triggerer;

	// 所有InputDatas移动写入DynamicDatas(消除FInstancedStruct深拷贝)
	// LockParts为TLRunTimeData专用字段(Task消费方读InRTData.LockParts),解包后不进DynamicDatas
	for (FBXSkillInputEntry& Entry : InPayload.InputDatas)
	{
		if (Entry.Tag == BXGameplayTags::BXSkillInput_LockParts)
		{
			if (const FBXSkillLockParts* LockParts = Entry.Value.GetPtr<FBXSkillLockParts>())
			{
				NewData->TLRunTimeData.LockParts = LockParts->Parts;
			}
			continue;
		}

		NewData->TLRunTimeData.DynamicDatas.Add(FBXTLDynamicDataSearchKey(-1, Entry.Tag), MoveTemp(Entry.Value));
	}

	// ===== 技能-行为集成(五步链:判定->清场->登记行为->首帧Task) =====
	// 行为先行判决(CanStart只读无副作用:挂起/拒绝关系/Agent检查/挤出目标保护)
	// 此后清场与登记理论上必过,原子性缺口闭合(失败路径零残留)
	if (UBXBehaviorComponent* BehaviorComp = InOwner->FindComponentByClass<UBXBehaviorComponent>())
	{
		if (InAsset->BehaviorTag.IsValid())
		{
			FBXBehaviorStartCheck StartCheck;
			if (!BehaviorComp->CanStartBehavior(InAsset->BehaviorTag, StartCheck))
			{
				UE_LOG(BXMGR_Skill, Warning, TEXT("UBXSkillManager::InternalPlaySkill: behavior rejected(%s). SkillID=%lld Tag=%s"), *StartCheck.FailReason, InSkillID, *InAsset->BehaviorTag.ToString());
				return INDEX_NONE;
			}

			// 保护先置位再清场(清场目标不含自身;窗口内切换由SkillComponent边界管理)
			if (InAsset->bProtectedBehavior)
			{
				BehaviorComp->SetBehaviorProtection(InAsset->BehaviorTag, InSkillID, true);
			}

			// ③ 清场:挤出与姿态行为互斥的活跃行为(受保护目标上面CanStart已拦,此处幂等防御)
			if (!BehaviorComp->InterruptBehaviorsConflicting(InAsset->BehaviorTag))
			{
				UE_LOG(BXMGR_Skill, Warning, TEXT("UBXSkillManager::InternalPlaySkill: behavior conflict protected. SkillID=%lld Tag=%s"), InSkillID, *InAsset->BehaviorTag.ToString());
				BehaviorComp->RemoveProtectionBySign(InSkillID);
				return INDEX_NONE;
			}

			// ⑤a 登记姿态行为(先于首帧Task:首帧Task查询行为表时已含本条目,视图自洽)
			if (!BehaviorComp->StartBehavior(InAsset->BehaviorTag, InSkillID))
			{
				UE_LOG(BXMGR_Skill, Warning, TEXT("UBXSkillManager::InternalPlaySkill: behavior start failed. SkillID=%lld Tag=%s"), InSkillID, *InAsset->BehaviorTag.ToString());
				BehaviorComp->RemoveProtectionBySign(InSkillID);
				return INDEX_NONE;
			}
		}
	}

	// 启动Task(初始化RunningSections并触发首帧KeyFrame)
	StartSkillTasks(*NewData);

	// ===== 技能-状态集成:EnterStates登记(Sign=SkillID,技能结束收束退出) =====
	if (UBXStateComponent* StateComp = InOwner->FindComponentByClass<UBXStateComponent>())
	{
		for (const FBXSkillEnterState& StateEntry : InAsset->EnterStates)
		{
			// 进入失败(链深度超限/节点缺失)仅告警不中断释放:状态是技能的附加产物
			if (!StateComp->EnterState(StateEntry.StateTag, InSkillID, StateEntry.Duration))
			{
				UE_LOG(BXMGR_Skill, Warning, TEXT("UBXSkillManager::InternalPlaySkill: enter state failed. SkillID=%lld State=%s"), InSkillID, *StateEntry.StateTag.ToString());
			}
		}

		// 配置自检:技能进入的状态禁用了自身姿态行为(当帧互锁,通常为资产配置错误)
		if (InAsset->BehaviorTag.IsValid() && !InAsset->EnterStates.IsEmpty())
		{
			if (UBXBehaviorComponent* BehaviorComp2 = InOwner->FindComponentByClass<UBXBehaviorComponent>())
			{
				if (BehaviorComp2->IsBehaviorSuspended(InAsset->BehaviorTag))
				{
					UE_LOG(BXMGR_Skill, Warning, TEXT("UBXSkillManager::InternalPlaySkill: skill behavior forbidden by its own EnterStates (config conflict). SkillID=%lld Tag=%s"), InSkillID, *InAsset->BehaviorTag.ToString());
				}
			}
		}
	}

	if (bUpdatingSkill)
	{
		// 更新窗口内:入挂起区Tick末合并(直接Add会触发TMap扩容rehash,使更新管线遍历中的数据引用悬空)
		PendingAddSkills.Add(MoveTemp(NewData));
	}
	else
	{
		// 记录Owner反向索引(挂起区条目延迟到合并时登记)
		OwnerSkillMap.FindOrAdd(InOwner).Add(InSkillID);
		SkillRTDatas.Add(InSkillID, MoveTemp(*NewData));
	}

	// 广播技能释放事件
	if (UBXEventManager* EventMgr = UBXEventManager::Get(this))
	{
		FBXTLEventParameter Param(InSkillID, EBXTLFinishReason::FR_TMax);
		EventMgr->BroadcastSingleEvent<FBXTLEventParameter>(BXGameplayTags::BXEvent_Skill_Released, InOwner, Param);
		EventMgr->BroadcastGlobalEvent<FBXTLEventParameter>(BXGameplayTags::BXEvent_Skill_Released, Param);
	}

	return InSkillID;
}

#pragma endregion Play



#pragma region Predict
int64 UBXSkillManager::StartPrediction(UBXSkillAsset* InAsset, AActor* InOwner, const FBXSkillSyncPayload& InPayload)
{
	if (!InAsset || !InOwner)
	{
		return INDEX_NONE;
	}

	int64 NewID = UBXFunctionLibrary::GetUniqueID();
	NewID = BXMakeSyncID(NewID, EBXSyncInitiator::Client);

	int64 Result = InternalPlaySkill(InAsset, InOwner, InPayload, NewID, EBXSyncInitiator::Client);
	if (Result == INDEX_NONE)
	{
		return INDEX_NONE;
	}

	// 双查:预测技能可能处于挂起区(客户端Task回调中的连招链),漏查会令PredictState未设置导致预测超时机制失效
	FBXSkillRuntimeData* Data = GetSkillRuntimeDataByID(Result);
	if (Data)
	{
		Data->PredictState = EBXPredictState::Predicting;

		UWorld* World = GetWorld();
		if (World)
		{
			Data->PredictStartTime = World->GetTimeSeconds();
		}
	}

	return Result;
}

void UBXSkillManager::ConfirmPrediction(int64 InSkillID, int64 InServerTimestamp)
{
	FBXSkillRuntimeData* Data = GetSkillRuntimeDataByID(InSkillID);
	if (!Data)
	{
		UE_LOG(BXMGR_Skill, Warning, TEXT("UBXSkillManager::ConfirmPrediction: SkillID=%lld not found."), InSkillID);
		return;
	}

	if (Data->PredictState != EBXPredictState::Predicting)
	{
		return;
	}

	Data->PredictState = EBXPredictState::Confirmed;
	Data->ServerTimestamp = InServerTimestamp;

	// 广播预测成功事件
	if (UBXEventManager* EventMgr = UBXEventManager::Get(this))
	{
		FBXTLEventParameter Param(InSkillID, EBXTLFinishReason::FR_TMax);
		EventMgr->BroadcastSingleEvent<FBXTLEventParameter>(BXGameplayTags::BXEvent_Skill_PredictSuccess, Data->TLRunTimeData.Owner, Param);
		EventMgr->BroadcastGlobalEvent<FBXTLEventParameter>(BXGameplayTags::BXEvent_Skill_PredictSuccess, Param);
	}
}

void UBXSkillManager::RollbackPrediction(int64 InSkillID)
{
	// 双查(挂起区条目可能在本帧内被否认回滚)
	FBXSkillRuntimeData* Data = GetSkillRuntimeDataByID(InSkillID);
	if (!Data)
	{
		return;
	}

	if (Data->PredictState != EBXPredictState::Predicting)
	{
		return;
	}

	InternalRollbackPredictedSkill(*Data);
}

void UBXSkillManager::InternalRollbackPredictedSkill(FBXSkillRuntimeData& InOutData)
{
	InOutData.PredictState = EBXPredictState::RollingBack;
	UE_LOG(BXMGR_Skill, Log, TEXT("UBXSkillManager::InternalRollbackPredictedSkill: Rolling back. SkillID=%lld."), InOutData.SkillID);

	// 停止所有Task,使用预测失败原因
	StopSkillTasks(InOutData, EBXTLFinishReason::FR_PredictionFailure);

	InOutData.bEarlyFinish = true;

	// 广播预测失败事件
	if (UBXEventManager* EventMgr = UBXEventManager::Get(this))
	{
		FBXTLEventParameter Param(InOutData.SkillID, EBXTLFinishReason::FR_PredictionFailure);
		EventMgr->BroadcastSingleEvent<FBXTLEventParameter>(BXGameplayTags::BXEvent_Skill_PredictFailed, InOutData.TLRunTimeData.Owner, Param);
		EventMgr->BroadcastGlobalEvent<FBXTLEventParameter>(BXGameplayTags::BXEvent_Skill_PredictFailed, Param);
	}
}

#pragma endregion Predict



#pragma region Accelerate
void UBXSkillManager::ServerAccelerate(FBXSkillRuntimeData& InOutData)
{
	const UBXSettings* Settings = GetDefault<UBXSettings>();
	if (!Settings || !Settings->bServerAccelerateOnReceive)
	{
		return;
	}

	if (InOutData.ClientTimestamp <= 0 || InOutData.ServerTimestamp <= 0)
	{
		return;
	}

	// 服务器世界时间域差值 = RPC上行延迟(含客户端时间同步误差),负值视为同步误差丢弃
	float DeltaSeconds = FMath::Max(0.0f, static_cast<float>(InOutData.ServerTimestamp - InOutData.ClientTimestamp) / 1000.0f);
	if (DeltaSeconds <= 0.0f)
	{
		return;
	}

	float AccelerateDuration = DeltaSeconds / Settings->ServerAccelerateRate;
	AccelerateDuration = ClampAccelerateDuration(InOutData, AccelerateDuration);
	if (AccelerateDuration <= 0.0f)
	{
		return;
	}

	UBXTLManager* TLMgr = UBXTLManager::Get(this);
	if (TLMgr)
	{
		TLMgr->ChangeTimelineRunTimeDataTickRate(InOutData.TLRunTimeData, Settings->ServerAccelerateRate);
	}

	InOutData.AccelerateRemainTime = AccelerateDuration;
	InOutData.AccelerateRate = Settings->ServerAccelerateRate;
	UE_LOG(BXMGR_Skill, Log, TEXT("UBXSkillManager::ServerAccelerate: SkillID=%lld Delay=%.0fms Rate=%.1f Duration=%.2fs."), InOutData.SkillID, DeltaSeconds * 1000.0f, Settings->ServerAccelerateRate, AccelerateDuration);
}

void UBXSkillManager::ClientAccelerate(FBXSkillRuntimeData& InOutData, int64 InBroadcastTimestamp)
{
	const UBXSettings* Settings = GetDefault<UBXSettings>();
	if (!Settings || !Settings->bClientAccelerateOnBroadcast)
	{
		return;
	}

	if (InBroadcastTimestamp <= 0)
	{
		return;
	}

	// 服务器世界时间域差值 = 广播传播延迟(含本机时间同步误差),负值视为同步误差丢弃
	int64 LocalServerTime = UBXFunctionLibrary::GetServerWorldTimeMilliseconds(this);
	float DeltaSeconds = FMath::Max(0.0f, static_cast<float>(LocalServerTime - InBroadcastTimestamp) / 1000.0f);
	if (DeltaSeconds <= 0.0f)
	{
		return;
	}

	float AccelerateDuration = DeltaSeconds / Settings->ClientAccelerateRate;
	AccelerateDuration = ClampAccelerateDuration(InOutData, AccelerateDuration);
	if (AccelerateDuration <= 0.0f)
	{
		return;
	}

	UBXTLManager* TLMgr = UBXTLManager::Get(this);
	if (TLMgr)
	{
		TLMgr->ChangeTimelineRunTimeDataTickRate(InOutData.TLRunTimeData, Settings->ClientAccelerateRate);
	}

	InOutData.AccelerateRemainTime = AccelerateDuration;
	InOutData.AccelerateRate = Settings->ClientAccelerateRate;
	UE_LOG(BXMGR_Skill, Log, TEXT("UBXSkillManager::ClientAccelerate: SkillID=%lld Delay=%.0fms Rate=%.1f Duration=%.2fs."), InOutData.SkillID, DeltaSeconds * 1000.0f, Settings->ClientAccelerateRate, AccelerateDuration);
}

float UBXSkillManager::ClampAccelerateDuration(const FBXSkillRuntimeData& InData, float InDuration)
{
	// 钳制加速时长上限为技能总时长的50%,避免长时间偏差导致技能长期倍速
	// 含无限循环片段(LoopTime<=0)的技能总时长无界,不参与钳制(否则退化为准入即拒,加速补偿完全失效)
	if (InData.SkillAsset && InData.SkillAsset->Sections.Num() > 0)
	{
		float TotalDuration = 0.0f;
		for (const FBXTLSection& Section : InData.SkillAsset->Sections)
		{
			if (Section.LoopTime <= 0)
			{
				return InDuration;
			}

			TotalDuration += Section.Duration * Section.LoopTime;
		}

		InDuration = FMath::Min(InDuration, TotalDuration * 0.5f);
	}

	return InDuration;
}

float UBXSkillManager::UpdateAccelerate(FBXSkillRuntimeData& InOutData, float InDeltaTime)
{
	if (InOutData.AccelerateRemainTime <= 0.0f)
	{
		return InDeltaTime;
	}

	float RunRate = InOutData.AccelerateRate;

	// 本帧加速部分时间(不超过剩余加速时长)
	float AcceleratedTime = FMath::Min(InDeltaTime, InOutData.AccelerateRemainTime);
	float NormalTime = InDeltaTime - AcceleratedTime;

	// 消耗加速时间
	InOutData.AccelerateRemainTime -= AcceleratedTime;
	if (InOutData.AccelerateRemainTime <= 0.0f)
	{
		InOutData.AccelerateRemainTime = 0.0f;

		// 加速结束,恢复正常速率
		UBXTLManager* TLMgr = UBXTLManager::Get(this);
		if (TLMgr)
		{
			TLMgr->ChangeTimelineRunTimeDataTickRate(InOutData.TLRunTimeData, 1.0f);
		}

		// RunRate已恢复1.0,返回等效Delta(加速部分按倍率折算+正常部分)
		return AcceleratedTime * RunRate + NormalTime;
	}

	// 仍在加速中,RunRate保持不变,返回原始Delta(由InternalUpdateTimeline中RunRate放大)
	return InDeltaTime;
}

#pragma endregion Accelerate



#pragma region Validate
bool UBXSkillManager::ServerValidateRelease(UBXSkillAsset* InAsset, AActor* InOwner, const FBXSkillSyncPayload& InPayload, int64 InClientTimestamp)
{
	if (!InAsset || !InOwner)
	{
		return false;
	}

	const UBXSettings* Settings = GetDefault<UBXSettings>();

	// 时间戳校验(服务器世界时间域,依赖引擎时间同步,不依赖各机器墙钟)
	if (InClientTimestamp <= 0)
	{
		UE_LOG(BXMGR_Skill, Warning, TEXT("UBXSkillManager::ServerValidateRelease: invalid ClientTimestamp(0), SkillAsset=%s."), *InAsset->GetName());
		return false;
	}

	int64 ServerTimestamp = UBXFunctionLibrary::GetServerWorldTimeMilliseconds(this);
	int64 RequestAge = ServerTimestamp - InClientTimestamp;

	// 时间戳双向校验:过老拒绝(重放);明显超前拒绝(伪造);小幅偏差视为客户端时间同步误差放行
	if (Settings)
	{
		const int64 MaxAge = Settings->SkillRequestMaxAgeMs;
		if (RequestAge > MaxAge || RequestAge < -MaxAge)
		{
			UE_LOG(BXMGR_Skill, Warning, TEXT("UBXSkillManager::ServerValidateRelease: Request age out of range(%lld ms, MaxAge=%lld). Server=%lld Client=%lld."), RequestAge, MaxAge, ServerTimestamp, InClientTimestamp);
			return false;
		}
	}

	// 释放条件校验
	UBXConditionManager* CondMgr = UBXConditionManager::Get(this);
	if (CondMgr && InAsset->ReleaseConditions.Num() > 0)
	{
		FBXTaskConditionParameter Param;
		Param.Owner = InOwner;
		Param.Instigator = InPayload.Instigator;

		for (UBXTaskCondition* Condition : InAsset->ReleaseConditions)
		{
			if (!Condition)
			{
				continue;
			}

			if (!CondMgr->CheckCondition(Condition, Param))
			{
				UE_LOG(BXMGR_Skill, Warning, TEXT("UBXSkillManager::ServerValidateRelease: Condition=%s failed. SkillAsset=%s."), *Condition->GetName(), *InAsset->GetName());
				return false;
			}
		}
	}

	return true;
}

#pragma endregion Validate



#pragma region Collision
void UBXSkillManager::ReceiveCollisionResults(int64 InSkillID, int32 InTaskFullIndex, const FGameplayTag& InDataTag, const FBXTHitResults& InResults)
{
	// 服务器开启自身碰撞检测时忽略客户端上报(服务器权威,防止重复结果)
	const UBXSettings* Settings = GetDefault<UBXSettings>();
	if (Settings && Settings->bServerCollisionCheck)
	{
		return;
	}

	// 双查(目标技能可能处于挂起区)
	FBXSkillRuntimeData* Data = GetSkillRuntimeDataByID(InSkillID);
	if (!Data)
	{
		UE_LOG(BXMGR_Skill, Warning, TEXT("UBXSkillManager::ReceiveCollisionResults: SkillID=%lld not found (ended or never existed)."), InSkillID);
		return;
	}

	UE_LOG(BXMGR_Skill, Log, TEXT("UBXSkillManager::ReceiveCollisionResults: Received. SkillID=%lld TaskFullIndex=%d Hits=%d."), InSkillID, InTaskFullIndex, InResults.Results.Num());

	// 找到FullIndex匹配的碰撞Task:清除等待标记,以服务器侧作用域双键写入碰撞数据并触发Success事件
	// (与客户端CollisionCheck行为一致:反应分支Task在服务器侧启动,事件作用域读取键可达,实现由客户端结果驱动服务器侧流程)
	UBXTLAsset* Asset = Data->TLRunTimeData.Timeline;
	bool bTaskFound = false;
	for (FBXTLSectionRTData& SectionRT : Data->TLRunTimeData.RunningSections)
	{
		if (!Asset || !Asset->Sections.IsValidIndex(SectionRT.Index))
		{
			continue;
		}

		const FBXTLSection& Section = Asset->Sections[SectionRT.Index];
		for (FBXTLTaskRTData& TaskRT : SectionRT.RunningTasks)
		{
			if (!Section.TaskList.IsValidIndex(TaskRT.Index))
			{
				continue;
			}

			UBXTask* Task = Section.TaskList[TaskRT.Index];
			if (!Task || UBXFunctionLibrary::GetTaskFullIndex(Asset, Task) != InTaskFullIndex)
			{
				continue;
			}

			TaskRT.bAwaitingClientCollision = false;
			TaskRT.ServerExtraLifeTimer = 0.0f;

			// 首个匹配实例生成作用域,双键写入(无作用域键+作用域键)并触发Success
			if (!bTaskFound)
			{
				bTaskFound = true;
				int64 Scope = UBXTProcessor::GenerateContextScope(Data->TLRunTimeData, TaskRT);
				UBXTProcessor::WriteContextData<FBXTHitResults>(Data->TLRunTimeData, InTaskFullIndex, InDataTag, Scope, InResults);
				UBXTProcessor::AddPendingTask(Data->TLRunTimeData, SectionRT, TaskRT, Scope, BXGameplayTags::BXTEvent_Success);
			}
		}
	}

	// 未找到匹配Task(Task已结束):仍写入无作用域键,供后续时间片段按索引读取兜底
	if (!bTaskFound)
	{
		Data->TLRunTimeData.DynamicDatas.Add(FBXTLDynamicDataSearchKey(InTaskFullIndex, InDataTag), FInstancedStruct::Make(InResults));
	}

	// 广播碰撞结果上报事件
	if (UBXEventManager* EventMgr = UBXEventManager::Get(this))
	{
		FBXTLEventParameter Param(InSkillID, EBXTLFinishReason::FR_TMax);
		EventMgr->BroadcastSingleEvent<FBXTLEventParameter>(BXGameplayTags::BXEvent_Collision_Reported, Data->TLRunTimeData.Owner, Param);
		EventMgr->BroadcastGlobalEvent<FBXTLEventParameter>(BXGameplayTags::BXEvent_Collision_Reported, Param);
	}
}

#pragma endregion Collision



#pragma region TaskBridge
void UBXSkillManager::StartSkillTasks(FBXSkillRuntimeData& InOutData)
{
	UBXTLManager* TLMgr = UBXTLManager::Get(this);
	if (TLMgr)
	{
		TLMgr->StartTimelineSections(InOutData.TLRunTimeData);
	}
}

void UBXSkillManager::StopSkillTasks(FBXSkillRuntimeData& InOutData, EBXTLFinishReason InReason)
{
	// 记录结束原因(BXEvent.Skill.Finished事件携带)
	InOutData.FinishReason = InReason;

	UBXTLManager* TLMgr = UBXTLManager::Get(this);
	if (!TLMgr)
	{
		return;
	}

	// 结束所有正在运行的Section
	for (FBXTLSectionRTData& SectionRT : InOutData.TLRunTimeData.RunningSections)
	{
		if (SectionRT.Index >= 0)
		{
			TLMgr->FinishTimelineSection(InOutData.TLRunTimeData, SectionRT, InReason);
		}
	}

	InOutData.TLRunTimeData.RunningSections.Empty();
}

#pragma endregion TaskBridge
