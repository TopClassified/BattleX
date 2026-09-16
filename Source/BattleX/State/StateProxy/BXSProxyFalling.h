#pragma once

#include "CoreMinimal.h"

#include "BXStateProxy.h"

#include "BXSProxyFalling.generated.h"



// 下落状态代理(BXState.Falling):击飞下落段保持滞空移动模式(空中受击脱离地面模式),
// 与上升代理物理同型(独立成类:两状态的物理需求后续分化互不影响);
// 状态结束不还原模式——自然下落落地由移动组件接管
UCLASS(Blueprintable, BlueprintType)
class BATTLEX_API UBXSProxyFalling : public UBXStateProxy
{
	GENERATED_BODY()

protected:
	virtual bool NativeStartState() override;

};
