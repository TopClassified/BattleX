#include "BXEventManager.h"
#include "BXSubSystem.h"



DEFINE_LOG_CATEGORY(BXMGR_Event);



void FBXEventManagerODL::NotifyUObjectDeleted(const UObjectBase* InObject, int32 Index)
{
	if (Manager.IsValid())
	{
		Manager->NotifyUObjectDeleted(InObject);
	}
}



#pragma region Important
UBXEventManager* UBXEventManager::Get(UObject* InWorldContext)
{
	UBXEventManager* Result = nullptr;

	if (InWorldContext)
	{
		if (UWorld* World = InWorldContext->GetWorld())
		{
			if (UGameInstance* GI = World->GetGameInstance())
			{
				if (UBXSubSystem* BXSS = GI->GetSubsystem<UBXSubSystem>())
				{
					Result = BXSS->GetManagerByClass<UBXEventManager>();
				}
			}

#if WITH_EDITOR
			if (!IsValid(Result))
			{
				for (TObjectIterator<UBXEventManager> It; It; ++It)
				{
					if (It->GetOuter() == World)
					{
						Result = *It;
						break;
					}
				}
			}
#endif
		}
	}

	return Result;
}
	
void UBXEventManager::Initialize()
{
	ObjectListener.Register(this);
	GUObjectArray.AddUObjectDeleteListener(&ObjectListener);
}

void UBXEventManager::Deinitialize()
{
	// 刷新可能残留的延迟变更
	FlushPendingMutations();

	GlobalEventCallbacks.Empty();
	SingleEventCallbacks.Empty();
	NativeGlobalCallbacks.Empty();
	NativeSingleCallbacks.Empty();
	UIDToGeneration.Empty();
	GlobalTargetMap.Empty();
	SingleKeyMap.Empty();
	SingleTargetMap.Empty();
	FunctionCache.Empty();
	PendingMutations.Empty();
	CachedInvalidTargets.Empty();
#if WITH_EDITOR
	EventHistory.Empty();
#endif

	GUObjectArray.RemoveUObjectDeleteListener(&ObjectListener);
}

uint32 UBXEventManager::GetOrAssignGeneration(UObject* InObject)
{
	if (!IsValid(InObject))
	{
		return 0;
	}

	uint32 UID = InObject->GetUniqueID();
	if (const uint32* Existing = UIDToGeneration.Find(UID))
	{
		return *Existing;
	}
	uint32 Gen = NextGeneration++;
	UIDToGeneration.Add(UID, Gen);
	return Gen;
}

FBXESingleKey UBXEventManager::MakeSingleKey(const FGameplayTag& InEventName, UObject* InInitiator)
{
	if (InInitiator)
	{
		return FBXESingleKey(InEventName, InInitiator->GetUniqueID(), GetOrAssignGeneration(InInitiator));
	}
	return FBXESingleKey(InEventName, 0, 0);
}

void UBXEventManager::NotifyUObjectDeleted(const UObjectBase* InObject)
{
	uint32 ID = InObject->GetUniqueID();

	// 递增 Generation，使旧 Key 失效（防止 UID 复用 ABA）
	if (uint32* Gen = UIDToGeneration.Find(ID))
	{
		*Gen = NextGeneration++;
	}

	// 移除关联的全局事件目标
	if (const TSet<FGameplayTag>* Result = GlobalTargetMap.Find(ID))
	{
		for (const FGameplayTag& EventName : *Result)
		{
			if (FBXECallbackMap* FindMap = GlobalEventCallbacks.Find(EventName))
			{
				// 对象正在销毁，使用 GetUniqueID 间接移除
				for (auto It = FindMap->Map.CreateIterator(); It; ++It)
				{
					if (It->Key == InObject || !IsValid(It->Key))
					{
						It.RemoveCurrent();
					}
				}
			}
		}
		GlobalTargetMap.Remove(ID);
	}

	// 移除关联的单体事件发起者（Key 中含 Generation，直接用存储的 Key 移除）
	if (const TSet<FBXESingleKey>* Result = SingleKeyMap.Find(ID))
	{
		for (const FBXESingleKey& Key : *Result)
		{
			SingleEventCallbacks.Remove(Key);
		}
		SingleKeyMap.Remove(ID);
	}

	// 移除关联的单体事件目标
	if (const TSet<FBXESingleKey>* Result = SingleTargetMap.Find(ID))
	{
		for (const FBXESingleKey& Key : *Result)
		{
			if (FBXECallbackMap* CBMap = SingleEventCallbacks.Find(Key))
			{
				for (auto It = CBMap->Map.CreateIterator(); It; ++It)
				{
					if (It->Key == InObject || !IsValid(It->Key))
					{
						It.RemoveCurrent();
					}
				}
				if (CBMap->Map.IsEmpty())
				{
					SingleEventCallbacks.Remove(Key);
				}
			}
		}
		SingleTargetMap.Remove(ID);
	}

	// 清理原生回调中 Context 指向该对象的条目
	for (auto& Pair : NativeGlobalCallbacks)
	{
		Pair.Value.RemoveAll([&InObject](const FBXNativeCallbackEntry& Entry)
		{
			return Entry.Context.IsExplicitlyNull() ? false : (Entry.Context.Get() == InObject);
		});
	}
	for (auto It = NativeSingleCallbacks.CreateIterator(); It; ++It)
	{
		It->Value.RemoveAll([&InObject](const FBXNativeCallbackEntry& Entry)
		{
			return Entry.Context.IsExplicitlyNull() ? false : (Entry.Context.Get() == InObject);
		});
		if (It->Value.IsEmpty())
		{
			It.RemoveCurrent();
		}
	}
}

#pragma endregion Important



#pragma region Event
bool UBXEventManager::RegisterGlobalEvent(const FGameplayTag& InEventName, UObject* InTarget, FName InFunctionName)
{
	SCOPE_CYCLE_COUNTER(STAT_BXEvent_Register);

	// 重入安全：广播期间延迟变更
	if (BroadcastDepth > 0)
	{
		TWeakObjectPtr<UBXEventManager> WeakThis(this);
		TWeakObjectPtr<UObject> WeakTarget(InTarget);
		PendingMutations.Add([WeakThis, InEventName, WeakTarget, InFunctionName]()
		{
			if (UBXEventManager* Mgr = WeakThis.Get())
			{
				if (UObject* Target = WeakTarget.Get())
				{
					Mgr->RegisterGlobalEvent(InEventName, Target, InFunctionName);
				}
			}
		});
		return true;
	}

	if (!IsValid(InTarget))
	{
		UE_LOG(BXMGR_Event, Warning, TEXT("Register Global Event(%s) Failed! Callback Target Is Invalid."), *InEventName.GetTagName().ToString());
		return false;
	}

	FBXECallbackMap* CBMap = GlobalEventCallbacks.Find(InEventName);
	if (!CBMap)
	{
		GlobalEventCallbacks.Add(InEventName);
		CBMap = GlobalEventCallbacks.Find(InEventName);
	}
	if (!CBMap)
	{
		UE_LOG(BXMGR_Event, Warning, TEXT("Register Global Event(%s) Failed! Can't Find Callback Map!"), *InEventName.GetTagName().ToString());
		return false;
	}
	
	if (InternalRegisterCallback(InEventName, CBMap, InTarget, InFunctionName))
	{
		InternalUpdateAssociation(InEventName, nullptr, InTarget, 1);
		return true;
	}

	UE_LOG(BXMGR_Event, Warning, TEXT("Register Global Event(%s) Failed! Internal Register Check Failed!"), *InEventName.GetTagName().ToString());
	return false;
}

bool UBXEventManager::UnregisterGlobalEvent(const FGameplayTag& InEventName, UObject* InTarget, FName InFunctionName)
{
	SCOPE_CYCLE_COUNTER(STAT_BXEvent_Unregister);

	// 重入安全：广播期间延迟变更
	if (BroadcastDepth > 0)
	{
		TWeakObjectPtr<UBXEventManager> WeakThis(this);
		TWeakObjectPtr<UObject> WeakTarget(InTarget);
		PendingMutations.Add([WeakThis, InEventName, WeakTarget, InFunctionName]()
		{
			if (UBXEventManager* Mgr = WeakThis.Get())
			{
				if (UObject* Target = WeakTarget.Get())
				{
					Mgr->UnregisterGlobalEvent(InEventName, Target, InFunctionName);
				}
			}
		});
		return true;
	}

	if (!IsValid(InTarget))
	{
		UE_LOG(BXMGR_Event, Warning, TEXT("Unregister Global Event(%s) Failed! Callback Target Is Invalid."), *InEventName.GetTagName().ToString());
		return false;
	}

	FBXECallbackMap* CBMap = GlobalEventCallbacks.Find(InEventName);
	if (!CBMap)
	{
		UE_LOG(BXMGR_Event, Warning, TEXT("Unregister Global Event(%s) Failed! Can't Find Callback Map!"), *InEventName.GetTagName().ToString());
		return false;
	}

	if (InternalUnregisterCallback(CBMap, InTarget, InFunctionName))
	{
		InternalUpdateAssociation(InEventName, nullptr, InTarget, 1);
		return true;
	}

	UE_LOG(BXMGR_Event, Warning, TEXT("Unregister Global Event(%s) Failed! Internal Unregister Check Failed!"), *InEventName.GetTagName().ToString());
	return false;
}

void UBXEventManager::BroadcastGlobalEvent(const FGameplayTag& InEventName, UScriptStruct* InStruct, void* InData)
{
	SCOPE_CYCLE_COUNTER(STAT_BXEvent_Broadcast);

	if (!IsValid(InStruct) || !InData)
	{
		return;
	}

	int32 ListenerCount = 0;

	// 快速路径：原生委托（避免 ProcessEvent 反射开销）
	if (const TArray<FBXNativeCallbackEntry>* NativeList = NativeGlobalCallbacks.Find(InEventName))
	{
		ListenerCount += NativeList->Num();
		InternalBroadcastNativeCallbacks(NativeList, InData);
	}

	// 慢速路径：UFunction 反射
	FBXECallbackMap* FindResult = GlobalEventCallbacks.Find(InEventName);
	if (FindResult)
	{
		ListenerCount += FindResult->Map.Num();
		InternalBroadcastEvent(FindResult, InStruct, InData);
	}

#if WITH_EDITOR
	RecordEventHistory(InEventName, 0, ListenerCount, false);
#endif
}

DEFINE_FUNCTION(UBXEventManager::execBroadcastGlobalEvent)
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
	Manager->BroadcastGlobalEvent(*EventNamePointer, ParameterProperty->Struct, ParameterPointer);
	P_NATIVE_END;
}
	
bool UBXEventManager::RegisterSingleEvent(const FGameplayTag& InEventName, UObject* InInitiator, UObject* InTarget, FName InFunctionName)
{
	SCOPE_CYCLE_COUNTER(STAT_BXEvent_Register);

	// 重入安全：广播期间延迟变更
	if (BroadcastDepth > 0)
	{
		TWeakObjectPtr<UBXEventManager> WeakThis(this);
		TWeakObjectPtr<UObject> WeakInitiator(InInitiator);
		TWeakObjectPtr<UObject> WeakTarget(InTarget);
		PendingMutations.Add([WeakThis, InEventName, WeakInitiator, WeakTarget, InFunctionName]()
		{
			if (UBXEventManager* Mgr = WeakThis.Get())
			{
				if (UObject* Initiator = WeakInitiator.Get())
				{
					if (UObject* Target = WeakTarget.Get())
					{
						Mgr->RegisterSingleEvent(InEventName, Initiator, Target, InFunctionName);
					}
				}
			}
		});
		return true;
	}

	if (!IsValid(InTarget) || !IsValid(InInitiator))
	{
		UE_LOG(BXMGR_Event, Warning, TEXT("Register Single Event(%s) Failed! Callback Target Or Event Initiator Is Invalid."), *InEventName.GetTagName().ToString());
		return false;
	}

	FBXESingleKey SearchKey = MakeSingleKey(InEventName, InInitiator);
	FBXECallbackMap* CBMap = SingleEventCallbacks.Find(SearchKey);
	if (!CBMap)
	{
		SingleEventCallbacks.Add(SearchKey);
		CBMap = SingleEventCallbacks.Find(SearchKey);
		InternalUpdateAssociation(InEventName, InInitiator, InTarget, 3);
	}
	if (!CBMap)
	{
		UE_LOG(BXMGR_Event, Warning, TEXT("Register Single Event(%s::%s) Failed! Can't Find Callback Map."), *InEventName.GetTagName().ToString(), *InInitiator->GetName());
		return false;
	}

	if (InternalRegisterCallback(InEventName, CBMap, InTarget, InFunctionName))
	{
		InternalUpdateAssociation(InEventName, InInitiator, InTarget, 2);
		return true;
	}

	UE_LOG(BXMGR_Event, Warning, TEXT("Register Single Event(%s::%s) Failed! Internal Register Check Failed!"), *InEventName.GetTagName().ToString(), *InInitiator->GetName());
	return false;
}
    
bool UBXEventManager::UnregisterSingleEvent(const FGameplayTag& InEventName, UObject* InInitiator, UObject* InTarget, FName InFunctionName)
{
	SCOPE_CYCLE_COUNTER(STAT_BXEvent_Unregister);

	// 重入安全：广播期间延迟变更
	if (BroadcastDepth > 0)
	{
		TWeakObjectPtr<UBXEventManager> WeakThis(this);
		TWeakObjectPtr<UObject> WeakInitiator(InInitiator);
		TWeakObjectPtr<UObject> WeakTarget(InTarget);
		PendingMutations.Add([WeakThis, InEventName, WeakInitiator, WeakTarget, InFunctionName]()
		{
			if (UBXEventManager* Mgr = WeakThis.Get())
			{
				if (UObject* Initiator = WeakInitiator.Get())
				{
					if (UObject* Target = WeakTarget.Get())
					{
						Mgr->UnregisterSingleEvent(InEventName, Initiator, Target, InFunctionName);
					}
				}
			}
		});
		return true;
	}

	if (!IsValid(InTarget) || !IsValid(InInitiator))
	{
		UE_LOG(BXMGR_Event, Warning, TEXT("Unregister Single Event(%s) Failed! Callback Target Or Event Initiator Is Invalid."), *InEventName.GetTagName().ToString());
		return false;
	}

	FBXESingleKey SearchKey = MakeSingleKey(InEventName, InInitiator);
	FBXECallbackMap* CBMap = SingleEventCallbacks.Find(SearchKey);
	if (!CBMap)
	{
		UE_LOG(BXMGR_Event, Warning, TEXT("Unregister Single Event(%s::%s) Failed! Can't Find Callback Map."), *InEventName.GetTagName().ToString(), *InInitiator->GetName());
		return false;
	}

	if (InternalUnregisterCallback(CBMap, InTarget, InFunctionName))
	{
		InternalUpdateAssociation(InEventName, InInitiator, InTarget, 2);
		return true;
	}

	UE_LOG(BXMGR_Event, Warning, TEXT("Unregister Single Event(%s::%s) Failed! Internal Unregister Check Failed!"), *InEventName.GetTagName().ToString(), *InInitiator->GetName());
	return false;
}

void UBXEventManager::BroadcastSingleEvent(const FGameplayTag& InEventName, UObject* InInitiator, UScriptStruct* InStruct, void* InData)
{
	SCOPE_CYCLE_COUNTER(STAT_BXEvent_Broadcast);

	if (!IsValid(InStruct) || !InData || !IsValid(InInitiator))
	{
		return;
	}

	FBXESingleKey SearchKey = MakeSingleKey(InEventName, InInitiator);
	int32 ListenerCount = 0;

	// 快速路径：原生委托
	if (const TArray<FBXNativeCallbackEntry>* NativeList = NativeSingleCallbacks.Find(SearchKey))
	{
		ListenerCount += NativeList->Num();
		InternalBroadcastNativeCallbacks(NativeList, InData);
	}

	// 慢速路径：UFunction 反射
	FBXECallbackMap* FindResult = SingleEventCallbacks.Find(SearchKey);
	if (FindResult)
	{
		ListenerCount += FindResult->Map.Num();
		InternalBroadcastEvent(FindResult, InStruct, InData);
	}

#if WITH_EDITOR
	RecordEventHistory(InEventName, InInitiator->GetUniqueID(), ListenerCount, true);
#endif
}

DEFINE_FUNCTION(UBXEventManager::execBroadcastSingleEvent)
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
	Manager->BroadcastSingleEvent(*EventNamePointer, *InitiatorPointer, ParameterProperty->Struct, ParameterPointer);
	P_NATIVE_END;
}


// ===== 原生事件接口实现 =====

FBXNativeCallbackHandle UBXEventManager::RegisterNativeGlobalEvent(const FGameplayTag& InEventName, UObject* InContext, FBXNativeEventDelegate InCallback)
{
	// 重入安全：广播期间延迟变更
	if (BroadcastDepth > 0)
	{
		TWeakObjectPtr<UBXEventManager> WeakThis(this);
		TWeakObjectPtr<UObject> WeakContext(InContext);
		PendingMutations.Add([WeakThis, InEventName, WeakContext, Callback = MoveTemp(InCallback)]() mutable
		{
			if (UBXEventManager* Mgr = WeakThis.Get())
			{
				if (UObject* Ctx = WeakContext.Get())
				{
					Mgr->RegisterNativeGlobalEvent(InEventName, Ctx, MoveTemp(Callback));
				}
				else
				{
					Mgr->RegisterNativeGlobalEvent(InEventName, nullptr, MoveTemp(Callback));
				}
			}
		});
		return FBXNativeCallbackHandle{0};
	}

	int32 NewID = NextNativeCallbackID++;
	FBXNativeCallbackEntry Entry;
	Entry.ID = NewID;
	Entry.Context = InContext;
	Entry.Callable = MoveTemp(InCallback);

	TArray<FBXNativeCallbackEntry>& List = NativeGlobalCallbacks.FindOrAdd(InEventName);
	List.Add(MoveTemp(Entry));

	return FBXNativeCallbackHandle{NewID};
}

bool UBXEventManager::UnregisterNativeGlobalEvent(const FGameplayTag& InEventName, FBXNativeCallbackHandle InHandle)
{
	// 重入安全：广播期间延迟变更
	if (BroadcastDepth > 0)
	{
		TWeakObjectPtr<UBXEventManager> WeakThis(this);
		PendingMutations.Add([WeakThis, InEventName, InHandle]()
		{
			if (UBXEventManager* Mgr = WeakThis.Get())
			{
				Mgr->UnregisterNativeGlobalEvent(InEventName, InHandle);
			}
		});
		return true;
	}

	TArray<FBXNativeCallbackEntry>* List = NativeGlobalCallbacks.Find(InEventName);
	if (!List)
	{
		return false;
	}

	int32 Removed = List->RemoveAll([InHandle](const FBXNativeCallbackEntry& Entry)
	{
		return Entry.ID == InHandle.ID;
	});

	if (List->IsEmpty())
	{
		NativeGlobalCallbacks.Remove(InEventName);
	}

	return Removed > 0;
}

FBXNativeCallbackHandle UBXEventManager::RegisterNativeSingleEvent(const FGameplayTag& InEventName, UObject* InInitiator, UObject* InContext, FBXNativeEventDelegate InCallback)
{
	// 重入安全：广播期间延迟变更
	if (BroadcastDepth > 0)
	{
		TWeakObjectPtr<UBXEventManager> WeakThis(this);
		TWeakObjectPtr<UObject> WeakInitiator(InInitiator);
		TWeakObjectPtr<UObject> WeakContext(InContext);
		PendingMutations.Add([WeakThis, InEventName, WeakInitiator, WeakContext, Callback = MoveTemp(InCallback)]() mutable
		{
			if (UBXEventManager* Mgr = WeakThis.Get())
			{
				if (UObject* Initiator = WeakInitiator.Get())
				{
					UObject* Ctx = WeakContext.Get();
					Mgr->RegisterNativeSingleEvent(InEventName, Initiator, Ctx, MoveTemp(Callback));
				}
			}
		});
		return FBXNativeCallbackHandle{0};
	}

	if (!IsValid(InInitiator))
	{
		return FBXNativeCallbackHandle{0};
	}

	FBXESingleKey SearchKey = MakeSingleKey(InEventName, InInitiator);
	int32 NewID = NextNativeCallbackID++;
	FBXNativeCallbackEntry Entry;
	Entry.ID = NewID;
	Entry.Context = InContext;
	Entry.Callable = MoveTemp(InCallback);

	TArray<FBXNativeCallbackEntry>& List = NativeSingleCallbacks.FindOrAdd(SearchKey);
	List.Add(MoveTemp(Entry));

	return FBXNativeCallbackHandle{NewID};
}

bool UBXEventManager::UnregisterNativeSingleEvent(const FGameplayTag& InEventName, UObject* InInitiator, FBXNativeCallbackHandle InHandle)
{
	// 重入安全：广播期间延迟变更
	if (BroadcastDepth > 0)
	{
		TWeakObjectPtr<UBXEventManager> WeakThis(this);
		TWeakObjectPtr<UObject> WeakInitiator(InInitiator);
		PendingMutations.Add([WeakThis, InEventName, WeakInitiator, InHandle]()
		{
			if (UBXEventManager* Mgr = WeakThis.Get())
			{
				if (UObject* Initiator = WeakInitiator.Get())
				{
					Mgr->UnregisterNativeSingleEvent(InEventName, Initiator, InHandle);
				}
			}
		});
		return true;
	}

	if (!IsValid(InInitiator))
	{
		return false;
	}

	FBXESingleKey SearchKey = MakeSingleKey(InEventName, InInitiator);
	TArray<FBXNativeCallbackEntry>* List = NativeSingleCallbacks.Find(SearchKey);
	if (!List)
	{
		return false;
	}

	int32 Removed = List->RemoveAll([InHandle](const FBXNativeCallbackEntry& Entry)
	{
		return Entry.ID == InHandle.ID;
	});

	if (List->IsEmpty())
	{
		NativeSingleCallbacks.Remove(SearchKey);
	}

	return Removed > 0;
}


// ===== 运行时事件定义 =====

bool UBXEventManager::DefineEvent(const FGameplayTag& InEventName, UScriptStruct* InStructType)
{
	if (!InEventName.IsValid() || !IsValid(InStructType))
	{
		UE_LOG(BXMGR_Event, Warning, TEXT("Define Event Failed! EventName Or StructType Is Invalid."));
		return false;
	}

	DefinedEvents.Add(InEventName, InStructType);
	return true;
}


// ===== 内部实现 =====

bool UBXEventManager::InternalRegisterCallback(const FGameplayTag& InEventName, FBXECallbackMap* InCBMap, UObject* InTarget, FName InFunctionName)
{
	UScriptStruct* EventParameterType = nullptr;
	if (UScriptStruct** FindResult = DefinedEvents.Find(InEventName))
	{
		EventParameterType = *FindResult;
	}
	if (!IsValid(EventParameterType))
	{
		UE_LOG(BXMGR_Event, Warning, TEXT("Event Is Invalid."));
		return false;
	}
	
	FBXECallback* CB = InCBMap->Map.Find(InTarget);
	if (!CB)
	{
		InCBMap->Map.Add(InTarget);
		CB = InCBMap->Map.Find(InTarget);
	}
	if (!CB)
	{
		UE_LOG(BXMGR_Event, Warning, TEXT("Add New Callback Array Failed."));
		return false;
	}

	// 使用缓存的 UFunction 查找
	UFunction* Function = FindFunctionCached(InTarget->GetClass(), InFunctionName);
	if (!IsValid(Function))
	{
		UE_LOG(BXMGR_Event, Warning, TEXT("Can't Find Function(%s)."), *InFunctionName.ToString());
		return false;
	}

	if (Function->NumParms != 1)
	{
		UE_LOG(BXMGR_Event, Warning, TEXT("The Function(%s) Argument Count Is Incorrect."), *InFunctionName.ToString());
		return false;
	}

	int32 ParamIndex = 0;
	for (TFieldIterator<FProperty> ParamIt(Function); ParamIt; ++ParamIt)
	{
		ParamIndex += 1;

		if (ParamIndex == 1)
		{
			FStructProperty* Property = CastField<FStructProperty>(*ParamIt);
			if (!Property || !Property->HasAnyPropertyFlags(CPF_Parm) || Property->HasAnyPropertyFlags(CPF_OutParm) || EventParameterType != Property->Struct)
			{
				UE_LOG(BXMGR_Event, Warning, TEXT("The Function(%s) Argument Type Is Incorrect."), *InFunctionName.ToString());
				return false;
			}
		}
		// 不再是参数反射，提前结束
		else
		{
			break;
		}
	}

	CB->Functions.AddUnique(Function);
	return true;
}

bool UBXEventManager::InternalUnregisterCallback(FBXECallbackMap* InCBMap, UObject* InTarget, FName InFunctionName)
{
	FBXECallback* CB = InCBMap->Map.Find(InTarget);
	if (!CB)
	{
		UE_LOG(BXMGR_Event, Warning, TEXT("Find Callback Array Failed. Target Not Registered."));
		return false;
	}

	// 如果函数名为空，则注销该目标对象
	if (InFunctionName.IsNone())
	{
		InCBMap->Map.Remove(InTarget);
		return true;
	}

	// 使用缓存的 UFunction 查找
	UFunction* Function = FindFunctionCached(InTarget->GetClass(), InFunctionName);
	if (!IsValid(Function))
	{
		UE_LOG(BXMGR_Event, Warning, TEXT("Find UFunction(%s::%s) Failed."), *InTarget->GetName(), *InFunctionName.ToString());
		return false;
	}

	CB->Functions.Remove(Function);
	if (CB->Functions.IsEmpty())
	{
		InCBMap->Map.Remove(InTarget);
	}

	return true;
}

void UBXEventManager::InternalBroadcastEvent(FBXECallbackMap* InCBMap, UScriptStruct* InDataType, void* InDataAddress)
{
	if (!InCBMap || !IsValid(InDataType) || !InDataAddress)
	{
		return;
	}

	// 重入守卫
	++BroadcastDepth;

	// 遍历回调（信任 ODL 已清理无效对象，仅做防御性检查，不在迭代中修改 Map）
	for (auto& Pair : InCBMap->Map)
	{
		UObject* Target = Pair.Key;
		if (!IsValid(Target) || !Target->IsValidLowLevelFast())
		{
			// 记录到预分配缓冲，广播结束后统一清理
			CachedInvalidTargets.Add(TPair<FBXECallbackMap*, UObject*>(InCBMap, Target));
			continue;
		}

		for (UFunction* Function : Pair.Value.Functions)
		{
			if (IsValid(Function))
			{
				Target->ProcessEvent(Function, InDataAddress);
			}
		}
	}

	--BroadcastDepth;

	// 最外层广播结束后：清理无效目标 + 刷新延迟变更
	if (BroadcastDepth == 0)
	{
		// 清理无效目标（复用预分配缓冲，避免每次广播分配）
		for (const auto& Pair : CachedInvalidTargets)
		{
			Pair.Key->Map.Remove(Pair.Value);
		}
		CachedInvalidTargets.Reset();

		FlushPendingMutations();
	}
}

void UBXEventManager::InternalBroadcastNativeCallbacks(const TArray<FBXNativeCallbackEntry>* InCallbacks, void* InDataAddress)
{
	if (!InCallbacks || InCallbacks->IsEmpty())
	{
		return;
	}

	// 重入守卫（原生回调可能触发其他事件）
	++BroadcastDepth;

	for (const FBXNativeCallbackEntry& Entry : *InCallbacks)
	{
		// 校验 Context 有效性（如果绑定了 Context）
		if (!Entry.Context.IsExplicitlyNull() && !Entry.Context.IsValid())
		{
			continue;
		}
		Entry.Callable(InDataAddress);
	}

	--BroadcastDepth;

	if (BroadcastDepth == 0)
	{
		FlushPendingMutations();
	}
}

void UBXEventManager::InternalUpdateAssociation(const FGameplayTag& InEventName, UObject* InInitiator, UObject* InTarget, int32 InType)
{
	// 添加/移除全局事件目标关联对象
	if (InType == 1)
	{
		if (!IsValid(InTarget))
		{
			return;
		}

		uint32 TargetUID = InTarget->GetUniqueID();
		bool bAdd = false;
		if (const FBXECallbackMap* CBMap = GlobalEventCallbacks.Find(InEventName))
		{
			if (CBMap->Map.Contains(InTarget))
			{
				bAdd = true;
			}
		}

		TSet<FGameplayTag>& Set = GlobalTargetMap.FindOrAdd(TargetUID);
		if (bAdd)
		{
			Set.Add(InEventName);
		}
		else
		{
			Set.Remove(InEventName);
			if (Set.IsEmpty())
			{
				GlobalTargetMap.Remove(TargetUID);
			}
		}
	}
	// 添加/移除单体事件目标关联对象
	else if (InType == 2)
	{
		if (!IsValid(InInitiator) || !IsValid(InTarget))
		{
			return;
		}

		uint32 TargetUID = InTarget->GetUniqueID();
		FBXESingleKey SearchKey = MakeSingleKey(InEventName, InInitiator);
		bool bAdd = false;
		if (const FBXECallbackMap* CBMap = SingleEventCallbacks.Find(SearchKey))
		{
			if (CBMap->Map.Contains(InTarget))
			{
				bAdd = true;
			}
		}

		TSet<FBXESingleKey>& Set = SingleTargetMap.FindOrAdd(TargetUID);
		if (bAdd)
		{
			Set.Add(SearchKey);
		}
		else
		{
			Set.Remove(SearchKey);
			if (Set.IsEmpty())
			{
				SingleTargetMap.Remove(TargetUID);
			}
		}
	}
	// 添加/移除单体事件发送者关联对象
	else if (InType == 3)
	{
		if (!IsValid(InInitiator))
		{
			return;
		}

		uint32 InitiatorUID = InInitiator->GetUniqueID();
		FBXESingleKey SearchKey = MakeSingleKey(InEventName, InInitiator);
		bool bAdd = SingleEventCallbacks.Contains(SearchKey);

		TSet<FBXESingleKey>& Set = SingleKeyMap.FindOrAdd(InitiatorUID);
		if (bAdd)
		{
			Set.Add(SearchKey);
		}
		else
		{
			Set.Remove(SearchKey);
			if (Set.IsEmpty())
			{
				SingleKeyMap.Remove(InitiatorUID);
			}
		}
	}
}

UFunction* UBXEventManager::FindFunctionCached(UClass* InClass, FName InFunctionName)
{
	if (!IsValid(InClass) || InFunctionName.IsNone())
	{
		return nullptr;
	}

	TPair<UClass*, FName> CacheKey(InClass, InFunctionName);
	if (UFunction** Cached = FunctionCache.Find(CacheKey))
	{
		if (IsValid(*Cached))
		{
			return *Cached;
		}
		// 缓存失效，移除
		FunctionCache.Remove(CacheKey);
	}

	UFunction* Func = InClass->FindFunctionByName(InFunctionName);
	if (IsValid(Func))
	{
		FunctionCache.Add(CacheKey, Func);
	}
	return Func;
}

void UBXEventManager::FlushPendingMutations()
{
	if (PendingMutations.IsEmpty())
	{
		return;
	}

	// 取出队列，清空原数组（防止重入时再次累积）
	TArray<TFunction<void()>> LocalMutations = MoveTemp(PendingMutations);
	for (const TFunction<void()>& Mutation : LocalMutations)
	{
		Mutation();
	}
}

#pragma endregion Event


#pragma region Debug
void UBXEventManager::DumpEventStats() const
{
	UE_LOG(BXMGR_Event, Log, TEXT("========== BattleX Event Stats =========="));
	UE_LOG(BXMGR_Event, Log, TEXT("Defined Events: %d"), DefinedEvents.Num());
	UE_LOG(BXMGR_Event, Log, TEXT("Broadcast Depth: %d"), BroadcastDepth);
	UE_LOG(BXMGR_Event, Log, TEXT("Pending Mutations: %d"), PendingMutations.Num());

	int32 TotalUFunctionCallbacks = 0;
	int32 TotalNativeCallbacks = 0;

	UE_LOG(BXMGR_Event, Log, TEXT("---------- Global Events ----------"));
	for (const auto& Pair : GlobalEventCallbacks)
	{
		int32 TargetCount = Pair.Value.Map.Num();
		int32 NativeCount = 0;
		if (const TArray<FBXNativeCallbackEntry>* NativeList = NativeGlobalCallbacks.Find(Pair.Key))
		{
			NativeCount = NativeList->Num();
		}
		UE_LOG(BXMGR_Event, Log, TEXT("  %s: %d UFunction targets, %d native callbacks"),
			*Pair.Key.ToString(), TargetCount, NativeCount);
		TotalUFunctionCallbacks += TargetCount;
		TotalNativeCallbacks += NativeCount;
	}

	UE_LOG(BXMGR_Event, Log, TEXT("---------- Single Events ----------"));
	UE_LOG(BXMGR_Event, Log, TEXT("  Total Single Keys: %d"), SingleEventCallbacks.Num());
	for (const auto& Pair : SingleEventCallbacks)
	{
		int32 TargetCount = Pair.Value.Map.Num();
		int32 NativeCount = 0;
		if (const TArray<FBXNativeCallbackEntry>* NativeList = NativeSingleCallbacks.Find(Pair.Key))
		{
			NativeCount = NativeList->Num();
		}
		TotalUFunctionCallbacks += TargetCount;
		TotalNativeCallbacks += NativeCount;
	}

	UE_LOG(BXMGR_Event, Log, TEXT("---------- Summary ----------"));
	UE_LOG(BXMGR_Event, Log, TEXT("  Total UFunction Callbacks: %d"), TotalUFunctionCallbacks);
	UE_LOG(BXMGR_Event, Log, TEXT("  Total Native Callbacks: %d"), TotalNativeCallbacks);
	UE_LOG(BXMGR_Event, Log, TEXT("  Function Cache Size: %d"), FunctionCache.Num());
	UE_LOG(BXMGR_Event, Log, TEXT("  UID Generation Entries: %d"), UIDToGeneration.Num());
	UE_LOG(BXMGR_Event, Log, TEXT("========================================="));
}

#if WITH_EDITOR
void UBXEventManager::RecordEventHistory(const FGameplayTag& InEventName, uint32 InInitiatorUID, int32 InListenerCount, bool bIsSingle)
{
	FBXEventHistoryEntry Entry;
	Entry.EventName = InEventName;
	Entry.Time = FPlatformTime::Seconds();
	Entry.InitiatorUID = InInitiatorUID;
	Entry.ListenerCount = InListenerCount;
	Entry.bIsSingle = bIsSingle;

	if (EventHistory.Num() < EventHistoryCapacity)
	{
		EventHistory.Add(MoveTemp(Entry));
	}
	else
	{
		EventHistory[EventHistoryIndex] = MoveTemp(Entry);
		EventHistoryIndex = (EventHistoryIndex + 1) % EventHistoryCapacity;
	}
}
#endif

#pragma endregion Debug
