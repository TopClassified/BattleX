#include "BXTAnimation.h"



#pragma region Editor
#if WITH_EDITOR
bool UBXTPlayAnimation::EnablePassiveTrigger()
{
	return false;
}

void UBXTPlayAnimation::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	if (LifeType == EBXTLifeType::L_Instant || LifeType == EBXTLifeType::L_Timeline)
	{
		LifeType = EBXTLifeType::L_DurationTimeline;
	}

	if (AssetType == EBXTAnimationAssetType::AAT_Montage)
	{
		// PlayRate无下限约束,0时除零得inf被烘焙进资产,任务永不结束
		Duration = IsValid(Montage) ? (Montage->GetPlayLength() / FMath::Max(PlayRate, 0.01f)) : 1.0f;
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif
#pragma endregion Editor
