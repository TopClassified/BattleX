#include "SBXTLEditorViewport.h"
#include "Editor/EditorEngine.h"
#include "Editor/UnrealEdEngine.h"
#include "Widgets/SOverlay.h"
#include "Widgets/Layout/SBox.h"

#include "Engine/Selection.h"
#include "Slate/SceneViewport.h"
#include "Framework/Commands/UICommandList.h"
#include "Framework/Application/SlateApplication.h"

#include "BXTLEditor.h"
#include "Preview/BXTLPreviewScene.h"

#include "Viewports.h"
#include "Viewport/BXTLEditorViewportToolbar.h"
#include "Viewport/BXTLEditorViewportClient.h"



#define LOCTEXT_NAMESPACE "BXTLEditorViewport"

SBXTLEditorViewport::~SBXTLEditorViewport()
{
	UEditorEngine* Editor = (UEditorEngine*)GEngine;
	Editor->OnPreviewFeatureLevelChanged().Remove(PreviewFeatureLevelChangedHandle);
}

void SBXTLEditorViewport::Construct(const FArguments& InArgs, const FEditorViewportParameter& InParameter)
{
	CachedEditor = StaticCastSharedRef<FBXTLEditor>(InParameter.AssetEditorToolkit);

	SSimpleEditorViewport::Construct
	(
		SSimpleEditorViewport::FArguments()
		.IsEnabled(FSlateApplication::Get().GetNormalExecutionAttribute())
		.AddMetaData<FTagMetaData>(TEXT("BX.BXTLEditor.Viewport")),
		InParameter
	);

	TSharedPtr<FBXTLPreviewScene> PreviewScene = CachedEditor.Pin()->GetPreviewScene();
	PreviewScene->OnCreateViewport(this, SceneViewport);

	Client->VisibilityDelegate.BindSP(this, &SBXTLEditorViewport::IsVisible);
}

void SBXTLEditorViewport::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	SSimpleEditorViewport::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

}

TSharedRef<FEditorViewportClient> SBXTLEditorViewport::MakeEditorViewportClient()
{
	TSharedPtr<FBXTLEditorViewportClient> NewViewportClient = MakeShareable(new FBXTLEditorViewportClient(CachedEditor.Pin(), CachedEditor.Pin()->GetPreviewScene().Get(), SharedThis(this)));

	NewViewportClient->ViewportType = LVT_Perspective;
	NewViewportClient->bSetListenerPosition = false;
	NewViewportClient->SetViewLocation(EditorViewportDefs::DefaultPerspectiveViewLocation);
	NewViewportClient->SetViewRotation(EditorViewportDefs::DefaultPerspectiveViewRotation);
	NewViewportClient->SetRealtime(true);
	NewViewportClient->SetShowStats(true);
	ViewportClient = NewViewportClient;

	return NewViewportClient.ToSharedRef();
}

TSharedPtr<SWidget> SBXTLEditorViewport::MakeViewportToolbar()
{
	return SNew(SVerticalBox)
	.Visibility(EVisibility::SelfHitTestInvisible)
	+ SVerticalBox::Slot()
	.AutoHeight()
	.VAlign(VAlign_Top)
	[
		SNew(SBXTLEditorViewportToolBar, CachedEditor.Pin(), SharedThis(this)).Cursor(EMouseCursor::Default)
	];
}

#undef LOCTEXT_NAMESPACE