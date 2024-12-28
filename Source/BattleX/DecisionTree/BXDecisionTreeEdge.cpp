#include "BXDecisionTreeEdge.h"

#include "BXFunctionLibrary.h"
#include "BXConditionManager.h"



bool UBXDecisionTreeEdge::CheckCondition(UObject* InWorldContext, UScriptStruct* InParameterType, void* InParameterAddress)
{
	if (!IsValid(InWorldContext))
	{
		return false;
	}

	UBXConditionManager* ConditionManager = UBXConditionManager::Get(InWorldContext);
	if (!IsValid(ConditionManager))
	{
		return false;
	}

	return ConditionManager->CheckCondition(Condition, InParameterType, InParameterAddress);
}

#if WITH_EDITOR
void UBXDecisionTreeEdge::ConstructEdge()
{
	RefreshEdgeTitle();
}

void UBXDecisionTreeEdge::CopyData(UBXDecisionTreeEdge* OtherEdge)
{
	if (!OtherEdge)
	{
		return;
	}

	if (OtherEdge->Condition)
	{
		Condition = NewObject<UBXCondition>(this, OtherEdge->Condition->GetClass());
		UBXFunctionLibrary::CopyObject(Condition, OtherEdge->Condition);
	}

	RefreshEdgeTitle();
}

void UBXDecisionTreeEdge::RefreshEdgeTitle()
{
	if (Condition)
	{
		EdgeTitle = FText::FromString(Condition->GetDescription());
	}
}

FText UBXDecisionTreeEdge::GetEdgeTitle() const
{
	return EdgeTitle;
}

void UBXDecisionTreeEdge::SetEdgeTitle(const FText& NewTitle)
{
	EdgeTitle = NewTitle;
}

void UBXDecisionTreeEdge::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	RefreshEdgeTitle();

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif
