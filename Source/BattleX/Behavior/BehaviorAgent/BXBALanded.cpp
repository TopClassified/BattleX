#include "BXBALanded.h"



// 瞬时行为(落地):事件由UBXBehaviorComponent管线统一广播,Hit参数经参数结构传递给监听方
bool UBXBADefaultLanded::NativeStartBehavior(const FInstancedStruct& InParameter)
{
	return true;
}

bool UBXBADefaultLanded::NativeCheckStartBehavior(const FInstancedStruct& InParameter)
{
	return true;
}
