#include "BXStateFunctionLibrary.h"

#include "BXFunctionLibrary.h"
#include "BXBehaviorComponent.h"



bool UBXStateFunctionLibrary::CheckForbiddenBehavior(AActor* InActor, const FGameplayTag& InBehaviorTag)
{
	if (!IsValid(InActor))
	{
		return false;
	}

	if (UBXBehaviorComponent* BXBComp = InActor->FindComponentByClass<UBXBehaviorComponent>())
	{
		return BXBComp->CheckForbiddenBehavior(InBehaviorTag);
	}

	return false;
}

void UBXStateFunctionLibrary::ChangeForbiddenBehavior(AActor* InActor, const FGameplayTag& InBehaviorTag, bool bForbidden, int64 InSign, EBXForbiddenBehaviorReason InReason)
{
	if (!IsValid(InActor))
	{
		return;
	}

	if (UBXBehaviorComponent* BXBComp = InActor->FindComponentByClass<UBXBehaviorComponent>())
	{
		BXBComp->ChangeForbiddenBehavior(InBehaviorTag, bForbidden, InSign, InReason);
	}
}

bool UBXStateFunctionLibrary::CheckActiveBehavior(AActor* InActor, const FGameplayTag& InBehaviorTag)
{
	if (!IsValid(InActor))
	{
		return false;
	}

	if (UBXBehaviorComponent* BXBComp = InActor->FindComponentByClass<UBXBehaviorComponent>())
	{
		return BXBComp->CheckActiveBehavior(InBehaviorTag);
	}
	
	return false;
}

bool UBXStateFunctionLibrary::StartBehavior(AActor* InActor, const FGameplayTag& InBehaviorTag)
{
	bool bResult = false;
		
	if (!IsValid(InActor))
	{
		return bResult;
	}

	if (UBXBehaviorComponent* BXBComp = InActor->FindComponentByClass<UBXBehaviorComponent>())
	{
		bResult = BXBComp->StartBehavior(InBehaviorTag);
	}

	return bResult;
}

DEFINE_FUNCTION(UBXStateFunctionLibrary::execStartBehaviorWithParameter)
{
	Stack.MostRecentProperty = nullptr;

	// 更新蓝图虚拟机栈顶指针
	Stack.StepCompiledIn<FProperty>(nullptr);
	// 获取第一个数据的参数的地址
	UObject** ActorPointer = (UObject**)Stack.MostRecentPropertyAddress;
	
	// 更新蓝图虚拟机栈顶指针
	Stack.StepCompiledIn<FProperty>(nullptr);
	// 获取第二个数据的参数的地址
	FGameplayTag* BehaviorPointer = (FGameplayTag*)Stack.MostRecentPropertyAddress;
		
	// 更新蓝图虚拟机栈顶指针
	Stack.StepCompiledIn<FProperty>(nullptr); 
	// 获取第三个无类型参数的内存地址
	uint8* ParameterPointer = Stack.MostRecentPropertyAddress;
	// 获取第三个参数的反射信息
	FStructProperty* ParameterProperty = CastField<FStructProperty>(Stack.MostRecentProperty);

	// 停止对蓝图栈的使用
	P_FINISH;

	bool bResult = false;
	
	if (!ActorPointer || !BehaviorPointer || !ParameterPointer || !ParameterProperty)
	{
		*(bool*)RESULT_PARAM = bResult;
		return;
	}

	AActor* Actor = Cast<AActor>(*ActorPointer);
	if (!IsValid(Actor))
	{
		*(bool*)RESULT_PARAM = bResult;
		return;
	}
	
	P_NATIVE_BEGIN;
	if (UBXBehaviorComponent* BehaviorComp = Actor->FindComponentByClass<UBXBehaviorComponent>())
	{
		FInstancedStruct IS;
		IS.InitializeAs(ParameterProperty->Struct.Get(), ParameterPointer);
		bResult = BehaviorComp->StartBehaviorWithParameter(*BehaviorPointer, IS);
		IS.Reset();
	}
	P_NATIVE_END;

	*(bool*)RESULT_PARAM = bResult;
}

bool UBXStateFunctionLibrary::StopBehavior(AActor* InActor, const FGameplayTag& InBehaviorTag)
{
	bool bResult = false;
		
	if (!IsValid(InActor))
	{
		return bResult;
	}

	if (UBXBehaviorComponent* BXBComp = InActor->FindComponentByClass<UBXBehaviorComponent>())
	{
		bResult = BXBComp->StopBehavior(InBehaviorTag);
	}

	return bResult;
}

DEFINE_FUNCTION(UBXStateFunctionLibrary::execStopBehaviorWithParameter)
{
	Stack.MostRecentProperty = nullptr;

	// 更新蓝图虚拟机栈顶指针
	Stack.StepCompiledIn<FProperty>(nullptr);
	// 获取第一个数据的参数的地址
	UObject** ActorPointer = (UObject**)Stack.MostRecentPropertyAddress;
	
	// 更新蓝图虚拟机栈顶指针
	Stack.StepCompiledIn<FProperty>(nullptr);
	// 获取第二个数据的参数的地址
	FGameplayTag* BehaviorPointer = (FGameplayTag*)Stack.MostRecentPropertyAddress;
		
	// 更新蓝图虚拟机栈顶指针
	Stack.StepCompiledIn<FProperty>(nullptr); 
	// 获取第三个无类型参数的内存地址
	uint8* ParameterPointer = Stack.MostRecentPropertyAddress;
	// 获取第三个参数的反射信息
	FStructProperty* ParameterProperty = CastField<FStructProperty>(Stack.MostRecentProperty);

	// 停止对蓝图栈的使用
	P_FINISH;

	bool bResult = false;
	
	if (!ActorPointer || !BehaviorPointer || !ParameterPointer || !ParameterProperty)
	{
		*(bool*)RESULT_PARAM = bResult;
		return;
	}

	AActor* Actor = Cast<AActor>(*ActorPointer);
	if (!IsValid(Actor))
	{
		*(bool*)RESULT_PARAM = bResult;
		return;
	}
	
	P_NATIVE_BEGIN;
	if (UBXBehaviorComponent* BehaviorComp = Actor->FindComponentByClass<UBXBehaviorComponent>())
	{
		FInstancedStruct IS;
		IS.InitializeAs(ParameterProperty->Struct.Get(), ParameterPointer);
		bResult = BehaviorComp->StopBehaviorWithParameter(*BehaviorPointer, IS);
		IS.Reset();
	}
	P_NATIVE_END;

	*(bool*)RESULT_PARAM = bResult;
}
