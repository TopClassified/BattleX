#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphSchema.h"
#include "ConnectionDrawingPolicy.h"

#include "BXTLGraphSchema.generated.h"



USTRUCT()
struct FBXTLGCreatTaskNode : public FEdGraphSchemaAction
{
	GENERATED_USTRUCT_BODY();

public:
	FBXTLGCreatTaskNode() {}

	FBXTLGCreatTaskNode(const FText& InNodeCategory, const FText& InMenuDesc, const FText& InToolTip, const int32 InGrouping, class UBXTask* InTask) : FEdGraphSchemaAction(InNodeCategory, InMenuDesc, InToolTip, InGrouping), TaskPointer(InTask) {}

	UEdGraphNode* PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode = true) override;

	void AddReferencedObjects(FReferenceCollector& Collector) override;

private:
	TWeakObjectPtr<class UBXTask> TaskPointer = nullptr;

};






class FBXTLGConnectionDrawingPolicy : public FConnectionDrawingPolicy
{
public:
	FBXTLGConnectionDrawingPolicy(int32 InBackLayerID, int32 InFrontLayerID, float ZoomFactor, const FSlateRect& InClippingRect, FSlateWindowElementList& InDrawElements);

	void DetermineWiringStyle(UEdGraphPin* OutputPin, UEdGraphPin* InputPin, FConnectionParams& Params) override;

};






UCLASS(MinimalAPI)
class UBXTLGraphSchema : public UEdGraphSchema
{
	GENERATED_BODY()

public:
	// 获取图表类型
	EGraphType GetGraphType(const UEdGraph* TestEdGraph) const override;

	// 检测两个Pin脚之间是否可以相连
	const FPinConnectionResponse CanCreateConnection(const UEdGraphPin* PinA, const UEdGraphPin* PinB) const override;

	// 创建连线的绘制策略
	class FConnectionDrawingPolicy* CreateConnectionDrawingPolicy(int32 InBackLayerID, int32 InFrontLayerID, float InZoomFactor, const FSlateRect& InClippingRect, class FSlateWindowElementList& InDrawElements, class UEdGraph* InGraphObj) const override;

	// 获取Pin脚颜色
	FLinearColor GetPinTypeColor(const FEdGraphPinType& PinType) const override;

};
