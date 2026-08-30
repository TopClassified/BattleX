#include "BXBehaviorProxy.h"

#include "BXFunctionLibrary.h"

// 位标志快捷判断(BehaviorFunctions为int32位组合,与EBXBehaviorProxyFunction一一对应)
#define BX_HAS_PROXY_FLAG(FlagName) (EnumHasAllFlags(static_cast<EBXBehaviorProxyFunction>(BehaviorFunctions), EBXBehaviorProxyFunction::FlagName))


int64 UBXBehaviorProxy::GetUniqueKey() const
{
	return UniqueKey;
}

AActor* UBXBehaviorProxy::GetOwner() const
{
	if (UActorComponent* Component = Cast<UActorComponent>(GetOuter()))
	{
		return Component->GetOwner();
	}

	return nullptr;
}

bool UBXBehaviorProxy::Initialize()
{
	UniqueKey = UBXFunctionLibrary::GetUniqueID();

	bool bResult1 = true;
	if (BX_HAS_PROXY_FLAG(BPF_NativeInitialize))
	{
		bResult1 = NativeInitialize();
	}

	bool bResult2 = true;
	if (BX_HAS_PROXY_FLAG(BPF_BPInitialize))
	{
		bResult2 = ScriptInitialize();
	}

	return (bResult1 && bResult2);
}

bool UBXBehaviorProxy::NativeInitialize()
{
	// 默认true与UBXStateMachine基类NativeXxx语义对齐(返回false会使未重写该虚函数的派生类对应操作恒失败)
	return true;
}

bool UBXBehaviorProxy::Deinitialize()
{
	bool bResult1 = true;
	if (BX_HAS_PROXY_FLAG(BPF_NativeCleanup))
	{
		bResult1 = NativeDeinitialize();
	}

	bool bResult2 = true;
	if (BX_HAS_PROXY_FLAG(BPF_BPCleanup))
	{
		bResult2 = ScriptDeinitialize();
	}

	return (bResult1 && bResult2);
}

bool UBXBehaviorProxy::NativeDeinitialize()
{
	return true;
}

bool UBXBehaviorProxy::EnableProxy()
{
	// 幂等(已启用直接返回)
	if (bEnabled)
	{
		return true;
	}

	bEnabled = true;

	bool bResult1 = true;
	if (BX_HAS_PROXY_FLAG(BPF_NativeEnableProxy))
	{
		bResult1 = NativeEnableProxy();
	}

	bool bResult2 = true;
	if (BX_HAS_PROXY_FLAG(BPF_BPEnableProxy))
	{
		bResult2 = ScriptEnableProxy();
	}

	return (bResult1 && bResult2);
}

bool UBXBehaviorProxy::NativeEnableProxy()
{
	return true;
}

bool UBXBehaviorProxy::DisableProxy()
{
	// 幂等(已禁用直接返回)
	if (!bEnabled)
	{
		return true;
	}

	bEnabled = false;

	bool bResult1 = true;
	if (BX_HAS_PROXY_FLAG(BPF_NativeDisableProxy))
	{
		bResult1 = NativeDisableProxy();
	}

	bool bResult2 = true;
	if (BX_HAS_PROXY_FLAG(BPF_BPDisableProxy))
	{
		bResult2 = ScriptDisableProxy();
	}

	return (bResult1 && bResult2);
}

bool UBXBehaviorProxy::NativeDisableProxy()
{
	return true;
}

bool UBXBehaviorProxy::StartBehavior(const FInstancedStruct& InParameter)
{
	// 权限门禁(未启用不得开始;事件型代理由管线隐式启用后再Start)
	if (!bEnabled)
	{
		return false;
	}

	// 记录最近启动参数后再执行(重复Start=重启语义)
	bStarted = true;
	LastStartParameter = InParameter;

	return ExecuteStartBehavior(InParameter);
}

bool UBXBehaviorProxy::StopBehavior(const FInstancedStruct& InParameter)
{
	// 真停语义:停止活动并清除开始标记(重复Stop幂等)
	if (!bStarted)
	{
		return true;
	}

	bStarted = false;
	return ExecuteStopBehavior(InParameter);
}

bool UBXBehaviorProxy::NativeStartBehavior(const FInstancedStruct& InParameter)
{
	// 默认true:未重写该虚函数的派生类对应操作视为成功(与NativeInitialize语义对齐)
	return true;
}

bool UBXBehaviorProxy::NativeStopBehavior(const FInstancedStruct& InParameter)
{
	return true;
}

bool UBXBehaviorProxy::UpdateProxy(float InDeltaTime)
{
	// 双重门禁:声明需要帧更新且已启用(组件Tick已按同条件过滤,此处防御直调)
	if (!bWantsProxyUpdate || !bEnabled)
	{
		return true;
	}

	bool bResult1 = true;
	if (BX_HAS_PROXY_FLAG(BPF_NativeUpdateProxy))
	{
		bResult1 = NativeUpdateProxy(InDeltaTime);
	}

	bool bResult2 = true;
	if (BX_HAS_PROXY_FLAG(BPF_BPUpdateProxy))
	{
		bResult2 = ScriptUpdateProxy(InDeltaTime);
	}

	return (bResult1 && bResult2);
}

bool UBXBehaviorProxy::NativeUpdateProxy(float InDeltaTime)
{
	return true;
}

bool UBXBehaviorProxy::CheckStartBehavior(const FInstancedStruct& InParameter)
{
	bool bResult1 = true;
	if (BX_HAS_PROXY_FLAG(BPF_NativeCheckStartBehavior))
	{
		bResult1 = NativeCheckStartBehavior(InParameter);
	}

	bool bResult2 = true;
	if (BX_HAS_PROXY_FLAG(BPF_BPCheckStartBehavior))
	{
		bResult2 = ScriptCheckStartBehavior(InParameter);
	}

	return (bResult1 && bResult2);
}

bool UBXBehaviorProxy::NativeCheckStartBehavior(const FInstancedStruct& InParameter)
{
	return true;
}

bool UBXBehaviorProxy::ExecuteStartBehavior(const FInstancedStruct& InParameter)
{
	bool bResult1 = true;
	if (BX_HAS_PROXY_FLAG(BPF_NativeStartBehavior))
	{
		bResult1 = NativeStartBehavior(InParameter);
	}

	bool bResult2 = true;
	if (BX_HAS_PROXY_FLAG(BPF_BPStartBehavior))
	{
		bResult2 = ScriptStartBehavior(InParameter);
	}

	return (bResult1 && bResult2);
}

bool UBXBehaviorProxy::ExecuteStopBehavior(const FInstancedStruct& InParameter)
{
	bool bResult1 = true;
	if (BX_HAS_PROXY_FLAG(BPF_NativeStopBehavior))
	{
		bResult1 = NativeStopBehavior(InParameter);
	}

	bool bResult2 = true;
	if (BX_HAS_PROXY_FLAG(BPF_BPStopBehavior))
	{
		bResult2 = ScriptStopBehavior(InParameter);
	}

	return (bResult1 && bResult2);
}