#include "BXSkillComponent.h"

#include "BXSkillAsset.h"
#include "BXSkillManager.h"
#include "BXFunctionLibrary.h"
#include "BXSettings.h"
#include "BXGameplayTags.h"
#include "BXTLStructs.h"
#include "BXTLManager.h"
#include "BXTStructs.h"
#include "BXNetStructs.h"
#include "Net/UnrealNetwork.h"



UBXSkillComponent::UBXSkillComponent()
{
	SetIsReplicatedByDefault(true);
}

void UBXSkillComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 仅初始同步发送:新连接拿最新快照重建,已有连接零属性流量(动态由显式RPC维护)
	DOREPLIFETIME_CONDITION(UBXSkillComponent, RunningSkillStates, COND_InitialOnly);
}

void UBXSkillComponent::PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker)
{
	Super::PreReplication(ChangedPropertyTracker);

	// 服务器端、属性收集前:从SkillRTDatas重建快照数组(解决运行数据新鲜度问题)
	AActor* Owner = GetOwner();
	if (!Owner || Owner->GetLocalRole() != ENetRole::ROLE_Authority)
	{
		return;
	}

	UBXSkillManager* SkillMgr = UBXSkillManager::Get(this);
	if (!SkillMgr)
	{
		return;
	}

	RunningSkillStates.Reset();
	for (int64 SkillID : OwnedSkillIDs)
	{
		FBXSkillRuntimeData* Data = SkillMgr->GetSkillRuntimeDataByID(SkillID);
		if (!Data || Data->bEarlyFinish)
		{
			continue;
		}

		FBXSkillReplicatedState& State = RunningSkillStates.AddDefaulted_GetRef();
		State.SkillID = SkillID;
		State.Initiator = Data->Initiator;
		BXToTLRunTimeProjection(Data->TLRunTimeData, State.TLRunTimeData, true);
	}
}

void UBXSkillComponent::OnRep_RunningSkillStates(TArray<FBXSkillReplicatedState> InOldStates)
{
	AActor* Owner = GetOwner();
	if (!Owner || Owner->GetLocalRole() == ENetRole::ROLE_Authority)
	{
		return;
	}

	// 新增条目:重建进行中的技能(已存在的ID在重建内部按去重跳过,幂等)
	for (const FBXSkillReplicatedState& State : RunningSkillStates)
	{
		bool bExisted = false;
		for (const FBXSkillReplicatedState& OldState : InOldStates)
		{
			if (OldState.SkillID == State.SkillID)
			{
				bExisted = true;
				break;
			}
		}

		if (!bExisted)
		{
			RebuildSkillFromState(State);
		}
	}

	// 消失条目:兜底停止本地实例(主通道MulticastStopSkill,仅处理RPC与属性乱序竞态)
	for (const FBXSkillReplicatedState& OldState : InOldStates)
	{
		bool bStillExists = false;
		for (const FBXSkillReplicatedState& State : RunningSkillStates)
		{
			if (State.SkillID == OldState.SkillID)
			{
				bStillExists = true;
				break;
			}
		}

		if (!bStillExists)
		{
			StopSkillIfNotPredicting(OldState.SkillID);
		}
	}
}

#pragma region API
int64 UBXSkillComponent::PlaySkill(UBXSkillAsset* InAsset, AActor* InInstigator, AActor* InTriggerer)
{
	return PlaySkillWithInputData(InAsset, TMap<FGameplayTag, FInstancedStruct>(), InInstigator, InTriggerer);
}

int64 UBXSkillComponent::PlaySkillWithTarget(UBXSkillAsset* InAsset, TArray<FBXBodyPartSelection>& InLockParts, AActor* InInstigator, AActor* InTriggerer)
{
	TMap<FGameplayTag, FInstancedStruct> InputDatas;
	InputDatas.Add(BXGameplayTags::BXSkillInput_LockParts, FInstancedStruct::Make(FBXSkillLockParts(InLockParts)));
	return PlaySkillWithInputData(InAsset, MoveTemp(InputDatas), InInstigator, InTriggerer);
}

int64 UBXSkillComponent::PlaySkillWithLocation(UBXSkillAsset* InAsset, FVector InAimLocation, AActor* InInstigator, AActor* InTriggerer)
{
	TMap<FGameplayTag, FInstancedStruct> InputDatas;
	InputDatas.Add(BXGameplayTags::BXSkillInput_AimLocation, FInstancedStruct::Make(InAimLocation));
	return PlaySkillWithInputData(InAsset, MoveTemp(InputDatas), InInstigator, InTriggerer);
}

int64 UBXSkillComponent::PlaySkillWithDirection(UBXSkillAsset* InAsset, FRotator InAimRotation, AActor* InInstigator, AActor* InTriggerer)
{
	TMap<FGameplayTag, FInstancedStruct> InputDatas;
	InputDatas.Add(BXGameplayTags::BXSkillInput_AimRotation, FInstancedStruct::Make(InAimRotation));
	return PlaySkillWithInputData(InAsset, MoveTemp(InputDatas), InInstigator, InTriggerer);
}

int64 UBXSkillComponent::PlaySkillWithInputData(UBXSkillAsset* InAsset, TMap<FGameplayTag, FInstancedStruct> InInputDatas, AActor* InInstigator, AActor* InTriggerer)
{
	if (!InAsset || !GetOwner())
	{
		return INDEX_NONE;
	}

	UBXSkillManager* SkillMgr = UBXSkillManager::Get(this);
	if (!SkillMgr)
	{
		return INDEX_NONE;
	}

	AActor* Owner = GetOwner();
	ENetRole LocalRole = Owner->GetLocalRole();

	// 补充释放位置和朝向到InputDatas(RPC传输形态为条目数组,逐条移动无深拷贝)
	TArray<FBXSkillInputEntry> MergedInputs;
	MergedInputs.Reserve(InInputDatas.Num() + 2);
	for (TPair<FGameplayTag, FInstancedStruct>& Pair : InInputDatas)
	{
		FBXSkillInputEntry& Entry = MergedInputs.AddDefaulted_GetRef();
		Entry.Tag = Pair.Key;
		Entry.Value = MoveTemp(Pair.Value);
	}
	{
		FBXSkillInputEntry& Entry = MergedInputs.AddDefaulted_GetRef();
		Entry.Tag = BXGameplayTags::BXSkillInput_ReleaseLocation;
		Entry.Value = FInstancedStruct::Make(Owner->GetActorLocation());
	}
	{
		FBXSkillInputEntry& Entry = MergedInputs.AddDefaulted_GetRef();
		Entry.Tag = BXGameplayTags::BXSkillInput_ReleaseRotation;
		Entry.Value = FInstancedStruct::Make(Owner->GetActorRotation());
	}

	// 构造同步头(时间戳为服务器世界时间域,依赖引擎时间同步)
	FBXSkillSyncHeader Header;
	Header.SkillAssetID = InAsset->ID;
	Header.ClientTimestamp = UBXFunctionLibrary::GetServerWorldTimeMilliseconds(this);

	// 构造Payload(兼作施放上下文与RPC传输体)
	FBXSkillSyncPayload Payload;
	Payload.Instigator = InInstigator ? InInstigator : Owner;
	Payload.Triggerer = InTriggerer ? InTriggerer : Owner;
	Payload.ClientTimestamp = Header.ClientTimestamp;
	Payload.InputDatas = MoveTemp(MergedInputs);

	// Standalone或服务器权威端:直接播放
	if (LocalRole == ENetRole::ROLE_Authority)
	{
		if (!IsCooldownReady(InAsset))
		{
			return INDEX_NONE;
		}

		Header.SkillID = SkillMgr->PlaySkill(InAsset, Owner, Payload);
		if (Header.SkillID == INDEX_NONE)
		{
			return INDEX_NONE;
		}

		Header.Initiator = EBXSyncInitiator::Server;
		Header.ServerTimestamp = UBXFunctionLibrary::GetServerWorldTimeMilliseconds(this);

		OwnedSkillIDs.Add(Header.SkillID);
		RecordCooldown(InAsset);
		MulticastPlaySkill(Header, Payload);
		return Header.SkillID;
	}

	// 自主端:客户端预测
	if (LocalRole == ENetRole::ROLE_AutonomousProxy)
	{
		// 本地冷却检查防连点(服务器仍会权威校验)
		if (!IsCooldownReady(InAsset))
		{
			return INDEX_NONE;
		}

		Header.SkillID = SkillMgr->StartPrediction(InAsset, Owner, Payload);
		if (Header.SkillID == INDEX_NONE)
		{
			return INDEX_NONE;
		}

		Header.Initiator = EBXSyncInitiator::Client;
		OwnedSkillIDs.Add(Header.SkillID);

		// 本地立即开始"假冷却"防止连点:服务器确认时转正为权威冷却,否认时移除
		RecordCooldown(InAsset);
		if (InAsset->Cooldown >= 0.0f)
		{
			PendingCooldownAssetIDs.Add(InAsset->ID);
		}

		ServerPlaySkill(Header, Payload);
		return Header.SkillID;
	}

	// 模拟端:不主动释放技能
	return INDEX_NONE;
}

void UBXSkillComponent::StopSkill(int64 InSkillID)
{
	UBXSkillManager* SkillMgr = UBXSkillManager::Get(this);
	if (!SkillMgr)
	{
		return;
	}

	SkillMgr->StopSkill(InSkillID, EBXTLFinishReason::FR_Interrupt);
	OwnedSkillIDs.Remove(InSkillID);
}

bool UBXSkillComponent::IsSkillRunning(int64 InSkillID) const
{
	return OwnedSkillIDs.Contains(InSkillID);
}

#pragma endregion API



#pragma region RPC
bool UBXSkillComponent::ServerPlaySkill_Validate(FBXSkillSyncHeader InHeader, FBXSkillSyncPayload InPayload)
{
	return true;
}

void UBXSkillComponent::ServerPlaySkill_Implementation(FBXSkillSyncHeader InHeader, FBXSkillSyncPayload InPayload)
{
	HandleServerPlaySkill(InHeader, InPayload);
}

void UBXSkillComponent::ClientPredictResult_Implementation(int64 InSkillID, bool bSuccess, int64 InServerTimestamp, float InCooldownRemaining)
{
	UBXSkillManager* SkillMgr = UBXSkillManager::Get(this);
	if (!SkillMgr)
	{
		return;
	}

	// 先取回技能资产用于假冷却结算(回滚只标记bEarlyFinish,运行数据下一帧才清理)
	UBXSkillAsset* PendingAsset = nullptr;
	if (FBXSkillRuntimeData* Data = SkillMgr->GetSkillRuntimeDataByID(InSkillID))
	{
		PendingAsset = Data->SkillAsset;
	}

	if (bSuccess)
	{
		SkillMgr->ConfirmPrediction(InSkillID, InServerTimestamp);
	}
	else
	{
		SkillMgr->RollbackPrediction(InSkillID);
		OwnedSkillIDs.Remove(InSkillID);
	}

	// 假冷却结算:
	// InCooldownRemaining>=0: 确认=假冷却转正 / 冷却拒绝=同步服务器真冷却(以 本地时间+服务器剩余时长 为准)
	// InCooldownRemaining<0: 服务器侧无冷却(条件/时间戳等否认),移除假冷却允许重试
	if (PendingAsset)
	{
		if (InCooldownRemaining >= 0.0f)
		{
			UWorld* World = GetWorld();
			if (World)
			{
				CooldownMap.Add(PendingAsset->ID, World->GetTimeSeconds() + InCooldownRemaining);
			}
		}
		else
		{
			ClearCooldown(PendingAsset);
		}

		PendingCooldownAssetIDs.Remove(PendingAsset->ID);
	}
}

void UBXSkillComponent::MulticastPlaySkill_Implementation(FBXSkillSyncHeader InHeader, FBXSkillSyncPayload InPayload)
{
	// 如果是自己发起的预测,跳过(已经预测了)
	UBXSkillManager* SkillMgr = UBXSkillManager::Get(this);
	if (!SkillMgr)
	{
		return;
	}

	if (SkillMgr->GetSkillRuntimeDataByID(InHeader.SkillID))
	{
		return;
	}

	// 服务器端也跳过(已经执行了)
	if (GetOwner() && GetOwner()->GetLocalRole() == ENetRole::ROLE_Authority)
	{
		return;
	}

	HandleClientPlaySkill(InHeader, MoveTemp(InPayload));
}

void UBXSkillComponent::MulticastStopSkill_Implementation(int64 InSkillID, uint8 InFinishReason)
{
	UBXSkillManager* SkillMgr = UBXSkillManager::Get(this);
	if (!SkillMgr)
	{
		return;
	}

	// 停止本地实例(服务器端实例已停,StopSkill的bEarlyFinish守卫会跳过)
	SkillMgr->StopSkill(InSkillID, static_cast<EBXTLFinishReason>(InFinishReason));
	OwnedSkillIDs.Remove(InSkillID);
}

bool UBXSkillComponent::ServerReportCollisionResults_Validate(int64 InSkillID, int32 InTaskFullIndex, FGameplayTag InDataTag, FBXTHitResults InResults)
{
	return true;
}

void UBXSkillComponent::ServerReportCollisionResults_Implementation(int64 InSkillID, int32 InTaskFullIndex, FGameplayTag InDataTag, FBXTHitResults InResults)
{
	UBXSkillManager* SkillMgr = UBXSkillManager::Get(this);
	if (!SkillMgr)
	{
		return;
	}

	SkillMgr->ReceiveCollisionResults(InSkillID, InTaskFullIndex, InDataTag, InResults);
}

#pragma endregion RPC



#pragma region Internal
void UBXSkillComponent::HandleServerPlaySkill(const FBXSkillSyncHeader& InHeader, const FBXSkillSyncPayload& InPayload)
{
	UBXSkillManager* SkillMgr = UBXSkillManager::Get(this);
	if (!SkillMgr)
	{
		return;
	}

	// 查找技能资产
	UBXTLManager* TLMgr = UBXTLManager::Get(this);
	if (!TLMgr)
	{
		return;
	}

	TSoftObjectPtr<UBXTLAsset> AssetPtr = TLMgr->GetSkillAssetByID(InHeader.SkillAssetID);
	UBXSkillAsset* SkillAsset = Cast<UBXSkillAsset>(AssetPtr.Get());
	if (!SkillAsset)
	{
		SkillAsset = Cast<UBXSkillAsset>(AssetPtr.LoadSynchronous());
	}

	if (!SkillAsset)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBXSkillComponent::HandleServerPlaySkill: SkillAsset not found. SkillAssetID=%d."), InHeader.SkillAssetID);
		ClientPredictResult(InHeader.SkillID, false, 0, -1.0f);
		return;
	}

	// 填充服务器时间戳(服务器世界时间域)
	FBXSkillSyncHeader Header = InHeader;
	Header.ServerTimestamp = UBXFunctionLibrary::GetServerWorldTimeMilliseconds(this);

	// SkillID校验:必须为客户端发起,且服务器上不存在(防伪造/防跨端ID碰撞覆盖正在运行的技能)
	if (BXGetSyncIDInitiator(InHeader.SkillID) != EBXSyncInitiator::Client || SkillMgr->GetSkillRuntimeDataByID(InHeader.SkillID) != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBXSkillComponent::HandleServerPlaySkill: invalid SkillID=%lld (initiator=%d, exists=%s)."), InHeader.SkillID, (int32)BXGetSyncIDInitiator(InHeader.SkillID), SkillMgr->GetSkillRuntimeDataByID(InHeader.SkillID) ? TEXT("true") : TEXT("false"));
		ClientPredictResult(InHeader.SkillID, false, Header.ServerTimestamp, -1.0f);
		return;
	}

	// 冷却校验(私有状态)
	if (!IsCooldownReady(SkillAsset))
	{
		UE_LOG(LogTemp, Warning, TEXT("UBXSkillComponent::HandleServerPlaySkill: SkillAsset=%s is on cooldown."), *SkillAsset->GetName());
		// 冷却拒绝:带回服务器权威冷却剩余,客户端假冷却同步为真冷却
		ClientPredictResult(InHeader.SkillID, false, Header.ServerTimestamp, GetCooldownRemainingSeconds(SkillAsset));
		return;
	}

	// 服务器校验(时间戳+释放条件)
	if (!SkillMgr->ServerValidateRelease(SkillAsset, GetOwner(), InPayload, InHeader.ClientTimestamp))
	{
		ClientPredictResult(InHeader.SkillID, false, Header.ServerTimestamp, -1.0f);
		return;
	}

	// 校验通过,播放技能
	int64 SkillID = SkillMgr->InternalPlaySkill(SkillAsset, GetOwner(), InPayload, InHeader.SkillID, InHeader.Initiator);
	if (SkillID == INDEX_NONE)
	{
		ClientPredictResult(InHeader.SkillID, false, Header.ServerTimestamp, -1.0f);
		return;
	}

	// 记录冷却
	RecordCooldown(SkillAsset);

	// 服务器登记该技能(客户端发起的技能同样纳入生命周期管理,Actor销毁时EndPlay会停止它们)
	OwnedSkillIDs.Add(SkillID);

	// 通知客户端预测成功,携带权威冷却剩余(客户端假冷却转正)
	const float CooldownRemaining = SkillAsset->Cooldown >= 0.0f ? SkillAsset->Cooldown : -1.0f;
	ClientPredictResult(InHeader.SkillID, true, Header.ServerTimestamp, CooldownRemaining);

	// 服务器加速弥补
	FBXSkillRuntimeData* Data = SkillMgr->GetSkillRuntimeDataByID(SkillID);
	if (Data)
	{
		Data->ServerTimestamp = Header.ServerTimestamp;
		SkillMgr->ServerAccelerate(*Data);
	}

	// 广播给所有客户端
	MulticastPlaySkill(Header, InPayload);
}

void UBXSkillComponent::HandleClientPlaySkill(const FBXSkillSyncHeader& InHeader, FBXSkillSyncPayload InPayload)
{
	UBXSkillManager* SkillMgr = UBXSkillManager::Get(this);
	if (!SkillMgr)
	{
		return;
	}

	// 查找技能资产
	UBXTLManager* TLMgr = UBXTLManager::Get(this);
	if (!TLMgr)
	{
		return;
	}

	TSoftObjectPtr<UBXTLAsset> AssetPtr = TLMgr->GetSkillAssetByID(InHeader.SkillAssetID);
	UBXSkillAsset* SkillAsset = Cast<UBXSkillAsset>(AssetPtr.Get());
	if (!SkillAsset)
	{
		SkillAsset = Cast<UBXSkillAsset>(AssetPtr.LoadSynchronous());
	}

	if (!SkillAsset)
	{
		return;
	}

	// 播放技能(移动语义,InPayload后续不再使用)
	int64 SkillID = SkillMgr->InternalPlaySkill(SkillAsset, GetOwner(), MoveTemp(InPayload), InHeader.SkillID, InHeader.Initiator);
	if (SkillID == INDEX_NONE)
	{
		return;
	}

	OwnedSkillIDs.Add(SkillID);

	// 客户端加速弥补
	FBXSkillRuntimeData* Data = SkillMgr->GetSkillRuntimeDataByID(SkillID);
	if (Data)
	{
		SkillMgr->ClientAccelerate(*Data, InHeader.ServerTimestamp);
	}
}

void UBXSkillComponent::RebuildSkillFromState(const FBXSkillReplicatedState& InState)
{
	AActor* Owner = GetOwner();
	if (!Owner || Owner->GetLocalRole() == ENetRole::ROLE_Authority)
	{
		return;
	}

	UBXSkillManager* SkillMgr = UBXSkillManager::Get(this);
	if (!SkillMgr)
	{
		return;
	}

	// 反投影恢复运行数据并续跑(已存在的ID在重建内部按去重跳过)
	int64 SkillID = SkillMgr->RebuildSkillFromProjection(InState, Owner);
	if (SkillID != INDEX_NONE)
	{
		OwnedSkillIDs.Add(SkillID);
	}
}

void UBXSkillComponent::StopSkillIfNotPredicting(int64 InSkillID)
{
	UBXSkillManager* SkillMgr = UBXSkillManager::Get(this);
	if (!SkillMgr)
	{
		return;
	}

	// 预测中的实例不因快照消失而停止(其生命周期由预测确认/回滚/超时管理)
	FBXSkillRuntimeData* Data = SkillMgr->GetSkillRuntimeDataByID(InSkillID);
	if (!Data || Data->PredictState == EBXPredictState::Predicting)
	{
		return;
	}

	SkillMgr->StopSkill(InSkillID, EBXTLFinishReason::FR_Interrupt);
	OwnedSkillIDs.Remove(InSkillID);
}

#pragma endregion Internal



#pragma region Lifecycle
void UBXSkillComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UBXSkillManager* SkillMgr = UBXSkillManager::Get(this);
	if (SkillMgr)
	{
		for (int64 SkillID : OwnedSkillIDs)
		{
			SkillMgr->StopSkill(SkillID, EBXTLFinishReason::FR_Interrupt);
		}
	}

	OwnedSkillIDs.Empty();

	Super::EndPlay(EndPlayReason);
}

#pragma endregion Lifecycle



#pragma region Cooldown
void UBXSkillComponent::RecordCooldown(UBXSkillAsset* InAsset)
{
	if (!InAsset || InAsset->Cooldown < 0.0f)
	{
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	float ExpireTime = World->GetTimeSeconds() + InAsset->Cooldown;
	CooldownMap.Add(InAsset->ID, ExpireTime);
}

void UBXSkillComponent::ClearCooldown(UBXSkillAsset* InAsset)
{
	if (!InAsset)
	{
		return;
	}

	CooldownMap.Remove(InAsset->ID);
}

bool UBXSkillComponent::IsCooldownReady(UBXSkillAsset* InAsset) const
{
	if (!InAsset)
	{
		return false;
	}

	if (InAsset->Cooldown < 0.0f)
	{
		return true;
	}

	const float* ExpireTime = CooldownMap.Find(InAsset->ID);
	if (!ExpireTime)
	{
		return true;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return true;
	}

	return World->GetTimeSeconds() >= *ExpireTime;
}

bool UBXSkillComponent::IsCooldownPending(UBXSkillAsset* InAsset) const
{
	return InAsset && PendingCooldownAssetIDs.Contains(InAsset->ID);
}

float UBXSkillComponent::GetCooldownRemainingSeconds(UBXSkillAsset* InAsset) const
{
	if (!InAsset)
	{
		return -1.0f;
	}

	const float* ExpireTime = CooldownMap.Find(InAsset->ID);
	if (!ExpireTime)
	{
		return -1.0f;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return -1.0f;
	}

	return FMath::Max(0.0f, *ExpireTime - World->GetTimeSeconds());
}

#pragma endregion Cooldown
