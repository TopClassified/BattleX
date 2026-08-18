#include "BXBuffComponent.h"

#include "BXBuffAsset.h"
#include "BXBuffManager.h"
#include "BXBuffEnums.h"
#include "BXFunctionLibrary.h"
#include "BXTLManager.h"
#include "BXTLAsset.h"
#include "Net/UnrealNetwork.h"



UBXBuffComponent::UBXBuffComponent()
{
	SetIsReplicatedByDefault(true);
}

void UBXBuffComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UBXBuffComponent, RunningBuffStates);
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

		int64 BuffID = Mgr->AddBuff(InAsset, Owner, Context);
		if (BuffID != INDEX_NONE)
		{
			OwnedBuffIDs.Add(BuffID);
			MulticastAddBuff(BuffID, InAsset, Owner, Context.Instigator, InLayer, InLevel);
		}

		return BuffID;
	}

	// 客户端:请求服务器添加
	if (LocalRole == ENetRole::ROLE_AutonomousProxy)
	{
		ServerRequestAddBuff(InAsset, InInstigator, InLayer, InLevel);
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
bool UBXBuffComponent::ServerRequestAddBuff_Validate(UBXBuffAsset* InAsset, AActor* InInstigator, int32 InLayer, int32 InLevel)
{
	return true;
}

void UBXBuffComponent::ServerRequestAddBuff_Implementation(UBXBuffAsset* InAsset, AActor* InInstigator, int32 InLayer, int32 InLevel)
{
	UBXBuffManager* Mgr = UBXBuffManager::Get(this);
	if (!Mgr || !InAsset || !GetOwner())
	{
		return;
	}

	FBXBuffPlayContext Context;
	Context.Instigator = InInstigator ? InInstigator : GetOwner();
	Context.Triggerer = GetOwner();
	Context.InitLayer = InLayer;
	Context.InitLevel = InLevel;

	// 服务器校验
	if (!Mgr->ServerValidateAddBuff(InAsset, GetOwner(), Context))
	{
		return;
	}

	int64 BuffID = Mgr->AddBuff(InAsset, GetOwner(), Context);
	if (BuffID != INDEX_NONE)
	{
		MulticastAddBuff(BuffID, InAsset, GetOwner(), Context.Instigator, InLayer, InLevel);
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

	Mgr->RemoveBuff(InBuffID, InLayerDelta);

	// 整体移除的广播由Manager.InternalRemoveBuff统一发送;层数变化仍需显式广播
	if (InLayerDelta != 0)
	{
		FBXBuffRuntimeData* Data = Mgr->GetBuffRuntimeDataByID(InBuffID);
		if (Data)
		{
			MulticastBuffLayerChanged(InBuffID, Data->CurrentLayer);
		}
	}
}

void UBXBuffComponent::MulticastAddBuff_Implementation(int64 InBuffID, UBXBuffAsset* InAsset, AActor* InOwner, AActor* InInstigator, int32 InLayer, int32 InLevel)
{
	UBXBuffManager* Mgr = UBXBuffManager::Get(this);
	if (!Mgr || !InAsset || !InOwner)
	{
		return;
	}

	// 服务器端已添加,跳过
	if (InOwner->GetLocalRole() == ENetRole::ROLE_Authority)
	{
		return;
	}

	FBXBuffPlayContext Context;
	Context.Instigator = InInstigator;
	Context.Triggerer = InOwner;
	Context.InitLayer = InLayer;
	Context.InitLevel = InLevel;

	int64 BuffID = Mgr->AddBuffWithID(InAsset, InOwner, Context, InBuffID);
	if (BuffID != INDEX_NONE)
	{
		OwnedBuffIDs.Add(BuffID);
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
	Mgr->RemoveBuffWithReason(InBuffID, static_cast<EBXBuffRemoveReason>(InRemoveReason));
	OwnedBuffIDs.Remove(InBuffID);
}

void UBXBuffComponent::MulticastBuffLayerChanged_Implementation(int64 InBuffID, int32 InNewLayer)
{
	// 层数变化由服务器权威控制,客户端无需额外处理(已在MulticastAddBuff或服务器Tick中同步)
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
		Mgr->ChangeBuffLayer(InState.BuffID, LayerDelta);
	}

	// 等级变化
	const int32 LevelDelta = InState.Level - Data->CurrentLevel;
	if (LevelDelta != 0)
	{
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

void UBXBuffComponent::AddBuffReplicatedState(int64 InBuffID)
{
	AActor* Owner = GetOwner();
	if (!Owner || Owner->GetLocalRole() != ENetRole::ROLE_Authority)
	{
		return;
	}

	UBXBuffManager* Mgr = UBXBuffManager::Get(this);
	if (!Mgr)
	{
		return;
	}

	FBXBuffRuntimeData* Data = Mgr->GetBuffRuntimeDataByID(InBuffID);
	if (!Data)
	{
		return;
	}

	FBXBuffReplicatedState& State = RunningBuffStates.AddDefaulted_GetRef();
	FillBuffReplicatedState(State, *Data);
}

void UBXBuffComponent::UpdateBuffReplicatedState(int64 InBuffID)
{
	AActor* Owner = GetOwner();
	if (!Owner || Owner->GetLocalRole() != ENetRole::ROLE_Authority)
	{
		return;
	}

	UBXBuffManager* Mgr = UBXBuffManager::Get(this);
	if (!Mgr)
	{
		return;
	}

	FBXBuffRuntimeData* Data = Mgr->GetBuffRuntimeDataByID(InBuffID);
	if (!Data)
	{
		return;
	}

	for (FBXBuffReplicatedState& State : RunningBuffStates)
	{
		if (State.BuffID == InBuffID)
		{
			FillBuffReplicatedState(State, *Data);
			break;
		}
	}
}

void UBXBuffComponent::RemoveBuffReplicatedState(int64 InBuffID)
{
	AActor* Owner = GetOwner();
	if (!Owner || Owner->GetLocalRole() != ENetRole::ROLE_Authority)
	{
		return;
	}

	RunningBuffStates.RemoveAll([InBuffID](const FBXBuffReplicatedState& Item)
	{
		return Item.BuffID == InBuffID;
	});
}

void UBXBuffComponent::OnRep_RunningBuffStates(TArray<FBXBuffReplicatedState> InOldStates)
{
	AActor* Owner = GetOwner();
	if (!Owner || Owner->GetLocalRole() == ENetRole::ROLE_Authority)
	{
		return;
	}

	for (const FBXBuffReplicatedState& State : RunningBuffStates)
	{
		const FBXBuffReplicatedState* OldState = nullptr;
		for (const FBXBuffReplicatedState& Candidate : InOldStates)
		{
			if (Candidate.BuffID == State.BuffID)
			{
				OldState = &Candidate;
				break;
			}
		}

		if (!OldState)
		{
			// 新增条目:重建进行中的BUFF(新复制到本地的对象初始化)
			RebuildBuffFromState(State);
		}
		else if (OldState->Layer != State.Layer || OldState->Level != State.Level
			|| OldState->SharedExpireServerTimestamp != State.SharedExpireServerTimestamp
			|| OldState->LayerExpireServerTimestamps != State.LayerExpireServerTimestamps)
		{
			// 变化条目:层/级/到期同步到已重建的本地实例
			ApplyBuffStateChange(State);
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
			RemoveBuffIfLocalExists(OldState.BuffID);
		}
	}
}

#pragma endregion Internal
