#pragma once

#include "CoreMinimal.h"



// ���ˢ���¼�
DECLARE_DELEGATE(FBXTLESlateRefreshPanel);

// ���ѡȡ�ڵ��¼�
DECLARE_DELEGATE(FBXTLESlateSelectNode);

// ���ȡ��ѡȡ���нڵ��¼�
DECLARE_DELEGATE(FBXTLESlateDeselectNodes);

// �����������¼�
DECLARE_DELEGATE_TwoParams(FBXTLESlateAddTask, UClass*, float)

// ���ɾ�������¼�
DECLARE_DELEGATE(FBXTLESlateDeleteTask);

// ��忽�������¼�
DECLARE_DELEGATE(FBXTLESlateCopyTask);

// ���ճ�������¼�
DECLARE_DELEGATE(FBXTLESlatePasteTask);

// ��嵼������ģ���¼�
DECLARE_DELEGATE(FBXTLESlateExportTemplate);

// �����ק����ڵ��¼�
DECLARE_DELEGATE_RetVal_FourParams(FReply, FBXTLESlateDragTTN, TSharedRef<class SBXTLTaskTrackNode>, const FPointerEvent&, const FVector2D&, const bool)

// �����ק����ڵ��¼�
DECLARE_DELEGATE_RetVal_FourParams(FReply, FBXTLESlateDragETN, TSharedRef<class SBXTLExtraTrackNode>, const FPointerEvent&, const FVector2D&, const bool)



// �߼�ˢ������¼�
DECLARE_MULTICAST_DELEGATE(FBXTLELogicRefreshPanel);

// �߼��ı�Ԥ��״̬�¼�
DECLARE_MULTICAST_DELEGATE_TwoParams(FBXTLELogicPreviewChanged, bool, bool);

// �߼��ı�ѡȡ�����¼�
DECLARE_MULTICAST_DELEGATE_OneParam(FBXTLELogicTaskSelected, TArray<class UBXTask*>&);
