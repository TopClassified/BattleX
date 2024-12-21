#include "BXSubSystem.h"

#include "BXManager.h"
#include "BXSettings.h"



#pragma region Important
void UBXSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const UBXSettings* Settings = GetDefault<UBXSettings>();
	if (!Settings)
	{
		return;
	}
	
	for (TArray<TSubclassOf<UBXManager>>::TConstIterator It(Settings->ManagerClasses); It; ++It)
	{
		UClass* ClassType = *It;
		if (!IsValid(ClassType))
		{
			continue;
		}
		
		UBXManager* Manager = NewObject<UBXManager>(this, ClassType);
		if (!IsValid(Manager))
		{
			continue;
		}

		Manager->Initialize();

		ManagerMap.Add(ClassType, Manager);
	}
}

void UBXSubSystem::Deinitialize()
{
	for (TMap<UClass*, UBXManager*>::TIterator It(ManagerMap); It; ++It)
	{
		if (!IsValid(It->Value))
		{
			continue;
		}
		
		It->Value->Deinitialize();
		It->Value->MarkAsGarbage();
	}
	ManagerMap.Reset();

	Super::Deinitialize();
}

#pragma endregion Important
