#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectSaveContext.h"
#include "BXTLGraph.h"

#include "BXBuffGraph.generated.h"



// BUFF图表(继承Timeline图表,重写节点创建逻辑以生成UBXBuffGraphNode,保存时同步层级配置到TaskBindings)
UCLASS()
class UBXBuffGraph : public UBXTLGraph
{
	GENERATED_BODY()

#pragma region Important
public:
	UBXBuffGraph();

	virtual ~UBXBuffGraph();

	// 创建一个新的Task节点(覆写以生成UBXBuffGraphNode)
	virtual void GenerateGraphNodeByTask(UBXTask* InTask, float InX, float InY) override;

	// 保存前同步层级配置到TaskBindings
	void PreSave(class FObjectPreSaveContext ObjectSaveContext) override;

#pragma endregion Important

};
