#include "BXTaskCondition.h"



#pragma region TaskConditionParameterHash
uint32 GetTypeHash(const FBXTaskConditionParameter& InParam)
{
	return HashCombineFast(
		GetTypeHash(InParam.Owner),
		GetTypeHash(InParam.Instigator)
	);
}
#pragma endregion TaskConditionParameterHash
