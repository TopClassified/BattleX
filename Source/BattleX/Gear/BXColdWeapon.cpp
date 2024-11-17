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

}

#pragma endregion Important



#pragma region Use
void ABXColdWeapon::InternalPostUsing(UPARAM(ref) FBXUsingGearInformation& UsingInfo)
{
	
	Super::InternalPostUsing(UsingInfo);
}

void ABXColdWeapon::InternalPreUnusing(UPARAM(ref) FBXUsingGearInformation& UnusingInfo)
{
	
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



#pragma region HitBox
bool ABXColdWeapon::GetHitBoxTransform(const FGameplayTag& InTag, FTransform& OutTransform)
{
	if (!IsValid(HitBoxComponent))
	{
		return false;
	}

	if (FBXShapeInformation* SInfo = HitBoxComponent->ShapeInformations.Find(InTag))
	{
		// 找到挂接的父组件
		TWeakObjectPtr<USceneComponent> AttachedComponent = UBXFunctionLibrary::GetSceneComponentByNameAndClass(Owner, SInfo->AttachParent, nullptr, false);;
		if (!AttachedComponent.IsValid())
		{
			AttachedComponent = GetRootComponent();
		}

		// 计算碰撞盒世界位置
		OutTransform = SInfo->Relation * AttachedComponent->GetSocketTransform(SInfo->Socket.BoneName);

		return true;
	}

	return false;
}

#pragma endregion HitBox
