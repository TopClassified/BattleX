#pragma once

#include "CoreMinimal.h"
#include "GameplayTags.h"
#include "GameFramework/Actor.h"

#include "BXGear.h"
#include "BXGearEnums.h"

#include "BXTLPreviewActor.generated.h" 



UCLASS(Blueprintable, EditInlineNew)
class UBXTLPreviewActor : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> ActorClass = nullptr;

	UPROPERTY(EditDefaultsOnly)
	FTransform SpawnTransform = FTransform::Identity;

	// 要使用的装备
	UPROPERTY(EditDefaultsOnly)
	TMap<FGameplayTag, TSubclassOf<ABXGear>> UsingGears;

	// 要锁定的身体部位
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag LockedBodyPart;

public:
	UFUNCTION(BlueprintNativeEvent)
	AActor* CreateActor(class UWorld* InWorld);
	virtual AActor* CreateActor_Implementation(class UWorld* InWorld);

};

