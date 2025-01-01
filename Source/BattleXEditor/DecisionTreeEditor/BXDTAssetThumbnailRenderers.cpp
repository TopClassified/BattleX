#include "BXDTAssetThumbnailRenderers.h"

#include "CanvasTypes.h"
#include "Engine/Font.h"
#include "Engine/Texture2D.h"
#include "SceneView.h"
#include "UObject/ConstructorHelpers.h"

#include "BXDecisionTreeTemplate.h"



UBXDTAssetThumbnailRenderers::UBXDTAssetThumbnailRenderers(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), m_NoImage(nullptr)
{
	ConstructorHelpers::FObjectFinder<UTexture2D> PSysThumbnail_NoImage(TEXT("/Engine/EditorMaterials/ParticleSystems/PSysThumbnail_NoImage"));
	m_NoImage = PSysThumbnail_NoImage.Object;

	NameFont = NewObject<UFont>(GetTransientPackage(), FName(this->GetFullName() + TEXT("Name")), RF_Transient);
	NameFont->FontCacheType = EFontCacheType::Runtime;
	NameFont->LegacyFontSize = 27.0f;
	NameFont->CompositeFont = *FCoreStyle::GetDefaultFont();

	TipFont = NewObject<UFont>(GetTransientPackage(), FName(this->GetFullName() + TEXT("Tips")), RF_Transient);
	TipFont->FontCacheType = EFontCacheType::Runtime;
	TipFont->LegacyFontSize = 20.0f;
	TipFont->CompositeFont = *FCoreStyle::GetDefaultFont();
}

void UBXDTAssetThumbnailRenderers::Draw(UObject* Object, int32 X, int32 Y, uint32 Width, uint32 Height, FRenderTarget* RenderTarget, FCanvas* Canvas, bool bAdditionalViewFamily)
{
	if (UBXDecisionTreeTemplate* Asset = Cast<UBXDecisionTreeTemplate>(Object))
	{
		Canvas->DrawTile(X, Y, Width, Height, 0.0f, 0.0f, 1.0f, 1.0f, FLinearColor::White, m_NoImage->GetResource(), false);

		Canvas->DrawShadowedText(X, Y + Height / 5.0f, Asset->ThumbnailMainTitle, NameFont, FLinearColor::White, FLinearColor::Yellow);
		Canvas->DrawShadowedText(X, Y + Height / 2.0f, Asset->ThumbnailSubTitle, TipFont, FLinearColor::White, FLinearColor::Yellow);
	}
}
