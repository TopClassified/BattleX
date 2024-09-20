#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "BXTEnums.h"
#include "BXTStructs.h"

#include "BXTask.generated.h"



UCLASS(Abstract, Blueprintable, EditInlineNew)
class BATTLEX_API UBXTask : public UObject
{
	GENERATED_BODY()

#pragma region Important
public:
	UBXTask();

public:
	// 网络类型
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Important", Meta = (Bitmask, BitmaskEnum = "EBXTNetType"))
	int32 NetTypes = 7;

	// 触发方式
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Important", Meta = (Bitmask, BitmaskEnum = "EBXTTriggerType"))
	int32 TriggerTypes = 1;

	// 开始时间
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Important")
	float StartTime = 0.0f;

	// 生命周期类型
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Important")
	EBXTLifeType LifeType = EBXTLifeType::L_Instant;

	// 持续时长
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Important")
	float Duration = 1.0f;

	// 选取的目标
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Important", Meta = (Bitmask, BitmaskEnum = "EBXTTargetType"))
	int32 TargetTypes = 1;
	// 如果目标类型包含"碰撞目标"，在这里添加碰撞数据来源
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Important", Meta = (EditCondition = "(TargetTypes & 16) > 0", EditConditionHides))
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
	TMap<FName, FBXTEvent> Events;

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
protected:
	// 编辑器注释
	UPROPERTY(EditDefaultsOnly, Category = "Editor")
	FText Annotation;
#endif

#if WITH_EDITOR
public:
	virtual void AlignTimeProperty(float InAlign);

	virtual FText GetDisplayName() const;

	virtual FText GetAnnotation() const;

	virtual void CopyDataFromOther(UBXTask* Other);

	virtual bool RefreshProperty();
	UFUNCTION(BlueprintImplementableEvent)
	bool ScriptRefreshProperty();

	UFUNCTION(BlueprintNativeEvent)
	void RefreshDataByPreviewObject(UObject* InObject, const struct FBXTLPreviewObjectData& InData);

	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
#pragma endregion Editor

};
