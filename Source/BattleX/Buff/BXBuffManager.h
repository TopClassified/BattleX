#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Tickable.h"

#include "BXManager.h"
#include "BXBuffEnums.h"
#include "BXBuffStructs.h"

#include "BXBuffManager.generated.h"



// BUFF管理器日志分类
DECLARE_LOG_CATEGORY_EXTERN(BXMGR_Buff, Log, All);



class UBXBuffAsset;



// BUFF管理器(管理BUFF特有逻辑,Task执行复用UBXTLManager)
UCLASS(Blueprintable)
class BATTLEX_API UBXBuffManager : public UBXManager, public FTickableGameObject
{
	GENERATED_BODY()

#pragma region Important
public:
	// 获取管理器实例
	UFUNCTION(BlueprintCallable, Category = "BattleX|Buff")
	static UBXBuffManager* Get(UObject* InWorldContext);

	// 初始化
	virtual void Initialize() override;

	// 反初始化
	virtual void Deinitialize() override;

#pragma endregion Important



#pragma region Tick
public:
	// 每帧更新
	virtual void Tick(float DeltaTime) override;

	// 是否允许每帧更新
	virtual bool IsTickable() const override { return true; }

	// 是否在编辑器中也更新
	virtual bool IsTickableInEditor() const override { return false; }

	// 获取StatId
	virtual TStatId GetStatId() const override { RETURN_QUICK_DECLARE_CYCLE_STAT(UBXBuffManager, STATGROUP_Tickables); }

protected:
	// 更新单个BUFF的BUFF特有逻辑
	void InternalUpdateBuff(FBXBuffRuntimeData& InOutData, float InDeltaTime);

	// 清理已结束的BUFF
	void CleanBuffTrash();

	// 是否正在更新BUFF
	bool bUpdatingBuff = false;

#pragma endregion Tick



#pragma region Buff
public:
	// 施加BUFF
	UFUNCTION(BlueprintCallable, Category = "BattleX|Buff")
	int64 AddBuff(UBXBuffAsset* InAsset, AActor* InOwner, UPARAM(ref) FBXBuffPlayContext& InContext);

	// 施加BUFF(支持外部传入指定ID,用于同步)
	int64 AddBuffWithID(UBXBuffAsset* InAsset, AActor* InOwner, const FBXBuffPlayContext& InContext, int64 InBuffID);

	// 服务器端校验添加BUFF
	bool ServerValidateAddBuff(UBXBuffAsset* InAsset, AActor* InOwner, const FBXBuffPlayContext& InContext);

	// 移除BUFF
	UFUNCTION(BlueprintCallable, Category = "BattleX|Buff")
	void RemoveBuff(int64 InID, int32 InLayerDelta = 0);

	// 移除BUFF(携带移除原因,网络同步路径用,保证本地事件Reason与服务器一致)
	void RemoveBuffWithReason(int64 InID, EBXBuffRemoveReason InReason);

	// 改变层数
	UFUNCTION(BlueprintCallable, Category = "BattleX|Buff")
	void ChangeBuffLayer(int64 InID, int32 InLayerDelta);

	// 改变等级
	UFUNCTION(BlueprintCallable, Category = "BattleX|Buff")
	void ChangeBuffLevel(int64 InID, int32 InLevelDelta);

	// 刷新生命时长
	UFUNCTION(BlueprintCallable, Category = "BattleX|Buff")
	void RefreshBuffLifetime(int64 InID);

	// 是否拥有指定BUFF
	UFUNCTION(BlueprintCallable, Category = "BattleX|Buff")
	bool HasBuff(AActor* InOwner, UBXBuffAsset* InAsset) const;

	// 是否拥有指定Tag的BUFF
	UFUNCTION(BlueprintCallable, Category = "BattleX|Buff")
	bool HasBuffByTag(AActor* InOwner, FGameplayTag InTag) const;

	// 根据ID获取BUFF运行数据
	FBXBuffRuntimeData* GetBuffRuntimeDataByID(int64 InID);

	// 广播BUFF事件(单体+全局)
	void BroadcastBuffEvent(const FBXBuffRuntimeData& InData, const FGameplayTag& InEventTag, const FBXEventBuffChanged& InParam);

protected:
	// 查找已存在的BUFF(共存策略:相同Instigator)
	FBXBuffRuntimeData* FindExistingBuff(AActor* InOwner, UBXBuffAsset* InAsset, AActor* InInstigator);

	// 查找最佳替换BUFF(不共存策略:等级最高+剩余时长最长)
	FBXBuffRuntimeData* FindBestBuffToReplace(AActor* InOwner, UBXBuffAsset* InAsset);

	// 内部添加BUFF
	void InternalAddBuff(FBXBuffRuntimeData& InOutData, const FBXBuffPlayContext& InContext);

	// 内部移除BUFF
	void InternalRemoveBuff(FBXBuffRuntimeData& InOutData, EBXBuffRemoveReason InReason);

	// 层级变更时刷新Task(区间切换+RebuildEffect)
	void InternalRefreshBuffTasksByLayer(FBXBuffRuntimeData& InOutData, int32 InOldLayer, int32 InNewLayer);

#pragma endregion Buff



#pragma region TaskBridge
protected:
	// 启动符合当前层级的Task
	void StartBuffTasks(FBXBuffRuntimeData& InOutData);

	// 停止所有Task
	void StopBuffTasks(FBXBuffRuntimeData& InOutData);

	// 停止单个Task
	void StopBuffTask(FBXBuffRuntimeData& InOutData, UBXTask* InTask);

	// 执行单个Task(复用UBXTLManager::ExecuteTimelineTask)
	void ExecuteBuffTask(FBXBuffRuntimeData& InOutData, UBXTask* InTask);

	// 重建单个Task的效果(复用UBXTProcessor::RebuildEffectTask)
	void RebuildBuffTaskEffect(FBXBuffRuntimeData& InOutData, UBXTask* InTask, int32 InOldLayer, int32 InNewLayer);

	// 判断Task是否在层级区间内
	bool IsTaskInLayerRange(const FBXBuffTaskBinding& InBinding, int32 InLayer) const;

#pragma endregion TaskBridge



#pragma region Data
protected:
	// BUFF运行数据列表(纯数据)
	UPROPERTY(Transient)
	TMap<int64, FBXBuffRuntimeData> BuffRTDatas;

	// Owner到BUFF ID的反向索引
	TMap<TWeakObjectPtr<AActor>, TArray<int64>> OwnerBuffMap;

#pragma endregion Data
};
