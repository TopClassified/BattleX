#pragma once

#include "ThumbnailRendering/DefaultSizedThumbnailRenderer.h"

#include "BXTLAssetThumbnailRenderer.generated.h"



UCLASS()
class BATTLEXEDITOR_API UBXTLAssetThumbnailRenderer : public UDefaultSizedThumbnailRenderer
{
	GENERATED_BODY()

public:
	UBXTLAssetThumbnailRenderer(const FObjectInitializer& ObjectInitializer);

	virtual void Draw(UObject* Object, int32 X, int32 Y, uint32 Width, uint32 Height, FRenderTarget* RenderTarget, FCanvas* Canvas, bool bAdditionalViewFamily) override;

private:
	UPROPERTY()
	class UTexture2D* m_NoImage = nullptr;

	UPROPERTY()
	class UFont* NameFont = nullptr;

	UPROPERTY()
	class UFont* TipFont = nullptr;
};

