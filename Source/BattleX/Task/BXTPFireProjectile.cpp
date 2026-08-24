#include "BXTPFireProjectile.h"

#include "BXTFireProjectile.h"
#include "BXProjectileManager.h"
#include "BXProjectileComponent.h"



void UBXTPFireProjectile::Start(FBXTLRunTimeData& InOutRTData, FBXTLSectionRTData& InOutRTSData, FBXTLTaskRTData& InOutRTTData)
{
	UBXTFireProjectile* Task = Cast<UBXTFireProjectile>(InOutRTTData.Task);
	if (!Task)
	{
		UE_LOG(BX_TP, Warning, TEXT("UBXTPFireProjectile::Start failed: Task cast to UBXTFireProjectile failed, Task=%s."), InOutRTTData.Task ? *InOutRTTData.Task->GetName() : TEXT("null"));
		return;
	}

	if (!Task->ProjectileType.IsValid())
	{
		UE_LOG(BX_TP, Warning, TEXT("UBXTPFireProjectile::Start failed: ProjectileType invalid, Task=%s."), *Task->GetName());
		return;
	}

	AActor* Owner = InOutRTData.Owner;
	if (!IsValid(Owner))
	{
		UE_LOG(BX_TP, Warning, TEXT("UBXTPFireProjectile::Start failed: Owner invalid, Task=%s."), *Task->GetName());
		return;
	}

	UBXProjectileManager* ProjectileMgr = UBXProjectileManager::Get(this);
	if (!ProjectileMgr)
	{
		UE_LOG(BX_TP, Warning, TEXT("UBXTPFireProjectile::Start failed: ProjectileManager missing, Task=%s."), *Task->GetName());
		return;
	}

	UBXProjectileComponent* Carrier = ProjectileMgr->GetOrCreateCarrier(Owner);
	if (!IsValid(Carrier))
	{
		UE_LOG(BX_TP, Warning, TEXT("UBXTPFireProjectile::Start failed: Carrier missing, Owner=%s Task=%s."), *Owner->GetName(), *Task->GetName());
		return;
	}

	FBXTPFireProjectileContext& TPC = InOutRTTData.DynamicData.GetMutable<FBXTPFireProjectileContext>();

	// 解析发射基准与目标列表
	const FTransform OwnerTransform = Owner->GetActorTransform();
	const FVector StartLocation = OwnerTransform.TransformPosition(Task->MuzzleOffset);
	const FVector OwnerUp = OwnerTransform.GetUnitAxis(EAxis::Z);

	TArray<AActor*> Targets;
	UBXTProcessor::GetTargetActorList(InOutRTData, InOutRTTData, Targets);

	// 逐目标发射时遍历全部目标,否则仅对首个目标(或无目标)发射一发
	const int32 FireCount = Task->bFirePerTarget ? FMath::Max(Targets.Num(), 1) : 1;
	for (int32 Index = 0; Index < FireCount; ++Index)
	{
		AActor* Target = Targets.IsValidIndex(Index) ? Targets[Index] : nullptr;
		const FVector FireDirection = BuildFireDirection(OwnerTransform, StartLocation, Target, Task->FireDirectionType);

		const int64 ProjectileID = Carrier->FireProjectile(Task->ProjectileType, StartLocation, FireDirection, OwnerUp, Target, Task->ContextData, InOutRTData.Instigator, InOutRTData.Triggerer);
		if (ProjectileID != 0)
		{
			TPC.FiredProjectileIDs.Add(ProjectileID);
		}
	}
}

FVector UBXTPFireProjectile::BuildFireDirection(const FTransform& InOwnerTransform, const FVector& InStartLocation, AActor* InTarget, EBXProjectileFireDirection InDirectionType) const
{
	if (InDirectionType == EBXProjectileFireDirection::FD_ToTarget && IsValid(InTarget))
	{
		const FVector ToTarget = InTarget->GetActorLocation() - InStartLocation;
		if (ToTarget.SizeSquared() > 1.0f)
		{
			return ToTarget.GetSafeNormal();
		}
	}

	return InOwnerTransform.GetUnitAxis(EAxis::X);
}
