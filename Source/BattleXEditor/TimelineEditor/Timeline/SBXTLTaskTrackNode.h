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



struct FBXTLTaskNodeData
{
	// 设置Node对应的Task对象
	void SetTaskNodeData(class UBXTask* InTask);

	// 获取Task对象
	class UBXTask* GetTask() const;

	// 获取Task名称
	FString GetTaskName() const;

	// 获取颜色
	TOptional<FLinearColor> GetColor() const;

	// 获取开始时间
	float GetStartTime() const;

	// 设置开始时间
	void SetStartTime(float Time);

	// 获取持续时间
	float GetDuration() const;

	// 设置持续时间
	void SetDuration(float InDuration);

	// 设置预览帧率
	void SetPreviewFrameRate(float InPreviewFrameRate);

private:
	// 缓存的Task对象
	TWeakObjectPtr<class UBXTask> CachedTask = nullptr;

};






class SBXTLTaskTrackNode : public SLeafWidget
{
#pragma region Important
public:
	friend class SBXTLTaskTrack;

	SLATE_BEGIN_ARGS(SBXTLTaskTrackNode) :_ViewInputMin(), _ViewInputMax(), _OnRefreshPanel(), _OnStartDragTTN() {}
	SLATE_ARGUMENT(class UBXTask*, Task)
	SLATE_ARGUMENT(TWeakPtr<class FBXTLController>, Controller)
	SLATE_ATTRIBUTE(float, ViewInputMin)
	SLATE_ATTRIBUTE(float, ViewInputMax)
	SLATE_ATTRIBUTE(float, TimelinePlayLength)
	SLATE_EVENT(FBXTLRefreshPanel, OnRefreshPanel)
	SLATE_EVENT(FBXTLStartDragTTN, OnStartDragTTN)
	SLATE_END_ARGS()

	void Construct(const FArguments& Declaration);

	virtual ~SBXTLTaskTrackNode();

	void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

	FBXTLTaskNodeData* GetTaskNodeDataPtr();

	const FBXTLTaskNodeData& GetTaskNodeData() const;

private:
	enum EDragType
	{
		StartTime,
		Duration,
		None
	};

	FBXTLTaskNodeData TaskNodeData;

	TWeakPtr<FBXTLController> Controller = nullptr;

#pragma endregion Important



#pragma region Render
public:
	// 绘制该控件
	int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const;

	// 绘制移动时的偏移虚影
	void DrawHandleOffset(const float& Offset, const float& HandleCenter, FSlateWindowElementList& OutDrawElements, int32 MarkerLayer, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FLinearColor NodeColour) const;

#pragma endregion Render



#pragma region Parameter
public:
	bool IsSelected() const;

	FText GetNotifyText() const;

	FVector2D GetSize() const;

	const FVector2D& GetScreenPosition() const;

	float GetDurationSize() const;

	FVector2D GetWidgetPosition() const;

	FVector2D GetNotifyPosition() const;

	FVector2D GetNotifyPositionOffset() const;

	FVector2D ComputeDesiredSize(float) const override;

	void UpdateSizeAndPosition(const FGeometry& AllottedGeometry);

private:
	bool bSelected;

	FSlateFontInfo Font;

	float TaskTimePositionX;

	float TaskDurationSizeX;

	FVector2D ScreenPosition;

	FVector2f TextSize;

	float LabelWidth;

	float NodeStartTime;

	float NodeDuration;

#pragma endregion Parameter



#pragma region Widget
public:
	// 接收聚焦
	FReply OnFocusReceived(const FGeometry& MyGeometry, const FFocusEvent& InFocusEvent) override;

	// 丢失聚焦
	void OnFocusLost(const FFocusEvent& InFocusEvent) override;

	// 是否支持键盘聚焦
	bool SupportsKeyboardFocus() const override;

	// 刷新拖拽类型
	void RefreshDragType(const FVector2D& CursorScreenPosition);

	// 是否正在被拖动
	bool BeingDragged() const;

	// 拖动回调
	FReply OnDragDetected(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	// 拖动被取消
	void DragCancelled();

	// 设置鼠标按下时的位置
	void SetLastMouseDownPosition(const FVector2D& CursorPosition);

	// 改变指针样式
	FCursorReply OnCursorQuery(const FGeometry& MyGeometry, const FPointerEvent& CursorEvent) const override;

private:
	int32 DragIndex;

	EDragType DragType;

	float WidgetX;

	FVector2D WidgetSize;

	TAttribute<float> ViewInputMin;

	TAttribute<float> ViewInputMax;

	float TimelinePlayLength;

	FVector2D LastMouseDownPosition;

	FVector2D CachedAllotedGeometrySize;

#pragma endregion Widget



#pragma region Callback
public:
	void OnEditorTaskSelectionChanged(TArray<class UBXTask*>& InTaskSelection);

#pragma endregion Callback



#pragma region Event
private:
	FBXTLRefreshPanel RefreshPanelEvent;

	FBXTLStartDragTTN StartDragTTNEvent;

#pragma endregion Event

};
