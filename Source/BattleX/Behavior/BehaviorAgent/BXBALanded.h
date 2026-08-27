#pragma once

#include "CoreMinimal.h"

#include "BXBehaviorAgent.h"

#include "BXBALanded.generated.h"



UCLASS(Blueprintable, BlueprintType)
class BATTLEX_API UBXBADefaultLanded : public UBXBehaviorAgent
{
	GENERATED_BODY()
	
public:
	virtual bool NativeStartBehavior(const FInstancedStruct& InParameter) override;

	virtual bool NativeCheckStartBehavior(const FInstancedStruct& InParameter) override;
	
};
