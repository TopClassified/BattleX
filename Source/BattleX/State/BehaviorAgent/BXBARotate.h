#pragma once

#include "CoreMinimal.h"

#include "BXBehaviorAgent.h"

#include "BXBARotate.generated.h"



UCLASS(Blueprintable, BlueprintType)
class BATTLEX_API UBXBADefaultRotate : public UBXBehaviorAgent
{
	GENERATED_BODY()
	
public:
	bool NativeStartBehavior(const FInstancedStruct& InParameter);

	bool NativeStopBehavior(const FInstancedStruct& InParameter);
	
	bool NativeCheckStartBehavior(const FInstancedStruct& InParameter);
	
};
