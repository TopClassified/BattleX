#pragma once

#include "CoreMinimal.h"
#include "UObject/GCObject.h"
#include "EditorUndoClient.h"
#include "GraphEditor.h"
#include "WorkflowOrientedApp/WorkflowTabManager.h"
#include "WorkflowOrientedApp/WorkflowCentricApplication.h"
#include "AdvancedPreviewSceneModule.h"
#include "Serialization/JsonSerializer.h"
#include "Subsystems/SubsystemCollection.h"

#include "BXTLEditorDelegates.h" 



namespace BXTLEditorModes
{
	extern const FName Timeline;
};

namespace BXTLEditorTabs
{
	extern const FName Graph;
	extern const FName Viewport;
	extern const FName Timeline;
	extern const FName TaskDetails;
	extern const FName AssetDetails;
};



class IBXTLEditor : public FWorkflowCentricApplication
{
public:
	static TSharedPtr<IBXTLEditor> OpenEditor(UObject* InAsset, const TSharedPtr<IToolkitHost>& InitToolkitHost);

	virtual class UBXTLAsset* GetEditingAsset() = 0;

protected:
	virtual void InitializeEditor(class UBXTLAsset* InAsset, const TSharedPtr<IToolkitHost>& InitToolkitHost) = 0;

	virtual TSharedPtr<class FBXTLPreviewScene> GetPreviewScene() const = 0;

};



class FBXTLEditor : public IBXTLEditor, public FGCObject, public FEditorUndoClient
{
#pragma region Important
public:
	FBXTLEditor();

	virtual ~FBXTLEditor();

	// 初始化
	void InitializeEditor(UBXTLAsset* InAsset, const TSharedPtr<IToolkitHost>& InitToolkitHost) override;

	// 更新
	void Tick(float DeltaTime);

	// 关闭
	void OnClose() override;

	// 获取正在编辑的资源
	class UBXTLAsset* GetEditingAsset();

	// 保存
	void SaveAsset_Execute() override;

	void AddReferencedObjects(FReferenceCollector& Collector) override;

	FString GetReferencerName() const override;

private:
	static int32 BXTLEditorNum;
	
	static int32 BXTLEditorIndex;

	TWeakObjectPtr<class UBXTLAsset> EditAsset = nullptr;
	
#pragma endregion Important



#pragma region Editor
public:
	void RegisterTabSpawners(const TSharedRef<class FTabManager>& TabManager) override;

	void UnregisterTabSpawners(const TSharedRef<class FTabManager>& TabManager) override;

	FText GetBaseToolkitName() const override;

	FName GetToolkitFName() const override;

	FString GetWorldCentricTabPrefix() const override;

	FLinearColor GetWorldCentricTabColorScale() const override;

	// 设置编辑器窗口UI
	void SetViewport(TSharedPtr<class SBXTLEditorViewport> InViewport);

	// 获得编辑器窗口UI
	TSharedPtr<class SBXTLEditorViewport> GetViewportWidget();

	// 设置属性细节窗口UI
	void SetDetailWidget(TSharedPtr<class SBXTLTaskDetailTab> InDetailWidget);

	// 显示对象的属性细节
	void ShowObjectDetail(UObject* InObject);

	// 编辑器模式
	static FText GetLocalizedMode(FName InMode);

	// 获得要显示的轨道
	void GetSectionsToShow(TArray<int32>& OutSectionsToShow) const;

	// 设置要显示的编辑轨道
	void SetEditSectionsToShow(int32 InIndex, bool bShow);

	// 获得要显示的编辑轨道
	void GetEditSectionsToShow(TArray<int32>& OutSectionsToShow) const;

protected:
	// 扩展工具栏
	void ExtendToolbar();

private:
	// 正在显示的轨道索引
	TArray<int32> SectionsToShow;

	// 编辑时显示的轨道索引
	TArray<int32> EditSectionsToShow;

	// 预览时显示的轨道索引
	TArray<int32> PreviewSectionsToShow;

	// 编辑器主窗口
	TSharedPtr<class SBXTLEditorViewport> Viewport;

	// 属性编辑窗口
	TSharedPtr<class SBXTLTaskDetailTab> TaskDetailWidget;

	// 工具栏
	TSharedPtr<class FExtender> ToolbarExtender;

	// 工具栏
	TSharedPtr<class FBXTLEditorToolbar> EditorToolbar;

	// 刷新时间属性时间戳
	int64 AlignTimePropertyTS = 0;

#pragma endregion Editor



#pragma region Command
public:
	// 播放技能
	void Play();

	// 是否正在播放技能
	bool IsPlaying() const;

	// 是否暂停播放技能
	bool IsPaused() const;

	// 是否停止播放技能
	bool IsStopped() const;

	// 暂停技能
	void Pause();

	// 停止技能
	void Stop();

	// 步进技能
	void Step();

	// 重置世界
	void ResetWorld();

	// 显示碰撞预览
	void ShowCollision();

	// 刷新所有时间轴资源数据
	void RefreshTimelineAssetProperty();

	// 自动优化
	int32 AutoOptimize();

protected:
	// 工具栏命令绑定
	void BindCommands();

private:
	// 预览碰撞
	bool bShowCollision = false;

#pragma endregion Command



#pragma region Preview
public:
	virtual TSharedPtr<class FBXTLPreviewScene> GetPreviewScene() const override;

	virtual TSharedPtr<class FBXTLPreviewProxy> GetPreviewProxy() const;

	virtual EBXBodyPartType GetLockedBodyPartType();

	// 是否需要暂停世界
	bool ShouldPauseWorld() const;

protected:
	// 创建预览场景
	void CreatePreviewScene();

	// 创建预览代理
	virtual void CreatePreviewProxy();

	// 初始化上下文
	void InitPreviewContext();

	// 重置上下文
	void ResetPreviewContext();

private:
	// 预览场景
	TSharedPtr<class FBXTLPreviewScene> PreviewScene;

	// 预览代理
	TSharedPtr<class FBXTLPreviewProxy> PreviewProxy;

#pragma endregion Preview



#pragma region Core
public:
	class UBXManager* GetBXManager();

	// 获取当前选中的Task
	TArray<class UBXTask*> GetTaskSelection();

	// Task选取发生改变
	void SetTaskSelection(TArray<class UBXTask*> InSelection, bool bIsOverride = true);

protected:
	// 收集Task类型
	void CollectAllTaskClass();

public:
	TArray<UClass*> TaskClassList;

	TArray<class UUserDefinedEnum*> EnumList;

private:
	TWeakObjectPtr<class UBXManager> CachedManager = nullptr;

	struct FSelectedTaskInfo
	{
		int32 SectionID = -1;
		int32 GroupID = -1;

		TArray<TWeakObjectPtr<class UBXTask>> TaskList;
	};
	FSelectedTaskInfo TaskSelection;

#pragma endregion Core



#pragma region Graph
public:
	void SetGraphEditor(const TSharedPtr<SGraphEditor>& InGraphEditor);

	void SetGraphEditorViewLocationByTask(class UBXTask* InTask);

	void OnSelectedNodesChanged(const TSet<class UObject*>& NewSelection);

	void OnSelectedNodesDeleted();

	void GenerateGraphNodes(TArray<class UBXTask*> InTaskList);

private:
	TSharedPtr<SGraphEditor> GraphEditor = nullptr;

	TArray<TWeakObjectPtr<UObject>> SelectedGraphNodes;

#pragma endregion Graph



#pragma region Event
public:
	FBXTLELogicRefreshPanel RefreshPanelEvent;

	FBXTLELogicPreviewChanged PreviewChangedEvent;
	
	FBXTLELogicTaskSelected TaskSelectedEvent;

#pragma endregion Event

};
