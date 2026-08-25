#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "BXProjectileStructs.h"

#include "BXProjectileComponent.generated.h"



// 子弹组件(挂在Actor上承载子弹系统RPC通道与复制快照,发射入口)
UCLASS(ClassGroup = "BattleX", meta = (BlueprintSpawnableComponent))
class BATTLEX_API UBXProjectileComponent : public UActorComponent
{
	GENERATED_BODY()

#pragma region API
public:
	UBXProjectileComponent();

	// 发射子弹(权威端权威发射并组播,自主客户端预测发射后RPC,仿真代理无发射权)
	UFUNCTION(BlueprintCallable, Category = "BattleX|Projectile")
	int64 FireProjectile(FGameplayTag InProjectileType, FVector InStartLocation, FVector InFireDirection, FVector InFireUpVector, AActor* InTarget, FInstancedStruct InContextData, AActor* InInstigator = nullptr, AActor* InTriggerer = nullptr);

#pragma endregion API



#pragma region RPC Fire
public:
	// 发射请求(客户端预测后发往服务器校验)
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerFireProjectile(FBXProjectileSyncHeader InHeader, FBXProjectileFireContext InContext);

#pragma endregion RPC Fire



#pragma region RPC HitReport
public:
	// 命中上报(客户端代劳检测命中后上报服务器权威校验)
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerReportProjectileHit(FBXProjectileHitPayload InPayload);

#pragma endregion RPC HitReport



#pragma region RPC Deny
public:
	// 预测否认(校验失败回滚,仅发往发起端;reason参数预留差异化处理,当前统一回滚)
	UFUNCTION(Client, Reliable)
	void ClientDenyProjectile(int64 InProjectileID, uint8 InDenyReason);

#pragma endregion RPC Deny



#pragma region RPC Multicast
public:
	// 发射广播(全端启动模拟,已预测客户端按ID确认保留本地时间线)
	UFUNCTION(NetMulticast, Reliable)
	void MulticastFireProjectile(FBXProjectileSyncHeader InHeader, FBXProjectileFireContext InContext);

	// 命中广播(全端表现+终态流转,服务器权威结算)
	UFUNCTION(NetMulticast, Reliable)
	void MulticastProjectileHit(FBXProjectileHitPayload InPayload, bool bTerminal);

	// 显式停止广播(拦截/打断,自然结束不广播由双端自行模拟一致;reason参数预留差异化表现,当前统一停止)
	UFUNCTION(NetMulticast, Reliable)
	void MulticastStopProjectile(int64 InProjectileID, uint8 InReason);

	// 快照批次广播(长寿命子弹定时位置快照,客户端弹簧矫正)
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastProjectileSnapshots(FBXProjectileSnapshotBatch InBatch);

#pragma endregion RPC Multicast



#pragma region Internal
public:
	// 复制快照OnRep(带旧值差分:新增条目重建,消失条目兜底移除)
	UFUNCTION()
	void OnRep_RunningProjectileStates(TArray<FBXProjectileReplicatedState> InOldStates);

	// 标记复制快照脏(结构变更即时刷新,Manager发射/回收时调用)
	void MarkReplicatedStatesDirty() { bReplicatedStatesDirty = true; }

#pragma endregion Internal



#pragma region Lifecycle
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// 服务器端属性收集前:从Manager重建快照(配合COND_InitialOnly仅新连接初始同步发送)
	virtual void PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker) override;

#pragma endregion Lifecycle



#pragma region Data
protected:
	// 运行中子弹的复制快照(COND_InitialOnly:新复制对象初始同步重建用,已有连接由显式RPC维护)
	UPROPERTY(ReplicatedUsing=OnRep_RunningProjectileStates)
	TArray<FBXProjectileReplicatedState> RunningProjectileStates;

	// 复制快照脏标记(发射/回收结构变更置位,PreReplication即时重建)
	bool bReplicatedStatesDirty = false;

	// 上次投影时的远程连接数(-1保证组件首个复制周期必建一次基线快照;连接数增加才触发重建)
	int32 LastProjectedConnectionCount = -1;

#pragma endregion Data
};
