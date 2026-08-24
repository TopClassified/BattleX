#pragma once

#include "CoreMinimal.h"

#include "BXTProcessor.h"

#include "BXTPFireProjectile.generated.h"



// 发射子弹任务的运行时上下文
USTRUCT(BlueprintType)
struct FBXTPFireProjectileContext
{
	GENERATED_USTRUCT_BODY()

public:
	// 已发射的子弹ID列表(逐目标发射时按目标顺序追加)
	UPROPERTY(Transient, BlueprintReadWrite)
	TArray<int64> FiredProjectileIDs;

};

// 发射子弹处理器(权威端权威发射,自主客户端预测发射,模拟端静默跳过由组播建立本地模拟)
UCLASS(BlueprintType, Blueprintable)
class BATTLEX_API UBXTPFireProjectile : public UBXTProcessor
{
	GENERATED_BODY()

public:
	virtual void Start(FBXTLRunTimeData& InOutRTData, FBXTLSectionRTData& InOutRTSData, FBXTLTaskRTData& InOutRTTData) override;

protected:
	// 构建单发发射方向(指向目标模式且目标有效时指向目标,否则所有者朝向)
	FVector BuildFireDirection(const FTransform& InOwnerTransform, const FVector& InStartLocation, AActor* InTarget, EBXProjectileFireDirection InDirectionType) const;
};
