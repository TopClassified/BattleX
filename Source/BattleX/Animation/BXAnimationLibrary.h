#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"

#include "BXAnimationLibrary.generated.h"



UCLASS()
class BATTLEX_API UBXAnimationLibrary : public UPrimaryDataAsset
{
	GENERATED_BODY()

#pragma region API
public:
	UFUNCTION(BlueprintCallable)
	static UAnimMontage* GetMontageFromActorByTag(AActor* InActor, const FGameplayTag& InTag);
	
	UFUNCTION(BlueprintCallable)
	static UAnimMontage* GetMontageFromAnimationLibraryByTag(UBXAnimationLibrary* InLibrary, const FGameplayTag& InTag);
	
#pragma endregion API
	
};
