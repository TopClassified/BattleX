// =============================================================================
//                             技能运行状态复制快照
//                            (运行数据投影方案)
//
//  ┌─ UE 属性复制时序 ─────────────────────────────────────────────────────┐
//  │                                                                        │
//  │  服务器每帧:                                                            │
//  │    1. World::Tick                                                      │
//  │         └─ SkillManager::Tick 推进 SkillRTDatas                        │
//  │            (运行数据在此持续变化)                                        │
//  │                                                                        │
//  │    2. NetDriver::Tick (对每个连接 × 相关Actor)                          │
//  │         ├─ 相关性判定                                                   │
//  │         │    (首次相关 = 打开ActorChannel = 本帧做初始同步)               │
//  │         ├─ 组件::PreReplication    (引擎钩子, 每帧、收集前)              │
//  │         ├─ 属性收集               (读取复制属性【当前存储值】)             │
//  │         ├─ 与 per-connection shadow 比较                                │
//  │         │    (变化的属性打包bunch)                                       │
//  │         └─ Flush 发送 (属性bunch + RPC bunch)                           │
//  │                                                                        │
//  │  客户端:                                                                │
//  │    新Actor    → 组件创建 → 初始属性应用(含COND_InitialOnly)              │
//  │               → OnRep 回调 (重建时机) → BeginPlay                       │
//  │    已有Actor  → delta属性 → OnRep                                       │
//  │                                                                        │
//  └────────────────────────────────────────────────────────────────────────┘
//
//  ┌─ 运行数据新鲜度问题 ────────────────────────────────────────────────────┐
//  │                                                                        │
//  │  属性复制发送的是"属性里存的值", 引擎不会替你生成值:                        │
//  │                                                                        │
//  │    条目在播放时写入一次                                                  │
//  │      → 运行数据持续变化, 而复制品是死的                                   │
//  │      → 新连接初始同步拿到的是写入时刻的旧值(进度≈0)                        │
//  │      → 与服务器实际进度相差整个已播时长                                   │
//  │                                                                        │
//  └────────────────────────────────────────────────────────────────────────┘
//
//  ┌─ 本文件方案: COND_InitialOnly + PreReplication刷新            ← 采用    │
//  │                                                                        │
//  │    服务器: 组件PreReplication(每帧、收集前)从SkillRTDatas重建快照数组,   │
//  │            数组标记COND_InitialOnly(仅初始同步发送,已有连接零属性流量)    │
//  │    新连接: 初始同步拿到"打开通道时刻"的最新运行数据投影                    │
//  │    已有连接: 技能动态维护完全由显式RPC驱动                               │
//  │            (MulticastPlaySkill / MulticastStopSkill, 均Reliable)        │
//  │    客户端: OnRep(带旧值)差分 → 新增条目重建 / 消失条目兜底停止            │
//  │                                                                        │
//  │    备注: UE5.4已移除FastArray的虚回调机制(FFastArraySerializer非多态),  │
//  │          且无服务器端发送前钩子, 故不采用FastArray方案。                   │
//  │                                                                        │
//  └────────────────────────────────────────────────────────────────────────┘
// =============================================================================

#pragma once

#include "CoreMinimal.h"

#include "BXNetEnums.h"
#include "Timeline/BXTLReplicated.h"

#include "BXSkillReplicated.generated.h"



// 技能复制条目:技能标识 + 发送时刻的运行数据投影
USTRUCT()
struct FBXSkillReplicatedState
{
	GENERATED_USTRUCT_BODY()

public:
	// 技能实例ID(与SkillRTDatas键一致,去重/停止定位)
	UPROPERTY()
	int64 SkillID = 0;

	// 发起端类型
	UPROPERTY()
	EBXSyncInitiator Initiator = EBXSyncInitiator::Client;

	// 运行数据投影(服务器PreReplication现场填充最新状态)
	UPROPERTY()
	FBXTLRunTimeDataReplicated TLRunTimeData;
};
