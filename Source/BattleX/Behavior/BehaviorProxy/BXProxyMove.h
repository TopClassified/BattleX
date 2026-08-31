#pragma once

#include "CoreMinimal.h"

#include "BXBehaviorProxy.h"

#include "BXProxyMove.generated.h"



// 移动代理(常驻型门控):启用/禁用推UBXCharacterMovementComponent本地开关,移动组件只读开关执行物理刹车
UCLASS(Blueprintable, BlueprintType)
class BATTLEX_API UBXProxyMove : public UBXBehaviorProxy
{
	GENERATED_BODY()

public:
	virtual bool NativeEnableProxy() override;

	virtual bool NativeDisableProxy() override;

	virtual bool NativeStartBehavior(const FInstancedStruct& InParameter) override;

	virtual bool NativeStopBehavior() override;

	virtual bool NativeCheckStartBehavior(const FInstancedStruct& InParameter) override;

};
