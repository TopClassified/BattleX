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
UBXShapeComponent* ABXColdWeapon::GetHitBoxComponent()
{
	if (!IsValid(HitBoxComponent))
	{
		return nullptr;
	}
	
	return HitBoxComponent;
}

bool ABXColdWeapon::GetHitBoxTransform(const FGameplayTag& InTag, FTransform& OutTransform)
{
	if (!IsValid(HitBoxComponent))
	{
		return false;
	}

	if (HitBoxComponent->GetShapeTransformByTag(InTag, OutTransform))
	{
		return true;
	}

	return false;
}

#pragma endregion HitBox
