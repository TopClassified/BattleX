#include "BXColdWeapon.h"

#include "Components/SkeletalMeshComponent.h"

#include "BXCollision.h"
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
	SetActorTickEnabled(false);

	Super::BeginPlay();
}

void ABXColdWeapon::TickActor(float DeltaTime, enum ELevelTick TickType, FActorTickFunction& ThisTickFunction)
{
	// 记录碰撞盒的位置
	if (RecordBoxNames.Num() > 0)
	{
		AddNewHitBoxRecord(RecordBoxNames);
	}
	else
	{
		SetActorTickEnabled(false);
	}

	float CurrentTime = GetWorld()->GetTimeSeconds();

	// 垃圾回收
	if (CurrentTime - LastGCTS > GCInterval)
	{
		LastGCTS = CurrentTime;

		// 移除失效的记录
		for (TMap<FName, FBXGHitBoxRecords>::TIterator It(HitBoxRecords); It; ++It)
		{
			It->Value.List.RemoveAll
			(
				[CurrentTime](const FBXGHitBoxRecord& Record)
				{
					return CurrentTime - Record.GameTime > 1.0f;
				}
			);
		}
	}

	Super::TickActor(DeltaTime, TickType, ThisTickFunction);
}

#pragma endregion Important



#pragma region State
void ABXColdWeapon::InternalChangeState(EBXGearState NewState)
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
void ABXColdWeapon::GetAllHitBoxNames(TArray<FName>& OutBoxNames)
{
	OutBoxNames.Reset();

	if (HitBoxComponent)
	{
		HitBoxComponent->ShapeInformations.GetKeys(OutBoxNames);
	}
}

void ABXColdWeapon::StartRecordLocation(const TArray<FName>& BoxNames)
{
	if (!HitBoxComponent)
	{
		return;
	}

	for (const FName& BoxName : BoxNames)
	{
		RecordBoxNames.AddUnique(BoxName);
	}

	AddNewHitBoxRecord(BoxNames);

	SetActorTickEnabled(true);
}

void ABXColdWeapon::StopRecordLocation(const TArray<FName>& BoxNames)
{
	for (const FName& BoxName : BoxNames)
	{
		RecordBoxNames.RemoveSwap(BoxName);
	}
}

void ABXColdWeapon::GetHitResultsInSeconds(AActor* Requester, float Seconds, const FName& BoxName, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, const FBXCFilter& Filter, TArray<FHitResult>& OutResults)
{
	OutResults.Reset();

	if (!RecordBoxNames.Contains(BoxName))
	{
		return;
	}
	
	float CurrentTime = GetWorld()->GetTimeSeconds();
	float TargetTime = CurrentTime - Seconds;

	if (FBXGHitBoxRecords* Records = HitBoxRecords.Find(BoxName))
	{
		FQuat StartQuat, EndQuat;
		FBXCParameter Parameter;
		Parameter.Requester = Requester;

		// 获取当前的位置信息
		EBXShapeType ShapeType = EBXShapeType::ST_Sphere;
		FVector ShapeSize(FVector::ZeroVector);
		FTransform WTransform = FTransform::Identity;
		if (FBXShapeInformation* Information = HitBoxComponent->ShapeInformations.Find(BoxName))
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
			return;
		}
		Parameter.EndLocation = WTransform.GetLocation();
		EndQuat = WTransform.GetRotation();
		Parameter.Scale = WTransform.GetScale3D();

		// 查询N秒之前的位置信息
		int32 Index = 0;
		for (int32 i = Records->List.Num() - 1; i >= 0; --i)
		{
			if (Records->List[i].GameTime < TargetTime)
			{
				Index = i;
				break;
			}
		}

		// 计算起始位置
		if (Index == Records->List.Num() - 1)
		{
			const FBXGHitBoxRecord& Record = Records->List[Index];
			Parameter.StartLocation = Record.WTransform.GetLocation();
			StartQuat = Record.WTransform.GetRotation();

			float Alpha = Seconds / FMath::Max(CurrentTime - Record.GameTime, 0.0001f);
			Alpha = FMath::Clamp(Alpha, 0.0f, 1.0f);

			Parameter.StartLocation = FMath::Lerp(Parameter.StartLocation, Parameter.EndLocation, Alpha);
			StartQuat = FQuat::Slerp(StartQuat, EndQuat, Alpha);
		}
		else
		{
			const FBXGHitBoxRecord& Record1 = Records->List[Index];
			const FBXGHitBoxRecord& Record2 = Records->List[Index + 1];

			float Alpha = (Record2.GameTime - TargetTime) / FMath::Max(Record2.GameTime - Record1.GameTime, 0.0001f);
			Alpha = FMath::Clamp(Alpha, 0.0f, 1.0f);

			Parameter.StartLocation = FMath::Lerp(Record1.WTransform.GetLocation(), Record2.WTransform.GetLocation(), Alpha);
			StartQuat = FQuat::Slerp(Record1.WTransform.GetRotation(), Record2.WTransform.GetRotation(), Alpha);
		}

		Parameter.StartRotation = StartQuat.Rotator();
		Parameter.EndRotation = EndQuat.Rotator();

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
}

void ABXColdWeapon::AddNewHitBoxRecord(const TArray<FName>& BoxNames)
{
	UClass* ActorClass = GetClass();
	UWorld* World = GetWorld();

	HelpMap1.Reset();
	for (const FName& BoxName : BoxNames)
	{
		if (FBXShapeInformation* Information = HitBoxComponent->ShapeInformations.Find(BoxName))
		{
			// 找到挂接的父组件
			USceneComponent* AttachedComponent = nullptr;
			if (USceneComponent** FindResult = HelpMap1.Find(Information->AttachParent))
			{
				AttachedComponent = *FindResult;
			}
			else
			{
				AttachedComponent = UBXFunctionLibrary::GetSceneComponentByNameAndClass(Owner, Information->AttachParent, nullptr, false);

				HelpMap1.Add(Information->AttachParent, AttachedComponent);
			}
			if (!AttachedComponent)
			{
				AttachedComponent = GetRootComponent();
			}

			// 记录位置
			FBXGHitBoxRecord Record;
			Record.WTransform = Information->Relation * AttachedComponent->GetSocketTransform(Information->Socket.BoneName);
			Record.GameTime = World->GetTimeSeconds();

			FBXGHitBoxRecords& Records = HitBoxRecords.FindOrAdd(BoxName);
			Records.List.Add(Record);
		}
	}
}

#pragma endregion Collision
