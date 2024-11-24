#include "BXTLPreviewProxy.h"

#include "Misc/FileHelper.h"
#include "Misc/MessageDialog.h"
#include "PlatformFeatures.h"
#include "HAL/PlatformFilemanager.h"

#include "BXTask.h"
#include "BXStructs.h"
#include "BXManager.h"
#include "BXTimelineComponent.h"
#include "BXHitReactionComponent.h"

#include "BXTLEditor.h"
#include "BXTLEditorDelegates.h"
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

}

void FBXTLPreviewProxy::Tick(float DeltaTime)
{
	if (!CachedEditor.IsValid())
	{
		return;
	}

	UBXManager* BXMgr = CachedEditor.Pin()->GetBXManager();
	if (!BXMgr)
	{
		return;
	}

	if (TimelineRunTimeDataID > 0)
	{
		// 根据运行数据刷新静态数据
		if (FBXTLRunTimeData* RTData = BXMgr->GetTimelineRunTimeDataByID(TimelineRunTimeDataID))
		{
			if (RTData->Timeline)
			{
				for (int32 i = 0; i < RTData->RunningSections.Num(); ++i)
				{
					FBXTLSectionRTData& SRTData = RTData->RunningSections[i];
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
						Task->GetDynamicObjectByRuntimeData(BXMgr, *RTData, SRTData, TRTData);

						// 烘焙数据
						if (IsBaking())
						{
							Task->BakingData(*RTData, SRTData, TRTData);
						}
					}

					if (IsBaking())
					{
						// 超出预期烘焙时间，直接标记为结束
						if (SRTData.RunTime > BakingSections[BakingSectionsHead - 1].Y * 0.0001f)
						{
							SRTData.bEarlyFinish = true;
						}
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

	// 尝试烘焙下一个时间片段
	if (IsBaking())
	{
		BakeNextSection();
	}
}

void FBXTLPreviewProxy::Finish()
{
	GEditor->SelectNone(false, true, false);

	if (CachedEditor.IsValid())
	{
		CachedEditor.Pin()->TaskSelectedEvent.RemoveAll(this);
	}

	// 注销关心的事件
	if (GEngine)
	{
		GEngine->OnActorMoved().RemoveAll(this);
		GEngine->OnComponentTransformChanged().RemoveAll(this);
	}
}

float FBXTLPreviewProxy::GetCurrentTime(int32 SectionID)
{
	if (!CachedEditor.IsValid())
	{
		return 0.0f;
	}

	UBXManager* BXMgr = CachedEditor.Pin()->GetBXManager();
	if (!BXMgr)
	{
		return 0.0f;
	}

	FBXTLRunTimeData* RTData = BXMgr->GetTimelineRunTimeDataByID(TimelineRunTimeDataID);
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

	UBXManager* BXMgr = CachedEditor.Pin()->GetBXManager();
	if (!BXMgr)
	{
		return;
	}

	FBXTLRunTimeData* RTData = BXMgr->GetTimelineRunTimeDataByID(TimelineRunTimeDataID);
	if (!RTData)
	{
		return;
	}

	for (TArray<FBXTLSectionRTData>::TIterator It(RTData->RunningSections); It; ++It)
	{
		OutIndexes.Add(It->Index);
	}
}

UBXTimelineComponent* FBXTLPreviewProxy::GetPreviewTimelineComponent()
{
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
	return Player->FindComponentByClass<UBXTimelineComponent>();
}

#pragma endregion Important



#pragma region Preview
UBXTLAsset* FBXTLPreviewProxy::GetPreviewAsset() const
{ 
	return CachedAsset.Get(); 
}

void FBXTLPreviewProxy::Play()
{
	if (!CachedAsset.IsValid() || IsBaking())
	{
		return;
	}

	if (!CachedEditor.IsValid())
	{
		return;
	}

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
	if (IsBaking())
	{
		return;
	}
	
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
	if (IsBaking())
	{
		return;
	}
	
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
	if (IsBaking())
	{
		return;
	}
	
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
	TimelineRunTimeDataID = 0;

	// TODO:尝试先找到技能组件
	if (UBXTimelineComponent* TLComponent = GetPreviewTimelineComponent())
	{
		TLComponent->StopTimeline(TimelineRunTimeDataID, EBXTLFinishReason::FR_Interrupt);
	}
}

void FBXTLPreviewProxy::Bake()
{
	if (!CachedAsset.IsValid() || !CachedEditor.IsValid())
	{
		return;
	}

	// 重置世界
	ResetWorld();
	
	// 获取哪些片段需要烘焙
	BakingSectionsHead = 0;
	BakingSections.Reset();
	for (int32 i = 0; i < CachedAsset->Sections.Num(); ++i)
	{
		float FinishTime = -1.0f;
		FBXTLSection& Section = CachedAsset->Sections[i];
		for (int32 j = 0; j < Section.TaskList.Num(); ++j)
		{
			UBXTask* Task = Cast<UBXTask>(Section.TaskList[j]);
			if (!Task)
			{
				continue;
			}

			if (Task->NeedBakeData())
			{
				Task->CleanBakedData();

				float EndTime = Task->StartTime + Task->Duration;
				if (Task->LifeType == EBXTLifeType::L_Timeline)
				{
					EndTime = Section.Duration;
				}
				
				if (FinishTime < EndTime)
				{
					FinishTime = EndTime;
				}
			}
		}

		if (FinishTime > 0.0f)
		{
			BakingSections.Add(FIntVector2(i, FMath::FloorToInt(FinishTime * 10000.0f)));
		}
	}

	if (BakingSections.Num() <= 0)
	{
		return;
	}

	// 记录一下原始数据
	OriginStartStartSectionIndexes.Reset();
	OriginStartStartSectionIndexes.Append(CachedAsset->StartSectionIndexes);
	
	// 关闭片段跳转
	CachedEditor.Pin()->GetBXManager()->CloseSectionJump(true);

	// 设置烘焙时的帧率
	CachedEditor.Pin()->SetPreviewFPS(100.0f);
	
	// 烘焙时间片段
	BakeNextSection();
}

bool FBXTLPreviewProxy::IsBaking() const
{
	return BakingSections.Num() > 0;
}

bool FBXTLPreviewProxy::IsRunning() const
{
	return bPlaying || BakingSections.Num() > 0;
}

void FBXTLPreviewProxy::ResetWorld()
{
	Stop();
}

void FBXTLPreviewProxy::InternalPlay()
{
	UBXManager* BXMgr = CachedEditor.Pin()->GetBXManager();
	if (!BXMgr)
	{
		return;
	}

	TSharedPtr<FBXTLPreviewScene> Scene = CachedEditor.Pin()->GetPreviewScene();
	if (!Scene.IsValid())
	{
		return;
	}

	AActor* Player = Scene->GetPlayerActor();
	if (!Player)
	{
		return;
	}
	
	if (UBXTimelineComponent* TLComponent = GetPreviewTimelineComponent())
	{
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
}

FBXTLRunTimeData* FBXTLPreviewProxy::GetTimelineRunTimeData()
{
	if (!CachedEditor.IsValid())
	{
		return nullptr;
	}

	UBXManager* BXMgr = CachedEditor.Pin()->GetBXManager();
	if (!BXMgr)
	{
		return nullptr;
	}

	return BXMgr->GetTimelineRunTimeDataByID(TimelineRunTimeDataID);
}

void FBXTLPreviewProxy::BakeNextSection()
{
	if (!CachedEditor.IsValid() || !CachedAsset.IsValid())
	{
		return;
	}

	if (TimelineRunTimeDataID > 0)
	{
		return;
	}

	// 烘焙完成
	if (BakingSectionsHead >= BakingSections.Num())
	{
		// 关闭烘焙帧率
		CachedEditor.Pin()->SetPreviewFPS(0.0f);
		// 打开片段跳转
		CachedEditor.Pin()->GetBXManager()->CloseSectionJump(false);
		// 还原数据
		CachedAsset->StartSectionIndexes.Reset();
		CachedAsset->StartSectionIndexes.Append(OriginStartStartSectionIndexes);
		// 清理临时数据
		BakingSectionsHead = 0;
		BakingSections.Reset();
		// 对烘焙数据进行后处理
		for (int32 i = 0; i < CachedAsset->Sections.Num(); ++i)
		{
			FBXTLSection& Section = CachedAsset->Sections[i];
			for (int32 j = 0; j < Section.TaskList.Num(); ++j)
			{
				if (!IsValid(Section.TaskList[j]))
				{
					continue;
				}
				
				Section.TaskList[j]->PostBakeData();
			}
		}
		
		return;
	}

	// 设置要烘焙的片段
	CachedAsset->StartSectionIndexes.Reset();
	CachedAsset->StartSectionIndexes.Add(BakingSections[BakingSectionsHead].X);

	// 播放片段
	InternalPlay();

	BakingSectionsHead = BakingSectionsHead + 1;
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

	UBXManager* BXMgr = CachedEditor.Pin()->GetBXManager();
	if (!BXMgr)
	{
		return;
	}

	FBXTLRunTimeData* RTData = BXMgr->GetTimelineRunTimeDataByID(TimelineRunTimeDataID);
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

	UBXManager* BXMgr = CachedEditor.Pin()->GetBXManager();
	if (!BXMgr)
	{
		return;
	}

	FBXTLRunTimeData* RTData = BXMgr->GetTimelineRunTimeDataByID(TimelineRunTimeDataID);
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
