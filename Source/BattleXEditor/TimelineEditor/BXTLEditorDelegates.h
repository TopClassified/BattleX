#pragma once

#include "CoreMinimal.h"



// ˢ�����
DECLARE_DELEGATE(FBXTLRefreshPanel);

// ѡȡ�ڵ�
DECLARE_DELEGATE(FBXTLSelectNode);

// ȡ��ѡȡ���нڵ�
DECLARE_DELEGATE(FBXTLDeselectNodes);

// ���Task
DECLARE_DELEGATE_TwoParams(FBXTLAddTask, UClass*, float)

// ɾ��Task
DECLARE_DELEGATE(FBXTLDeleteTask);

// ����Task
DECLARE_DELEGATE(FBXTLCopyTask);

// ճ��Task
DECLARE_DELEGATE(FBXTLPasteTask);

// ����Taskģ��
DECLARE_DELEGATE(FBXTLExportTemplate);

// ˢ��ʱ����
DECLARE_MULTICAST_DELEGATE(FBXTLRefreshSections);

// ��ʼ��קTask�ڵ�
DECLARE_DELEGATE_RetVal_FourParams(FReply, FBXTLStartDragTTN, TSharedRef<class SBXTLTaskTrackNode>, const FPointerEvent&, const FVector2D&, const bool)

// ��ʼ��ק����ڵ�
DECLARE_DELEGATE_RetVal_FourParams(FReply, FBXTLStartDragETN, TSharedRef<class SBXTLExtraTrackNode>, const FPointerEvent&, const FVector2D&, const bool)

// Ԥ��״̬�ı��¼�
DECLARE_MULTICAST_DELEGATE_TwoParams(FBXTLPreviewStateChanged, bool, bool);

// Taskѡȡ�����ı�
DECLARE_MULTICAST_DELEGATE_OneParam(FBXTLTaskSelectionChanged, TArray<class UBXTask*>&);
