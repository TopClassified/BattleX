#pragma once

#include "CoreMinimal.h"
#include "EditorViewportClient.h"



class BATTLEXEDITOR_API FSimpleEditorViewportClient : public FEditorViewportClient
{
#pragma region Important
public:
	FSimpleEditorViewportClient(FEditorModeTools* InModeTools, FPreviewScene* InPreviewScene = nullptr, const TWeakPtr<SEditorViewport>& InEditorViewportWidget = nullptr);

	virtual void Tick(float DeltaSeconds) override;

public:
	FColor FadeColor;

	float FadeAmount;

	bool bEnableFading;

private:
	FEditorModeTools* ModelTools = nullptr;

#pragma endregion Important



#pragma region Render
public:
	// 重载窗口绘制
	virtual void Draw(FViewport* InViewport, FCanvas* Canvas) override;

	// 设置窗口的渲染参数
	virtual void SetupViewForRendering(FSceneViewFamily& ViewFamily, FSceneView& View) override;

	// 设置窗口绘制碰撞参数
	virtual void SetEngineShowFlagCollision(bool bShowCollision);

#pragma endregion Render



#pragma region Logic
public:
	// 处理点击事件
	virtual void ProcessClick(FSceneView& View, HHitProxy* HitProxy, FKey Key, EInputEvent Event, uint32 HitX, uint32 HitY) override;

	// 透视摄像机移动时触发的回调
	virtual void PerspectiveCameraMoved() override;

	// 追踪鼠标移动停止时的回调
	virtual void TrackingStopped() override;

	// 检查对象是否可以在窗口中移动
	bool CheckActorCanMove(const AActor* InActor) const;

	// 从编辑器已选的组件中获取可移动的对象和组件
	void GetSelectedActorsAndComponentsForMove(TArray<AActor*>& OutActorsToMove, TArray<USceneComponent*>& OutComponentsToMove) const;

	// 得到锁定的对象
	AActor* GetLockedActor() const;

	// 设置锁定的对象
	void SetLockedActor(AActor* InActor);

	// 检查对象是否被锁定
	bool CheckActorIsLocked(const TWeakObjectPtr<AActor> TestActor) const;

	// 是否有对象被锁定
	bool HasAnyActorLocked() const;

protected:
	// 获取被选中的对象
	AActor* GetSelectedActor() const;

	// 获取被选中的组件
	USceneComponent* GetSelectedComponent() const;

private:
	void LockActorInternal(AActor* InActor);

	// 移动窗口到被锁定的对象
	void MoveViewportToLockedActor();

	// 移动被锁定的对象到窗口
	void MoveLockedActorToViewport();

	// 窗口内对象发生移动的回调函数
	void OnActorMoved(AActor* InActor);

private:
	TWeakObjectPtr<AActor> LockedActor;

#pragma endregion Logic



#pragma region Widget
public:
	// 获取Transform编辑部件的位置
	virtual FVector GetWidgetLocation() const override;

	// 获取Transform编辑部件的坐标矩阵
	virtual FMatrix GetWidgetCoordSystem() const override;

	// 设置Transform编辑部件的空间坐标系类型
	virtual void SetWidgetCoordSystemSpace(ECoordSystem NewCoordSystem) override;

	// 获取Transform编辑部件的空间坐标系类型
	virtual ECoordSystem GetWidgetCoordSystemSpace() const override;

	// 获取Transform编辑部件的模式
	virtual void SetWidgetMode(UE::Widget::EWidgetMode NewMode) override;

	// 设置Transform编辑部件的模式
	virtual UE::Widget::EWidgetMode GetWidgetMode() const override;

	// 应用Transform编辑部件的坐标增量
	virtual bool InputWidgetDelta(FViewport* InViewport, EAxisList::Type CurrentAxis, FVector& Drag, FRotator& Rot, FVector& Scale) override;

private:
	UE::Widget::EWidgetMode WidgetMode = UE::Widget::EWidgetMode::WM_Translate;

	ECoordSystem WidgetCoordSystem = ECoordSystem::COORD_World;

#pragma endregion Widget

};

