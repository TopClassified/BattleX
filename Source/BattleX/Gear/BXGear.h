#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "BXGearEnums.h"
#include "BXGearStructs.h"
#include "BXGearData.h"

#include "BXGear.generated.h" 



UCLASS(Abstract, Blueprintable, BlueprintType)
class BATTLEX_API ABXGear : public AActor
{
	GENERATED_BODY()

#pragma region Important
public:
	ABXGear(const FObjectInitializer& ObjectInitializer);

	virtual ~ABXGear();

public:
	// 装备类型
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag GearType = BXGameplayTags::BXGearType_Weapon;

	// 装备数据
	UPROPERTY(Transient, BlueprintReadOnly)
	UBXGearData* GearData = nullptr;

	// 装备拥有者
	UPROPERTY(Transient, BlueprintReadOnly)
	class UBXGearComponent* OwnerComponent = nullptr;

#pragma endregion Important



#pragma region Equip
public:
	// 装上之前
	UFUNCTION(BlueprintCallable, Category = "Equip")
	void PreEquip(UPARAM(ref) FBXEquipGearInformation& EquipInfo);

	// 装上之后
	UFUNCTION(BlueprintCallable, Category = "Equip")
	void PostEquip(UPARAM(ref) FBXEquipGearInformation& EquipInfo);

	// 卸下之前
	UFUNCTION(BlueprintCallable, Category = "Equip")
	void PreUnequip(UPARAM(ref) FBXEquipGearInformation& UnequipInfo);

	// 卸下之后
	UFUNCTION(BlueprintCallable, Category = "Equip")
	void PostUnequip(UPARAM(ref) FBXEquipGearInformation& UnequipInfo);

protected:
	virtual void InternalPreEquip(FBXEquipGearInformation& EquipInfo);
	UFUNCTION(BlueprintImplementableEvent, Category = "Equip")
	void ScriptPreEquip(UPARAM(ref) FBXEquipGearInformation& EquipInfo);

	virtual void InternalPostEquip(FBXEquipGearInformation& EquipInfo);
	UFUNCTION(BlueprintImplementableEvent, Category = "Equip")
	void ScriptPostEquip(UPARAM(ref) FBXEquipGearInformation& EquipInfo);

	virtual void InternalPreUnequip(FBXEquipGearInformation& UnequipInfo);
	UFUNCTION(BlueprintImplementableEvent, Category = "Equip")
	void ScriptPreUnequip(UPARAM(ref) FBXEquipGearInformation& UnequipInfo);

	virtual void InternalPostUnequip(FBXEquipGearInformation& UnequipInfo);
	UFUNCTION(BlueprintImplementableEvent, Category = "Equip")
	void ScriptPostUnequip(UPARAM(ref) FBXEquipGearInformation& UnequipInfo);

protected:
	// 要执行的函数(默认只执行C++函数)
	UPROPERTY(EditDefaultsOnly, Category = "Equip", Meta = (Bitmask, BitmaskEnum = "EBXEquipFunction"))
	int32 EquipFunctions = 85;

#pragma endregion Equip



#pragma region Use
public:
	// 使用之前
	UFUNCTION(BlueprintCallable, Category = "Use")
	void PreUsing(UPARAM(ref) FBXUsingGearInformation& UsingInfo);

	// 使用之后
	UFUNCTION(BlueprintCallable, Category = "Use")
	void PostUsing(UPARAM(ref) FBXUsingGearInformation& UsingInfo);

	// 弃用之前
	UFUNCTION(BlueprintCallable, Category = "Use")
	void PreUnusing(UPARAM(ref) FBXUsingGearInformation& UnusingInfo);

	// 弃用之后
	UFUNCTION(BlueprintCallable, Category = "Use")
	void PostUnusing(UPARAM(ref) FBXUsingGearInformation& UnusingInfo);

protected:
	virtual void InternalPreUsing(FBXUsingGearInformation& UsingInfo);
	UFUNCTION(BlueprintImplementableEvent, Category = "Use")
	void ScriptPreUsing(UPARAM(ref) FBXUsingGearInformation& UsingInfo);

	virtual void InternalPostUsing(FBXUsingGearInformation& UsingInfo);
	UFUNCTION(BlueprintImplementableEvent, Category = "Use")
	void ScriptPostUsing(UPARAM(ref) FBXUsingGearInformation& UsingInfo);

	virtual void InternalPreUnusing(FBXUsingGearInformation& UnusingInfo);
	UFUNCTION(BlueprintImplementableEvent, Category = "Use")
	void ScriptPreUnusing(UPARAM(ref) FBXUsingGearInformation& UnusingInfo);

	virtual void InternalPostUnusing(FBXUsingGearInformation& UnusingInfo);
	UFUNCTION(BlueprintImplementableEvent, Category = "Use")
	void ScriptPostUnusing(UPARAM(ref) FBXUsingGearInformation& UnusingInfo);

protected:
	// 要执行的函数(默认只执行C++函数)
	UPROPERTY(EditDefaultsOnly, Category = "Use", Meta = (Bitmask, BitmaskEnum = "EBXUseFunction"))
	int32 UseFunctions = 85;

#pragma endregion Use



#pragma region State
public:
	// 获取当前的状态
	FGameplayTag GetCurrentState() const;

	// 改变状态
	UFUNCTION(BlueprintCallable, Category = "State")
	void ChangeState(FGameplayTag NewState);

protected:
	virtual void InternalChangeState(FGameplayTag NewState);
	UFUNCTION(BlueprintImplementableEvent, Category = "State")
	void ScriptChangeState(FGameplayTag NewState);

protected:
	// 当前状态
	UPROPERTY(Transient, BlueprintReadWrite)
	FGameplayTag CurrentState = BXGameplayTags::BXGearSlot_Default;

	// 要执行的函数(默认只执行C++函数)
	UPROPERTY(EditDefaultsOnly, Category = "State", Meta = (Bitmask, BitmaskEnum = "EBXChangeStateFunction"))
	int32 ChangeStateFunctions = 1;

#pragma endregion State



#pragma region Attach
public:
	// 挂接装备
	UFUNCTION(BlueprintCallable, Category = "Attach")
	void AttachToSocket();

public:
	// 挂接位置
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FBXGearAttachmentConfig> AttachmentConfigs;

	// 装备挂载者
	UPROPERTY(Transient, BlueprintReadOnly)
	class USkeletalMeshComponent* AttachTarget = nullptr;

#pragma endregion Attach



#pragma region Collision
public:
	UFUNCTION(BlueprintCallable, Category = "Collision")
	virtual void GetHitResults(float InStartTime, FGameplayTagContainer& BoxTags, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, const FBXCFilter& Filter, TArray<FHitResult>& OutResults);

#pragma endregion Collision

};