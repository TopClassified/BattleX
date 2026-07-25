#pragma once

#include "CoreMinimal.h"
#include "Tickable.h"
#include "GameplayTagContainer.h"
#include "Stats/Stats.h"

#include "BXManager.h"

#include "BXEventManager.generated.h"



DECLARE_LOG_CATEGORY_EXTERN(BXMGR_Event, Log, All);

// 事件系统统计组
DECLARE_STATS_GROUP(TEXT("BattleX Event"), STATGROUP_BXEvent, STATCAT_Advanced);
DECLARE_CYCLE_STAT(TEXT("Event Broadcast"), STAT_BXEvent_Broadcast, STATGROUP_BXEvent);
DECLARE_CYCLE_STAT(TEXT("Event Register"), STAT_BXEvent_Register, STATGROUP_BXEvent);
DECLARE_CYCLE_STAT(TEXT("Event Unregister"), STAT_BXEvent_Unregister, STATGROUP_BXEvent);


// 原生事件委托（C++ 高速通道，避免 ProcessEvent 反射开销）
// 参数为事件数据的 void* 指针，调用方需自行转换为实际类型
using FBXNativeEventDelegate = TFunction<void(void*)>;


// 原生回调句柄，用于注销
struct BATTLEX_API FBXNativeCallbackHandle
{
	int32 ID = 0;
	bool IsValid() const { return ID != 0; }
};


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
	
	// 兼容构造（不携带 Generation，Gen=0，用于蓝图/外部调用）
	FBXESingleKey(const FGameplayTag& InEventName, UObject* InInitiator)
		: EventName(InEventName), InitiatorUID(InInitiator ? InInitiator->GetUniqueID() : 0) {}
	
	// 完整构造（携带 Generation，Manager 内部使用）
	FBXESingleKey(const FGameplayTag& InEventName, uint32 InInitiatorUID, uint32 InInitiatorGen)
		: EventName(InEventName), InitiatorUID(InInitiatorUID), InitiatorGeneration(InInitiatorGen) {}

public:
	UPROPERTY(Transient)
	FGameplayTag EventName;

	UPROPERTY(Transient)
	uint32 InitiatorUID = 0;
	
	// Generation 用于防止 UID 复用导致的 ABA 问题
	UPROPERTY(Transient)
	uint32 InitiatorGeneration = 0;

public:
	friend bool operator==(const FBXESingleKey& Val1, const FBXESingleKey& Val2)
	{
		return Val1.EventName == Val2.EventName 
			&& Val1.InitiatorUID == Val2.InitiatorUID 
			&& Val1.InitiatorGeneration == Val2.InitiatorGeneration;
	}

	friend uint32 GetTypeHash(const FBXESingleKey& Val)
	{
		return HashCombine(HashCombine(GetTypeHash(Val.EventName), GetTypeHash(Val.InitiatorUID)), GetTypeHash(Val.InitiatorGeneration));
	}
};


// 原生回调条目（C++ 高速通道，不参与反射序列化）
struct FBXNativeCallbackEntry
{
	int32 ID = 0;
	// 可选上下文，用于 ODL 自动清理（为空则不校验）
	TWeakObjectPtr<UObject> Context;
	FBXNativeEventDelegate Callable;
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

	// UID → Generation 映射，防止 UID 复用导致的 ABA 问题
	TMap<uint32, uint32> UIDToGeneration;
	uint32 NextGeneration = 1;

	// 获取或分配对象的 Generation
	uint32 GetOrAssignGeneration(UObject* InObject);

	// 构造带 Generation 的单播 Key（Manager 内部使用）
	FBXESingleKey MakeSingleKey(const FGameplayTag& InEventName, UObject* InInitiator);

	// 记录对象的关联信息（使用 TSet 提升注册/注销性能）
	TMap<uint32, TSet<FGameplayTag>> GlobalTargetMap;
	TMap<uint32, TSet<FBXESingleKey>> SingleKeyMap;
	TMap<uint32, TSet<FBXESingleKey>> SingleTargetMap;
#pragma endregion Important


#pragma region BlueprintEvent
public:
	// 注册全局事件
	UFUNCTION(BlueprintCallable)
	bool RegisterGlobalEvent(const FGameplayTag& InEventName, UObject* InTarget, FName InFunctionName);

	// 注销全局事件(函数名为None，则注销所有回调)
	UFUNCTION(BlueprintCallable)
	bool UnregisterGlobalEvent(const FGameplayTag& InEventName, UObject* InTarget, FName InFunctionName = NAME_None);

	// 广播全局事件
	template<typename T>
	void BroadcastGlobalEvent(const FGameplayTag& InEventName, T& InParameter)
	{
		BroadcastGlobalEvent(InEventName, T::StaticStruct(), static_cast<void*>(&InParameter));
	}
	// 广播全局事件
	void BroadcastGlobalEvent(const FGameplayTag& InEventName, UScriptStruct* InStruct, void* InData);
	// 广播全局事件
	UFUNCTION(BlueprintCallable, CustomThunk, meta = (CustomStructureParam = "InParameter"), Category = "Event")
	void BroadcastGlobalEvent(const FGameplayTag& InEventName, int32 InParameter);
	DECLARE_FUNCTION(execBroadcastGlobalEvent);
	
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
		BroadcastSingleEvent(InEventName, InInitiator, T::StaticStruct(), static_cast<void*>(&InParameter));
	}
	// 广播单体事件
	void BroadcastSingleEvent(const FGameplayTag& InEventName, UObject* InInitiator, UScriptStruct* InStruct, void* InData);
	// 广播单体事件
	UFUNCTION(BlueprintCallable, CustomThunk, meta = (CustomStructureParam = "InParameter"), Category = "Event")
	void BroadcastSingleEvent(const FGameplayTag& InEventName, UObject* InInitiator, int32 InParameter);
	DECLARE_FUNCTION(execBroadcastSingleEvent);
#pragma endregion BlueprintEvent


#pragma region NativeEvent
public:
	// 注册原生全局事件回调，返回句柄用于注销（InContext 用于 ODL 自动清理，可为空）
	FBXNativeCallbackHandle RegisterNativeGlobalEvent(const FGameplayTag& InEventName, UObject* InContext, FBXNativeEventDelegate InCallback);

	// 注销原生全局事件回调
	bool UnregisterNativeGlobalEvent(const FGameplayTag& InEventName, FBXNativeCallbackHandle InHandle);

	// 注册原生单体事件回调，返回句柄用于注销
	FBXNativeCallbackHandle RegisterNativeSingleEvent(const FGameplayTag& InEventName, UObject* InInitiator, UObject* InContext, FBXNativeEventDelegate InCallback);

	// 注销原生单体事件回调
	bool UnregisterNativeSingleEvent(const FGameplayTag& InEventName, UObject* InInitiator, FBXNativeCallbackHandle InHandle);
#pragma endregion NativeEvent


#pragma region RuntimeDefinition
public:
	// 运行时定义事件参数类型（也可通过 DefinedEvents 默认配置）
	UFUNCTION(BlueprintCallable, Category = "Event")
	bool DefineEvent(const FGameplayTag& InEventName, UScriptStruct* InStructType);
#pragma endregion RuntimeDefinition


#pragma region Internal
protected:
	bool InternalRegisterCallback(const FGameplayTag& InEventName, FBXECallbackMap* InCBMap, UObject* InTarget, FName InFunctionName);

	bool InternalUnregisterCallback(FBXECallbackMap* InCBMap, UObject* InTarget, FName InFunctionName);

	// 广播核心实现（含重入守卫、原生委托优先、预分配清理缓冲）
	void InternalBroadcastEvent(FBXECallbackMap* InCBMap, UScriptStruct* InDataType, void* InDataAddress);

	// 广播原生回调
	void InternalBroadcastNativeCallbacks(const TArray<FBXNativeCallbackEntry>* InCallbacks, void* InDataAddress);

	// 更新对象的关联信息，InType: 1:全局目标 2:单体目标 3:单体广播者
	void InternalUpdateAssociation(const FGameplayTag& InEventName, UObject* InInitiator, UObject* InTarget, int32 InType);

	// 查找 UFunction（带缓存）
	UFunction* FindFunctionCached(UClass* InClass, FName InFunctionName);

	// 刷新延迟变更队列（广播结束后调用）
	void FlushPendingMutations();

	// 延迟执行需要 Manager 的操作到广播结束后（内部自动捕获 WeakThis 生命周期）
	void DeferManagerOperation(TFunction<void(UBXEventManager*)> InOperation);
#pragma endregion Internal


#pragma region DataMember
protected:
	// 定义的事件（运行时可扩展）
	UPROPERTY(EditDefaultsOnly, Category = "Event")
	TMap<FGameplayTag, UScriptStruct*> DefinedEvents;

	// 全局事件回调（蓝图/UFunction 通道）
	UPROPERTY(Transient)
	TMap<FGameplayTag, FBXECallbackMap> GlobalEventCallbacks;

	// 单体事件回调（蓝图/UFunction 通道）
	UPROPERTY(Transient)
	TMap<FBXESingleKey, FBXECallbackMap> SingleEventCallbacks;

	// 原生全局事件回调（C++ 高速通道）
	TMap<FGameplayTag, TArray<FBXNativeCallbackEntry>> NativeGlobalCallbacks;

	// 原生单体事件回调（C++ 高速通道）
	TMap<FBXESingleKey, TArray<FBXNativeCallbackEntry>> NativeSingleCallbacks;

	// UFunction 查找缓存：(Class, FunctionName) → UFunction*
	TMap<TPair<UClass*, FName>, UFunction*> FunctionCache;

	// 广播深度计数器，>0 时注册/注销操作会被延迟到广播结束
	int32 BroadcastDepth = 0;
	// 延迟变更队列
	TArray<TFunction<void()>> PendingMutations;

	// 原生回调 ID 生成器
	int32 NextNativeCallbackID = 1;

	// 记录 (CBMap, InvalidTarget) 对，广播结束后统一清理
	TArray<TPair<FBXECallbackMap*, UObject*>> CachedInvalidTargets;
#pragma endregion DataMember


#pragma region Debug
public:
	// 打印事件统计信息到日志
	UFUNCTION(BlueprintCallable, Category = "Event|Debug")
	void DumpEventStats() const;

#if WITH_EDITOR
	// 事件历史记录（环形缓冲）
	struct FBXEventHistoryEntry
	{
		FGameplayTag EventName;
		double Time = 0.0;
		uint32 InitiatorUID = 0;
		int32 ListenerCount = 0;
		bool bIsSingle = false;
	};
	
	static constexpr int32 EventHistoryCapacity = 256;
	TArray<FBXEventHistoryEntry> EventHistory;
	int32 EventHistoryIndex = 0;
	
	void RecordEventHistory(const FGameplayTag& InEventName, uint32 InInitiatorUID, int32 InListenerCount, bool bIsSingle);
#endif
#pragma endregion Debug
};
