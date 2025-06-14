#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#include "BXBeatenTreeNode.h"
#include "BXGameplayTags.h"
#include "BXDecisionTreeTemplate.h"

#include "BXBeatenTreeTemplate.generated.h"



USTRUCT(BlueprintType)
struct BATTLEX_API FBXBeatenTreeCondParam
{
	GENERATED_USTRUCT_BODY()
	
public:
	// 硬直状态
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag StunState = BXGameplayTags::BXStunState_Normal;

	// 攻击类型
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag AttackType = BXGameplayTags::BXAttackType_Light;

	// 攻击力度
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag AttackForce = BXGameplayTags::BXAttackForce_0;

	// 攻击者相对位置
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag AttackRPos = BXGameplayTags::BXAttackRPos_Front;

};






UCLASS(Abstract)
class BATTLEX_API UBXBeatenTreeType : public UBXDecisionTreeType
{
	GENERATED_BODY()

public:
	UBXBeatenTreeType();

};






UCLASS()
class BATTLEX_API UBXBeatenTreeTemplate : public UBXDecisionTreeTemplate
{
	GENERATED_BODY()

public:
	UBXBeatenTreeTemplate()
	{
		TreeType = UBXBeatenTreeType::StaticClass();
	}	

private:
	TArray<UBXBeatenTreeNode*> VisitedNodes;

	
#if WITH_EDITOR
public:
	void RefreshLogicMessage() override;

	void RefreshNodeMessage(UBXBeatenTreeNode* InCurNode, int32 CurrentDepth);

private:
	int32 TotalSkillNum = 0;
#endif

};
