#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Text/STextBlock.h"

#include "BXTLEditorDelegates.h"



DECLARE_DELEGATE_OneParam(FBXTLPickTaskGroup, FName);



// TaskGroup选择器
class SBXTLTaskGroupPicker : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SBXTLTaskGroupPicker)
	{

	}
	SLATE_ARGUMENT(TArray<TSharedPtr<FName>>, AllTemplateNames)
	SLATE_EVENT(FBXTLPickTaskGroup, PickTaskGroupEvent)
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
			PickTaskGroupEvent.ExecuteIfBound(SelectedName);
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

	FBXTLPickTaskGroup PickTaskGroupEvent;
};