#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateStyle.h" 



class BATTLEXEDITOR_API FBXTLEditorStyle
{
public:
	static void Initialize();

	static void Shutdown();

	static void ReloadTextures();

	static const ISlateStyle* Get();

	static FName GetStyleSetName();

	static const FSlateBrush* GetBrush(FName BrushName);

private:
	static TSharedRef<class FSlateStyleSet> Create();

	static TSharedPtr<class FSlateStyleSet> StyleInstance;

};