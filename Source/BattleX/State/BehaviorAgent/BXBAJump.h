#pragma once

#include "CoreMinimal.h"

#include "BXBehaviorAgent.h"

#include "BXBAJump.generated.h"



UCLASS(Blueprintable, BlueprintType)
class BATTLEX_API UBXBADefaultJump : public UBXBehaviorAgent
{
	GENERATED_BODY()
	
public:
	bool NativeStartBehavior(const FInstancedStruct& InParameter);
	
	bool NativeCheckStartBehavior(const FInstancedStruct& InParameter);
	
};
