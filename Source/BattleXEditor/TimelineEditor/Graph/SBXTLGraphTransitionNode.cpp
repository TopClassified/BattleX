#include "SBXTLGraphTransitionNode.h"

#include "SGraphPin.h"
#include "NodeFactory.h"
#include "Widgets/Text/SInlineEditableTextBlock.h"

#include "BXTLGraphNode.h"
#include "BXTLGraphTransitionNode.h"
#include "SBXTLGraphTransitionPin.h"



#define LOCTEXT_NAMESPACE "SBXTLGraphTransitionNode"

void SBXTLGraphTransitionNode::Construct(const FArguments& InArgs, UBXTLGraphTransitionNode* InNode)
{
	GraphNode = InNode;

	UpdateGraphNode();
}

void SBXTLGraphTransitionNode::UpdateGraphNode()
{
	//SGraphNode::UpdateGraphNode();

	this->GetOrAddSlot(ENodeZone::Center)
	.HAlign(HAlign_Center)
	.VAlign(VAlign_Center)
	[
		SNew(SBorder)
		.BorderImage(FAppStyle::GetBrush("Sequencer.Section.BackgroundTint"))
		.BorderBackgroundColor(FColor::White)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Center)
			[
				SAssignNew(LeftNodeBox, SVerticalBox)
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(SNumericEntryBox<float>)
				.EditableTextBoxStyle(FAppStyle::Get(), "Graph.EditableTextBox")
				.BorderForegroundColor(FColor::Black)
				.Value(this, &SBXTLGraphTransitionNode::GetNumericValue)
				.OnValueCommitted(this, &SBXTLGraphTransitionNode::SetNumericValue)
				.Visibility(EVisibility::Visible)
				.IsEnabled(true)
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.HAlign(HAlign_Right)
			.VAlign(VAlign_Center)
			[
				SAssignNew(RightNodeBox, SVerticalBox)
			]
		]
	];

	CreatePinWidgets();

	for (int32 i = 0; i < InputPins.Num(); ++i)
	{
		InputPins[i]->SetVisibility(EVisibility::HitTestInvisible);
	}

	for (int32 i = 0; i < OutputPins.Num(); ++i)
	{
		OutputPins[i]->SetVisibility(EVisibility::HitTestInvisible);
	}
}

TSharedPtr<SGraphPin> SBXTLGraphTransitionNode::CreatePinWidget(UEdGraphPin* Pin) const
{
	return SNew(SBXTLGraphTransitionPin, Pin);
}

TOptional<float> SBXTLGraphTransitionNode::GetNumericValue() const
{
	if (UBXTLGraphTransitionNode* GNode = Cast<UBXTLGraphTransitionNode>(GraphNode))
	{
		return GNode->Delay;
	}

	return 0.0f;
}

void SBXTLGraphTransitionNode::SetNumericValue(float InValue, ETextCommit::Type CommitInfo)
{
	if (UBXTLGraphTransitionNode* GNode = Cast<UBXTLGraphTransitionNode>(GraphNode))
	{
		GNode->Delay = InValue;

		if (InputPins.Num() <= 0)
		{
			return;
		}

		UEdGraphPin* LogicPin = InputPins[0]->GetPinObj();
		if (!LogicPin)
		{
			return;
		}

		if (LogicPin->LinkedTo.Num() <= 0)
		{
			return;
		}

		if (UBXTLGraphNode* LinkedNode = Cast<UBXTLGraphNode>(LogicPin->LinkedTo[0]->GetOwningNode()))
		{
			LinkedNode->RefreshGraphNodeInformation();
		}
	}
}

#undef LOCTEXT_NAMESPACE
