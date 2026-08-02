#include "BXConditionManager.h"
#include "BXSubSystem.h"
#include "HAL/PlatformTime.h"
#include "Engine/World.h"
#include "Task/Condition/BXTaskCondition.h"
#include "DecisionTree/BXDecisionTreeCondition.h"



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
	NativeCheckMap.Reset();

	// 注册组合条件Native检查
	RegisterNativeCheck(UBXTaskConditionComposite::StaticClass(), &UBXConditionManager::NativeCheckTaskComposite);
	RegisterNativeCheck(UBXDecisionTreeConditionComposite::StaticClass(), &UBXConditionManager::NativeCheckDecisionTreeComposite);

	// 绑定 World Tick Start 回调，在帧最早时机刷新时间戳
	FWorldDelegates::OnWorldTickStart.AddUObject(this, &UBXConditionManager::OnWorldTickStart);

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
	FWorldDelegates::OnWorldTickStart.RemoveAll(this);
	NativeCheckMap.Empty();
	DerivedCache_Int.Empty();
	DerivedCache_Float.Empty();
	DerivedCache_Struct.Empty();
}
#pragma endregion Important



#pragma region FrameTime
void UBXConditionManager::OnWorldTickStart(UWorld* InWorld, ELevelTick InTickType, float InDeltaTime)
{
	// 在 World Tick 最早时机刷新当前帧时间戳，确保本帧后续 CheckCondition 调用都用同一值
	CurrentFrameTime = FPlatformTime::Seconds();
}
#pragma endregion FrameTime



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

	const TSubclassOf<UBXCondition> CondClass = InCondition->GetClass();
	bool CheckResult = false;
	bool bEvaluated = false;

	// 快速路径：Native函数直接调用，绕过ProcessEvent
	if (FBXNativeCheckFunc* NativeFunc = NativeCheckMap.Find(CondClass))
	{
		CheckResult = (this->**NativeFunc)(InCondition, InParameterType, InParameterAddress);
		bEvaluated = true;
	}
	// 慢速路径：通过ProcessEvent调用蓝图UFunction
	else if (FBXConditionFunctionParameter* FindResult = ConditionToFunctionMap.Find(CondClass))
	{
		if (IsValid(FindResult->Function) && FindResult->ParameterNames.Num() == 3)
		{
			FObjectProperty* ConditionProperty = CastField<FObjectProperty>(FindResult->Function->FindPropertyByName(FindResult->ParameterNames[0]));
			FStructProperty* ParameterProperty = CastField<FStructProperty>(FindResult->Function->FindPropertyByName(FindResult->ParameterNames[1]));

			if (ConditionProperty && ParameterProperty
				&& CondClass == ConditionProperty->PropertyClass
				&& InParameterType == ParameterProperty->Struct)
			{
				uint8* Buffer = static_cast<uint8*>(FMemory::MallocZeroed(FindResult->Function->ParmsSize));
				if (Buffer)
				{
					FMemory::Memcpy(Buffer, &InCondition, ConditionProperty->GetSize());
					FMemory::Memcpy(Buffer + ConditionProperty->GetSize(), InParameterAddress, ParameterProperty->GetSize());

					ProcessEvent(FindResult->Function, Buffer);
					CheckResult = *(reinterpret_cast<bool*>(Buffer + ConditionProperty->GetSize() + ParameterProperty->GetSize()));
					FMemory::Free(Buffer);
					bEvaluated = true;
				}
			}
		}
	}

	if (!bEvaluated)
	{
		return false;
	}

	return InCondition->bNot ? !CheckResult : CheckResult;
}
#pragma endregion Condition



#pragma region DerivedKey
bool FBXDerivedKey::operator==(const FBXDerivedKey& Other) const
{
	return ConditionClass == Other.ConditionClass
		&& ParamType == Other.ParamType
		&& ParamHash == Other.ParamHash;
}

uint32 GetTypeHash(const FBXDerivedKey& Key)
{
	return HashCombineFast(
		HashCombineFast(
			GetTypeHash(Key.ConditionClass),
			GetTypeHash(Key.ParamType)
		),
		Key.ParamHash
	);
}
#pragma endregion DerivedKey



#pragma region ConditionFunctions
void UBXConditionManager::RegisterNativeCheck(TSubclassOf<UBXCondition> InClass, FBXNativeCheckFunc InFunc)
{
	if (InClass && InFunc)
	{
		NativeCheckMap.Add(InClass, InFunc);
	}
}

bool UBXConditionManager::NativeCheckTaskComposite(UBXCondition* InCondition, UScriptStruct* InParameterType, void* InParameterAddress)
{
	UBXTaskConditionComposite* Composite = Cast<UBXTaskConditionComposite>(InCondition);
	if (!Composite)
	{
		return false;
	}

	// 空Children永远返回True
	if (Composite->Children.Num() == 0)
	{
		return true;
	}

	for (UBXTaskCondition* Child : Composite->Children)
	{
		if (!IsValid(Child))
		{
			continue;
		}

		const bool bChildResult = CheckCondition(Child, InParameterType, InParameterAddress);

		if (Composite->Logic == EBXLogicOperator::And)
		{
			if (!bChildResult)
			{
				return false;
			}
		}
		else
		{
			if (bChildResult)
			{
				return true;
			}
		}
	}

	return Composite->Logic == EBXLogicOperator::And;
}

bool UBXConditionManager::NativeCheckDecisionTreeComposite(UBXCondition* InCondition, UScriptStruct* InParameterType, void* InParameterAddress)
{
	UBXDecisionTreeConditionComposite* Composite = Cast<UBXDecisionTreeConditionComposite>(InCondition);
	if (!Composite)
	{
		return false;
	}

	if (Composite->Children.Num() == 0)
	{
		return true;
	}

	for (UBXDecisionTreeCondition* Child : Composite->Children)
	{
		if (!IsValid(Child))
		{
			continue;
		}

		const bool bChildResult = CheckCondition(Child, InParameterType, InParameterAddress);

		if (Composite->Logic == EBXLogicOperator::And)
		{
			if (!bChildResult)
			{
				return false;
			}
		}
		else
		{
			if (bChildResult)
			{
				return true;
			}
		}
	}

	return Composite->Logic == EBXLogicOperator::And;
}
#pragma endregion ConditionFunctions
