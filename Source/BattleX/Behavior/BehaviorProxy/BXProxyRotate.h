#pragma once

#include "CoreMinimal.h"

#include "BXBehaviorProxy.h"

#include "BXProxyRotate.generated.h"



// 转向代理(常驻型门控):启用/禁用推UBXCharacterMovementComponent本地开关,移动组件只读开关停物理转向
UCLASS(Blueprintable, BlueprintType)
class BATTLEX_API UBXProxyRotate : public UBXBehaviorProxy
{
	GENERATED_BODY()

public:
	virtual bool NativeEnableProxy() override;

	virtual bool NativeDisableProxy() override;

	virtual bool NativeStartBehavior(const FInstancedStruct& InParameter) override;

	virtual bool NativeStopBehavior() override;

	virtual bool NativeCheckStartBehavior(const FInstancedStruct& InParameter) override;

};
