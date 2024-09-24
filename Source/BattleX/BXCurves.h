#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Curves/CurveVector.h"
#include "Curves/CurveLinearColor.h"

#include "BXCurves.generated.h" 



USTRUCT(BlueprintType)
struct BATTLEX_API FBXRemapFloatCurve
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	bool bNeedRemap = true;

	UPROPERTY(EditDefaultsOnly)
	FRuntimeFloatCurve Curve;
};



USTRUCT(BlueprintType)
struct BATTLEX_API FBXRemapVectorCurve
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	bool bNeedRemap = true;

	UPROPERTY(EditDefaultsOnly)
	FRuntimeVectorCurve Curve;
};



USTRUCT(BlueprintType)
struct BATTLEX_API FBXRemapColorCurve
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	bool bNeedRemap = true;

	UPROPERTY(EditDefaultsOnly)
	FRuntimeCurveLinearColor Curve;
};
