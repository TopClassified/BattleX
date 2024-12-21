#pragma once

#include "CoreMinimal.h"
#include "BXManager.h"

#include "BXMiscManager.generated.h" 



struct FHTRegisteredFunction
{
public:
	FHTRegisteredFunction() {}
	FHTRegisteredFunction(UObject* InObject, UFunction* InFunction): Object(InObject), Function(InFunction) {}
	FHTRegisteredFunction(UObject* InObject, UFunction* InFunction, float InInterval): Object(InObject), Function(InFunction), Interval(InInterval) {}
	FHTRegisteredFunction(FHTRegisteredFunction&& InOther)
	{
		Object = InOther.Object;
		Function = InOther.Function;
		Interval = InOther.Interval;
		RemainTime = InOther.RemainTime;
	}
	FHTRegisteredFunction(const FHTRegisteredFunction& InOther)
	{
		Object = InOther.Object;
		Function = InOther.Function;
		Interval = InOther.Interval;
		RemainTime = InOther.RemainTime;
	}
	FHTRegisteredFunction& operator=(FHTRegisteredFunction&& InOther)
	{
		Object = InOther.Object;
		Function = InOther.Function;
		Interval = InOther.Interval;
		RemainTime = InOther.RemainTime;

		return *this;
	}
	FHTRegisteredFunction& operator=(const FHTRegisteredFunction& InOther)
	{
		Object = InOther.Object;
		Function = InOther.Function;
		Interval = InOther.Interval;
		RemainTime = InOther.RemainTime;

		return *this;
	}

	bool operator==(const FHTRegisteredFunction& Other) const
	{
		return Object == Other.Object && Function == Other.Function;
	}
		
public:
	TWeakObjectPtr<UObject> Object = nullptr;
	TWeakObjectPtr<UFunction> Function = nullptr;
	float Interval = 0.1f;
	float RemainTime = 0.0f;
};

class FBXHelperRunnable : public FRunnable
{
public:
	virtual uint32 Run() override;

	virtual void Stop() override;
	
public:
	bool bShouldStop = false;
	TArray<FHTRegisteredFunction> HTRegisteredFunctions;
	TLockFreePointerListFIFO<FHTRegisteredFunction, 32> HTPendingRegisteredFunctions;
	TLockFreePointerListFIFO<FHTRegisteredFunction, 32> HTPendingUnregisteredFunctions;
	
};






UCLASS(Blueprintable)
class BATTLEX_API UBXMiscManager : public UBXManager
{
	GENERATED_BODY()

#pragma region Important
public:
	UFUNCTION(BlueprintCallable)
	static UBXMiscManager* Get(UObject* InWorldContext);
	
	virtual void Initialize();

	virtual void Deinitialize();

#pragma endregion Important


	
#pragma region HelperThread
public:
	UFUNCTION(BlueprintCallable)
	void RegisterHTFunction(UObject* InObject, FName InFunctionName, float InInterval);
	
	UFUNCTION(BlueprintCallable)
	void UnregisterHTFunction(UObject* InObject, FName InFunctionName);

	UFUNCTION(BlueprintCallable)
	void UnregisterHTFunctionByUObject(UObject* InObject);
	
protected:
	TUniquePtr<FRunnableThread> HelperThread = nullptr;
	TUniquePtr<FBXHelperRunnable> HelperRunnable = nullptr;
	
#pragma endregion HelperThread
	
};
