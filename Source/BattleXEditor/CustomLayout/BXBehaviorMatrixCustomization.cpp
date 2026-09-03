#include "BXBehaviorMatrixCustomization.h"

#include "Behavior/BXBehaviorSettings.h"
#include "BXGameplayTags.h"

#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "DetailWidgetRow.h"
#include "IDetailsView.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/SNullWidget.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Layout/SScrollBar.h"
#include "Widgets/SOverlay.h"
#include "Layout/Clipping.h"
#include "Rendering/SlateRenderTransform.h"
#include "Fonts/FontMeasure.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/SWindow.h"
#include "Widgets/Layout/SBorder.h"
#include "Framework/Application/SlateApplication.h"
#include "GameplayTagContainer.h"
#include "SGameplayTagCombo.h"
#include "Styling/CoreStyle.h"
#include "Misc/ConfigCacheIni.h"
#include "DragAndDrop/DecoratedDragDropOp.h"

#define LOCTEXT_NAMESPACE "BXBehaviorMatrix"

// 矩阵统一行高(标签列/数据行/表头条一致,纵向滚动时跨面板逐行对齐;行头控件内部同样引用)
constexpr float MatrixRowHeight = 30.0f;

// ── 行头拖拽排序 ──
// 轴拖拽操作:行头拖出时生成,携带源轴索引;FDecoratedDragDropOp 提供跟随光标的轴名提示
class FBXAxisDragDropOp : public FDecoratedDragDropOp
{
public:
	DRAG_DROP_OPERATOR_TYPE(FBXAxisDragDropOp, FDecoratedDragDropOp)

	// 被拖拽轴在 RelationTags 中的源索引
	int32 SourceIndex = INDEX_NONE;

	static TSharedRef<FBXAxisDragDropOp> New(int32 InSourceIndex, const FText& InAxisName)
	{
		TSharedRef<FBXAxisDragDropOp> Operation = MakeShareable(new FBXAxisDragDropOp());
		Operation->SourceIndex = InSourceIndex;
		Operation->CurrentHoverText = InAxisName;
		Operation->Construct();
		return Operation;
	}
};

// 可拖拽行头:按下左键启动拖拽检测(未过阈值不产生拖拽),拖过阈值生成轴拖拽操作;
// 悬停拖拽按落点在本行头上/下半场点亮顶部/底部插入指示线,松手执行轴移动(行头与列头同轴同步);
// 行头整体视觉(标签/悬停高亮/插入指示线)在此内部组装,宿主经 GetLabel/GetHighlight 登记悬停联动缓存
class SBXDraggableAxisHeader : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SBXDraggableAxisHeader)
		: _SourceIndex(INDEX_NONE)
		{}
		// 本行头对应的轴索引(RelationTags 下标)
		SLATE_ARGUMENT(int32, SourceIndex)
		// 轴显示名(标签文本+拖拽跟随光标文本共用)
		SLATE_ARGUMENT(FText, LabelText)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, FBXBehaviorSettingsCustomization* InOwner)
	{
		Owner = InOwner;
		SourceIndex = InArgs._SourceIndex;
		LabelText = InArgs._LabelText;

		// 标签文本(悬停联动染黑,常态=标准前景;HitTestInvisible 让输入穿透到本控件)
		Label = SNew(STextBlock)
			.Text(LabelText)
			.Font(FCoreStyle::GetDefaultFontStyle("Regular", 9))
			.ColorAndOpacity(FSlateColor::UseForeground())
			.Visibility(EVisibility::HitTestInvisible);

		// 黄底高亮层(悬停联动点亮;垫在文字下层)
		Highlight = SNew(SBorder)
			.BorderImage(FCoreStyle::Get().GetBrush(TEXT("WhiteBrush")))
			.ColorAndOpacity(FLinearColor(1.0f, 0.82f, 0.0f, 1.0f))
			.Visibility(EVisibility::Hidden)
			[
				SNew(SBox)
			];

		// 顶部/底部插入指示线(悬停拖拽按落点半场点亮,3px 亮蓝条)
		auto MakeInsertLine = []() -> TSharedPtr<SBorder>
		{
			return SNew(SBorder)
				.BorderImage(FCoreStyle::Get().GetBrush(TEXT("WhiteBrush")))
				.ColorAndOpacity(FLinearColor(0.2f, 0.55f, 1.0f, 0.95f))
				.Visibility(EVisibility::Hidden)
				[
					SNew(SBox)
				];
		};
		InsertAboveLine = MakeInsertLine();
		InsertBelowLine = MakeInsertLine();

		ChildSlot
		[
			SNew(SBox)
			.HeightOverride(MatrixRowHeight)
			[
				SNew(SOverlay)
				+ SOverlay::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				[
					Highlight.ToSharedRef()
				]
				+ SOverlay::Slot()
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				[
					Label.ToSharedRef()
				]
				+ SOverlay::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Top)
				.Padding(FMargin(2.0f, 0.0f))
				[
					SNew(SBox)
					.HeightOverride(3.0f)
					[
						InsertAboveLine.ToSharedRef()
					]
				]
				+ SOverlay::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Bottom)
				.Padding(FMargin(2.0f, 0.0f))
				[
					SNew(SBox)
					.HeightOverride(3.0f)
					[
						InsertBelowLine.ToSharedRef()
					]
				]
			]
		];
	}

	// 按下左键即启动拖拽检测(阈值内松手只是无效按下,不影响悬停/提示)
	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override
	{
		if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
		{
			return FReply::Handled().DetectDrag(SharedThis(this), EKeys::LeftMouseButton);
		}
		return FReply::Unhandled();
	}

	// 拖过阈值:生成轴拖拽操作(跟随光标显示轴名)
	virtual FReply OnDragDetected(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override
	{
		if (MouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
		{
			return FReply::Handled().BeginDragDrop(FBXAxisDragDropOp::New(SourceIndex, LabelText));
		}
		return FReply::Unhandled();
	}

	// 悬停拖拽:按落点位于本行头上/下半场点亮对应插入指示线;悬停源行头自身不显示(放回原位)
	virtual FReply OnDragOver(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent) override
	{
		const FBXAxisDragDropOp* Operation = GetAxisDragOp(DragDropEvent);
		if (!Operation)
		{
			return FReply::Unhandled();
		}

		const bool bInsertBelow = IsPointerInLowerHalf(MyGeometry, DragDropEvent);
		const bool bIsSourceHeader = (Operation->SourceIndex == SourceIndex);
		if (InsertAboveLine.IsValid())
		{
			InsertAboveLine->SetVisibility(!bIsSourceHeader && !bInsertBelow ? EVisibility::HitTestInvisible : EVisibility::Hidden);
		}
		if (InsertBelowLine.IsValid())
		{
			InsertBelowLine->SetVisibility(!bIsSourceHeader && bInsertBelow ? EVisibility::HitTestInvisible : EVisibility::Hidden);
		}
		return FReply::Handled();
	}

	// 离开/拖拽收尾(引擎对取消路径也向已悬停控件补发OnDragLeave):熄灭插入指示线
	virtual void OnDragLeave(const FDragDropEvent& DragDropEvent) override
	{
		if (InsertAboveLine.IsValid())
		{
			InsertAboveLine->SetVisibility(EVisibility::Hidden);
		}
		if (InsertBelowLine.IsValid())
		{
			InsertBelowLine->SetVisibility(EVisibility::Hidden);
		}
	}

	// 松手:源轴移到本行头槽位(落点上/下半场=插到其前/其后),宿主 Commit+换网格
	virtual FReply OnDrop(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent) override
	{
		const FBXAxisDragDropOp* Operation = GetAxisDragOp(DragDropEvent);
		if (!Operation || !Owner)
		{
			return FReply::Unhandled();
		}

		const bool bInsertBelow = IsPointerInLowerHalf(MyGeometry, DragDropEvent);
		Owner->MoveAxis(Operation->SourceIndex, SourceIndex + (bInsertBelow ? 1 : 0));
		return FReply::Handled();
	}

	// 悬停抓手光标(可拖拽的视觉暗示)
	virtual FCursorReply OnCursorQuery(const FGeometry& MyGeometry, const FPointerEvent& CursorEvent) const override
	{
		return FCursorReply::Cursor(EMouseCursor::GrabHand);
	}

	// 悬停联动缓存登记入口(宿主 HandleCellHovered 染黄/染黑用)
	TSharedRef<STextBlock> GetLabel() const { return Label.ToSharedRef(); }
	TSharedRef<SBorder> GetHighlight() const { return Highlight.ToSharedRef(); }

private:
	// 从拖拽事件取本编辑器的轴拖拽操作(外部拖拽/其它类型返回nullptr)
	static const FBXAxisDragDropOp* GetAxisDragOp(const FDragDropEvent& DragDropEvent)
	{
		const TSharedPtr<FDragDropOperation> Operation = DragDropEvent.GetOperation();
		if (Operation.IsValid() && Operation->IsOfType<FBXAxisDragDropOp>())
		{
			return static_cast<const FBXAxisDragDropOp*>(Operation.Get());
		}
		return nullptr;
	}

	// 落点是否在本行头下半场(上半场=插到其前,下半场=插到其后)
	static bool IsPointerInLowerHalf(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent)
	{
		const FVector2D LocalPosition = MyGeometry.AbsoluteToLocal(DragDropEvent.GetScreenSpacePosition());
		return LocalPosition.Y > MyGeometry.GetLocalSize().Y * 0.5f;
	}

	FBXBehaviorSettingsCustomization* Owner = nullptr;
	int32 SourceIndex = INDEX_NONE;
	FText LabelText;
	TSharedPtr<STextBlock> Label;
	TSharedPtr<SBorder> Highlight;
	// 顶部/底部插入指示线(3px亮蓝条,Hidden 常态)
	TSharedPtr<SBorder> InsertAboveLine;
	TSharedPtr<SBorder> InsertBelowLine;
};

TSharedRef<IDetailCustomization> FBXBehaviorSettingsCustomization::MakeInstance()
{
	return MakeShareable(new FBXBehaviorSettingsCustomization());
}

void FBXBehaviorSettingsCustomization::CustomizeDetails(IDetailLayoutBuilder& InDetailBuilder)
{
	// 缓存设置对象(矩阵读写直接走配置CDO)
	// GetDetailsViewSharedPtr返回TSharedPtr需用->(旧GetDetailsView返回裸指针已于5.5弃用);
	// Project Settings页面选中对象可能不含Settings实例(如页面无属性可显示时),回退CDO保证矩阵始终可用
	const TArray<TWeakObjectPtr<UObject>>& Objects = InDetailBuilder.GetDetailsViewSharedPtr()->GetSelectedObjects();
	CachedSettings = nullptr;
	for (const TWeakObjectPtr<UObject>& Object : Objects)
	{
		if (UBXBehaviorSettings* Settings = Cast<UBXBehaviorSettings>(Object.Get()))
		{
			CachedSettings = Settings;
			break;
		}
	}
	if (!CachedSettings.IsValid())
	{
		CachedSettings = GetMutableDefault<UBXBehaviorSettings>();
	}

	// 矩阵网格容器:后续增删轴只SetContent换网格本体,任何变更都不走ForceRefreshDetails
	MatrixContainer = SNew(SBox);
	MatrixContainer->SetContent(MakeMatrixWidget());

	// 关系字段收编为矩阵:仅隐藏三个原始属性,保留分类本体显示自定义矩阵行
	// (引擎BuildCategories对HideCategory的分类整体跳过——含自定义行,原实现因此从不渲染;
	//  HideProperty保留分类可见性,自定义行才会出现)
	InDetailBuilder.HideProperty(GET_MEMBER_NAME_CHECKED(UBXBehaviorSettings, RelationTags));
	InDetailBuilder.HideProperty(GET_MEMBER_NAME_CHECKED(UBXBehaviorSettings, ExpelRelations));
	InDetailBuilder.HideProperty(GET_MEMBER_NAME_CHECKED(UBXBehaviorSettings, RejectRelations));

	IDetailCategoryBuilder& MatrixCategory = InDetailBuilder.EditCategory("Matrix");
	MatrixCategory.AddCustomRow(LOCTEXT("MatrixRow", "关系矩阵"))
		.WholeRowContent()
		[
			MatrixContainer.ToSharedRef()
		];
}

UBXBehaviorSettings* FBXBehaviorSettingsCustomization::GetSettings() const
{
	return CachedSettings.Get();
}

FString FBXBehaviorSettingsCustomization::GetAxisDisplayName(const FGameplayTag& InTag) const
{
	// 矩阵轴全部位于 BXBehavior.* 行为族下,UI 显示省略父族前缀;族外 Tag 原样显示
	FString TagString = InTag.GetTagName().ToString();
	TagString.RemoveFromStart(BXGameplayTags::BXBehavior_Root.GetTag().GetTagName().ToString() + TEXT("."));
	return TagString;
}

TSharedRef<SWidget> FBXBehaviorSettingsCustomization::MakeMatrixWidget()
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

	UBXBehaviorSettings* Settings = GetSettings();
	if (!Settings)
	{
		return SNullWidget::NullWidget;
	}

	const int32 AxisNum = Settings->RelationTags.Num();

	// ── 显式尺寸(冻结行头/列头的三块面板共用同一组列宽/行高,跨面板对齐的前提) ──
	// 网格文本统一 9pt,测得宽度即渲染宽度
	const FSlateFontInfo GridFont = FCoreStyle::GetDefaultFontStyle("Regular", 9);
	auto MeasureText = [&GridFont](const FString& InText) -> float
	{
		return FSlateApplication::Get().GetRenderer()->GetFontMeasureService()->Measure(FStringView(*InText), GridFont).X;
	};

	// 常态行高统一用文件级 MatrixRowHeight(标签列/数据行/表头条一致,纵向滚动时跨面板逐行对齐)

	// 行为命名约定:显示名≤16字符——表格按 16 字符统一列宽(标签列/数据列同宽,"禁+中"等单元格文本远小于该宽度)
	const float ColumnWidth = MeasureText(TEXT("ABCDEFGHIJKLMNOP")) + 36.0f;

	// ── 冻结表头条(常驻可见:角标+列头按钮;横向位移由网格体横向滚动回调驱动) ──
	TSharedRef<SHorizontalBox> HeaderStrip = SNew(SHorizontalBox);
	HeaderStrip->AddSlot()
		.AutoWidth()
		[
			SNew(SBox)
			.WidthOverride(ColumnWidth)
			.HeightOverride(MatrixRowHeight)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("HeaderCorner", "行＼列"))
				.Font(GridFont)
			]
		];
	for (int32 Col = 0; Col < AxisNum; ++Col)
	{
		const int32 ColIndexForHeader = Col;

		// 列头文本(顶层常驻显示;悬停高亮联动染黑,常态=弱化前景;HitTestInvisible 让点击穿透到按钮)
		TSharedRef<STextBlock> HeaderText = SNew(STextBlock)
			.Text(FText::FromString(GetAxisDisplayName(Settings->RelationTags[Col])))
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
				.HeightOverride(MatrixRowHeight)
				.VAlign(VAlign_Fill)
				[
					SNew(SOverlay)
					+ SOverlay::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					[
						SNew(SButton)
						.OnClicked(FOnClicked::CreateRaw(this, &FBXBehaviorSettingsCustomization::OnRemoveAxisClicked, ColIndexForHeader))
						.ToolTipText(FText::FromString(FString::Printf(TEXT("完整名: %s\n点击删除该轴(连带清除其全部关系配置)"), *Settings->RelationTags[Col].ToString())))
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

	// ── 标签列 + 单元格行(同处纵向滚动器:纵向天然同步;标签列在横向滚动器外:横向钉住不动) ──
	TSharedRef<SVerticalBox> LabelColumn = SNew(SVerticalBox);
	TSharedRef<SVerticalBox> BodyRows = SNew(SVerticalBox);

	for (int32 Row = 0; Row < AxisNum; ++Row)
	{
		// 行头(可拖拽重排序:标签/悬停高亮/插入指示线由行头控件内部组装;
		// 列头与本行头同索引,随 RelationTags 同步移动)
		TSharedRef<SBXDraggableAxisHeader> RowHeader = SNew(SBXDraggableAxisHeader, this)
			.SourceIndex(Row)
			.LabelText(FText::FromString(GetAxisDisplayName(Settings->RelationTags[Row])))
			.ToolTipText(FText::FromString(FString::Printf(TEXT("完整名: %s\n拖拽可调整轴顺序(行头与列头同步移动)"), *Settings->RelationTags[Row].ToString())));
		RowLabelWidgets.Add(Row, RowHeader->GetLabel());
		RowLabelHighlightWidgets.Add(Row, RowHeader->GetHighlight());

		LabelColumn->AddSlot()
			.AutoHeight()
			[
				RowHeader
			];

		TSharedRef<SHorizontalBox> BodyRow = SNew(SHorizontalBox);
		for (int32 Col = 0; Col < AxisNum; ++Col)
		{
			const int32 RowIndex = Row;
			const int32 ColIndex = Col;

			// 单元格文本(顶层常驻显示;悬停高亮联动染黑,常态=标准前景;HitTestInvisible 让点击穿透到按钮)
			TSharedRef<STextBlock> CellText = SNew(STextBlock)
				.Text(this, &FBXBehaviorSettingsCustomization::GetCellText, RowIndex, ColIndex)
				.Font(GridFont)
				.ColorAndOpacity(FSlateColor::UseForeground())
				.Visibility(EVisibility::HitTestInvisible);
			CellTextWidgets.Add((uint64(RowIndex) << 32) | uint32(ColIndex), CellText);

			// 单元格按钮(悬停回调联动行头/列头/本格高亮;底色按关系着色:空=默认,禁用=蓝,中断=红,禁+中=紫)
			TSharedRef<SButton> CellButton = SNew(SButton)
				.ButtonStyle(GetCellButtonStyle(GetCellRelation(RowIndex, ColIndex)))
				.OnClicked(FOnClicked::CreateRaw(this, &FBXBehaviorSettingsCustomization::OnCellClicked, RowIndex, ColIndex))
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
					.HeightOverride(MatrixRowHeight)
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
				.HeightOverride(MatrixRowHeight)
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
				// 5.8 无 SetRenderTranslate,经 SetRenderTransform 平移(冻结表头条跟随横向滚动)
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
				.Text(LOCTEXT("AddAxis", "+ 添加矩阵轴"))
				.OnClicked(FOnClicked::CreateRaw(this, &FBXBehaviorSettingsCustomization::OnAddAxisClicked))
				.ToolTipText(LOCTEXT("AddAxisTip", "弹出GameplayTag选择器,仅列出 BXBehavior.* 行为族Tag(如 BXBehavior.Dodge)"))
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			.Padding(12.0f, 0.0f)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("AxisHint", "拖拽行头调整轴顺序(行列同步);点击列头删除该轴;单元格点击循环:空→禁用→中断→禁用并中断(对角线=同行为自关系)"))
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
	if (AxisNum >= 10)
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

void FBXBehaviorSettingsCustomization::Commit(UBXBehaviorSettings* InSettings)
{
	if (!InSettings)
	{
		return;
	}

	// 变更落盘+重建运行时索引(轻量,单元格点击高频路径;不动视图)
	// 行为关系配置写入插件 Config 目录(随插件分发):SaveConfig 默认落点是项目 Config/DefaultBattleX.ini,
	// 与插件层副本形成两份漂移;显式传 Filename 落插件文件(读取侧 PostInitProperties 亦直读该文件)
	const TCHAR* SectionName = TEXT("/Script/BattleX.BXBehaviorSettings");
	const FString PluginIniPath = UBXBehaviorSettings::GetPluginConfigIniPath();
	if (!PluginIniPath.IsEmpty())
	{
		InSettings->SaveConfig(CPF_Config, *PluginIniPath);
		GConfig->Flush(false, *PluginIniPath);

		// 迁移清理:默认落点(项目 DefaultBattleX.ini)若残留本类旧节,清掉并落盘——项目层同节会以更高优先级遮蔽插件层
		const FString ProjectIniPath = GConfig->GetConfigFilename(TEXT("BattleX"));
		if (!ProjectIniPath.Equals(PluginIniPath) && GConfig->DoesSectionExist(SectionName, ProjectIniPath))
		{
			GConfig->EmptySection(SectionName, ProjectIniPath);
			GConfig->Flush(false, ProjectIniPath);
		}
	}
	else
	{
		InSettings->SaveConfig();
	}

	InSettings->RebuildRelationIndex();
}

void FBXBehaviorSettingsCustomization::RebuildMatrixGrid()
{
	// 增删轴后只换网格本体(SBox SetContent),不走ForceRefreshDetails——
	// 整视图重建要重跑全部属性反射枚举+定制+设置页重排,是设置页卡顿根源
	if (MatrixContainer.IsValid())
	{
		MatrixContainer->SetContent(MakeMatrixWidget());
	}
}

void FBXBehaviorSettingsCustomization::MoveAxis(int32 InFromIndex, int32 InInsertSlot)
{
	UBXBehaviorSettings* Settings = GetSettings();
	if (!Settings || !Settings->RelationTags.IsValidIndex(InFromIndex))
	{
		return;
	}

	// 插入槽位钳到合法区间[0,Num];源自身两侧(原位前/原位后)=位置未变,不动配置不落盘
	const int32 InsertSlot = FMath::Clamp(InInsertSlot, 0, Settings->RelationTags.Num());
	if (InFromIndex == InsertSlot || InFromIndex + 1 == InsertSlot)
	{
		return;
	}

	// 原数组槽位换算到移除后的插入下标:源在槽位左侧时,槽位前元素整体左移一位;
	// 列头与行头共用本数组,移动后行列两轴同步;关系配置按Tag键存储,与顺序无关零迁移
	const FGameplayTag MovedTag = Settings->RelationTags[InFromIndex];
	Settings->RelationTags.RemoveAt(InFromIndex);
	Settings->RelationTags.Insert(MovedTag, (InFromIndex < InsertSlot) ? InsertSlot - 1 : InsertSlot);

	Commit(Settings);
	RebuildMatrixGrid();
}

void FBXBehaviorSettingsCustomization::HandleCellHovered(int32 InRowIndex, int32 InColumnIndex)
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

void FBXBehaviorSettingsCustomization::HandleCellUnhovered(int32 InRowIndex, int32 InColumnIndex)
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

FReply FBXBehaviorSettingsCustomization::OnAddAxisClicked()
{
	UBXBehaviorSettings* Settings = GetSettings();
	if (!Settings)
	{
		return FReply::Unhandled();
	}

	// 弹出独立窗口内的Tag选择器(SGameplayTagCombo,选择后经OnAxisTagSelected落轴)
	TSharedRef<SWindow> PickerWindow = SNew(SWindow)
		.Title(LOCTEXT("AddAxisWindowTitle", "选择矩阵轴(行为/族Tag)"))
		.SizingRule(ESizingRule::Autosized)
		.AutoCenter(EAutoCenter::PrimaryWorkArea);

	TWeakObjectPtr<UBXBehaviorSettings> WeakSettings = Settings;
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
				.Text(LOCTEXT("AddAxisPrompt", "选择加入矩阵轴的行为Tag(仅列出 BXBehavior.* 行为族,如 BXBehavior.Dodge)"))
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SGameplayTagCombo)
				// 只列 BXBehavior.* 行为族(Filter 为根名串,引擎经 GetFilteredGameplayRootTags 裁剪树根;
				// 5.8 的原生Tag常量是 FNativeGameplayTag 包装,经 GetTag() 取 FGameplayTag)
				.Filter(BXGameplayTags::BXBehavior_Root.GetTag().ToString())
				.OnTagChanged_Lambda([this, WeakSettings, WeakWindow](const FGameplayTag& SelectedTag)
				{
					if (!SelectedTag.IsValid())
					{
						return;
					}

					if (UBXBehaviorSettings* SettingsPtr = WeakSettings.Get())
					{
						SettingsPtr->RelationTags.AddUnique(SelectedTag);
						Commit(SettingsPtr);
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

void FBXBehaviorSettingsCustomization::OnAxisTagSelected(const FGameplayTag& InTag)
{
	// lambda路径直接处理,保留空实现以满足声明(未来需要非模态选择器时启用)
}

FReply FBXBehaviorSettingsCustomization::OnRemoveAxisClicked(int32 InAxisIndex)
{
	UBXBehaviorSettings* Settings = GetSettings();
	if (!Settings || !Settings->RelationTags.IsValidIndex(InAxisIndex))
	{
		return FReply::Unhandled();
	}

	const FGameplayTag AxisTag = Settings->RelationTags[InAxisIndex];

	// 清除该轴的全部关系配置(作为行键的两表条目 + 作为列在各行容器中的引用)
	Settings->ExpelRelations.Remove(AxisTag);
	Settings->RejectRelations.Remove(AxisTag);
	for (TPair<FGameplayTag, FGameplayTagContainer>& Pair : Settings->ExpelRelations)
	{
		Pair.Value.RemoveTag(AxisTag);
	}
	for (TPair<FGameplayTag, FGameplayTagContainer>& Pair : Settings->RejectRelations)
	{
		Pair.Value.RemoveTag(AxisTag);
	}

	// 移除轴本体并收尾(只换网格本体,不重建Details整视图)
	Settings->RelationTags.RemoveAt(InAxisIndex);
	Commit(Settings);
	RebuildMatrixGrid();

	return FReply::Handled();
}

FReply FBXBehaviorSettingsCustomization::OnCellClicked(int32 InRowIndex, int32 InColumnIndex)
{
	UBXBehaviorSettings* Settings = GetSettings();
	if (!Settings || !Settings->RelationTags.IsValidIndex(InRowIndex) || !Settings->RelationTags.IsValidIndex(InColumnIndex))
	{
		return FReply::Unhandled();
	}

	const FGameplayTag RowTag = Settings->RelationTags[InRowIndex];
	const FGameplayTag ColTag = Settings->RelationTags[InColumnIndex];
	if (!RowTag.IsValid() || !ColTag.IsValid())
	{
		return FReply::Unhandled();
	}

	// 四态循环:空(天然共存)→禁用→中断→禁用并中断→空
	// (位0=中断[ExpelRelations],位1=禁用[RejectRelations];空格不是关系,是关系的缺席=天然共存;
	//  对角线自关系同等可配:自禁用挡同Tag重入,自中断=新实例顶掉旧实例)
	static constexpr uint8 NextOf[4] = { 2, 3, 1, 0 };
	const uint8 Current = GetCellRelation(InRowIndex, InColumnIndex);
	const uint8 Next = NextOf[Current];

	// 清除旧关系
	if (FGameplayTagContainer* Container = Settings->ExpelRelations.Find(RowTag))
	{
		Container->RemoveTag(ColTag);
		if (Container->IsEmpty())
		{
			Settings->ExpelRelations.Remove(RowTag);
		}
	}
	if (FGameplayTagContainer* Container = Settings->RejectRelations.Find(RowTag))
	{
		Container->RemoveTag(ColTag);
		if (Container->IsEmpty())
		{
			Settings->RejectRelations.Remove(RowTag);
		}
	}

	// 写入新关系(禁用与中断两轴独立,可同格并存)
	if (Next == 1 || Next == 3)
	{
		Settings->ExpelRelations.FindOrAdd(RowTag).AddTag(ColTag);
	}
	if (Next == 2 || Next == 3)
	{
		Settings->RejectRelations.FindOrAdd(RowTag).AddTag(ColTag);
	}

	Commit(Settings);

	// 直改单元格文本(不走ForceRefreshDetails:整视图重建是矩阵卡顿根源)
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

uint8 FBXBehaviorSettingsCustomization::GetCellRelation(int32 InRowIndex, int32 InColumnIndex) const
{
	UBXBehaviorSettings* Settings = GetSettings();
	if (!Settings)
	{
		return 0;
	}

	if (!Settings->RelationTags.IsValidIndex(InRowIndex) || !Settings->RelationTags.IsValidIndex(InColumnIndex))
	{
		return 0;
	}

	const FGameplayTag& RowTag = Settings->RelationTags[InRowIndex];
	const FGameplayTag& ColTag = Settings->RelationTags[InColumnIndex];

	// 两轴独立读取,同格配置返回禁用+中断
	uint8 Relation = 0;

	if (const FGameplayTagContainer* Container = Settings->ExpelRelations.Find(RowTag))
	{
		if (Container->HasTagExact(ColTag))
		{
			Relation |= 1;
		}
	}

	if (const FGameplayTagContainer* Container = Settings->RejectRelations.Find(RowTag))
	{
		if (Container->HasTagExact(ColTag))
		{
			Relation |= 2;
		}
	}

	return Relation;
}

FText FBXBehaviorSettingsCustomization::GetCellText(int32 InRowIndex, int32 InColumnIndex) const
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

const FButtonStyle* FBXBehaviorSettingsCustomization::GetCellButtonStyle(uint8 InRelation)
{
	// 空=默认按钮样式
	if (InRelation == 0)
	{
		return &FCoreStyle::Get().GetWidgetStyle<FButtonStyle>("Button");
	}

	// 着色样式副本按关系缓存(样式指针被 SButton 引用,须随定制实例存活)
	if (const TSharedPtr<FButtonStyle>* Cached = CellButtonStyles.Find(InRelation))
	{
		return Cached->Get();
	}

	// 禁用=重蓝,中断=重红,禁+中=重紫(更深更重;按钮文字为白色,底色必须压暗保证对比度);以默认按钮样式为底,四态画刷统一着色
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
