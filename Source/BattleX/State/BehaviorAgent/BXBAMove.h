#pragma once

#include "CoreMinimal.h"

#include "BXBehaviorAgent.h"

#include "BXBAMove.generated.h"



UCLASS(Blueprintable, BlueprintType)
class BATTLEX_API UBXBADefaultMove : public UBXBehaviorAgent
{
	GENERATED_BODY()
	
public:
	bool NativeStartBehavior(const FInstancedStruct& InParameter);

	bool NativeStopBehavior(const FInstancedStruct& InParameter);
	
	bool NativeCheckStartBehavior(const FInstancedStruct& InParameter);
	
};
