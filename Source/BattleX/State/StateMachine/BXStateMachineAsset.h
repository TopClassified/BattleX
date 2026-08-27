#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#include "DecisionTree/BXDecisionTreeNode.h"
#include "DecisionTree/BXDecisionTreeTemplate.h"
#include "State/BXStateStructs.h"

#include "BXStateMachineAsset.generated.h"



class AActor;



// 状态机类型(决策树派生,允许环:硬直链回环)
UCLASS(Abstract)
class BATTLEX_API UBXStateMachineType : public UBXDecisionTreeType
{
	GENERATED_BODY()

public:
	UBXStateMachineType();
};



// 状态机资产(可视化状态机:节点=状态,边=转移条件)
UCLASS()
class BATTLEX_API UBXStateMachineAsset : public UBXDecisionTreeTemplate
{
	GENERATED_BODY()

public:
	UBXStateMachineAsset()
	{
		TreeType = UBXStateMachineType::StaticClass();
	}

public:
	// 查询状态Tag对应的节点(状态机图为扁平结构:节点全部注册于RootNodes,连接关系走TreeEdges)
	UBXSMStateNode* FindStateNode(const FGameplayTag& InStateTag) const;

	// 查询默认节点(空转回退用;全图至多一个,多个标记取首个)
	UBXSMStateNode* FindDefaultStateNode() const;

	// 收集全部状态节点的Tag(组件注册状态→状态机映射用)
	void CollectStateTags(FGameplayTagContainer& OutTags) const;

#if WITH_EDITORONLY_DATA
protected:
	// 状态Tag→节点缓存(编辑器保存期重建)
	UPROPERTY(Transient)
	TMap<FGameplayTag, TObjectPtr<UBXSMStateNode>> StateNodeMap;
#endif

#if WITH_EDITOR
public:
	void RefreshLogicMessage() override;
#endif
};



// 状态节点(节点=状态:Tag+时长+禁用;表现统一由转移边承载)
UCLASS()
class BATTLEX_API UBXSMStateNode : public UBXDecisionTreeNode
{
	GENERATED_BODY()

public:
	UBXSMStateNode();

public:
	// 状态Tag
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag StateTag;

	// 默认节点(状态机曾激活过后空转时自动进入:链尾/被中断后归位;全图至多一个标记)
	UPROPERTY(EditDefaultsOnly)
	bool bIsDefaultNode = false;

	// 驻留时长(≤0无限;外部进入携带优先)
	UPROPERTY(EditDefaultsOnly)
	float Duration = -1.0f;

	// 存续期禁用的行为(Tag层级)
	UPROPERTY(EditDefaultsOnly)
	FGameplayTagContainer ForbiddenBehaviors;

#if WITH_EDITOR
public:
	void RefreshNodeTitle() override;

	void SetNodeTitle(const FText& NewTitle) override;

protected:
	void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};



// 状态转移边(条件=决策树Instanced Condition,评估时机=到期/每帧)
UCLASS()
class BATTLEX_API UBXSMTransitionEdge : public UBXDecisionTreeEdge
{
	GENERATED_BODY()

public:
	// 评估时机
	UPROPERTY(EditDefaultsOnly)
	EBXTransitionEvaluate EvaluateType = EBXTransitionEvaluate::TE_OnExpired;

	// 转移表现(状态间过渡表现的唯一配置入口)
	UPROPERTY(EditDefaultsOnly)
	FBXStatePresentation TransitionPresentation;

#if WITH_EDITOR
public:
	void ConstructEdge() override;

	void RefreshEdgeTitle() override;
#endif
};



// 状态机转移评估上下文(条件参数)
USTRUCT(BlueprintType)
struct FBXSMTransitionContext
{
	GENERATED_USTRUCT_BODY()

public:
	// 评估者
	UPROPERTY(Transient)
	TObjectPtr<AActor> Owner = nullptr;

	// 来源状态Tag
	UPROPERTY(Transient)
	FGameplayTag FromTag;

	// 服务器世界时间(毫秒)
	UPROPERTY(Transient)
	int64 ServerTimeMs = 0;
};
