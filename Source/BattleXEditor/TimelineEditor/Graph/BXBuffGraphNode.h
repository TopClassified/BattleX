#pragma once

#include "CoreMinimal.h"

#include "BXTLGraphNode.h"

#include "BXBuffGraphNode.generated.h"



// BUFF图表节点(继承Timeline图表节点,扩展层级配置属性)
UCLASS(MinimalAPI)
class UBXBuffGraphNode : public UBXTLGraphNode
{
	GENERATED_BODY()

#pragma region Layer
public:
	// 层级执行区间下限,0代表不限制
	UPROPERTY(EditAnywhere, Category = "Layer", meta = (ClampMin = 0))
	int32 MinLayer = 0;

	// 层级执行区间上限,0代表不限制
	UPROPERTY(EditAnywhere, Category = "Layer", meta = (ClampMin = 0))
	int32 MaxLayer = 0;

#pragma endregion Layer



#pragma region Lifecycle
public:
	// 销毁节点时移除对应的Task
	void DestroyNode() override;

#pragma endregion Lifecycle

};
