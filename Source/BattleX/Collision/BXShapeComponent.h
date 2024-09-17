#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "BXStructs.h"

#include "BXShapeComponent.generated.h"



UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BATTLEX_API UBXShapeComponent : public UActorComponent
{
	GENERATED_BODY()

#pragma region Important
public:	
	UBXShapeComponent();

public:
	// ÐÎ×´ÐÅÏ¢
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, FBXShapeInformation> ShapeInformations;

#pragma endregion Important



#pragma region Editor
#if WITH_EDITOR
public:
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
#pragma endregion Editor

};
