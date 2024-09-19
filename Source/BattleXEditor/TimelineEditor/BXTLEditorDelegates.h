#pragma once

#include "CoreMinimal.h"



// 面板刷新事件
DECLARE_DELEGATE(FBXTLESlateRefreshPanel);

// 面板选取节点事件
DECLARE_DELEGATE(FBXTLESlateSelectNode);

// 面板取消选取所有节点事件
DECLARE_DELEGATE(FBXTLESlateDeselectNodes);

// 面板添加任务事件
DECLARE_DELEGATE_TwoParams(FBXTLESlateAddTask, UClass*, float)

// 面板删除任务事件
DECLARE_DELEGATE(FBXTLESlateDeleteTask);

// 面板拷贝任务事件
DECLARE_DELEGATE(FBXTLESlateCopyTask);

// 面板粘贴任务事件
DECLARE_DELEGATE(FBXTLESlatePasteTask);

// 面板导出任务模板事件
DECLARE_DELEGATE(FBXTLESlateExportTemplate);

// 面板拖拽任务节点事件
DECLARE_DELEGATE_RetVal_FourParams(FReply, FBXTLESlateDragTTN, TSharedRef<class SBXTLTaskTrackNode>, const FPointerEvent&, const FVector2D&, const bool)

// 面板拖拽额外节点事件
DECLARE_DELEGATE_RetVal_FourParams(FReply, FBXTLESlateDragETN, TSharedRef<class SBXTLExtraTrackNode>, const FPointerEvent&, const FVector2D&, const bool)



// 逻辑刷新面板事件
DECLARE_MULTICAST_DELEGATE(FBXTLELogicRefreshPanel);

// 逻辑改变预览状态事件
DECLARE_MULTICAST_DELEGATE_TwoParams(FBXTLELogicPreviewChanged, bool, bool);

// 逻辑改变选取任务事件
DECLARE_MULTICAST_DELEGATE_OneParam(FBXTLELogicTaskSelected, TArray<class UBXTask*>&);
