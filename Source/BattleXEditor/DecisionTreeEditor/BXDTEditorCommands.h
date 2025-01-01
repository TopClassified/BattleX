#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "Framework/Commands/UICommandInfo.h"



class FBXDTEditorCommands : public TCommands<FBXDTEditorCommands>
{
public:
	FBXDTEditorCommands() : TCommands<FBXDTEditorCommands>
	(
		TEXT("DecisionTreeEditor"), 
		NSLOCTEXT("Contexts", "DecisionTreeEditor", "DecisionTree Editor"), 
		NAME_None, 
		FAppStyle::GetAppStyleSetName()
	)
	{
		
	}

	virtual void RegisterCommands() override;

public:
	TSharedPtr<FUICommandInfo> BackToPreviousGraph;

};