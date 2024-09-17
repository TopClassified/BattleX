#include "BXTLEditorCommands.h"



#define LOCTEXT_NAMESPACE "BXTLEditorCommands"

void FBXTLEditorCommands::RegisterCommands()
{
	UI_COMMAND(Play, "Play", "Play Ability.", EUserInterfaceActionType::Button, FInputChord(EKeys::SpaceBar));
	UI_COMMAND(Stop, "Stop", "Stop Ability.", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(Step, "Step", "Forward Step.", EUserInterfaceActionType::Button, FInputChord(EKeys::D));

	UI_COMMAND(ResetWorld, "Reset World", "Resets the preview World.", EUserInterfaceActionType::Button, FInputChord(EKeys::R));
	UI_COMMAND(ShowCollision, "Show Collision", "Show Collision.", EUserInterfaceActionType::Button, FInputChord(EKeys::C));
}

#undef LOCTEXT_NAMESPACE