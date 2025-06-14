#include "BXSMStun.h"

#include "BXGameplayTags.h"



#pragma region Important
void UBXSMStun::NativeUpdate(float InDeltaTime)
{
	
}

#pragma endregion Important



#pragma region State
bool UBXSMStun::NativeEnterState(const FGameplayTag& InState, const FInstancedStruct& InParameter)
{
	bool bResult = false;
	
	// 退出之前的状态
	if (CurrentState != InState)
	{
		FInstancedStruct IS;
		ExitState(CurrentState, IS);
	}

	// 进入新的状态
	if (InState == BXGameplayTags::BXStunState_Normal)
	{
		bResult = EnterNormal(InParameter);
	}
	else if (InState == BXGameplayTags::BXStunState_Knockback)
	{
		EnterKnockback(InParameter);
	}
	else if (InState == BXGameplayTags::BXStunState_Knockdown)
	{
		EnterKnockdown(InParameter);
	}
	else if (InState == BXGameplayTags::BXStunState_Prone)
	{
		EnterProne(InParameter);
	}
	else if (InState == BXGameplayTags::BXStunState_Ascending)
	{
		EnterAscending(InParameter);
	}
	else if (InState == BXGameplayTags::BXStunState_Falling)
	{
		EnterFalling(InParameter);
	}
	else if (InState == BXGameplayTags::BXStunState_Floating)
	{
		EnterFloating(InParameter);
	}
	else if (InState == BXGameplayTags::BXStunState_Execution)
	{
		EnterExecution(InParameter);
	}

	if (!bResult)
	{
		CurrentState = InState;
	}

	return bResult;
}

bool UBXSMStun::NativeExitState(const FGameplayTag& InState, const FInstancedStruct& InParameter)
{
	if (InState == BXGameplayTags::BXStunState_Normal)
	{
		return ExitNormal(InParameter);
	}
	else if (InState == BXGameplayTags::BXStunState_Knockback)
	{
		return ExitKnockback(InParameter);
	}
	else if (InState == BXGameplayTags::BXStunState_Knockdown)
	{
		return ExitKnockdown(InParameter);
	}
	else if (InState == BXGameplayTags::BXStunState_Prone)
	{
		return ExitProne(InParameter);
	}
	else if (InState == BXGameplayTags::BXStunState_Ascending)
	{
		return ExitAscending(InParameter);
	}
	else if (InState == BXGameplayTags::BXStunState_Falling)
	{
		return ExitFalling(InParameter);
	}
	else if (InState == BXGameplayTags::BXStunState_Floating)
	{
		return ExitFloating(InParameter);
	}
	else if (InState == BXGameplayTags::BXStunState_Execution)
	{
		return ExitExecution(InParameter);
	}

	return false;
}
	
#pragma endregion State



#pragma region Stun
bool UBXSMStun::EnterNormal(const FInstancedStruct& InParameter)
{
	return false;
}

bool UBXSMStun::ExitNormal(const FInstancedStruct& InParameter)
{
	return false;
}

bool UBXSMStun::EnterKnockback(const FInstancedStruct& InParameter)
{
	return false;
}

bool UBXSMStun::ExitKnockback(const FInstancedStruct& InParameter)
{
	return false;
}

bool UBXSMStun::EnterKnockdown(const FInstancedStruct& InParameter)
{
	return false;
}

bool UBXSMStun::ExitKnockdown(const FInstancedStruct& InParameter)
{
	return false;
}

bool UBXSMStun::EnterProne(const FInstancedStruct& InParameter)
{
	return false;
}

bool UBXSMStun::ExitProne(const FInstancedStruct& InParameter)
{
	return false;
}

bool UBXSMStun::EnterAscending(const FInstancedStruct& InParameter)
{
	return false;
}

bool UBXSMStun::ExitAscending(const FInstancedStruct& InParameter)
{
	return false;
}

bool UBXSMStun::EnterFalling(const FInstancedStruct& InParameter)
{
	return false;
}

bool UBXSMStun::ExitFalling(const FInstancedStruct& InParameter)
{
	return false;
}

bool UBXSMStun::EnterFloating(const FInstancedStruct& InParameter)
{
	return false;
}

bool UBXSMStun::ExitFloating(const FInstancedStruct& InParameter)
{
	return false;
}

bool UBXSMStun::EnterExecution(const FInstancedStruct& InParameter)
{
	return false;
}

bool UBXSMStun::ExitExecution(const FInstancedStruct& InParameter)
{
	return false;
}
	
#pragma endregion Stun
