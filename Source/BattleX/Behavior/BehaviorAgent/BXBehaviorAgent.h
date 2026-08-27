#pragma once

#include "CoreMinimal.h"
#include "StructUtils/InstancedStruct.h"

#include "BXBehaviorAgent.generated.h"



// 开始/停止行为要执行的函数(位标志:BehaviorFunctions按位组合)
// 旧枚举为顺序值(0..9)与cpp位掩码用法错位,编辑器Bitmask勾选标签与实际执行函数不对齐,此处对齐为1<<n
// 10个标志需10位(1<<9=512)超出uint8,故基类型uint32且不带BlueprintType(UHT限制:BlueprintType枚举仅支持uint8基类型);
// 枚举仅作BehaviorFunctions(int32)的BitmaskEnum元数据供编辑器勾选框使用,蓝图侧不直接引用枚举类型,无暴露损失
UENUM(Meta = (Bitflags))
enum class EBXBehaviorAgentFunction : uint32
{
	BAF_None                     = 0            UMETA(Hidden),
	BAF_NativeInitialize         = 1 << 0       UMETA(DisplayName = "C++初始化"),
	BAF_BPInitialize             = 1 << 1       UMETA(DisplayName = "蓝图初始化"),
	BAF_NativeCleanup            = 1 << 2       UMETA(DisplayName = "C++清理"),
	BAF_BPCleanup                = 1 << 3       UMETA(DisplayName = "蓝图清理"),
	BAF_NativeStartBehavior      = 1 << 4       UMETA(DisplayName = "C++开始行为"),
	BAF_BPStartBehavior          = 1 << 5       UMETA(DisplayName = "蓝图开始行为"),
	BAF_NativeStopBehavior       = 1 << 6       UMETA(DisplayName = "C++停止行为"),
	BAF_BPStopBehavior           = 1 << 7       UMETA(DisplayName = "蓝图停止行为"),
	BAF_NativeCheckStartBehavior = 1 << 8       UMETA(DisplayName = "C++检查行为能否开始"),
	BAF_BPCheckStartBehavior     = 1 << 9       UMETA(DisplayName = "蓝图检查行为能否开始"),
};
ENUM_CLASS_FLAGS(EBXBehaviorAgentFunction);



UCLASS(Abstract, Blueprintable, BlueprintType)
class BATTLEX_API UBXBehaviorAgent : public UObject
{
	GENERATED_BODY()
	
public:
	// 获取唯一ID
	UFUNCTION(BlueprintCallable)
	int64 GetUniqueKey() const;

	// 获取拥有者
	UFUNCTION(BlueprintCallable)
	AActor* GetOwner() const;
	
	// 初始化
	UFUNCTION(BlueprintCallable)
	bool Initialize();
	virtual bool NativeInitialize();
	UFUNCTION(BlueprintImplementableEvent)
	bool ScriptInitialize();
	
	// 逆初始化
	UFUNCTION(BlueprintCallable)
	bool Deinitialize();
	virtual bool NativeDeinitialize();
	UFUNCTION(BlueprintImplementableEvent)
	bool ScriptDeinitialize();
	
	// 开始行为
	UFUNCTION(BlueprintCallable)
	bool StartBehavior(const FInstancedStruct& InParameter);
	virtual bool NativeStartBehavior(const FInstancedStruct& InParameter);
	UFUNCTION(BlueprintImplementableEvent)
	bool ScriptStartBehavior(const FInstancedStruct& InParameter);
	
	// 停止行为
	UFUNCTION(BlueprintCallable)
	bool StopBehavior(const FInstancedStruct& InParameter);
	virtual bool NativeStopBehavior(const FInstancedStruct& InParameter);
	UFUNCTION(BlueprintImplementableEvent)
	bool ScriptStopBehavior(const FInstancedStruct& InParameter);

	// 判断行为是否可以开始
	UFUNCTION(BlueprintCallable)
	bool CheckStartBehavior(const FInstancedStruct& InParameter);
	virtual bool NativeCheckStartBehavior(const FInstancedStruct& InParameter);
	UFUNCTION(BlueprintImplementableEvent)
	bool ScriptCheckStartBehavior(const FInstancedStruct& InParameter);

public:
	// 要执行的函数(位组合,默认=C++五件套)
	UPROPERTY(EditDefaultsOnly, Meta = (Bitmask, BitmaskEnum = "/Script/BattleX.EBXBehaviorAgentFunction"))
	int32 BehaviorFunctions = 341;

protected:
	// 唯一ID
	UPROPERTY(Transient, BlueprintReadOnly)
	int64 UniqueKey = 0;
	
};