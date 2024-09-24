#include "BXTLEditorViewportToolbar.h"

#include "EditorViewportCommands.h"
#include "SEditorViewportViewMenu.h"
#include "SEditorViewportToolBarMenu.h"
#include "STransformViewportToolbar.h"
#include "Widgets/Input/SNumericEntryBox.h"

#include "BXTLEditor.h"
#include "Preview/BXTLPreviewScene.h"
#include "BXTLEditorViewportClient.h"
#include "SBXTLEditorViewport.h" 



#define LOCTEXT_NAMESPACE "BXTLEditorViewportToolbar"

void SBXTLEditorViewportToolBar::Construct(const FArguments& InArgs, TSharedPtr<FBXTLEditor> InEditor, TSharedPtr<SBXTLEditorViewport> InViewport)
{
	CachedEditor = InEditor;
	CachedViewport = InViewport;
	TSharedPtr<FSimpleEditorViewportClient> SimpeEdViewportClient = StaticCastSharedPtr<FSimpleEditorViewportClient>(InViewport->GetViewportClient());
	
	TSharedRef<SHorizontalBox> ToolbarContainer = SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(2.0f, 2.0f)
		[
			SNew(SEditorViewportToolbarMenu)
			.ParentToolBar(SharedThis(this))
			.Image("EditorViewportToolBar.MenuDropdown")
			.OnGetMenuContent(this, &SBXTLEditorViewportToolBar::GenerateViewMenu)
		]
		+ SHorizontalBox::Slot()
		.Padding(3.0f, 1.0f)
		.HAlign(HAlign_Right)
		[
			SNew(STransformViewportToolBar)
			.Viewport(CachedViewport.Pin())
			.CommandList(CachedViewport.Pin()->GetCommandList())
			.Visibility(this, &SBXTLEditorViewportToolBar::GetTransformToolBarVisibility)
		];

	ChildSlot
	[
		SNew(SBorder)
		.BorderImage(FAppStyle::GetBrush("NoBorder"))
		.ForegroundColor(FAppStyle::GetSlateColor("DefaultForeground"))
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				ToolbarContainer
			]
		]
	];

	SViewportToolBar::Construct(SViewportToolBar::FArguments());
}

EVisibility SBXTLEditorViewportToolBar::GetTransformToolBarVisibility() const
{
	return EVisibility::Visible;
}

TSharedRef<SWidget> SBXTLEditorViewportToolBar::GenerateViewMenu()
{
	const bool bInShouldCloseWindowAfterMenuSelection = true;
	FMenuBuilder ViewMenuBuilder(bInShouldCloseWindowAfterMenuSelection, CachedViewport.Pin()->GetCommandList());

	// 相机模式
	ViewMenuBuilder.BeginSection("ViewportCamera", LOCTEXT("ViewMenu_CameraLabel", "Camera"));
	{
		ViewMenuBuilder.AddMenuEntry(FEditorViewportCommands::Get().FocusViewportToSelection);

		ViewMenuBuilder.AddSubMenu
		(
			LOCTEXT("CameraFollowModeLabel", "Camera Follow Mode"),
			LOCTEXT("CameraFollowModeTooltip", "Set various camera follow modes"),
			FNewMenuDelegate::CreateLambda
			(
				[this](FMenuBuilder& InSubMenuBuilder)
				{
					InSubMenuBuilder.BeginSection("SkillViewportCameraFollowMode", LOCTEXT("ViewMenu_CameraFollowModeLabel", "Camera Follow Mode"));
					{
						InSubMenuBuilder.PushCommandList(CachedViewport.Pin()->GetCommandList().ToSharedRef());

						InSubMenuBuilder.AddMenuEntry
						(
							GetCameraModeEntryLable(BXTLViewportCameraMode::Free),
							FText::FromString(""), 
							FSlateIcon(), 
							FUIAction(FExecuteAction::CreateRaw(this, &SBXTLEditorViewportToolBar::SetCameraMode, BXTLViewportCameraMode::Free))
						);

						InSubMenuBuilder.AddMenuEntry
						(
							GetCameraModeEntryLable(BXTLViewportCameraMode::RealGame),
							FText::FromString(""), 
							FSlateIcon(), 
							FUIAction(FExecuteAction::CreateSP(this, &SBXTLEditorViewportToolBar::SetCameraMode, BXTLViewportCameraMode::RealGame))
						);
					
						InSubMenuBuilder.PopCommandList();
					}
					InSubMenuBuilder.EndSection();
				}
			),
			false,
			FSlateIcon(FAppStyle::GetAppStyleSetName(), "AnimViewportMenu.CameraFollow")
		);

		ViewMenuBuilder.AddWidget(MakeFOVWidget(), LOCTEXT("Viewport_FOVLabel", "Field Of View"));
	}
	ViewMenuBuilder.EndSection();

	return ViewMenuBuilder.MakeWidget();
}

FText SBXTLEditorViewportToolBar::GetCameraModeEntryLable(const FName InMode, bool bSkillBone) const
{
	bool bCurrMode = GetCameraMode() == InMode;
	if (InMode == BXTLViewportCameraMode::Free)
	{
		return FText::FromString(bCurrMode ? "* Free Camera" : "Free Camera");
	}
	else if (InMode == BXTLViewportCameraMode::RealGame)
	{
		return FText::FromString(bCurrMode ? "* In Game Camera" : "In Game Camera");
	}

	return FText::FromString("");
}

void SBXTLEditorViewportToolBar::SetCameraMode(FName InCameraMode)
{
	TSharedPtr<FBXTLEditorViewportClient> ViewportClient = StaticCastSharedPtr<FBXTLEditorViewportClient>(CachedViewport.Pin()->GetViewportClient());
	ViewportClient->ChangeViewMode(InCameraMode);
}

FName SBXTLEditorViewportToolBar::GetCameraMode() const
{
	TSharedPtr<FBXTLEditorViewportClient> ViewportClient = StaticCastSharedPtr<FBXTLEditorViewportClient>(CachedViewport.Pin()->GetViewportClient());
	return ViewportClient->GetViewMode();
}

TSharedRef<SWidget> SBXTLEditorViewportToolBar::MakeFOVWidget() const
{
	const float FOVMin = 5.f;
	const float FOVMax = 170.f;

	return SNew(SBox)
		.HAlign(HAlign_Right)
		[
			SNew(SBox)
			.Padding(FMargin(4.0f, 0.0f, 0.0f, 0.0f))
			.WidthOverride(100.0f)
			[
				SNew(SNumericEntryBox<float>)
				.Font(FAppStyle::GetFontStyle(TEXT("MenuItem.Font")))
				.AllowSpin(true)
				.MinValue(FOVMin)
				.MaxValue(FOVMax)
				.MinSliderValue(FOVMin)
				.MaxSliderValue(FOVMax)
				.Value_Lambda
				(
					[this]() -> float 
					{
						return CachedViewport.Pin()->GetViewportClient()->ViewFOV;
					}
				)
				.OnValueChanged_Lambda
				(
					[this](float NewValue) 
					{
						CachedViewport.Pin()->GetViewportClient()->FOVAngle = NewValue;
						CachedViewport.Pin()->GetViewportClient()->ViewFOV = NewValue;
						CachedViewport.Pin()->GetViewportClient()->Invalidate();
					}
				)
			]
		];
}

#undef LOCTEXT_NAMESPACE
