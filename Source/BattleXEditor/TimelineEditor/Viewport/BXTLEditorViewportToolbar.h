#pragma once

#include "CoreMinimal.h"
#include "SViewportToolBar.h"



class SBXTLEditorViewportToolBar : public SViewportToolBar
{
public:
	SLATE_BEGIN_ARGS(SBXTLEditorViewportToolBar) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, TSharedPtr<class FBXTLEditor> InEditor, TSharedPtr<class SBXTLEditorViewport> InViewport);

private:
	EVisibility GetTransformToolBarVisibility() const;

	TSharedRef<class SWidget> GenerateViewMenu();
	
	FText GetCameraModeEntryLable(FName Mode, bool bSkillBone = false) const;

	void SetCameraMode(FName CameraMode);

	FName GetCameraMode() const;

	TSharedRef<class SWidget> MakeFOVWidget() const;

private:
	TWeakPtr<class FBXTLEditor> CachedEditor = nullptr;

	TWeakPtr<class SBXTLEditorViewport> CachedViewport = nullptr;

	TSharedPtr<class FExtender> OptionsMenuExtender = nullptr;
};