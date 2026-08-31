#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"
#include "GameplayTagContainer.h"
#include "Internationalization/Text.h"

class IDetailLayoutBuilder;
class UBXBehaviorSettings;


// UBXBehaviorSettings的Detail定制:行为关系渲染为矩阵网格
// 轴经"+添加矩阵轴"(GameplayTag选择器)添加,点击表头轴名删除;单元格四态循环:空→接管→禁止→禁止+接管
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

	// 添加矩阵轴(弹出GameplayTag选择器)
	FReply OnAddAxisClicked();

	// 添加轴的Tag选择回调(TagPicker为窗口模态回调,存待选轴索引)
	void OnAxisTagSelected(const FGameplayTag& InTag);

	// 删除矩阵轴(连带清除该轴的全部关系配置)
	FReply OnRemoveAxisClicked(int32 InAxisIndex);

	// 单元格按钮回调(循环切换关系:空→接管→禁止→禁止+接管)
	FReply OnCellClicked(int32 InRowIndex, int32 InColumnIndex);

	// 获取单元格当前关系
	uint8 GetCellRelation(int32 InRowIndex, int32 InColumnIndex) const;

	// 单元格文本
	FText GetCellText(int32 InRowIndex, int32 InColumnIndex) const;

	// 是否对角线
	bool IsDiagonal(int32 InRowIndex, int32 InColumnIndex) const;

private:
	TWeakObjectPtr<UBXBehaviorSettings> CachedSettings;

	// Details视图引用(矩阵结构变化后ForceRefreshDetails重建整个定制;
	// 定制实例由视图持有并先于视图析构,裸指针与引擎内同类定制一致)
	IDetailLayoutBuilder* CachedDetailBuilder = nullptr;

	// 变更收尾:保存配置+重建索引+刷新视图
	void CommitAndRefresh(UBXBehaviorSettings* InSettings);
};
