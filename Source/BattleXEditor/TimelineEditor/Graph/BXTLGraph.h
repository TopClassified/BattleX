#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraph.h"

#include "BXTLEditor.h"

#include "BXTask.h"

#include "BXTLGraph.generated.h" 



UCLASS()
class UBXTLGraph : public UEdGraph
{
	GENERATED_BODY()

public:
	UBXTLGraph();

	virtual ~UBXTLGraph();

	void Init();

	void Uninit();
	
	void PreSave(class FObjectPreSaveContext ObjectSaveContext) override;

	// 刷新图表
	void RefreshGraph();

	// 刷新图表信息
	void RefreshGraphInformation();

	// 检查Task节点是否合法
	bool CheckTaskNodeValid(UBXTask* InTask);

	// 根据Task获取对应节点
	UEdGraphNode* GetGraphNodeByTask(UBXTask* InTask);

	// 创建一个新的Task节点
	void GenerateGraphNodeByTask(UBXTask* InTask, float InX, float InY);

	// 创建一组新的Task节点
	void GenerateGraphNodesByTasks(TArray<UBXTask*>& InTaskList);

	// 删除一个节点
	void DeleteGraphNodes(TArray<UEdGraphNode*>& InNodes);

	// 删除一个Task节点
	void DeleteGraphNodeByTask(UBXTask* InTask);

	// 尝试根据Task信息连接将Node1的pin连接上Node2
	void TryAutoConnectPin(class UBXTLGraphNode* Node1, class UBXTLGraphNode* Node2);

	// 对齐时间
	void AlignTimeProperty(float InAlign);

};
