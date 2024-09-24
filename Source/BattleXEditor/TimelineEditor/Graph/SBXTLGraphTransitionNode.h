#pragma once

#include "CoreMinimal.h" 
#include "SGraphNode.h"



class SBXTLGraphTransitionNode : public SGraphNode
{
public:
	SLATE_BEGIN_ARGS(SBXTLGraphTransitionNode) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, class UBXTLGraphTransitionNode* InNode);

	void UpdateGraphNode() override;

	TSharedPtr<SGraphPin> CreatePinWidget(UEdGraphPin* Pin) const override;

protected:
	TOptional<float> GetNumericValue() const;

	void SetNumericValue(float InText, ETextCommit::Type CommitInfo);

};
