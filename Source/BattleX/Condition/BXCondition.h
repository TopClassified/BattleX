#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BXCondition.generated.h"



UCLASS(Abstract, Blueprintable, EditInlineNew)
class BATTLEX_API UBXCondition : public UObject
{
	GENERATED_BODY()
	
public:
	// 结果取反
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bNot = false;


#if WITH_EDITOR
public:
	// 获取条件的描述
	virtual FString GetDescription();
#endif

};
