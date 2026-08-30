#pragma once

#include "CoreMinimal.h"
#include "StructUtils/InstancedStruct.h"

#include "BXBehaviorProxy.generated.h"



// 启用/禁用/开始/停止/更新要执行的函数(位标志:BehaviorFunctions按位组合)
// 旧枚举为顺序值(0..9)与cpp位掩码用法错位,编辑器Bitmask勾选标签与实际执行函数不对齐,此处对齐为1<<n
// 16个标志需16位(1<<15=32768)超出uint8,故基类型uint32且不带BlueprintType(UHT限制:BlueprintType枚举仅支持uint8基类型);
// 枚举仅作BehaviorFunctions(int32)的BitmaskEnum元数据供编辑器勾选框使用,蓝图侧不直接引用枚举类型,无暴露损失
UENUM(Meta = (Bitflags))
enum class EBXBehaviorProxyFunction : uint32
{
	BPF_None                     = 0            UMETA(Hidden),
	BPF_NativeInitialize         = 1 << 0       UMETA(DisplayName = "C++初始化"),
	BPF_BPInitialize             = 1 << 1       UMETA(DisplayName = "蓝图初始化"),
	BPF_NativeCleanup            = 1 << 2       UMETA(DisplayName = "C++清理"),
	BPF_BPCleanup                = 1 << 3       UMETA(DisplayName = "蓝图清理"),
	BPF_NativeStartBehavior      = 1 << 4       UMETA(DisplayName = "C++开始行为"),
	BPF_BPStartBehavior          = 1 << 5       UMETA(DisplayName = "蓝图开始行为"),
	BPF_NativeStopBehavior       = 1 << 6       UMETA(DisplayName = "C++停止行为"),
	BPF_BPStopBehavior           = 1 << 7       UMETA(DisplayName = "蓝图停止行为"),
	BPF_NativeCheckStartBehavior = 1 << 8       UMETA(DisplayName = "C++检查行为能否开始"),
	BPF_BPCheckStartBehavior     = 1 << 9       UMETA(DisplayName = "蓝图检查行为能否开始"),
	BPF_NativeEnableProxy        = 1 << 10      UMETA(DisplayName = "C++启用代理"),
	BPF_BPEnableProxy            = 1 << 11      UMETA(DisplayName = "蓝图启用代理"),
	BPF_NativeDisableProxy       = 1 << 12      UMETA(DisplayName = "C++禁用代理"),
	BPF_BPDisableProxy           = 1 << 13      UMETA(DisplayName = "蓝图禁用代理"),
	BPF_NativeUpdateProxy        = 1 << 14      UMETA(DisplayName = "C++更新代理"),
	BPF_BPUpdateProxy            = 1 << 15      UMETA(DisplayName = "蓝图更新代理"),
};
ENUM_CLASS_FLAGS(EBXBehaviorProxyFunction);



// 行为代理(一个行为域的总代理,组件是唯一命令源,代理不自治不持有禁用来源逻辑)
// 双轴命令:Enable/Disable为权限轴(持有基层开关,如移动组件的Blocked布尔),Start/Stop为活动轴(事实上报/技能姿态)
// 权限/活动状态机(bEnabled/bStarted)由基类统一簿记,派生类只重写Native/Script执行槽位
UCLASS(Abstract, Blueprintable, BlueprintType)
class BATTLEX_API UBXBehaviorProxy : public UObject
{
	GENERATED_BODY()
	
public:
	// 获取唯一ID
	UFUNCTION(BlueprintCallable)
	int64 GetUniqueKey() const;

	// 获取拥有者
	UFUNCTION(BlueprintCallable)
	AActor* GetOwner() const;

	// 是否已启用(权限轴)
	UFUNCTION(BlueprintCallable)
	bool IsEnabled() const { return bEnabled; }

	// 是否已开始(活动轴)
	UFUNCTION(BlueprintCallable)
	bool IsStarted() const { return bStarted; }

	// 是否需要帧更新(bWantsProxyUpdate且已启用时组件Tick才转发UpdateProxy)
	UFUNCTION(BlueprintCallable)
	bool WantsProxyUpdate() const { return bWantsProxyUpdate; }

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

	// 启用代理(权限轴;基类/子类Enable槽位)
	UFUNCTION(BlueprintCallable)
	bool EnableProxy();
	virtual bool NativeEnableProxy();
	UFUNCTION(BlueprintImplementableEvent)
	bool ScriptEnableProxy();

	// 禁用代理(权限轴;基类/子类Disable槽位;不触碰活动轴)
	UFUNCTION(BlueprintCallable)
	bool DisableProxy();
	virtual bool NativeDisableProxy();
	UFUNCTION(BlueprintImplementableEvent)
	bool ScriptDisableProxy();

	// 开始行为(活动轴;仅已启用时代理生效,未启用返回false)
	UFUNCTION(BlueprintCallable)
	bool StartBehavior(const FInstancedStruct& InParameter);
	virtual bool NativeStartBehavior(const FInstancedStruct& InParameter);
	UFUNCTION(BlueprintImplementableEvent)
	bool ScriptStartBehavior(const FInstancedStruct& InParameter);
	
	// 停止行为(活动轴;真停语义,置bStarted=false)
	UFUNCTION(BlueprintCallable)
	bool StopBehavior(const FInstancedStruct& InParameter);
	virtual bool NativeStopBehavior(const FInstancedStruct& InParameter);
	UFUNCTION(BlueprintImplementableEvent)
	bool ScriptStopBehavior(const FInstancedStruct& InParameter);

	// 帧更新(组件Tick转发;bWantsProxyUpdate且已启用时执行)
	bool UpdateProxy(float InDeltaTime);
	virtual bool NativeUpdateProxy(float InDeltaTime);
	UFUNCTION(BlueprintImplementableEvent)
	bool ScriptUpdateProxy(float InDeltaTime);

	// 判断行为是否可以开始
	UFUNCTION(BlueprintCallable)
	bool CheckStartBehavior(const FInstancedStruct& InParameter);
	virtual bool NativeCheckStartBehavior(const FInstancedStruct& InParameter);
	UFUNCTION(BlueprintImplementableEvent)
	bool ScriptCheckStartBehavior(const FInstancedStruct& InParameter);

protected:
	// 执行开始(位掩码分发;公开包装做簿记后调用)
	bool ExecuteStartBehavior(const FInstancedStruct& InParameter);

	// 执行停止(位掩码分发;公开包装与禁用收停共用)
	bool ExecuteStopBehavior(const FInstancedStruct& InParameter);

public:
	// 要执行的函数(位组合,默认=C++五件套+启用/禁用)
	UPROPERTY(EditDefaultsOnly, Meta = (Bitmask, BitmaskEnum = "/Script/BattleX.EBXBehaviorProxyFunction"))
	int32 BehaviorFunctions = 5461;

protected:
	// 是否需要帧更新(默认关闭;组件Tick按此过滤转发UpdateProxy)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Proxy")
	bool bWantsProxyUpdate = false;

	// 唯一ID
	UPROPERTY(Transient, BlueprintReadOnly)
	int64 UniqueKey = 0;

	// 权限轴:已启用
	bool bEnabled = false;

	// 活动轴:已开始
	bool bStarted = false;

	// 最近一次启动参数(StartBehavior记录)
	FInstancedStruct LastStartParameter;
	
};