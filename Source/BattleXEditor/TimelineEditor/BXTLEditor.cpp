#include "BXTLEditor.h"
#include "UObject/SavePackage.h"
#include "Modules/ModuleManager.h"
#include "Engine/BlueprintGeneratedClass.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "GameFramework/WorldSettings.h"

#include "Misc/FileHelper.h"
#include "EdGraphUtilities.h"
#include "HAL/PlatformFilemanager.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Framework/Commands/GenericCommands.h"

#include "BXTLEditorToolbar.h"
#include "BXTLEditorCommands.h"
#include "BXTLEditorSettings.h"
#include "Graph/BXTLGraph.h"
#include "Graph/BXTLGraphNode.h"
#include "Graph/SBXTLGraphNode.h"
#include "Graph/BXTLGraphSchema.h"
#include "Graph/BXTLGraphTransitionNode.h"
#include "Graph/SBXTLGraphTransitionNode.h"
#include "Preview/BXTLPreviewProxy.h"
#include "Preview/BXTLPreviewScene.h"
#include "Preview/BXTLPreviewGameMode.h"
#include "Viewport/SBXTLEditorViewport.h"
#include "Viewport/BXTLEditorViewportClient.h"
#include "TabFactory/BXTLEditorMode.h"
#include "TabFactory/SBXTLTaskTab.h"

#include "BXManager.h"
#include "BXTLAsset.h"
#include "BXGameplayTags.h"



const FName BXTLEditorAppIdentifier = TEXT("BXTLEditor");

const FName BXTLEditorModes::Timeline(TEXT("Timeline"));

const FName BXTLEditorTabs::Graph(TEXT("Graph"));
const FName BXTLEditorTabs::Viewport(TEXT("Viewport"));
const FName BXTLEditorTabs::Timeline(TEXT("Timeline"));
const FName BXTLEditorTabs::TaskDetails(TEXT("TaskDetails"));
const FName BXTLEditorTabs::AssetDetails(TEXT("AssetDetails"));



#define LOCTEXT_NAMESPACE "BXTLEditor"

class FBXTLGraphNodeFactory : public FGraphPanelNodeFactory
{
	virtual TSharedPtr<SGraphNode> CreateNode(UEdGraphNode* Node) const override
	{
		if (UBXTLGraphNode* GraphNode = Cast<UBXTLGraphNode>(Node))
		{
			TSharedRef<SBXTLGraphNode> SNode = SNew(SBXTLGraphNode, GraphNode);
			GraphNode->NodeWidget = SNode;
			
			return SNode;
		}

		if (UBXTLGraphTransitionNode* GraphNode = Cast<UBXTLGraphTransitionNode>(Node))
		{
			return SNew(SBXTLGraphTransitionNode, GraphNode);
		}

		return nullptr;
	}
};
TSharedPtr<FGraphPanelNodeFactory> BXTLGraphNodeFactory;



TSharedPtr<IBXTLEditor> IBXTLEditor::OpenEditor(UObject* InAsset, const TSharedPtr<IToolkitHost>& InitToolkitHost)
{
	check(InAsset);

	TSharedPtr<FBXTLEditor> EditorPtr(new FBXTLEditor());

	EditorPtr->InitializeEditor(Cast<UBXTLAsset>(InAsset), InitToolkitHost);

	return EditorPtr;
}



#pragma region Important
int32 FBXTLEditor::BXTLEditorNum = 0;
int32 FBXTLEditor::BXTLEditorIndex = 0;

FBXTLEditor::FBXTLEditor()
{
	FBXTLEditor::BXTLEditorNum += 1;
	FBXTLEditor::BXTLEditorIndex += 1;

	// 注册节点工厂
	BXTLGraphNodeFactory = MakeShareable(new FBXTLGraphNodeFactory());
	FEdGraphUtilities::RegisterVisualNodeFactory(BXTLGraphNodeFactory);
}

FBXTLEditor::~FBXTLEditor()
{

}

void FBXTLEditor::InitializeEditor(UBXTLAsset* InAsset, const TSharedPtr<IToolkitHost>& InInitToolkitHost)
{
	// 缓存编辑的资源指针
	check(InAsset);
	EditAsset = InAsset;

	// 获取所有的Task信息
	CollectAllTaskClass();

	// 尝试创建逻辑图表
	if (!EditAsset->Graph)
	{
		EditAsset->Graph = CastChecked<UEdGraph>
		(
			FBlueprintEditorUtils::CreateNewGraph
			(
				EditAsset.Get(), TEXT("Graph"),
				UBXTLGraph::StaticClass(),
				UBXTLGraphSchema::StaticClass()
			)
		);
	}
	if (UBXTLGraph* BXTLGraph = Cast<UBXTLGraph>(EditAsset->Graph))
	{
		BXTLGraph->Init();
	}
		

	// 初始化UE的资源编辑器
	const bool bCreateDefaultStandaloneMenu = true;
	const bool bCreateDefaultToolbar = true;
	FAssetEditorToolkit::InitAssetEditor(EToolkitMode::Standalone, InInitToolkitHost, BXTLEditorAppIdentifier, FTabManager::FLayout::NullLayout, bCreateDefaultStandaloneMenu, bCreateDefaultToolbar, InAsset);

	// 绑定指令
	BindCommands();
	// 扩展工具栏
	ExtendToolbar();

	// 创建预览场景
	CreatePreviewScene();
	// 初始化预览场景
	InitPreviewContext();

	// 初始化资源
	EditAsset->InitByEditor(PreviewScene->GetWorld());
	EditSectionsToShow.Append(EditAsset->StartSectionIndexes);

	// 添加编辑器模式
	AddApplicationMode(BXTLEditorModes::Timeline, MakeShareable(new FBXTLEditorMode(BXTLEditorModes::Timeline, SharedThis(this))));
	// 使用时间轴模式
	SetCurrentMode(BXTLEditorModes::Timeline);
}

void FBXTLEditor::Tick(float DeltaTime)
{
	PreviewProxy->Tick(DeltaTime);

	// 对齐时间属性
	if (PreviewProxy->IsStopped())
	{
		int64 CurrentTS = UBXFunctionLibrary::GetUtcMillisecond();
		if (CurrentTS - AlignTimePropertyTS > 125 && EditAsset.IsValid())
		{
			AlignTimePropertyTS = CurrentTS;

			if (const UBXTLEditorSettings* Setting = GetDefault<UBXTLEditorSettings>())
			{
				if (UBXTLGraph* Graph = Cast<UBXTLGraph>(EditAsset->Graph))
				{
					Graph->AlignTimeProperty(Setting->PreviewTickRate);
				}

				EditAsset->AlignTimeProperty(Setting->PreviewTickRate);
			}
		}
	}

	// 检查是否要刷新时间轴
	bool bNeedRefreshPanel = false;
	if (PreviewProxy->IsStopped())
	{
		for (int32 i = 0; i < EditSectionsToShow.Num(); ++i)
		{
			if (!SectionsToShow.Contains(EditSectionsToShow[i]))
			{
				bNeedRefreshPanel = true;
				break;
			}
		}

		if (!bNeedRefreshPanel)
		{
			for (int32 i = 0; i < SectionsToShow.Num(); ++i)
			{
				if (!EditSectionsToShow.Contains(SectionsToShow[i]))
				{
					bNeedRefreshPanel = true;
					break;
				}
			}
		}

		SectionsToShow.Reset();
		SectionsToShow.Append(EditSectionsToShow);
	}
	else
	{
		PreviewSectionsToShow.Reset();
		PreviewProxy->GetRunningSectionIndexes(PreviewSectionsToShow);

		for (int32 i = 0; i < PreviewSectionsToShow.Num(); ++i)
		{
			if (!SectionsToShow.Contains(PreviewSectionsToShow[i]))
			{
				bNeedRefreshPanel = true;
				break;
			}
		}

		if (!bNeedRefreshPanel)
		{
			for (int32 i = 0; i < SectionsToShow.Num(); ++i)
			{
				if (!PreviewSectionsToShow.Contains(SectionsToShow[i]))
				{
					bNeedRefreshPanel = true;
					break;
				}
			}
		}

		SectionsToShow.Reset();
		SectionsToShow.Append(PreviewSectionsToShow);
	}

	if (bNeedRefreshPanel)
	{
		RefreshPanelEvent.Broadcast();
	}
}

void FBXTLEditor::OnClose()
{
	if (PreviewProxy.IsValid())
	{
		PreviewProxy->Finish();
	}

	if (PreviewScene.IsValid())
	{
		PreviewScene->Finish();
	}

	if (CachedManager.IsValid())
	{
		CachedManager->Deinitialize();
		CachedManager->RemoveFromRoot();
		CachedManager->MarkAsGarbage();
		CachedManager = nullptr;
	}

	if (UBXTLGraph* BXTLGraph = Cast<UBXTLGraph>(EditAsset->Graph))
	{
		BXTLGraph->Uninit();
	}
	
	FBXTLEditor::BXTLEditorNum -= 1;

	FWorkflowCentricApplication::OnClose();
}

UBXTLAsset* FBXTLEditor::GetEditingAsset()
{
	return Cast<UBXTLAsset>(GetEditingObject());
}

void FBXTLEditor::SaveAsset_Execute()
{
	if (AutoOptimize() <= 5)
	{
		FAssetEditorToolkit::SaveAsset_Execute();
	}
}

void FBXTLEditor::AddReferencedObjects(FReferenceCollector& Collector)
{
	Collector.AddReferencedObject(EditAsset);
}

FString FBXTLEditor::GetReferencerName() const
{
	return TEXT("BattleXTimelineEditor");
}

#pragma endregion Important



#pragma region Editor
void FBXTLEditor::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(LOCTEXT("BXTLEditorMenu", "BXTLEditor"));

	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);
}

void FBXTLEditor::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);
}
FText FBXTLEditor::GetBaseToolkitName() const
{
	return LOCTEXT("AppLabel", "BXTLEditor");
}

FName FBXTLEditor::GetToolkitFName() const
{
	return FName("BXTLEditor");
}

FString FBXTLEditor::GetWorldCentricTabPrefix() const
{
	return LOCTEXT("WorldCentricTabPrefix", "BattleXEditor").ToString();
}

FLinearColor FBXTLEditor::GetWorldCentricTabColorScale() const
{
	return FLinearColor(0.3f, 0.2f, 0.5f, 0.5f);
}

void FBXTLEditor::SetViewport(TSharedPtr<SBXTLEditorViewport> InViewport)
{ 
	Viewport = InViewport; 
}

TSharedPtr<SBXTLEditorViewport> FBXTLEditor::GetViewportWidget()
{ 
	return Viewport; 
}

void FBXTLEditor::SetDetailWidget(TSharedPtr<SBXTLTaskDetailTab> InDetailWidget)
{
	TaskDetailWidget = InDetailWidget;
}

void FBXTLEditor::ExtendToolbar()
{
	// 创建工具栏
	if (!EditorToolbar.IsValid())
	{
		EditorToolbar = MakeShareable(new FBXTLEditorToolbar());
	}

	if (ToolbarExtender.IsValid())
	{
		RemoveToolbarExtender(ToolbarExtender);
		ToolbarExtender.Reset();
	}

	ToolbarExtender = MakeShareable(new FExtender);

	EditorToolbar->SetupToolbar(ToolbarExtender, SharedThis(this));

	//EditorToolbar->AddModesToolbar(ToolbarExtender);

	EditorToolbar->AddTimelineToolbar(ToolbarExtender);

	AddToolbarExtender(ToolbarExtender);

}

void FBXTLEditor::ShowObjectDetail(UObject* InObject)
{
	if (TaskDetailWidget.IsValid())
	{
		TaskDetailWidget->SetDetailObject(InObject);
	}
}

FText FBXTLEditor::GetLocalizedMode(FName InMode)
{
	static TMap<FName, FText> LocModes;

	if (LocModes.Num() == 0)
	{
		LocModes.Add(BXTLEditorModes::Timeline, LOCTEXT("EditorMode", "Timeline"));
	}

	if (const FText* OutDesc = LocModes.Find(InMode))
	{
		return *OutDesc;
	}

	return LOCTEXT("EditorMode", "Unknown");
}

void FBXTLEditor::GetSectionsToShow(TArray<int32>& OutSectionsToShow) const
{
	OutSectionsToShow.Reset();
	OutSectionsToShow.Append(SectionsToShow);
}

void FBXTLEditor::SetEditSectionsToShow(int32 InIndex, bool bShow)
{
	if (bShow)
	{
		EditSectionsToShow.AddUnique(InIndex);
	}
	else
	{
		EditSectionsToShow.RemoveSingleSwap(InIndex);
	}
}

void FBXTLEditor::GetEditSectionsToShow(TArray<int32>& OutSectionsToShow) const
{
	OutSectionsToShow.Reset();
	OutSectionsToShow.Append(EditSectionsToShow);
}

#pragma endregion Editor



#pragma region Command
void FBXTLEditor::BindCommands()
{
	// 注册指令
	FBXTLEditorCommands::Register();

	const FBXTLEditorCommands& Commands = FBXTLEditorCommands::Get();
	const TSharedRef<FUICommandList>& UICommandsList = GetToolkitCommands();

	UICommandsList->MapAction(Commands.Play, FExecuteAction::CreateSP(this, &FBXTLEditor::Play));
	UICommandsList->MapAction(Commands.Stop, FExecuteAction::CreateSP(this, &FBXTLEditor::Stop));
	UICommandsList->MapAction(Commands.Step, FExecuteAction::CreateSP(this, &FBXTLEditor::Step));

	UICommandsList->MapAction(Commands.ResetWorld, FExecuteAction::CreateSP(this, &FBXTLEditor::ResetWorld));
	UICommandsList->MapAction(Commands.ShowCollision, FExecuteAction::CreateSP(this, &FBXTLEditor::ShowCollision));

	UICommandsList->MapAction(FGenericCommands::Get().Delete, FExecuteAction::CreateRaw(this, &FBXTLEditor::OnSelectedNodesDeleted));
}

void FBXTLEditor::Play()
{
	if (!PreviewScene.IsValid())
	{
		return;
	}

	if (PreviewProxy->IsPlaying())
	{
		PreviewProxy->Pause();
	}
	else if (PreviewProxy->IsPaused())
	{
		PreviewProxy->Resume();
	}
	else
	{
		PreviewProxy->Play();
	}
}

bool FBXTLEditor::IsPlaying() const
{
	return PreviewProxy->IsPlaying();
}

bool FBXTLEditor::IsPaused() const
{
	return PreviewProxy->IsPaused();
}

bool FBXTLEditor::IsStopped() const
{
	return PreviewProxy->IsStopped();
}

void FBXTLEditor::Pause()
{
	if (PreviewProxy->IsPaused())
	{
		PreviewProxy->Resume();
	}
	else
	{
		PreviewProxy->Pause();
	}
}

void FBXTLEditor::Stop()
{
	if (!PreviewProxy->IsStopped())
		PreviewProxy->Stop();
}

void FBXTLEditor::Step()
{
	if (PreviewProxy->IsStopped())
	{
		PreviewProxy->Play();
		PreviewProxy->Pause();
	}
	else if (PreviewProxy->IsPlaying())
	{
		PreviewProxy->Pause();
	}

	TSharedPtr<FBXTLEditorViewportClient> ViewportClient = StaticCastSharedPtr<FBXTLEditorViewportClient>(Viewport.Get()->GetViewportClient());
	if (ViewportClient.IsValid())
	{
		if (const UBXTLEditorSettings* Setting = GetDefault<UBXTLEditorSettings>())
		{
			ViewportClient->TickWorld(1.0f / FMath::Max(Setting->PreviewTickRate, 0.1f));
		}
		else
		{
			ViewportClient->TickWorld(1.0f / 60.0f);
		}
	}
}

void FBXTLEditor::ResetWorld()
{
	ResetPreviewContext();
}

void FBXTLEditor::ShowCollision()
{
	bShowCollision = !bShowCollision;

	if (CachedManager.IsValid())
	{
		CachedManager->ChangeShowCollision(bShowCollision);
	}

	TSharedPtr<FBXTLEditorViewportClient> ViewportClient = StaticCastSharedPtr<FBXTLEditorViewportClient>(Viewport.Get()->GetViewportClient());
	if (ViewportClient.IsValid())
	{
		ViewportClient->SetEngineShowFlagCollision(bShowCollision);
	}
}

void FBXTLEditor::RefreshTimelineAssetProperty()
{
	if (!CachedManager.IsValid())
	{
		return;
	}

	bool Flag = false;
	TArray<int32> IDs = CachedManager->GetTimelineAssetIDs();
	for (int32 i = 0; i < IDs.Num(); ++i)
	{
		TSoftObjectPtr<UBXTLAsset> AssetPath = CachedManager->GetTimelineAssetByID(IDs[i]);
		if (AssetPath.IsNull())
		{
			continue;
		}

		UBXTLAsset* Asset = AssetPath.LoadSynchronous();
		if (!IsValid(Asset))
		{
			continue;
		}

		Flag = false;

		for (int32 j = 0; j < Asset->Sections.Num(); ++j)
		{
			FBXTLSection& Section = Asset->Sections[j];

			for (int32 k = 0; k < Section.TaskList.Num(); ++k)
			{
				UBXTask* Task = Section.TaskList[k];
				if (!IsValid(Task))
				{
					continue;
				}

				Flag |= Task->RefreshProperty();
				Flag |= Task->ScriptRefreshProperty();
			}
		}

		if (Flag)
		{
			Asset->MarkPackageDirty();
		}
	}
}

int32 FBXTLEditor::AutoOptimize()
{
	if (EditAsset.IsValid())
	{
		// 手动执行预保存
		FObjectSaveContextData OSCD;
		UE::SavePackageUtilities::CallPreSave(EditAsset.Get(), OSCD);

		// TODO

		return 0;
	}

	return 0;
}

#pragma endregion Command



#pragma region Preview
TSharedPtr<FBXTLPreviewScene> FBXTLEditor::GetPreviewScene() const
{ 
	return PreviewScene; 
}

TSharedPtr<FBXTLPreviewProxy> FBXTLEditor::GetPreviewProxy() const
{ 
	return PreviewProxy; 
}

FGameplayTag FBXTLEditor::GetLockedBodyPartType()
{
	if (!EditAsset.IsValid())
	{
		return BXGameplayTags::BXBodyPart_Default;
	}

	if (UBXTLPreviewActor* PreviewTargetInformation = Cast<UBXTLPreviewActor>(EditAsset->TargetInformation))
	{
		return PreviewTargetInformation->LockedBodyPart;
	}

	return BXGameplayTags::BXBodyPart_Default;
}

bool FBXTLEditor::ShouldPauseWorld() const
{
	if (PreviewProxy->IsPaused())
	{
		return true;
	}

	return false;
}

void FBXTLEditor::CreatePreviewScene()
{
	if (!PreviewScene.IsValid())
	{
		const UBXTLEditorSettings* EditorSetting = GetDefault<UBXTLEditorSettings>();

		FPreviewScene::ConstructionValues CVS;
		CVS.bEditor = true;
		CVS.bAllowAudioPlayback = true;
		CVS.bShouldSimulatePhysics = true;
		if (EditorSetting && EditorSetting->GameModeClass.Get())
		{
			CVS.DefaultGameMode = EditorSetting->GameModeClass.Get();
		}
		else
		{
			CVS.DefaultGameMode = ABXTLPreviewGameMode::StaticClass();
		}

		PreviewScene = MakeShareable(new FBXTLPreviewScene(CVS, SharedThis(this)));
		UWorld* PreviewWorld = PreviewScene->GetWorld();

		// 对世界重命名
		FString PreviewWorldName = TEXT("BXTLEW") + FString::FromInt(FBXTLEditor::BXTLEditorIndex);
		PreviewWorld->Rename(*PreviewWorldName, PreviewWorld->GetOuter());
	}
}

void FBXTLEditor::CreatePreviewProxy()
{
	PreviewProxy = MakeShareable(new FBXTLPreviewProxy(EditAsset.Get(), SharedThis(this)));
}

void FBXTLEditor::InitPreviewContext()
{
	check(PreviewScene.IsValid());

	PreviewScene->InitPreviewWorld();

	CreatePreviewProxy();
}

void FBXTLEditor::ResetPreviewContext()
{
	check(PreviewScene.IsValid());

	PreviewProxy->ResetWorld();

	PreviewScene->ResetPreviewWorld();
}

#pragma endregion Preview



#pragma region Core
UBXManager* FBXTLEditor::GetBXManager()
{ 
	if (!CachedManager.IsValid())
	{
		CachedManager = NewObject<UBXManager>(PreviewScene.IsValid() ? PreviewScene->GetWorld() : nullptr);
		CachedManager->Initialize();
		CachedManager->AddToRoot();
	}

	return CachedManager.Get();
}

void FBXTLEditor::CollectAllTaskClass()
{
	// 筛选出Task蓝图资源
	auto CheckClass = [&](FAssetData& AssetMsg)
	{
		FString ClassPath = AssetMsg.GetSoftObjectPath().ToString();
		if (!ClassPath.Contains(TEXT("BP_BXT_")))
		{
			return;
		}

		UBlueprint* CurBP = LoadObject<UBlueprint>(nullptr, *ClassPath);
		UClass* CurClass = CurBP->GeneratedClass;

		if (!CurClass)
		{
			return;
		}

		if (UBXTask* Task = Cast<UBXTask>(CurClass->GetDefaultObject()))
		{
			TaskClassList.AddUnique(Task->GetClass());
			return;
		}
		return;
	};

	FARFilter Filter;
	Filter.bRecursivePaths = true;
	Filter.bRecursiveClasses = true;
	Filter.ClassPaths.AddUnique(UBlueprint::StaticClass()->GetClassPathName());
	Filter.ClassPaths.AddUnique(UBlueprintGeneratedClass::StaticClass()->GetClassPathName());

	TArray<FAssetData> BlueprintList;
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
	AssetRegistryModule.Get().GetAssets(Filter, BlueprintList);
	for (int32 i = 0; i < BlueprintList.Num(); ++i)
	{
		CheckClass(BlueprintList[i]);
	}
}

TArray<UBXTask*> FBXTLEditor::GetTaskSelection()
{
	TArray<UBXTask*> Result;
	for (int32 i = 0; i < TaskSelection.TaskList.Num(); ++i)
	{
		Result.Add(TaskSelection.TaskList[i].Get());
	}

	return Result;
}

void FBXTLEditor::SetTaskSelection(TArray<UBXTask*> InSelection, bool bIsOverride)
{
	if (IsPlaying())
	{
		TaskSelection.TaskList.Empty();
		return;
	}

	if (!EditAsset.IsValid())
	{
		TaskSelection.TaskList.Empty();
		return;
	}

	if (bIsOverride)
	{
		TaskSelection.TaskList.Empty();
	}

	if (InSelection.Num() > 0)
	{
		if (TaskSelection.TaskList.Num() > 0)
		{
			EditAsset->GetSectionIDAndGroupID(TaskSelection.TaskList[0].Get(), TaskSelection.SectionID, TaskSelection.GroupID);
		}
		else
		{
			EditAsset->GetSectionIDAndGroupID(InSelection[0], TaskSelection.SectionID, TaskSelection.GroupID);
		}

		int32 SectionID = -1, GroupID = -1;
		for (int32 i = 0; i < InSelection.Num(); ++i)
		{
			EditAsset->GetSectionIDAndGroupID(InSelection[i], SectionID, GroupID);

			if (SectionID == TaskSelection.SectionID)
			{
				TaskSelection.TaskList.AddUnique(InSelection[i]);
			}
		}
	}

	TArray<UBXTask*> CurTaskList = GetTaskSelection();
	if (CurTaskList.Num() > 0)
	{
		ShowObjectDetail(CurTaskList[0]);
	}
	else
	{
		ShowObjectDetail(nullptr);
	}

	if (CurTaskList.Num() > 0)
	{
		// 清理图表的选中信息
		if (GraphEditor.IsValid())
		{
			GraphEditor->ClearSelectionSet();
		}

		// 图表自动跳转到对应位置
		if (CurTaskList.Num() == 1)
		{
			SetGraphEditorViewLocationByTask(CurTaskList[0]);
		}
	}

	// 广播事件
	TaskSelectedEvent.Broadcast(CurTaskList);
}

#pragma endregion Core



#pragma region Graph
void FBXTLEditor::SetGraphEditor(const TSharedPtr<SGraphEditor>& InGraphEditor)
{
	GraphEditor = InGraphEditor;
}

void FBXTLEditor::SetGraphEditorViewLocationByTask(UBXTask* InTask)
{
	if (!InTask || !GraphEditor.IsValid())
	{
		return;
	}

	if (UBXTLGraph* Graph = Cast<UBXTLGraph>(EditAsset->Graph))
	{
		if (UEdGraphNode* Node = Graph->GetGraphNodeByTask(InTask))
		{
			GraphEditor->SetViewLocation(FVector2D(Node->NodePosX - 100.0f, Node->NodePosY - 100.0f), 1.0f);
		}
	}
}

void FBXTLEditor::OnSelectedNodesChanged(const TSet<UObject*>& NewSelection)
{
	SelectedGraphNodes.Empty();
	for (UObject* SelectionEntry : NewSelection)
	{
		SelectedGraphNodes.Add(SelectionEntry);
	}

	if (SelectedGraphNodes.Num() > 0)
	{
		SetTaskSelection({}, true);

		if (UBXTLGraphNode* Node = Cast<UBXTLGraphNode>(SelectedGraphNodes[0]))
		{
			ShowObjectDetail(Node->CachedTask);
		}
	}
}

void FBXTLEditor::OnSelectedNodesDeleted()
{
	if (UBXTLGraph* Graph = Cast<UBXTLGraph>(EditAsset->Graph))
	{
		TArray<UEdGraphNode*> CurNodes;
		for (TArray<TWeakObjectPtr<UObject>>::TIterator It(SelectedGraphNodes); It; ++It)
		{
			if (UEdGraphNode* Node = Cast<UEdGraphNode>(*It))
			{
				CurNodes.AddUnique(Node);
			}
		}

		Graph->DeleteGraphNodes(CurNodes);
	}

	if (GraphEditor.IsValid())
	{
		GraphEditor->ClearSelectionSet();
	}
}

void FBXTLEditor::GenerateGraphNodes(TArray<UBXTask*> InTaskList)
{
	if (InTaskList.Num() <= 0)
	{
		return;
	}

	if (UBXTLGraph* Graph = Cast<UBXTLGraph>(EditAsset->Graph))
	{
		Graph->GenerateGraphNodesByTasks(InTaskList);
	}
}

#pragma endregion Graph

#undef LOCTEXT_NAMESPACE
