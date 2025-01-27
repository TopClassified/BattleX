#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "BXBehaviorComponent.h"

#include "BXStateFunctionLibrary.generated.h"



UCLASS()
class BATTLEX_API UBXStateFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public: 
	// 检查行为是否被禁止
	UFUNCTION(BlueprintCallable)
	static bool CheckForbiddenBehavior(AActor* InActor, const FGameplayTag& InBehaviorTag);
	
	// 改变行为的禁用状态
	UFUNCTION(BlueprintCallable)
	static void ChangeForbiddenBehavior(AActor* InActor, const FGameplayTag& InBehaviorTag, bool bForbidden, int64 InSign, EBXForbiddenBehaviorReason InReason);
	
	// 检查当前是否正在进行某种行为
	UFUNCTION(BlueprintCallable)
	static bool CheckActiveBehavior(AActor* InActor, const FGameplayTag& InBehaviorTag);

	// 开始行为
	UFUNCTION(BlueprintCallable)
	static bool StartBehavior(AActor* InActor, const FGameplayTag& InBehaviorTag);
	
	// 开始行为(有参数版本)
	template<typename T>
	static bool StartBehaviorWithParameter(AActor* InActor, const FGameplayTag& InBehaviorTag, const T& InParameter)
	{
		bool bResult = false;
		
		if (!IsValid(InActor))
		{
			return bResult;
		}

		if (UBXBehaviorComponent* BXBComp = InActor->FindComponentByClass<UBXBehaviorComponent>())
		{
			FInstancedStruct IS;
			IS.InitializeAs(T::StaticStruct(), reinterpret_cast<const uint8*>(&InParameter));
			bResult = BXBComp->StartBehaviorWithParameter(InBehaviorTag, IS);
			IS.Reset();
		}

		return bResult;
	}
	UFUNCTION(BlueprintCallable, CustomThunk, meta = (CustomStructureParam = "InParameter"))
	static bool StartBehaviorWithParameter(AActor* InActor, const FGameplayTag& InBehaviorTag, int32 InParameter);
	DECLARE_FUNCTION(execStartBehaviorWithParameter);

	// 停止行为
	UFUNCTION(BlueprintCallable)
	static bool StopBehavior(AActor* InActor, const FGameplayTag& InBehaviorTag);

	// 停止行为(有参数版本)
	template<typename T>
	static bool StopBehaviorWithParameter(AActor* InActor, const FGameplayTag& InBehaviorTag, const T& InParameter)
	{
		bool bResult = false;
		
		if (!IsValid(InActor))
		{
			return bResult;
		}

		if (UBXBehaviorComponent* BXBComp = InActor->FindComponentByClass<UBXBehaviorComponent>())
		{
			FInstancedStruct IS;
			IS.InitializeAs(T::StaticStruct(), reinterpret_cast<const uint8*>(&InParameter));
			bResult = BXBComp->StopBehaviorWithParameter(InBehaviorTag, IS);
			IS.Reset();
		}

		return bResult;
	}
	UFUNCTION(BlueprintCallable, CustomThunk, meta = (CustomStructureParam = "InParameter"))
	static bool StopBehaviorWithParameter(AActor* InActor, const FGameplayTag& InBehaviorTag, int32 InParameter);
	DECLARE_FUNCTION(execStopBehaviorWithParameter);
	
};
