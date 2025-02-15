#include "BXGameplayTags.h"



namespace BXGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXEvent_TimelineStarted, "BXEvent.TimelineStarted", "时间轴已开始");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXEvent_TimelineClosing, "BXEvent.TimelineClosing", "时间轴将结束");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXEvent_ChangeForbiddenBehavior, "BXEvent.ChangeForbiddenBehavior", "行为禁止改变");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXEvent_Behavior_Move, "BXEvent.Behavior.Move", "行为事件 移动");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXEvent_Behavior_Rotate, "BXEvent.Behavior.Rotate", "行为事件 转向");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXEvent_Behavior_Jump, "BXEvent.Behavior.Jump", "行为事件 跳跃");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXEvent_Behavior_Landed, "BXEvent.Behavior.Landed", "行为事件 落地");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXBehavior_Locomotion_Move, "BXBehavior.Locomotion.Move", "主动移动");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXBehavior_Locomotion_Rotate, "BXBehavior.Locomotion.Rotate", "主动转向");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXImmBehavior_Locomotion_Jump, "BXImmBehavior.Locomotion.Jump", "主动跳跃(瞬间行为)");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXImmBehavior_Locomotion_Landed, "BXImmBehavior.Locomotion.Landed", "落地(瞬间行为)");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXDamageType_Default, "BXDamageType.Default", "默认伤害类型");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXStunState_NoStun, "BXStunState.NoStun", "无硬直");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXStunState_Repel, "BXStunState.Repel", "击退");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXStunState_LieDown, "BXStunState.LieDown", "倒地");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXStunState_Crawl, "BXStunState.Crawl", "趴下");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXStunState_Rise, "BXStunState.Rise", "起飞");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXStunState_Fall, "BXStunState.Fall", "坠落");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXStunState_Float, "BXStunState.Float", "浮空");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXStunState_Execution, "BXStunState.Execution", "处决");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXAttackType_Light, "BXAttackType.Light", "轻击");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXAttackType_Repel, "BXAttackType.Repel", "击退");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXAttackType_HitDown, "BXAttackType.HitDown", "击倒");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXAttackType_BlowFly, "BXAttackType.BlowFly", "击飞");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXAttackType_LiftUp, "BXAttackType.LiftUp", "挑空");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(BXAttackType_BlowDown, "BXAttackType.BlowDown", "击落");
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
}
