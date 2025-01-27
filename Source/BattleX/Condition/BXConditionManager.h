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
	DECLARE_FUNCTION(execCheckCondition);

	bool CheckCondition(UBXCondition* InCondition, UScriptStruct* InParameterType, void* InParameterAddress);

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
