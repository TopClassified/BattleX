#pragma once

#include "CoreMinimal.h"
#include "SSingleObjectDetailsPanel.h"

#include "BXTLEditor.h"
#include "Preview/BXTLPreviewScene.h"
#include "Viewport/SBXTLEditorViewport.h"



class SBXTLAssetTimelineTab;
class SBXTLAssetSection : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SBXTLAssetSection) {};
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, const TSharedPtr<FBXTLEditor>& InAssetEditorToolkit, const TSharedPtr<SBXTLAssetTimelineTab>& AssetEditWidget, int32 InSectionIndex);

private:
	void ChangeSectionName(const FText& InText, ETextCommit::Type CommitInfo);

	void ChangeSectionLoopTime(const FText& InText, ETextCommit::Type CommitInfo);

	void ChangeSectionDuration(const FText& InText, ETextCommit::Type CommitInfo);

	TSharedRef<SWidget> BuildSectionSubMenu();

	void AddSection();

	void DeleteSection();

	FText GetSectionName() const;

	FText GetSectionLoopTime() const;

	FText GetSectionDuration() const;
	 
public:
	TWeakPtr<FBXTLEditor> CachedEditor = nullptr;

	TWeakPtr<SBXTLAssetTimelineTab> CachedEditTab = nullptr;

	TSharedPtr<class STimeline> TimelineWidget = nullptr;

	TSharedPtr<class FBXTLController> TimelineController = nullptr;

};



class SBXTLAssetTimelineTab : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SBXTLAssetTimelineTab) {};
	SLATE_END_ARGS()

public:
	void Construct(const FArguments& InArgs, const TSharedPtr<FBXTLEditor>& InEditor);

	virtual ~SBXTLAssetTimelineTab();

	void ResetSections();

	void OnRefreshPanel();

	FReply OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;

	FReply OnKeyUp(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;

	void OnSectionCheckedChanged(ECheckBoxState State, int32 Index);

	ECheckBoxState IsSectionChecked(int32 Index) const;

protected:
	void AddSection();

	void DeleteSection(int32 Index);

	TSharedRef<SWidget> BuildSectionSubMenu();

private:
	TArray<int32> HelpArray;

	TWeakPtr<FBXTLEditor> CachedEditor = nullptr;

	TArray<TSharedPtr<class SBXTLAssetSection>> SectionWidgets;

	TSharedPtr<SWidget> SectionMenu = nullptr;

	TSharedPtr<SScrollBox> ScrollBox = nullptr;

	TSharedPtr<SVerticalBox> VerticalBox = nullptr;

	TSharedPtr<SHorizontalBox> HorizontalBox = nullptr;

};



class SBXTLAssetDetailTab : public SSingleObjectDetailsPanel
{
public:
	SLATE_BEGIN_ARGS(SBXTLAssetDetailTab) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, TSharedPtr<class FBXTLEditor> InEditor);

	virtual UObject* GetObjectToObserve() const override;

	virtual TSharedRef<class SWidget> PopulateSlot(TSharedRef<class SWidget> PropertyEditorWidget) override;

	virtual FText GetAssetDisplayName() const;

	virtual EVisibility GetAssetDisplayNameVisibility() const;

private:
	TWeakPtr<class FBXTLEditor> CachedEditor = nullptr;

};
