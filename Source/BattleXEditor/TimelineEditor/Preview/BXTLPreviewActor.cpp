#include "BXTLPreviewActor.h"

#include "Engine/World.h"
#include "GameFramework/Character.h"

#include "Components/SkeletalMeshComponent.h"

#include "BXGearComponent.h" 



AActor* UBXTLPreviewActor::CreateActor_Implementation(UWorld* InWorld)
{
	if (!ActorClass || !InWorld)
	{
		return nullptr;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AActor* Result = InWorld->SpawnActorAbsolute(ActorClass.Get(), SpawnTransform, SpawnParams);
	if (Result)
	{
		if (UBXGearComponent* GearComponent = Result->FindComponentByClass<UBXGearComponent>())
		{
			for (TMap<EBXGearSlot, TSubclassOf<ABXGear>>::TIterator It(UsingGears); It; ++It)
			{
				GearComponent->ChangeEquipGearByClass(It->Key, 0, It->Value.Get(), Result->FindComponentByClass<USkeletalMeshComponent>());
				GearComponent->SwitchUsingGear(It->Key, 0);
				GearComponent->ChangeUsingGearState(It->Key, EBXGearState::S_Open);
			}
		}
	}

	return Result;
}
