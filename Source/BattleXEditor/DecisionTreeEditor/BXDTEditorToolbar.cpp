#include "BXDTEditorToolbar.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"

#include "BXDTEditor.h"
#include "BXDTEditorCommands.h"



#define LOCTEXT_NAMESPACE "BXDTEditorToolbar"

void FBXDTEditorToolbar::AddGraphToolbar(TSharedPtr<FExtender> Extender)
{
	check(DecisionTreeEditor.IsValid());
	TSharedPtr<FBXDTEditor> DecisionTreeEditorPtr = DecisionTreeEditor.Pin();

	TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
	ToolbarExtender->AddToolBarExtension("Asset", EExtensionHook::After, DecisionTreeEditorPtr->GetToolkitCommands(), FToolBarExtensionDelegate::CreateSP(this, &FBXDTEditorToolbar::FillGraphToolbar));	
	DecisionTreeEditorPtr->AddToolbarExtender(ToolbarExtender);
}

void FBXDTEditorToolbar::FillGraphToolbar(FToolBarBuilder& ToolbarBuilder)
{
	const FBXDTEditorCommands& Commands = FBXDTEditorCommands::Get();

	ToolbarBuilder.BeginSection("Graph");
	ToolbarBuilder.AddToolBarButton(Commands.BackToPreviousGraph);
	ToolbarBuilder.EndSection();
}

#undef LOCTEXT_NAMESPACE
