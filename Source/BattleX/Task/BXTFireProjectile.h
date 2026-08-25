#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "StructUtils/InstancedStruct.h"

#include "Task/BXTask.h"
#include "Projectile/BXProjectileStructs.h"

#include "BXTFireProjectile.generated.h"



// 发射子弹任务(技能时间轴内发射,委托子弹系统统一模拟,发射后瞬时结束)
UCLASS(BlueprintType, Blueprintable)
class BATTLEX_API UBXTFireProjectile : public UBXTask
{
	GENERATED_BODY()

public:
	// 子弹种类标签(对应子弹DataAsset的ProjectileType)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	FGameplayTag ProjectileType;

	// 发射方向类型
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	EBXProjectileFireDirection FireDirectionType = EBXProjectileFireDirection::FD_OwnerForward;

	// 发射位置偏移(所有者局部空间,如枪口位置)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	FVector MuzzleOffset = FVector::ZeroVector;

	// 逐目标各发一发(关闭时仅对首个目标发射,无目标时按所有者朝向发射一发)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	bool bFirePerTarget = true;

	// 发射附带上下文数据(发射时拷贝进每发子弹,命中事件原样带回)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	FInstancedStruct ContextData;
};
