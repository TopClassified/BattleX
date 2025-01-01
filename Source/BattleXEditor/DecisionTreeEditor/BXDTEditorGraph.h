#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraph.h"

#include "BXDTEditorGraph.generated.h"



UCLASS()
class UBXDTEditorGraph : public UEdGraph
{
	GENERATED_BODY()

public:
	UBXDTEditorGraph();

	virtual ~UBXDTEditorGraph();

	virtual bool Modify(bool bAlwaysMarkDirty = true) override;
	
	virtual void PostEditUndo() override;

protected:
	void Clear();
};
