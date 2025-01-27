#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimInstance.h"

#include "BXAnimInstance.generated.h"



USTRUCT(BlueprintType)
struct FBXPlayAnimationPermission
{
	GENERATED_USTRUCT_BODY()

public:
	FBXPlayAnimationPermission() {}
	FBXPlayAnimationPermission(int64 InPriority, int64 InPermission): Priority(InPriority), Permission(InPermission) {}
	
public:
	UPROPERTY(Transient, BlueprintReadWrite)
	int64 Priority = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int64 Permission = 0;
	
};



UCLASS(transient, Blueprintable, BlueprintType, Within=SkeletalMeshComponent)
class BATTLEX_API UBXAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	// 获取动画播放权限
	UFUNCTION(BlueprintCallable)
	int64 GetPlayAnimationPermission(const FGameplayTag& InAnimationTag, int32 InPriority);
	
	// 归还行为权限
	UFUNCTION(BlueprintCallable)
	void RevokePlayAnimationPermission(const FGameplayTag& InAnimationTag, int64 InPermission);

public:
	// 动画播放权限
	UPROPERTY(Transient)
	TMap<FGameplayTag, FBXPlayAnimationPermission> PlayAnimationPermissionMap;
	
};
