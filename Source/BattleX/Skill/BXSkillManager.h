#pragma once

#include "CoreMinimal.h"
#include "Tickable.h"
#include "Engine/World.h"

#include "BXManager.h"
#include "BXSkillStructs.h"
#include "BXNetStructs.h"
#include "BXSkillReplicated.h"

#include "BXSkillManager.generated.h"



DECLARE_LOG_CATEGORY_EXTERN(BXMGR_Skill, Log, All);



class UBXSkillAsset;
class UBXConditionManager;



// 技能管理器(管理技能运行时数据与同步框架,Task执行复用UBXTLManager)
UCLASS(Blueprintable)
class BATTLEX_API UBXSkillManager : public UBXManager, public FTickableGameObject
{
	GENERATED_BODY()

#pragma region Important
public:
	// 获取管理器实例
	UFUNCTION(BlueprintCallable, Category = "BattleX|Skill")
	static UBXSkillManager* Get(UObject* InWorldContext);

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
	virtual TStatId GetStatId() const override { RETURN_QUICK_DECLARE_CYCLE_STAT(UBXSkillManager, STATGROUP_Tickables); }

	virtual UWorld* GetTickableGameObjectWorld() const override;

	virtual ETickableTickType GetTickableTickType() const override;

	virtual bool IsAllowedToTick() const override;

protected:
	// 更新单个技能
	void InternalUpdateSkill(FBXSkillRuntimeData& InOutData, float InDeltaTime);

	// 清理已结束的技能
	void CleanSkillTrash();

	// 检查预测超时
	void CheckPredictTimeout();

	// 合并更新期间挂起的新增技能入主容器(Tick末调用,纯数据搬移不触发回调)
	void MergePendingAddSkills();

	// 是否正在更新技能(更新窗口内Task回调中的PlaySkill入挂起区,避免TMap扩容rehash使遍历中数据引用悬空)
	bool bUpdatingSkill = false;

	// 更新期间挂起的新增技能(TUniquePtr保证对象地址稳定,回调链中持引用安全;Tick末合并入SkillRTDatas后清空)
	TArray<TUniquePtr<FBXSkillRuntimeData>> PendingAddSkills;

#pragma endregion Tick



#pragma region Play
public:
	// 本地播放技能(Standalone或服务器直接调用,不经过网络)
	UFUNCTION(BlueprintCallable, Category = "BattleX|Skill")
	int64 PlaySkill(UBXSkillAsset* InAsset, AActor* InOwner, const FBXSkillSyncPayload& InPayload);

	// 停止技能
	UFUNCTION(BlueprintCallable, Category = "BattleX|Skill")
	void StopSkill(int64 InSkillID, EBXTLFinishReason InReason);

	// 根据ID获取技能运行数据
	FBXSkillRuntimeData* GetSkillRuntimeDataByID(int64 InID);

	// 根据复制状态投影重建技能(新复制到客户端的对象初始化用,恢复运行数据直接续跑)
	// 返回SkillID;ID已存在时直接返回该ID(幂等),资产无效返回INDEX_NONE
	int64 RebuildSkillFromProjection(const FBXSkillReplicatedState& InState, AActor* InOwner);

	// 内部播放技能(支持外部传入指定ID,用于同步)
	int64 InternalPlaySkill(UBXSkillAsset* InAsset, AActor* InOwner, const FBXSkillSyncPayload& InPayload, int64 InSkillID, EBXSyncInitiator InInitiator);

	// 内部播放技能(移动语义重载,消除DynamicDatas写入深拷贝,调用方不可再使用InPayload)
	int64 InternalPlaySkill(UBXSkillAsset* InAsset, AActor* InOwner, FBXSkillSyncPayload&& InPayload, int64 InSkillID, EBXSyncInitiator InInitiator);

#pragma endregion Play



#pragma region Predict
public:
	// 客户端预测启动(由SkillComponent调用)
	int64 StartPrediction(UBXSkillAsset* InAsset, AActor* InOwner, const FBXSkillSyncPayload& InPayload);

	// 服务器确认预测成功
	void ConfirmPrediction(int64 InSkillID, int64 InServerTimestamp);

	// 服务器否认预测,执行回滚
	void RollbackPrediction(int64 InSkillID);

protected:
	// 内部回滚预测技能
	void InternalRollbackPredictedSkill(FBXSkillRuntimeData& InOutData);

#pragma endregion Predict



#pragma region Accelerate
public:
	// 服务器端加速弥补网络延迟
	void ServerAccelerate(FBXSkillRuntimeData& InOutData);

	// 其他客户端收到广播后加速
	void ClientAccelerate(FBXSkillRuntimeData& InOutData, int64 InBroadcastTimestamp);

protected:
	// 更新加速状态(每帧调用,到时恢复),返回本帧等效推进Delta(含加速弥补,卡顿保护)
	float UpdateAccelerate(FBXSkillRuntimeData& InOutData, float InDeltaTime);

	// 钳制加速时长上限为技能总时长的50%
	float ClampAccelerateDuration(const FBXSkillRuntimeData& InData, float InDuration);

#pragma endregion Accelerate



#pragma region Validate
public:
	// 服务器端释放校验(时间戳+释放条件,冷却校验由SkillComponent负责)
	bool ServerValidateRelease(UBXSkillAsset* InAsset, AActor* InOwner, const FBXSkillSyncPayload& InPayload, int64 InClientTimestamp);

#pragma endregion Validate



#pragma region Collision
public:
	// 接收客户端上报的碰撞结果
	void ReceiveCollisionResults(int64 InSkillID, int32 InTaskFullIndex, const FGameplayTag& InDataTag, const struct FBXTHitResults& InResults);

#pragma endregion Collision



#pragma region TaskBridge
protected:
	// 启动技能的所有Task(委托给UBXTLManager::StartTimelineSections)
	void StartSkillTasks(FBXSkillRuntimeData& InOutData);

	// 停止技能的所有Task(委托给UBXTLManager::FinishTimelineSection)
	void StopSkillTasks(FBXSkillRuntimeData& InOutData, EBXTLFinishReason InReason);

#pragma endregion TaskBridge



#pragma region Data
protected:
	// 技能运行数据列表
	UPROPERTY(Transient)
	TMap<int64, FBXSkillRuntimeData> SkillRTDatas;

	// Owner到SkillID的反向索引
	TMap<TWeakObjectPtr<AActor>, TArray<int64>> OwnerSkillMap;

#pragma endregion Data

};
