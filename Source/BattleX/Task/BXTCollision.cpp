#include "BXTCollision.h"

#include "UObject/ObjectSaveContext.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimSequence.h"
#include "Animation/Skeleton.h"

#include "BXTAnimation.h"
#include "BXFunctionLibrary.h"
#include "Timeline/BXTLAsset.h"



#pragma region Editor
#if WITH_EDITOR
void UBXTCollision::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	if (LifeType == EBXTLifeType::L_Duration || LifeType == EBXTLifeType::L_DurationTimeline)
	{
		Count = FMath::Floor(Duration / Interval);
	}
	else
	{
		Count = 0;
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif
#pragma endregion Editor







UBXTTrackHitBox::UBXTTrackHitBox()
{
	DisplayName = FText::FromString(TEXT("碰撞盒轨迹命中检测"));
}

#pragma region Editor
#if WITH_EDITOR
void UBXTTrackHitBox::PreSave(FObjectPreSaveContext SaveContext)
{
	Super::PreSave(SaveContext);

	BoneSampledTrajectory.List.Reset();

	// 骨骼名无效则跳过
	if (TrajectoryBone.BoneName.IsNone())
	{
		return;
	}

	// 通过Outer获取所在的UBXTLAsset
	UBXTLAsset* OwnerAsset = nullptr;
	int32 OwnerSectionIndex = -1;

	if (UBXTLAsset* Asset = Cast<UBXTLAsset>(GetOuter()))
	{
		for (int32 i = 0; i < Asset->Sections.Num(); ++i)
		{
			if (Asset->Sections[i].TaskList.Contains(this))
			{
				OwnerAsset = Asset;
				OwnerSectionIndex = i;
				break;
			}
		}
	}

	if (!OwnerAsset || OwnerSectionIndex < 0)
	{
		return;
	}

	// 计算本Task的时间范围
	const float TaskStart = StartTime;
	const float TaskEnd = (LifeType == EBXTLifeType::L_Timeline)
		? OwnerAsset->Sections[OwnerSectionIndex].Duration
		: StartTime + Duration;

	// 收集同一Section中所有有效的动画Task
	const FBXTLSection& Section = OwnerAsset->Sections[OwnerSectionIndex];
	TArray<UBXTPlayAnimation*> AnimTasks;
	for (UBXTask* Task : Section.TaskList)
	{
		UBXTPlayAnimation* AnimTask = Cast<UBXTPlayAnimation>(Task);
		if (!IsValid(AnimTask) || !IsValid(AnimTask->Montage))
		{
			continue;
		}
		AnimTasks.Add(AnimTask);
	}

	if (AnimTasks.Num() <= 0)
	{
		return;
	}

	// 以30fps采样
	const float SampleRate = 30.0f;
	const float SampleInterval = 1.0f / SampleRate;
	const float SampleDuration = TaskEnd - TaskStart;
	const int32 SampleCount = FMath::Max(2, FMath::CeilToInt(SampleDuration * SampleRate) + 1);

	for (int32 i = 0; i < SampleCount; ++i)
	{
		const float SampleTime = FMath::Min(TaskStart + i * SampleInterval, TaskEnd);

		// 在当前采样时间点选取优先级最高的动画Task
		UBXTPlayAnimation* SelectedAnimTask = nullptr;
		for (UBXTPlayAnimation* AnimTask : AnimTasks)
		{
			const float AnimStart = AnimTask->StartTime;
			const float AnimEnd = (AnimTask->LifeType == EBXTLifeType::L_Timeline)
				? Section.Duration
				: AnimTask->StartTime + AnimTask->Duration;

			// 当前采样时间不在该动画范围内则跳过
			if (SampleTime < AnimStart || SampleTime >= AnimEnd)
			{
				continue;
			}

			if (!SelectedAnimTask)
			{
				SelectedAnimTask = AnimTask;
				continue;
			}

			// PlayPriority更高则替换，相同则选StartTime更大的
			if (AnimTask->PlayPriority > SelectedAnimTask->PlayPriority ||
				(AnimTask->PlayPriority == SelectedAnimTask->PlayPriority && AnimTask->StartTime > SelectedAnimTask->StartTime))
			{
				SelectedAnimTask = AnimTask;
			}
		}

		if (!IsValid(SelectedAnimTask))
		{
			continue;
		}

		// 从Montage中采样骨骼
		UAnimMontage* Montage = SelectedAnimTask->Montage;
		USkeleton* Skeleton = Montage->GetSkeleton();
		if (!IsValid(Skeleton))
		{
			continue;
		}

		const int32 BoneIndex = Skeleton->GetReferenceSkeleton().FindBoneIndex(TrajectoryBone.BoneName);
		if (BoneIndex == INDEX_NONE)
		{
			continue;
		}

		// 计算动画内的偏移时间并换算到Montage时间
		const float AnimOffset = SampleTime - SelectedAnimTask->StartTime;
		const float MontageTime = AnimOffset / SelectedAnimTask->PlayRate;

		FBoneContainer BoneContainer;
		BoneContainer.InitializeTo(TArray<FBoneIndexType>{ (FBoneIndexType)BoneIndex }, FCurveEvaluationOption(false), *Skeleton);

		FCompactPose Pose;
		Pose.SetBoneContainer(&BoneContainer);

		FBlendedCurve Curve;
		Curve.InitFrom(BoneContainer);

		FAnimExtractContext ExtrCtx(MontageTime, false);
		UAnimSequenceBase* SeqBase = Montage->GetAnimCompositeSection(0).GetLinkedSequence();
		if (!IsValid(SeqBase))
		{
			// fallback: 直接从第一个slot的第一个segment取sequence
			for (const FSlotAnimationTrack& SlotTrack : Montage->SlotAnimTracks)
			{
				for (const FAnimSegment& Seg : SlotTrack.AnimTrack.AnimSegments)
				{
					SeqBase = Seg.GetAnimReference();
					break;
				}
				if (SeqBase)
				{
					break;
				}
			}
		}

		UAnimSequence* AnimSeq = Cast<UAnimSequence>(SeqBase);
		if (!IsValid(AnimSeq))
		{
			continue;
		}

		FStackCustomAttributes Attrs;
		FAnimationPoseData PoseData(Pose, Curve, Attrs);
		AnimSeq->GetBonePose(PoseData, ExtrCtx);

		// 获取骨骼的模型空间Transform（从根到目标骨骼累乘）
		FTransform BoneTransform = FTransform::Identity;
		const FReferenceSkeleton& RefSkeleton = Skeleton->GetReferenceSkeleton();
		TArray<int32> BoneChain;
		int32 CurrentBone = BoneIndex;
		while (CurrentBone != INDEX_NONE)
		{
			BoneChain.Insert(CurrentBone, 0);
			CurrentBone = RefSkeleton.GetParentIndex(CurrentBone);
		}

		for (int32 ChainBone : BoneChain)
		{
			FCompactPoseBoneIndex CompactIndex = BoneContainer.MakeCompactPoseIndex(FMeshPoseBoneIndex(ChainBone));
			if (CompactIndex.IsValid())
			{
				BoneTransform = Pose[CompactIndex] * BoneTransform;
			}
		}

		FBXTrajectoryPoint Point;
		Point.Time = SampleTime;
		Point.Transform = BoneTransform;
		BoneSampledTrajectory.List.Add(Point);
	}

	// 优化：移除差异不大的相邻点
	TArray<FBXTrajectoryPoint>& List = BoneSampledTrajectory.List;
	if (List.Num() < 3)
	{
		return;
	}

	// 共线合并
	TArray<int32> SlopeIndexList;
	SlopeIndexList.Add(0);
	for (int32 i = 1; i < List.Num() - 1; ++i)
	{
		if (!UBXFunctionLibrary::AreCollinear(List[SlopeIndexList.Last()].Transform.GetLocation(), List[i].Transform.GetLocation(), List[i + 1].Transform.GetLocation(), TrajectoryOptimization.X))
		{
			SlopeIndexList.Add(i);
		}
	}
	SlopeIndexList.Add(List.Num() - 1);

	// 方向差异合并
	TArray<int32> AngleIndexList;
	AngleIndexList.Add(0);
	const float RadiansError = FMath::DegreesToRadians(TrajectoryOptimization.Y);
	for (int32 i = 1; i < List.Num() - 1; ++i)
	{
		if (RadiansError < List[AngleIndexList.Last()].Transform.GetRotation().AngularDistance(List[i].Transform.GetRotation()))
		{
			AngleIndexList.Add(i);
		}
	}
	AngleIndexList.Add(List.Num() - 1);

	// 缩放差异合并
	TArray<int32> ScaleIndexList;
	ScaleIndexList.Add(0);
	for (int32 i = 1; i < List.Num() - 1; ++i)
	{
		if (!List[ScaleIndexList.Last()].Transform.GetScale3D().Equals(List[i].Transform.GetScale3D(), TrajectoryOptimization.Z))
		{
			ScaleIndexList.Add(i);
		}
	}
	ScaleIndexList.Add(List.Num() - 1);

	TArray<FBXTrajectoryPoint> Optimized;
	for (int32 i = 0; i < List.Num(); ++i)
	{
		if (SlopeIndexList.Contains(i) || AngleIndexList.Contains(i) || ScaleIndexList.Contains(i))
		{
			Optimized.Add(List[i]);
		}
	}
	List = MoveTemp(Optimized);
}

void UBXTTrackHitBox::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	if (LifeType == EBXTLifeType::L_Instant)
	{
		LifeType = EBXTLifeType::L_DurationTimeline;
	}

	// 这个Task只能让拥有者使用
	TargetTypes = (1 << (int32)EBXTTargetType::T_Owner);

	Super::PostEditChangeProperty(PropertyChangedEvent);
}

void UBXTTrackHitBox::RefreshDataBeforePreview()
{

}

bool UBXTTrackHitBox::EnablePassiveTrigger()
{
	return false;
}
#endif
#pragma endregion Editor







UBXTTrackWeaponHitBox::UBXTTrackWeaponHitBox()
{
	DisplayName = FText::FromString(TEXT("武器碰撞盒轨迹命中检测"));
}
