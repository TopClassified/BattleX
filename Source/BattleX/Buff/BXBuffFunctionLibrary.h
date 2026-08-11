#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "BXBuffStructs.h"

#include "BXBuffFunctionLibrary.generated.h"



class UBXBuffAsset;



// BUFF函数库(提供BP友好的静态API)
UCLASS()
class BATTLEX_API UBXBuffFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

#pragma region API
public:
	// 施加BUFF
	UFUNCTION(BlueprintCallable, Category = "BattleX|Buff", meta = (WorldContext = "WorldContextObject"))
	static int64 AddBuff(UObject* WorldContextObject, UBXBuffAsset* InAsset, AActor* InOwner, AActor* InInstigator, int32 InLayer = 1, int32 InLevel = 1);

	// 移除BUFF
	UFUNCTION(BlueprintCallable, Category = "BattleX|Buff", meta = (WorldContext = "WorldContextObject"))
	static void RemoveBuff(UObject* WorldContextObject, int64 InBuffID, int32 InLayerDelta = 0);

	// 改变层数
	UFUNCTION(BlueprintCallable, Category = "BattleX|Buff", meta = (WorldContext = "WorldContextObject"))
	static void ChangeBuffLayer(UObject* WorldContextObject, int64 InBuffID, int32 InLayerDelta);

	// 改变等级
	UFUNCTION(BlueprintCallable, Category = "BattleX|Buff", meta = (WorldContext = "WorldContextObject"))
	static void ChangeBuffLevel(UObject* WorldContextObject, int64 InBuffID, int32 InLevelDelta);

	// 刷新生命时长
	UFUNCTION(BlueprintCallable, Category = "BattleX|Buff", meta = (WorldContext = "WorldContextObject"))
	static void RefreshBuffLifetime(UObject* WorldContextObject, int64 InBuffID);

	// 是否拥有指定BUFF
	UFUNCTION(BlueprintCallable, Category = "BattleX|Buff", meta = (WorldContext = "WorldContextObject"))
	static bool HasBuff(UObject* WorldContextObject, AActor* InOwner, UBXBuffAsset* InAsset);

	// 是否拥有指定Tag的BUFF
	UFUNCTION(BlueprintCallable, Category = "BattleX|Buff", meta = (WorldContext = "WorldContextObject"))
	static bool HasBuffByTag(UObject* WorldContextObject, AActor* InOwner, FGameplayTag InTag);

#pragma endregion API
};
