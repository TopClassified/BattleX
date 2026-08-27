#include "BXBehaviorSettings.h"

#include "BXGameplayTags.h"



EBXBehaviorRelation UBXBehaviorSettings::GetRelation(const FGameplayTag& InEntering, const FGameplayTag& InExisting) const
{
	if (InEntering == InExisting || !InEntering.IsValid() || !InExisting.IsValid())
	{
		return EBXBehaviorRelation::BR_None;
	}

	return FindRelation(InEntering, InExisting);
}

void UBXBehaviorSettings::GetExpelTargets(const FGameplayTag& InEntering, TArray<FGameplayTag>& OutTags) const
{
	OutTags.Reset();

	if (!InEntering.IsValid())
	{
		return;
	}

	if (const FGameplayTagContainer* Container = ExpelRelations.Find(InEntering))
	{
		for (int32 i = 0; i < Container->Num(); ++i)
		{
			OutTags.Add(Container->GetByIndex(i));
		}
	}

	// 族Tag自身可作关系键(如注册BXBehavior.Locomotion行为族)
	for (const TPair<FGameplayTag, FGameplayTagContainer>& Pair : ExpelRelations)
	{
		if (InEntering.MatchesTag(Pair.Key))
		{
			for (int32 i = 0; i < Pair.Value.Num(); ++i)
			{
				OutTags.AddUnique(Pair.Value.GetByIndex(i));
			}
		}
	}
}

bool UBXBehaviorSettings::IsRejectedByAny(const FGameplayTag& InEntering, const FGameplayTagContainer& InActiveBehaviors) const
{
	if (!InEntering.IsValid() || InActiveBehaviors.IsEmpty())
	{
		return false;
	}

	for (const TPair<FGameplayTag, FGameplayTagContainer>& Pair : RejectRelations)
	{
		// 行Tag匹配:精确命中或进入行为是行Tag的族成员
		if (InEntering.MatchesTag(Pair.Key))
		{
			// 列存在检查:活跃集合中任一行为命中列Tag(列方向同样支持族匹配)
			for (int32 i = 0; i < Pair.Value.Num(); ++i)
			{
				const FGameplayTag& RejectTag = Pair.Value.GetByIndex(i);
				for (int32 j = 0; j < InActiveBehaviors.Num(); ++j)
				{
					if (InActiveBehaviors.GetByIndex(j).MatchesTag(RejectTag))
					{
						return true;
					}
				}
			}
		}
	}

	return false;
}

EBXBehaviorRelation UBXBehaviorSettings::FindRelation(const FGameplayTag& InEntering, const FGameplayTag& InExisting) const
{
	// 挤出优先(同一对配置了挤出与拒绝时,挤出语义更具体)
	if (ExpelRelations.Contains(InEntering))
	{
		if (const FGameplayTagContainer* Container = ExpelRelations.Find(InEntering))
		{
			if (InExisting.MatchesAny(*Container))
			{
				return EBXBehaviorRelation::BR_Expel;
			}
		}
	}

	if (RejectRelations.Contains(InEntering))
	{
		if (const FGameplayTagContainer* Container = RejectRelations.Find(InEntering))
		{
			if (InExisting.MatchesAny(*Container))
			{
				return EBXBehaviorRelation::BR_Reject;
			}
		}
	}

	// 族Tag方向匹配:行Tag为族,进入行为为其子Tag时继承关系
	for (const TPair<FGameplayTag, FGameplayTagContainer>& Pair : ExpelRelations)
	{
		if (InEntering.MatchesTag(Pair.Key) && InExisting.MatchesAny(Pair.Value))
		{
			return EBXBehaviorRelation::BR_Expel;
		}
	}

	for (const TPair<FGameplayTag, FGameplayTagContainer>& Pair : RejectRelations)
	{
		if (InEntering.MatchesTag(Pair.Key) && InExisting.MatchesAny(Pair.Value))
		{
			return EBXBehaviorRelation::BR_Reject;
		}
	}

	return EBXBehaviorRelation::BR_None;
}
