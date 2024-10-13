#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"

#include "BXBehaviorComponent.generated.h"



UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BATTLEX_API UBXBehaviorComponent : public UActorComponent
{
	GENERATED_BODY()

#pragma region Important
public:	
	UBXBehaviorComponent();

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
	// 正在执行的行为
	UPROPERTY(Transient, BlueprintReadWrite)
	FGameplayTagContainer PerformingBehaviors;

	// 行为权限
	UPROPERTY(Transient)
	TMap<FGameplayTag, FInt64Vector2> BehaviorPermissionMap;
	
#pragma endregion Important



#pragma region MovementCallback
protected:
	UFUNCTION()
	void ReceiveDoJump();
	
	UFUNCTION()
	void ReceiveStartProactiveMoving();

	UFUNCTION()
	void ReceiveStopProactiveMoving();
	
	UFUNCTION()
	void ReceiveStartProactiveRotating();
    	
	UFUNCTION()
	void ReceiveStopProactiveRotating();
	
#pragma endregion MovementCallback



#pragma region API
public:
	// 检查当前是否正在进行某种行为
	UFUNCTION(BlueprintCallable)
	static bool IsCurrentlyPerformingBehavior(AActor* InActor, const FGameplayTag& InBehaviorTag);

	// 获取行为权限
	UFUNCTION(BlueprintCallable)
	static int64 GetBehaviorPermission(AActor* InActor, const FGameplayTag& InBehaviorTag, int32 InPriority);
	
	// 归还行为权限
	UFUNCTION(BlueprintCallable)
	static void RevokeBehaviorPermission(AActor* InActor, const FGameplayTag& InBehaviorTag, int64 InPermission);

#pragma endregion API
	
};
