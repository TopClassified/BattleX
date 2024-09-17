#pragma once

#include "CoreMinimal.h"
#include "Input/Reply.h"
#include "ITransportControl.h"

#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

class InPreviewProxy;
class FTimelineController;

class BATTLEXEDITOR_API STimelineTransportControls : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(STimelineTransportControls) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, const TSharedRef<FTimelineController>& InEditorTimelineController);

private:
	FReply OnClick_Forward_Step();

	FReply OnClick_Forward();

	EPlaybackMode::Type GetPlaybackMode() const;

private:
	TWeakPtr<FTimelineController> EditorTimelineController;

};