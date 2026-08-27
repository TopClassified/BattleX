#include "BXSkillAsset.h"



bool UBXSkillAsset::IsInCancelWindow(float InTimelineTime) const
{
	for (const FBXSkillCancelWindow& Window : CancelWindows)
	{
		if (Window.Contains(InTimelineTime))
		{
			return true;
		}
	}

	return false;
}
