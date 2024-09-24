#pragma once

#include "Input/Reply.h"
#include "Styling/SlateBrush.h"
#include "Framework/MultiBox/MultiBoxBuilder.h" 



class FBXTLEditorToolbar : public TSharedFromThis<FBXTLEditorToolbar>
{
public:
	// 建立工具栏
	void SetupToolbar(TSharedPtr<FExtender> Extender, TSharedPtr<class FBXTLEditor> InEditor);

	// 添加模式工具栏
	//void AddModesToolbar(TSharedPtr<FExtender> Extender);

	// 添加时间轴工具栏
	void AddTimelineToolbar(TSharedPtr<FExtender> Extender);

private:
	// 填充模式工具栏
	//void FillModesToolbar(FToolBarBuilder& ToolbarBuilder);

	// 填充时间轴工具栏
	void FillTimelineModeToolbar(FToolBarBuilder& ToolbarBuilder);

	// 数据处理菜单栏
	TSharedRef<SWidget> GenerateProcessDataMenu();

	FText GetPlayText() const;

	FText GetPlayToolTip() const;

	FSlateIcon GetPlayIcon() const;

private:
	TWeakPtr<class FBXTLEditor> CachedEditor;

	FSlateIcon PlayIcon;

	FSlateIcon PauseIcon;

};
