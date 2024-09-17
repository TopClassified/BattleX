#include "SSimpleEditorViewport.h"
#include "Framework/Application/SlateApplication.h"
#include "AdvancedPreviewSceneModule.h"
#include "LevelViewportActions.h"
#include "PreviewScene.h"


#pragma region Important
void SSimpleEditorViewport::Construct(const FArguments& InArgs, const FEditorViewportParameter& InParameter)
{
	PreviewScenePtr = InParameter.PreviewScene;
	AssetEditorToolkitPtr = InParameter.AssetEditorToolkit;
	ViewportIndex = InParameter.ViewportIndex;

	SEditorViewport::Construct(SEditorViewport::FArguments().IsEnabled(FSlateApplication::Get().GetNormalExecutionAttribute()).AddMetaData<FTagMetaData>(TEXT("SimpleViewport")));

	if (Client)
	{
		Client->VisibilityDelegate.BindSP(this, &SSimpleEditorViewport::IsVisible);
	}

	// 恢复上次使用的功能级别
	auto ScenePtr = PreviewScenePtr.Pin();
	if (ScenePtr.IsValid())
	{
		UWorld* World = ScenePtr->GetWorld();
		if (World != nullptr)
		{
			World->SetFeatureLevel(GWorld->GetFeatureLevel());
		}
	}

	// 这里是关于场景渲染的设置发生改变后，需要通知世界设置成新的Feature
	UEditorEngine* Editor = (UEditorEngine*)GEngine;
	PreviewFeatureLevelChangedHandle = Editor->OnPreviewFeatureLevelChanged().AddLambda
	(
		[this](ERHIFeatureLevel::Type NewFeatureLevel)
		{
			auto ScenePtr = PreviewScenePtr.Pin();
			if (ScenePtr.IsValid())
			{
				UWorld* World = ScenePtr->GetWorld();
				if (World != nullptr)
				{
					World->ChangeFeatureLevel(NewFeatureLevel);
				}
			}
		}
	);
}

TSharedRef<FEditorViewportClient> SSimpleEditorViewport::MakeEditorViewportClient()
{
	ViewportClient = MakeShareable(new FSimpleEditorViewportClient(nullptr, PreviewScenePtr.Pin().Get(), SharedThis(this)));
	ViewportClient->ViewportType = LVT_Perspective;
	ViewportClient->bSetListenerPosition = false;
	ViewportClient->SetViewLocation(EditorViewportDefs::DefaultPerspectiveViewLocation);
	ViewportClient->SetViewRotation(EditorViewportDefs::DefaultPerspectiveViewRotation);

	return ViewportClient.ToSharedRef();
}

TSharedPtr<SWidget> SSimpleEditorViewport::MakeViewportToolbar()
{
	return SAssignNew(ViewportToolbar, SViewportToolBar);
}

void SSimpleEditorViewport::BindCommands()
{
	SEditorViewport::BindCommands();

	FUICommandList& CommandListRef = *CommandList;
	const FLevelViewportCommands& LevelViewportActions = FLevelViewportCommands::Get();
	CommandListRef.MapAction
	(
		LevelViewportActions.ToggleGameView,
		FExecuteAction::CreateSP(this, &SSimpleEditorViewport::ToggleGameView),
		FCanExecuteAction::CreateSP(this, &SSimpleEditorViewport::CanToggleGameView),
		FIsActionChecked::CreateSP(this, &SSimpleEditorViewport::IsInGameView)
	);

	CommandListRef.MapAction
	(
		LevelViewportActions.EjectActorPilot,
		FExecuteAction::CreateSP(this, &SSimpleEditorViewport::OnActorUnlock),
		FCanExecuteAction::CreateSP(this, &SSimpleEditorViewport::CanExecuteActorUnlock)
	);
}

#pragma endregion Important



#pragma region EditLogic
void SSimpleEditorViewport::PostUndo(bool bSuccess)
{
	ViewportClient->Invalidate();
}

void SSimpleEditorViewport::PostRedo(bool bSuccess)
{
	ViewportClient->Invalidate();
}

UWorld* SSimpleEditorViewport::GetWorld() const
{
	if (PreviewScenePtr.Pin())
	{
		return PreviewScenePtr.Pin()->GetWorld();
	}

	return nullptr;
}

TSharedPtr<SOverlay> SSimpleEditorViewport::GetViewportOverlay()
{
	return ViewportOverlay;
}

TSharedPtr<FSimpleEditorViewportClient> SSimpleEditorViewport::GetSimpleViewportClient() const
{
	return StaticCastSharedPtr<FSimpleEditorViewportClient>(ViewportClient);
}

void SSimpleEditorViewport::ToggleGameView()
{
	if (ViewportClient->IsPerspective())
	{
		bool bGameViewEnable = !ViewportClient->IsInGameView();

		ViewportClient->SetGameView(bGameViewEnable);
	}
}

bool SSimpleEditorViewport::CanToggleGameView() const
{
	return ViewportClient->IsPerspective();
}

bool SSimpleEditorViewport::IsInGameView() const
{
	return ViewportClient->IsInGameView();
}

void SSimpleEditorViewport::OnActorUnlock()
{
	if (ViewportClient->GetLockedActor() != nullptr)
	{
		ViewportClient->SetLockedActor(nullptr);
		ViewportClient->ViewFOV = ViewportClient->FOVAngle;
	}
}

bool SSimpleEditorViewport::CanExecuteActorUnlock() const
{
	return ViewportClient->GetLockedActor() != nullptr;
}

#pragma endregion EditLogic


