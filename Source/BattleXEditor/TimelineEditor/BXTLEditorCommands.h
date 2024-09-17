#pragma once

#include "Styling/ISlateStyle.h"
#include "Framework/Commands/Commands.h"



class FBXTLEditorCommands : public TCommands<FBXTLEditorCommands>
{
public:
	FBXTLEditorCommands() : TCommands<FBXTLEditorCommands>(TEXT("BXTLEditor"), NSLOCTEXT("Contexts", "BXTLEditor", "BXTLEditor"), NAME_None, TEXT("Waiting")) {}

	virtual void RegisterCommands() override;

public:
	TSharedPtr<FUICommandInfo> Play;
	TSharedPtr<FUICommandInfo> Stop;
	TSharedPtr<FUICommandInfo> Step;

	TSharedPtr<FUICommandInfo> ResetWorld;
	TSharedPtr<FUICommandInfo> ShowCollision;
};
