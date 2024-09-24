#include "BXTLAssetTypeActions.h"

#include "EditorModeManager.h"
#include "Misc/MessageDialog.h"

#include "BXTLEditor.h"

#include "BXTLAsset.h"
#include "BXSkillAsset.h" 


#define LOCTEXT_NAMESPACE "AssetTypeActions_BXTLAsset"

FAssetTypeActions_BXTLAsset::FAssetTypeActions_BXTLAsset(EAssetTypeCategories::Type InAssetCategory) : MyAssetCategory(InAssetCategory)
{

}

FAssetTypeActions_BXTLAsset::~FAssetTypeActions_BXTLAsset()
{

}

FText FAssetTypeActions_BXTLAsset::GetName() const
{
	return LOCTEXT("FAssetTypeActions_BXTLAsset", "Timeline");
}

FColor FAssetTypeActions_BXTLAsset::GetTypeColor() const
{
	return FColor::Green;
}

UClass* FAssetTypeActions_BXTLAsset::GetSupportedClass() const
{
	return UBXTLAsset::StaticClass();
}

void FAssetTypeActions_BXTLAsset::GetActions(const TArray<UObject*>& InObjects, FMenuBuilder& MenuBuilder)
{

}

void FAssetTypeActions_BXTLAsset::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor)
{
	for (UObject* Object : InObjects)
	{
		if (UBXTLAsset* Asset = Cast<UBXTLAsset>(Object))
		{
			IBXTLEditor::OpenEditor(Asset, EditWithinLevelEditor);
		}
	}
}

uint32 FAssetTypeActions_BXTLAsset::GetCategories()
{
	MyAssetCategory = EAssetTypeCategories::Type::Gameplay;

	return MyAssetCategory;
}






FAssetTypeActions_BXSkillAsset::FAssetTypeActions_BXSkillAsset(EAssetTypeCategories::Type InAssetCategory) : MyAssetCategory(InAssetCategory)
{

}

FAssetTypeActions_BXSkillAsset::~FAssetTypeActions_BXSkillAsset()
{

}

FText FAssetTypeActions_BXSkillAsset::GetName() const
{
	return LOCTEXT("FAssetTypeActions_BXSkillAsset", "Skill");
}

FColor FAssetTypeActions_BXSkillAsset::GetTypeColor() const
{
	return FColor::Cyan;
}

UClass* FAssetTypeActions_BXSkillAsset::GetSupportedClass() const
{
	return UBXSkillAsset::StaticClass();
}

void FAssetTypeActions_BXSkillAsset::GetActions(const TArray<UObject*>& InObjects, FMenuBuilder& MenuBuilder)
{

}

void FAssetTypeActions_BXSkillAsset::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor)
{
	for (UObject* Object : InObjects)
	{
		if (UBXSkillAsset* SkillAsset = Cast<UBXSkillAsset>(Object))
		{
			IBXTLEditor::OpenEditor(SkillAsset, EditWithinLevelEditor);
		}
	}
}

uint32 FAssetTypeActions_BXSkillAsset::GetCategories()
{
	MyAssetCategory = EAssetTypeCategories::Type::Gameplay;

	return MyAssetCategory;
}

#undef LOCTEXT_NAMESPACE