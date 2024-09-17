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
	// ����ʱ����
	UFUNCTION(BlueprintCallable)
	virtual int64 PlayTimeline(UBXTLAsset* InAsset, UPARAM(ref) FBXTLPlayContext& InContext);

	// ֹͣʱ����
	UFUNCTION(BlueprintCallable)
	virtual void StopTimeline(int64 InID, EBXTLFinishReason InReason);

	// ���յ�ʱ���ἴ������
	virtual void ReceiveTimelineWillFinish(int64 InID, EBXTLFinishReason InReason);
	UFUNCTION(BlueprintImplementableEvent)
	void ScriptReceiveTimelineWillFinish(int64 InID, EBXTLFinishReason InReason);

	// ����ʱ������Դ����ѯ�Ƿ����������е�����
	UFUNCTION(BlueprintCallable)
	virtual bool IsTimelineRunning(UBXTLAsset* InAsset);

	// ����ID����ѯ�Ƿ����������е�����
	UFUNCTION(BlueprintCallable)
	virtual bool IsTimelineRunningByID(int64 InID);

protected:
	// ʱ������������ID�б�
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
