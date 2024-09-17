#pragma once

#include "CoreMinimal.h"

#include "BXGearData.generated.h"



UCLASS()
class BATTLEX_API UBXGearData : public UObject
{
	GENERATED_BODY()
	
public:
	// ×°±¸Àà
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class ABXGear> GearClass;
	
};
