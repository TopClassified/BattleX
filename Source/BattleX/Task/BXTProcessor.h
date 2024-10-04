#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "StructUtils/Public/StructView.h"

#include "BXEnums.h"
#include "BXTLEnums.h"
#include "BXTEnums.h"
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

	/*
	// 解析坐标系创建器
	UFUNCTION(BlueprintCallable, Category = "API")
	static bool AnalyzeTransformCreater(AActor* InTarget, const FBXTLRunTimeData& InRTData, const FBXTTransformCreater& InCreater, FBXTTransformCreaterResult& OutResult);

	// 解析坐标系创建器列表
	UFUNCTION(BlueprintCallable, Category = "API")
	static bool AnalyzeTransformCreaterList(AActor* InTarget, const FBXTLRunTimeData& InRTData, const TArray<FBXTTransformCreater>& InCreaterList, FBXTTransformCreaterResult& OutResult);
	
	// 记录上下文数据
	template<typename T>
	static void RecordContextData(FBXTLRunTimeData& InOutRTData, const FBXTOutputInfo& InInfo, const T& InData)
	{
		
	}
	UFUNCTION(BlueprintPure, CustomThunk, meta = (CustomStructureParam = "InValue"))
	static void RecordContextData(const FBXTOutputInfo& InInfo, UPARAM(ref) FBXTLRunTimeData& InOutRTData, int32 InValue);
	DECLARE_FUNCTION(execRecordContextData)
	{
		Stack.MostRecentProperty = nullptr;

		// 更新蓝图虚拟机栈顶指针
		Stack.StepCompiledIn<FProperty>(NULL);
		// 获取第一个数据的参数的地址
		void* InDataPointer = Stack.MostRecentPropertyAddress;
		// 获取第一个参数的反射信息
		FProperty* InDataProperty = CastField<FProperty>(Stack.MostRecentProperty);

		// 更新蓝图虚拟机栈顶指针
		Stack.StepCompiledIn<FProperty>(NULL);
		// 获取第二个无类型参数的内存地址
		void* OutDataPointer = Stack.MostRecentPropertyAddress;
		// 获取第二个参数的反射信息
		FStructProperty* OutStructProp = CastField<FStructProperty>(Stack.MostRecentProperty);

		// 停止对蓝图栈的使用
		P_FINISH;

		if (!InDataPointer || !OutDataPointer || !InDataProperty || !OutStructProp)
		{
			return;
		}

		FBSNoTypeData* OutDataPtr = (FBSNoTypeData*)OutDataPointer;
		if (FStructProperty* InStructProp = CastField<FStructProperty>(InDataProperty))
		{
			OutDataPtr->DataType = EBSDataType::DT_Struct;

			P_NATIVE_BEGIN;
			InternalSetDataToNoTypeDataStruct(InDataPointer, InStructProp->Struct, InStructProp->GetSize(), OutDataPtr);
			P_NATIVE_END;

			return;
		}
		else
		{
			if (FIntProperty* Int32Prop = CastField<FIntProperty>(InDataProperty))
			{
				OutDataPtr->DataType = EBSDataType::DT_Int32;
			}
			else if (FFloatProperty* FloatProp = CastField<FFloatProperty>(InDataProperty))
			{
				OutDataPtr->DataType = EBSDataType::DT_Float;
			}

			P_NATIVE_BEGIN;
			InternalSetDataToNoTypeDataStruct(InDataPointer, nullptr, InDataProperty->GetSize(), OutDataPtr);
			P_NATIVE_END;

			return;
		}
	}
	
	// 从一个无类型数据结构体中获取数据
	UFUNCTION(BlueprintPure, CustomThunk, meta = (CustomStructureParam = "OutValue"))
	static void GetDataFromNoTypeDataStruct(const FBSNoTypeData& InNoTypeData, int32& OutValue);
	static void InternalGetDataFromNoTypeDataStruct(const FBSNoTypeData& InNoTypeData, void* OutPtr, int32 DataSize);
	DECLARE_FUNCTION(execGetDataFromNoTypeDataStruct)
	{
		Stack.MostRecentProperty = nullptr;

		// 更新蓝图虚拟机栈顶指针
		Stack.StepCompiledIn<FProperty>(NULL);
		// 获取无类型结构体参数的地址
		void* InDataPointer = Stack.MostRecentPropertyAddress;
		// 获取第一个参数的反射信息
		FStructProperty* InStructProp = CastField<FStructProperty>(Stack.MostRecentProperty);

		// 更新蓝图虚拟机栈顶指针
		Stack.StepCompiledIn<FProperty>(NULL);
		// 获取第二个参数的内存地址
		void* OutDataPointer = Stack.MostRecentPropertyAddress;
		// 获取第二个参数的反射信息
		FProperty* OutDataProperty = CastField<FProperty>(Stack.MostRecentProperty);

		// 停止对蓝图栈的使用
		P_FINISH;

		if (!InDataPointer || !OutDataPointer || !InStructProp || !OutDataProperty)
		{
			return;
		}

		// 无类型参数
		FBSNoTypeData* InNoTypeData = (FBSNoTypeData*)InDataPointer;
		// 判断是不是结构体类型
		if (InNoTypeData->DataType == EBSDataType::DT_Struct)
		{
			if (FStructProperty* OutStructProp = CastField<FStructProperty>(OutDataProperty))
			{
				if (InNoTypeData->UnStableSizeData && OutStructProp->Struct == InNoTypeData->UnStableSizeData->GetStruct())
				{
					P_NATIVE_BEGIN;
					InternalGetDataFromNoTypeDataStruct(*InNoTypeData, OutDataPointer, OutDataProperty->GetSize());
					P_NATIVE_END;

					return;
				}
			}
		}
		else if (OutDataProperty)
		{
			P_NATIVE_BEGIN;
			InternalGetDataFromNoTypeDataStruct(*InNoTypeData, OutDataPointer, OutDataProperty->GetSize());
			P_NATIVE_END;

			return;
		}
	}
	
	
protected:
	static USceneComponent* AnalyzeTransformCreaterCoordinateType(EBXTCoordinateType InType, const FBXTLRunTimeData& InRTData, FTransform& OutTransform);
	*/
#pragma endregion GlobalAPI

};
