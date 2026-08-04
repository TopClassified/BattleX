#pragma once

#include "CoreMinimal.h"

#include "BXTProcessor.h"

#include "BXTPFlowControl.generated.h"



// Switch流程控制Processor的运行时上下文（无自定义数据）
USTRUCT(BlueprintType)
struct FBXTPSwitchContext
{
	GENERATED_BODY()
};



// Switch流程控制Processor
UCLASS(BlueprintType, Blueprintable)
class BATTLEX_API UBXTPSwitch : public UBXTProcessor
{
	GENERATED_BODY()

public:
	// 遍历Cases评估条件，首个匹配分支触发对应EventTag
	virtual void Start(FBXTLRunTimeData& InOutRTData, FBXTLSectionRTData& InOutRTSData, FBXTLTaskRTData& InOutRTTData) override;
};
