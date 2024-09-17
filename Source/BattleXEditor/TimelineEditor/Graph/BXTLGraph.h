#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraph.h"

#include "BXTLGraph.generated.h"



UCLASS()
class UBXTLGraph : public UEdGraph
{
	GENERATED_BODY()

public:
	UBXTLGraph();

	virtual ~UBXTLGraph();

	// 刷新图表
	void RefreshGraph();

	// 创建一个新的Task节点
	void GenerateGraphNodeByTask(class UBXTask* InTask);
	void GenerateGraphNodeByTask(class UBXTask* InTask, float InPosX, float InPosY);

	// 尝试根据Task信息连接将Node1的pin连接上Node2
	void TryAutoConnectPin(class UBXTLGraphNode* Node1, class UBXTLGraphNode* Node2);

};
