#pragma once

#include "CoreMinimal.h"
#include "Tickable.h"
#include "Engine/World.h"

#include "BXTLStructs.h"

#include "BXManager.generated.h" 



DECLARE_LOG_CATEGORY_EXTERN(BX_MGR, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBXTimelineStarted, int64, TimelineID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBXTimelineWillEnd, int64, TimelineID, EBXTLFinishReason, Reason);



UCLASS(Blueprintable)
class BATTLEX_API UBXManager : public UObject, public FTickableGameObject
{
	GENERATED_BODY()

#pragma region Important
public:
	UFUNCTION(BlueprintCallable)
	static UBXManager* Get(UObject* InWorldContext);

	virtual void Initialize();

	virtual void Deinitialize();

#pragma endregion Important



#pragma region Tick
public:
	virtual UWorld* GetTickableGameObjectWorld() const override;

	virtual ETickableTickType GetTickableTickType() const override;

	virtual bool IsAllowedToTick() const override;

	virtual TStatId GetStatId() const override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
	void ScriptTick(float DeltaTime);

protected:
	// 垃圾清理时间间隔
	UPROPERTY(EditDefaultsOnly)
	float GCInterval = 20.0f;

	// 垃圾清理计时器
	float GCTimer = 0.0f;

#pragma endregion Tick



#pragma region Asset
public:
	// 根据ID找到时间轴资源
	UFUNCTION(BlueprintCallable)
	TSoftObjectPtr<class UBXTLAsset> GetTimelineAssetByID(int32 InID);

	// 根据ID找到技能资源
	UFUNCTION(BlueprintCallable)
	TSoftObjectPtr<class UBXTLAsset> GetSkillAssetByID(int32 InID);

	// 获取时间轴资源ID列表
	UFUNCTION(BlueprintCallable)
	TArray<int32> GetTimelineAssetIDs();

	// 获取技能资源ID列表
	UFUNCTION(BlueprintCallable)
	TArray<int32> GetSkillAssetIDs();

protected:
	void CollectTimelineAssetPath();

protected:
	// 时间轴资源路径查询表
	UPROPERTY(Transient)
	TMap<int32, FBXTimelineAssetInformation> TimelineAssetMap;

#pragma endregion Asset



#pragma region Timeline
public:
	// 正在更新时间轴运行数据
	UFUNCTION(BlueprintCallable)
	bool IsUpdatingTimeline() const;

	// 播放时间轴
	UFUNCTION(BlueprintCallable)
	int64 PlayTimeline(UBXTLAsset* InAsset, AActor* InOwner, UPARAM(ref) FBXTLPlayContext& InContext);

	// 停止时间轴
	UFUNCTION(BlueprintCallable)
	void StopTimeline(int64 InID, EBXTLFinishReason InReason);

	// 改变时间轴更新速率
	UFUNCTION(BlueprintCallable)
	void ChangeTimelineTickRate(int64 InID, float InRate);
	
	// 更新时间轴
	void UpdateTimeline(float InDeltaTime);

	// 结束时间轴片段
	void FinishTimelineSection(FBXTLRunTimeData& InOutData, FBXTLSectionRTData& InOutSectionData, EBXTLFinishReason InReason);

	// 处理时间片段待执行任务
	void ProcessTimelineSectionPendingTasks(FBXTLRunTimeData& InOutData, FBXTLSectionRTData& InOutSectionData);

	// 执行任务
	bool ExecuteTimelineTask(FBXTLRunTimeData& InOutData, FBXTLSectionRTData& InOutSectionData, int32 InTaskIndex, ENetMode InNetMode, ENetRole InRoleType, float InStartOffset);

	// 根据任务类型获取处理器
	class UBXTProcessor* GetTLTProcessorByTLTClass(UClass* TaskClass);

	// 根据ID获取时间轴运行数据
	FBXTLRunTimeData* GetTimelineRunTimeDataByID(int64 InID);
	
protected:
	void InternalUpdateTimeline(FBXTLRunTimeData& InOutData, float InDeltaTime);

	void CleanTimelineTrash();
	
	void CleanTimeline();

protected:
	// 缓存任务处理器类型映射表
	UPROPERTY(Transient)
	TMap<UClass*, UClass*> TimelineTaskProcessorTypeMap;

	// 缓存任务自定义数据集类型映射表
	UPROPERTY(Transient)
	TMap<UClass*, UScriptStruct*> TimelineTaskTypeMap;

	// 任务处理器列表
	UPROPERTY(Transient)
	TMap<UClass*, class UBXTProcessor*> TimelineTaskProcessors;

	// 时间轴运行数据列表
	UPROPERTY(Transient)
	TMap<int64, FBXTLRunTimeData> TimelineRTDatas;

	// 托管的任务数据列表
	UPROPERTY(Transient)
	TArray<FBXTLTaskHostingData> TimelineTaskHostingDatas;
	// 运行托管任务所需的辅助结构体
	UPROPERTY(Transient)
	FBXTLRunTimeData HelpHostingData1;
	UPROPERTY(Transient)
	FBXTLSectionRTData HelpHostingData2;

	// 正在更新时间轴运行数据
	bool bUpdatingTimeline = false;

#pragma endregion Timeline



#pragma region Event
public:
	UPROPERTY(BlueprintAssignable)
	FBXTimelineStarted TimelineStarted;

	UPROPERTY(BlueprintAssignable)
	FBXTimelineWillEnd TimelineWillEnd;

#pragma endregion Event



#pragma region Callback
protected:
	UFUNCTION(BlueprintCallable)
	void OnWorldCleanupStart(UWorld* World, bool bSessionEnded, bool bCleanupResources);

#pragma endregion Callback



#pragma region Debug
public:
	UFUNCTION(BlueprintPure)
	bool GetShowCollision() const;
	
	UFUNCTION(BlueprintCallable)
	void ChangeShowCollision(bool InShow);

protected:
	bool bShowCollision = false;

#pragma endregion Debug

};
