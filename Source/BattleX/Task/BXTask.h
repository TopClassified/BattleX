#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "BXTEnums.h"
#include "BXTStructs.h"

#include "BXTask.generated.h"



#if WITH_EDITOR
DECLARE_MULTICAST_DELEGATE(FBXTRefreshInputOutput);
#endif



UCLASS(Abstract, Blueprintable, EditInlineNew)
class BATTLEX_API UBXTask : public UObject
{
	GENERATED_BODY()

#pragma region Important
public:
	UBXTask();

public:
	// 网络类型
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Important", Meta = (Bitmask, BitmaskEnum = "/Script/BattleX.EBXTNetType"))
	int32 NetTypes = 7;

	// 触发方式
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Important", Meta = (Bitmask, BitmaskEnum = "/Script/BattleX.EBXTTriggerType"))
	int32 TriggerTypes = 1;

	// 开始时间
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Important")
	float StartTime = 0.0f;

	// 生命周期类型
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Important")
	EBXTLifeType LifeType = EBXTLifeType::L_Instant;

	// 持续时长
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Important", Meta = (ClampMin = "0.05"))
	float Duration = 1.0f;

	// 选取的目标
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Important", Meta = (Bitmask, BitmaskEnum = "/Script/BattleX.EBXTTargetType"))
	int32 TargetTypes = 1;
	// 如果目标类型包含"碰撞目标"，在这里添加碰撞数据来源
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Important", Meta = (EditCondition = "bNeedCollisionInput", EditConditionHides))
	TArray<FBXTInputInfo> CollisionInputDatas;

#pragma endregion Important



#pragma region RunTimeData
public:
	// 该Task的数据读取(获取"XXX"Task生产的名为"xxx"的数据)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "RunTimeData")
	TArray<FBXTInputInfo> InputDatas;

	// 该Task的数据输出(生产名为"xxx"的"xxx"类型的数据)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "RunTimeData")
	TArray<FBXTOutputInfo> OutputDatas;

#pragma endregion RunTimeData



#pragma region Event
public:
	// 事件触发Task列表
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Event")
	TMap<FGameplayTag, FBXTEvent> Events;

#pragma endregion Event



#pragma region Resource
public:
	// 收集引用资源
	virtual void CollectResources(TArray<FSoftObjectPath>& OutResources) {}

	// 收集蓝图引用资源
	UFUNCTION(BlueprintImplementableEvent)
	void CollectBPResources(TArray<FSoftObjectPath>& OutResources);

#pragma endregion Resource



#pragma region Editor
#if WITH_EDITORONLY_DATA
public:
	// 编辑器名称
	UPROPERTY(EditDefaultsOnly, Category = "Editor")
	FText DisplayName;

	// 编辑器注释
	UPROPERTY(EditDefaultsOnly, Category = "Editor")
	FText Annotation;

	// 被哪些任务触发
	UPROPERTY(VisibleDefaultsOnly, Category = "Event")
	TArray<TSoftObjectPtr<UBXTask>> TriggeredByList;

	// 是否需呀碰撞信息
	UPROPERTY(VisibleDefaultsOnly, Category = "Important", AdvancedDisplay)
	bool bNeedCollisionInput = false;
#endif

public:
	// 刷新脚本属性
	UFUNCTION(BlueprintImplementableEvent)
	bool ScriptRefreshProperty();
	
	// 是否需要烘焙数据
	UFUNCTION(BlueprintImplementableEvent)
	bool NeedBakeData();

	// 清理烘焙的旧数据
	UFUNCTION(BlueprintNativeEvent)
	void CleanBakedData();
	virtual void CleanBakedData_Implementation();
	
	// 烘焙数据
	UFUNCTION(BlueprintNativeEvent)
	void BakingData(const FBXTLRunTimeData& InOutRTData, const FBXTLSectionRTData& InOutRTSData, const FBXTLTaskRTData& InOutRTTData);
	virtual void BakingData_Implementation(const FBXTLRunTimeData& InOutRTData, const FBXTLSectionRTData& InOutRTSData, const FBXTLTaskRTData& InOutRTTData);

	// 烘焙数据后处理
	UFUNCTION(BlueprintNativeEvent)
	void PostBakeData();
	virtual void PostBakeData_Implementation();
	
	// 根据动态数据，获取动态对象信息
	UFUNCTION(BlueprintNativeEvent)
	void GetDynamicObjectByRuntimeData(class UBXManager* InBXMgr, const FBXTLRunTimeData& InOutRTData, const FBXTLSectionRTData& InOutRTSData, const FBXTLTaskRTData& InOutRTTData);
	virtual void GetDynamicObjectByRuntimeData_Implementation(class UBXManager* InBXMgr, const FBXTLRunTimeData& InOutRTData, const FBXTLSectionRTData& InOutRTSData, const FBXTLTaskRTData& InOutRTTData);
	
#if WITH_EDITOR
public:
	virtual void PreSave(FObjectPreSaveContext SaveContext) override;
	
	virtual void AlignTimeProperty(float InAlign);

	virtual FText GetDisplayName() const;

	virtual FText GetAnnotation() const;

	virtual void CopyDataFromOther(UBXTask* Other);

	virtual bool RefreshProperty();

	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
	
	virtual void RefreshTransformCreaters();

	// 允许被动触发
	virtual bool EnablePassiveTrigger();

public:
	FBXTRefreshInputOutput RefreshInputOutput;
#endif
#pragma endregion Editor

};
