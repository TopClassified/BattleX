#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#include "EdGraph/EdGraphPin.h"
#include "EdGraph/EdGraphNode.h"

#include "BXEnums.h"

#include "BXTLGraphNode.generated.h"



USTRUCT(BlueprintType)
struct FBXTLGNodePin
{
	GENERATED_USTRUCT_BODY()

public:
	// 信息类型
	/*
	* 0:Task执行
	* 1:Task事件
	* 2:Task碰撞输入
 	* 3:Task输入数据
	* 4:Task输出数据
	*/
	UPROPERTY()
	uint8 PinType = 0;

	UPROPERTY()
	int64 UniqueID = 0;

	UPROPERTY()
	FName ExtraName;

	UPROPERTY()
	FName PinGUIDName;

public:
	bool operator == (const FBXTLGNodePin& Info) const
	{
		return PinType == Info.PinType && UniqueID == Info.UniqueID && ExtraName.IsEqual(Info.ExtraName);
	}
};



UCLASS(MinimalAPI)
class UBXTLGraphNode : public UEdGraphNode
{
	GENERATED_BODY()

#pragma region Important
public:
	UBXTLGraphNode();

	virtual ~UBXTLGraphNode();

public:
	UPROPERTY()
	class UBXTask* CachedTask = nullptr;

	class SBXTLGraphNode* NodeWidget = nullptr;

#pragma endregion Important



#pragma region Pin
public:
	// 创建节点的Pin脚
	void AllocateDefaultPins() override;

	// 刷新Pin信息
	void UpdatePins();

	// 获取Pin的颜色
	FLinearColor GetPinColor(const UEdGraphPin* InPin);

	// 获取Pin的信息
	FBXTLGNodePin* GetPinInformation(const UEdGraphPin* InPin);

	// 获取Pin的数据类型
	void GetPinDataType(const UEdGraphPin* InPin, EBXDataType& OutType, UScriptStruct*& OutStructType);

	FName GetPinNameFromTagName(const FName& InTag);

	// 刷新Pin信息列表
	void RefreshPinInformationList(TArray<FBXTLGNodePin>& InPinInformationList);

	// 根据GUID获取Pin对象
	UEdGraphPin* GetPinByGUID(FGuid InGuid);

	// 根据ExtraName获取Pin对象
	UEdGraphPin* GetPinByName(FName InExtraName);

	// 根据Pin信息创建Pin
	void CreatePinByInformation(FBXTLGNodePin& InInformation);

	void NodeConnectionListChanged();

private:
	UPROPERTY()
	TArray<FBXTLGNodePin> PinInformations;

#pragma endregion Pin



#pragma region Property
public:
	void RefreshGraphNodeInformation();

	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;

#pragma endregion Property



#pragma region Edit
public:
	virtual void PrepareForCopying() override;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

#pragma endregion Edit

};
