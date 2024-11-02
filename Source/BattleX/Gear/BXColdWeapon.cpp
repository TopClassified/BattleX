#include "BXColdWeapon.h"

#include "Components/SkeletalMeshComponent.h"

#include "BXManager.h"
#include "BXCollision.h"
#include "BXGearComponent.h"
#include "BXFunctionLibrary.h"


#pragma region Important
ABXColdWeapon::ABXColdWeapon(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	HitBoxComponent = CreateDefaultSubobject<UBXShapeComponent>(TEXT("HitBox"));
}

ABXColdWeapon::~ABXColdWeapon()
{
	
}

void ABXColdWeapon::BeginPlay()
{
	
	
	Super::BeginPlay();
}

void ABXColdWeapon::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	StopRecordLocation();
}

#pragma endregion Important



#pragma region Use
void ABXColdWeapon::InternalPostUsing(UPARAM(ref) FBXUsingGearInformation& UsingInfo)
{
	// TODO：考虑角色类型决定是否开启位置记录，并将该逻辑移动到蓝图触发
	StartRecordLocation();
	
	Super::InternalPostUsing(UsingInfo);
}

void ABXColdWeapon::InternalPreUnusing(UPARAM(ref) FBXUsingGearInformation& UnusingInfo)
{
	StopRecordLocation();
	
	Super::InternalPreUnusing(UnusingInfo);
}
	
#pragma endregion Use



#pragma region State
void ABXColdWeapon::InternalChangeState(FGameplayTag NewState)
{
	if (CurrentState == NewState)
	{
		return;
	}

	CurrentState = NewState;

	// 挂接到特定位置
	AttachToSocket();
}

#pragma endregion State



#pragma region Collision
void ABXColdWeapon::StartRecordLocation()
{
	if (!HitBoxComponent)
	{
		return;
	}
	
	AddNewHitBoxRecord();
	
	// 使用辅助线程记录位置信息
	if (UBXManager* Manager = UBXManager::Get(this))
	{
		Manager->RegisterHTFunction(this, FName(TEXT("AddNewHitBoxRecord")), UpdateHitBoxRecordInterval);
	}
}

void ABXColdWeapon::StopRecordLocation()
{
	// 停止使用辅助线程记录位置信息
	if (UBXManager* Manager = UBXManager::Get(this))
	{
		Manager->UnregisterHTFunction(this, FName(TEXT("AddNewHitBoxRecord")));
	}
}

void ABXColdWeapon::GetHitResults(float InStartTime, FGameplayTagContainer& BoxTags, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, const FBXCFilter& Filter, TArray<FHitResult>& OutResults, FVector4 OverrideRules)
{
	OutResults.Reset();

	if (!IsValid(OwnerComponent))
	{
		return;
	}

	TArray<FHitResult> TempResults;
	TArray<FTransform> OutTransforms;
	float CurrentTime = UBXFunctionLibrary::GetClientTimeSeconds(this);

	bUsingHitBoxRecords = true;
	for (int32 Loop = 0; Loop < BoxTags.Num(); ++Loop)
	{
		const FGameplayTag& BoxTag = BoxTags.GetByIndex(Loop);
		
		FBXGHitBoxRecords* Records = HitBoxRecords.Find(BoxTag);
		if (!Records)
		{
			continue;
		}
		
		FBXCParameter Parameter;
		Parameter.Requester = OwnerComponent->GetOwner();

		// 获取当前的位置信息
		EBXShapeType ShapeType = EBXShapeType::ST_Sphere;
		FVector ShapeSize(FVector::ZeroVector);
		FTransform WTransform = FTransform::Identity;
		if (FBXShapeInformation* Information = HitBoxComponent->ShapeInformations.Find(BoxTag))
		{
			USceneComponent* AttachedComponent = UBXFunctionLibrary::GetSceneComponentByNameAndClass(Owner, Information->AttachParent, nullptr, false);
			if (!AttachedComponent)
			{
				AttachedComponent = GetRootComponent();
			}

			ShapeType = Information->ShapeType;
			ShapeSize = Information->ShapeSize;
			WTransform = Information->Relation * AttachedComponent->GetSocketTransform(Information->Socket.BoneName);
		}
		else
		{
			continue;
		}

		// 根据受击盒记录列表，获取范围列表
		UBXCollisionLibrary::GetTransformListFromBXHitBoxRecords
		(
			*Records,
			InStartTime,
			CurrentTime,
			OutTransforms,
			WTransform,
			OverrideRules.X > 0.0f ? OverrideRules.X : TrackOptimizationRules.X,
			OverrideRules.Y > 0.0f ? OverrideRules.Y : TrackOptimizationRules.Y,
			OverrideRules.Z > 0.0f ? OverrideRules.Z : TrackOptimizationRules.Z
		);

		if (OutTransforms.Num() <= 0)
		{
			return;
		}
		
		if (OutTransforms.Num() < 2)
		{
			Parameter.StartLocation = OutTransforms[0].GetLocation();
			Parameter.StartRotation = OutTransforms[0].GetRotation().Rotator();
			Parameter.Scale = OutTransforms[0].GetScale3D();
			Parameter.EndLocation = FVector::ZeroVector;
			Parameter.EndRotation = FRotator::ZeroRotator;

			// 调用碰撞查询接口
			if (ShapeType == EBXShapeType::ST_Sphere)
			{
				OutResults = UBXCollisionLibrary::SphereCheck(Parameter, ObjectTypes, ShapeSize.X, Filter);
			}
			else if (ShapeType == EBXShapeType::ST_Capsule)
			{
				OutResults = UBXCollisionLibrary::CapsuleCheck(Parameter, ObjectTypes, FVector2D(ShapeSize.X, ShapeSize.Y), Filter);
			}
			else if (ShapeType == EBXShapeType::ST_Box)
			{
				OutResults = UBXCollisionLibrary::BoxCheck(Parameter, ObjectTypes, ShapeSize, Filter);
			}
		}
		else
		{
			for (int32 i = 0; i < OutTransforms.Num() - 1; ++i)
			{
				Parameter.StartLocation = OutTransforms[i].GetLocation();
				Parameter.StartRotation = OutTransforms[i].GetRotation().Rotator();
				Parameter.EndLocation = OutTransforms[i + 1].GetLocation();
				Parameter.EndRotation = OutTransforms[i + 1].GetRotation().Rotator();
				Parameter.Scale = (OutTransforms[i].GetScale3D() + OutTransforms[i + 1].GetScale3D()) * 0.5f;
			
				// 调用碰撞查询接口
				if (ShapeType == EBXShapeType::ST_Sphere)
				{
					TempResults = UBXCollisionLibrary::SphereCheck(Parameter, ObjectTypes, ShapeSize.X, Filter);
				}
				else if (ShapeType == EBXShapeType::ST_Capsule)
				{
					TempResults = UBXCollisionLibrary::CapsuleCheck(Parameter, ObjectTypes, FVector2D(ShapeSize.X, ShapeSize.Y), Filter, OverrideRules.W > 0.0f ? OverrideRules.W : TrackOptimizationRules.W);
				}
				else if (ShapeType == EBXShapeType::ST_Box)
				{
					TempResults = UBXCollisionLibrary::BoxCheck(Parameter, ObjectTypes, ShapeSize, Filter, OverrideRules.W > 0.0f ? OverrideRules.W : TrackOptimizationRules.W);
				}

				// 结果去重合并
				UBXCollisionLibrary::CombineCollisionResults(TempResults, OutResults);	
			}
		}
	}

	bUsingHitBoxRecords = false;
}

void ABXColdWeapon::AddNewHitBoxRecord()
{
	UClass* ActorClass = GetClass();

	HBRHelpMap.Reset();
	for (TMap<FGameplayTag, FBXShapeInformation>::TIterator It(HitBoxComponent->ShapeInformations); It; ++It)
	{
		const FBXShapeInformation& Information = It->Value;

		// 找到挂接的父组件
		TWeakObjectPtr<USceneComponent> AttachedComponent = nullptr;
		if (TWeakObjectPtr<USceneComponent>* FindResult = HBRHelpMap.Find(It->Key))
		{
			AttachedComponent = *FindResult;
		}
		else
		{
			AttachedComponent = UBXFunctionLibrary::GetSceneComponentByNameAndClass(Owner, Information.AttachParent, nullptr, false);
			HBRHelpMap.Add(It->Key, AttachedComponent);
		}
		if (!AttachedComponent.IsValid())
		{
			AttachedComponent = GetRootComponent();
		}
			
		// 记录位置
		FBXGHitBoxRecord Record;
		Record.WTransform = Information.Relation * AttachedComponent->GetSocketTransform(Information.Socket.BoneName);
		Record.GameTime = UBXFunctionLibrary::GetClientTimeSeconds(this);

		FBXGHitBoxRecords& Records = HitBoxRecords.FindOrAdd(It->Key);
		if (Records.List.Num() <= 0 || Record.GameTime - Records.List.Last().GameTime > UpdateHitBoxRecordInterval * 0.5f)
		{
			Records.List.Add(Record);
		}
	}

	int64 CurrentTS = UBXFunctionLibrary::GetUtcMillisecond();
	if (CurrentTS - CleanBoxRecordsTS > 5000 && !bUsingHitBoxRecords)
	{
		CleanBoxRecordsTS = CurrentTS;
		float CurrentTime = UBXFunctionLibrary::GetClientTimeSeconds(this);
		
		// 移除失效的记录
		for (TMap<FGameplayTag, FBXGHitBoxRecords>::TIterator It(HitBoxRecords); It; ++It)
		{
			It->Value.List.RemoveAll
			(
				[&](const FBXGHitBoxRecord& Record)
				{
					return CurrentTime - Record.GameTime > HitBoxRecordExpiryTime;
				}
			);
		}
	}
}

#pragma endregion Collision
