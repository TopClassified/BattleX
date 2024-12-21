#pragma once

#include "CoreMinimal.h"
#include "Tickable.h"
#include "GameplayTagContainer.h"

#include "BXManager.h"

#include "BXEventManager.generated.h"



DECLARE_LOG_CATEGORY_EXTERN(BXMGR_Event, Log, All);



USTRUCT(BlueprintType)
struct FBXECallback
{
	GENERATED_USTRUCT_BODY()
	
public:
	UPROPERTY(Transient)
	TArray<UFunction*> Functions;
};

USTRUCT(BlueprintType)
struct FBXECallbackMap
{
	GENERATED_USTRUCT_BODY()
	
public:
	UPROPERTY(Transient)
	TMap<UObject*, FBXECallback> Map;
};

USTRUCT(BlueprintType)
struct FBXESingleKey
{
	GENERATED_USTRUCT_BODY()

public:
	FBXESingleKey() {}
	FBXESingleKey(const FGameplayTag& InEventName, UObject* InInitiator) : EventName(InEventName), Initiator(InInitiator) {}
	
public:
	UPROPERTY(Transient)
	FGameplayTag EventName;
	
	UPROPERTY(Transient)
	const UObject* Initiator = nullptr;

public:
	friend bool operator==(const FBXESingleKey& Val1, const FBXESingleKey& Val2)
	{
		return Val1.EventName == Val2.EventName && Val1.Initiator == Val2.Initiator;
	}

	friend uint32 GetTypeHash(const FBXESingleKey& Val)
	{
		return HashCombine(GetTypeHash(Val.EventName), GetTypeHash(Val.Initiator));
	}
};



class FBXEventManagerODL : public FUObjectArray::FUObjectDeleteListener
{
public:
	void Register(class UBXEventManager* InManager) { Manager = InManager; }

	virtual void NotifyUObjectDeleted(const UObjectBase* InObject, int32 Index) override;

	virtual void OnUObjectArrayShutdown() override {}

private:
	TWeakObjectPtr<class UBXEventManager> Manager = nullptr;
};



UCLASS(Blueprintable)
class BATTLEX_API UBXEventManager : public UBXManager
{
	GENERATED_BODY()

#pragma region Important
public:
	UFUNCTION(BlueprintCallable)
	static UBXEventManager* Get(UObject* InWorldContext);
	
	virtual void Initialize();

	virtual void Deinitialize();

	// 某个对象被回收的事件回调
	void NotifyUObjectDeleted(const UObjectBase* InObject);
	
protected:
	// Object监听器
	FBXEventManagerODL ObjectListener;

	// 记录对象的关联信息
	TMap<uint32, TArray<FGameplayTag>> GlobalTargetMap;
	TMap<uint32, TArray<FGameplayTag>> SingleKeyMap;
	TMap<uint32, TArray<FGameplayTag>> SingleTargetMap;
	
#pragma endregion Important



#pragma region Event
public:
	// 注册全局事件
	UFUNCTION(BlueprintCallable)
	bool RegisterGlobalEvent(const FGameplayTag& InEventName, UObject* InTarget, FName InFunctionName);

	// 注销全局事件(函数名为None，则注销所有回调)
	UFUNCTION(BlueprintCallable)
	bool UnregisterGlobalEvent(const FGameplayTag& InEventName, UObject* InTarget, FName InFunctionName = NAME_None);

	// 广播全局事件
	template<typename T>
	void BroadcastGlobalEvent(const FGameplayTag& InEventName, const T& InParameter)
	{
		FBXECallbackMap* FindResult = GlobalEventCallbacks.Find(InEventName);
		if (!FindResult)
		{
			return;
		}

		void* ParameterAddress = &InParameter;
		InternalBroadcastEvent(FindResult, T::StaticStruct(), ParameterAddress);
	}
	UFUNCTION(BlueprintCallable, CustomThunk, meta = (CustomStructureParam = "InParameter"), Category = "Event")
	void BroadcastGlobalEvent(const FGameplayTag& InEventName, int32 InParameter);
	DECLARE_FUNCTION(execBroadcastGlobalEvent)
	{
		Stack.MostRecentProperty = nullptr;

		// 更新蓝图虚拟机栈顶指针
		Stack.StepCompiledIn<FProperty>(nullptr);
		// 获取第一个数据的参数的地址
		FGameplayTag* EventNamePointer = (FGameplayTag*)Stack.MostRecentPropertyAddress;
		
		// 更新蓝图虚拟机栈顶指针
		Stack.StepCompiledIn<FProperty>(nullptr); 
		// 获取第二个无类型参数的内存地址
		uint8* ParameterPointer = Stack.MostRecentPropertyAddress;
		// 获取第二个参数的反射信息
		FStructProperty* ParameterProperty = CastField<FStructProperty>(Stack.MostRecentProperty);

		// 停止对蓝图栈的使用
		P_FINISH;

		if (!EventNamePointer || !ParameterPointer || !ParameterProperty)
		{
			return;
		}
		
		UBXEventManager* Manager = P_THIS_CAST(UBXEventManager);
		if (!IsValid(Manager) || !Manager->IsValidLowLevel())
		{
			return;
		}
		
		P_NATIVE_BEGIN;
		FBXECallbackMap* FindResult = Manager->GlobalEventCallbacks.Find(*EventNamePointer);
		if (!FindResult)
		{
			return;
		}
		Manager->InternalBroadcastEvent(FindResult, ParameterProperty->Struct, ParameterPointer);
		P_NATIVE_END;
	}
	
	// 注册单体事件
	UFUNCTION(BlueprintCallable)
    bool RegisterSingleEvent(const FGameplayTag& InEventName, UObject* InInitiator, UObject* InTarget, FName InFunctionName);
    
    // 注销单体事件(函数名为None，则注销所有回调)
    UFUNCTION(BlueprintCallable)
    bool UnregisterSingleEvent(const FGameplayTag& InEventName, UObject* InInitiator, UObject* InTarget, FName InFunctionName = NAME_None);

	// 广播单体事件
	template<typename T>
	void BroadcastSingleEvent(const FGameplayTag& InEventName, UObject* InInitiator, T& InParameter)
	{
		FBXECallbackMap* FindResult = SingleEventCallbacks.Find(FBXESingleKey(InEventName, InInitiator));
		if (!FindResult)
		{
			return;
		}

		void* ParameterAddress = static_cast<void*>(&InParameter);
		InternalBroadcastEvent(FindResult, T::StaticStruct(), ParameterAddress);
	}
	UFUNCTION(BlueprintCallable, CustomThunk, meta = (CustomStructureParam = "InParameter"), Category = "Event")
	void BroadcastSingleEvent(const FGameplayTag& InEventName, UObject* InInitiator, int32 InParameter);
	DECLARE_FUNCTION(execBroadcastSingleEvent)
	{
		Stack.MostRecentProperty = nullptr;

		// 更新蓝图虚拟机栈顶指针
		Stack.StepCompiledIn<FProperty>(nullptr);
		// 获取第一个数据的参数的地址
		FGameplayTag* EventNamePointer = (FGameplayTag*)Stack.MostRecentPropertyAddress;
		
		// 更新蓝图虚拟机栈顶指针
		Stack.StepCompiledIn<FProperty>(nullptr);
		// 获取第二个数据的参数的地址
		UObject** InitiatorPointer = (UObject**)Stack.MostRecentPropertyAddress;
		
		// 更新蓝图虚拟机栈顶指针
		Stack.StepCompiledIn<FProperty>(nullptr); 
		// 获取第三个无类型参数的内存地址
		uint8* ParameterPointer = Stack.MostRecentPropertyAddress;
		// 获取第三个参数的反射信息
		FStructProperty* ParameterProperty = CastField<FStructProperty>(Stack.MostRecentProperty);

		// 停止对蓝图栈的使用
		P_FINISH;

		if (!EventNamePointer || !InitiatorPointer || !ParameterPointer || !ParameterProperty)
		{
			return;
		}
		
		UBXEventManager* Manager = P_THIS_CAST(UBXEventManager);
		if (!IsValid(Manager) || !Manager->IsValidLowLevel())
		{
			return;
		}
		
		P_NATIVE_BEGIN;
		FBXECallbackMap* FindResult = Manager->SingleEventCallbacks.Find(FBXESingleKey(*EventNamePointer, *InitiatorPointer));
		if (!FindResult)
		{
			return;
		}
		Manager->InternalBroadcastEvent(FindResult, ParameterProperty->Struct, ParameterPointer);
		P_NATIVE_END;
	}
	
protected:
	bool InternalRegisterCallback(const FGameplayTag& InEventName, FBXECallbackMap* InCBMap, UObject* InTarget, FName InFunctionName);

	bool InternalUnregisterCallback(FBXECallbackMap* InCBMap, UObject* InTarget, FName InFunctionName);
	
	void InternalBroadcastEvent(FBXECallbackMap* InCBList, UScriptStruct* InDataType, void* InDataAddress);

	// 更新对象的关联信息，InType: 1:全局目标 2:单体目标 3:单体广播者
	void InternalUpdateAssociation(const FGameplayTag& InEventName, UObject* InInitiator, UObject* InTarget, int32 InType);
	
protected:
	// 定义的事件
	UPROPERTY(EditDefaultsOnly, Category = "Event")
	TMap<FGameplayTag, UScriptStruct*> DefinedEvents;
	
	// 全局事件回调
	UPROPERTY(Transient)
	TMap<FGameplayTag, FBXECallbackMap> GlobalEventCallbacks;

	// 单体事件回调
	UPROPERTY(Transient)
	TMap<FBXESingleKey, FBXECallbackMap> SingleEventCallbacks;
	
#pragma endregion Event
	
};
