#include "SBXBonePicker.h"
#include "Animation/Skeleton.h"
#include "AnimationRuntime.h"
#include "Engine/SkeletalMesh.h"
#include "ReferenceSkeleton.h"
#include "Widgets/Input/SSearchBox.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Views/SListView.h"
#include "Widgets/Views/STableRow.h"

void SBXBonePicker::Construct(const FArguments& InArgs)
{
	WeakSkeletalMesh = InArgs._SkeletalMesh;
	OnBonePicked = InArgs._OnBonePicked;

	InternalBuildBoneTree();
	FilteredNodes = RootNodes;

	ChildSlot
	[
		SNew(SBox)
		.MinDesiredWidth(300.0f)
		.MaxDesiredHeight(400.0f)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0.0f, 0.0f, 0.0f, 4.0f)
			[
				SNew(SSearchBox)
				.OnTextChanged(this, &SBXBonePicker::OnFilterTextChanged)
			]
			+ SVerticalBox::Slot()
			.FillHeight(1.0f)
			[
				SNew(SBorder)
				[
					SAssignNew(TreeView, STreeView<TSharedPtr<FBXBoneTreeNode>>)
					.TreeItemsSource(&FilteredNodes)
					.OnGenerateRow(this, &SBXBonePicker::OnGenerateRow)
					.OnGetChildren(this, &SBXBonePicker::OnGetChildren)
					.OnSelectionChanged(this, &SBXBonePicker::OnSelectionChanged)
					.SelectionMode(ESelectionMode::Single)
				]
			]
		]
	];
}

void SBXBonePicker::InternalBuildBoneTree()
{
	RootNodes.Empty();
	AllNodes.Empty();

	USkeletalMesh* Mesh = WeakSkeletalMesh.Get();
	if (!Mesh)
	{
		return;
	}

	USkeleton* Skeleton = Mesh->GetSkeleton();
	if (!Skeleton)
	{
		return;
	}

	const FReferenceSkeleton& RefSkeleton = Skeleton->GetReferenceSkeleton();
	const TArray<FMeshBoneInfo>& BoneInfos = RefSkeleton.GetRefBoneInfo();
	TArray<TSharedPtr<FBXBoneTreeNode>> Nodes;
	Nodes.SetNum(BoneInfos.Num());

	for (int32 i = 0; i < BoneInfos.Num(); ++i)
	{
		Nodes[i] = MakeShareable(new FBXBoneTreeNode());
		Nodes[i]->BoneName = BoneInfos[i].Name;
		Nodes[i]->BoneIndex = i;
	}

	for (int32 i = 0; i < BoneInfos.Num(); ++i)
	{
		int32 ParentIndex = BoneInfos[i].ParentIndex;
		if (ParentIndex != INDEX_NONE && Nodes.IsValidIndex(ParentIndex))
		{
			Nodes[ParentIndex]->Children.Add(Nodes[i]);
		}
		else
		{
			RootNodes.Add(Nodes[i]);
		}
	}

	AllNodes = Nodes;
}

void SBXBonePicker::InternalRebuildFilteredTree()
{
	FilteredNodes.Empty();

	if (FilterText.IsEmpty())
	{
		FilteredNodes = RootNodes;
		return;
	}

	for (const TSharedPtr<FBXBoneTreeNode>& Node : AllNodes)
	{
		if (Node.IsValid() && Node->BoneName.ToString().Contains(FilterText))
		{
			FilteredNodes.Add(Node);
		}
	}
}

TSharedRef<ITableRow> SBXBonePicker::OnGenerateRow(TSharedPtr<FBXBoneTreeNode> InNode, const TSharedRef<STableViewBase>& InOwnerTable)
{
	return SNew(STableRow<TSharedPtr<FBXBoneTreeNode>>, InOwnerTable)
	[
		SNew(STextBlock)
		.Text(InNode.IsValid() ? FText::FromName(InNode->BoneName) : FText())
	];
}

void SBXBonePicker::OnGetChildren(TSharedPtr<FBXBoneTreeNode> InNode, TArray<TSharedPtr<FBXBoneTreeNode>>& OutChildren)
{
	if (InNode.IsValid() && FilterText.IsEmpty())
	{
		OutChildren = InNode->Children;
	}
}

void SBXBonePicker::OnSelectionChanged(TSharedPtr<FBXBoneTreeNode> InNode, ESelectInfo::Type InSelectInfo)
{
	if (InNode.IsValid())
	{
		OnBonePicked.ExecuteIfBound(InNode->BoneName);
	}
}

void SBXBonePicker::OnFilterTextChanged(const FText& InFilterText)
{
	FilterText = InFilterText.ToString();
	InternalRebuildFilteredTree();
	if (TreeView.IsValid())
	{
		TreeView->RequestTreeRefresh();
	}
}
