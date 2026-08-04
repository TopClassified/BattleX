#pragma once

#include "CoreMinimal.h"
#include "IPropertyTypeCustomization.h"

class USkeletalMesh;

// FBXBoneSelector的属性定制，BoneName通过骨骼树窗口选择
class FBXBoneSelectorCustomization : public IPropertyTypeCustomization
{
public:
	// 创建实例
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();

	// 定制表头
	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> InPropertyHandle, FDetailWidgetRow& InHeaderRow, IPropertyTypeCustomizationUtils& InCustomizationUtils) override;

	// 定制子项
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> InPropertyHandle, IDetailChildrenBuilder& InChildBuilder, IPropertyTypeCustomizationUtils& InCustomizationUtils) override;

private:
	// 获取骨骼名称文本
	FText InternalGetBoneNameText() const;

	// 骨骼选择回调
	void OnBonePicked(FName InBoneName);

	// 获取骨骼选择窗口内容
	TSharedRef<SWidget> InternalGetBonePickerContent();

	// 获取当前SkeletalMesh
	USkeletalMesh* InternalGetSkeletalMesh() const;

private:
	TSharedPtr<IPropertyHandle> PropertyHandle;

	TSharedPtr<IPropertyHandle> BoneNameHandle;

	TSharedPtr<IPropertyHandle> SkeletalMeshHandle;
};
