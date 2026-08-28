#pragma once

#include "CoreMinimal.h"

#include "BXBehaviorProxy.h"

#include "BXProxyJump.generated.h"



// 跳跃代理(常驻型门控):启用/禁用推UBXCharacterMovementComponent本地开关,CanAttemptJump只读开关
UCLASS(Blueprintable, BlueprintType)
class BATTLEX_API UBXProxyJump : public UBXBehaviorProxy
{
	GENERATED_BODY()

public:
	virtual bool NativeEnableProxy() override;

	virtual bool NativeDisableProxy() override;

	virtual bool NativeStartBehavior(const FInstancedStruct& InParameter) override;

	virtual bool NativeCheckStartBehavior(const FInstancedStruct& InParameter) override;

};
