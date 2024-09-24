#pragma once

#include "Toolkits/IToolkitHost.h"
#include "AssetTypeActions_Base.h" 



class FAssetTypeActions_BXTLAsset : public FAssetTypeActions_Base
{
public:
	FAssetTypeActions_BXTLAsset(EAssetTypeCategories::Type InAssetCategory);

	~FAssetTypeActions_BXTLAsset();

	virtual FText GetName() const override;

	virtual FColor GetTypeColor() const override;

	virtual UClass* GetSupportedClass() const override;

	virtual bool HasActions(const TArray<UObject*>& InObjects) const override { return true; }

	virtual void GetActions(const TArray<UObject*>& InObjects, FMenuBuilder& MenuBuilder) override;

	virtual void OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor = TSharedPtr<IToolkitHost>()) override;

	virtual uint32 GetCategories() override;

private:
	EAssetTypeCategories::Type MyAssetCategory;

};






class FAssetTypeActions_BXSkillAsset : public FAssetTypeActions_Base
{
public:
	FAssetTypeActions_BXSkillAsset(EAssetTypeCategories::Type InAssetCategory);

	~FAssetTypeActions_BXSkillAsset();

	virtual FText GetName() const override;

	virtual FColor GetTypeColor() const override;

	virtual UClass* GetSupportedClass() const override;

	virtual bool HasActions(const TArray<UObject*>& InObjects) const override { return true; }

	virtual void GetActions(const TArray<UObject*>& InObjects, FMenuBuilder& MenuBuilder) override;

	virtual void OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor = TSharedPtr<IToolkitHost>()) override;

	virtual uint32 GetCategories() override;

private:
	EAssetTypeCategories::Type MyAssetCategory;

};
