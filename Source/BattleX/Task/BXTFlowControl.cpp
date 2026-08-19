#include "BXTFlowControl.h"
#include "BXGameplayTags.h"

UBXTSwitch::UBXTSwitch()
{
	LifeType = EBXTLifeType::L_Instant;
	// 保留基类预置的Start/End条目(StartTask/EndTask末尾无条件以这两个Tag调用AddPendingTask,
	// 清掉会令每次触发刷"EventTag not found"警告)
	Events.Empty();
	Events.Add(BXGameplayTags::BXTEvent_Start);
	Events.Add(BXGameplayTags::BXTEvent_End);
	DefaultEventTag = BXGameplayTags::BXTEvent_BranchDefault;
}

#if WITH_EDITOR
void UBXTSwitch::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	if (PropertyChangedEvent.Property)
	{
		const FName ChangedName = PropertyChangedEvent.Property->GetFName();
		if (ChangedName == GET_MEMBER_NAME_CHECKED(UBXTSwitch, Cases) || ChangedName == GET_MEMBER_NAME_CHECKED(UBXTSwitch, DefaultEventTag) || ChangedName == GET_MEMBER_NAME_CHECKED(FBXTSwitchCase, EventTag))
		{
			// Cases变更时，为EventTag无效的Case分配未使用的Branch tag（从小序号开始）
			if (ChangedName == GET_MEMBER_NAME_CHECKED(UBXTSwitch, Cases))
			{
				const FGameplayTag BranchTags[] = { BXGameplayTags::BXTEvent_Branch01, BXGameplayTags::BXTEvent_Branch02, BXGameplayTags::BXTEvent_Branch03, BXGameplayTags::BXTEvent_Branch04, BXGameplayTags::BXTEvent_Branch05, BXGameplayTags::BXTEvent_Branch06, BXGameplayTags::BXTEvent_Branch07, BXGameplayTags::BXTEvent_Branch08, BXGameplayTags::BXTEvent_Branch09, BXGameplayTags::BXTEvent_Branch10, BXGameplayTags::BXTEvent_Branch11, BXGameplayTags::BXTEvent_Branch12, BXGameplayTags::BXTEvent_Branch13, BXGameplayTags::BXTEvent_Branch14, BXGameplayTags::BXTEvent_Branch15, BXGameplayTags::BXTEvent_Branch16 };

				TSet<FGameplayTag> UsedTags;
				for (const FBXTSwitchCase& Case : Cases)
				{
					if (Case.EventTag.IsValid())
					{
						UsedTags.Add(Case.EventTag);
					}
				}

				for (FBXTSwitchCase& Case : Cases)
				{
					if (!Case.EventTag.IsValid())
					{
						for (const FGameplayTag& Tag : BranchTags)
						{
							if (!UsedTags.Contains(Tag))
							{
								Case.EventTag = Tag;
								UsedTags.Add(Tag);
								break;
							}
						}
					}
				}
			}

			// 重建Events：Cases中所有EventTag + DefaultEventTag(保留基类预置的Start/End,否则StartTask/EndTask触发时刷警告)
			Events.Empty();
			Events.Add(BXGameplayTags::BXTEvent_Start);
			Events.Add(BXGameplayTags::BXTEvent_End);
			for (const FBXTSwitchCase& Case : Cases)
			{
				if (Case.EventTag.IsValid())
				{
					Events.Add(Case.EventTag, FBXTEvent());
				}
			}
			if (DefaultEventTag.IsValid() && !Events.Contains(DefaultEventTag))
			{
				Events.Add(DefaultEventTag, FBXTEvent());
			}
		}
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif
