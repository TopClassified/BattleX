#include "BXShapeComponent.h"

#include "BXFunctionLibrary.h"



#pragma region Important 
UBXShapeComponent::UBXShapeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

#pragma endregion Important



#pragma region API
bool UBXShapeComponent::GetShapeTransformByTag(const FGameplayTag& InTag, FTransform& OutTransform)
{
	FBXShapeInformation* Information = ShapeInformations.Find(InTag);
	if (!Information)
	{
		return false;
	}

	AActor* Owner = GetOwner();
	if (!IsValid(Owner))
	{
		return false;
	}
	
	// 找到挂接的父组件
	TWeakObjectPtr<USceneComponent> AttachedComponent = UBXFunctionLibrary::GetSceneComponentByNameAndClass(Owner, Information->AttachParent, nullptr, false);;
	if (!AttachedComponent.IsValid())
	{
		AttachedComponent = Owner->GetRootComponent();
	}

	// 计算碰撞盒世界位置
	OutTransform = Information->Relation * AttachedComponent->GetSocketTransform(Information->Socket.BoneName);

	return true;
}
	
#pragma endregion API



#pragma region Editor
#if WITH_EDITOR
void UBXShapeComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	for (TMap<FGameplayTag, FBXShapeInformation>::TIterator It(ShapeInformations); It; ++It)
	{
		if (It->Value.ShapeType == EBXShapeType::ST_Sphere)
		{
			It->Value.ShapeSize.X = FMath::Max(It->Value.ShapeSize.X, 0.1f);
			It->Value.ShapeSize.Y = 0.0f;
			It->Value.ShapeSize.Z = 0.0f;
		}
		else if (It->Value.ShapeType == EBXShapeType::ST_Capsule)
		{
			It->Value.ShapeSize.X = FMath::Max(It->Value.ShapeSize.X, 0.1f);
			It->Value.ShapeSize.Y = FMath::Max(It->Value.ShapeSize.X, It->Value.ShapeSize.Y);
			It->Value.ShapeSize.Z = 0.0f;
		}
		else if (It->Value.ShapeType == EBXShapeType::ST_Box)
		{
			It->Value.ShapeSize.X = FMath::Max(It->Value.ShapeSize.X, 0.1f);
			It->Value.ShapeSize.Y = FMath::Max(It->Value.ShapeSize.Y, 0.1f);
			It->Value.ShapeSize.Z = FMath::Max(It->Value.ShapeSize.Z, 0.1f);
		}
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif
#pragma endregion Editor
