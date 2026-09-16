#pragma once

#include "CoreMinimal.h"

#include "BXStateProxy.h"

#include "BXSProxyExecution.generated.h"



// 处决状态代理(BXStunState.Execution):定身的物理部分——进入时速度清零(受术者瞬间停死,不滑行);
// 移动/跳跃/转向的封锁归状态Forbid列表(BXBehavior.Rotate/移动系Tag),勿在此重复登记
UCLASS(Blueprintable, BlueprintType)
class BATTLEX_API UBXSProxyExecution : public UBXStateProxy
{
	GENERATED_BODY()

protected:
	virtual bool NativeStartState() override;

};
