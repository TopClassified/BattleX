#pragma once

#include "CoreMinimal.h"

#include "BXSkillAsset.h"
#include "BXDecisionTreeEdge.h"

#include "BXCombatTreeEdge.generated.h"



UCLASS()
class BATTLEX_API UBXCombatTreeEdge : public UBXDecisionTreeEdge
{
	GENERATED_BODY()

public:
	// 检查哪些技能的连招窗口
	UPROPERTY(EditDefaultsOnly)
	TArray<TSoftObjectPtr<UBXSkillAsset>> ComboWindows;

#if WITH_EDITOR
public:
	void CopyData(UBXDecisionTreeEdge* OtherNode) override;

	void RefreshEdgeTitle() override;

	void SetEdgeTitle(const FText& NewTitle) override;

	void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

};
