#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"

class UBXBehaviorSettings;

// UBXBehaviorSettings的Detail定制:行为关系渲染为矩阵网格
// 单元格三态循环:并存(空)→挤出→拒绝;对角线禁用(重复启动语义由组件管线处理)
class FBXBehaviorSettingsCustomization : public IDetailCustomization
{
public:
	// 创建实例
	static TSharedRef<IDetailCustomization> MakeInstance();

	// 定制细节
	virtual void CustomizeDetails(IDetailLayoutBuilder& InDetailBuilder) override;

private:
	// 获取矩阵设置对象
	UBXBehaviorSettings* GetSettings() const;

	// 渲染矩阵网格
	TSharedRef<SWidget> MakeMatrixWidget();

	// 单元格按钮回调(循环切换关系:并存→挤出→拒绝)
	FReply OnCellClicked(int32 InRowIndex, int32 InColumnIndex);

	// 获取单元格当前关系
	uint8 GetCellRelation(int32 InRowIndex, int32 InColumnIndex) const;

	// 单元格文本
	FText GetCellText(int32 InRowIndex, int32 InColumnIndex) const;

	// 是否对角线
	bool IsDiagonal(int32 InRowIndex, int32 InColumnIndex) const;

private:
	TWeakObjectPtr<UBXBehaviorSettings> CachedSettings;
};
