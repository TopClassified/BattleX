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

#pragma endregion GlobalAPI

};