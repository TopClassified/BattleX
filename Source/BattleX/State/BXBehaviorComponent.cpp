#include "BXBehaviorComponent.h"

#include "BXGameplayTags.h"
#include "BXEventStructs.h"
#include "BXEventManager.h"



DEFINE_LOG_CATEGORY(BXCOMP_Behavior);



#pragma region Important
UBXBehaviorComponent::UBXBehaviorComponent()
{

}

void UBXBehaviorComponent::BeginPlay()
{
	// 初始化行为代理实例
	for (TMap<FGameplayTag, TSubclassOf<UBXBehaviorAgent>>::TIterator It(BehaviorAgentConfigs); It; ++It)
	{
		if (!IsValid(It->Value))
		{
			continue;
		}

		UBXBehaviorAgent* NewAgent = NewObject<UBXBehaviorAgent>(this, It->Value);
		if (!IsValid(NewAgent))
		{
			continue;
		}

		NewAgent->Initialize();
		BehaviorAgents.Add(It->Key, NewAgent);
	}
	
	Super::BeginPlay();
}

void UBXBehaviorComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	for (TMap<FGameplayTag, UBXBehaviorAgent*>::TIterator It(BehaviorAgents); It; ++It)
	{
		if (!IsValid(It->Value))
		{
			continue;
		}

		It->Value->Deinitialize();
		It->Value->MarkAsGarbage();
	}
	BehaviorAgents.Empty();
	
	Super::EndPlay(EndPlayReason);
}

#pragma endregion Important



#pragma region API
bool UBXBehaviorComponent::CheckForbiddenBehavior(const FGameplayTag& InBehaviorTag)
{
	return ForbiddenBehaviors.Contains(InBehaviorTag);
}

bool UBXBehaviorComponent::CheckForbiddenBehaviorWithReason(const FGameplayTag& InBehaviorTag, EBXForbiddenBehaviorReason& OutReason)
{
	if (FBXForbiddenBehaviorInformationList* Result = ForbiddenBehaviors.Find(InBehaviorTag))
	{
		if (!Result->List.IsEmpty())
		{
			OutReason = Result->List.Last().Reason;
			return true;	
		}
	}

	return false;
}

void UBXBehaviorComponent::ChangeForbiddenBehavior(const FGameplayTag& InBehaviorTag, bool bForbidden, int64 InSign, EBXForbiddenBehaviorReason InReason)
{
	bool bNeedCallback = false;
	
	if (bForbidden)
	{
		if (FBXForbiddenBehaviorInformationList* FindResult = ForbiddenBehaviors.Find(InBehaviorTag))
		{
			FindResult->List.Add(FBXForbiddenBehaviorInformation(InSign, InReason));
		}
		else
		{
			FBXForbiddenBehaviorInformationList Temp;
			Temp.List.Add(FBXForbiddenBehaviorInformation(InSign, InReason));
			ForbiddenBehaviors.Add(InBehaviorTag, Temp);
			bNeedCallback = true;
		}
	}
	else
	{
		if (FBXForbiddenBehaviorInformationList* FindResult = ForbiddenBehaviors.Find(InBehaviorTag))
		{
			// 移除所有同来源同原因的条目(同一(Sign,Reason)可被重复添加,单条Remove会残留脏条目导致行为仍被禁止)
			FBXForbiddenBehaviorInformation FBI(InSign, InReason);
			FindResult->List.RemoveAll([&FBI](const FBXForbiddenBehaviorInformation& Item) { return Item == FBI; });

			if (FindResult->List.IsEmpty())
			{
				ForbiddenBehaviors.Remove(InBehaviorTag);
				bNeedCallback = true;
			}
		}
	}

	UBXEventManager* BXEMgr = UBXEventManager::Get(this);
	if (bNeedCallback && IsValid(BXEMgr))
	{
		FBXEventForbiddenBehavior EventParameter(InBehaviorTag, bForbidden, InReason);
		BXEMgr->BroadcastSingleEvent<FBXEventForbiddenBehavior>(BXGameplayTags::BXEvent_ChangeForbiddenBehavior, this, EventParameter);
	}
}

bool UBXBehaviorComponent::CheckActiveBehavior(const FGameplayTag& InBehaviorTag)
{
	// 原实现误查禁止表(行为被禁止时误报激活中,真正激活时误报未激活),正确语义是查询激活记录
	return ActiveBehaviors.HasTag(InBehaviorTag);
}

bool UBXBehaviorComponent::StartBehavior(const FGameplayTag& InBehaviorTag)
{
	FInstancedStruct IS;
	return StartBehaviorWithParameter(InBehaviorTag, IS);
}

bool UBXBehaviorComponent::StartBehaviorWithParameter(const FGameplayTag& InBehaviorTag, const FInstancedStruct& InParameter)
{
	bool bResult = false;
	
	if (CheckForbiddenBehavior(InBehaviorTag))
	{
		UE_LOG(BXCOMP_Behavior, Log, TEXT("Start Behavior(%s) Failed! The Behavior Is Forbidden!"), *InBehaviorTag.GetTagName().ToString());
		return bResult;
	}

	if (UBXBehaviorAgent** FindResult = BehaviorAgents.Find(InBehaviorTag))
	{
		UBXBehaviorAgent* Agent = *FindResult;
		if (!IsValid(Agent))
		{
			UE_LOG(BXCOMP_Behavior, Log, TEXT("Start Behavior(%s) Failed! The Behavior Agent Does Not Exist!"), *InBehaviorTag.GetTagName().ToString());
			return bResult;
		}

		if (!Agent->CheckStartBehavior(InParameter))
		{
			UE_LOG(BXCOMP_Behavior, Log, TEXT("Start Behavior(%s) Failed! The Behavior Can't Start!"), *InBehaviorTag.GetTagName().ToString());
			return bResult;
		}

		// 禁用其他行为(bForbidden=true为添加禁止,原实现误传false变成解除禁止,行为禁用体系整体颠倒)
		TArray<FGameplayTag> ForbiddenTags;
		if (FGameplayTagContainer* Container = ForbiddenBehaviorConfigs.Find(InBehaviorTag))
		{
			for (int32 i = 0; i < Container->Num(); ++i)
			{
				const FGameplayTag& Tag = Container->GetByIndex(i);
				ChangeForbiddenBehavior(Tag, true, Agent->GetUniqueKey(), EBXForbiddenBehaviorReason::FB_OtherBehavior);
				ForbiddenTags.Add(Tag);
			}
		}

		// 中断其他行为
		if (FGameplayTagContainer* Container = InterruptBehaviorConfigs.Find(InBehaviorTag))
		{
			for (int32 i = 0; i < Container->Num(); ++i)
			{
				const FGameplayTag& Tag = Container->GetByIndex(i);
				StopBehavior(Tag);
			}
		}

		// 开始行为
		bResult = Agent->StartBehavior(InParameter);
		if (bResult)
		{
			// 非瞬时行为，要记录状态(原实现每次按名查未注册的根Tag,查找失败返回空Tag导致永不记录,Stop链路整体失效)
			if (InBehaviorTag.MatchesTag(BXGameplayTags::BXBehavior_Root))
			{
				ActiveBehaviors.AddTag(InBehaviorTag);
			}
		}
		else
		{
			// 启动失败回滚已添加的禁止条目(否则关联行为被无谓永久禁止)
			for (const FGameplayTag& Tag : ForbiddenTags)
			{
				ChangeForbiddenBehavior(Tag, false, Agent->GetUniqueKey(), EBXForbiddenBehaviorReason::FB_OtherBehavior);
			}

			UE_LOG(BXCOMP_Behavior, Log, TEXT("Start Behavior(%s) Failed!"), *InBehaviorTag.GetTagName().ToString());
		}
	}

	return bResult;
}

bool UBXBehaviorComponent::StopBehavior(const FGameplayTag& InBehaviorTag)
{
	FInstancedStruct IS;
	return StopBehaviorWithParameter(InBehaviorTag, IS);
}

bool UBXBehaviorComponent::StopBehaviorWithParameter(const FGameplayTag& InBehaviorTag, const FInstancedStruct& InParameter)
{
	bool bResult = false;

	if (!ActiveBehaviors.HasTag(InBehaviorTag))
	{
		UE_LOG(BXCOMP_Behavior, Log, TEXT("Stop Behavior(%s) Failed! The Behavior Is Not Active!"), *InBehaviorTag.GetTagName().ToString());
		return bResult;
	}

	if (UBXBehaviorAgent** FindResult = BehaviorAgents.Find(InBehaviorTag))
	{
		UBXBehaviorAgent* Agent = *FindResult;
		if (!IsValid(Agent))
		{
			UE_LOG(BXCOMP_Behavior, Log, TEXT("Stop Behavior(%s) Failed! The Behavior Agent Does Not Exist!"), *InBehaviorTag.GetTagName().ToString());
			return bResult;
		}

		// 停止行为
		bResult = Agent->StopBehavior(InParameter);

		if (bResult)
		{
			// 启用其他行为(bForbidden=false为解除禁止,原实现误传true变成添加禁止,行为停止后关联行为被永久禁止)
			if (FGameplayTagContainer* Container = ForbiddenBehaviorConfigs.Find(InBehaviorTag))
			{
				for (int32 i = 0; i < Container->Num(); ++i)
				{
					const FGameplayTag& Tag = Container->GetByIndex(i);
					ChangeForbiddenBehavior(Tag, false, Agent->GetUniqueKey(), EBXForbiddenBehaviorReason::FB_OtherBehavior);
				}
			}

			// 从列表中移除
			ActiveBehaviors.RemoveTag(InBehaviorTag);
		}
		else
		{
			UE_LOG(BXCOMP_Behavior, Log, TEXT("Stop Behavior(%s) Failed!"), *InBehaviorTag.GetTagName().ToString());
		}
	}

	return bResult;
}

#pragma endregion API
