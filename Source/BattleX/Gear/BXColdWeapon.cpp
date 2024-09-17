#include "BXColdWeapon.h"

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
	// ��¼��ײ�е�λ��
	if (RecordBoxNames.Num() > 0)
	{
		AddNewHitBoxRecord(RecordBoxNames);
	}
	else
	{
		SetActorTickEnabled(false);
	}

	float CurrentTime = GetWorld()->GetTimeSeconds();

	// ��������
	if (CurrentTime - LastGCTS > GCInterval)
	{
		LastGCTS = CurrentTime;

		// �Ƴ�ʧЧ�ļ�¼
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

		// ��ȡ��ǰ��λ����Ϣ
		EBXShapeType ShapeType = EBXShapeType::ST_Sphere;
		FVector ShapeSize(FVector::ZeroVector);
		FTransform WTransform = FTransform::Identity;
		if (FBXShapeInformation* Information = HitBoxComponent->ShapeInformations.Find(BoxName))
		{
			USceneComponent* AttachParent = UBXFunctionLibrary::GetSceneComponentByNameAndClass(Owner, Information->AttachToComponent, nullptr, false);
			if (!AttachParent)
			{
				AttachParent = GetRootComponent();
			}

			ShapeType = Information->ShapeType;
			ShapeSize = Information->ShapeSize;
			WTransform = Information->Relation * AttachParent->GetSocketTransform(Information->AttachToSocket.BoneName);
		}
		else
		{
			return;
		}
		Parameter.EndLocation = WTransform.GetLocation();
		EndQuat = WTransform.GetRotation();
		Parameter.Scale = WTransform.GetScale3D();

		// ��ѯN��֮ǰ��λ����Ϣ
		int32 Index = 0;
		for (int32 i = Records->List.Num() - 1; i >= 0; --i)
		{
			if (Records->List[i].GameTime < TargetTime)
			{
				Index = i;
				break;
			}
		}

		// ������ʼλ��
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

		// ������ײ��ѯ�ӿ�
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
			// �ҵ��ҽӵĸ����
			USceneComponent* AttachParent = nullptr;
			if (USceneComponent** FindResult = HelpMap1.Find(Information->AttachToComponent))
			{
				AttachParent = *FindResult;
			}
			else
			{
				AttachParent = UBXFunctionLibrary::GetSceneComponentByNameAndClass(Owner, Information->AttachToComponent, nullptr, false);

				HelpMap1.Add(Information->AttachToComponent, AttachParent);
			}
			if (!AttachParent)
			{
				AttachParent = GetRootComponent();
			}

			// ��¼λ��
			FBXGHitBoxRecord Record;
			Record.WTransform = Information->Relation * AttachParent->GetSocketTransform(Information->AttachToSocket.BoneName);
			Record.GameTime = World->GetTimeSeconds();

			FBXGHitBoxRecords& Records = HitBoxRecords.FindOrAdd(BoxName);
			Records.List.Add(Record);
		}
	}
}

#pragma endregion Collision
