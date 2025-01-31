#pragma once

#include "CoreMinimal.h"
#include "State/BXBehaviorAgent.h"

#include "BXBADefaultLanded.generated.h"



UCLASS(Blueprintable, BlueprintType)
class BATTLEX_API UBXBADefaultLanded : public UBXBehaviorAgent
{
	GENERATED_BODY()
	
public:
	bool NativeStartBehavior(const FInstancedStruct& InParameter);
	
	bool NativeCheckStartBehavior(const FInstancedStruct& InParameter);
	
};
