#include "BXCombatTreeEdge.h"



#if WITH_EDITOR
void UBXCombatTreeEdge::CopyData(UBXDecisionTreeEdge* OtherNode)
{
	Super::CopyData(OtherNode);

	if (UBXCombatTreeEdge* Other = Cast<UBXCombatTreeEdge>(OtherNode))
	{
		ComboWindows.Empty();
		ComboWindows.Append(Other->ComboWindows);
	}
}

void UBXCombatTreeEdge::RefreshEdgeTitle()
{
	FString Title;

	if (ComboWindows.Num() > 0)
	{
		EdgeMessageSize.X = 200.0f;
		for (int32 i = 0; i < ComboWindows.Num(); ++i)
		{
			Title += TEXT("After ");
			Title += ComboWindows[i].GetAssetName();
			if (UBXSkillAsset* Skill = ComboWindows[i].LoadSynchronous())
			{
				Title += TEXT("  ");
				Title += Skill->Name.ToString();
			}
			Title += TEXT("\n");
		}
	}
	else
	{
		EdgeMessageSize.X = 100.0f;
		Title = TEXT("No Condition");
	}

	EdgeMessageSize.Y = FMath::Max(30.0f, ComboWindows.Num() * 16.0f);
	EdgeTitle = FText::FromString(Title);
}

void UBXCombatTreeEdge::SetEdgeTitle(const FText& NewTitle)
{

}

void UBXCombatTreeEdge::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	RefreshEdgeTitle();

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif
