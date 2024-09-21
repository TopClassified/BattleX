#pragma once

#include "CoreMinimal.h"
#include "SGraphNode.h"



class SBXTLGraphNode : public SGraphNode
{
public:
	SLATE_BEGIN_ARGS(SBXTLGraphNode) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, class UBXTLGraphNode* InNode);

	void UpdateGraphNode() override;

	bool IsNameReadOnly() const override;

};
