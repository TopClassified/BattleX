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

	bool IsStopped() const;

	void ResetWorld();

protected:
	FBXTLRunTimeData* GetTimelineRunTimeData();

private:
	// 开始播放的时间戳
	int64 PlayTimeStamp = 0;

	// 正在播放
	bool bPlaying = false;

	// 正在暂停
	bool bPause = false;

	// 时间轴运行数据ID
	int64 TimelineRunTimeDataID = 0;

#pragma endregion Preview



#pragma region Callback
public:
	void OnObjectMoved(UObject* InObject);

	void OnTaskSelected(TArray<class UBXTask*>& SelectTaskList);

#pragma endregion Callback
	
};