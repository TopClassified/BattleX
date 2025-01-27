#pragma once

#include "CoreMinimal.h"
#include "InstancedStruct.h"

#include "BXBehaviorAgent.generated.h"



// 开始/停止行为要执行的函数
UENUM(BlueprintType, Meta = (Bitflags))
enum class EBXBehaviorAgentFunction : uint8
{
	BAF_NativeInitialize         = 0                  UMETA(DisplayName = "C++初始化"),
	BAF_BPInitialize                                  UMETA(DisplayName = "蓝图初始化"),
	BAF_NativeCleanup                                 UMETA(DisplayName = "C++清理"),
	BAF_BPCleanup                                     UMETA(DisplayName = "蓝图清理"),
	BAF_NativeStartBehavior                           UMETA(DisplayName = "C++开始行为"),
	BAF_BPStartBehavior                               UMETA(DisplayName = "蓝图开始行为"),
	BAF_NativeStopBehavior                            UMETA(DisplayName = "C++停止行为"),
	BAF_BPStopBehavior                                UMETA(DisplayName = "蓝图停止行为"),
	BAF_NativeCheckStartBehavior                      UMETA(DisplayName = "C++检查行为能否开始"),
	BAF_BPCheckStartBehavior                          UMETA(DisplayName = "蓝图检查行为能否开始"),

	BAF_TMax                                          UMETA(Hidden)
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
	// 要执行的函数
	UPROPERTY(EditDefaultsOnly, Meta = (Bitmask, BitmaskEnum = "/Script/BattleX.EBXBehaviorAgentFunction"))
	int32 BehaviorFunctions = 341;

protected:
	// 唯一ID
	UPROPERTY(Transient, BlueprintReadOnly)
	int64 UniqueKey = 0;
	
};