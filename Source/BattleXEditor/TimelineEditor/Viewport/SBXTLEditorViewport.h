#pragma once

#include "IDocumentation.h"
#include "SSimpleEditorViewport.h" 



class SBXTLEditorViewport : public SSimpleEditorViewport
{
#pragma region Important
public:
	SLATE_BEGIN_ARGS(SBXTLEditorViewport) {}
	SLATE_END_ARGS()

	virtual ~SBXTLEditorViewport();

	void Construct(const FArguments& InArgs, const FEditorViewportParameter& InParameter);

	void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

	virtual TSharedRef<FEditorViewportClient> MakeEditorViewportClient() override;

	// UE5.8: MakeViewportToolbar 已废弃(final),改为经 PopulateViewportOverlays 在窗口内部顶部插入工具栏,保持原有布局
	virtual void PopulateViewportOverlays(TSharedRef<SOverlay> Overlay) override;

protected:
	TWeakPtr<class FBXTLEditor> CachedEditor = nullptr;

#pragma endregion Important

};