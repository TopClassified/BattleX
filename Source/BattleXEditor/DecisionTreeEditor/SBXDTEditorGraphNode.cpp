#include "SBXDTEditorGraphNode.h"

#include "Widgets/Layout/SBox.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/SInlineEditableTextBlock.h"
#include "SGraphPin.h"
#include "SCommentBubble.h"
#include "SlateOptMacros.h"
#include "ScopedTransaction.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

#include "BXDTEditorGraphNode.h"
#include "BXDTEditorGraphEdge.h"



#define LOCTEXT_NAMESPACE "SBXDTEditorGraphNode"

class SBXDTEditorGraphPin : public SGraphPin
{
public:
	SLATE_BEGIN_ARGS(SBXDTEditorGraphPin) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UEdGraphPin* InPin)
	{
		this->SetCursor(EMouseCursor::Default);

		bShowLabel = true;

		GraphPinObj = InPin;
		check(GraphPinObj != nullptr);

		const UEdGraphSchema* Schema = GraphPinObj->GetSchema();
		check(Schema);

		SBorder::Construct
		(
			SBorder::FArguments()
			.BorderImage(this, &SBXDTEditorGraphPin::GetPinBorder)
			.BorderBackgroundColor(this, &SBXDTEditorGraphPin::GetPinColor)
			.OnMouseButtonDown(this, &SBXDTEditorGraphPin::OnPinMouseDown)
			.Cursor(this, &SBXDTEditorGraphPin::GetPinCursor)
			.Padding(FMargin(10.0f))
		);
	}

protected:
	virtual FSlateColor GetPinColor() const override
	{
		return FLinearColor(0.02f, 0.02f, 0.02f);
	}

	virtual TSharedRef<SWidget>	GetDefaultValueWidget() override
	{
		return SNew(STextBlock);
	}

	const FSlateBrush* GetPinBorder() const
	{
		return FAppStyle::GetBrush(TEXT("Graph.StateNode.Body"));
	}
};






void SBXDTEditorGraphNode::Construct(const FArguments& InArgs, UBXDTEditorGraphNode* InNode)
{
	GraphNode = InNode;
	UpdateGraphNode();
	InNode->SEdNode = SharedThis(this);
}

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SBXDTEditorGraphNode::UpdateGraphNode()
{
	const FMargin NodePadding = FMargin(2.0f);

	InputPins.Empty();
	OutputPins.Empty();

	// Reset variables that are going to be exposed, in case we are refreshing an already setup node.
	RightNodeBox.Reset();
	LeftNodeBox.Reset();
	OutputPinBox.Reset();

	TSharedPtr<SErrorText> ErrorText;
	TSharedPtr<SNodeTitle> NodeTitle = SNew(SNodeTitle, GraphNode);

	this->ContentScale.Bind(this, &SGraphNode::GetContentScale);
	this->GetOrAddSlot(ENodeZone::Center)
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Center)
		[
			SNew(SBorder)
			.BorderImage(FAppStyle::GetBrush("Graph.StateNode.Body"))
			.Padding(0.0f)
			.BorderBackgroundColor(this, &SBXDTEditorGraphNode::GetBorderBackgroundColor)
			[
				SNew(SOverlay)

				// Pins and node details
				+ SOverlay::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				[
					SNew(SVerticalBox)

					// INPUT PIN AREA
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SBox)
						.MinDesiredHeight(NodePadding.Top)
						[
							SAssignNew(LeftNodeBox, SVerticalBox)
						]
					]

					// STATE NAME AREA
					+ SVerticalBox::Slot()
					.Padding(FMargin(NodePadding.Left, 0.0f, NodePadding.Right, 0.0f))
					[
						SNew(SVerticalBox)
						+ SVerticalBox::Slot()
						.AutoHeight()
						[
							SAssignNew(NodeBody, SBorder)
							.BorderImage(FAppStyle::GetBrush("BTEditor.Graph.BTNode.Body"))
							.BorderBackgroundColor(this, &SBXDTEditorGraphNode::GetBackgroundColor)
							.HAlign(HAlign_Fill)
							.VAlign(VAlign_Center)
							.Visibility(EVisibility::SelfHitTestInvisible)
							[
								SNew(SOverlay)
								+ SOverlay::Slot()
								.HAlign(HAlign_Fill)
								.VAlign(VAlign_Fill)
								[
									SNew(SVerticalBox)
									+ SVerticalBox::Slot()
									.AutoHeight()
									[
										SNew(SHorizontalBox)
										+ SHorizontalBox::Slot()
										.AutoWidth()
										[
											// POPUP ERROR MESSAGE
											SAssignNew(ErrorText, SErrorText)
											.BackgroundColor(this, &SBXDTEditorGraphNode::GetErrorColor)
											.ToolTipText(this, &SBXDTEditorGraphNode::GetErrorMsgToolTip)
										]

										+ SHorizontalBox::Slot()
										.AutoWidth()
										.Padding(FMargin(10.0f, 5.0f, 5.0f, 10.0f))
										[
											SNew(SVerticalBox)
											+ SVerticalBox::Slot()
											.AutoHeight()
											[
												SAssignNew(InlineEditableText, SInlineEditableTextBlock)
												.Text(this, &SBXDTEditorGraphNode::GetNodeTitle)
												.Style(FAppStyle::Get(), "Graph.StateNode.NodeTitleInlineEditableText")
												.OnVerifyTextChanged(this, &SBXDTEditorGraphNode::OnVerifyNameTextChanged)
												.OnTextCommitted(this, &SBXDTEditorGraphNode::OnNameTextCommited)
												.IsSelected(this, &SBXDTEditorGraphNode::IsSelectedExclusively)
												.IsReadOnly(this, &SBXDTEditorGraphNode::IsNameReadOnly)
											]
										]
									]
								]
							]
						]
					]

					// OUTPUT PIN AREA
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SBox)
						.MinDesiredHeight(NodePadding.Bottom)
						[
							SAssignNew(RightNodeBox, SVerticalBox)
							+ SVerticalBox::Slot()
							.HAlign(HAlign_Fill)
							.VAlign(VAlign_Fill)
							.Padding(20.0f, 0.0f)
							.FillHeight(1.0f)
							[
								SAssignNew(OutputPinBox, SHorizontalBox)
							]
						]
					]
				]
			]
		];


	// Create comment bubble
	TSharedPtr<SCommentBubble> CommentBubble;
	const FSlateColor CommentColor = FLinearColor::White;

	SAssignNew(CommentBubble, SCommentBubble)
		.GraphNode(GraphNode)
		.Text(this, &SGraphNode::GetNodeComment)
		.OnTextCommitted(this, &SGraphNode::OnCommentTextCommitted)
		.ColorAndOpacity(CommentColor)
		.AllowPinning(true)
		.EnableTitleBarBubble(true)
		.EnableBubbleCtrls(true)
		.GraphLOD(this, &SGraphNode::GetCurrentLOD)
		.IsGraphNodeHovered(this, &SGraphNode::IsHovered);

	GetOrAddSlot(ENodeZone::TopCenter)
		.SlotOffset(TAttribute<FVector2D>(CommentBubble.Get(), &SCommentBubble::GetOffset))
		.SlotSize(TAttribute<FVector2D>(CommentBubble.Get(), &SCommentBubble::GetSize))
		.AllowScaling(TAttribute<bool>(CommentBubble.Get(), &SCommentBubble::IsScalingAllowed))
		.VAlign(VAlign_Top)
		[
			CommentBubble.ToSharedRef()
		];

	ErrorReporting = ErrorText;
	ErrorReporting->SetError(ErrorMsg);
	CreatePinWidgets();
}

void SBXDTEditorGraphNode::CreatePinWidgets()
{
	UBXDTEditorGraphNode* StateNode = CastChecked<UBXDTEditorGraphNode>(GraphNode);

	for (int32 PinIdx = 0; PinIdx < StateNode->Pins.Num(); PinIdx++)
	{
		UEdGraphPin* MyPin = StateNode->Pins[PinIdx];
		if (!MyPin->bHidden)
		{
			TSharedPtr<SGraphPin> NewPin = SNew(SBXDTEditorGraphPin, MyPin);

			AddPin(NewPin.ToSharedRef());
		}
	}
}

void SBXDTEditorGraphNode::AddPin(const TSharedRef<SGraphPin>& PinToAdd)
{
	PinToAdd->SetOwner(SharedThis(this));

	const UEdGraphPin* PinObj = PinToAdd->GetPinObj();
	const bool bAdvancedParameter = PinObj && PinObj->bAdvancedView;
	if (bAdvancedParameter)
	{
		PinToAdd->SetVisibility(TAttribute<EVisibility>(PinToAdd, &SGraphPin::IsPinVisibleAsAdvanced));
	}

	if (PinToAdd->GetDirection() == EEdGraphPinDirection::EGPD_Input)
	{
		LeftNodeBox->AddSlot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.FillHeight(1.0f)
			.Padding(20.0f, 0.0f)
			[
				PinToAdd
			];

		InputPins.Add(PinToAdd);
	}
	else
	{
		OutputPinBox->AddSlot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.FillWidth(1.0f)
			[
				PinToAdd
			];

		OutputPins.Add(PinToAdd);
	}
}

bool SBXDTEditorGraphNode::IsNameReadOnly() const
{
	return false;
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SBXDTEditorGraphNode::OnNameTextCommited(const FText& InText, ETextCommit::Type CommitInfo)
{
	SGraphNode::OnNameTextCommited(InText, CommitInfo);

	UBXDTEditorGraphNode* MyNode = CastChecked<UBXDTEditorGraphNode>(GraphNode);

	if (MyNode != nullptr && MyNode->GraphNode != nullptr)
	{
		const FScopedTransaction Transaction(LOCTEXT("DecisionTreeEditorRenameNode", "DecisionTree Editor: Rename Node"));
		MyNode->Modify();
		MyNode->GraphNode->Modify();
		MyNode->GraphNode->SetNodeTitle(InText);
		UpdateGraphNode();
	}
}

FSlateColor SBXDTEditorGraphNode::GetBorderBackgroundColor() const
{
	UBXDTEditorGraphNode* MyNode = CastChecked<UBXDTEditorGraphNode>(GraphNode);
	if (!MyNode || MyNode->Pins.Num() < 2)
	{
		return FLinearColor(0.0f, 0.22f, 0.4f);
	}

	UBXDTEditorGraphEdge* TheEdge = nullptr;
	if (MyNode->Pins[0]->LinkedTo.Num() > 0)
	{
		TheEdge = Cast<UBXDTEditorGraphEdge>(MyNode->Pins[0]->LinkedTo[0]->GetOwningNode());
	}
	else if (MyNode->Pins[1]->LinkedTo.Num() > 0)
	{
		TheEdge = Cast<UBXDTEditorGraphEdge>(MyNode->Pins[1]->LinkedTo[0]->GetOwningNode());
	}

	return FLinearColor(0.0f, 0.22f, 0.4f);
}

FSlateColor SBXDTEditorGraphNode::GetBackgroundColor() const
{
	return FLinearColor(0.1f, 0.1f, 0.1f);
}

EVisibility SBXDTEditorGraphNode::GetDragOverMarkerVisibility() const
{
	return EVisibility::Visible;
}

const FSlateBrush* SBXDTEditorGraphNode::GetNameIcon() const
{
	return FAppStyle::GetBrush(TEXT("BTEditor.Graph.BTNode.Icon"));
}

FText SBXDTEditorGraphNode::GetNodeTitle() const
{
	UBXDTEditorGraphNode* TheNode = Cast<UBXDTEditorGraphNode>(this->GraphNode);
	if (TheNode)
	{
		return TheNode->GetNodeTitle(ENodeTitleType::Type::FullTitle);
	}
	return FText();
}

#undef LOCTEXT_NAMESPACE
