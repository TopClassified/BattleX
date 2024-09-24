#pragma once

#include "CoreMinimal.h"

#include "BXGearData.generated.h" 



UCLASS()
class BATTLEX_API UBXGearData : public UObject
{
	GENERATED_BODY()
	
public:
	// 装备类
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class ABXGear> GearClass;
	
};
