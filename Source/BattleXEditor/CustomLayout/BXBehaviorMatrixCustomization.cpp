#include "BXBehaviorMatrixCustomization.h"

#include "Behavior/BXBehaviorSettings.h"

#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "DetailWidgetRow.h"
#include "IDetailsView.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/SWindow.h"
#include "Widgets/Layout/SBorder.h"
#include "Framework/Application/SlateApplication.h"
#include "GameplayTagContainer.h"
#include "SGameplayTagCombo.h"
#include "Styling/CoreStyle.h"

#define LOCTEXT_NAMESPACE "BXBehaviorMatrix"

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

	// 缓存视图引用(矩阵结构变化后ForceRefreshDetails重建定制)
	CachedDetailBuilder = &InDetailBuilder;

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
			MakeMatrixWidget()
		];
}

UBXBehaviorSettings* FBXBehaviorSettingsCustomization::GetSettings() const
{
	return CachedSettings.Get();
}

TSharedRef<SWidget> FBXBehaviorSettingsCustomization::MakeMatrixWidget()
{
	TSharedRef<SVerticalBox> MatrixBox = SNew(SVerticalBox);

	UBXBehaviorSettings* Settings = GetSettings();
	if (!Settings)
	{
		return MatrixBox;
	}

	const int32 AxisNum = Settings->RelationTags.Num();

	// 轴编辑行:添加轴按钮(Tag选择器下拉)+说明文本(空网格时不至于无从下手)
	TSharedRef<SHorizontalBox> AxisEditRow = SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.VAlign(VAlign_Center)
		[
			SNew(SButton)
			.Text(LOCTEXT("AddAxis", "+ 添加矩阵轴"))
			.OnClicked(FOnClicked::CreateRaw(this, &FBXBehaviorSettingsCustomization::OnAddAxisClicked))
			.ToolTipText(LOCTEXT("AddAxisTip", "弹出GameplayTag选择器,选择行为/族Tag加入矩阵轴(如 BXBehavior.Locomotion)"))
		]
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.VAlign(VAlign_Center)
		.Padding(12.0f, 0.0f)
		[
			SNew(STextBlock)
			.Text(LOCTEXT("AxisHint", "点击表头轴名可删除该轴;单元格点击循环:空→接管→禁止→禁止+接管"))
			.Font(FCoreStyle::GetDefaultFontStyle("Regular", 9))
			.ColorAndOpacity(FSlateColor::UseSubduedForeground())
		];
	MatrixBox->AddSlot()
		.AutoHeight()
		.Padding(0.0f, 0.0f, 0.0f, 6.0f)
		[
			AxisEditRow
		];

	// 表头行(列Tag名;点击轴名删除该轴)
	TSharedRef<SHorizontalBox> HeaderRow = SNew(SHorizontalBox);
	HeaderRow->AddSlot()
		.AutoWidth()
		[
			SNew(SBox)
			.WidthOverride(140.0f)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("HeaderCorner", "行＼列"))
			]
		];
	for (int32 Col = 0; Col < AxisNum; ++Col)
	{
		const int32 ColIndexForHeader = Col;

		HeaderRow->AddSlot()
			.AutoWidth()
			[
				SNew(SBox)
				.WidthOverride(90.0f)
				[
					SNew(SButton)
					.OnClicked(FOnClicked::CreateRaw(this, &FBXBehaviorSettingsCustomization::OnRemoveAxisClicked, ColIndexForHeader))
					.ToolTipText(LOCTEXT("RemoveAxisTip", "点击删除该轴(连带清除其全部关系配置)"))
					.Content()
					[
						SNew(STextBlock)
						.Text(FText::FromString(Settings->RelationTags[Col].ToString()))
						.ColorAndOpacity(FSlateColor::UseSubduedForeground())
					]
				]
			];
	}
	MatrixBox->AddSlot()
		.AutoHeight()
		[
			HeaderRow
		];

	// 数据行
	for (int32 Row = 0; Row < AxisNum; ++Row)
	{
		TSharedRef<SHorizontalBox> RowBox = SNew(SHorizontalBox);

		// 行Tag名
		RowBox->AddSlot()
			.AutoWidth()
			[
				SNew(SBox)
				.WidthOverride(140.0f)
				[
					SNew(STextBlock)
					.Text(FText::FromString(Settings->RelationTags[Row].ToString()))
				]
			];

		// 单元格
		for (int32 Col = 0; Col < AxisNum; ++Col)
		{
			const int32 RowIndex = Row;
			const int32 ColIndex = Col;

			RowBox->AddSlot()
				.AutoWidth()
				[
					SNew(SBox)
					.WidthOverride(90.0f)
					[
						SNew(SButton)
						.IsEnabled(!IsDiagonal(RowIndex, ColIndex))
						.OnClicked(FOnClicked::CreateRaw(this, &FBXBehaviorSettingsCustomization::OnCellClicked, RowIndex, ColIndex))
						.Content()
						[
							SNew(STextBlock)
							.Text(this, &FBXBehaviorSettingsCustomization::GetCellText, RowIndex, ColIndex)
						]
					]
				];
		}

		MatrixBox->AddSlot()
			.AutoHeight()
			[
				RowBox
			];
	}

	return MatrixBox;
}

void FBXBehaviorSettingsCustomization::CommitAndRefresh(UBXBehaviorSettings* InSettings)
{
	if (!InSettings)
	{
		return;
	}

	// 变更收尾:保存配置+重建运行时索引+强制刷新视图(矩阵网格是构建期一次性生成,增删轴/改关系必须重建定制才会反映)
	InSettings->SaveConfig();
	InSettings->RebuildRelationIndex();
	if (CachedDetailBuilder)
	{
		CachedDetailBuilder->ForceRefreshDetails();
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
				.Text(LOCTEXT("AddAxisPrompt", "选择加入矩阵轴的Tag(支持族Tag,如 BXBehavior.Locomotion)"))
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SGameplayTagCombo)
				.OnTagChanged_Lambda([this, WeakSettings, WeakWindow](const FGameplayTag& SelectedTag)
				{
					if (!SelectedTag.IsValid())
					{
						return;
					}

					if (UBXBehaviorSettings* SettingsPtr = WeakSettings.Get())
					{
						SettingsPtr->RelationTags.AddUnique(SelectedTag);
						CommitAndRefresh(SettingsPtr);
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

	// 移除轴本体并收尾
	Settings->RelationTags.RemoveAt(InAxisIndex);
	CommitAndRefresh(Settings);

	return FReply::Handled();
}

FReply FBXBehaviorSettingsCustomization::OnCellClicked(int32 InRowIndex, int32 InColumnIndex)
{
	UBXBehaviorSettings* Settings = GetSettings();
	if (!Settings || IsDiagonal(InRowIndex, InColumnIndex))
	{
		return FReply::Unhandled();
	}

	if (!Settings->RelationTags.IsValidIndex(InRowIndex) || !Settings->RelationTags.IsValidIndex(InColumnIndex))
	{
		return FReply::Unhandled();
	}

	const FGameplayTag RowTag = Settings->RelationTags[InRowIndex];
	const FGameplayTag ColTag = Settings->RelationTags[InColumnIndex];
	if (!RowTag.IsValid() || !ColTag.IsValid())
	{
		return FReply::Unhandled();
	}

	// 四态循环:空(天然共存)→接管→禁止→禁止+接管→空
	// ("并存"不是一种关系,是关系的缺席=空单元格;禁止+接管=两张表同格配置,取消窗口标准配法)
	const uint8 Current = GetCellRelation(InRowIndex, InColumnIndex);
	const uint8 Next = (Current + 1) % 4;

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

	// 写入新关系(禁止与接管两轴独立,可同格并存)
	if (Next == 1 || Next == 3)
	{
		Settings->ExpelRelations.FindOrAdd(RowTag).AddTag(ColTag);
	}
	if (Next == 2 || Next == 3)
	{
		Settings->RejectRelations.FindOrAdd(RowTag).AddTag(ColTag);
	}

	CommitAndRefresh(Settings);

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

	// 两轴独立读取,同格配置返回禁止+接管
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
	if (IsDiagonal(InRowIndex, InColumnIndex))
	{
		return LOCTEXT("Diagonal", "—");
	}

	const uint8 Relation = GetCellRelation(InRowIndex, InColumnIndex);
	switch (Relation)
	{
	case 1:
		return LOCTEXT("Expel", "接管");
	case 2:
		return LOCTEXT("Forbid", "禁止");
	case 3:
		return LOCTEXT("ForbidExpel", "禁止+接管");
	default:
		return LOCTEXT("Empty", "空");
	}
}

bool FBXBehaviorSettingsCustomization::IsDiagonal(int32 InRowIndex, int32 InColumnIndex) const
{
	return InRowIndex == InColumnIndex;
}

#undef LOCTEXT_NAMESPACE
