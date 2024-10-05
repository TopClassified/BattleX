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



UCLASS(Transient, Abstract, Blueprintable, BlueprintType)
class BATTLEX_API UBXTProcessor : public UObject
{
	GENERATED_BODY()

#pragma region Important
public:
	class UWorld* GetWorld() const override;

protected:
	virtual bool Start(FBXTLRunTimeData& InOutRTData, FBXTLSectionRTData& InOutRTSData, FBXTLTaskRTData& InOutRTTData, class UBXTask* InTask) { return true; }
	UFUNCTION(BlueprintImplementableEvent, Category = "Important")
	bool ScriptStart(UPARAM(ref) FBXTLRunTimeData& InOutRTData, UPARAM(ref) FBXTLSectionRTData& InOutRTSData, UPARAM(ref) FBXTLTaskRTData& InOutRTTData, class UBXTask* InTask);

	virtual bool Update(FBXTLRunTimeData& InOutRTData, FBXTLSectionRTData& InOutRTSData, FBXTLTaskRTData& InOutRTTData, class UBXTask* InTask, float InDeltaTime, float InTimeRate) { return true; }
	UFUNCTION(BlueprintImplementableEvent, Category = "Important")
	bool ScriptUpdate(UPARAM(ref) FBXTLRunTimeData& InOutRTData, UPARAM(ref) FBXTLSectionRTData& InOutRTSData, UPARAM(ref) FBXTLTaskRTData& InOutRTTData, class UBXTask* InTask, float InDeltaTime, float InTimeRate);

	virtual bool End(FBXTLRunTimeData& InOutRTData, FBXTLSectionRTData& InOutRTSData, FBXTLTaskRTData& InOutRTTData, class UBXTask* InTask, EBXTLFinishReason InReason) { return true; }
	UFUNCTION(BlueprintImplementableEvent, Category = "Important")
	bool ScriptEnd(UPARAM(ref) FBXTLRunTimeData& InOutRTData, UPARAM(ref) FBXTLSectionRTData& InOutRTSData, UPARAM(ref) FBXTLTaskRTData& InOutRTTData, class UBXTask* InTask, EBXTLFinishReason InReason);

protected:
	// 要执行的函数(默认只执行C++函数)
	UPROPERTY(EditDefaultsOnly, Category = "Important", Meta = (Bitmask, BitmaskEnum = "EBXTProcessorFunction"))
	int32 ExecuteFunctions = 21;

#pragma endregion Important



#pragma region API
public:
	UFUNCTION(BlueprintCallable, Category = "API")
	bool StartTask(UPARAM(ref) FBXTLRunTimeData& InOutRTData, UPARAM(ref) FBXTLSectionRTData& InOutRTSData, UPARAM(ref) FBXTLTaskRTData& InOutRTTData, class UBXTask* InTask);

	UFUNCTION(BlueprintCallable, Category = "API")
	bool UpdateTask(UPARAM(ref) FBXTLRunTimeData& InOutRTData, UPARAM(ref) FBXTLSectionRTData& InOutRTSData, UPARAM(ref) FBXTLTaskRTData& InOutRTTData, class UBXTask* InTask, float InDeltaTime, float InTimeRate = 1.0f);

	UFUNCTION(BlueprintCallable, Category = "API")
	bool EndTask(UPARAM(ref) FBXTLRunTimeData& InOutRTData, UPARAM(ref) FBXTLSectionRTData& InOutRTSData, UPARAM(ref) FBXTLTaskRTData& InOutRTTData, class UBXTask* InTask, EBXTLFinishReason InReason);

#pragma endregion API



#pragma region GlobalAPI
public:
	UFUNCTION(BlueprintCallable, Category = "API")
	static bool IsTaskCompleted(class UBXTask* InTask, const FBXTLTaskRTData& InTaskData, EBXTLFinishReason& OutReason);

	UFUNCTION(BlueprintCallable, Category = "API")
	static bool AddPendingTask(UPARAM(ref) FBXTLRunTimeData& InOutRTData, UPARAM(ref) FBXTLSectionRTData& InOutRTSData, UPARAM(ref) FBXTLTaskRTData& InOutRTTData, const FName& InEventName);
	
	// 解析坐标系创建器
	UFUNCTION(BlueprintCallable, Category = "API")
	static bool AnalyzeTransformCreater(AActor* InTarget, const FBXTLRunTimeData& InRTData, const FBXTTransformCreater& InCreater, FBXTTransformCreaterResult& OutResult);

	// 解析坐标系创建器列表
	UFUNCTION(BlueprintCallable, Category = "API")
	static bool AnalyzeTransformCreaterList(AActor* InTarget, const FBXTLRunTimeData& InRTData, const TArray<FBXTTransformCreater>& InCreaterList, FBXTTransformCreaterResult& OutResult);
	
	// 写入上下文数据
	template<typename T>
	static void WriteContextData(FBXTLRunTimeData& InOutRTData, int32 InFullIndex, FName InDataName, const T& InData)
	{
		if (!InOutRTData.StaticData || InFullIndex < 0)
		{
			return;
		}
		
		InOutRTData.DynamicDatas.Add(FBXTLDynamicDataSearchKey(InFullIndex, InDataName), FInstancedStruct::Make<T>(InData));
	}
	UFUNCTION(BlueprintPure, CustomThunk, meta = (CustomStructureParam = "InValue"))
	static void WriteContextData(UPARAM(ref) FBXTLRunTimeData& InOutRTData, int32 InFullIndex, FName InDataName, int32 InValue);
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
		FName* DataNamePointer = (FName*)Stack.MostRecentPropertyAddress;

		// 更新蓝图虚拟机栈顶指针
		Stack.StepCompiledIn<FProperty>(nullptr);
		// 获取第四个无类型参数的内存地址
		uint8* ValuePointer = Stack.MostRecentPropertyAddress;
		// 获取第四个参数的反射信息
		FStructProperty* ValueProperty = CastField<FStructProperty>(Stack.MostRecentProperty);

		// 停止对蓝图栈的使用
		P_FINISH;

		if (!RTDataPointer || !FullIndexPointer || !DataNamePointer || !ValuePointer || !ValueProperty)
		{
			return;
		}
		
		P_NATIVE_BEGIN;
		FInstancedStruct NewValue;
		NewValue.InitializeAs(ValueProperty->Struct, ValuePointer);
		RTDataPointer->DynamicDatas.Add(FBXTLDynamicDataSearchKey(*FullIndexPointer, *DataNamePointer), NewValue);
		P_NATIVE_END;
	}
	
	// 读取上下文数据
	template<typename T>
	static T* ReadContextData(const FBXTLRunTimeData& InOutRTData, int32 InFullIndex, FName InDataName)
	{
		if (!InOutRTData.StaticData || InFullIndex < 0)
		{
			return nullptr;
		}
		
		if (const FInstancedStruct* FindResult = InOutRTData.DynamicDatas.Find(FBXTLDynamicDataSearchKey(InFullIndex, InDataName)))
		{
			if (FindResult->GetScriptStruct() == TBaseStructure<T>::Get())
			{
				return (T*)FindResult->GetMemory();
			}
		}

		return nullptr;
	}
	UFUNCTION(BlueprintPure, CustomThunk, meta = (CustomStructureParam = "OutValue"))
	static void ReadContextData(const FBXTLRunTimeData& InOutRTData, int32 InFullIndex, FName InDataName, int32& OutValue);
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
		FName* DataNamePointer = (FName*)Stack.MostRecentPropertyAddress;

		// 更新蓝图虚拟机栈顶指针
		Stack.StepCompiledIn<FProperty>(nullptr);
		// 获取第四个无类型参数的内存地址
		uint8* ValuePointer = Stack.MostRecentPropertyAddress;
		// 获取第四个参数的反射信息
		FStructProperty* ValueProperty = CastField<FStructProperty>(Stack.MostRecentProperty);

		// 停止对蓝图栈的使用
		P_FINISH;

		if (!RTDataPointer || !FullIndexPointer || !DataNamePointer || !ValuePointer || !ValueProperty)
		{
			return;
		}
		
		P_NATIVE_BEGIN;
		if (FInstancedStruct* FindResult = RTDataPointer->DynamicDatas.Find(FBXTLDynamicDataSearchKey(*FullIndexPointer, *DataNamePointer)))
		{
			if (ValueProperty->Struct == FindResult->GetScriptStruct())
			{
				ValueProperty->Struct->CopyScriptStruct(ValuePointer, FindResult->GetMemory());
			}
		}
		P_NATIVE_END;
	}

protected:
	static USceneComponent* AnalyzeTransformCreaterCoordinateType(const FBXTTransformCreater& InCreater, bool bUseOrigin, const FBXTLRunTimeData& InRTData, FTransform& OutTransform);
	
#pragma endregion GlobalAPI

};
