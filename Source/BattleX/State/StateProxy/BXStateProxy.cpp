#include "BXStateProxy.h"

#include "BXFunctionLibrary.h"

// 位标志快捷判断(StateFunctions为int32位组合,与EBXStateProxyFunction一一对应)
#define BX_HAS_STATE_PROXY_FLAG(FlagName) (EnumHasAllFlags(static_cast<EBXStateProxyFunction>(StateFunctions), EBXStateProxyFunction::FlagName))



int64 UBXStateProxy::GetUniqueKey() const
{
	return UniqueKey;
}

AActor* UBXStateProxy::GetOwner() const
{
	if (UActorComponent* Component = Cast<UActorComponent>(GetOuter()))
	{
		return Component->GetOwner();
	}

	return nullptr;
}

bool UBXStateProxy::Initialize()
{
	UniqueKey = UBXFunctionLibrary::GetUniqueID();

	bool bResult1 = true;
	if (BX_HAS_STATE_PROXY_FLAG(SPF_NativeInitialize))
	{
		bResult1 = NativeInitialize();
	}

	bool bResult2 = true;
	if (BX_HAS_STATE_PROXY_FLAG(SPF_BPInitialize))
	{
		bResult2 = ScriptInitialize();
	}

	return (bResult1 && bResult2);
}

bool UBXStateProxy::NativeInitialize()
{
	// 默认true与UBXBehaviorProxy基类语义对齐(返回false会使未重写该虚函数的派生类对应操作恒失败)
	return true;
}

bool UBXStateProxy::Deinitialize()
{
	bool bResult1 = true;
	if (BX_HAS_STATE_PROXY_FLAG(SPF_NativeCleanup))
	{
		bResult1 = NativeDeinitialize();
	}

	bool bResult2 = true;
	if (BX_HAS_STATE_PROXY_FLAG(SPF_BPCleanup))
	{
		bResult2 = ScriptDeinitialize();
	}

	return (bResult1 && bResult2);
}

bool UBXStateProxy::NativeDeinitialize()
{
	return true;
}

bool UBXStateProxy::StartState()
{
	// 记录活动标记后执行(重复Start=重启语义)
	bStarted = true;

	return ExecuteStartState();
}

bool UBXStateProxy::StopState()
{
	// 真停语义:停止活动并清除开始标记(重复Stop幂等)
	if (!bStarted)
	{
		return true;
	}

	bStarted = false;
	return ExecuteStopState();
}

bool UBXStateProxy::NativeStartState()
{
	// 默认true:未重写该虚函数的派生类对应操作视为成功(与NativeInitialize语义对齐)
	return true;
}

bool UBXStateProxy::NativeStopState()
{
	return true;
}

bool UBXStateProxy::UpdateState(float InDeltaTime)
{
	// 双重门禁:声明需要帧更新且已开始(组件Tick已按同条件过滤,此处防御直调)
	if (!bWantsStateUpdate || !bStarted)
	{
		return true;
	}

	bool bResult1 = true;
	if (BX_HAS_STATE_PROXY_FLAG(SPF_NativeUpdateState))
	{
		bResult1 = NativeUpdateState(InDeltaTime);
	}

	bool bResult2 = true;
	if (BX_HAS_STATE_PROXY_FLAG(SPF_BPUpdateState))
	{
		bResult2 = ScriptUpdateState(InDeltaTime);
	}

	return (bResult1 && bResult2);
}

bool UBXStateProxy::NativeUpdateState(float InDeltaTime)
{
	return true;
}

bool UBXStateProxy::ExecuteStartState()
{
	bool bResult1 = true;
	if (BX_HAS_STATE_PROXY_FLAG(SPF_NativeStartState))
	{
		bResult1 = NativeStartState();
	}

	bool bResult2 = true;
	if (BX_HAS_STATE_PROXY_FLAG(SPF_BPStartState))
	{
		bResult2 = ScriptStartState();
	}

	return (bResult1 && bResult2);
}

bool UBXStateProxy::ExecuteStopState()
{
	bool bResult1 = true;
	if (BX_HAS_STATE_PROXY_FLAG(SPF_NativeStopState))
	{
		bResult1 = NativeStopState();
	}

	bool bResult2 = true;
	if (BX_HAS_STATE_PROXY_FLAG(SPF_BPStopState))
	{
		bResult2 = ScriptStopState();
	}

	return (bResult1 && bResult2);
}
