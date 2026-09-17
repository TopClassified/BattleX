#pragma once

#include "CoreMinimal.h"

#include "BXStateProxy.h"

#include "BXSProxyFloating.generated.h"



// 滞空状态代理(BXState.Stun.Floating):浮空连段核心物理——切Flying模式自持悬浮(无重力悬停),
// 退出还原Falling恢复下落;进入时速度置零防止残留速度带着角色漂移(悬浮位置由进入时刻锚定)
UCLASS(Blueprintable, BlueprintType)
class BATTLEX_API UBXSProxyFloating : public UBXStateProxy
{
	GENERATED_BODY()

protected:
	virtual bool NativeStartState() override;

	virtual bool NativeStopState() override;

};
