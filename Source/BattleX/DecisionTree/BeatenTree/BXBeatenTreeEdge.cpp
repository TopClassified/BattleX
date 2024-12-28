#include "BXBeatenTreeEdge.h"
#include "BXBeatenTreeCondition.h"



UBXBeatenTreeEdge::UBXBeatenTreeEdge()
{

#if WITH_EDITORONLY_DATA
	EdgeMessageSize = FVector2D(150.0f, 50.0f);
#endif

}

#if WITH_EDITOR
void UBXBeatenTreeEdge::ConstructEdge()
{
	Super::ConstructEdge();

	// 创建ConditionGroup
	if (!IsValid(Condition))
	{
		Condition = NewObject<UBXBeatenTreeCondition>(this);
	}

	CheckConditionConfig();
}

void UBXBeatenTreeEdge::CheckConditionConfig()
{
	if (!IsValid(Condition))
	{
		return;
	}
	
	if (UBXBeatenTreeCondition* EdgeCondition = Cast<UBXBeatenTreeCondition>(Condition))
	{
		// 根据深度检查配置数据是否正确
		if (EdgeShortestDepth == 1)
		{
			EdgeCondition->bCheckStunState = true;
			EdgeCondition->bCheckAttackType = false;
			EdgeCondition->bCheckAttackForce = false;
			EdgeCondition->bCheckAttackRPos = false;
		}
		else if (EdgeShortestDepth == 2)
		{
			EdgeCondition->bCheckStunState = false;
			EdgeCondition->bCheckAttackType = true;
			EdgeCondition->bCheckAttackForce = false;
			EdgeCondition->bCheckAttackRPos = false;
		}
		else if (EdgeShortestDepth == 3)
		{
			EdgeCondition->bCheckStunState = false;
			EdgeCondition->bCheckAttackType = false;
			EdgeCondition->bCheckAttackForce = true;
			EdgeCondition->bCheckAttackRPos = false;
		}
		else if (EdgeShortestDepth == 4)
		{
			EdgeCondition->bCheckStunState = false;
			EdgeCondition->bCheckAttackType = false;
			EdgeCondition->bCheckAttackForce = false;
			EdgeCondition->bCheckAttackRPos = true;
		}
		else
		{
			EdgeCondition->bCheckStunState = false;
			EdgeCondition->bCheckAttackType = false;
			EdgeCondition->bCheckAttackForce = false;
			EdgeCondition->bCheckAttackRPos = false;
		}
	}
}

void UBXBeatenTreeEdge::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	CheckConditionConfig();

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif
