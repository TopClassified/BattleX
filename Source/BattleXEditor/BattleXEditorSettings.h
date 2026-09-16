#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/DeveloperSettings.h"

#include "TimelineEditor/BXTLEditorTemplate.h"
#include "TimelineEditor/Preview/BXTLPreviewActor.h"

#include "BattleXEditorSettings.generated.h"



// BattleX编辑器设置(2026-09-16由BattleXTimelineEditor改名扩展:所有编辑器相关设置项收拢于此;
// 现含时间轴编辑器的Preview/PreviewScene/Edit配置,后续新增编辑器设置一律入本类)
UCLASS(Config = BattleX, DefaultConfig, meta = (DisplayName = "BattleXEditor"))
class BATTLEXEDITOR_API UBattleXEditorSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UBattleXEditorSettings(const FObjectInitializer& ObjectInitializer);

	// Project Settings 页面定位(引擎自动发现UDeveloperSettings,按此注册;勿手动ISettingsModule注册——会双页面)
	virtual FName GetContainerName() const override;
	virtual FName GetCategoryName() const override;
	virtual FName GetSectionName() const override;
#if WITH_EDITOR
	virtual FText GetSectionText() const override;
	virtual FText GetSectionDescription() const override;
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

public:
	// 编辑器Tick频率
	UPROPERTY(Config, EditAnywhere, Category = "Preview")
	float PreviewTickRate = 60.0f;

	// Debug运行高亮(黄色框)结束后残留时长(残留期内透明度由不透明线性过渡到全透明)
	UPROPERTY(Config, EditAnywhere, Category = "Preview", meta = (ClampMin = "0.0", Units = "s"))
	float RunningHighlightResidualDuration = 1.0f;



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
