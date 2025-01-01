#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"

#include "BXDecisionTreeEdge.h"

#include "SBXDTEditorGraphEdge.h"

#include "BXDTEditorGraphEdge.generated.h"



UCLASS(MinimalAPI)
class UBXDTEditorGraphEdge : public UEdGraphNode
{
	GENERATED_BODY()

public:
	UBXDTEditorGraphEdge();
	virtual ~UBXDTEditorGraphEdge();

	UPROPERTY()
	class UEdGraph* Graph;

	UPROPERTY(VisibleAnywhere, Instanced)
	UBXDecisionTreeEdge* GraphEdge = nullptr;

	TWeakPtr<SBXDTEditorGraphEdge> SEdEdge = nullptr;

	virtual void AllocateDefaultPins() override;

	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;

	virtual void PinConnectionListChanged(UEdGraphPin* Pin) override;

	virtual void PrepareForCopying() override;

	virtual UEdGraphPin* GetInputPin() const { return Pins[0]; }
	virtual UEdGraphPin* GetOutputPin() const { return Pins[1]; }

	void CreateConnections(UEdGraphPin* StartPin, UEdGraphPin* EndPin);

	class UBXDTEditorGraphNode* GetStartNode();
	class UBXDTEditorGraphNode* GetEndNode();

	virtual int32 GetNodeDepth();
	virtual void ResetEdgeMessage();

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

};
