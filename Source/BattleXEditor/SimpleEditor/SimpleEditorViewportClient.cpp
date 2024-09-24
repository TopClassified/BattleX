#include "SimpleEditorViewportClient.h"
#include "EngineUtils.h"
#include "Engine/Selection.h"
#include "PreviewScene.h"
#include "Camera/CameraComponent.h"

#include "UnrealEdGlobals.h"
#include "EditorModeManager.h"
#include "Editor/EditorEngine.h"
#include "Editor/UnrealEdEngine.h" 



#pragma region Important
FSimpleEditorViewportClient::FSimpleEditorViewportClient(FEditorModeTools* InModeTools, FPreviewScene* InPreviewScene, const TWeakPtr<SEditorViewport>& InEditorViewportWidget) : FEditorViewportClient(InModeTools, InPreviewScene, InEditorViewportWidget), FadeAmount(0.f), bEnableFading(false), ModelTools(InModeTools)
{
	DrawHelper.PerspectiveGridSize = HALF_WORLD_MAX1;
	DrawHelper.AxesLineThickness = 0.0f;
	DrawHelper.bDrawGrid = true;

	EngineShowFlags.Game = 0;
	EngineShowFlags.ScreenSpaceReflections = 1;
	EngineShowFlags.AmbientOcclusion = 1;
	EngineShowFlags.SetSnap(0);
	EngineShowFlags.DisableAdvancedFeatures();
	EngineShowFlags.SetSeparateTranslucency(true);
	EngineShowFlags.SetCompositeEditorPrimitives(true);
	EngineShowFlags.SetParticles(true);
	EngineShowFlags.ModeWidgets = true;

	SetRealtime(true);
	SetShowStats(true);

	UWorld* PreviewWorld = InPreviewScene->GetWorld();
	PreviewWorld->bAllowAudioPlayback = true;

	GEngine->OnActorMoved().AddRaw(this, &FSimpleEditorViewportClient::OnActorMoved);
}

void FSimpleEditorViewportClient::Tick(float DeltaSeconds)
{
	FEditorViewportClient::Tick(DeltaSeconds);

	// 跟随锁定的对象
	MoveViewportToLockedActor();
}

#pragma endregion Important



#pragma region Render
void FSimpleEditorViewportClient::Draw(FViewport* InViewport, FCanvas* Canvas)
{
	FEditorViewportClient::Draw(InViewport, Canvas);
}

void FSimpleEditorViewportClient::SetupViewForRendering(FSceneViewFamily& ViewFamily, FSceneView& View)
{
	FEditorViewportClient::SetupViewForRendering(ViewFamily, View);

	if (!ViewFamily.EngineShowFlags.LightComplexity)
	{
		if (bEnableFading)
		{
			View.OverlayColor = FadeColor;
			View.OverlayColor.A = FMath::Clamp(FadeAmount, 0.f, 1.f);
		}
	}
}

void FSimpleEditorViewportClient::SetEngineShowFlagCollision(bool bShowCollision)
{
	EngineShowFlags.SetCollision(bShowCollision);
}

#pragma endregion Render



#pragma region Logic
void FSimpleEditorViewportClient::ProcessClick(FSceneView& View, HHitProxy* HitProxy, FKey Key, EInputEvent Event, uint32 HitX, uint32 HitY)
{
	const FViewportClick Click(&View, this, Key, Event, HitX, HitY);

	if (HitProxy && HitProxy->IsA(HActor::StaticGetType()))
	{
		HActor* ObjectHitProxy = (HActor*)HitProxy;
		AActor* HActor = ObjectHitProxy->Actor;

		if (HActor->IsA<AWorldSettings>())
		{
			TArray<USceneComponent*> Components;
			HActor->GetComponents<USceneComponent>(Components);
			for (int32 i = 0; i < Components.Num(); ++i)
			{
				if (Components[i] == ObjectHitProxy->PrimComponent)
				{
					GEditor->SelectNone(false, true, false);
					GEditor->SelectComponent(Components[i], true, true, true);
					break;
				}
			}
		}
		else
		{
			GEditor->SelectNone(false, true, false);
			GEditor->SelectActor(HActor, true, true, true);
		}
	}

	GUnrealEd->ComponentVisManager.HandleClick(this, HitProxy, Click);
}

void FSimpleEditorViewportClient::PerspectiveCameraMoved()
{
	FEditorViewportClient::PerspectiveCameraMoved();

	MoveLockedActorToViewport();
	MoveViewportToLockedActor();
}

void FSimpleEditorViewportClient::TrackingStopped()
{
	TArray<AActor*> ActorsToMove;
	TArray<USceneComponent*> ComponentsToMove;
	GetSelectedActorsAndComponentsForMove(ActorsToMove, ComponentsToMove);

	for (USceneComponent* Component : ComponentsToMove)
	{
		Component->PostEditComponentMove(true);
		GEditor->BroadcastEndObjectMovement(*Component);
	}

	for (AActor* Actor : ActorsToMove)
	{
		Actor->PostEditMove(true);
		GEditor->BroadcastEndObjectMovement(*Actor);
	}

	GEditor->BroadcastActorsMoved(ActorsToMove);
}

bool FSimpleEditorViewportClient::CheckActorCanMove(const AActor* InActor) const
{
	if (!GEditor || !InActor)
	{
		return false;
	}

	// 对象的位置被锁，返回失败
	if (InActor->IsLockLocation())
	{
		return false;
	}

	if (GEditor->PlayWorld)
	{
		if (bIsSimulateInEditorViewport)
		{
			// 检查该对象是否是编辑器预览世界中的对象
			if (!(GEditor->PlayWorld == InActor->GetOuter()->GetOuter()))
			{
				return false;
			}
		}
		else if (!(GEditor->EditorWorld == InActor->GetOuter()->GetOuter()))
		{
			return false;
		}
	}

	return true;
}

void FSimpleEditorViewportClient::GetSelectedActorsAndComponentsForMove(TArray<AActor*>& OutActorsToMove, TArray<USceneComponent*>& OutComponentsToMove) const
{
	OutActorsToMove.Reset();
	OutComponentsToMove.Reset();


	// 检查编辑器目前是否选中了组件
	if (GEditor->GetSelectedComponentCount() > 0)
	{
		// 遍历选中的可编辑组件
		for (FSelectedEditableComponentIterator EditableComponentIt(GEditor->GetSelectedEditableComponentIterator()); EditableComponentIt; ++EditableComponentIt)
		{
			USceneComponent* SceneComponent = Cast<USceneComponent>(*EditableComponentIt);
			if (!SceneComponent)
			{
				continue;
			}

			// 递归检查是否有组件的父组件被选中
			bool bParentAlsoSelected = false;
			USceneComponent* Parent = SceneComponent->GetAttachParent();
			while (Parent != nullptr)
			{
				if (Parent->IsSelected())
				{
					bParentAlsoSelected = true;
					break;
				}

				Parent = Parent->GetAttachParent();
			}

			// 如果该对象不允许移动，则跳过
			AActor* ComponentOwner = SceneComponent->GetOwner();
			if (!CheckActorCanMove(ComponentOwner))
			{
				continue;
			}

			const bool bIsRootComponent = (ComponentOwner && (ComponentOwner->GetRootComponent() == SceneComponent));
			// 如果选中的是跟组件，则该根组件的对象符合条件
			if (bIsRootComponent)
			{
				OutActorsToMove.Add(ComponentOwner);
			}
			// 如果该组件的父组件没有被选中，则符合条件
			else if (!bParentAlsoSelected)
			{
				OutComponentsToMove.Add(SceneComponent);
			}
		}
	}

	// 如果没有符合条件的组件或者对象，则直接返回
	if (OutComponentsToMove.Num() || OutActorsToMove.Num())
	{
		return;
	}

	// 遍历编辑器选中的对象，防止漏掉部分对象
	for (FSelectionIterator It(GEditor->GetSelectedActorIterator()); It; ++It)
	{
		AActor* Actor = CastChecked<AActor>(*It);

		USceneComponent* RootComponent = Actor->GetRootComponent();
		if (RootComponent && RootComponent->IsSelected())
		{
			continue;
		}

		if (!CheckActorCanMove(Actor))
		{
			continue;
		}

		OutActorsToMove.Add(Actor);
	}
}

AActor* FSimpleEditorViewportClient::GetLockedActor() const
{
	return LockedActor.Get();
}

void FSimpleEditorViewportClient::SetLockedActor(AActor* InActor)
{
	LockActorInternal(InActor);
}

bool FSimpleEditorViewportClient::CheckActorIsLocked(const TWeakObjectPtr<AActor> TestActor) const
{
	if (!TestActor.IsValid())
	{
		return false;
	}

	if (!LockedActor.IsValid())
	{
		return false;
	}

	return TestActor.Get() == LockedActor.Get();
}

bool FSimpleEditorViewportClient::HasAnyActorLocked() const
{
	return LockedActor.IsValid();
}

AActor* FSimpleEditorViewportClient::GetSelectedActor() const
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

USceneComponent* FSimpleEditorViewportClient::GetSelectedComponent() const
{
	if (USelection* Selection = GEditor->GetSelectedComponents())
	{
		TArray<USceneComponent*> SelectedComponents;
		int32 Num = Selection->GetSelectedObjects(SelectedComponents);
		if (Num > 0)
		{
			for (USceneComponent* Component : SelectedComponents)
			{
				if (Component->GetWorld() == GetWorld())
				{
					return Component;
				}
			}
		}
	}

	return nullptr;
}

void FSimpleEditorViewportClient::LockActorInternal(AActor* InActor)
{
	LockedActor = InActor;

	MoveViewportToLockedActor();
}

void FSimpleEditorViewportClient::MoveViewportToLockedActor()
{
	if (LockedActor.IsValid())
	{
		SetViewLocation(LockedActor->GetActorLocation());
		SetViewRotation(LockedActor->GetActorRotation());
		if (UCameraComponent* CameraComp = Cast<UCameraComponent>(LockedActor->GetComponentByClass(UCameraComponent::StaticClass())))
		{
			ViewFOV = CameraComp->FieldOfView;
		}

		Invalidate();
	}
}

void FSimpleEditorViewportClient::MoveLockedActorToViewport()
{
	if (LockedActor.IsValid())
	{
		if (!LockedActor->IsLockLocation())
		{
			LockedActor->SetActorLocation(GetViewLocation(), false);
			LockedActor->SetActorRotation(GetViewRotation());

			USceneComponent* LockedActorComponent = LockedActor->GetRootComponent();
			TOptional<FRotator> PreviousRotator;
			if (LockedActorComponent)
			{
				PreviousRotator = LockedActorComponent->GetRelativeRotation();
			}

			if (LockedActorComponent)
			{
				const FRotator Rot = PreviousRotator.GetValue();
				FRotator ActorRotWind, ActorRotRem;
				Rot.GetWindingAndRemainder(ActorRotWind, ActorRotRem);
				const FQuat ActorQ = ActorRotRem.Quaternion();
				const FQuat ResultQ = LockedActorComponent->GetRelativeRotation().Quaternion();
				FRotator NewActorRotRem = FRotator(ResultQ);
				ActorRotRem.SetClosestToMe(NewActorRotRem);
				FRotator DeltaRot = NewActorRotRem - ActorRotRem;
				DeltaRot.Normalize();
				LockedActorComponent->SetRelativeRotationExact(Rot + DeltaRot);
			}
		}
	}
}

void FSimpleEditorViewportClient::OnActorMoved(AActor* InActor)
{
	if (CheckActorIsLocked(InActor))
	{
		MoveViewportToLockedActor();
	}
}

#pragma endregion Logic



#pragma region Widget
FVector FSimpleEditorViewportClient::GetWidgetLocation() const
{
	return ModeTools->GetWidgetLocation();
}

FMatrix FSimpleEditorViewportClient::GetWidgetCoordSystem() const
{
	return ModeTools->GetCustomInputCoordinateSystem();
}

void FSimpleEditorViewportClient::SetWidgetCoordSystemSpace(ECoordSystem NewCoordSystem)
{
	ModeTools->SetCoordSystem(NewCoordSystem);

	RedrawAllViewportsIntoThisScene();
}

ECoordSystem FSimpleEditorViewportClient::GetWidgetCoordSystemSpace() const
{
	return ModeTools->GetCoordSystem();
}

void FSimpleEditorViewportClient::SetWidgetMode(UE::Widget::EWidgetMode NewMode)
{ 
	WidgetMode = NewMode; 
}

UE::Widget::EWidgetMode FSimpleEditorViewportClient::GetWidgetMode() const
{ 
	return WidgetMode; 
}

bool FSimpleEditorViewportClient::InputWidgetDelta(FViewport* InViewport, EAxisList::Type InCurrentAxis, FVector& InDrag, FRotator& InRot, FVector& InScale)
{
	// 如果当前正在编辑可视化组件（例如SplineComponent），则直接退出
	if (GUnrealEd->ComponentVisManager.HandleInputDelta(this, InViewport, InDrag, InRot, InScale))
	{
		return true;
	}

	bool bHandled = false;

	AActor* SelectedActor = GetSelectedActor();
	USceneComponent* SelectedComponent = GetSelectedComponent();
	if ((SelectedActor || SelectedComponent) && InCurrentAxis != EAxisList::None)
	{
		// 将增量应用于对象上
		if (SelectedComponent)
		{
			GEditor->ApplyDeltaToComponent
			(
				SelectedComponent, true,
				&InDrag, &InRot, &InScale,
				SelectedComponent->GetRelativeLocation()
			);
		}
		else if (SelectedActor && !SelectedActor->IsA<AWorldSettings>())
		{
			GEditor->ApplyDeltaToActor
			(
				SelectedActor, true,
				&InDrag, &InRot, &InScale,
				false, false, false
			);
		}

		if (!InRot.IsZero())
		{
			FRotator TranslateRotateWidgetRotation(0, ModeTools->TranslateRotateXAxisAngle, 0);
			TranslateRotateWidgetRotation += InRot;
			ModeTools->TranslateRotateXAxisAngle = TranslateRotateWidgetRotation.Yaw;

			FRotator Widget2DRotation(ModeTools->TranslateRotate2DAngle, 0, 0);
			Widget2DRotation += InRot;
			ModeTools->TranslateRotate2DAngle = Widget2DRotation.Pitch;
		}
		ModeTools->PivotLocation += InDrag;
		ModeTools->SnappedLocation += InDrag;

		bHandled = true;
	}

	return bHandled;
}

#pragma endregion Widget

