#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameFramework/Actor.h"

#include "BXEnums.h"

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

	// Ҫ���������岿λ
	UPROPERTY(EditDefaultsOnly)
	EBXBodyPartType LockedBodyPart = EBXBodyPartType::BPT_Default;

public:
	UFUNCTION(BlueprintNativeEvent)
	AActor* CreateActor(class UWorld* InWorld);
	virtual AActor* CreateActor_Implementation(class UWorld* InWorld);

};

