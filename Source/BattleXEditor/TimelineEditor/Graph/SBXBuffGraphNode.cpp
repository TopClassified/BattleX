#include "SBXBuffGraphNode.h"

#include "Widgets/Input/SSpinBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SBox.h"

#include "BXBuffGraphNode.h"



#define LOCTEXT_NAMESPACE "SBXBuffGraphNode"

void SBXBuffGraphNode::Construct(const FArguments& InArgs, UBXBuffGraphNode* InNode)
{
	GraphNode = InNode;

	UpdateGraphNode();
}

void SBXBuffGraphNode::UpdateGraphNode()
{
	SBXTLGraphNode::UpdateGraphNode();
}

void SBXBuffGraphNode::CreateBelowWidgetControls(TSharedPtr<SVerticalBox> MainBox)
{
	if (MainBox.IsValid())
	{
		MainBox->AddSlot()
		.AutoHeight()
		[
			CreateLayerConfigWidget()
		];
	}
}

int32 SBXBuffGraphNode::GetMinLayer() const
{
	if (const UBXBuffGraphNode* BuffNode = Cast<UBXBuffGraphNode>(GraphNode))
	{
		return BuffNode->MinLayer;
	}

	return 0;
}

int32 SBXBuffGraphNode::GetMaxLayer() const
{
	if (const UBXBuffGraphNode* BuffNode = Cast<UBXBuffGraphNode>(GraphNode))
	{
		return BuffNode->MaxLayer;
	}

	return 0;
}

void SBXBuffGraphNode::OnMinLayerChanged(int32 NewValue)
{
	if (UBXBuffGraphNode* BuffNode = Cast<UBXBuffGraphNode>(GraphNode))
	{
		BuffNode->Modify();
		BuffNode->MinLayer = FMath::Max(0, NewValue);
	}
}

void SBXBuffGraphNode::OnMaxLayerChanged(int32 NewValue)
{
	if (UBXBuffGraphNode* BuffNode = Cast<UBXBuffGraphNode>(GraphNode))
	{
		BuffNode->Modify();
		BuffNode->MaxLayer = FMath::Max(0, NewValue);
	}
}

TSharedRef<SWidget> SBXBuffGraphNode::CreateLayerConfigWidget()
{
	return SNew(SHorizontalBox)
	+ SHorizontalBox::Slot()
	.AutoWidth()
	.VAlign(VAlign_Center)
	.Padding(FMargin(4.0f, 2.0f, 2.0f, 2.0f))
	[
		SNew(STextBlock)
		.Text(LOCTEXT("LayerLabel", "Layer:"))
		.ColorAndOpacity(FLinearColor::White)
	]
	+ SHorizontalBox::Slot()
	.AutoWidth()
	.VAlign(VAlign_Center)
	.Padding(FMargin(2.0f, 2.0f, 2.0f, 2.0f))
	[
		SNew(SBox)
		.WidthOverride(50.0f)
		[
			SNew(SSpinBox<int32>)
			.Value(this, &SBXBuffGraphNode::GetMinLayer)
			.OnValueChanged(this, &SBXBuffGraphNode::OnMinLayerChanged)
			.MinSliderValue(0)
			.MaxValue(999)
			.Delta(1)
		]
	]
	+ SHorizontalBox::Slot()
	.AutoWidth()
	.VAlign(VAlign_Center)
	.Padding(FMargin(2.0f, 2.0f, 2.0f, 2.0f))
	[
		SNew(STextBlock)
		.Text(FText::FromString(TEXT("-")))
		.ColorAndOpacity(FLinearColor::White)
	]
	+ SHorizontalBox::Slot()
	.AutoWidth()
	.VAlign(VAlign_Center)
	.Padding(FMargin(2.0f, 2.0f, 4.0f, 2.0f))
	[
		SNew(SBox)
		.WidthOverride(50.0f)
		[
			SNew(SSpinBox<int32>)
			.Value(this, &SBXBuffGraphNode::GetMaxLayer)
			.OnValueChanged(this, &SBXBuffGraphNode::OnMaxLayerChanged)
			.MinSliderValue(0)
			.MaxValue(999)
			.Delta(1)
		]
	];
}

#undef LOCTEXT_NAMESPACE
