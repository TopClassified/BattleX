#include "BXTLEditorViewportClient.h"
#include "EngineUtils.h"
#include "UnrealEdGlobals.h"
#include "EditorModeManager.h"
#include "Editor/UnrealEdEngine.h"
#include "Editor/EditorPerProjectUserSettings.h"

#include "IXRTrackingSystem.h"
#include "SceneViewExtension.h"
#include "AssetViewerSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/PlayerCameraManager.h"
#include "GameFramework/GameMode.h"
#include "GameFramework/GameState.h"
#include "GameFramework/WorldSettings.h"

#include "BXTLEditor.h"
#include "BXTLEditorSettings.h"
#include "BXTLEditorDelegates.h"
#include "Preview/BXTLPreviewScene.h" 



const FName BXTLViewportCameraMode::Free = "Free";
const FName BXTLViewportCameraMode::RealGame = "RealGame";



#pragma region Important
FBXTLEditorViewportClient::FBXTLEditorViewportClient
(
	TSharedPtr<FBXTLEditor> InEditor, FPreviewScene* InPreviewScene, const TWeakPtr<SEditorViewport>& InViewportWidget
) : FSimpleEditorViewportClient(&GLevelEditorModeTools(), InPreviewScene, InViewportWidget), ViewportCameraMode(BXTLViewportCameraMode::Free)
{
	CachedEditor = InEditor;

	SetRealtime(true);

	ViewFOV = 90.0f;

	DrawHelper.bDrawGrid = true;
	DrawHelper.AxesLineThickness = 0.0f;
	DrawHelper.PerspectiveGridSize = HALF_WORLD_MAX1;

	EngineShowFlags.Game = 0;
	EngineShowFlags.ScreenSpaceReflections = 1;
	EngineShowFlags.AmbientOcclusion = 1;
	EngineShowFlags.SetSnap(0);
	EngineShowFlags.DisableAdvancedFeatures();
	EngineShowFlags.SetSeparateTranslucency(true);
	EngineShowFlags.SetCompositeEditorPrimitives(true);
	EngineShowFlags.SetParticles(true);
	if (UAssetViewerSettings::Get()->Profiles[GetMutableDefault<UEditorPerProjectUserSettings>()->AssetViewerProfileIndex].bPostProcessingEnabled)
	{
		EngineShowFlags.EnableAdvancedFeatures();
	}
	else
	{
		EngineShowFlags.DisableAdvancedFeatures();
	}

	if (CachedEditor.IsValid())
	{
		CachedEditor.Pin()->PreviewChangedEvent.AddRaw(this, &FBXTLEditorViewportClient::OnPreviewStateChanged);
	}
}

FBXTLEditorViewportClient::~FBXTLEditorViewportClient()
{
	if (CachedEditor.IsValid())
	{
		CachedEditor.Pin()->PreviewChangedEvent.RemoveAll(this);
	}
}

void FBXTLEditorViewportClient::Tick(float DeltaSeconds)
{
	FSimpleEditorViewportClient::Tick(DeltaSeconds);

	if (CachedEditor.IsValid() && !CachedEditor.Pin()->ShouldPauseWorld())
	{
		TickWorld(DeltaSeconds);
	}
}

void FBXTLEditorViewportClient::TickWorld(float DeltaSeconds)
{
	HandlerPreviewScenePreTick();

	if (CachedEditor.IsValid())
	{
		CachedEditor.Pin()->Tick(DeltaSeconds);
	}

	if (PreviewScene->GetWorld())
	{
		PreviewScene->GetWorld()->Tick(LEVELTICK_All, DeltaSeconds);
	}

	HandlerPreviewScenePostTick();
}

void FBXTLEditorViewportClient::HandlerPreviewScenePreTick()
{

}

void FBXTLEditorViewportClient::HandlerPreviewScenePostTick()
{
	if (ViewportCameraMode == BXTLViewportCameraMode::RealGame)
	{
		APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
		SetViewLocation(CameraManager->GetCameraLocation());
		SetViewRotation(CameraManager->GetCameraRotation());

		ViewFOV = CameraManager->GetFOVAngle();

		Invalidate();
	}
}

#pragma endregion Important



#pragma region Logic
void FBXTLEditorViewportClient::ProcessClick(FSceneView& View, HHitProxy* HitProxy, FKey Key, EInputEvent Event, uint32 HitX, uint32 HitY)
{
	// 处于运行时，屏蔽点击逻辑
	if (CachedEditor.IsValid() && CachedEditor.Pin()->IsPlaying() && !CachedEditor.Pin()->IsPaused())
	{
		return;
	}

	// 部分类型的Actor屏蔽点击事件
	if (HitProxy && HitProxy->IsA(HActor::StaticGetType()))
	{
		HActor* ActorHitProxy = (HActor*)HitProxy;
		AActor* ConsideredActor = ActorHitProxy->Actor;
		if (ConsideredActor && (ConsideredActor->IsA<AGameMode>() || ConsideredActor->IsA<AGameState>() || ConsideredActor->IsA<AController>()))
		{
			return;
		}
	}

	FSimpleEditorViewportClient::ProcessClick(View, HitProxy, Key, Event, HitX, HitY);
}

#pragma endregion Logic



#pragma region Event
void FBXTLEditorViewportClient::OnPreviewStateChanged(bool InPlaying, bool InPause)
{
	if (InPlaying && !InPause)
	{
		SetWidgetMode(UE::Widget::EWidgetMode::WM_None);
	}
	else
	{
		SetWidgetMode(UE::Widget::EWidgetMode::WM_Translate);
	}
}

#pragma endregion Event



#pragma region Viewport
void FBXTLEditorViewportClient::ChangeViewMode(const FName& InCameraMode)
{
	ViewportCameraMode = InCameraMode;

	// 重置旋转量
	FRotator RawRot = GetViewRotation();
	RawRot.Roll = 0.f;
	SetViewRotation(RawRot);
	EnableCameraLock(false);

	Invalidate();
}

FName FBXTLEditorViewportClient::GetViewMode() const
{
	return ViewportCameraMode;
}

void FBXTLEditorViewportClient::OverridePostProcessSettings(FSceneView& View)
{
	FSimpleEditorViewportClient::OverridePostProcessSettings(View);

	if (APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0))
	{
		TArray<float> const* CameraAnimPPBlendWeights;
		TArray<FPostProcessSettings> const* CameraAnimPPSettings;

		CameraManager->GetCachedPostProcessBlends(CameraAnimPPSettings, CameraAnimPPBlendWeights);

		for (int32 PPIdx = 0; PPIdx < CameraAnimPPBlendWeights->Num(); ++PPIdx)
		{
			View.OverridePostProcessSettings((*CameraAnimPPSettings)[PPIdx], (*CameraAnimPPBlendWeights)[PPIdx]);
		}
	}
}

#pragma endregion Viewport
