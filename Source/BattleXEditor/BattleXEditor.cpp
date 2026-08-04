#include "BattleXEditor.h"

#include "UnrealEdGlobals.h"
#include "Editor/UnrealEdEngine.h"
#include "AssetToolsModule.h"
#include "PropertyEditorModule.h"
#include "BXDecisionTreeTemplate.h"

#include "BXTLAsset.h"
#include "BXSkillAsset.h"
#include "BXShapeComponent.h"
#include "BXStructs.h"

#include "BXTLAssetTypeActions.h"
#include "BXTLAssetThumbnailRenderer.h"
#include "BXDTAssetTypeActions.h"
#include "BXDTAssetThumbnailRenderers.h"
#include "ComponentVisualizers/BXShapeComponentVisualizer.h"
#include "CustomLayout/BXBoneSelectorCustomization.h"
#include "CustomLayout/BXFunctionSelectorCustomization.h"



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

	// 注册属性定制
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.RegisterCustomPropertyTypeLayout(FBXBoneSelector::StaticStruct()->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FBXBoneSelectorCustomization::MakeInstance));
	PropertyModule.RegisterCustomPropertyTypeLayout(FBXFunctionSelector::StaticStruct()->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FBXFunctionSelectorCustomization::MakeInstance));
}

void FBattleXEditorModule::ShutdownModule()
{
	if (GUnrealEd)
	{
		GUnrealEd->UnregisterComponentVisualizer(UBXShapeComponent::StaticClass()->GetFName());
	}

	// 注销属性定制
	if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
		PropertyModule.UnregisterCustomPropertyTypeLayout(FBXBoneSelector::StaticStruct()->GetFName());
		PropertyModule.UnregisterCustomPropertyTypeLayout(FBXFunctionSelector::StaticStruct()->GetFName());
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FBattleXEditorModule, BattleX)