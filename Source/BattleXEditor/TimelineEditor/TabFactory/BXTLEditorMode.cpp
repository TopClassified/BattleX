#include "BXTLEditorMode.h"

#include "BXTLAsset.h"

#include "BXTLEditor.h"
#include "SBXTLAssetTab.h"
#include "SBXTLTaskTab.h"
#include "SBXTLEditorViewportTab.h" 



#define DECLARE_TAB_SUMMONER_BEGIN(SummonerName, TabIdentify) \
struct SummonerName : public FWorkflowTabFactory \
{ \
public: \
	static TSharedRef<class FWorkflowTabFactory> Create(const TSharedRef<class FWorkflowCentricApplication>& InHostingApp) \
	{ \
		return MakeShareable(new SummonerName(InHostingApp)); \
	} \
public: \
	SummonerName(TSharedPtr<class FAssetEditorToolkit> InHostingApp) \
		: FWorkflowTabFactory(TabIdentify, InHostingApp) {}

#define DECLARE_TAB_SUMMONER_END };



// 创建预览窗口
DECLARE_TAB_SUMMONER_BEGIN(FEditorViewportSummoner, BXTLEditorTabs::Viewport)
TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	TSharedPtr<FBXTLEditor> BXTLEditor = StaticCastSharedPtr<FBXTLEditor>(HostingApp.Pin());
	return SNew(SBXTLEditorViewportTab, BXTLEditor.ToSharedRef(), BXTLEditor->GetPreviewScene().ToSharedRef(), 0);
}
DECLARE_TAB_SUMMONER_END



// 创建时间轴
DECLARE_TAB_SUMMONER_BEGIN(FAssetEditSummoner, BXTLEditorTabs::Timeline)
TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	TSharedPtr<FBXTLEditor> BXTLEditor = StaticCastSharedPtr<FBXTLEditor>(HostingApp.Pin());
	return SNew(SBXTLAssetTimelineTab, BXTLEditor);
}
DECLARE_TAB_SUMMONER_END



// 创建资源属性面板
DECLARE_TAB_SUMMONER_BEGIN(FAssetDetailSummoner, BXTLEditorTabs::AssetDetails)
TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	TSharedPtr<FBXTLEditor> BXTLEditor = StaticCastSharedPtr<FBXTLEditor>(HostingApp.Pin());
	return SNew(SBXTLAssetDetailTab, BXTLEditor.ToSharedRef());
}
DECLARE_TAB_SUMMONER_END



// 创建Task属性面板
DECLARE_TAB_SUMMONER_BEGIN(FDetailTabSummoner, BXTLEditorTabs::TaskDetails)
TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	TSharedPtr<FBXTLEditor> Editor = StaticCastSharedPtr<FBXTLEditor>(HostingApp.Pin());

	TSharedPtr<SBXTLTaskDetailTab> DetailsTab = SNew(SBXTLTaskDetailTab);
	Editor->SetDetailWidget(DetailsTab);

	return DetailsTab.ToSharedRef();
}
DECLARE_TAB_SUMMONER_END



// 图表编辑器
DECLARE_TAB_SUMMONER_BEGIN(FLogicGraphEditorSummoner, BXTLEditorTabs::Graph)
TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	TSharedPtr<FBXTLEditor> Editor = StaticCastSharedPtr<FBXTLEditor>(HostingApp.Pin());

	FGraphAppearanceInfo AppearanceInfo;
	AppearanceInfo.CornerText = FText::FromString(TEXT("Graph"));

	SGraphEditor::FGraphEditorEvents InEvents;
	InEvents.OnSelectionChanged = SGraphEditor::FOnSelectionChanged::CreateSP(Editor.Get(), &FBXTLEditor::OnSelectedNodesChanged);

	TSharedPtr<SGraphEditor> NewViewPort;
	SAssignNew(NewViewPort, SGraphEditor)
	.IsEditable(true)
	.Appearance(AppearanceInfo)
	.GraphToEdit(Editor->GetEditingAsset()->Graph)
	.GraphEvents(InEvents)
	.AutoExpandActionMenu(true)
	.ShowGraphStateOverlay(false);

	Editor->SetGraphEditor(NewViewPort);

	return NewViewPort.ToSharedRef();
}
DECLARE_TAB_SUMMONER_END



FBXTLEditorMode::FBXTLEditorMode(const FName& InModeName, TSharedRef<FBXTLEditor> InEditor) : FApplicationMode(InModeName), CachedEditor(InEditor)
{
	// 创建Mode所需的Tabs
	CreateModeTabs(CachedEditor.Pin().ToSharedRef(), TabFactories);

	// 基础布局
	TabLayout = FTabManager::NewLayout("BXTimeline")
	->AddArea
	(
		FTabManager::NewPrimaryArea()
		->SetOrientation(Orient_Vertical)
		// 上
		->Split
		(
			FTabManager::NewSplitter()
			->SetSizeCoefficient(1.f)
			->SetOrientation(Orient_Horizontal)
			// 左
			->Split
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.15f)
				->SetHideTabWell(true)
				->AddTab(BXTLEditorTabs::AssetDetails, ETabState::OpenedTab)
			)
			// 中
			->Split
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.7f)
				->SetHideTabWell(true)
				->AddTab(BXTLEditorTabs::Viewport, ETabState::OpenedTab)
			)
			// 右
			->Split
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.15f)
				->SetHideTabWell(true)
				->AddTab(BXTLEditorTabs::TaskDetails, ETabState::OpenedTab)
			)
		)
		// 下
		->Split
		(
			FTabManager::NewSplitter()
			->SetSizeCoefficient(0.6f)
			->SetOrientation(Orient_Horizontal)
			// 左
			->Split
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.3f)
				->SetHideTabWell(true)
				->AddTab(BXTLEditorTabs::Graph, ETabState::OpenedTab)
			)
			// 右
			->Split
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.7f)
				->SetHideTabWell(true)
				->AddTab(BXTLEditorTabs::Timeline, ETabState::OpenedTab)
			)
		)
	);
}

void FBXTLEditorMode::RegisterTabFactories(TSharedPtr<FTabManager> InTabManager)
{
	TSharedPtr<FWorkflowCentricApplication> HostingApp = CachedEditor.Pin();
	HostingApp->RegisterTabSpawners(InTabManager.ToSharedRef());
	HostingApp->PushTabFactories(TabFactories);

	FApplicationMode::RegisterTabFactories(InTabManager);
}

void FBXTLEditorMode::AddTabFactory(FCreateWorkflowTabFactory FactoryCreator)
{
	if (FactoryCreator.IsBound())
	{
		TabFactories.RegisterFactory(FactoryCreator.Execute(CachedEditor.Pin()));
	}
}

void FBXTLEditorMode::RemoveTabFactory(FName TabFactoryID)
{
	TabFactories.UnregisterFactory(TabFactoryID);
}

void FBXTLEditorMode::CreateModeTabs(const TSharedRef<FBXTLEditor> HostingAppPt, FWorkflowAllowedTabSet& OutTabFactories)
{
	OutTabFactories.RegisterFactory(FEditorViewportSummoner::Create(HostingAppPt));

	OutTabFactories.RegisterFactory(FAssetEditSummoner::Create(HostingAppPt));

	OutTabFactories.RegisterFactory(FAssetDetailSummoner::Create(HostingAppPt));

	OutTabFactories.RegisterFactory(FDetailTabSummoner::Create(HostingAppPt));

	OutTabFactories.RegisterFactory(FLogicGraphEditorSummoner::Create(HostingAppPt));
}
