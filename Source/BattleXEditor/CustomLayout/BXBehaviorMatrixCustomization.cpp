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
#include "Widgets/Layout/SGridPanel.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/SWindow.h"
#include "Widgets/Layout/SBorder.h"
#include "Framework/Application/SlateApplication.h"
#include "GameplayTagContainer.h"
#include "SGameplayTagCombo.h"
#include "Styling/CoreStyle.h"
#include "Interfaces/IPluginManager.h"
#include "Misc/ConfigCacheIni.h"

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

	UBXBehaviorSettings* Settings = GetSettings();
	if (!Settings)
	{
		return SNullWidget::NullWidget;
	}

	const int32 AxisNum = Settings->RelationTags.Num();

	// 网格面板按内容自适应列宽:行/列轴名完整显示不截断,单元格随所在列对齐
	TSharedRef<SGridPanel> Grid = SNew(SGridPanel);

	// 轴编辑行:添加轴按钮(Tag选择器下拉)+说明文本(空网格时不至于无从下手)
	Grid->AddSlot(0, 0)
		.ColumnSpan(AxisNum + 1)
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
				.Text(LOCTEXT("AxisHint", "点击表头轴名可删除该轴;单元格点击循环:空→禁用→中断→禁用并中断(对角线=同行为自关系)"))
				.Font(FCoreStyle::GetDefaultFontStyle("Regular", 9))
				.ColorAndOpacity(FSlateColor::UseSubduedForeground())
			]
		];

	// 表头行(列Tag名;点击轴名删除该轴)
	Grid->AddSlot(0, 1)
		.Padding(2.0f, 2.0f)
		[
			SNew(STextBlock)
			.Text(LOCTEXT("HeaderCorner", "行＼列"))
		];
	for (int32 Col = 0; Col < AxisNum; ++Col)
	{
		const int32 ColIndexForHeader = Col;

		Grid->AddSlot(Col + 1, 1)
			.Padding(2.0f, 2.0f)
			[
				SNew(SButton)
				.OnClicked(FOnClicked::CreateRaw(this, &FBXBehaviorSettingsCustomization::OnRemoveAxisClicked, ColIndexForHeader))
				.ToolTipText(FText::FromString(FString::Printf(TEXT("完整名: %s\n点击删除该轴(连带清除其全部关系配置)"), *Settings->RelationTags[Col].ToString())))
				.Content()
				[
					SNew(STextBlock)
					.Text(FText::FromString(GetAxisDisplayName(Settings->RelationTags[Col])))
					.ColorAndOpacity(FSlateColor::UseSubduedForeground())
				]
			];
	}

	// 数据行(含对角线:同行为自关系可配,自禁用=挡同Tag重入,自中断=新实例顶掉旧实例)
	for (int32 Row = 0; Row < AxisNum; ++Row)
	{
		Grid->AddSlot(0, Row + 2)
			.Padding(2.0f, 2.0f)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(FText::FromString(GetAxisDisplayName(Settings->RelationTags[Row])))
				.ToolTipText(FText::FromString(FString::Printf(TEXT("完整名: %s"), *Settings->RelationTags[Row].ToString())))
			];

		for (int32 Col = 0; Col < AxisNum; ++Col)
		{
			const int32 RowIndex = Row;
			const int32 ColIndex = Col;

			TSharedRef<STextBlock> CellText = SNew(STextBlock)
				.Text(this, &FBXBehaviorSettingsCustomization::GetCellText, RowIndex, ColIndex);
			CellTextWidgets.Add((uint64(RowIndex) << 32) | uint32(ColIndex), CellText);

			Grid->AddSlot(Col + 1, Row + 2)
				.Padding(2.0f, 2.0f)
				[
					SNew(SButton)
					.OnClicked(FOnClicked::CreateRaw(this, &FBXBehaviorSettingsCustomization::OnCellClicked, RowIndex, ColIndex))
					.Content()
					[
						CellText
					]
				];
		}
	}

	// 横向滚动(轴名较长时网格宽于设置页可视区)
	return SNew(SBox)
		[
			SNew(SScrollBox)
			.Orientation(Orient_Horizontal)
			+ SScrollBox::Slot()
			[
				Grid
			]
		];
}

void FBXBehaviorSettingsCustomization::Commit(UBXBehaviorSettings* InSettings)
{
	if (!InSettings)
	{
		return;
	}

	// 变更落盘+重建运行时索引(轻量,单元格点击高频路径;不动视图)
	// 行为关系配置写入插件 Config 目录(随插件分发):SaveConfig 默认落点是项目 Config/DefaultBattleX.ini,
	// 与插件层副本形成两份漂移;显式传 Filename 落插件文件(读取链经 GPluginLayers 的 PluginDefault 层覆盖)
	const TCHAR* SectionName = TEXT("/Script/BattleX.BXBehaviorSettings");
	const FString PluginIniPath = GetPluginConfigIniPath();
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

FString FBXBehaviorSettingsCustomization::GetPluginConfigIniPath()
{
	if (const TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin(TEXT("BattleX")))
	{
		return FPaths::Combine(Plugin->GetBaseDir(), TEXT("Config"), TEXT("DefaultBattleX.ini"));
	}
	return FString();
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
		return LOCTEXT("ForbidInterrupt", "禁用并中断");
	default:
		return LOCTEXT("Empty", "空");
	}
}

#undef LOCTEXT_NAMESPACE
