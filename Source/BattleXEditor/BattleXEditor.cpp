#include "BattleXEditor.h"

#include "UnrealEdGlobals.h"
#include "Editor/UnrealEdEngine.h"
#include "AssetToolsModule.h"

#include "BXTLAsset.h"
#include "BXSkillAsset.h"
#include "BXShapeComponent.h"

#include "BXTLAssetTypeActions.h"
#include "BXTLAssetThumbnailRenderer.h"
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
	// ע��ʱ������Դ
	AssetToolsModule.RegisterAssetTypeActions(MakeShareable(new FAssetTypeActions_BXTLAsset(CurrentAssetCategory)));
	// ע�Ἴ����Դ
	AssetToolsModule.RegisterAssetTypeActions(MakeShareable(new FAssetTypeActions_BXSkillAsset(CurrentAssetCategory)));

	// �Զ�������ͼ
	UThumbnailManager::Get().UnregisterCustomRenderer(UBXTLAsset::StaticClass());
	UThumbnailManager::Get().RegisterCustomRenderer(UBXTLAsset::StaticClass(), UBXTLAssetThumbnailRenderer::StaticClass());
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