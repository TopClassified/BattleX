#pragma once

#include "CoreMinimal.h"

#include "BXDecisionTreeTemplate.h"

#include "BXCombatTreeTemplate.generated.h"



UCLASS(Abstract)
class BATTLEX_API UBXCombatTreeType : public UBXDecisionTreeType
{
	GENERATED_BODY()

public:
	UBXCombatTreeType();

};






UCLASS()
class BATTLEX_API UBXCombatTreeTemplate : public UBXDecisionTreeTemplate
{
	GENERATED_BODY()
	
public:
	UBXCombatTreeTemplate()
	{
		TreeType = UBXCombatTreeType::StaticClass();
	}
};
