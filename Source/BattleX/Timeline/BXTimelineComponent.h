#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "BXTLAsset.h"
#include "BXManager.h"

#include "BXTimelineComponent.generated.h" 



UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BATTLEX_API UBXTimelineComponent : public UActorComponent
{
	GENERATED_BODY()

#pragma region Important
public:	
	UBXTimelineComponent();

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

#pragma endregion Important
		
	

#pragma region Timeline
public:
	// 播放时间轴
	UFUNCTION(BlueprintCallable)
	virtual int64 PlayTimeline(UBXTLAsset* InAsset, UPARAM(ref) FBXTLPlayContext& InContext);

	// 停止时间轴
	UFUNCTION(BlueprintCallable)
	virtual void StopTimeline(int64 InID, EBXTLFinishReason InReason);

	// 接收到时间轴即将结束
	virtual void ReceiveTimelineWillFinish(int64 InID, EBXTLFinishReason InReason);
	UFUNCTION(BlueprintImplementableEvent)
	void ScriptReceiveTimelineWillFinish(int64 InID, EBXTLFinishReason InReason);

	// 根据时间轴资源，查询是否有正在运行的数据
	UFUNCTION(BlueprintCallable)
	virtual bool IsTimelineRunning(UBXTLAsset* InAsset);

	// 根据ID，查询是否有正在运行的数据
	UFUNCTION(BlueprintCallable)
	virtual bool IsTimelineRunningByID(int64 InID);

protected:
	// 时间轴运行数据ID列表
	UPROPERTY(Transient, BlueprintReadWrite)
	TArray<int64> TimelineRunTimeDataIDs;

#pragma endregion Timeline



#pragma region Event
public:
	UPROPERTY(BlueprintAssignable)
	FBXTimelineStarted TimelineStarted;

	UPROPERTY(BlueprintAssignable)
	FBXTimelineWillEnd TimelineWillEnd;

#pragma endregion Event

};
