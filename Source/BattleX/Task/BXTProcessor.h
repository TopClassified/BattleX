#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "StructUtils/Public/StructView.h"

#include "BXEnums.h"
#include "BXTask.h"
#include "BXTLEnums.h"
#include "BXTEnums.h"
#include "BXTLStructs.h"
#include "BXTStructs.h"

#include "BXTProcessor.generated.h"



DECLARE_LOG_CATEGORY_EXTERN(BX_TP, Log, All);



UCLASS(Transient, Abstract, Blueprintable, BlueprintType)
class BATTLEX_API UBXTProcessor : public UObject
{
	GENERATED_BODY()

#pragma region Important
public:
	class UWorld* GetWorld() const override;

protected:
	virtual void Start(FBXTLRunTimeData& InOutRTData, FBXTLSectionRTData& InOutRTSData, FBXTLTaskRTData& InOutRTTData) {}
	UFUNCTION(BlueprintImplementableEvent, Category = "Important")
	void ScriptStart(UPARAM(ref) FBXTLRunTimeData& InOutRTData, UPARAM(ref) FBXTLSectionRTData& InOutRTSData, UPARAM(ref) FBXTLTaskRTData& InOutRTTData);

	virtual void Update(FBXTLRunTimeData& InOutRTData, FBXTLSectionRTData& InOutRTSData, FBXTLTaskRTData& InOutRTTData, float InDeltaTime) {}
	UFUNCTION(BlueprintImplementableEvent, Category = "Important")
	void ScriptUpdate(UPARAM(ref) FBXTLRunTimeData& InOutRTData, UPARAM(ref) FBXTLSectionRTData& InOutRTSData, UPARAM(ref) FBXTLTaskRTData& InOutRTTData, float InDeltaTime);

	virtual void End(FBXTLRunTimeData& InOutRTData, FBXTLSectionRTData& InOutRTSData, FBXTLTaskRTData& InOutRTTData, EBXTLFinishReason InReason) {}
	UFUNCTION(BlueprintImplementableEvent, Category = "Important")
	void ScriptEnd(UPARAM(ref) FBXTLRunTimeData& InOutRTData, UPARAM(ref) FBXTLSectionRTData& InOutRTSData, UPARAM(ref) FBXTLTaskRTData& InOutRTTData, EBXTLFinishReason InReason);

	virtual void ChangeTickRate(FBXTLTaskRTData& InOutRTTData, float InRate) {}
	UFUNCTION(BlueprintImplementableEvent, Category = "Important")
	void ScriptChangeTickRate(FBXTLTaskRTData& InOutRTTData, float InRate);

protected:
	// 要执行的函数(默认只执行C++函数)
	UPROPERTY(EditDefaultsOnly, Category = "Important", Meta = (Bitmask, BitmaskEnum = "EBXTProcessorFunction"))
	int32 ExecuteFunctions = 85;

#pragma endregion Important



#pragma region API
public:
	UFUNCTION(BlueprintCallable, Category = "API")
	void StartTask(UPARAM(ref) FBXTLRunTimeData& InOutRTData, UPARAM(ref) FBXTLSectionRTData& InOutRTSData, UPARAM(ref) FBXTLTaskRTData& InOutRTTData);

	UFUNCTION(BlueprintCallable, Category = "API")
	void UpdateTask(UPARAM(ref) FBXTLRunTimeData& InOutRTData, UPARAM(ref) FBXTLSectionRTData& InOutRTSData, UPARAM(ref) FBXTLTaskRTData& InOutRTTData, float InDeltaTime);

	UFUNCTION(BlueprintCallable, Category = "API")
	void EndTask(UPARAM(ref) FBXTLRunTimeData& InOutRTData, UPARAM(ref) FBXTLSectionRTData& InOutRTSData, UPARAM(ref) FBXTLTaskRTData& InOutRTTData, EBXTLFinishReason InReason);

	UFUNCTION(BlueprintCallable, Category = "API")
	void ChangeTaskTickRate(UPARAM(ref) FBXTLTaskRTData& InOutRTTData, float InRate);
	
#pragma endregion API



#pragma region GlobalAPI
public:
	// 任务是否结束
	UFUNCTION(BlueprintCallable, Category = "API")
	static bool IsTaskCompleted(const FBXTLTaskRTData& InTaskData, EBXTLFinishReason& OutReason);

	// 添加被动触发任务
	UFUNCTION(BlueprintCallable, Category = "API")
	static bool AddPendingTask(UPARAM(ref) FBXTLRunTimeData& InOutRTData, UPARAM(ref) FBXTLSectionRTData& InOutRTSData, UPARAM(ref) FBXTLTaskRTData& InOutRTTData, int64 InScope, const FGameplayTag& InEventTag);

	// 创建上下文数据作用域
	UFUNCTION(BlueprintCallable, Category = "API")
	static int64 GenerateContextScope(UPARAM(ref) FBXTLRunTimeData& InOutRTData, UPARAM(ref) FBXTLTaskRTData& InOutRTTData);
	
	// 根据目标掩码获取组件列表
	UFUNCTION(BlueprintCallable, Category = "API")
	static void GetTargetComponentList(const FBXTLRunTimeData& InRTData, const FBXTLTaskRTData& InTaskData, TArray<USceneComponent*>& OutComponents);
	// 根据目标掩码获取角色列表
	UFUNCTION(BlueprintCallable, Category = "API")
	static void GetTargetActorList(const FBXTLRunTimeData& InRTData, const FBXTLTaskRTData& InTaskData, TArray<AActor*>& OutActors);
	
	// 解析坐标系创建器
	UFUNCTION(BlueprintCallable, Category = "API")
	static bool AnalyzeTransformCreater(const FBXTLRunTimeData& InRTData, const FBXTLTaskRTData& InTaskData, AActor* InTarget, const FBXTTransformCreater& InCreater, FBXTTransformCreaterResult& OutResult);
	// 解析坐标系创建器列表
	UFUNCTION(BlueprintCallable, Category = "API")
	static bool AnalyzeTransformCreaterList(const FBXTLRunTimeData& InRTData, const FBXTLTaskRTData& InTaskData, AActor* InTarget, const TArray<FBXTTransformCreater>& InCreaterList, FBXTTransformCreaterResult& OutResult);
	
	// 写入上下文数据
	template<typename T>
	static void WriteContextData(FBXTLRunTimeData& InOutRTData, int32 InFullIndex, FGameplayTag InDataTag, int64 InParentScope, const T& InData)
	{
		if (!InOutRTData.Timeline || InFullIndex < 0)
		{
			return;
		}
		
		InOutRTData.DynamicDatas.Add(FBXTLDynamicDataSearchKey(InFullIndex, InDataTag), FInstancedStruct::Make<T>(InData));
		InOutRTData.DynamicDatas.Add(FBXTLDynamicDataSearchKey(InFullIndex, InDataTag, InParentScope), FInstancedStruct::Make<T>(InData));
	}
	UFUNCTION(BlueprintCallable, CustomThunk, meta = (CustomStructureParam = "InValue"), Category = "API")
	static void WriteContextData(UPARAM(ref) FBXTLRunTimeData& InOutRTData, int32 InFullIndex, FGameplayTag InDataTag, int64 InParentScope, int32 InValue);
	DECLARE_FUNCTION(execWriteContextData)
	{
		Stack.MostRecentProperty = nullptr;

		// 更新蓝图虚拟机栈顶指针
		Stack.StepCompiledIn<FProperty>(nullptr);
		// 获取第一个数据的参数的地址
		FBXTLRunTimeData* RTDataPointer = (FBXTLRunTimeData*)Stack.MostRecentPropertyAddress;

		// 更新蓝图虚拟机栈顶指针
		Stack.StepCompiledIn<FProperty>(nullptr);
		// 获取第二个数据的参数的地址
		int32* FullIndexPointer = (int32*)Stack.MostRecentPropertyAddress;

		// 更新蓝图虚拟机栈顶指针
		Stack.StepCompiledIn<FProperty>(nullptr);
		// 获取第三个数据的参数的地址
		FGameplayTag* DataTagPointer = (FGameplayTag*)Stack.MostRecentPropertyAddress;

		// 更新蓝图虚拟机栈顶指针
		Stack.StepCompiledIn<FProperty>(nullptr);
		// 获取第四个数据的参数的地址
		int64* ParentScopePointer = (int64*)Stack.MostRecentPropertyAddress;
		
		// 更新蓝图虚拟机栈顶指针
		Stack.StepCompiledIn<FProperty>(nullptr);
		// 获取第五个无类型参数的内存地址
		uint8* ValuePointer = Stack.MostRecentPropertyAddress;
		// 获取第五个参数的反射信息
		FStructProperty* ValueProperty = CastField<FStructProperty>(Stack.MostRecentProperty);

		// 停止对蓝图栈的使用
		P_FINISH;

		if (!RTDataPointer || !FullIndexPointer || !DataTagPointer || !ValuePointer || !ValueProperty || !ParentScopePointer)
		{
			return;
		}
		
		P_NATIVE_BEGIN;
		FInstancedStruct NewValue;
		NewValue.InitializeAs(ValueProperty->Struct, ValuePointer);
		RTDataPointer->DynamicDatas.Add(FBXTLDynamicDataSearchKey(*FullIndexPointer, *DataTagPointer), NewValue);
		RTDataPointer->DynamicDatas.Add(FBXTLDynamicDataSearchKey(*FullIndexPointer, *DataTagPointer, *ParentScopePointer), NewValue);
		P_NATIVE_END;
	}
	// 读取上下文数据
	template<typename T>
	static T* ReadContextData(const FBXTLRunTimeData& InOutRTData, int32 InFullIndex, FGameplayTag InDataTag, int64 InParentScope)
	{
		if (!InOutRTData.Timeline || InFullIndex < 0)
		{
			return nullptr;
		}

		int64 ParentScope = InParentScope;
		const FInt64Vector2* ResultPointer = InOutRTData.ScopeGraph.Find(ParentScope);
		while (ResultPointer)
		{
			// 如果全量索引相同，则中断
			if (InFullIndex == ResultPointer->Y)
			{
				break;
			}
			
			// 如果父作用域就是自己，则中断
			if (ParentScope == ResultPointer->X)
			{
				break;
			}

			ParentScope = ResultPointer->X;
			ResultPointer = InOutRTData.ScopeGraph.Find(ParentScope);
		}

		// 判断作用域合法性
		if (!ResultPointer || InFullIndex != ResultPointer->Y)
		{
			ParentScope = 0;
		}
		
		if (const FInstancedStruct* FindResult = InOutRTData.DynamicDatas.Find(FBXTLDynamicDataSearchKey(InFullIndex, InDataTag, ParentScope)))
		{
			if (FindResult->GetScriptStruct() == TBaseStructure<T>::Get())
			{
				return (T*)FindResult->GetMemory();
			}
		}

		return nullptr;
	}
	UFUNCTION(BlueprintCallable, CustomThunk, meta = (CustomStructureParam = "OutValue"), Category = "API")
	static bool ReadContextData(const FBXTLRunTimeData& InOutRTData, int32 InFullIndex, FGameplayTag InDataTag, int64 InParentScope, int32& OutValue);
	DECLARE_FUNCTION(execReadContextData)
	{
		Stack.MostRecentProperty = nullptr;

		// 更新蓝图虚拟机栈顶指针
		Stack.StepCompiledIn<FProperty>(nullptr);
		// 获取第一个数据的参数的地址
		FBXTLRunTimeData* RTDataPointer = (FBXTLRunTimeData*)Stack.MostRecentPropertyAddress;

		// 更新蓝图虚拟机栈顶指针
		Stack.StepCompiledIn<FProperty>(nullptr);
		// 获取第二个数据的参数的地址
		int32* FullIndexPointer = (int32*)Stack.MostRecentPropertyAddress;

		// 更新蓝图虚拟机栈顶指针
		Stack.StepCompiledIn<FProperty>(nullptr);
		// 获取第三个数据的参数的地址
		FGameplayTag* DataTagPointer = (FGameplayTag*)Stack.MostRecentPropertyAddress;

		// 更新蓝图虚拟机栈顶指针
		Stack.StepCompiledIn<FProperty>(nullptr);
		// 获取第四个数据的参数的地址
		int64* ParentScopePointer = (int64*)Stack.MostRecentPropertyAddress;
		
		// 更新蓝图虚拟机栈顶指针
		Stack.StepCompiledIn<FProperty>(nullptr);
		// 获取第五个无类型参数的内存地址
		uint8* ValuePointer = Stack.MostRecentPropertyAddress;
		// 获取第五个参数的反射信息
		FStructProperty* ValueProperty = CastField<FStructProperty>(Stack.MostRecentProperty);

		// 停止对蓝图栈的使用
		P_FINISH;

		bool bResult = false;
		if (!RTDataPointer || !FullIndexPointer || !DataTagPointer || !ValuePointer || !ValueProperty || !ParentScopePointer)
		{
			*(bool*)RESULT_PARAM = bResult;
			return;
		}
		
		P_NATIVE_BEGIN;
		int64 ParentScope = *ParentScopePointer;
		const FInt64Vector2* ResultPointer = RTDataPointer->ScopeGraph.Find(ParentScope);
		while (ResultPointer)
		{
			// 如果全量索引相同，则中断
			if (*FullIndexPointer == ResultPointer->Y)
			{
				break;
			}
			
			// 如果父作用域就是自己，则中断
			if (ParentScope == ResultPointer->X)
			{
				break;
			}

			ParentScope = ResultPointer->X;
			ResultPointer = RTDataPointer->ScopeGraph.Find(ParentScope);
		}

		// 判断作用域合法性
		if (!ResultPointer || *FullIndexPointer != ResultPointer->Y)
		{
			ParentScope = 0;
		}
		
		if (FInstancedStruct* FindResult = RTDataPointer->DynamicDatas.Find(FBXTLDynamicDataSearchKey(*FullIndexPointer, *DataTagPointer, ParentScope)))
		{
			bResult = true;
			if (ValueProperty->Struct == FindResult->GetScriptStruct())
			{
				ValueProperty->Struct->CopyScriptStruct(ValuePointer, FindResult->GetMemory());
			}
		}
		P_NATIVE_END;

		*(bool*)RESULT_PARAM = bResult;
	}

protected:
	static USceneComponent* AnalyzeTransformCreaterCoordinateType(const FBXTLRunTimeData& InRTData, const FBXTLTaskRTData& InTaskData, const FBXTTransformCreater& InCreater, bool bUseOrigin, FTransform& OutTransform);
	
#pragma endregion GlobalAPI

};
