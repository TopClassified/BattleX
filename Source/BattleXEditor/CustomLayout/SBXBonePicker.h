#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Views/STreeView.h"

class USkeletalMesh;

// 骨骼树节点
struct FBXBoneTreeNode
{
	// 骨骼名称
	FName BoneName = NAME_None;

	// 骨骼索引
	int32 BoneIndex = INDEX_NONE;

	// 子节点列表
	TArray<TSharedPtr<FBXBoneTreeNode>> Children;
};

// 骨骼选择回调
DECLARE_DELEGATE_OneParam(FBXOnBonePicked, FName);

// 骨骼选择窗口，以树形展示骨骼层级
class SBXBonePicker : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SBXBonePicker)
	{
	}
	SLATE_ARGUMENT(TWeakObjectPtr<USkeletalMesh>, SkeletalMesh)
	SLATE_EVENT(FBXOnBonePicked, OnBonePicked)
	SLATE_END_ARGS()

	// 构建窗口
	void Construct(const FArguments& InArgs);

private:
	// 构建骨骼树
	void InternalBuildBoneTree();

	// 重建过滤后的节点列表
	void InternalRebuildFilteredTree();

	// 生成行控件
	TSharedRef<ITableRow> OnGenerateRow(TSharedPtr<FBXBoneTreeNode> InNode, const TSharedRef<STableViewBase>& InOwnerTable);

	// 获取子节点
	void OnGetChildren(TSharedPtr<FBXBoneTreeNode> InNode, TArray<TSharedPtr<FBXBoneTreeNode>>& OutChildren);

	// 选中回调
	void OnSelectionChanged(TSharedPtr<FBXBoneTreeNode> InNode, ESelectInfo::Type InSelectInfo);

	// 搜索文本变更
	void OnFilterTextChanged(const FText& InFilterText);

private:
	TWeakObjectPtr<USkeletalMesh> WeakSkeletalMesh;

	TArray<TSharedPtr<FBXBoneTreeNode>> RootNodes;

	TArray<TSharedPtr<FBXBoneTreeNode>> AllNodes;

	TArray<TSharedPtr<FBXBoneTreeNode>> FilteredNodes;

	FString FilterText;

	FBXOnBonePicked OnBonePicked;

	TSharedPtr<STreeView<TSharedPtr<FBXBoneTreeNode>>> TreeView;
};
