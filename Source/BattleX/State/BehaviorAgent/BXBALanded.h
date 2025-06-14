#pragma once

#include "CoreMinimal.h"

#include "BXBehaviorAgent.h"

#include "BXBALanded.generated.h"



UCLASS(Blueprintable, BlueprintType)
class BATTLEX_API UBXBADefaultLanded : public UBXBehaviorAgent
{
	GENERATED_BODY()
	
public:
	bool NativeStartBehavior(const FInstancedStruct& InParameter);
	
	bool NativeCheckStartBehavior(const FInstancedStruct& InParameter);
	
};
