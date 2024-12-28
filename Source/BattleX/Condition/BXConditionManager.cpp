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

		bool bValid = false;
		int32 ParameterCount = 0;
		for (TFieldIterator<FProperty> ParamIt(Function); ParamIt; ++ParamIt)
		{
			ParameterCount += 1;

			// 判断第一个参数是否合法
			if (ParameterCount == 1)
			{
				FObjectProperty* Property = CastField<FObjectProperty>(*ParamIt);
				if (!Property || Property->PropertyClass != It->Key)
				{
					bValid = false;
					UE_LOG(BXMGR_Condition, Warning, TEXT("Failed To Initialize The Condition Execution Function(%s), The First Parameter(Need %s) Is Invalid."), *Function->GetName(), *It->Key->GetName());
					break;
				}

				FunctionParameter.ParameterNames.Add(Property->GetFName());
			}

			// 判断第二个参数是否合法
			if (ParameterCount == 2)
			{
				FStructProperty* Property = CastField<FStructProperty>(*ParamIt);
				if (!Property)
				{
					bValid = false;
					UE_LOG(BXMGR_Condition, Warning, TEXT("Failed To Initialize The Condition Execution Function(%s), The Second Parameter Is Invalid."), *Function->GetName());
					break;
				}

				FunctionParameter.ParameterNames.Add(Property->GetFName());
			}

			// 判断返回参数是否合法
			if (ParameterCount == 3)
			{
				FBoolProperty* Property = CastField<FBoolProperty>(*ParamIt);
				if (!Property || !(Property->PropertyFlags & CPF_ReturnParm))
				{
					bValid = false;
					UE_LOG(BXMGR_Condition, Warning, TEXT("Failed To Initialize The Condition Execution Function(%s), The Return Parameter Is Invalid."), *Function->GetName());
					break;
				}

				FunctionParameter.ParameterNames.Add(Property->GetFName());
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
bool UBXConditionManager::CheckCondition(UBXCondition* InCondition, UScriptStruct* InParameterType, void* InParameterAddress)
{
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
		FMemory::Memcpy(Buffer, InCondition, ConditionProperty->GetSize());
		// 拷贝参数内存
		FMemory::Memcpy(Buffer + ConditionProperty->GetSize(), InParameterAddress, ParameterProperty->GetSize());

		// 执行函数
		ProcessEvent(FindResult->Function, Buffer);
		// 得到结果
		bool CheckResult = *(reinterpret_cast<bool*>(Buffer + FindResult->Function->ReturnValueOffset));

		// 释放内存
		FMemory::Free(Buffer);

		return CheckResult;
	}

	return false;
}
#pragma endregion Condition
