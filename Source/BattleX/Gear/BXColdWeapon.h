#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ShapeComponent.h"

#include "BXGear.h"
#include "BXShapeComponent.h"

#include "BXColdWeapon.generated.h"



UCLASS(Abstract, Blueprintable, BlueprintType)
class BATTLEX_API ABXColdWeapon : public ABXGear
{
	GENERATED_BODY()

#pragma region Important
public:
	ABXColdWeapon(const FObjectInitializer& ObjectInitializer);

	virtual ~ABXColdWeapon();

	virtual void BeginPlay() override;

	virtual void TickActor(float DeltaTime, enum ELevelTick TickType, FActorTickFunction& ThisTickFunction) override;

protected:
	// ����������
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GCInterval = 10.0f;
	// ��һ������������ʱ���
	float LastGCTS = 0.0f;

	// ��ײ��Ϣ���
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UBXShapeComponent* HitBoxComponent = nullptr;

#pragma endregion Important



#pragma region Collision
public:
	// ��ȡ���е���ײ������
	UFUNCTION(BlueprintCallable, Category = "Collision")
	virtual void GetAllHitBoxNames(TArray<FName>& OutBoxNames);

	// ��ʼ��¼λ��
	UFUNCTION(BlueprintCallable, Category = "Collision")
	virtual void StartRecordLocation(const TArray<FName>& BoxNames);

	// ֹͣ��¼λ��
	UFUNCTION(BlueprintCallable, Category = "Collision")
	virtual void StopRecordLocation(const TArray<FName>& BoxNames);

	// ��ȡN���ڣ�ĳ����ײ�е���ײ�����Ϣ
	UFUNCTION(BlueprintCallable, Category = "Collision")
	virtual void GetHitResultsInSeconds(AActor* Requester, float Seconds, const FName& BoxName, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, const FBXCFilter& Filter, TArray<FHitResult>& OutResults);

protected:
	virtual void AddNewHitBoxRecord(const TArray<FName>& BoxNames);

protected:
	// Ҫ��¼����ײ������
	UPROPERTY(Transient, BlueprintReadWrite, Category = "Collision")
	TArray<FName> RecordBoxNames;

	// 1������ײ��λ����Ϣ
	UPROPERTY(Transient, BlueprintReadWrite, Category = "Collision")
	TMap<FName, FBXGHitBoxRecords> HitBoxRecords;

	// ��������
	UPROPERTY(Transient)
	TMap<FName, USceneComponent*> HelpMap1;

#pragma endregion Collision

};