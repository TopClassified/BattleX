#include "SBXTLTaskGroupPicker.h"
#include "Components/VerticalBox.h"
#include "Widgets/Input/SEditableText.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Layout/SBox.h" 



void SBXTLTaskGroupPicker::Construct(const FArguments& InArgs)
{
	AllTemplateNames = InArgs._AllTemplateNames;
	PickTaskGroupEvent = InArgs._PickTaskGroupEvent;
	
	FilteredTemplateNames = AllTemplateNames;

	ChildSlot
	[
		SNew(SBox)
		.MinDesiredWidth(300.0f)
		.MaxDesiredHeight(400.0f)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.HAlign(HAlign_Center)
			.Padding(0.0f, 12.0f, 0.0f, 12.0f)
			.AutoHeight()
			[
				SNew(SEditableText)
				.HintText(FText::FromString(TEXT("搜索TaskTemplate名称")))
				.OnTextChanged(this, &SBXTLTaskGroupPicker::SetFilterName)
			]
			
			+ SVerticalBox::Slot()
			.HAlign(HAlign_Center)
			.Padding(0.0f, 0.0f, 0.0f, 12.0f)
			.AutoHeight()
			[
				SNew(SComboBox<TSharedPtr<FName>>)
				.OptionsSource(&FilteredTemplateNames)
				.MaxListHeight(400.0f)
				.OnGenerateWidget(this, &SBXTLTaskGroupPicker::OnGenerateComboWidget)
				.OnSelectionChanged(this, &SBXTLTaskGroupPicker::OnComboSelectionChanged)
				[
					SNew(STextBlock)
					.Text(this, &SBXTLTaskGroupPicker::GetSelectTaskName)
				]
			]
		]
	];
}

void SBXTLTaskGroupPicker::SetFilterName(const FText& NewSubjectName)
{
	FilteredTemplateNames.Empty();

	FilterName = FName(NewSubjectName.ToString());

	if (FilterName.IsNone())
	{
		FilteredTemplateNames.Append(AllTemplateNames);
	}
	else
	{
		for (TSharedPtr<FName> SingleName : AllTemplateNames)
		{
			if (SingleName.Get()->ToString().Contains(FilterName.ToString()))
			{
				FilteredTemplateNames.Add(SingleName);
			}
		}
	}
}