#include "SBXTLAssetTab.h"
#include "EditorStyleSet.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SSpacer.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Layout/SSeparator.h"
#include "Widgets/Layout/SExpandableArea.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Text/SInlineEditableTextBlock.h"

#include "STimeline.h"
#include "BXTLEditor.h"
#include "BXTLEditorUtilities.h"
#include "Preview/BXTLPreviewProxy.h"
#include "Timeline/BXTLController.h"

#include "BXTLAsset.h" 



#define LOCTEXT_NAMESPACE "SBXTLAssetTimelineTab"

void SBXTLAssetSection::Construct(const FArguments& InArgs, const TSharedPtr<FBXTLEditor>& InAssetEditorToolkit, const TSharedPtr<SBXTLAssetTimelineTab>& AssetEditWidget, int32 InSectionIndex)
{
	CachedEditor = InAssetEditorToolkit;
	CachedEditTab = AssetEditWidget;

	// 创建时间轴
	TimelineController = MakeShared<FBXTLController>();
	TimelineController->Initialize(CachedEditor.Pin(), CachedEditor.Pin()->GetPreviewProxy(), InSectionIndex);
	TimelineWidget = SNew(STimeline, TimelineController.ToSharedRef());

	TSharedPtr<SBox> OutBorder;
	TSharedPtr<SBorder> OutOuterBorder;
	TSharedPtr<SHorizontalBox> OutInnerHorizontalBox;

	this->ChildSlot
	[
		SNew(SExpandableArea)
		.HeaderContent()
		[
			SAssignNew(OutBorder, SBox)
			.Padding(2.0f)
			[
				SAssignNew(OutInnerHorizontalBox, SHorizontalBox)
				+ SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				.HAlign(HAlign_Left)
				.Padding(2.0f, 1.0f)
				.AutoWidth()
				[
					SNew(SInlineEditableTextBlock)
					.IsReadOnly(true)
					.Text(this, &SBXTLAssetSection::GetSectionName)
					.IsSelected(FIsSelected::CreateLambda([]() { return false; }))
					.OnTextCommitted(this, &SBXTLAssetSection::ChangeSectionName)
				]

				+ SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				.HAlign(HAlign_Left)
				.Padding(10.0f, 1.0f, 2.0f, 1.0f)
				.AutoWidth()
				[
					SNew(STextBlock)
					.Text(FText::FromString(TEXT("LoopTime:")))
				]

				+ SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				.HAlign(HAlign_Left)
				.Padding(1.0f, 1.0f)
				.AutoWidth()
				[
					SNew(SEditableTextBox)
					.Text(this, &SBXTLAssetSection::GetSectionLoopTime)
					.OnTextCommitted(this, &SBXTLAssetSection::ChangeSectionLoopTime)
				]

				+ SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				.HAlign(HAlign_Left)
				.Padding(10.0f, 1.0f, 2.0f, 1.0f)
				.AutoWidth()
				[
					SNew(STextBlock)
					.Text(FText::FromString(TEXT("Duration:")))
				]

				+ SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				.HAlign(HAlign_Left)
				.Padding(1.0f, 1.0f)
				.AutoWidth()
				[
					SNew(SEditableTextBox)
					.Text(this, &SBXTLAssetSection::GetSectionDuration)
					.OnTextCommitted(this, &SBXTLAssetSection::ChangeSectionDuration)
				]
			]
		]
		.BodyContent()
		[
			SNew(SBox)
			.MinDesiredHeight(370)
			.Padding(FMargin(0, 2, 0, 2))
			[
				TimelineWidget.ToSharedRef()
			]
		]
	];
}

void SBXTLAssetSection::ChangeSectionName(const FText& InText, ETextCommit::Type CommitInfo)
{
	if (InText.IsEmpty())
	{
		return;
	}

	if (UBXTLAsset* Asset = CachedEditor.Pin()->GetEditingAsset())
	{
		Asset->ChangeSectionName(TimelineController->GetSectionID(), InText);
	}
}

void SBXTLAssetSection::ChangeSectionLoopTime(const FText& InText, ETextCommit::Type CommitInfo)
{
	if (InText.IsEmpty())
	{
		return;
	}

	if (UBXTLAsset* Asset = CachedEditor.Pin()->GetEditingAsset())
	{
		Asset->ChangeSectionLoopTime(TimelineController->GetSectionID(), FCString::Atoi(*InText.ToString()));
	}
}

void SBXTLAssetSection::ChangeSectionDuration(const FText& InText, ETextCommit::Type CommitInfo)
{
	if (InText.IsEmpty())
	{
		return;
	}

	if (UBXTLAsset* Asset = CachedEditor.Pin()->GetEditingAsset())
	{
		Asset->ChangeSectionDuration(TimelineController->GetSectionID(), FCString::Atof(*InText.ToString()));
	}
}

FText SBXTLAssetSection::GetSectionName() const
{
	if (UBXTLAsset* Asset = CachedEditor.Pin()->GetEditingAsset())
	{
		int32 Index = TimelineController->GetSectionID();
		if (Asset->Sections.IsValidIndex(Index))
		{
			return FText::FromString(Asset->Sections[Index].Name.ToString());
		}
	}

	return FText::FromString(TEXT("None"));
}

FText SBXTLAssetSection::GetSectionLoopTime() const
{
	if (UBXTLAsset* Asset = CachedEditor.Pin()->GetEditingAsset())
	{
		int32 Index = TimelineController->GetSectionID();
		if (Asset->Sections.IsValidIndex(Index))
		{
			return FText::FromString(FString::FromInt(Asset->Sections[Index].LoopTime));
		}
	}

	return FText::FromString(TEXT("1"));
}

FText SBXTLAssetSection::GetSectionDuration() const
{
	if (UBXTLAsset* Asset = CachedEditor.Pin()->GetEditingAsset())
	{
		int32 Index = TimelineController->GetSectionID();
		if (Asset->Sections.IsValidIndex(Index))
		{
			return FText::FromString(FString::SanitizeFloat(Asset->Sections[Index].Duration));
		}
	}

	return FText::FromString(TEXT("1"));
}






void SBXTLAssetTimelineTab::Construct(const FArguments& InArgs, const TSharedPtr<FBXTLEditor>& InEditor)
{
	CachedEditor = InEditor;
	if (CachedEditor.IsValid())
	{
		CachedEditor.Pin()->RefreshPanelEvent.AddRaw(this, &SBXTLAssetTimelineTab::OnRefreshPanel);
	}

	VerticalBox = SNew(SVerticalBox).Visibility(EVisibility::SelfHitTestInvisible)
	+ SVerticalBox::Slot()
	.AutoHeight()
	[
		SAssignNew(HorizontalBox, SHorizontalBox).Visibility(EVisibility::SelfHitTestInvisible)
	]
	+ SVerticalBox::Slot()
	.FillHeight(1.0f)
	[
		SAssignNew(ScrollBox, SScrollBox).Orientation(Orient_Vertical)
	];

	SectionMenu = FBXTLEditorUtilities::MakeTrackButton(LOCTEXT("BXSection", "Section Menu"), FOnGetContent::CreateSP(this, &SBXTLAssetTimelineTab::BuildSectionSubMenu));

	HorizontalBox->AddSlot()
	.AutoWidth()
	.VAlign(VAlign_Center)
	.HAlign(HAlign_Left)
	.Padding(FMargin(10.0f, 0.0f, 0.0f, 0.0f))
	[
		SectionMenu.ToSharedRef()
	];

	this->ChildSlot[VerticalBox.ToSharedRef()];

	ResetSections();
}

SBXTLAssetTimelineTab::~SBXTLAssetTimelineTab()
{
	if (CachedEditor.IsValid())
	{
		CachedEditor.Pin()->RefreshPanelEvent.RemoveAll(this);
	}
}

void SBXTLAssetTimelineTab::ResetSections()
{
	if (!CachedEditor.IsValid())
	{
		return;
	}

	UBXTLAsset* Asset = CachedEditor.Pin()->GetEditingAsset();
	if (!Asset)
	{
		return;
	}

	HorizontalBox->ClearChildren();
	SectionWidgets.Empty();
	
	HorizontalBox->AddSlot()
	.AutoWidth()
	.VAlign(VAlign_Center)
	.HAlign(HAlign_Left)
	.Padding(FMargin(10.0f, 0.0f))
	[
		SectionMenu.ToSharedRef()
	];

	for (int32 i = 0; i < Asset->Sections.Num(); i++)
	{
		const FBXTLSection& Section = Asset->Sections[i];

		// 创建时间轴选取按钮
		if (HorizontalBox.IsValid())
		{
			HorizontalBox->AddSlot()
			.VAlign(VAlign_Center)
			.HAlign(HAlign_Left)
			.AutoWidth()
			[
				SNew(SBox)
				.Padding(FMargin(10.0f, 0.0f, 0.0f, 0.0f))
				[
					SNew(SCheckBox)
					.Style(FAppStyle::Get(), "DetailsView.SectionButton")
					.OnCheckStateChanged(this, &SBXTLAssetTimelineTab::OnSectionCheckedChanged, i)
					.IsChecked(this, &SBXTLAssetTimelineTab::IsSectionChecked, i)
					[
						SNew(STextBlock)
						.TextStyle(FAppStyle::Get(), "SmallText")
						.Text(Section.Name)
					]
				]
			];
		}

		// 创建时间轴
		TSharedPtr<SBXTLAssetSection> NewSection = SNew(SBXTLAssetSection, CachedEditor.Pin(), SharedThis(this), i);
		SectionWidgets.Add(NewSection);
	}

	OnRefreshPanel();
}

void SBXTLAssetTimelineTab::OnRefreshPanel()
{
	ScrollBox->ClearChildren();

	if (CachedEditor.IsValid())
	{
		CachedEditor.Pin()->GetSectionsToShow(HelpArray);

		for (int32 i = 0; i < SectionWidgets.Num(); i++)
		{
			if (HelpArray.Contains(i))
			{
				ScrollBox->AddSlot()[SectionWidgets[i].ToSharedRef()];
			}
		}
	}
}

FReply SBXTLAssetTimelineTab::OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == EKeys::LeftControl)
	{
		for (int32 i = 0; i < SectionWidgets.Num(); ++i)
		{
			if (FBXTLController* TC = static_cast<FBXTLController*>(SectionWidgets[i]->TimelineController.Get()))
			{
				TC->ChangeCtrlState(true);
			}
		}
	}

	return FReply::Unhandled();
}

FReply SBXTLAssetTimelineTab::OnKeyUp(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == EKeys::LeftControl)
	{
		for (int32 i = 0; i < SectionWidgets.Num(); ++i)
		{
			if (FBXTLController* TC = static_cast<FBXTLController*>(SectionWidgets[i]->TimelineController.Get()))
			{
				TC->ChangeCtrlState(false);
			}
		}
	}

	return FReply::Unhandled();
}

void SBXTLAssetTimelineTab::OnSectionCheckedChanged(ECheckBoxState State, int32 Index)
{
	if (State == ECheckBoxState::Unchecked)
	{
		if (CachedEditor.IsValid())
		{
			CachedEditor.Pin()->SetEditSectionsToShow(Index, false);
		}
	}
	else if (State == ECheckBoxState::Checked)
	{
		if (CachedEditor.IsValid())
		{
			CachedEditor.Pin()->SetEditSectionsToShow(Index, true);
		}
	}
}

ECheckBoxState SBXTLAssetTimelineTab::IsSectionChecked(int32 Index) const
{
	if (CachedEditor.IsValid())
	{
		TArray<int32> SectionArray;
		CachedEditor.Pin()->GetEditSectionsToShow(SectionArray);

		if (SectionArray.Contains(Index))
		{
			return ECheckBoxState::Checked;
		}
	}

	return ECheckBoxState::Unchecked;
}

void SBXTLAssetTimelineTab::AddSection()
{
	if (!CachedEditor.Pin()->IsStopped())
	{
		return;
	}

	if (UBXTLAsset* Asset = CachedEditor.Pin()->GetEditingAsset())
	{
		Asset->AddSection(FText());
		ResetSections();
	}
}

void SBXTLAssetTimelineTab::DeleteSection(int32 Index)
{
	if (!CachedEditor.Pin()->IsStopped())
	{
		return;
	}

	if (UBXTLAsset* Asset = CachedEditor.Pin()->GetEditingAsset())
	{
		Asset->RemoveSection(Index);
		ResetSections();
	}
}

TSharedRef<SWidget> SBXTLAssetTimelineTab::BuildSectionSubMenu()
{
	FMenuBuilder MenuBuilder(true, nullptr);

	MenuBuilder.BeginSection("Section Actions", LOCTEXT("MenuSectionS", "Section"));
	{
		MenuBuilder.AddMenuEntry
		(
			LOCTEXT("NewSection_Lable", "Add New"),
			LOCTEXT("NewSection_Tooltip", "Add A New Section"),
			FSlateIcon(),
			FUIAction(FExecuteAction::CreateSP(this, &SBXTLAssetTimelineTab::AddSection)),
			NAME_None,
			EUserInterfaceActionType::Button
		);

		if (UBXTLAsset* Asset = CachedEditor.Pin()->GetEditingAsset())
		{
			for (int32 i = 0; i < Asset->Sections.Num(); i++)
			{
				FText SubMenuName = FText::FromString(TEXT("Delete: ") + Asset->Sections[i].Name.ToString());

				MenuBuilder.AddMenuEntry
				(
					SubMenuName,
					SubMenuName,
					FSlateIcon(),
					FUIAction(FExecuteAction::CreateLambda([i, this]() { this->DeleteSection(i); })),
					NAME_None,
					EUserInterfaceActionType::Button
				);
			}
		}
	}
	MenuBuilder.EndSection();

	return MenuBuilder.MakeWidget();
}






void SBXTLAssetDetailTab::Construct(const FArguments& InArgs, TSharedPtr<FBXTLEditor> InEditor)
{
	CachedEditor = InEditor;

	SSingleObjectDetailsPanel::Construct(SSingleObjectDetailsPanel::FArguments().HostCommandList(CachedEditor.Pin()->GetToolkitCommands()), true, true);
}

UObject* SBXTLAssetDetailTab::GetObjectToObserve() const
{
	return Cast<UObject>(CachedEditor.Pin()->GetEditingAsset());
}

TSharedRef<SWidget> SBXTLAssetDetailTab::PopulateSlot(TSharedRef<SWidget> PropertyEditorWidget)
{
	return SNew(SVerticalBox)
	+ SVerticalBox::Slot()
	.AutoHeight()
	[
		SNew(SBorder)
		.BorderImage(FAppStyle::GetBrush(TEXT("Graph.TitleBackground")))
		.HAlign(HAlign_Center)
		.Visibility(this, &SBXTLAssetDetailTab::GetAssetDisplayNameVisibility)
	]
	+ SVerticalBox::Slot()
	.FillHeight(1)
	[
		PropertyEditorWidget
	];
}

EVisibility SBXTLAssetDetailTab::GetAssetDisplayNameVisibility() const
{
	return (GetObjectToObserve() != NULL) ? EVisibility::Hidden : EVisibility::Collapsed;
}

FText SBXTLAssetDetailTab::GetAssetDisplayName() const
{
	return LOCTEXT("BXTLAssetDetail", "Asset Properties");
}

#undef LOCTEXT_NAMESPACE