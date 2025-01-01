#pragma once

#include "CoreMinimal.h"
#include "ThumbnailRendering/DefaultSizedThumbnailRenderer.h"

#include "BXDTAssetThumbnailRenderers.generated.h"


UCLASS()
class UBXDTAssetThumbnailRenderers : public UDefaultSizedThumbnailRenderer
{
	GENERATED_BODY()

public:
	UBXDTAssetThumbnailRenderers(const FObjectInitializer& ObjectInitializer);

	virtual void Draw(UObject* Object, int32 X, int32 Y, uint32 Width, uint32 Height, FRenderTarget* RenderTarget, FCanvas* Canvas, bool bAdditionalViewFamily) override;

private:
	UPROPERTY()
	class UTexture2D* m_NoImage = NULL;

	UPROPERTY()
	class UFont* NameFont = NULL;

	UPROPERTY()
	class UFont* TipFont = NULL;
};

