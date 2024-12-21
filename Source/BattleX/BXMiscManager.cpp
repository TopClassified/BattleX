#include "BXMiscManager.h"
#include "BXSubSystem.h"



uint32 FBXHelperRunnable::Run()
{
	double LastTime = FPlatformTime::Seconds();
	
	while (!bShouldStop)  
	{
		double CurrentTime = FPlatformTime::Seconds();
		double DeltaTime = CurrentTime - LastTime;
		LastTime = CurrentTime;
		
		for (TArray<FHTRegisteredFunction>::TIterator It(HTRegisteredFunctions); It; ++It)
		{
			if (It->Object.IsValid() && It->Function.IsValid())
			{
				It->RemainTime -= DeltaTime;
				if (It->RemainTime <= 0.0f)
				{
					It->RemainTime = It->Interval;
					It->Object->ProcessEvent(It->Function.Get(), nullptr);
				}
			}
			else
			{
				It.RemoveCurrentSwap();
			}
		}

		int32 Count = 0;
		while(Count < 20 && !HTPendingRegisteredFunctions.IsEmpty())
		{
			if (FHTRegisteredFunction* Pointer = HTPendingRegisteredFunctions.Pop())
			{
				if (!HTRegisteredFunctions.Contains(*Pointer))
				{
					HTRegisteredFunctions.Add(*Pointer);
				}	
			}
			
			Count += 1;
		}

		Count = 0;
		while(Count < 20 && !HTPendingUnregisteredFunctions.IsEmpty())
		{
			if (FHTRegisteredFunction* Pointer = HTPendingUnregisteredFunctions.Pop())
			{
				if (Pointer->Function == nullptr)
				{
					for (TArray<FHTRegisteredFunction>::TIterator It2(HTRegisteredFunctions); It2; ++It2)
					{
						if (Pointer->Object == It2->Object)
						{
							It2.RemoveCurrentSwap();
						}
					}
				}
				else
				{
					HTRegisteredFunctions.RemoveSwap(*Pointer);
				}
			}

			Count += 1;
		}
	}
	
	return 0;
}

void FBXHelperRunnable::Stop()
{
	bShouldStop = true;
}






#pragma region Important
UBXMiscManager* UBXMiscManager::Get(UObject* InWorldContext)
{
	UBXMiscManager* Result = nullptr;

	if (InWorldContext)
	{
		if (UWorld* World = InWorldContext->GetWorld())
		{
			if (UGameInstance* GI = World->GetGameInstance())
			{
				if (UBXSubSystem* BXSS = GI->GetSubsystem<UBXSubSystem>())
				{
					Result = BXSS->GetManagerByClass<UBXMiscManager>();
				}
			}

#if WITH_EDITOR
			if (!IsValid(Result))
			{
				for (TObjectIterator<UBXMiscManager> It; It; ++It)
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
	
void UBXMiscManager::Initialize()
{
	// 创建辅助线程
	HelperRunnable = MakeUnique<FBXHelperRunnable>();
	HelperThread = TUniquePtr<FRunnableThread>(FRunnableThread::Create(HelperRunnable.Get(), TEXT("BX Helper Thread")));
}

void UBXMiscManager::Deinitialize()
{
	// 结束辅助线程
	if (HelperThread.IsValid())
	{
		HelperThread->Kill();
		HelperThread = nullptr;
	}
	if (HelperRunnable.IsValid())
	{
		HelperRunnable->Stop();
		HelperRunnable = nullptr;
	}
}

#pragma endregion Important



#pragma region HelperThread
void UBXMiscManager::RegisterHTFunction(UObject* InObject, FName InFunctionName, float InInterval)
{
	if (!HelperRunnable.IsValid())
	{
		return;
	}
	
	UFunction* Function = InObject->FindFunction(InFunctionName);
	if (!IsValid(Function))
	{
		return;
	}

	TSharedPtr<FHTRegisteredFunction> Task = MakeShared<FHTRegisteredFunction>();
	Task->Object = InObject;
	Task->Function = Function;
	Task->Interval = InInterval;
	
	HelperRunnable->HTPendingRegisteredFunctions.Push(Task.Get());
}

void UBXMiscManager::UnregisterHTFunction(UObject* InObject, FName InFunctionName)
{
	if (!HelperRunnable.IsValid())
	{
		return;
	}

	UFunction* Function = InObject->FindFunction(InFunctionName);
	if (!IsValid(Function))
	{
		return;
	}

	TSharedPtr<FHTRegisteredFunction> Task = MakeShared<FHTRegisteredFunction>();
	Task->Object = InObject;
	Task->Function = Function;
	
	HelperRunnable->HTPendingUnregisteredFunctions.Push(Task.Get());
}

void UBXMiscManager::UnregisterHTFunctionByUObject(UObject* InObject)
{
	if (!HelperRunnable.IsValid())
	{
		return;
	}
	
	TSharedPtr<FHTRegisteredFunction> Task = MakeShared<FHTRegisteredFunction>();
	Task->Object = InObject;
	Task->Function = nullptr;
	
	HelperRunnable->HTPendingUnregisteredFunctions.Push(Task.Get());
}
	
#pragma endregion HelperThread
