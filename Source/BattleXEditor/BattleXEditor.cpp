#include "BattleXEditor.h"

#include "UnrealEdGlobals.h"
#include "Editor/UnrealEdEngine.h"
#include "AssetToolsModule.h"
#include "BXDecisionTreeTemplate.h"

#include "BXTLAsset.h"
#include "BXSkillAsset.h"
#include "BXShapeComponent.h"

#include "BXTLAssetTypeActions.h"
#include "BXTLAssetThumbnailRenderer.h"
#include "BXDTAssetTypeActions.h"
#include "BXDTAssetThumbnailRenderers.h"
#include "ComponentVisualizers/BXShapeComponentVisualizer.h" 



#define LOCTEXT_NAMESPACE "FBattleXEditorModule"

void FBattleXEditorModule::StartupModule()
{
	if (GUnrealEd)
	{
		TSharedPtr<FBXShapeComponentVisualizer> Visualizer = MakeShareable(new FBXShapeComponentVisualizer);
		GUnrealEd->RegisterComponentVisualizer(UBXShapeComponent::StaticClass()->GetFName(), Visualizer);
		Visualizer->OnRegister();
	}

	IAssetTools& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	EAssetTypeCategories::Type CurrentAssetCategory = EAssetTypeCategories::Type::Gameplay;
	// 注册时间轴资源
	AssetToolsModule.RegisterAssetTypeActions(MakeShareable(new FAssetTypeActions_BXTLAsset(CurrentAssetCategory)));
	// 注册技能资源
	AssetToolsModule.RegisterAssetTypeActions(MakeShareable(new FAssetTypeActions_BXSkillAsset(CurrentAssetCategory)));
	// 注册决策树资源
	AssetToolsModule.RegisterAssetTypeActions(MakeShareable(new FBXDecisionTreeAssetTypeActions(CurrentAssetCategory)));
	// 注册受击树资源
	AssetToolsModule.RegisterAssetTypeActions(MakeShareable(new FBXBeatenTreeAssetTypeActions(CurrentAssetCategory)));
	// 注册出招表资源
	AssetToolsModule.RegisterAssetTypeActions(MakeShareable(new FBXCombatTreeAssetTypeActions(CurrentAssetCategory)));

	// 自定义缩略图
	UThumbnailManager::Get().UnregisterCustomRenderer(UBXTLAsset::StaticClass());
	UThumbnailManager::Get().RegisterCustomRenderer(UBXTLAsset::StaticClass(), UBXTLAssetThumbnailRenderer::StaticClass());
	UThumbnailManager::Get().UnregisterCustomRenderer(UBXDecisionTreeTemplate::StaticClass());
	UThumbnailManager::Get().RegisterCustomRenderer(UBXDecisionTreeTemplate::StaticClass(), UBXDTAssetThumbnailRenderers::StaticClass());
}

void FBattleXEditorModule::ShutdownModule()
{
	if (GUnrealEd)
	{
		GUnrealEd->UnregisterComponentVisualizer(UBXShapeComponent::StaticClass()->GetFName());
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FBattleXEditorModule, BattleX)