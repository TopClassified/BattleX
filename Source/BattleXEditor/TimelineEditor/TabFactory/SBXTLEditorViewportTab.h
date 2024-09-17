#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

#include "BXTLEditor.h"
#include "Preview/BXTLPreviewScene.h"
#include "Viewport/SBXTLEditorViewport.h"



class SBXTLEditorViewportTab : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SBXTLEditorViewportTab) {}
	SLATE_END_ARGS()

public:
	void Construct(const FArguments& InArgs, const TSharedRef<FBXTLEditor>& InAssetEditorToolkit, const TSharedRef<FBXTLPreviewScene>& InPreviewScene, int32 InViewportIndex);

private:
	TSharedPtr<SBXTLEditorViewport> CachedViewport = nullptr;

};