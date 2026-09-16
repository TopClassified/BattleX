#include "SBXStateRelationMatrix.h"

#include "State/BXStateBehaviorSettings.h"
#include "BXGameplayTags.h"

#include "Widgets/SBoxPanel.h"
#include "Widgets/SNullWidget.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Layout/SScrollBar.h"
#include "Widgets/SOverlay.h"
#include "Rendering/SlateRenderTransform.h"
#include "Fonts/FontMeasure.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/SWindow.h"
#include "Framework/Application/SlateApplication.h"
#include "GameplayTagContainer.h"
#include "SGameplayTagCombo.h"
#include "Styling/CoreStyle.h"

#define LOCTEXT_NAMESPACE "SBXStateRelationMatrix"

// 矩阵统一行高(标签列/数据行/表头条一致,纵向滚动时跨面板逐行对齐)
// 注意:同模块unity build下文件级符号须带自身前缀防跨cpp撞名(行为矩阵侧为MatrixRowHeight)
constexpr float StateMatrixRowHeight = 30.0f;

void SBXStateRelationMatrix::Construct(const FArguments& InArgs, UBXStateBehaviorSettings* InSettings)
{
	CachedSettings = InSettings ? InSettings : GetMutableDefault<UBXStateBehaviorSettings>();

	// 矩阵网格容器:后续增删轴只SetContent换网格本体,任何变更都不重建宿主Details视图
	MatrixContainer = SNew(SBox);
	MatrixContainer->SetContent(MakeMatrixWidget());

	ChildSlot
	[
		MatrixContainer.ToSharedRef()
	];
}

UBXStateBehaviorSettings* SBXStateRelationMatrix::GetSettings() const
{
	return CachedSettings;
}

FString SBXStateRelationMatrix::GetStateAxisDisplayName(const FGameplayTag& InTag) const
{
	// 状态轴显示省略命名空间首段(BXStunState.Knockback → Knockback,未来 BXState.X → X);无点原样显示
	FString TagString = InTag.GetTagName().ToString();
	int32 DotIndex = INDEX_NONE;
	if (TagString.FindChar(TEXT('.'), DotIndex))
	{
		TagString.RightChopInline(DotIndex + 1);
	}
	return TagString;
}

FString SBXStateRelationMatrix::GetBehaviorAxisDisplayName(const FGameplayTag& InTag) const
{
	// 行为列与行为关系矩阵同规:省略 BXBehavior. 父族前缀;族外 Tag 原样显示
	FString TagString = InTag.GetTagName().ToString();
	TagString.RemoveFromStart(BXGameplayTags::BXBehavior_Root.GetTag().GetTagName().ToString() + TEXT("."));
	return TagString;
}

TSharedRef<SWidget> SBXStateRelationMatrix::MakeMatrixWidget()
{
	CellTextWidgets.Reset();
	CellHighlightWidgets.Reset();
	CellButtonWidgets.Reset();
	RowLabelWidgets.Reset();
	RowLabelHighlightWidgets.Reset();
	ColumnHeaderWidgets.Reset();
	ColumnHeaderHighlightWidgets.Reset();
	HoveredRowIndex = INDEX_NONE;
	HoveredColumnIndex = INDEX_NONE;

	UBXStateBehaviorSettings* Settings = GetSettings();
	if (!Settings)
	{
		return SNullWidget::NullWidget;
	}

	const int32 RowNum = Settings->StateRelationTags.Num();
	const int32 ColNum = Settings->BehaviorRelationTags.Num();

	// ── 显式尺寸(冻结表头的三块面板共用同一组列宽/行高,跨面板对齐的前提) ──
	const FSlateFontInfo GridFont = FCoreStyle::GetDefaultFontStyle("Regular", 9);
	auto MeasureText = [&GridFont](const FString& InText) -> float
	{
		return FSlateApplication::Get().GetRenderer()->GetFontMeasureService()->Measure(FStringView(*InText), GridFont).X;
	};

	// 表格按 16 字符统一列宽(行/列名远小于该宽度,与行为关系矩阵同规)
	const float ColumnWidth = MeasureText(TEXT("ABCDEFGHIJKLMNOP")) + 36.0f;

	// ── 冻结表头条(常驻可见:角标+行为列头按钮;横向位移由网格体横向滚动回调驱动) ──
	TSharedRef<SHorizontalBox> HeaderStrip = SNew(SHorizontalBox);
	HeaderStrip->AddSlot()
		.AutoWidth()
		[
			SNew(SBox)
			.WidthOverride(ColumnWidth)
			.HeightOverride(StateMatrixRowHeight)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("HeaderCorner", "状态＼行为"))
				.Font(GridFont)
			]
		];
	for (int32 Col = 0; Col < ColNum; ++Col)
	{
		const int32 ColIndexForHeader = Col;

		// 列头文本(悬停高亮联动染黑,常态=弱化前景;HitTestInvisible 让点击穿透到按钮)
		TSharedRef<STextBlock> HeaderText = SNew(STextBlock)
			.Text(FText::FromString(GetBehaviorAxisDisplayName(Settings->BehaviorRelationTags[Col])))
			.Font(GridFont)
			.ColorAndOpacity(FSlateColor::UseSubduedForeground())
			.Visibility(EVisibility::HitTestInvisible);
		ColumnHeaderWidgets.Add(Col, HeaderText);

		// 列头黄底高亮层(悬停联动点亮)
		TSharedRef<SBorder> HeaderHighlight = SNew(SBorder)
			.BorderImage(FCoreStyle::Get().GetBrush(TEXT("WhiteBrush")))
			.ColorAndOpacity(FLinearColor(1.0f, 0.82f, 0.0f, 1.0f))
			.Visibility(EVisibility::Hidden)
			[
				SNew(SBox)
			];
		ColumnHeaderHighlightWidgets.Add(Col, HeaderHighlight);

		HeaderStrip->AddSlot()
			.AutoWidth()
			[
				SNew(SBox)
				.WidthOverride(ColumnWidth)
				.HeightOverride(StateMatrixRowHeight)
				.VAlign(VAlign_Fill)
				[
					SNew(SOverlay)
					+ SOverlay::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					[
						SNew(SButton)
						.OnClicked(FOnClicked::CreateRaw(this, &SBXStateRelationMatrix::OnRemoveBehaviorAxisClicked, ColIndexForHeader))
						.ToolTipText(FText::FromString(FString::Printf(TEXT("完整名: %s\n点击删除该行为列(连带清除各状态行中对该行为的配置)"), *Settings->BehaviorRelationTags[Col].ToString())))
					]
					+ SOverlay::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					[
						HeaderHighlight
					]
					+ SOverlay::Slot()
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
					[
						HeaderText
					]
				]
			];
	}

	// 表头条裁剪容器(横向滚动时右溢出裁掉;表头条整体随滚动偏移反向平移)
	TSharedRef<SBox> HeaderClip = SNew(SBox)
		.Clipping(EWidgetClipping::ClipToBounds)
		[
			HeaderStrip
		];

	// ── 标签列(状态行头) + 单元格行(同处纵向滚动器:纵向天然同步;标签列在横向滚动器外:横向钉住不动) ──
	TSharedRef<SVerticalBox> LabelColumn = SNew(SVerticalBox);
	TSharedRef<SVerticalBox> BodyRows = SNew(SVerticalBox);

	for (int32 Row = 0; Row < RowNum; ++Row)
	{
		const int32 RowIndex = Row;

		// 行头文本(悬停高亮联动染黑,常态=标准前景;HitTestInvisible 让点击穿透到按钮)
		TSharedRef<STextBlock> RowLabelText = SNew(STextBlock)
			.Text(FText::FromString(GetStateAxisDisplayName(Settings->StateRelationTags[Row])))
			.Font(GridFont)
			.ColorAndOpacity(FSlateColor::UseForeground())
			.Visibility(EVisibility::HitTestInvisible);
		RowLabelWidgets.Add(Row, RowLabelText);

		// 行头黄底高亮层(悬停联动点亮)
		TSharedRef<SBorder> RowHighlight = SNew(SBorder)
			.BorderImage(FCoreStyle::Get().GetBrush(TEXT("WhiteBrush")))
			.ColorAndOpacity(FLinearColor(1.0f, 0.82f, 0.0f, 1.0f))
			.Visibility(EVisibility::Hidden)
			[
				SNew(SBox)
			];
		RowLabelHighlightWidgets.Add(Row, RowHighlight);

		LabelColumn->AddSlot()
			.AutoHeight()
			[
				SNew(SBox)
				.HeightOverride(StateMatrixRowHeight)
				[
					SNew(SOverlay)
					+ SOverlay::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					[
						SNew(SButton)
						.OnClicked(FOnClicked::CreateRaw(this, &SBXStateRelationMatrix::OnRemoveStateAxisClicked, RowIndex))
						.ToolTipText(FText::FromString(FString::Printf(TEXT("完整名: %s\n点击删除该状态行(连带清除其全部关系配置)"), *Settings->StateRelationTags[Row].ToString())))
					]
					+ SOverlay::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					[
						RowHighlight
					]
					+ SOverlay::Slot()
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
					[
						RowLabelText
					]
				]
			];

		TSharedRef<SHorizontalBox> BodyRow = SNew(SHorizontalBox);
		for (int32 Col = 0; Col < ColNum; ++Col)
		{
			const int32 ColIndex = Col;

			// 单元格文本(悬停高亮联动染黑,常态=标准前景;HitTestInvisible 让点击穿透到按钮)
			TSharedRef<STextBlock> CellText = SNew(STextBlock)
				.Text(this, &SBXStateRelationMatrix::GetCellText, RowIndex, ColIndex)
				.Font(GridFont)
				.ColorAndOpacity(FSlateColor::UseForeground())
				.Visibility(EVisibility::HitTestInvisible);
			CellTextWidgets.Add((uint64(RowIndex) << 32) | uint32(ColIndex), CellText);

			// 单元格按钮(悬停回调联动行头/列头/本格高亮;底色按关系着色:空=默认,禁用=蓝,中断=红,禁+中=紫)
			TSharedRef<SButton> CellButton = SNew(SButton)
				.ButtonStyle(GetCellButtonStyle(GetCellRelation(RowIndex, ColIndex)))
				.OnClicked(FOnClicked::CreateRaw(this, &SBXStateRelationMatrix::OnCellClicked, RowIndex, ColIndex))
				.OnHovered(FSimpleDelegate::CreateLambda([this, RowIndex, ColIndex]() { HandleCellHovered(RowIndex, ColIndex); }))
				.OnUnhovered(FSimpleDelegate::CreateLambda([this, RowIndex, ColIndex]() { HandleCellUnhovered(RowIndex, ColIndex); }));
			CellButtonWidgets.Add((uint64(RowIndex) << 32) | uint32(ColIndex), CellButton);

			// 单元格黄底高亮层(悬停联动点亮,盖住按钮底色形成黄底)
			TSharedRef<SBorder> CellHighlight = SNew(SBorder)
				.BorderImage(FCoreStyle::Get().GetBrush(TEXT("WhiteBrush")))
				.ColorAndOpacity(FLinearColor(1.0f, 0.82f, 0.0f, 1.0f))
				.Visibility(EVisibility::Hidden)
				[
					SNew(SBox)
				];
			CellHighlightWidgets.Add((uint64(RowIndex) << 32) | uint32(ColIndex), CellHighlight);

			BodyRow->AddSlot()
				.AutoWidth()
				[
					SNew(SBox)
					.WidthOverride(ColumnWidth)
					.HeightOverride(StateMatrixRowHeight)
					.VAlign(VAlign_Fill)
					[
						SNew(SOverlay)
						+ SOverlay::Slot()
						.HAlign(HAlign_Fill)
						.VAlign(VAlign_Fill)
						[
							CellButton
						]
						+ SOverlay::Slot()
						.HAlign(HAlign_Fill)
						.VAlign(VAlign_Fill)
						[
							CellHighlight
						]
						+ SOverlay::Slot()
						.HAlign(HAlign_Center)
						.VAlign(VAlign_Center)
						[
							CellText
						]
					]
				];
		}

		BodyRows->AddSlot()
			.AutoHeight()
			[
				SNew(SBox)
				.HeightOverride(StateMatrixRowHeight)
				.VAlign(VAlign_Fill)
				[
					BodyRow
				]
			];
	}

	// ── 双向滚动视口(仅网格体):外纵向+内横向,滚动条经 ExternalScrollbar 钉在视口右缘/底缘 ──
	TSharedRef<SScrollBar> VerticalBar = SNew(SScrollBar)
		.Orientation(Orient_Vertical)
		.Thickness(FVector2D(9.0f, 9.0f));
	TSharedRef<SScrollBar> HorizontalBar = SNew(SScrollBar)
		.Orientation(Orient_Horizontal)
		.Thickness(FVector2D(9.0f, 9.0f));

	// 内层横向:ConsumeMouseWheel=Never 让纵向滚轮穿透给外层;横向滚动驱动冻结表头条反向平移
	TSharedRef<SScrollBox> BodyHScroller = SNew(SScrollBox)
		.Orientation(Orient_Horizontal)
		.ExternalScrollbar(HorizontalBar)
		.ConsumeMouseWheel(EConsumeMouseWheel::Never)
		.OnUserScrolled(FOnUserScrolled::CreateLambda([WeakHeaderStrip = TWeakPtr<SHorizontalBox>(HeaderStrip)](float ScrollOffset)
		{
			if (TSharedPtr<SHorizontalBox> HeaderStripPtr = WeakHeaderStrip.Pin())
			{
				HeaderStripPtr->SetRenderTransform(TOptional<FSlateRenderTransform>(FSlateRenderTransform(FVector2D(-ScrollOffset, 0.0f))));
			}
		}))
		+ SScrollBox::Slot()
		[
			BodyRows
		];

	TSharedRef<SScrollBox> BodyVScroller = SNew(SScrollBox)
		.Orientation(Orient_Vertical)
		.ExternalScrollbar(VerticalBar)
		+ SScrollBox::Slot()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(SBox)
				.WidthOverride(ColumnWidth)
				[
					LabelColumn
				]
			]
			+ SHorizontalBox::Slot()
			[
				BodyHScroller
			]
		];

	// 视口 Overlay:两条滚动条钉在右缘/底缘
	TSharedRef<SOverlay> BodyViewport = SNew(SOverlay)
		+ SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			BodyVScroller
		]
		+ SOverlay::Slot()
		.HAlign(HAlign_Right)
		.VAlign(VAlign_Fill)
		.Padding(0.0f, 0.0f, 1.0f, 0.0f)
		[
			VerticalBar
		]
		+ SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Bottom)
		.Padding(0.0f, 0.0f, 0.0f, 1.0f)
		[
			HorizontalBar
		];

	// ── 组装:轴编辑行(常驻) + 冻结表头条 + 滚动视口;轴多时视口定高内部滚动 ──
	TSharedRef<SVerticalBox> MatrixBox = SNew(SVerticalBox);

	MatrixBox->AddSlot()
		.AutoHeight()
		.Padding(0.0f, 0.0f, 0.0f, 6.0f)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			[
				SNew(SButton)
				.Text(LOCTEXT("AddStateAxis", "+ 添加状态轴"))
				.OnClicked(FOnClicked::CreateRaw(this, &SBXStateRelationMatrix::OnAddAxisClicked, true))
				.ToolTipText(LOCTEXT("AddStateAxisTip", "弹出GameplayTag选择器,仅列出 BXStunState.* 状态Tag(状态Tag树迁移时需同步更新过滤器)"))
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			.Padding(12.0f, 0.0f, 0.0f, 0.0f)
			[
				SNew(SButton)
				.Text(LOCTEXT("AddBehaviorAxis", "+ 添加行为轴"))
				.OnClicked(FOnClicked::CreateRaw(this, &SBXStateRelationMatrix::OnAddAxisClicked, false))
				.ToolTipText(LOCTEXT("AddBehaviorAxisTip", "弹出GameplayTag选择器,仅列出 BXBehavior.* 行为族Tag(如 BXBehavior.Dodge)"))
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			.Padding(12.0f, 0.0f)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("AxisHint", "点击行头/列头删除对应轴;单元格点击循环:空→禁用→中断→禁用并中断(行=该状态进入时中断哪些行为+存续期禁用哪些行为)"))
				.Font(FCoreStyle::GetDefaultFontStyle("Regular", 9))
				.ColorAndOpacity(FSlateColor::UseSubduedForeground())
			]
		];

	MatrixBox->AddSlot()
		.AutoHeight()
		[
			HeaderClip
		];

	// 轴多时给视口定高(内部纵向滚动),轴少时自然高度随设置页滚动
	TSharedRef<SWidget> BodyArea = BodyViewport;
	if (FMath::Max(RowNum, ColNum) >= 10)
	{
		BodyArea = SNew(SBox)
			.HeightOverride(400.0f)
			[
				BodyViewport
			];
	}

	MatrixBox->AddSlot()
		.AutoHeight()
		[
			BodyArea
		];

	return MatrixBox;
}

void SBXStateRelationMatrix::Commit()
{
	// 变更落盘(轻量,单元格点击高频路径;不动视图)
	if (UBXStateBehaviorSettings* Settings = GetSettings())
	{
		Settings->SaveToPluginConfig();
	}
}

void SBXStateRelationMatrix::RebuildMatrixGrid()
{
	// 增删轴后只换网格本体(SBox SetContent),不重建宿主Details视图——
	// 整视图重建要重跑全部属性反射枚举+定制+设置页重排,是设置页卡顿根源
	if (MatrixContainer.IsValid())
	{
		MatrixContainer->SetContent(MakeMatrixWidget());
	}
}

void SBXStateRelationMatrix::HandleCellHovered(int32 InRowIndex, int32 InColumnIndex)
{
	// 先恢复上一次的行头/列头/单元格配色,再点亮当前行/列的黄底黑字
	HandleCellUnhovered(HoveredRowIndex, HoveredColumnIndex);

	HoveredRowIndex = InRowIndex;
	HoveredColumnIndex = InColumnIndex;

	// 行头:黄底高亮层点亮 + 文字染黑
	if (TSharedPtr<SBorder> RowHighlight = RowLabelHighlightWidgets.FindRef(InRowIndex).Pin())
	{
		RowHighlight->SetVisibility(EVisibility::HitTestInvisible);
	}
	if (TSharedPtr<STextBlock> RowLabel = RowLabelWidgets.FindRef(InRowIndex).Pin())
	{
		RowLabel->SetColorAndOpacity(FSlateColor(FLinearColor::Black));
	}

	// 列头:黄底高亮层点亮 + 文字染黑
	if (TSharedPtr<SBorder> HeaderHighlight = ColumnHeaderHighlightWidgets.FindRef(InColumnIndex).Pin())
	{
		HeaderHighlight->SetVisibility(EVisibility::HitTestInvisible);
	}
	if (TSharedPtr<STextBlock> HeaderText = ColumnHeaderWidgets.FindRef(InColumnIndex).Pin())
	{
		HeaderText->SetColorAndOpacity(FSlateColor(FLinearColor::Black));
	}

	// 单元格:黄底高亮层点亮 + 文字染黑
	const uint64 CellKey = (uint64(InRowIndex) << 32) | uint32(InColumnIndex);
	if (TSharedPtr<SBorder> CellHighlight = CellHighlightWidgets.FindRef(CellKey).Pin())
	{
		CellHighlight->SetVisibility(EVisibility::HitTestInvisible);
	}
	if (TSharedPtr<STextBlock> CellText = CellTextWidgets.FindRef(CellKey).Pin())
	{
		CellText->SetColorAndOpacity(FSlateColor(FLinearColor::Black));
	}
}

void SBXStateRelationMatrix::HandleCellUnhovered(int32 InRowIndex, int32 InColumnIndex)
{
	if (InRowIndex == INDEX_NONE && InColumnIndex == INDEX_NONE)
	{
		return;
	}

	// 恢复常态配色(行头/列头/单元格黄底熄灭,文字回常态前景)
	if (TSharedPtr<SBorder> RowHighlight = RowLabelHighlightWidgets.FindRef(InRowIndex).Pin())
	{
		RowHighlight->SetVisibility(EVisibility::Hidden);
	}
	if (TSharedPtr<STextBlock> RowLabel = RowLabelWidgets.FindRef(InRowIndex).Pin())
	{
		RowLabel->SetColorAndOpacity(FSlateColor::UseForeground());
	}

	if (TSharedPtr<SBorder> HeaderHighlight = ColumnHeaderHighlightWidgets.FindRef(InColumnIndex).Pin())
	{
		HeaderHighlight->SetVisibility(EVisibility::Hidden);
	}
	if (TSharedPtr<STextBlock> HeaderText = ColumnHeaderWidgets.FindRef(InColumnIndex).Pin())
	{
		HeaderText->SetColorAndOpacity(FSlateColor::UseSubduedForeground());
	}

	const uint64 CellKey = (uint64(InRowIndex) << 32) | uint32(InColumnIndex);
	if (TSharedPtr<SBorder> CellHighlight = CellHighlightWidgets.FindRef(CellKey).Pin())
	{
		CellHighlight->SetVisibility(EVisibility::Hidden);
	}
	if (TSharedPtr<STextBlock> CellText = CellTextWidgets.FindRef(CellKey).Pin())
	{
		CellText->SetColorAndOpacity(FSlateColor::UseForeground());
	}

	if (HoveredRowIndex == InRowIndex && HoveredColumnIndex == InColumnIndex)
	{
		HoveredRowIndex = INDEX_NONE;
		HoveredColumnIndex = INDEX_NONE;
	}
}

FReply SBXStateRelationMatrix::OnAddAxisClicked(bool bInStateAxis)
{
	UBXStateBehaviorSettings* Settings = GetSettings();
	if (!Settings)
	{
		return FReply::Unhandled();
	}

	// 弹出独立窗口内的Tag选择器(SGameplayTagCombo,选择后落轴)
	TSharedRef<SWindow> PickerWindow = SNew(SWindow)
		.Title(bInStateAxis ? LOCTEXT("AddStateAxisWindowTitle", "选择状态轴(状态Tag)") : LOCTEXT("AddBehaviorAxisWindowTitle", "选择行为轴(行为/族Tag)"))
		.SizingRule(ESizingRule::Autosized)
		.AutoCenter(EAutoCenter::PrimaryWorkArea);

	TWeakObjectPtr<UBXStateBehaviorSettings> WeakSettings = Settings;
	TWeakPtr<SWindow> WeakWindow = PickerWindow;

	PickerWindow->SetContent(
		SNew(SBorder)
		.Padding(12.0f)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0.0f, 0.0f, 0.0f, 8.0f)
			[
				SNew(STextBlock)
				.Text(bInStateAxis
					? LOCTEXT("AddStateAxisPrompt", "选择加入矩阵行轴的状态Tag(仅列出 BXStunState.* 硬直状态)")
					: LOCTEXT("AddBehaviorAxisPrompt", "选择加入矩阵列轴的行为Tag(仅列出 BXBehavior.* 行为族,如 BXBehavior.Dodge)"))
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SGameplayTagCombo)
				// 状态轴过滤器=当前状态命名空间BXStunState(状态Tag树迁移至BXState.*时需同步更新此过滤串);
				// 行为轴过滤器=BXBehavior行为族根(Filter 为根名串,引擎经 GetFilteredGameplayRootTags 裁剪树根)
				.Filter(bInStateAxis ? FString(TEXT("BXStunState")) : BXGameplayTags::BXBehavior_Root.GetTag().ToString())
				.OnTagChanged_Lambda([this, WeakSettings, WeakWindow, bInStateAxis](const FGameplayTag& SelectedTag)
				{
					if (!SelectedTag.IsValid())
					{
						return;
					}

					if (UBXStateBehaviorSettings* SettingsPtr = WeakSettings.Get())
					{
						if (bInStateAxis)
						{
							SettingsPtr->StateRelationTags.AddUnique(SelectedTag);
						}
						else
						{
							SettingsPtr->BehaviorRelationTags.AddUnique(SelectedTag);
						}
						SettingsPtr->SaveToPluginConfig();
						RebuildMatrixGrid();
					}

					if (TSharedPtr<SWindow> WindowPin = WeakWindow.Pin())
					{
						WindowPin->RequestDestroyWindow();
					}
				})
			]
		]);

	FSlateApplication::Get().AddWindow(PickerWindow);
	return FReply::Handled();
}

FReply SBXStateRelationMatrix::OnRemoveStateAxisClicked(int32 InAxisIndex)
{
	UBXStateBehaviorSettings* Settings = GetSettings();
	if (!Settings || !Settings->StateRelationTags.IsValidIndex(InAxisIndex))
	{
		return FReply::Unhandled();
	}

	const FGameplayTag AxisTag = Settings->StateRelationTags[InAxisIndex];

	// 清除该状态行的全部关系配置(两表行键条目)
	Settings->InterruptRelations.Remove(AxisTag);
	Settings->ForbidRelations.Remove(AxisTag);

	// 移除轴本体并收尾(只换网格本体,不重建宿主Details视图)
	Settings->StateRelationTags.RemoveAt(InAxisIndex);
	Commit();
	RebuildMatrixGrid();

	return FReply::Handled();
}

FReply SBXStateRelationMatrix::OnRemoveBehaviorAxisClicked(int32 InAxisIndex)
{
	UBXStateBehaviorSettings* Settings = GetSettings();
	if (!Settings || !Settings->BehaviorRelationTags.IsValidIndex(InAxisIndex))
	{
		return FReply::Unhandled();
	}

	const FGameplayTag AxisTag = Settings->BehaviorRelationTags[InAxisIndex];

	// 清除各状态行容器中对该行为列的引用
	for (TPair<FGameplayTag, FGameplayTagContainer>& Pair : Settings->InterruptRelations)
	{
		Pair.Value.RemoveTag(AxisTag);
	}
	for (TPair<FGameplayTag, FGameplayTagContainer>& Pair : Settings->ForbidRelations)
	{
		Pair.Value.RemoveTag(AxisTag);
	}

	// 移除轴本体并收尾(只换网格本体,不重建宿主Details视图)
	Settings->BehaviorRelationTags.RemoveAt(InAxisIndex);
	Commit();
	RebuildMatrixGrid();

	return FReply::Handled();
}

FReply SBXStateRelationMatrix::OnCellClicked(int32 InRowIndex, int32 InColumnIndex)
{
	UBXStateBehaviorSettings* Settings = GetSettings();
	if (!Settings || !Settings->StateRelationTags.IsValidIndex(InRowIndex) || !Settings->BehaviorRelationTags.IsValidIndex(InColumnIndex))
	{
		return FReply::Unhandled();
	}

	const FGameplayTag RowTag = Settings->StateRelationTags[InRowIndex];
	const FGameplayTag ColTag = Settings->BehaviorRelationTags[InColumnIndex];
	if (!RowTag.IsValid() || !ColTag.IsValid())
	{
		return FReply::Unhandled();
	}

	// 四态循环:空(天然共存)→禁用→中断→禁用并中断→空
	// (位0=中断[InterruptRelations],位1=禁用[ForbidRelations];空格不是关系,是关系的缺席=天然共存;
	//  同格双配置=禁用并中断:进入时停运列中在位行为+存续期挡启动)
	static constexpr uint8 NextOf[4] = { 2, 3, 1, 0 };
	const uint8 Current = GetCellRelation(InRowIndex, InColumnIndex);
	const uint8 Next = NextOf[Current];

	// 清除旧关系
	if (FGameplayTagContainer* Container = Settings->InterruptRelations.Find(RowTag))
	{
		Container->RemoveTag(ColTag);
		if (Container->IsEmpty())
		{
			Settings->InterruptRelations.Remove(RowTag);
		}
	}
	if (FGameplayTagContainer* Container = Settings->ForbidRelations.Find(RowTag))
	{
		Container->RemoveTag(ColTag);
		if (Container->IsEmpty())
		{
			Settings->ForbidRelations.Remove(RowTag);
		}
	}

	// 写入新关系(禁用与中断两轴独立,可同格并存)
	if (Next == 1 || Next == 3)
	{
		Settings->InterruptRelations.FindOrAdd(RowTag).AddTag(ColTag);
	}
	if (Next == 2 || Next == 3)
	{
		Settings->ForbidRelations.FindOrAdd(RowTag).AddTag(ColTag);
	}

	Commit();

	// 直改单元格文本(不重建视图:整视图重建是矩阵卡顿根源)
	if (TSharedPtr<STextBlock> CellText = CellTextWidgets.FindRef((uint64(InRowIndex) << 32) | uint32(InColumnIndex)).Pin())
	{
		CellText->SetText(GetCellText(InRowIndex, InColumnIndex));
	}

	// 按新关系切换按钮着色样式(禁用=蓝,中断=红,禁+中=紫,空=默认)
	if (TSharedPtr<SButton> CellButton = CellButtonWidgets.FindRef((uint64(InRowIndex) << 32) | uint32(InColumnIndex)).Pin())
	{
		CellButton->SetButtonStyle(GetCellButtonStyle(Next));
	}

	return FReply::Handled();
}

uint8 SBXStateRelationMatrix::GetCellRelation(int32 InRowIndex, int32 InColumnIndex) const
{
	UBXStateBehaviorSettings* Settings = GetSettings();
	if (!Settings)
	{
		return 0;
	}

	if (!Settings->StateRelationTags.IsValidIndex(InRowIndex) || !Settings->BehaviorRelationTags.IsValidIndex(InColumnIndex))
	{
		return 0;
	}

	const FGameplayTag& RowTag = Settings->StateRelationTags[InRowIndex];
	const FGameplayTag& ColTag = Settings->BehaviorRelationTags[InColumnIndex];

	// 两表独立读取,同格配置返回禁用+中断
	uint8 Relation = 0;

	if (const FGameplayTagContainer* Container = Settings->InterruptRelations.Find(RowTag))
	{
		if (Container->HasTagExact(ColTag))
		{
			Relation |= 1;
		}
	}

	if (const FGameplayTagContainer* Container = Settings->ForbidRelations.Find(RowTag))
	{
		if (Container->HasTagExact(ColTag))
		{
			Relation |= 2;
		}
	}

	return Relation;
}

FText SBXStateRelationMatrix::GetCellText(int32 InRowIndex, int32 InColumnIndex) const
{
	switch (GetCellRelation(InRowIndex, InColumnIndex))
	{
	case 1:
		return LOCTEXT("Interrupt", "中断");
	case 2:
		return LOCTEXT("Forbid", "禁用");
	case 3:
		return LOCTEXT("ForbidInterrupt", "禁+中");
	default:
		return LOCTEXT("Empty", "空");
	}
}

const FButtonStyle* SBXStateRelationMatrix::GetCellButtonStyle(uint8 InRelation)
{
	// 空=默认按钮样式
	if (InRelation == 0)
	{
		return &FCoreStyle::Get().GetWidgetStyle<FButtonStyle>("Button");
	}

	// 着色样式副本按关系缓存(样式指针被 SButton 引用,须随控件实例存活)
	if (const TSharedPtr<FButtonStyle>* Cached = CellButtonStyles.Find(InRelation))
	{
		return Cached->Get();
	}

	// 禁用=重蓝,中断=重红,禁+中=重紫(与行为关系矩阵同配色;按钮文字为白色,底色必须压暗保证对比度)
	FLinearColor Tint(0.03f, 0.10f, 0.28f, 1.0f);
	if (InRelation == 1)
	{
		Tint = FLinearColor(0.28f, 0.03f, 0.03f, 1.0f);
	}
	else if (InRelation == 3)
	{
		Tint = FLinearColor(0.18f, 0.06f, 0.32f, 1.0f);
	}

	FButtonStyle Style = FCoreStyle::Get().GetWidgetStyle<FButtonStyle>("Button");
	Style.Normal.TintColor = Tint;
	Style.Hovered.TintColor = FLinearColor(
		FMath::Min(Tint.R + 0.08f, 1.0f),
		FMath::Min(Tint.G + 0.08f, 1.0f),
		FMath::Min(Tint.B + 0.08f, 1.0f), 1.0f);
	Style.Pressed.TintColor = Tint * 0.7f;
	Style.Disabled.TintColor = Tint;

	TSharedPtr<FButtonStyle> CachedStyle = MakeShareable(new FButtonStyle(MoveTemp(Style)));
	CellButtonStyles.Add(InRelation, CachedStyle);
	return CachedStyle.Get();
}

#undef LOCTEXT_NAMESPACE
