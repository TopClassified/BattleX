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
		FBXTLRunTimeData* RTData = BXMgr->GetTimelineRunTimeDataByID(TimelineRunTimeDataID);
		if (!RTData)
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
	GEditor->SelectNone(false, true, false);

	if (!CachedAsset.IsValid())
	{
		return;
	}

	if (!CachedEditor.IsValid())
	{
		return;
	}

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
		Context.Timestamp = UBXFunctionLibrary::GetUtcMillisecond();
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

	bPlaying = true;
	bPause = false;

	if (CachedEditor.IsValid())
	{
		CachedEditor.Pin()->PreviewChangedEvent.Broadcast(bPlaying, bPause);
	}
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
	TimelineRunTimeDataID = 0;

	// TODO:尝试先找到技能组件
	if (UBXTimelineComponent* TLComponent = GetPreviewTimelineComponent())
	{
		TLComponent->StopTimeline(TimelineRunTimeDataID, EBXTLFinishReason::FR_Interrupt);
	}
}

bool FBXTLPreviewProxy::IsStopped() const
{
	return !bPlaying;
}

void FBXTLPreviewProxy::ResetWorld()
{
	Stop();
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

	FBXTLPreviewObjectData* Data = RTData->PreviewObjects.Find(InObject);
	if (!Data || !Data->Task)
	{
		return;
	}

	Data->Task->RefreshDataByPreviewObject(InObject, *Data);
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

	for (TMap<UObject*, FBXTLPreviewObjectData>::TIterator It(RTData->PreviewObjects); It; ++It)
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
	}

	GEditor->SelectNone(false, true, false);
}

#pragma endregion Callback
