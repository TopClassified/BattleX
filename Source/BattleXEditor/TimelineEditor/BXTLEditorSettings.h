#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/DeveloperSettings.h"

#include "BXTLEditorTemplate.h"
#include "Preview/BXTLPreviewActor.h"

#include "BXTLEditorSettings.generated.h" 




UCLASS(Config = Editor, DefaultConfig, meta = (DisplayName = "BXTLEditorSettings"))
class BATTLEXEDITOR_API UBXTLEditorSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UBXTLEditorSettings(const FObjectInitializer& ObjectInitializer);

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

public:
	// 编辑器Tick频率
	UPROPERTY(Config, EditAnywhere, Category = "Preview")
	float PreviewTickRate = 60.0f;

	// 最长烘焙时间
	float MaxBakeTime = 100.0f;
	


	UPROPERTY(Config, EditAnywhere, Category = "PreviewScene")
	TSubclassOf<class ABXTLPreviewGameMode> GameModeClass = nullptr;

	UPROPERTY(Config, EditAnywhere, Category = "PreviewScene")
	TSoftObjectPtr<UWorld> DefaultViewMap;

	UPROPERTY(Config, EditAnywhere, Category = "PreviewScene")
	TSubclassOf<UBXTLPreviewActor> PlayerType = UBXTLPreviewActor::StaticClass();

	UPROPERTY(Config, EditAnywhere, Category = "PreviewScene")
	TSubclassOf<UBXTLPreviewActor> TargetType = UBXTLPreviewActor::StaticClass();



	UPROPERTY(Config, EditAnywhere, Category = "Edit")
	TSoftObjectPtr<UBXTLTaskGroupTemplate> Template = nullptr;

};