#pragma once

#include "TimelineTrack.h"

#include "BXTLAsset.h" 



class FBXTLTaskGroupTrack : public FTimelineTrack
{
	TIMELINE_DECLARE_TRACK(FBXTLTaskGroupTrack, FTimelineTrack);

#pragma region Important
public:
	FBXTLTaskGroupTrack(const TSharedRef<class FBXTLController>& InController, FBXTLTaskGroup& InGroupData, const FText& InDisplayName, const FText& InToolTipText);

	// 绘制Group轨道左边的大纲
	TSharedRef<class SWidget> GenerateContainerWidgetForOutliner(const TSharedRef<SOutlinerItem>& InRow) override;

	bool CanRename() const override;

	FText GetLabel() const override;

protected:
	int32 GetGroupIndex() const;

	// 按索引解析当前组数据(Undo/属性面板修改Groups会使数组重排,裸指针悬垂,失效时返回nullptr)
	FBXTLTaskGroup* GetGroupData() const;

private:
	// 组在Section.Groups内的索引(原裸指针在数组重排后悬垂,绘制/改名路径读写已释放内存)
	int32 GroupID = INDEX_NONE;

#pragma endregion Important



#pragma region Callback
protected:
	void OnCommitName(const FText& InText, ETextCommit::Type CommitInfo);

	TSharedRef<class SWidget> OnBuildSubMenu();

	void OnFillTaskMenu(class FMenuBuilder& MenuBuilder);

	void OnFillTaskTemplateMenu(class FMenuBuilder& MenuBuilder);

	void OnAddTask(UClass* InTaskClass);

	void OnPasteTasks();

	void OnAddGroup();

	void OnDeleteGroup();

	void OnAddTaskTemplate(FName TemplateName);

#pragma endregion Callback

};