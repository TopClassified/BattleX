#pragma once

#include "UObject/Object.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

#include "Engine/DataAsset.h"

#include "BXTask.h"
#include "BXTLStructs.h"

#include "BXTLAsset.generated.h" 



USTRUCT()
struct FBXTLTaskGroup
{
	GENERATED_USTRUCT_BODY()

public:
	FBXTLTaskGroup() : Name() {}
	FBXTLTaskGroup(FText GroupName) : Name(GroupName) {}
	FBXTLTaskGroup(FString GroupName)
	{
		Name = FText::FromString(GroupName);
	}

public:
	UPROPERTY()
	FText Name;

	UPROPERTY()
	TArray<TWeakObjectPtr<UBXTask>> TaskList;
};



USTRUCT(BlueprintType)
struct FBXTLSection
{
	GENERATED_USTRUCT_BODY()

public:
	FBXTLSection()
	{
#if WITH_EDITOR
		Groups.Add(FBXTLTaskGroup(TEXT("Group:0")));
#endif
	}

	FBXTLSection(FText SectionName)
	{
#if WITH_EDITOR
		Name = SectionName;
		Groups.Add(FBXTLTaskGroup(TEXT("Group:0")));
#endif
	}

	FBXTLSection(FString SectionName)
	{
#if WITH_EDITOR
		Name = FText::FromString(SectionName);
		Groups.Add(FBXTLTaskGroup(TEXT("Group:0")));
#endif
	}

public:
	// 持续时间
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (ClampMin = "0.5"))
	float Duration = 1.0f;

	// 循环次数(小于等于0代表无限循环)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 LoopTime = 1;

	// 额外的时间信息
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<EBXTLExtraType, FVector2f> ExtraTime;

	// 下一个时间轴索引(大于等于0起效)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 NextIndex = -1;

	// Task列表
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced)
	TArray<UBXTask*> TaskList;

	// 关键帧列表
	UPROPERTY(BlueprintReadOnly)
	TArray<FBXTLKeyFrame> KeyFrames;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditDefaultsOnly)
	FText Name;

	UPROPERTY()
	TArray<FBXTLTaskGroup> Groups;
#endif

};



// 结构体用量直方图条目(保存期统计)
USTRUCT()
struct FBXTLStructUsageEntry
{
	GENERATED_USTRUCT_BODY()

public:
	// 结构体类型
	UPROPERTY(VisibleDefaultsOnly, Category = "LayoutCache")
	TObjectPtr<UScriptStruct> Type = nullptr;

	// 资产内使用计数
	UPROPERTY(VisibleDefaultsOnly, Category = "LayoutCache")
	int32 Count = 0;
};



UCLASS(BlueprintType, Blueprintable)
class BATTLEX_API UBXTLAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

#pragma region Important
public:
	UBXTLAsset();

public:
	// 资源ID
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Important")
	int32 ID;

	// 运行片段
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Important")
	TArray<FBXTLSection> Sections;

	// 开始时执行哪些时间片段
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Important")
	TArray<int32> StartSectionIndexes;

#if WITH_EDITORONLY_DATA
	// 逻辑图
	UPROPERTY()
	class UEdGraph* Graph = nullptr;
#endif
#pragma endregion Important



#pragma region Resource
public:
	virtual void CollectResources();

	const TArray<FSoftObjectPath>& GetAllResources() { return AllResources; }

protected:
	// 所有引用的资源路径
	UPROPERTY(VisibleDefaultsOnly, Category = "Resource")
	TArray<FSoftObjectPath> AllResources;

#pragma endregion Resource



#pragma region LayoutCache
public:
	// ===== 保存期布局缓存区(PreSave统计,运行时Reserve依据;纯性能提示,缺失或偏小不影响正确性) =====

	// 自定义结构体用量直方图(内联命中率分析与后续池化规划依据)
	UPROPERTY(VisibleDefaultsOnly, Category = "LayoutCache")
	TArray<FBXTLStructUsageEntry> CachedStructUsages;

	// 上下文数据条目预估(Σ每任务OutputDatas×2,双键写入;DynamicDatas的Reserve入参)
	UPROPERTY(VisibleDefaultsOnly, Category = "LayoutCache")
	int32 CachedContextDataEstimate = 0;

	// 各Section驻留任务数预估(非Instant计数,下标对应Sections;RunningTasks的Reserve入参)
	UPROPERTY(VisibleDefaultsOnly, Category = "LayoutCache")
	TArray<int32> CachedSectionTaskEstimates;

#if WITH_EDITOR
protected:
	// 保存期统计布局缓存(PreSave调用)
	void BuildLayoutCache();
#endif
#pragma endregion LayoutCache



#pragma region EditorProperty
#if WITH_EDITORONLY_DATA
public:
	// 名称
	UPROPERTY(EditDefaultsOnly, Category = "Edit")
	FText Name;

	// 说明
	UPROPERTY(EditDefaultsOnly, Category = "Edit")
	FText Tips;

	// 预览角色
	UPROPERTY(EditDefaultsOnly, Instanced, Category = "Edit")
	UObject* PlayerInformation = nullptr;

	// 预览目标
	UPROPERTY(EditDefaultsOnly, Instanced, Category = "Edit")
	UObject* TargetInformation = nullptr;
	
#endif
#pragma endregion EditorProperty



#pragma region Editor
#if WITH_EDITOR
public:
	// 通过编辑器打开的初始化
	virtual void InitByEditor(UObject* WorldContext);

	virtual void PreEditChange(FProperty* PropertyThatWillChange) override;

	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

	virtual void PostEditChangeChainProperty(struct FPropertyChangedChainEvent& PropertyChangedEvent) override;

	// 资源保存前的预处理
	virtual void PreSave(FObjectPreSaveContext SaveContext) override;

	// 编辑器预览前刷新数据
	virtual void RefreshDataBeforePreview();

	// 刷新时间轴信息
	virtual void RefreshSections();

	// 对齐时间属性
	virtual void AlignTimeProperty(float InAlign);

	// 清理无效的Task依赖关系
	virtual void CleanInvalidTaskDependency(UBXTask* InvalidTask);

	// 添加Task
	virtual bool AddTask(int32 SectionID, int32 GroupID, UBXTask* Task);

	// 删除Task
	virtual bool RemoveTask(int32 SectionID, int32 GroupID, UBXTask* Task);
	virtual bool RemoveTask(UBXTask* Task);

	// 查询Task所在的SectionID和GroupID
	bool GetSectionIDAndGroupID(UBXTask* Task, int32& OutSectionID, int32& OutGroupID);

	// 添加Group
	virtual bool AddGroup(int32 SectionID, FText NewGroupName);

	// 删除Group
	virtual bool RemoveGroup(int32 SectionID, int32 GroupID);

	// 添加空Section
	virtual bool AddSection(FText NewSectionName);

	// 删除Section
	virtual bool RemoveSection(int32 SectionID);

	// 修改Section名称
	virtual void ChangeSectionName(int32 SectionID, FText NewName);

	// 修改Section循环次数
	virtual void ChangeSectionLoopTime(int32 SectionID, int32 NewLoop);

	// 修改Section时长
	virtual void ChangeSectionDuration(int32 SectionID, float NewDuration);
#endif
#pragma endregion Editor

};
