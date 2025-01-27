#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "BXManager.h"

#include "BXSubSystem.generated.h" 



UCLASS()
class BATTLEX_API UBXSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

#pragma region Important
public:
	void Initialize(FSubsystemCollectionBase& Collection) override;
	
	void Deinitialize() override;

#pragma endregion Important



#pragma region Manager
public:
	template<typename T>
	T* GetManagerByClass()
	{
		UClass* ManagerClass = T::StaticClass();
		if (!IsValid(ManagerClass))
		{
			return nullptr;
		}
		
		for (TMap<UClass*, UBXManager*>::TIterator It(ManagerMap); It; ++It)
		{
			if (It->Key->IsChildOf(ManagerClass))
			{
				return Cast<T>(It->Value);
			}	
		}

		return nullptr;
	}
	
private:
	UPROPERTY(Transient)
	TMap<UClass*, UBXManager*> ManagerMap;

#pragma endregion Manager

};
