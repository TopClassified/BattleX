#include "BXBeatenTreeCondition.h"



UBXBeatenTreeCondition::UBXBeatenTreeCondition()
{
	
}

#if WITH_EDITOR
FString UBXBeatenTreeCondition::GetDescription()
{
	FString Result;
	if (bCheckStunState)
	{
		for (int32 i = 0; i < StunState.Num(); ++i)
		{
			Result += StunState.GetByIndex(i).ToString();
			Result += TEXT(" ");
		}
		Result += TEXT("\n");
	}

	if (bCheckAttackType)
	{
		for (int32 i = 0; i < AttackType.Num(); ++i)
		{
			Result += AttackType.GetByIndex(i).ToString();
			Result += TEXT(" ");
		}
		Result += TEXT("\n");
	}

	if (bCheckAttackForce)
	{
		for (int32 i = 0; i < AttackForce.Num(); ++i)
		{
			Result += AttackForce.GetByIndex(i).ToString();
			Result += TEXT(" ");
		}
		Result += TEXT("\n");
	}

	if (bCheckAttackRPos)
	{
		for (int32 i = 0; i < AttackRPos.Num(); ++i)
		{
			Result += AttackRPos.GetByIndex(i).ToString();
			Result += TEXT(" ");
		}
		Result += TEXT("\n");
	}

	return Result;
}
#endif