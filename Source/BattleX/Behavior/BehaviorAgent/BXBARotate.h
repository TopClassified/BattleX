#pragma once

#include "CoreMinimal.h"

#include "BXBehaviorAgent.h"

#include "BXBARotate.generated.h"



UCLASS(Blueprintable, BlueprintType)
class BATTLEX_API UBXBADefaultRotate : public UBXBehaviorAgent
{
	GENERATED_BODY()
	
public:
	virtual bool NativeStartBehavior(const FInstancedStruct& InParameter) override;

	virtual bool NativeStopBehavior(const FInstancedStruct& InParameter) override;

	virtual bool NativeCheckStartBehavior(const FInstancedStruct& InParameter) override;
	
};
