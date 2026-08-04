#pragma once

#include "CoreMinimal.h"
#include "IPropertyTypeCustomization.h"
#include "Widgets/Input/SComboButton.h"

// FBXFunctionSelector的属性定制，FunctionName通过函数列表下拉选择
class FBXFunctionSelectorCustomization : public IPropertyTypeCustomization
{
public:
	// 创建实例
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();

	// 定制表头
	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> InPropertyHandle, FDetailWidgetRow& InHeaderRow, IPropertyTypeCustomizationUtils& InCustomizationUtils) override;

	// 定制子项
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> InPropertyHandle, IDetailChildrenBuilder& InChildBuilder, IPropertyTypeCustomizationUtils& InCustomizationUtils) override;

private:
	// 获取函数名文本
	FText InternalGetFunctionNameText() const;

	// 函数选择回调
	void OnFunctionSelected(TSharedPtr<FName> InFunctionName, ESelectInfo::Type InSelectInfo);

	// 获取函数选择下拉内容
	TSharedRef<SWidget> InternalGetFunctionPickerContent();

	// 获取目标类（优先TargetClass字段，为空时从outer对象推导）
	UClass* InternalGetTargetClass() const;

	// 收集目标类的函数列表
	void InternalCollectFunctions(UClass* InClass, TArray<TSharedPtr<FName>>& OutFunctions);

	// 生成下拉列表行
	TSharedRef<ITableRow> OnGenerateFunctionRow(TSharedPtr<FName> InFunctionName, const TSharedRef<STableViewBase>& InOwnerTable);

	// 搜索文本变更
	void OnFilterTextChanged(const FText& InFilterText);

private:
	TSharedPtr<IPropertyHandle> PropertyHandle;

	TSharedPtr<IPropertyHandle> TargetClassHandle;

	TSharedPtr<IPropertyHandle> FunctionNameHandle;

	TArray<TSharedPtr<FName>> AllFunctions;

	TArray<TSharedPtr<FName>> FilteredFunctions;

	FString FilterText;

	TSharedPtr<SComboButton> ComboButton;
};
