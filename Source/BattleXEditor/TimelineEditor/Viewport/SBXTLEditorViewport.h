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

	virtual TSharedPtr<SWidget> MakeViewportToolbar() override;

protected:
	TWeakPtr<class FBXTLEditor> CachedEditor = nullptr;

#pragma endregion Important

};