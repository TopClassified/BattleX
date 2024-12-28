#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "BXDecisionTreeNode.h"
#include "BXDecisionTreeEdge.h"
#include "BXDecisionTreeActuator.h"

#include "BXDecisionTreeTemplate.generated.h"




UCLASS(BlueprintType, Blueprintable)
class BATTLEX_API UBXDecisionTreeType : public UObject
{
	GENERATED_BODY()

public:
	// 决策执行器类型
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Runtime")
	TSubclassOf<UBXDecisionTreeActuator> ActuatorType = nullptr;

	
#if WITH_EDITORONLY_DATA
public:
	// 是否允许出现环路
	UPROPERTY(EditDefaultsOnly, Category = "Edit")
	bool bAllowCycle = false;

	// 默认创建的节点类型，为空则不创建
	UPROPERTY(EditDefaultsOnly, Category = "Edit")
	TSubclassOf<UBXDecisionTreeNode> FirstNodeType = nullptr;
	
	// 节点类型
	UPROPERTY(EditDefaultsOnly, Category = "Edit")
	TArray<TSubclassOf<UBXDecisionTreeNode>> NodeTypes;

	// 边类型
	UPROPERTY(EditDefaultsOnly, Category = "Edit")
	TSubclassOf<UBXDecisionTreeEdge> EdgeType = UBXDecisionTreeEdge::StaticClass();

	// 加载的资源对象
	UPROPERTY(EditDefaultsOnly, Category = "Edit")
	TArray<TSubclassOf<UObject>> LoadAssetTypes;
#endif

#if WITH_EDITOR
public:
	UFUNCTION(BlueprintImplementableEvent)
	FString GetTypeName();
#endif
	
};






UCLASS(BlueprintType, Blueprintable)
class BATTLEX_API UBXDecisionTreeTemplate : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	virtual void PreLoad(UObject* WorldContext) {}

public:
	// 该模板的ID
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	int32 ID = 0;

	// 决策树的根节点数组
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	TArray<UBXDecisionTreeNode*> RootNodes;

	// 决策树的边信息
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	TArray<UBXDecisionTreeEdge*> TreeEdges;

	// 决策树类型信息
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UBXDecisionTreeType> TreeType = nullptr;


#if WITH_EDITORONLY_DATA
public:
	// 缩略图主要标题
	UPROPERTY(EditDefaultsOnly, Category = "Editor")
	FText ThumbnailMainTitle;

	// 缩略图附加标题
	UPROPERTY(EditDefaultsOnly, Category = "Editor")
	FText ThumbnailSubTitle;

	UPROPERTY()
	class UEdGraph* EdGraph = nullptr;
#endif

#if WITH_EDITOR
public:
	virtual void InitByEditor();

	virtual void InitSaver();

	virtual void AddEdgeMessage(UBXDecisionTreeNode* InFromNode, UBXDecisionTreeNode* InToNode, UBXDecisionTreeEdge* InEdge);

	virtual void RefreshLogicMessage();

private:
	TArray<TWeakObjectPtr<UBXDecisionTreeNode>> EditedNodes;
#endif

};
