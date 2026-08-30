#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#include "BXStateEnums.h"

#include "BXStateStructs.generated.h"



class UAnimMontage;
class UBXSkillAsset;
class UBXTLAsset;



// 状态来源(来源独立计时,先到先退)
USTRUCT(BlueprintType)
struct FBXStateSource
{
	GENERATED_USTRUCT_BODY()

public:
	FBXStateSource() {}
	FBXStateSource(int64 InSign, float InDuration) : Sign(InSign), Duration(InDuration) {}

	bool operator==(const FBXStateSource& Other) const
	{
		return Sign == Other.Sign;
	}

public:
	// 来源签名(技能SkillID/时间轴TLID/0状态机自身)
	UPROPERTY(Transient, BlueprintReadOnly)
	int64 Sign = 0;

	// 状态时长(≤0无限)
	UPROPERTY(Transient, BlueprintReadOnly)
	float Duration = -1.0f;

	// 已持续时间
	UPROPERTY(Transient, BlueprintReadOnly)
	float ElapsedTime = 0.0f;
};



// 状态运行时数据(事实表条目)
USTRUCT(BlueprintType)
struct FBXStateRuntimeData
{
	GENERATED_USTRUCT_BODY()

public:
	bool HasSource(int64 InSign) const
	{
		for (const FBXStateSource& Source : Sources)
		{
			if (Source.Sign == InSign)
			{
				return true;
			}
		}

		return false;
	}

	// 最小剩余时间(来源独立计时;含无限来源返回浮点最大值=永不到期,无来源返回-1)
	// 旧实现-1哨兵与"负数已超时/无条目-1"混叠,且破坏数值比较(如Remaining<X条件对无限态误判为真),无限改显式最大值
	float GetRemainingTime() const
	{
		bool bHasFinite = false;
		float MinRemaining = -1.0f;
		for (const FBXStateSource& Source : Sources)
		{
			// 含无限来源→整体无限(数值比较安全:任何"剩余<X"判定对无限态恒假)
			if (Source.Duration <= 0.0f)
			{
				return TNumericLimits<float>::Max();
			}

			const float Remaining = Source.Duration - Source.ElapsedTime;
			if (!bHasFinite || Remaining < MinRemaining)
			{
				MinRemaining = Remaining;
				bHasFinite = true;
			}
		}

		return MinRemaining;
	}

	// 是否含无限时长来源(永不到期;到期判定用,区分-1无限与负数已超时)
	bool HasInfiniteSource() const
	{
		for (const FBXStateSource& Source : Sources)
		{
			if (Source.Duration <= 0.0f)
			{
				return true;
			}
		}

		return false;
	}

public:
	// 状态Tag
	UPROPERTY(Transient, BlueprintReadOnly)
	FGameplayTag Tag;

	// 来源列表(空代表不存在)
	UPROPERTY(Transient, BlueprintReadOnly)
	TArray<FBXStateSource> Sources;
};



// 状态表现(三通道,详情面板按Type显隐对应资产属性)
USTRUCT(BlueprintType)
struct FBXStatePresentation
{
	GENERATED_USTRUCT_BODY()

public:
	// 配置完整性:通道已选则对应资产路径必须已配置(仅查路径非空,加载失败由触发期诊断)
	bool IsValid() const
	{
		switch (Type)
		{
		case EBXPresentationType::PT_Skill:
			return !SkillAsset.IsNull();
		case EBXPresentationType::PT_Timeline:
			return !TimelineAsset.IsNull();
		case EBXPresentationType::PT_Animation:
			return !MontageAsset.IsNull();
		default:
			return false;
		}
	}

public:
	// 表现通道(仅选中通道对应的资产属性在详情面板显示)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EBXPresentationType Type = EBXPresentationType::PT_None;

	// 技能资源(自带同步体系)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "Type==EBXPresentationType::PT_Skill", EditConditionHides))
	TSoftObjectPtr<class UBXSkillAsset> SkillAsset;

	// 时间轴资源(不同步的技能,纯表现)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "Type==EBXPresentationType::PT_Timeline", EditConditionHides))
	TSoftObjectPtr<class UBXTLAsset> TimelineAsset;

	// 纯动画Montage
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "Type==EBXPresentationType::PT_Animation", EditConditionHides))
	TSoftObjectPtr<class UAnimMontage> MontageAsset;
};



// 裸状态配置(非族状态:Buff/标记类,无状态机)
USTRUCT(BlueprintType)
struct FBXStateConfig
{
	GENERATED_USTRUCT_BODY()

public:
	// 默认时长(外部携带优先,≤0无限)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Duration = -1.0f;

	// 存续期中断的行为(停运在跑,不挡启动;Tag层级:禁BXBehavior根=全禁,禁族Tag=禁族)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTagContainer InterruptBehaviors;

	// 存续期禁止的行为(挡启动;Tag层级同上)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTagContainer ForbidBehaviors;

	// 进入表现
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FBXStatePresentation EntryPresentation;

	// 退出表现(预测回滚强制不触发)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FBXStatePresentation ExitPresentation;
};
