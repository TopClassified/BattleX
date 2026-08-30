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
#include "BXEventStructs.h"
#include "BXEventManager.h"
#include "Behavior/BXBehaviorComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/NetDriver.h"



UBXSkillComponent::UBXSkillComponent()
{
	SetIsReplicatedByDefault(true);

	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
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

	// 服务器端、属性收集前每帧执行
	AActor* Owner = GetOwner();
	if (!Owner || Owner->GetLocalRole() != ENetRole::ROLE_Authority)
	{
		return;
	}

	// RunningSkillStates为COND_InitialOnly:仅新连接初始同步时发送,已有连接零流量
	// 故仅当远程连接数增加(新客户端连入)时才重建快照,避免每帧每技能全量深拷贝DynamicDatas
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
		RebuildRunningSkillStates();
	}
	// 断线回落仅同步计数不重建(InitialOnly已发收不回);不回落则"断N+连N"净计数不变会漏触发
	LastProjectedConnectionCount = ConnectionCount;
}

void UBXSkillComponent::RebuildRunningSkillStates()
{
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
			UE_LOG(BXMGR_Skill, Log, TEXT("UBXSkillComponent::OnRep_RunningSkillStates: Rebuild entry. SkillID=%lld TimelineID=%d RunTime=%.2fs Sections=%d."), State.SkillID, State.TLRunTimeData.TimelineID, State.TLRunTimeData.RunTime, State.TLRunTimeData.RunningSections.Num());
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
			UE_LOG(BXMGR_Skill, Log, TEXT("UBXSkillComponent::OnRep_RunningSkillStates: Entry vanished, fallback stop. SkillID=%lld."), OldState.SkillID);
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
			UE_LOG(BXMGR_Skill, Log, TEXT("UBXSkillComponent::PlaySkillWithInputData: Authority rejected by cooldown. Asset=%s."), *InAsset->GetName());
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
		// 有技能运行→启用Tick(取消窗口边界管理)
		SetComponentTickEnabled(true);
		UE_LOG(BXMGR_Skill, Log, TEXT("UBXSkillComponent::PlaySkillWithInputData: Authority play. SkillID=%lld Asset=%s."), Header.SkillID, *InAsset->GetName());
		MulticastPlaySkill(Header, Payload);
		return Header.SkillID;
	}

	// 自主端:客户端预测
	if (LocalRole == ENetRole::ROLE_AutonomousProxy)
	{
		// 本地冷却检查防连点(服务器仍会权威校验)
		if (!IsCooldownReady(InAsset))
		{
			UE_LOG(BXMGR_Skill, Log, TEXT("UBXSkillComponent::PlaySkillWithInputData: Prediction rejected by local cooldown. Asset=%s."), *InAsset->GetName());
			return INDEX_NONE;
		}

		Header.SkillID = SkillMgr->StartPrediction(InAsset, Owner, Payload);
		if (Header.SkillID == INDEX_NONE)
		{
			return INDEX_NONE;
		}

		Header.Initiator = EBXSyncInitiator::Client;
		OwnedSkillIDs.Add(Header.SkillID);
		// 有技能运行→启用Tick(取消窗口边界管理)
		SetComponentTickEnabled(true);

		// 本地立即开始"假冷却"防止连点:服务器确认时转正为权威冷却,否认时移除
		RecordCooldown(InAsset);
		if (InAsset->Cooldown >= 0.0f)
		{
			PendingCooldownAssetIDs.Add(InAsset->ID);
			PendingCooldownSkills.Add(Header.SkillID, InAsset->ID);
		}

		UE_LOG(BXMGR_Skill, Log, TEXT("UBXSkillComponent::PlaySkillWithInputData: Prediction started. SkillID=%lld Asset=%s."), Header.SkillID, *InAsset->GetName());
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
	// 钳制输入数据规模,防止恶意客户端撑大服务器内存(超限断连)
	return InPayload.InputDatas.Num() <= 64;
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

	// 取回预测登记的资产ID(登记表独立于运行数据,预测超时回滚清理后迟到的结果仍可正确结算)
	int32 PendingAssetID = INDEX_NONE;
	if (const int32* FindResult = PendingCooldownSkills.Find(InSkillID))
	{
		PendingAssetID = *FindResult;
		PendingCooldownSkills.Remove(InSkillID);
	}

	if (bSuccess)
	{
		SkillMgr->ConfirmPrediction(InSkillID, InServerTimestamp);
		UE_LOG(BXMGR_Skill, Log, TEXT("UBXSkillComponent::ClientPredictResult: Confirmed. SkillID=%lld CooldownRemaining=%.2fs."), InSkillID, InCooldownRemaining);
	}
	else
	{
		SkillMgr->RollbackPrediction(InSkillID);
		OwnedSkillIDs.Remove(InSkillID);
		UE_LOG(BXMGR_Skill, Log, TEXT("UBXSkillComponent::ClientPredictResult: Denied, rolled back. SkillID=%lld ServerCooldownRemaining=%.2fs."), InSkillID, InCooldownRemaining);
	}

	// 假冷却结算:
	// InCooldownRemaining>=0: 确认=假冷却转正 / 冷却拒绝=同步服务器真冷却(以 本地时间+服务器剩余时长 为准)
	// InCooldownRemaining<0: 服务器侧无冷却(条件/时间戳等否认),移除假冷却允许重试
	if (PendingAssetID != INDEX_NONE)
	{
		if (InCooldownRemaining >= 0.0f)
		{
			UWorld* World = GetWorld();
			if (World)
			{
				CooldownMap.Add(PendingAssetID, World->GetTimeSeconds() + InCooldownRemaining);
			}
		}
		else
		{
			CooldownMap.Remove(PendingAssetID);
		}

		PendingCooldownAssetIDs.Remove(PendingAssetID);
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

	UE_LOG(BXMGR_Skill, Log, TEXT("UBXSkillComponent::MulticastStopSkill: Received. SkillID=%lld Reason=%d."), InSkillID, (int32)InFinishReason);

	// 停止本地实例(服务器端实例已停,StopSkill的bEarlyFinish守卫会跳过)
	SkillMgr->StopSkill(InSkillID, static_cast<EBXTLFinishReason>(InFinishReason));
	OwnedSkillIDs.Remove(InSkillID);
}

bool UBXSkillComponent::ServerReportCollisionResults_Validate(int64 InSkillID, int32 InTaskFullIndex, FGameplayTag InDataTag, FBXTHitResults InResults)
{
	// 钳制碰撞结果规模,防止恶意客户端撑大服务器内存(超限断连)
	return InResults.Results.Num() <= 100;
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
	UE_LOG(BXMGR_Skill, Log, TEXT("UBXSkillComponent::HandleServerPlaySkill: Server play. SkillID=%lld Asset=%s Initiator=%d."), SkillID, *SkillAsset->GetName(), (int32)InHeader.Initiator);

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
	UE_LOG(BXMGR_Skill, Log, TEXT("UBXSkillComponent::HandleClientPlaySkill: Remote play. SkillID=%lld Asset=%s."), SkillID, *SkillAsset->GetName());

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

void UBXSkillComponent::InternalOnSkillFinished(int64 InSkillID)
{
	OwnedSkillIDs.Remove(InSkillID);

	// 无运行技能→关闭Tick(取消窗口管理零开销)
	if (OwnedSkillIDs.IsEmpty())
	{
		SetComponentTickEnabled(false);
	}

	// 技能结束时预测结果仍未到达(超时回滚):释放假冷却允许重试,迟到的结果因无登记直接跳过结算
	int32 PendingAssetID = INDEX_NONE;
	if (PendingCooldownSkills.RemoveAndCopyValue(InSkillID, PendingAssetID))
	{
		CooldownMap.Remove(PendingAssetID);
		PendingCooldownAssetIDs.Remove(PendingAssetID);
	}
}

#pragma endregion Internal



#pragma region Lifecycle
void UBXSkillComponent::BeginPlay()
{
	Super::BeginPlay();

	// 技能互锁监听:姿态行为被挤出/挂起(Sign=本技能SkillID)→中断本技能
	if (UBXEventManager* EventMgr = UBXEventManager::Get(this))
	{
		BehaviorExitCallbackHandle = EventMgr->RegisterNativeSingleEvent(
			BXGameplayTags::BXEvent_Behavior_Exit,
			GetOwner(),
			this,
			[this](void* InData)
			{
				if (const FBXEventBehaviorChanged* Parameter = static_cast<const FBXEventBehaviorChanged*>(InData))
				{
					// 聚合转成员(捕获this的lambda,组件生命周期内安全)
					OnBehaviorExitEvent(*Parameter);
				}
			});
	}
}

void UBXSkillComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// 注销互锁监听
	if (UBXEventManager* EventMgr = UBXEventManager::Get(this))
	{
		EventMgr->UnregisterNativeSingleEvent(BXGameplayTags::BXEvent_Behavior_Exit, GetOwner(), BehaviorExitCallbackHandle);
	}

	UBXSkillManager* SkillMgr = UBXSkillManager::Get(this);
	if (SkillMgr)
	{
		for (int64 SkillID : OwnedSkillIDs)
		{
			SkillMgr->StopSkill(SkillID, EBXTLFinishReason::FR_Interrupt);
		}
	}

	OwnedSkillIDs.Empty();
	CancelWindowStates.Empty();

	Super::EndPlay(EndPlayReason);
}

void UBXSkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateCancelWindows(DeltaTime);
}

void UBXSkillComponent::UpdateCancelWindows(float InDeltaTime)
{
	// 无运行中技能时零开销(未启用Tick时同样不进入)
	if (OwnedSkillIDs.IsEmpty())
	{
		return;
	}

	UBXSkillManager* SkillMgr = UBXSkillManager::Get(this);
	UBXBehaviorComponent* BehaviorComp = GetOwner() ? GetOwner()->FindComponentByClass<UBXBehaviorComponent>() : nullptr;
	if (!SkillMgr || !BehaviorComp)
	{
		return;
	}

	// 快照遍历(窗口切换可能触发行为变更回调)
	TArray<int64> SkillIDs;
	SkillIDs.Reserve(OwnedSkillIDs.Num());
	for (int64 SkillID : OwnedSkillIDs)
	{
		SkillIDs.Add(SkillID);
	}

	for (int64 SkillID : SkillIDs)
	{
		FBXSkillRuntimeData* Data = SkillMgr->GetSkillRuntimeDataByID(SkillID);
		if (!Data || !Data->SkillAsset)
		{
			continue;
		}

		UBXSkillAsset* Asset = Data->SkillAsset;
		if (!Asset->BehaviorTag.IsValid() || Asset->CancelWindows.IsEmpty() || !Asset->bWaiveOnCancelWindow)
		{
			continue;
		}

		// 窗口判定(技能时间轴当前时间)
		const bool bInWindow = Asset->IsInCancelWindow(Data->TLRunTimeData.RunTime);

		// 边界切换才写豁免表(常规帧零开销;缓存初值false=窗口外未豁免,与豁免语义对齐)
		bool* CachedState = CancelWindowStates.Find(SkillID);
		if (!CachedState)
		{
			CachedState = &CancelWindowStates.Add(SkillID, false);
		}

		if (*CachedState != bInWindow)
		{
			*CachedState = bInWindow;
			BehaviorComp->SetBehaviorWaiver(Asset->BehaviorTag, SkillID, bInWindow);
		}
	}

	// 清理已结束技能的窗口缓存(CleanSkillTrash先移除OwnedSkillIDs条目)
	TArray<int64> StaleIDs;
	for (const TPair<int64, bool>& Pair : CancelWindowStates)
	{
		if (!OwnedSkillIDs.Contains(Pair.Key))
		{
			StaleIDs.Add(Pair.Key);
		}
	}
	for (int64 SkillID : StaleIDs)
	{
		CancelWindowStates.Remove(SkillID);
	}
}

void UBXSkillComponent::OnBehaviorExitEvent(const FBXEventBehaviorChanged& InParameter)
{
	// 互锁:本组件持有的技能姿态行为被挤出/中断→技能中断(Q1任一语义)
	if (!OwnedSkillIDs.Contains(InParameter.Sign))
	{
		return;
	}

	// 仅挤出/中断触发互锁(手动停止/清空是技能结束自身收束,不回路中断)
	if (InParameter.Reason != EBXBehaviorEndReason::BER_Expelled
		&& InParameter.Reason != EBXBehaviorEndReason::BER_Interrupted)
	{
		return;
	}

	UBXSkillManager* SkillMgr = UBXSkillManager::Get(this);
	if (!SkillMgr)
	{
		return;
	}

	// 姿态行为中断→技能互锁中断(行为Exit事件在StopBehavior管线内,StopSkill标记延迟清理,调用栈安全)
	SkillMgr->StopSkill(InParameter.Sign, EBXTLFinishReason::FR_Interrupt);
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
