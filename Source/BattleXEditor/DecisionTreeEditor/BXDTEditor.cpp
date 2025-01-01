#include "BXDTEditor.h"

#include "IDetailsView.h"
#include "Misc/FileHelper.h"
#include "Modules/ModuleManager.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Framework/Commands/GenericCommands.h"
#include "ScopedTransaction.h"
#include "GraphEditorActions.h"
#include "PropertyEditorModule.h"
#include "Kismet2/SClassPickerDialog.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "HAL/PlatformFilemanager.h"
#include "HAL/PlatformApplicationMisc.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

#include "BXDTEditorGraph.h"
#include "BXDTEditorSchema.h"
#include "BXDTEditorToolbar.h"
#include "BXDTEditorCommands.h"
#include "BXDTEditorGraphNode.h"
#include "BXDTEditorGraphEdge.h"
#include "SBXDTEditorGraphEdge.h"
#include "SBXDTEditorGraphNode.h"



#define LOCTEXT_NAMESPACE "DecisionTreeEditor"

TSharedPtr<SGraphNode> FBXDTEditorGraphPanelNodeFactory::CreateNode(UEdGraphNode* Node) const
{
	if (UBXDTEditorGraphNode* EdNode_GraphNode = Cast<UBXDTEditorGraphNode>(Node))
	{
		return SNew(SBXDTEditorGraphNode, EdNode_GraphNode);
	}
	else if (UBXDTEditorGraphEdge* EdNode_GrpahEdge = Cast<UBXDTEditorGraphEdge>(Node))
	{
		return SNew(SBXDTEditorGraphEdge, EdNode_GrpahEdge);
	}

	return nullptr;
}






const FName DecisionTreeEditorAppName = FName(TEXT("DecisionTreeEditor"));

struct FDecisionTreeEditorTabs
{
	static const FName PropertyID;
	static const FName ViewportID;
	static const FName SettingsID;
};
const FName FDecisionTreeEditorTabs::PropertyID(TEXT("DecisionTreeProperty"));
const FName FDecisionTreeEditorTabs::ViewportID(TEXT("Viewport"));
const FName FDecisionTreeEditorTabs::SettingsID(TEXT("DecisionTreeSettings"));



#pragma region Important
FBXDTEditor::FBXDTEditor()
{
	DecisionTreeTemplate = nullptr;

	NodeFactory = MakeShareable(new FBXDTEditorGraphPanelNodeFactory());
	FEdGraphUtilities::RegisterVisualNodeFactory(NodeFactory);
}

FBXDTEditor::~FBXDTEditor()
{
	if (NodeFactory.IsValid())
	{
		FEdGraphUtilities::UnregisterVisualNodeFactory(NodeFactory);
		NodeFactory.Reset();
	}
}

void FBXDTEditor::InitEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UBXDecisionTreeTemplate* InTreeTemp)
{
	DecisionTreeTemplate = InTreeTemp;
	// 初始化资源对象
	if (DecisionTreeTemplate.IsValid())
	{
		CreateEdGraph(DecisionTreeTemplate.Get(), DecisionTreeTemplate->EdGraph);
		DecisionTreeTemplate->InitByEditor();

		if (DecisionTreeTemplate->TreeType)
		{
			UBXDecisionTreeType* TreeType = Cast<UBXDecisionTreeType>(DecisionTreeTemplate->TreeType.GetDefaultObject());

			if (TreeType && DecisionTreeTemplate->EdGraph && DecisionTreeTemplate->EdGraph->Nodes.Num() <= 0 && TreeType->FirstNodeType)
			{
				UBXDTEditorGraphNode* FirstNode = NewObject<UBXDTEditorGraphNode>(DecisionTreeTemplate->EdGraph);
				FirstNode->GraphNode = NewObject<UBXDecisionTreeNode>(DecisionTreeTemplate->EdGraph->GetOuter(), TreeType->FirstNodeType);
				FirstNode->GraphNode->ConstructNode();

				DecisionTreeTemplate->EdGraph->AddNode(FirstNode, true, false);
				FirstNode->CreateNewGuid();
				FirstNode->PostPlacedNewNode();
				FirstNode->AllocateDefaultPins();
				FirstNode->NodePosX = 0.0f;
				FirstNode->NodePosY = 0.0f;
				FirstNode->GraphNode->SetFlags(RF_Transactional);
				FirstNode->SetFlags(RF_Transactional);
			}

			if (TreeType && TreeType->LoadAssetTypes.Num() > 0)
			{
				TArray<FAssetData> AssetData;
				FAssetRegistryModule& AssetRegistryModule = FModuleManager::GetModuleChecked<FAssetRegistryModule>("AssetRegistry");
				FARFilter Filter;
				Filter.PackagePaths.Add("/Game");
				for (int32 i = 0; i < TreeType->LoadAssetTypes.Num(); ++i)
				{
					Filter.ClassPaths.Add(TreeType->LoadAssetTypes[i]->GetClassPathName());
				}
				Filter.bRecursivePaths = true;
				Filter.bRecursiveClasses = true;
				AssetData.Empty();
				AssetRegistryModule.Get().GetAssets(Filter, AssetData);
			}
		}
	}

	FGraphEditorCommands::Register();
	FBXDTEditorCommands::Register();

	BindCommands();
	CreateCommandList();

	if (!ToolbarBuilder.IsValid())
	{
		ToolbarBuilder = MakeShareable(new FBXDTEditorToolbar(SharedThis(this)));
	}

	TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
	ToolbarBuilder->AddGraphToolbar(ToolbarExtender);

	// 创建主要的编辑窗口
	CreateMainEditWidgets();

	// Layout
	const TSharedRef<FTabManager::FLayout> StandaloneDefaultLayout = FTabManager::NewLayout("Standalone_DecisionTreeEditor_Layout_v1")
		->AddArea
		(
			FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)
			->Split
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.1f)
				->AddTab(TEXT("DecisionTreeEditor"), ETabState::OpenedTab)->SetHideTabWell(true)
			)
			->Split
			(
				FTabManager::NewSplitter()->SetOrientation(Orient_Horizontal)->SetSizeCoefficient(0.9f)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.65f)
					->AddTab(FDecisionTreeEditorTabs::ViewportID, ETabState::OpenedTab)->SetHideTabWell(true)
				)
				->Split
				(
					FTabManager::NewSplitter()->SetOrientation(Orient_Vertical)
					->Split
					(
						FTabManager::NewStack()
						->SetSizeCoefficient(0.7f)
						->AddTab(FDecisionTreeEditorTabs::PropertyID, ETabState::OpenedTab)->SetHideTabWell(true)
					)
					->Split
					(
						FTabManager::NewStack()
						->SetSizeCoefficient(0.3f)
						->AddTab(FDecisionTreeEditorTabs::SettingsID, ETabState::OpenedTab)
					)
				)
			)
		);

	const bool bCreateDefaultStandaloneMenu = true;
	const bool bCreateDefaultToolbar = true;
	FAssetEditorToolkit::InitAssetEditor
	(
		Mode, InitToolkitHost, DecisionTreeEditorAppName, StandaloneDefaultLayout,
		bCreateDefaultStandaloneMenu, bCreateDefaultToolbar, DecisionTreeTemplate.Get(), false
	);

	RegenerateMenusAndToolbars();

	// 初始化时刷新一遍节点/边的文字内容
	for (int32 i = 0; i < DecisionTreeTemplate->EdGraph->Nodes.Num(); ++i)
	{
		if (UBXDTEditorGraphNode* CurNode = Cast<UBXDTEditorGraphNode>(DecisionTreeTemplate->EdGraph->Nodes[i]))
		{
			if (CurNode->GraphNode)
			{
				CurNode->GraphNode->RefreshNodeTitle();
			}
		}
		else if (UBXDTEditorGraphEdge* CurEdge = Cast<UBXDTEditorGraphEdge>(DecisionTreeTemplate->EdGraph->Nodes[i]))
		{
			if (CurEdge->GraphEdge)
			{
				CurEdge->GraphEdge->RefreshEdgeTitle();
			}
		}
	}
}

void FBXDTEditor::OnClose()
{
	FAssetEditorToolkit::OnClose();
}

void FBXDTEditor::SaveAsset_Execute()
{
	PreSaveExecute();
	
	FAssetEditorToolkit::SaveAsset_Execute();
}

void FBXDTEditor::AddReferencedObjects(FReferenceCollector& Collector)
{
	Collector.AddReferencedObject(DecisionTreeTemplate);
}

FString FBXDTEditor::GetDocumentationLink() const
{
	return TEXT("");
}

FString FBXDTEditor::GetReferencerName() const
{
	return "FDecisionTreeEditor";
}

void FBXDTEditor::PreSaveExecute()
{
	DecisionTreeTemplate->ID = FCString::Atoi(*DecisionTreeTemplate->GetName());

	RefreshGraphMessage(DecisionTreeTemplate->EdGraph);
}

#pragma endregion Important



#pragma region Tools
TSharedPtr<FBXDTEditorToolbar> FBXDTEditor::GetToolbarBuilder()
{
	return ToolbarBuilder;
}

FName FBXDTEditor::GetToolkitFName() const
{
	return FName("FBXDTEditor");
}

FText FBXDTEditor::GetBaseToolkitName() const
{
	return LOCTEXT("DecisionTreeEditorAppLabel", "DecisionTree Editor");
}

FText FBXDTEditor::GetToolkitName() const
{
	const bool bDirtyState = DecisionTreeTemplate->GetOutermost()->IsDirty();

	FFormatNamedArguments Args;
	Args.Add(TEXT("DecisionTreeName"), FText::FromString(DecisionTreeTemplate->GetName()));
	Args.Add(TEXT("DirtyState"), bDirtyState ? FText::FromString(TEXT("*")) : FText::GetEmpty());
	return FText::Format(LOCTEXT("DecisionTreeEditorToolkitName", "{DecisionTreeName}{DirtyState}"), Args);
}

FText FBXDTEditor::GetToolkitToolTipText() const
{
	return FAssetEditorToolkit::GetToolTipTextForObject(DecisionTreeTemplate.Get());
}

#pragma endregion Tools



#pragma region Tab
void FBXDTEditor::RegisterToolbarTab(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);
}

void FBXDTEditor::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(LOCTEXT("WorkspaceMenu_DecisionTreeEditor", "DecisionTree Editor"));
	auto WorkspaceMenuCategoryRef = WorkspaceMenuCategory.ToSharedRef();

	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	// 数据可视化面板
	InTabManager->RegisterTabSpawner
	(
		FDecisionTreeEditorTabs::ViewportID, FOnSpawnTab::CreateSP(this, &FBXDTEditor::SpawnTab_Viewport))
		.SetDisplayName(LOCTEXT("GraphCanvasTab", "Viewport"))
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FAppStyle::GetAppStyleSetName(), "GraphEditor.EventGraph_16x")
	);

	// 属性细节面板
	InTabManager->RegisterTabSpawner
	(
		FDecisionTreeEditorTabs::PropertyID, FOnSpawnTab::CreateSP(this, &FBXDTEditor::SpawnTab_Details))
		.SetDisplayName(LOCTEXT("DetailsTab", "Property"))
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FAppStyle::GetAppStyleSetName(), "LevelEditor.Tabs.Details")
	);
}

void FBXDTEditor::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);

	InTabManager->UnregisterTabSpawner(FDecisionTreeEditorTabs::ViewportID);
	InTabManager->UnregisterTabSpawner(FDecisionTreeEditorTabs::PropertyID);
	InTabManager->UnregisterTabSpawner(FDecisionTreeEditorTabs::SettingsID);
}

FString FBXDTEditor::GetWorldCentricTabPrefix() const
{
	return TEXT("DecisionTreeEditor");
}

FLinearColor FBXDTEditor::GetWorldCentricTabColorScale() const
{
	return FLinearColor::White;
}

TSharedRef<SDockTab> FBXDTEditor::SpawnTab_Viewport(const FSpawnTabArgs& Args)
{
	check(Args.GetTabId() == FDecisionTreeEditorTabs::ViewportID);

	SAssignNew(ViewportTab, SDockTab).Label(LOCTEXT("ViewportTab_Title", "Viewport"));

	if (ViewportWidgetStack.Num() > 0)
		ViewportTab->SetContent(ViewportWidgetStack[0].ToSharedRef());

	return ViewportTab.ToSharedRef();
}

TSharedRef<SDockTab> FBXDTEditor::SpawnTab_Details(const FSpawnTabArgs& Args)
{
	check(Args.GetTabId() == FDecisionTreeEditorTabs::PropertyID);

	TSharedPtr<SDockTab> TheTab = SNew(SDockTab)
		.Label(LOCTEXT("Details_Title", "Property"))
		[
			PropertyWidget.ToSharedRef()
		];

	TheTab->SetTabIcon(FAppStyle::GetBrush("LevelEditor.Tabs.Details"));

	return TheTab.ToSharedRef();
}

TSharedRef<SDockTab> FBXDTEditor::SpawnTab_EditorSettings(const FSpawnTabArgs& Args)
{
	check(Args.GetTabId() == FDecisionTreeEditorTabs::SettingsID);

	TSharedPtr<SDockTab> TheTab = SNew(SDockTab)
		.Label(LOCTEXT("EditorSettings_Title", "Decision Tree Editor Setttings"))
		[
			EditorSettingsWidget.ToSharedRef()
		];

	TheTab->SetTabIcon(FAppStyle::GetBrush("LevelEditor.Tabs.Details"));

	return TheTab.ToSharedRef();
}

#pragma endregion Tab



#pragma region Editor
void FBXDTEditor::CreateMainEditWidgets()
{
	// 创建节点编辑窗口
	CreateViewportWidget(TEXT("Decision Tree Editor"), DecisionTreeTemplate->EdGraph);

	FDetailsViewArgs Args;
	Args.bHideSelectionTip = true;
	Args.NotifyHook = this;

	// 创建属性编辑窗口
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyWidget = PropertyModule.CreateDetailView(Args);
	PropertyWidget->SetObject(DecisionTreeTemplate.Get());
	PropertyWidget->OnFinishedChangingProperties().AddSP(this, &FBXDTEditor::OnFinishedChangingProperties);
}

bool FBXDTEditor::CreateViewportWidget(FString ViewportName, UEdGraph* OwnerGraph)
{
	FGraphAppearanceInfo AppearanceInfo;
	AppearanceInfo.CornerText = FText::FromString(ViewportName);

	SGraphEditor::FGraphEditorEvents InEvents;
	InEvents.OnSelectionChanged = SGraphEditor::FOnSelectionChanged::CreateSP(this, &FBXDTEditor::OnSelectedNodesChanged);
	InEvents.OnNodeDoubleClicked = FSingleNodeEvent::CreateSP(this, &FBXDTEditor::OnNodeDoubleClicked);

	TSharedPtr<SGraphEditor> NewViewPort;

	SAssignNew(NewViewPort, SGraphEditor)
		.AdditionalCommands(GraphEditorCommands)
		.IsEditable(true)
		.Appearance(AppearanceInfo)
		.GraphToEdit(OwnerGraph)
		.GraphEvents(InEvents)
		.AutoExpandActionMenu(true)
		.ShowGraphStateOverlay(false);

	if (NewViewPort.IsValid())
	{
		ViewportWidgetStack.Add(NewViewPort);
		return true;
	}
	
	return false;
}

void FBXDTEditor::CreateEdGraph(UObject* TheOwner, UEdGraph*& TheGraphPtr)
{
	// 如果该决策树还没有可视化界面，则进行创建
	if (!TheGraphPtr)
	{
		TheGraphPtr = CastChecked<UBXDTEditorGraph>
		(
			FBlueprintEditorUtils::CreateNewGraph
			(
				TheOwner, NAME_None,
				UBXDTEditorGraph::StaticClass(),
				UBXDTEditorSchema::StaticClass()
			)
		);
	}
}

void FBXDTEditor::CreateCommandList()
{
	if (GraphEditorCommands.IsValid())
		return;

	GraphEditorCommands = MakeShareable(new FUICommandList);

	GraphEditorCommands->MapAction
	(
		FGenericCommands::Get().SelectAll,
		FExecuteAction::CreateRaw(this, &FBXDTEditor::SelectAllNodes),
		FCanExecuteAction::CreateRaw(this, &FBXDTEditor::CanSelectAllNodes)
	);

	GraphEditorCommands->MapAction
	(
		FGenericCommands::Get().Delete,
		FExecuteAction::CreateRaw(this, &FBXDTEditor::DeleteSelectedNodes),
		FCanExecuteAction::CreateRaw(this, &FBXDTEditor::CanDeleteNodes)
	);

	GraphEditorCommands->MapAction
	(
		FGenericCommands::Get().Copy,
		FExecuteAction::CreateRaw(this, &FBXDTEditor::CopySelectedNodes),
		FCanExecuteAction::CreateRaw(this, &FBXDTEditor::CanCopyNodes)
	);

	GraphEditorCommands->MapAction
	(
		FGenericCommands::Get().Cut,
		FExecuteAction::CreateRaw(this, &FBXDTEditor::CutSelectedNodes),
		FCanExecuteAction::CreateRaw(this, &FBXDTEditor::CanCutNodes)
	);

	GraphEditorCommands->MapAction
	(
		FGenericCommands::Get().Paste,
		FExecuteAction::CreateRaw(this, &FBXDTEditor::PasteNodes),
		FCanExecuteAction::CreateRaw(this, &FBXDTEditor::CanPasteNodes)
	);

	GraphEditorCommands->MapAction
	(
		FGenericCommands::Get().Duplicate,
		FExecuteAction::CreateRaw(this, &FBXDTEditor::DuplicateNodes),
		FCanExecuteAction::CreateRaw(this, &FBXDTEditor::CanDuplicateNodes)
	);

	GraphEditorCommands->MapAction
	(
		FGenericCommands::Get().Rename,
		FExecuteAction::CreateSP(this, &FBXDTEditor::OnRenameNode),
		FCanExecuteAction::CreateSP(this, &FBXDTEditor::CanRenameNodes)
	);
}

void FBXDTEditor::BindCommands()
{
	const FBXDTEditorCommands& Commands = FBXDTEditorCommands::Get();
	const TSharedRef<FUICommandList>& UICommandList = GetToolkitCommands();

	UICommandList->MapAction(Commands.BackToPreviousGraph, FExecuteAction::CreateSP(this, &FBXDTEditor::BackToPreviousGraph));
}

TSharedPtr<SGraphEditor> FBXDTEditor::GetCurrentGraphEditor() const
{
	return ViewportWidgetStack.Last();
}

void FBXDTEditor::SelectAllNodes()
{
	TSharedPtr<SGraphEditor> CurrentGraphEditor = GetCurrentGraphEditor();
	if (CurrentGraphEditor.IsValid())
	{
		CurrentGraphEditor->SelectAllNodes();
	}
}

bool FBXDTEditor::CanSelectAllNodes()
{
	return true;
}

FGraphPanelSelectionSet FBXDTEditor::GetSelectedNodes() const
{
	FGraphPanelSelectionSet CurrentSelection;
	TSharedPtr<SGraphEditor> FocusedGraphEd = GetCurrentGraphEditor();
	if (FocusedGraphEd.IsValid())
	{
		CurrentSelection = FocusedGraphEd->GetSelectedNodes();
	}

	return CurrentSelection;
}

void FBXDTEditor::DeleteNodes(FGraphPanelSelectionSet& SelectedNodes)
{
	for (FGraphPanelSelectionSet::TConstIterator NodeIt(SelectedNodes); NodeIt; ++NodeIt)
	{
		UEdGraphNode* EdNode = Cast<UEdGraphNode>(*NodeIt);
		if (!EdNode || !EdNode->CanUserDeleteNode())
		{
			continue;
		}

		// 如果是决策树节点，则有可能要断开节点之间的链接关系
		if (UBXDTEditorGraphNode* EdNode_Node = Cast<UBXDTEditorGraphNode>(EdNode))
		{
			EdNode_Node->Modify();

			if (const UEdGraphSchema* Schema = EdNode_Node->GetSchema())
			{
				Schema->BreakNodeLinks(*EdNode_Node);
			}

			EdNode_Node->DestroyNode();
		}
		else
		{
			EdNode->Modify();
			EdNode->DestroyNode();
		}
	}
}

void FBXDTEditor::DeleteSelectedNodes()
{
	TSharedPtr<SGraphEditor> CurrentGraphEditor = GetCurrentGraphEditor();
	if (!CurrentGraphEditor.IsValid())
	{
		return;
	}

	const FScopedTransaction Transaction(FGenericCommands::Get().Delete->GetDescription());

	CurrentGraphEditor->GetCurrentGraph()->Modify();
	FGraphPanelSelectionSet SelectedNodes = CurrentGraphEditor->GetSelectedNodes();
	CurrentGraphEditor->ClearSelectionSet();

	DeleteNodes(SelectedNodes);
}

bool FBXDTEditor::CanDeleteNodes()
{
	const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
	for (FGraphPanelSelectionSet::TConstIterator SelectedIter(SelectedNodes); SelectedIter; ++SelectedIter)
	{
		UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter);
		if (Node && Node->CanUserDeleteNode())
		{
			return true;
		}
	}

	return false;
}

void FBXDTEditor::DeleteSelectedDuplicatableNodes()
{
	TSharedPtr<SGraphEditor> CurrentGraphEditor = GetCurrentGraphEditor();
	if (!CurrentGraphEditor.IsValid())
	{
		return;
	}

	const FGraphPanelSelectionSet OldSelectedNodes = CurrentGraphEditor->GetSelectedNodes();
	CurrentGraphEditor->ClearSelectionSet();

	for (FGraphPanelSelectionSet::TConstIterator SelectedIter(OldSelectedNodes); SelectedIter; ++SelectedIter)
	{
		UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter);
		if (Node && Node->CanDuplicateNode())
		{
			CurrentGraphEditor->SetNodeSelection(Node, true);
		}
	}

	DeleteSelectedNodes();

	CurrentGraphEditor->ClearSelectionSet();

	for (FGraphPanelSelectionSet::TConstIterator SelectedIter(OldSelectedNodes); SelectedIter; ++SelectedIter)
	{
		if (UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter))
		{
			CurrentGraphEditor->SetNodeSelection(Node, true);
		}
	}
}

void FBXDTEditor::CutSelectedNodes()
{
	CopySelectedNodes();
	DeleteSelectedDuplicatableNodes();
}

bool FBXDTEditor::CanCutNodes()
{
	return CanCopyNodes() && CanDeleteNodes();
}

void FBXDTEditor::CopySelectedNodes()
{
	FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();

	FString ExportedText;

	for (FGraphPanelSelectionSet::TIterator SelectedIter(SelectedNodes); SelectedIter; ++SelectedIter)
	{
		UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter);
		if (!Node)
		{
			SelectedIter.RemoveCurrent();
			continue;
		}

		// 如果要复制一条边，要检查它的连接点是否需要被复制
		if (UBXDTEditorGraphEdge* EdNode_Edge = Cast<UBXDTEditorGraphEdge>(*SelectedIter))
		{
			UBXDTEditorGraphNode* StartNode = EdNode_Edge->GetStartNode();
			UBXDTEditorGraphNode* EndNode = EdNode_Edge->GetEndNode();

			if (!SelectedNodes.Contains(StartNode) || !SelectedNodes.Contains(EndNode))
			{
				SelectedIter.RemoveCurrent();
				continue;
			}
		}

		Node->PrepareForCopying();
	}

	FEdGraphUtilities::ExportNodesToText(SelectedNodes, ExportedText);
	FPlatformApplicationMisc::ClipboardCopy(*ExportedText);
}

bool FBXDTEditor::CanCopyNodes()
{
	const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
	for (FGraphPanelSelectionSet::TConstIterator SelectedIter(SelectedNodes); SelectedIter; ++SelectedIter)
	{
		UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter);
		if (Node && Node->CanDuplicateNode())
		{
			return true;
		}
	}

	return false;
}

void FBXDTEditor::PasteNodes()
{
	TSharedPtr<SGraphEditor> CurrentGraphEditor = GetCurrentGraphEditor();

	if (CurrentGraphEditor.IsValid())
		PasteNodesHere(CurrentGraphEditor->GetPasteLocation());
}

void FBXDTEditor::PasteNodesHere(const FVector2D& Location)
{
	TSharedPtr<SGraphEditor> CurrentGraphEditor = GetCurrentGraphEditor();
	if (!CurrentGraphEditor.IsValid())
	{
		return;
	}

	UEdGraph* EdGraph = CurrentGraphEditor->GetCurrentGraph();
	if (!EdGraph)
	{
		return;
	}

	const FScopedTransaction Transaction(FGenericCommands::Get().Paste->GetDescription());
	EdGraph->Modify();

	CurrentGraphEditor->ClearSelectionSet();

	FString TextToImport;
	FPlatformApplicationMisc::ClipboardPaste(TextToImport);

	TSet<UEdGraphNode*> PastedNodes;
	FEdGraphUtilities::ImportNodesFromText(EdGraph, TextToImport, PastedNodes);

	// 创建新的数据节点对象，从旧的对象中获取数据并进行复制
	for (TSet<UEdGraphNode*>::TIterator It(PastedNodes); It; ++It)
	{
		UEdGraphNode* Node = *It;

		// 处理节点
		if (UBXDTEditorGraphNode* TmpNode = Cast<UBXDTEditorGraphNode>(Node))
		{
			UBXDecisionTreeNode* OriginNode = TmpNode->GraphNode;
			TmpNode->GraphNode = NewObject<UBXDecisionTreeNode>(DecisionTreeTemplate.Get(), OriginNode->GetClass());
			TmpNode->GraphNode->CopyData(OriginNode);

			TmpNode->GraphNode->RefreshNodeTitle();
		}
		// 处理边
		else if (UBXDTEditorGraphEdge* TmpEdge = Cast<UBXDTEditorGraphEdge>(Node))
		{
			UBXDecisionTreeEdge* OriginEdge = TmpEdge->GraphEdge;
			TmpEdge->GraphEdge = NewObject<UBXDecisionTreeEdge>(DecisionTreeTemplate.Get(), OriginEdge->GetClass());
			TmpEdge->GraphEdge->CopyData(OriginEdge);

			TmpEdge->GraphEdge->RefreshEdgeTitle();
		}
	}

	// 计算所有复制节点的平均位置
	FVector2D AvgNodePosition(0.0f, 0.0f);
	float InvNumNodes = 1.0f / float(PastedNodes.Num());
	for (TSet<UEdGraphNode*>::TIterator It(PastedNodes); It; ++It)
	{
		UEdGraphNode* Node = *It;
		AvgNodePosition.X += Node->NodePosX;
		AvgNodePosition.Y += Node->NodePosY;
	}
	AvgNodePosition.X *= InvNumNodes;
	AvgNodePosition.Y *= InvNumNodes;

	// 将复制出来的节点放到正确的位置上
	for (TSet<UEdGraphNode*>::TIterator It(PastedNodes); It; ++It)
	{
		UEdGraphNode* Node = *It;
		CurrentGraphEditor->SetNodeSelection(Node, true);

		Node->NodePosX = (Node->NodePosX - AvgNodePosition.X) + Location.X;
		Node->NodePosY = (Node->NodePosY - AvgNodePosition.Y) + Location.Y;

		Node->SnapToGrid(16);

		Node->CreateNewGuid();
	}

	// 标记相关文件需要保存
	CurrentGraphEditor->NotifyGraphChanged();
	UObject* GraphOwner = EdGraph->GetOuter();
	if (GraphOwner)
	{
		GraphOwner->PostEditChange();
		GraphOwner->MarkPackageDirty();
	}
}

bool FBXDTEditor::CanPasteNodes()
{
	TSharedPtr<SGraphEditor> CurrentGraphEditor = GetCurrentGraphEditor();
	if (!CurrentGraphEditor.IsValid())
	{
		return false;
	}

	FString ClipboardContent;
	FPlatformApplicationMisc::ClipboardPaste(ClipboardContent);

	return FEdGraphUtilities::CanImportNodesFromText(CurrentGraphEditor->GetCurrentGraph(), ClipboardContent);
}

void FBXDTEditor::DuplicateNodes()
{
	CopySelectedNodes();
	PasteNodes();
}

bool FBXDTEditor::CanDuplicateNodes()
{
	return CanCopyNodes();
}

void FBXDTEditor::OpenSelectNode()
{
	FGraphPanelSelectionSet CurSelectInfo = GetSelectedNodes();
	
	UBXDTEditorGraphNode* EdGraphNode = Cast<UBXDTEditorGraphNode>(CurSelectInfo.Array()[0]);
	if (!IsValid(EdGraphNode))
	{
		return;
	}

	UBXDecisionTreeNode* GraphNode = Cast<UBXDecisionTreeNode>(EdGraphNode->GraphNode);
	if (!IsValid(GraphNode))
	{
		return;
	}
	
	/*bool CanOpenSubGraph = false;
	if (GraphNode->TreeType)
	{
		CanOpenSubGraph = true;
	}
	else
	{
		// 尝试找到默认的类型信息
		GraphNode->TryFindDefaultTypeMessage();

		if (!GraphNode->TypeMessage)
		{
			FClassViewerInitializationOptions Options;
			Options.Mode = EClassViewerMode::ClassPicker;
			Options.bIsActorsOnly = false;
			Options.bIsBlueprintBaseOnly = true;
			Options.ClassFilters.Add(MakeShared<FDTTypeFilter>());
			Options.ExtraPickerCommonClasses.Add(NULL);

			UClass* ChosenClass = NULL;
			const bool bPressOk = SClassPickerDialog::PickClass
			(
				LOCTEXT("SetSubGraphType", "Graph Type Message"),
				Options, ChosenClass, UDTTypeMessage::StaticClass()
			);

			if (bPressOk && ChosenClass)
			{
				CanOpenSubGraph = true;
				GraphNode->TypeMessage = ChosenClass;
			}
		}
		else
		{
			CanOpenSubGraph = true;
		}
	}
			
	CreateEdGraph(GraphNode, GraphNode->EdGraph);
	CreateViewportWidget(TEXT("Graph Node Editor"), GraphNode->EdGraph);
			
	if (CanOpenSubGraph && ViewportWidgetStack.Num() > 0)
	{
		ViewportTab->SetContent(ViewportWidgetStack.Last().ToSharedRef());
	}*/
}

bool FBXDTEditor::CanOpenSelectNode()
{
	FGraphPanelSelectionSet CurSelectInfo = GetSelectedNodes();
	if (CurSelectInfo.Num() != 1)
	{
		return false;
	}

	UBXDTEditorGraphNode* EdGraphNode = Cast<UBXDTEditorGraphNode>(CurSelectInfo.Array()[0]);
	if (!IsValid(EdGraphNode))
	{
		return false;
	}
	
	if (EdGraphNode->GraphNode->GetClass()->IsChildOf<UBXDecisionTreeNode>())
	{
		return true;
	}

	return false;
}

void FBXDTEditor::BackToPreviousGraph()
{
	if (ViewportWidgetStack.Num() > 1)
	{
		ViewportWidgetStack.RemoveAt(ViewportWidgetStack.Num() - 1);
		ViewportTab->SetContent(ViewportWidgetStack.Last().ToSharedRef());
	}
	else if (ViewportWidgetStack.Num() > 0)
	{
		ViewportTab->SetContent(ViewportWidgetStack.Last().ToSharedRef());
	}
}

void FBXDTEditor::GraphSettings()
{
	PropertyWidget->SetObject(DecisionTreeTemplate.Get());
}

bool FBXDTEditor::CanGraphSettings() const
{
	return true;
}

void FBXDTEditor::OnRenameNode()
{
	TSharedPtr<SGraphEditor> CurrentGraphEditor = GetCurrentGraphEditor();
	if (CurrentGraphEditor.IsValid())
	{
		const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
		for (FGraphPanelSelectionSet::TConstIterator NodeIt(SelectedNodes); NodeIt; ++NodeIt)
		{
			UEdGraphNode* SelectedNode = Cast<UEdGraphNode>(*NodeIt);
			if (SelectedNode != NULL && SelectedNode->bCanRenameNode)
			{
				CurrentGraphEditor->IsNodeTitleVisible(SelectedNode, true);
				break;
			}
		}
	}
}

bool FBXDTEditor::CanRenameNodes() const
{
	return true;
}

void FBXDTEditor::RefreshGraphMessage(UEdGraph* CurSaveGraph)
{
	if (!CurSaveGraph)
	{
		return;
	}
	
	/* 递归刷新子图表
	for (int32 i = 0; i < CurSaveGraph->Nodes.Num(); ++i)
	{
		if (UBXDTEditorGraphNode* CurEdNode = Cast<UBXDTEditorGraphNode>(CurSaveGraph->Nodes[i]))
		{
			if (UBXDecisionTreeNode* CurNode = Cast<UBXDecisionTreeNode>(CurEdNode->GraphNode))
			{
				RefreshGraphMessage(CurNode->EdGraph);
			}
		}
	}*/
	
	// 先检查图表中的节点类型是否合法
	CheckGraphNodeType(CurSaveGraph);
	
	// 对节点按照“从左到右”进行排序
	CurSaveGraph->Nodes.Sort
	(
		[](const UEdGraphNode& A, const UEdGraphNode& B)
		{
			return A.NodePosX < B.NodePosX;
		}
	);
	
	// 刷新图表信息
	/*if (UBXDecisionTreeNode* CurNode = Cast<UBXDecisionTreeNode>(CurSaveGraph->GetOuter()))
	{
		CurNode->InitSaver();

		for (int32 i = 0; i < CurSaveGraph->Nodes.Num(); ++i)
		{
			if (UBXDTEditorGraphEdge* CheckEdEdge = Cast<UBXDTEditorGraphEdge>(CurSaveGraph->Nodes[i]))
			{
				CurNode->AddEdgeMessage(CheckEdEdge->GetStartNode()->GraphNode, CheckEdEdge->GetEndNode()->GraphNode, CheckEdEdge->GraphEdge);
			}
		}

		CollectRootMessage(CurSaveGraph, CurNode->RootNodes);

		CurNode->RefreshLogicMessage();
	}
	else*/
	if (UBXDecisionTreeTemplate* CurTemp = Cast<UBXDecisionTreeTemplate>(CurSaveGraph->GetOuter()))
	{
		CurTemp->InitSaver();

		for (int32 i = 0; i < CurSaveGraph->Nodes.Num(); ++i)
		{
			if (UBXDTEditorGraphEdge* CheckEdEdge = Cast<UBXDTEditorGraphEdge>(CurSaveGraph->Nodes[i]))
			{
				CurTemp->AddEdgeMessage(CheckEdEdge->GetStartNode()->GraphNode, CheckEdEdge->GetEndNode()->GraphNode, CheckEdEdge->GraphEdge);
			}
		}

		CollectRootMessage(CurSaveGraph, CurTemp->RootNodes);

		CurTemp->RefreshLogicMessage();
	}
}

void FBXDTEditor::CheckGraphNodeType(UEdGraph* CurSaveGraph)
{
	if (!CurSaveGraph)
	{
		return;
	}

	UBXDecisionTreeType* TreeType = Cast<UBXDecisionTreeType>(DecisionTreeTemplate->TreeType.GetDefaultObject());
	if (!TreeType)
	{
		return;
	}

	// 删除类型不对的节点
	FGraphPanelSelectionSet WillDeleteNodes;
	for (int32 i = 0; i < CurSaveGraph->Nodes.Num(); ++i)
	{
		if (UBXDTEditorGraphNode* CurEdNode = Cast<UBXDTEditorGraphNode>(CurSaveGraph->Nodes[i]))
		{
			// 检查节点类型
			if (!TreeType->NodeTypes.Contains(CurEdNode->GraphNode->GetClass()))
			{
				WillDeleteNodes.Add(CurEdNode);
				continue;
			}
		}

		// 检查边类型
		if (UBXDTEditorGraphEdge* CurEdEdge = Cast<UBXDTEditorGraphEdge>(CurSaveGraph->Nodes[i]))
		{
			if (TreeType->EdgeType != CurEdEdge->GraphEdge->GetClass())
			{
				WillDeleteNodes.Add(CurEdEdge);
			}
		}
	}
	DeleteNodes(WillDeleteNodes);
}

void FBXDTEditor::CollectRootMessage(UEdGraph* CurSaveGraph, TArray<UBXDecisionTreeNode*>& RootNodes)
{
	// 如果没有任何边的终点是该节点，则认为该节点是一个根节点
	for (int32 i = 0; i < CurSaveGraph->Nodes.Num(); ++i)
	{
		UBXDTEditorGraphNode* CheckEdNode = Cast<UBXDTEditorGraphNode>(CurSaveGraph->Nodes[i]);
		if (!IsValid(CheckEdNode))
		{
			continue;
		}
		
		if (CheckEdNode->GraphNode && CheckEdNode->GraphNode->InEdges.Num() <= 0)
		{
			RootNodes.Add(CheckEdNode->GraphNode);
		}
	}
}

#pragma endregion Editor



#pragma region Event
void FBXDTEditor::OnNodeDoubleClicked(UEdGraphNode* Node)
{
	if (CanOpenSelectNode())
	{
		OpenSelectNode();
	}
}

void FBXDTEditor::OnSelectedNodesChanged(const TSet<UObject*>& NewSelection)
{
	TArray<UObject*> Selection;

	for (UObject* SelectionEntry : NewSelection)
	{
		Selection.Add(SelectionEntry);
	}

	if (Selection.Num() == 0)
	{
		PropertyWidget->SetObject(DecisionTreeTemplate.Get());
	}
	else
	{
		PropertyWidget->SetObjects(Selection);
	}
}

void FBXDTEditor::OnFinishedChangingProperties(const FPropertyChangedEvent& PropertyChangedEvent)
{
	return;
}

#pragma endregion Event

#undef LOCTEXT_NAMESPACE
