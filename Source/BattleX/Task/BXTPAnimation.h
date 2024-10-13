#pragma once

#include "CoreMinimal.h"
#include "Task/BXTProcessor.h"

#include "BXTPAnimation.generated.h"



USTRUCT(BlueprintType)
struct FBXTPPAContextParameter
{
	GENERATED_USTRUCT_BODY()

public:
	FBXTPPAContextParameter() {}
	FBXTPPAContextParameter(int64 InPermission, int32 InInsID) : Permission(InPermission), MontageInstanceID(InInsID) {}
	
public:
	// 权限ID
	UPROPERTY(Transient, BlueprintReadWrite)
	int64 Permission = -1;

	// 蒙太奇实例ID
	UPROPERTY(Transient, BlueprintReadWrite)
	int32 MontageInstanceID = -1;
	
};

USTRUCT(BlueprintType)
struct FBXTPPlayAnimationContext
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(Transient, BlueprintReadWrite)
	TArray<USkeletalMeshComponent*> TComponents;

	UPROPERTY(Transient, BlueprintReadWrite)
	TArray<FBXTPPAContextParameter> Parameters;
	
};



UCLASS(BlueprintType, Blueprintable)
class BATTLEX_API UBXTPPlayAnimation : public UBXTProcessor
{
	GENERATED_BODY()
	
public:
	virtual void Start(FBXTLRunTimeData& InOutRTData, FBXTLSectionRTData& InOutRTSData, FBXTLTaskRTData& InOutRTTData, class UBXTask* InTask) override;

	virtual void Update(FBXTLRunTimeData& InOutRTData, FBXTLSectionRTData& InOutRTSData, FBXTLTaskRTData& InOutRTTData, class UBXTask* InTask, float InDeltaTime) override;
	
	virtual void End(FBXTLRunTimeData& InOutRTData, FBXTLSectionRTData& InOutRTSData, FBXTLTaskRTData& InOutRTTData, class UBXTask* InTask, EBXTLFinishReason InReason) override;

	virtual void ChangeTickRate(FBXTLTaskRTData& InOutRTTData, class UBXTask* InTask, float InRate) override;
	
};
