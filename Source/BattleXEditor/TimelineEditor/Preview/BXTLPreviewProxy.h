#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "BXTLStructs.h"
#include "BXTimelineComponent.h" 



class FBXTLPreviewProxy
{
#pragma region Important
public:
	FBXTLPreviewProxy(class UBXTLAsset* InAsset, const TSharedPtr<class FBXTLEditor>& InEditor);

	virtual ~FBXTLPreviewProxy();

	void Tick(float DeltaTime);

	void Finish();

	float GetCurrentTime(int32 SectionID);

	void GetRunningSectionIndexes(TArray<int32>& OutIndexes);

private:
	UBXTimelineComponent* GetPreviewTimelineComponent();

private:
	// 资源文件
	TWeakObjectPtr<class UBXTLAsset> CachedAsset;

	// 编辑器
	TWeakPtr<class FBXTLEditor> CachedEditor;

#pragma endregion Important



#pragma region Preview
public:
	class UBXTLAsset* GetPreviewAsset() const;

	void Play();

	bool IsPlaying() const;

	void Pause();

	bool IsPaused() const;

	void Resume();

	void Stop();

	void Bake();

	bool IsBaking() const;

	bool IsRunning() const;
	
	void ResetWorld();

protected:
	void InternalPlay();
	
	FBXTLRunTimeData* GetTimelineRunTimeData();
	
	void BakeNextSection();

private:
	// 开始播放的时间戳
	int64 PlayTimeStamp = 0;

	// 正在播放
	bool bPlaying = false;

	// 正在暂停
	bool bPause = false;

	// 正在烘焙的时间片段
	TArray<FIntVector2> BakingSections;
	// 正在烘焙第几个时间片段
	int32 BakingSectionsHead = 0;
	// 记录原始的起始片段数据
	TArray<int32> OriginStartStartSectionIndexes;

	// 时间轴运行数据ID
	int64 TimelineRunTimeDataID = 0;

#pragma endregion Preview



#pragma region Callback
public:
	void OnActorMoving(AActor* InActor);

	void OnComponentMoving(USceneComponent* InComponent, ETeleportType InType);

	void OnTaskSelected(TArray<class UBXTask*>& SelectTaskList);

protected:
	void OnObjectMoving(UObject* InObject);

#pragma endregion Callback
	
};