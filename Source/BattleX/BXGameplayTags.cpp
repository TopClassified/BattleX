#include "BXGameplayTags.h"



namespace BXGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXEvent_TimelineStarted, "BXEvent.TimelineStarted", "时间轴已开始");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXEvent_TimelineClosing, "BXEvent.TimelineClosing", "时间轴将结束");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXEvent_Behavior_Enter, "BXEvent.Behavior.Enter", "行为开始(条目从无到有,含挂起恢复)");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXEvent_Behavior_Exit, "BXEvent.Behavior.Exit", "行为结束(最后来源退出:挤出/挂起/回滚)");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXEvent_State_Enter, "BXEvent.State.Enter", "状态进入(条目从无到有)");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXEvent_State_Exit, "BXEvent.State.Exit", "状态退出(最后来源退出)");

	// 行为族(平铺:全部直接挂在根下,2026-09-01 整理,移除 Locomotion/Attack 分层)
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXBehavior_Root, "BXBehavior", "行为根Tag(持续行为命名空间,激活记录判定用)");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXBehavior_Walk, "BXBehavior.Walk", "走路");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXBehavior_Run, "BXBehavior.Run", "跑步");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXBehavior_Sprint, "BXBehavior.Sprint", "冲刺");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXBehavior_Jump, "BXBehavior.Jump", "跳跃(瞬间行为)");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXBehavior_Landed, "BXBehavior.Landed", "落地(瞬间行为)");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXBehavior_LowPrioritySkill, "BXBehavior.LowPrioritySkill", "低优先技能(低优先姿态行为域)");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXBehavior_HighPrioritySkill, "BXBehavior.HighPrioritySkill", "高优先技能(高优先姿态行为域)");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXBehavior_Defense, "BXBehavior.Defense", "防御");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXBehavior_Block, "BXBehavior.Block", "格挡");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXBehavior_Parry, "BXBehavior.Parry", "弹反");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXBehavior_Dodge, "BXBehavior.Dodge", "闪避");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXBehavior_PerfectDodge, "BXBehavior.PerfectDodge", "极限闪避");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXBehavior_ParallelSkill, "BXBehavior.ParallelSkill", "可并行技能(不与其它姿态互斥的姿态行为域)");

	// 动画(已移出行为族,原 BXBehavior.PlayMontage.Default)
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXMontage_Default, "BXMontage.Default", "播放Montage默认Tag");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXDamageType_Default, "BXDamageType.Default", "默认伤害类型");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXStunState_Normal, "BXStunState.Normal", "无硬直");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXStunState_Knockback, "BXStunState.Knockback", "击退");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXStunState_Knockdown, "BXStunState.Knockdown", "击倒");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXStunState_Prone, "BXStunState.Prone", "击趴");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXStunState_Ascending, "BXStunState.Ascending", "上升");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXStunState_Falling, "BXStunState.Falling", "下落");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXStunState_Floating, "BXStunState.Floating", "滞空");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXStunState_Execution, "BXStunState.Execution", "处决");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXAttackType_Light, "BXAttackType.Light", "轻击");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXAttackType_KnockBack, "BXAttackType.KnockBack", "击退");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXAttackType_KnockDown, "BXAttackType.KnockDown", "击倒");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXAttackType_Launch, "BXAttackType.Launch", "击飞");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXAttackType_Uppercut, "BXAttackType.Uppercut", "上挑");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXAttackType_AirKnockdown, "BXAttackType.AirKnockdown", "击落");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXAttackType_Drag, "BXAttackType.Drag", "拖拽");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXAttackForce_0, "BXAttackForce.0", "力度0");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXAttackForce_1, "BXAttackForce.1", "力度1");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXAttackForce_2, "BXAttackForce.2", "力度2");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXAttackForce_3, "BXAttackForce.3", "力度3");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXAttackForce_4, "BXAttackForce.4", "力度4");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXAttackForce_5, "BXAttackForce.5", "力度5");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXAttackRPos_Front, "BXAttackRPos.Front", "前方");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXAttackRPos_Back, "BXAttackRPos.Back", "后方");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXAttackRPos_Left, "BXAttackRPos.Left", "左方");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXAttackRPos_Right, "BXAttackRPos.Right", "右方");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXBodyPart_Default, "BXBodyPart.Default", "默认部位");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXBodyPart_Head, "BXBodyPart.Head", "头部");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXBodyPart_UpperBody, "BXBodyPart.UpperBody", "上半身");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXBodyPart_LowerBody, "BXBodyPart.LowerBody", "下半身");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXBodyPart_Tail, "BXBodyPart.Tail", "尾巴");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXBodyPart_Limbs1, "BXBodyPart.Limbs1", "肢体1");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXBodyPart_Limbs2, "BXBodyPart.Limbs2", "肢体2");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXBodyPart_Limbs3, "BXBodyPart.Limbs3", "肢体3");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXBodyPart_Limbs4, "BXBodyPart.Limbs4", "肢体4");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXBodyPart_Limbs5, "BXBodyPart.Limbs5", "肢体5");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXBodyPart_Limbs6, "BXBodyPart.Limbs6", "肢体6");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXBodyPart_Limbs7, "BXBodyPart.Limbs7", "肢体7");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXBodyPart_Limbs8, "BXBodyPart.Limbs8", "肢体8");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXGearType_Default, "BXGearType.Default", "默认类型");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXGearType_Weapon, "BXGearType.Weapon", "武器");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXGearType_Armor, "BXGearType.Armor", "防具");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXGearType_Ornament, "BXGearType.Ornament", "饰品");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXGearSlot_Default, "BXGearSlot.Default", "默认插槽");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXGearSlot_LeftHand, "BXGearSlot.LeftHand", "左手");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXGearSlot_RightHand, "BXGearSlot.RightHand", "右手");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXGearSlot_Head, "BXGearSlot.Head", "头部");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXGearSlot_Chest, "BXGearSlot.Chest", "胸部");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXGearSlot_Arms, "BXGearSlot.Arms", "胳膊");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXGearSlot_Waist, "BXGearSlot.Waist", "腰部");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXGearSlot_Legs, "BXGearSlot.Legs", "腿部");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXGearSlot_LeftFoot, "BXGearSlot.LeftFoot", "左脚");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXGearSlot_RightFoot, "BXGearSlot.RightFoot", "右脚");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXGearSlot_Jewelry1, "BXGearSlot.Jewelry1", "饰品1");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXGearSlot_Jewelry2, "BXGearSlot.Jewelry2", "饰品2");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXGearSlot_Jewelry3, "BXGearSlot.Jewelry3", "饰品3");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXGearSlot_Jewelry4, "BXGearSlot.Jewelry4", "饰品4");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXGearSlot_Jewelry5, "BXGearSlot.Jewelry5", "饰品5");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXGearSlot_Jewelry6, "BXGearSlot.Jewelry6", "饰品6");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXGearSlot_Jewelry7, "BXGearSlot.Jewelry7", "饰品7");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXGearSlot_Jewelry8, "BXGearSlot.Jewelry8", "饰品8");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXGearState_Default, "BXGearState.Default", "默认状态");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXGearState_Close, "BXGearState.Close", "收起");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXGearState_Open, "BXGearState.Open", "展开");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXGearHitBox_Main, "BXGearHitBox.Main", "主碰撞盒");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXTEvent_Start, "BXTEvent.Start", "开始");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXTEvent_End, "BXTEvent.End", "结束");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXTEvent_Success, "BXTEvent.Success", "成功");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXTEvent_Failure, "BXTEvent.Failure", "失败");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXTEvent_Trigger, "BXTEvent.Trigger", "触发");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXTEvent_Branch01, "BXTEvent.Branch01", "分支01");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXTEvent_Branch02, "BXTEvent.Branch02", "分支02");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXTEvent_Branch03, "BXTEvent.Branch03", "分支03");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXTEvent_Branch04, "BXTEvent.Branch04", "分支04");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXTEvent_Branch05, "BXTEvent.Branch05", "分支05");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXTEvent_Branch06, "BXTEvent.Branch06", "分支06");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXTEvent_Branch07, "BXTEvent.Branch07", "分支07");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXTEvent_Branch08, "BXTEvent.Branch08", "分支08");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXTEvent_Branch09, "BXTEvent.Branch09", "分支09");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXTEvent_Branch10, "BXTEvent.Branch10", "分支10");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXTEvent_Branch11, "BXTEvent.Branch11", "分支11");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXTEvent_Branch12, "BXTEvent.Branch12", "分支12");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXTEvent_Branch13, "BXTEvent.Branch13", "分支13");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXTEvent_Branch14, "BXTEvent.Branch14", "分支14");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXTEvent_Branch15, "BXTEvent.Branch15", "分支15");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXTEvent_Branch16, "BXTEvent.Branch16", "分支16");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXTEvent_BranchDefault, "BXTEvent.BranchDefault", "默认分支");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXTData_Transform, "BXTData.Transform", "方位");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXTData_ColResults1, "BXTData.ColResults1", "碰撞结果1");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXTData_ColResults2, "BXTData.ColResults2", "碰撞结果2");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXTData_ColResults3, "BXTData.ColResults3", "碰撞结果3");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXTData_ColResults4, "BXTData.ColResults4", "碰撞结果4");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXTData_ColResults5, "BXTData.ColResults5", "碰撞结果5");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXRelationShip_Self, "BXRelationShip.Self", "自己");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXRelationShip_Friendly, "BXRelationShip.Friendly", "友好");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXRelationShip_Neutral, "BXRelationShip.Neutral", "中立");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXRelationShip_Hostile, "BXRelationShip.Hostile", "敌对");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXActorType_Player, "BXActorType.Player", "玩家");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXActorType_NPC, "BXActorType.NPC", "NPC");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXActorType_Monster, "BXActorType.Monster", "小怪");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXActorType_BOSS, "BXActorType.BOSS", "BOSS");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXEvent_Buff_Added, "BXEvent.Buff.Added", "BUFF添加");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXEvent_Buff_Removed, "BXEvent.Buff.Removed", "BUFF移除");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXEvent_Buff_LayerChanged, "BXEvent.Buff.LayerChanged", "BUFF层级改变");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXEvent_Buff_LevelChanged, "BXEvent.Buff.LevelChanged", "BUFF等级改变");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXEvent_Buff_LifetimeRefreshed, "BXEvent.Buff.LifetimeRefreshed", "BUFF生命时长刷新");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXEvent_Skill_Released, "BXEvent.Skill.Released", "技能已释放");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXEvent_Skill_Finished, "BXEvent.Skill.Finished", "技能已结束(自然/中断/回滚,参数含结束原因)");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXEvent_Skill_PredictSuccess, "BXEvent.Skill.PredictSuccess", "技能预测成功");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXEvent_Skill_PredictFailed, "BXEvent.Skill.PredictFailed", "技能预测失败");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXEvent_Collision_Reported, "BXEvent.Collision.Reported", "碰撞结果已上报");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXSkillInput_ReleaseLocation, "BXSkillInput.ReleaseLocation", "技能输入 释放位置");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXSkillInput_ReleaseRotation, "BXSkillInput.ReleaseRotation", "技能输入 释放朝向");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXSkillInput_AimLocation, "BXSkillInput.AimLocation", "技能输入 瞄准位置");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXSkillInput_AimRotation, "BXSkillInput.AimRotation", "技能输入 瞄准朝向");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXSkillInput_LockParts, "BXSkillInput.LockParts", "技能输入 锁定部位列表");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXEvent_Projectile_Fired, "BXEvent.Projectile.Fired", "子弹已发射");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXEvent_Projectile_Hit, "BXEvent.Projectile.Hit", "子弹命中");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXEvent_Projectile_Finished, "BXEvent.Projectile.Finished", "子弹已结束");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXEvent_Projectile_Denied, "BXEvent.Projectile.Denied", "子弹预测被否认");
}
