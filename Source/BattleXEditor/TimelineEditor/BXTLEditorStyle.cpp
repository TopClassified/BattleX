#include "BXTLEditorStyle.h"
#include "Styling/CoreStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Framework/Application/SlateApplication.h"



const FVector2D Icon12x12(12.0f, 12.0f);
const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);
const FVector2D Icon40x40(40.0f, 40.0f);
const FVector2D Icon64x64(64.0f, 64.0f);
const FVector2D Icon128x128(128.0f, 128.0f);

TSharedPtr<FSlateStyleSet> FBXTLEditorStyle::StyleInstance = nullptr;

void FBXTLEditorStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FBXTLEditorStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FBXTLEditorStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("BXTLEditorStyle"));
	return StyleSetName;
}

#define IMAGE_BRUSH(RelativePath, ...) FSlateImageBrush(Style->RootToContentDir(RelativePath, TEXT(".png")), __VA_ARGS__)
#define BOX_BRUSH(RelativePath, ...) FSlateBoxBrush(Style->RootToContentDir(RelativePath, TEXT(".png")), __VA_ARGS__)
#define BORDER_BRUSH(RelativePath, ...) FSlateBorderBrush(Style->RootToContentDir(RelativePath, TEXT(".png")), __VA_ARGS__)
#define TTF_FONT(RelativePath, ...) FSlateFontInfo(Style->RootToContentDir(RelativePath, TEXT(".ttf")), __VA_ARGS__)
#define OTF_FONT(RelativePath, ...) FSlateFontInfo(Style->RootToContentDir(RelativePath, TEXT(".otf")), __VA_ARGS__)
#define DEFAULT_FONT(...) FCoreStyle::GetDefaultFontStyle(__VA_ARGS__)
#define IMAGE_PLUGIN_BRUSH(RelativePath, ...) FSlateImageBrush(Style->RootToContentDir(RelativePath, TEXT(".png")), __VA_ARGS__)
#define BOX_PLUGIN_BRUSH(RelativePath, ...) FSlateBoxBrush(Style->RootToContentDir(RelativePath, TEXT(".png")), __VA_ARGS__)

TSharedRef< FSlateStyleSet > FBXTLEditorStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("BXTLEditorStyle"));
	FString ResPath = FPaths::ProjectContentDir() + FString(TEXT("Editor"));
	Style->SetContentRoot(ResPath);

	Style->Set("BSAEditor.DataProcess", new IMAGE_PLUGIN_BRUSH(TEXT("Icons/DataProcess"), Icon40x40));
	Style->Set("Sequencer.PhotoStudio", new IMAGE_PLUGIN_BRUSH(TEXT("Icons/PhotoStudio"), Icon16x16));

	FTextBlockStyle NormalText = FTextBlockStyle()
		.SetFont(DEFAULT_FONT("Regular", FCoreStyle::RegularTextSize))
		.SetColorAndOpacity(FSlateColor::UseForeground())
		.SetShadowOffset(FVector2D::ZeroVector)
		.SetShadowColorAndOpacity(FLinearColor::Black)
		.SetHighlightColor( FLinearColor( 0.02f, 0.3f, 0.0f ) )
		.SetHighlightShape( BOX_BRUSH( "Common/TextBlockHighlightShape", FMargin(3.f/8.f) ) );

	Style->Set("TextBlock.PlotNode.SpeakerName", FTextBlockStyle(NormalText)
		.SetFont(DEFAULT_FONT("BoldCondensed", 16))
		.SetColorAndOpacity(FLinearColor(0.9, 0.08f, 0.35f, 0.9f))
		.SetShadowOffset(FVector2D::ZeroVector)
	);

	return Style;
}

#undef IMAGE_BRUSH
#undef BOX_BRUSH
#undef BORDER_BRUSH
#undef TTF_FONT
#undef OTF_FONT
#undef DEFAULT_FONT

void FBXTLEditorStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle* FBXTLEditorStyle::Get()
{
	return StyleInstance.Get();
}

const FSlateBrush* FBXTLEditorStyle::GetBrush(FName BrushName)
{
	if (StyleInstance.IsValid())
	{
		return StyleInstance->GetBrush(BrushName);
	}

	return nullptr;
}
