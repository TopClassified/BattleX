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
	GlobalEventCallbacks.Empty();
	SingleEventCallbacks.Empty();
	
	GUObjectArray.RemoveUObjectDeleteListener(&ObjectListener);
}

void UBXEventManager::NotifyUObjectDeleted(const UObjectBase* InObject)
{
	uint32 ID = InObject->GetUniqueID();
	if (const UObject* Object = static_cast<const UObject*>(InObject))
	{
		// 移除关联的全局事件目标
		if (TArray<FGameplayTag>* Result = GlobalTargetMap.Find(ID))
		{
			for (TArray<FGameplayTag>::TIterator It(*Result); It; ++It)
			{
				if (FBXECallbackMap* FindMap = GlobalEventCallbacks.Find(*It))
				{
					FindMap->Map.Remove(Object);
				}
			}
			
			GlobalTargetMap.Remove(ID);
		}

		// 移除关联的单体事件发起者
		if (TArray<FGameplayTag>* Result = SingleKeyMap.Find(ID))
		{
			FBXESingleKey SearchKey;
			for (TArray<FGameplayTag>::TIterator It(*Result); It; ++It)
			{
				SearchKey.EventName = *It;
				SearchKey.Initiator = Object;
				SingleEventCallbacks.Remove(SearchKey);
			}
			
			SingleKeyMap.Remove(ID);
		}

		// 移除关联的单体事件目标
		if (TArray<FGameplayTag>* Result = SingleTargetMap.Find(ID))
		{
			for (TMap<FBXESingleKey, FBXECallbackMap>::TIterator It(SingleEventCallbacks); It; ++It)
			{
				if (Result->Contains(It->Key.EventName))
				{
					It->Value.Map.Remove(Object);
				}
			}
			
			SingleTargetMap.Remove(ID);
		}
	}
}

#pragma endregion Important



#pragma region Event
bool UBXEventManager::RegisterGlobalEvent(const FGameplayTag& InEventName, UObject* InTarget, FName InFunctionName)
{
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
	
bool UBXEventManager::RegisterSingleEvent(const FGameplayTag& InEventName, UObject* InInitiator, UObject* InTarget, FName InFunctionName)
{
	if (!IsValid(InTarget) || !IsValid(InInitiator))
	{
		UE_LOG(BXMGR_Event, Warning, TEXT("Register Single Event(%s) Failed! Callback Target Or Event Initiator Is Invalid."), *InEventName.GetTagName().ToString());
		return false;
	}

	FBXESingleKey SearchKey(InEventName, InInitiator);
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
	if (!IsValid(InTarget) || !IsValid(InInitiator))
	{
		UE_LOG(BXMGR_Event, Warning, TEXT("Unregister Single Event(%s) Failed! Callback Target Or Event Initiator Is Invalid."), *InEventName.GetTagName().ToString());
		return false;
	}

	FBXESingleKey SearchKey(InEventName, InInitiator);
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

	UFunction* Function = InTarget->GetClass()->FindFunctionByName(InFunctionName);
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
	
	for (TFieldIterator<FProperty> ParamIt(Function); ParamIt; ++ParamIt)
	{
		FStructProperty* Property = CastField<FStructProperty>(*ParamIt);
		if (!Property || EventParameterType != Property->Struct)
		{
			UE_LOG(BXMGR_Event, Warning, TEXT("The Function(%s) Argument Type Is Incorrect."), *InFunctionName.ToString());
			return false;
		}
		
		if (!(Property->PropertyFlags & CPF_Parm) || (Property->PropertyFlags & CPF_ReturnParm) || (Property->PropertyFlags & CPF_OutParm))
		{
			UE_LOG(BXMGR_Event, Warning, TEXT("The Function(%s) Argument Flag Is Incorrect."), *InFunctionName.ToString());
			return false;
		}
	}

	CB->Functions.Add(Function);
	return true;
}

bool UBXEventManager::InternalUnregisterCallback(FBXECallbackMap* InCBMap, UObject* InTarget, FName InFunctionName)
{
	FBXECallback* CB = InCBMap->Map.Find(InTarget);
	if (!CB)
	{
		InCBMap->Map.Add(InTarget);
		CB = InCBMap->Map.Find(InTarget);
	}
	if (!CB)
	{
		UE_LOG(BXMGR_Event, Warning, TEXT("Find Callback Array Failed."));
		return false;
	}

	// 如果函数名为空，则注销该目标对象
	if (InFunctionName.IsNone())
	{
		InCBMap->Map.Remove(InTarget);
		return true;
	}
	
	UFunction* Function = InTarget->GetClass()->FindFunctionByName(InFunctionName);
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
	
	for (TMap<UObject*, FBXECallback>::TIterator CB(InCBMap->Map); CB; ++CB)
	{
		if (!IsValid(CB->Key) || !CB->Key->IsValidLowLevelFast())
		{
			continue;
		}
    			
		for (TArray<UFunction*>::TIterator It(CB->Value.Functions); It; ++It)
		{
			UFunction* Function = *It;
			if (!IsValid(Function))
			{
				UE_LOG(BXMGR_Event, Warning, TEXT("Broadcast Event Failed! Object(%s) UFunction Is Invalid."), *CB->Key->GetName());
				continue;
			}
    
			CB->Key->ProcessEvent(Function, InDataAddress);
		}
	}
}

void UBXEventManager::InternalUpdateAssociation(const FGameplayTag& InEventName, UObject* InInitiator, UObject* InTarget, int32 InType)
{
	auto UpdateAssociationList = [&](TMap<uint32, TArray<FGameplayTag>>& InOutMap, uint32& InUID, bool bFlag)
	{
		TArray<FGameplayTag>* Result = InOutMap.Find(InUID);
		if (Result)
		{
			if (bFlag)
			{
				Result->AddUnique(InEventName);
			}
			else
			{
				Result->RemoveSwap(InEventName);
				if (Result->IsEmpty())
				{
					InOutMap.Remove(InUID);
				}
			}
		}
		else
		{
			if (bFlag)
			{
				InOutMap.Add(InUID, {InEventName});
			}
		}
	};
	
	// 添加/移除全局事件目标关联对象
	if (InType == 1)
	{
		if (!IsValid(InTarget))
		{
			return;
		}

		uint32 TargetUID = InTarget->GetUniqueID();
		bool bFlag = false;
		if (FBXECallbackMap* CBMap = GlobalEventCallbacks.Find(InEventName))
		{
			if (CBMap->Map.Contains(InTarget))
			{
				bFlag = true;
			}
		}
		
		UpdateAssociationList(GlobalTargetMap, TargetUID, bFlag);
	}
	// 添加/移除单体事件目标关联对象
	else if (InType == 2)
	{
		if (!IsValid(InInitiator) || !IsValid(InTarget))
		{
			return;
		}
		
		uint32 TargetUID = InTarget->GetUniqueID();
		FBXESingleKey SearchKey(InEventName, InInitiator);
		bool bFlag = false;
		if (FBXECallbackMap* CBMap = SingleEventCallbacks.Find(SearchKey))
		{
			if (CBMap->Map.Contains(InTarget))
			{
				bFlag = true;
			}
		}

		UpdateAssociationList(SingleTargetMap, TargetUID, bFlag);
	}
	// 添加/移除单体事件发送者关联对象
	else if (InType == 3)
	{
		if (!IsValid(InInitiator))
		{
			return;
		}

		uint32 InitiatorUID = InInitiator->GetUniqueID();
		FBXESingleKey SearchKey(InEventName, InInitiator);
		bool bFlag = SingleEventCallbacks.Contains(SearchKey);

		UpdateAssociationList(SingleKeyMap, InitiatorUID, bFlag);
	}
}

#pragma endregion Event
