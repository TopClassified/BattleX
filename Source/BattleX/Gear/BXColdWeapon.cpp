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

void ABXColdWeapon::GetHitResults(float InStartTime, FGameplayTagContainer& BoxTags, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, const FBXCFilter& Filter, TArray<FHitResult>& OutResults)
{
	OutResults.Reset();

	if (!IsValid(OwnerComponent))
	{
		return;
	}
	
	float CurrentTime = UBXFunctionLibrary::GetClientTimeSeconds(this);

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

		// 查询N秒之前的位置信息
		int32 StartIndex = 0;
		int32 EndIndex = Records->List.Num() - 1;
		for (int32 i = EndIndex; i >= 0; --i)
		{
			if (Records->List[i].GameTime < InStartTime)
			{
				StartIndex = i;
				break;
			}
		}

		TArray<FHitResult> TempResults;
		for (int32 i = StartIndex; i <= EndIndex; ++i)
		{
			TempResults.Reset();

			// 计算起始位置
			if (i == Records->List.Num() - 1)
			{
				const FBXGHitBoxRecord& Record = Records->List[i];
				Parameter.StartLocation = Record.WTransform.GetLocation();
				Parameter.StartRotation = Record.WTransform.GetRotation().Rotator();

				Parameter.EndLocation = WTransform.GetLocation();
				Parameter.EndRotation = WTransform.GetRotation().Rotator();

				// 根据时间点，调整一下起始信息
				float Alpha = FMath::Max(0.0f, (InStartTime - Record.GameTime) / (CurrentTime - Record.GameTime));
				Parameter.StartLocation = FMath::Lerp(Parameter.StartLocation, Parameter.EndLocation, Alpha);
				Parameter.StartRotation = FMath::Lerp(Parameter.StartRotation, Parameter.EndRotation, Alpha);

				Parameter.Scale = (Record.WTransform.GetScale3D() + WTransform.GetScale3D()) * 0.5f;
			}
			else
			{
				const FBXGHitBoxRecord& Record1 = Records->List[i];
				Parameter.StartLocation = Record1.WTransform.GetLocation();
				Parameter.StartRotation = Record1.WTransform.GetRotation().Rotator();
				
				const FBXGHitBoxRecord& Record2 = Records->List[i + 1];
				Parameter.EndLocation = Record2.WTransform.GetLocation();
				Parameter.EndRotation = Record2.WTransform.GetRotation().Rotator();

				// 根据时间点，调整一下起始信息
				float Alpha = FMath::Max(0.0f, (InStartTime - Record1.GameTime) / (Record2.GameTime - Record1.GameTime));
				Parameter.StartLocation = FMath::Lerp(Parameter.StartLocation, Parameter.EndLocation, Alpha);
				Parameter.StartRotation = FMath::Lerp(Parameter.StartRotation, Parameter.EndRotation, Alpha);

				Parameter.Scale = (Record1.WTransform.GetScale3D() + Record2.WTransform.GetScale3D()) * 0.5f;
			}
			
			// 调用碰撞查询接口
			if (ShapeType == EBXShapeType::ST_Sphere)
			{
				TempResults = UBXCollisionLibrary::SphereCheck(Parameter, ObjectTypes, ShapeSize.X, Filter);
			}
			else if (ShapeType == EBXShapeType::ST_Capsule)
			{
				TempResults = UBXCollisionLibrary::CapsuleCheck(Parameter, ObjectTypes, FVector2D(ShapeSize.X, ShapeSize.Y), Filter);
			}
			else if (ShapeType == EBXShapeType::ST_Box)
			{
				TempResults = UBXCollisionLibrary::BoxCheck(Parameter, ObjectTypes, ShapeSize, Filter);
			}

			// 结果去重合并
			UBXCollisionLibrary::CombineCollisionResults(TempResults, OutResults);
		}
	}
	
	int64 CurrentTS = UBXFunctionLibrary::GetUtcMillisecond();
	if (!bAddingBoxRecords && CurrentTS - CleanBoxRecordsTS > 5000)
	{
		bCleaningBoxRecords = true;
		CleanBoxRecordsTS = CurrentTS;
		
		// 移除失效的记录
		for (TMap<FGameplayTag, FBXGHitBoxRecords>::TIterator It(HitBoxRecords); It; ++It)
		{
			It->Value.List.RemoveAll
			(
				[CurrentTime](const FBXGHitBoxRecord& Record)
				{
					return CurrentTime - Record.GameTime > 1.0f;
				}
			);
		}
		bCleaningBoxRecords = false;
	}
}

void ABXColdWeapon::AddNewHitBoxRecord()
{
	// 正在清理信息队列，直接退出
	if (bCleaningBoxRecords)
	{
		return;
	}
	
	bAddingBoxRecords = true;
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

		// 不要频繁记录
		FBXGHitBoxRecords& Records = HitBoxRecords.FindOrAdd(It->Key);
		if (Record.GameTime - Records.List.Last().GameTime > UpdateHitBoxRecordInterval * 0.5f)
		{
			Records.List.Add(Record);
		}
	}
	bAddingBoxRecords = false;
}

#pragma endregion Collision
