#include "BXTPAnimation.h"

#include "BXTAnimation.h"
#include "BXAnimationLibrary.h"
#include "BXBehaviorComponent.h"



void UBXTPPlayAnimation::Start(FBXTLRunTimeData& InOutRTData, FBXTLSectionRTData& InOutRTSData, FBXTLTaskRTData& InOutRTTData, UBXTask* InTask)
{
	UBXTPlayAnimation* Task = Cast<UBXTPlayAnimation>(InTask);
	if (!Task)
	{
		return;
	}
	
	// 获取任务的自定义数据结构
	FBXTPPlayAnimationContext& TPC = InOutRTTData.DynamicData.GetMutable<FBXTPPlayAnimationContext>();

	// 获取任务目标
	TArray<AActor*> Targets;
	UBXTProcessor::GetTargetActorList(InOutRTData, InTask, Targets);
	
	// 尝试获取播放动画的权限并进行播放
	for (TArray<AActor*>::TIterator It(Targets); It; ++It)
	{
		// 获取播放动画的组件
		USkeletalMeshComponent* TComponent = Cast<USkeletalMeshComponent>(UBXFunctionLibrary::GetSceneComponentByNameAndClass(*It, Task->PlayComponentName, USkeletalMeshComponent::StaticClass()));
		TPC.TComponents.Add(TComponent);
		if (!IsValid(TComponent))
		{
			TPC.Parameters.Add(FBXTPPAContextParameter());
			continue;
		}
		
		// 尝试获取动画播放权限
		int64 Permission = UBXBehaviorComponent::GetBehaviorPermission(*It, Task->PlayAnimBehaviorTag, Task->PlayPriority);
		TPC.Parameters.Add(FBXTPPAContextParameter(Permission, -1));
		if (Permission < 0)
		{
			continue;
		}
		
		// 获取要播放的蒙太奇
		UAnimMontage* Montage = nullptr;
		if (Task->AssetType == EBXTAnimationAssetType::AAT_Montage)
		{
			Montage = Task->Montage;
		}
		else if (Task->AssetType == EBXTAnimationAssetType::AAT_Library)
		{
			Montage = UBXAnimationLibrary::GetMontageFromActorByTag(*It, Task->Tag);
		}
		if (!IsValid(Montage))
		{
			continue;
		}

		// 播放动画
		if (UAnimInstance* AnimIns = TComponent->GetAnimInstance())
		{
			AnimIns->Montage_Play(Montage, Task->PlayRate * InOutRTData.RunRate, EMontagePlayReturnType::MontageLength, InOutRTTData.RunTime, Task->bStopGroup);
			if (FAnimMontageInstance* MontageInstance = AnimIns->GetInstanceForMontage(Montage))
			{
				if (!Task->bEnableRootmotion)
				{
					MontageInstance->PushDisableRootMotion();
				}

				TPC.Parameters.Last().MontageInstanceID = MontageInstance->GetInstanceID();
			}
		}
	}

	// 设置下一次更新时间
	InOutRTTData.NextTick = Task->DelayInterruptedByMove;
}

void UBXTPPlayAnimation::Update(FBXTLRunTimeData& InOutRTData, FBXTLSectionRTData& InOutRTSData, FBXTLTaskRTData& InOutRTTData, UBXTask* InTask, float InDeltaTime)
{
	UBXTPlayAnimation* Task = Cast<UBXTPlayAnimation>(InTask);
	if (!Task)
	{
		return;
	}
	
	// 获取任务的自定义数据结构
	FBXTPPlayAnimationContext& TPC = InOutRTTData.DynamicData.GetMutable<FBXTPPlayAnimationContext>();

	// 判断是否正在主动移动
	FGameplayTag PMovingTag = FGameplayTag::RequestGameplayTag(TEXT("BXBehavior.ProactiveMoving"));
	for (TArray<USkeletalMeshComponent*>::TIterator It(TPC.TComponents); It; ++It)
	{
		UAnimInstance* AnimIns = (*It)->GetAnimInstance();
		if (!IsValid(AnimIns) || !TPC.Parameters.IsValidIndex(It.GetIndex()))
		{
			continue;
		}
		
		AActor* Owner = (*It)->GetOwner();
		if (!IsValid(Owner))
		{
			continue;
		}

		if (!UBXBehaviorComponent::IsCurrentlyPerformingBehavior(Owner, PMovingTag))
		{
			continue;
		}

		// 停止蒙太奇播放
		if (FAnimMontageInstance* MIns = AnimIns->GetMontageInstanceForID(TPC.Parameters[It.GetIndex()].MontageInstanceID))
		{
			AnimIns->Montage_Stop(Task->InterruptedBlendOut, MIns->Montage);
		}
	}
	
	// 设置下一次更新时间
	InOutRTTData.NextTick = 0.1f;
}
	
void UBXTPPlayAnimation::End(FBXTLRunTimeData& InOutRTData, FBXTLSectionRTData& InOutRTSData, FBXTLTaskRTData& InOutRTTData, UBXTask* InTask, EBXTLFinishReason InReason)
{
	// 获取任务的自定义数据结构
	FBXTPPlayAnimationContext& TPC = InOutRTTData.DynamicData.GetMutable<FBXTPPlayAnimationContext>();

	UBXTPlayAnimation* Task = Cast<UBXTPlayAnimation>(InTask);
	float BlendOut = IsValid(Task) ? Task->InterruptedBlendOut : 0.0f;
	
	for (TArray<USkeletalMeshComponent*>::TIterator It(TPC.TComponents); It; ++It)
	{
		UAnimInstance* AnimIns = (*It)->GetAnimInstance();
		if (!IsValid(AnimIns) || !TPC.Parameters.IsValidIndex(It.GetIndex()))
		{
			continue;
		}
		
		AActor* Owner = (*It)->GetOwner();
		if (!IsValid(Owner))
		{
			continue;
		}

		// 停止蒙太奇播放
		if (FAnimMontageInstance* MIns = AnimIns->GetMontageInstanceForID(TPC.Parameters[It.GetIndex()].MontageInstanceID))
		{
			AnimIns->Montage_Stop(BlendOut, MIns->Montage);
		}

		// 归还行为权限
		UBXBehaviorComponent::RevokeBehaviorPermission(Owner, Task->PlayAnimBehaviorTag, TPC.Parameters[It.GetIndex()].Permission);
	}
}

void UBXTPPlayAnimation::ChangeTickRate(FBXTLTaskRTData& InOutRTTData, UBXTask* InTask, float InRate)
{
	UBXTPlayAnimation* Task = Cast<UBXTPlayAnimation>(InTask);
	if (!Task)
	{
		return;
	}
	
	// 获取任务的自定义数据结构
	FBXTPPlayAnimationContext& TPC = InOutRTTData.DynamicData.GetMutable<FBXTPPlayAnimationContext>();

	for (TArray<USkeletalMeshComponent*>::TIterator It(TPC.TComponents); It; ++It)
	{
		UAnimInstance* AnimIns = (*It)->GetAnimInstance();
		if (!IsValid(AnimIns) || !TPC.Parameters.IsValidIndex(It.GetIndex()))
		{
			continue;
		}

		// 设置播放速率
		if (FAnimMontageInstance* MIns = AnimIns->GetMontageInstanceForID(TPC.Parameters[It.GetIndex()].MontageInstanceID))
		{
			MIns->SetPlayRate(Task->PlayRate * InRate);
		}
	}
}
