#pragma once

#include "CoreMinimal.h"

#include "BXGear.h"
#include "BXShapeComponent.h"
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
	// 获取碰撞请求者
	UFUNCTION(BlueprintCallable)
	virtual AActor* GetCollisionRequester(UActorComponent* InComponent);
	
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
struct FBXTPTrackHitBoxContext : public FBXTPCollisionContext
{
	GENERATED_USTRUCT_BODY()

public:
	// 碰撞盒管理组件列表
	UPROPERTY(Transient, BlueprintReadWrite)
	TArray<UBXShapeComponent*> ShapeComponents;

	// 开始时间
	UPROPERTY(Transient, BlueprintReadWrite)
	float StartTime = 0.0f;

	// 当前检测次数
	UPROPERTY(Transient, BlueprintReadWrite)
	int32 CurrentCount = 0;
	
};

UCLASS(BlueprintType, Blueprintable)
class BATTLEX_API UBXTPTrackHitBox : public UBXTPCollision
{
	GENERATED_BODY()
	
public:
	virtual void Start(FBXTLRunTimeData& InOutRTData, FBXTLSectionRTData& InOutRTSData, FBXTLTaskRTData& InOutRTTData) override;

	virtual void Update(FBXTLRunTimeData& InOutRTData, FBXTLSectionRTData& InOutRTSData, FBXTLTaskRTData& InOutRTTData, float InDeltaTime) override;
	
	virtual void End(FBXTLRunTimeData& InOutRTData, FBXTLSectionRTData& InOutRTSData, FBXTLTaskRTData& InOutRTTData, EBXTLFinishReason InReason) override;

protected:
	virtual void CollisionCheck(FBXTLRunTimeData& InOutRTData, FBXTLSectionRTData& InOutRTSData, FBXTLTaskRTData& InOutRTTData, int32 InTargetCheckCount);
	
};






UCLASS(BlueprintType, Blueprintable)
class BATTLEX_API UBXTPTrackWeaponHitBox : public UBXTPTrackHitBox
{
	GENERATED_BODY()
	
public:
	virtual void Start(FBXTLRunTimeData& InOutRTData, FBXTLSectionRTData& InOutRTSData, FBXTLTaskRTData& InOutRTTData) override;

protected:
	virtual AActor* GetCollisionRequester(UActorComponent* InComponent) override;
	
};
