#pragma once

#include "CoreMinimal.h"

#include "BXBehaviorAgent.h"

#include "BXBAJump.generated.h"



UCLASS(Blueprintable, BlueprintType)
class BATTLEX_API UBXBADefaultJump : public UBXBehaviorAgent
{
	GENERATED_BODY()
	
public:
	virtual bool NativeStartBehavior(const FInstancedStruct& InParameter) override;

	virtual bool NativeCheckStartBehavior(const FInstancedStruct& InParameter) override;
	
};
