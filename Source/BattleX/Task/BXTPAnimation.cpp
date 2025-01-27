#include "BXTPAnimation.h"

#include "BXTAnimation.h"
#include "BXAnimInstance.h"
#include "BXAnimationLibrary.h"
#include "BXStateFunctionLibrary.h"



void UBXTPPlayAnimation::Start(FBXTLRunTimeData& InOutRTData, FBXTLSectionRTData& InOutRTSData, FBXTLTaskRTData& InOutRTTData)
{
	UBXTPlayAnimation* Task = Cast<UBXTPlayAnimation>(InOutRTTData.Task);
	if (!Task)
	{
		return;
	}
	
	// 获取任务的自定义数据结构
	FBXTPPlayAnimationContext& TPC = InOutRTTData.DynamicData.GetMutable<FBXTPPlayAnimationContext>();

	// 获取任务目标
	TArray<AActor*> Targets;
	UBXTProcessor::GetTargetActorList(InOutRTData, InOutRTTData, Targets);
	
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

		UBXAnimInstance* AnimIns = Cast<UBXAnimInstance>(TComponent->GetAnimInstance());
		if (!AnimIns)
		{
			TPC.Parameters.Add(FBXTPPAContextParameter());
			continue;
		}
		
		// 尝试获取动画播放权限
		int64 Permission = AnimIns->GetPlayAnimationPermission(Task->PlayAnimBehaviorTag, Task->PlayPriority);
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

	// 设置下一次更新时间
	InOutRTTData.NextTick = Task->DelayInterruptedByMove;
}

void UBXTPPlayAnimation::Update(FBXTLRunTimeData& InOutRTData, FBXTLSectionRTData& InOutRTSData, FBXTLTaskRTData& InOutRTTData, float InDeltaTime)
{
	UBXTPlayAnimation* Task = Cast<UBXTPlayAnimation>(InOutRTTData.Task);
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

		if (!UBXStateFunctionLibrary::CheckActiveBehavior(Owner, PMovingTag))
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
	
void UBXTPPlayAnimation::End(FBXTLRunTimeData& InOutRTData, FBXTLSectionRTData& InOutRTSData, FBXTLTaskRTData& InOutRTTData, EBXTLFinishReason InReason)
{
	// 获取任务的自定义数据结构
	FBXTPPlayAnimationContext& TPC = InOutRTTData.DynamicData.GetMutable<FBXTPPlayAnimationContext>();

	UBXTPlayAnimation* Task = Cast<UBXTPlayAnimation>(InOutRTTData.Task);
	float BlendOut = IsValid(Task) ? Task->InterruptedBlendOut : 0.0f;
	
	for (TArray<USkeletalMeshComponent*>::TIterator It(TPC.TComponents); It; ++It)
	{
		UBXAnimInstance* AnimIns = Cast<UBXAnimInstance>((*It)->GetAnimInstance());
		if (!IsValid(AnimIns) || !TPC.Parameters.IsValidIndex(It.GetIndex()))
		{
			continue;
		}
		
		// 停止蒙太奇播放
		if (FAnimMontageInstance* MIns = AnimIns->GetMontageInstanceForID(TPC.Parameters[It.GetIndex()].MontageInstanceID))
		{
			AnimIns->Montage_Stop(BlendOut, MIns->Montage);
		}

		// 归还动画播放权限
		AnimIns->RevokePlayAnimationPermission(Task->PlayAnimBehaviorTag, TPC.Parameters[It.GetIndex()].Permission);
	}
}

void UBXTPPlayAnimation::ChangeTickRate(FBXTLTaskRTData& InOutRTTData, float InRate)
{
	UBXTPlayAnimation* Task = Cast<UBXTPlayAnimation>(InOutRTTData.Task);
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
