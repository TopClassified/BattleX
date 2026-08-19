#include "BXMiscManager.h"
#include "BXSubSystem.h"


uint32 FBXHelperRunnable::Run()
{
	double LastTime = FPlatformTime::Seconds();

	while (!bShouldStop)
	{
		// 休眠避免忙等烧核(1ms粒度对>=0.1s级的注册间隔足够)
		FPlatformProcess::Sleep(0.001f);

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

					// [游戏线程投递]辅助线程仅产生到期事件:UObject/蓝图反射体系非线程安全,
					// ProcessEvent禁止在本线程调用,由Manager::Tick(游戏线程)消费执行
					FHTExpiredCall* ExpiredCall = new FHTExpiredCall(It->Object, It->Function);
					HTExpiredFunctions.Push(ExpiredCall);
				}
			}
			else
			{
				It.RemoveCurrentSwap();
			}
		}

		// 消费注册请求(裸指针所有权:游戏线程new入队 → 本线程Pop接管,使用后delete)
		int32 Count = 0;
		while (Count < 20 && !HTPendingRegisteredFunctions.IsEmpty())
		{
			if (FHTRegisteredFunction* Pointer = HTPendingRegisteredFunctions.Pop())
			{
				if (!HTRegisteredFunctions.Contains(*Pointer))
				{
					HTRegisteredFunctions.Add(*Pointer);
				}

				delete Pointer;
			}

			Count += 1;
		}

		// 消费注销请求(同上所有权约定)
		Count = 0;
		while (Count < 20 && !HTPendingUnregisteredFunctions.IsEmpty())
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

				delete Pointer;
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
	// 结束辅助线程(Kill含Stop+Wait,确保线程退出后再清理其数据)
	if (HelperThread.IsValid())
	{
		HelperThread->Kill();
		HelperThread = nullptr;
	}
	if (HelperRunnable.IsValid())
	{
		// 清理到期队列残留(所有权归游戏线程,停机时统一释放防泄漏)
		while (!HelperRunnable->HTExpiredFunctions.IsEmpty())
		{
			if (FHTExpiredCall* ExpiredCall = HelperRunnable->HTExpiredFunctions.Pop())
			{
				delete ExpiredCall;
			}
		}

		HelperRunnable = nullptr;
	}
}

void UBXMiscManager::Tick(float DeltaTime)
{
	if (!HelperRunnable.IsValid())
	{
		return;
	}

	// 游戏线程消费到期队列并执行(单帧限额防极端积压卡帧)
	int32 Count = 0;
	while (Count < 64 && !HelperRunnable->HTExpiredFunctions.IsEmpty())
	{
		if (FHTExpiredCall* ExpiredCall = HelperRunnable->HTExpiredFunctions.Pop())
		{
			UObject* Object = ExpiredCall->Object.Get();
			UFunction* Function = ExpiredCall->Function.Get();
			if (IsValid(Object) && IsValid(Function))
			{
				Object->ProcessEvent(Function, nullptr);
			}

			delete ExpiredCall;
		}

		Count += 1;
	}
}

UWorld* UBXMiscManager::GetTickableGameObjectWorld() const
{
	if (!GetOuter())
	{
		return nullptr;
	}

	return GetOuter()->GetWorld();
}

ETickableTickType UBXMiscManager::GetTickableTickType() const
{
	return IsTemplate() ? ETickableTickType::Never : ETickableTickType::Always;
}

bool UBXMiscManager::IsAllowedToTick() const
{
	return IsValid(this) && !IsUnreachable();
}

#pragma endregion Important



#pragma region HelperThread
void UBXMiscManager::RegisterHTFunction(UObject* InObject, FName InFunctionName, float InInterval)
{
	if (!HelperRunnable.IsValid() || !IsValid(InObject))
	{
		return;
	}

	UFunction* Function = InObject->FindFunction(InFunctionName);
	if (!IsValid(Function))
	{
		return;
	}

	// 裸指针所有权转移给队列,辅助线程Pop后接管并delete(原实现推局部TSharedPtr的裸指针,函数返回即悬垂)
	FHTRegisteredFunction* Task = new FHTRegisteredFunction(InObject, Function, InInterval);
	HelperRunnable->HTPendingRegisteredFunctions.Push(Task);
}

void UBXMiscManager::UnregisterHTFunction(UObject* InObject, FName InFunctionName)
{
	if (!HelperRunnable.IsValid() || !IsValid(InObject))
	{
		return;
	}

	UFunction* Function = InObject->FindFunction(InFunctionName);
	if (!IsValid(Function))
	{
		return;
	}

	FHTRegisteredFunction* Task = new FHTRegisteredFunction(InObject, Function);
	HelperRunnable->HTPendingUnregisteredFunctions.Push(Task);
}

void UBXMiscManager::UnregisterHTFunctionByUObject(UObject* InObject)
{
	if (!HelperRunnable.IsValid() || !IsValid(InObject))
	{
		return;
	}

	FHTRegisteredFunction* Task = new FHTRegisteredFunction(InObject, nullptr);
	HelperRunnable->HTPendingUnregisteredFunctions.Push(Task);
}

#pragma endregion HelperThread
