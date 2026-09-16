#pragma once

#include "CoreMinimal.h"

#include "BXStateProxy.generated.h"



// 要执行的函数(位标志:StateFunctions按位组合)
// 仅活动轴(开始/停止)——状态没有禁止账本语义(挡启动归行为侧Forbid),无权限轴Enable/Disable
// 基类型uint32不带BlueprintType(UHT限制:BlueprintType枚举仅支持uint8基类型,同EBXBehaviorProxyFunction处理)
UENUM(Meta = (Bitflags))
enum class EBXStateProxyFunction : uint32
{
	SPF_None              = 0        UMETA(Hidden),
	SPF_NativeInitialize  = 1 << 0   UMETA(DisplayName = "C++初始化"),
	SPF_BPInitialize      = 1 << 1   UMETA(DisplayName = "蓝图初始化"),
	SPF_NativeCleanup     = 1 << 2   UMETA(DisplayName = "C++清理"),
	SPF_BPCleanup         = 1 << 3   UMETA(DisplayName = "蓝图清理"),
	SPF_NativeStartState  = 1 << 4   UMETA(DisplayName = "C++开始状态"),
	SPF_BPStartState      = 1 << 5   UMETA(DisplayName = "蓝图开始状态"),
	SPF_NativeStopState   = 1 << 6   UMETA(DisplayName = "C++停止状态"),
	SPF_BPStopState       = 1 << 7   UMETA(DisplayName = "蓝图停止状态"),
	SPF_NativeUpdateState = 1 << 8   UMETA(DisplayName = "C++更新状态"),
	SPF_BPUpdateState     = 1 << 9   UMETA(DisplayName = "蓝图更新状态"),
};
ENUM_CLASS_FLAGS(EBXStateProxyFunction);



// 状态代理(一个状态的物理执行体,状态组件是唯一命令源,代理不自治)
// 状态条目从无到有→StartState,最后来源退出→StopState;同Sign刷新/来源级新增不重启(与门控/表现/事件同语义)
// 与行为代理的差异:无权限轴(Enable/Disable)——状态禁行为的门控走行为侧Forbid账本,代理只承担门控表达不了的
// 物理效果(移动模式切换/定身/朝向锁定);启动无参数——网络同步不上传参数,需要时从基层组件现场取参
UCLASS(Abstract, Blueprintable, BlueprintType)
class BATTLEX_API UBXStateProxy : public UObject
{
	GENERATED_BODY()

public:
	// 获取唯一ID
	UFUNCTION(BlueprintCallable)
	int64 GetUniqueKey() const;

	// 获取拥有者
	UFUNCTION(BlueprintCallable)
	AActor* GetOwner() const;

	// 是否已开始(活动轴)
	UFUNCTION(BlueprintCallable)
	bool IsStarted() const { return bStarted; }

	// 是否需要帧更新(bWantsStateUpdate且已开始时组件Tick才转发UpdateState)
	UFUNCTION(BlueprintCallable)
	bool WantsStateUpdate() const { return bWantsStateUpdate; }

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

	// 开始状态(条目从无到有时执行;重复Start=重启语义,物理操作须幂等)
	UFUNCTION(BlueprintCallable)
	bool StartState();
	virtual bool NativeStartState();
	UFUNCTION(BlueprintImplementableEvent)
	bool ScriptStartState();

	// 停止状态(条目死亡时执行;真停语义置bStarted=false,重复Stop幂等)
	// 所有退出原因(含预测回滚)均执行——物理必须对称还原,否则预测端/跟随端物理漂移
	UFUNCTION(BlueprintCallable)
	bool StopState();
	virtual bool NativeStopState();
	UFUNCTION(BlueprintImplementableEvent)
	bool ScriptStopState();

	// 帧更新(组件Tick转发;bWantsStateUpdate且已开始时执行)
	bool UpdateState(float InDeltaTime);
	virtual bool NativeUpdateState(float InDeltaTime);
	UFUNCTION(BlueprintImplementableEvent)
	bool ScriptUpdateState(float InDeltaTime);

protected:
	// 执行开始(位掩码分发;公开包装做簿记后调用)
	bool ExecuteStartState();

	// 执行停止(位掩码分发;公开包装做簿记后调用)
	bool ExecuteStopState();

public:
	// 要执行的函数(位组合,默认=C++开始/停止/初始化/清理四件套)
	UPROPERTY(EditDefaultsOnly, Meta = (Bitmask, BitmaskEnum = "/Script/BattleX.EBXStateProxyFunction"))
	int32 StateFunctions = 85;

protected:
	// 是否需要帧更新(默认关闭;组件Tick按此过滤转发UpdateState)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Proxy")
	bool bWantsStateUpdate = false;

	// 唯一ID
	UPROPERTY(Transient, BlueprintReadOnly)
	int64 UniqueKey = 0;

	// 活动轴:已开始
	bool bStarted = false;

};
