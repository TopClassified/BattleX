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
) : FSimpleEditorViewportClient(nullptr, InPreviewScene, InViewportWidget), ViewportCameraMode(BXTLViewportCameraMode::Free)
{
	// ModeTools传nullptr由基类自建私有实例:复用GLevelEditorModeTools时,UE5.8新TRS Gizmo的
	// ITF点击交互组会吞掉预览视口的点击(仅服务关卡编辑器),导致无法选中/拖拽
	CachedEditor = InEditor;

	SetRealtime(true);

	ViewFOV = 90.0f;

	DrawHelper.bDrawGrid = true;
	DrawHelper.AxesLineThickness = 0.0f;
	// UE5.8: PerspectiveGridSize 为 float,HALF_WORLD_MAX1 为 double,显式截断消除精度截断警告
	DrawHelper.PerspectiveGridSize = static_cast<float>(HALF_WORLD_MAX1);

	EngineShowFlags.Game = 0;
	EngineShowFlags.ScreenSpaceReflections = 1;
	EngineShowFlags.AmbientOcclusion = 1;
	EngineShowFlags.SetSnap(0);
	EngineShowFlags.DisableAdvancedFeatures();
	EngineShowFlags.SetSeparateTranslucency(true);
	EngineShowFlags.SetCompositeEditorPrimitives(true);
	EngineShowFlags.SetParticles(true);
	// Profiles可能为空或索引越界(用户设置损坏时),越界检查防止崩溃
	const TArray<FPreviewSceneProfile>& ViewerProfiles = UAssetViewerSettings::Get()->Profiles;
	const int32 ProfileIndex = GetMutableDefault<UEditorPerProjectUserSettings>()->AssetViewerProfileIndex;
	if (ViewerProfiles.IsValidIndex(ProfileIndex) && ViewerProfiles[ProfileIndex].bPostProcessingEnabled)
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
	double TickDelta = DeltaSeconds;
	if (const UBXTLEditorSettings* Settings = GetDefault<UBXTLEditorSettings>())
	{
		TickDelta = 1.0f / Settings->PreviewTickRate;
		TickDelta = FMath::FloorToDouble((RemainTime + DeltaSeconds) / TickDelta) * TickDelta;
		RemainTime = RemainTime + DeltaSeconds - TickDelta;
	}
	if (ForceFPS > 0.0f)
	{
		TickDelta = 1.0f / ForceFPS;
	}
	if (TickDelta <= 0.0f)
	{
		return;
	}
	
	FSimpleEditorViewportClient::Tick(TickDelta);

	// 播放中隐藏Gizmo禁止拖拽(空闲/暂停世界静止可自由摆放)
	const bool bRunningNow = CachedEditor.IsValid() && CachedEditor.Pin()->IsRunning();
	if (bRunningNow != bLastRunningState)
	{
		bLastRunningState = bRunningNow;
		ShowWidget(!bRunningNow);
	}

	// 编辑器逻辑Tick须独立于世界Tick运行:空闲/暂停时世界静止(便于拖拽摆放),
	// 但轨道刷新(SectionsToShow→RefreshPanelEvent)、时间属性对齐、Debug运行高亮等编辑器逻辑仍需每帧驱动,
	// 否则编辑器打开后时间轴面板空白,须先Play一次(世界恢复Tick)才能看到内容
	if (CachedEditor.IsValid())
	{
		CachedEditor.Pin()->Tick(TickDelta);
	}

	if (CachedEditor.IsValid() && !CachedEditor.Pin()->ShouldPauseWorld())
	{
		TickWorld(TickDelta);
	}
}

bool FBXTLEditorViewportClient::InputWidgetDelta(FViewport* InViewport, EAxisList::Type InCurrentAxis, FVector& InDrag, FRotator& InRot, FVector& InScale)
{
	// 播放中拒绝拖拽增量(兜底,Gizmo已隐藏)
	if (CachedEditor.IsValid() && CachedEditor.Pin()->IsRunning())
	{
		return true;
	}

	return FSimpleEditorViewportClient::InputWidgetDelta(InViewport, InCurrentAxis, InDrag, InRot, InScale);
}

void FBXTLEditorViewportClient::TickWorld(float DeltaSeconds)
{
	HandlerPreviewScenePreTick();

	// 编辑器逻辑Tick已上移至Tick(独立于世界暂停状态),此处仅负责推进世界

	// PreviewScene可能未就绪(视口先于场景创建时Tick被触发)
	if (PreviewScene && PreviewScene->GetWorld())
	{
		PreviewScene->GetWorld()->Tick(LEVELTICK_All, DeltaSeconds);
	}

	HandlerPreviewScenePostTick();
}

void FBXTLEditorViewportClient::SetForceFPS(double InFPS)
{
	ForceFPS = InFPS;
}

void FBXTLEditorViewportClient::HandlerPreviewScenePreTick()
{

}

void FBXTLEditorViewportClient::HandlerPreviewScenePostTick()
{
	if (ViewportCameraMode == BXTLViewportCameraMode::RealGame)
	{
		// 预览世界可能无玩家相机管理器,判空防止崩溃
		APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
		if (CameraManager)
		{
			SetViewLocation(CameraManager->GetCameraLocation());
			SetViewRotation(CameraManager->GetCameraRotation());

			ViewFOV = CameraManager->GetFOVAngle();

			Invalidate();
		}
	}
}

#pragma endregion Important



#pragma region Logic
void FBXTLEditorViewportClient::ProcessClick(FSceneView& View, HHitProxy* HitProxy, FKey Key, EInputEvent Event, uint32 HitX, uint32 HitY)
{
	// 处于运行时，屏蔽点击逻辑
	if (CachedEditor.IsValid() && CachedEditor.Pin()->IsRunning())
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
