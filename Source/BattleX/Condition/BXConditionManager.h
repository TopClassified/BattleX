#pragma once

#include "CoreMinimal.h"
#include "InstancedStruct.h"
#include "Engine/EngineBaseTypes.h"

#include "BXManager.h"
#include "BXCondition.h"
#include "BXConditionEnums.h"
#include "BXStructs.h"
#include "Task/BXTaskCondition.h"
#include "Timeline/BXTLStructs.h"

#include "BXConditionManager.generated.h"



DECLARE_LOG_CATEGORY_EXTERN(BXMGR_Condition, Log, All);



// 派生结果缓存键（按条件类+参数标识同一类运算的输入）
struct FBXDerivedKey
{
	// 条件类
	UClass* ConditionClass = nullptr;

	// 参数类型
	UScriptStruct* ParamType = nullptr;

	// 参数哈希
	uint32 ParamHash = 0;

	// 相等比较
	bool operator==(const FBXDerivedKey& Other) const;

	// 类型哈希函数
	friend uint32 GetTypeHash(const FBXDerivedKey& Key);
};



// 派生结果缓存项模板（存储运算中间结果与帧时间戳）
template<typename T>
struct TBXDerivedEntry
{
	// 缓存值
	T Value{};

	// 缓存帧时间戳
	double CachedTime = 0.0;
};



// 条件执行函数参数信息
USTRUCT(BlueprintType)
struct FBXConditionFunctionParameter
{
	GENERATED_BODY()

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
	// 获取条件管理器实例
	UFUNCTION(BlueprintCallable)
	static UBXConditionManager* Get(UObject* InWorldContext);

	// 初始化管理器
	virtual void Initialize();

	// 反初始化管理器
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
	// 条件检查（C++模板版本，类型安全）
	template<typename T>
	bool CheckCondition(UBXCondition* InCondition, const T& InParameter)
	{
		return CheckCondition(InCondition, T::StaticStruct(), &InParameter);
	}

	// 条件检查（蓝图泛型版本，InParameter为通配符输入引脚）
	UFUNCTION(BlueprintCallable, CustomThunk, meta = (CustomStructureParam = "InParameter"))
	bool CheckCondition(UBXCondition* InCondition, int32 InParameter);
	DECLARE_FUNCTION(execCheckCondition);

	// 条件检查（运行时版本，由模板版本与蓝图thunk转发）
	bool CheckCondition(UBXCondition* InCondition, UScriptStruct* InParameterType, const void* InParameterAddress);

	// Native条件检查函数签名（int64承载指针地址，绕过UHT对void*的限制）
	using FBXNativeCheckFunc = bool(UBXConditionManager::*)(UBXCondition* InCondition, int64 InParameterTypeAddress, int64 InParameterAddress);

	// 注册Native条件检查函数（注册后走C++直接调用，绕过ProcessEvent）
	void RegisterNativeCheck(TSubclassOf<UBXCondition> InClass, FBXNativeCheckFunc InFunc);

protected:
	// 条件到函数名的配置表
	UPROPERTY(EditDefaultsOnly)
	TMap<TSubclassOf<UBXCondition>, FBXFunctionSelector> ConditionToFunctionConfig;

	// 条件到函数的映射表
	UPROPERTY(Transient)
	TMap<TSubclassOf<UBXCondition>, FBXConditionFunctionParameter> ConditionToFunctionMap;

	// Native条件检查函数映射表（直接C++调用，绕过ProcessEvent）
	TMap<TSubclassOf<UBXCondition>, FBXNativeCheckFunc> NativeCheckMap;
#pragma endregion Condition



#pragma region ConditionFunctions
public:
	// Task组合条件Native检查（AND/OR短路求值，空Children返回True）
	UFUNCTION()
	bool NativeCheckTaskComposite(UBXCondition* InCondition, int64 InParameterTypeAddress, int64 InParameterAddress);

	// 决策树组合条件Native检查（AND/OR短路求值，空Children返回True）
	UFUNCTION()
	bool NativeCheckDecisionTreeComposite(UBXCondition* InCondition, int64 InParameterTypeAddress, int64 InParameterAddress);
#pragma endregion ConditionFunctions



#pragma region TaskConditionParamBuilder
public:
	// Native参数构造函数签名（RTData透传const引用，OutParam用int64承载地址，写入调用方提供的内存）
	using FBXNativeTaskParamBuilderFunc = void(UBXConditionManager::*)(UBXTaskCondition* InCondition, const FBXTLRunTimeData& InRTData, const FBXTLSectionRTData& InRTSData, const FBXTLTaskRTData& InRTTData, int64 InOutParamAddress);

	// 注册Native参数构造函数（注册后走C++直接调用，绕过ProcessEvent）
	void RegisterNativeTaskParamBuilder(TSubclassOf<UBXTaskCondition> InClass, FBXNativeTaskParamBuilderFunc InFunc);

	// 构建Task条件参数（C++模板版本，类型安全，命中返回参数值，否则返回空）
	template<typename TParam>
	TOptional<TParam> BuildTaskConditionParameter(UBXTaskCondition* InCondition, const FBXTLRunTimeData& InRTData, const FBXTLSectionRTData& InRTSData, const FBXTLTaskRTData& InRTTData)
	{
		TParam Local;
		if (InternalBuildTaskConditionParameter(InCondition, InRTData, InRTSData, InRTTData, TParam::StaticStruct(), &Local))
		{
			return Local;
		}
		return TOptional<TParam>{};
	}

	// 构建Task条件参数（蓝图泛型版本，OutParameter为通配符输出引脚，可传入任意条件参数结构体）
	UFUNCTION(BlueprintCallable, CustomThunk, meta = (CustomStructureParam = "OutParameter"))
	bool BuildTaskConditionParameter(UBXTaskCondition* InCondition, const FBXTLRunTimeData& InRTData, const FBXTLSectionRTData& InRTSData, const FBXTLTaskRTData& InRTTData, UPARAM(ref) int32 OutParameter);

	DECLARE_FUNCTION(execBuildTaskConditionParameter);

private:
	// 构建Task条件参数（实现版本，InOutParamType为调用方传入的结构体类型，用于蓝图Builder签名校验，OutParam为调用方提供的内存地址，由模板版本与蓝图thunk转发）
	bool InternalBuildTaskConditionParameter(UBXTaskCondition* InCondition, const FBXTLRunTimeData& InRTData, const FBXTLSectionRTData& InRTSData, const FBXTLTaskRTData& InRTTData, UScriptStruct* InOutParamType, void* InOutParamAddress);

protected:
	// 条件参数构造函数配置表（TaskConditionClass → Builder函数名）
	UPROPERTY(EditDefaultsOnly)
	TMap<TSubclassOf<UBXTaskCondition>, FBXFunctionSelector> TaskConditionParamBuilderConfig;

	// 条件参数构造函数运行时映射（蓝图Builder，TaskConditionClass → UFunction+参数名）
	UPROPERTY(Transient)
	TMap<TSubclassOf<UBXTaskCondition>, FBXConditionFunctionParameter> TaskConditionParamBuilderMap;

	// Native参数构造函数映射表（TaskConditionClass → Native函数指针）
	TMap<TSubclassOf<UBXTaskCondition>, FBXNativeTaskParamBuilderFunc> NativeTaskParamBuilderMap;
#pragma endregion TaskConditionParamBuilder



#pragma region TaskConditionParamBuilderFunctions
public:
	// 默认Task条件参数构造（填充Owner/Instigator）
	UFUNCTION()
	void NativeBuildDefaultTaskParam(UBXTaskCondition* InCondition, const FBXTLRunTimeData& InRTData, const FBXTLSectionRTData& InRTSData, const FBXTLTaskRTData& InRTTData, int64 InOutParamAddress);
#pragma endregion TaskConditionParamBuilderFunctions



#pragma region DerivedCache
public:
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

	// Blueprint可调用版本：int32 Get（未命中返回false，命中写入OutValue并返回true）
	UFUNCTION(BlueprintCallable, CustomThunk, meta = (CustomStructureParam = "InParameter"))
	bool GetDerivedInt(UClass* InConditionClass, int32 InParameter, int32& OutValue);
	DECLARE_FUNCTION(execGetDerivedInt);

	// Blueprint可调用版本：int32 Set
	UFUNCTION(BlueprintCallable, CustomThunk, meta = (CustomStructureParam = "InParameter"))
	void SetDerivedInt(UClass* InConditionClass, int32 InParameter, int32 InValue);
	DECLARE_FUNCTION(execSetDerivedInt);

	// Blueprint可调用版本：float Get
	UFUNCTION(BlueprintCallable, CustomThunk, meta = (CustomStructureParam = "InParameter"))
	bool GetDerivedFloat(UClass* InConditionClass, int32 InParameter, float& OutValue);
	DECLARE_FUNCTION(execGetDerivedFloat);

	// Blueprint可调用版本：float Set
	UFUNCTION(BlueprintCallable, CustomThunk, meta = (CustomStructureParam = "InParameter"))
	void SetDerivedFloat(UClass* InConditionClass, int32 InParameter, float InValue);
	DECLARE_FUNCTION(execSetDerivedFloat);

	// Blueprint可调用版本：Struct Get（OutResult为通配符输出引脚，类型需与缓存写入类型一致）
	UFUNCTION(BlueprintCallable, CustomThunk, meta = (CustomStructureParam = "InParameter,OutResult"))
	bool GetDerivedStruct(UClass* InConditionClass, int32 InParameter, int32 OutResult);
	DECLARE_FUNCTION(execGetDerivedStruct);

	// Blueprint可调用版本：Struct Set（InResult为通配符输入引脚）
	UFUNCTION(BlueprintCallable, CustomThunk, meta = (CustomStructureParam = "InParameter,InResult"))
	void SetDerivedStruct(UClass* InConditionClass, int32 InParameter, int32 InResult);
	DECLARE_FUNCTION(execSetDerivedStruct);

private:
	// 构造派生缓存键（模板版本，委托到InternalMakeDerivedKey）
	template<typename TParam>
	FBXDerivedKey MakeDerivedKey(UClass* InConditionClass, const TParam& InParam) const;

	// 派生缓存键构造（运行时版本，支持任意UScriptStruct）
	FBXDerivedKey InternalMakeDerivedKey(UClass* InConditionClass, UScriptStruct* InParamType, const void* InParamAddress) const;

	// 派生缓存运行时实现：int32 Get（由Blueprint thunk转发）
	bool InternalGetDerivedInt(UClass* InConditionClass, UScriptStruct* InParamType, const void* InParamAddress, int32& OutValue) const;

	// 派生缓存运行时实现：int32 Set（由Blueprint thunk转发）
	void InternalSetDerivedInt(UClass* InConditionClass, UScriptStruct* InParamType, const void* InParamAddress, int32 InValue);

	// 派生缓存运行时实现：float Get（由Blueprint thunk转发）
	bool InternalGetDerivedFloat(UClass* InConditionClass, UScriptStruct* InParamType, const void* InParamAddress, float& OutValue) const;

	// 派生缓存运行时实现：float Set（由Blueprint thunk转发）
	void InternalSetDerivedFloat(UClass* InConditionClass, UScriptStruct* InParamType, const void* InParamAddress, float InValue);

	// 派生缓存运行时实现：Struct Get（由Blueprint thunk转发）
	bool InternalGetDerivedStruct(UClass* InConditionClass, UScriptStruct* InParamType, const void* InParamAddress, UScriptStruct* InResultType, void* InOutResultAddress) const;

	// 派生缓存运行时实现：Struct Set（由Blueprint thunk转发）
	void InternalSetDerivedStruct(UClass* InConditionClass, UScriptStruct* InParamType, const void* InParamAddress, UScriptStruct* InResultType, const void* InResultAddress);

protected:
	// int32派生结果缓存表
	TMap<FBXDerivedKey, TBXDerivedEntry<int32>> DerivedCache_Int;

	// float派生结果缓存表
	TMap<FBXDerivedKey, TBXDerivedEntry<float>> DerivedCache_Float;

	// Struct派生结果缓存表
	TMap<FBXDerivedKey, TBXDerivedEntry<FInstancedStruct>> DerivedCache_Struct;

	// 缓存条目上限，超过则整体清空
	static constexpr int32 MaxDerivedEntries = 1024;
#pragma endregion DerivedCache

};






template<typename TParam>
FBXDerivedKey UBXConditionManager::MakeDerivedKey(UClass* InConditionClass, const TParam& InParam) const
{
	return InternalMakeDerivedKey(InConditionClass, TParam::StaticStruct(), &InParam);
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
