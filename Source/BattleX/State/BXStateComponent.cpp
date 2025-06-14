#include "BXStateComponent.h"
#include "BXEventManager.h"



DEFINE_LOG_CATEGORY(BXCOMP_State);



#pragma region Important
UBXStateComponent::UBXStateComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UBXStateComponent::BeginPlay()
{
	for (TMap<TSubclassOf<UBXStateMachine>, FGameplayTagContainer>::TIterator It(StateMachineConfig); It; ++It)
	{
		if (!IsValid(It->Key))
		{
			continue;
		}
		
		UBXStateMachine* NewMachine = NewObject<UBXStateMachine>(this, It->Key);
		if (!IsValid(NewMachine))
		{
			continue;
		}
		NewMachine->Initialize();
		StateMachines.Add(NewMachine);

		FGameplayTagContainer& Container = It->Value;
		for (int32 i = 0; i < Container.Num(); ++i)
		{
			const FGameplayTag& Tag = Container.GetByIndex(i);
			StateToStateMachineMap.Add(Tag, NewMachine);
		}
	}
	
	Super::BeginPlay();
}

void UBXStateComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	StateToStateMachineMap.Empty();

	for (int32 i = 0; i < StateMachines.Num(); ++i)
	{
		if (!IsValid(StateMachines[i]))
		{
			continue;
		}

		StateMachines[i]->Deinitialize();
		StateMachines[i]->MarkAsGarbage();
	}
	StateMachines.Empty();
}

void UBXStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateStateMachine(DeltaTime);
}

#pragma endregion Important



#pragma region State
bool UBXStateComponent::EnterState(const FGameplayTag& InState)
{
	FInstancedStruct IS;
	return EnterStateWithParameter(InState, IS);
}

bool UBXStateComponent::EnterStateWithParameter(const FGameplayTag& InState, const FInstancedStruct& InParameter)
{
	bool bResult = false;
	
	TSoftObjectPtr<UBXStateMachine>* FindResult = StateToStateMachineMap.Find(InState);
	if (!FindResult)
	{
		UE_LOG(BXCOMP_State, Log, TEXT("Enter State(%s) Failed! The State Machine Is Invalid!"), *InState.GetTagName().ToString());
		return bResult;
	}

	UBXStateMachine* StateMachine = FindResult->Get();
	if (!IsValid(StateMachine))
	{
		UE_LOG(BXCOMP_State, Log, TEXT("Enter State(%s) Failed! The State Machine Is Invalid!"), *InState.GetTagName().ToString());
		return bResult;
	}

	bResult = StateMachine->EnterState(InState, InParameter);

	return bResult;
}

bool UBXStateComponent::ExitState(const FGameplayTag& InState)
{
	FInstancedStruct IS;
	return ExitStateWithParameter(InState, IS);
}

bool UBXStateComponent::ExitStateWithParameter(const FGameplayTag& InState, const FInstancedStruct& InParameter)
{
	bool bResult = false;
	
	TSoftObjectPtr<UBXStateMachine>* FindResult = StateToStateMachineMap.Find(InState);
	if (!FindResult)
	{
		UE_LOG(BXCOMP_State, Log, TEXT("Exit State(%s) Failed! The State Machine Is Invalid!"), *InState.GetTagName().ToString());
		return bResult;
	}

	UBXStateMachine* StateMachine = FindResult->Get();
	if (!IsValid(StateMachine))
	{
		UE_LOG(BXCOMP_State, Log, TEXT("Exit State(%s) Failed! The State Machine Is Invalid!"), *InState.GetTagName().ToString());
		return bResult;
	}

	bResult = StateMachine->ExitState(InState, InParameter);

	return bResult;
}

void UBXStateComponent::UpdateStateMachine(float InDeltaTime)
{
	// 更新状态机
	for (int32 i = 0; i < StateMachines.Num(); ++i)
	{
		UBXStateMachine* StateMachine = StateMachines[i];
		if (!IsValid(StateMachine))
		{
			continue;
		}

		StateMachine->Update(InDeltaTime);
	}
}

#pragma endregion State
