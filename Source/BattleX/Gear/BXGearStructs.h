#pragma once

#include "CoreMinimal.h"

#include "BXGearStructs.generated.h"



// 装上装备的相关信息
USTRUCT(BlueprintType)
struct FBXEquipGearInformation
{
	GENERATED_USTRUCT_BODY()

public:


};



// 使用装备的相关信息
USTRUCT(BlueprintType)
struct FBXUsingGearInformation
{
	GENERATED_USTRUCT_BODY()

public:

};



// 挂起装备相关信息
USTRUCT(BlueprintType)
struct FBXSheathGearInformation
{
	GENERATED_USTRUCT_BODY()

public:

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
