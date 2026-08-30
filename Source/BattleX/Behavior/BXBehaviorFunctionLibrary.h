#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "Behavior/BXBehaviorStructs.h"
#include "Behavior/BXBehaviorComponent.h"

#include "BXBehaviorFunctionLibrary.generated.h"



// 行为/状态系统静态查询库
UCLASS()
class BATTLEX_API UBXBehaviorFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// 查询目标是否正在进行指定行为(族Tag匹配)
	UFUNCTION(BlueprintCallable, Category = "BattleX|Behavior")
	static bool CheckActiveBehavior(AActor* InTarget, const FGameplayTag& InBehaviorTag);

	// 查询目标行为是否被禁用(禁止命中,挡启动;中断不参与)
	UFUNCTION(BlueprintCallable, Category = "BattleX|Behavior")
	static bool IsBehaviorDisabled(AActor* InTarget, const FGameplayTag& InBehaviorTag);

	// 行为能否开始(聚合判定:挂起/代理权限/拒绝关系(豁免感知)/代理检查)
	UFUNCTION(BlueprintCallable, Category = "BattleX|Behavior")
	static bool CanStartBehavior(AActor* InTarget, const FGameplayTag& InBehaviorTag);

	// 查询目标行为是否处于豁免中(生效期间因其在位而命中的拒绝关系暂不执行)
	UFUNCTION(BlueprintCallable, Category = "BattleX|Behavior")
	static bool IsBehaviorWaived(AActor* InTarget, const FGameplayTag& InBehaviorTag);

	// 开始行为(Sign默认0系统常驻)
	UFUNCTION(BlueprintCallable, Category = "BattleX|Behavior")
	static bool StartBehavior(AActor* InTarget, const FGameplayTag& InBehaviorTag, int64 InSign = 0);

	// 停止行为(仅移除该Sign来源)
	UFUNCTION(BlueprintCallable, Category = "BattleX|Behavior")
	static bool StopBehavior(AActor* InTarget, const FGameplayTag& InBehaviorTag, int64 InSign = 0);

	// 开始行为(有参数版本)
	template<typename T>
	static bool StartBehaviorWithParameter(AActor* InTarget, const FGameplayTag& InBehaviorTag, const T& InParameter, int64 InSign = 0)
	{
		bool bResult = false;

		if (!IsValid(InTarget))
		{
			return bResult;
		}

		if (UBXBehaviorComponent* BehaviorComp = InTarget->FindComponentByClass<UBXBehaviorComponent>())
		{
			FInstancedStruct IS;
			IS.InitializeAs(T::StaticStruct(), reinterpret_cast<const uint8*>(&InParameter));
			bResult = BehaviorComp->StartBehaviorWithParameter(InBehaviorTag, MoveTemp(IS), InSign);
		}

		return bResult;
	}

	// 停止行为(有参数版本)
	template<typename T>
	static bool StopBehaviorWithParameter(AActor* InTarget, const FGameplayTag& InBehaviorTag, const T& InParameter, int64 InSign = 0)
	{
		bool bResult = false;

		if (!IsValid(InTarget))
		{
			return bResult;
		}

		if (UBXBehaviorComponent* BehaviorComp = InTarget->FindComponentByClass<UBXBehaviorComponent>())
		{
			FInstancedStruct IS;
			IS.InitializeAs(T::StaticStruct(), reinterpret_cast<const uint8*>(&InParameter));
			bResult = BehaviorComp->StopBehaviorWithParameter(InBehaviorTag, IS, InSign);
		}

		return bResult;
	}

	// 查询目标是否处于指定状态
	UFUNCTION(BlueprintCallable, Category = "BattleX|State")
	static bool CheckStateActive(AActor* InTarget, const FGameplayTag& InStateTag);

	// 查询目标状态剩余时间(多来源取最小;含无限来源返回浮点最大值,无条目返回-1)
	UFUNCTION(BlueprintCallable, Category = "BattleX|State")
	static float GetStateRemainingSeconds(AActor* InTarget, const FGameplayTag& InStateTag);
};
