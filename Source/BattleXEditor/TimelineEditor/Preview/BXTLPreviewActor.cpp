#include "BXTLPreviewActor.h"

#include "Engine/World.h"
#include "GameFramework/Character.h"



AActor* UBXTLPreviewActor::CreateActor_Implementation(UWorld* InWorld)
{
	if (!ActorClass || !InWorld)
	{
		return nullptr;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	return InWorld->SpawnActorAbsolute(ActorClass.Get(), SpawnTransform, SpawnParams);
}
