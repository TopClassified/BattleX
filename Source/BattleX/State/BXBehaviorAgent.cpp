#include "BXBehaviorAgent.h"

#include "BXFunctionLibrary.h"



int64 UBXBehaviorAgent::GetUniqueKey() const
{
	return UniqueKey;
}

bool UBXBehaviorAgent::Initialize()
{
	UniqueKey = UBXFunctionLibrary::GetUniqueID();
	
	bool bResult1 = true;
	if ((BehaviorFunctions & 1) > 0)
	{
		bResult1 = NativeInitialize();
	}

	bool bResult2 = true;
	if ((BehaviorFunctions & 2) > 0)
	{
		bResult2 = ScriptInitialize();
	}

	return (bResult1 && bResult2);
}

bool UBXBehaviorAgent::NativeInitialize()
{
	return false;
}

bool UBXBehaviorAgent::Deinitialize()
{
	bool bResult1 = true;
	if ((BehaviorFunctions & 4) > 0)
	{
		bResult1 = NativeDeinitialize();
	}

	bool bResult2 = true;
	if ((BehaviorFunctions & 8) > 0)
	{
		bResult2 = ScriptDeinitialize();
	}

	return (bResult1 && bResult2);
}

bool UBXBehaviorAgent::NativeDeinitialize()
{
	return false;
}

bool UBXBehaviorAgent::StartBehavior(const FInstancedStruct& InParameter)
{
	bool bResult1 = true;
	if ((BehaviorFunctions & 16) > 0)
	{
		bResult1 = NativeStartBehavior(InParameter);
	}

	bool bResult2 = true;
	if ((BehaviorFunctions & 32) > 0)
	{
		bResult2 = ScriptStartBehavior(InParameter);
	}

	return (bResult1 && bResult2);
}

bool UBXBehaviorAgent::NativeStartBehavior(const FInstancedStruct& InParameter)
{
	return false;
}

bool UBXBehaviorAgent::StopBehavior(const FInstancedStruct& InParameter)
{
	bool bResult1 = true;
	if ((BehaviorFunctions & 64) > 0)
	{
		bResult1 = NativeStopBehavior(InParameter);
	}

	bool bResult2 = true;
	if ((BehaviorFunctions & 128) > 0)
	{
		bResult2 = ScriptStopBehavior(InParameter);
	}

	return (bResult1 && bResult2);
}

bool UBXBehaviorAgent::NativeStopBehavior(const FInstancedStruct& InParameter)
{
	return false;
}

bool UBXBehaviorAgent::CheckStartBehavior(const FInstancedStruct& InParameter)
{
	bool bResult1 = true;
	if ((BehaviorFunctions & 256) > 0)
	{
		bResult1 = NativeCheckStartBehavior(InParameter);
	}

	bool bResult2 = true;
	if ((BehaviorFunctions & 512) > 0)
	{
		bResult2 = ScriptCheckStartBehavior(InParameter);
	}

	return (bResult1 && bResult2);
}

bool UBXBehaviorAgent::NativeCheckStartBehavior(const FInstancedStruct& InParameter)
{
	return false;
}
