#pragma once

#include "CoreMinimal.h"
#include "InstancedStruct.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"

#include "StateMachine/BXStateMachine.h"

#include "BXStateComponent.generated.h"



DECLARE_LOG_CATEGORY_EXTERN(BXCOMP_State, Log, All);



UCLASS(BlueprintType, Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BATTLEX_API UBXStateComponent : public UActorComponent
{
	GENERATED_BODY()

#pragma region Important
public:	
	UBXStateComponent();
	
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// 状态机配置
	UPROPERTY(EditDefaultsOnly)
	TMap<TSubclassOf<UBXStateMachine>, FGameplayTagContainer> StateMachineConfig;

	// 状态机实例
	UPROPERTY(Transient)
	TArray<UBXStateMachine*> StateMachines;
	
	// 状态->状态机映射表
	UPROPERTY(Transient)
	TMap<FGameplayTag, TSoftObjectPtr<UBXStateMachine>> StateToStateMachineMap;
	
#pragma endregion Important


	
#pragma region State
public:
	// 进入状态
	UFUNCTION(BlueprintCallable)
	bool EnterState(const FGameplayTag& InState);

	// 进入状态(有参数版本)
	UFUNCTION(BlueprintCallable)
	bool EnterStateWithParameter(const FGameplayTag& InState, const FInstancedStruct& InParameter);

	// 退出状态
	UFUNCTION(BlueprintCallable)
	bool ExitState(const FGameplayTag& InState);

	// 退出状态(有参数版本)
	UFUNCTION(BlueprintCallable)
	bool ExitStateWithParameter(const FGameplayTag& InState, const FInstancedStruct& InParameter);

protected:
	// 更新所有的状态机
	void UpdateStateMachine(float InDeltaTime);
	
#pragma endregion State
	
};
