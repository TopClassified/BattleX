#pragma once

#include "CoreMinimal.h"

#include "BXDecisionTreeEdge.h"

#include "BXBeatenTreeEdge.generated.h"



UCLASS()
class BATTLEX_API UBXBeatenTreeEdge : public UBXDecisionTreeEdge
{
	GENERATED_BODY()
	
public:
	UBXBeatenTreeEdge();
	

#if WITH_EDITORONLY_DATA
public:
	UPROPERTY(VisibleDefaultsOnly)
	int32 EdgeShortestDepth = 100000;
#endif

#if WITH_EDITOR
public:
	void ConstructEdge() override;

	void SetEdgeTitle(const FText& NewTitle) override {}

	void CheckConditionConfig();

protected:
	void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

};
