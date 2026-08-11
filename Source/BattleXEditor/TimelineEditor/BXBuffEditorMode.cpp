#include "BXBuffEditorMode.h"

#include "BXTLEditor.h"
#include "BXBuffEditor.h"



FBXBuffEditorMode::FBXBuffEditorMode(const FName& InModeName, TSharedRef<FBXBuffEditor> InEditor)
	: FBXTLEditorMode(InModeName, InEditor)
{
	// 替换布局: 去掉Timeline窗口,Graph占满下方
	TabLayout = FTabManager::NewLayout("BXBuffEditor")
	->AddArea
	(
		FTabManager::NewPrimaryArea()
		->SetOrientation(Orient_Vertical)
		// 上
		->Split
		(
			FTabManager::NewSplitter()
			->SetSizeCoefficient(0.4f)
			->SetOrientation(Orient_Horizontal)
			// 左
			->Split
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.2f)
				->SetHideTabWell(true)
				->AddTab(BXTLEditorTabs::AssetDetails, ETabState::OpenedTab)
			)
			// 中
			->Split
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.6f)
				->SetHideTabWell(true)
				->AddTab(BXTLEditorTabs::Viewport, ETabState::OpenedTab)
			)
			// 右
			->Split
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.2f)
				->SetHideTabWell(true)
				->AddTab(BXTLEditorTabs::TaskDetails, ETabState::OpenedTab)
			)
		)
		// 下
		->Split
		(
			FTabManager::NewStack()
			->SetSizeCoefficient(0.6f)
			->SetHideTabWell(true)
			->AddTab(BXTLEditorTabs::Graph, ETabState::OpenedTab)
		)
	);
}
