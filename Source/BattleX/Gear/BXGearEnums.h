#pragma once 



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



// 状态改变时装备要执行的函数
UENUM(BlueprintType, Meta = (Bitflags))
enum class EBXChangeStateFunction : uint8
{
	EF_NativeChangeState          = 0              UMETA(DisplayName = "C++改变状态"),
	EF_BPChangeState                               UMETA(DisplayName = "蓝图改变状态"),

	EF_TMax                                     UMETA(Hidden)
};
ENUM_CLASS_FLAGS(EBXChangeStateFunction);
