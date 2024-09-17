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
	// �������岿λ���ͣ���ȡ���岿λ����
	UFUNCTION(BlueprintCallable)
	bool GetBodyPartByType(EBXBodyPartType InType, FBXBodyPartRTInformation& OutInformation);

	// ��������͹������ƣ���ȡ���岿λ����
	UFUNCTION(BlueprintCallable)
	bool GetBodyPartByComponentAndBone(USceneComponent* InComponent, FName InBoneName, FBXBodyPartRTInformation& OutInformation);

public:
	// ���岿λ������Ϣ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BodyPart")
	TMap<EBXBodyPartType, FBXBodyPartConfig> BodyPartConfigs;

protected:
	// ���岿λ������Ϣ
	UPROPERTY(Transient, BlueprintReadWrite, Category = "BodyPart")
	TMap<EBXBodyPartType, FBXBodyPartRTInformation> BodyPartRTDatas;

#pragma endregion BodyPart

};
