#pragma once

#include "SimpleEditorViewportClient.h"
#include "LevelEditorViewport.h"

#include "Preview/BXTLPreviewScene.h" 



namespace BXTLViewportCameraMode
{
	extern const FName Free;
	extern const FName RealGame;
};



class FBXTLEditorViewportClient : public FSimpleEditorViewportClient
{
#pragma region Important
public:
	FBXTLEditorViewportClient(TSharedPtr<class FBXTLEditor> InEditor, FPreviewScene* InPreviewScene, const TWeakPtr<SEditorViewport>& InViewportWidget);

	virtual ~FBXTLEditorViewportClient();

	virtual void Tick(float DeltaSeconds) override;

	void TickWorld(float DeltaSeconds);

private:
	void HandlerPreviewScenePreTick();

	void HandlerPreviewScenePostTick();

private:
	float RemainTick = 0.0f;

	TWeakPtr<class FBXTLEditor> CachedEditor = nullptr;

#pragma endregion Important



#pragma region Logic
public:
	// 处理点击事件
	void ProcessClick(FSceneView& View, HHitProxy* HitProxy, FKey Key, EInputEvent Event, uint32 HitX, uint32 HitY) override;

#pragma endregion Logic



#pragma region Event
public:
	// 预览状态发生改变的回调事件
	void OnPreviewStateChanged(bool InPlaying, bool InPause);

#pragma endregion Event



#pragma region Viewport
public:
	void ChangeViewMode(const FName& InCameraMode);

	FName GetViewMode() const;
	
	virtual void OverridePostProcessSettings(FSceneView& View) override;

private:
	FName ViewportCameraMode;

#pragma endregion Viewport

};
