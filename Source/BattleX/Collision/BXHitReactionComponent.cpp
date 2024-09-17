#include "BXHitReactionComponent.h"

#include "BXFunctionLibrary.h"



#pragma region Important
UBXHitReactionComponent::UBXHitReactionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UBXHitReactionComponent::BeginPlay()
{
	SetComponentTickEnabled(false);

	// 初始化身体部位数据
	for (TMap<EBXBodyPartType, FBXBodyPartConfig>::TIterator It(BodyPartConfigs); It; ++It)
	{
		USceneComponent* Component = UBXFunctionLibrary::GetSceneComponentByNameAndClass(GetOwner(), It->Value.ComponentName, UPrimitiveComponent::StaticClass(), false);
		if (IsValid(Component))
		{
			FBXBodyPartRTInformation& NewInformation = BodyPartRTDatas.Add(It->Key);
			NewInformation.PartType = It->Key;
			NewInformation.Component = Component;
			NewInformation.BoneNames.Append(It->Value.BoneNames);
			NewInformation.bCanCollision = It->Value.bCanCollision;
			NewInformation.bCanLock = It->Value.bCanLock;
		}
	}

	Super::BeginPlay();	
}

void UBXHitReactionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{

	Super::EndPlay(EndPlayReason);
}

#pragma endregion Important



#pragma region BodyPart
bool UBXHitReactionComponent::GetBodyPartByType(EBXBodyPartType InType, FBXBodyPartRTInformation& OutInformation)
{
	if (FBXBodyPartRTInformation* BodyPart = BodyPartRTDatas.Find(InType))
	{
		OutInformation = *BodyPart;

		return true;
	}

	return false;
}

bool UBXHitReactionComponent::GetBodyPartByComponentAndBone(USceneComponent* InComponent, FName InBoneName, FBXBodyPartRTInformation& OutInformation)
{
	if (!IsValid(InComponent) || InBoneName.IsNone())
	{
		return false;
	}

	for (TMap<EBXBodyPartType, FBXBodyPartRTInformation>::TIterator It(BodyPartRTDatas); It; ++It)
	{
		if (It->Value.Component == InComponent && It->Value.BoneNames.Contains(InBoneName))
		{
			OutInformation = It->Value;

			return true;
		}
	}

	return false;
}

#pragma endregion BodyPart
