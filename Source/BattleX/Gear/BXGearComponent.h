#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "BXGear.h"
#include "BXGearEnums.h"

#include "BXGearComponent.generated.h" 



DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPreEquipGearDelegate, UPARAM(ref) FBXEquipGearInformation&, Information);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPostEquipGearDelegate, UPARAM(ref) FBXEquipGearInformation&, Information);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPreUnequipGearDelegate, UPARAM(ref) FBXEquipGearInformation&, Information);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPostUnequipGearDelegate, UPARAM(ref) FBXEquipGearInformation&, Information);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPreUsingGearDelegate, UPARAM(ref) FBXUsingGearInformation&, Information);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPostUsingGearDelegate, UPARAM(ref) FBXUsingGearInformation&, Information);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPreUnusingGearDelegate, UPARAM(ref) FBXUsingGearInformation&, Information);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPostUnusingGearDelegate, UPARAM(ref) FBXUsingGearInformation&, Information);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FChangeGearStateDelegate, ABXGear*, Gear, FGameplayTag, OldState, FGameplayTag, NewState);



UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BATTLEX_API UBXGearComponent : public UActorComponent
{
	GENERATED_BODY()

#pragma region Important
public:
	UBXGearComponent();

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
	// 每个插槽的最大容量
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FGameplayTag, int32> SlotMaxSize;

	// 装上的装备
	UPROPERTY(Transient, BlueprintReadWrite)
	TMap<FGameplayTag, FBXGears> EquipGears;

	// 正在使用的装备索引
	UPROPERTY(Transient, BlueprintReadWrite)
	TMap<FGameplayTag, int32> UsingGearIndexs;

	UPROPERTY(Transient)
	FBXEquipGearInformation HelpEquipInformation;

	UPROPERTY(Transient)
	FBXUsingGearInformation HelpUsingInformation;

#pragma endregion Important



#pragma region Delegate
public:
	// 装备前
	UPROPERTY(BlueprintAssignable)
	FPreEquipGearDelegate PreEquipGearEvent;

	// 装备后
	UPROPERTY(BlueprintAssignable)
	FPostEquipGearDelegate PostEquipGearEvent;

	// 卸下装备前
	UPROPERTY(BlueprintAssignable)
	FPreUnequipGearDelegate PreUnequipGearEvent;

	// 卸下装备后
	UPROPERTY(BlueprintAssignable)
	FPostUnequipGearDelegate PostUnequipGearEvent;

	// 使用装备前
	UPROPERTY(BlueprintAssignable)
	FPreUsingGearDelegate PreUsingGearEvent;

	// 使用装备后
	UPROPERTY(BlueprintAssignable)
	FPostUsingGearDelegate PostUsingGearEvent;

	// 弃用装备前
	UPROPERTY(BlueprintAssignable)
	FPreUnusingGearDelegate PreUnusingGearEvent;

	// 弃用装备后
	UPROPERTY(BlueprintAssignable)
	FPostUnusingGearDelegate PostUnusingGearEvent;

	// 装备状态发生改变
	UPROPERTY(BlueprintAssignable)
	FChangeGearStateDelegate ChangeGearStateEvent;

#pragma endregion Delegate



#pragma region API
public:
	// 获取所有的装备列表
	UFUNCTION(BlueprintCallable)
	virtual void GetEquipGearList(TArray<ABXGear*>& OutList, bool bNeedReset = true);

	// 获取装备插槽的装备列表
	UFUNCTION(BlueprintCallable)
	virtual void GetEquipGearsBySlot(FGameplayTag InSlot, TArray<ABXGear*>& OutGears);

	// 获取正在使用的装备
	UFUNCTION(BlueprintCallable)
	virtual ABXGear* GetUsingGear(FGameplayTag InSlot);

	// 获取正在使用的装备的插槽
	UFUNCTION(BlueprintCallable)
	virtual FGameplayTag GetUsingGearSlot(ABXGear* InGear);

	// 切换正在使用的装备
	UFUNCTION(BlueprintCallable)
	virtual void SwitchUsingGear(FGameplayTag InSlot, int32 InIndex);

	// 改变装备状态
	UFUNCTION(BlueprintCallable)
	virtual void ChangeUsingGearState(FGameplayTag InSlot, FGameplayTag InNewState);

	// 改变装备插槽
	UFUNCTION(BlueprintCallable)
	virtual void ChangeEquipGear(FGameplayTag InSlot, int32 InIndex, ABXGear* InGear, class USkeletalMeshComponent* AttachParent);

	// 改变装备插槽
	UFUNCTION(BlueprintCallable)
	virtual void ChangeEquipGearByClass(FGameplayTag InSlot, int32 InIndex, UClass* InGearClass, class USkeletalMeshComponent* AttachParent);

	// 改变装备插槽
	UFUNCTION(BlueprintCallable)
	virtual void ChangeEquipGearByData(FGameplayTag InSlot, int32 InIndex, UBXGearData* InGearData, class USkeletalMeshComponent* AttachParent);

	// 卸下所有装备
	UFUNCTION(BlueprintCallable)
	virtual void UnequipAllGears(bool bForceDestroy = false);

#pragma endregion API

};
