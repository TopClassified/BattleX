#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "GameplayTagContainer.h"
#include "Internationalization/Text.h"

class UBXStateBehaviorSettings;
class SBorder;
class SBox;
class STextBlock;
struct FButtonStyle;



// 状态-行为关系矩阵控件(行=状态Tag,列=行为Tag,两轴独立;2026-09-16起由UBXSettings的BattleX页面定制注入——
// 该设置类已关闭自动注册无独立页面,矩阵直接操作其CDO,变更经SaveToPluginConfig直写插件ini)
// 轴在打开页面时按已注册Tag自动补齐(行=BXState.Stun根、列=BXBehavior根下后代追加到末尾,
// 未注册残留的轴与关系条目一并清理——状态矩阵无运行时索引,自愈收在此处);表头纯展示(悬停提示完整名);
// 单元格四态循环:空→禁用→中断→禁用并中断(空格=天然共存不落数据);
// 任何变更(单元格/增删轴)都不走ForceRefreshDetails/整视图重建:
// 单元格点击直改单元格文本,增删轴经SBox容器SetContent只换网格本体;
// 冻结表头条(常驻可见):显式列宽/行高下三块面板(表头条/标签列/网格体)跨面板对齐,
// 表头条横向位移由网格体横向滚动回调驱动,标签列与网格体同处纵向滚动器天然同步;
// 单元格悬停时其行头/列头与单元格本身全部黄底黑字高亮,一眼定位当前在配哪个状态对哪个行为的关系;
// 行头显示状态Tag时省略命名空间首段(BXState.Stun.Knockback → Stun.Knockback),列头省略 BXBehavior. 前缀
class SBXStateRelationMatrix : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SBXStateRelationMatrix) {}
	SLATE_END_ARGS()

	// 构造(InSettings为空时回退CDO)
	void Construct(const FArguments& InArgs, UBXStateBehaviorSettings* InSettings = nullptr);

private:
	// 获取设置对象
	UBXStateBehaviorSettings* GetSettings() const;

	// 状态轴显示名(省略命名空间首段:BXState.Knockback → Knockback;悬停提示保留完整名)
	FString GetStateAxisDisplayName(const FGameplayTag& InTag) const;

	// 行为轴显示名(BXBehavior.* 行为族Tag省略父族前缀,与行为关系矩阵一致)
	FString GetBehaviorAxisDisplayName(const FGameplayTag& InTag) const;

	// 表头悬停提示:完整名+原生Tag注释(中文说明,经FGameplayTagNode::GetDevComment;无注释时仅完整名)
	FString GetTagTooltip(const FGameplayTag& InTag) const;

	// 渲染矩阵网格
	TSharedRef<SWidget> MakeMatrixWidget();

	// 轴同步:行=BXState根、列=BXBehavior根下已注册后代,缺失的追加到对应轴数组末尾(不打乱现有顺序);
	// 未注册Tag的轴与关系条目(键/列引用)一并清理(改名/删除残留,与行为矩阵自愈语义对齐);有变更才落盘一次
	int32 EnsureAxesComplete();

	// 递归收集根Tag下全部已注册后代(不含根自身;Tag树无环,递归安全)
	void CollectTagDescendants(const FGameplayTag& InRootTag, TArray<FGameplayTag>& OutTags) const;

	// 单元格按钮回调(循环切换关系:空→禁用→中断→禁用并中断)
	FReply OnCellClicked(int32 InRowIndex, int32 InColumnIndex);

	// 获取单元格当前关系(位0=中断,位1=禁用)
	uint8 GetCellRelation(int32 InRowIndex, int32 InColumnIndex) const;

	// 单元格文本
	FText GetCellText(int32 InRowIndex, int32 InColumnIndex) const;

	// 单元格按钮样式:空=默认,禁用=蓝,中断=红,禁+中=紫(着色副本缓存在控件实例上,指针须长期存活)
	const FButtonStyle* GetCellButtonStyle(uint8 InRelation);

	// 变更落盘(无运行时索引,轻量可高频)
	void Commit();

	// 重建矩阵网格本体(SBox容器SetContent换内容,不重建宿主Details视图)
	void RebuildMatrixGrid();

	// 悬停联动:单元格悬停时将其行头/列头染黄,一目了然当前在配哪个状态对哪个行为的关系
	void HandleCellHovered(int32 InRowIndex, int32 InColumnIndex);

	// 悬停退出:恢复行头/列头常态配色
	void HandleCellUnhovered(int32 InRowIndex, int32 InColumnIndex);

private:
	UBXStateBehaviorSettings* CachedSettings = nullptr;

	// 矩阵网格容器(增删轴后SetContent换网格;控件由宿主定制持有,容器随控件同生命周期)
	TSharedPtr<SBox> MatrixContainer;

	// 单元格文本控件缓存(键=(行<<32)|列;单元格点击后SetText直改,避免整视图重建)
	TMap<uint64, TWeakPtr<STextBlock>> CellTextWidgets;

	// 单元格按钮缓存(键=(行<<32)|列;点击后按关系切换着色样式)
	TMap<uint64, TWeakPtr<SButton>> CellButtonWidgets;

	// 单元格着色样式缓存(键=关系位码;样式副本被 SButton 指针引用,须随控件实例存活)
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
