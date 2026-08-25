#include "BXProjectileComponent.h"

#include "BXProjectileManager.h"
#include "BXSubSystem.h"
#include "BXFunctionLibrary.h"
#include "BXNetStructs.h"

#include "Net/UnrealNetwork.h"
#include "Engine/NetDriver.h"



#pragma region API
UBXProjectileComponent::UBXProjectileComponent()
{
	// 组件启用复制(UE5.4的bReplicates是private)
	SetIsReplicatedByDefault(true);
}

int64 UBXProjectileComponent::FireProjectile(FGameplayTag InProjectileType, FVector InStartLocation, FVector InFireDirection, FVector InFireUpVector, AActor* InTarget, FInstancedStruct InContextData, AActor* InInstigator, AActor* InTriggerer)
{
	UBXProjectileManager* ProjectileMgr = UBXProjectileManager::Get(this);
	if (!ProjectileMgr)
	{
		return 0;
	}

	// 组装发射上下文
	FBXProjectileFireContext Context;
	Context.Instigator = InInstigator ? InInstigator : GetOwner();
	Context.Triggerer = InTriggerer;
	Context.StartLocation = InStartLocation;
	Context.FireDirection = InFireDirection.GetSafeNormal();
	Context.FireUpVector = InFireUpVector.GetSafeNormal();
	Context.Target = InTarget;
	Context.ClientTimestamp = UBXFunctionLibrary::GetServerWorldTimeMilliseconds(this);
	Context.ContextData = MoveTemp(InContextData);

	if (Context.FireDirection.IsNearlyZero())
	{
		Context.FireDirection = FVector::ForwardVector;
	}

	if (Context.FireUpVector.IsNearlyZero())
	{
		Context.FireUpVector = FVector::UpVector;
	}

	// 权威端(服务器/单机):直接权威发射(内含组播)
	const ENetRole OwnerRole = GetOwnerRole();
	if (OwnerRole == ROLE_Authority)
	{
		return ProjectileMgr->FireProjectile(InProjectileType, Context, this);
	}

	// 仿真代理无发射权
	if (OwnerRole != ROLE_AutonomousProxy)
	{
		return 0;
	}

	// 客户端预测发射:本地立即模拟,同时发RPC等待服务器确认/否认
	const int64 PredictedID = BXMakeSyncID(UBXFunctionLibrary::GetUniqueID(), EBXSyncInitiator::Client);

	FBXProjectileSyncHeader Header;
	Header.ProjectileID = PredictedID;
	Header.ProjectileType = InProjectileType;
	Header.Initiator = EBXSyncInitiator::Client;
	Header.ClientTimestamp = Context.ClientTimestamp;

	if (ProjectileMgr->StartPredictedProjectile(InProjectileType, Context, PredictedID, this) == 0)
	{
		return 0;
	}

	ServerFireProjectile(Header, Context);
	return PredictedID;
}

#pragma endregion API



#pragma region RPC Fire
// UE5.8: _Validate/_Implementation 定义签名须与UHT生成的按值声明一致(原const&定义与生成声明不匹配)
bool UBXProjectileComponent::ServerFireProjectile_Validate(FBXProjectileSyncHeader InHeader, FBXProjectileFireContext InContext)
{
	return InHeader.ProjectileID != 0 && InHeader.ProjectileType.IsValid();
}

void UBXProjectileComponent::ServerFireProjectile_Implementation(FBXProjectileSyncHeader InHeader, FBXProjectileFireContext InContext)
{
	UBXProjectileManager* ProjectileMgr = UBXProjectileManager::Get(this);
	if (!ProjectileMgr)
	{
		return;
	}

	// 校验失败:否认回滚(仅发往发起端)
	EBXProjectileDenyReason DenyReason = EBXProjectileDenyReason::DR_DuplicateID;
	if (!ProjectileMgr->HandleServerFireProjectile(InHeader, InContext, this, DenyReason))
	{
		ClientDenyProjectile(InHeader.ProjectileID, static_cast<uint8>(DenyReason));
	}
}

#pragma endregion RPC Fire



#pragma region RPC HitReport
// UE5.8: _Validate 定义签名须与UHT生成的按值声明一致(原const&定义与生成声明不匹配)
bool UBXProjectileComponent::ServerReportProjectileHit_Validate(FBXProjectileHitPayload InPayload)
{
	return InPayload.ProjectileID != 0;
}

void UBXProjectileComponent::ServerReportProjectileHit_Implementation(FBXProjectileHitPayload InPayload)
{
	UBXProjectileManager* ProjectileMgr = UBXProjectileManager::Get(this);
	if (ProjectileMgr)
	{
		ProjectileMgr->HandleServerReportProjectileHit(InPayload, this);
	}
}

#pragma endregion RPC HitReport



#pragma region RPC Deny
void UBXProjectileComponent::ClientDenyProjectile_Implementation(int64 InProjectileID, uint8 InDenyReason)
{
	UBXProjectileManager* ProjectileMgr = UBXProjectileManager::Get(this);
	if (ProjectileMgr)
	{
		ProjectileMgr->HandleProjectileDenied(InProjectileID);
	}
}

#pragma endregion RPC Deny



#pragma region RPC Multicast
void UBXProjectileComponent::MulticastFireProjectile_Implementation(FBXProjectileSyncHeader InHeader, FBXProjectileFireContext InContext)
{
	UBXProjectileManager* ProjectileMgr = UBXProjectileManager::Get(this);
	if (!ProjectileMgr)
	{
		return;
	}

	// 已存在(本地预测弹/服务器已发射):预测弹清除等待标记,本端不重建
	if (ProjectileMgr->IsProjectileRunning(InHeader.ProjectileID))
	{
		ProjectileMgr->ConfirmPredictedProjectile(InHeader.ProjectileID);
		return;
	}

	ProjectileMgr->StartRemoteProjectile(InHeader, InContext, this);
}

void UBXProjectileComponent::MulticastProjectileHit_Implementation(FBXProjectileHitPayload InPayload, bool bTerminal)
{
	UBXProjectileManager* ProjectileMgr = UBXProjectileManager::Get(this);
	if (ProjectileMgr)
	{
		ProjectileMgr->HandleHitMulticast(InPayload, bTerminal);
	}
}

void UBXProjectileComponent::MulticastStopProjectile_Implementation(int64 InProjectileID, uint8 InReason)
{
	UBXProjectileManager* ProjectileMgr = UBXProjectileManager::Get(this);
	if (ProjectileMgr)
	{
		ProjectileMgr->HandleStopMulticast(InProjectileID);
	}
}

void UBXProjectileComponent::MulticastProjectileSnapshots_Implementation(FBXProjectileSnapshotBatch InBatch)
{
	UBXProjectileManager* ProjectileMgr = UBXProjectileManager::Get(this);
	if (ProjectileMgr)
	{
		ProjectileMgr->HandleSnapshotMulticast(InBatch);
	}
}

#pragma endregion RPC Multicast



#pragma region Internal
void UBXProjectileComponent::OnRep_RunningProjectileStates(TArray<FBXProjectileReplicatedState> InOldStates)
{
	UBXProjectileManager* ProjectileMgr = UBXProjectileManager::Get(this);
	if (!ProjectileMgr)
	{
		return;
	}

	// 新增条目:确定性重放重建(Late Join续跑)
	for (const FBXProjectileReplicatedState& State : RunningProjectileStates)
	{
		bool bExisted = false;
		for (const FBXProjectileReplicatedState& OldState : InOldStates)
		{
			if (OldState.ProjectileID == State.ProjectileID)
			{
				bExisted = true;
				break;
			}
		}

		if (!bExisted)
		{
			ProjectileMgr->RebuildProjectileFromState(State, this);
		}
	}

	// 消失条目:非预测弹兜底移除(服务器侧已结束,组播乱序保护)
	for (const FBXProjectileReplicatedState& OldState : InOldStates)
	{
		bool bStillRunning = false;
		for (const FBXProjectileReplicatedState& State : RunningProjectileStates)
		{
			if (State.ProjectileID == OldState.ProjectileID)
			{
				bStillRunning = true;
				break;
			}
		}

		if (!bStillRunning)
		{
			ProjectileMgr->RemoveProjectileIfNotPredicted(OldState.ProjectileID);
		}
	}
}

#pragma endregion Internal



#pragma region Lifecycle
void UBXProjectileComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 仅初始同步发送:新连接拿最新快照重建,已有连接零属性流量(动态由显式RPC维护)
	DOREPLIFETIME_CONDITION(UBXProjectileComponent, RunningProjectileStates, COND_InitialOnly);
}

void UBXProjectileComponent::PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker)
{
	Super::PreReplication(ChangedPropertyTracker);

	AActor* Owner = GetOwner();
	if (!Owner || Owner->GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	// InitialOnly仅新连接消费:重建触发条件为"脏标记(结构变更)或远程连接数增加(新客户端连入)"
	// 常规帧零重建开销;时序:连接加入ClientConnections后才在后续ServerReplicateActors打开通道,
	// PreReplication在同一次flush序列化前运行,当帧重建即被新连接消费,无窗口期
	UNetDriver* NetDriver = GetWorld() ? GetWorld()->GetNetDriver() : nullptr;
	if (!NetDriver)
	{
		return;
	}

	const int32 ConnectionCount = NetDriver->ClientConnections.Num();
	const bool bNewConnection = ConnectionCount > LastProjectedConnectionCount;
	// 断线回落仅同步计数不重建(InitialOnly已发收不回);不回落则"断N+连N"净计数不变会漏触发
	LastProjectedConnectionCount = ConnectionCount;

	if (!bReplicatedStatesDirty && !bNewConnection)
	{
		return;
	}
	bReplicatedStatesDirty = false;

	UBXProjectileManager* ProjectileMgr = UBXProjectileManager::Get(this);
	if (!ProjectileMgr)
	{
		return;
	}

	ProjectileMgr->BuildReplicatedStates(Owner, RunningProjectileStates);
}

#pragma endregion Lifecycle
