#pragma once

#include "CoreMinimal.h"
#include "InstancedStruct.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"

#include "BXStateEnums.h"
#include "BehaviorAgent/BXBehaviorAgent.h"

#include "BXBehaviorComponent.generated.h"



DECLARE_LOG_CATEGORY_EXTERN(BXCOMP_Behavior, Log, All);



// 行为禁止信息
USTRUCT(BlueprintType)
struct FBXForbiddenBehaviorInformation
{
	GENERATED_USTRUCT_BODY()

public:
	FBXForbiddenBehaviorInformation() {}
	FBXForbiddenBehaviorInformation(int64 InSign, EBXForbiddenBehaviorReason InReason): Sign(InSign), Reason(InReason) {}

	friend bool operator==(const FBXForbiddenBehaviorInformation& Val1, const FBXForbiddenBehaviorInformation& Val2)
	{
		return Val1.Sign == Val2.Sign && Val1.Reason == Val2.Reason;
	}
	
public:
	UPROPERTY(Transient, BlueprintReadWrite)
	int64 Sign = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EBXForbiddenBehaviorReason Reason = EBXForbiddenBehaviorReason::FB_TMax;
	
};

USTRUCT(BlueprintType)
struct FBXForbiddenBehaviorInformationList
{
	GENERATED_USTRUCT_BODY()
	
public:
	UPROPERTY(Transient, BlueprintReadWrite)
	TArray<FBXForbiddenBehaviorInformation> List;
	
};



UCLASS(BlueprintType, Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BATTLEX_API UBXBehaviorComponent : public UActorComponent
{
	GENERATED_BODY()

#pragma region Important
public:	
	UBXBehaviorComponent();

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
	// 行为禁用表（A行为执行中时，要禁用哪些行为）
	UPROPERTY(EditDefaultsOnly)
	TMap<FGameplayTag, FGameplayTagContainer> ForbiddenBehaviorConfigs;

	// 行为中断表（A行为开始执行前，要中断哪些行为）
	UPROPERTY(EditDefaultsOnly)
	TMap<FGameplayTag, FGameplayTagContainer> InterruptBehaviorConfigs;
	
	// 行为代理类
	UPROPERTY(EditDefaultsOnly)
	TMap<FGameplayTag, TSubclassOf<UBXBehaviorAgent>> BehaviorAgentConfigs;

	// 行为代理实例
	UPROPERTY(Transient, BlueprintReadOnly)
	TMap<FGameplayTag, UBXBehaviorAgent*> BehaviorAgents;
	
	// 被禁止的行为
	UPROPERTY(Transient, BlueprintReadWrite)
	TMap<FGameplayTag, FBXForbiddenBehaviorInformationList> ForbiddenBehaviors;

	// 正在执行的行为
	UPROPERTY(Transient, BlueprintReadWrite)
	FGameplayTagContainer ActiveBehaviors;
	
#pragma endregion Important



#pragma region API
public:
	// 检查行为是否被禁止
	UFUNCTION(BlueprintCallable)
	bool CheckForbiddenBehavior(const FGameplayTag& InBehaviorTag);
	UFUNCTION(BlueprintCallable)
	bool CheckForbiddenBehaviorWithReason(const FGameplayTag& InBehaviorTag, EBXForbiddenBehaviorReason& OutReason);
	
	// 改变行为的禁用状态
	UFUNCTION(BlueprintCallable)
	void ChangeForbiddenBehavior(const FGameplayTag& InBehaviorTag, bool bForbidden, int64 InSign, EBXForbiddenBehaviorReason InReason);
	
	// 检查行为是否被激活
	UFUNCTION(BlueprintCallable)
	bool CheckActiveBehavior(const FGameplayTag& InBehaviorTag);

	// 开始行为
	UFUNCTION(BlueprintCallable)
	bool StartBehavior(const FGameplayTag& InBehaviorTag);

	// 开始行为(有参数版本)
	UFUNCTION(BlueprintCallable)
	bool StartBehaviorWithParameter(const FGameplayTag& InBehaviorTag, const FInstancedStruct& InParameter);

	// 停止行为
	UFUNCTION(BlueprintCallable)
	bool StopBehavior(const FGameplayTag& InBehaviorTag);

	// 停止行为(有参数版本)
	UFUNCTION(BlueprintCallable)
	bool StopBehaviorWithParameter(const FGameplayTag& InBehaviorTag, const FInstancedStruct& InParameter);

#pragma endregion API
	
};
