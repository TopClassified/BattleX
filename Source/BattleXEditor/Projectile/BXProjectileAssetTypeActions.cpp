#include "BXProjectileAssetTypeActions.h"

#include "BXProjectileAsset.h"



#define LOCTEXT_NAMESPACE "AssetTypeActions_BXProjectileAsset"

FAssetTypeActions_BXProjectileAsset::FAssetTypeActions_BXProjectileAsset(EAssetTypeCategories::Type InAssetCategory) : MyAssetCategory(InAssetCategory)
{

}

FText FAssetTypeActions_BXProjectileAsset::GetName() const
{
	return LOCTEXT("FAssetTypeActions_BXProjectileAsset", "Projectile");
}

FColor FAssetTypeActions_BXProjectileAsset::GetTypeColor() const
{
	return FColor::Orange;
}

UClass* FAssetTypeActions_BXProjectileAsset::GetSupportedClass() const
{
	return UBXProjectileAsset::StaticClass();
}

uint32 FAssetTypeActions_BXProjectileAsset::GetCategories()
{
	return MyAssetCategory;
}

#undef LOCTEXT_NAMESPACE
