#pragma once

#include "CoreMinimal.h"

#include "BXStateProxy.h"

#include "BXSProxyAscending.generated.h"



// 上升状态代理(BXState.Ascending):被挑空上升段确保滞空移动模式——
// 地面模式(Walking)下垂直速度会被地面约束清零,外部施加的挑空向上速度无法生效;
// 向上速度本身由命中技能/表现施加(现场已在外部速度里,代理不重复给速度)
UCLASS(Blueprintable, BlueprintType)
class BATTLEX_API UBXSProxyAscending : public UBXStateProxy
{
	GENERATED_BODY()

protected:
	virtual bool NativeStartState() override;

};
