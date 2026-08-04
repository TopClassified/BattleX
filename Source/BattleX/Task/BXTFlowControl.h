#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#include "Task/BXTask.h"
#include "Task/BXTaskCondition.h"

#include "BXTFlowControl.generated.h"



// Switch分支配置
USTRUCT(BlueprintType)
struct FBXTSwitchCase
{
	GENERATED_BODY()

public:
	// 分支条件（支持UBXTaskCondition及其派生类如Composite，空Children的Composite返回True，可作为Default分支放在末尾）
	UPROPERTY(EditAnywhere, Instanced, Category = "Condition")
	UBXTaskCondition* Condition = nullptr;

	// 条件满足时触发的事件Tag
	UPROPERTY(EditAnywhere, Category = "Event")
	FGameplayTag EventTag;
};



// 流程控制Switch任务，按顺序评估分支条件，首个匹配的分支触发对应事件后结束
UCLASS(BlueprintType, Blueprintable)
class BATTLEX_API UBXTSwitch : public UBXTask
{
	GENERATED_BODY()

public:
	UBXTSwitch();

public:
	// 分支列表，按顺序判断，第一个匹配的分支触发对应事件后结束
	UPROPERTY(EditAnywhere, Meta = (TitleProperty = "EventTag"), Category = "Switch")
	TArray<FBXTSwitchCase> Cases;

	// 所有分支都不匹配时触发的事件Tag（默认BXTEvent_BranchDefault）
	UPROPERTY(EditAnywhere, Category = "Switch")
	FGameplayTag DefaultEventTag;

#if WITH_EDITOR
public:
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
