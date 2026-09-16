#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"

class IDetailLayoutBuilder;



// UBXSettings的Detail定制:BattleX页面注入行为/状态两个关系矩阵
// (两个关系设置类SupportsAutoRegistration=false无独立页面,矩阵控件直接操作其CDO,
// 变更经各自SaveToPluginConfig直写插件ini;引擎同名Section不同对象是替换不是合并——SettingsCategory.cpp
// AddSection,故合并只能走本定制注入路径,不可让两个关系类复用BattleX的SectionName)
class FBXSettingsCustomization : public IDetailCustomization
{
public:
	// 创建实例
	static TSharedRef<IDetailCustomization> MakeInstance();

	// 定制细节
	virtual void CustomizeDetails(IDetailLayoutBuilder& InDetailBuilder) override;
};
