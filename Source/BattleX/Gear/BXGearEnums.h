#pragma once



// 装备类型
UENUM(BlueprintType)
enum class EBXGearType : uint8
{
	GT_None                 = 0             UMETA(DisplayName = "未知"),
	GT_Weapon                               UMETA(DisplayName = "武器"),
	GT_Armor                                UMETA(DisplayName = "防具"),
	GT_Ornament                             UMETA(DisplayName = "装饰品"),

	GT_TMax                                 UMETA(Hidden)
};



// 装备插槽
UENUM(BlueprintType)
enum class EBXGearSlot : uint8
{
	GS_None                 = 0             UMETA(DisplayName = "未知"),
	GS_LeftHand                             UMETA(DisplayName = "左手"),
	GS_RightHand                            UMETA(DisplayName = "右手"),
	GS_Head                                 UMETA(DisplayName = "头部"),
	GS_Chest                                UMETA(DisplayName = "胸部"),
	GS_Arms                                 UMETA(DisplayName = "胳膊"),
	GS_Waist                                UMETA(DisplayName = "腰部"),
	GS_Legs                                 UMETA(DisplayName = "腿部"),
	GS_LeftFoot                             UMETA(DisplayName = "左脚"),
	GS_RightFoot                            UMETA(DisplayName = "右脚"),
	GS_Jewelry1                             UMETA(DisplayName = "饰品1"),
	GS_Jewelry2                             UMETA(DisplayName = "饰品2"),
	GS_Jewelry3                             UMETA(DisplayName = "饰品3"),
	GS_Jewelry4                             UMETA(DisplayName = "饰品4"),
	GS_Jewelry5                             UMETA(DisplayName = "饰品5"),
	GS_Jewelry6                             UMETA(DisplayName = "饰品6"),
	GS_Jewelry7                             UMETA(DisplayName = "饰品7"),
	GS_Jewelry8                             UMETA(DisplayName = "饰品8"),

	GS_TMax                                 UMETA(Hidden)
};



// 装上/卸下装备要执行的函数
UENUM(BlueprintType, Meta = (Bitflags))
enum class EBXEquipFunction : uint8
{
	EF_NativePreEquip          = 0              UMETA(DisplayName = "C++装上之前"),
	EF_BPPreEquip                               UMETA(DisplayName = "蓝图装上之前"),
	EF_NativePostEquip                          UMETA(DisplayName = "C++装上之后"),
	EF_BPPostEquip                              UMETA(DisplayName = "蓝图装上之后"),
	EF_NativePreUnequip                         UMETA(DisplayName = "C++卸下之前"),
	EF_BPPreUnequip                             UMETA(DisplayName = "蓝图卸下之前"),
	EF_NativePostUnequip                        UMETA(DisplayName = "C++卸下之后"),
	EF_BPPostUnequip                            UMETA(DisplayName = "蓝图卸下之后"),

	EF_TMax                                     UMETA(Hidden)
};
ENUM_CLASS_FLAGS(EBXEquipFunction);



// 使用/弃用装备要执行的函数
UENUM(BlueprintType, Meta = (Bitflags))
enum class EBXUseFunction : uint8
{
	EF_NativePreUsing          = 0              UMETA(DisplayName = "C++使用之前"),
	EF_BPPreUsing                               UMETA(DisplayName = "蓝图使用之前"),
	EF_NativePostUsing                          UMETA(DisplayName = "C++使用之后"),
	EF_BPPostUsing                              UMETA(DisplayName = "蓝图使用之后"),
	EF_NativePreUnusing                         UMETA(DisplayName = "C++弃用之前"),
	EF_BPPreUnusing                             UMETA(DisplayName = "蓝图弃用之前"),
	EF_NativePostUnusing                        UMETA(DisplayName = "C++弃用之后"),
	EF_BPPostUnusing                            UMETA(DisplayName = "蓝图弃用之后"),

	EF_TMax                                     UMETA(Hidden)
};
ENUM_CLASS_FLAGS(EBXUseFunction);



// 出鞘/入鞘装备要执行的函数
UENUM(BlueprintType, Meta = (Bitflags))
enum class EBXSheathFunction : uint8
{
	EF_NativePreSheath          = 0              UMETA(DisplayName = "C++入鞘之前"),
	EF_BPPreSheath                               UMETA(DisplayName = "蓝图入鞘之前"),
	EF_NativePostSheath                          UMETA(DisplayName = "C++入鞘之后"),
	EF_BPPostSheath                              UMETA(DisplayName = "蓝图入鞘之后"),
	EF_NativePreUnsheath                         UMETA(DisplayName = "C++出鞘之前"),
	EF_BPPreUnsheath                             UMETA(DisplayName = "蓝图出鞘之前"),
	EF_NativePostUnsheath                        UMETA(DisplayName = "C++出鞘之后"),
	EF_BPPostUnsheath                            UMETA(DisplayName = "蓝图出鞘之后"),

	EF_TMax                                     UMETA(Hidden)
};
ENUM_CLASS_FLAGS(EBXSheathFunction);
