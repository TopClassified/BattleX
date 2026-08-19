#include "BXBuffManager.h"

#include "BXBuffAsset.h"
#include "BXBuffComponent.h"
#include "BXSubSystem.h"
#include "BXFunctionLibrary.h"
#include "BXGameplayTags.h"
#include "BXTask.h"
#include "BXTProcessor.h"
#include "BXTLManager.h"
#include "BXEventManager.h"



DEFINE_LOG_CATEGORY(BXMGR_Buff);



// 服务器端获取Owner的BUFF组件(客户端不维护复制快照)
static UBXBuffComponent* GetOwnerBuffComponent(AActor* InOwner)
{
	if (!InOwner || InOwner->GetNetMode() == NM_Client)
	{
		return nullptr;
	}

	return InOwner->FindComponentByClass<UBXBuffComponent>();
}



#pragma region Important
UBXBuffManager* UBXBuffManager::Get(UObject* InWorldContext)
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
				return BXSS->GetManagerByClass<UBXBuffManager>();
			}
		}
	}

	return nullptr;
}

void UBXBuffManager::Initialize()
{
	Super::Initialize();

	UBXEventManager* EventMgr = UBXEventManager::Get(this);
	if (EventMgr)
	{
		EventMgr->DefineEvent(BXGameplayTags::BXEvent_Buff_Added, FBXEventBuffChanged::StaticStruct());
		EventMgr->DefineEvent(BXGameplayTags::BXEvent_Buff_Removed, FBXEventBuffChanged::StaticStruct());
		EventMgr->DefineEvent(BXGameplayTags::BXEvent_Buff_LayerChanged, FBXEventBuffChanged::StaticStruct());
		EventMgr->DefineEvent(BXGameplayTags::BXEvent_Buff_LevelChanged, FBXEventBuffChanged::StaticStruct());
		EventMgr->DefineEvent(BXGameplayTags::BXEvent_Buff_LifetimeRefreshed, FBXEventBuffChanged::StaticStruct());
	}
}

void UBXBuffManager::Deinitialize()
{
	BuffRTDatas.Empty();
	OwnerBuffMap.Empty();

	Super::Deinitialize();
}

#pragma endregion Important



#pragma region Tick
void UBXBuffManager::Tick(float DeltaTime)
{
	// 快照遍历:Task执行/移除广播回调可能同步添加BUFF(BUFF链),避免TMap迭代器因扩容rehash失效
	TArray<int64> TickBuffIDs;
	TickBuffIDs.Reserve(BuffRTDatas.Num());
	for (const TPair<int64, FBXBuffRuntimeData>& Pair : BuffRTDatas)
	{
		TickBuffIDs.Add(Pair.Key);
	}

	// 更新窗口:期间Task回调中的AddBuff入挂起区(直接Add触发TMap扩容rehash会令遍历中的InOutData引用悬空)
	bUpdatingBuff = true;

	for (int64 BuffID : TickBuffIDs)
	{
		FBXBuffRuntimeData* Data = BuffRTDatas.Find(BuffID);
		if (Data && !Data->bEarlyFinish)
		{
			InternalUpdateBuff(*Data, DeltaTime);
		}
	}

	CleanBuffTrash();

	bUpdatingBuff = false;

	// 合并挂起的新增BUFF(合并点在全部遍历外,此处TMap扩容安全;staging条目下帧起参与更新,与同步路径下帧可见一致)
	MergePendingAddBuffs();
}

void UBXBuffManager::InternalUpdateBuff(FBXBuffRuntimeData& InOutData, float InDeltaTime)
{
	UBXTLManager* TLMgr = UBXTLManager::Get(this);
	if (!TLMgr)
	{
		return;
	}

	if (InOutData.TLRunTimeData.RunningSections.Num() == 0)
	{
		return;
	}

	FBXTLRunTimeData& TLData = InOutData.TLRunTimeData;
	FBXTLSectionRTData& SectionRT = TLData.RunningSections[0];

	// Task执行交给UBXTLManager
	TLMgr->UpdateTimelineSectionTasks(TLData, SectionRT, InDeltaTime);

	// 事件链处理交给UBXTLManager
	TLMgr->ProcessTimelineSectionPendingTasks(TLData, SectionRT);

	// BUFF特有逻辑:生命时长推进
	UBXBuffAsset* Asset = InOutData.BuffAsset;
	if (!Asset)
	{
		return;
	}

	if (Asset->LifeType == EBXBuffLifeType::BL_Infinite)
	{
		return;
	}

	if (Asset->LifeType == EBXBuffLifeType::BL_Manual)
	{
		return;
	}

	if (Asset->LayerLifeMode == EBXBuffLayerLifeMode::BLL_Shared)
	{
		InOutData.RunTime += InDeltaTime;

		if (InOutData.RunTime >= Asset->BuffDuration)
		{
			TLMgr->FinishTimelineSection(TLData, SectionRT, EBXTLFinishReason::FR_EndOfLife);
			InternalRemoveBuff(InOutData, EBXBuffRemoveReason::BRR_Expired);
		}
	}
	else
	{
		for (int32 i = InOutData.LayerRunTimes.Num() - 1; i >= 0; --i)
		{
			InOutData.LayerRunTimes[i] += InDeltaTime;

			if (InOutData.LayerRunTimes[i] >= Asset->BuffDuration)
			{
				InOutData.LayerRunTimes.RemoveAt(i);

				int32 OldLayer = InOutData.CurrentLayer;
				InOutData.CurrentLayer = InOutData.LayerRunTimes.Num();

				InternalRefreshBuffTasksByLayer(InOutData, OldLayer, InOutData.CurrentLayer);

				// 服务器刷新复制快照(层级变化仅经复制通道同步,与ChangeBuffLayer对称,否则客户端层数/到期时刻持续陈旧)
				if (UBXBuffComponent* BuffComponent = GetOwnerBuffComponent(InOutData.TLRunTimeData.Owner))
				{
					BuffComponent->UpdateBuffReplicatedState(InOutData.BuffID);
				}

				FBXEventBuffChanged Param;
				Param.OldLayer = OldLayer;
				Param.NewLayer = InOutData.CurrentLayer;
				BroadcastBuffEvent(InOutData, BXGameplayTags::BXEvent_Buff_LayerChanged, Param);

				// 回调中BP可能同步移除BUFF(清空LayerRunTimes)或改层,每轮复查索引防越界
				if (!BuffRTDatas.Contains(InOutData.BuffID) || i > InOutData.LayerRunTimes.Num())
				{
					return;
				}
			}
		}

		if (InOutData.CurrentLayer <= 0)
		{
			TLMgr->FinishTimelineSection(TLData, SectionRT, EBXTLFinishReason::FR_EndOfLife);
			InternalRemoveBuff(InOutData, EBXBuffRemoveReason::BRR_Expired);
		}
	}
}

void UBXBuffManager::CleanBuffTrash()
{
	TArray<int64> TrashIDs;

	for (TMap<int64, FBXBuffRuntimeData>::TIterator It(BuffRTDatas); It; ++It)
	{
		if (It->Value.bEarlyFinish)
		{
			TrashIDs.Add(It->Key);
		}
	}

	for (int64 TrashID : TrashIDs)
	{
		FBXBuffRuntimeData TrashData;
		BuffRTDatas.RemoveAndCopyValue(TrashID, TrashData);

		if (AActor* Owner = TrashData.TLRunTimeData.Owner)
		{
			if (TArray<int64>* IDs = OwnerBuffMap.Find(Owner))
			{
				IDs->Remove(TrashID);
				if (IDs->Num() == 0)
				{
					OwnerBuffMap.Remove(Owner);
				}
			}

			// 通知组件清理本地登记(OwnedBuffIDs),否则自然到期/耗尽的BUFF ID永久残留
			if (UBXBuffComponent* BuffComponent = Owner->FindComponentByClass<UBXBuffComponent>())
			{
				BuffComponent->InternalOnBuffFinished(TrashID);
			}
		}
	}
}

#pragma endregion Tick



#pragma region Buff
int64 UBXBuffManager::AddBuff(UBXBuffAsset* InAsset, AActor* InOwner, FBXBuffPlayContext& InContext)
{
	if (!InAsset || !InOwner)
	{
		UE_LOG(BXMGR_Buff, Warning, TEXT("UBXBuffManager::AddBuff failed: InAsset=%s InOwner=%s."), InAsset ? TEXT("valid") : TEXT("null"), InOwner ? TEXT("valid") : TEXT("null"));
		return INDEX_NONE;
	}

	// 入口统一钳制(本函数为BlueprintCallable,BP直调可绕过组件/函数库路径的ServerValidateAddBuff,
	// 负数InitLayer会令LayerRunTimes.Init以负数长度调用触发TArray check崩溃)
	InContext.InitLayer = FMath::Clamp(InContext.InitLayer, 1, InAsset->MaxLayer);
	InContext.InitLevel = FMath::Clamp(InContext.InitLevel, 1, InAsset->MaxLevel);

	// 共存策略处理
	if (InAsset->CoexistPolicy == EBXBuffCoexistPolicy::BC_Coexist)
	{
		if (FBXBuffRuntimeData* Existing = FindExistingBuff(InOwner, InAsset, InContext.Instigator))
		{
			// 层级按增量叠加,等级按差值对齐(与Replace分支行为对称)
			ChangeBuffLayer(Existing->BuffID, InContext.InitLayer);
			ChangeBuffLevel(Existing->BuffID, InContext.InitLevel - Existing->CurrentLevel);

			if (InAsset->bRefreshLifetimeOnAdd)
			{
				RefreshBuffLifetime(Existing->BuffID);
			}

			return Existing->BuffID;
		}
	}
	else
	{
		if (FBXBuffRuntimeData* Best = FindBestBuffToReplace(InOwner, InAsset))
		{
			Best->TLRunTimeData.Instigator = InContext.Instigator;
			Best->TLRunTimeData.Triggerer = InContext.Triggerer;

			ChangeBuffLayer(Best->BuffID, InContext.InitLayer);
			ChangeBuffLevel(Best->BuffID, InContext.InitLevel - Best->CurrentLevel);

			return Best->BuffID;
		}
	}

	// 创建新运行时数据
	int64 NewID = UBXFunctionLibrary::GetUniqueID();

	// 构建并激活(更新窗口内自动入挂起区)
	InternalBuildNewBuff(InAsset, InOwner, InContext, NewID);

	// 服务器维护复制快照(新连接/新相关客户端重建用,须在InternalAddBuff填充层级后登记)
	if (UBXBuffComponent* BuffComponent = GetOwnerBuffComponent(InOwner))
	{
		BuffComponent->AddBuffReplicatedState(NewID);
	}

	UE_LOG(BXMGR_Buff, Log, TEXT("UBXBuffManager::AddBuff: BuffID=%lld Asset=%s Layer=%d Level=%d."), NewID, *InAsset->GetName(), InContext.InitLayer, InContext.InitLevel);
	return NewID;
}

int64 UBXBuffManager::AddBuffWithID(UBXBuffAsset* InAsset, AActor* InOwner, const FBXBuffPlayContext& InContext, int64 InBuffID)
{
	if (!InAsset || !InOwner || InBuffID == INDEX_NONE)
	{
		return INDEX_NONE;
	}

	// 如果ID已存在(含挂起区),直接返回
	if (GetBuffRuntimeDataByID(InBuffID))
	{
		return InBuffID;
	}

	// 构建并激活(更新窗口内自动入挂起区)
	InternalBuildNewBuff(InAsset, InOwner, InContext, InBuffID);

	return InBuffID;
}

bool UBXBuffManager::ServerValidateAddBuff(UBXBuffAsset* InAsset, AActor* InOwner, const FBXBuffPlayContext& InContext, FBXBuffPlayContext& OutContext)
{
	if (!InAsset || !InOwner)
	{
		return false;
	}

	// 钳制初始层级/等级到资产范围(不信任客户端传入值,越界InitLayer会撑爆LayerRunTimes数组)
	OutContext = InContext;
	OutContext.InitLayer = FMath::Clamp(InContext.InitLayer, 1, InAsset->MaxLayer);
	OutContext.InitLevel = FMath::Clamp(InContext.InitLevel, 1, InAsset->MaxLevel);
	return true;
}

void UBXBuffManager::RemoveBuff(int64 InID, int32 InLayerDelta)
{
	// 双查(挂起区条目同样可被移除:标记bEarlyFinish后合并时跳过入表)
	FBXBuffRuntimeData* Data = GetBuffRuntimeDataByID(InID);
	if (!Data || Data->bEarlyFinish)
	{
		return;
	}

	if (InLayerDelta > 0 && Data->BuffAsset->LayerLifeMode == EBXBuffLayerLifeMode::BLL_Independent)
	{
		int32 LayersToRemove = FMath::Min(InLayerDelta, Data->CurrentLayer);

		for (int32 i = 0; i < LayersToRemove; ++i)
		{
			if (Data->LayerRunTimes.Num() > 0)
			{
				Data->LayerRunTimes.RemoveAt(Data->LayerRunTimes.Num() - 1);
			}
		}

		const int64 BuffID = InID;
		int32 OldLayer = Data->CurrentLayer;
		Data->CurrentLayer = Data->LayerRunTimes.Num();

		if (Data->CurrentLayer <= 0)
		{
			InternalRemoveBuff(*Data, EBXBuffRemoveReason::BRR_Manual);
		}
		else
		{
			InternalRefreshBuffTasksByLayer(*Data, OldLayer, Data->CurrentLayer);

			// 手动按层移除与到期/ChangeBuffLayer路径对称:刷新复制快照并广播层级变化事件,否则客户端层数脱节且监听者无法感知
			// (回调后重查走双查:目标条目可能在挂起区)
			if (FBXBuffRuntimeData* RefoundData = GetBuffRuntimeDataByID(BuffID))
			{
				if (UBXBuffComponent* BuffComponent = GetOwnerBuffComponent(RefoundData->TLRunTimeData.Owner))
				{
					BuffComponent->UpdateBuffReplicatedState(BuffID);
				}

				FBXEventBuffChanged Param;
				Param.OldLayer = OldLayer;
				Param.NewLayer = RefoundData->CurrentLayer;
				BroadcastBuffEvent(*RefoundData, BXGameplayTags::BXEvent_Buff_LayerChanged, Param);
			}
		}
	}
	else
	{
		InternalRemoveBuff(*Data, EBXBuffRemoveReason::BRR_Manual);
	}
}

void UBXBuffManager::RemoveBuffWithReason(int64 InID, EBXBuffRemoveReason InReason)
{
	// 双查(同RemoveBuff)
	FBXBuffRuntimeData* Data = GetBuffRuntimeDataByID(InID);
	if (!Data || Data->bEarlyFinish)
	{
		return;
	}

	InternalRemoveBuff(*Data, InReason);
}

void UBXBuffManager::ChangeBuffLayer(int64 InID, int32 InLayerDelta)
{
	// 双查(挂起区条目可被同帧改层)
	FBXBuffRuntimeData* Data = GetBuffRuntimeDataByID(InID);
	if (!Data || Data->bEarlyFinish)
	{
		return;
	}

	UBXBuffAsset* Asset = Data->BuffAsset;
	if (!Asset)
	{
		return;
	}

	int32 OldLayer = Data->CurrentLayer;
	int32 NewLayer = FMath::Clamp(OldLayer + InLayerDelta, 1, Asset->MaxLayer);

	if (NewLayer == OldLayer)
	{
		return;
	}

	Data->CurrentLayer = NewLayer;

	// 独立层级模式:同步LayerRunTimes数组
	if (Asset->LayerLifeMode == EBXBuffLayerLifeMode::BLL_Independent)
	{
		if (NewLayer > OldLayer)
		{
			for (int32 i = OldLayer; i < NewLayer; ++i)
			{
				Data->LayerRunTimes.Add(0.0f);
			}
		}
		else
		{
			for (int32 i = OldLayer; i > NewLayer; --i)
			{
				if (Data->LayerRunTimes.Num() > 0)
				{
					Data->LayerRunTimes.RemoveAt(Data->LayerRunTimes.Num() - 1);
				}
			}
		}
	}

	InternalRefreshBuffTasksByLayer(*Data, OldLayer, NewLayer);

	// 服务器刷新复制快照(层数与各层到期时刻变化)
	if (UBXBuffComponent* BuffComponent = GetOwnerBuffComponent(Data->TLRunTimeData.Owner))
	{
		BuffComponent->UpdateBuffReplicatedState(InID);
	}

	FBXEventBuffChanged Param;
	Param.OldLayer = OldLayer;
	Param.NewLayer = NewLayer;
	BroadcastBuffEvent(*Data, BXGameplayTags::BXEvent_Buff_LayerChanged, Param);
}

void UBXBuffManager::ChangeBuffLevel(int64 InID, int32 InLevelDelta)
{
	// 双查(同ChangeBuffLayer)
	FBXBuffRuntimeData* Data = GetBuffRuntimeDataByID(InID);
	if (!Data || Data->bEarlyFinish)
	{
		return;
	}

	UBXBuffAsset* Asset = Data->BuffAsset;
	if (!Asset)
	{
		return;
	}

	int32 OldLevel = Data->CurrentLevel;
	int32 NewLevel = FMath::Clamp(OldLevel + InLevelDelta, 1, Asset->MaxLevel);

	if (NewLevel == OldLevel)
	{
		return;
	}

	Data->CurrentLevel = NewLevel;

	// 服务器刷新复制快照(等级变化)
	if (UBXBuffComponent* BuffComponent = GetOwnerBuffComponent(Data->TLRunTimeData.Owner))
	{
		BuffComponent->UpdateBuffReplicatedState(InID);
	}

	FBXEventBuffChanged Param;
	Param.OldLevel = OldLevel;
	Param.NewLevel = NewLevel;
	BroadcastBuffEvent(*Data, BXGameplayTags::BXEvent_Buff_LevelChanged, Param);
}

void UBXBuffManager::RefreshBuffLifetime(int64 InID)
{
	// 双查(同ChangeBuffLayer)
	FBXBuffRuntimeData* Data = GetBuffRuntimeDataByID(InID);
	if (!Data || Data->bEarlyFinish)
	{
		return;
	}

	Data->RunTime = 0.0f;

	for (float& LayerTime : Data->LayerRunTimes)
	{
		LayerTime = 0.0f;
	}

	// 服务器刷新复制快照(到期时刻重置)
	if (UBXBuffComponent* BuffComponent = GetOwnerBuffComponent(Data->TLRunTimeData.Owner))
	{
		BuffComponent->UpdateBuffReplicatedState(InID);
	}

	BroadcastBuffEvent(*Data, BXGameplayTags::BXEvent_Buff_LifetimeRefreshed, FBXEventBuffChanged());
}

bool UBXBuffManager::HasBuff(AActor* InOwner, UBXBuffAsset* InAsset) const
{
	if (const TArray<int64>* IDs = OwnerBuffMap.Find(InOwner))
	{
		for (int64 ID : *IDs)
		{
			const FBXBuffRuntimeData* Data = BuffRTDatas.Find(ID);
			if (Data && !Data->bEarlyFinish && Data->BuffAsset == InAsset)
			{
				return true;
			}
		}
	}

	// 挂起区扫描(更新窗口内新增的BUFF未登记OwnerBuffMap,漏扫会令同帧HasBuff误报false)
	for (const TUniquePtr<FBXBuffRuntimeData>& Pending : PendingAddBuffs)
	{
		if (Pending.IsValid() && !Pending->bEarlyFinish && Pending->BuffAsset == InAsset && Pending->TLRunTimeData.Owner == InOwner)
		{
			return true;
		}
	}

	return false;
}

bool UBXBuffManager::HasBuffByTag(AActor* InOwner, FGameplayTag InTag) const
{
	if (const TArray<int64>* IDs = OwnerBuffMap.Find(InOwner))
	{
		for (int64 ID : *IDs)
		{
			const FBXBuffRuntimeData* Data = BuffRTDatas.Find(ID);
			if (Data && !Data->bEarlyFinish && Data->BuffAsset && Data->BuffAsset->BuffTags.HasTag(InTag))
			{
				return true;
			}
		}
	}

	// 挂起区扫描(同HasBuff)
	for (const TUniquePtr<FBXBuffRuntimeData>& Pending : PendingAddBuffs)
	{
		if (Pending.IsValid() && !Pending->bEarlyFinish && Pending->BuffAsset && Pending->BuffAsset->BuffTags.HasTag(InTag) && Pending->TLRunTimeData.Owner == InOwner)
		{
			return true;
		}
	}

	return false;
}

FBXBuffRuntimeData* UBXBuffManager::GetBuffRuntimeDataByID(int64 InID)
{
	if (FBXBuffRuntimeData* Data = BuffRTDatas.Find(InID))
	{
		return Data;
	}

	// 挂起区双查(更新窗口内新增的BUFF尚未合并入主容器,漏查会令同帧查询/共存判定/快照填充失效)
	for (const TUniquePtr<FBXBuffRuntimeData>& Pending : PendingAddBuffs)
	{
		if (Pending.IsValid() && Pending->BuffID == InID)
		{
			return Pending.Get();
		}
	}

	return nullptr;
}

void UBXBuffManager::BroadcastBuffEvent(const FBXBuffRuntimeData& InData, const FGameplayTag& InEventTag, const FBXEventBuffChanged& InParam)
{
	UBXEventManager* EventMgr = UBXEventManager::Get(this);
	if (!EventMgr)
	{
		return;
	}

	FBXEventBuffChanged Param = InParam;
	Param.BuffInstanceID = InData.BuffID;
	Param.BuffAsset = InData.BuffAsset;
	Param.Owner = InData.TLRunTimeData.Owner;
	Param.Instigator = InData.TLRunTimeData.Instigator;
	Param.Triggerer = InData.TLRunTimeData.Triggerer;
	Param.NewLayer = InData.CurrentLayer;
	Param.NewLevel = InData.CurrentLevel;

	// 单体事件:以Owner为Initiator
	EventMgr->BroadcastSingleEvent<FBXEventBuffChanged>(InEventTag, InData.TLRunTimeData.Owner, Param);

	// 全局事件:所有监听者都能收到
	EventMgr->BroadcastGlobalEvent<FBXEventBuffChanged>(InEventTag, Param);
}

FBXBuffRuntimeData* UBXBuffManager::FindExistingBuff(AActor* InOwner, UBXBuffAsset* InAsset, AActor* InInstigator)
{
	if (const TArray<int64>* IDs = OwnerBuffMap.Find(InOwner))
	{
		for (int64 ID : *IDs)
		{
			FBXBuffRuntimeData* Data = BuffRTDatas.Find(ID);
			if (Data && !Data->bEarlyFinish && Data->BuffAsset == InAsset && Data->TLRunTimeData.Instigator == InInstigator)
			{
				return Data;
			}
		}
	}

	// 挂起区扫描(共存判定必须覆盖同帧新建的BUFF,否则重复施加会绕过共存叠加产生双实例)
	for (const TUniquePtr<FBXBuffRuntimeData>& Pending : PendingAddBuffs)
	{
		if (Pending.IsValid() && !Pending->bEarlyFinish && Pending->BuffAsset == InAsset && Pending->TLRunTimeData.Owner == InOwner && Pending->TLRunTimeData.Instigator == InInstigator)
		{
			return Pending.Get();
		}
	}

	return nullptr;
}

FBXBuffRuntimeData* UBXBuffManager::FindBestBuffToReplace(AActor* InOwner, UBXBuffAsset* InAsset)
{
	FBXBuffRuntimeData* Best = nullptr;

	if (const TArray<int64>* IDs = OwnerBuffMap.Find(InOwner))
	{
		for (int64 ID : *IDs)
		{
			FBXBuffRuntimeData* Data = BuffRTDatas.Find(ID);
			if (!Data || Data->bEarlyFinish || Data->BuffAsset != InAsset)
			{
				continue;
			}

			if (!Best)
			{
				Best = Data;
				continue;
			}

			// 取等级最高
			if (Data->CurrentLevel > Best->CurrentLevel)
			{
				Best = Data;
			}
			else if (Data->CurrentLevel == Best->CurrentLevel)
			{
				// 等级相同取剩余时长最长
				float DataRemain = Data->BuffAsset->BuffDuration - Data->RunTime;
				float BestRemain = Best->BuffAsset->BuffDuration - Best->RunTime;

				if (DataRemain > BestRemain)
				{
					Best = Data;
				}
			}
		}
	}

	// 挂起区扫描(替换判定须与共存判定同样覆盖同帧新建条目,候选纳入同一套比较)
	for (const TUniquePtr<FBXBuffRuntimeData>& Pending : PendingAddBuffs)
	{
		if (!Pending.IsValid() || Pending->bEarlyFinish || Pending->BuffAsset != InAsset || Pending->TLRunTimeData.Owner != InOwner)
		{
			continue;
		}

		if (!Best)
		{
			Best = Pending.Get();
			continue;
		}

		if (Pending->CurrentLevel > Best->CurrentLevel)
		{
			Best = Pending.Get();
		}
		else if (Pending->CurrentLevel == Best->CurrentLevel)
		{
			float PendingRemain = Pending->BuffAsset->BuffDuration - Pending->RunTime;
			float BestRemain = Best->BuffAsset->BuffDuration - Best->RunTime;

			if (PendingRemain > BestRemain)
			{
				Best = Pending.Get();
			}
		}
	}

	return Best;
}

void UBXBuffManager::InternalAddBuff(FBXBuffRuntimeData& InOutData, const FBXBuffPlayContext& InContext)
{
	// 填充三角色
	InOutData.TLRunTimeData.Instigator = InContext.Instigator;
	InOutData.TLRunTimeData.Triggerer = InContext.Triggerer;
	InOutData.CurrentLayer = InContext.InitLayer;
	InOutData.CurrentLevel = InContext.InitLevel;

	// 初始化单Section运行时数据(BUFF只用一个Section)
	FBXTLSectionRTData SectionRT;
	SectionRT.Index = 0;
	InOutData.TLRunTimeData.RunningSections.Add(SectionRT);

	// 独立生命周期初始化每层计时
	if (InOutData.BuffAsset->LayerLifeMode == EBXBuffLayerLifeMode::BLL_Independent)
	{
		InOutData.LayerRunTimes.Init(0.0f, InContext.InitLayer);
	}

	// 启动符合当前层级的Task
	StartBuffTasks(InOutData);

	// 广播BUFF添加事件
	BroadcastBuffEvent(InOutData, BXGameplayTags::BXEvent_Buff_Added, FBXEventBuffChanged());
}

FBXBuffRuntimeData* UBXBuffManager::InternalBuildNewBuff(UBXBuffAsset* InAsset, AActor* InOwner, const FBXBuffPlayContext& InContext, int64 InBuffID)
{
	// 先在堆上完整构建(含Task启动与Added广播):回调链中递归AddBuff时本对象地址稳定,
	// 且递归条目同样按时机入挂起区或主容器,均无TMap扩容风险
	TUniquePtr<FBXBuffRuntimeData> NewData = MakeUnique<FBXBuffRuntimeData>();
	NewData->BuffAsset = InAsset;
	NewData->BuffID = InBuffID;
	NewData->TLRunTimeData.ID = InBuffID;
	NewData->TLRunTimeData.Timeline = InAsset;
	NewData->TLRunTimeData.Owner = InOwner;

	InternalAddBuff(*NewData, InContext);

	if (bUpdatingBuff)
	{
		// 更新窗口内:入挂起区Tick末合并(直接Add会触发TMap扩容rehash,使更新管线遍历中的数据引用悬空)
		PendingAddBuffs.Add(MoveTemp(NewData));
	}
	else
	{
		OwnerBuffMap.FindOrAdd(InOwner).Add(InBuffID);
		BuffRTDatas.Add(InBuffID, MoveTemp(*NewData));
	}

	return GetBuffRuntimeDataByID(InBuffID);
}

void UBXBuffManager::MergePendingAddBuffs()
{
	for (TUniquePtr<FBXBuffRuntimeData>& Pending : PendingAddBuffs)
	{
		if (!Pending.IsValid())
		{
			continue;
		}

		// 更新中被移除的条目:不并入主容器,仅清理组件本地登记(与CleanBuffTrash对bEarlyFinish条目的处理对称;
		// Removed广播与MulticastRemoveBuff已在InternalRemoveBuff执行过,此处禁止重复)
		if (Pending->bEarlyFinish)
		{
			if (AActor* Owner = Pending->TLRunTimeData.Owner)
			{
				if (UBXBuffComponent* BuffComponent = Owner->FindComponentByClass<UBXBuffComponent>())
				{
					BuffComponent->InternalOnBuffFinished(Pending->BuffID);
				}
			}

			continue;
		}

		// 纯数据搬移:OwnerBuffMap登记延迟至此(更新窗口内对其Add新键同样有rehash风险),合并点在全部遍历外扩容安全
		OwnerBuffMap.FindOrAdd(Pending->TLRunTimeData.Owner).Add(Pending->BuffID);
		BuffRTDatas.Add(Pending->BuffID, MoveTemp(*Pending));
	}

	PendingAddBuffs.Reset();
}

void UBXBuffManager::InternalRemoveBuff(FBXBuffRuntimeData& InOutData, EBXBuffRemoveReason InReason)
{
	// 幂等守卫:到期路径的FinishTimelineSection会触发Task End回调,BP可在回调中先行RemoveBuff同ID
	// (此时已完整移除含广播),无守卫会二次执行导致Removed事件与MulticastRemoveBuff重复
	if (InOutData.bEarlyFinish)
	{
		return;
	}

	// 停止所有Task
	StopBuffTasks(InOutData);

	InOutData.bEarlyFinish = true;

	// 广播BUFF移除事件
	FBXEventBuffChanged Param;
	Param.RemoveReason = static_cast<uint8>(InReason);
	BroadcastBuffEvent(InOutData, BXGameplayTags::BXEvent_Buff_Removed, Param);

	// 服务器端显式广播移除(所有移除路径统一收束点:手动/到期/层级耗尽),远端客户端同步移除
	// 客户端本地实例被移除时不广播(Multicast RPC在客户端调用无效)
	AActor* Owner = InOutData.TLRunTimeData.Owner;
	if (Owner && Owner->GetNetMode() != NM_Client)
	{
		if (UBXBuffComponent* BuffComponent = Owner->FindComponentByClass<UBXBuffComponent>())
		{
			UE_LOG(BXMGR_Buff, Log, TEXT("UBXBuffManager::InternalRemoveBuff: BuffID=%lld Reason=%d, broadcasting."), InOutData.BuffID, (int32)InReason);
			BuffComponent->MulticastRemoveBuff(InOutData.BuffID, static_cast<uint8>(InReason));
			BuffComponent->RemoveBuffReplicatedState(InOutData.BuffID);
		}
	}
	else
	{
		UE_LOG(BXMGR_Buff, Log, TEXT("UBXBuffManager::InternalRemoveBuff: BuffID=%lld Reason=%d (local client instance)."), InOutData.BuffID, (int32)InReason);
	}
}

void UBXBuffManager::InternalRefreshBuffTasksByLayer(FBXBuffRuntimeData& InOutData, int32 InOldLayer, int32 InNewLayer)
{
	UBXBuffAsset* Asset = InOutData.BuffAsset;
	if (!Asset)
	{
		return;
	}

	for (const FBXBuffTaskBinding& Binding : Asset->TaskBindings)
	{
		if (!Binding.Task)
		{
			continue;
		}

		bool bWasInRange = IsTaskInLayerRange(Binding, InOldLayer);
		bool bIsInRange = IsTaskInLayerRange(Binding, InNewLayer);

		if (bWasInRange && !bIsInRange)
		{
			// 离开区间:停止Task
			StopBuffTask(InOutData, Binding.Task);
		}
		else if (!bWasInRange && bIsInRange)
		{
			// 进入区间:启动Task
			ExecuteBuffTask(InOutData, Binding.Task);
		}
		else if (bIsInRange)
		{
			// 仍在区间内:重建效果(需求11高级层方案)
			RebuildBuffTaskEffect(InOutData, Binding.Task, InOldLayer, InNewLayer);
		}
	}
}

#pragma endregion Buff



#pragma region TaskBridge
void UBXBuffManager::StartBuffTasks(FBXBuffRuntimeData& InOutData)
{
	UBXBuffAsset* Asset = InOutData.BuffAsset;
	if (!Asset)
	{
		return;
	}

	for (const FBXBuffTaskBinding& Binding : Asset->TaskBindings)
	{
		if (!Binding.Task)
		{
			UE_LOG(BXMGR_Buff, Warning, TEXT("UBXBuffManager::StartBuffTasks: Binding.Task is null, skipped. BuffID=%lld."), InOutData.BuffID);
			continue;
		}

		if (!IsTaskInLayerRange(Binding, InOutData.CurrentLayer))
		{
			continue;
		}

		ExecuteBuffTask(InOutData, Binding.Task);
	}
}

void UBXBuffManager::StopBuffTasks(FBXBuffRuntimeData& InOutData)
{
	UBXTLManager* TLMgr = UBXTLManager::Get(this);
	if (!TLMgr)
	{
		return;
	}

	if (InOutData.TLRunTimeData.RunningSections.Num() == 0)
	{
		return;
	}

	TLMgr->FinishTimelineSection(
		InOutData.TLRunTimeData,
		InOutData.TLRunTimeData.RunningSections[0],
		EBXTLFinishReason::FR_Interrupt);
}

void UBXBuffManager::StopBuffTask(FBXBuffRuntimeData& InOutData, UBXTask* InTask)
{
	UBXTLManager* TLMgr = UBXTLManager::Get(this);
	if (!TLMgr || !InTask)
	{
		return;
	}

	if (InOutData.TLRunTimeData.RunningSections.Num() == 0)
	{
		return;
	}

	FBXTLRunTimeData& TLData = InOutData.TLRunTimeData;
	FBXTLSectionRTData& SectionRT = TLData.RunningSections[0];

	for (TArray<FBXTLTaskRTData>::TIterator It(SectionRT.RunningTasks); It; ++It)
	{
		FBXTLTaskRTData& TaskRT = *It;

		if (TaskRT.Task == InTask)
		{
			if (UBXTProcessor* Processor = TLMgr->GetTLTProcessorByTLTClass(InTask->GetClass()))
			{
				// EndTask内部已按地址从RunningTasks移除该条目,外层禁止再RemoveCurrent(否则误删左移进来的相邻条目/末尾元素越界)
				Processor->EndTask(TLData, SectionRT, TaskRT, EBXTLFinishReason::FR_Interrupt);
			}
			else
			{
				It.RemoveCurrent();
			}

			break;
		}
	}
}

void UBXBuffManager::ExecuteBuffTask(FBXBuffRuntimeData& InOutData, UBXTask* InTask)
{
	UBXTLManager* TLMgr = UBXTLManager::Get(this);
	if (!TLMgr || !InTask)
	{
		UE_LOG(BXMGR_Buff, Warning, TEXT("UBXBuffManager::ExecuteBuffTask failed: TLMgr=%s InTask=%s."), TLMgr ? TEXT("valid") : TEXT("null"), InTask ? TEXT("valid") : TEXT("null"));
		return;
	}

	if (InOutData.TLRunTimeData.RunningSections.Num() == 0)
	{
		UE_LOG(BXMGR_Buff, Warning, TEXT("UBXBuffManager::ExecuteBuffTask failed: RunningSections is empty. Task=%s."), *InTask->GetName());
		return;
	}

	FBXTLRunTimeData& TLData = InOutData.TLRunTimeData;
	FBXTLSectionRTData& SectionRT = TLData.RunningSections[0];

	// 查找Task在Sections[0].TaskList中的索引
	int32 TaskIndex = INDEX_NONE;
	if (InOutData.BuffAsset && InOutData.BuffAsset->Sections.Num() > 0)
	{
		InOutData.BuffAsset->Sections[0].TaskList.Find(InTask, TaskIndex);
	}

	if (TaskIndex == INDEX_NONE)
	{
		UE_LOG(BXMGR_Buff, Warning, TEXT("UBXBuffManager::ExecuteBuffTask failed: Task=%s not found in Sections[0].TaskList, Sections.Num=%d TaskList.Num=%d."), *InTask->GetName(), InOutData.BuffAsset ? InOutData.BuffAsset->Sections.Num() : -1, (InOutData.BuffAsset && InOutData.BuffAsset->Sections.Num() > 0) ? InOutData.BuffAsset->Sections[0].TaskList.Num() : -1);
		return;
	}

	// 构造网络上下文
	ENetMode NetMode = TLData.Owner ? TLData.Owner->GetNetMode() : NM_Standalone;
	ENetRole LocalRole = TLData.Owner ? TLData.Owner->GetLocalRole() : ROLE_Authority;

	// 完全复用UBXTLManager的Task启动逻辑
	TLMgr->ExecuteTimelineTask(TLData, SectionRT, TaskIndex, NetMode, LocalRole, 0.0f);
}

void UBXBuffManager::RebuildBuffTaskEffect(FBXBuffRuntimeData& InOutData, UBXTask* InTask, int32 InOldLayer, int32 InNewLayer)
{
	UBXTLManager* TLMgr = UBXTLManager::Get(this);
	if (!TLMgr || !InTask)
	{
		return;
	}

	if (InOutData.TLRunTimeData.RunningSections.Num() == 0)
	{
		return;
	}

	FBXTLRunTimeData& TLData = InOutData.TLRunTimeData;
	FBXTLSectionRTData& SectionRT = TLData.RunningSections[0];

	for (FBXTLTaskRTData& TaskRT : SectionRT.RunningTasks)
	{
		if (TaskRT.Task == InTask)
		{
			if (UBXTProcessor* Processor = TLMgr->GetTLTProcessorByTLTClass(InTask->GetClass()))
			{
				Processor->RebuildEffectTask(TLData, SectionRT, TaskRT, InOldLayer, InNewLayer);
			}

			break;
		}
	}
}

bool UBXBuffManager::IsTaskInLayerRange(const FBXBuffTaskBinding& InBinding, int32 InLayer) const
{
	// (0, 0)代表无层级限制
	if (InBinding.MinLayer == 0 && InBinding.MaxLayer == 0)
	{
		return true;
	}

	if (InBinding.MaxLayer == 0)
	{
		return InLayer >= InBinding.MinLayer;
	}

	if (InBinding.MinLayer == 0)
	{
		return InLayer <= InBinding.MaxLayer;
	}

	return InLayer >= InBinding.MinLayer && InLayer <= InBinding.MaxLayer;
}

#pragma endregion TaskBridge
