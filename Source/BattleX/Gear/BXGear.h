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
	EBXGearType GearType = EBXGearType::GT_Weapon;

	// 装备数据
	UPROPERTY(Transient, BlueprintReadOnly)
	UBXGearData* GearData = nullptr;

#pragma endregion Important



#pragma region Equip
public:
	// 将要装上
	UFUNCTION(BlueprintCallable, Category = "Equip")
	void PreEquip(UPARAM(ref) FBXEquipGearInformation& EquipInfo);

	// 已经装上
	UFUNCTION(BlueprintCallable, Category = "Equip")
	void PostEquip(UPARAM(ref) FBXEquipGearInformation& EquipInfo);

	// 将要卸下
	UFUNCTION(BlueprintCallable, Category = "Equip")
	void PreUnequip(UPARAM(ref) FBXEquipGearInformation& UnequipInfo);

	// 已经卸下
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
	// 将要使用
	UFUNCTION(BlueprintCallable, Category = "Use")
	void PreUsing(UPARAM(ref) FBXUsingGearInformation& UsingInfo);

	// 已经使用
	UFUNCTION(BlueprintCallable, Category = "Use")
	void PostUsing(UPARAM(ref) FBXUsingGearInformation& UsingInfo);

	// 将要弃用
	UFUNCTION(BlueprintCallable, Category = "Use")
	void PreUnusing(UPARAM(ref) FBXUsingGearInformation& UnusingInfo);

	// 已经弃用
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



#pragma region Sheath
public:
	// 将要入鞘
	UFUNCTION(BlueprintCallable, Category = "Sheath")
	void PreSheath(UPARAM(ref) FBXSheathGearInformation& SheathInfo);

	// 已经入鞘
	UFUNCTION(BlueprintCallable, Category = "Sheath")
	void PostSheath(UPARAM(ref) FBXSheathGearInformation& SheathInfo);

	// 将要出鞘
	UFUNCTION(BlueprintCallable, Category = "Sheath")
	void PreUnsheath(UPARAM(ref) FBXSheathGearInformation& UnsheathInfo);

	// 已经出鞘
	UFUNCTION(BlueprintCallable, Category = "Sheath")
	void PostUnsheath(UPARAM(ref) FBXSheathGearInformation& UnsheathInfo);

protected:
	virtual void InternalPreSheath(FBXSheathGearInformation& SheathInfo);
	UFUNCTION(BlueprintImplementableEvent, Category = "Sheath")
	void ScriptPreSheath(UPARAM(ref) FBXSheathGearInformation& SheathInfo);

	virtual void InternalPostSheath(FBXSheathGearInformation& SheathInfo);
	UFUNCTION(BlueprintImplementableEvent, Category = "Sheath")
	void ScriptPostSheath(UPARAM(ref) FBXSheathGearInformation& SheathInfo);

	virtual void InternalPreUnsheath(FBXSheathGearInformation& UnsheathInfo);
	UFUNCTION(BlueprintImplementableEvent, Category = "Sheath")
	void ScriptPreUnsheath(UPARAM(ref) FBXSheathGearInformation& UnsheathInfo);

	virtual void InternalPostUnsheath(FBXSheathGearInformation& UnsheathInfo);
	UFUNCTION(BlueprintImplementableEvent, Category = "Sheath")
	void ScriptPostUnsheath(UPARAM(ref) FBXSheathGearInformation& UnsheathInfo);

protected:
	// 要执行的函数(默认只执行C++函数)
	UPROPERTY(EditDefaultsOnly, Category = "Sheath", Meta = (Bitmask, BitmaskEnum = "EBXSheathFunction"))
	int32 SheathFunctions = 85;

#pragma endregion Sheath

};