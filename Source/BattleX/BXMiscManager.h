#pragma once

#include "CoreMinimal.h"
#include "Tickable.h"
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

// 到期调用(辅助线程计时到期产生,游戏线程消费执行)
// 所有权:辅助线程new入队 → Manager::Tick Pop后执行并delete
struct FHTExpiredCall
{
public:
	FHTExpiredCall() {}
	FHTExpiredCall(TWeakObjectPtr<UObject> InObject, TWeakObjectPtr<UFunction> InFunction) : Object(InObject), Function(InFunction) {}

public:
	TWeakObjectPtr<UObject> Object = nullptr;
	TWeakObjectPtr<UFunction> Function = nullptr;
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

	// 到期调用队列(辅助线程产出,游戏线程Manager::Tick消费;UObject/蓝图反射非线程安全,禁止辅助线程直接ProcessEvent)
	TLockFreePointerListFIFO<FHTExpiredCall, 32> HTExpiredFunctions;

};




UCLASS(Blueprintable)
class BATTLEX_API UBXMiscManager : public UBXManager, public FTickableGameObject
{
	GENERATED_BODY()

#pragma region Important
public:
	UFUNCTION(BlueprintCallable)
	static UBXMiscManager* Get(UObject* InWorldContext);

	virtual void Initialize();

	virtual void Deinitialize();

	// 每帧更新(游戏线程消费到期队列并执行,蓝图函数的线程安全执行点)
	virtual void Tick(float DeltaTime) override;

	// 是否允许每帧更新
	virtual bool IsTickable() const override { return true; }

	// 是否在编辑器中也更新
	virtual bool IsTickableInEditor() const override { return false; }

	// 获取StatId
	virtual TStatId GetStatId() const override { RETURN_QUICK_DECLARE_CYCLE_STAT(UBXMiscManager, STATGROUP_Tickables); }

	virtual UWorld* GetTickableGameObjectWorld() const override;

	virtual ETickableTickType GetTickableTickType() const override;

	virtual bool IsAllowedToTick() const override;

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
