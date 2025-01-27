#include "BXConditionManager.h"
#include "BXSubSystem.h"



DEFINE_LOG_CATEGORY(BXMGR_Condition);



#pragma region Important
UBXConditionManager* UBXConditionManager::Get(UObject* InWorldContext)
{
	UBXConditionManager* Result = nullptr;

	if (!IsValid(InWorldContext))
	{
		return nullptr;
	}

	UWorld* World = InWorldContext->GetWorld();
	if (!IsValid(World))
	{
		return nullptr;
	}
	
	if (UGameInstance* GI = World->GetGameInstance())
	{
		if (UBXSubSystem* BXSS = GI->GetSubsystem<UBXSubSystem>())
		{
			Result = BXSS->GetManagerByClass<UBXConditionManager>();
		}
	}

#if WITH_EDITOR
	if (!IsValid(Result))
	{
		for (TObjectIterator<UBXConditionManager> It; It; ++It)
		{
			if (It->GetOuter() == World)
			{
				Result = *It;
				break;
			}
		}
	}
#endif

	return Result;
}
	
void UBXConditionManager::Initialize()
{
	ConditionToFunctionMap.Reset();

	UClass* Class = GetClass();
	if (!IsValid(Class))
	{
		return;
	}
	
	for (TMap<TSubclassOf<UBXCondition>, FName>::TIterator It(ConditionToFunctionConfig); It; ++It)
	{
		UFunction* Function = Class->FindFunctionByName(It->Value);
		if (!IsValid(Function))
		{
			UE_LOG(BXMGR_Condition, Warning, TEXT("Can't Find Function(%s)."), *It->Value.ToString());
			continue;
		}

		// 参数个数不对
		if (Function->NumParms != 3)
		{
			UE_LOG(BXMGR_Condition, Warning, TEXT("The Function(%s) Argument Count Is Incorrect."), *It->Value.ToString());
			continue;
		}

		FBXConditionFunctionParameter FunctionParameter;
		FunctionParameter.Function = Function;

		bool bValid = true;
		int32 ParamIndex = 0;
		for (TFieldIterator<FProperty> ParamIt(Function); ParamIt; ++ParamIt)
		{
			ParamIndex += 1;

			// 判断第一个参数是否合法
			if (ParamIndex == 1)
			{
				FObjectProperty* Property = CastField<FObjectProperty>(*ParamIt);
				if (!Property || !Property->HasAnyPropertyFlags(CPF_Parm) || Property->HasAnyPropertyFlags(CPF_OutParm) || Property->PropertyClass != It->Key )
				{
					bValid = false;
					UE_LOG(BXMGR_Condition, Warning, TEXT("Failed To Initialize The Condition Execution Function(%s), The First Parameter(Need %s) Is Invalid."), *Function->GetName(), *It->Key->GetName());
					break;
				}

				FunctionParameter.ParameterNames.Add(Property->GetFName());
			}
			// 判断第二个参数是否合法
			else if (ParamIndex == 2)
			{
				FStructProperty* Property = CastField<FStructProperty>(*ParamIt);
				if (!Property || !Property->HasAnyPropertyFlags(CPF_Parm) || Property->HasAnyPropertyFlags(CPF_OutParm))
				{
					bValid = false;
					UE_LOG(BXMGR_Condition, Warning, TEXT("Failed To Initialize The Condition Execution Function(%s), The Second Parameter Is Invalid."), *Function->GetName());
					break;
				}

				FunctionParameter.ParameterNames.Add(Property->GetFName());
			}
			// 判断返回参数是否合法
			else if (ParamIndex == 3)
			{
				FBoolProperty* Property = CastField<FBoolProperty>(*ParamIt);
				if (!Property || !Property->HasAnyPropertyFlags(CPF_Parm) || !Property->HasAnyPropertyFlags(CPF_OutParm))
				{
					bValid = false;
					UE_LOG(BXMGR_Condition, Warning, TEXT("Failed To Initialize The Condition Execution Function(%s), The Return Parameter Is Invalid."), *Function->GetName());
					break;
				}

				FunctionParameter.ParameterNames.Add(Property->GetFName());
			}
			// 不再是参数反射，提前结束
			else
			{
				break;
			}
		}

		if (bValid)
		{
			ConditionToFunctionMap.Add(It->Key, FunctionParameter);
		}
	}
}

void UBXConditionManager::Deinitialize()
{
	
}
#pragma endregion Important



#pragma region Condition
DEFINE_FUNCTION(UBXConditionManager::execCheckCondition)
{
	Stack.MostRecentProperty = nullptr;

	// 更新蓝图虚拟机栈顶指针
	Stack.StepCompiledIn<FProperty>(nullptr);
	// 获取第一个数据的参数的地址
	UBXCondition* ConditionPointer = nullptr;
	FObjectProperty* ConditionProperty = CastField<FObjectProperty>(Stack.MostRecentProperty);
	if (ConditionProperty)
	{
		ConditionPointer = Cast<UBXCondition>(ConditionProperty->GetObjectPtrPropertyValue(Stack.MostRecentPropertyAddress).Get());
	}
		
	// 更新蓝图虚拟机栈顶指针
	Stack.StepCompiledIn<FProperty>(nullptr);
	// 获取第二个无类型参数的内存地址
	uint8* ParameterPointer = Stack.MostRecentPropertyAddress;
	// 获取第二个参数的反射信息
	FStructProperty* ParameterProperty = CastField<FStructProperty>(Stack.MostRecentProperty);

	// 停止对蓝图栈的使用
	P_FINISH;

	bool CheckResult = false;

	if (!IsValid(ConditionPointer) || !ConditionPointer->IsValidLowLevelFast() || !ParameterPointer || !ParameterProperty)
	{
		*(bool*)RESULT_PARAM = CheckResult;
		return;
	}

	UBXConditionManager* Manager = P_THIS_CAST(UBXConditionManager);
	if (!IsValid(Manager) || !Manager->IsValidLowLevel())
	{
		*(bool*)RESULT_PARAM = CheckResult;
		return;
	}
	
	P_NATIVE_BEGIN;
	CheckResult = Manager->CheckCondition(ConditionPointer, ParameterProperty->Struct, ParameterPointer);
	P_NATIVE_END;

	*(bool*)RESULT_PARAM = CheckResult;
}

bool UBXConditionManager::CheckCondition(UBXCondition* InCondition, UScriptStruct* InParameterType, void* InParameterAddress)
{
	if (!IsValid(InCondition) || !IsValid(InParameterType) || !InParameterAddress)
	{
		return false;
	}
	
	if (FBXConditionFunctionParameter* FindResult = ConditionToFunctionMap.Find(InCondition->GetClass()))
	{
		if (!IsValid(FindResult->Function) || FindResult->ParameterNames.Num() != 3)
		{
			return false;
		}

		FObjectProperty* ConditionProperty = CastField<FObjectProperty>(FindResult->Function->FindPropertyByName(FindResult->ParameterNames[0]));
		if (!ConditionProperty || InCondition->GetClass() != ConditionProperty->PropertyClass)
		{
			return false;
		}

		FStructProperty* ParameterProperty = CastField<FStructProperty>(FindResult->Function->FindPropertyByName(FindResult->ParameterNames[1]));
		if (!ParameterProperty || InParameterType != ParameterProperty->Struct)
		{
			return false;
		}

		// 申请函数参数内存
		uint8* Buffer = static_cast<uint8*>(FMemory::MallocZeroed(FindResult->Function->ParmsSize));
		if (!Buffer)
		{
			return false;
		}
		
		// 拷贝条件内存
		FMemory::Memcpy(Buffer, &InCondition, ConditionProperty->GetSize());
		// 拷贝参数内存
		FMemory::Memcpy(Buffer + ConditionProperty->GetSize(), InParameterAddress, ParameterProperty->GetSize());

		// 执行函数
		ProcessEvent(FindResult->Function, Buffer);
		// 得到结果
		bool CheckResult = *(reinterpret_cast<bool*>(Buffer + ConditionProperty->GetSize() + ParameterProperty->GetSize()));

		// 释放内存
		FMemory::Free(Buffer);

		return CheckResult;
	}

	return false;
}
#pragma endregion Condition
