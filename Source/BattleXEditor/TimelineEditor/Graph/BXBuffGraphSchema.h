#pragma once

#include "CoreMinimal.h"
#include "BXTLGraphSchema.h"

#include "BXBuffGraphSchema.generated.h"



// BUFF图表创建Task动作
USTRUCT()
struct FBXBuffCreateTaskAction : public FEdGraphSchemaAction
{
	GENERATED_USTRUCT_BODY()

public:
	FBXBuffCreateTaskAction() {}

	FBXBuffCreateTaskAction(const FText& InNodeCategory, const FText& InMenuDesc, const FText& InToolTip, const int32 InGrouping, UClass* InTaskClass)
		: FEdGraphSchemaAction(InNodeCategory, InMenuDesc, InToolTip, InGrouping), TaskClass(InTaskClass) {}

	// 执行动作: 创建Task并生成节点
	virtual UEdGraphNode* PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode = true) override;

private:
	// 要创建的Task类型
	TWeakObjectPtr<UClass> TaskClass = nullptr;

};



// BUFF图表Schema(继承Timeline Schema,扩展右键菜单以创建Task)
UCLASS()
class UBXBuffGraphSchema : public UBXTLGraphSchema
{
	GENERATED_BODY()

public:
	// 获取图表上下文菜单动作
	virtual void GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const override;

};
