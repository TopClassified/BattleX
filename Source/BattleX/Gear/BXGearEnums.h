#pragma once 



// 装上/卸下装备要执行的函数
UENUM(BlueprintType, Meta = (Bitflags))
enum class EBXEquipGearFunction : uint8
{
	EGF_NativePreEquip          = 0              UMETA(DisplayName = "C++装上之前"),
	EGF_BPPreEquip                               UMETA(DisplayName = "蓝图装上之前"),
	EGF_NativePostEquip                          UMETA(DisplayName = "C++装上之后"),
	EGF_BPPostEquip                              UMETA(DisplayName = "蓝图装上之后"),
	EGF_NativePreUnequip                         UMETA(DisplayName = "C++卸下之前"),
	EGF_BPPreUnequip                             UMETA(DisplayName = "蓝图卸下之前"),
	EGF_NativePostUnequip                        UMETA(DisplayName = "C++卸下之后"),
	EGF_BPPostUnequip                            UMETA(DisplayName = "蓝图卸下之后"),

	EGF_TMax                                     UMETA(Hidden)
};
ENUM_CLASS_FLAGS(EBXEquipGearFunction);



// 使用/弃用装备要执行的函数
UENUM(BlueprintType, Meta = (Bitflags))
enum class EBXUseGearFunction : uint8
{
	UGF_NativePreUsing          = 0              UMETA(DisplayName = "C++使用之前"),
	UGF_BPPreUsing                               UMETA(DisplayName = "蓝图使用之前"),
	UGF_NativePostUsing                          UMETA(DisplayName = "C++使用之后"),
	UGF_BPPostUsing                              UMETA(DisplayName = "蓝图使用之后"),
	UGF_NativePreUnusing                         UMETA(DisplayName = "C++弃用之前"),
	UGF_BPPreUnusing                             UMETA(DisplayName = "蓝图弃用之前"),
	UGF_NativePostUnusing                        UMETA(DisplayName = "C++弃用之后"),
	UGF_BPPostUnusing                            UMETA(DisplayName = "蓝图弃用之后"),

	UGF_TMax                                     UMETA(Hidden)
};
ENUM_CLASS_FLAGS(EBXUseGearFunction);



// 状态改变时装备要执行的函数
UENUM(BlueprintType, Meta = (Bitflags))
enum class EBXChangeGearStateFunction : uint8
{
	CGSF_NativeChangeState          = 0              UMETA(DisplayName = "C++改变状态"),
	CGSF_BPChangeState                               UMETA(DisplayName = "蓝图改变状态"),

	CGSF_TMax                                        UMETA(Hidden)
};
ENUM_CLASS_FLAGS(EBXChangeGearStateFunction);
