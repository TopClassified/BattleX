#include "BXBuffComponent.h"

#include "BXBuffAsset.h"
#include "BXBuffManager.h"
#include "BXBuffEnums.h"
#include "BXFunctionLibrary.h"
#include "BXTLManager.h"
#include "BXTLAsset.h"
#include "Net/UnrealNetwork.h"
#include "Engine/NetDriver.h"



UBXBuffComponent::UBXBuffComponent()
{
	SetIsReplicatedByDefault(true);
}

void UBXBuffComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 仅初始同步发送:新连接拿最新快照重建,已有连接零属性流量(动态由显式RPC维护)
	DOREPLIFETIME_CONDITION(UBXBuffComponent, RunningBuffStates, COND_InitialOnly);
}

void UBXBuffComponent::PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker)
{
	Super::PreReplication(ChangedPropertyTracker);

	// 服务器端、属性收集前每帧执行
	AActor* Owner = GetOwner();
	if (!Owner || Owner->GetLocalRole() != ENetRole::ROLE_Authority)
	{
		return;
	}

	// RunningBuffStates为COND_InitialOnly:仅新连接初始同步时发送,已有连接零流量
	// 故仅当远程连接数增加(新客户端连入)时才重建快照,避免每帧每BUFF全量投影
	// 时序:连接加入ClientConnections后才会在后续ServerReplicateActors打开通道并序列化初始状态,
	// PreReplication在同一次flush的序列化前运行,当帧重建即被新连接消费,无窗口期
	UNetDriver* NetDriver = GetWorld() ? GetWorld()->GetNetDriver() : nullptr;
	if (!NetDriver)
	{
		return;
	}

	const int32 ConnectionCount = NetDriver->ClientConnections.Num();
	if (ConnectionCount > LastProjectedConnectionCount)
	{
		RebuildRunningBuffStates();
	}
	// 断线回落仅同步计数不重建(InitialOnly已发收不回);不回落则"断N+连N"净计数不变会漏触发
	LastProjectedConnectionCount = ConnectionCount;
}

static void FillBuffReplicatedState(FBXBuffReplicatedState& InOutState, const FBXBuffRuntimeData& InData);

void UBXBuffComponent::RebuildRunningBuffStates()
{
	UBXBuffManager* Mgr = UBXBuffManager::Get(this);
	if (!Mgr)
	{
		return;
	}

	RunningBuffStates.Reset();
	for (int64 BuffID : OwnedBuffIDs)
	{
		// 双查入口(挂起区条目同样需投影),bEarlyFinish条目即将清理不投影
		FBXBuffRuntimeData* Data = Mgr->GetBuffRuntimeDataByID(BuffID);
		if (!Data || Data->bEarlyFinish)
		{
			continue;
		}

		FBXBuffReplicatedState& State = RunningBuffStates.AddDefaulted_GetRef();
		FillBuffReplicatedState(State, *Data);
	}
}

#pragma region API
int64 UBXBuffComponent::AddBuff(UBXBuffAsset* InAsset, AActor* InInstigator, int32 InLayer, int32 InLevel)
{
	if (!InAsset || !GetOwner())
	{
		return INDEX_NONE;
	}

	AActor* Owner = GetOwner();
	ENetRole LocalRole = Owner->GetLocalRole();

	// 服务器权威端:直接添加并广播
	if (LocalRole == ENetRole::ROLE_Authority)
	{
		UBXBuffManager* Mgr = UBXBuffManager::Get(this);
		if (!Mgr)
		{
			return INDEX_NONE;
		}

		FBXBuffPlayContext Context;
		Context.Instigator = InInstigator ? InInstigator : Owner;
		Context.Triggerer = Owner;
		Context.InitLayer = InLayer;
		Context.InitLevel = InLevel;

		// 服务器本地调用同样钳制层级/等级到资产范围(与RPC路径一致,越界InitLayer会撑爆LayerRunTimes)
		FBXBuffPlayContext ValidatedContext;
		if (!Mgr->ServerValidateAddBuff(InAsset, Owner, Context, ValidatedContext))
		{
			return INDEX_NONE;
		}

		int64 BuffID = Mgr->AddBuff(InAsset, Owner, ValidatedContext);
		if (BuffID != INDEX_NONE)
		{
			OwnedBuffIDs.Add(BuffID);
			MulticastAddBuff(BuffID, InAsset->ID, Owner, ValidatedContext.Instigator, ValidatedContext.InitLayer, ValidatedContext.InitLevel);
		}

		return BuffID;
	}

	// 客户端:请求服务器添加(传资产ID,服务器经注册表解析)
	if (LocalRole == ENetRole::ROLE_AutonomousProxy)
	{
		ServerRequestAddBuff(InAsset->ID, InInstigator, InLayer, InLevel);
		return INDEX_NONE;
	}

	return INDEX_NONE;
}

void UBXBuffComponent::RemoveBuff(int64 InBuffID, int32 InLayerDelta)
{
	AActor* Owner = GetOwner();
	if (!Owner)
	{
		return;
	}

	ENetRole LocalRole = Owner->GetLocalRole();

	// 服务器权威端:直接移除(移除广播由Manager.InternalRemoveBuff统一收束发送)
	if (LocalRole == ENetRole::ROLE_Authority)
	{
		UBXBuffManager* Mgr = UBXBuffManager::Get(this);
		if (!Mgr)
		{
			return;
		}

		Mgr->RemoveBuff(InBuffID, InLayerDelta);

		if (InLayerDelta == 0)
		{
			OwnedBuffIDs.Remove(InBuffID);
		}
		return;
	}

	// 客户端:请求服务器移除
	if (LocalRole == ENetRole::ROLE_AutonomousProxy)
	{
		ServerRequestRemoveBuff(InBuffID, InLayerDelta);
	}
}

void UBXBuffComponent::ChangeBuffLayer(int64 InBuffID, int32 InLayerDelta)
{
	if (UBXBuffManager* Mgr = UBXBuffManager::Get(this))
	{
		Mgr->ChangeBuffLayer(InBuffID, InLayerDelta);
	}
}

void UBXBuffComponent::ChangeBuffLevel(int64 InBuffID, int32 InLevelDelta)
{
	if (UBXBuffManager* Mgr = UBXBuffManager::Get(this))
	{
		Mgr->ChangeBuffLevel(InBuffID, InLevelDelta);
	}
}

void UBXBuffComponent::RefreshBuffLifetime(int64 InBuffID)
{
	if (UBXBuffManager* Mgr = UBXBuffManager::Get(this))
	{
		Mgr->RefreshBuffLifetime(InBuffID);
	}
}

bool UBXBuffComponent::HasBuff(UBXBuffAsset* InAsset) const
{
	if (UBXBuffManager* Mgr = UBXBuffManager::Get(const_cast<UBXBuffComponent*>(this)))
	{
		return Mgr->HasBuff(GetOwner(), InAsset);
	}

	return false;
}

bool UBXBuffComponent::HasBuffByTag(FGameplayTag InTag) const
{
	if (UBXBuffManager* Mgr = UBXBuffManager::Get(const_cast<UBXBuffComponent*>(this)))
	{
		return Mgr->HasBuffByTag(GetOwner(), InTag);
	}

	return false;
}

#pragma endregion API



#pragma region Lifecycle
void UBXBuffComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UBXBuffManager* Mgr = UBXBuffManager::Get(this))
	{
		for (int64 BuffID : OwnedBuffIDs)
		{
			Mgr->RemoveBuff(BuffID);
		}
	}

	OwnedBuffIDs.Empty();

	Super::EndPlay(EndPlayReason);
}

#pragma endregion Lifecycle



#pragma region Sync
bool UBXBuffComponent::ServerRequestAddBuff_Validate(int32 InBuffAssetID, AActor* InInstigator, int32 InLayer, int32 InLevel)
{
	return true;
}

void UBXBuffComponent::ServerRequestAddBuff_Implementation(int32 InBuffAssetID, AActor* InInstigator, int32 InLayer, int32 InLevel)
{
	UBXBuffManager* Mgr = UBXBuffManager::Get(this);
	UBXTLManager* TLMgr = UBXTLManager::Get(this);
	if (!Mgr || !TLMgr || !GetOwner())
	{
		return;
	}

	// 查找BUFF资产(注册表解析,防客户端伪造对象引用,与Late Join重建同源)
	TSoftObjectPtr<UBXTLAsset> AssetPtr = TLMgr->GetTimelineAssetByID(InBuffAssetID);
	UBXBuffAsset* BuffAsset = Cast<UBXBuffAsset>(AssetPtr.Get());
	if (!BuffAsset)
	{
		BuffAsset = Cast<UBXBuffAsset>(AssetPtr.LoadSynchronous());
	}

	if (!BuffAsset)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBXBuffComponent::ServerRequestAddBuff: BuffAsset not found. BuffAssetID=%d."), InBuffAssetID);
		return;
	}

	FBXBuffPlayContext Context;
	Context.Instigator = InInstigator ? InInstigator : GetOwner();
	Context.Triggerer = GetOwner();
	Context.InitLayer = InLayer;
	Context.InitLevel = InLevel;

	// 服务器校验(层级/等级钳制到资产范围)
	FBXBuffPlayContext ValidatedContext;
	if (!Mgr->ServerValidateAddBuff(BuffAsset, GetOwner(), Context, ValidatedContext))
	{
		return;
	}

	int64 BuffID = Mgr->AddBuff(BuffAsset, GetOwner(), ValidatedContext);
	if (BuffID != INDEX_NONE)
	{
		// 登记本地持有(与Authority路径对称,否则服务器持有列表缺RPC来源BUFF,Owner EndPlay不清理导致Task对已销毁Owner继续执行)
		OwnedBuffIDs.Add(BuffID);
		MulticastAddBuff(BuffID, BuffAsset->ID, GetOwner(), ValidatedContext.Instigator, ValidatedContext.InitLayer, ValidatedContext.InitLevel);
	}
}

bool UBXBuffComponent::ServerRequestRemoveBuff_Validate(int64 InBuffID, int32 InLayerDelta)
{
	return true;
}

void UBXBuffComponent::ServerRequestRemoveBuff_Implementation(int64 InBuffID, int32 InLayerDelta)
{
	UBXBuffManager* Mgr = UBXBuffManager::Get(this);
	if (!Mgr)
	{
		return;
	}

	// 整体移除与层数变化均由Manager统一处理:移除广播收束于InternalRemoveBuff,层数变化经复制快照同步
	Mgr->RemoveBuff(InBuffID, InLayerDelta);
}

void UBXBuffComponent::MulticastAddBuff_Implementation(int64 InBuffID, int32 InBuffAssetID, AActor* InOwner, AActor* InInstigator, int32 InLayer, int32 InLevel)
{
	UBXBuffManager* Mgr = UBXBuffManager::Get(this);
	UBXTLManager* TLMgr = UBXTLManager::Get(this);
	if (!Mgr || !TLMgr || !InOwner)
	{
		return;
	}

	// 服务器端已添加,跳过
	if (InOwner->GetLocalRole() == ENetRole::ROLE_Authority)
	{
		return;
	}

	// 查找BUFF资产(注册表解析,与ServerRequestAddBuff/RebuildBuffFromState同源)
	TSoftObjectPtr<UBXTLAsset> AssetPtr = TLMgr->GetTimelineAssetByID(InBuffAssetID);
	UBXBuffAsset* BuffAsset = Cast<UBXBuffAsset>(AssetPtr.Get());
	if (!BuffAsset)
	{
		BuffAsset = Cast<UBXBuffAsset>(AssetPtr.LoadSynchronous());
	}

	if (!BuffAsset)
	{
		UE_LOG(BXMGR_Buff, Warning, TEXT("UBXBuffComponent::MulticastAddBuff: BuffAsset not found. BuffAssetID=%d."), InBuffAssetID);
		return;
	}

	FBXBuffPlayContext Context;
	Context.Instigator = InInstigator;
	Context.Triggerer = InOwner;
	Context.InitLayer = InLayer;
	Context.InitLevel = InLevel;

	int64 BuffID = Mgr->AddBuffWithID(BuffAsset, InOwner, Context, InBuffID);
	if (BuffID != INDEX_NONE)
	{
		OwnedBuffIDs.Add(BuffID);
		UE_LOG(BXMGR_Buff, Log, TEXT("UBXBuffComponent::MulticastAddBuff: Remote add. BuffID=%lld Asset=%s Layer=%d Level=%d."), BuffID, *BuffAsset->GetName(), InLayer, InLevel);
	}
}

void UBXBuffComponent::MulticastRemoveBuff_Implementation(int64 InBuffID, uint8 InRemoveReason)
{
	UBXBuffManager* Mgr = UBXBuffManager::Get(this);
	if (!Mgr)
	{
		return;
	}

	// 服务器端已移除,跳过(数据仅标记bEarlyFinish,RemoveBuff的守卫会直接返回)
	FBXBuffRuntimeData* Data = Mgr->GetBuffRuntimeDataByID(InBuffID);
	if (!Data)
	{
		return;
	}

	// 携带服务器移除原因,保证本地BXEvent.Buff.Removed事件Reason一致
	UE_LOG(BXMGR_Buff, Log, TEXT("UBXBuffComponent::MulticastRemoveBuff: Remote remove. BuffID=%lld Reason=%d."), InBuffID, (int32)InRemoveReason);
	Mgr->RemoveBuffWithReason(InBuffID, static_cast<EBXBuffRemoveReason>(InRemoveReason));
	OwnedBuffIDs.Remove(InBuffID);
}

void UBXBuffComponent::MulticastBuffStateChanged_Implementation(FBXBuffReplicatedState InState)
{
	// 服务器端已应用变化,ApplyBuffStateChange内部权威端守卫直接返回
	ApplyBuffStateChange(InState);
}

#pragma endregion Sync



#pragma region Internal
// 服务器维护:从运行数据填充快照条目(到期时刻按"当前剩余"换算为服务器世界时间戳)
static void FillBuffReplicatedState(FBXBuffReplicatedState& InOutState, const FBXBuffRuntimeData& InData)
{
	InOutState.BuffID = InData.BuffID;
	InOutState.BuffAssetID = InData.BuffAsset ? InData.BuffAsset->ID : 0;
	InOutState.Layer = InData.CurrentLayer;
	InOutState.Level = InData.CurrentLevel;
	InOutState.Instigator = InData.TLRunTimeData.Instigator;
	InOutState.SharedExpireServerTimestamp = 0;
	InOutState.LayerExpireServerTimestamps.Reset();

	UBXBuffAsset* Asset = InData.BuffAsset;
	if (!Asset || Asset->LifeType != EBXBuffLifeType::BL_Duration || Asset->BuffDuration <= 0.0f)
	{
		return;
	}

	const int64 NowMs = UBXFunctionLibrary::GetServerWorldTimeMilliseconds(InData.TLRunTimeData.Owner);
	if (Asset->LayerLifeMode == EBXBuffLayerLifeMode::BLL_Shared)
	{
		const float Remaining = FMath::Clamp(Asset->BuffDuration - InData.RunTime, 0.0f, Asset->BuffDuration);
		InOutState.SharedExpireServerTimestamp = NowMs + static_cast<int64>(Remaining * 1000.0f);
	}
	else
	{
		InOutState.LayerExpireServerTimestamps.Reserve(InData.LayerRunTimes.Num());
		for (float LayerRunTime : InData.LayerRunTimes)
		{
			const float Remaining = FMath::Clamp(Asset->BuffDuration - LayerRunTime, 0.0f, Asset->BuffDuration);
			InOutState.LayerExpireServerTimestamps.Add(NowMs + static_cast<int64>(Remaining * 1000.0f));
		}
	}
}

void UBXBuffComponent::RebuildBuffFromState(const FBXBuffReplicatedState& InState)
{
	AActor* Owner = GetOwner();
	if (!Owner || Owner->GetLocalRole() == ENetRole::ROLE_Authority)
	{
		return;
	}

	UBXBuffManager* Mgr = UBXBuffManager::Get(this);
	UBXTLManager* TLMgr = UBXTLManager::Get(this);
	if (!Mgr || !TLMgr)
	{
		return;
	}

	// 已存在(Multicast先到)跳过
	if (Mgr->GetBuffRuntimeDataByID(InState.BuffID))
	{
		return;
	}

	// 查找BUFF资产
	TSoftObjectPtr<UBXTLAsset> AssetPtr = TLMgr->GetTimelineAssetByID(InState.BuffAssetID);
	UBXTLAsset* Asset = AssetPtr.Get();
	if (!Asset)
	{
		Asset = AssetPtr.LoadSynchronous();
	}

	UBXBuffAsset* BuffAsset = Cast<UBXBuffAsset>(Asset);
	if (!BuffAsset)
	{
		return;
	}

	FBXBuffPlayContext Context;
	Context.Instigator = InState.Instigator;
	Context.Triggerer = Owner;
	Context.InitLayer = InState.Layer;
	Context.InitLevel = InState.Level;

	int64 BuffID = Mgr->AddBuffWithID(BuffAsset, Owner, Context, InState.BuffID);
	if (BuffID == INDEX_NONE)
	{
		return;
	}

	OwnedBuffIDs.Add(BuffID);
	UE_LOG(BXMGR_Buff, Log, TEXT("UBXBuffComponent::RebuildBuffFromState: Rebuilt. BuffID=%lld Asset=%s Layer=%d Level=%d."), BuffID, *BuffAsset->GetName(), InState.Layer, InState.Level);

	// 计时对齐:按到期时间戳回填已流逝时长
	FBXBuffRuntimeData* Data = Mgr->GetBuffRuntimeDataByID(BuffID);
	if (!Data || !Data->BuffAsset || Data->BuffAsset->LifeType != EBXBuffLifeType::BL_Duration)
	{
		return;
	}

	UBXBuffAsset* RuntimeAsset = Data->BuffAsset;
	const int64 NowMs = UBXFunctionLibrary::GetServerWorldTimeMilliseconds(this);
	const float Duration = RuntimeAsset->BuffDuration;
	if (RuntimeAsset->LayerLifeMode == EBXBuffLayerLifeMode::BLL_Shared && InState.SharedExpireServerTimestamp > 0)
	{
		const float Remaining = FMath::Clamp(static_cast<float>(InState.SharedExpireServerTimestamp - NowMs) / 1000.0f, 0.0f, Duration);
		Data->RunTime = FMath::Max(0.0f, Duration - Remaining);
	}
	else if (RuntimeAsset->LayerLifeMode == EBXBuffLayerLifeMode::BLL_Independent)
	{
		Data->LayerRunTimes.Reset();
		for (int64 ExpireMs : InState.LayerExpireServerTimestamps)
		{
			const float Remaining = FMath::Clamp(static_cast<float>(ExpireMs - NowMs) / 1000.0f, 0.0f, Duration);
			Data->LayerRunTimes.Add(FMath::Max(0.0f, Duration - Remaining));
		}
	}
}

void UBXBuffComponent::ApplyBuffStateChange(const FBXBuffReplicatedState& InState)
{
	AActor* Owner = GetOwner();
	if (!Owner || Owner->GetLocalRole() == ENetRole::ROLE_Authority)
	{
		return;
	}

	UBXBuffManager* Mgr = UBXBuffManager::Get(this);
	if (!Mgr)
	{
		return;
	}

	FBXBuffRuntimeData* Data = Mgr->GetBuffRuntimeDataByID(InState.BuffID);
	if (!Data)
	{
		return;
	}

	// 层数变化(走Manager接口触发RebuildEffect)
	const int32 LayerDelta = InState.Layer - Data->CurrentLayer;
	if (LayerDelta != 0)
	{
		UE_LOG(BXMGR_Buff, Log, TEXT("UBXBuffComponent::ApplyBuffStateChange: Layer changed. BuffID=%lld %d -> %d."), InState.BuffID, Data->CurrentLayer, InState.Layer);
		Mgr->ChangeBuffLayer(InState.BuffID, LayerDelta);
	}

	// 等级变化
	const int32 LevelDelta = InState.Level - Data->CurrentLevel;
	if (LevelDelta != 0)
	{
		UE_LOG(BXMGR_Buff, Log, TEXT("UBXBuffComponent::ApplyBuffStateChange: Level changed. BuffID=%lld %d -> %d."), InState.BuffID, Data->CurrentLevel, InState.Level);
		Mgr->ChangeBuffLevel(InState.BuffID, LevelDelta);
	}

	// 计时对齐(时长刷新/层数增减后的到期变化)
	Data = Mgr->GetBuffRuntimeDataByID(InState.BuffID);
	if (Data && Data->BuffAsset && Data->BuffAsset->LifeType == EBXBuffLifeType::BL_Duration)
	{
		const int64 NowMs = UBXFunctionLibrary::GetServerWorldTimeMilliseconds(this);
		const float Duration = Data->BuffAsset->BuffDuration;

		if (Data->BuffAsset->LayerLifeMode == EBXBuffLayerLifeMode::BLL_Shared && InState.SharedExpireServerTimestamp > 0)
		{
			const float Remaining = FMath::Clamp(static_cast<float>(InState.SharedExpireServerTimestamp - NowMs) / 1000.0f, 0.0f, Duration);
			Data->RunTime = FMath::Max(0.0f, Duration - Remaining);
		}
		else if (Data->BuffAsset->LayerLifeMode == EBXBuffLayerLifeMode::BLL_Independent)
		{
			for (int32 i = 0; i < Data->LayerRunTimes.Num() && i < InState.LayerExpireServerTimestamps.Num(); ++i)
			{
				const float Remaining = FMath::Clamp(static_cast<float>(InState.LayerExpireServerTimestamps[i] - NowMs) / 1000.0f, 0.0f, Duration);
				Data->LayerRunTimes[i] = FMath::Max(0.0f, Duration - Remaining);
			}
		}
	}
}

void UBXBuffComponent::RemoveBuffIfLocalExists(int64 InBuffID)
{
	UBXBuffManager* Mgr = UBXBuffManager::Get(this);
	if (!Mgr || !Mgr->GetBuffRuntimeDataByID(InBuffID))
	{
		return;
	}

	Mgr->RemoveBuffWithReason(InBuffID, EBXBuffRemoveReason::BRR_Manual);
	OwnedBuffIDs.Remove(InBuffID);
}

void UBXBuffComponent::InternalOnBuffFinished(int64 InBuffID)
{
	OwnedBuffIDs.Remove(InBuffID);
}

void UBXBuffComponent::BroadcastBuffStateChanged(int64 InBuffID)
{
	AActor* Owner = GetOwner();
	if (!Owner || Owner->GetLocalRole() != ENetRole::ROLE_Authority)
	{
		return;
	}

	UBXBuffManager* Mgr = UBXBuffManager::Get(this);
	FBXBuffRuntimeData* Data = Mgr ? Mgr->GetBuffRuntimeDataByID(InBuffID) : nullptr;
	if (!Data)
	{
		return;
	}

	FBXBuffReplicatedState State;
	FillBuffReplicatedState(State, *Data);
	MulticastBuffStateChanged(State);
}

void UBXBuffComponent::OnRep_RunningBuffStates(TArray<FBXBuffReplicatedState> InOldStates)
{
	AActor* Owner = GetOwner();
	if (!Owner || Owner->GetLocalRole() == ENetRole::ROLE_Authority)
	{
		return;
	}

	// 新增条目:重建进行中的BUFF(已存在的ID在重建内部按去重跳过,幂等)
	for (const FBXBuffReplicatedState& State : RunningBuffStates)
	{
		bool bExisted = false;
		for (const FBXBuffReplicatedState& OldState : InOldStates)
		{
			if (OldState.BuffID == State.BuffID)
			{
				bExisted = true;
				break;
			}
		}

		if (!bExisted)
		{
			RebuildBuffFromState(State);
		}
	}

	// 消失条目:兜底移除本地实例(主通道MulticastRemoveBuff,仅处理RPC与属性乱序竞态)
	for (const FBXBuffReplicatedState& OldState : InOldStates)
	{
		bool bStillExists = false;
		for (const FBXBuffReplicatedState& State : RunningBuffStates)
		{
			if (State.BuffID == OldState.BuffID)
			{
				bStillExists = true;
				break;
			}
		}

		if (!bStillExists)
		{
			UE_LOG(BXMGR_Buff, Log, TEXT("UBXBuffComponent::OnRep_RunningBuffStates: Entry vanished, fallback remove. BuffID=%lld."), OldState.BuffID);
			RemoveBuffIfLocalExists(OldState.BuffID);
		}
	}
}

#pragma endregion Internal
