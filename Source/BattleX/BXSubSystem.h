#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "BXManager.h"

#include "BXSubSystem.generated.h" 



UCLASS(Abstract)
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
		if (UBXManager** FindResult = ManagerMap.Find(T::StaticClass()))
		{
			return Cast<T>(*FindResult);
		}

		return nullptr;
	}
	
private:
	UPROPERTY(Transient)
	TMap<UClass*, UBXManager*> ManagerMap;

#pragma endregion Manager

};
