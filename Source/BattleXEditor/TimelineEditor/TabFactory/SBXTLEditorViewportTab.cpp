#include "SBXTLEditorViewportTab.h"



void SBXTLEditorViewportTab::Construct(const FArguments& InArgs, const TSharedRef<FBXTLEditor>& InAssetEditorToolkit, const TSharedRef<FBXTLPreviewScene>& InPreviewScene, int32 InViewportIndex)
{
	FEditorViewportParameter ViewportParameter(InPreviewScene, InAssetEditorToolkit, InViewportIndex);
	CachedViewport = SNew(SBXTLEditorViewport, ViewportParameter);

	TSharedPtr<FEditorViewportClient> ViewportClient = CachedViewport->GetViewportClient();
	ViewportClient->SetRealtime(true);

	InAssetEditorToolkit->SetViewport(CachedViewport);

	this->ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.FillHeight(1)
		[
			CachedViewport.ToSharedRef()
		]
	];
}
