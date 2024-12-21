#pragma once

#include "CoreMinimal.h"
#include "InstancedStruct.h"

#include "BXManager.h"
#include "BXCondition.h"

#include "BXConditionManager.generated.h"



DECLARE_LOG_CATEGORY_EXTERN(BXMGR_Condition, Log, All);



// 条件执行函数参数信息
USTRUCT(BlueprintType)
struct FBXConditionFunctionParameter
{
	GENERATED_USTRUCT_BODY()

public:
	// 执行函数反射对象
	UPROPERTY(Transient)
	UFunction* Function = nullptr;

	// 参数名称列表
	UPROPERTY(Transient)
	TArray<FName> ParameterNames;
};



UCLASS(Blueprintable)
class BATTLEX_API UBXConditionManager : public UBXManager
{
	GENERATED_BODY()

#pragma region Important
public:
	UFUNCTION(BlueprintCallable)
	static UBXConditionManager* Get(UObject* InWorldContext);
	
	virtual void Initialize();

	virtual void Deinitialize();
	
#pragma endregion Important



#pragma region Condition
public:
	template<typename T>
	bool CheckCondition(UBXCondition* InCondition, const T& InParameter)
	{
		return InternalCheckCondition(InCondition, T::StaticStruct(), &InParameter);
	}

	UFUNCTION(BlueprintCallable, CustomThunk, meta = (CustomStructureParam = "InParameter"))
	bool CheckCondition(UBXCondition* InCondition, int32 InParameter);
	DECLARE_FUNCTION(execCheckCondition)
	{
		Stack.MostRecentProperty = nullptr;

		// 更新蓝图虚拟机栈顶指针
		Stack.StepCompiledIn<FProperty>(nullptr);
		// 获取第一个数据的参数的地址
		UBXCondition* ConditionPointer = reinterpret_cast<UBXCondition*>(Stack.MostRecentPropertyAddress);
		
		// 更新蓝图虚拟机栈顶指针
		Stack.StepCompiledIn<FProperty>(nullptr);
		// 获取第二个无类型参数的内存地址
		uint8* ParameterPointer = Stack.MostRecentPropertyAddress;
		// 获取第二个参数的反射信息
		FStructProperty* ParameterProperty = CastField<FStructProperty>(Stack.MostRecentProperty);

		// 停止对蓝图栈的使用
		P_FINISH;

		if (!IsValid(ConditionPointer) || !ConditionPointer->IsValidLowLevelFast() || !ParameterPointer || !ParameterProperty)
		{
			return;
		}

		UBXConditionManager* Manager = P_THIS_CAST(UBXConditionManager);
		if (!IsValid(Manager) || !Manager->IsValidLowLevel())
		{
			return;
		}

		bool CheckResult = false;
		
		P_NATIVE_BEGIN;
		CheckResult = Manager->InternalCheckCondition(ConditionPointer, ParameterProperty->Struct, ParameterPointer);
		P_NATIVE_END;

		*(bool*)RESULT_PARAM = CheckResult;
	}

protected:
	bool InternalCheckCondition(UBXCondition* InCondition, UScriptStruct* InParameterType, void* InParameterAddress);

protected:
	// 条件到函数名的配置表
	UPROPERTY(EditDefaultsOnly)
	TMap<TSubclassOf<UBXCondition>, FName> ConditionToFunctionConfig;

	// 条件到函数的映射表
	UPROPERTY(Transient)
	TMap<TSubclassOf<UBXCondition>, FBXConditionFunctionParameter> ConditionToFunctionMap;
	
#pragma endregion Condition



#pragma region ConditionFunctions
public:
	
	
#pragma endregion ConditionFunctions
	
};
