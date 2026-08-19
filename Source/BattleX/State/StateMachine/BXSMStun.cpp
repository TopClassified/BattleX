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

	// 进入新的状态(原实现仅Normal分支接收返回值,其余7个分支的进入结果全部被丢弃)
	if (InState == BXGameplayTags::BXStunState_Normal)
	{
		bResult = EnterNormal(InParameter);
	}
	else if (InState == BXGameplayTags::BXStunState_Knockback)
	{
		bResult = EnterKnockback(InParameter);
	}
	else if (InState == BXGameplayTags::BXStunState_Knockdown)
	{
		bResult = EnterKnockdown(InParameter);
	}
	else if (InState == BXGameplayTags::BXStunState_Prone)
	{
		bResult = EnterProne(InParameter);
	}
	else if (InState == BXGameplayTags::BXStunState_Ascending)
	{
		bResult = EnterAscending(InParameter);
	}
	else if (InState == BXGameplayTags::BXStunState_Falling)
	{
		bResult = EnterFalling(InParameter);
	}
	else if (InState == BXGameplayTags::BXStunState_Floating)
	{
		bResult = EnterFloating(InParameter);
	}
	else if (InState == BXGameplayTags::BXStunState_Execution)
	{
		bResult = EnterExecution(InParameter);
	}

	// 进入成功才更新当前状态(原实现条件写反,进入成功反而不更新,失败却更新,状态跟踪与实际脱节)
	if (bResult)
	{
		CurrentState = InState;
	}

	return bResult;
}

bool UBXSMStun::NativeExitState(const FGameplayTag& InState, const FInstancedStruct& InParameter)
{
	bool bResult = false;

	if (InState == BXGameplayTags::BXStunState_Normal)
	{
		bResult = ExitNormal(InParameter);
	}
	else if (InState == BXGameplayTags::BXStunState_Knockback)
	{
		bResult = ExitKnockback(InParameter);
	}
	else if (InState == BXGameplayTags::BXStunState_Knockdown)
	{
		bResult = ExitKnockdown(InParameter);
	}
	else if (InState == BXGameplayTags::BXStunState_Prone)
	{
		bResult = ExitProne(InParameter);
	}
	else if (InState == BXGameplayTags::BXStunState_Ascending)
	{
		bResult = ExitAscending(InParameter);
	}
	else if (InState == BXGameplayTags::BXStunState_Falling)
	{
		bResult = ExitFalling(InParameter);
	}
	else if (InState == BXGameplayTags::BXStunState_Floating)
	{
		bResult = ExitFloating(InParameter);
	}
	else if (InState == BXGameplayTags::BXStunState_Execution)
	{
		bResult = ExitExecution(InParameter);
	}

	// 退出成功后复位到默认态(否则CurrentState残留已退出的状态,再次进入同状态时跳过Exit直接进入,进入/退出配对断裂)
	if (bResult)
	{
		CurrentState = BXGameplayTags::BXStunState_Normal;
	}

	return bResult;
}

#pragma endregion State



#pragma region Stun
// 各状态的具体进入/退出效果待实现,存根返回true(与UBXStateMachine基类NativeEnterState/NativeExitState默认语义一致:
// 返回false会使EnterState/ExitState对外恒报失败,状态跟踪与上层调用全部失效)
bool UBXSMStun::EnterNormal(const FInstancedStruct& InParameter)
{
	return true;
}

bool UBXSMStun::ExitNormal(const FInstancedStruct& InParameter)
{
	return true;
}

bool UBXSMStun::EnterKnockback(const FInstancedStruct& InParameter)
{
	return true;
}

bool UBXSMStun::ExitKnockback(const FInstancedStruct& InParameter)
{
	return true;
}

bool UBXSMStun::EnterKnockdown(const FInstancedStruct& InParameter)
{
	return true;
}

bool UBXSMStun::ExitKnockdown(const FInstancedStruct& InParameter)
{
	return true;
}

bool UBXSMStun::EnterProne(const FInstancedStruct& InParameter)
{
	return true;
}

bool UBXSMStun::ExitProne(const FInstancedStruct& InParameter)
{
	return true;
}

bool UBXSMStun::EnterAscending(const FInstancedStruct& InParameter)
{
	return true;
}

bool UBXSMStun::ExitAscending(const FInstancedStruct& InParameter)
{
	return true;
}

bool UBXSMStun::EnterFalling(const FInstancedStruct& InParameter)
{
	return true;
}

bool UBXSMStun::ExitFalling(const FInstancedStruct& InParameter)
{
	return true;
}

bool UBXSMStun::EnterFloating(const FInstancedStruct& InParameter)
{
	return true;
}

bool UBXSMStun::ExitFloating(const FInstancedStruct& InParameter)
{
	return true;
}

bool UBXSMStun::EnterExecution(const FInstancedStruct& InParameter)
{
	return true;
}

bool UBXSMStun::ExitExecution(const FInstancedStruct& InParameter)
{
	return true;
}

#pragma endregion Stun
