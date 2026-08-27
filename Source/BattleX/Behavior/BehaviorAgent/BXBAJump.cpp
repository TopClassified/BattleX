#include "BXBAJump.h"



// 开始/停止事件由UBXBehaviorComponent管线统一广播(BXEvent.Behavior.Enter/Exit)
bool UBXBADefaultJump::NativeStartBehavior(const FInstancedStruct& InParameter)
{
	return true;
}

bool UBXBADefaultJump::NativeCheckStartBehavior(const FInstancedStruct& InParameter)
{
	return true;
}
