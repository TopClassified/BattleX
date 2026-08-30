// =============================================================================
//                        行为/状态 运行状态复制快照
//                       (运行数据投影方案,同技能系统形态)
//
//  方案说明(详细时序图见 BXSkillReplicated.h 头注释,机制完全一致):
//
//    服务器: 组件 PreReplication(每帧、属性收集前)在远程连接数增加(新客户端
//            连入)时从 ActiveBehaviors/ActiveStates 全量重建快照数组。
//    通道:   快照数组 COND_InitialOnly——仅新连接初始同步发送,已有连接零属性流量;
//            已有连接的行为/状态动态由显式 RPC 维护:
//              行为  MulticastBehaviorEnter / MulticastBehaviorExit / MulticastControlBehavior
//              状态  MulticastStateEnter    / MulticastStateExit
//            (均 Reliable;RPC 与快照的乱序竞态由 OnRep 差分兜底)。
//    客户端: OnRep(带旧值)差分 → 新增条目重建(Late Join)/消失条目兜底清理。
//
//    UE5.4 已移除 FastArray 虚回调机制且无服务器端发送前钩子,不采用 FastArray。
// =============================================================================

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#include "BXStateBehaviorReplicated.generated.h"



// 行为复制条目级标志位(Flags):bit0=中断中(LateJoin重建不启代理,等控制包恢复)
#define BX_SYNC_FLAG_BEHAVIOR_STOPPED 0x01



// 行为复制条目:行为Tag + 来源签名列表 + 条目级状态标志
USTRUCT()
struct FBXBehaviorReplicatedState
{
	GENERATED_USTRUCT_BODY()

public:
	// 行为Tag(事实表键)
	UPROPERTY()
	FGameplayTag BehaviorTag;

	// 来源签名列表(Sign=0系统常驻同样传输)
	UPROPERTY()
	TArray<int64> Signs;

	// 条目级标志(BX_SYNC_FLAG_BEHAVIOR_*:bit0=代理未启动——LateJoin重建时不自动Start)
	UPROPERTY()
	uint8 Flags = 0;
};



// 状态来源同步条目(最小投影:剩余时长代替流逝时间,重建时以接收时刻为计时零点)
USTRUCT()
struct FBXStateSyncSource
{
	GENERATED_USTRUCT_BODY()

public:
	// 来源签名(SkillID/TLID/0系统)
	UPROPERTY()
	int64 Sign = 0;

	// 发送时刻的剩余时长(秒,≤0无限;投影即冻结,不随网络延迟缩水)
	UPROPERTY()
	float RemainingDuration = -1.0f;
};



// 状态复制条目:状态Tag + 来源列表
USTRUCT()
struct FBXStateReplicatedState
{
	GENERATED_USTRUCT_BODY()

public:
	// 状态Tag(事实表键)
	UPROPERTY()
	FGameplayTag StateTag;

	// 来源列表(独立计时先到先退)
	UPROPERTY()
	TArray<FBXStateSyncSource> Sources;
};
