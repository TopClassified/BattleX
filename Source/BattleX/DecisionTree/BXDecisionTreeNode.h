#pragma once

#include "CoreMinimal.h"

#include "BXCondition.h"

#include "BXDecisionTreeNode.generated.h"



USTRUCT()
struct BATTLEX_API FBXDTNodePin
{
	GENERATED_USTRUCT_BODY()

public:
	FBXDTNodePin() {}
	FBXDTNodePin(bool InOutput, FName InName) : bOutput(InOutput), PinName(InName) {}

public:
	// Pin脚类型(false为输入Pin，true为输出Pin)
	UPROPERTY()
	bool bOutput = false;

	// Pin脚名称
	UPROPERTY()
	FName PinName = NAME_None;
};






UCLASS(Abstract, Blueprintable, BlueprintType)
class BATTLEX_API UBXDecisionTreeNode : public UObject
{
	GENERATED_BODY()

public:
	UBXDecisionTreeNode();
	
	bool CheckCondition(UObject* InWorldContext, UScriptStruct* InParameterType, void* InParameterAddress);

public:
	// 该节点的检查条件
	UPROPERTY(EditDefaultsOnly, Instanced)
	UBXCondition* Condition = nullptr;
	
	// 从该节点出发的边的索引
	UPROPERTY(VisibleDefaultsOnly)
	TArray<int32> OutEdges;

	// 到达该节点的边的索引
	UPROPERTY(VisibleDefaultsOnly)
	TArray<int32> InEdges;


#if WITH_EDITORONLY_DATA
protected:
	// 节点信息
	UPROPERTY(EditDefaultsOnly)
	FText NodeTitle;

	// 节点Pin脚信息
	UPROPERTY()
	TArray<FBXDTNodePin> PinInformations;
#endif

	
#if WITH_EDITOR
public:
	virtual void ConstructNode();

	virtual void CopyData(UBXDecisionTreeNode* OtherNode);

	virtual void RefreshNodeTitle();

	virtual FText GetNodeTitle() const;

	virtual void SetNodeTitle(const FText& InText);

	virtual bool CanCreateConnection(int32 SelfPinID, UBXDecisionTreeNode* Other, int32 OtherPinID, FText& ErrorMessage);

	virtual bool CanUserDelete() const;

	const TArray<FBXDTNodePin>* GetNodePinList();

protected:
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

};
