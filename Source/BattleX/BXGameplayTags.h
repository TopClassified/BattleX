#pragma once

#include "NativeGameplayTags.h"



namespace BXGameplayTags
{
	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXBehavior_Locomotion_Move);
	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXBehavior_Locomotion_Rotate);
	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXBehavior_Locomotion_Jump);
	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXBehavior_Action_CastSkill);
	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXBehavior_Action_CastPSkill);
	
	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXDamageType_Default);
	
	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXStagger_Normal);
	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXStagger_Repel);
	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXStagger_LieDown);
	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXStagger_Crawl);
	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXStagger_Rise);
	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXStagger_Fall);
	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXStagger_Float);
	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXStagger_Execution);

	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXAttackType_Light);
	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXAttackType_Repel);
	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXAttackType_HitDown);
	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXAttackType_BlowFly);
	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXAttackType_LiftUp);
	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXAttackType_BlowDown);
	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXAttackType_Drag);

	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXAttackForce_0);
	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXAttackForce_1);
	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXAttackForce_2);
	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXAttackForce_3);
	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXAttackForce_4);
	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXAttackForce_5);

	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXAttackRPos_Front);
	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXAttackRPos_Back);
	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXAttackRPos_Left);
	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXAttackRPos_Right);

	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXBodyPart_Default);
	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXBodyPart_Head);
	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXBodyPart_UpperBody);
	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXBodyPart_LowerBody);
	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXBodyPart_Tail);
	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXBodyPart_Limbs1);
	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXBodyPart_Limbs2);
	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXBodyPart_Limbs3);
	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXBodyPart_Limbs4);
	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXBodyPart_Limbs5);
	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXBodyPart_Limbs6);
	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXBodyPart_Limbs7);
	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXBodyPart_Limbs8);

	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXGearType_Default);
	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXGearType_Weapon);
	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXGearType_Armor);
	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXGearType_Ornament);
	
	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXGearSlot_Default);
	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXGearSlot_LeftHand);
	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXGearSlot_RightHand);
	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXGearSlot_Head);
	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXGearSlot_Chest);
	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXGearSlot_Arms);
	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXGearSlot_Waist);
	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXGearSlot_Legs);
	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXGearSlot_LeftFoot);
	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXGearSlot_RightFoot);
	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXGearSlot_Jewelry1);
	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXGearSlot_Jewelry2);
	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXGearSlot_Jewelry3);
	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXGearSlot_Jewelry4);
	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXGearSlot_Jewelry5);
	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXGearSlot_Jewelry6);
	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXGearSlot_Jewelry7);
	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXGearSlot_Jewelry8);
	
	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXGearState_Default);
	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXGearState_Close);
	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXGearState_Open);

	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXGearHitBox_Main);

	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXTEvent_Success);
	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXTEvent_Failure);
	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXTEvent_Trigger);

	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXTData_Transform);
	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXTData_ColResults1);
	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXTData_ColResults2);
	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXTData_ColResults3);
	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXTData_ColResults4);
	BATTLEX_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(BXTData_ColResults5);
}