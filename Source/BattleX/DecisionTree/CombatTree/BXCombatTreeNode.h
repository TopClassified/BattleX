#pragma once

#include "CoreMinimal.h"

#include "BXSkillAsset.h"
#include "BXDecisionTreeNode.h"

#include "BXCombatTreeNode.generated.h"



UCLASS()
class BATTLEX_API UBXCombatTreeRootNode : public UBXDecisionTreeNode
{
	GENERATED_BODY()

public:
	UBXCombatTreeRootNode();

#if WITH_EDITOR
public:
	void SetNodeTitle(const FText& NewTitle) override;

	virtual bool CanCreateConnection(int32 SelfPinID, UBXDecisionTreeNode* Other, int32 OtherPinID, FText& ErrorMessage) override;

	virtual bool CanUserDelete() const override;
#endif

};






UCLASS()
class BATTLEX_API UBXCombatTreeNode : public UBXDecisionTreeNode
{
	GENERATED_BODY()
	
public:
	UBXCombatTreeNode();

public:
	// 要释放的技能
	UPROPERTY(EditDefaultsOnly, Category = "Common")
	TSoftObjectPtr<UBXSkillAsset> ReleaseSkillAsset = nullptr;

	// 检测技能释放条件
	UPROPERTY(EditDefaultsOnly, Category = "Common")
	bool bCheckReleaseCondition = false;

#if WITH_EDITOR
public:
	void CopyData(UBXDecisionTreeNode* OtherNode) override;

	void RefreshNodeTitle() override;

	void SetNodeTitle(const FText& NewTitle) override;

protected:
	void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

	void PostEditChangeChainProperty(struct FPropertyChangedChainEvent& PropertyChangedEvent) override;
#endif
	
};
