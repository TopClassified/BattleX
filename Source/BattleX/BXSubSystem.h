#pragma once

#include "CoreMinimal.h"

#include "Subsystems/GameInstanceSubsystem.h"

#include "BXSubSystem.generated.h" 



UCLASS(DisplayName = "BattleX Subsystem")
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
	UFUNCTION(BlueprintCallable)
	class UBXManager* GetBXManager() const;

private:
	UPROPERTY(Transient)
	class UBXManager* BXManager = nullptr;

#pragma endregion Manager

};
