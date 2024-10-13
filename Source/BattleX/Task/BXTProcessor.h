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
	virtual void Start(FBXTLRunTimeData& InOutRTData, FBXTLSectionRTData& InOutRTSData, FBXTLTaskRTData& InOutRTTData, class UBXTask* InTask) {}
	UFUNCTION(BlueprintImplementableEvent, Category = "Important")
	void ScriptStart(UPARAM(ref) FBXTLRunTimeData& InOutRTData, UPARAM(ref) FBXTLSectionRTData& InOutRTSData, UPARAM(ref) FBXTLTaskRTData& InOutRTTData, class UBXTask* InTask);

	virtual void Update(FBXTLRunTimeData& InOutRTData, FBXTLSectionRTData& InOutRTSData, FBXTLTaskRTData& InOutRTTData, class UBXTask* InTask, float InDeltaTime) {}
	UFUNCTION(BlueprintImplementableEvent, Category = "Important")
	void ScriptUpdate(UPARAM(ref) FBXTLRunTimeData& InOutRTData, UPARAM(ref) FBXTLSectionRTData& InOutRTSData, UPARAM(ref) FBXTLTaskRTData& InOutRTTData, class UBXTask* InTask, float InDeltaTime);

	virtual void End(FBXTLRunTimeData& InOutRTData, FBXTLSectionRTData& InOutRTSData, FBXTLTaskRTData& InOutRTTData, class UBXTask* InTask, EBXTLFinishReason InReason) {}
	UFUNCTION(BlueprintImplementableEvent, Category = "Important")
	void ScriptEnd(UPARAM(ref) FBXTLRunTimeData& InOutRTData, UPARAM(ref) FBXTLSectionRTData& InOutRTSData, UPARAM(ref) FBXTLTaskRTData& InOutRTTData, class UBXTask* InTask, EBXTLFinishReason InReason);

	virtual void ChangeTickRate(FBXTLTaskRTData& InOutRTTData, class UBXTask* InTask, float InRate) {}
	UFUNCTION(BlueprintImplementableEvent, Category = "Important")
	void ScriptChangeTickRate(FBXTLTaskRTData& InOutRTTData, class UBXTask* InTask, float InRate);

protected:
	// 要执行的函数(默认只执行C++函数)
	UPROPERTY(EditDefaultsOnly, Category = "Important", Meta = (Bitmask, BitmaskEnum = "EBXTProcessorFunction"))
	int32 ExecuteFunctions = 85;

#pragma endregion Important



#pragma region API
public:
	UFUNCTION(BlueprintCallable, Category = "API")
	void StartTask(UPARAM(ref) FBXTLRunTimeData& InOutRTData, UPARAM(ref) FBXTLSectionRTData& InOutRTSData, UPARAM(ref) FBXTLTaskRTData& InOutRTTData, class UBXTask* InTask);

	UFUNCTION(BlueprintCallable, Category = "API")
	void UpdateTask(UPARAM(ref) FBXTLRunTimeData& InOutRTData, UPARAM(ref) FBXTLSectionRTData& InOutRTSData, UPARAM(ref) FBXTLTaskRTData& InOutRTTData, class UBXTask* InTask, float InDeltaTime);

	UFUNCTION(BlueprintCallable, Category = "API")
	void EndTask(UPARAM(ref) FBXTLRunTimeData& InOutRTData, UPARAM(ref) FBXTLSectionRTData& InOutRTSData, UPARAM(ref) FBXTLTaskRTData& InOutRTTData, class UBXTask* InTask, EBXTLFinishReason InReason);

	UFUNCTION(BlueprintCallable, Category = "API")
	void ChangeTaskTickRate(UPARAM(ref) FBXTLTaskRTData& InOutRTTData, class UBXTask* InTask, float InRate);
	
#pragma endregion API



#pragma region GlobalAPI
public:
	// 任务是否结束
	UFUNCTION(BlueprintCallable, Category = "API")
	static bool IsTaskCompleted(class UBXTask* InTask, const FBXTLTaskRTData& InTaskData, EBXTLFinishReason& OutReason);

	// 添加被动触发任务
	UFUNCTION(BlueprintCallable, Category = "API")
	static bool AddPendingTask(UPARAM(ref) FBXTLRunTimeData& InOutRTData, UPARAM(ref) FBXTLSectionRTData& InOutRTSData, UPARAM(ref) FBXTLTaskRTData& InOutRTTData, const FName& InEventName);

	// 根据目标掩码获取组件列表
	UFUNCTION(BlueprintCallable, Category = "API")
	static void GetTargetComponentList(const FBXTLRunTimeData& InRTData, class UBXTask* InTask, TArray<USceneComponent*>& OutComponents);
	// 根据目标掩码获取角色列表
	UFUNCTION(BlueprintCallable, Category = "API")
	static void GetTargetActorList(const FBXTLRunTimeData& InRTData, class UBXTask* InTask, TArray<AActor*>& OutActors);
	
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
