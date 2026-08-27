#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "StructUtils/InstancedStruct.h"

#include "BXBehaviorEnums.h"

#include "BXBehaviorStructs.generated.h"



// 行为来源(行为生命周期归驱动方,无时长概念)
USTRUCT(BlueprintType)
struct FBXBehaviorSource
{
	GENERATED_USTRUCT_BODY()

public:
	FBXBehaviorSource() {}
	FBXBehaviorSource(int64 InSign) : Sign(InSign) {}

	bool operator==(const FBXBehaviorSource& Other) const
	{
		return Sign == Other.Sign;
	}

public:
	// 来源签名(技能SkillID/时间轴TLID/客户端SyncID,0代表系统常驻)
	UPROPERTY(Transient, BlueprintReadOnly)
	int64 Sign = 0;
};



// 行为运行时数据(事实表条目)
USTRUCT(BlueprintType)
struct FBXBehaviorRuntimeData
{
	GENERATED_USTRUCT_BODY()

public:
	bool HasSource(int64 InSign) const
	{
		for (const FBXBehaviorSource& Source : Sources)
		{
			if (Source.Sign == InSign)
			{
				return true;
			}
		}

		return false;
	}

public:
	// 行为Tag
	UPROPERTY(Transient, BlueprintReadOnly)
	FGameplayTag Tag;

	// 来源列表(空代表不存在)
	UPROPERTY(Transient, BlueprintReadOnly)
	TArray<FBXBehaviorSource> Sources;

	// 最近一次启动参数(挂起恢复时Agent回放;重启语义,重复Start覆盖)
	UPROPERTY(Transient, BlueprintReadOnly)
	FInstancedStruct LastStartParameter;
};



// 挂起遮蔽(状态禁用通道:key为状态禁用列表中的Tag,行为条目不移表,仅Agent停转+查询遮蔽)
USTRUCT()
struct FBXSuspendMask
{
	GENERATED_USTRUCT_BODY()

public:
	// 禁用它的状态Tag集合(最后一个退出才解除遮蔽)
	UPROPERTY()
	TSet<FGameplayTag> ByStates;
};



// 取消窗口保护记录
USTRUCT()
struct FBXProtectionRecord
{
	GENERATED_USTRUCT_BODY()

public:
	FBXProtectionRecord() {}
	FBXProtectionRecord(int64 InSign, bool InProtected) : Sign(InSign), bProtected(InProtected) {}

	bool operator==(const FBXProtectionRecord& Other) const
	{
		return Sign == Other.Sign;
	}

public:
	// 来源签名(技能SkillID)
	UPROPERTY()
	int64 Sign = 0;

	// 是否受保护
	UPROPERTY()
	bool bProtected = false;
};



// 行为开始检查结果
USTRUCT(BlueprintType)
struct FBXBehaviorStartCheck
{
	GENERATED_USTRUCT_BODY()

public:
	// 检查是否通过
	UPROPERTY(Transient, BlueprintReadOnly)
	bool bCanStart = true;

	// 失败原因(诊断/出招表缓冲策略)
	UPROPERTY(Transient, BlueprintReadOnly)
	FString FailReason;
};
