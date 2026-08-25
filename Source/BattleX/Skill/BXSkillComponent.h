#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "BXSkillStructs.h"
#include "BXNetStructs.h"
#include "BXSkillReplicated.h"
#include "BXTStructs.h"

#include "BXSkillComponent.generated.h"



class UBXSkillAsset;



// 技能组件(挂在Actor上,提供BP友好API,承载同步RPC)
UCLASS(ClassGroup = "BattleX", meta = (BlueprintSpawnableComponent))
class BATTLEX_API UBXSkillComponent : public UActorComponent
{
	GENERATED_BODY()

#pragma region API
public:
	UBXSkillComponent();

	// 释放技能(无锁定)
	UFUNCTION(BlueprintCallable, Category = "BattleX|Skill")
	int64 PlaySkill(UBXSkillAsset* InAsset, AActor* InInstigator = nullptr, AActor* InTriggerer = nullptr);

	// 释放技能(锁定目标)
	UFUNCTION(BlueprintCallable, Category = "BattleX|Skill")
	int64 PlaySkillWithTarget(UBXSkillAsset* InAsset, UPARAM(ref) TArray<FBXBodyPartSelection>& InLockParts, AActor* InInstigator = nullptr, AActor* InTriggerer = nullptr);

	// 释放技能(锁定位置)
	UFUNCTION(BlueprintCallable, Category = "BattleX|Skill")
	int64 PlaySkillWithLocation(UBXSkillAsset* InAsset, FVector InAimLocation, AActor* InInstigator = nullptr, AActor* InTriggerer = nullptr);

	// 释放技能(锁定朝向)
	UFUNCTION(BlueprintCallable, Category = "BattleX|Skill")
	int64 PlaySkillWithDirection(UBXSkillAsset* InAsset, FRotator InAimRotation, AActor* InInstigator = nullptr, AActor* InTriggerer = nullptr);

	// 释放技能(统一收束口,所有输入数据通过InputDatas传递)
	// 按值传参并在内部移动:C++调用方可MoveTemp传入避免拷贝,蓝图调用方传入的Map不受影响
	UFUNCTION(BlueprintCallable, Category = "BattleX|Skill")
	int64 PlaySkillWithInputData(UBXSkillAsset* InAsset, TMap<FGameplayTag, FInstancedStruct> InInputDatas, AActor* InInstigator = nullptr, AActor* InTriggerer = nullptr);

	// 停止技能
	UFUNCTION(BlueprintCallable, Category = "BattleX|Skill")
	void StopSkill(int64 InSkillID);

	// 是否正在播放指定技能
	UFUNCTION(BlueprintCallable, Category = "BattleX|Skill")
	bool IsSkillRunning(int64 InSkillID) const;

	// 查询指定技能的冷却是否为"假冷却"(本地预测已开始,等待服务器确认/否认)
	UFUNCTION(BlueprintPure, Category = "BattleX|Skill")
	bool IsCooldownPending(UBXSkillAsset* InAsset) const;

#pragma endregion API



#pragma region RPC SkillPlay
public:
	// 释放技能(单一RPC,所有数据通过Payload传递)
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerPlaySkill(FBXSkillSyncHeader InHeader, FBXSkillSyncPayload InPayload);

#pragma endregion RPC SkillPlay



#pragma region RPC PredictResult
public:
	// 预测结果(Reliable,仅发给发起端)。InCooldownRemaining>=0为服务器权威冷却剩余时长(秒):
	// 确认时用于假冷却转正,冷却拒绝时用于同步真冷却;-1代表无服务器冷却(否认时移除假冷却)
	UFUNCTION(Client, Reliable)
	void ClientPredictResult(int64 InSkillID, bool bSuccess, int64 InServerTimestamp, float InCooldownRemaining);

#pragma endregion RPC PredictResult



#pragma region RPC SkillMulticast
public:
	// 广播给所有客户端(合并后的完整Payload)。Reliable:丢包会导致远端客户端永远看不到该技能
	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlaySkill(FBXSkillSyncHeader InHeader, FBXSkillSyncPayload InPayload);

	// 广播技能中断(服务器主动Stop/Actor死亡/条件强制中断时,远端客户端同步停止)
	UFUNCTION(NetMulticast, Reliable)
	void MulticastStopSkill(int64 InSkillID, uint8 InFinishReason);

#pragma endregion RPC SkillMulticast



#pragma region RPC CollisionReport
public:
	// 碰撞结果上报
	UFUNCTION(Server, Unreliable, WithValidation)
	void ServerReportCollisionResults(int64 InSkillID, int32 InTaskFullIndex, FGameplayTag InDataTag, FBXTHitResults InResults);

#pragma endregion RPC CollisionReport



#pragma region Internal
protected:
	// 处理服务器收到释放RPC后的校验与执行
	void HandleServerPlaySkill(const FBXSkillSyncHeader& InHeader, const FBXSkillSyncPayload& InPayload);

	// 处理客户端收到广播后的播放(值传递以支持MoveTemp,消除InternalPlaySkill最后一次深拷贝)
	void HandleClientPlaySkill(const FBXSkillSyncHeader& InHeader, FBXSkillSyncPayload InPayload);

	// 复制快照OnRep(带旧值差分:新增条目重建,消失条目兜底停止)
	UFUNCTION()
	void OnRep_RunningSkillStates(TArray<FBXSkillReplicatedState> InOldStates);

	// 从SkillRTDatas重建RunningSkillStates快照(新客户端连入时调用)
	void RebuildRunningSkillStates();

public:
	// 根据复制状态重建进行中的技能(新复制到本地的对象初始化用)
	void RebuildSkillFromState(const FBXSkillReplicatedState& InState);

	// 停止本地技能(预测中的实例除外;快照条目消失的乱序兜底)
	void StopSkillIfNotPredicting(int64 InSkillID);

	// 技能实例结束通知(Manager清理运行数据时调用):移除OwnedSkillIDs登记,预测超时未结算的假冷却一并释放
	void InternalOnSkillFinished(int64 InSkillID);

#pragma endregion Internal



#pragma region Lifecycle
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// 服务器端、属性收集前每帧调用:从SkillRTDatas重建快照数组为最新运行数据投影
	// (配合COND_InitialOnly:仅新连接初始同步发送,已有连接零属性流量)
	virtual void PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

#pragma endregion Lifecycle



#pragma region Data
protected:
	// 本组件持有的技能实例ID列表
	UPROPERTY(Transient)
	TArray<int64> OwnedSkillIDs;

	// 运行中技能的复制快照(COND_InitialOnly:新复制到客户端的对象初始同步时重建用,
	// 已有连接的技能动态由显式RPC维护,详见BXSkillReplicated.h文件头注释)
	UPROPERTY(ReplicatedUsing=OnRep_RunningSkillStates)
	TArray<FBXSkillReplicatedState> RunningSkillStates;

	// 上次投影时的远程连接数(-1保证组件首个复制周期必建一次基线快照;连接数增加才重建快照)
	int32 LastProjectedConnectionCount = -1;

#pragma endregion Data



#pragma region Cooldown
public:
	// 记录冷却(服务器端调用,客户端预测成功后本地调用以防连点)
	void RecordCooldown(UBXSkillAsset* InAsset);

	// 清除冷却记录(预测被服务器否认且无服务器冷却时调用)
	void ClearCooldown(UBXSkillAsset* InAsset);

	// 查询冷却是否结束
	bool IsCooldownReady(UBXSkillAsset* InAsset) const;

protected:
	// 查询指定技能的冷却剩余时长(秒,无冷却记录返回-1)
	float GetCooldownRemainingSeconds(UBXSkillAsset* InAsset) const;

	// 冷却表:SkillAssetID → 冷却到期时间
	UPROPERTY(Transient)
	TMap<int32, float> CooldownMap;

	// 假冷却中的资产ID集合(本地预测已开始计时,等待服务器确认转正或否认移除)
	UPROPERTY(Transient)
	TSet<int32> PendingCooldownAssetIDs;

	// 预测中技能登记表:SkillID → SkillAssetID(假冷却结算依据,不依赖运行数据存活,预测超时回滚后迟到的结果仍可正确结算)
	UPROPERTY(Transient)
	TMap<int64, int32> PendingCooldownSkills;

#pragma endregion Cooldown

};
