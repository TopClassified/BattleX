#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"

#include "BXDecisionTreeNode.h"

#include "SBXDTEditorGraphNode.h"

#include "BXDTEditorGraphNode.generated.h"



UCLASS(MinimalAPI)
class UBXDTEditorGraphNode : public UEdGraphNode
{
	GENERATED_BODY()

public:
	UBXDTEditorGraphNode();
	virtual ~UBXDTEditorGraphNode();

	UPROPERTY(VisibleAnywhere, Instanced)
	UBXDecisionTreeNode* GraphNode = nullptr;

	TWeakPtr<SBXDTEditorGraphNode> SEdNode = nullptr;

	void SetDecisionTreeNode(UBXDecisionTreeNode* InNode);
	UBXDTEditorGraph* GetDecisionTreeEdGraph();

	// 创建节点的Pin脚
	virtual void AllocateDefaultPins() override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual void PrepareForCopying() override;

	virtual FLinearColor GetBackgroundColor() const;
	virtual UEdGraphPin* GetPinByIndex(int32 InIndex) const;
	virtual int32 GetPinIndex(const UEdGraphPin* InPin) const;
	virtual UEdGraphPin* GetPinByName(FName InName) const;

	virtual bool CanUserDeleteNode() const;

	// 获取该节点的深度
	virtual int32 GetNodeDepth();

	void DestroyNode() override;

	virtual void PostEditUndo() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

};
