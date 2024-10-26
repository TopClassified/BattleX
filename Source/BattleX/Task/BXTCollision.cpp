#include "BXTCollision.h"



#if WITH_EDITOR
void UBXTTrackWeaponCollision::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	if (LifeType == EBXTLifeType::L_Instant)
	{
		LifeType = EBXTLifeType::L_DurationTimeline;
	}

	if (TargetTypes > 0)
	{
		// 选择了多个目标
		if ((TargetTypes & (TargetTypes - 1)) != 0)
		{
			bCacheRTransform = false;
		}

		// 选择了碰撞目标
		if ((TargetTypes & (1 << (int32)EBXTTargetType::T_CollisionResults)) != 0)
		{
			bCacheRTransform = false;
		}
	}
	
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
	
#endif
