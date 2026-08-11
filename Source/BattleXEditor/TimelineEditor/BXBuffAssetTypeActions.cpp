#include "BXBuffAssetTypeActions.h"

#include "EditorModeManager.h"
#include "Misc/MessageDialog.h"

#include "BXBuffEditor.h"

#include "BXBuffAsset.h"


#define LOCTEXT_NAMESPACE "AssetTypeActions_BXBuffAsset"

FAssetTypeActions_BXBuffAsset::FAssetTypeActions_BXBuffAsset(EAssetTypeCategories::Type InAssetCategory) : MyAssetCategory(InAssetCategory)
{

}

FAssetTypeActions_BXBuffAsset::~FAssetTypeActions_BXBuffAsset()
{

}

FText FAssetTypeActions_BXBuffAsset::GetName() const
{
	return LOCTEXT("FAssetTypeActions_BXBuffAsset", "Buff");
}

FColor FAssetTypeActions_BXBuffAsset::GetTypeColor() const
{
	return FColor::Magenta;
}

UClass* FAssetTypeActions_BXBuffAsset::GetSupportedClass() const
{
	return UBXBuffAsset::StaticClass();
}

void FAssetTypeActions_BXBuffAsset::GetActions(const TArray<UObject*>& InObjects, FMenuBuilder& MenuBuilder)
{

}

void FAssetTypeActions_BXBuffAsset::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor)
{
	for (UObject* Object : InObjects)
	{
		if (UBXBuffAsset* BuffAsset = Cast<UBXBuffAsset>(Object))
		{
			TSharedPtr<FBXBuffEditor> EditorPtr(new FBXBuffEditor());
			EditorPtr->InitializeEditor(BuffAsset, EditWithinLevelEditor);
		}
	}
}

uint32 FAssetTypeActions_BXBuffAsset::GetCategories()
{
	MyAssetCategory = EAssetTypeCategories::Type::Gameplay;

	return MyAssetCategory;
}

#undef LOCTEXT_NAMESPACE
