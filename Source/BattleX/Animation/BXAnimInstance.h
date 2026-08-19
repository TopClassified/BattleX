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
	
	// 运行时唯一ID,不应在细节面板被编辑(与Priority的Transient一致)
	UPROPERTY(Transient, BlueprintReadWrite)
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
