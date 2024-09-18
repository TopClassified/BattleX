#pragma once

#include "Misc/FrameTime.h"
#include "UObject/GCObject.h"
#include "Containers/ArrayView.h"
#include "Templates/SharedPointer.h"

#include "TimelineController.h"



class FBXTLController : public FTimelineController
{
#pragma region Important
public:
	FBXTLController();

	virtual ~FBXTLController();

	// 初始化
	void Initialize(const TSharedPtr<class FBXTLEditor>& InAssetEditorToolkit, TSharedPtr<class FBXTLPreviewProxy> InPreviewProxy, int32 InSectionIndex);

	// 刷新轨道
	void RefreshTracks() override;

	void PostClearTrackSelection() override;

#pragma endregion Important



#pragma region Parameter
public:
	int32 GetSectionID();
	
	// 获取当前游标的位置
	FFrameNumber GetScrubPosition() const override;

	// 设置游标位置
	void SetScrubPosition(FFrameTime NewScrubPostion) const override;

	// 获取总时长
	float GetPlayLength() const override;

	// 获取帧率
	double GetFrameRate() const override;

	// 获取资源对象
	class UBXTLAsset* GetAsset() const;

	// 获取编辑器
	TWeakPtr<class FBXTLEditor> GetEditor() const;

private:
	TWeakPtr<class FBXTLEditor> CachedEditor = nullptr;

	TSharedPtr<class FBXTLPreviewProxy> CachedPreviewProxy = nullptr;

	int32 SectionIndex = 0;

	mutable float ScrubPosition = 0.0f;

#pragma endregion Parameter



#pragma region Input
public:
	void ChangeCtrlState(bool bPress);

private:
	bool bPressCtrl = false;

#pragma endregion Input



#pragma region Task
public:
	// 清除Task选择信息
	void ClearTaskSelection();

	// 选中Task
	void ChangeTaskSelection(const TArray<class UBXTask*>& Tasks);

	// 改变Task的名称
	void ChangeTaskName(class UBXTask* TheTask, const FText& NewName);

	// 改变Task轨道位置
	void ChangeTaskPosition(class UBXTask* SrcTask, class UBXTask* DestTask);

	// 改变Task所属的Group
	void ChangeTaskGroup(class UBXTask* SrcTask, struct FBXTLTaskGroup& DestGroupData);

	// 创建Task组
	void AddNewTaskGroup(const FText& InGroupName);

	// 删除Task组
	void DeleteTaskGroup(struct FBXTLTaskGroup& InGroup);

	// 创建Task
	class UBXTask* AddNewTask(struct FBXTLTaskGroup& InGroup, UClass* InTaskClass, float StartTime = 0.0f);

	// 创建Task对应的图表节点
	void CreateTaskGraphNode(class UBXTask* TheTask);

	// 删除Task
	void DeleteTask(class UBXTask* TheTask);
	
	// 删除选中的Task
	void DeleteSelectedTasks();

	// 获取选中的Tasks
	TArray<class UBXTask*> GetSelectedTasks();

	// 复制选中的Tasks
	void CopySelectedTasks();

	// 粘贴选中的Tasks
	void PasteSelectedTasks(FString InPasteMsg, int32 InGroupID);

	// 导出选中的Tasks到TaskTemplate
	void ExportSelectedTaskTemplate();

	// 导入对应名称的TaskTemplate
	void ImportTaskTemplate(FName InTemplateName, int32 InGroupID);

private:
	// 根据Group指针查询GroupID
	int32 FindGroupID(struct FBXTLTaskGroup& InGroup);

	void InternalExportSelectedTaskTemplate();

	// 保存Template名称
	FText TemplateName;

	// 设置模板名称的UI
	TSharedPtr<SWindow> TemplateNameWindow;

#pragma endregion Task



#pragma region Callback
public:
	// 完成自动优化事件回调
	void ReceiveAutoOptimizeDone(class UBXTLAsset* Asset);

	// 属性变更事件回调
	void OnAssetPropertyChange(FName PropertyName);

#pragma endregion Callback

};