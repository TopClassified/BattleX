#include "BXFunctionSelectorCustomization.h"
#include "BXStructs.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "PropertyHandle.h"
#include "Widgets/Input/SComboButton.h"
#include "Widgets/Input/SSearchBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Views/SListView.h"
#include "Widgets/Views/STableRow.h"

TSharedRef<IPropertyTypeCustomization> FBXFunctionSelectorCustomization::MakeInstance()
{
	return MakeShareable(new FBXFunctionSelectorCustomization());
}

void FBXFunctionSelectorCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> InPropertyHandle, FDetailWidgetRow& InHeaderRow, IPropertyTypeCustomizationUtils& InCustomizationUtils)
{
	PropertyHandle = InPropertyHandle;
	TargetClassHandle = InPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FBXFunctionSelector, TargetClass));
	FunctionNameHandle = InPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FBXFunctionSelector, FunctionName));

	InHeaderRow
	.NameContent()
	[
		InPropertyHandle->CreatePropertyNameWidget()
	]
	.ValueContent()
	.MinDesiredWidth(200.0f)
	[
		SAssignNew(ComboButton, SComboButton)
		.OnGetMenuContent(this, &FBXFunctionSelectorCustomization::InternalGetFunctionPickerContent)
		.ContentPadding(2.0f)
		.ButtonContent()
		[
			SNew(STextBlock)
			.Text(this, &FBXFunctionSelectorCustomization::InternalGetFunctionNameText)
		]
	];
}

void FBXFunctionSelectorCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> InPropertyHandle, IDetailChildrenBuilder& InChildBuilder, IPropertyTypeCustomizationUtils& InCustomizationUtils)
{
	if (TargetClassHandle.IsValid())
	{
		InChildBuilder.AddProperty(TargetClassHandle.ToSharedRef());
	}
}

FText FBXFunctionSelectorCustomization::InternalGetFunctionNameText() const
{
	if (FunctionNameHandle.IsValid())
	{
		FName Value = NAME_None;
		if (FunctionNameHandle->GetValue(Value) == FPropertyAccess::Success)
		{
			return FText::FromName(Value);
		}
	}
	return FText::GetEmpty();
}

void FBXFunctionSelectorCustomization::OnFunctionSelected(TSharedPtr<FName> InFunctionName, ESelectInfo::Type InSelectInfo)
{
	if (FunctionNameHandle.IsValid() && InFunctionName.IsValid())
	{
		FunctionNameHandle->SetValue(*InFunctionName);
	}

	if (ComboButton.IsValid())
	{
		ComboButton->SetIsOpen(false);
	}
}

TSharedRef<SWidget> FBXFunctionSelectorCustomization::InternalGetFunctionPickerContent()
{
	UClass* TargetClass = InternalGetTargetClass();
	AllFunctions.Empty();
	InternalCollectFunctions(TargetClass, AllFunctions);
	FilteredFunctions = AllFunctions;

	return SNew(SBox)
	.MinDesiredWidth(280.0f)
	.MaxDesiredHeight(350.0f)
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0.0f, 0.0f, 0.0f, 4.0f)
		[
			SNew(SSearchBox)
			.OnTextChanged(this, &FBXFunctionSelectorCustomization::OnFilterTextChanged)
		]
		+ SVerticalBox::Slot()
		.FillHeight(1.0f)
		[
			SNew(SBorder)
			[
				SNew(SListView<TSharedPtr<FName>>)
				.ListItemsSource(&FilteredFunctions)
				.OnGenerateRow(this, &FBXFunctionSelectorCustomization::OnGenerateFunctionRow)
				.OnSelectionChanged(this, &FBXFunctionSelectorCustomization::OnFunctionSelected)
				.SelectionMode(ESelectionMode::Single)
			]
		]
	];
}

UClass* FBXFunctionSelectorCustomization::InternalGetTargetClass() const
{
	if (TargetClassHandle.IsValid())
	{
		UObject* ObjectValue = nullptr;
		if (TargetClassHandle->GetValue(ObjectValue) == FPropertyAccess::Success && ObjectValue)
		{
			return Cast<UClass>(ObjectValue);
		}
	}

	if (PropertyHandle.IsValid())
	{
		TArray<UObject*> Outers;
		PropertyHandle->GetOuterObjects(Outers);
		for (UObject* Outer : Outers)
		{
			if (Outer)
			{
				return Outer->GetClass();
			}
		}
	}

	return nullptr;
}

void FBXFunctionSelectorCustomization::InternalCollectFunctions(UClass* InClass, TArray<TSharedPtr<FName>>& OutFunctions)
{
	if (!InClass)
	{
		return;
	}

	for (TFieldIterator<UFunction> It(InClass, EFieldIteratorFlags::IncludeSuper, EFieldIteratorFlags::ExcludeDeprecated); It; ++It)
	{
		UFunction* Func = *It;
		if (Func)
		{
			OutFunctions.Add(MakeShareable(new FName(Func->GetFName())));
		}
	}
}

TSharedRef<ITableRow> FBXFunctionSelectorCustomization::OnGenerateFunctionRow(TSharedPtr<FName> InFunctionName, const TSharedRef<STableViewBase>& InOwnerTable)
{
	return SNew(STableRow<TSharedPtr<FName>>, InOwnerTable)
	[
		SNew(STextBlock)
		.Text(InFunctionName.IsValid() ? FText::FromName(*InFunctionName) : FText())
	];
}

void FBXFunctionSelectorCustomization::OnFilterTextChanged(const FText& InFilterText)
{
	FilterText = InFilterText.ToString();
	FilteredFunctions.Empty();

	if (FilterText.IsEmpty())
	{
		FilteredFunctions = AllFunctions;
		return;
	}

	for (const TSharedPtr<FName>& FuncName : AllFunctions)
	{
		if (FuncName.IsValid() && FuncName->ToString().Contains(FilterText))
		{
			FilteredFunctions.Add(FuncName);
		}
	}
}
