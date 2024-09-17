#pragma once

#include "CoreMinimal.h"
#include "Input/Reply.h"
#include "Misc/Attribute.h"

#include "SNodePanel.h"
#include "SCurveEditor.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

#include "BXTLEditorDelegates.h"

#include "BXTLEnums.h"



namespace EExtraTrackHandleHit
{
	enum Type
	{
		Start,
		End,
		None
	};
}



class SBXTLExtraTrackNode : public SLeafWidget
{
#pragma region Important
public:
	friend class SBXTLExtraTrack;

	SLATE_BEGIN_ARGS(SBXTLExtraTrackNode) :_ViewInputMin(), _ViewInputMax(), _OnRefreshPanel(), _OnStartDragNode() {}
	SLATE_ARGUMENT(class UBXTLAsset*, Asset)
	SLATE_ARGUMENT(int32, SectionIndex)
	SLATE_ARGUMENT(EBXTLExtraType, NodeType)
	SLATE_ATTRIBUTE(float, ViewInputMin)
	SLATE_ATTRIBUTE(float, ViewInputMax)
	SLATE_ATTRIBUTE(float, TimelinePlayLength)
	SLATE_EVENT(FBXTLRefreshPanel, OnRefreshPanel)
	SLATE_EVENT(FBXTLStartDragETN, OnStartDragNode)
	SLATE_END_ARGS()

	void Construct(const FArguments& Declaration);

	virtual ~SBXTLExtraTrackNode();

	void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

private:
	TWeakObjectPtr<class UBXTLAsset> CachedAsset = nullptr;

#pragma endregion Important



#pragma region Render
public:
	// 绘制该控件
	int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const;

	// 绘制移动时的偏移虚影
	void DrawHandleOffset(const float& Offset, const float& HandleCentre, FSlateWindowElementList& OutDrawElements, int32 MarkerLayer, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FLinearColor NodeColour) const;

#pragma endregion Render



#pragma region Parameter
public:
	FText GetNotifyText() const;

	FText GetNodeTooltip() const;

	FVector2D GetSize() const;

	const FVector2D& GetScreenPosition() const;

	float GetDurationSize() const;

	FVector2D GetWidgetPosition() const;

	FVector2D GetNotifyPosition() const;

	FVector2D GetNotifyPositionOffset() const;

	FVector2D ComputeDesiredSize(float) const override;

	void UpdateSizeAndPosition(const FGeometry& AllottedGeometry);

	FText GetNodeName() const;

	float GetNodeStartTime();

	float GetNodeDuration();

	void SetNodeStartTime(float InStartTime);

	void SetNodeDuration(float InDuration);

private:
	int32 SectionIndex = -1;

	EBXTLExtraType NodeType = EBXTLExtraType::ET_TMax;

	FSlateFontInfo Font;

	float NotifyTimePositionX;

	float NotifyDurationSizeX;

	float NotifyScrubHandleCentre;

	FVector2D ScreenPosition;

	bool bDrawTooltipToRight;

	FVector2D TextSize;

	float LabelWidth;

#pragma endregion Parameter



#pragma region Widget
public:
	// 判断是否命中
	bool HitTest(const FGeometry& AllottedGeometry, FVector2D MouseLocalPose) const;

	// 接收聚焦
	FReply OnFocusReceived(const FGeometry& MyGeometry, const FFocusEvent& InFocusEvent) override;

	// 丢失聚焦
	void OnFocusLost(const FFocusEvent& InFocusEvent) override;

	// 是否支持键盘聚焦
	bool SupportsKeyboardFocus() const override;

	// 分析鼠标拖拽TaskNode的类型
	EExtraTrackHandleHit::Type DurationHandleHitTest(const FVector2D& CursorScreenPosition) const;

	// 是否正在被拖动
	bool BeingDragged() const;

	// 拖动回调
	FReply OnDragDetected(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	
	// 拖动被取消
	void DragCancelled();

	// 设置鼠标按下时的位置
	void SetLastMouseDownPosition(const FVector2D& CursorPosition);

	// 鼠标移动回调
	FReply OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	
	// 鼠标按键抬起回调
	FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	// 鼠标悬停查询
	FCursorReply OnCursorQuery(const FGeometry& MyGeometry, const FPointerEvent& CursorEvent) const override;

	float HandleOverflowPan(const FVector2D& ScreenCursorPos, float TrackScreenSpaceXPosition, float TrackScreenSpaceMin, float TrackScreenSpaceMax);

private:
	bool bBeingDragged;

	int32 DragMarkerTransactionIdx;

	EExtraTrackHandleHit::Type CurrentDragHandle;

	float WidgetX;

	FVector2D WidgetSize;

	TAttribute<float> ViewInputMin;

	TAttribute<float> ViewInputMax;

	float TimelinePlayLength;

	FVector2D LastMouseDownPosition;

	FGeometry CachedTrackGeometry;

	FVector2D CachedAllotedGeometrySize;

#pragma endregion Widget



#pragma region Event
private:
	FBXTLRefreshPanel RefreshPanelEvent;

	FBXTLStartDragETN StartDragNodeEvent;

#pragma endregion Event

};
