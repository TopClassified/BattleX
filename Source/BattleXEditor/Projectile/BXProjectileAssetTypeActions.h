#pragma once

#include "Toolkits/IToolkitHost.h"
#include "AssetTypeActions_Base.h"



// 子弹资产入口(GamePlay分类创建菜单,双击打开默认Details编辑器)
class FAssetTypeActions_BXProjectileAsset : public FAssetTypeActions_Base
{
public:
	FAssetTypeActions_BXProjectileAsset(EAssetTypeCategories::Type InAssetCategory);

	// IAssetTypeActions interface
	virtual FText GetName() const override;
	virtual FColor GetTypeColor() const override;
	virtual UClass* GetSupportedClass() const override;
	virtual uint32 GetCategories() override;
	// End of IAssetTypeActions interface

private:
	EAssetTypeCategories::Type MyAssetCategory;

};
