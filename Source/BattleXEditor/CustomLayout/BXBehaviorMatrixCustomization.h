#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"
#include "GameplayTagContainer.h"
#include "Internationalization/Text.h"

class IDetailLayoutBuilder;
class UBXBehaviorSettings;
class STextBlock;


// UBXBehaviorSettings的Detail定制:行为关系渲染为矩阵网格
// 轴经"+添加矩阵轴"(GameplayTag选择器)添加,点击表头轴名删除;
// 单元格四态循环:空→禁用→中断→禁用并中断(含对角线自关系:自禁用挡同Tag重入,自中断=新实例顶掉旧实例);
// 任何变更(单元格/增删轴)都不走ForceRefreshDetails(整视图重建是设置页卡顿根源):
// 单元格点击直改单元格文本,增删轴经SBox容器SetContent只换网格本体
class FBXBehaviorSettingsCustomization : public IDetailCustomization
{
public:
	// 创建实例
	static TSharedRef<IDetailCustomization> MakeInstance();

	// 定制细节
	virtual void CustomizeDetails(IDetailLayoutBuilder& InDetailBuilder) override;

private:
	// 获取设置对象
	UBXBehaviorSettings* GetSettings() const;

	// 轴显示名(BXBehavior.* 行为族Tag省略父族前缀:BXBehavior.PerfectDodge → PerfectDodge;悬停提示保留完整名)
	FString GetAxisDisplayName(const FGameplayTag& InTag) const;

	// 渲染矩阵网格
	TSharedRef<SWidget> MakeMatrixWidget();

	// 添加矩阵轴(弹出GameplayTag选择器)
	FReply OnAddAxisClicked();

	// 添加轴的Tag选择回调(TagPicker为窗口模态回调,存待选轴索引)
	void OnAxisTagSelected(const FGameplayTag& InTag);

	// 删除矩阵轴(连带清除该轴的全部关系配置)
	FReply OnRemoveAxisClicked(int32 InAxisIndex);

	// 单元格按钮回调(循环切换关系:空→禁用→中断→禁用并中断)
	FReply OnCellClicked(int32 InRowIndex, int32 InColumnIndex);

	// 获取单元格当前关系
	uint8 GetCellRelation(int32 InRowIndex, int32 InColumnIndex) const;

	// 单元格文本
	FText GetCellText(int32 InRowIndex, int32 InColumnIndex) const;

	// 变更落盘:保存配置+重建运行时索引(无视图重建,轻量可高频)
	void Commit(UBXBehaviorSettings* InSettings);

	// 行为关系配置的目标 ini:插件 Config/DefaultBattleX.ini(随插件分发;找不到插件时返回空,Commit 回退默认落点)
	static FString GetPluginConfigIniPath();

	// 重建矩阵网格本体(SBox容器SetContent换内容,不重建Details整视图)
	void RebuildMatrixGrid();

private:
	TWeakObjectPtr<UBXBehaviorSettings> CachedSettings;

	// 矩阵网格容器(增删轴后SetContent换网格;定制实例由视图持有,容器随定制同生命周期)
	TSharedPtr<SBox> MatrixContainer;

	// 单元格文本控件缓存(键=(行<<32)|列;单元格点击后SetText直改,避免整视图重建)
	TMap<uint64, TWeakPtr<STextBlock>> CellTextWidgets;
};
