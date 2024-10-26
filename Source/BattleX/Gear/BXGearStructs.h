#pragma once

#include "CoreMinimal.h"
#include "BXGameplayTags.h"
#include "GameplayTagContainer.h"

#include "BXGearStructs.generated.h"



// 装备挂接配置
USTRUCT(BlueprintType)
struct FBXGearAttachmentConfig
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag Slot = BXGameplayTags::BXGearSlot_Default;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag State = BXGameplayTags::BXGearState_Default;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<class USkeletalMesh> Mesh = nullptr;

	UPROPERTY(EditDefaultsOnly)
	FName Socket = NAME_None;

	UPROPERTY(EditDefaultsOnly)
	FTransform Relation = FTransform::Identity;

};



// 装上装备的相关信息
USTRUCT(BlueprintType)
struct FBXEquipGearInformation
{
	GENERATED_USTRUCT_BODY()

public:
	// 装备
	UPROPERTY(Transient, BlueprintReadWrite)
	AActor* Gear = nullptr;

	// 挂载到拥有者的哪个模型组件上
	UPROPERTY(Transient, BlueprintReadWrite)
	class USkeletalMeshComponent* AttachParent = nullptr;

	// 拥有者的装备组件
	UPROPERTY(Transient, BlueprintReadWrite)
	class UBXGearComponent* OwnerComponent = nullptr;

public:
	void Reset()
	{
		Gear = nullptr;
		AttachParent = nullptr;
		OwnerComponent = nullptr;
	}

};



// 使用装备的相关信息
USTRUCT(BlueprintType)
struct FBXUsingGearInformation
{
	GENERATED_USTRUCT_BODY()

public:
	// 装备
	UPROPERTY(Transient, BlueprintReadWrite)
	AActor* Gear = nullptr;

};



// 装备碰撞盒记录信息
USTRUCT(BlueprintType)
struct FBXGHitBoxRecord
{
	GENERATED_USTRUCT_BODY()

public:
	// 方位
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform WTransform = FTransform::Identity;

	// 游戏内虚拟时间
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GameTime = 0.0f;

};

USTRUCT(BlueprintType)
struct FBXGHitBoxRecords
{
	GENERATED_USTRUCT_BODY()

public:
	// 位置
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FBXGHitBoxRecord> List;

};



// 装备列表
USTRUCT(BlueprintType)
struct FBXGears
{
	GENERATED_USTRUCT_BODY()

public:
	FBXGears() {}
	FBXGears(int32 InSize)
	{
		List.Empty(InSize);
		List.AddZeroed(InSize);
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<class ABXGear*> List;

};
