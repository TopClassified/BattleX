#pragma once

#include "CoreMinimal.h"

#include "BXDecisionTreeEdge.generated.h"



UCLASS(Abstract, Blueprintable, BlueprintType)
class BATTLEX_API UBXDecisionTreeEdge : public UObject
{
	GENERATED_BODY()

public:
	bool CheckCondition(UObject* InWorldContext, UScriptStruct* InParameterType, void* InParameterAddress);

public:
	// 该节点的检查条件
	UPROPERTY(EditDefaultsOnly, Instanced)
	UBXCondition* Condition = nullptr;
	
	// 边的出发节点
	UPROPERTY(VisibleDefaultsOnly)
	class UBXDecisionTreeNode* StartNode = nullptr;

	// 边的终止节点
	UPROPERTY(VisibleDefaultsOnly)
	class UBXDecisionTreeNode* EndNode = nullptr;


#if WITH_EDITORONLY_DATA
public:
	// 边信息框的大小
	UPROPERTY(EditDefaultsOnly)
	FVector2D EdgeMessageSize = FVector2D(5.0f, 5.0f);

protected:
	UPROPERTY()
	FText EdgeTitle;
#endif

#if WITH_EDITOR
public:
	virtual void ConstructEdge();

	virtual void CopyData(UBXDecisionTreeEdge* OtherEdge);

	virtual void RefreshEdgeTitle();

	virtual FText GetEdgeTitle() const;

	virtual void SetEdgeTitle(const FText& NewTitle);

protected:
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

};

