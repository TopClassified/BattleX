#pragma once

#include "CoreMinimal.h"

#include "EdGraph/EdGraphNode.h"

#include "BXTLGraphTransitionNode.generated.h" 



UCLASS(MinimalAPI)
class UBXTLGraphTransitionNode : public UEdGraphNode
{
	GENERATED_BODY()

#pragma region Important
public:
	UBXTLGraphTransitionNode();

	virtual ~UBXTLGraphTransitionNode();

	UEdGraphNode* GetLinkTargetNode();

public:
	// 延迟时间
	UPROPERTY()
	float Delay = 0.0f;

	class SBXTLGraphNode* NodeWidget = nullptr;

#pragma endregion Important



#pragma region Pin
public:
	// 创建节点的Pin脚
	void AllocateDefaultPins() override;

	void CreateConnections(UEdGraphPin* StartPin, UEdGraphPin* EndPin);

	void PinConnectionListChanged(UEdGraphPin* Pin);

#pragma endregion Pin

};
