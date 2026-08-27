#include "BXBehaviorAgent.h"

#include "BXFunctionLibrary.h"

// 位标志快捷判断(BehaviorFunctions为int32位组合,与EBXBehaviorAgentFunction一一对应)
#define BX_HAS_AGENT_FLAG(FlagName) (EnumHasAllFlags(static_cast<EBXBehaviorAgentFunction>(BehaviorFunctions), EBXBehaviorAgentFunction::FlagName))


int64 UBXBehaviorAgent::GetUniqueKey() const
{
	return UniqueKey;
}

AActor* UBXBehaviorAgent::GetOwner() const
{
	if (UActorComponent* Component = Cast<UActorComponent>(GetOuter()))
	{
		return Component->GetOwner();
	}

	return nullptr;
}

bool UBXBehaviorAgent::Initialize()
{
	UniqueKey = UBXFunctionLibrary::GetUniqueID();

	bool bResult1 = true;
	if (BX_HAS_AGENT_FLAG(BAF_NativeInitialize))
	{
		bResult1 = NativeInitialize();
	}

	bool bResult2 = true;
	if (BX_HAS_AGENT_FLAG(BAF_BPInitialize))
	{
		bResult2 = ScriptInitialize();
	}

	return (bResult1 && bResult2);
}

bool UBXBehaviorAgent::NativeInitialize()
{
	// 默认true与UBXStateMachine基类NativeXxx语义对齐(返回false会使未重写该虚函数的派生类对应操作恒失败)
	return true;
}

bool UBXBehaviorAgent::Deinitialize()
{
	bool bResult1 = true;
	if (BX_HAS_AGENT_FLAG(BAF_NativeCleanup))
	{
		bResult1 = NativeDeinitialize();
	}

	bool bResult2 = true;
	if (BX_HAS_AGENT_FLAG(BAF_BPCleanup))
	{
		bResult2 = ScriptDeinitialize();
	}

	return (bResult1 && bResult2);
}

bool UBXBehaviorAgent::NativeDeinitialize()
{
	return true;
}

bool UBXBehaviorAgent::StartBehavior(const FInstancedStruct& InParameter)
{
	bool bResult1 = true;
	if (BX_HAS_AGENT_FLAG(BAF_NativeStartBehavior))
	{
		bResult1 = NativeStartBehavior(InParameter);
	}

	bool bResult2 = true;
	if (BX_HAS_AGENT_FLAG(BAF_BPStartBehavior))
	{
		bResult2 = ScriptStartBehavior(InParameter);
	}

	return (bResult1 && bResult2);
}

bool UBXBehaviorAgent::NativeStartBehavior(const FInstancedStruct& InParameter)
{
	return true;
}

bool UBXBehaviorAgent::StopBehavior(const FInstancedStruct& InParameter)
{
	bool bResult1 = true;
	if (BX_HAS_AGENT_FLAG(BAF_NativeStopBehavior))
	{
		bResult1 = NativeStopBehavior(InParameter);
	}

	bool bResult2 = true;
	if (BX_HAS_AGENT_FLAG(BAF_BPStopBehavior))
	{
		bResult2 = ScriptStopBehavior(InParameter);
	}

	return (bResult1 && bResult2);
}

bool UBXBehaviorAgent::NativeStopBehavior(const FInstancedStruct& InParameter)
{
	return true;
}

bool UBXBehaviorAgent::CheckStartBehavior(const FInstancedStruct& InParameter)
{
	bool bResult1 = true;
	if (BX_HAS_AGENT_FLAG(BAF_NativeCheckStartBehavior))
	{
		bResult1 = NativeCheckStartBehavior(InParameter);
	}

	bool bResult2 = true;
	if (BX_HAS_AGENT_FLAG(BAF_BPCheckStartBehavior))
	{
		bResult2 = ScriptCheckStartBehavior(InParameter);
	}

	return (bResult1 && bResult2);
}

bool UBXBehaviorAgent::NativeCheckStartBehavior(const FInstancedStruct& InParameter)
{
	return true;
}
