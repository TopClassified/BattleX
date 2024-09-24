#include "BXSubSystem.h"

#include "BXManager.h" 



#pragma region Important
void UBXSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	BXManager = NewObject<UBXManager>(this);
	if (BXManager)
	{
		BXManager->Initialize();
	}
}

void UBXSubSystem::Deinitialize()
{
	if (BXManager)
	{
		BXManager->Deinitialize();
		BXManager->MarkAsGarbage();
	}
	BXManager = nullptr;

	Super::Deinitialize();
}

#pragma endregion Important



#pragma region Manager
UBXManager* UBXSubSystem::GetBXManager() const
{
	return BXManager;
}

#pragma endregion Manager
