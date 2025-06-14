#include "BXStateMachine.h"



#pragma region Important
bool UBXStateMachine::Initialize()
{
	bool bResult1 = true;
	if ((StateMachineFunctions & 1) > 0)
	{
		bResult1 = NativeInitialize();
	}

	bool bResult2 = true;
	if ((StateMachineFunctions & 2) > 0)
	{
		bResult2 = ScriptInitialize();
	}

	return (bResult1 && bResult2);
}

bool UBXStateMachine::NativeInitialize()
{
	return true;
}

bool UBXStateMachine::Deinitialize()
{
	bool bResult1 = true;
	if ((StateMachineFunctions & 4) > 0)
	{
		bResult1 = NativeDeinitialize();
	}

	bool bResult2 = true;
	if ((StateMachineFunctions & 8) > 0)
	{
		bResult2 = ScriptDeinitialize();
	}

	return (bResult1 && bResult2);
}

bool UBXStateMachine::NativeDeinitialize()
{
	return true;
}

void UBXStateMachine::Update(float InDeltaTime)
{
	if ((StateMachineFunctions & 16) > 0)
	{
		NativeUpdate(InDeltaTime);
	}

	if ((StateMachineFunctions & 32) > 0)
	{
		ScriptUpdate(InDeltaTime);
	}
}

void UBXStateMachine::NativeUpdate(float InDeltaTime)
{
	return;
}

#pragma endregion Important



#pragma region State
bool UBXStateMachine::EnterState(const FGameplayTag& InState, const FInstancedStruct& InParameter)
{
	bool bResult1 = true;
	if ((StateMachineFunctions & 64) > 0)
	{
		bResult1 = NativeEnterState(InState, InParameter);
	}

	bool bResult2 = true;
	if ((StateMachineFunctions & 128) > 0)
	{
		bResult2 = ScriptEnterState(InState, InParameter);
	}

	return (bResult1 && bResult2);
}

bool UBXStateMachine::NativeEnterState(const FGameplayTag& InState, const FInstancedStruct& InParameter)
{
	return true;
}

bool UBXStateMachine::ExitState(const FGameplayTag& InState, const FInstancedStruct& InParameter)
{
	bool bResult1 = true;
	if ((StateMachineFunctions & 256) > 0)
	{
		bResult1 = NativeExitState(InState, InParameter);
	}

	bool bResult2 = true;
	if ((StateMachineFunctions & 512) > 0)
	{
		bResult2 = ScriptExitState(InState, InParameter);
	}

	return (bResult1 && bResult2);
}

bool UBXStateMachine::NativeExitState(const FGameplayTag& InState, const FInstancedStruct& InParameter)
{
	return true;
}

#pragma endregion State
