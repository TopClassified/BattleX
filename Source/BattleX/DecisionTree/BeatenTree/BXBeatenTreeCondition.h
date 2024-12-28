#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#include "BXCondition.h"

#include "BXBeatenTreeCondition.generated.h"



UCLASS()
class BATTLEX_API UBXBeatenTreeCondition : public UBXCondition
{
	GENERATED_BODY()

public:
	UBXBeatenTreeCondition();
	
public:
	// 比较硬直状态
	UPROPERTY(EditDefaultsOnly, Meta = (PinHiddenByDefault, InlineEditConditionToggle))
	bool bCheckStunState = false;
	UPROPERTY(EditDefaultsOnly, Meta = (EditCondition = "bCheckStunState"))
	FGameplayTagContainer StunState;

	// 比较攻击类型
	UPROPERTY(EditDefaultsOnly, Meta = (PinHiddenByDefault, InlineEditConditionToggle))
	bool bCheckAttackType = false;
	UPROPERTY(EditDefaultsOnly, Meta = (EditCondition = "bCheckAttackType"))
	FGameplayTagContainer AttackType;

	// 比较攻击力度
	UPROPERTY(EditDefaultsOnly, Meta = (PinHiddenByDefault, InlineEditConditionToggle))
	bool bCheckAttackForce = false;
	UPROPERTY(EditDefaultsOnly, Meta = (EditCondition = "bCheckAttackForce"))
	FGameplayTagContainer AttackForce;

	// 比较相对位置
	UPROPERTY(EditDefaultsOnly, Meta = (PinHiddenByDefault, InlineEditConditionToggle))
	bool bCheckAttackRPos = false;
	UPROPERTY(EditDefaultsOnly, Meta = (EditCondition = "bCheckRelation"))
	FGameplayTagContainer AttackRPos;

	
#if WITH_EDITOR
protected:
	FString GetDescription() override;
#endif

};
