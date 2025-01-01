#pragma once

#include "CoreMinimal.h"
#include "Framework/MultiBox/MultiBoxExtender.h"



class FBXDTEditorToolbar : public TSharedFromThis<FBXDTEditorToolbar>
{
public:
	FBXDTEditorToolbar(TSharedPtr<class FBXDTEditor> InDecisionTreeEditor) : DecisionTreeEditor(InDecisionTreeEditor) {}

	void AddGraphToolbar(TSharedPtr<FExtender> Extender);

private:
	void FillGraphToolbar(FToolBarBuilder& ToolbarBuilder);

protected:
	TWeakPtr<class FBXDTEditor> DecisionTreeEditor;

};