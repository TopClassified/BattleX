#pragma once

#include "CoreMinimal.h"

#include "BXBehaviorProxy.h"

#include "BXProxyLanded.generated.h"



// 落地代理(事件型瞬时事:无基层开关,随管线隐式启停,Hit参数经参数结构传递给监听方)
UCLASS(Blueprintable, BlueprintType)
class BATTLEX_API UBXProxyLanded : public UBXBehaviorProxy
{
	GENERATED_BODY()

public:
	virtual bool NativeStartBehavior(const FInstancedStruct& InParameter) override;

	virtual bool NativeCheckStartBehavior(const FInstancedStruct& InParameter) override;

};
