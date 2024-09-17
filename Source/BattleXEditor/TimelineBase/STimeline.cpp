#include "STimeline.h"
#include "MovieSceneFwd.h"
#include "RHIDefinitions.h"
#include "ScopedTransaction.h"
#include "Modules/ModuleManager.h"
#include "Framework/Application/SlateApplication.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"

#include "SOutliner.h"
#include "STrackArea.h"
#include "STimelineOverlay.h"
#include "STimelineSplitterOverlay.h"
#include "STimelineTransportControls.h"
#include "TimelineController.h"

#include "ISequencerWidgetsModule.h"
#include "FrameNumberNumericInterface.h"
#include "IPersonaPreviewScene.h"
#include "Preferences/PersonaOptions.h"
#include "EditorStyleSet.h"
#include "Styling/ISlateStyle.h"
#include "Fonts/FontMeasure.h"
#include "EditorWidgetsModule.h"

#include "Widgets/SWidget.h"
#include "Widgets/Input/SSpinBox.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SSpacer.h"
#include "Widgets/Layout/SGridPanel.h"
#include "Widgets/Input/SSearchBox.h"
#include "Widgets/Layout/SSplitter.h"
#include "Widgets/Input/STextEntryPopup.h"
#include "Widgets/Layout/SScrollBorder.h"



#define LOCTEXT_NAMESPACE "STimeline"

const FName STimeline::InSnapName = TEXT("TimelineSnap");

void STimeline::Construct(const FArguments& InArgs, const TSharedRef<FTimelineController>& InEditorTimelineController)
{
	TWeakPtr<FTimelineController> WeakModel = InEditorTimelineController;

	TimelineController = InEditorTimelineController;
	OnReceivedFocus = InArgs._OnReceivedFocus;

	int32 TickResolutionValue = InEditorTimelineController->GetTickResolution();
	int32 SequenceFrameRate = FMath::RoundToInt(InEditorTimelineController->GetFrameRate());

	ViewRange = MakeAttributeLambda
	(
		[WeakModel]() 
		{ 
			return WeakModel.IsValid() ? WeakModel.Pin()->GetViewRange() : FAnimatedRange(0.0, 0.0); 
		}
	);

	TAttribute<EFrameNumberDisplayFormats> DisplayFormat = MakeAttributeLambda
	(
		[]()
		{
			return GetDefault<UPersonaOptions>()->TimelineDisplayFormat;
		}
	);

	TAttribute<EFrameNumberDisplayFormats> DisplayFormatSecondary = MakeAttributeLambda
	(
		[]()
		{
			return GetDefault<UPersonaOptions>()->TimelineDisplayFormat == EFrameNumberDisplayFormats::Frames ? EFrameNumberDisplayFormats::Seconds : EFrameNumberDisplayFormats::Frames;
		}
	);

	TAttribute<FFrameRate> TickResolution = MakeAttributeLambda
	(
		[TickResolutionValue]()
		{
			return FFrameRate(TickResolutionValue, 1);
		}
	);

	TAttribute<FFrameRate> DisplayRate = MakeAttributeLambda
	(
		[SequenceFrameRate]()
		{
			return FFrameRate(SequenceFrameRate, 1);
		}
	);

	// Create our numeric type interface so we can pass it to the time slider below.
	NumericTypeInterface = MakeShareable(new FFrameNumberInterface(DisplayFormat, 0, TickResolution, DisplayRate));
	SecondaryNumericTypeInterface = MakeShareable(new FFrameNumberInterface(DisplayFormatSecondary, 0, TickResolution, DisplayRate));

	FTimeSliderArgs TimeSliderArgs;
	{
		TimeSliderArgs.ScrubPosition = MakeAttributeLambda([WeakModel](){ return WeakModel.IsValid() ? WeakModel.Pin()->GetScrubPosition() : FFrameTime(0); });
		TimeSliderArgs.ViewRange = ViewRange;
		TimeSliderArgs.PlaybackRange = MakeAttributeLambda([WeakModel](){ return WeakModel.IsValid() ? WeakModel.Pin()->GetPlaybackRange() : TRange<FFrameNumber>(0, 0); });
		TimeSliderArgs.ClampRange = MakeAttributeLambda([WeakModel](){ return WeakModel.IsValid() ? WeakModel.Pin()->GetWorkingRange() : FAnimatedRange(0.0, 0.0); });
		TimeSliderArgs.DisplayRate = DisplayRate;
		TimeSliderArgs.TickResolution =  MakeAttributeLambda([TickResolutionValue]() { return FFrameRate(TickResolutionValue, 1); });
		TimeSliderArgs.OnViewRangeChanged = FOnViewRangeChanged::CreateSP(&InEditorTimelineController.Get(), &FTimelineController::HandleViewRangeChanged);
		TimeSliderArgs.OnClampRangeChanged = FOnTimeRangeChanged::CreateSP(&InEditorTimelineController.Get(), &FTimelineController::HandleWorkingRangeChanged);
		TimeSliderArgs.IsPlaybackRangeLocked = true;
		TimeSliderArgs.PlaybackStatus = EMovieScenePlayerStatus::Stopped;
		TimeSliderArgs.NumericTypeInterface = NumericTypeInterface;
		TimeSliderArgs.OnScrubPositionChanged = FOnScrubPositionChanged::CreateSP(this, &STimeline::HandleScrubPositionChanged);
	}

	TimeSliderController = MakeShareable(new FTimeSliderController(TimeSliderArgs, InEditorTimelineController, SharedThis(this), SecondaryNumericTypeInterface));
	TSharedRef<FTimeSliderController> TimeSliderControllerRef = TimeSliderController.ToSharedRef();

	// Create the top slider
	const bool bMirrorLabels = false;
	ISequencerWidgetsModule& SequencerWidgets = FModuleManager::Get().LoadModuleChecked<ISequencerWidgetsModule>("SequencerWidgets");
	TopTimeSlider = SequencerWidgets.CreateTimeSlider(TimeSliderControllerRef, bMirrorLabels);

	// Create bottom time range slider
	TSharedRef<ITimeSlider> BottomTimeRange = SequencerWidgets.CreateTimeRange
	(
		FTimeRangeArgs
		(
			EShowRange::ViewRange | EShowRange::WorkingRange | EShowRange::PlaybackRange,
			EShowRange::ViewRange | EShowRange::WorkingRange,
			TimeSliderControllerRef,
			EVisibility::Visible,
			NumericTypeInterface.ToSharedRef()
		),
		SequencerWidgets.CreateTimeRangeSlider(TimeSliderControllerRef)
	);

	TSharedRef<SScrollBar> ScrollBar = SNew(SScrollBar).Thickness(FVector2D(5.0f, 5.0f));

	InEditorTimelineController->RefreshTracks();

	TrackArea = SNew(STrackArea, InEditorTimelineController, TimeSliderControllerRef);
	Outliner = SNew(SOutliner, InEditorTimelineController, TrackArea.ToSharedRef()).ExternalScrollbar(ScrollBar).Clipping(EWidgetClipping::ClipToBounds).FilterText_Lambda([this](){ return FilterText; });

	TrackArea->SetOutliner(Outliner);

	ColumnFillCoefficients[0] = 0.2f;
	ColumnFillCoefficients[1] = 0.8f;

	TAttribute<float> FillCoefficient_0, FillCoefficient_1;
	{
		FillCoefficient_0.Bind(TAttribute<float>::FGetter::CreateSP(this, &STimeline::GetColumnFillCoefficient, 0));
		FillCoefficient_1.Bind(TAttribute<float>::FGetter::CreateSP(this, &STimeline::GetColumnFillCoefficient, 1));
	}

	const int32 Column0 = 0, Column1 = 1;
	const int32 Row0 = 0, Row1 = 1, Row2 = 2, Row3 = 3, Row4 = 4;

	const float CommonPadding = 3.f;
	const FMargin ResizeBarPadding(4.f, 0, 0, 0);

	ChildSlot
	[
		SNew(SOverlay)
		+SOverlay::Slot()
		[
			SNew(SVerticalBox)
			+SVerticalBox::Slot()
			[
				SNew(SOverlay)
				+SOverlay::Slot()
				[
					SNew(SGridPanel)
					.FillRow(1, 1.0f)
					.FillColumn(0, FillCoefficient_0)
					.FillColumn(1, FillCoefficient_1)
	
					// 时间轴主要部分
					+SGridPanel::Slot(Column0, Row1, SGridPanel::Layer(10))
					.ColumnSpan(2)
					[
						SNew(SHorizontalBox)
						+SHorizontalBox::Slot()
						[
							SNew(SOverlay)
							+SOverlay::Slot()
							[
								SNew(SScrollBorder, Outliner.ToSharedRef())
								[
									SNew(SHorizontalBox)
									// 轨道左侧的大纲
									+SHorizontalBox::Slot()
									.FillWidth(FillCoefficient_0)
									[
										SNew(SBox)
										[
											Outliner.ToSharedRef()
										]
									]
									// 轨道
									+SHorizontalBox::Slot()
									.FillWidth(FillCoefficient_1)
									[
										SNew(SBox)
										.Padding(ResizeBarPadding)
										.Clipping(EWidgetClipping::ClipToBounds)
										[
											TrackArea.ToSharedRef()
										]
									]
								]
							]

							+SOverlay::Slot()
							.HAlign(HAlign_Right)
							[
								ScrollBar
							]
						]
					]

					// 一个小垫片
					+SGridPanel::Slot(Column1, Row0)
					.Padding(ResizeBarPadding)
					.RowSpan(2)
					[
						SNew(SBorder)
						.BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
						[
							SNew(SSpacer)
						]
					]

					// 时间刻度 + 游标
					+SGridPanel::Slot(Column1, Row0, SGridPanel::Layer(10))
					.Padding(ResizeBarPadding)
					[
						SNew(SBorder)
						.BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
						.BorderBackgroundColor(FLinearColor(0.5f, 0.5f, 0.5f, 1.0f))
						.Padding(FMargin(0.0f, 1.0f, 0.0f, 1.0f))
						.Clipping(EWidgetClipping::ClipToBounds)
						[
							TopTimeSlider.ToSharedRef()
						]
					]

					// 左边的起始绿线 和 右边的终止红线
					+SGridPanel::Slot(Column1, Row1, SGridPanel::Layer(10))
					.Padding(ResizeBarPadding)
					[
						SNew(STimelineOverlay, TimeSliderControllerRef)
						.Visibility(EVisibility::HitTestInvisible)
						.DisplayScrubPosition(false)
						.DisplayTickLines(true)
						.Clipping(EWidgetClipping::ClipToBounds)
						.PaintPlaybackRangeArgs(FPaintPlaybackRangeArgs(FAppStyle::GetBrush("Sequencer.Timeline.PlayRange_L"), FAppStyle::GetBrush("Sequencer.Timeline.PlayRange_R"), 6.f))
					]

					// 游标下方的红线
					+SGridPanel::Slot(Column1, Row1, SGridPanel::Layer(20))
					.Padding(ResizeBarPadding)
					[
						SNew(STimelineOverlay, TimeSliderControllerRef)
						.Visibility(EVisibility::HitTestInvisible)
						.DisplayScrubPosition(true)
						.DisplayTickLines(false)
						.Clipping(EWidgetClipping::ClipToBounds)
					]

					// 下方滑动条
					+SGridPanel::Slot(Column1, Row3, SGridPanel::Layer(10))
					.Padding(ResizeBarPadding)
					[
						SNew(SBorder)
						.BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
						.BorderBackgroundColor(FLinearColor(0.5f, 0.5f, 0.5f, 1.0f))
						.Clipping(EWidgetClipping::ClipToBounds)
						.Padding(0)
						[
							BottomTimeRange
						]
					]
				]


				+ SOverlay::Slot()
				[
					SNew(STimelineSplitterOverlay)
					.Style(FAppStyle::Get(), "AnimTimeline.Outliner.Splitter")
					.Visibility(EVisibility::SelfHitTestInvisible)

					+ SSplitter::Slot()
					.Value(FillCoefficient_0)
					.OnSlotResized(SSplitter::FOnSlotResized::CreateSP(this, &STimeline::OnColumnFillCoefficientChanged, 0))
					[
						SNew(SSpacer)
					]

					+ SSplitter::Slot()
					.Value(FillCoefficient_1)
					.OnSlotResized(SSplitter::FOnSlotResized::CreateSP(this, &STimeline::OnColumnFillCoefficientChanged, 1))
					[
						SNew(SSpacer)
					]
				]

			]
		]
	];
}

FReply STimeline::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (MouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		FWidgetPath WidgetPath = MouseEvent.GetEventPath() != nullptr ? *MouseEvent.GetEventPath() : FWidgetPath();

		const bool bCloseAfterSelection = true;
		FMenuBuilder MenuBuilder(bCloseAfterSelection, nullptr);

		MenuBuilder.BeginSection("TimelineOptions", LOCTEXT("TimelineOptions", "Timeline Options"));
		{
			MenuBuilder.AddMenuEntry
			(
				LOCTEXT("TimeFormat_SnapText", "SnapMode"),
				LOCTEXT("TimeFormat_SnapText", "Display the time SnapMode"),
				FSlateIcon(),
				FUIAction
				(
					FExecuteAction::CreateLambda
					(
						[this]()
						{
							this->ToggleTimelineEnabledSnaps();
						}
					),
					FCanExecuteAction(),
					FIsActionChecked::CreateLambda
					(
						[this]() 
						{ 
							return this->IsDisplayTimelineEnabledSnapsChecked(); 
						}
					)
				),
				NAME_None,
				EUserInterfaceActionType::RadioButton
			);

			MenuBuilder.AddSubMenu
			(
				LOCTEXT("TimeFormat", "Time Format"),
				LOCTEXT("TimeFormatTooltip", "Choose the format of times we display in the timeline"),
				FNewMenuDelegate::CreateLambda
				(
					[this](FMenuBuilder& InMenuBuilder)
					{
						InMenuBuilder.BeginSection("TimeFormat", LOCTEXT("TimeFormat", "Time Format"));
						{
							InMenuBuilder.AddMenuEntry
							(
								LOCTEXT("TimeFormat_SecondsText", "Seconds"),
								LOCTEXT("TimeFormat_SecondsText", "Display the time in seconds"),
								FSlateIcon(),
								FUIAction
								(
									FExecuteAction::CreateLambda
									(
										[this]() 
										{
											this->SetDisplayFormat(EFrameNumberDisplayFormats::Seconds);
										}
									),
									FCanExecuteAction(),
									FIsActionChecked::CreateLambda
									(
										[this]() 
										{ 
											return this->IsDisplayFormatChecked(EFrameNumberDisplayFormats::Seconds); 
										}
									)
								),
								NAME_None,
								EUserInterfaceActionType::RadioButton
							);

							InMenuBuilder.AddMenuEntry
							(
								LOCTEXT("TimeFormat_FrameText", "Frames"),
								LOCTEXT("TimeFormat_FrameText", "Display the time in frames"),
								FSlateIcon(),
								FUIAction
								(
									FExecuteAction::CreateLambda
									(
										[this]()
										{
											this->SetDisplayFormat(EFrameNumberDisplayFormats::Frames);
										}
									),
									FCanExecuteAction(),
									FIsActionChecked::CreateLambda
									(
										[this]() 
										{ 
											return this->IsDisplayFormatChecked(EFrameNumberDisplayFormats::Frames); 
										}
									)
								),
								NAME_None,
								EUserInterfaceActionType::RadioButton
							);
						}
						InMenuBuilder.EndSection();

						InMenuBuilder.BeginSection("TimelineAdditional", LOCTEXT("TimelineAdditional", "Additional Display"));
						{
							InMenuBuilder.AddMenuEntry
							(
								LOCTEXT("TimeFormat_PercentageText", "Percentage"),
								LOCTEXT("TimeFormat_PercentageText", "Display the percentage along with the time with the scrubber"),
								FSlateIcon(),
								FUIAction
								(
									FExecuteAction::CreateLambda
									(
										[this]()
										{
											this->ToggleDisplayPercentage();
										}
									),
									FCanExecuteAction(),
									FIsActionChecked::CreateLambda
									(
										[this]() 
										{ 
											return this->IsDisplayPercentageChecked();
										}
									)
								),
								NAME_None,
								EUserInterfaceActionType::RadioButton
							);

							InMenuBuilder.AddMenuEntry
							(
								LOCTEXT("TimeFormat_SecondaryText", "Secondary"),
								LOCTEXT("TimeFormat_SecondaryText", "Display the time or frame count as a secondary format along with the scrubber"),
								FSlateIcon(),
								FUIAction
								(
									FExecuteAction::CreateLambda
									(
										[this]()
										{
											this->ToggleDisplaySecondary();
										}
									),
									FCanExecuteAction(),
									FIsActionChecked::CreateLambda
									(
										[this]() 
										{ 
											return this->IsDisplaySecondaryChecked(); 
										}
									)
								),
								NAME_None,
								EUserInterfaceActionType::RadioButton
							);
						}
						InMenuBuilder.EndSection();
					}
				)
			);
		}
		MenuBuilder.EndSection();

		FSlateApplication::Get().PushMenu(SharedThis(this), WidgetPath, MenuBuilder.MakeWidget(), FSlateApplication::Get().GetCursorPos(), FPopupTransitionEffect(FPopupTransitionEffect::ContextMenu));

		return FReply::Handled();
	}
	
	return FReply::Unhandled();
}

void STimeline::OnShowPopupOfAppendAnimation(FWidgetPath WidgetPath, bool bBegin)
{
	TSharedRef<STextEntryPopup> TextEntry = SNew(STextEntryPopup).Label(LOCTEXT("AppendAnim_AskNumFrames", "Number of Frames to Append")).OnTextCommitted(this, &STimeline::OnSequenceAppendedCalled, bBegin);

	// Show dialog to enter new track name
	FSlateApplication::Get().PushMenu
	(
		SharedThis(this),
		WidgetPath,
		TextEntry,
		FSlateApplication::Get().GetCursorPos(),
		FPopupTransitionEffect(FPopupTransitionEffect::TypeInPopup)
	);
}

void STimeline::OnSequenceAppendedCalled(const FText & InNewGroupText, ETextCommit::Type CommitInfo, bool bBegin)
{
	// just a concern
	const static int32 MaxFrame = 1000;

	// handle only onEnter. This is a big thing to apply when implicit focus change or any other event
	if (CommitInfo == ETextCommit::OnEnter)
	{
		int32 NumFrames = FCString::Atoi(*InNewGroupText.ToString());
		if (NumFrames > 0 && NumFrames < MaxFrame)
		{
			FSlateApplication::Get().DismissAllMenus();
		}
	}
}

TSharedRef<INumericTypeInterface<double>> STimeline::GetNumericTypeInterface() const
{
	return NumericTypeInterface.ToSharedRef();
}

// FFrameRate::ComputeGridSpacing doesnt deal well with prime numbers, so we have a custom impl here
static bool ComputeGridSpacing(const FFrameRate& InFrameRate, float PixelsPerSecond, double& OutMajorInterval, int32& OutMinorDivisions, float MinTickPx, float DesiredMajorTickPx)
{
	// First try built-in spacing
	bool bResult = InFrameRate.ComputeGridSpacing(PixelsPerSecond, OutMajorInterval, OutMinorDivisions, MinTickPx, DesiredMajorTickPx);
	if(!bResult || OutMajorInterval == 1.0)
	{
		if (PixelsPerSecond <= 0.f)
		{
			return false;
		}

		const int32 RoundedFPS = FMath::RoundToInt(InFrameRate.AsDecimal());

		if (RoundedFPS > 0)
		{
			// Showing frames
			TArray<int32, TInlineAllocator<10>> CommonBases;

			// Divide the rounded frame rate by 2s, 3s or 5s recursively
			{
				const int32 Denominators[] = { 2, 3, 5 };

				int32 LowestBase = RoundedFPS;
				for (;;)
				{
					CommonBases.Add(LowestBase);
	
					if (LowestBase % 2 == 0)      
					{ 
						LowestBase = LowestBase / 2; 
					}
					else if (LowestBase % 3 == 0) 
					{ 
						LowestBase = LowestBase / 3; 
					}
					else if (LowestBase % 5 == 0) 
					{ 
						LowestBase = LowestBase / 5; 
					}
					else
					{ 
						int32 LowestResult = LowestBase;
						for(int32 Denominator : Denominators)
						{
							int32 Result = LowestBase / Denominator;
							if(Result > 0 && Result < LowestResult)
							{
								LowestResult = Result;
							}
						}

						if(LowestResult < LowestBase)
						{
							LowestBase = LowestResult;
						}
						else
						{
							break;
						}
					}
				}
			}

			Algo::Reverse(CommonBases);

			const int32 Scale = FMath::CeilToInt(DesiredMajorTickPx / PixelsPerSecond * InFrameRate.AsDecimal());
			const int32 BaseIndex = FMath::Min(Algo::LowerBound(CommonBases, Scale), CommonBases.Num()-1);
			const int32 Base = CommonBases[BaseIndex];

			int32 MajorIntervalFrames = FMath::CeilToInt(Scale / float(Base)) * Base;
			OutMajorInterval  = MajorIntervalFrames * InFrameRate.AsInterval();

			// Find the lowest number of divisions we can show that's larger than the minimum tick size
			OutMinorDivisions = 0;
			for (int32 DivIndex = 0; DivIndex < BaseIndex; ++DivIndex)
			{
				if (Base % CommonBases[DivIndex] == 0)
				{
					int32 MinorDivisions = MajorIntervalFrames/CommonBases[DivIndex];
					if (OutMajorInterval / MinorDivisions * PixelsPerSecond >= MinTickPx)
					{
						OutMinorDivisions = MinorDivisions;
						break;
					}
				}
			}
		}
	}

	return OutMajorInterval != 0;
}

bool STimeline::GetGridMetrics(float PhysicalWidth, double& OutMajorInterval, int32& OutMinorDivisions) const
{
	FSlateFontInfo SmallLayoutFont = FCoreStyle::GetDefaultFontStyle("Regular", 8);
	TSharedRef<FSlateFontMeasure> FontMeasureService = FSlateApplication::Get().GetRenderer()->GetFontMeasureService();

	FFrameRate DisplayRate(FMath::RoundToInt(TimelineController.Pin()->GetFrameRate()), 1);
	double BiggestTime = ViewRange.Get().GetUpperBoundValue();
	FString TickString = NumericTypeInterface->ToString((BiggestTime * DisplayRate).FrameNumber.Value);
	FVector2D MaxTextSize = FontMeasureService->Measure(TickString, SmallLayoutFont);

	static float MajorTickMultiplier = 2.f;

	float MinTickPx = MaxTextSize.X + 5.f;
	float DesiredMajorTickPx = MaxTextSize.X * MajorTickMultiplier;

	if (PhysicalWidth > 0)
	{
		return ComputeGridSpacing
		(
			DisplayRate,
			PhysicalWidth / ViewRange.Get().Size<double>(),
			OutMajorInterval,
			OutMinorDivisions,
			MinTickPx,
			DesiredMajorTickPx
		);
	}

	return false;
}

TSharedPtr<ITimeSliderController> STimeline::GetTimeSliderController() const 
{ 
	return TimeSliderController; 
}

void STimeline::OnOutlinerSearchChanged( const FText& Filter )
{
	FilterText = Filter;

	Outliner->RefreshFilter();
}

void STimeline::OnColumnFillCoefficientChanged(float FillCoefficient, int32 ColumnIndex)
{
	ColumnFillCoefficients[ColumnIndex] = FillCoefficient;
}

void STimeline::HandleKeyComplete()
{
	TimelineController.Pin()->RefreshTracks();
}

class UAnimSingleNodeInstance* STimeline::GetPreviewInstance() const
{
	return nullptr;
}

void STimeline::HandleScrubPositionChanged(FFrameTime NewScrubPosition, bool bIsScrubbing, bool bEvaluate)
{
	TimelineController.Pin()->SetScrubPosition(NewScrubPosition);
}

double STimeline::GetSpinboxDelta() const
{
	return FFrameRate(TimelineController.Pin()->GetTickResolution(), 1).AsDecimal() * FFrameRate(FMath::RoundToInt(TimelineController.Pin()->GetFrameRate()), 1).AsInterval();
}

void STimeline::SetPlayTime(double InBXADeltaTime)
{

}

void STimeline::SetDisplayFormat(EFrameNumberDisplayFormats InFormat)
{
	GetMutableDefault<UPersonaOptions>()->TimelineDisplayFormat = InFormat;
}

bool STimeline::IsDisplayFormatChecked(EFrameNumberDisplayFormats InFormat) const
{
	return GetDefault<UPersonaOptions>()->TimelineDisplayFormat == InFormat;
}

void STimeline::ToggleDisplayPercentage()
{
	GetMutableDefault<UPersonaOptions>()->bTimelineDisplayPercentage = !GetDefault<UPersonaOptions>()->bTimelineDisplayPercentage;
}

bool STimeline::IsDisplayPercentageChecked() const
{
	return GetDefault<UPersonaOptions>()->bTimelineDisplayPercentage;
}

void STimeline::ToggleDisplaySecondary()
{
	GetMutableDefault<UPersonaOptions>()->bTimelineDisplayFormatSecondary = !GetDefault<UPersonaOptions>()->bTimelineDisplayFormatSecondary;
}

bool STimeline::IsDisplaySecondaryChecked() const
{
	return GetDefault<UPersonaOptions>()->bTimelineDisplayFormatSecondary;
}

void STimeline::ToggleTimelineEnabledSnaps()
{
	if (GetDefault<UPersonaOptions>()->TimelineEnabledSnaps.Contains(InSnapName))
	{
		GetMutableDefault<UPersonaOptions>()->TimelineEnabledSnaps.Remove(InSnapName);
	}
	else
	{
		GetMutableDefault<UPersonaOptions>()->TimelineEnabledSnaps.AddUnique(InSnapName);
	}
}

bool STimeline::IsDisplayTimelineEnabledSnapsChecked() const
{
	return IsEnabledSnaps();
}

bool STimeline::IsEnabledSnaps()
{
	return !GetDefault<UPersonaOptions>()->TimelineEnabledSnaps.Contains(InSnapName);
}


#undef LOCTEXT_NAMESPACE
