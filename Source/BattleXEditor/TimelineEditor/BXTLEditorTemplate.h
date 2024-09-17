#pragma once

#include "UObject/Object.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"
#include "Engine/DataAsset.h"

#include "BXTask.h"

#include "BXTLEditorTemplate.generated.h"



USTRUCT()
struct FBXTLTaskGroupInformation
{
	GENERATED_USTRUCT_BODY()

public:
	FBXTLTaskGroupInformation() : Name(NAME_None) {}
	FBXTLTaskGroupInformation(FName GrroupName) : Name(GrroupName) {}

public:
	UPROPERTY(EditAnywhere)
	FName Name = NAME_None;

	UPROPERTY(EditAnywhere)
	TArray<UBXTask*> TaskList;

};



UCLASS(Blueprintable)
class UBXTLTaskGroupTemplate : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TArray<FBXTLTaskGroupInformation> Groups;

};