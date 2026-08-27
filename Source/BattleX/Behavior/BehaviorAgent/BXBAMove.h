#pragma once

#include "CoreMinimal.h"

#include "BXBehaviorAgent.h"

#include "BXBAMove.generated.h"



UCLASS(Blueprintable, BlueprintType)
class BATTLEX_API UBXBADefaultMove : public UBXBehaviorAgent
{
	GENERATED_BODY()
	
public:
	virtual bool NativeStartBehavior(const FInstancedStruct& InParameter) override;

	virtual bool NativeStopBehavior(const FInstancedStruct& InParameter) override;

	virtual bool NativeCheckStartBehavior(const FInstancedStruct& InParameter) override;
	
};
