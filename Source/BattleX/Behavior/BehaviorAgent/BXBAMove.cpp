#include "BXBAMove.h"



// 开始/停止事件由UBXBehaviorComponent管线统一广播(BXEvent.Behavior.Enter/Exit)
// Agent只负责基层组件的执行逻辑,不再手动发事件
bool UBXBADefaultMove::NativeStartBehavior(const FInstancedStruct& InParameter)
{
	return true;
}

bool UBXBADefaultMove::NativeStopBehavior(const FInstancedStruct& InParameter)
{
	return true;
}

bool UBXBADefaultMove::NativeCheckStartBehavior(const FInstancedStruct& InParameter)
{
	return true;
}
