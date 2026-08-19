#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "BXBuffStructs.h"
#include "BXBuffReplicated.h"

#include "BXBuffComponent.generated.h"



class UBXBuffAsset;



// BUFF组件(挂在Actor上,提供BP友好API,转发到UBXBuffManager)
UCLASS(ClassGroup = "BattleX", meta = (BlueprintSpawnableComponent))
class BATTLEX_API UBXBuffComponent : public UActorComponent
{
	GENERATED_BODY()

#pragma region API
public:
	UBXBuffComponent();

	// 施加BUFF
	UFUNCTION(BlueprintCallable, Category = "BattleX|Buff")
	int64 AddBuff(UBXBuffAsset* InAsset, AActor* InInstigator = nullptr, int32 InLayer = 1, int32 InLevel = 1);

	// 移除BUFF(InLayerDelta<=0整体移除;>0时独立层级模式下移除指定层数,层数耗尽自动整体移除)
	UFUNCTION(BlueprintCallable, Category = "BattleX|Buff")
	void RemoveBuff(int64 InBuffID, int32 InLayerDelta = 0);

	// 改变层数
	UFUNCTION(BlueprintCallable, Category = "BattleX|Buff")
	void ChangeBuffLayer(int64 InBuffID, int32 InLayerDelta);

	// 改变等级
	UFUNCTION(BlueprintCallable, Category = "BattleX|Buff")
	void ChangeBuffLevel(int64 InBuffID, int32 InLevelDelta);

	// 刷新生命时长
	UFUNCTION(BlueprintCallable, Category = "BattleX|Buff")
	void RefreshBuffLifetime(int64 InBuffID);

	// 是否拥有指定BUFF
	UFUNCTION(BlueprintCallable, Category = "BattleX|Buff")
	bool HasBuff(UBXBuffAsset* InAsset) const;

	// 是否拥有指定Tag的BUFF
	UFUNCTION(BlueprintCallable, Category = "BattleX|Buff")
	bool HasBuffByTag(FGameplayTag InTag) const;

#pragma endregion API



#pragma region RPC RequestAddBuff
public:
	// 请求添加BUFF(传资产ID,服务器经注册表解析,防客户端伪造对象引用)
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRequestAddBuff(int32 InBuffAssetID, AActor* InInstigator, int32 InLayer, int32 InLevel);

#pragma endregion RPC RequestAddBuff



#pragma region RPC RequestRemoveBuff
public:
	// 请求移除BUFF
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRequestRemoveBuff(int64 InBuffID, int32 InLayerDelta);

#pragma endregion RPC RequestRemoveBuff



#pragma region RPC MulticastAddBuff
public:
	// 广播添加BUFF(传资产ID,接收端经注册表解析;资产对象引用非Actor/Component无法经RPC序列化)
	UFUNCTION(NetMulticast, Reliable)
	void MulticastAddBuff(int64 InBuffID, int32 InBuffAssetID, AActor* InOwner, AActor* InInstigator, int32 InLayer, int32 InLevel);

#pragma endregion RPC MulticastAddBuff



#pragma region RPC MulticastRemoveBuff
public:
	// 广播移除BUFF
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRemoveBuff(int64 InBuffID, uint8 InRemoveReason);

#pragma endregion RPC MulticastRemoveBuff



#pragma region Internal
public:
	// 根据复制状态重建进行中的BUFF(新复制到本地的对象初始化用)
	void RebuildBuffFromState(const FBXBuffReplicatedState& InState);

	// 同步快照条目变化到已重建的本地实例(层/级/到期变化)
	void ApplyBuffStateChange(const FBXBuffReplicatedState& InState);

	// 移除本地存在的BUFF(快照条目消失的乱序兜底)
	void RemoveBuffIfLocalExists(int64 InBuffID);

	// BUFF实例结束通知(Manager清理运行数据时调用):移除OwnedBuffIDs登记,否则自然到期的BUFF ID永久残留
	void InternalOnBuffFinished(int64 InBuffID);

	// 服务器维护快照条目:BUFF添加后加入
	void AddBuffReplicatedState(int64 InBuffID);

	// 服务器维护快照条目:状态变化(层/级/时长刷新)后刷新
	void UpdateBuffReplicatedState(int64 InBuffID);

	// 服务器维护快照条目:BUFF移除后删除
	void RemoveBuffReplicatedState(int64 InBuffID);

protected:
	// 复制快照OnRep(带旧值差分:新增条目重建,变化条目同步状态,消失条目兜底移除)
	UFUNCTION()
	void OnRep_RunningBuffStates(TArray<FBXBuffReplicatedState> InOldStates);

#pragma endregion Internal



#pragma region Lifecycle
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// 组件销毁时移除所有BUFF
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

#pragma endregion Lifecycle



#pragma region Data
protected:
	// 本组件持有的BUFF实例ID列表
	UPROPERTY(Transient)
	TArray<int64> OwnedBuffIDs;

	// 运行中BUFF的复制快照(无条件复制:低频变化承担初始重建+层/级/到期持续同步)
	UPROPERTY(ReplicatedUsing=OnRep_RunningBuffStates)
	TArray<FBXBuffReplicatedState> RunningBuffStates;

#pragma endregion Data
};
