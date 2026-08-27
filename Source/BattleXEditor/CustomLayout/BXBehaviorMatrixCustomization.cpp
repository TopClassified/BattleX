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

#define LOCTEXT_NAMESPACE "BXBehaviorMatrix"

TSharedRef<IDetailCustomization> FBXBehaviorSettingsCustomization::MakeInstance()
{
	return MakeShareable(new FBXBehaviorSettingsCustomization());
}

void FBXBehaviorSettingsCustomization::CustomizeDetails(IDetailLayoutBuilder& InDetailBuilder)
{
	// 缓存设置对象(CDO,矩阵读写直接走配置)
	// GetDetailsViewSharedPtr返回TSharedPtr需用->(旧GetDetailsView返回裸指针已于5.5弃用)
	const TArray<TWeakObjectPtr<UObject>>& Objects = InDetailBuilder.GetDetailsViewSharedPtr()->GetSelectedObjects();
	if (Objects.Num() > 0)
	{
		CachedSettings = Cast<UBXBehaviorSettings>(Objects[0].Get());
	}

	// 关系字段收编为矩阵(隐藏原始Map编辑)
	InDetailBuilder.HideCategory("Matrix");

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

	// 表头行(列Tag名)
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
		HeaderRow->AddSlot()
			.AutoWidth()
			[
				SNew(SBox)
				.WidthOverride(90.0f)
				[
					SNew(STextBlock)
					.Text(FText::FromString(Settings->RelationTags[Col].ToString()))
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

	// 三态循环:并存→挤出→拒绝→并存
	const uint8 Current = GetCellRelation(InRowIndex, InColumnIndex);
	const uint8 Next = (Current + 1) % 3;

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

	// 写入新关系
	if (Next == 1)
	{
		Settings->ExpelRelations.FindOrAdd(RowTag).AddTag(ColTag);
	}
	else if (Next == 2)
	{
		Settings->RejectRelations.FindOrAdd(RowTag).AddTag(ColTag);
	}

	// 标记配置脏(保存到ini)
	Settings->SaveConfig();

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

	if (const FGameplayTagContainer* Container = Settings->ExpelRelations.Find(RowTag))
	{
		if (Container->HasTagExact(ColTag))
		{
			return 1;
		}
	}

	if (const FGameplayTagContainer* Container = Settings->RejectRelations.Find(RowTag))
	{
		if (Container->HasTagExact(ColTag))
		{
			return 2;
		}
	}

	return 0;
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
		return LOCTEXT("Expel", "挤出");
	case 2:
		return LOCTEXT("Reject", "拒绝");
	default:
		return LOCTEXT("Coexist", "并存");
	}
}

bool FBXBehaviorSettingsCustomization::IsDiagonal(int32 InRowIndex, int32 InColumnIndex) const
{
	return InRowIndex == InColumnIndex;
}

#undef LOCTEXT_NAMESPACE
