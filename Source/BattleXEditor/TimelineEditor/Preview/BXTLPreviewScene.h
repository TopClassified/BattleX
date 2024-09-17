#pragma once

#include "RHI.h"
#include "AdvancedPreviewScene.h"
#include "Engine/GameViewportClient.h"



class FBXTLPreviewScene : public FAdvancedPreviewScene
{
#pragma region Important
public:
	FBXTLPreviewScene(ConstructionValues CVS, TSharedPtr<class FBXTLEditor> InEditor);

	virtual ~FBXTLPreviewScene();

	void OnCreateViewport(class SBXTLEditorViewport* InEditorViewport, TSharedPtr<class FSceneViewport> InSceneViewport);

	virtual void Tick(float DeltaTime) override;

	void Finish();

	void AddReferencedObjects(FReferenceCollector& Collector);

private:
	TWeakPtr<class FBXTLEditor> CachedEditor = nullptr;

	TWeakObjectPtr<AActor> FloorOwner = nullptr;

#pragma endregion Important



public:
	// 初始化预览世界
	void InitPreviewWorld();

	// 重置预览世界
	void ResetPreviewWorld();

	// 创建预览对象
	void SpawnPreviewActors();

	// 销毁预览对象
	void DestroyPreviewActors();

	// 获取预览玩家对象
	AActor* GetPlayerActor() const;

	// 获取预览目标对象
	AActor* GetTargetActor() const;

	// 获取选取的对象
	AActor* GetSelectedActor() const;

	// 获取所有的预览对象
	TArray<TWeakObjectPtr<AActor>> GetPreviewActors() const;

protected:
	// 加载地图
	class ULevelStreaming* LoadExternalMap(TSoftObjectPtr<UWorld> NewMap);

	// 对象发生移动
	void OnActorMoved(AActor* InActor);

	// 组件发生移动
	void OnComponentMoved(USceneComponent* InComponent, ETeleportType InType);

private:
	TWeakObjectPtr<ULevelStreaming> ExternalLevel = nullptr;

	TWeakObjectPtr<ULevelStreaming> ExternalLevelAdd = nullptr;

	TWeakObjectPtr<APlayerController> PlayerController = nullptr;

	TWeakObjectPtr<AActor> PreviewPlayer = nullptr;

	TWeakObjectPtr<AActor> PreviewTarget = nullptr;

	TArray<TWeakObjectPtr<AActor>> PreviewActors;

#pragma endregion Preview

};
