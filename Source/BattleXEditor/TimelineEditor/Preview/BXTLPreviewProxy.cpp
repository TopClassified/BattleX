#include "BXTLPreviewProxy.h"

#include "Misc/FileHelper.h"
#include "Misc/MessageDialog.h"
#include "HAL/PlatformFilemanager.h"

#include "BXTask.h"
#include "BXTLAsset.h"
#include "BXStructs.h"
#include "BXTLManager.h"
#include "BXHitReactionComponent.h"

#include "BXTLEditor.h"
#include "Preview/BXTLPreviewScene.h" 



#pragma region Important
FBXTLPreviewProxy::FBXTLPreviewProxy(UBXTLAsset* InAsset, const TSharedPtr<FBXTLEditor>& InEditor) : CachedAsset(InAsset), CachedEditor(InEditor)
{
	if (CachedEditor.IsValid())
	{
		CachedEditor.Pin()->TaskSelectedEvent.AddRaw(this, &FBXTLPreviewProxy::OnTaskSelected);
	}

	// 注册关心的事件
	if (GEngine)
	{
		GEngine->OnActorMoving().AddRaw(this, &FBXTLPreviewProxy::OnActorMoving);
		GEngine->OnComponentTransformChanged().AddRaw(this, &FBXTLPreviewProxy::OnComponentMoving);
	}
}

FBXTLPreviewProxy::~FBXTLPreviewProxy()
{
	// 兜底解绑全局委托(raw委托持有裸指针,对象销毁不会自动失效;RemoveAll幂等,Finish已解绑时为空操作)
	if (GEngine)
	{
		GEngine->OnActorMoving().RemoveAll(this);
		GEngine->OnComponentTransformChanged().RemoveAll(this);
	}
}

void FBXTLPreviewProxy::Tick(float DeltaTime)
{
	if (!CachedEditor.IsValid())
	{
		return;
	}

	UBXTLManager* BXTLMgr = CachedEditor.Pin()->GetCachedManager<UBXTLManager>();
	if (!BXTLMgr)
	{
		return;
	}

	if (TimelineRunTimeDataID > 0)
	{
		// 根据运行数据刷新静态数据
		if (FBXTLRunTimeData* RTData = BXTLMgr->GetTimelineRunTimeDataByID(TimelineRunTimeDataID))
		{
			if (RTData->Timeline)
			{
				for (int32 i = 0; i < RTData->RunningSections.Num(); ++i)
			{
				FBXTLSectionRTData& SRTData = RTData->RunningSections[i];
				// 播放/暂停期间资产Section可被删改,索引越界校验(与GetRunningTasks的防御对齐)
				if (!RTData->Timeline->Sections.IsValidIndex(SRTData.Index))
				{
					continue;
				}
				FBXTLSection& Section = RTData->Timeline->Sections[SRTData.Index];

					for (int32 j = 0; j < SRTData.RunningTasks.Num(); ++j)
					{
						const FBXTLTaskRTData& TRTData = SRTData.RunningTasks[j];
						if (!Section.TaskList.IsValidIndex(TRTData.Index))
						{
							continue;
						}

						UBXTask* Task = Section.TaskList[TRTData.Index];
						if (!Task)
						{
							continue;
						}

						// 获取动态对象信息
						Task->GetDynamicObjectByRuntimeData(BXTLMgr, *RTData, SRTData, TRTData);
					}
				}
			}
		}
		else
		{
			Stop();
			TimelineRunTimeDataID = 0;
		}
	}
}

void FBXTLPreviewProxy::Finish()
{
	GEditor->SelectNone(false, true, false);

	if (CachedEditor.IsValid())
	{
		CachedEditor.Pin()->TaskSelectedEvent.RemoveAll(this);
	}

	// 注销关心的事件(绑定的委托为OnActorMoving而非OnActorMoved,解绑错委托会留下悬空raw指针导致主视口拖动Actor时崩溃)
	if (GEngine)
	{
		GEngine->OnActorMoving().RemoveAll(this);
		GEngine->OnComponentTransformChanged().RemoveAll(this);
	}
}

float FBXTLPreviewProxy::GetCurrentTime(int32 SectionID)
{
	if (!CachedEditor.IsValid())
	{
		return 0.0f;
	}

	UBXTLManager* BXTLMgr = CachedEditor.Pin()->GetCachedManager<UBXTLManager>();
	if (!BXTLMgr)
	{
		return 0.0f;
	}

	FBXTLRunTimeData* RTData = BXTLMgr->GetTimelineRunTimeDataByID(TimelineRunTimeDataID);
	if (!RTData)
	{
		return 0.0f;
	}

	for (TArray<FBXTLSectionRTData>::TIterator It(RTData->RunningSections); It; ++It)
	{
		if (It->Index == SectionID)
		{
			return It->RunTime;
		}
	}

	return 0.0f;
}

void FBXTLPreviewProxy::GetRunningSectionIndexes(TArray<int32>& OutIndexes)
{
	if (!CachedEditor.IsValid())
	{
		return;
	}

	UBXTLManager* BXTLMgr = CachedEditor.Pin()->GetCachedManager<UBXTLManager>();
	if (!BXTLMgr)
	{
		return;
	}

	FBXTLRunTimeData* RTData = BXTLMgr->GetTimelineRunTimeDataByID(TimelineRunTimeDataID);
	if (!RTData)
	{
		return;
	}

	for (TArray<FBXTLSectionRTData>::TIterator It(RTData->RunningSections); It; ++It)
	{
		OutIndexes.Add(It->Index);
	}
}

void FBXTLPreviewProxy::GetRunningTasks(TArray<UBXTask*>& OutTasks)
{
	OutTasks.Reset();

	if (!CachedEditor.IsValid())
	{
		return;
	}

	UBXTLManager* BXTLMgr = CachedEditor.Pin()->GetCachedManager<UBXTLManager>();
	if (!BXTLMgr)
	{
		return;
	}

	FBXTLRunTimeData* RTData = BXTLMgr->GetTimelineRunTimeDataByID(TimelineRunTimeDataID);
	if (!RTData || !RTData->Timeline)
	{
		return;
	}

	for (const FBXTLSectionRTData& SRTData : RTData->RunningSections)
	{
		if (!RTData->Timeline->Sections.IsValidIndex(SRTData.Index))
		{
			continue;
		}

		const FBXTLSection& Section = RTData->Timeline->Sections[SRTData.Index];
		for (const FBXTLTaskRTData& TRTData : SRTData.RunningTasks)
		{
			if (Section.TaskList.IsValidIndex(TRTData.Index))
			{
				OutTasks.AddUnique(Section.TaskList[TRTData.Index]);
			}
		}
	}
}

UBXTLComponent* FBXTLPreviewProxy::GetPreviewTimelineComponent()
{
	// 编辑器可能已销毁(Pin()为空,空指针解引用)
	if (!CachedEditor.IsValid())
	{
		return nullptr;
	}

	TSharedPtr<FBXTLPreviewScene> Scene = CachedEditor.Pin()->GetPreviewScene();
	if (!Scene.IsValid())
	{
		return nullptr;
	}

	AActor* Player = Scene->GetPlayerActor();
	if (!Player)
	{
		return nullptr;
	}

	// TODO:尝试先找到技能组件
	return Player->FindComponentByClass<UBXTLComponent>();
}

#pragma endregion Important



#pragma region Preview
UBXTLAsset* FBXTLPreviewProxy::GetPreviewAsset() const
{ 
	return CachedAsset.Get(); 
}

void FBXTLPreviewProxy::Play()
{
	if (!CachedAsset.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("FBXTLPreviewProxy::Play failed: CachedAsset is invalid."));
		return;
	}

	if (!CachedEditor.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("FBXTLPreviewProxy::Play failed: CachedEditor is invalid, Asset=%s."), *CachedAsset->GetName());
		return;
	}

	// 播放前刷新数据
	CachedAsset->RefreshDataBeforePreview();

	GEditor->SelectNone(false, true, false);

	InternalPlay();

	bPlaying = true;
	bPause = false;
	CachedEditor.Pin()->PreviewChangedEvent.Broadcast(bPlaying, bPause);
}

bool FBXTLPreviewProxy::IsPlaying() const
{
	return bPlaying && !bPause;
}

void FBXTLPreviewProxy::Pause()
{
	bPause = true;

	if (CachedEditor.IsValid())
	{
		CachedEditor.Pin()->PreviewChangedEvent.Broadcast(bPlaying, bPause);
	}
}

bool FBXTLPreviewProxy::IsPaused() const
{
	return (bPlaying && bPause);
}

void FBXTLPreviewProxy::Resume()
{
	bPause = false;

	GEditor->SelectNone(false, true, false);

	if (CachedEditor.IsValid())
	{
		CachedEditor.Pin()->SetTaskSelection(TArray<UBXTask*>{});
		CachedEditor.Pin()->PreviewChangedEvent.Broadcast(bPlaying, bPause);
	}
}

void FBXTLPreviewProxy::Stop()
{
	bPlaying = false;
	bPause = false;

	GEditor->SelectNone(false, true, false);

	if (CachedEditor.IsValid())
	{
		CachedEditor.Pin()->SetTaskSelection(TArray<UBXTask*>{});
		CachedEditor.Pin()->PreviewChangedEvent.Broadcast(bPlaying, bPause);
	}

	if (TimelineRunTimeDataID <= 0)
	{
		return;
	}

	// 先缓存ID再清零(原实现先清零后使用,StopTimeline(0)找不到数据,时间轴永不被停止:Task的End不执行,运行数据残留Manager持续Tick)
	const int64 StoppedTimelineID = TimelineRunTimeDataID;
	TimelineRunTimeDataID = 0;

	// TODO:尝试先找到技能组件
	if (UBXTLComponent* TLComponent = GetPreviewTimelineComponent())
	{
		TLComponent->StopTimeline(StoppedTimelineID, EBXTLFinishReason::FR_Interrupt);
	}
}

bool FBXTLPreviewProxy::IsRunning() const
{
	return bPlaying;
}

void FBXTLPreviewProxy::ResetWorld()
{
	Stop();
}

void FBXTLPreviewProxy::InternalPlay()
{
	UBXTLManager* BXTLMgr = CachedEditor.Pin()->GetCachedManager<UBXTLManager>();
	if (!BXTLMgr)
	{
		UE_LOG(LogTemp, Warning, TEXT("FBXTLPreviewProxy::InternalPlay failed: BXTLManager is null, Asset=%s."), *CachedAsset->GetName());
		return;
	}

	TSharedPtr<FBXTLPreviewScene> Scene = CachedEditor.Pin()->GetPreviewScene();
	if (!Scene.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("FBXTLPreviewProxy::InternalPlay failed: PreviewScene is invalid, Asset=%s."), *CachedAsset->GetName());
		return;
	}

	AActor* Player = Scene->GetPlayerActor();
	if (!Player)
	{
		UE_LOG(LogTemp, Warning, TEXT("FBXTLPreviewProxy::InternalPlay failed: PlayerActor is null, Asset=%s."), *CachedAsset->GetName());
		return;
	}

	UBXTLComponent* TLComponent = GetPreviewTimelineComponent();
	if (!TLComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("FBXTLPreviewProxy::InternalPlay failed: TLComponent is null on Player=%s, Asset=%s."), *Player->GetName(), *CachedAsset->GetName());
		return;
	}

	FBXTLPlayContext Context;
	Context.Instigator = Player;
	Context.Triggerer = Player;
	if (AActor* Target = Scene->GetTargetActor())
	{
		if (UBXHitReactionComponent* HitReaction = Target->FindComponentByClass<UBXHitReactionComponent>())
		{
			FBXBodyPartSelection& NewSelection = Context.LockParts.AddDefaulted_GetRef();
			NewSelection.BodyPart = CachedEditor.Pin()->GetLockedBodyPartType();
			NewSelection.Owner = HitReaction;
		}
	}

	TimelineRunTimeDataID = TLComponent->PlayTimeline(CachedAsset.Get(), Context);
}

FBXTLRunTimeData* FBXTLPreviewProxy::GetTimelineRunTimeData()
{
	if (!CachedEditor.IsValid())
	{
		return nullptr;
	}

	UBXTLManager* BXTLMgr = CachedEditor.Pin()->GetCachedManager<UBXTLManager>();
	if (!BXTLMgr)
	{
		return nullptr;
	}

	return BXTLMgr->GetTimelineRunTimeDataByID(TimelineRunTimeDataID);
}

#pragma endregion Preview



#pragma region Callback
void FBXTLPreviewProxy::OnActorMoving(AActor* InActor)
{
	OnObjectMoving(InActor);
}

void FBXTLPreviewProxy::OnComponentMoving(USceneComponent* InComponent, ETeleportType InType)
{
	OnObjectMoving(InComponent);
}

void FBXTLPreviewProxy::OnObjectMoving(UObject* InObject)
{
	if (!IsValid(InObject) || !InObject->IsValidLowLevelFast())
	{
		return;
	}

	// 全局委托回调时编辑器可能已销毁(Pin()空指针解引用),与Tick/GetTimelineRunTimeData防御对齐
	if (!CachedEditor.IsValid())
	{
		return;
	}

	UBXTLManager* BXTLMgr = CachedEditor.Pin()->GetCachedManager<UBXTLManager>();
	if (!BXTLMgr)
	{
		return;
	}

	FBXTLRunTimeData* RTData = BXTLMgr->GetTimelineRunTimeDataByID(TimelineRunTimeDataID);
	if (!RTData)
	{
		return;
	}

	// 获取Task运行时创建的动态对象
}

void FBXTLPreviewProxy::OnTaskSelected(TArray<UBXTask*>& SelectTaskList)
{
	if (SelectTaskList.Num() <= 0 || !CachedAsset.IsValid())
	{
		return;
	}

	UBXTLManager* BXTLMgr = CachedEditor.Pin()->GetCachedManager<UBXTLManager>();
	if (!BXTLMgr)
	{
		return;
	}

	FBXTLRunTimeData* RTData = BXTLMgr->GetTimelineRunTimeDataByID(TimelineRunTimeDataID);
	if (!RTData)
	{
		return;
	}

	/*for (TMap<UObject*, FBXTLPreviewObjectData>::TIterator It(RTData->PreviewObjects); It; ++It)
	{
		if (It->Value.Task == SelectTaskList[0])
		{
			GEditor->SelectNone(false, true, false);
			if (AActor* DActor = Cast<AActor>(It->Key))
			{
				GEditor->SelectActor(DActor, true, true, true);
			}
			else if (USceneComponent* DComponent = Cast<USceneComponent>(It->Key))
			{
				GEditor->SelectComponent(DComponent, true, true, true);
			}

			return;
		}
	}*/

	GEditor->SelectNone(false, true, false);
}

#pragma endregion Callback
