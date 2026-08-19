#include "BXConditionManager.h"
#include "BXSubSystem.h"
#include "HAL/PlatformTime.h"
#include "Engine/World.h"
#include "Task/BXTaskCondition.h"
#include "DecisionTree/BXDecisionTreeCondition.h"



DEFINE_LOG_CATEGORY(BXMGR_Condition);



// Native条件检查函数Registry（函数名 → 成员函数指针），用函数内静态变量避免跨翻译单元初始化顺序问题
static TMap<FName, UBXConditionManager::FBXNativeCheckFunc>& AccessNativeCheckRegistry()
{
	static TMap<FName, UBXConditionManager::FBXNativeCheckFunc> Registry;
	return Registry;
}

// 宏：定义Native条件检查函数并自动注册到Registry，漏写宏=函数未定义=链接错误
#define IMPLEMENT_NATIVE_CHECK(Class, FuncName) \
	static bool GRegistered_##FuncName = (AccessNativeCheckRegistry().Add(TEXT(#FuncName), &Class::FuncName), true); \
	bool Class::FuncName(UBXCondition* InCondition, int64 InParameterTypeAddress, int64 InParameterAddress)



// Native参数构造函数Registry（函数名 → 成员函数指针），用函数内静态变量避免跨翻译单元初始化顺序问题
static TMap<FName, UBXConditionManager::FBXNativeTaskParamBuilderFunc>& AccessNativeTaskParamBuilderRegistry()
{
	static TMap<FName, UBXConditionManager::FBXNativeTaskParamBuilderFunc> Registry;
	return Registry;
}

// 宏：定义Native参数构造函数并自动注册到Registry，漏写宏=函数未定义=链接错误
#define IMPLEMENT_NATIVE_TASK_PARAM_BUILDER(Class, FuncName) \
	static bool GRegistered_##FuncName = (AccessNativeTaskParamBuilderRegistry().Add(TEXT(#FuncName), &Class::FuncName), true); \
	void Class::FuncName(UBXTaskCondition* InCondition, const FBXTLRunTimeData& InRTData, const FBXTLSectionRTData& InRTSData, const FBXTLTaskRTData& InRTTData, int64 InOutParamAddress)



#pragma region DerivedKey
bool FBXDerivedKey::operator==(const FBXDerivedKey& Other) const
{
	return ConditionClass == Other.ConditionClass && ParamType == Other.ParamType && ParamHash == Other.ParamHash;
}

uint32 GetTypeHash(const FBXDerivedKey& Key)
{
	return HashCombineFast(HashCombineFast(GetTypeHash(Key.ConditionClass), GetTypeHash(Key.ParamType)), Key.ParamHash);
}
#pragma endregion DerivedKey



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
	NativeTaskParamBuilderMap.Reset();
	TaskConditionParamBuilderMap.Reset();

	// 绑定 World Tick Start 回调，在帧最早时机刷新时间戳
	FWorldDelegates::OnWorldTickStart.AddUObject(this, &UBXConditionManager::OnWorldTickStart);

	UClass* Class = GetClass();
	if (!IsValid(Class))
	{
		return;
	}

	// 加载条件检查函数配置（ConditionClass → 函数名），Registry命中=Native，否则走蓝图UFunction通道
	for (TMap<TSubclassOf<UBXCondition>, FBXFunctionSelector>::TIterator It(ConditionToFunctionConfig); It; ++It)
	{
		if (!It->Key)
		{
			continue;
		}

		// Native函数：Registry命中即注册成员函数指针，绕过ProcessEvent（无需UFUNCTION/反射）
		if (const FBXNativeCheckFunc* NativeFunc = AccessNativeCheckRegistry().Find(It->Value.FunctionName))
		{
			NativeCheckMap.Add(It->Key, *NativeFunc);
			continue;
		}

		// 蓝图函数：通过反射查找UFunction并校验参数签名
		UFunction* Function = Class->FindFunctionByName(It->Value.FunctionName);
		if (!IsValid(Function))
		{
			UE_LOG(BXMGR_Condition, Warning, TEXT("Can't Find Function(%s)."), *It->Value.FunctionName.ToString());
			continue;
		}

		if (Function->NumParms != 3)
		{
			UE_LOG(BXMGR_Condition, Warning, TEXT("The Function(%s) Argument Count Is Incorrect."), *It->Value.FunctionName.ToString());
			continue;
		}

		FBXConditionFunctionParameter FunctionParameter;
		FunctionParameter.Function = Function;

		bool bValid = true;
		int32 ParamIndex = 0;
		for (TFieldIterator<FProperty> ParamIt(Function); ParamIt; ++ParamIt)
		{
			ParamIndex += 1;

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

	// 加载条件参数构造函数配置（TaskConditionClass → Builder函数名），Registry命中=Native，否则走蓝图UFunction通道
	for (TMap<TSubclassOf<UBXTaskCondition>, FBXFunctionSelector>::TIterator It(TaskConditionParamBuilderConfig); It; ++It)
	{
		if (!It->Key)
		{
			continue;
		}

		// Native函数：Registry命中即注册成员函数指针，绕过ProcessEvent（无需UFUNCTION/反射）
		if (const FBXNativeTaskParamBuilderFunc* NativeFunc = AccessNativeTaskParamBuilderRegistry().Find(It->Value.FunctionName))
		{
			NativeTaskParamBuilderMap.Add(It->Key, *NativeFunc);
			continue;
		}

		// 蓝图Builder：通过反射查找UFunction并校验参数签名（签名：Condition, OutParam&）
		UFunction* Function = Class->FindFunctionByName(It->Value.FunctionName);
		if (!IsValid(Function))
		{
			UE_LOG(BXMGR_Condition, Warning, TEXT("Can't Find ParamBuilder Function(%s)."), *It->Value.FunctionName.ToString());
			continue;
		}

		if (Function->NumParms != 2)
		{
			UE_LOG(BXMGR_Condition, Warning, TEXT("The ParamBuilder Function(%s) Argument Count Is Incorrect."), *It->Value.FunctionName.ToString());
			continue;
		}

		FBXConditionFunctionParameter FunctionParameter;
		FunctionParameter.Function = Function;

		bool bValid = true;
		int32 ParamIndex = 0;
		for (TFieldIterator<FProperty> ParamIt(Function); ParamIt; ++ParamIt)
		{
			ParamIndex += 1;
			FProperty* Property = *ParamIt;

			if (!Property || !Property->HasAnyPropertyFlags(CPF_Parm))
			{
				bValid = false;
				break;
			}

			FunctionParameter.ParameterNames.Add(Property->GetFName());
		}

		if (bValid && FunctionParameter.ParameterNames.Num() == 2)
		{
			TaskConditionParamBuilderMap.Add(It->Key, FunctionParameter);
		}
	}
}

void UBXConditionManager::Deinitialize()
{
	FWorldDelegates::OnWorldTickStart.RemoveAll(this);
	NativeCheckMap.Empty();
	ConditionToFunctionMap.Empty();
	NativeTaskParamBuilderMap.Empty();
	TaskConditionParamBuilderMap.Empty();
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

bool UBXConditionManager::CheckCondition(UBXCondition* InCondition, UScriptStruct* InParameterType, const void* InParameterAddress)
{
	if (!IsValid(InCondition) || !IsValid(InParameterType) || !InParameterAddress)
	{
		return false;
	}

	const TSubclassOf<UBXCondition> CondClass = InCondition->GetClass();
	bool CheckResult = false;
	bool bEvaluated = false;

	// 快速路径：Native函数直接调用，绕过ProcessEvent（int64承载指针地址，const_cast仅为绕过int64承载的签名限制，Native函数内部不应修改入参）
	if (FBXNativeCheckFunc* NativeFunc = NativeCheckMap.Find(CondClass))
	{
		CheckResult = (this->**NativeFunc)(InCondition, reinterpret_cast<int64>(InParameterType), reinterpret_cast<int64>(const_cast<void*>(InParameterAddress)));
		bEvaluated = true;
	}
	// 慢速路径：通过ProcessEvent调用蓝图UFunction
	else if (FBXConditionFunctionParameter* FindResult = ConditionToFunctionMap.Find(CondClass))
	{
		if (IsValid(FindResult->Function) && FindResult->ParameterNames.Num() == 3)
		{
			FObjectProperty* ConditionProperty = CastField<FObjectProperty>(FindResult->Function->FindPropertyByName(FindResult->ParameterNames[0]));
			FStructProperty* ParameterProperty = CastField<FStructProperty>(FindResult->Function->FindPropertyByName(FindResult->ParameterNames[1]));

			if (ConditionProperty && ParameterProperty && CondClass == ConditionProperty->PropertyClass && InParameterType == ParameterProperty->Struct)
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

void UBXConditionManager::RegisterNativeCheck(TSubclassOf<UBXCondition> InClass, FBXNativeCheckFunc InFunc)
{
	if (InClass && InFunc)
	{
		NativeCheckMap.Add(InClass, InFunc);
	}
}
#pragma endregion Condition



#pragma region ConditionFunctions
IMPLEMENT_NATIVE_CHECK(UBXConditionManager, NativeCheckTaskComposite)
{
	check(InParameterTypeAddress != 0 && InParameterAddress != 0);

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

	UScriptStruct* ParameterType = reinterpret_cast<UScriptStruct*>(InParameterTypeAddress);
	void* ParameterAddress = reinterpret_cast<void*>(InParameterAddress);

	for (UBXTaskCondition* Child : Composite->Children)
	{
		if (!IsValid(Child))
		{
			continue;
		}

		const bool bChildResult = CheckCondition(Child, ParameterType, ParameterAddress);

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

IMPLEMENT_NATIVE_CHECK(UBXConditionManager, NativeCheckDecisionTreeComposite)
{
	check(InParameterTypeAddress != 0 && InParameterAddress != 0);

	UBXDecisionTreeConditionComposite* Composite = Cast<UBXDecisionTreeConditionComposite>(InCondition);
	if (!Composite)
	{
		return false;
	}

	if (Composite->Children.Num() == 0)
	{
		return true;
	}

	UScriptStruct* ParameterType = reinterpret_cast<UScriptStruct*>(InParameterTypeAddress);
	void* ParameterAddress = reinterpret_cast<void*>(InParameterAddress);

	for (UBXDecisionTreeCondition* Child : Composite->Children)
	{
		if (!IsValid(Child))
		{
			continue;
		}

		const bool bChildResult = CheckCondition(Child, ParameterType, ParameterAddress);

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



#pragma region TaskConditionParamBuilder
void UBXConditionManager::RegisterNativeTaskParamBuilder(TSubclassOf<UBXTaskCondition> InClass, FBXNativeTaskParamBuilderFunc InFunc)
{
	if (InClass && InFunc)
	{
		NativeTaskParamBuilderMap.Add(InClass, InFunc);
	}
}

DEFINE_FUNCTION(UBXConditionManager::execBuildTaskConditionParameter)
{
	// 参数1: UBXTaskCondition*
	Stack.MostRecentProperty = nullptr;
	Stack.StepCompiledIn<FProperty>(nullptr);
	UBXTaskCondition* ConditionPointer = nullptr;
	if (FObjectProperty* ConditionProperty = CastField<FObjectProperty>(Stack.MostRecentProperty))
	{
		ConditionPointer = Cast<UBXTaskCondition>(ConditionProperty->GetObjectPtrPropertyValue(Stack.MostRecentPropertyAddress).Get());
	}

	// 参数2: FBXTLRunTimeData（拷贝到本地，供后续C++调用按引用使用）
	FBXTLRunTimeData RTData;
	Stack.StepCompiledIn<FStructProperty>(&RTData);

	// 参数3: FBXTLSectionRTData
	FBXTLSectionRTData RTSData;
	Stack.StepCompiledIn<FStructProperty>(&RTSData);

	// 参数4: FBXTLTaskRTData
	FBXTLTaskRTData RTTData;
	Stack.StepCompiledIn<FStructProperty>(&RTTData);

	// 参数5: 通配符输出参数（获取蓝图侧变量地址与反射信息）
	Stack.MostRecentProperty = nullptr;
	Stack.StepCompiledIn<FProperty>(nullptr);
	uint8* OutParamAddress = Stack.MostRecentPropertyAddress;
	FStructProperty* OutParamProperty = CastField<FStructProperty>(Stack.MostRecentProperty);

	P_FINISH;

	bool bResult = false;

	UBXConditionManager* Manager = P_THIS_CAST(UBXConditionManager);
	if (IsValid(Manager) && IsValid(ConditionPointer) && OutParamAddress && OutParamProperty)
	{
		P_NATIVE_BEGIN;
		bResult = Manager->InternalBuildTaskConditionParameter(ConditionPointer, RTData, RTSData, RTTData, OutParamProperty->Struct, OutParamAddress);
		P_NATIVE_END;
	}

	*(bool*)RESULT_PARAM = bResult;
}

bool UBXConditionManager::InternalBuildTaskConditionParameter(UBXTaskCondition* InCondition, const FBXTLRunTimeData& InRTData, const FBXTLSectionRTData& InRTSData, const FBXTLTaskRTData& InRTTData, UScriptStruct* InOutParamType, void* InOutParamAddress)
{
	if (!IsValid(InCondition) || !IsValid(InOutParamType) || !InOutParamAddress)
	{
		return false;
	}

	// 向上回溯查找Builder（先Native通道，再蓝图UFunction通道）
	for (UClass* Class = InCondition->GetClass(); Class; Class = Class->GetSuperClass())
	{
		if (!Class->IsChildOf<UBXTaskCondition>())
		{
			break;
		}

		// 快速路径：Native函数直接调用，写入调用方提供的内存
		if (FBXNativeTaskParamBuilderFunc* NativeFunc = NativeTaskParamBuilderMap.Find(Class))
		{
			// Native写入按FBXTaskConditionParameter布局,调用方传入无关小结构会越界写(蓝图路径与fallback均有此校验)
			if (!InOutParamType->IsChildOf(FBXTaskConditionParameter::StaticStruct()))
			{
				UE_LOG(BXMGR_Condition, Warning, TEXT("InternalBuildTaskConditionParameter: ParamType(%s) is not child of FBXTaskConditionParameter, cannot use NativeBuilder."), *InOutParamType->GetName());
				return false;
			}

			(this->**NativeFunc)(InCondition, InRTData, InRTSData, InRTTData, reinterpret_cast<int64>(InOutParamAddress));
			return true;
		}

		// 慢速路径：通过ProcessEvent调用蓝图UFunction（签名：Condition, OutParam&）
		if (FBXConditionFunctionParameter* FindResult = TaskConditionParamBuilderMap.Find(Class))
		{
			if (!IsValid(FindResult->Function) || FindResult->ParameterNames.Num() != 2)
			{
				continue;
			}

			FObjectProperty* ConditionProperty = CastField<FObjectProperty>(FindResult->Function->FindPropertyByName(FindResult->ParameterNames[0]));
			FStructProperty* ParamProperty = CastField<FStructProperty>(FindResult->Function->FindPropertyByName(FindResult->ParameterNames[1]));

			// 校验Condition属性类匹配，且OutParam类型是蓝图Builder声明类型的派生类或相同（避免越界写入）
			if (!ConditionProperty || !ParamProperty || !ConditionProperty->PropertyClass || !ConditionProperty->PropertyClass->IsChildOf(Class))
			{
				continue;
			}

			if (!InOutParamType->IsChildOf(ParamProperty->Struct))
			{
				UE_LOG(BXMGR_Condition, Warning, TEXT("InternalBuildTaskConditionParameter: ParamType(%s) is not child of Builder ParamType(%s)."), *InOutParamType->GetName(), *ParamProperty->Struct->GetName());
				continue;
			}

			// 蓝图Builder签名无RTData，先预填Owner/Instigator到OutParam供蓝图使用（要求OutParam以FBXTaskConditionParameter为基类）
			if (!InOutParamType->IsChildOf(FBXTaskConditionParameter::StaticStruct()))
			{
				UE_LOG(BXMGR_Condition, Warning, TEXT("InternalBuildTaskConditionParameter: ParamType(%s) is not child of FBXTaskConditionParameter."), *InOutParamType->GetName());
				continue;
			}

			FBXTaskConditionParameter* BaseParam = static_cast<FBXTaskConditionParameter*>(InOutParamAddress);
			BaseParam->Owner = InRTData.Owner;
			BaseParam->Instigator = InRTData.Instigator;

			uint8* Buffer = static_cast<uint8*>(FMemory::MallocZeroed(FindResult->Function->ParmsSize));
			if (Buffer)
			{
				FMemory::Memcpy(Buffer, &InCondition, ConditionProperty->GetSize());
				FMemory::Memcpy(Buffer + ConditionProperty->GetSize(), InOutParamAddress, ParamProperty->GetSize());

				ProcessEvent(FindResult->Function, Buffer);

				FMemory::Memcpy(InOutParamAddress, Buffer + ConditionProperty->GetSize(), ParamProperty->GetSize());
				FMemory::Free(Buffer);
				return true;
			}
		}
	}

	// 未找到映射配置时，回退到默认Builder填充基础参数（要求OutParam以FBXTaskConditionParameter为基类）
	if (!InOutParamType->IsChildOf(FBXTaskConditionParameter::StaticStruct()))
	{
		UE_LOG(BXMGR_Condition, Warning, TEXT("InternalBuildTaskConditionParameter: ParamType(%s) is not child of FBXTaskConditionParameter, cannot fallback to DefaultBuilder."), *InOutParamType->GetName());
		return false;
	}

	NativeBuildDefaultTaskParam(InCondition, InRTData, InRTSData, InRTTData, reinterpret_cast<int64>(InOutParamAddress));
	return true;
}
#pragma endregion TaskConditionParamBuilder



#pragma region TaskConditionParamBuilderFunctions
IMPLEMENT_NATIVE_TASK_PARAM_BUILDER(UBXConditionManager, NativeBuildDefaultTaskParam)
{
	check(InOutParamAddress != 0);

	FBXTaskConditionParameter* Param = reinterpret_cast<FBXTaskConditionParameter*>(InOutParamAddress);
	Param->Owner = InRTData.Owner;
	Param->Instigator = InRTData.Instigator;
}
#pragma endregion TaskConditionParamBuilderFunctions



#pragma region DerivedCache
DEFINE_FUNCTION(UBXConditionManager::execGetDerivedInt)
{
	// 参数1: UClass*
	Stack.MostRecentProperty = nullptr;
	Stack.StepCompiledIn<FProperty>(nullptr);
	UClass* ConditionClass = nullptr;
	if (FObjectProperty* ObjProperty = CastField<FObjectProperty>(Stack.MostRecentProperty))
	{
		ConditionClass = Cast<UClass>(ObjProperty->GetObjectPtrPropertyValue(Stack.MostRecentPropertyAddress).Get());
	}

	// 参数2: 通配符输入参数
	Stack.MostRecentProperty = nullptr;
	Stack.StepCompiledIn<FProperty>(nullptr);
	uint8* ParamAddress = Stack.MostRecentPropertyAddress;
	UScriptStruct* ParamType = nullptr;
	if (FStructProperty* ParamProperty = CastField<FStructProperty>(Stack.MostRecentProperty))
	{
		ParamType = ParamProperty->Struct;
	}

	// 参数3: int32& OutValue
	Stack.MostRecentProperty = nullptr;
	Stack.StepCompiledIn<FProperty>(nullptr);
	int32* OutValuePtr = reinterpret_cast<int32*>(Stack.MostRecentPropertyAddress);

	P_FINISH;

	bool bResult = false;
	UBXConditionManager* Manager = P_THIS_CAST(UBXConditionManager);
	if (IsValid(Manager) && IsValid(ConditionClass) && ParamType && ParamAddress && OutValuePtr)
	{
		P_NATIVE_BEGIN;
		bResult = Manager->InternalGetDerivedInt(ConditionClass, ParamType, ParamAddress, *OutValuePtr);
		P_NATIVE_END;
	}

	*(bool*)RESULT_PARAM = bResult;
}

DEFINE_FUNCTION(UBXConditionManager::execSetDerivedInt)
{
	// 参数1: UClass*
	Stack.MostRecentProperty = nullptr;
	Stack.StepCompiledIn<FProperty>(nullptr);
	UClass* ConditionClass = nullptr;
	if (FObjectProperty* ObjProperty = CastField<FObjectProperty>(Stack.MostRecentProperty))
	{
		ConditionClass = Cast<UClass>(ObjProperty->GetObjectPtrPropertyValue(Stack.MostRecentPropertyAddress).Get());
	}

	// 参数2: 通配符输入参数
	Stack.MostRecentProperty = nullptr;
	Stack.StepCompiledIn<FProperty>(nullptr);
	uint8* ParamAddress = Stack.MostRecentPropertyAddress;
	UScriptStruct* ParamType = nullptr;
	if (FStructProperty* ParamProperty = CastField<FStructProperty>(Stack.MostRecentProperty))
	{
		ParamType = ParamProperty->Struct;
	}

	// 参数3: int32 InValue
	Stack.MostRecentProperty = nullptr;
	Stack.StepCompiledIn<FProperty>(nullptr);
	int32 InValue = *reinterpret_cast<int32*>(Stack.MostRecentPropertyAddress);

	P_FINISH;

	UBXConditionManager* Manager = P_THIS_CAST(UBXConditionManager);
	if (IsValid(Manager) && IsValid(ConditionClass) && ParamType && ParamAddress)
	{
		P_NATIVE_BEGIN;
		Manager->InternalSetDerivedInt(ConditionClass, ParamType, ParamAddress, InValue);
		P_NATIVE_END;
	}
}

DEFINE_FUNCTION(UBXConditionManager::execGetDerivedFloat)
{
	// 参数1: UClass*
	Stack.MostRecentProperty = nullptr;
	Stack.StepCompiledIn<FProperty>(nullptr);
	UClass* ConditionClass = nullptr;
	if (FObjectProperty* ObjProperty = CastField<FObjectProperty>(Stack.MostRecentProperty))
	{
		ConditionClass = Cast<UClass>(ObjProperty->GetObjectPtrPropertyValue(Stack.MostRecentPropertyAddress).Get());
	}

	// 参数2: 通配符输入参数
	Stack.MostRecentProperty = nullptr;
	Stack.StepCompiledIn<FProperty>(nullptr);
	uint8* ParamAddress = Stack.MostRecentPropertyAddress;
	UScriptStruct* ParamType = nullptr;
	if (FStructProperty* ParamProperty = CastField<FStructProperty>(Stack.MostRecentProperty))
	{
		ParamType = ParamProperty->Struct;
	}

	// 参数3: float& OutValue
	Stack.MostRecentProperty = nullptr;
	Stack.StepCompiledIn<FProperty>(nullptr);
	float* OutValuePtr = reinterpret_cast<float*>(Stack.MostRecentPropertyAddress);

	P_FINISH;

	bool bResult = false;
	UBXConditionManager* Manager = P_THIS_CAST(UBXConditionManager);
	if (IsValid(Manager) && IsValid(ConditionClass) && ParamType && ParamAddress && OutValuePtr)
	{
		P_NATIVE_BEGIN;
		bResult = Manager->InternalGetDerivedFloat(ConditionClass, ParamType, ParamAddress, *OutValuePtr);
		P_NATIVE_END;
	}

	*(bool*)RESULT_PARAM = bResult;
}

DEFINE_FUNCTION(UBXConditionManager::execSetDerivedFloat)
{
	// 参数1: UClass*
	Stack.MostRecentProperty = nullptr;
	Stack.StepCompiledIn<FProperty>(nullptr);
	UClass* ConditionClass = nullptr;
	if (FObjectProperty* ObjProperty = CastField<FObjectProperty>(Stack.MostRecentProperty))
	{
		ConditionClass = Cast<UClass>(ObjProperty->GetObjectPtrPropertyValue(Stack.MostRecentPropertyAddress).Get());
	}

	// 参数2: 通配符输入参数
	Stack.MostRecentProperty = nullptr;
	Stack.StepCompiledIn<FProperty>(nullptr);
	uint8* ParamAddress = Stack.MostRecentPropertyAddress;
	UScriptStruct* ParamType = nullptr;
	if (FStructProperty* ParamProperty = CastField<FStructProperty>(Stack.MostRecentProperty))
	{
		ParamType = ParamProperty->Struct;
	}

	// 参数3: float InValue
	Stack.MostRecentProperty = nullptr;
	Stack.StepCompiledIn<FProperty>(nullptr);
	float InValue = *reinterpret_cast<float*>(Stack.MostRecentPropertyAddress);

	P_FINISH;

	UBXConditionManager* Manager = P_THIS_CAST(UBXConditionManager);
	if (IsValid(Manager) && IsValid(ConditionClass) && ParamType && ParamAddress)
	{
		P_NATIVE_BEGIN;
		Manager->InternalSetDerivedFloat(ConditionClass, ParamType, ParamAddress, InValue);
		P_NATIVE_END;
	}
}

DEFINE_FUNCTION(UBXConditionManager::execGetDerivedStruct)
{
	// 参数1: UClass*
	Stack.MostRecentProperty = nullptr;
	Stack.StepCompiledIn<FProperty>(nullptr);
	UClass* ConditionClass = nullptr;
	if (FObjectProperty* ObjProperty = CastField<FObjectProperty>(Stack.MostRecentProperty))
	{
		ConditionClass = Cast<UClass>(ObjProperty->GetObjectPtrPropertyValue(Stack.MostRecentPropertyAddress).Get());
	}

	// 参数2: 通配符输入参数
	Stack.MostRecentProperty = nullptr;
	Stack.StepCompiledIn<FProperty>(nullptr);
	uint8* ParamAddress = Stack.MostRecentPropertyAddress;
	UScriptStruct* ParamType = nullptr;
	if (FStructProperty* ParamProperty = CastField<FStructProperty>(Stack.MostRecentProperty))
	{
		ParamType = ParamProperty->Struct;
	}

	// 参数3: 通配符输出参数 OutResult
	Stack.MostRecentProperty = nullptr;
	Stack.StepCompiledIn<FProperty>(nullptr);
	uint8* OutResultAddress = Stack.MostRecentPropertyAddress;
	UScriptStruct* OutResultType = nullptr;
	if (FStructProperty* OutResultProperty = CastField<FStructProperty>(Stack.MostRecentProperty))
	{
		OutResultType = OutResultProperty->Struct;
	}

	P_FINISH;

	bool bResult = false;
	UBXConditionManager* Manager = P_THIS_CAST(UBXConditionManager);
	if (IsValid(Manager) && IsValid(ConditionClass) && ParamType && ParamAddress && OutResultType && OutResultAddress)
	{
		P_NATIVE_BEGIN;
		bResult = Manager->InternalGetDerivedStruct(ConditionClass, ParamType, ParamAddress, OutResultType, OutResultAddress);
		P_NATIVE_END;
	}

	*(bool*)RESULT_PARAM = bResult;
}

DEFINE_FUNCTION(UBXConditionManager::execSetDerivedStruct)
{
	// 参数1: UClass*
	Stack.MostRecentProperty = nullptr;
	Stack.StepCompiledIn<FProperty>(nullptr);
	UClass* ConditionClass = nullptr;
	if (FObjectProperty* ObjProperty = CastField<FObjectProperty>(Stack.MostRecentProperty))
	{
		ConditionClass = Cast<UClass>(ObjProperty->GetObjectPtrPropertyValue(Stack.MostRecentPropertyAddress).Get());
	}

	// 参数2: 通配符输入参数 InParameter
	Stack.MostRecentProperty = nullptr;
	Stack.StepCompiledIn<FProperty>(nullptr);
	uint8* ParamAddress = Stack.MostRecentPropertyAddress;
	UScriptStruct* ParamType = nullptr;
	if (FStructProperty* ParamProperty = CastField<FStructProperty>(Stack.MostRecentProperty))
	{
		ParamType = ParamProperty->Struct;
	}

	// 参数3: 通配符输入参数 InResult
	Stack.MostRecentProperty = nullptr;
	Stack.StepCompiledIn<FProperty>(nullptr);
	uint8* InResultAddress = Stack.MostRecentPropertyAddress;
	UScriptStruct* InResultType = nullptr;
	if (FStructProperty* InResultProperty = CastField<FStructProperty>(Stack.MostRecentProperty))
	{
		InResultType = InResultProperty->Struct;
	}

	P_FINISH;

	UBXConditionManager* Manager = P_THIS_CAST(UBXConditionManager);
	if (IsValid(Manager) && IsValid(ConditionClass) && ParamType && ParamAddress && InResultType && InResultAddress)
	{
		P_NATIVE_BEGIN;
		Manager->InternalSetDerivedStruct(ConditionClass, ParamType, ParamAddress, InResultType, InResultAddress);
		P_NATIVE_END;
	}
}

FBXDerivedKey UBXConditionManager::InternalMakeDerivedKey(UClass* InConditionClass, UScriptStruct* InParamType, const void* InParamAddress) const
{
	FBXDerivedKey Key;
	Key.ConditionClass = InConditionClass;
	Key.ParamType = InParamType;
	Key.ParamHash = IsValid(InParamType) ? InParamType->GetStructTypeHash(InParamAddress) : 0;
	return Key;
}

bool UBXConditionManager::InternalGetDerivedInt(UClass* InConditionClass, UScriptStruct* InParamType, const void* InParamAddress, int32& OutValue) const
{
	if (!IsValid(InConditionClass) || !IsValid(InParamType) || !InParamAddress)
	{
		return false;
	}

	const FBXDerivedKey Key = InternalMakeDerivedKey(InConditionClass, InParamType, InParamAddress);
	if (const TBXDerivedEntry<int32>* Entry = DerivedCache_Int.Find(Key))
	{
		if (Entry->CachedTime == CurrentFrameTime)
		{
			OutValue = Entry->Value;
			return true;
		}
	}
	return false;
}

void UBXConditionManager::InternalSetDerivedInt(UClass* InConditionClass, UScriptStruct* InParamType, const void* InParamAddress, int32 InValue)
{
	if (!IsValid(InConditionClass) || !IsValid(InParamType) || !InParamAddress)
	{
		return;
	}

	const FBXDerivedKey Key = InternalMakeDerivedKey(InConditionClass, InParamType, InParamAddress);
	TBXDerivedEntry<int32>& Entry = DerivedCache_Int.FindOrAdd(Key);
	Entry.Value = InValue;
	Entry.CachedTime = CurrentFrameTime;

	if (DerivedCache_Int.Num() > MaxDerivedEntries)
	{
		DerivedCache_Int.Empty();
	}
}

bool UBXConditionManager::InternalGetDerivedFloat(UClass* InConditionClass, UScriptStruct* InParamType, const void* InParamAddress, float& OutValue) const
{
	if (!IsValid(InConditionClass) || !IsValid(InParamType) || !InParamAddress)
	{
		return false;
	}

	const FBXDerivedKey Key = InternalMakeDerivedKey(InConditionClass, InParamType, InParamAddress);
	if (const TBXDerivedEntry<float>* Entry = DerivedCache_Float.Find(Key))
	{
		if (Entry->CachedTime == CurrentFrameTime)
		{
			OutValue = Entry->Value;
			return true;
		}
	}
	return false;
}

void UBXConditionManager::InternalSetDerivedFloat(UClass* InConditionClass, UScriptStruct* InParamType, const void* InParamAddress, float InValue)
{
	if (!IsValid(InConditionClass) || !IsValid(InParamType) || !InParamAddress)
	{
		return;
	}

	const FBXDerivedKey Key = InternalMakeDerivedKey(InConditionClass, InParamType, InParamAddress);
	TBXDerivedEntry<float>& Entry = DerivedCache_Float.FindOrAdd(Key);
	Entry.Value = InValue;
	Entry.CachedTime = CurrentFrameTime;

	if (DerivedCache_Float.Num() > MaxDerivedEntries)
	{
		DerivedCache_Float.Empty();
	}
}

bool UBXConditionManager::InternalGetDerivedStruct(UClass* InConditionClass, UScriptStruct* InParamType, const void* InParamAddress, UScriptStruct* InResultType, void* InOutResultAddress) const
{
	if (!IsValid(InConditionClass) || !IsValid(InParamType) || !InParamAddress || !IsValid(InResultType) || !InOutResultAddress)
	{
		return false;
	}

	const FBXDerivedKey Key = InternalMakeDerivedKey(InConditionClass, InParamType, InParamAddress);
	if (const TBXDerivedEntry<FInstancedStruct>* Entry = DerivedCache_Struct.Find(Key))
	{
		if (Entry->CachedTime == CurrentFrameTime && Entry->Value.GetScriptStruct() == InResultType)
		{
			InResultType->CopyScriptStruct(InOutResultAddress, Entry->Value.GetMemory());
			return true;
		}
	}
	return false;
}

void UBXConditionManager::InternalSetDerivedStruct(UClass* InConditionClass, UScriptStruct* InParamType, const void* InParamAddress, UScriptStruct* InResultType, const void* InResultAddress)
{
	if (!IsValid(InConditionClass) || !IsValid(InParamType) || !InParamAddress || !IsValid(InResultType) || !InResultAddress)
	{
		return;
	}

	const FBXDerivedKey Key = InternalMakeDerivedKey(InConditionClass, InParamType, InParamAddress);
	TBXDerivedEntry<FInstancedStruct>& Entry = DerivedCache_Struct.FindOrAdd(Key);
	Entry.Value.InitializeAs(InResultType, static_cast<const uint8*>(InResultAddress));
	Entry.CachedTime = CurrentFrameTime;

	if (DerivedCache_Struct.Num() > MaxDerivedEntries)
	{
		DerivedCache_Struct.Empty();
	}
}
#pragma endregion DerivedCache
