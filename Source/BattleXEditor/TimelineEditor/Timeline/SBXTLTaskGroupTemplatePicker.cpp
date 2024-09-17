#include "SBXTLTaskGroupTemplatePicker.h"
#include "Components/VerticalBox.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Input/SEditableText.h"

#include "BXTLEditorTemplate.h"



void SBXTLTaskGroupTemplatePicker::Construct(const FArguments& InArgs)
{
	AllTemplateNames = InArgs._AllTemplateNames;
	AddTemplateEvent = InArgs._AddTemplateEvent;
	
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
				.OnTextChanged(this, &SBXTLTaskGroupTemplatePicker::SetFilterName)
			]
			
			+ SVerticalBox::Slot()
			.HAlign(HAlign_Center)
			.Padding(0.0f, 0.0f, 0.0f, 12.0f)
			.AutoHeight()
			[
				SNew(SComboBox<TSharedPtr<FName>>)
				.OptionsSource(&FilteredTemplateNames)
				.MaxListHeight(400.0f)
				.OnGenerateWidget(this, &SBXTLTaskGroupTemplatePicker::OnGenerateComboWidget)
				.OnSelectionChanged(this, &SBXTLTaskGroupTemplatePicker::OnComboSelectionChanged)
				[
					SNew(STextBlock)
					.Text(this, &SBXTLTaskGroupTemplatePicker::GetSelectTaskName)
				]
			]
		]
	];
}

void SBXTLTaskGroupTemplatePicker::SetFilterName(const FText& NewSubjectName)
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