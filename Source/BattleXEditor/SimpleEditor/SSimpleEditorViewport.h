#pragma once

#include "CoreMinimal.h"
#include "EditorUndoClient.h"
#include "IPersonaViewport.h"

#include "SEditorViewport.h"
#include "SViewportToolBar.h"

#include "SimpleEditorViewportClient.h"
#include "Toolkits/AssetEditorToolkit.h"


struct FEditorViewportParameter
{
public:
	FEditorViewportParameter(TSharedRef<FPreviewScene> InPreviewScene, TSharedRef<FAssetEditorToolkit> InAssetEditorToolkit, int32 InViewportIndex) : PreviewScene(InPreviewScene), AssetEditorToolkit(InAssetEditorToolkit), ViewportIndex(InViewportIndex){}
	
public:
	TSharedRef<FPreviewScene> PreviewScene;

	TSharedRef<FAssetEditorToolkit> AssetEditorToolkit;

	int32 ViewportIndex;
};



class BATTLEXEDITOR_API SSimpleEditorViewport : public SEditorViewport, public FEditorUndoClient
{
#pragma region Important
public:
	SLATE_BEGIN_ARGS(SSimpleEditorViewport) {}
	SLATE_END_ARGS()

	// 构造函数
	void Construct(const FArguments& InArgs, const FEditorViewportParameter& InParameter);
	
	// 创建窗口逻辑体
	virtual TSharedRef<FEditorViewportClient> MakeEditorViewportClient() override;

	// 创建窗口工具栏
	virtual TSharedPtr<SWidget> MakeViewportToolbar() override;

	// 绑定命令
	virtual void BindCommands() override;

protected:
	// 窗口的逻辑体
	TSharedPtr<FSimpleEditorViewportClient> ViewportClient;

	// 窗口的工具栏
	TSharedPtr<SViewportToolBar> ViewportToolbar;

	// 窗口的场景
	TWeakPtr<FPreviewScene> PreviewScenePtr;

	// 编辑器工具包
	TWeakPtr<FAssetEditorToolkit> AssetEditorToolkitPtr;

	// 窗口ID
	int32 ViewportIndex;

	FDelegateHandle PreviewFeatureLevelChangedHandle;

#pragma endregion Important

	
	
#pragma region EditLogic
public:
	// 撤销
	virtual void PostUndo(bool bSuccess);

	// 重做
	virtual void PostRedo(bool bSuccess);

	// 获取窗口对应的世界
	virtual UWorld* GetWorld() const override;

	TSharedPtr<SOverlay> GetViewportOverlay();

	// 获取窗口的逻辑体
	TSharedPtr<FSimpleEditorViewportClient> GetSimpleViewportClient() const;

protected:
	// 切换成游戏视角
	void ToggleGameView();

	// 是否可以切换成游戏视角
	bool CanToggleGameView() const;

	// 是否处于游戏视角
	bool IsInGameView() const;

	// 解锁对象
	void OnActorUnlock();

	// 是否可以解锁对象
	bool CanExecuteActorUnlock() const;

#pragma endregion EditLogic

};
