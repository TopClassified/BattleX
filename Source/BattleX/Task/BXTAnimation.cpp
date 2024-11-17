#include "BXTAnimation.h"



UBXTPlayAnimation::UBXTPlayAnimation()
{
	DisplayName = FText::FromString(TEXT("播放动画"));
}

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
		Duration = IsValid(Montage) ? (Montage->GetPlayLength() / PlayRate) : 1.0f;
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif
#pragma endregion Editor
