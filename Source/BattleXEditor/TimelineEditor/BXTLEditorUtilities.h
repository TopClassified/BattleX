#pragma once

#include "CoreMinimal.h"
#include "ClassViewerModule.h"

#include "SBXTLTaskGroupPicker.h"



// Editor公用的方法集合
class FBXTLEditorUtilities
{
public:
	// 创建Task选择器
	static void MakeNewTaskPicker(class FMenuBuilder& MenuBuilder, const FOnClassPicked& OnTaskClassPicked);

	// 创建轨道按钮
	static TSharedRef<SWidget> MakeTrackButton(FText HoverText, FOnGetContent MenuContent);

	// 创建TaskTemplate选择器
	static void MakeNewTaskTemplatePicker(class FMenuBuilder& MenuBuilder, const FBXTLPickTaskGroup& OnTemplatePicked);

	// 根据模板恢复Task关系
	static void RestoreTasksRelation(TArray<class UBXTask*>& DestTasks, const TArray<class UBXTask*>& SrcTasks);
};
