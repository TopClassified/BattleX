#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"
#include "GameplayTagContainer.h"
#include "Internationalization/Text.h"

class IDetailLayoutBuilder;
class UBXBehaviorSettings;
class SBorder;
class STextBlock;
struct FButtonStyle;


// UBXBehaviorSettings的Detail定制:行为关系渲染为矩阵网格
// 轴经"+添加矩阵轴"(GameplayTag选择器)添加,点击列头删除;行头拖拽排序(行列同步跟随);
// 单元格四态循环:空→禁用→中断→禁用并中断(含对角线自关系:自禁用挡同Tag重入,自中断=新实例顶掉旧实例);
// 任何变更(单元格/增删轴)都不走ForceRefreshDetails(整视图重建是设置页卡顿根源):
// 单元格点击直改单元格文本,增删轴经SBox容器SetContent只换网格本体;
// 冻结行头/列头(常驻可见):显式列宽/行高下三块面板(表头条/标签列/网格体)跨面板对齐,
// 表头条横向位移由网格体横向滚动回调驱动,标签列与网格体同处纵向滚动器天然同步;
// 单元格悬停时其行头/列头与单元格本身全部黄底黑字高亮,一眼定位当前在配哪两个行为的关系;
// 行头可拖拽排序:拖到目标行头上/下半场=插到其前/其后,行头与列头同轴同步移动(两轴共用RelationTags数组,
// 关系配置按Tag键存储与顺序无关,重排零迁移)
class FBXBehaviorSettingsCustomization : public IDetailCustomization
{
public:
	// 创建实例
	static TSharedRef<IDetailCustomization> MakeInstance();

	// 定制细节
	virtual void CustomizeDetails(IDetailLayoutBuilder& InDetailBuilder) override;

	// 拖拽排序:把 InFromIndex 的轴移动到插入槽位 InInsertSlot(原数组槽位语义0..Num,
	// 源自身两侧视为未变不落盘;供行头拖放控件调用,控件类定义在cpp内)
	void MoveAxis(int32 InFromIndex, int32 InInsertSlot);

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

	// 单元格按钮样式:空=默认,禁用=蓝,中断=红,禁+中=紫(着色副本缓存在定制实例上,指针须长期存活)
	const FButtonStyle* GetCellButtonStyle(uint8 InRelation);

	// 变更落盘:保存配置+重建运行时索引(无视图重建,轻量可高频)
	void Commit(UBXBehaviorSettings* InSettings);

	// 重建矩阵网格本体(SBox容器SetContent换内容,不重建Details整视图)
	void RebuildMatrixGrid();

	// 悬停联动:单元格悬停时将其行头/列头染黄,一目了然当前在配哪两个行为的关系
	void HandleCellHovered(int32 InRowIndex, int32 InColumnIndex);

	// 悬停退出:恢复行头/列头常态配色
	void HandleCellUnhovered(int32 InRowIndex, int32 InColumnIndex);

private:
	TWeakObjectPtr<UBXBehaviorSettings> CachedSettings;

	// 矩阵网格容器(增删轴后SetContent换网格;定制实例由视图持有,容器随定制同生命周期)
	TSharedPtr<SBox> MatrixContainer;

	// 单元格文本控件缓存(键=(行<<32)|列;单元格点击后SetText直改,避免整视图重建)
	TMap<uint64, TWeakPtr<STextBlock>> CellTextWidgets;

	// 单元格按钮缓存(键=(行<<32)|列;点击后按关系切换着色样式)
	TMap<uint64, TWeakPtr<SButton>> CellButtonWidgets;

	// 单元格着色样式缓存(键=关系位码;样式副本被 SButton 指针引用,须随定制实例存活)
	TMap<uint8, TSharedPtr<FButtonStyle>> CellButtonStyles;

	// 单元格悬停高亮覆盖层缓存(键=(行<<32)|列;悬停时点亮黄色覆盖层)
	TMap<uint64, TWeakPtr<SBorder>> CellHighlightWidgets;

	// 行头/列头文本控件缓存(键=行/列索引;悬停时文字染黑)
	TMap<int32, TWeakPtr<STextBlock>> RowLabelWidgets;
	TMap<int32, TWeakPtr<STextBlock>> ColumnHeaderWidgets;

	// 行头/列头黄底高亮层缓存(键=行/列索引;悬停时点亮黄底)
	TMap<int32, TWeakPtr<SBorder>> RowLabelHighlightWidgets;
	TMap<int32, TWeakPtr<SBorder>> ColumnHeaderHighlightWidgets;

	// 当前悬停的单元格(INDEX_NONE=无)
	int32 HoveredRowIndex = INDEX_NONE;
	int32 HoveredColumnIndex = INDEX_NONE;
};
