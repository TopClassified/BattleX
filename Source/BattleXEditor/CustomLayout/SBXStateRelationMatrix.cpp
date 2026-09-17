#include "SBXStateRelationMatrix.h"

#include "State/BXStateBehaviorSettings.h"

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
#include "Framework/Application/SlateApplication.h"
#include "GameplayTagContainer.h"
#include "GameplayTagsManager.h"
#include "Styling/CoreStyle.h"

#define LOCTEXT_NAMESPACE "SBXStateRelationMatrix"

// 矩阵统一行高(标签列/数据行/表头条一致,纵向滚动时跨面板逐行对齐)
// 注意:同模块unity build下文件级符号须带自身前缀防跨cpp撞名(行为矩阵侧为MatrixRowHeight)
constexpr float StateMatrixRowHeight = 30.0f;

void SBXStateRelationMatrix::Construct(const FArguments& InArgs, UBXStateBehaviorSettings* InSettings)
{
	CachedSettings = InSettings ? InSettings : GetMutableDefault<UBXStateBehaviorSettings>();

	// 打开页面时轴同步:按已注册Tag自动补齐缺失轴+清未注册残留,零变更零写入
	EnsureAxesComplete();

	// 矩阵网格容器:后续任何变更都不重建宿主Details视图
	MatrixContainer = SNew(SBox);
	MatrixContainer->SetContent(MakeMatrixWidget());

	ChildSlot
	[
		MatrixContainer.ToSharedRef()
	];
}

int32 SBXStateRelationMatrix::EnsureAxesComplete()
{
	UBXStateBehaviorSettings* Settings = GetSettings();
	if (!Settings)
	{
		return 0;
	}

	int32 ChangedCount = 0;

	// 残留清理:两轴数组中未注册的Tag(改名/删除残留)移除
	for (int32 i = Settings->StateRelationTags.Num() - 1; i >= 0; --i)
	{
		if (!UGameplayTagsManager::Get().FindTagNode(Settings->StateRelationTags[i]).IsValid())
		{
			Settings->StateRelationTags.RemoveAt(i);
			++ChangedCount;
		}
	}
	for (int32 i = Settings->BehaviorRelationTags.Num() - 1; i >= 0; --i)
	{
		if (!UGameplayTagsManager::Get().FindTagNode(Settings->BehaviorRelationTags[i]).IsValid())
		{
			Settings->BehaviorRelationTags.RemoveAt(i);
			++ChangedCount;
		}
	}

	// 自动补齐:行=BXState根下已注册后代(根未注册=无状态Tag,静默跳过),列=BXBehavior根下后代;
	// 缺失的追加到末尾(不打乱现有顺序)
	{
		const FGameplayTag StateRootTag = UGameplayTagsManager::Get().RequestGameplayTag(FName(TEXT("BXState.Stun")), false);
		TArray<FGameplayTag> StateDescendants;
		if (StateRootTag.IsValid())
		{
			CollectTagDescendants(StateRootTag, StateDescendants);
		}
		for (const FGameplayTag& AxisTag : StateDescendants)
		{
			if (!Settings->StateRelationTags.Contains(AxisTag))
			{
				Settings->StateRelationTags.Add(AxisTag);
				++ChangedCount;
			}
		}

		TArray<FGameplayTag> BehaviorDescendants;
		// 根Tag已移除显式定义(2026-09-16):层级由字符串天然构成,树根用字面量隐式节点
		CollectTagDescendants(FGameplayTag::RequestGameplayTag(FName(TEXT("BXBehavior"))), BehaviorDescendants);
		for (const FGameplayTag& AxisTag : BehaviorDescendants)
		{
			if (!Settings->BehaviorRelationTags.Contains(AxisTag))
			{
				Settings->BehaviorRelationTags.Add(AxisTag);
				++ChangedCount;
			}
		}
	}

	// 打开页面零写入(2026-09-16数据损毁教训):轴同步纯内存,仅影响本次渲染;
	// 状态矩阵无运行时索引,不做打开期关系清理——未注册残留键不可见且无害,待用户显式编辑时自行处置
	return ChangedCount;
}

void SBXStateRelationMatrix::CollectTagDescendants(const FGameplayTag& InRootTag, TArray<FGameplayTag>& OutTags) const
{
	const FGameplayTagContainer Children = UGameplayTagsManager::Get().RequestGameplayTagChildren(InRootTag);
	for (const FGameplayTag& Child : Children)
	{
		OutTags.AddUnique(Child);
		CollectTagDescendants(Child, OutTags);
	}
}

UBXStateBehaviorSettings* SBXStateRelationMatrix::GetSettings() const
{
	return CachedSettings;
}

FString SBXStateRelationMatrix::GetStateAxisDisplayName(const FGameplayTag& InTag) const
{
	// 状态轴显示省略命名空间首段(BXState.Stun.Knockback → Stun.Knockback);无点原样显示
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
	TagString.RemoveFromStart(TEXT("BXBehavior."));
	return TagString;
}

FString SBXStateRelationMatrix::GetTagTooltip(const FGameplayTag& InTag) const
{
	// 完整名+原生Tag注释(中文说明;UE_DEFINE_GAMEPLAY_TAG_COMMENT 的注释经注册期写入节点 DevComment)
	FString Tooltip = FString::Printf(TEXT("完整名: %s"), *InTag.ToString());
	if (TSharedPtr<FGameplayTagNode> TagNode = UGameplayTagsManager::Get().FindTagNode(InTag))
	{
		const FString& DevComment = TagNode->GetDevComment();
		if (!DevComment.IsEmpty())
		{
			Tooltip += FString::Printf(TEXT("\n说明: %s"), *DevComment);
		}
	}
	return Tooltip;
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
		// 列头文本(悬停高亮联动染黑,常态=弱化前景;直挂tooltip含完整名+中文说明)
		TSharedRef<STextBlock> HeaderText = SNew(STextBlock)
			.Text(FText::FromString(GetBehaviorAxisDisplayName(Settings->BehaviorRelationTags[Col])))
			.Font(GridFont)
			.ColorAndOpacity(FSlateColor::UseSubduedForeground())
			.ToolTipText(FText::FromString(GetTagTooltip(Settings->BehaviorRelationTags[Col])));
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
				.ToolTipText(FText::FromString(GetTagTooltip(Settings->BehaviorRelationTags[Col])))
				[
					SNew(SOverlay)
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
		// 单元格循环的回调参数与缓存键沿用本行索引(行头按钮删除后仍被单元格使用,勿再清)
		const int32 RowIndex = Row;

		// 行头文本(悬停高亮联动染黑,常态=标准前景;直挂tooltip含完整名+中文说明)
		TSharedRef<STextBlock> RowLabelText = SNew(STextBlock)
			.Text(FText::FromString(GetStateAxisDisplayName(Settings->StateRelationTags[Row])))
			.Font(GridFont)
			.ColorAndOpacity(FSlateColor::UseForeground())
			.ToolTipText(FText::FromString(GetTagTooltip(Settings->StateRelationTags[Row])));
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
				.ToolTipText(FText::FromString(GetTagTooltip(Settings->StateRelationTags[Row])))
				[
					SNew(SOverlay)
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

			// 单元格文本(悬停高亮联动染黑,常态=标准前景;HitTestInvisible 让悬停提示穿透到表头容器)
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

	// ── 纵向平铺(2026-09-16:无纵向滚动器,矩阵随内容自然展开、随设置页滚动)──
	// 仅横向滚动:滚动条钉在网格体底缘,横向滚动驱动冻结表头条反向平移;
	// ConsumeMouseWheel=Never 让纵向滚轮穿透给设置页(鼠标悬在矩阵上滚轮仍滚页面)
	TSharedRef<SScrollBar> HorizontalBar = SNew(SScrollBar)
		.Orientation(Orient_Horizontal)
		.Thickness(FVector2D(9.0f, 9.0f));

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

	// 网格体 Overlay:标签列(横向钉住) + 横向滚动区;横向滚动条钉在底缘
	TSharedRef<SOverlay> BodyViewport = SNew(SOverlay)
		+ SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.Padding(0.0f, 0.0f, 0.0f, 10.0f)
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
		]
		+ SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Bottom)
		.Padding(0.0f, 0.0f, 0.0f, 1.0f)
		[
			HorizontalBar
		];

	// ── 组装:说明行 + 冻结表头条 + 平铺网格体(纵向随内容展开,无定高视口) ──
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
				SNew(STextBlock)
				.Text(LOCTEXT("AxisHint", "轴按已注册Tag自动补齐(行=BXState.Stun.*,列=BXBehavior.*);单元格点击循环:空→禁用→中断→禁用并中断(行=该状态进入时中断哪些行为+存续期禁用哪些行为)"))
				.Font(FCoreStyle::GetDefaultFontStyle("Regular", 9))
				.ColorAndOpacity(FSlateColor::UseSubduedForeground())
			]
		];

	MatrixBox->AddSlot()
		.AutoHeight()
		[
			HeaderClip
		];

	MatrixBox->AddSlot()
		.AutoHeight()
		[
			BodyViewport
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
