#include "BXShapeComponent.h"



#pragma region Important 
UBXShapeComponent::UBXShapeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

#pragma endregion Important



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
