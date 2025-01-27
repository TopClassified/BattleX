#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

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

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

#pragma endregion Important



#pragma region Use
public:
	void InternalPostUsing(UPARAM(ref) FBXUsingGearInformation& UsingInfo) override;

	void InternalPreUnusing(UPARAM(ref) FBXUsingGearInformation& UnusingInfo) override;
	
#pragma endregion Use



#pragma region State
protected:
	virtual void InternalChangeState(FGameplayTag NewState) override;

#pragma endregion State



#pragma region HitBox
public:
	// 获取碰撞盒管理组件
	UFUNCTION(BlueprintCallable)
	UBXShapeComponent* GetHitBoxComponent();
	
	// 获取碰撞盒当前位置
	UFUNCTION(BlueprintCallable)
	bool GetHitBoxTransform(const FGameplayTag& InTag, FTransform& OutTransform);
	
protected:
	// 碰撞信息组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UBXShapeComponent* HitBoxComponent = nullptr;

#pragma endregion HitBox

};