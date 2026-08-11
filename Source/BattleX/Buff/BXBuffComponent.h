#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "BXBuffStructs.h"

#include "BXBuffComponent.generated.h"



class UBXBuffAsset;



// BUFF组件(挂在Actor上,提供BP友好API,转发到UBXBuffManager)
UCLASS(ClassGroup = "BattleX", meta = (BlueprintSpawnableComponent))
class BATTLEX_API UBXBuffComponent : public UActorComponent
{
	GENERATED_BODY()

#pragma region API
public:
	// 施加BUFF
	UFUNCTION(BlueprintCallable, Category = "BattleX|Buff")
	int64 AddBuff(UBXBuffAsset* InAsset, AActor* InInstigator = nullptr, int32 InLayer = 1, int32 InLevel = 1);

	// 移除BUFF
	UFUNCTION(BlueprintCallable, Category = "BattleX|Buff")
	void RemoveBuff(int64 InBuffID, int32 InLayerDelta = 0);

	// 改变层数
	UFUNCTION(BlueprintCallable, Category = "BattleX|Buff")
	void ChangeBuffLayer(int64 InBuffID, int32 InLayerDelta);

	// 改变等级
	UFUNCTION(BlueprintCallable, Category = "BattleX|Buff")
	void ChangeBuffLevel(int64 InBuffID, int32 InLevelDelta);

	// 刷新生命时长
	UFUNCTION(BlueprintCallable, Category = "BattleX|Buff")
	void RefreshBuffLifetime(int64 InBuffID);

	// 是否拥有指定BUFF
	UFUNCTION(BlueprintCallable, Category = "BattleX|Buff")
	bool HasBuff(UBXBuffAsset* InAsset) const;

	// 是否拥有指定Tag的BUFF
	UFUNCTION(BlueprintCallable, Category = "BattleX|Buff")
	bool HasBuffByTag(FGameplayTag InTag) const;

#pragma endregion API



#pragma region Lifecycle
public:
	// 组件销毁时移除所有BUFF
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

#pragma endregion Lifecycle



#pragma region Data
protected:
	// 本组件持有的BUFF实例ID列表
	UPROPERTY(Transient)
	TArray<int64> OwnedBuffIDs;

#pragma endregion Data
};
