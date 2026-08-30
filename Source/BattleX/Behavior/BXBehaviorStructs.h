#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "StructUtils/InstancedStruct.h"

#include "BehaviorProxy/BXBehaviorProxy.h"

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



// 禁止账本来源条目(挡启动的持续禁令;多来源叠加,最后一个移除才失效)
USTRUCT(BlueprintType)
struct FBXBehaviorForbidSource
{
	GENERATED_USTRUCT_BODY()

public:
	FBXBehaviorForbidSource() {}
	FBXBehaviorForbidSource(const FGameplayTag& InSourceTag, int64 InSign) : SourceTag(InSourceTag), Sign(InSign) {}

	bool operator==(const FBXBehaviorForbidSource& Other) const
	{
		return SourceTag == Other.SourceTag && Sign == Other.Sign;
	}

public:
	// 来源标识(状态Tag/在位行为Tag/系统来源Tag)
	UPROPERTY(Transient, BlueprintReadOnly)
	FGameplayTag SourceTag;

	// 来源签名(配对解除;矩阵贡献=0)
	UPROPERTY(Transient, BlueprintReadOnly)
	int64 Sign = 0;
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



// 行为代理配置(Tag→代理类;代理出生即启用,禁用只由禁止原子造成)
USTRUCT(BlueprintType)
struct FBXBehaviorProxyConfig
{
	GENERATED_USTRUCT_BODY()

public:
	// 代理类
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UBXBehaviorProxy> ProxyClass;
};
