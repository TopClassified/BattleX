#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Text/STextBlock.h"



DECLARE_DELEGATE_OneParam(FAddTaskGroupTemplateDelegate, FName);



// TaskGroup选择器
class SBXTLTaskGroupTemplatePicker : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SBXTLTaskGroupTemplatePicker) {}
	SLATE_ARGUMENT(TArray<TSharedPtr<FName>>, AllTemplateNames)
	SLATE_EVENT(FAddTaskGroupTemplateDelegate, AddTemplateEvent)
	SLATE_END_ARGS()

	virtual void Construct(const FArguments& InArgs);

public:
	TSharedRef<SWidget> OnGenerateComboWidget(TSharedPtr<FName> InComboID)
	{
		if (InComboID.IsValid())
		{
			return SNew(STextBlock).Text(FText::FromName(*InComboID));
		}

		return SNew(STextBlock).Text(FText());
	}

	void OnComboSelectionChanged(TSharedPtr<FName> NewValue, ESelectInfo::Type SelectInfo)
	{
		if (NewValue.IsValid())
		{
			SelectedName = *NewValue.Get();
			AddTemplateEvent.ExecuteIfBound(SelectedName);
		}
	}

	FText GetSelectTaskName() const
	{
		return FText::FromName(SelectedName);
	}

private:
	void SetFilterName(const FText& NewSubjectName);

private:
	TArray<TSharedPtr<FName>> AllTemplateNames;

	TArray<TSharedPtr<FName>> FilteredTemplateNames;

	FName FilterName;

	FName SelectedName;

	FAddTaskGroupTemplateDelegate AddTemplateEvent;

};