#pragma once

#include "CoreMinimal.h"



// 刷新面板
DECLARE_DELEGATE(FBXTLRefreshPanel);

// 选取节点
DECLARE_DELEGATE(FBXTLSelectNode);

// 取消选取所有节点
DECLARE_DELEGATE(FBXTLDeselectNodes);

// 添加Task
DECLARE_DELEGATE_TwoParams(FBXTLAddTask, UClass*, float)

// 删除Task
DECLARE_DELEGATE(FBXTLDeleteTask);

// 拷贝Task
DECLARE_DELEGATE(FBXTLCopyTask);

// 粘贴Task
DECLARE_DELEGATE(FBXTLPasteTask);

// 导出Task模板
DECLARE_DELEGATE(FBXTLExportTemplate);

// 刷新时间轴
DECLARE_MULTICAST_DELEGATE(FBXTLRefreshSections);

// 开始拖拽Task节点
DECLARE_DELEGATE_RetVal_FourParams(FReply, FBXTLStartDragTTN, TSharedRef<class SBXTLTaskTrackNode>, const FPointerEvent&, const FVector2D&, const bool)

// 开始拖拽额外节点
DECLARE_DELEGATE_RetVal_FourParams(FReply, FBXTLStartDragETN, TSharedRef<class SBXTLExtraTrackNode>, const FPointerEvent&, const FVector2D&, const bool)

// 预览状态改变事件
DECLARE_MULTICAST_DELEGATE_TwoParams(FBXTLPreviewStateChanged, bool, bool);

// Task选取发生改变
DECLARE_MULTICAST_DELEGATE_OneParam(FBXTLTaskSelectionChanged, TArray<class UBXTask*>&);
