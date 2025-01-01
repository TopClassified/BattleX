#pragma once

#include "GraphEditor.h"
#include "EdGraphUtilities.h"
#include "Misc/NotifyHook.h"
#include "ClassViewerFilter.h"
#include "Toolkits/AssetEditorToolkit.h"
#include "Editor/PropertyEditor/Public/IDetailsView.h"

#include "Widgets/Docking/SDockTab.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

#include "BXDecisionTreeTemplate.h"



class FBXDTEditorGraphPanelNodeFactory : public FGraphPanelNodeFactory
{
	virtual TSharedPtr<SGraphNode> CreateNode(UEdGraphNode* Node) const override;
};






class FBXDTEditor : public FAssetEditorToolkit, public FNotifyHook, public FGCObject
{
#pragma region Important
public:
	FBXDTEditor();
	
	virtual ~FBXDTEditor();

	void InitEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UBXDecisionTreeTemplate* InTreeTemp);

	virtual void OnClose() override;

	virtual void SaveAsset_Execute() override;

	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;

	virtual FString GetDocumentationLink() const override;
	
	virtual FString GetReferencerName() const override;

private:
	void PreSaveExecute();

private:
	TSharedPtr<FGraphPanelNodeFactory> NodeFactory;
	
	TWeakObjectPtr<UBXDecisionTreeTemplate> DecisionTreeTemplate = nullptr;
	
#pragma endregion Important



#pragma region Tools
public:
	TSharedPtr<class FBXDTEditorToolbar> GetToolbarBuilder();
	
	virtual FName GetToolkitFName() const override;
	
	virtual FText GetBaseToolkitName() const override;
	
	virtual FText GetToolkitName() const override;
	
	virtual FText GetToolkitToolTipText() const override;

private:
	TSharedPtr<class FBXDTEditorToolbar> ToolbarBuilder;
	
#pragma endregion Tools



#pragma region Tab
public:
	void RegisterToolbarTab(const TSharedRef<class FTabManager>& TabManager);
	
	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& TabManager) override;
	
	virtual void UnregisterTabSpawners(const TSharedRef<FTabManager>& TabManager) override;

	virtual FString GetWorldCentricTabPrefix() const override;

	virtual FLinearColor GetWorldCentricTabColorScale() const override;

private:
	TSharedRef<SDockTab> SpawnTab_Viewport(const FSpawnTabArgs& Args);
	
	TSharedRef<SDockTab> SpawnTab_Details(const FSpawnTabArgs& Args);
	
	TSharedRef<SDockTab> SpawnTab_EditorSettings(const FSpawnTabArgs& Args);

private:
	// 编辑窗口
	TSharedPtr<SDockTab> ViewportTab;

#pragma endregion Tab



#pragma region Editor
private:
	// 创建主要的编辑窗口
	void CreateMainEditWidgets();
	
	// 创建新的节点编辑窗口
	bool CreateViewportWidget(FString ViewportName, UEdGraph* OwnerGraph);

	void CreateEdGraph(UObject* TheOwner, UEdGraph*& TheGraphPtr);

	void CreateCommandList();

	void BindCommands();

	TSharedPtr<SGraphEditor> GetCurrentGraphEditor() const;

	void SelectAllNodes();
	bool CanSelectAllNodes();
	FGraphPanelSelectionSet GetSelectedNodes() const;

	void DeleteNodes(FGraphPanelSelectionSet& SelectedNodes);
	void DeleteSelectedNodes();
	bool CanDeleteNodes();
	void DeleteSelectedDuplicatableNodes();

	void CutSelectedNodes();
	bool CanCutNodes();

	void CopySelectedNodes();
	bool CanCopyNodes();

	void PasteNodes();
	void PasteNodesHere(const FVector2D& Location);
	bool CanPasteNodes();

	void DuplicateNodes();
	bool CanDuplicateNodes();

	void OpenSelectNode();
	bool CanOpenSelectNode();

	// 回到上一层
	void BackToPreviousGraph();

	void GraphSettings();
	bool CanGraphSettings() const;

	void OnRenameNode();
	bool CanRenameNodes() const;

private:
	void RefreshGraphMessage(UEdGraph* CurSaveGraph);

	void CheckGraphNodeType(UEdGraph* CurSaveGraph);

	void CollectRootMessage(UEdGraph* CurSaveGraph, TArray<UBXDecisionTreeNode*>& RootNodes);

private:
	// 编辑窗口栈
	TArray<TSharedPtr<SGraphEditor>> ViewportWidgetStack;

	TSharedPtr<IDetailsView> PropertyWidget;
	
	TSharedPtr<IDetailsView> EditorSettingsWidget;

	TSharedPtr<FUICommandList> GraphEditorCommands;
	
#pragma endregion Editor



#pragma region Event
protected:
	void OnNodeDoubleClicked(UEdGraphNode* Node);

	void OnSelectedNodesChanged(const TSet<class UObject*>& NewSelection);

	void OnFinishedChangingProperties(const FPropertyChangedEvent& PropertyChangedEvent);
	
#pragma endregion Event



#pragma region ClassFilter
private:
	class FDTTypeFilter : public IClassViewerFilter
	{
	public:
		virtual bool IsClassAllowed(const FClassViewerInitializationOptions& InInitOptions, const UClass* InClass, TSharedRef<FClassViewerFilterFuncs> InFilterFuncs) override
		{
			return InClass->IsChildOf<UBXDecisionTreeType>() && !InClass->HasAnyClassFlags(CLASS_Abstract);
		}

		virtual bool IsUnloadedClassAllowed(const FClassViewerInitializationOptions& InInitOptions, const TSharedRef< const IUnloadedBlueprintData > InClass, TSharedRef<FClassViewerFilterFuncs> InFilterFuncs) override
		{
			return InClass->IsChildOf(UBXDecisionTreeType::StaticClass()) && !InClass->HasAnyClassFlags(CLASS_Abstract);
		}
	};

#pragma endregion ClassFilter

};