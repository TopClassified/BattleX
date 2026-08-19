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
#include "Collision/BXShapeComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogBXTHB, Log, All);



#pragma region Editor
#if WITH_EDITOR
void UBXTIntervalCollision::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	if (LifeType == EBXTLifeType::L_Duration || LifeType == EBXTLifeType::L_DurationTimeline)
	{
		// Interval的ClampMin仅约束UI输入,代码/蓝图直设可为0,除零结果转int32溢出
		Count = FMath::Floor(Duration / FMath::Max(Interval, 0.001f));
	}
	else
	{
		Count = 0;
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif
#pragma endregion Editor







#pragma region Editor
#if WITH_EDITOR
void UBXTTrackHitBox::PreSave(FObjectPreSaveContext SaveContext)
{
	Super::PreSave(SaveContext);

	BoneSampledTrajectory.List.Reset();

	if (!HitBoxTag.IsValid())
	{
		UE_LOG(LogBXTHB, Warning, TEXT("PreSave skip: HitBoxTag invalid, Task=%s"), *GetName());
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
		UE_LOG(LogBXTHB, Warning, TEXT("PreSave skip: OwnerAsset invalid, Task=%s, Outer=%s"), *GetName(), *GetOuter()->GetName());
		return;
	}

	// 计算Task时间范围
	const float TaskStart = StartTime;
	const float TaskEnd = (LifeType == EBXTLifeType::L_Timeline) ? OwnerAsset->Sections[OwnerSectionIndex].Duration : StartTime + Duration;

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

	// 解析采样骨骼来源(子类可重写以提供不同来源,如武器挂接Socket)
	FName SampleBoneName = NAME_None;
	USkeletalMesh* TargetSkeletalMesh = nullptr;
	if (!ResolveSampleBoneSource(OwnerAsset, OwnerSectionIndex, SampleBoneName, TargetSkeletalMesh))
	{
		UE_LOG(LogBXTHB, Warning, TEXT("PreSave skip: ResolveSampleBoneSource failed, HitBoxTag=%s, Task=%s"), *HitBoxTag.ToString(), *GetName());
		return;
	}

	if (SampleBoneName.IsNone())
	{
		UE_LOG(LogBXTHB, Warning, TEXT("PreSave skip: SampleBoneName is None, Task=%s"), *GetName());
		return;
	}

	// 优先按Socket解析,否则按骨骼处理
	bool bIsSocket = false;
	FTransform SocketRelativeTransform = FTransform::Identity;

	if (IsValid(TargetSkeletalMesh))
	{
		if (USkeletalMeshSocket* Socket = TargetSkeletalMesh->FindSocket(SampleBoneName))
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
		UE_LOG(LogBXTHB, Warning, TEXT("PreSave skip: BoneIndex not found, SampleBoneName=%s, Task=%s"), *SampleBoneName.ToString(), *GetName());
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

	UE_LOG(LogBXTHB, Log, TEXT("PreSave bake: Task=%s Bone=%s bIsSocket=%d SampleRange=[%.3f,%.3f]"), *GetName(), *SampleBoneName.ToString(), (int32)bIsSocket, TaskStart, TaskEnd);

	// 60fps采样
	const float SampleRate = 60.0f;
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
		FTransform FinalTransform = bIsSocket ? (SocketRelativeTransform * BoneComponentSpace) : BoneComponentSpace;

		FBXTrajectoryPoint Point;
		Point.Time = SampleTime;
		Point.Transform = FinalTransform;
		BoneSampledTrajectory.List.Add(Point);
	}

	UE_LOG(LogBXTHB, Log, TEXT("PreSave done: trajectory count=%d, Task=%s"), BoneSampledTrajectory.List.Num(), *GetName());
}

void UBXTTrackHitBox::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	if (LifeType == EBXTLifeType::L_Instant)
	{
		LifeType = EBXTLifeType::L_DurationTimeline;
	}

	// 这个Task只能让拥有者使用
	TargetTypes = (1 << (int32)EBXTTargetType::T_Owner);

	// PolylineConfig数值范围限制: X=1~10 Y=1~60 Z=1~180
	PolylineConfig.X = FMath::Clamp(PolylineConfig.X, 1, 10);
	PolylineConfig.Y = FMath::Clamp(PolylineConfig.Y, 1, 60);
	PolylineConfig.Z = FMath::Clamp(PolylineConfig.Z, 1, 180);

	Super::PostEditChangeProperty(PropertyChangedEvent);
}

void UBXTTrackHitBox::RefreshDataBeforePreview()
{

}

bool UBXTTrackHitBox::ResolveSampleBoneSource(UBXTLAsset* OwnerAsset, int32 SectionIndex, FName& OutBoneName, USkeletalMesh*& OutSkeletalMesh)
{
	if (!IsValid(OwnerAsset) || !IsValid(OwnerAsset->PlayerInformation))
	{
		return false;
	}

	UObject* PlayerInfo = OwnerAsset->PlayerInformation;
	UClass* InfoClass = PlayerInfo->GetClass();

	// 反射访问ActorClass字段从角色CDO拿ShapeComponent和SkeletalMesh
	USkeletalMesh* CharacterSkeletalMesh = nullptr;
	UBXShapeComponent* ShapeComp = nullptr;

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
				ShapeComp = CharCDO->FindComponentByClass<UBXShapeComponent>();
			}
		}
	}

	if (!IsValid(ShapeComp))
	{
		UE_LOG(LogBXTHB, Warning, TEXT("ResolveSampleBoneSource: ShapeComponent not found on character CDO, Task=%s"), *GetName());
		return false;
	}

	// 根据HitBoxTag找ShapeInformation,取其Socket作为采样骨骼
	const FBXShapeInformation* SInfo = ShapeComp->ShapeInformations.Find(HitBoxTag);
	if (!SInfo)
	{
		UE_LOG(LogBXTHB, Warning, TEXT("ResolveSampleBoneSource: ShapeInformation not found for HitBoxTag=%s, Task=%s"), *HitBoxTag.ToString(), *GetName());
		return false;
	}

	// 角色CDO未配SkeletalMesh时Socket模式会失效,输出警告提示配置异常
	if (!IsValid(CharacterSkeletalMesh))
	{
		UE_LOG(LogBXTHB, Warning, TEXT("ResolveSampleBoneSource: CharacterSkeletalMesh invalid on character CDO, Task=%s"), *GetName());
		return false;
	}

	OutBoneName = SInfo->Socket.BoneName;
	OutSkeletalMesh = CharacterSkeletalMesh;
	return true;
}

bool UBXTTrackHitBox::EnablePassiveTrigger()
{
	return false;
}
#endif
#pragma endregion Editor








#pragma region Editor
#if WITH_EDITOR
bool UBXTTrackWeaponHitBox::ResolveSampleBoneSource(UBXTLAsset* OwnerAsset, int32 SectionIndex, FName& OutBoneName, USkeletalMesh*& OutSkeletalMesh)
{
	if (!IsValid(OwnerAsset) || !IsValid(OwnerAsset->PlayerInformation))
	{
		return false;
	}

	UObject* PlayerInfo = OwnerAsset->PlayerInformation;
	UClass* InfoClass = PlayerInfo->GetClass();

	// 反射访问UsingGears字段获取武器类
	FMapProperty* UsingGearsProp = CastField<FMapProperty>(InfoClass->FindPropertyByName(TEXT("UsingGears")));
	if (!UsingGearsProp)
	{
		UE_LOG(LogBXTHB, Warning, TEXT("WeaponHitBox ResolveSampleBoneSource: UsingGears property not found, Task=%s"), *GetName());
		return false;
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
		UE_LOG(LogBXTHB, Warning, TEXT("WeaponHitBox ResolveSampleBoneSource: no weapon class for slot %s, Task=%s"), *WeaponSlot.ToString(), *GetName());
		return false;
	}

	ABXGear* GearCDO = WeaponClass->GetDefaultObject<ABXGear>();
	if (!IsValid(GearCDO))
	{
		return false;
	}

	// 从武器CDO获取HitBoxComponent,验证HitBoxTag存在
	ABXMeleeWeapon* WeaponCDO = Cast<ABXMeleeWeapon>(GearCDO);
	if (!IsValid(WeaponCDO))
	{
		UE_LOG(LogBXTHB, Warning, TEXT("WeaponHitBox ResolveSampleBoneSource: WeaponClass %s is not a MeleeWeapon, Task=%s"),
			*WeaponClass->GetName(), *GetName());
		return false;
	}

	UBXShapeComponent* WeaponHitBoxComp = WeaponCDO->GetHitBoxComponent();
	if (!IsValid(WeaponHitBoxComp))
	{
		UE_LOG(LogBXTHB, Warning, TEXT("WeaponHitBox ResolveSampleBoneSource: HitBoxComponent invalid, Task=%s"), *GetName());
		return false;
	}

	if (!WeaponHitBoxComp->ShapeInformations.Contains(HitBoxTag))
	{
		UE_LOG(LogBXTHB, Warning, TEXT("WeaponHitBox ResolveSampleBoneSource: HitBoxTag %s not found in weapon HitBoxComponent, Task=%s"), *HitBoxTag.ToString(), *GetName());
		return false;
	}

	// 从AttachmentConfigs找匹配Slot且State为Open的挂接配置(采样角色挂接Socket)
	FName WeaponAttachSocket = NAME_None;
	bool bFoundConfig = false;
	for (const FBXGearAttachmentConfig& Config : GearCDO->AttachmentConfigs)
	{
		if (Config.Slot == WeaponSlot && Config.State == BXGameplayTags::BXGearState_Open)
		{
			WeaponAttachSocket = Config.Socket;
			bFoundConfig = true;
			break;
		}
	}
	if (!bFoundConfig || WeaponAttachSocket.IsNone())
	{
		UE_LOG(LogBXTHB, Warning, TEXT("WeaponHitBox ResolveSampleBoneSource: no AttachmentConfig for slot %s with State=Open, Task=%s"), *WeaponSlot.ToString(), *GetName());
		return false;
	}

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
		UE_LOG(LogBXTHB, Warning, TEXT("WeaponHitBox ResolveSampleBoneSource: CharacterSkeletalMesh invalid, Task=%s"), *GetName());
		return false;
	}

	OutBoneName = WeaponAttachSocket;
	OutSkeletalMesh = CharacterSkeletalMesh;

	UE_LOG(LogBXTHB, Log, TEXT("WeaponHitBox ResolveSampleBoneSource: Task=%s WeaponClass=%s AttachSocket=%s HitBoxTag=%s"), *GetName(), *WeaponClass->GetName(), *WeaponAttachSocket.ToString(), *HitBoxTag.ToString());

	return true;
}
#endif
#pragma endregion Editor
