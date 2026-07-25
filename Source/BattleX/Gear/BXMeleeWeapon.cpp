#include "BXMeleeWeapon.h"

#include "Components/SkeletalMeshComponent.h"

#include "BXManager.h"
#include "BXCollision.h"
#include "BXGearComponent.h"
#include "BXFunctionLibrary.h"



#pragma region Important
ABXMeleeWeapon::ABXMeleeWeapon(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	HitBoxComponent = CreateDefaultSubobject<UBXShapeComponent>(TEXT("HitBox"));
}

ABXMeleeWeapon::~ABXMeleeWeapon()
{
	
}

void ABXMeleeWeapon::BeginPlay()
{
	
	Super::BeginPlay();
}

void ABXMeleeWeapon::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

}

#pragma endregion Important



#pragma region Use
void ABXMeleeWeapon::InternalPostUsing(UPARAM(ref) FBXUsingGearInformation& UsingInfo)
{
	
	Super::InternalPostUsing(UsingInfo);
}

void ABXMeleeWeapon::InternalPreUnusing(UPARAM(ref) FBXUsingGearInformation& UnusingInfo)
{
	
	Super::InternalPreUnusing(UnusingInfo);
}
	
#pragma endregion Use



#pragma region State
void ABXMeleeWeapon::InternalChangeState(FGameplayTag NewState)
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
UBXShapeComponent* ABXMeleeWeapon::GetHitBoxComponent()
{
	if (!IsValid(HitBoxComponent))
	{
		return nullptr;
	}
	
	return HitBoxComponent;
}

bool ABXMeleeWeapon::GetHitBoxTransform(const FGameplayTag& InTag, FTransform& OutTransform)
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
