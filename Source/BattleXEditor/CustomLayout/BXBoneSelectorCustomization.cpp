#include "BXBoneSelectorCustomization.h"
#include "SBXBonePicker.h"
#include "BXStructs.h"
#include "Engine/SkeletalMesh.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "PropertyHandle.h"
#include "Widgets/Input/SComboButton.h"
#include "Widgets/Text/STextBlock.h"

TSharedRef<IPropertyTypeCustomization> FBXBoneSelectorCustomization::MakeInstance()
{
	return MakeShareable(new FBXBoneSelectorCustomization());
}

void FBXBoneSelectorCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> InPropertyHandle, FDetailWidgetRow& InHeaderRow, IPropertyTypeCustomizationUtils& InCustomizationUtils)
{
	PropertyHandle = InPropertyHandle;
	BoneNameHandle = InPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FBXBoneSelector, BoneName));
	SkeletalMeshHandle = InPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FBXBoneSelector, SkeletalMesh));

	InHeaderRow
	.NameContent()
	[
		InPropertyHandle->CreatePropertyNameWidget()
	]
	.ValueContent()
	.MinDesiredWidth(200.0f)
	[
		SNew(SComboButton)
		.OnGetMenuContent(this, &FBXBoneSelectorCustomization::InternalGetBonePickerContent)
		.ContentPadding(2.0f)
		.ButtonContent()
		[
			SNew(STextBlock)
			.Text(this, &FBXBoneSelectorCustomization::InternalGetBoneNameText)
		]
	];
}

void FBXBoneSelectorCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> InPropertyHandle, IDetailChildrenBuilder& InChildBuilder, IPropertyTypeCustomizationUtils& InCustomizationUtils)
{
	if (SkeletalMeshHandle.IsValid())
	{
		InChildBuilder.AddProperty(SkeletalMeshHandle.ToSharedRef());
	}
}

FText FBXBoneSelectorCustomization::InternalGetBoneNameText() const
{
	if (BoneNameHandle.IsValid())
	{
		FName Value = NAME_None;
		if (BoneNameHandle->GetValue(Value) == FPropertyAccess::Success)
		{
			return FText::FromName(Value);
		}
	}
	return FText::GetEmpty();
}

void FBXBoneSelectorCustomization::OnBonePicked(FName InBoneName)
{
	if (BoneNameHandle.IsValid())
	{
		BoneNameHandle->SetValue(InBoneName);
	}
}

TSharedRef<SWidget> FBXBoneSelectorCustomization::InternalGetBonePickerContent()
{
	USkeletalMesh* Mesh = InternalGetSkeletalMesh();
	return SNew(SBXBonePicker)
		.SkeletalMesh(Mesh)
		.OnBonePicked(this, &FBXBoneSelectorCustomization::OnBonePicked);
}

USkeletalMesh* FBXBoneSelectorCustomization::InternalGetSkeletalMesh() const
{
	if (!SkeletalMeshHandle.IsValid())
	{
		return nullptr;
	}

	TArray<void*> RawData;
	SkeletalMeshHandle->AccessRawData(RawData);
	if (RawData.Num() == 0 || !RawData[0])
	{
		return nullptr;
	}

	FSoftObjectPath* Path = reinterpret_cast<FSoftObjectPath*>(RawData[0]);
	return Cast<USkeletalMesh>(Path->TryLoad());
}
