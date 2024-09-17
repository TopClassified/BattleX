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
	// ÿ����۵��������
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EBXGearSlot, int32> SlotMaxSize;

	// װ�ϵ�װ��
	UPROPERTY(Transient, BlueprintReadWrite)
	TMap<EBXGearSlot, FBXGears> EquipGears;

	// ����ʹ�õ�װ������
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
	// װ��ǰ
	UPROPERTY(BlueprintAssignable)
	FPreEquipGearDelegate OnPreEquipGear;

	// װ����
	UPROPERTY(BlueprintAssignable)
	FPostEquipGearDelegate OnPostEquipGear;

	// ж��װ��ǰ
	UPROPERTY(BlueprintAssignable)
	FPreUnequipGearDelegate OnPreUnequipGear;

	// ж��װ����
	UPROPERTY(BlueprintAssignable)
	FPostUnequipGearDelegate OnPostUnequipGear;

	// ʹ��װ��ǰ
	UPROPERTY(BlueprintAssignable)
	FPreUsingGearDelegate OnPreUsingGear;

	// ʹ��װ����
	UPROPERTY(BlueprintAssignable)
	FPostUsingGearDelegate OnPostUsingGear;

	// ����װ��ǰ
	UPROPERTY(BlueprintAssignable)
	FPreUnusingGearDelegate OnPreUnusingGear;

	// ����װ����
	UPROPERTY(BlueprintAssignable)
	FPostUnusingGearDelegate OnPostUnusingGear;

	// װ������ǰ
	UPROPERTY(BlueprintAssignable)
	FPreSheathGearDelegate OnPreSheathGear;

	// װ�������
	UPROPERTY(BlueprintAssignable)
	FPostSheathGearDelegate OnPostSheathGear;

	// װ������ǰ
	UPROPERTY(BlueprintAssignable)
	FPreUnsheathGearDelegate OnPreUnsheathGear;

	// װ�����Ѻ�
	UPROPERTY(BlueprintAssignable)
	FPostUnsheathGearDelegate OnPostUnsheathGear;

#pragma endregion Delegate



#pragma region API
public:
	// ��ȡ����ʹ�õ�װ��
	UFUNCTION(BlueprintCallable)
	virtual ABXGear* GetUsingGear(EBXGearSlot InSlot);

	// �л�����ʹ�õ�װ��
	UFUNCTION(BlueprintCallable)
	virtual void SwitchUsingGear(EBXGearSlot InSlot, int32 InIndex);

	// ����װ��
	UFUNCTION(BlueprintCallable)
	virtual void SheathGear(EBXGearSlot InSlot, bool bSheath);

	// ��ȡװ����۵�װ���б�
	UFUNCTION(BlueprintCallable)
	virtual void GetEquipGears(EBXGearSlot InSlot, TArray<ABXGear*>& OutGears);

	// �ı�װ�����
	UFUNCTION(BlueprintCallable)
	virtual void ChangeEquipGear(EBXGearSlot InSlot, int32 InIndex, ABXGear* InGear);

	// �ı�װ�����
	UFUNCTION(BlueprintCallable)
	virtual void ChangeEquipGearByClass(EBXGearSlot InSlot, int32 InIndex, UClass* InGearClass);

	// �ı�װ�����
	UFUNCTION(BlueprintCallable)
	virtual void ChangeEquipGearByData(EBXGearSlot InSlot, int32 InIndex, UBXGearData* InGearData);

#pragma endregion API

};
