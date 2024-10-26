#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "BXStructs.h"

#include "BXHitReactionComponent.generated.h"



UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BATTLEX_API UBXHitReactionComponent : public UActorComponent
{
	GENERATED_BODY()

#pragma region Important
public:	
	UBXHitReactionComponent();

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

#pragma endregion Important



#pragma region BodyPart
public:
	// 根据身体部位类型，获取身体部位数据
	UFUNCTION(BlueprintCallable)
	bool GetBodyPartByType(FGameplayTag InPartType, FBXBodyPartRTInformation& OutInformation);

	// 根据组件和骨骼名称，获取身体部位数据
	UFUNCTION(BlueprintCallable)
	bool GetBodyPartByComponentAndBone(USceneComponent* InComponent, FName InBoneName, FBXBodyPartRTInformation& OutInformation);

public:
	// 身体部位配置信息
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BodyPart")
	TMap<FGameplayTag, FBXBodyPartConfig> BodyPartConfigs;

protected:
	// 身体部位运行信息
	UPROPERTY(Transient, BlueprintReadWrite, Category = "BodyPart")
	TMap<FGameplayTag, FBXBodyPartRTInformation> BodyPartRTDatas;

#pragma endregion BodyPart

};
