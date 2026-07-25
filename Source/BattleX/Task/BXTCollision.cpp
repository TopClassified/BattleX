#include "BXTCollision.h"

#include "UObject/ObjectSaveContext.h"
#include "UObject/UnrealType.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimSequence.h"
#include "Animation/Skeleton.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/Character.h"

#include "BXTAnimation.h"
#include "BXFunctionLibrary.h"
#include "Timeline/BXTLAsset.h"
#include "BXMeleeWeapon.h"

DEFINE_LOG_CATEGORY_STATIC(LogBXTHB, Log, All);



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

	if (TrajectoryBone.BoneName.IsNone())
	{
		UE_LOG(LogBXTHB, Warning, TEXT("PreSave skip: BoneName is None, Task=%s"), *GetName());
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
		UE_LOG(LogBXTHB, Warning, TEXT("PreSave skip: OwnerAsset invalid, Task=%s, Outer=%s"),
			*GetName(), *GetOuter()->GetName());
		return;
	}

	// 计算Task时间范围
	const float TaskStart = StartTime;
	const float TaskEnd = (LifeType == EBXTLifeType::L_Timeline)
		? OwnerAsset->Sections[OwnerSectionIndex].Duration
		: StartTime + Duration;

	// 收集Section中所有有效动画Task
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
		UE_LOG(LogBXTHB, Warning, TEXT("PreSave skip: no valid AnimTask, Task=%s"), *GetName());
		return;
	}

	// 优先按Socket解析，否则按骨骼处理
	USkeletalMesh* TargetSkeletalMesh = TrajectoryBone.SkeletalMesh.LoadSynchronous();
	bool bIsSocket = false;
	FTransform SocketRelativeTransform = FTransform::Identity;
	FName SampleBoneName = TrajectoryBone.BoneName;

	if (IsValid(TargetSkeletalMesh))
	{
		if (USkeletalMeshSocket* Socket = TargetSkeletalMesh->FindSocket(TrajectoryBone.BoneName))
		{
			bIsSocket = true;
			SocketRelativeTransform = Socket->GetSocketLocalTransform();
			SampleBoneName = Socket->BoneName;
		}
	}

	// 取首个有效Montage的Skeleton作为索引基准
	USkeleton* SharedSkeleton = nullptr;
	for (UBXTPlayAnimation* AnimTask : AnimTasks)
	{
		if (IsValid(AnimTask) && IsValid(AnimTask->Montage))
		{
			SharedSkeleton = AnimTask->Montage->GetSkeleton();
			break;
		}
	}
	if (!IsValid(SharedSkeleton))
	{
		UE_LOG(LogBXTHB, Warning, TEXT("PreSave skip: SharedSkeleton invalid, Task=%s"), *GetName());
		return;
	}

	const int32 BoneIndex = SharedSkeleton->GetReferenceSkeleton().FindBoneIndex(SampleBoneName);
	if (BoneIndex == INDEX_NONE)
	{
		UE_LOG(LogBXTHB, Warning, TEXT("PreSave skip: BoneIndex not found, SampleBoneName=%s, Task=%s"),
			*SampleBoneName.ToString(), *GetName());
		return;
	}

	// 构建根->目标骨骼链,累乘local得到component space
	const FReferenceSkeleton& RefSkeleton = SharedSkeleton->GetReferenceSkeleton();
	TArray<FBoneIndexType> RequiredBones;
	{
		int32 CurrentBone = BoneIndex;
		while (CurrentBone != INDEX_NONE)
		{
			RequiredBones.Insert((FBoneIndexType)CurrentBone, 0);
			CurrentBone = RefSkeleton.GetParentIndex(CurrentBone);
		}
	}
	if (RequiredBones.Num() <= 0)
	{
		UE_LOG(LogBXTHB, Warning, TEXT("PreSave skip: RequiredBones empty, Task=%s"), *GetName());
		return;
	}

	UE_LOG(LogBXTHB, Log, TEXT("PreSave bake: Task=%s Bone=%s bIsSocket=%d SampleRange=[%.3f,%.3f]"),
		*GetName(), *SampleBoneName.ToString(), (int32)bIsSocket, TaskStart, TaskEnd);

	// 100fps采样
	const float SampleRate = 100.0f;
	const float SampleInterval = 1.0f / SampleRate;
	const float SampleDuration = TaskEnd - TaskStart;
	const int32 SampleCount = FMath::Max(2, FMath::CeilToInt(SampleDuration * SampleRate) + 1);

	for (int32 i = 0; i < SampleCount; ++i)
	{
		const float SampleTime = FMath::Min(TaskStart + i * SampleInterval, TaskEnd);

		// 选取当前时间优先级最高的动画Task
		UBXTPlayAnimation* SelectedAnimTask = nullptr;
		for (UBXTPlayAnimation* AnimTask : AnimTasks)
		{
			const float AnimStart = AnimTask->StartTime;
			const float AnimEnd = (AnimTask->LifeType == EBXTLifeType::L_Timeline)
				? Section.Duration
				: AnimTask->StartTime + AnimTask->Duration;

			if (SampleTime < AnimStart || SampleTime >= AnimEnd)
			{
				continue;
			}

			if (!SelectedAnimTask)
			{
				SelectedAnimTask = AnimTask;
				continue;
			}

			// Priority高者优先,相同则取StartTime更大者
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

		// 从Montage采样骨骼
		UAnimMontage* Montage = SelectedAnimTask->Montage;
		USkeleton* Skeleton = Montage->GetSkeleton();
		if (!IsValid(Skeleton))
		{
			continue;
		}

		// 换算到Montage时间
		const float AnimOffset = SampleTime - SelectedAnimTask->StartTime;
		const float MontageTime = AnimOffset / SelectedAnimTask->PlayRate;

		// 取Montage首个有效AnimSequence作为采样源
		UAnimSequence* AnimSeq = nullptr;
		for (const FSlotAnimationTrack& SlotTrack : Montage->SlotAnimTracks)
		{
			for (const FAnimSegment& Seg : SlotTrack.AnimTrack.AnimSegments)
			{
				AnimSeq = Cast<UAnimSequence>(Seg.GetAnimReference());
				if (AnimSeq)
				{
					break;
				}
			}
			if (AnimSeq)
			{
				break;
			}
		}
		if (!IsValid(AnimSeq))
		{
			continue;
		}

		// 逐骨骼取local transform累乘父链得到component space
		FTransform BoneComponentSpace = FTransform::Identity;
		for (int32 ChainBone : RequiredBones)
		{
			FTransform LocalAtom = FTransform::Identity;
			FSkeletonPoseBoneIndex PoseBoneIndex(ChainBone);
			AnimSeq->GetBoneTransform(LocalAtom, PoseBoneIndex, static_cast<double>(MontageTime), false);
			BoneComponentSpace = LocalAtom * BoneComponentSpace;
		}

		// Socket模式叠加Socket相对挂接骨骼的Transform
		FTransform FinalTransform = bIsSocket
			? (SocketRelativeTransform * BoneComponentSpace)
			: BoneComponentSpace;

		FBXTrajectoryPoint Point;
		Point.Time = SampleTime;
		Point.Transform = FinalTransform;
		BoneSampledTrajectory.List.Add(Point);
	}

	UE_LOG(LogBXTHB, Log, TEXT("PreSave sampled %d points before optimization, Task=%s"),
		BoneSampledTrajectory.List.Num(), *GetName());

	// 优化:移除差异不大的相邻点
	TArray<FBXTrajectoryPoint>& List = BoneSampledTrajectory.List;
	if (List.Num() < 3)
	{
		UE_LOG(LogBXTHB, Log, TEXT("PreSave done: skip optimization, final count=%d, Task=%s"),
			List.Num(), *GetName());
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

	// 旋转差异合并
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

	UE_LOG(LogBXTHB, Log, TEXT("PreSave done: final trajectory count=%d, Task=%s"), List.Num(), *GetName());
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

#if WITH_EDITOR
void UBXTTrackWeaponHitBox::PreSave(FObjectPreSaveContext SaveContext)
{
	UBXTLAsset* OwnerAsset = Cast<UBXTLAsset>(GetOuter());
	if (!IsValid(OwnerAsset) || !IsValid(OwnerAsset->PlayerInformation))
	{
		UE_LOG(LogBXTHB, Warning, TEXT("WeaponHitBox PreSave skip: OwnerAsset or PlayerInformation invalid, Task=%s"),
			*GetName());
		Super::PreSave(SaveContext);
		return;
	}

	UObject* PlayerInfo = OwnerAsset->PlayerInformation;
	UClass* InfoClass = PlayerInfo->GetClass();

	// 反射访问UsingGears字段
	FMapProperty* UsingGearsProp = CastField<FMapProperty>(InfoClass->FindPropertyByName(TEXT("UsingGears")));
	if (!UsingGearsProp)
	{
		UE_LOG(LogBXTHB, Warning, TEXT("WeaponHitBox PreSave skip: UsingGears property not found, Task=%s"), *GetName());
		Super::PreSave(SaveContext);
		return;
	}

	FScriptMapHelper MapHelper(UsingGearsProp, UsingGearsProp->ContainerPtrToValuePtr<void>(PlayerInfo));

	TSubclassOf<ABXGear> WeaponClass = nullptr;
	for (int32 i = 0; i < MapHelper.GetMaxIndex(); ++i)
	{
		if (!MapHelper.IsValidIndex(i))
		{
			continue;
		}

		FGameplayTag* KeyTag = reinterpret_cast<FGameplayTag*>(MapHelper.GetKeyPtr(i));
		if (*KeyTag == WeaponSlot)
		{
			UObject** ValuePtr = reinterpret_cast<UObject**>(MapHelper.GetValuePtr(i));
			WeaponClass = (*ValuePtr) ? Cast<UClass>(*ValuePtr) : nullptr;
			break;
		}
	}

	if (!WeaponClass)
	{
		UE_LOG(LogBXTHB, Warning, TEXT("WeaponHitBox PreSave skip: no weapon class for slot %s, Task=%s"),
			*WeaponSlot.ToString(), *GetName());
		Super::PreSave(SaveContext);
		return;
	}

	ABXGear* GearCDO = WeaponClass->GetDefaultObject<ABXGear>();
	if (!IsValid(GearCDO))
	{
		UE_LOG(LogBXTHB, Warning, TEXT("WeaponHitBox PreSave skip: GearCDO invalid, Task=%s"), *GetName());
		Super::PreSave(SaveContext);
		return;
	}

	// 从AttachmentConfigs找匹配Slot且State为Open的挂接配置(预览角色武器处于Open状态)
	FName WeaponAttachSocket = NAME_None;
	FTransform WeaponAttachRelation;
	bool bFoundConfig = false;
	for (const FBXGearAttachmentConfig& Config : GearCDO->AttachmentConfigs)
	{
		if (Config.Slot == WeaponSlot && Config.State == BXGameplayTags::BXGearState_Open)
		{
			WeaponAttachSocket = Config.Socket;
			WeaponAttachRelation = Config.Relation;
			bFoundConfig = true;
			break;
		}
	}
	if (!bFoundConfig || WeaponAttachSocket.IsNone())
	{
		UE_LOG(LogBXTHB, Warning, TEXT("WeaponHitBox PreSave skip: no AttachmentConfig for slot %s with State=Open, Task=%s"),
			*WeaponSlot.ToString(), *GetName());
		Super::PreSave(SaveContext);
		return;
	}

	UE_LOG(LogBXTHB, Log, TEXT("WeaponHitBox PreSave: Task=%s WeaponClass=%s AttachSocket=%s"),
		*GetName(), *WeaponClass->GetName(), *WeaponAttachSocket.ToString());

	// 反射访问ActorClass字段从角色CDO拿SkeletalMesh
	USkeletalMesh* CharacterSkeletalMesh = nullptr;
	if (FObjectProperty* ActorClassProp = CastField<FObjectProperty>(InfoClass->FindPropertyByName(TEXT("ActorClass"))))
	{
		UObject* ActorClassObj = ActorClassProp->GetObjectPropertyValue_InContainer(PlayerInfo);
		UClass* ActorClass = Cast<UClass>(ActorClassObj);
		if (ActorClass)
		{
			AActor* ActorCDO = ActorClass->GetDefaultObject<AActor>();
			if (ACharacter* CharCDO = Cast<ACharacter>(ActorCDO))
			{
				if (USkeletalMeshComponent* MeshComp = CharCDO->GetMesh())
				{
					CharacterSkeletalMesh = MeshComp->GetSkeletalMeshAsset();
				}
			}
		}
	}

	if (!IsValid(CharacterSkeletalMesh))
	{
		UE_LOG(LogBXTHB, Warning, TEXT("WeaponHitBox PreSave: CharacterSkeletalMesh invalid, Task=%s"), *GetName());
	}

	// 推导出的Socket和SkeletalMesh塞给父类TrajectoryBone,触发父类PreSave烘焙
	TrajectoryBone.BoneName = WeaponAttachSocket;
	if (IsValid(CharacterSkeletalMesh))
	{
		TrajectoryBone.SkeletalMesh = CharacterSkeletalMesh;
	}

	Super::PreSave(SaveContext);
}
#endif

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
