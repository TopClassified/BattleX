#include "BXTLEditorToolbar.h"

#include "ToolMenus.h"
#include "Widgets/Layout/SSpacer.h"
#include "WorkflowOrientedApp/SModeWidget.h"

#include "BXTLEditor.h"
#include "BXTLEditorStyle.h"
#include "BXTLEditorCommands.h"   



#define LOCTEXT_NAMESPACE "BXTLEditorToolbar"

void FBXTLEditorToolbar::SetupToolbar(TSharedPtr<FExtender> Extender, TSharedPtr<FBXTLEditor> InEditor)
{
	CachedEditor = InEditor;

	PlayIcon = FSlateIcon(FAppStyle::GetAppStyleSetName(), FName(TEXT("Animation.Forward")));
	PauseIcon = FSlateIcon(FAppStyle::GetAppStyleSetName(), FName(TEXT("Animation.Pause")));
}

/*void FBXTLEditorToolbar::AddModesToolbar(TSharedPtr<FExtender> Extender)
{
	Extender->AddToolBarExtension
	(
		"Asset",
		EExtensionHook::After,
		CachedEditor.Pin()->GetToolkitCommands(),
		FToolBarExtensionDelegate::CreateSP(this, &FBXTLEditorToolbar::FillModesToolbar)
	);
}*/

void FBXTLEditorToolbar::AddTimelineToolbar(TSharedPtr<FExtender> Extender)
{
	Extender->AddToolBarExtension
	(
		"Asset",
		EExtensionHook::After,
		CachedEditor.Pin()->GetToolkitCommands(),
		FToolBarExtensionDelegate::CreateSP(this, &FBXTLEditorToolbar::FillTimelineModeToolbar)
	);
}

/*void FBXTLEditorToolbar::FillModesToolbar(FToolBarBuilder& ToolbarBuilder)
{
	TAttribute<FName> GetActiveMode(CachedEditor.Pin().ToSharedRef(), &FBXTLEditor::GetCurrentMode);
	FOnModeChangeRequested SetActiveMode = FOnModeChangeRequested::CreateSP(CachedEditor.Pin().ToSharedRef(), &FBXTLEditor::SetCurrentMode);

	CachedEditor.Pin()->AddToolbarWidget(SNew(SSpacer).Size(FVector2D(4.0f, 1.0f)));

	CachedEditor.Pin()->AddToolbarWidget
	(
		SNew(SModeWidget, FBXTLEditor::GetLocalizedMode(BXTLEditorModes::Timeline), BXTLEditorModes::Timeline)
		.OnGetActiveMode(GetActiveMode)
		.OnSetActiveMode(SetActiveMode)
		.ToolTipText(LOCTEXT("MainModeButtonTooltip", "Switch to Main Mode"))
	);

	CachedEditor.Pin()->AddToolbarWidget
	(
		SNew(SModeWidget, FBXTLEditor::GetLocalizedMode(BXTLEditorModes::LogicGraph), BXTLEditorModes::LogicGraph)
		.OnGetActiveMode(GetActiveMode)
		.OnSetActiveMode(SetActiveMode)
		.ToolTipText(LOCTEXT("LogicGraphModeButtonTooltip", "Switch to Logic Graph Mode"))
	);

	CachedEditor.Pin()->AddToolbarWidget(SNew(SSpacer).Size(FVector2D(4.0f, 1.0f)));
}*/

void FBXTLEditorToolbar::FillTimelineModeToolbar(FToolBarBuilder& ToolbarBuilder)
{
	const FBXTLEditorCommands& Commands = FBXTLEditorCommands::Get();

	ToolbarBuilder.BeginSection("Timeline");
	{
		ToolbarBuilder.AddToolBarButton
		(
			Commands.Play,
			NAME_None,
			TAttribute<FText>(this, &FBXTLEditorToolbar::GetPlayText),
			TAttribute<FText>(this, &FBXTLEditorToolbar::GetPlayToolTip),
			TAttribute<FSlateIcon>(this, &FBXTLEditorToolbar::GetPlayIcon)
		);
		ToolbarBuilder.AddToolBarButton(Commands.Stop);
		ToolbarBuilder.AddToolBarButton(Commands.Step);
	}
	ToolbarBuilder.EndSection();

	ToolbarBuilder.BeginSection("Other");
	{
		ToolbarBuilder.AddToolBarButton
		(
			Commands.ResetWorld, NAME_None,
			LOCTEXT("Toolbar_ResetWorld", "ResetWorld"),
			LOCTEXT("Toolbar_ResetWorld", "ResetWorld"),
			FSlateIcon(FAppStyle::GetAppStyleSetName(), "ContentBrowser.AssetActions.ReimportAsset")
		);
	}
	ToolbarBuilder.EndSection();

	ToolbarBuilder.BeginSection("Data");
	{
		ToolbarBuilder.AddComboButton
		(
			FUIAction(),
			FOnGetContent::CreateRaw(this, &FBXTLEditorToolbar::GenerateProcessDataMenu),
			LOCTEXT("ProcessData", "ProcessData"),
			LOCTEXT("ProcessData", "ProcessData"),
			FSlateIcon(FBXTLEditorStyle::GetStyleSetName(), TEXT("BXTLEditor.ProcessData"))
		);

		UToolMenu* ProcessDataMenu = UToolMenus::Get()->ExtendMenu("BXTLEditor.BXTLEditorToolBar.ProcessData");
		FToolMenuSection& Section = ProcessDataMenu->AddSection("ProcessData", LOCTEXT("BSAEditor.ProcessData", "ProcessData"));
		Section.AddMenuEntry
		(
			"Refresh All Timeline Asset",
			LOCTEXT("Refresh All Timeline", "Refresh All Timeline"),
			LOCTEXT("Refresh All Timeline", "Refresh All Timeline"),
			FSlateIcon(),
			FUIAction(FExecuteAction::CreateRaw(CachedEditor.Pin().Get(), &FBXTLEditor::RefreshTimelineAssetProperty), FCanExecuteAction())
		);
	}
	ToolbarBuilder.EndSection();
}

TSharedRef<SWidget> FBXTLEditorToolbar::GenerateProcessDataMenu()
{
	TSharedPtr<FUICommandList> InCommandList = nullptr;
	TSharedPtr<FExtender> MenuExtender = nullptr;

	FToolMenuContext MenuContext(InCommandList, MenuExtender);

	return UToolMenus::Get()->GenerateWidget("BXTLEditor.BXTLEditorToolBar.ProcessData", MenuContext);
}

FText FBXTLEditorToolbar::GetPlayText() const
{
	if (CachedEditor.Pin()->IsPlaying())
	{
		return LOCTEXT("TimelineCommand", "Pause");
	}

	return LOCTEXT("TimelineCommand", "Play");
}

FText FBXTLEditorToolbar::GetPlayToolTip() const
{
	if (CachedEditor.Pin()->IsPlaying())
	{
		return LOCTEXT("TimelineCommandToolTip", "Pause Ability.");
	}

	return LOCTEXT("TimelineCommandToolTip", "Play Ability.");
}

FSlateIcon FBXTLEditorToolbar::GetPlayIcon() const
{
	if (CachedEditor.Pin()->IsPlaying())
	{
		return PauseIcon;
	}

	return PlayIcon;
}

#undef LOCTEXT_NAMESPACE