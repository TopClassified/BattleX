#pragma once

#include "CoreMinimal.h"

#include "BXStateMachineAsset.h"

#include "BXStateMachineInstance.generated.h"



// 状态机运行时实例(纯运行态:资产引用+当前节点,不持有状态事实)
UCLASS()
class BATTLEX_API UBXStateMachineInstance : public UObject
{
	GENERATED_BODY()

public:
	UBXStateMachineInstance() {}

	// 初始化(绑定资产)
	void Initialize(UBXStateMachineAsset* InAsset);

	// 反初始化
	void Deinitialize();

public:
	// 状态机资产
	UPROPERTY(Transient)
	TObjectPtr<UBXStateMachineAsset> Asset = nullptr;

	// 当前节点(空=SM空转:无硬直用无条目表达)
	UPROPERTY(Transient)
	TObjectPtr<UBXSMStateNode> CurrentNode = nullptr;

	// 默认节点缓存(Initialize期解析;空转回退用,资产未配置默认节点则为空)
	UPROPERTY(Transient)
	TObjectPtr<UBXSMStateNode> DefaultNode = nullptr;

	// 曾激活标记(首次外部进入后置位;默认回退仅在曾激活后生效,休眠机不自动唤醒)
	bool bActivatedOnce = false;
};
