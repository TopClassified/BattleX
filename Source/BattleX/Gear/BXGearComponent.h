#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "BXGear.h"

#include "BXGearComponent.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPreEquipGearDelegate, ABXGear*, Gear, UPARAM(ref) FBXEquipGearInformation&, Information);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPostEquipGearDelegate, ABXGear*, Gear, UPARAM(ref) FBXEquipGearInformation&, Information);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPreUnequipGearDelegate, ABXGear*, Gear, UPARAM(ref) FBXEquipGearInformation&, Information);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPostUnequipGearDelegate, ABXGear*, Gear, UPARAM(ref) FBXEquipGearInformation&, Information);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPreUsingGearDelegate, ABXGear*, Gear, UPARAM(ref) FBXUsingGearInformation&, Information);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPostUsingGearDelegate, ABXGear*, Gear, UPARAM(ref) FBXUsingGearInformation&, Information);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPreUnusingGearDelegate, ABXGear*, Gear, UPARAM(ref) FBXUsingGearInformation&, Information);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPostUnusingGearDelegate, ABXGear*, Gear, UPARAM(ref) FBXUsingGearInformation&, Information);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPreSheathGearDelegate, ABXGear*, Gear, UPARAM(ref) FBXSheathGearInformation&, Information);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPostSheathGearDelegate, ABXGear*, Gear, UPARAM(ref) FBXSheathGearInformation&, Information);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPreUnsheathGearDelegate, ABXGear*, Gear, UPARAM(ref) FBXSheathGearInformation&, Information);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPostUnsheathGearDelegate, ABXGear*, Gear, UPARAM(ref) FBXSheathGearInformation&, Information);



UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BATTLEX_API UBXGearComponent : public UActorComponent
{
	GENERATED_BODY()

#pragma region Important
public:
	UBXGearComponent();

	virtual void BeginPlay() override;

public:
	// 每个插槽的最大容量
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EBXGearSlot, int32> SlotMaxSize;

	// 装上的装备
	UPROPERTY(Transient, BlueprintReadWrite)
	TMap<EBXGearSlot, FBXGears> EquipGears;

	// 正在使用的装备索引
	UPROPERTY(Transient, BlueprintReadWrite)
	TMap<EBXGearSlot, int32> UsingGearIndexs;

	UPROPERTY(Transient)
	FBXEquipGearInformation HelpEquipInformation;

	UPROPERTY(Transient)
	FBXUsingGearInformation HelpUsingInformation;

	UPROPERTY(Transient)
	FBXSheathGearInformation HelpSheathInformation;

#pragma endregion Important



#pragma region Delegate
public:
	// 装备前
	UPROPERTY(BlueprintAssignable)
	FPreEquipGearDelegate OnPreEquipGear;

	// 装备后
	UPROPERTY(BlueprintAssignable)
	FPostEquipGearDelegate OnPostEquipGear;

	// 卸下装备前
	UPROPERTY(BlueprintAssignable)
	FPreUnequipGearDelegate OnPreUnequipGear;

	// 卸下装备后
	UPROPERTY(BlueprintAssignable)
	FPostUnequipGearDelegate OnPostUnequipGear;

	// 使用装备前
	UPROPERTY(BlueprintAssignable)
	FPreUsingGearDelegate OnPreUsingGear;

	// 使用装备后
	UPROPERTY(BlueprintAssignable)
	FPostUsingGearDelegate OnPostUsingGear;

	// 弃用装备前
	UPROPERTY(BlueprintAssignable)
	FPreUnusingGearDelegate OnPreUnusingGear;

	// 弃用装备后
	UPROPERTY(BlueprintAssignable)
	FPostUnusingGearDelegate OnPostUnusingGear;

	// 装备挂起前
	UPROPERTY(BlueprintAssignable)
	FPreSheathGearDelegate OnPreSheathGear;

	// 装备挂起后
	UPROPERTY(BlueprintAssignable)
	FPostSheathGearDelegate OnPostSheathGear;

	// 装备唤醒前
	UPROPERTY(BlueprintAssignable)
	FPreUnsheathGearDelegate OnPreUnsheathGear;

	// 装备唤醒后
	UPROPERTY(BlueprintAssignable)
	FPostUnsheathGearDelegate OnPostUnsheathGear;

#pragma endregion Delegate



#pragma region API
public:
	// 获取正在使用的装备
	UFUNCTION(BlueprintCallable)
	virtual ABXGear* GetUsingGear(EBXGearSlot InSlot);

	// 切换正在使用的装备
	UFUNCTION(BlueprintCallable)
	virtual void SwitchUsingGear(EBXGearSlot InSlot, int32 InIndex);

	// 收起装备
	UFUNCTION(BlueprintCallable)
	virtual void SheathGear(EBXGearSlot InSlot, bool bSheath);

	// 获取装备插槽的装备列表
	UFUNCTION(BlueprintCallable)
	virtual void GetEquipGears(EBXGearSlot InSlot, TArray<ABXGear*>& OutGears);

	// 改变装备插槽
	UFUNCTION(BlueprintCallable)
	virtual void ChangeEquipGear(EBXGearSlot InSlot, int32 InIndex, ABXGear* InGear);

	// 改变装备插槽
	UFUNCTION(BlueprintCallable)
	virtual void ChangeEquipGearByClass(EBXGearSlot InSlot, int32 InIndex, UClass* InGearClass);

	// 改变装备插槽
	UFUNCTION(BlueprintCallable)
	virtual void ChangeEquipGearByData(EBXGearSlot InSlot, int32 InIndex, UBXGearData* InGearData);

#pragma endregion API

};
