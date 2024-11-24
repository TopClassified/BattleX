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
	// 形状信息
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FGameplayTag, FBXShapeInformation> ShapeInformations;

#pragma endregion Important



#pragma region API
public:
	UFUNCTION(BlueprintCallable)
	bool GetShapeTransformByTag(const FGameplayTag& InTag, FTransform& OutTransform);
	
#pragma endregion API



#pragma region Editor
#if WITH_EDITOR
public:
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
#pragma endregion Editor

};
