#include "STimelineTransportControls.h"
#include "EditorWidgetsModule.h"
#include "Modules/ModuleManager.h"
#include "TimelineController.h" 



void STimelineTransportControls::Construct(const FArguments& InArgs, const TSharedRef<FTimelineController>& InEditorTimelineController)
{
	EditorTimelineController = InEditorTimelineController;

	FEditorWidgetsModule& EditorWidgetsModule = FModuleManager::LoadModuleChecked<FEditorWidgetsModule>("EditorWidgets");

	FTransportControlArgs TransportControlArgs;
	TransportControlArgs.OnForwardPlay = FOnClicked::CreateSP(this, &STimelineTransportControls::OnClick_Forward);
	TransportControlArgs.OnForwardStep = FOnClicked::CreateSP(this, &STimelineTransportControls::OnClick_Forward_Step);
	TransportControlArgs.OnGetPlaybackMode = FOnGetPlaybackMode::CreateSP(this, &STimelineTransportControls::GetPlaybackMode);

	ChildSlot
	[
		EditorWidgetsModule.CreateTransportControl(TransportControlArgs)
	];
}

FReply STimelineTransportControls::OnClick_Forward_Step()
{
	return FReply::Handled();
}

FReply STimelineTransportControls::OnClick_Forward()
{
	return FReply::Handled();
}

EPlaybackMode::Type STimelineTransportControls::GetPlaybackMode() const
{
	return EPlaybackMode::Stopped;
}
