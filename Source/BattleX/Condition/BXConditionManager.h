#pragma once

#include "CoreMinimal.h"
#include "InstancedStruct.h"
#include "Engine/EngineBaseTypes.h"

#include "BXManager.h"
#include "BXCondition.h"
#include "BXConditionEnums.h"

#include "BXConditionManager.generated.h"



DECLARE_LOG_CATEGORY_EXTERN(BXMGR_Condition, Log, All);



// 派生结果缓存键（按条件类+参数标识同一类运算的输入）
struct FBXDerivedKey
{
	UClass* ConditionClass = nullptr;
	UScriptStruct* ParamType = nullptr;
	uint32 ParamHash = 0;

	bool operator==(const FBXDerivedKey& Other) const;

	friend uint32 GetTypeHash(const FBXDerivedKey& Key);
};



// 派生结果缓存项模板（存储运算中间结果与帧时间戳）
template<typename T>
struct TBXDerivedEntry
{
	T Value{};
	double CachedTime = 0.0;
};



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



#pragma region FrameTime
public:
	// World Tick 开始时的回调，用于刷新当前帧时间戳
	void OnWorldTickStart(UWorld* InWorld, ELevelTick InTickType, float InDeltaTime);

protected:
	// 当前帧时间戳（每帧 World Tick 开始时更新一次，帧内复用）
	UPROPERTY(Transient)
	double CurrentFrameTime = 0.0;
#pragma endregion FrameTime



#pragma region Condition
public:
	template<typename T>
	bool CheckCondition(UBXCondition* InCondition, const T& InParameter)
	{
		return CheckCondition(InCondition, T::StaticStruct(), &InParameter);
	}

	UFUNCTION(BlueprintCallable, CustomThunk, meta = (CustomStructureParam = "InParameter"))
	bool CheckCondition(UBXCondition* InCondition, int32 InParameter);

	DECLARE_FUNCTION(execCheckCondition);

	bool CheckCondition(UBXCondition* InCondition, UScriptStruct* InParameterType, void* InParameterAddress);

	// Native条件检查函数签名（统一类型擦除签名，内部自行转型）
	using FBXNativeCheckFunc = bool(UBXConditionManager::*)(UBXCondition* InCondition, UScriptStruct* InParameterType, void* InParameterAddress);

	// 注册Native条件检查函数（注册后走C++直接调用，绕过ProcessEvent）
	void RegisterNativeCheck(TSubclassOf<UBXCondition> InClass, FBXNativeCheckFunc InFunc);

	// 派生结果缓存：int32版本（查缓存，未命中返回nullptr）
	template<typename TParam>
	const int32* GetDerivedInt(UClass* InConditionClass, const TParam& InParam);

	// 派生结果缓存：float版本（查缓存，未命中返回nullptr）
	template<typename TParam>
	const float* GetDerivedFloat(UClass* InConditionClass, const TParam& InParam);

	// 派生结果缓存：FInstancedStruct版本（查缓存，未命中返回nullptr）
	template<typename TResult, typename TParam>
	const TResult* GetDerivedStruct(UClass* InConditionClass, const TParam& InParam);

	// 派生结果缓存：写入int32结果，返回缓存内地址
	template<typename TParam>
	const int32* SetDerivedInt(UClass* InConditionClass, const TParam& InParam, int32 InValue);

	// 派生结果缓存：写入float结果，返回缓存内地址
	template<typename TParam>
	const float* SetDerivedFloat(UClass* InConditionClass, const TParam& InParam, float InValue);

	// 派生结果缓存：写入FInstancedStruct结果，返回缓存内地址
	template<typename TResult, typename TParam>
	const TResult* SetDerivedStruct(UClass* InConditionClass, const TParam& InParam, const TResult& InValue);

protected:
	// 条件到函数名的配置表
	UPROPERTY(EditDefaultsOnly)
	TMap<TSubclassOf<UBXCondition>, FName> ConditionToFunctionConfig;

	// 条件到函数的映射表
	UPROPERTY(Transient)
	TMap<TSubclassOf<UBXCondition>, FBXConditionFunctionParameter> ConditionToFunctionMap;

	// Native条件检查函数映射表（直接C++调用，绕过ProcessEvent）
	TMap<TSubclassOf<UBXCondition>, FBXNativeCheckFunc> NativeCheckMap;

	// 派生结果缓存表（同帧同类同参数的运算中间结果复用）
	TMap<FBXDerivedKey, TBXDerivedEntry<int32>> DerivedCache_Int;
	TMap<FBXDerivedKey, TBXDerivedEntry<float>> DerivedCache_Float;
	TMap<FBXDerivedKey, TBXDerivedEntry<FInstancedStruct>> DerivedCache_Struct;

	// 缓存条目上限，超过则整体清空
	static constexpr int32 MaxDerivedEntries = 1024;

	// 构造派生缓存键
	template<typename TParam>
	FBXDerivedKey MakeDerivedKey(UClass* InConditionClass, const TParam& InParam) const;
#pragma endregion Condition



#pragma region ConditionFunctions
public:
	// Task组合条件Native检查（AND/OR短路求值，空Children返回True）
	bool NativeCheckTaskComposite(UBXCondition* InCondition, UScriptStruct* InParameterType, void* InParameterAddress);

	// 决策树组合条件Native检查（AND/OR短路求值，空Children返回True）
	bool NativeCheckDecisionTreeComposite(UBXCondition* InCondition, UScriptStruct* InParameterType, void* InParameterAddress);
#pragma endregion ConditionFunctions

};






template<typename TParam>
FBXDerivedKey UBXConditionManager::MakeDerivedKey(UClass* InConditionClass, const TParam& InParam) const
{
	FBXDerivedKey Key;
	Key.ConditionClass = InConditionClass;
	Key.ParamType = TParam::StaticStruct();
	Key.ParamHash = TParam::StaticStruct()->GetStructTypeHash(&InParam);
	return Key;
}

template<typename TParam>
const int32* UBXConditionManager::GetDerivedInt(UClass* InConditionClass, const TParam& InParam)
{
	const FBXDerivedKey Key = MakeDerivedKey(InConditionClass, InParam);
	if (const TBXDerivedEntry<int32>* Entry = DerivedCache_Int.Find(Key))
	{
		if (Entry->CachedTime == CurrentFrameTime)
		{
			return &Entry->Value;
		}
	}
	return nullptr;
}

template<typename TParam>
const float* UBXConditionManager::GetDerivedFloat(UClass* InConditionClass, const TParam& InParam)
{
	const FBXDerivedKey Key = MakeDerivedKey(InConditionClass, InParam);
	if (const TBXDerivedEntry<float>* Entry = DerivedCache_Float.Find(Key))
	{
		if (Entry->CachedTime == CurrentFrameTime)
		{
			return &Entry->Value;
		}
	}
	return nullptr;
}

template<typename TResult, typename TParam>
const TResult* UBXConditionManager::GetDerivedStruct(UClass* InConditionClass, const TParam& InParam)
{
	const FBXDerivedKey Key = MakeDerivedKey(InConditionClass, InParam);
	if (const TBXDerivedEntry<FInstancedStruct>* Entry = DerivedCache_Struct.Find(Key))
	{
		if (Entry->CachedTime == CurrentFrameTime)
		{
			return Entry->Value.GetPtr<TResult>();
		}
	}
	return nullptr;
}

template<typename TParam>
const int32* UBXConditionManager::SetDerivedInt(UClass* InConditionClass, const TParam& InParam, int32 InValue)
{
	const FBXDerivedKey Key = MakeDerivedKey(InConditionClass, InParam);
	TBXDerivedEntry<int32>& Entry = DerivedCache_Int.FindOrAdd(Key);
	Entry.Value = InValue;
	Entry.CachedTime = CurrentFrameTime;

	if (DerivedCache_Int.Num() > MaxDerivedEntries)
	{
		DerivedCache_Int.Empty();
	}

	return &Entry.Value;
}

template<typename TParam>
const float* UBXConditionManager::SetDerivedFloat(UClass* InConditionClass, const TParam& InParam, float InValue)
{
	const FBXDerivedKey Key = MakeDerivedKey(InConditionClass, InParam);
	TBXDerivedEntry<float>& Entry = DerivedCache_Float.FindOrAdd(Key);
	Entry.Value = InValue;
	Entry.CachedTime = CurrentFrameTime;

	if (DerivedCache_Float.Num() > MaxDerivedEntries)
	{
		DerivedCache_Float.Empty();
	}

	return &Entry.Value;
}

template<typename TResult, typename TParam>
const TResult* UBXConditionManager::SetDerivedStruct(UClass* InConditionClass, const TParam& InParam, const TResult& InValue)
{
	const FBXDerivedKey Key = MakeDerivedKey(InConditionClass, InParam);
	TBXDerivedEntry<FInstancedStruct>& Entry = DerivedCache_Struct.FindOrAdd(Key);
	Entry.Value = Make<TResult>(InValue);
	Entry.CachedTime = CurrentFrameTime;

	if (DerivedCache_Struct.Num() > MaxDerivedEntries)
	{
		DerivedCache_Struct.Empty();
	}

	return Entry.Value.GetPtr<TResult>();
}
