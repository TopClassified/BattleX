#include "BXTLPreviewScene.h"
#include "RHI.h"
#include "RHIDefinitions.h"
#include "EngineUtils.h"
#include "EngineSettings.h"
#include "Engine/Selection.h"
#include "Camera/CameraActor.h"

#include "EditorLevelUtils.h"
#include "Slate/SceneViewport.h"
#include "Camera/CameraComponent.h"
#include "Engine/LevelStreamingDynamic.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/MovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/StaticMeshComponent.h"

#include "BXTLAsset.h"
#include "BXTLEditor.h"
#include "BXTLPreviewProxy.h"
#include "BXTLEditorSettings.h"
#include "BXTLEditorDelegates.h"
#include "Gear/BXGearComponent.h"
#include "Viewport/SBXTLEditorViewport.h" 



#pragma region Important
FBXTLPreviewScene::FBXTLPreviewScene(ConstructionValues CVS, TSharedPtr<FBXTLEditor> InEditor) : FAdvancedPreviewScene(CVS), CachedEditor(InEditor)
{
	
}

FBXTLPreviewScene::~FBXTLPreviewScene()
{

}

void FBXTLPreviewScene::OnCreateViewport(SBXTLEditorViewport* InEditorViewport, TSharedPtr<FSceneViewport> InSceneViewport)
{
	const UBXTLEditorSettings* EditorSettings = GetDefault<UBXTLEditorSettings>();
	if (!EditorSettings->DefaultViewMap.IsNull())
	{
		ExternalLevel = LoadExternalMap(EditorSettings->DefaultViewMap);
	}

	if (UStaticMesh* FloorMesh = LoadObject<UStaticMesh>(NULL, TEXT("/Engine/EditorMeshes/AssetViewer/Floor_Mesh.Floor_Mesh"), NULL, LOAD_None, NULL))
	{
		FloorOwner = GetWorld()->SpawnActor<AActor>(AActor::StaticClass(), FTransform::Identity);
		FloorOwner->SetActorLocation(FVector(0.0f, 0.0f, -110.0f));
		UStaticMeshComponent* NewFloor = NewObject<UStaticMeshComponent>(FloorOwner.Get());
		FloorOwner->SetRootComponent(NewFloor);
		FloorOwner->AttachToActor(FloorOwner.Get(), FAttachmentTransformRules::KeepWorldTransform);

		NewFloor->RegisterComponent();
		NewFloor->SetStaticMesh(FloorMesh);
		NewFloor->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
		NewFloor->AttachToComponent(FloorOwner->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
		NewFloor->SetRelativeLocation(FVector::ZeroVector);
		NewFloor->SetRelativeScale3D(FVector(10.0f, 10.0f, 1.0f));
		NewFloor->bReceivesDecals = true;

		FloorMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	// 注册关心的事件
	GEngine->OnActorMoving().AddRaw(this, &FBXTLPreviewScene::OnActorMoving);
}

void FBXTLPreviewScene::Tick(float DeltaTime)
{
	FAdvancedPreviewScene::Tick(DeltaTime);

	TSharedPtr<SBXTLEditorViewport> ViewportWidget = CachedEditor.Pin()->GetViewportWidget();
	TSharedPtr<FEditorViewportClient> ViewportClient = ViewportWidget.IsValid() ? ViewportWidget->GetViewportClient() : nullptr;
	if (ViewportClient)
	{
		GEditor->RedrawAllViewports();
	}
}

void FBXTLPreviewScene::Finish()
{
	if (GetWorld())
	{
		DestroyPreviewActors();

		if (PlayerController.IsValid())
		{
			PlayerController->RemoveFromRoot();
			GetWorld()->EditorDestroyActor(PlayerController.Get(), false);
		}

		if (GetWorld()->GetGameState())
		{
			GetWorld()->GetGameState()->RouteEndPlay(EEndPlayReason::Destroyed);
		}
	}

	// 注销关心的事件
	GEngine->OnActorMoved().RemoveAll(this);
}

void FBXTLPreviewScene::AddReferencedObjects(FReferenceCollector& Collector)
{
	FPreviewScene::AddReferencedObjects(Collector);

	if (CachedEditor.IsValid())
	{
		CachedEditor.Pin()->AddReferencedObjects(Collector);
	}
}

#pragma endregion Important



#pragma region Preview
void FBXTLPreviewScene::InitPreviewWorld()
{
	UWorld* World = GetWorld();

	World->GetWorldSettings()->SetIsTemporarilyHiddenInEditor(false);
	World->GetWorldSettings()->NotifyBeginPlay();

	// 创建PlayerController并设置摄像机
	PlayerController = World->SpawnActorDeferred<APlayerController>(APlayerController::StaticClass(), FTransform::Identity);
	if (PlayerController.IsValid())
	{
		PlayerController->bAutoManageActiveCameraTarget = true;
		PlayerController->FinishSpawning(FTransform::Identity);
		PlayerController->AddToRoot();

		// 初始化镜头
		PlayerController->PlayerCameraManager->DispatchBeginPlay();
		PlayerController->PlayerCameraManager->bDebugClientSideCamera = true;
	}

	// 自动重置世界
	ResetPreviewWorld();
}

void FBXTLPreviewScene::ResetPreviewWorld()
{
	UWorld* World = GetWorld();

	DestroyPreviewActors();

	// 手动销毁所有的摄像机对象
	for (TActorIterator<ACameraActor> It(GetWorld()); It; ++It)
	{
		It->Destroy();
	}

	SpawnPreviewActors();
}

void FBXTLPreviewScene::SpawnPreviewActors()
{
	if (UBXTLAsset* Asset = CachedEditor.Pin()->GetEditingAsset())
	{
		UWorld* World = GetWorld();

		if (UBXTLPreviewActor* PInfo = Cast<UBXTLPreviewActor>(Asset->PlayerInformation))
		{
			PreviewPlayer = PInfo->CreateActor(World);
			PreviewActors.Add(PreviewPlayer);
		}

		if (UBXTLPreviewActor* PInfo = Cast<UBXTLPreviewActor>(Asset->TargetInformation))
		{
			PreviewTarget = PInfo->CreateActor(World);
			PreviewActors.Add(PreviewTarget);
		}

		// 手动调用新对象的BeginPlay
		for (int32 i = 0; i < PreviewActors.Num(); ++i)
		{
			if (PreviewActors[i].IsValid())
			{
				PreviewActors[i]->AddToRoot();
				PreviewActors[i]->DispatchBeginPlay();

				// 手动设置移动组件的一些信息
				if (UMovementComponent* MoveComp = PreviewActors[i]->FindComponentByClass<UMovementComponent>())
				{
					MoveComp->SetUpdatedComponent(PreviewActors[i]->GetRootComponent());
				}

				if (APawn* Pawn = Cast<APawn>(PreviewActors[i]))
				{
					if (Pawn == PreviewPlayer)
					{
						if (PlayerController.IsValid())
						{
							PlayerController->Possess(Pawn);
						}
					}
					else
					{
						Pawn->SpawnDefaultController();
					}
				}
			}
		}
	}

	// 清理无效摄像机
	for (TActorIterator<ACameraActor> It(GetWorld()); It; ++It)
	{
		It->Destroy();
	}
}

void FBXTLPreviewScene::DestroyPreviewActors()
{
	GEditor->SelectNone(false, false);

	if (PlayerController.IsValid())
	{
		PlayerController->UnPossess();
	}

	UWorld* World = GetWorld();

	TArray<ABXGear*> GearList;

	for (TWeakObjectPtr<AActor> ActorPtr : PreviewActors)
	{
		if (!ActorPtr.IsValid())
		{
			continue;
		}
		
		// 删除所有装备中的武器
		if (UBXGearComponent* GearComp = ActorPtr->FindComponentByClass<UBXGearComponent>())
		{
			GearComp->GetEquipGearList(GearList);
			GearComp->UnequipAllGears();
		}
		for (TArray<ABXGear*>::TIterator It(GearList); It; ++It)
		{
			World->EditorDestroyActor(*It, false);
		}

		// 调整控制器
		if (APawn* Pawn = Cast<APawn>(ActorPtr))
		{
			if (AController* Controller = Pawn->GetController())
			{
				Controller->UnPossess();
				World->EditorDestroyActor(Controller, false);
			}
		}

		ActorPtr->RemoveFromRoot();
		World->EditorDestroyActor(ActorPtr.Get(), false);
	}
	PreviewActors.Empty();

	PreviewPlayer = nullptr;
	PreviewTarget = nullptr;
}

AActor* FBXTLPreviewScene::GetPlayerActor() const
{
	return PreviewPlayer.Get();
}

AActor* FBXTLPreviewScene::GetTargetActor() const
{
	return PreviewTarget.Get();
}

AActor* FBXTLPreviewScene::GetSelectedActor() const
{
	if (USelection* Selection = GEditor->GetSelectedActors())
	{
		TArray<AActor*> SelectedActors;
		int32 Num = Selection->GetSelectedObjects(SelectedActors);
		if (Num > 0)
		{
			for (AActor* Actor : SelectedActors)
			{
				if (Actor->GetWorld() == GetWorld())
				{
					return Actor;
				}
			}
		}
	}

	return nullptr;
}

TArray<TWeakObjectPtr<AActor>> FBXTLPreviewScene::GetPreviewActors() const
{
	return PreviewActors;
}

ULevelStreaming* FBXTLPreviewScene::LoadExternalMap(TSoftObjectPtr<UWorld> NewMap)
{
	UWorld* World = GetWorld();
	FWorldContext* WorldConetxt = GEngine->GetWorldContextFromWorld(World);

	ULevelStreaming* NewLevel = EditorLevelUtils::AddLevelToWorld(World, *NewMap.GetLongPackageName(), ULevelStreamingDynamic::StaticClass());
	if (NewLevel)
	{
		NewLevel->SetFlags(RF_Transactional);
		World->SetCurrentLevel(World->PersistentLevel);
	}

	return NewLevel;
}

void FBXTLPreviewScene::OnActorMoving(AActor* InActor)
{
	if (!IsValid(InActor) || !InActor->IsValidLowLevelFast())
	{
		return;
	}

	if (!CachedEditor.IsValid())
	{
		return;
	}

	UBXTLAsset* Asset = CachedEditor.Pin()->GetEditingAsset();
	if (!Asset)
	{
		return;
	}

	if (InActor == PreviewPlayer)
	{
		if (CachedEditor.Pin()->IsStopped())
		{
			if (UBXTLPreviewActor* PInfo = Cast<UBXTLPreviewActor>(Asset->PlayerInformation))
			{
				PInfo->SpawnTransform = InActor->GetActorTransform();
			}
		}
	}
	else if (InActor == PreviewTarget)
	{
		if (CachedEditor.Pin()->IsStopped())
		{
			if (UBXTLPreviewActor* PInfo = Cast<UBXTLPreviewActor>(Asset->TargetInformation))
			{
				PInfo->SpawnTransform = InActor->GetActorTransform();
			}
		}
	}

	Asset->MarkPackageDirty();
}

#pragma endregion Preview

