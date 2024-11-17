#pragma once

#include "CoreMinimal.h"

#include "BXGear.h"
#include "BXTCollision.h"
#include "BXTProcessor.h"

#include "BXTPCollision.generated.h"



USTRUCT(BlueprintType)
struct FBXTPCollisionContext
{
	GENERATED_USTRUCT_BODY()

public:
	// 冷却信息
	UPROPERTY(Transient, BlueprintReadWrite)
	TMap<AActor*, float> CoolDownInformations;
	
};

UCLASS(BlueprintType, Blueprintable)
class BATTLEX_API UBXTPCollision : public UBXTProcessor
{
	GENERATED_BODY()

protected:
	// 使用C++的冷却检查
	UPROPERTY(EditDefaultsOnly)
	bool bUseNativeCheckCoolDown = true;

	// 使用脚本的冷却检查
	UPROPERTY(EditDefaultsOnly)
	bool bUseScriptCheckCoolDown = false;
	
protected:
	// 检查冷却
	UFUNCTION(BlueprintCallable)
	bool CheckCoolDownCompleted(UBXTCollision* InTask, const FHitResult& InHitResult, float InCheckTime, FBXTPCollisionContext& InOutRTData);
	bool CheckCoolDown(UBXTCollision* InTask, const FHitResult& InHitResult, float InCheckTime, FBXTPCollisionContext& InOutRTData);
	UFUNCTION(BlueprintImplementableEvent)
	bool ScriptCheckCoolDown(UBXTCollision* InTask, const FHitResult& InHitResult, float InCheckTime, UPARAM(ref) FBXTPCollisionContext& InOutRTData);

	// 检查角色合法性
	UFUNCTION(BlueprintImplementableEvent)
	bool CheckActor(UBXTCollision* InTask, AActor* InRequester, const FHitResult& InHitResult);

	// 数量限制
	UFUNCTION(BlueprintImplementableEvent)
	void LimitHitResults(FGameplayTag InLimitType, int32 InLimitNumber, UPARAM(ref) TArray<FHitResult>& OutResults);
	
};






USTRUCT(BlueprintType)
struct FBXTPTrackWeaponCollisionContext : public FBXTPCollisionContext
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(Transient, BlueprintReadWrite)
	TArray<ABXGear*> Gears;

	UPROPERTY(Transient, BlueprintReadWrite)
	float StartCheckTime = 0.0f;
	
	UPROPERTY(Transient, BlueprintReadWrite)
	float UpdateInterval = 0.0f;
	
	UPROPERTY(Transient, BlueprintReadWrite)
	int32 CurrentCount = 0;
	
};

UCLASS(BlueprintType, Blueprintable)
class BATTLEX_API UBXTPTrackWeaponCollision : public UBXTPCollision
{
	GENERATED_BODY()
	
public:
	virtual void Start(FBXTLRunTimeData& InOutRTData, FBXTLSectionRTData& InOutRTSData, FBXTLTaskRTData& InOutRTTData) override;

	virtual void Update(FBXTLRunTimeData& InOutRTData, FBXTLSectionRTData& InOutRTSData, FBXTLTaskRTData& InOutRTTData, float InDeltaTime) override;
	
	virtual void End(FBXTLRunTimeData& InOutRTData, FBXTLSectionRTData& InOutRTSData, FBXTLTaskRTData& InOutRTTData, EBXTLFinishReason InReason) override;

protected:
	virtual void CollisionCheck(const TArray<ABXGear*>& InGears, float InStartTime, float InEndTime, class UBXTTrackWeaponCollision* InTask, TArray<FHitResult>& OutHitResults);
	
};
