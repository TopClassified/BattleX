#include "BXBARotate.h"



// 开始/停止事件由UBXBehaviorComponent管线统一广播(BXEvent.Behavior.Enter/Exit)
bool UBXBADefaultRotate::NativeStartBehavior(const FInstancedStruct& InParameter)
{
	return true;
}

bool UBXBADefaultRotate::NativeStopBehavior(const FInstancedStruct& InParameter)
{
	return true;
}

bool UBXBADefaultRotate::NativeCheckStartBehavior(const FInstancedStruct& InParameter)
{
	return true;
}
